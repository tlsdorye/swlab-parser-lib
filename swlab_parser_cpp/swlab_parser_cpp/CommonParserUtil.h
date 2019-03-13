#pragma once
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
	template<typename ELEM, typename = allocator<ELEM>> class CONT>
class CommonParserUtil
{
	//static_assert(is_base_of<TokenInterface, TOKEN>::value, "TOKEN muse inherit from TokenInterface");
private:
	map<string, function<TOKEN(string)>> tokenBuilders;
	map<string, function<CONT<AST>()>> treeBuilders;
	list<Terminal<TOKEN>> terminals;
	
	vector<map<string, vector<string>>> action_table;
	vector<map<string, vector<string>>> goto_table;
	vector<string> grammar_table;

	int grammar_rule_index;

	deque<StackElement*> parse_stack;
	string startSymbol;

public:
	CommonParserUtil()
	{

	}

	// function < return-type (parameter)> functor
	void lex(string regExp, function<TOKEN(string)> func)
	{
		tokenBuilders.insert({ regExp, func });
	}

	void lexing(const vector<string> &filepaths)
	{
		for (auto it : filepaths)
		{
			ifstream reader = ifstream(it);
			vector<string> lines;
			if (reader.is_open())
			{
				string line;

				while (getline(reader, line)) lines.push_back(line);
				for (int i = 0; i < lines.size(); i++) matching(lines[i], i + 1);
			}
			else
			{
				// 파일이 안열림 -> 파일 경로 에러?
				cout << "error - ifstream\n";
				return;
			}
		}

		//test
		//testTokenBuilders();
		//testTerminals();
	}

	//matching function
	void matching(string &_line, int lineno)
	{
		int charIdx = 0;
		string line = _line;
		smatch sm;
		while (line != "")
		{
			bool matched = false;
			for (auto it : tokenBuilders)
			{
				regex curr_regex = regex(it.first);
				
				// 매칭 되는것이 있고, prefix가 없을 경우 -> 첫문자부터 매칭되는경우
				if (regex_search(line, sm, curr_regex) && sm.prefix() == "")
				{
					// sm[0]: matched string_token
					try {
						terminals.push_back(Terminal<TOKEN>(sm[0], (it.second)(sm[0]), charIdx, lineno));
					} 
					catch (TokenException& e) { e.printerror(); };
					
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

	vector<string> split_line(string line, string regExp)
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

	CONT<AST> get(int idx)
	{
		string productionRule = grammar_table[grammar_rule_index];
		vector<string> splitRule = split_line(productionRule, "([^\t ]+)");
		int length = splitRule.size() - 2;
		int last_stack_tree_index = parse_stack.size() - 1;
		int offset = (length * 2) - ((idx - 1) * 2 + 1);
		CONT<AST> tree;// = parse_stack[last_stack_tree_index - offset];
		return tree;
	}

	string getText(int idx)
	{
		string ret;
		return ret;
	}

	void ruleStartSymbol(string startSymbol)
	{
		this->startSymbol = startSymbol;
	}

	void rule(string productionRule, function<CONT<AST>()> func)
	{
		treeBuilders.insert({ productionRule, func });
	}

	void parsing(const vector<string> filepaths)
	{
		readInitialize();
		lexing(filepaths);

		parse_stack = deque<StackElement*>();
		parse_stack.push_back(new ParseState("0"));

		int y = 1;

		//while (!terminals.empty())
		while(y--)
		{
			string str_state = dynamic_cast<ParseState*>(parse_stack.back())->getState();
			Terminal<TOKEN> currTerminal = terminals.front();
			vector<string> action = getAction(stoi(str_state), currTerminal);
			for (auto it : action) cout << it << " ";
			cout << endl;
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
				vector<string> words = split_line(line, "([0-9]+)(?:. )([^\n]+)");
				if (words.size() < 2 || grammar_table.size() != stoi(words[0]))
				{
					printerror("error: grammar_rule split");
					return;
				}
				grammar_table.push_back(words[1]);
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
				vector<string> tokens = split_line(line, "([^\t ]+)");
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
			while (getline(gotoReader, line)); //goto_table.push_back(line);
		else
		{
			"error: gotoReader is not open\n";
			return;
		}

		//test
		//testReader();
	}

	vector<string> getAction(int& int_state, Terminal<TOKEN>& term)
	{
		//int int_state = stoi(currState);
		vector<string> action;
		if (int_state < 0 && int_state >= action_table.size()) return action; // error: out of range at action_table
		map<string, vector<string>>::iterator it = action_table[int_state].find(term.getToken().getSToken());
		if (it == action_table[int_state].end()) return action; // error: Nothing find at action_table[int_state]
		return it->second;

		return action;
	}



	void testReader()
	{
		for (int i = 0; i < grammar_table.size(); i++) cout << setw(3) << i << ": ~" << grammar_table[i] << "~\n";
		test_action_tables();
		for (int i = 0; i < goto_table.size(); i++) cout << setw(3) << i << ":: ^" << goto_table[i] << "^\n";
	}

	void test_action_tables()
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

	void testTerminals()
	{
		cout << "Terminals size: " << terminals.size() << endl;
		for (auto it : terminals)
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
		for_each(tokenBuilders.begin(), tokenBuilders.end(), [](auto it) {
			cout << "< regExp: " << it.first;
			cout << ", Token: " << (it.second)("+").getSToken();
			cout << " >\n";
		});
	}

	void testTreeBuilders()
	{
		cout << "treeBuilder size: " << treeBuilders.size() << endl;
		for_each(treeBuilders.begin(), treeBuilders.end(), [](auto it) {
			cout << "B\n";
			CONT<AST> astree = (it.second)();
			cout << "productionRule: " << it.first;
			cout << ", AST: ";
			for (auto it : astree) cout << "[" << it.toString() << "] ";
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

