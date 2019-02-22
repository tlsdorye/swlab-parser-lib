#pragma once
#include <functional>
#include <string>
#include <map> // test¿ë
#include <vector>
#include <array>
#include <regex>
using namespace std;

template <typename TOKEN>
class CommonParserUtil
{
private:
	// test¿ë tokenBuilder
	map<string, function<TOKEN(string)>> tokenBuilder;
	vector<TOKEN> V;

public:
	CommonParserUtil()
	{

	}
	void lex(string regExp, function<TOKEN(string)> func) // function < return-type (parameter)> functor
	{
		tokenBuilder[regExp] = func;
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



