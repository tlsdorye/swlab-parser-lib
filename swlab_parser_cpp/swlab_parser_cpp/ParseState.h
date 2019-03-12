#pragma once
#include "StackElement.h"
#include <string>
using namespace std;

class ParseState : public StackElement
{
private:
	string state;
public:
	ParseState();
	ParseState(string state);
	string getState();
	void setState(string state);
	void toString();
};

