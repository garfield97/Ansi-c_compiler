%option noyywrap

%{
/* Now in a section of C that will be embedded
   into the auto-generated code. Flex will not
   try to interpret code surrounded by %{ ... %} */

/* Bring in our declarations for token types and
   the yylval variable. */
#include "c_parser.tab.hpp"
#include "c_lexer_helper.hpp"


// This is to work around an irritating bug in Flex
// https://stackoverflow.com/questions/46213840/get-rid-of-warning-implicit-declaration-of-function-fileno-in-flex
extern "C" int fileno(FILE *stream);

/* End the embedded code section. */
%}


KEYWORD     			auto|double|int|struct|break|else|long|switch|case|enum|register|typedef|char|extern|return|union|const|float|short|unsigned|continue|for|signed|void|default|goto|sizeof|volatile|do|if|static|while
			
IDENTIFIER  			[_a-zA-Z][0-9_a-zA-Z]*

INTEGER_SUFFIX          ([uU][lL]?) | ([lL][uU]?)

DECIMAL_CONSTANT        [1-9][0-9]*[integer_suffix]?

OCTAL_CONSTANT		    [0][0-7]*[integer_suffix]?

HEXADECIMAL_CONSTANT    (0x|0X)[0-9a-fA-F]+[integer_suffix]?

CHARACTER_CONSTANT      ['][.]+[']

STRING_LITERAL			["][.]+["]

INCLUDE					#[.^\n]

WHITESPACE				[ \n\t]

OTHER					.


%%

{KEYWORD}			{ fprintf(stderr, "Keyword : %s\n", yytext);
						toString();	//stored in wordValue
						return KEYWORD;
					}

{IDENTIFIER}		{ fprintf(stderr, "Identifier : %s\n", yytext);
						toString();
						return IDENTIFIER; 
					}

{DECIMAL_CONSTANT}  { fprintf(stderr, "Decimal : %s\n", yytext);
						bool u = false, l = false;
						checkIntSuffix(u,l);
						if(!u && !l)	{ get_DECIMAL(); 	return DECIMAL;}
						if(u && !l)		{ get_DECIMAL_U(); 	return DECIMAL_;}
						if(!u && l)		{ get_DECIMAL_L(); 	return DECIMAL_L;}
						if(u && l)		{ get_DECIMAL_UL(); return DECIMAL_UL;}
					}

{OCTAL_CONSTANT}  	{ fprintf(stderr, "Octal : %s\n", yytext);
						bool u = false, l = false;
						checkIntSuffix(u,l);
						if(!u && !l)	{ get_OCTAL(); 		return OCTAL;}
						if(u && !l)		{ get_OCTAL_U(); 	return OCTAL_;}
						if(!u && l)		{ get_OCTAL_L(); 	return OCTAL_L;}
						if(u && l)		{ get_OCTAL_UL(); 	return OCTAL_UL;}
					}
					
{HEXADECIMAL_CONSTANT}  	{ fprintf(stderr, "Hexadecimal : %s\n", yytext);
						bool u = false, l = false;
						checkIntSuffix(u,l);
						if(!u && !l)	{ get_HEXAD(); 		return HEXAD;}
						if(u && !l)		{ get_HEXADL_U(); 	return HEXAD_;}
						if(!u && l)		{ get_HEXAD_L(); 	return HEXAD_L;}
						if(u && l)		{ get_HEXAD_UL(); 	return HEXAD_UL;}
					}

{CHARACTER_CONSTANT} { fprintf(stderr, "Character : %s\n", yytext);
						get_CHARACTER();
						return CHARACTER_CONSTANT; 
					 }

{STRING_LITERAL}	 { fprintf(stderr, "String : %s\n", yytext);
						toString();
						return STRING_LITERAL;
					 }

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

{INCLUDE}		{ fprintf(stderr, "#Include\n"); }

{WHITESPACE}    { fprintf(stderr, "Newline, tab or space\n"); }


{OTHER}			{ yyerror(yytext); }


%%


void toString(){
 	std::string *word = new std::string; // take value out of yylex scope
	*word = yytext;
	yylval.string = word;
}


void checkIntSuffix(bool &u_exist, bool &l_exist){
	int size = strlen(yytext);
	if(size < 2){
		return;
	}
	if(yytext[size-1] == ('u' || 'U') || yytext[size-2] == ('u' || 'U')) u_exist = true;
	if(yytext[size-1] == ('l' || 'L') || yytext[size-2] == ('l' || 'L')) l_exist = true;
}


void get_DECIMAL(){
	int num;
	sscanf(yytext, "%d", &num);
	yylval.intValue = num;
}

void get_DECIMAL_U(){   
    unsigned int num;
	sscanf(yytext, "%u", &num);
	yylval.uintValue = num;
}

void get_DECIMAL_L(){   
    long int num;
	sscanf(yytext, "%d", &num);
	yylval.longintValue = num;
}

void get_DECIMAL_UL(){   
    unsigned long num;
	sscanf(yytext, "%u", &num);
	yylval.longuintValue = num;
}



void get_OCTAL(){
    int num;
    sscanf(yytext,"%o", &num);
    yylval.intValue = num;  
}

void get_OCTAL_U(){
    unsigned int num;
    sscanf(yytext,"%o", &num);    
    yylval.uintValue = num;   
}

void get_OCTAL_L(){
 
    long int num;
    sscanf(yytext,"%o", &num);    
    yylval.longintValue = num;
}

void get_OCTAL_UL(){
    unsigned long num;
    sscanf(yytext,"%o", &num);    
    yylval.longuintValue = num;   
}



void get_HEXA(){
    int num;
    sscanf(yytext,"%x", &num); 
    yylval.intValue = num;
}

void get_HEXA_U(){
    unsigned int num;
	sscanf(yytext, "%x", &num);
	yylval.uintValue = num;    
}

void get_HEXA_L(){   
    long int num;
    sscanf(yytext,"%x",&num);
    yylval.longintValue = num;   
} 
  
void get_HEXA_UL(){
    
    unsigned long int num;
    sscanf(yytext,"%x",&num);
    yylval.longuintValue = num;
}
    


void get_CHARACTER(){
    char val;
    sscanf(yytext,"%c", &val);
    yylval.characterValue = val;
}
    


/* Error handler. This will get called if none of the rules match. */
void yyerror (char const *s)
{
  fprintf (stderr, "Flex Error: %s\n", s); /* s is the text that wasn't matched */
}
