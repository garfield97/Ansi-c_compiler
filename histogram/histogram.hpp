#ifndef histogram_hpp
#define histogram_hpp

#include <string>

enum TokenType{

    None   = 0, // This indicates there are no more tokens
    Keyword,
    Identifier,
    Decimal_constant,
    Octal_constant,
    Hexadecimal_constant,
    Character_constant
};

union TokenValue{
    std::string *wordValue;
    int intValue;
    unsigned int uintValue;
    long int longintValue;
    unsigned long longuintValue;
    char characterValue;

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
void yyerror (char const *s);
void checkIntSuffix(bool &u_exist, bool &l_exist);

void getiDecimal();
void getuDecimal();
void getlDecimal();
void getluDecimal();
void getiHexa();
void getuHexa();
void getlHexa();
void getluHexa();
void getiOctal();
void getuOctal();
void getlOctal();
void getluOctal();

void getCharacter();



#endif
