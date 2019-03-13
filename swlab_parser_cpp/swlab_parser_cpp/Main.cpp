#include "Main.h"
using namespace std;

void printToken(Token tok)
{
	cout << "printToken: " << tok.getSToken() << endl;
}

void printTerminal(Terminal<Token> term)
{
	cout << "printTerminal: [";
	cout << term.getSyntax() << ", " << term.getToken().getSToken() << ", ";
	cout << term.getCharIdx() << ", " << term.getLineIdx() << " ]\n";
}

int main()
{
	//Token T(Token::END_OF_TOKEN);
	//Terminal<Token> Term("syn-tax", T, 1, 2);

	//printToken(T);
	//printTerminal(Term);

	//CommonParserUtil<Token, Expr, vector> pu;
	//Lexer lexer(pu);
	//pu.testTokenBuilder();

	vector<string> filepaths;
	filepaths.push_back("oneline.txt");
	//filepaths.push_back("multiline.txt");
	//pu.lexing(filepaths);
	//pu.testTerminals();
	Parser P;
	P.parsing(filepaths);
	//P.testCommonParserUtil_TreeBuilder();
	//ParseState* ps = new ParseState("0");
	//StackElement* elem = dynamic_cast<StackElement*>(ps);
	//ParseState* p = dynamic_cast<ParseState*>(elem);
	//cout << p->getState();
}