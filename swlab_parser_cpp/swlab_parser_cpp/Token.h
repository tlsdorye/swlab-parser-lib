#pragma once
#include "TokenInterface.h"
#include <map>
using namespace std;

class Token : public TokenInterface
{
private:
	static map<string, string> tokenmap;
	string strToken;
public:
	Token();
	Token(string s);
	bool checkToken(string s);
	string toString();
};