#include "Lit.h"

Lit::Lit(int i)
{
	this->integerLit = i;
}

int Lit::getInteger()
{
	return integerLit;
}
