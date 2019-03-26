#include "BinOp.h"

BinOp::BinOp(OpKind op, Expr* left, Expr* right)
{
	this->op = op;
	this->left = left;
	this->right = right;
	this->type = "BinOp";
}

OpKind BinOp::getOpKind()
{
	return op;
}

Expr* BinOp::getRight()
{
	return right;
}

Expr* BinOp::getLeft()
{
	return left;
}

string BinOp::getStrOpKind(OpKind opkind)
{
	string ret;
	switch (opkind)
	{
	case OpKind::ADD:
		ret = "+";
		break;
	case OpKind::SUB:
		ret = "-";
		break;
	case OpKind::MUL:
		ret = "*";
		break;
	case OpKind::DIV:
		ret = "/";
		break;
	default:
		break;
	}
	return ret;
}

string BinOp::toString()
{
	return string("BinOp(" + getStrOpKind(op) + "," + left->toString() + "," + right->toString() + ")");
}