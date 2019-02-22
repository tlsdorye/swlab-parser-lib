#include "Token.h"

string strToken[] = 
{
	"$","_", "(", ")",
	"identifier", "integer_number",
	"+", "-", "*", "/", "=", ";"
};

string getStrToken(Token token)
{
	return strToken[(int)token];
}