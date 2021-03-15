%{
/* Livro C - 1988_KernighanRitchie_TheCProgrammingLanguage2ed.pdf
 *                                 
 * Para compilar                    
 * i) Bison                         
 * > bison -d .y
 *                                  
 * O que produz                     
 * ".tab.c" e ".tab.h" 
 * ii) Flex            
 * > flex -4.l 
 *                     
 * O que produz                     
 *                                  
 * "lex.yy.c"                       
 * iii) gcc                         
 *                                  
 * gcc lex.yy.c .tab.c -lfl -lm -o 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int yylex(void);
/* int warning(char *); */
int yyerror(char *);

void oncircle(char *);
void reflection(char *,char *,char *);
void upstr(char *);
void cleanName(char *);

 
extern int yylex();
extern FILE *yyin;
extern FILE *yyout;

typedef struct idVariables {
  char variable[20];
} IdVariables;

IdVariables listIdVariables[1000];
int numIdVariables=0;

typedef struct propositions {
  char proposition[150];
} Propositions;

char proposition[600];
char newProposition[600];

Propositions listPropositions[1000];
int numPropositions=0;
int numHypotheses=0;

int numMedPoints=1;
char midpointsIds[20];
char newPointsIds1[20],newPointsIds2[20];
  
char fileName[60];
char fullFileName[60];
char outputFileName[60];
char conjectureName[60];
// to be able to convert from ASCI string to integer
char numero[10];
 
// variáveis auxiliares
int mpi,aux;

 
%}

%union {
   int number;
   char *string;
}

%token POINT MIDPOINT FOOT PERPENDICULAR ONLINE LINE
%token CIRCLE ONCIRCLE ONRCIRCLE CYCLIC RECTANGLE
%token REFLECTION ORTHOCENTER CIRCUMCENTER ONTLINE
%token PARALLELOGRAM QUADRANGLE  COLLINEAR PARALLEL LCTANGENT CONTRIANGLE
%token TRIANGLE RTRIANGLE PETRIANGLE SIMTRIANGLE ISOTRIANGLE EQTRIANGLE
%token SQUARE NSQUARE PSQUARE
%token EQDISTANCE ONBLINE ONPLINE ONALINE 
%token ANGLEBISECTOR INCENTER LRATIO PRATIO TRATIO RATIO PENTAGON 
%token INTERSECTIONCC INTERSECTIONLC INTERSECTIONLL INTERSECTIONLT  INTERSECTIONPP
%token SANGLE EQANGLE RTRAPEZOID TRAPEZOID SYM
%token SEPARADOR SHOW NOSHOW FINALIZA
%token <string> VARIABLE 
%token <number> NUMERO

%%

linha:		'\n'
	|	comando  
        |       linha comando
	|	error '\n' {yyerrok;}
		;

comando: 	POINT variaveis SEPARADOR
                  { /* Builds the initial list of points */ }
        |       MIDPOINT VARIABLE VARIABLE VARIABLE
                  {
		    upstr($2);upstr($3);upstr($4);
		    strcpy(proposition,"midp(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);		    
		  }
        |       FOOT VARIABLE VARIABLE VARIABLE VARIABLE
                  {
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    strcpy(proposition,"\nperp(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,") & coll(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);		    
		  }
        |       PERPENDICULAR VARIABLE VARIABLE VARIABLE VARIABLE
                  {
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    strcpy(proposition,"\nperp(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);		    
		  }
        |       ONLINE VARIABLE VARIABLE VARIABLE
                  {
		    upstr($2);upstr($3);upstr($4);
		    strcpy(proposition,"\ncoll(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);		    
		  }
        |       ONRCIRCLE VARIABLE VARIABLE VARIABLE VARIABLE
                  { /* circle E O A with EO _|_ AO */
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    newPointsIds1[0]='\0';
		    strcpy(newPointsIds1,"NWPNT");
		    mpi=5;
		    aux=numMedPoints;
		     do {
		      newPointsIds1[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds1);
		    strcpy(proposition,"\ncircle(");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,") & perp(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);		    
		  }
        |       CYCLIC VARIABLE VARIABLE VARIABLE VARIABLE
                  {		   
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    strcpy(proposition,"\ncyclic(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);		 
		  }
        |       ONCIRCLE VARIABLE VARIABLE VARIABLE
                  {
		    /* oncircle v1 v2 v3 -> circle(v2,v3,v1,n1) */
		    // one new point (uses the midpoint counter)
		    upstr($2);upstr($3);upstr($4);
		    newPointsIds1[0]='\0';
		    strcpy(newPointsIds1,"NWPNT");
		    mpi=5;
		    aux=numMedPoints;
		     do {
		      newPointsIds1[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds1);
		    // building the proposition circle
		    strcpy(proposition,"\ncircle(");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       ONTLINE VARIABLE VARIABLE VARIABLE VARIABLE
                  {
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    strcpy(proposition,"\nperp(");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       ONPLINE VARIABLE VARIABLE VARIABLE VARIABLE
                  {
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    strcpy(proposition,"\npara(");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       ONBLINE VARIABLE VARIABLE VARIABLE
                  { /* Point on Bisection (bisectriz) 
		     * onbline C A B =
		     *   midpoint(M,A,B) & perp(A,B,M,C)
		     */
		    upstr($2);upstr($3);upstr($4);
		    // one new point (uses the midpoint counter)
		    upstr($2);upstr($3);upstr($4);
		    newPointsIds1[0]='\0';
		    strcpy(newPointsIds1,"NWPNT");
		    mpi=5;
		    aux=numMedPoints;
		     do {
		      newPointsIds1[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds1);
		    // Building the command
		    strcpy(proposition,"\nmidpoint(");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,") & perp(");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	

		  } 
        |       ONALINE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE
                  {
		    upstr($2);upstr($3);upstr($4);upstr($5);upstr($6);upstr($7);
		    strcpy(proposition,"\neqangle(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$6);
		    strcat(proposition,",");
		    strcat(proposition,$6);
		    strcat(proposition,",");
		    strcat(proposition,$7);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       circle
        |       COLLINEAR VARIABLE VARIABLE VARIABLE
                 { 
		    upstr($2);upstr($3);upstr($4);
		    upstr($2);upstr($3);upstr($4);
		    strcpy(proposition,"\ncoll(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       SHOW {
		       numHypotheses=numPropositions;
		     } 
        |       NOSHOW {
	                 numHypotheses=0;
		       }
        |       TRIANGLE VARIABLE VARIABLE VARIABLE
                  { /* not relevant */ }
        |       QUADRANGLE VARIABLE VARIABLE VARIABLE VARIABLE
                  { /* ignore */ }
        |       PARALLELOGRAM VARIABLE VARIABLE VARIABLE VARIABLE
                  {
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    strcpy(proposition,"\npara(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,") & para(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       SQUARE VARIABLE VARIABLE VARIABLE VARIABLE
                  { /*
		     * Square, two consecutive equal angles and the
		     * corresponding sides equal in length
		     * SQUARE A B C D =
		     *  eqangle(B,A,A,D,A,D,D,C) &
		     *  cong(A,D,D,C) 
		     */
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    strcpy(proposition,"\neqangle(");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,") & cong(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       PSQUARE VARIABLE VARIABLE VARIABLE 
                  { /*
		     * PSQUARE A B C =
		     *  AB || CD & AD || BC & AB=BC & AB _|_ BC
		     */
		    upstr($2);upstr($3);upstr($4);
		    // one new point (uses the midpoint counter)
		    upstr($2);upstr($3);upstr($4);
		    newPointsIds1[0]='\0';
		    strcpy(newPointsIds1,"NWPNT");
		    mpi=5;
		    aux=numMedPoints;
		     do {
		      newPointsIds1[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds1);
		    // Building the command
		    strcpy(proposition,"\npara(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,") & para(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,") & cong(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,") & perp(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       NSQUARE VARIABLE VARIABLE VARIABLE
                  { /*
		     * NSQUARE A B C =
		     *  AB || CD & AD || BC & AB=BC & AB _|_ BC
		     */
		    upstr($2);upstr($3);upstr($4);
		    // one new point (uses the midpoint counter)
		    upstr($2);upstr($3);upstr($4);
		    newPointsIds1[0]='\0';
		    strcpy(newPointsIds1,"NWPNT");
		    mpi=5;
		    aux=numMedPoints;
		     do {
		      newPointsIds1[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds1);
		    // Building the command
		    strcpy(proposition,"\npara(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,") & para(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,") & cong(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,") & perp(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);
		  }
        |       PENTAGON VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE
                  { /* Ignore */ }
        |       RTRIANGLE VARIABLE VARIABLE VARIABLE
                  { /* Triangle rectangle in the 1st point */
		    upstr($2);upstr($3);upstr($4);
		    strcpy(proposition,"\nperp(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       PETRIANGLE VARIABLE VARIABLE VARIABLE
                  { /* Triangle ??? - ignore */
		  }
        |       ISOTRIANGLE VARIABLE VARIABLE VARIABLE
                  { /*
		     * Isosceles Triangle - two congruente sides
		     *  ISOTRIANGLE A B C =
		     *   cong(A,B,B,C) | cong(A,B,A,C)
		     */
		    upstr($2);upstr($3);upstr($4);
		    strcpy(proposition,"\n(cong(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,") | cong(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,"))");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       REFLECTION VARIABLE VARIABLE VARIABLE
                  {	
		    upstr($2);upstr($3);upstr($4);	 
		    reflection($2,$3,$4);
		    strcpy(listPropositions[numPropositions++].proposition,newProposition);
		  }
        |       ORTHOCENTER VARIABLE VARIABLE VARIABLE VARIABLE
                  {
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    strcpy(proposition,"\nperp(");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,") & perp(");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,") & perp(");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       CIRCUMCENTER VARIABLE VARIABLE VARIABLE VARIABLE
                  {
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    
		    // Builds the new midpoint Id
		    strcpy(midpointsIds,"MIDPNT");
		    mpi=6;
		    aux=numMedPoints;
		     do {
		      midpointsIds[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    // Add a new quantified variable
		    strcpy(listIdVariables[numIdVariables++].variable,midpointsIds);
		    // builds the midp & perp for that point
		    strcpy(proposition,"\nmidp(");
		    strcat(proposition,midpointsIds);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,") & perp(");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,midpointsIds);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,")");
		    // add that to the list of propositions
		    strcpy(listPropositions[numPropositions++].proposition,proposition);
		    // Builds the new midpoint Id
		    strcpy(midpointsIds,"MIDPNT");
		    mpi=6;
		    aux=numMedPoints;
		     do {
		      midpointsIds[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    // Add a new quantified variable
		    strcpy(listIdVariables[numIdVariables++].variable,midpointsIds);
		    // builds the midp & perp for that point
		    strcpy(proposition,"midp(");
		    strcat(proposition,midpointsIds);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,") & perp(");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,midpointsIds);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,")");
		    // add that to the list of propositions
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		    // Builds the new midpoint Id
		    strcpy(midpointsIds,"MIDPNT");
		    mpi=6;
		    aux=numMedPoints;
		     do {
		      midpointsIds[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    // Add a new quantified variable
		    strcpy(listIdVariables[numIdVariables++].variable,midpointsIds);
		    // builds the midp & perp for that point
		    strcpy(proposition,"midp(");
		    strcat(proposition,midpointsIds);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,") & perp(");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,midpointsIds);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,")");
		    // add that to the list of propositions
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       INCENTER VARIABLE VARIABLE VARIABLE VARIABLE
                  { /*
		     * INCENTER I A B C =
		     * eqangle(I,A,A,B) & eqangle(I,B,B,C) & eqangle(I,C,C,A)
		     */
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    strcpy(proposition,"\neqangle(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,") & eqangle(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,") & eqangle(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       SANGLE VARIABLE VARIABLE VARIABLE NUMERO
                  {
		    upstr($2);upstr($3);upstr($4);
		    strcpy(proposition,"\nangle(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    sprintf(numero,"%d",$5);
		    strcat(proposition,numero);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		    //fprintf(yyout,"eqangle(%s,%s,%s,%s,%s,%s) &",$2,$3,$4,$5,$6,$7);
		  }
        |       EQANGLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE
                  {
		    upstr($2);upstr($3);upstr($4);upstr($5);upstr($6);upstr($7);upstr($8);upstr($9);
		    strcpy(proposition,"\neqangle(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$6);
		    strcat(proposition,",");
		    strcat(proposition,$7);
		    strcat(proposition,",");
		    strcat(proposition,$8);
		    strcat(proposition,",");
		    strcat(proposition,$9);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       EQANGLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE
                  { /* EQANGLE A B C D E F = EQANGLE A B B C D E E F */
		    upstr($2);upstr($3);upstr($4);upstr($5);upstr($6);upstr($7);
		    strcpy(proposition,"\neqangle(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$6);
		    strcat(proposition,",");
		    strcat(proposition,$6);
		    strcat(proposition,",");
		    strcat(proposition,$7);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       RTRAPEZOID VARIABLE VARIABLE VARIABLE VARIABLE
                  { /* Trapezoid A B C D, AB _|_ AD AB || CD */
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    strcpy(proposition,"\nperp(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,") & para(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       TRAPEZOID VARIABLE VARIABLE VARIABLE VARIABLE
                  { /* Trapezoid A B C D, AB || CD */
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    strcat(proposition,"\npara(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       RECTANGLE VARIABLE VARIABLE VARIABLE VARIABLE
                  { /* Trapezoid A B C D, AB || CD & AD || BC */
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    strcat(proposition,"\npara(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,") & para(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       ANGLEBISECTOR VARIABLE VARIABLE VARIABLE VARIABLE
                  { /*
		      ANGLE_BISECTOR E C D A = eqangle(A,D,D,E,E,D,D,C)
		    */
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    strcpy(proposition,"\neqangle(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       INTERSECTIONLL VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE
                  { /* Intersection Line Line */

		    upstr($2);upstr($3);upstr($4);upstr($5);upstr($6);
		    strcpy(proposition,"\ncoll(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,") & coll(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$6);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       INTERSECTIONLT VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE
                  { /* Intersection E C D A A B 
		     * point E is in line CD
		     * 
		     */

		    upstr($2);upstr($3);upstr($4);upstr($5);upstr($6);
		    strcpy(proposition,"\ncoll(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,") & coll(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$6);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       INTERSECTIONLC VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE
                  { /* Intersection Line Circle */

		    upstr($2);upstr($3);upstr($4);upstr($5);upstr($6);
		    newPointsIds1[0]='\0';
		    strcpy(newPointsIds1,"NWPNT");
		    mpi=5          ;
		    aux=numMedPoints;
		     do {
		      newPointsIds1[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds1);
		    strcpy(proposition,"\ncoll(");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,") & circle(");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$6);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       INTERSECTIONCC VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE
                  { /* intersection Circle Circle */

		    upstr($2);upstr($3);upstr($4);upstr($5);upstr($6);
		    newPointsIds1[0]='\0';
		    strcpy(newPointsIds1,"NWPNT");
		    mpi=5          ;
		    aux=numMedPoints;
		     do {
		      newPointsIds1[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds1);
		    newPointsIds2[0]='\0';
		    strcpy(newPointsIds2,"NWPNT");
		    mpi=5          ;
		    aux=numMedPoints;
		     do {
		      newPointsIds2[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds2);
		    strcpy(proposition,"\ncircle(");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,") & circle(");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds2);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       INTERSECTIONPP VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE
                  { /* Intersection of two parallel lines to form a rectangle */

		    upstr($2);upstr($3);upstr($4);upstr($5);upstr($6);upstr($7);upstr($8);
		    strcpy(proposition,"\npara(");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,") & para(");
		    strcat(proposition,$7);
		    strcat(proposition,",");
		    strcat(proposition,$8);
		    strcat(proposition,",");
		    strcat(proposition,$6);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       PARALLEL VARIABLE VARIABLE VARIABLE VARIABLE 
                  {
		    strcpy(proposition,"\npara(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       SIMTRIANGLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE
                  { /* Simetric triangles */
		    upstr($2);upstr($3);upstr($4);upstr($5);upstr($6);upstr($7);
		    strcpy(proposition,"\neqangle(");
		    // <A = <1 & <B = <2
		    strcat(proposition,$3);strcat(proposition,",");
		    strcat(proposition,$2);strcat(proposition,",");
		    strcat(proposition,$2);strcat(proposition,",");
		    strcat(proposition,$4);strcat(proposition,",");
		    strcat(proposition,$6);strcat(proposition,",");
		    strcat(proposition,$5);strcat(proposition,",");
		    strcat(proposition,$5);strcat(proposition,",");
		    strcat(proposition,$7);strcat(proposition,") & (eqangle(");
	    	    strcat(proposition,$2);strcat(proposition,",");
		    strcat(proposition,$3);strcat(proposition,",");
		    strcat(proposition,$3);strcat(proposition,",");
		    strcat(proposition,$4);strcat(proposition,",");
		    strcat(proposition,$5);strcat(proposition,",");
		    strcat(proposition,$6);strcat(proposition,",");
		    strcat(proposition,$6);strcat(proposition,",");
		    strcat(proposition,$7);strcat(proposition,") |\neqangle(");
		    // <A = <1 & <B = <3
		    strcat(proposition,$3);strcat(proposition,",");
		    strcat(proposition,$2);strcat(proposition,",");
		    strcat(proposition,$2);strcat(proposition,",");
		    strcat(proposition,$4);strcat(proposition,",");
		    strcat(proposition,$6);strcat(proposition,",");
		    strcat(proposition,$5);strcat(proposition,",");
		    strcat(proposition,$5);strcat(proposition,",");
		    strcat(proposition,$7);strcat(proposition,")) & (eqangle(");
	    	    strcat(proposition,$2);strcat(proposition,",");
		    strcat(proposition,$3);strcat(proposition,",");
		    strcat(proposition,$3);strcat(proposition,",");
		    strcat(proposition,$4);strcat(proposition,",");
		    strcat(proposition,$5);strcat(proposition,",");
		    strcat(proposition,$7);strcat(proposition,",");
		    strcat(proposition,$7);strcat(proposition,",");
		    strcat(proposition,$6);strcat(proposition,") |\neqangle(");
		    // <A = <2 & <B = <1
		    strcat(proposition,$3);strcat(proposition,",");
		    strcat(proposition,$2);strcat(proposition,",");
		    strcat(proposition,$2);strcat(proposition,",");
		    strcat(proposition,$4);strcat(proposition,",");
		    strcat(proposition,$5);strcat(proposition,",");
		    strcat(proposition,$6);strcat(proposition,",");
		    strcat(proposition,$6);strcat(proposition,",");
		    strcat(proposition,$7);strcat(proposition,")) & (eqangle(");
	    	    strcat(proposition,$2);strcat(proposition,",");
		    strcat(proposition,$3);strcat(proposition,",");
		    strcat(proposition,$3);strcat(proposition,",");
		    strcat(proposition,$4);strcat(proposition,",");
		    strcat(proposition,$6);strcat(proposition,",");
		    strcat(proposition,$5);strcat(proposition,",");
		    strcat(proposition,$5);strcat(proposition,",");
		    strcat(proposition,$7);strcat(proposition,") |\neqangle(");
		    // <A = <2 & <B = <3
		    strcat(proposition,$3);strcat(proposition,",");
		    strcat(proposition,$2);strcat(proposition,",");
		    strcat(proposition,$2);strcat(proposition,",");
		    strcat(proposition,$4);strcat(proposition,",");
		    strcat(proposition,$5);strcat(proposition,",");
		    strcat(proposition,$6);strcat(proposition,",");
		    strcat(proposition,$6);strcat(proposition,",");
		    strcat(proposition,$7);strcat(proposition,")) & (eqangle(");
	    	    strcat(proposition,$2);strcat(proposition,",");
		    strcat(proposition,$3);strcat(proposition,",");
		    strcat(proposition,$3);strcat(proposition,",");
		    strcat(proposition,$4);strcat(proposition,",");
		    strcat(proposition,$5);strcat(proposition,",");
		    strcat(proposition,$7);strcat(proposition,",");
		    strcat(proposition,$7);strcat(proposition,",");
		    strcat(proposition,$6);strcat(proposition,") |\neqangle(");
		    // <A = <3 ; <B = <1
		    strcat(proposition,$3);strcat(proposition,",");
		    strcat(proposition,$2);strcat(proposition,",");
		    strcat(proposition,$2);strcat(proposition,",");
		    strcat(proposition,$4);strcat(proposition,",");
		    strcat(proposition,$5);strcat(proposition,",");
		    strcat(proposition,$7);strcat(proposition,",");
		    strcat(proposition,$7);strcat(proposition,",");
		    strcat(proposition,$6);strcat(proposition,")) & (eqangle(");
	    	    strcat(proposition,$2);strcat(proposition,",");
		    strcat(proposition,$3);strcat(proposition,",");
		    strcat(proposition,$3);strcat(proposition,",");
		    strcat(proposition,$4);strcat(proposition,",");
		    strcat(proposition,$6);strcat(proposition,",");
		    strcat(proposition,$5);strcat(proposition,",");
		    strcat(proposition,$5);strcat(proposition,",");
		    strcat(proposition,$7);strcat(proposition,") |\neqangle(");
		    // <A = <3 ; <B = <2
		    strcat(proposition,$3);strcat(proposition,",");
		    strcat(proposition,$2);strcat(proposition,",");
		    strcat(proposition,$2);strcat(proposition,",");
		    strcat(proposition,$4);strcat(proposition,",");
		    strcat(proposition,$5);strcat(proposition,",");
		    strcat(proposition,$7);strcat(proposition,",");
		    strcat(proposition,$7);strcat(proposition,",");
		    strcat(proposition,$6);strcat(proposition,")) & eqangle(");
	    	    strcat(proposition,$2);strcat(proposition,",");
		    strcat(proposition,$3);strcat(proposition,",");
		    strcat(proposition,$3);strcat(proposition,",");
		    strcat(proposition,$4);strcat(proposition,",");
		    strcat(proposition,$5);strcat(proposition,",");
		    strcat(proposition,$6);strcat(proposition,",");
		    strcat(proposition,$6);strcat(proposition,",");
		    strcat(proposition,$7);strcat(proposition,")"); 
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       EQTRIANGLE VARIABLE VARIABLE VARIABLE 
                  { /*
		     * Equilateral triangle
		     * EQ_TRIANGLE C A B 
		     *  = eqangle(B,A,A,C,A,B,B,C) & eqangle(A,B,B,C,A,C,C,B)
		     */
		    upstr($2);upstr($3);upstr($4);
		    strcpy(proposition,"\neqangle(");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,") & eqangle(");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       CONTRIANGLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE 
                  { /*
		     * Congruence of triangles
		     * CONTRIANGLE A B C D E F
		     *  = contri(A,B,C,D,E,F)
		     */
		    upstr($2);upstr($3);upstr($4);
		    upstr($5);upstr($6);upstr($7);
		    strcpy(proposition,"\ncontri(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,",");
		    strcat(proposition,$6);
		    strcat(proposition,",");
		    strcat(proposition,$7);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       EQDISTANCE VARIABLE VARIABLE VARIABLE VARIABLE
                  {
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    strcpy(proposition,"\ncong(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       LCTANGENT VARIABLE VARIABLE VARIABLE 
                  {
		    upstr($2);upstr($3);upstr($4);
		    strcpy(proposition,"\nperp(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       LINE VARIABLE VARIABLE
                  { /* not relevant */ }
        |       LRATIO VARIABLE VARIABLE VARIABLE VARIABLE NUMERO NUMERO
                  { /* not relevant ??? */
		    char numero[10];
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    strcpy(proposition,"\nratio(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,") = ");
		    sprintf(numero,"%d",$6);
		    strcat(proposition,numero);
		    strcat(proposition,"/");
		    sprintf(numero,"%d", $7);
		    strcat(proposition,numero);
		    strcpy(listPropositions[numPropositions++].proposition,proposition); }
        |       PRATIO VARIABLE VARIABLE VARIABLE VARIABLE NUMERO NUMERO
                  { /* not relevant ??? */
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    strcpy(proposition,"\nratio(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,") = ");
		    sprintf (numero,"%d",$6);
		    strcat(proposition,numero);
		    strcat(proposition,"/");
		    //		    strcat(proposition,$7);
		    sprintf(numero,"%d", $7);
		    strcat(proposition,numero);
		    strcpy(listPropositions[numPropositions++].proposition,proposition); }
        |       RATIO VARIABLE VARIABLE VARIABLE VARIABLE NUMERO NUMERO
                  { /* AB/CD = n1/n2 */
		    char numero[10];
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    strcpy(proposition,"\nratio(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,") = ");
		    sprintf(numero,"%d",$6);
		    strcat(proposition,numero);
		    strcat(proposition,"/");
		    sprintf(numero,"%d", $7);
		    strcat(proposition,numero);
		    strcpy(listPropositions[numPropositions++].proposition,proposition);
		  }
        |       FINALIZA { /* */ }
        |       VARIABLE { fprintf(yyout,"%s ",$1); }
	|	NUMERO
        |       SEPARADOR 
		;

circle:         CIRCLE VARIABLE VARIABLE
                  { /* circle centre point -> circle center point point point */
		    // two new points (uses the midpoint counter)
		    upstr($2);upstr($3);
		    newPointsIds1[0]='\0';
		    newPointsIds2[0]='\0';
		    strcpy(newPointsIds1,"NWPNT");
		    mpi=5          ;
		    aux=numMedPoints;
		     do {
		      newPointsIds1[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds1);
		    strcpy(newPointsIds2,"NWPNT");
		    mpi=5          ;
		    aux=numMedPoints;
		     do {
		      newPointsIds2[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    // adding the two new points to the list of points
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds2);
		    // building the proposition circle
		    strcpy(proposition,"\ncircle(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds1);
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds2);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       CIRCLE VARIABLE VARIABLE VARIABLE
                  {/* circle centre point point 
                      -> circle center point point point */
		    // one new point (uses the midpoint counter)
		    
		    upstr($2);upstr($3);upstr($4);
		    newPointsIds1[0]='\0';
		    strcpy(newPointsIds1,"NWPNT");
		    mpi=5          ;
		    aux=numMedPoints;
		     do {
		      newPointsIds1[mpi++] = (aux % 10) + '0';
		    } while ((aux=aux/10) != 0);
		    numMedPoints++;
		    // adding the new point to the list of points
		    strcpy(listIdVariables[numIdVariables++].variable,newPointsIds1);
		    // building the proposition circle
		    strcpy(proposition,"\ncircle(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,newPointsIds2);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);			    
		  }
        |       CIRCLE VARIABLE VARIABLE VARIABLE VARIABLE
                  {
		    upstr($2);upstr($3);upstr($4);upstr($5);
		    strcpy(proposition,"circle(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$5);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);	
		  }
        |       SYM VARIABLE VARIABLE VARIABLE
                  { /* SYM C B A = coll(C,B,A) & cong(A,B,A,C) */
		    upstr($2);upstr($3);upstr($4);
		    strcat(proposition,"\ncoll(");
		    strcat(proposition,$2);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,") & cong(");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,",");
		    strcat(proposition,$4);
		    strcat(proposition,",");
		    strcat(proposition,$3);
		    strcat(proposition,")");
		    strcpy(listPropositions[numPropositions++].proposition,proposition);
		  }


variaveis:      VARIABLE
                 {
		   upstr($1);
		   strcpy(listIdVariables[numIdVariables++].variable,$1);
		 }
          |     VARIABLE
	         {
		   upstr($1);
		   strcpy(listIdVariables[numIdVariables++].variable,$1);
		 }
                variaveis 
             	;

%%


int main(int argc, char *argv[]) {
  
  if (argc < 2) {
    printf("Error: usage ./jgexToTPTP <full-name-of-a-JGEX-file>\n\n");
    return(1);
  }

  
  strncpy(fileName,argv[1],strlen(argv[1])-4);
  strcpy(fullFileName,argv[1]);
  
  yyin = fopen(fullFileName,"r");

  strcpy(outputFileName,fileName);
  strcpy(conjectureName,fileName);
 
  cleanName(conjectureName);

  strcat(outputFileName,".p");

  yyout = fopen(outputFileName,"w");
  
  yyparse();

  /*
    If there isn't a 'SHOW' statement, the .gex file is not a
    conjecture, so the convertion to TPTP format is not done
   */
  if (numHypotheses==0) {

    char command[50];
    strcpy(command, "rm -f ");
    strcat(command,outputFileName);
    system(command);
    return(1);   // error code, not a conjecture
  }
  // if numHypotheses > 0 then proceed

  
  // Include the Axions
  fprintf(yyout,"\n%%----Include Geometry Deductive Database Method axioms\ninclude('geometryDeductiveDatabaseMethod.ax').\n\nfof(exemplo%s,conjecture,( ! [ ",conjectureName);
  // Universal quantified Points
  for (int i=0;i<numIdVariables;i++) {
    fprintf(yyout,"%s",listIdVariables[i].variable);
    if (i<numIdVariables-1) {
      fprintf(yyout,",");
    }
    else {
      fprintf(yyout," ");
    }
  }
  
  fprintf(yyout,"] :\n");
  
  // Parenthesis embracing all the conjecture
  fprintf(yyout,"(");
  // Parenthesis embracing the hypothesis
  fprintf(yyout,"(");
  for (int i=0;i<numHypotheses;i++) {
    fprintf(yyout,"%s",listPropositions[i].proposition);
    if (i<numHypotheses-1) {
      fprintf(yyout," & ");
    }
    else {
      fprintf(yyout,"\n");
    }
  }
  // Closing the parenthesis in the hypothesis and opening it in the conclusions 
  fprintf(yyout,")\n=>\n(");
  for (int i=numHypotheses;i<numPropositions;i++) {
    fprintf(yyout,"%s",listPropositions[i].proposition);
    if (i<numPropositions-1) {
      fprintf(yyout," & ");
    }
    else {
      fprintf(yyout,"\n");
    }
  }
  // Close the parenthesis on the conclusions
  fprintf(yyout,")\n");
  // Close the parenthesis on the conjecture
  fprintf(yyout,")\n");
  // Close all
  fprintf(yyout,")).");
  
  fclose(yyout);
}

int yyerror(char *s) {
  // printf("%s\n",s);
};

/*
 * Given that the filename will be used as part of the name of the
 * conjecture in the fof statment it must obey to TPTP rules. It
 * cannot contain other symbols not letters and/or digits
 * --> conjectureName
 * <-- conjectureName (without non-letters and non-digits symbols
 */
void cleanName(char conjectureName[]){
  char aux[60];
  int pos=0,posAux=0;
  int fileNameLength=strlen(conjectureName);

  while (pos < fileNameLength) {
    if (conjectureName[pos] >= 'a' && conjectureName[pos] <= 'z' ||
	conjectureName[pos] >= 'A' && conjectureName[pos] <= 'Z' ||
	conjectureName[pos] >= '0' && conjectureName[pos] <= '9') {
      aux[posAux++] = conjectureName[pos];
    }
    pos++;
  }
  aux[posAux]='\0';
  strcpy(conjectureName,aux);
}


void oncircle(char arg[]){
  char arg1[6],arg2[6],arg3[6];
  int j,k;

  for (int i=0;i<numPropositions;i++) {
    if (!strncmp(listPropositions[i].proposition,"circle(",7)){
      j=0;
      k=0;
      while(listPropositions[i].proposition[7+k] != ','){
	arg1[j++]=listPropositions[i].proposition[7+k++];
      }
      arg1[j]='\0';
      j=0;k++;
      while(listPropositions[i].proposition[7+k] != ','){
	arg2[j++]=listPropositions[i].proposition[7+k++];
      }
      arg2[j]='\0';
      j=0;k++;
      while(listPropositions[i].proposition[7+k] != ','){
	arg3[j++]=listPropositions[i].proposition[7+k++];
      }
      arg3[j]='\0';

      //      printf("--- %s,%s,%s,%s ---",arg,arg1,arg2,arg3);
      newProposition[0]='\0';
      strncat(newProposition,listPropositions[i].proposition,7);
      strcat(newProposition,arg1);
      strcat(newProposition,",");
      strcat(newProposition,arg2);
      strcat(newProposition,",");
      strcat(newProposition,arg3);
      strcat(newProposition,",");
      strcat(newProposition,arg);
      strcat(newProposition,")");
    }
  }
}


void reflection(char arg1[],char arg2[],char arg3[]) {
  newProposition[0]='\0';
  strcat(newProposition,"\ncoll(");
  strcat(newProposition,arg2);
  strcat(newProposition,",");
  strcat(newProposition,arg3);
  strcat(newProposition,",");
  strcat(newProposition,arg1);
  strcat(newProposition,") & cong(");
  strcat(newProposition,arg2);
  strcat(newProposition,",");
  strcat(newProposition,arg3);
  strcat(newProposition,",");
  strcat(newProposition,arg2);
  strcat(newProposition,",");
  strcat(newProposition,arg1);
  strcat(newProposition,")");  
}

void upstr(char * temp) {
  char * name;
  name = strtok(temp,":");

  // Convert to upper case
  char *s = name;
  while (*s) {
    *s = toupper((unsigned char) *s);
    s++;
  }
}
