#pragma once
#include "StackElement.h"
#include <vector>
using namespace std;

template<typename AST, 
	template<typename ELEM, typename = allocator<ELEM>> class CONT>
class Nonterminal : public StackElement
{
private:
	CONT<AST> tree;
public:
	Nonterminal(LIST<AST> tree)
	{
		this->tree = tree;
	}

	CONT<AST> getTree()
	{
		return tree;
	}

	void setTree(CONT<AST> tree)
	{
		this->tree = tree;
	}

	void toString()
	{

	}
};