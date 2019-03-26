#pragma once
#include "Parser.h"

Parser::Parser()
{
	new Lexer(pu);

	pu.setStartSymbol("SeqExpr'");
	pu.rule("SeqExpr' -> SeqExpr", [this]()->vector<Expr*> {return pu.getTree(1); });
	pu.rule("SeqExpr -> SeqExpr ; AssignExpr", [this]()->vector<Expr*> {
		vector<Expr*> seqexpr = pu.getTree(1);
		vector<Expr*> assignexpr = pu.getTree(3);
		seqexpr.insert(seqexpr.end(), assignexpr.begin(), assignexpr.end());
		return seqexpr;
	});
	pu.rule("SeqExpr -> AssignExpr", [this]()->vector<Expr*> {
		vector<Expr*> seqexpr;
		vector<Expr*> assignexpr = pu.getTree(1);
		seqexpr.insert(seqexpr.end(), assignexpr.begin(), assignexpr.end());
		return seqexpr;
	});
	pu.rule("AssignExpr -> identifier = AssignExpr", [this]()->vector<Expr*> {
		string identifier = pu.getText(1);
		vector<Expr*> assignexpr = pu.getTree(3);
		vector<Expr*> ret;
		ret.push_back(new Assign(identifier, assignexpr[0]));
		return ret;
	});

	pu.rule("AssignExpr -> AdditiveExpr", [this]()->vector<Expr*> {return pu.getTree(1); });
	pu.rule("AdditiveExpr -> AdditiveExpr + MultiplicativeExpr", [this]()->vector<Expr*> {
		vector<Expr*> additiveexpr = pu.getTree(1);
		vector<Expr*> multiplicativeexpr = pu.getTree(3);
		vector<Expr*> ret;
		ret.push_back(new BinOp(OpKind::ADD, additiveexpr[0], multiplicativeexpr[0]));
		return ret;
	});
	pu.rule("AdditiveExpr -> AdditiveExpr - MultiplicativeExpr", [this]()->vector<Expr*> {
		vector<Expr*> additiveexpr = pu.getTree(1);
		vector<Expr*> multiplicativeexpr = pu.getTree(3);
		vector<Expr*> ret;
		ret.push_back(new BinOp(OpKind::SUB, additiveexpr[0], multiplicativeexpr[0]));
		return ret;
	});
	pu.rule("AdditiveExpr -> MultiplicativeExpr", [this]()->vector<Expr*> {return pu.getTree(1); });
	pu.rule("MultiplicativeExpr -> MultiplicativeExpr * PrimaryExpr", [this]()->vector<Expr*> {
		vector<Expr*> multicativeexpr = pu.getTree(1);
		vector<Expr*> primaryexpr = pu.getTree(3);
		vector<Expr*> ret;
		ret.push_back(new BinOp(OpKind::MUL, multicativeexpr[0], primaryexpr[0]));
		return ret;
	});
	pu.rule("MultiplicativeExpr -> MultiplicativeExpr / PrimaryExpr", [this]()->vector<Expr*> {
		vector<Expr*> multicativeexpr = pu.getTree(1);
		vector<Expr*> primaryexpr = pu.getTree(3);
		vector<Expr*> ret;
		ret.push_back(new BinOp(OpKind::DIV, multicativeexpr[0], primaryexpr[0]));
		return ret;
	});
	pu.rule("MultiplicativeExpr -> PrimaryExpr", [this]()->vector<Expr*> {return pu.getTree(1); });
	pu.rule("PrimaryExpr -> identifier", [this]()->vector<Expr*> {
		vector<Expr*> ret;
		ret.push_back(new Var(pu.getText(1)));
		return ret;
	});
	pu.rule("PrimaryExpr -> integer_number", [this]()->vector<Expr*> {
		int integer_number = stoi(pu.getText(1));
		vector<Expr*> ret;
		ret.push_back(new Lit(integer_number));
		return ret;
	});
	pu.rule("PrimaryExpr -> ( AssignExpr )", [this]()->vector<Expr*> {return pu.getTree(2); });
}

vector<Expr*> Parser::parsing(vector<string> filepaths)
{
	return pu.parsing(filepaths);
}

void Parser::lexing(vector<string> filepaths)
{
	pu.Lexing(filepaths);
}

void Parser::testCommonParserUtil_TreeBuilder()
{
	pu.testTreeBuilders();
}

// cpp lambda: [introducer](parameter) ->return-type { statement };
// cpp lambda: [introducer](parameter) {statement}
// [introducer]: []�ȿ� �ܺκ����� ������� ���� �Լ��� �̸� capture�ؼ� ���� ���ο��� �̿� ����
// (parameter): ()�ȿ� ����� ���� ���ڸ� ��ִ´�.
// ->return-type: �����Լ��� ��ȯŸ�� ���´� ��������
// {statement}: ���� ���ο��� ������ ������ ���´�