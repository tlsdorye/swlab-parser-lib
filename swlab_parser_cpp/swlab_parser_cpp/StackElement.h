#pragma once
#include <string>
using namespace std;

class StackElement
{
private:
	string type;
public:
	StackElement()
	{

	}
	virtual string toString()
	{
		return type;
	}
};

