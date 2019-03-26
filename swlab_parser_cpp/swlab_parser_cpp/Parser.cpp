#pragma once
#include "Parser.h"

Parser::Parser()
{
	new Lexer(parser_util);

	parser_util.setStartSymbol("SeqExpr'");
	parser_util.AddTreeLambda("SeqExpr' -> SeqExpr", [this]()->vector<Expr*> {
		return parser_util.getStackInTrees(1);
	});
	parser_util.AddTreeLambda("SeqExpr -> SeqExpr ; AssignExpr", [this]()->vector<Expr*> {
		vector<Expr*> seqexpr = parser_util.getStackInTrees(1);
		vector<Expr*> assignexpr = parser_util.getStackInTrees(3);
		seqexpr.insert(seqexpr.end(), assignexpr.begin(), assignexpr.end());
		return seqexpr;
	});
	parser_util.AddTreeLambda("SeqExpr -> AssignExpr", [this]()->vector<Expr*> {
		vector<Expr*> seqexpr;
		vector<Expr*> assignexpr = parser_util.getStackInTrees(1);
		seqexpr.insert(seqexpr.end(), assignexpr.begin(), assignexpr.end());
		return seqexpr;
	});
	parser_util.AddTreeLambda("AssignExpr -> identifier = AssignExpr", [this]()->vector<Expr*> {
		string identifier = parser_util.getStackInSyntax(1);
		vector<Expr*> assignexpr = parser_util.getStackInTrees(3);
		vector<Expr*> ret;
		ret.push_back(new Assign(identifier, assignexpr[0]));
		return ret;
	});

	parser_util.AddTreeLambda("AssignExpr -> AdditiveExpr", [this]()->vector<Expr*> {
		return parser_util.getStackInTrees(1);
	});
	parser_util.AddTreeLambda("AdditiveExpr -> AdditiveExpr + MultiplicativeExpr", [this]()->vector<Expr*> {
		vector<Expr*> additiveexpr = parser_util.getStackInTrees(1);
		vector<Expr*> multiplicativeexpr = parser_util.getStackInTrees(3);
		vector<Expr*> ret;
		ret.push_back(new BinOp(OpKind::ADD, additiveexpr[0], multiplicativeexpr[0]));
		return ret;
	});
	parser_util.AddTreeLambda("AdditiveExpr -> AdditiveExpr - MultiplicativeExpr", [this]()->vector<Expr*> {
		vector<Expr*> additiveexpr = parser_util.getStackInTrees(1);
		vector<Expr*> multiplicativeexpr = parser_util.getStackInTrees(3);
		vector<Expr*> ret;
		ret.push_back(new BinOp(OpKind::SUB, additiveexpr[0], multiplicativeexpr[0]));
		return ret;
	});
	parser_util.AddTreeLambda("AdditiveExpr -> MultiplicativeExpr", [this]()->vector<Expr*> {
		return parser_util.getStackInTrees(1);
	});
	parser_util.AddTreeLambda("MultiplicativeExpr -> MultiplicativeExpr * PrimaryExpr", [this]()->vector<Expr*> {
		vector<Expr*> multicativeexpr = parser_util.getStackInTrees(1);
		vector<Expr*> primaryexpr = parser_util.getStackInTrees(3);
		vector<Expr*> ret;
		ret.push_back(new BinOp(OpKind::MUL, multicativeexpr[0], primaryexpr[0]));
		return ret;
	});
	parser_util.AddTreeLambda("MultiplicativeExpr -> MultiplicativeExpr / PrimaryExpr", [this]()->vector<Expr*> {
		vector<Expr*> multicativeexpr = parser_util.getStackInTrees(1);
		vector<Expr*> primaryexpr = parser_util.getStackInTrees(3);
		vector<Expr*> ret;
		ret.push_back(new BinOp(OpKind::DIV, multicativeexpr[0], primaryexpr[0]));
		return ret;
	});
	parser_util.AddTreeLambda("MultiplicativeExpr -> PrimaryExpr", [this]()->vector<Expr*> {
		return parser_util.getStackInTrees(1);
	});
	parser_util.AddTreeLambda("PrimaryExpr -> identifier", [this]()->vector<Expr*> {
		vector<Expr*> ret;
		ret.push_back(new Var(parser_util.getStackInSyntax(1)));
		return ret;
	});
	parser_util.AddTreeLambda("PrimaryExpr -> integer_number", [this]()->vector<Expr*> {
		int integer_number = stoi(parser_util.getStackInSyntax(1));
		vector<Expr*> ret;
		ret.push_back(new Lit(integer_number));
		return ret;
	});
	parser_util.AddTreeLambda("PrimaryExpr -> ( AssignExpr )", [this]()->vector<Expr*> {
		return parser_util.getStackInTrees(2);
	});
}

vector<Expr*> Parser::Parsing(vector<string> filepaths)
{
	return parser_util.Parsing(filepaths);
}

void Parser::Lexing(vector<string> filepaths)
{
	parser_util.Lexing(filepaths);
}

// cpp lambda: [introducer](parameter) ->return-type { statement };
// cpp lambda: [introducer](parameter) {statement}
// [introducer]: []안에 외부변수를 써넣으면 람다 함수가 이를 capture해서 람다 내부에서 이용 가능
// (parameter): ()안에 실행시 받을 인자를 써넣는다.
// ->return-type: 람다함수의 반환타입 적는다 생략가능
// {statement}: 람다 내부에서 실행할 내용을 적는다