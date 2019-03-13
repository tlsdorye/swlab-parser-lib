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
// [introducer]: []�ȿ� �ܺκ����� ������� ���� �Լ��� �̸� capture�ؼ� ���� ���ο��� �̿� ����
// (parameter): ()�ȿ� ����� ���� ���ڸ� ��ִ´�.
// ->return-type: �����Լ��� ��ȯŸ�� ���´� ��������
// {statement}: ���� ���ο��� ������ ������ ���´�