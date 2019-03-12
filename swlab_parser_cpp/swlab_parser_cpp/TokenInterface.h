#pragma once
#include <string>
using namespace std;

class TokenInterface
{
public:
	virtual bool checkToken(string s) = 0;
	virtual string toString() = 0;
};