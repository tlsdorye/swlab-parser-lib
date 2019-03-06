#include "BinOp.h"

BinOp::BinOp(OpKind op, Expr left, Expr right)
{
	this->op = op;
	this->left = left;
	this->right = right;
}

OpKind BinOp::getOpKind()
{
	return op;
}

Expr BinOp::getRight()
{
	return right;
}

Expr BinOp::getLeft()
{
	return left;
}

void BinOp::toString()
{

}