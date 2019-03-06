#pragma once
#include "StackElement.h"
#include "Token.h"

template<typename TOKEN>
class Terminal : public StackElement
{
private:
	string syntax;
	TOKEN tok;
	int charIdx;
	int lineIdx;
public:
	Terminal() {}

	Terminal(string syntax, TOKEN tok, int charIdx, int lineIdx)
	{
		this->syntax = syntax;
		this->tok = tok;
		this->charIdx = charIdx;
		this->lineIdx = lineIdx;
	}

	string getSyntax()
	{
		return syntax;
	}

	void setSyntax(string syntax)
	{
		this->syntax = syntax;
	}

	TOKEN getToken()
	{
		return tok;
	}

	void setToken(TOKEN tok)
	{
		this->tok = tok;
	}

	int getCharIdx()
	{
		return charIdx;
	}

	void setCharIdx(int charIdx)
	{
		this->charIdx = charIdx;
	}

	int getLineIdx()
	{
		return lineIdx;
	}

	void setLineIdx(int lineIdx)
	{
		this->lineIdx = lineIdx;
	}

	void toString()
	{

	}
};

