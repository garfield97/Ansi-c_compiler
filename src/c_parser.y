%code requires{
  #include "ast.hpp"

  #include <cassert>

  extern const Expression *g_root; // A way of getting the AST out

  //! This is to fix problems when generating C++
  // We are declaring the functions provided by Flex, so
  // that Bison generated code can call them.
  int yylex(void);
  void yyerror(const char *);
}

// Represents the value associated with any kind of
// AST node.
%union{
  const Expression *expr;
  std::string *string;
  int intValue;
  unsigned int uintValue;
  long int longintValue;
  unsigned long longuintValue;
  char characterValue;
}


%token KEYWORD IDENTIFIER
%token DECIMAL DECIMAL_U DECIMAL_L DECIMAL_UL
%token OCTAL OCTAL_U OCTAL_L OCTAL_UL
%token HEXAD HEXAD_U HEXAD_L HEXAD_UL
%token CHARACTER_CONSTANT


%type <expr> 
%type <string> KEYWORD IDENTIFIER STRING_LITERAL
%type <intValue> DECIMAL OCTAL HEXAD
%type <uintValue> DECIMAL_U OCTAL_U HEXAD_U
%type <longintValue> DECIMAL_L OCTAL_L HEXAD_L
%type <longuintValue> DECIMAL_UL OCTAL_UL HEXAD_IL
%type <characterValue> CHARACTER_CONSTANT


%start ROOT

%%

/* The TODO notes a are just a guide, and are non-exhaustive.
   The expectation is that you do each one, then compile and test.
   Testing should be done using patterns that target the specific
   feature; the testbench is there to make sure that you haven't
   broken anything while you added it.
*/

ROOT : EXPR { g_root = $1; }

EXPR : TERM                 { $$ = $1; }
	| EXPR T_PLUS TERM		{ $$ = new AddOperator($1, $3); }
	| EXPR T_MINUS TERM		{ $$ = new SubOperator($1, $3); }

TERM : FACTOR                { $$ = $1; }
	| TERM T_TIMES FACTOR  { $$ = new MulOperator($1, $3); }
	| TERM T_DIVIDE FACTOR { $$ = new DivOperator($1, $3); }
	
FACTOR : T_NUMBER          					{ $$ = new Number( $1 ); }
       | T_LBRACKET EXPR T_RBRACKET 		{ $$ = $2; }
	   | T_VARIABLE							{ $$ = new Variable( *$1 ); }
	   | T_LOG T_LBRACKET EXPR T_RBRACKET	{ $$ = new LogFunction($3); }
	   | T_EXP T_LBRACKET EXPR T_RBRACKET	{ $$ = new ExpFunction($3); }
	   | T_SQRT T_LBRACKET EXPR T_RBRACKET  { $$ = new SqrtFunction($3); }
	   | FACTOR T_EXPONENT FACTOR					{ $$ = new ExpOperator($1, $3); }

/* TODO-6 : Extend support to other functions. Requires modifications here, and to FACTOR. */
FUNCTION_NAME : T_LOG { $$ = new std::string("log"); }
				| T_EXP { $$ = new std::string("exp"); }
				| T_SQRT { $$ = new std::string("sqrt"); }

%%

const Expression *g_root; // Definition of variable (to match declaration earlier)

const Expression *parseAST()
{
  g_root=0;
  yyparse();
  return g_root;
}
