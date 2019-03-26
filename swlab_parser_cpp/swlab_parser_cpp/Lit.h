#pragma once
#include "Expr.h"

class Lit : public Expr
{
private:
	int integer_value;
public:
	Lit(int i);
	int get_integer_value();
	string toString();
};

