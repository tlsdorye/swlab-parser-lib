#pragma once
#include "StackElement.h"

class State : public StackElement
{
private:
	int state;
public:
	State();
	State(int state);
	int getState();
	void setState(int state);
	void toString();
};

