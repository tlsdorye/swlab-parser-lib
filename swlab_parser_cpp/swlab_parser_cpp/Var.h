#pragma once
#include "Expr.h"

class Var : public Expr
{
private:
	string var_name;
public:
	Var(string var_name);
	string get_var_name();
	string toString();
};

