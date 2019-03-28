#include "Main.h"
using namespace std;


int main()
{
	//string program = "";
	//string option = "/c ../genlrparser-exe.exe ../mygrammar.grm -output a.txt b.txt c.txt";
	//C:\Users\tlsdo\Source\Repos\swlab_parser_cpp\swlab_parser_cpp\swlab_parser_cpp
	//ShellExecute(NULL, NULL, "cmd", NULL, NULL, SW_SHOWNORMAL);

	vector<string> filepaths;
	//filepaths.push_back("oneline.txt");
	filepaths.push_back("multiline.txt");

	Parser P;
	vector<Expr*> exprSeq = P.Parsing(filepaths);
	for (auto it : exprSeq) cout << "parsing result: " << it->toString() << endl;
}