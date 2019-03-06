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
// [introducer]: []안에 외부변수를 써넣으면 람다 함수가 이를 capture해서 람다 내부에서 이용 가능
// (parameter): ()안에 실행시 받을 인자를 써넣는다.
// ->return-type: 람다함수의 반환타입 적는다 생략가능
// {statement}: 람다 내부에서 실행할 내용을 적는다