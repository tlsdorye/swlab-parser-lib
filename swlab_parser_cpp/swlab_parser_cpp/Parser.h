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
	CommonParserUtil<Token, Expr, vector> pu;
public:
	Parser();
	void parsing(vector<string> filepaths);
	void lexing(vector<string> filepaths);
};
