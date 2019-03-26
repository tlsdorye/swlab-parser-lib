#include "Lit.h"
#include <iostream>
using namespace std;

Lit::Lit(int i)
{
	this->integerLit = i;
	this->type = "Lit";
}

int Lit::getInteger()
{
	return integerLit;
}

string Lit::toString()
{
	return string("Lit(" + to_string(integerLit) + ")");
}
