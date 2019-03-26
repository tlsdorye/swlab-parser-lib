#include "Var.h"

Var::Var(string var_name)
{
	this->var_name = var_name;
	this->type = "Var";
}

string Var::get_var_name()
{
	return var_name;
}


string Var::toString()
{
	string ret("(" + var_name + ")");
	return ret;
}