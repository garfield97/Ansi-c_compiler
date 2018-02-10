%option noyywrap

%{
/* Now in a section of C that will be embedded
   into the auto-generated code. Flex will not
   try to interpret code surrounded by %{ ... %} */

/* Bring in our declarations for token types and
   the yylval variable. */
#include "c_parser.tab.hpp"


// This is to work around an irritating bug in Flex
// https://stackoverflow.com/questions/46213840/get-rid-of-warning-implicit-declaration-of-function-fileno-in-flex
extern "C" int fileno(FILE *stream);

/* End the embedded code section. */
%}


Keyword     			auto|double|int|struct|break|else|long|switch|case|enum|register|typedef|char|extern|return|union|const|float|short|unsigned|continue|for|signed|void|default|goto|sizeof|volatile|do|if|static|while
			
Identifier  			[_a-zA-Z][0-9_a-zA-Z]*

Integer_suffix          ([uU][lL]?) | ([lL][uU]?)

Decimal_constant        [1-9][0-9]*[integer_suffix]?

Octal_constant		    [0][0-7]*[integer_suffix]?

Hexadecimal_constant    (0x|0X)[0-9a-fA-F]+[integer_suffix]?

Character_constant      ['][.]+[']

String_literal			["][.]+["]

L_BRACE					[{]

R_BRACE					[}]

L_BRACKET				[(]

R_BRACKET				[)]

L_INDEX					[[]

R_INDEX					[]]

ADD						[+]

MINUS					[-]

MUL						[*]

DIV						[/]

MOD						[%]

EQUAL					[=]

NOT_EQUAL				[!=]

LESS_THAN				[<]

LESS_THAN_ORE			[<=]

MORE_THAN				[>]

MORE_THAN_ORE			[>=]

L_AND					[&&]

L_OR					[||]

B_AND					[&]

B_OR					[|]

B_XOR					[^]

INCLUDE					#[.^\n]

WhiteSpace				[ \n\t]

Other					.


%%

{Keyword}			{ fprintf(stderr, "Keyword : %s\n", yytext);
						toString();	//stored in wordValue
						return Keyword;
					}

{Identifier}		{ fprintf(stderr, "Identifier : %s\n", yytext);
						toString();
						return Identifier; 
					}

{Decimal_constant}  { fprintf(stderr, "Decimal : %s\n", yytext);
						bool u = false, l = false;
						checkIntSuffix(u,l);
						if(!u && !l) getiDecimal();
						if(u && !l) getuDecimal();
						if(!u && l) getlDecimal();
						if(u && l) getluDecimal();
						return Decimal_constant; 
					}

{Octal_constant}  	{ fprintf(stderr, "Octal : %s\n", yytext);
						bool u = false, l = false;
						checkIntSuffix(u,l);
						if(!u && !l) getiOctal();
						if(u && !l) getuOctal();
						if(!u && l) getlOctal();
						if(u && l) getluOctal();
						return Octal_constant; 
					}
					
{Hexadecimal_constant}  	{ fprintf(stderr, "Hexadecimal : %s\n", yytext);
						bool u = false, l = false;
						checkIntSuffix(u,l);
						if(!u && !l) getiHexa();
						if(u && !l) getuHexa();
						if(!u && l) getlHexa();
						if(u && l) getluHexa();
						return Hexadecimal_constant; 
					}

{Character_constant} { fprintf(stderr, "Character : %s\n", yytext);
						getCharacter();
						return Character_constant; 
					}


{WhiteSpace}    { fprintf(stderr, "Newline, tab or space\n"); }


{Other}			{ yyerror(yytext); }


%%

void getuDecimal(){
    
    unsigned int num;
	sscanf(yytext, "%u", &num);
	yylval.uintValue = num;

}

void getiDecimal(){

	int num;

	sscanf(yytext, "%d", &num);

	yylval.intValue = num;

}


void getiOctal(){

    int num;

    sscanf(yytext,"%o", &num);
    
    yylval.intValue = num;
    
}


void getiHexa(){

    int num;

    sscanf(yytext,"%x", &num);
    
    yylval.intValue = num;
}


void getlDecimal(){
   
    long int num;
	sscanf(yytext, "%d", &num);
	yylval.longintValue = num;

}

void getluDecimal(){   

    unsigned long num;
	sscanf(yytext, "%u", &num);
	yylval.longuintValue = num;

}

void getuHexa(){

    unsigned int num;
	sscanf(yytext, "%x", &num);
	yylval.uintValue = num;    

}


void getlHexa(){
    
    long int num;
    sscanf(yytext,"%x",&num);
    yylval.longintValue = num;
    
}
    
void getluHexa(){
    
    unsigned long int num;
    sscanf(yytext,"%x",&num);
    yylval.longuintValue = num;
}
    
void getuOctal(){

    unsigned int num;
    sscanf(yytext,"%o", &num);    
    yylval.uintValue = num;
    
}

void getlOctal(){
 
    long int num;
    sscanf(yytext,"%o", &num);    
    yylval.longintValue = num;

}

void getluOctal(){

    unsigned long num;
    sscanf(yytext,"%o", &num);    
    yylval.longuintValue = num;
    
    
}

void getCharacter(){
    char val;
    sscanf(yytext,"%c", &val);
    yylval.characterValue = val;
}
    

void checkIntSuffix(bool &u_exist, bool &l_exist){
	int size = strlen(yytext);
	if(size < 2){
		return;
	}
	if(yytext[size-1] == ('u' || 'U') || yytext[size-2] == ('u' || 'U')) u_exist = true;
	if(yytext[size-1] == ('l' || 'L') || yytext[size-2] == ('l' || 'L')) l_exist = true;
}


void toString(){
 	std::string *word = new std::string; // take value out of yylex scope
	*word = yytext;
	yylval.wordValue = word;
}

/* Error handler. This will get called if none of the rules match. */
void yyerror (char const *s)
{
  fprintf (stderr, "Flex Error: %s\n", s); /* s is the text that wasn't matched */
}
