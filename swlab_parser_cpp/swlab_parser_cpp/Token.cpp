#include "Token.h"

map<EToken, string> str_to_enum_token =
{
	{EToken::END_OF_TOKEN, "$"} ,
	{EToken::EMPTY_EPACE, "_" },{ EToken::OPEN_PAREN, "(" },
	{EToken::CLOSE_PAREN, ")" },{ EToken::IDENTIFIER, "identifier" },
	{EToken::INTEGER_NUMBER, "integer_number" },{ EToken::ADD, "+" },
	{ EToken::SUB, "-" },{ EToken::MUL, "*" },{ EToken::DIV, "/" },
	{ EToken::EQ, "=" },{ EToken::SEMICOLON, ";" }
};

Token::Token()
{

}

Token::Token(EToken eToken)
{
	map<EToken, string>::iterator it = str_to_enum_token.find(eToken);
	if (it != str_to_enum_token.end())
	{
		this->str_token = it->second;
		this->enum_token = eToken;
	}
}

EToken Token::get_enum_token()
{
	return this->enum_token;
}

string Token::get_str_token()
{
	return this->str_token;
}