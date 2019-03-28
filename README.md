# swlab_parser_cpp
swlap_parser_cpp version

## 1. Token class �ۼ����

1. enum class ENUM_TOKEN �� ����ϴ�.
ex)
```
enum class ENUM_TOKEN
{
	END_OF_TOKEN,
	EMPTY_EPACE,
	OPEN_PAREN,
	CLOSE_PAREN,
	IDENTIFIER,
	INTEGER_NUMBER,
	ADD,
	SUB,
	MUL,
	DIV,
	EQ,
	SEMICOLON
};
```

2. ENUM_TOKEN �� str_token ������ ������ ������ �־�� �մϴ�.
ex)
```
map<ENUM_TOKEN, string> str_to_enum_token =
{
	{ENUM_TOKEN::END_OF_TOKEN, "$"} ,
	{ENUM_TOKEN::EMPTY_EPACE, "_" },{ ENUM_TOKEN::OPEN_PAREN, "(" },
	{ENUM_TOKEN::CLOSE_PAREN, ")" },{ ENUM_TOKEN::IDENTIFIER, "identifier" },
	{ENUM_TOKEN::INTEGER_NUMBER, "integer_number" },{ ENUM_TOKEN::ADD, "+" },
	{ ENUM_TOKEN::SUB, "-" },{ ENUM_TOKEN::MUL, "*" },{ ENUM_TOKEN::DIV, "/" },
	{ ENUM_TOKEN::EQ, "=" },{ ENUM_TOKEN::SEMICOLON, ";" }
};
```

3. Token class �� �ݵ�� �Ʒ��� ����� ������ �մϴ�.
```
��� ����:
	ENUM_TOKEN enum_token
	string str_token

������:
	Token(ENUM_TOKEN enum_token)

��� �Լ�:
	string get_str_token() : ENUM_TOKEN �� �����Ǵ� str_token�� ��ȯ
```

## 2. Lexer class �ۼ����

1. Lexer class�� AST node class�� �� node�� ��� CONTAINER�� ���ø� ���ڸ� ������ �մϴ�.
ex)
```
template<typename AST,
	template<typename ELEM, typename = allocator<ELEM>> class CONTAINER>
class Lexer
{
	...
};
```

2. Lexer class�� �����ڴ� CommonParserUtil �� parameter�� �޽��ϴ�. 
�׸��� CommonParserUtil �� void AddTokenLambda(regex_expr, lambda_expression)�� �Լ��� �̿��Ͽ� Token�� �ʿ��� lambda�� ����ϴ�.
END_OF_TOKEN�� CommonParserUtil �� void LexEndOfTokn(ENUM_TOKEN) �Լ��� �̿��մϴ�.
ex)
```
Lexer(CommonParserUtil<Token, AST, CONTAINER> &parser_util)
{
	parser_util.LexEndOfToken(Token(ENUM_TOKEN::END_OF_TOKEN));
	parser_util.AddTokenLambda("[0-9]+"[]()->Token {return Token(ENUM_TOKEN::INTEGER_NUMBER); });
	...
}
```

## 3. Parser class �ۼ����

1. Parser class�� AST node class�� �� node�� ��� CONTAINER�� ���ø� ���ڸ� ������ �մϴ�.
ex)
```
template<typename AST,
	template<typename ELEM, typename = allocator<ELEM>> class CONTAINER>
class Parser
{
	...
}
```

2. CommonParserUtil �� ��������� �����ϴ�.

3. Parser ������ ���ο��� CommonParserUtil�� void AddTreeLambda(production_rule, lambda_expression) �Լ��� �̿��Ͽ� Tree�� �ʿ��� lambda�� ����ϴ�
lambda �ۼ������ ������ �����ϴ�.
ex)
```
production_rule
	Nonterminal -> Terminal + Nonterminal �� �������� �����մϴ�.


Nonterminal
	production_rule �� �������ΰ��
		CommonParserUtil �� CONTAINER<AST> GetStackInTrees(index) �� �̿��Ͽ� tree�� �����ɴϴ�.
		�ش� Nonterminal�� production_rule�� ��ġ�� x��� �ϸ� index = 1 + (2*(x-1))
			ex) ù��° ��ġ�� ���: index = 1
				�ι��� ��ġ�� ���: index = 3

	production_rule�� �����ΰ��
		�������� ����� CONTAINER<AST> �� ���·� ���� ��ȯ�մϴ�.

Terminal
	CommonParserUtil �� CONTAINER<AST> GetStackInSyntax(index) �� �̿��Ͽ� syntax�� �����ɴϴ�.
	�ش� Terminal�� production_rule�� ��ġ�� x��� �ϸ� index = 1 + (2*(x-1))
		ex) ù��° ��ġ�� ���: index = 1
			�ι��� ��ġ�� ���: index = 3
```

* CONTAINER�� vector, list �� �����մϴ�. (�ٸ� container�� Ȯ������ ����)