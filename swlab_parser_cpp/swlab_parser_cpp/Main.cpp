#include "Main.h"
using namespace std;

void printToken(Token tok)
{
	cout << "printToken: " << tok.get_str_token() << endl;
}

void printTerminal(Terminal<Token> term)
{
	cout << "printTerminal: [";
	cout << term.getSyntax() << ", " << term.getToken().get_str_token() << ", ";
	cout << term.getCharIdx() << ", " << term.getLineIdx() << " ]\n";
}

int main()
{
	vector<string> filepaths;
	filepaths.push_back("oneline.txt");
	//filepaths.push_back("multiline.txt");

	Parser P;
	vector<Expr*> exprSeq = P.Parsing(filepaths);
	for (auto it : exprSeq) cout << "parsing result: " << it->toString() << endl;
}