#include "Lexer.h"

Lexer::Lexer(CommonParserUtil<Token, Expr, vector> &pu)
{
	//pu.lexEndToken(Token::END_OF_TOKEN);
	pu.lex("[ \t\n]", [](string text)->Token {return Token("_", EToken::EMPTY_EPACE); });
	pu.lex("[0-9]+", [](string text)->Token {return Token("integer_number", EToken::INTEGER_NUMBER); });
	pu.lex("\\(", [](string text)->Token {return Token("(", EToken::OPEN_PAREN); });
	pu.lex("\\)", [](string text)->Token {return Token(")", EToken::CLOSE_PAREN); });
	pu.lex("\\+", [](string text)->Token {return Token("+", EToken::ADD); });
	pu.lex("\\-", [](string text)->Token {return Token("-", EToken::SUB); });
	pu.lex("\\*", [](string text)->Token {return Token("*", EToken::MUL); });
	pu.lex("\\/", [](string text)->Token {return Token("/", EToken::DIV); });
	pu.lex("\\=", [](string text)->Token {return Token("=", EToken::EQ); });
	pu.lex("\\;", [](string text)->Token {return Token(";", EToken::SEMICOLON); });
	pu.lex("[a-zA-Z]+[a-zA-Z0-9]*", [](string text)->Token {return Token("identifier", EToken::IDENTIFIER); });
}

// cpp lambda: [introducer](parameter) ->return-type { statement };
// cpp lambda: [introducer](parameter) {statement}
// [introducer]: []안에 외부변수를 써넣으면 람다 함수가 이를 capture해서 람다 내부에서 이용 가능
// (parameter): ()안에 실행시 받을 인자를 써넣는다.
// ->return-type: 람다함수의 반환타입 적는다 생략가능
// {statement}: 람다 내부에서 실행할 내용을 적는다