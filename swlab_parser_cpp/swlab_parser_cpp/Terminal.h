#pragma once
#include "StackElement.h"
#include "Token.h"

template<typename TOKEN>
class Terminal : public StackElement
{
private:
	string syntax;
	TOKEN token;
	int char_idx;
	int line_idx;
public:
	Terminal(string syntax, TOKEN tok, int char_idx, int line_idx)
	{
		this->syntax = syntax;
		this->token = tok;
		this->char_idx = char_idx;
		this->line_idx = line_idx;
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
		return token;
	}

	void setToken(TOKEN token)
	{
		this->token = token;
	}

	int getCharIdx()
	{
		return char_idx;
	}

	void setCharIdx(int char_idx)
	{
		this->char_idx = char_idx;
	}

	int getLineIdx()
	{
		return line_idx;
	}

	void setLineIdx(int line_idx)
	{
		this->line_idx = line_idx;
	}

	string toString()
	{
		return token.get_str_token();
	}
};

