#ifndef histogram_hpp
#define histogram_hpp

#include <string>

enum TokenType{

    None   = 0, // This indicates there are no more tokens
    Keyword,
    Identifier,
    Floating-constant,
    Decimal-constant,
    Octal-constants,
    Hexadecimal-constant,
    Number , // token codes must be positive
    Word      // = 2, implicit from C++ enumeration
};

union TokenValue{
    double numberValue;
    float floatValue;
    std::string *wordValue;
    int intValue;
    uint uintValue;
    long int longintValue;
    long uint longuintValue;
    
    
};

// This is a global variable used to move the
// attribute value from the lexer back to the
// main program.
// By convention it is called yylval, as that is
// the name that will be automatially generated
// by Bison (see next lab).
extern TokenValue yylval;

// This is the lexer function defined by flex. Each
// time it is called, a token type value will be
// returned.
extern int yylex();


// functions used in pattern matching
void toString();
void wordB();
void yyerror (char const *s);
void getHexa();
void getOctal();
void getDecimal();



#endif
