#include "Lexer.h"

Lexer::Lexer(CommonParserUtil<Token, Expr, vector> &parser_util)
{
	parser_util.LexEndOfToken(Token(EToken::END_OF_TOKEN));
	parser_util.AddTokenLambda("[ \t\n]", [](string text)->Token {return Token(EToken::EMPTY_EPACE); });
	parser_util.AddTokenLambda("[0-9]+", [](string text)->Token {return Token(EToken::INTEGER_NUMBER); });
	parser_util.AddTokenLambda("\\(", [](string text)->Token {return Token(EToken::OPEN_PAREN); });
	parser_util.AddTokenLambda("\\)", [](string text)->Token {return Token(EToken::CLOSE_PAREN); });
	parser_util.AddTokenLambda("\\+", [](string text)->Token {return Token(EToken::ADD); });
	parser_util.AddTokenLambda("\\-", [](string text)->Token {return Token(EToken::SUB); });
	parser_util.AddTokenLambda("\\*", [](string text)->Token {return Token(EToken::MUL); });
	parser_util.AddTokenLambda("\\/", [](string text)->Token {return Token(EToken::DIV); });
	parser_util.AddTokenLambda("\\=", [](string text)->Token {return Token(EToken::EQ); });
	parser_util.AddTokenLambda("\\;", [](string text)->Token {return Token(EToken::SEMICOLON); });
	parser_util.AddTokenLambda("[a-zA-Z]+[a-zA-Z0-9]*", [](string text)->Token {return Token(EToken::IDENTIFIER); });
}

// cpp lambda: [introducer](parameter) ->return-type { statement };
// cpp lambda: [introducer](parameter) {statement}
// [introducer]: []안에 외부변수를 써넣으면 람다 함수가 이를 capture해서 람다 내부에서 이용 가능
// (parameter): ()안에 실행시 받을 인자를 써넣는다.
// ->return-type: 람다함수의 반환타입 적는다 생략가능
// {statement}: 람다 내부에서 실행할 내용을 적는다