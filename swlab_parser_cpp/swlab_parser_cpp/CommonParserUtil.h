#pragma once
#include "Terminal.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <functional>
#include <string>
#include <map> // test용
#include <vector>
#include <array>
#include <regex>
#include <utility>
using namespace std;

template <typename TOKEN>
class CommonParserUtil
{
private:
	map<string, function<TOKEN(string)>> tokenBuilder;
	vector<TOKEN> tokenV;
	vector<Terminal<TOKEN>> terminals;
	vector<string> RegExprToken;

public:
	CommonParserUtil()
	{

	}

	// function < return-type (parameter)> functor
	void lex(string regExp, function<TOKEN(string)> func) 
	{
		tokenBuilder[regExp] = func;
	}

	void lexing(ifstream reader)
	{
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

	//matching function
	void matching(string &_line, int lineno)
	{
		int charIdx = 0;
		string line = _line;
		smatch sm;
		while (line != "")
		{
			bool matched = false;
			for (auto it : tokenBuilder)
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
		for_each(tokenBuilder.begin(), tokenBuilder.end(), [](auto it) {
			cout << "< regExp: " << it.first;
			cout << ", Token: " << getStrToken((it.second)("+"));
			cout << " >\n";
		});
	}
};