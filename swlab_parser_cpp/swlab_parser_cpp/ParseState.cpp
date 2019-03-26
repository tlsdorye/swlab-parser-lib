#include "ParseState.h"

ParseState::ParseState()
{
}

ParseState::ParseState(string state)
{
	this->state = state;
}

string ParseState::get_state()
{
	return state;
}

void ParseState::set_state(string state)
{
	this->state = state;
}

string ParseState::toString()
{
	return state;
}
