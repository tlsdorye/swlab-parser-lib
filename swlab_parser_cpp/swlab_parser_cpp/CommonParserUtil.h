#pragma once
#include "Terminal.h"
#include <iostream>
#include <fstream>
#include <functional>
#include <string>
#include <map> // test용
#include <vector>
#include <array>
#include <regex>
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
	void lex(string regExp, function<TOKEN(string)> func) // function < return-type (parameter)> functor
	{
		tokenBuilder[regExp] = func;
	}

	void lexing(ifstream reader)
	{
		vector<string> lines;
		if (reader.is_open())
		{
			string line;
			vector<regex> regExps;

			while (getline(reader, line)) lines.push_back(line);
			for (auto it : tokenBuilder) regExps.push_back(regex(it.first));
			for (int i = 0; i < lines.size(); i++) matching(regExps, lines[i], i + 1);

		}
		else
		{
			// 파일이 안열림 -> 파일 경로 에러?
		}
	}

	//matching function
	void matching(vector<regex> &regExps, string &_line, int lineno)
	{
		int charIdx = 0;
		string line = _line;
		smatch sm;
		while (line != "")
		{
			bool matched = false;
			for (auto it : regExps)
			{
				//매칭되는것이 있고, prefix가 없을경우 -> 첫문자부터 매칭되는경우
				if (regex_search(line, sm, it) && sm.prefix() == "")
				{
					terminals.push_back(Terminal<TOKEN>(sm[0], TOKEN::ADD, charIdx, lineno));
					line = sm.suffix();
					charIdx += sm[0].length();
					matched = true;
					break;
				}
			}
			if (!matched)
			{
				cout << "no matching! \n";
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
			cout << it.getSyntax() << " " << getStrToken(it.getToken()) << " ";
			cout << it.getCharIdx() << " " << it.getLineIdx() << "\n";
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



