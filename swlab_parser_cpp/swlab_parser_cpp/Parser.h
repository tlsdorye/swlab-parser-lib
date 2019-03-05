#pragma once
#include "Token.h"
#include "CommonParserUtil.h"
#include <fstream>
#include <iostream>
using namespace std;

class Parser
{
private:
	CommonParserUtil<Token> pu;
public:
	Parser();
	void Parsing(ifstream reader, bool flag);
	void Parsing(ifstream reader);
	void Lexing(ifstream reader, bool flag);
	void Lexing(ifstream reader);
};
