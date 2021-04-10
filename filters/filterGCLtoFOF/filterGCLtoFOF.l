%{

#include <stdlib.h>
#include <string.h>

#include "filterGCLtoFOF.tab.h"

%}


%%

	/* Ignore whitespace */

[\t ]+

	/* Comments */
				  
%.*\n	{ }
				  
%.*	{ return '\n'; }

	/* Numbers */

(([0-9]+)|([0-9]*\.[0-9]+)) { 
                              yylval.number = atof(yytext);
                              return NUMBER;
                            }

	/* Strings */

\"[^\"\n]*[\"\n] {

                   yylval.string = strdup(yytext+1);
                   if (yylval.string[yyleng-2] != '"') 
	             printf("Unterminated character string");
                   else yylval.string[yyleng-2] = '\0';
                   return STRING;
                 }

\$[^\$\n]*[\$\n] {

                   yylval.string = strdup(yytext);
                   if (yylval.string[yyleng-1] != '$') 
                     printf("Unterminated character string");
                   return STRING;
                 }

	/* Reserved words */

point		{ return POINT; }

line		{ return LINE; }

circle          { return CIRCLE; /* circle c O B */ }

intersec	{ return INTERSECTION; }

intersec2	{ return INTERSECTION2; }

med             { return MED; /* med l2 A C */ }

mediatrice      { return MEDIATRICE; /* mediatrice mn M N */ }

bis             { return BIS; /* bis l1 A B C */ }

perp            { return PERP; /* perp l3 P ap */ }

foot            { return FOOT; /* foot D A bc */ }

parallel	{ return PARALLEL; /* parallel G F A O */ }

onsegment	{ return ONSEGMENT; }

online		{ return ONLINE; }

oncircle	{ return ONCIRCLE; /* oncircle D O A */ }

bisector        { return BISECTOR; }

prove           { return PROVE; }

pythagoras_difference3 { return PD3; }

equal           { return EQUAL; }

mult            { return MULT; }

sum             { return SUM; }

sratio          { return SRATIO; }

signed_area3    { return SA3; }

signed_area4    { return SA4; }

segment		{ return SEGMENT; } 

angle           { return ANGLE; /* angle O A D */ }

alg_sum_zero3   { return ALGSUMZERO3; }

identical	{ return IDENTICAL; /* identical H Hl */ }

collinear	{ return COLLINEAR; /* collinear A D F */ }

perpendicular	{ return PERPENDICULAR; /*perpendicular O S P Q */ }

midpoint	{ return MIDPOINT; /* midpoint M B C */ }

same_length	{ return SAMELENGTH; /* same_length A E E C */ }

harmonic	{ return HARMONIC; /* harmonic L K F G */ }

cyclic          { return CYCLIC; /* cyclic H Q D P */ }

eqangle         { return EQANGLE; /* eqangle B A M N A C */ }

pbisector       { return PBISECTO; /* pbisector A O O1 */ }

	/* Variables */

[a-zA-Z_][a-zA-Z0-9_']* {  
                           yylval.string=strdup(yytext);
                           return VARIABLE; 
                        }
	/* New line */

\n { return '\n'; }

        /* { and } */

[{}]  { return yytext[0]; }

	/* Anything else */

. { return yytext[0]; }

%%

// equal { angle P A P1 } { angle F1 K F }
