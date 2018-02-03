
character-constant      []

%option noyywrap

%{

#include "histogram.hpp"


// This is to work around an irritating bug in Flex
// https://stackoverflow.com/questions/46213840/get-rid-of-warning-implicit-declaration-of-function-fileno-in-flex
extern "C" int fileno(FILE *stream);

/* End the embedded code section. */
%}

Keyword     			auto|double|int|struct|break|else|long|switch|case
						|enum|register|typedef|char|extern|return|union|const
						|float|short|unsigned|continue|for|signed|void|default
						|goto|sizeof|volatile|do|if|static|while
			
Identifier  			[_a-zA-Z][0-9_a-zA-Z]*

Integer-suffix          ([uU][lL]?) | ([lL][uU]?)

Decimal-constant        [1-9][0-9]*[integer-suffix]?

Octal-constant		    [0][0-7]*[integer-suffix]?

Hexadecimal-constant    (0x|0X)[0-9a-fA-F]+[integer-suffix]?

Floating-constant       [+-]?( ([0-9]+[.][0-9]*) | ([.][0-9]+) | ([0-9]+([.][0-9]*)?[eE][+-]?[0-9]+) | ([.][0-9]+[eE][+-]?[0-9]+) )[lfLF]?

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

{Decimal-constant}  { fprintf(stderr, "Decimal : %s\n", yytext);
						bool u = false;, f = false;
						checkSuffix(u,f);
						if(!u && !f) getiDecimal();
						if(u && !f) getuDecimal();
						if(!u && f) getlDecimal();
						if(u && f) getluDecimal();
						return Decimal-constant; 
					}

{Octal-constant}  	{ fprintf(stderr, "Octal : %s\n", yytext);
						bool u = false;, f = false;
						checkSuffix(u,f);
						if(!u && !f) getiOctal();
						if(u && !f) getuOctal();
						if(!u && f) getlOctal();
						if(u && f) getluOctal();
						return Octal-constant; 
					}
					
{Hexadecimal-constant}  	{ fprintf(stderr, "Hexadecimal : %s\n", yytext);
						bool u = false;, f = false;
						checkSuffix(u,f);
						if(!u && !f) getiHexa();
						if(u && !f) getuHexa();
						if(!u && f) getlHexa();
						if(u && f) getluHexa();
						return Hexadecimal-constant; 
					}


{WhiteSpace}    { fprintf(stderr, "Newline, tab or space\n"); }


{Other}			{ yyerror(yytext); }


%%


void getiDecimal(){

	int num

	sscanf(yytext, "%d", &num);

	yylval.intValue = num;

}


void getiOctal(){

    int num;

    sscanf(ytext,"%o", &num);
    
    yylval.intValue = num;
    
}


void getiHexa(){

    int num;

    sscanf(ytext,"%x", &num);
    
    yylval.intValue = num;
}

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



checkSuffix(bool &u_exist, bool &l_exist){
	int size = strlen(yytext);
	if(size < 2){
		return;
	}
	if(yytext[] == ('u' || 'U') || yytext[] == ('u' || 'U')) u_exist = true;
	if(yytext[] == ('l' || 'L') || yytext[] == ('l' || 'L')) l_exist = true;
}


void toString(){
 	std::string *word = new std::string; // take value out of yylex scope
	*word = yytext;
	yylval.wordValue = word;
}

void wordB(){
	// remove brackets from yytext
	yytext[strlen(yytext)-1] = '\0'; //removes ]

	for(unsigned int i=0; i < strlen(yytext); i++){
		yytext[i] = yytext[i+1];
	}	// shift char's

 	std::string *word = new std::string; // take value out of yylex scope
	*word = yytext;
	yylval.wordValue = word;
}

/* Error handler. This will get called if none of the rules match. */
void yyerror (char const *s)
{
  fprintf (stderr, "Flex Error: %s\n", s); /* s is the text that wasn't matched */
}
























