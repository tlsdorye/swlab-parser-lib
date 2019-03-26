#pragma once
#include <string>
using namespace std;

class Expr
{
public:
	string type;
	Expr() {}
	virtual string toString()
	{
		return type;
	}
};

