#pragma once
#include "Terminal.h"
#include "State.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <functional>
#include <string>
#include <map> // test용
#include <vector>
#include <stack>
#include <array>
#include <regex>
#include <utility>
using namespace std;

template <typename TOKEN, typename AST>
class CommonParserUtil
{
private:
	map<string, function<TOKEN(string)>> tokenBuilders;
	map<string, function<vector<AST>()>> treeBuilders;
	vector<Terminal<TOKEN>> terminals;
	vector<string> grammar_rules;
	int grammar_rule_index;
	stack<StackElement*> parse_stack;
	string startSymbol;

public:
	CommonParserUtil()
	{

	}

	// function < return-type (parameter)> functor
	void lex(string regExp, function<TOKEN(string)> func) 
	{
		tokenBuilders[regExp] = func;
	}

	void lexing(vector<string> &filepaths)
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
		testTerminals();
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
					terminals.push_back(Terminal<TOKEN>(sm[0], (it.second)(sm[0]), charIdx, lineno));
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

	vector<AST> get(int idx)
	{

	}

	void ruleStartSymbol(string startSymbol)
	{
		this->startSymbol = startSymbol;
	}

	void rule(string productionRule, function<vector<AST>()> func)
	{
		treeBuilders[produtionRule] = func;
	}

	void parsing(vector<string> filepaths)
	{
		readInitialize();
		lexing(filepaths);

		parse_stack = stack<StackElement*>();
		parse_stack.push(&State(0));
		int x = dynamic_cast<State*>(parse_stack.top())->getState();
		cout << x << endl;
	}

	void readInitialize()
	{

	}

	void testTerminals()
	{
		cout << "Terminals size: " << terminals.size() << endl;
		for (auto it : terminals)
		{
			cout << "terminal: ";
			cout << setw(5) << it.getSyntax() << " ";
			cout << setw(15) << getStrToken(it.getToken()) << " ";
			cout << setw(3) << it.getCharIdx() << " ";
			cout << setw(3) << it.getLineIdx() << "\n";
		}
	}

	//testprint to tokenBuilder
	void testTokenBuilder()
	{
		for_each(tokenBuilders.begin(), tokenBuilders.end(), [](auto it) {
			cout << "< regExp: " << it.first;
			cout << ", Token: " << getStrToken((it.second)("+"));
			cout << " >\n";
		});
	}
};