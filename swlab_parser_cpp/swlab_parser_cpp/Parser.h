#pragma once
#include "Token.h"
#include "CommonParserUtil.h"
#include "Lexer.h"
#include "Expr.h"
#include "Assign.h"
#include "BinOp.h"
#include "Lit.h"
#include "Var.h"
#include <vector>
using namespace std;

template<typename AST,
	template<typename ELEM, typename = allocator<ELEM>> class CONTAINER>
class Parser
{
private:
	CommonParserUtil<Token, AST, CONTAINER> parser_util;
public:
	Parser()
	{
		new Lexer(parser_util);

		parser_util.SetStartSymbol("SeqExpr'");
		parser_util.AddTreeLambda("SeqExpr' -> SeqExpr", 0, [this]()->CONTAINER<AST*> {
			return parser_util.GetStackInTrees(1);
		});
		parser_util.AddTreeLambda("SeqExpr -> SeqExpr ; AssignExpr", 1, [this]()->CONTAINER<AST*> {
			CONTAINER<AST*> seqexpr = parser_util.GetStackInTrees(1);
			CONTAINER<AST*> assignexpr = parser_util.GetStackInTrees(3);
			seqexpr.insert(seqexpr.end(), assignexpr.begin(), assignexpr.end());
			return seqexpr;
		});
		parser_util.AddTreeLambda("SeqExpr -> AssignExpr", 2, [this]()->CONTAINER<AST*> {
			CONTAINER<AST*> assignexpr = parser_util.GetStackInTrees(1);
			CONTAINER<AST*> seqexpr(assignexpr);
			return seqexpr;
		});
		parser_util.AddTreeLambda("AssignExpr -> identifier = AssignExpr", 3, [this]()->CONTAINER<AST*> {
			string identifier = parser_util.GetStackInSyntax(1);
			CONTAINER<AST*> assignexpr = parser_util.GetStackInTrees(3);
			CONTAINER<AST*> ret(1, new Assign(identifier, *assignexpr.begin()));
			return ret;
		});

		parser_util.AddTreeLambda("AssignExpr -> AdditiveExpr", 4, [this]()->CONTAINER<AST*> {
			return parser_util.GetStackInTrees(1);
		});
		parser_util.AddTreeLambda("AdditiveExpr -> AdditiveExpr + MultiplicativeExpr", 5, [this]()->CONTAINER<Expr*> {
			CONTAINER<AST*> additiveexpr = parser_util.GetStackInTrees(1);
			CONTAINER<AST*> multiplicativeexpr = parser_util.GetStackInTrees(3);
			CONTAINER<AST*> ret(1, new BinOp(OpKind::ADD, *additiveexpr.begin(), *multiplicativeexpr.begin()));
			return ret;
		});
		parser_util.AddTreeLambda("AdditiveExpr -> AdditiveExpr - MultiplicativeExpr", 6, [this]()->CONTAINER<AST*> {
			CONTAINER<AST*> additiveexpr = parser_util.GetStackInTrees(1);
			CONTAINER<AST*> multiplicativeexpr = parser_util.GetStackInTrees(3);
			CONTAINER<AST*> ret(1, new BinOp(OpKind::SUB, *additiveexpr.begin(), *multiplicativeexpr.begin()));
			return ret;
		});
		parser_util.AddTreeLambda("AdditiveExpr -> MultiplicativeExpr", 7, [this]()->CONTAINER<AST*> {
			return parser_util.GetStackInTrees(1);
		});
		parser_util.AddTreeLambda("MultiplicativeExpr -> MultiplicativeExpr * PrimaryExpr", 8, [this]()->CONTAINER<Expr*> {
			CONTAINER<AST*> multicativeexpr = parser_util.GetStackInTrees(1);
			CONTAINER<AST*> primaryexpr = parser_util.GetStackInTrees(3);
			CONTAINER<AST*> ret(1, new BinOp(OpKind::MUL, *multicativeexpr.begin(), *primaryexpr.begin()));
			return ret;
		});
		parser_util.AddTreeLambda("MultiplicativeExpr -> MultiplicativeExpr / PrimaryExpr", 9, [this]()->CONTAINER<AST*> {
			CONTAINER<AST*> multicativeexpr = parser_util.GetStackInTrees(1);
			CONTAINER<AST*> primaryexpr = parser_util.GetStackInTrees(3);
			CONTAINER<AST*> ret(1, new BinOp(OpKind::DIV, *multicativeexpr.begin(), *primaryexpr.begin()));
			return ret;
		});
		parser_util.AddTreeLambda("MultiplicativeExpr -> PrimaryExpr", 10, [this]()->CONTAINER<AST*> {
			return parser_util.GetStackInTrees(1);
		});
		parser_util.AddTreeLambda("PrimaryExpr -> identifier", 11, [this]()->CONTAINER<AST*> {
			string var_name = parser_util.GetStackInSyntax(1);
			CONTAINER<AST*> ret(1, new Var(var_name));
			return ret;
		});
		parser_util.AddTreeLambda("PrimaryExpr -> integer_number", 12, [this]()->CONTAINER<Expr*> {
			int integer_number = stoi(parser_util.GetStackInSyntax(1));
			CONTAINER<AST*> ret(1, new Lit(integer_number));
			return ret;
		});
		parser_util.AddTreeLambda("PrimaryExpr -> ( AssignExpr )", 13, [this]()->vector<AST*> {
			return parser_util.GetStackInTrees(2);
		});
	}

	CONTAINER<AST*> Parsing(CONTAINER<string> filepaths)
	{
		return parser_util.Parsing(filepaths);
	}

	void Lexing(CONTAINER<string> filepaths)
	{
		parser_util.Lexing(filepaths);
	}
};

// cpp lambda: [introducer](parameter) ->return-type { statement };
// cpp lambda: [introducer](parameter) {statement}
// [introducer]: []안에 외부변수를 써넣으면 람다 함수가 이를 capture해서 람다 내부에서 이용 가능
// (parameter): ()안에 실행시 받을 인자를 써넣는다.
// ->return-type: 람다함수의 반환타입 적는다 생략가능
// {statement}: 람다 내부에서 실행할 내용을 적는다