
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

Octal-constants         [0][0-7]*[integer-suffix]?

Hexadecimal-constant    (0x|0X)[0-9a-fA-F]+[integer-suffix]?

Floating-constant       [+-]?( ([0-9]+[.][0-9]*) | ([.][0-9]+) | ([0-9]+([.][0-9]*)?[eE][+-]?[0-9]+) | ([.][0-9]+[eE][+-]?[0-9]+) )[lfLF]?

WhiteSpace	[ \n\t]

Other		.


%%

{Keyword}			{ fprintf(stderr, "Keyword : %s\n", yytext);
						toString();	//stored in wordValue
						return Keyword;
					}

{Identifier}		{ fprintf(stderr, "Identifier : %s\n", yytext);
						toString();
						return Identifier; 
					}

{Decimal-constant}  { fprintf(stderr, "Number : %s\n", yytext);
						
						return Decimal-constant; 
					}

{Octal-constant}  { fprintf(stderr, "Number : %s\n", yytext);
						yylval.numberValue = atof(yytext);
						return Number; 
					}

{Word}			{ fprintf(stderr, "Word : %s\n", yytext); 
					  word();
                  	  return Word; 
               	}

{WordBra} 		{ fprintf(stderr, "Word : %s\n", yytext); 
					  wordB();
                      return Word; 
               	}

{WhiteSpace}    { fprintf(stderr, "Newline, tab or space\n"); }


{Other}			{ yyerror(yytext); }


%%


void getDecimal(){

	int num

	sscanf(yytext, "%d", &num);

	yylval.intValue = num;

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
























