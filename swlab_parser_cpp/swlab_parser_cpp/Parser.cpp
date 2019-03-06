#pragma once
#include "Parser.h"

Parser::Parser()
{
	new Lexer(pu);

	pu.ruleStartSymbol("SeqExpr'");
	pu.rule("SeqExpr' -> SeqExpr", [this]()->vector<Expr> { return pu.get(1); });
	pu.rule("SeqExpr -> SeqExpr ; AssignExpr", [this]()->vector<Expr> {
		vector<Expr> seqexpr = pu.get(1);
		vector<Expr> assignexpr = pu.get(3);
		seqexpr.insert(seqexpr.end(), assignexpr.begin(), assignexpr.end());
		return seqexpr;
	});
	pu.rule("SeqExpr -> AssignExpr", [this]()->vector<Expr> {
		vector<Expr> seqexpr;
		vector<Expr> assignexpr = pu.get(1);

	});
	pu.rule("", []()->vector<Expr> {});
	pu.rule("", []()->vector<Expr> {});
	pu.rule("", []()->vector<Expr> {});
	pu.rule("", []()->vector<Expr> {});
	pu.rule("", []()->vector<Expr> {});
	pu.rule("", []()->vector<Expr> {});
}

void Parser::parsing(vector<string> filepaths)
{
	pu.parsing(filepaths);
}

void Parser::lexing(vector<string> filepaths)
{
	pu.lexing(filepaths);
}

// cpp lambda: [introducer](parameter) ->return-type { statement };
// cpp lambda: [introducer](parameter) {statement}
// [introducer]: []�ȿ� �ܺκ����� ������� ���� �Լ��� �̸� capture�ؼ� ���� ���ο��� �̿� ����
// (parameter): ()�ȿ� ����� ���� ���ڸ� ��ִ´�.
// ->return-type: �����Լ��� ��ȯŸ�� ���´� ��������
// {statement}: ���� ���ο��� ������ ������ ���´�