#pragma once
#include "Parser.h"

Parser::Parser()
{
	new Lexer(pu);

	pu.ruleStartSymbol("SeqExpr'");
	pu.rule("SeqExpr' -> SeqExpr", [this]()->vector<Expr> {return pu.get(1); });
	pu.rule("SeqExpr -> SeqExpr ; AssignExpr", [this]()->vector<Expr> {
		vector<Expr> seqexpr = pu.get(1);
		vector<Expr> assignexpr = pu.get(3);
		seqexpr.insert(seqexpr.end(), assignexpr.begin(), assignexpr.end());
		return seqexpr;
	});
	pu.rule("SeqExpr -> AssignExpr", [this]()->vector<Expr> {
		vector<Expr> seqexpr;
		vector<Expr> assignexpr = pu.get(1);
		seqexpr.insert(seqexpr.end(), assignexpr.begin(), assignexpr.end());
		return seqexpr;
	});
	pu.rule("AssignExpr -> identifier = AssignExpr", [this]()->vector<Expr> {
		string identifier = pu.getText(1);
		vector<Expr> assignexpr = pu.get(3);
		vector<Expr> ret;
		ret.push_back(Assign(identifier, assignexpr[0]));
		return ret;
	});

	pu.rule("AssignExpr -> AdditiveExpr", [this]()->vector<Expr> {return pu.get(1); });
	pu.rule("AdditiveExpr -> AdditiveExpr + MultiplicativeExpr", [this]()->vector<Expr> {
		vector<Expr> additiveexpr = pu.get(1);
		vector<Expr> multiplicativeexpr = pu.get(3);
		vector<Expr> ret;
		ret.push_back(BinOp(OpKind::ADD, additiveexpr[0], multiplicativeexpr[0]));
		return ret;
	});
	pu.rule("AdditiveExpr -> AdditiveExpr - MultiplicativeExpr", [this]()->vector<Expr> {
		vector<Expr> additiveexpr = pu.get(1);
		vector<Expr> multiplicativeexpr = pu.get(3);
		vector<Expr> ret;
		ret.push_back(BinOp(OpKind::SUB, additiveexpr[0], multiplicativeexpr[0]));
		return ret;
	});
	pu.rule("AdditiveExpr -> MultiplicativeExpr", [this]()->vector<Expr> {return pu.get(1); });
	pu.rule("MultiplicativeExpr -> MultiplicativeExpr * PrimaryExpr", [this]()->vector<Expr> {
		vector<Expr> multicativeexpr = pu.get(1);
		vector<Expr> primaryexpr = pu.get(3);
		vector<Expr> ret;
		ret.push_back(BinOp(OpKind::MUL, multicativeexpr[0], primaryexpr[0]));
		return ret;
	});
	pu.rule("MultiplicativeExpr->MultiplicativeExpr / PrimaryExpr", [this]()->vector<Expr> {
		vector<Expr> multicativeexpr = pu.get(1);
		vector<Expr> primaryexpr = pu.get(3);
		vector<Expr> ret;
		ret.push_back(BinOp(OpKind::DIV, multicativeexpr[0], primaryexpr[0]));
		return ret;
	});
	pu.rule("MultiplicativeExpr -> PrimaryExpr", [this]()->vector<Expr> {return pu.get(1); });
	pu.rule("PrimaryExpr -> identifier", [this]()->vector<Expr> {
		string integer_number_str = pu.getText(1);
		int integer_number = stoi(integer_number_str);
		vector<Expr> ret;
		ret.push_back(Lit(integer_number));
		return ret;
	});
	pu.rule("PrimaryExpr -> ( AssignExpr )", [this]()->vector<Expr> {return pu.get(2); });
}

void Parser::parsing(vector<string> filepaths)
{
	pu.parsing(filepaths);
}

void Parser::lexing(vector<string> filepaths)
{
	pu.lexing(filepaths);
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