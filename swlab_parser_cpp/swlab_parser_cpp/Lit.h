#pragma once
#include "Expr.h"

class Lit : public Expr
{
private:
	int integerLit;
public:
	Lit(int i);
	int getInteger();
};

