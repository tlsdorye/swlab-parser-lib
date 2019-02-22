#include "Lexer.h"

Lexer::Lexer(CommonParserUtil<Token> &pu)
{
	pu.lex("\\$", [](string text)->Token {return Token::END_OF_TOKEN; });
	pu.lex("[ \t\n]", [](string text)->Token {return Token::EMPTY_SPACE; });
	pu.lex("[0-9]+", [](string text)->Token {return Token::INTEGER_NUMBER; });
	pu.lex("\\(", [](string text)->Token {return Token::OPEN_PAREN; });
	pu.lex("\\)", [](string text)->Token {return Token::CLOSE_PAREN; });
	pu.lex("\\+", [](string text)->Token {return Token::ADD; });
	pu.lex("\\-", [](string text)->Token {return Token::SUB; });
	pu.lex("\\*", [](string text)->Token {return Token::MUL; });
	pu.lex("\\/", [](string text)->Token {return Token::DIV; });
	pu.lex("\\=", [](string text)->Token {return Token::EQ; });
	pu.lex("\\;", [](string text)->Token {return Token::SEMICOLON; });
	pu.lex("[a-zA-Z]+[a-zA-Z0-9]", [](string text)->Token {return Token(Token::IDENTIFIER); });
}

// cpp lambda: [introducer](parameter) ->return-type { statement };
// cpp lambda: [introducer](parameter) {statement}
// [introducer]: []�ȿ� �ܺκ����� ������� ���� �Լ��� �̸� capture�ؼ� ���� ���ο��� �̿� ����
// (parameter): ()�ȿ� ����� ���� ���ڸ� ��ִ´�.
// ->return-type: �����Լ��� ��ȯŸ�� ���´� ��������
// {statement}: ���� ���ο��� ������ ������ ���´�