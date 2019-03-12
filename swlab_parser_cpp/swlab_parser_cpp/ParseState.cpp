#include "ParseState.h"

ParseState::ParseState()
{
}

ParseState::ParseState(string state)
{
	this->state = state;
}

string ParseState::getState()
{
	return state;
}

void ParseState::setState(string state)
{
	this->state = state;
}

void ParseState::toString()
{
	
}
