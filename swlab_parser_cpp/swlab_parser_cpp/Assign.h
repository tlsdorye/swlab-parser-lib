#pragma once
#include "Expr.h"

class Assign : public Expr
{
private:
	string var_name;
	Expr* rhs;
public:
	Assign(string var_name, Expr* rhs);
	string get_var_name();
	Expr* get_rhs();
	string toString();
};

