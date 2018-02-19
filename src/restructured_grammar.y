UNIT_TRANSL : DECLARATION_EXTERNAL
            | UNIT_TRANSL DECLARATION_EXTERNAL




DECLARATION_EXTERNAL : DEFINITION_FUNCTION
                     : DECLARATION
                     
                     
DECLARATION : SPECIFIER_DECLARATION ';'
            | SPECIFIER_DECLARATION DECLARATOR_INIT_LIST ';'
            
            
            
            

SPECIFIER_DECLARATION : SPECIFIER_STORE_CLASS
                       | SPECIFIER_STORE_CLASS SPECIFIER_DECLARATION
                       | SPECIFIER_TYPE
                       | SPECIFIER_TYPE SPECIFIER_DECLARATION
                       | QUALIFIER_TYPE
                       | QUALIFIER_TYPE SPECIFIER_DECLARATION
 
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

QUALIFIER_TYPE : CONST
               | VOLATILE 
 
    
    
    
    
    

DECLARATOR_INIT_LIST : DECLARATOR_INIT
                     | DECLARATOR_INIT_LIST ',' DECLARATOR_INIT


DECLARATOR_INIT : DECLARATOR
                | DECLARATOR ASSIGN INITIALIZER
                
                
INITIALIZER : EXPR_ASSIGNMENT    
            | L_BRACKET LIST_INITIALIZER R_BRACKET
            | L_BRACKET LIST_INITIALIZER ',' R_BRACKET 

LIST_INITIALIZER : INITIALIZER
                 | LIST_INITIALIZER ',' INITIALIZER
                
                
                
                
                
                
                
                   
                     
DEFINITION_FUNCTION : SPECIFIER_DECLARATION DECLARATOR LIST_DECLARATION STATEMENT_COMPOUND
                    | SPECIFIER_DECLARATION DECLARATOR STATEMENT_COMPOUND
                    | DECLARATOR LIST_DECLARATION STATEMENT_COMPOUND
                    | DECLARATOR STATEMENT_COMPOUND
    
DECLARATOR : POINTER DECLARATOR_DIRECT
           | DECLARATOR_DIRECT
           
DECLARATOR_DIRECT : IDENTIFIER
                  | L_BRACKET DECLARATOR R_BRACKET
                  | DECLARATOR_DIRECT L_SQUARE EXPR_CONST R_SQUARE
                  | DECLARATOR_DIRECT L_SQUARE R_SQUARE
                  | DECLARATOR_DIRECT L_BRACKET LIST_PARAM_TYPE R_BRACKET
                  | DECLARATOR_DIRECT L_BRACKET LIST_IDENTIFIER R_BRACKET
                  | DECLARATOR_DIRECT L_BRACKET R_BRACKET



POINTER : OP_MUL
        | OP_MUL LIST_QUALIFIER_TYPE
        | OP_MUL POINTER
        | OP_MUL LIST_QUALIFIER_TYPE POINTER     

LIST_QUALIFIER_TYPE : QUALIFIER_TYPE
                    | LIST_QUALIFIER_TYPE QUALIFIER_TYPE               
                   
LIST_DECLARATION : DECLARATION
                 | LIST_DECLARATION DECLARATION                    
                    
STATEMENT_COMPOUND : L_BRACE R_BRACE
                   | L_BRACE LIST_STATEMENT R_BRACE
                   | L_BRACE LIST_DECLARATION R_BRACE
                   | L_BRACE LIST_DECLARATION LIST_STATEMENT R_BRACE
                   
                   
                   
                   
                   
                   
                   
                   
