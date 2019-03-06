#pragma once
#include "Token.h"
#include "CommonParserUtil.h"
#include "Expr.h"

class Lexer
{
public:
	Lexer(CommonParserUtil<Token, Expr> &pu);
};