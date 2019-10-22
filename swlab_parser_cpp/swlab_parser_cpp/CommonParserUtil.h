#pragma once
#include "Nonterminal.h"
#include "Terminal.h"
#include "ParseState.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <set>
#include <regex>
#include <utility>
#include <algorithm>
#include <functional>
#include <Windows.h>
using namespace std;

template <typename TOKEN, typename AST, 
	template<typename ELEM, typename = allocator<ELEM>> class CONTAINER>
class CommonParserUtil
{
	//static_assert(is_base_of<TokenInterface, TOKEN>::value, "TOKEN muse inherit from TokenInterface");
private:
	map<string, function<TOKEN(string)>> regex_to_token;
	map<int, function<CONTAINER<AST*>()>> index_to_trees;
	
	vector<map<string, vector<string>>> action_table;
	map<pair<string, string>, string> goto_table;
	vector<vector<string>> grammar_table;
	int curr_grammar_index;

	vector<string> str_grammar;

	queue<Terminal<TOKEN>> terminal_queue;

	deque<StackElement*> parse_stack;
	string start_symbol;	
	TOKEN end_of_token;

public:
	CommonParserUtil()
	{

	}

	// function < return-type (parameter)> functor
	void AddTokenLambda(string regex_exp, function<TOKEN(string)> lambda_token)
	{
		regex_to_token.insert({ regex_exp, lambda_token });
	}

	void LexEndOfToken(TOKEN token)
	{
		end_of_token = token;
	}
	
	void Lexing(const vector<string> &filepaths)
	{
		for (auto it : filepaths)
		{
			ifstream reader = ifstream(it);
			if (reader.is_open())
			{
				string line;
				int lineno = 1;
				while (getline(reader, line)) Matching(line, lineno++);
			}
			else
			{
				PrintError("fileopen_error - ifstream");
				return;
			}
			terminal_queue.push(Terminal<TOKEN>(end_of_token.get_str_token(), end_of_token, -1, -1));
		}
	}

	void Matching(string &_line, int lineno)
	{
		int charIdx = 0;
		string line = _line;
		smatch match_result;
		while (line != "")
		{
			bool matched = false;
			for (auto it : regex_to_token)
			{
				regex curr_regex = regex(it.first);
				
				// 매칭 되는것이 있고, prefix가 없을 경우 -> 첫문자부터 매칭되는경우
				if (regex_search(line, match_result, curr_regex) && match_result.prefix() == "")
				{
					// sm[0]: matched string_token
					if (it.second(match_result[0]).get_str_token() != "_")
						terminal_queue.push(Terminal<TOKEN>(match_result[0], (it.second)(match_result[0]), charIdx, lineno));
					
					line = match_result.suffix();
					charIdx += match_result[0].length();
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

	vector<string> SplitLine(string line, string regex_exp)
	{
		vector<string> tokens;
		regex reg(regex_exp);
		smatch match_result;
		while (regex_search(line, match_result, reg))
		{
			for (int i = 1; i < match_result.size(); i++)
				tokens.push_back(match_result[i].str());
			line = match_result.suffix().str();
		}
		return tokens;
	}

	CONTAINER<AST*> GetStackInTrees(int idx)
	{
		int length = grammar_table[curr_grammar_index].size() - 2;
		int last_idx = parse_stack.size() - 1;
		int offset = (length * 2) - ((idx - 1) * 2 + 1);
		if (Nonterminal<AST, CONTAINER>* nt = dynamic_cast<Nonterminal<AST, CONTAINER>*>(parse_stack[last_idx - offset])) {
			return nt->getTrees();
		}
		else {
			PrintError("downcasting error - Nonterminal");
		}
		/*
		Nonterminal<AST, CONTAINER>* nt = (Nonterminal<AST, CONTAINER>*)parse_stack[last_idx - offset];
		return nt->getTrees();	
		*/
	}

	string GetStackInSyntax(int idx)
	{
		int length = grammar_table[curr_grammar_index].size() - 2;
		int last_idx = parse_stack.size() - 1;
		int offset = (length * 2) - ((idx - 1) * 2 + 1);
		if (Terminal<TOKEN>* term = dynamic_cast<Terminal<TOKEN>*>(parse_stack[last_idx - offset])) {
			return term->getSyntax();
		}
		else {
			PrintError("downcasting error - Terminal");
		}
	}

	void SetStartSymbol(string startSymbol)
	{
		this->start_symbol = startSymbol;
	}

	void AddTreeLambda(string grammar_rule, function<CONTAINER<AST*>()> lambda_grammar_rule)
	{
		str_grammar.push_back(grammar_rule);
		index_to_trees.insert({ str_grammar.size() - 1, lambda_grammar_rule });
	}

	CONTAINER<AST*> Parsing(const vector<string> filepaths)
	{
		LoadAutomaton();
		Lexing(filepaths);

		parse_stack = deque<StackElement*>();
		parse_stack.push_back(new ParseState("0")); // 반드시 delete

		while (!terminal_queue.empty())
		{
			string curr_state = dynamic_cast<ParseState*>(parse_stack.back())->get_state();
			Terminal<TOKEN> curr_terminal = terminal_queue.front();

			vector<string> next_action = FindAction(curr_state, curr_terminal);
		
			if (next_action[0] == "Accept")
			{
				terminal_queue.pop();
				if (Nonterminal<AST, CONTAINER>* nt = 
					dynamic_cast<Nonterminal<AST, CONTAINER>*>(parse_stack[1])) {
					return nt->getTrees();
				}
				else {
					PrintError("downcasting error - Nonterminal");
					break;
				}
				
			}
			else if (next_action[0] == "Shift")
			{
				parse_stack.push_back(new Terminal<TOKEN>(curr_terminal));
				parse_stack.push_back(new ParseState(next_action[1]));

				terminal_queue.pop();
			}
			else if (next_action[0] == "Reduce")
			{
				curr_grammar_index = stoi(next_action[1]);

				string lhs = grammar_table[curr_grammar_index][0];

				vector<string>::iterator rhs_start = grammar_table[curr_grammar_index].begin() + 2;
				vector<string>::iterator rhs_end = grammar_table[curr_grammar_index].end();
				vector<string> rhs(rhs_start, rhs_end);

				CONTAINER<AST*> trees = index_to_trees[curr_grammar_index]();

				for (int i = 0; i < rhs.size() * 2; i++)
				{
					delete parse_stack.back();
					parse_stack.pop_back();
				}

				curr_state = dynamic_cast<ParseState*>(parse_stack.back())->get_state();

				parse_stack.push_back(new Nonterminal<AST, CONTAINER>(trees));
				auto it = goto_table.find({ curr_state, lhs });
				if(it != goto_table.end()) parse_stack.push_back(new ParseState(it->second));
				else
				{
					PrintError("");
				}
			}
		}

	}

	void LoadAutomaton()
	{
		ifstream grammar_reader("grammar_rules.txt");
		ifstream action_reader("action_table.txt");
		ifstream goto_reader("goto_table.txt");

		if (!grammar_reader.good() || !action_reader.good() || !goto_reader.good()) 
		{
			CreateAutomaton();
			grammar_reader.close();
			action_reader.close();
			goto_reader.close();
			grammar_reader.open("grammar_rules.txt");
			action_reader.open("action_table.txt");
			goto_reader.open("goto_table.txt");
		}

		string line;
		grammar_table.clear(); // capacity는 그대로
		action_table.clear();
		goto_table.clear();

		while (getline(grammar_reader, line))
		{
			vector<string> index_split = SplitLine(line, "([0-9]+)(?:. )([^\n]+)");
			vector<string> grammar_split = SplitLine(index_split[1], "([^\t ]+)");
			int index = stoi(index_split[0]);
			while (grammar_table.size() <= index) grammar_table.push_back(vector<string>());
			grammar_table[index] = grammar_split;
		}

		while (getline(action_reader, line))
		{
			vector<string> tokens = SplitLine(line, "([^\t ]+)");
			int state = stoi(tokens[0]);
			if (action_table.size() <= state) action_table.push_back(map<string, vector<string>>());
			for (int i = 2; i < tokens.size(); i++) action_table[state][tokens[1]].push_back(tokens[i]);
		}

		while (getline(goto_reader, line))
		{
			vector<string> tokens = SplitLine(line, "([^\t ]+)");
			goto_table.insert({ { tokens[0], tokens[1] }, tokens[2] });
		}
	}

	vector<string> FindAction(string curr_state, Terminal<TOKEN>& term)
	{
		int state = stoi(curr_state);

		if (state < 0 && state >= action_table.size()) {
			PrintError("out_of_range error - action_table");
		}

		map<string, vector<string>>::iterator it = action_table[state].find(term.getToken().get_str_token());

		if (it == action_table[state].end()){
			PrintError("not found error - action_table[state]");
		}

		return it->second;
	}

	void CreateAutomaton()
	{
		set<string> nonterminals;
		vector<vector<string>> grammar_rule_split;
		for (auto grammar : str_grammar) grammar_rule_split.push_back(SplitLine(grammar, "([^\t ]+)"));

		for (auto it : grammar_rule_split) nonterminals.insert(it[0]);
		
		if (start_symbol == "")
		{
			PrintError("null error - start_symbol ");
			return;
		}

		string mygrammar_content = "CFG \"" + start_symbol + "\" [\n";

		for (auto token : grammar_rule_split)
		{
			mygrammar_content += "\tProductionRule \"" + token[0] + "\" [";
			for (int j = 2; j < token.size(); j++)
			{
				set<string>::iterator iter = nonterminals.find(token[j]);
				if (iter != nonterminals.end()) mygrammar_content += "Nonterminal \"";
				else mygrammar_content += "Terminal \"";
				mygrammar_content += token[j] + "\"";
				if (j < token.size() - 1) mygrammar_content += ", ";
			}
			mygrammar_content += "]";
			if (token != grammar_rule_split[grammar_rule_split.size() - 1])
				mygrammar_content += ",\n";
			else mygrammar_content += "\n";
		}
		mygrammar_content += "]";

		ofstream mygrammar_writer("mygrammar.grm");
		mygrammar_writer << mygrammar_content;
		mygrammar_writer.close();

		string instr = "/c genlrparser-exe.exe mygrammar.grm -output grammar_rules.txt action_table.txt goto_table.txt > genlrparser_result.txt";
		
		SHELLEXECUTEINFO shell_execute_info = { 0 };
		shell_execute_info.cbSize = sizeof(SHELLEXECUTEINFO);
		shell_execute_info.fMask = SEE_MASK_NOCLOSEPROCESS;
		shell_execute_info.hwnd = NULL;
		shell_execute_info.lpVerb = NULL;
		shell_execute_info.lpFile = "cmd";
		shell_execute_info.lpParameters = instr.c_str();
		shell_execute_info.lpDirectory = NULL;
		shell_execute_info.nShow = SW_SHOWNORMAL;
		shell_execute_info.hInstApp = NULL;

		cout << "genlrparser is starting...\n";
		ShellExecuteEx(&shell_execute_info);

		cout << "waiting for genlrparser...\n";
		WaitForSingleObject(shell_execute_info.hProcess, INFINITE);
		CloseHandle(shell_execute_info.hProcess);

		ifstream result_reader("genlrparser_result.txt");
		if (result_reader.is_open())
		{
			string result;
			result_reader >> result;
			cout << "genlrparser: " + result + "\n";
			if (result == "Done") LoadAutomaton();
		}
		else 
		{
			PrintError("execute error - genlrparser");
			return;
		}
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

	void test_terminal_queue()
	{
		cout << "Terminals size: " << terminal_queue.size() << endl;
		for (auto it : terminal_queue)
		{
			cout << "terminal: ";
			cout << setw(5) << it.getSyntax() << " ";
			cout << setw(15) << it.getToken().getSToken() << " ";
			cout << setw(3) << it.getCharIdx() << " ";
			cout << setw(3) << it.getLineIdx() << "\n";
		}
	}
	
	void test_regex_to_token()
	{
		for_each(regex_to_token.begin(), regex_to_token.end(), [](auto it) {
			cout << "< regExp: " << it.first;
			cout << ", Token: " << (it.second)("+").getSToken();
			cout << " >\n";
		});
	}

	void test_grammar_to_trees()
	{
		cout << "treeBuilder size: " << index_to_trees.size() << endl;
		for_each(index_to_trees.begin(), index_to_trees.end(), [](auto it) {
			cout << "B\n";
			CONTAINER<AST*> astree = (it.second)();
			cout << "productionRule: " << it.first;
			cout << ", AST: ";
			for (auto it : astree) cout << "[" << it->toString() << "] ";
			cout << "\n";	
		});
	}

	void PrintError(string error_message)
	{
		cout << error_message << endl;
		return;
	}
};

