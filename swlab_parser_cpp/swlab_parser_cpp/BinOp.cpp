#include "BinOp.h"

BinOp::BinOp(OpKind op, Expr* left, Expr* right)
{
	this->op = op;
	this->left = left;
	this->right = right;
	this->type = "BinOp";
}

OpKind BinOp::get_op()
{
	return op;
}

Expr* BinOp::get_right()
{
	return right;
}

Expr* BinOp::get_left()
{
	return left;
}

string BinOp::get_str_op(OpKind opkind)
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
	string ret("(" + left->toString() + " " + get_str_op(op) + " " + right->toString() + ")");
	return ret;
}