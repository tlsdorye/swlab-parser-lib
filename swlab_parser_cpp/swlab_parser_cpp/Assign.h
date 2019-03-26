#pragma once
#include "Expr.h"

class Assign : public Expr
{
private:
	string varName;
	Expr* rhs;
public:
	Assign(string varName, Expr* rhs);
	string getVarName();
	Expr* getRhs();
	string toString();
};

