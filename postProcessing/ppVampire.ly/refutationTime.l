%{ 

#include "refutationTime.tab.h"


%}


numero    [0-9]+"."[0-9]+

%%

"% Termination reason: Refutation" { return(REFUTATION); }


"% Time elapsed: " { return(TIMEELAPSED); }

{numero}   {
             yylval.number=atof(yytext);
             return(NUMERO);
           }


[ \t]          { /* skip white spaces */ }
[\n]           { /* skip newlines */ } 
.              { /* skip everything else */ }
%%

