# swlab_parser_cpp
swlap_parser_cpp version

## 1. Token class 작성요령

1. enum class ENUM_TOKEN 을 만듭니다.
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

2. ENUM_TOKEN 과 str_token 사이의 정보를 가지고 있어야 합니다.
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

3. Token class 는 반드시 아래의 멤버를 가져야 합니다.

멤버 변수
```
ENUM_TOKEN enum_token
string str_token
```
생성자
```
Token(ENUM_TOKEN enum_token)
```
멤버 함수
```
string get_str_token() : ENUM_TOKEN 에 대응되는 str_token을 반환
```

## 2. Lexer class 작성요령

1. Lexer class는 AST node class와 그 node를 담는 CONTAINER를 템플릿 인자를 가져야 합니다.
```
template<typename AST,
	template<typename ELEM, typename = allocator<ELEM>> class CONTAINER>
class Lexer
{
	...
};
```

2. Lexer class의 생성자는 CommonParserUtil 을 parameter로 받습니다. 

그리고 CommonParserUtil 의 void AddTokenLambda(regex_expr, lambda_exp)를 함수를 이용하여 Token에 필요한 lambda를 만듭니다.

END_OF_TOKEN은 CommonParserUtil 의 void LexEndOfTokn(ENUM_TOKEN) 함수를 이용합니다.
```
Lexer(CommonParserUtil<Token, AST, CONTAINER> &parser_util)
{
	parser_util.LexEndOfToken(Token(ENUM_TOKEN::END_OF_TOKEN));
	parser_util.AddTokenLambda("[0-9]+"[]()->Token {return Token(ENUM_TOKEN::INTEGER_NUMBER); });
	...
}
```

## 3. Parser class 작성요령

1. Parser class는 AST node class와 그 node를 담는 CONTAINER를 템플릿 인자를 가져야 합니다.
```
template<typename AST,
	template<typename ELEM, typename = allocator<ELEM>> class CONTAINER>
class Parser
{
	...
}
```

2. CommonParserUtil 을 멤버변수로 갖습니다.

3.Start_symbol은 CommonParserUtil 의 void SetStartSymbol(start_tymbol) 함수를 이용합니다.
```
parser_util.SetStartSymbol("SeqExpr`");
```

3. Parser 생성자 내부에서 CommonParserUtil의 void AddTreeLambda(production_rule, index, lambda_expression) 함수룰 이용하여 Tree에 필요한 lambda를 만듭니다

4. lambda 작성요령은 다음과 같습니다.

1) production_rule 작성

```
AssignExpr -> identifier = AssignExpr
```

2) 작성한 production_rule을 실행하는 람다식을 구성

Nonterminal: production_rule 의 오른쪽인경우
- CommonParserUtil 의 CONTAINER<AST> GetStackInTrees(index) 를 이용하여 tree를 가져옵니다.
- 해당 Nonterminal이 production_rule의 위치를 x라고 하면 index = 2*x - 1
	
```	
CONTAINER<AST> assignexpr = parser_util.GetStackInTrees(3);
```

Nonterminal: production_rule의 왼쪽인경우
- 오른쪽의 결과를 CONTAINER<AST> 의 형태로 만들어서 반환합니다.

```
CONTAINER<AST> left_assignexpr;
...
return left_assignexpr;
```

Terminal
- CommonParserUtil 의 CONTAINER<AST> GetStackInSyntax(index) 를 이용하여 syntax를 가져옵니다.
- 해당 Terminal이 production_rule의 위치를 x라고 하면 index = 2*x - 1

```
string identifier = parser_util.GetStackInSyntax(1);
```

3. parser_util.AddTreeLambda(production_rule, lambda)함수를 이용하여 만든 람다식을 넣습니다.

최종 예시

```
parser_util.AddTreeLambda("AssignExpr -> identifier = AssignExpr", 3, []()->CONTAINER<AST*> {
	string identifier = parser_util.GetStackInSyntax(1);
	CONTAINER<AST*> assignexpr = parser_util.GetStackInTrees(3);
	CONTAINER<AST*> ret(1, new Assign(identifier, *assignexpr.begin()));
	return ret;
});
```

* CONTAINER는 vector, list 가 가능합니다. (다른 container는 확인하지 않음)
