%{

#include <stdio.h>
#include <string.h>

extern FILE * yyin;
extern FILE * yyout;

int yylex(void);
/* int warning(char *); */
int yyerror(char *);

int npId=0,nlId=0;

typedef char* Vars;
Vars pointsIds[100];

char* varId;
char auxstr[10];

typedef struct lines {
  Vars lineId;
  Vars pointsIds[2];
} Lines;

typedef struct hypotheses {
  char hypothesis[100];
} Hypotheses;

typedef struct conjectures {
  char conjecture[200];
} Conjectures;

Lines gclLines[100];
int nLines=0;

Hypotheses tptpHypotheses[100];
Conjectures tptpConjectures[20];
int numHyp=0;
int numConj=0;
 
%}

%union {
   double number;
   char *string;
} 

%token <number> NUMBER TOKPI
%token <string> STRING
%token <string> VARIABLE PROVEEQUAL

%token POINT LINE CIRCLE ONSEGMENT ONLINE ONCIRCLE
%token INTERSECTION INTERSECTION2 SEGMENT ANGLE
%token MED BIS PERP FOOT
%token PROVE IDENTICAL 
%token PD3 SRATIO SA3 SA4
%token EQUAL MULT SUM ALGSUMZERO3
%token MIDPOINT MEDIATRICE PERPENDICULAR BISECTOR
%token COLLINEAR PARALLEL SAMELENGTH
%token HARMONIC CYCLIC EQANGLE PBISECTO

%% 

input: 
       line
     | input '\n' line
     ;


line:
    | command
    ;


command: POINT VARIABLE NUMBER NUMBER
            {
	      //	      printf("point %s %f %f\n",$2,$3,$4);
	      pointsIds[npId] = (Vars) malloc(10);
	      strcpy(pointsIds[npId++],$2);
	    }
       | MIDPOINT VARIABLE VARIABLE VARIABLE
            {
	      sprintf(tptpHypotheses[numHyp++].hypothesis,"midp(%s,%s,%s)",$2,$3,$4);
	    }
       | ONLINE VARIABLE VARIABLE VARIABLE
            {
	      sprintf(tptpHypotheses[numHyp++].hypothesis,"coll(%s,%s,%s)",$3,$4,$2);
	    }
       | LINE VARIABLE VARIABLE VARIABLE
            {
	      nLines++;
	      gclLines[nlId].lineId=(Vars) malloc(10);
	      strcpy(gclLines[nlId].lineId,$2);
	      gclLines[nlId].pointsIds[0]=(Vars) malloc(10);
	      strcpy(gclLines[nlId].pointsIds[0],$3);	      
	      gclLines[nlId].pointsIds[1]=(Vars) malloc(10);
	      strcpy(gclLines[nlId++].pointsIds[1],$4);	      
	      // printf("linha %s %s %s\n",$2,$3,$4);
	    }
       | PARALLEL VARIABLE VARIABLE VARIABLE
            {
	      int i,pos=-1;
	      int conj=0;
	      // New line (the parallel line)
	      nLines++;
	      // line Id
	      gclLines[nlId].lineId=(Vars) malloc(10);
	      strcpy(gclLines[nlId].lineId,$2);
	      // first point of the new line
	      gclLines[nlId].pointsIds[0]=(Vars) malloc(10);
	      strcpy(gclLines[nlId].pointsIds[0],$3);
	      // create a new point
	      sprintf(auxstr, "X%d", npId+1);
	      pointsIds[npId] = (Vars) malloc(10);
	      strcpy(pointsIds[npId++],auxstr);
	      // second point of the line
	      gclLines[nlId].pointsIds[1]=(Vars) malloc(10);
	      strcpy(gclLines[nlId++].pointsIds[1],auxstr);	  
	      // get the points of the (old) line 
	      for (i=0; i < nLines && pos != -1; i++) {
		if (!strcmp($4,gclLines[i].lineId)) {
		  pos = i;
		}
	      }
	      // gclLines[pos].pointsIds[0],gclLines[pos].pointsIds[1]
              sprintf(tptpHypotheses[numHyp++].hypothesis,"para(%s,%s,%s,%s)",gclLines[i].pointsIds[0],gclLines[i].pointsIds[1],$3,auxstr);
	    }
       | CIRCLE VARIABLE VARIABLE VARIABLE
            {
	      sprintf(auxstr, "X%d", npId+1);
	      pointsIds[npId] = (Vars) malloc(10);
	      strcpy(pointsIds[npId++],auxstr);
	      sprintf(auxstr, "X%d", npId+1);
	      pointsIds[npId] = (Vars) malloc(10);
	      strcpy(pointsIds[npId++],auxstr);
	      sprintf(tptpHypotheses[numHyp++].hypothesis,"circle(%s,%s,%s,%s)",$3,$4,pointsIds[npId-2],pointsIds[npId-1]);
	     }
       | INTERSECTION VARIABLE VARIABLE VARIABLE VARIABLE VARIABLE
            {
	      sprintf(tptpHypotheses[numHyp++].hypothesis,"coll(%s,%s,%s) & coll(%s,%s,%s)\n",$3,$4,$2,$5,$6,$2);
	    }
       | INTERSECTION VARIABLE VARIABLE VARIABLE
              { int i;
		int conj=0;
		for (i=0;i<nLines;i++) {
		  if (!strcmp($3,gclLines[i].lineId)) {
		    //		    printf("coll(%s,%s,%s)",gclLines[i].pointsIds[0],gclLines[i].pointsIds[1],$2);
		    sprintf(tptpHypotheses[numHyp++].hypothesis,"coll(%s,%s,%s)",gclLines[i].pointsIds[0],gclLines[i].pointsIds[1],$2);
		  }
		  if (!strcmp($4,gclLines[i].lineId)) {
		    //		    printf("coll(%s,%s,%s)",gclLines[i].pointsIds[0],gclLines[i].pointsIds[1],$2);
		    sprintf(tptpHypotheses[numHyp++].hypothesis,"coll(%s,%s,%s)",gclLines[i].pointsIds[0],gclLines[i].pointsIds[1],$2);
		  }
		}
	    }
       // P_ACD = P_BCD, AB perpendicular to CD
       | PROVE '{' EQUAL '{' PD3 VARIABLE VARIABLE VARIABLE '}' '{' PD3 VARIABLE VARIABLE VARIABLE '}' '}' 
            {
	      sprintf(tptpConjectures[numConj++].conjecture,"pythagoras_difference3 %s %s %s  pythagoras_difference3 %s %s %s ",$6,$7,$8,$12,$13,$14);
	    }
       // S_ABC=0, points A,B,C are collinear
       | PROVE '{' EQUAL '{' SA3 VARIABLE VARIABLE VARIABLE '}' '{' NUMBER '}' '}' 
            {
	      sprintf(tptpConjectures[numConj++].conjecture,"coll(%s,%s,%s)",$6,$7,$8);
	    }
       // S_ABC=0, points A,B,C are collinear
       | PROVE '{' EQUAL '{' SA3 VARIABLE VARIABLE VARIABLE '}' NUMBER '}' 
            {
	      sprintf(tptpConjectures[numConj++].conjecture,"coll(%s,%s,%s)",$6,$7,$8);
	    }
       // sratio_ABCD = sratio_EFGH
       | PROVE '{' EQUAL '{' SRATIO VARIABLE VARIABLE VARIABLE VARIABLE '}' '{'  SRATIO VARIABLE VARIABLE VARIABLE VARIABLE '}' '}' 
            {
	      sprintf(tptpConjectures[numConj++].conjecture,"eqratio(%s,%s,%s,%s,%s,%s,%s,%s)",$6,$7,$8,$9,$13,$14,$15,$16);
	    }
       | PROVE '{' PARALLEL VARIABLE VARIABLE VARIABLE VARIABLE '}'
            {
	      sprintf(tptpConjectures[numConj++].conjecture,"parallel(%s,%s,%s,%s)",$4,$5,$6,$7);
	    }

       | PROVE '{' SAMELENGTH VARIABLE VARIABLE VARIABLE VARIABLE '}'
            {
	      sprintf(tptpConjectures[numConj++].conjecture,"cong(%s,%s,%s,%s)",$4,$5,$6,$7);
	    }

       | error { }
       ;


%%

int main (int argc,char *argv[]) {

  char conjectureName[20], fileTPTP[20];

    
  if (argc < 2) {
    printf("Usage ./gclToTPTP <name_of_gcl_file>\n");
    return 0;
  }

  int lengthConjectureName = strlen(argv[1]);
  strncat(conjectureName,argv[1],lengthConjectureName-4);
  strcpy(fileTPTP,conjectureName);
  strcat(fileTPTP,".p");
  yyin = fopen(argv[1], "r" );
  
  yyparse();

  if (numConj == 0) { // no valid conjecture --> no output
    return(1);
  }
  
  yyout = fopen(fileTPTP, "w" );

  
  fprintf(yyout,"%% %s\n\n",conjectureName);
  // Include the Axions
  fprintf(yyout,"\n%%----Include Geometry Deductive Database Method axioms\ninclude('geometryDeductiveDatabaseMethod.ax').\n\nfof(tgtp%s,conjecture,( ! [ ",conjectureName);
  
  //  fprintf(yyout,"\t( ! [");
  for (int j=0;j<npId;j++){
    fprintf(yyout,"%s",pointsIds[j]);
    if (j<npId-1) fprintf(yyout,",");
  }
  fprintf(yyout,"] : ");

  //  Hypothesis
  fprintf(yyout,"\n\t\t(\n");

  for (int j=0;j<numHyp;j++) {
    fprintf(yyout,"\t\t %s",tptpHypotheses[j].hypothesis);
    if (j<numHyp-1) {
      fprintf(yyout," &\n");
    }
    else {
      fprintf(yyout,"\n\t\t)\n");
    }      
  }
    
  // Conjectures

  fprintf(yyout,"\t\t=>\n");
  fprintf(yyout,"\t\t(\n");  
  for (int j=0;j<numConj;j++) {
    fprintf(yyout,"\t\t %s",tptpConjectures[j].conjecture);
    if (j<numConj-1) {
      fprintf(yyout," &\n");
    }
    else {
      fprintf(yyout,"\n\t\t)\n");
    }      
  }
  fprintf(yyout,"\t)\n).\n");
  
}

int yyerror(char *s)
{
}
