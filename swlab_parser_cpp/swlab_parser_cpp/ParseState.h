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
	string get_state();
	void set_state(string state);
	string toString();
};

