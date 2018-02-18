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

           
%token EXPR
 
%token IDENTIFIER STRING_LITERAL SIZEOF 
 
%token INT_C UNSIGNED_C LONG_C UNSIGNED_LONG_C
%token CHARACTER_C

// Keywords
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE  BREAK RETURN
%token TYPEDEF EXTERN STATIC AUTO REGISTER STRUCT UNION ENUM ELIPSIS

%token R_BRACKET L_BRACKET R_SQUARE L_SQUARE R_BRACE L_BRACE


// Operators
%token OP_PLUS OP_MINUS OP_MUL OP_DIV OP_MOD OP_INC OP_DEC
%token OP_L OP_G OP_LE OP_GE OP_EQ OP_NE
%token OP_LNOT OP_LAND OP_LOR
%token OP_BAND OP_BOR OP_BXOR OP_B_ONESC OP_BRIGHT OP_BLEFT
%token OP_PTR 


// Assignments
%token ASSIGN AND_ASSIGN OR_ASSIGN XOR_ASSIGN
%token RIGHT_ASSIGN LEFT_ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN


%type <expr> EXPR,EXPR_PRIMARY,EXPR_POSTFIX,EXPR_UNARY,NAME_TYPE,OPR_UNARY,EXPR_CAST,EXPR_MUL,EXPR_ADD,
             EXPR_SHIFT,EXPR_RELATIONAL,
                
                                       // for grammar production rules
%type <string> IDENTIFIER STRING_LITERAL
%type <intValue> INT_C
%type <uintValue> UNSIGNED_C
%type <longintValue> LONG_C
%type <longuintValue> UNSIGNED_LONG_C
%type <characterValue> CHARACTER_C


%start ROOT

%%


ROOT : EXPR { g_root = $1; }


EXPR_PRIMARY : IDENTIFIER
            |INT_C 
            |UNSIGNED_C 
            |LONG_C 
            |UNSIGNED_LONG_C
            |CHARACTER_C
            |STRING_LITERAL
            |L_BRACKET EXPR R_BRACKET
            

EXPR_POSTFIX : EXPR_PRIMARY
             | EXPR_POSTFIX L_SQUARE EXPR R_SQUAR
             | EXPR_POSTFIX L_BRACKET R_BRACKET
             | EXPR_POSTFIX L_BRACKET ARG_EXPR_LIST R_BRACKET
             | EXPR_POSTFIX . IDENTIFIER
             | EXPR_POSTFIX OP_PTR IDENTIFIER
             | EXPR_POSTFIX OP_INC
             | EXPR_POSTFIX OP_DEC


           

EXPR_UNARY : EXPR_POSTFIX 
           | OP_INC EXPR_UNARY
           | OP_DEC EXPR_UNARY
           | EXPR_UNARY EXPR_CAST
           | SIZEOF EXPR_UNARY
           | SIZEOF L_BRACKET NAME_TYPE R_BRACKET            



OPR_UNARY : OP_BAND
          | OP_MUL
          | OP_PLUS
          | OP_MINUS
          | OP_B_ONESC
          | OP_LNOT
          
          
EXPR_CAST : EXPR_UNARY
          | L_BRACKET NAME_TYPE R_BRACKET EXPR_CAST
        
          
          
          
EXPR_MUL : EXPR_CAST
         | EXPR_MUL OP_MUL EXPR_CAST
         | EXPR_MUL OP_DIV EXPR_CAST
         | EXPR_MUL OP_MOD EXPR_CAST
         
         
         
EXPR_ADD : EXPR_MUL
         | EXPR_ADD OP_PLUS EXPR_MUL
         | EXPR_ADD OP_MINUS EXPR_MUL
         
         
         
EXPR_SHIFT : EXPR_ADD
           | EXPR_SHIFT OP_LEFT EXPR_ADD
           | EXPR_SHIFT OP_RIGHT EXPR_ADD
           
EXPR_RELATIONAL : EXPR_SHIFT
                | EXPR_RELATIONAL OP_LEFT EXPR_SHIFT
                | EXPR_RELATIONAL OP_RIGHT EXPR_SHIFT
                | EXPR_RELATIONAL OP_LE EXPR_SHIFT
                | EXPR_RELATIONAL OP_GE EXPR_SHIFT
                






%%

const Expression *g_root; // Definition of variable (to match declaration earlier)

const Expression *parseAST()
{
  g_root=0;
  yyparse();
  return g_root;
}
