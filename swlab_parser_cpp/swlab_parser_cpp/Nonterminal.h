#pragma once
#include "StackElement.h"
#include <vector>
using namespace std;

template<typename AST, 
	template<typename ELEM, typename = allocator<ELEM>> class CONTAINER>
class Nonterminal : public StackElement
{
private:
	CONTAINER<AST*> trees;
public:
	Nonterminal(CONTAINER<AST*> trees)
	{
		this->trees = trees;
	}

	CONTAINER<AST*> getTrees()
	{
		return trees;
	}

	void setTrees(CONTAINER<AST*> trees)
	{
		this->trees = trees;
	}

	string toString()
	{
		return "Nonterminal";
	}
};