#include "Assign.h"

Assign::Assign(string var_name, Expr* rhs)
{
	this->var_name = var_name;
	this->rhs = rhs;
	this->type = "Assign";
}

string Assign::get_var_name()
{
	return var_name;
}

Expr* Assign::get_rhs()
{
	return rhs;
}

string Assign::toString()
{
	string ret("(" + var_name + "=" + rhs->toString() + ")");
	return ret;
}
