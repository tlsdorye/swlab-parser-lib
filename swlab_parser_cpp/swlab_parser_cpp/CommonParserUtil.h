#pragma once
#include "Nonterminal.h"
#include "Terminal.h"
#include "ParseState.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <functional>
#include <string>
#include <map> // test용
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <array>
#include <regex>
#include <utility>
#include <algorithm>
#include <cctype>
#include <locale>
#include <list>
#include <type_traits>
using namespace std;

template <typename TOKEN, typename AST, 
	template<typename ELEM, typename = allocator<ELEM>> class CONTAINER>
class CommonParserUtil
{
	//static_assert(is_base_of<TokenInterface, TOKEN>::value, "TOKEN muse inherit from TokenInterface");
private:
	map<string, function<TOKEN(string)>> regex_to_token_by_lambda;
	map<string, function<CONTAINER<AST*>()>> grammar_to_trees_by_lambda;
	
	vector<map<string, vector<string>>> action_table;
	map<pair<string, string>, string> goto_table;
	vector<vector<string>> grammar_table;

	list<Terminal<TOKEN>> terminal_list; // -> queue로 바꿔도 좋을듯

	deque<StackElement*> parse_stack;
	string start_symbol;
	int curr_grammar_index;

	TOKEN end_of_token;
public:
	CommonParserUtil()
	{

	}

	// function < return-type (parameter)> functor
	void Lex(string regExp, function<TOKEN(string)> func)
	{
		regex_to_token_by_lambda.insert({ regExp, func });
	}

	void LexEndOfToken(TOKEN tok)
	{
		end_of_token = tok;
	}
	
	void Lexing(const vector<string> &filepaths)
	{
		for (auto it : filepaths)
		{
			ifstream reader = ifstream(it);
			vector<string> lines;
			if (reader.is_open())
			{
				string line;

				while (getline(reader, line)) lines.push_back(line);
				for (int i = 0; i < lines.size(); i++) Matching(lines[i], i + 1);
			}
			else
			{
				// 파일이 안열림 -> 파일 경로 에러?
				cout << "error - ifstream\n";
				return;
			}
			terminal_list.push_back(Terminal<TOKEN>(end_of_token.getSToken(), end_of_token, -1, -1));
		}

		//test
		//testTokenBuilders();
		//testTerminals();
	}

	//matching function
	void Matching(string &_line, int lineno)
	{
		int charIdx = 0;
		string line = _line;
		smatch sm;
		while (line != "")
		{
			bool matched = false;
			for (auto it : regex_to_token_by_lambda)
			{
				regex curr_regex = regex(it.first);
				
				// 매칭 되는것이 있고, prefix가 없을 경우 -> 첫문자부터 매칭되는경우
				if (regex_search(line, sm, curr_regex) && sm.prefix() == "")
				{
					// sm[0]: matched string_token
					if (it.second(sm[0]).getSToken() != "_")
						terminal_list.push_back(Terminal<TOKEN>(sm[0], (it.second)(sm[0]), charIdx, lineno));
					
					line = sm.suffix();
					charIdx += sm[0].length();
					matched = true;
					break;
				}
			}
			if (!matched)
			{
				cout << "no matching!!\n";
				return;
			}
		}
	}

	vector<string> SplitLine(string line, string regExp)
	{
		vector<string> tokens;
		regex reg(regExp);
		smatch sm;
		while (regex_search(line, sm, reg))
		{
			for (int i = 1; i < sm.size(); i++)
				tokens.push_back(sm[i].str());
			line = sm.suffix().str();
		}
		return tokens;
	}

	CONTAINER<AST*> getTree(int idx)
	{
		int length = grammar_table[curr_grammar_index].size() - 2;
		int last_stack_idx = parse_stack.size() - 1;
		int offset = (length * 2) - ((idx - 1) * 2 + 1);
		if (Nonterminal<AST, CONTAINER>* nt = dynamic_cast<Nonterminal<AST, CONTAINER>*>(parse_stack[last_stack_idx - offset])) {
			return nt->getTree();
		}
		else {
			printerror("downcasting error - Nonterminal");
		}
	}

	string getText(int idx)
	{
		int length = grammar_table[curr_grammar_index].size() - 2;
		int last_stack_idx = parse_stack.size() - 1;
		int offset = (length * 2) - ((idx - 1) * 2 + 1);
		if (Terminal<TOKEN>* term = dynamic_cast<Terminal<TOKEN>*>(parse_stack[last_stack_idx - offset])) {
			return term->getSyntax();
		}
		else {
			printerror("downcasting error - Terminal");
		}
	}

	void setStartSymbol(string startSymbol)
	{
		this->start_symbol = startSymbol;
	}

	void rule(string productionRule, function<CONTAINER<AST*>()> func)
	{
		grammar_to_trees_by_lambda.insert({ productionRule, func });
	}

	CONTAINER<AST*> parsing(const vector<string> filepaths)
	{
		readInitialize();
		Lexing(filepaths);

		parse_stack = deque<StackElement*>();
		parse_stack.push_back(new ParseState("0")); // 반드시 delete

		while (!terminal_list.empty())
		{
			//
			//cout << "stack: ";
			//for (auto it : parse_stack) cout << it->toString() << ", ";
			//cout << endl;
			//
			string str_currState = dynamic_cast<ParseState*>(parse_stack.back())->getState();
			Terminal<TOKEN> currTerminal = terminal_list.front();
			vector<string> action = getAction(str_currState, currTerminal);
			//
			//cout << "state: " << str_currState << endl;
			//cout << "term : " << currTerminal.getSyntax() << endl;
			//for (auto it : action) cout << it << " ";
			//cout << endl;
			//
			if (action[0] == "Accept")
			{
				terminal_list.pop_front();
				Nonterminal<AST, CONTAINER>* nt = dynamic_cast<Nonterminal<AST, CONTAINER>*>(parse_stack[1]);
				return nt->getTree();
			}
			else if (action[0] == "Shift")
			{
				parse_stack.push_back(new Terminal<TOKEN>(currTerminal));
				parse_stack.push_back(new ParseState(action[1]));
				terminal_list.pop_front();
			}
			else if (action[0] == "Reduce")
			{
				curr_grammar_index = stoi(action[1]);
				string lhs = grammar_table[curr_grammar_index][0];
				vector<string>::iterator rhs_start = grammar_table[curr_grammar_index].begin() + 2;
				vector<string>::iterator rhs_end = grammar_table[curr_grammar_index].end();
				vector<string> rhs(rhs_start, rhs_end);

				string grammar_rule;
				for (auto it : grammar_table[curr_grammar_index]) grammar_rule += it + " ";
				grammar_rule = grammar_rule.substr(0, grammar_rule.size() - 1);
				CONTAINER<AST*> tree = grammar_to_trees_by_lambda[grammar_rule]();

				for (int i = 0; i < rhs.size(); i++)
				{
					delete parse_stack.back();
					parse_stack.pop_back();
					delete parse_stack.back();
					parse_stack.pop_back();
				}
				//
				//cout << "stack: ";
				//for (auto it : parse_stack) cout << it->toString() << ", ";
				//cout << endl;
				//		
				str_currState = dynamic_cast<ParseState*>(parse_stack.back())->getState();

				parse_stack.push_back(new Nonterminal<AST, CONTAINER>(tree));
				parse_stack.push_back(new ParseState(goto_table[{str_currState, lhs}]));
			}
		}

	}

	void readInitialize()
	{
		ifstream grammarReader("grammar_rules.txt");
		ifstream actionReader("action_table.txt");
		ifstream gotoReader("goto_table.txt");

		string line;
		grammar_table.clear(); // capacity는 그대로
		action_table.clear();
		goto_table.clear();

		if (grammarReader.is_open())
		{
			while (getline(grammarReader, line))
			{
				vector<string> index_split = SplitLine(line, "([0-9]+)(?:. )([^\n]+)");
				vector<string> grammar_split = SplitLine(index_split[1], "([^\t ]+)");
				int index = stoi(index_split[0]);
				while (grammar_table.size() <= index) grammar_table.push_back(vector<string>());
				grammar_table[index] = grammar_split;
			}
		}
		else
		{
			"error: grammarReader is not open\n";
			return;
		}

		if (actionReader.is_open())
		{
			while (getline(actionReader, line)) //action_tables.push_back(line);
			{
				vector<string> tokens = SplitLine(line, "([^\t ]+)");
				int state = stoi(tokens[0]);
				if (action_table.size() <= state) action_table.push_back(map<string, vector<string>>());
				for (int i = 2; i < tokens.size(); i++) action_table[state][tokens[1]].push_back(tokens[i]);
			}
		}
			
		else
		{
			"error: actionReader is not open\n";
			return;
		}

		if (gotoReader.is_open())
			while (getline(gotoReader, line))
			{
				vector<string> tokens = SplitLine(line, "([^\t ]+)");
				goto_table.insert({ {tokens[0], tokens[1]}, tokens[2] });
			}
		else
		{
			"error: gotoReader is not open\n";
			return;
		}

		//test
		//test_grammar_table();
		//test_action_table();
		//test_goto_table();
	}

	vector<string> getAction(string str_state, Terminal<TOKEN>& term)
	{
		int int_state = stoi(str_state);
		//int int_state = stoi(currState);
		vector<string> action;
		if (int_state < 0 && int_state >= action_table.size()) return action; // error: out of range at action_table
		map<string, vector<string>>::iterator it = action_table[int_state].find(term.getToken().getSToken());
		if (it == action_table[int_state].end()) return action; // error: Nothing find at action_table[int_state]
		return it->second;

		return action;
	}

	string get_str_state(ParseState str_currState, string index)
	{

	}

	void test_grammar_table() 
	{
		for (int i = 0; i < grammar_table.size(); i++)
		{
			cout << setw(3) << i << ": ";
			for (auto it : grammar_table[i])
				cout << it << ", ";
			cout << endl;
		}
	}

	void test_action_table()
	{
		for (int i = 0; i < action_table.size(); i++)
		{
			for (auto it : action_table[i])
			{
				cout << setw(3) << i << ": ";
				cout << setw(15) << it.first << " => [";
				for (auto it : it.second) cout << setw(7) << it << " ";
				cout << "]\n";
			}
		}
	}

	void test_goto_table()
	{
		for_each(goto_table.begin(), goto_table.end(), [](auto it) {
			cout << "key: {" << setw(2) << it.first.first << ", " << setw(20) << it.first.second;
			cout << "}, val: " << it.second << endl;
		});
	}

	void testTerminals()
	{
		cout << "Terminals size: " << terminal_list.size() << endl;
		for (auto it : terminal_list)
		{
			cout << "terminal: ";
			cout << setw(5) << it.getSyntax() << " ";
			cout << setw(15) << it.getToken().getSToken() << " ";
			cout << setw(3) << it.getCharIdx() << " ";
			cout << setw(3) << it.getLineIdx() << "\n";
		}
	}

	//testprint to tokenBuilder
	void testTokenBuilders()
	{
		for_each(regex_to_token_by_lambda.begin(), regex_to_token_by_lambda.end(), [](auto it) {
			cout << "< regExp: " << it.first;
			cout << ", Token: " << (it.second)("+").getSToken();
			cout << " >\n";
		});
	}

	void testTreeBuilders()
	{
		cout << "treeBuilder size: " << grammar_to_trees_by_lambda.size() << endl;
		for_each(grammar_to_trees_by_lambda.begin(), grammar_to_trees_by_lambda.end(), [](auto it) {
			cout << "B\n";
			CONTAINER<AST*> astree = (it.second)();
			cout << "productionRule: " << it.first;
			cout << ", AST: ";
			for (auto it : astree) cout << "[" << it->toString() << "] ";
			cout << "\n";	
		});
	}

	void printerror(string s)
	{
		cout << s << endl;
		return;
	}

	/*

	vector<string> split(string line, const string &delimiter)
	{
		vector<string> tokens;
		string token;
		size_t pos;
		while ((pos = line.find(delimiter)) != string::npos)
		{
			token = line.substr(0, pos);
			trim(token);
			tokens.push_back(token);
			line.erase(0, pos + delimiter.length());
		}
		trim(line);
		tokens.push_back(line);
		return tokens;
	}

	string &trim(std::string &s) 
	{
		s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
		s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
		return s;
	}

	*/
};

