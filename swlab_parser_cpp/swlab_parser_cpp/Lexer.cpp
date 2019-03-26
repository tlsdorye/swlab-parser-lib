#include "Lexer.h"

Lexer::Lexer(CommonParserUtil<Token, Expr, vector> &pu)
{
	//pu.lexEndToken(Token::END_OF_TOKEN);
	pu.LexEndOfToken(Token("$", EToken::END_OF_TOKEN));
	pu.Lex("[ \t\n]", [](string text)->Token {return Token("_", EToken::EMPTY_EPACE); });
	pu.Lex("[0-9]+", [](string text)->Token {return Token("integer_number", EToken::INTEGER_NUMBER); });
	pu.Lex("\\(", [](string text)->Token {return Token("(", EToken::OPEN_PAREN); });
	pu.Lex("\\)", [](string text)->Token {return Token(")", EToken::CLOSE_PAREN); });
	pu.Lex("\\+", [](string text)->Token {return Token("+", EToken::ADD); });
	pu.Lex("\\-", [](string text)->Token {return Token("-", EToken::SUB); });
	pu.Lex("\\*", [](string text)->Token {return Token("*", EToken::MUL); });
	pu.Lex("\\/", [](string text)->Token {return Token("/", EToken::DIV); });
	pu.Lex("\\=", [](string text)->Token {return Token("=", EToken::EQ); });
	pu.Lex("\\;", [](string text)->Token {return Token(";", EToken::SEMICOLON); });
	pu.Lex("[a-zA-Z]+[a-zA-Z0-9]*", [](string text)->Token {return Token("identifier", EToken::IDENTIFIER); });
}

// cpp lambda: [introducer](parameter) ->return-type { statement };
// cpp lambda: [introducer](parameter) {statement}
// [introducer]: []�ȿ� �ܺκ����� ������� ���� �Լ��� �̸� capture�ؼ� ���� ���ο��� �̿� ����
// (parameter): ()�ȿ� ����� ���� ���ڸ� ��ִ´�.
// ->return-type: �����Լ��� ��ȯŸ�� ���´� ��������
// {statement}: ���� ���ο��� ������ ������ ���´�