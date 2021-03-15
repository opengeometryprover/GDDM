%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int yylex(void);
/* int warning(char *); */
int yyerror(char *);

extern FILE *yyin;
extern FILE *yyout;

int refutation=0; // false by default
float timeelapsed;

 
%}

%union {
   float number;
   char *string;
}

%token REFUTATION TIMEELAPSED
%token <number> NUMERO

%%


linha:		'\n'
	|	comando  
        |       linha comando
	|	error '\n' {yyerrok;}
		;


comando:      REFUTATION { refutation=1; }
        |     TIMEELAPSED NUMERO
                 {
		   timeelapsed = $2;
		 }
        |      NUMERO { /* ignore */ }

%%


int main(int argc, char *argv[]) {
  
  yyin = fopen(argv[1],"r");

  yyout = fopen(argv[2],"w");
  
  yyparse();

  if (refutation) {
    printf("%g",timeelapsed);
  }
  else {
    printf("---");
  }

  // finalisa
  fclose(yyin);
  fclose(yyout);
}

int yyerror(char *s) {
  // printf("%s\n",s);
};
