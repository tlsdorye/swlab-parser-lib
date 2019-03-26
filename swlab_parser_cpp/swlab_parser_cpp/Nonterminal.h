#pragma once
#include "StackElement.h"
#include <vector>
using namespace std;

template<typename AST, 
	template<typename ELEM, typename = allocator<ELEM>> class CONTAINER>
class Nonterminal : public StackElement
{
private:
	CONTAINER<AST*> tree;
	//Object tree
public:
	Nonterminal(CONTAINER<AST*> tree)
	{
		this->tree = tree;
	}

	CONTAINER<AST*> getTree()
	{
		return tree;
	}

	void setTree(CONTAINER<AST*> tree)
	{
		this->tree = tree;
	}

	string toString()
	{
		return "Nonterminal";
	}
};