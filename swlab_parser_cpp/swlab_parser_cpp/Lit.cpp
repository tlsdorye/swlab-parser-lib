#include "Lit.h"

Lit::Lit(int i)
{
	this->integerLit = i;
}

int Lit::getInteger()
{
	return integerLit;
}

string Lit::toString()
{
	return string("Lit(" + to_string(integerLit) + ")");
}
