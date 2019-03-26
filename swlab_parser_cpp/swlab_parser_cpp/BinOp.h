#pragma once
#include "Expr.h"

enum class OpKind {
	ADD = 1,
	SUB,
	MUL,
	DIV
};

class BinOp : public Expr
{
private:
	Expr* left, *right;
protected:
	OpKind op;
public:
	BinOp(OpKind op, Expr* left, Expr* right);
	OpKind get_op();
	Expr* get_right();
	Expr* get_left();
	string get_str_op(OpKind opkind);
	string toString();
};

