#pragma once
#include <map>
#include <string>
#include <iostream>
using namespace std;

enum class EToken
{
	END_OF_TOKEN,
	EMPTY_EPACE,
	OPEN_PAREN,
	CLOSE_PAREN,
	IDENTIFIER,
	INTEGER_NUMBER,
	ADD,
	SUB,
	MUL,
	DIV,
	EQ,
	SEMICOLON
};

class TokenException
{
private:
	string error_message;
public:
	TokenException() {};
	TokenException(string s)
	{
		error_message = s;
	}
	void printerror()
	{
		cout << error_message;
	}
};

class Token
{
private:
	EToken eToken;
	string sToken;
public:
	Token();
	Token(string sToken, EToken eToken);
	EToken getEToken();
	string getSToken();
};