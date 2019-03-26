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
	Expr* left;
	Expr* right;
protected:
	OpKind op;
public:
	BinOp(OpKind op, Expr* left, Expr* right);
	OpKind getOpKind();
	Expr* getRight();
	Expr* getLeft();
	string getStrOpKind(OpKind opkind);
	string toString();
};

