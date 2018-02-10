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

            
%token IDENTIFIER STRING_LITERAL SIZEOF  
%token INT_C UNSIGNED_C LONG_C UNSIGNED_LONG_C
%token CHARACTER_C
  
%token TYPEDEF EXTERN STATIC AUTO REGISTER 
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM
%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE  BREAK RETURN


%type <expr>
// need to add type for the above tokens
%type <intValue> INT_C
%type <uintValue> UNSIGNED_C
%type <longintValue> LONG_C
%type <longuintValue> UNSIGNED_LONG_C
%type <characterValue> CHARACTER_C


%start ROOT

%%


ROOT : EXPR { g_root = $1; }

EXPR : TERM                 { $$ = $1; }

%%

const Expression *g_root; // Definition of variable (to match declaration earlier)

const Expression *parseAST()
{
  g_root=0;
  yyparse();
  return g_root;
}
