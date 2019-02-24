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

	string filePath = "oneline.txt";
	//pu.lexing(ifstream(filePath));

	string line[] =
	{
		"x x ++ = 123 123 123 ;"
	};
	regex rx("[0-9]+");
	smatch sm;
	for (auto it : line)
	{
		if (regex_search(it, sm, rx))
		{
			cout << "matchs for: '" << it << "'\n";
			cout << "Prefix: '" << sm.prefix() << "'\n";
			for (size_t i = 0; i < sm.size(); i++)
				cout << i << ": '" << sm[i] << "'\n";
			cout << "Suffix: '" << sm.suffix() << "'\n\n";
		}
	}

	pu.lexing(ifstream(filePath));
	pu.testTerminals();
}