#include "Token.h"

map<string, string> Token::tokenmap =
{
	{"_", "EMPTY_SPACE"}, {"(", "OPEN_PAREN"},
	{")", "CLOSE_PAREN"}, {"identifier", "IDENTIFIER"},
	{"integer_number", "INTEGER_NUMBER"}, {"+", "ADD"},
	{"-", "SUB"}, {"*", "MUL"}, {"/", "DIV"}, 
	{"=", "EQ"}, {";", "SEMICOLON"}
};

Token::Token() 
{

}

Token::Token(string s)
{
	strToken = s;
}

bool Token::checkToken(string s)
{
	map<string, string>::iterator it = tokenmap.find(s);
	if (it != tokenmap.end() && it->second == strToken) return true;
	return false;
}

string Token::toString()
{
	return strToken;
}