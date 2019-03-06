#pragma once
#include "Token.h"
#include "CommonParserUtil.h"
#include "Lexer.h"
#include "Expr.h"
#include <vector>
using namespace std;

class Parser
{
private:
	CommonParserUtil<Token, Expr> pu;
public:
	Parser();
	void parsing(vector<string> filepaths);
	void lexing(vector<string> filepaths);
};
