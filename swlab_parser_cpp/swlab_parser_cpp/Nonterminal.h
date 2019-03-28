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
		string all_tree_root_type = "[";
		for (auto it : trees) all_tree_root_type += it->type + ", ";
		all_tree_root_type = all_tree_root_type.substr(0, all_tree_root_type.size() - 2);
		all_tree_root_type += "]";
		return all_tree_root_type;
	}
};