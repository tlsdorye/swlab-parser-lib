#pragma once
#include "Expr.h"

class Var : public Expr
{
private:
	string varName;
public:
	Var(string varName);
	string getVarName();

};

