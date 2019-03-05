#include "Main.h"
#include <regex>
#include <cstring>
using namespace std;

void printToken(Token tok)
{
	cout << "printToken: " << getStrToken(tok) << endl;
}

void printTerminal(Terminal<Token> term)
{
	cout << "printTerminal: [";
	cout << term.getSyntax() << ", " << getStrToken(term.getToken()) << ", ";
	cout << term.getCharIdx() << ", " << term.getLineIdx() << " ]\n";
}

int main()
{
	//Token T(Token::END_OF_TOKEN);
	//Terminal<Token> Term("syn-tax", T, 1, 2);

	//printToken(T);
	//printTerminal(Term);

	CommonParserUtil<Token> pu;
	Lexer lexer(pu);
	//pu.testTokenBuilder();

	string filePath = "multiline.txt";
	//pu.lexing(ifstream(filePath));
	//pu.testTerminals();
	Parser P;
	P.Parsing(ifstream(filePath), false);
}