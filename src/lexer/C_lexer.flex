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

Character-constant      []

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
						checkIntSuffix(f,l);
						if(!f && !l) getdFloat();
						if(f && !l) getfFloat();
						if(!f && l) getlFloat();
						return Floating-constant;
					}


{WhiteSpace}    { fprintf(stderr, "Newline, tab or space\n"); }


{Other}			{ yyerror(yytext); }


%%

checkIntSuffix(bool &u_exist, bool &l_exist){
void getuDecimal(){
    
    uint num;
	sscanf(yytext, "%u", &num);
	yylval.uintValue = num;

}

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


void getlDecimal(){
   
    long int num;
	sscanf(yytext, "%d", &num);
	yylval.longintValue = num;

}

void getluDecimal(){   

    long uint num;
	sscanf(yytext, "%u", &num);
	yylval.longuintValue = num;

}

void getuHexa(){

    uint num;
	sscanf(yytext, "%x", &num);
	yylval.uintValue = num;    

}


void getlHexa(){
    
    long int num;
    sscanf(yytext,"%x",&num);
    yylval.longintValue = num;
    
}
    
void getluHexa(){
    
    long uint num;
    sscanf(yytext,"%x",&num);
    yylval.longuintValue = num;
}
    
void getuOctal(){

    uint num;
    sscanf(ytext,"%o", &num);    
    yylval.uintValue = num;
    
}

void getlOctal(){
 
    long int num;
    sscanf(ytext,"%o", &num);    
    yylval.longintValue = num;

}

void getluOctal(){

    long uint num;
    sscanf(ytext,"%o", &num);    
    yylval.longuintValue = num;
    
    
}




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






void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();void getdFloat();
void getfFloat();
void getlFloat();

















