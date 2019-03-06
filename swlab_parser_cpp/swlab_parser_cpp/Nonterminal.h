#pragma once
#include "StackElement.h"
#include <vector>
using namespace std;

template<typename AST>
class Nonterminal : public StackElement
{
private:
	vector<AST> tree;
public:
	Nonterminal(vector<AST> tree)
	{
		this->tree = tree;
	}

	vector<AST> getTree()
	{
		return tree;
	}

	void setTree(vector<AST> tree)
	{
		this->tree = tree;
	}

	void toString()
	{

	}
};