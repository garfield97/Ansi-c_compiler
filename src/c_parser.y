%code requires{
  #include "ast.hpp"

  #include <cassert>

  extern const Node *g_root; // A way of getting the AST out

  //! This is to fix problems when generating C++
  // We are declaring the functions provided by Flex, so
  // that Bison generated code can call them.
  int yylex(void);
  void yyerror(FILE* fp, const char *);
}

%parse-param { FILE* fp };

// Represents the value associated with any kind of
// AST node.
%union{
  const Node* node;
  std::string* string;
  int intValue;
  unsigned int uintValue;
  long int longintValue;
  unsigned long longuintValue;
  char characterValue;
}

           

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


%type <node> EXPR,EXPR_PRIMARY,EXPR_POSTFIX,EXPR_UNARY,NAME_TYPE,OPR_UNARY,EXPR_CAST,EXPR_MUL,EXPR_ADD,
             EXPR_SHIFT,EXPR_RELATIONAL,EXPR_EQUALITY,EXPR_AND,EXPR_XOR,EXPR_INCLUSIVE_OR,
             EXPR_LOGIC_AND,EXPR_LOGIC_OR,EXPR_CONDITIONAL,OPR_ASSIGNMENT,EXPR_CONST,DECLARATION,SPECIFIER_DECLARATION
             DECLARATOR_INIT_LIST,DECLARATOR_INIT,SPECIFIER_STORE_CLASS,SPECIFIER_TYPE,SPECIFIER_UNION_OR_STRUCT,UNION_OR_STRUCT
                             
                                       // for grammar production rules
%type <string> IDENTIFIER STRING_LITERAL
%type <intValue> INT_C
%type <uintValue> UNSIGNED_C
%type <longintValue> LONG_C
%type <longuintValue> UNSIGNED_LONG_C
%type <characterValue> CHARACTER_C


%start ROOT

%%


ROOT : PROGRAM { g_root = $1; }

//
EXPR_PRIMARY : IDENTIFIER
            |INT_C 
            |UNSIGNED_C 
            |LONG_C 
            |UNSIGNED_LONG_C
            |CHARACTER_C
            |STRING_LITERAL
            |L_BRACKET EXPR R_BRACKET
            
//
EXPR_POSTFIX : EXPR_PRIMARY
             | EXPR_POSTFIX L_SQUARE EXPR R_SQUARE
             | EXPR_POSTFIX L_BRACKET R_BRACKET
             | EXPR_POSTFIX L_BRACKET ARG_EXPR_LIST R_BRACKET
             | EXPR_POSTFIX '.' IDENTIFIER
             | EXPR_POSTFIX OP_PTR IDENTIFIER
             | EXPR_POSTFIX OP_INC
             | EXPR_POSTFIX OP_DEC

//
ARG_EXPR_LIST : EXPR_ASSIGNMENT
              | ARG_EXPR_LIST ',' EXPR_ASSIGNMENT

           
//
EXPR_UNARY : EXPR_POSTFIX 
           | OP_INC EXPR_UNARY
           | OP_DEC EXPR_UNARY
           | OPR_UNARY EXPR_CAST
           | SIZEOF EXPR_UNARY
           | SIZEOF L_BRACKET NAME_TYPE R_BRACKET            


//
OPR_UNARY : OP_BAND
          | OP_MUL
          | OP_PLUS
          | OP_MINUS
          | OP_B_ONESC
          | OP_LNOT
          
//          
EXPR_CAST : EXPR_UNARY
          | L_BRACKET NAME_TYPE R_BRACKET EXPR_CAST
        
          
          
//          
EXPR_MUL : EXPR_CAST
         | EXPR_MUL OP_MUL EXPR_CAST
         | EXPR_MUL OP_DIV EXPR_CAST
         | EXPR_MUL OP_MOD EXPR_CAST
         
         
//         
EXPR_ADD : EXPR_MUL
         | EXPR_ADD OP_PLUS EXPR_MUL
         | EXPR_ADD OP_MINUS EXPR_MUL
            
         
         
//         
EXPR_SHIFT : EXPR_ADD
           | EXPR_SHIFT OP_BLEFT EXPR_ADD
           | EXPR_SHIFT OP_BRIGHT EXPR_ADD
        
        
        
        
//           
EXPR_RELATIONAL : EXPR_SHIFT
                | EXPR_RELATIONAL OP_L EXPR_SHIFT
                | EXPR_RELATIONAL OP_G EXPR_SHIFT
                | EXPR_RELATIONAL OP_LE EXPR_SHIFT
                | EXPR_RELATIONAL OP_GE EXPR_SHIFT
     
     
     
//                
EXPR_EQUALITY : EXPR_RELATIONAL
              | EXPR_EQUALITY OP_EQ EXPR_RELATIONAL
              | EXPR_EQUALITY OP_NE EXPR_RELATIONAL



//
EXPR_AND : EXPR_EQUALITY
         | EXPR_AND OP_BAND EXPR_EQUALITY
         



//
EXPR_XOR : EXPR_AND
         | EXPR_XOR OP_BXOR EXPR_AND
         


//
EXPR_INCLUSIVE_OR : EXPR_XOR 
                  | EXPR_INCLUSIVE_OR OP_BOR EXPR_XOR
                  
                  
                  
//                  
EXPR_LOGIC_AND : EXPR_INCLUSIVE_OR  
               | EXPR_LOGIC_AND OP_LAND EXPR_INCLUSIVE_OR
             
             
               
//               
EXPR_LOGIC_OR : EXPR_LOGIC_AND
              | EXPR_LOGIC_OR OP_LOR EXPR_LOGIC_AND
              
              
              
     
//             
EXPR_CONDITIONAL : EXPR_LOGIC_OR
                 | EXPR_LOGIC_OR '?' EXPR ':' EXPR_CONDITIONAL
         
         
//        
EXPR_ASSIGNMENT : EXPR_CONDITIONAL
                | EXPR_UNARY OPR_ASSIGNMENT EXPR_ASSIGNMENT


//             
OPR_ASSIGNMENT : ASSIGN           { $$ = new opr_assignment("=");   }
               | MUL_ASSIGN       { $$ = new opr_assignment("*=");  }
               | DIV_ASSIGN       { $$ = new opr_assignment("/=");  }
               | MOD_ASSIGN       { $$ = new opr_assignment("%=");  }
               | ADD_ASSIGN       { $$ = new opr_assignment("+=");  }
               | SUB_ASSIGN       { $$ = new opr_assignment("-=");  }
               | LEFT_ASSIGN      { $$ = new opr_assignment("<<="); }
               | RIGHT_ASSIGN     { $$ = new opr_assignment(">>="); }
               | AND_ASSIGN       { $$ = new opr_assignment("&=");  }
               | XOR_ASSIGN       { $$ = new opr_assignment("^=");  }
               | OR_ASSIGN        { $$ = new opr_assignment("|=");  }


//
EXPR : EXPR_ASSIGNMENT
     | EXPR ',' EXPR_ASSIGNMENT
     
     
     
     
//     
EXPR_CONST : EXPR_CONDITIONAL   { $$ = $1; }



//
DECLARATION : SPECIFIER_DECLARATION ';'
            | SPECIFIER_DECLARATION DECLARATOR_INIT_LIST ';'
            



//
SPECIFIER_DECLARATION : SPECIFIER_STORE_CLASS
                       | SPECIFIER_STORE_CLASS SPECIFIER_DECLARATION
                       | SPECIFIER_TYPE
                       | SPECIFIER_TYPE SPECIFIER_DECLARATION
                       | QUALIFIER_TYPE
                       | QUALIFIER_TYPE SPECIFIER_DECLARATION
 
 
                       
//
DECLARATOR_INIT_LIST : DECLARATOR_INIT
                     | DECLARATOR_INIT_LIST ',' DECLARATOR_INIT
   
                     
//                     
DECLARATOR_INIT : DECLARATOR
                | DECLARATOR ASSIGN INITIALIZER
                
                
                

SPECIFIER_STORE_CLASS : TYPEDEF
                      | EXTERN
                      | STATIC
                      | AUTO
                      | REGISTER
              
              
              
                      
SPECIFIER_TYPE : VOID
               | CHAR
               | SHORT
               | INT
               | LONG
               | FLOAT
               | DOUBLE
               | SIGNED
               | UNSIGNED
               | SPECIFIER_UNION_OR_STRUCT
               | SPECIFIER_ENUM 
               | NAME_TYPE
               
               
               
               
               
SPECIFIER_UNION_OR_STRUCT : UNION_OR_STRUCT IDENTIFIER L_BRACE DECLARATION_LIST_STRUCT R_BRACE
                          | UNION_OR_STRUCT L_BRACE DECLAR_LIST_STRUCT R_BRACE
                          | UNION_OR_STRUCT IDENTIFIER
                          
                          
                          
                          
//                          
UNION_OR_STRUCT : STRUCT
                | UNION
                
                
                
                
DECLARATION_LIST_STRUCT : DECLARATION_STRUCT
                        | DECLARATION_LIST_STRUCT DECLARATION_STRUCT
                        


DECLARATION_STRUCT : LIST_SPEC_QUAL LIST_STRUCT_DECLARATOR ';'


//
LIST_SPEC_QUAL : SPECIFIER_TYPE LIST_SPEC_QUAL
               | SPECIFIER_TYPE 
               | QUALIFIER_TYPE LIST_SPEC_QUAL
               | QUALIFIER_TYPE
               
               
                             
LIST_STRUCT_DECLARATOR : STRUCT_DECLARATOR
                       | LIST_STRUCT_DECLARATOR ',' STRUCT_DECLARATOR
                       





STRUCT_DECLARATOR : DECLARATOR
                  | ':' EXPR_CONST
                  | DECLARATOR ':' EXPR_CONST
                  



                         
//                          
SPECIFIER_ENUM : ENUM L_BRACE LIST_ENUMERATOR R_BRACE
               | ENUM IDENTIFIER L_BRACE LIST_ENUMERATOR R_BRACE
               | ENUM IDENTIFIER
               




//
LIST_ENUMERATOR : ENUMERATOR
                | LIST_ENUMERATOR ',' ENUMERATOR
                
//                
ENUMERATOR : IDENTIFIER 
           | IDENTIFIER ASSIGN EXPR_CONST
           
//
QUALIFIER_TYPE : CONST
               | VOLATILE
               
//               
DECLARATOR : DECLARATOR_POINTER_DIRECT
           : DECLARATOR_DIRECT
           
DECLARATOR_DIRECT : IDENTIFIER
                  | L_BRACKET DECLARATOR R_BRACKET
                  | DECLARATOR_DIRECT L_SQUARE EXPR_CONST R_SQUARE
                  | DECLARATOR_DIRECT L_SQUARE R_SQUARE
                  | DECLARATOR_DIRECT L_BRACKET LIST_PARAM_TYPE R_BRACKET
                  | DECLARATOR_DIRECT L_BRACKET LIST_IDENTIFIER R_BRACKET
                  | DECLARATOR_DIRECT L_BRACKET R_BRACKET

//
POINTER : OP_MUL
        | OP_MUL LIST_QUALIFIER_TYPE
        | OP_MUL POINTER
        | OP_MUL LIST_QUALIFIER_TYPE POINTER

//
LIST_QUALIFIER_TYPE : QUALIFIER_TYPE
                    | LIST_QUALIFIER_TYPE QUALIFIER_TYPE


//
LIST_PARAM_TYPE : LIST_PARAMETER
                | LIST_PARAMETER ',' ELLIPSIS


//
LIST_PARAMETER : DECLARATION_PARAMETER
               | LIST_PARAMETER ',' DECLARATION_PARAMETER
               


DECLARATION_PARAMETER : SPECIFIER_DECLARATION DECLARATOR        
                      | SPECIFIER_DECLARATION DECLARATOR_ABSTRACT
                      | SPECIFIER_DECLARATION
                      
                      
//
LIST_IDENTIFIER : IDENTIFIER 
                | LIST_IDENTIFIER ',' IDENTIFIER

//
NAME_TYPE : LIST_SPEC_QUAL
          | LIST_SPEC_QUAL DECLARATOR_ABSTRACT


DECLARATOR_ABSTRACT : POINTER
                    | DECLARATOR_DIRECT_ABSTRACT
                    | POINTER DECLARATOR_DIRECT_ABSTRACT
   
   
   
                    
DECLARATOR_DIRECT_ABSTRACT : L_BRACKET DECLARATOR_ABSTRACT R_BRACKET
                           | L_SQUARE R_SQUARE
                           | L_SQUARE EXPR_CONST R_SQUARE
                           | DECLARATOR_DIRECT_ABSTRACT L_SQUARE R_SQUARE
                           | DECLARATOR_DIRECT_ABSTRACT L_SQUARE EXPR_CONST R_SQUARE
                           | L_BRACKET R_BRACKET
                           | L_BRACKET LIST_PARAM_TYPE R_BRACKET
                           | DECLARATOR_DIRECT_ABSTRACT L_BRACKET R_BRACKET
                           | DECLARATOR_DIRECT_ABSTRACT L_BRACKET LIST_PARAM_TYPE R_BRACKET
                           
                                         
//
INITIALIZER : EXPR_ASSIGNMENT    
            | L_BRACKET LIST_INITIALIZER R_BRACKET
            | L_BRACKET LIST_INITIALIZER ',' R_BRACKET 
            

//
LIST_INITIALIZER : INITIALIZER
                 | LIST_INITIALIZER ',' INITIALIZER
                 
//               
STATEMENT : STATEMENT_LABELED
          | STATEMENT_COMPOUND
          | STATEMENT_EXPR
          | STATEMENT_SELECTION
          | STATEMENT_ITERATION
          | STATEMENT_JUMP


//          
STATEMENT_LABELED : IDENTIFIER ':' STATEMENT
                  | CASE EXPR_CONST ':' STATEMENT
                  | DEFAULT ':' STATEMENT
                  


//
STATEMENT_COMPOUND : L_BRACE R_BRACE
                   | L_BRACE LIST_STATEMENT R_BRACE
                   | L_BRACE LIST_DECLARATION R_BRACE
                   | L_BRACE LIST_DECLARATION LIST_STATEMENT R_BRACE
                   
//
LIST_DECLARATION : DECLARATION
                 | LIST_DECLARATION DECLARATION
                 
//              
LIST_STATEMENT : STATEMENT
               | LIST_STATEMENT STATEMENT
                  

//
STATEMENT_EXPR : ';'
               | EXPR ';'
               

//
STATEMENT_SELECTION : IF L_BRACKET EXPR R_BRACKET STATEMENT
                    | IF L_BRACKET EXPR R_BRACKET STATEMENT ELSE STATEMENT
                    | SWITCH L_BRACKET EXPR R_BRACKET STATEMENT
                    


//
STATEMENT_ITERATION : WHILE L_BRACKET EXPR R_BRACKET STATEMENT
                    | DO STATEMENT WHILE L_BRACKET EXPR R_BRACKET ';' 
                    | FOR L_BRACKET STATEMENT_EXPR STATEMENT_EXPR R_BRACKET STATEMENT
                    | FOR L_BRACKET STATEMENT_EXPR STATEMENT_EXPR EXPR R_BRACKET STATEMENT



//
STATEMENT_JUMP : GOTO IDENTIFIER ';'
               | CONTINUE ';'
               | BREAK ';'
               | RETURN ';'
               | RETURN EXPR ';'


//               
PROGRAM : DECLARATION_EXTERNAL
            | PROGRAM DECLARATION_EXTERNAL


//
DECLARATION_EXTERNAL : DEFINITION_FUNCTION
                     | DECLARATION

//
DEFINITION_FUNCTION : SPECIFIER_DECLARATION DECLARATOR LIST_DECLARATION STATEMENT_COMPOUND
                    | SPECIFIER_DECLARATION DECLARATOR STATEMENT_COMPOUND
                    | DECLARATOR LIST_DECLARATION STATEMENT_COMPOUND
                    | DECLARATOR STATEMENT_COMPOUND
         
            


%%


const Node *g_root; // Definition of variable (to match declaration earlier)

const Node *parseAST(FILE* fp)
{
  g_root=0;
  yyparse(fp);
  return g_root;
}


#include <stdio.h>

extern char yytext[];
extern int column;

yyerror(s)
char *s;
{
	fflush(stdout);
	printf("\n%*s\n%*s\n", column, "^", column, s);
}


