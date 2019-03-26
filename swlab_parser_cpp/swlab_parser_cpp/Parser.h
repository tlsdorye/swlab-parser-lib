#pragma once
#include "Token.h"
#include "CommonParserUtil.h"
#include "Lexer.h"
#include "Expr.h"
#include "Assign.h"
#include "BinOp.h"
#include "Lit.h"
#include "Var.h"
#include <vector>
using namespace std;

class Parser
{
private:
	CommonParserUtil<Token, Expr, vector> parser_util;
public:
	Parser();
	vector<Expr*> Parsing(vector<string> filepaths);
	void Lexing(vector<string> filepaths);
};
