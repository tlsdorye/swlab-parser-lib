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
// [introducer]: []�ȿ� �ܺκ����� ������� ���� �Լ��� �̸� capture�ؼ� ���� ���ο��� �̿� ����
// (parameter): ()�ȿ� ����� ���� ���ڸ� ��ִ´�.
// ->return-type: �����Լ��� ��ȯŸ�� ���´� ��������
// {statement}: ���� ���ο��� ������ ������ ���´�