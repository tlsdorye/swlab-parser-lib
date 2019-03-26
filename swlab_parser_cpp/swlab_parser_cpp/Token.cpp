#include "Token.h"

map<string, EToken> str_to_enum_token =
{
	{"$", EToken::END_OF_TOKEN },
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
	map<string, EToken>::iterator it = str_to_enum_token.find(sToken);
	if (it == str_to_enum_token.end()) throw TokenException("Invalid string Token\n");
	else if (eToken != it->second) throw TokenException("Enum Token does not match string Token\n");
	this->enum_token = eToken;
	this->str_token = sToken;
}

EToken Token::get_enum_token()
{
	return this->enum_token;
}

string Token::get_str_token()
{
	return this->str_token;
}