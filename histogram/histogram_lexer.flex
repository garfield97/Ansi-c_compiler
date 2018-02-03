%option noyywrap

%{
/* Now in a section of C that will be embedded
   into the auto-generated code. Flex will not
   try to interpret code surrounded by %{ ... %} */

/* Bring in our declarations for token types and
   the yylval variable. */
#include "histogram.hpp"


// This is to work around an irritating bug in Flex
// https://stackoverflow.com/questions/46213840/get-rid-of-warning-implicit-declaration-of-function-fileno-in-flex
extern "C" int fileno(FILE *stream);

/* End the embedded code section. */
%}



Fraction	[-]?[0-9]+[/][0-9]+

DecimalI	[-]?[0-9]+

DecimalF	[-]?[0-9]+[.][0-9]*

Word		[a-zA-Z]+

WordBra		[[][^]\n]*[]] 

WhiteSpace	[ \n\t]

hexadecimal-constant    (0x|0X)[0-9a-fA-F]+[integer-suffix]?

octal-constants         [0][0-7]*[integer-suffix]?

decimal-constant        [1-9][0-9]*[integer-suffix]?

integer-suffix          ([uU][lL]?) | ([lL][uU]?)



Other		.


%%

{Fraction}		{ fprintf(stderr, "Number : %s\n", yytext);
                      calcFraction();
                      return Number; 
                }

{DecimalI}      { fprintf(stderr, "Number : %s\n", yytext);
                      yylval.numberValue = atof(yytext);
                      return Number; 
                }

{DecimalF}		{ fprintf(stderr, "Number : %s\n", yytext);
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


void calcFraction(){

	int num, den;

	sscanf(yytext, "%d/%d", &num, &den);

	yylval.numberValue = double(num) / double(den);

}

void word(){
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
