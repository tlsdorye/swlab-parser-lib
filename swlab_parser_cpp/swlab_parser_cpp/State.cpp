#include "State.h"

State::State()
{
}

State::State(int state)
{
	this->state = state;
}

int State::getState()
{
	return state;
}

void State::setState(int state)
{
	this->state = state;
}

void State::toString()
{
	
}
