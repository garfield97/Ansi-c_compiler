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

Character-constant      '[.]'

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
						bool u = false;, l = false;
						checkIntSuffix(u,l);
						if(!u && !l) getiDecimal();
						if(u && !l) getuDecimal();
						if(!u && l) getlDecimal();
						if(u && l) getluDecimal();
						return Decimal-constant; 
					}

{Octal-constant}  	{ fprintf(stderr, "Octal : %s\n", yytext);
						bool u = false;, l = false;
						checkIntSuffix(u,l);
						if(!u && !l) getiOctal();
						if(u && !l) getuOctal();
						if(!u && l) getlOctal();
						if(u && l) getluOctal();
						return Octal-constant; 
					}
					
{Hexadecimal-constant}  	{ fprintf(stderr, "Hexadecimal : %s\n", yytext);
						bool u = false;, l = false;
						checkIntSuffix(u,l);
						if(!u && !l) getiHexa();
						if(u && !l) getuHexa();
						if(!u && l) getlHexa();
						if(u && l) getluHexa();
						return Hexadecimal-constant; 
					}
					
{Floating-constant}	{ fprintf(stderr, "Floating : %s\n", yytext);
						bool f = false;, l = false;
						checkFloatSuffix(f,l);
						if(!f && !l) getdFloat();
						if(f && !l) getfFloat();
						if(!f && l) getlFloat();
						return Floating-constant;
					}

{Character-constant} { fprintf(stderr, "Character : %s\n", yytext);
						getCharacter();
						return Character-constant; 
					}


{WhiteSpace}    { fprintf(stderr, "Newline, tab or space\n"); }


{Other}			{ yyerror(yytext); }


%%

void getuDecimal(){
    
    unsigned int num;
	sscanf(yytext, "%u", &num);
	yylval.unsigned intValue = num;

}

void getiDecimal(){

	int num

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
	yylval.longunsigned intValue = num;

}

void getuHexa(){

    unsigned int num;
	sscanf(yytext, "%x", &num);
	yylval.unsigned intValue = num;    

}


void getlHexa(){
    
    long int num;
    sscanf(yytext,"%x",&num);
    yylval.longintValue = num;
    
}
    
void getluHexa(){
    
    unsigned long int num;
    sscanf(yytext,"%x",&num);
    yylval.longunsigned intValue = num;
}
    
void getuOctal(){

    unsigned int num;
    sscanf(yytext,"%o", &num);    
    yylval.unsigned intValue = num;
    
}

void getlOctal(){
 
    long int num;
    sscanf(yytext,"%o", &num);    
    yylval.longintValue = num;

}

void getluOctal(){

    unsigned long num;
    sscanf(yytext,"%o", &num);    
    yylval.longunsigned intValue = num;
    
    
}

void getdFloat(){
    
    double num;
    sscanf(yytext,"%f", &num);    
    yylval.dfloatValue = num;

}
    



void getfFloat(){

    float num;
    sscanf(yytext,"%f", &num);    
    yylval.ffloatValue = num;
}


void getlFloat(){

    long double num;
    sscanf(yytext,"%f", &num);    
    yylval.lfloatValue = num;

}

void getCharacter(){
    char val;
    sscanf(yytext,"%c", &val);
    yylval.characterValue = val;
}
    



checkIntSuffix(bool &u_exist, bool &l_exist){
	int size = strlen(yytext);
	if(size < 2){
		return;
	}
	if(yytext[size-1] == ('u' || 'U') || yytext[size-2] == ('u' || 'U')) u_exist = true;
	if(yytext[size-1] == ('l' || 'L') || yytext[size-2] == ('l' || 'L')) l_exist = true;
}

checkFloatSuffix(bool &f_exist, bool &l_exist){
	int last = strlen(yytext) - 1;
	if(size < 1){
		return;
	}

	if(yytext[last] == ('f' || 'F') ) f_exist = true;
	else if(yytext[last] == ('l' || 'L') l_exist = true;
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























