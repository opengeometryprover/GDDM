%option noyywrap

%{

#include <iostream>
#include <string>

#include "parser.h"

%}

%%

":"                     { return COLON; }
","                     { return COMMA; }
"&"                     { return CONJ; }
"fof"                   { return FOF; }
"!"                     { return FORALL; }
"."                     { return FULLSTOP; }
"["                     { return LBRKT; }
"("                     { return LPAREN; }
"]"                     { return RBRKT; }
")"                     { return RPAREN; }
"=>"                    { return STXCONS; }

"circle"                { return CIRCLE; }
"coll"                  { return COLL; }
"cong"                  { return CONG; }
"contri"                { return CONTRI; }
"cyclic"                { return CYCLIC; }
"eqangle"               { return EQANGLE; }
"eqratio"               { return EQRATIO; }
"midp"                  { return MIDP; }
"para"                  { return PARA; }
"perp"                  { return PERP; }
"simtri"                { return SIMTRI; }


[A-Za-z][A-Za-z0-9]*    { yylval = yytext; return ID; }
[ \t\r\n]               { }

.                       { std::cout << "Unknown character!" << std::endl; }

%%
