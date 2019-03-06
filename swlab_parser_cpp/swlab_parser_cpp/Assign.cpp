#include "Assign.h"

Assign::Assign(string varName, Expr rhs)
{
	this->varName = varName;
	this->rhs = rhs;
}

string Assign::getVarName()
{
	return varName;
}

Expr Assign::getRhs()
{
	return rhs;
}

void Assign::toString()
{

}