#pragma once
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
	// test용 tokenBuilder
	map<string, function<TOKEN(string)>> tokenBuilder;
	vector<TOKEN> tokenV;
	vector<TOKEN> V;

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
		string read_string;
		while (getline(reader, read_string))
		{
			if (reader.is_open())
			{
				cout << "read string: " << read_string << endl;
				regex number("[0-9]+");
				sregex_iterator it(read_string.begin(), read_string.end(), number);
				sregex_iterator end;
				while (it != end)
				{
					smatch m = *it;
					printf("%s\n", m.str(0).c_str());
					//tokenV.push_back(tokenBuilder["[0-9]+"](m.str(0).c_str()));
					it++;
				}
			}
			else
			{
				cout << "파일을 찾을 수 없습니다." << endl;
			}

			for (int i = 0; i < tokenV.size(); i++)
			{
				//cout << getStrToken(tokenV[i]) << endl;
			}
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



