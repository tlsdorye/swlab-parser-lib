#include "Var.h"

Var::Var(string varName)
{
	this->varName = varName;

}

string Var::getVarName()
{
	return varName;
}


string Var::toString()
{
	return string("Var(" + varName +")");
}