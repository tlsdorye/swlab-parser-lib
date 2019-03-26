#include "Assign.h"

Assign::Assign(string varName, Expr* rhs)
{
	this->varName = varName;
	this->rhs = rhs;
	this->type = "Assign";
}

string Assign::getVarName()
{
	return varName;
}

Expr* Assign::getRhs()
{
	return rhs;
}

string Assign::toString()
{
	return string("Assign(" + varName + "," + rhs->toString() + ")");
}