#include "Lit.h"
#include <iostream>
using namespace std;

Lit::Lit(int i)
{
	this->integer_value = i;
	this->type = "Lit";
}

int Lit::get_integer_value()
{
	return integer_value;
}

string Lit::toString()
{
	string ret("(" + to_string(integer_value) + ")");
	return ret;
}
