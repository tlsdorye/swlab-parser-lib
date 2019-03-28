#include "Main.h"
using namespace std;


int main()
{
	vector<string> filepaths;
	//filepaths.push_back("oneline.txt");
	filepaths.push_back("multiline.txt");

	Parser P;
	vector<Expr*> exprSeq = P.Parsing(filepaths);
	for (auto it : exprSeq) cout << "parsing result: " << it->toString() << endl;
}