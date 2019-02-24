#pragma once
#include <string>
using namespace std;

enum class Token
{
	EMPTY_SPACE,
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

string strToken[];

string getStrToken(Token _token);
