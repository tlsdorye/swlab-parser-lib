#include "Token.h"

map<string, EToken> CheckToken =
{
	{ "_", EToken::EMPTY_EPACE },{ "(", EToken::OPEN_PAREN },
	{ ")", EToken::CLOSE_PAREN },{ "identifier", EToken::IDENTIFIER },
	{ "integer_number", EToken::INTEGER_NUMBER },{ "+", EToken::ADD },
	{ "-", EToken::SUB },{ "*", EToken::MUL },{ "/", EToken::DIV },
	{ "=", EToken::EQ },{ ";", EToken::SEMICOLON }
};

Token::Token()
{

}

Token::Token(string sToken, EToken eToken)
{
	map<string, EToken>::iterator it = CheckToken.find(sToken);
	if (it == CheckToken.end()) throw TokenException("Invalid string Token\n");
	else if (eToken != it->second) throw TokenException("Enum Token does not match string Token\n");
	this->eToken = eToken;
	this->sToken = sToken;
}

EToken Token::getEToken()
{
	return this->eToken;
}

string Token::getSToken()
{
	return this->sToken;
}