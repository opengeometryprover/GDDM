%{

#include <iostream>
#include <string>		/* Not needed.  Why? */

#include "axioms.hpp"

Axiom ax;
Predicate pred;

int yylex(void);
void yyerror(std::string);

extern Axiom_list axioms;

%}

%define api.value.type {std::string}

%token COLON
%token COMMA
%token CONJ
%token FOF
%token FORALL
%token FULLSTOP
%token ID
%token LBRKT
%token LPAREN
%token RBRKT
%token RPAREN
%token STXCONS

%token COLL
%token MIDP
%token CIRCLE
%token CONG
%token CONTRI
%token CYCLIC
%token EQANGLE
%token EQRATIO
%token PARA
%token PERP
%token SIMTRI

%%

%start axiom_list;

axiom_list:
|	axiom_list axiom
	{
	    ax.points = {};
	    ax.antecedents = {};
	}
;

axiom:
	FOF LPAREN ID COMMA ID COMMA FORALL LBRKT pt_list RBRKT COLON LPAREN LPAREN antecedent_list RPAREN STXCONS consequence RPAREN RPAREN FULLSTOP
	{
	    ax.name = $3;
	    axioms.push_back(ax);
	}
;

pt_list:
	ID
	{
	    ax.points.insert($1);
	}
|	pt_list COMMA ID
	{
	    ax.points.insert($3);
	}
;

antecedent_list:
	antecedent
	{
	    ax.antecedents.push_back(pred);
	}
|	antecedent_list CONJ antecedent
	{
	    ax.antecedents.push_back(pred);
	}
;

antecedent:
	CIRCLE LPAREN ID COMMA ID COMMA ID COMMA ID RPAREN
	{
	    pred.name = "circle";
	    pred.points = {};
            pred.points.push_back($3);
            pred.points.push_back($5);
            pred.points.push_back($7);
            pred.points.push_back($9);
	}
|	COLL LPAREN ID COMMA ID COMMA ID RPAREN
	{
	    pred.name = "coll";
	    pred.points = {};
            pred.points.push_back($3);
            pred.points.push_back($5);
            pred.points.push_back($7);
	}
|	CONG LPAREN ID COMMA ID COMMA ID COMMA ID RPAREN
	{
	    pred.name = "cong";
	    pred.points = {};
            pred.points.push_back($3);
            pred.points.push_back($5);
            pred.points.push_back($7);
            pred.points.push_back($9);
	}
|	CONTRI LPAREN ID COMMA ID COMMA ID COMMA ID COMMA ID COMMA ID RPAREN
	{
	    pred.name = "contri";
	    pred.points = {};
            pred.points.push_back($3);
            pred.points.push_back($5);
            pred.points.push_back($7);
            pred.points.push_back($9);
            pred.points.push_back($11);
            pred.points.push_back($13);
	}
|	CYCLIC LPAREN ID COMMA ID COMMA ID COMMA ID RPAREN
	{
	    pred.name = "cyclic";
	    pred.points = {};
            pred.points.push_back($3);
            pred.points.push_back($5);
            pred.points.push_back($7);
            pred.points.push_back($9);
	}
|	EQANGLE LPAREN ID COMMA ID COMMA ID COMMA ID COMMA ID COMMA ID COMMA ID COMMA ID RPAREN
	{
	    pred.name = "eqangle";
	    pred.points = {};
            pred.points.push_back($3);
            pred.points.push_back($5);
            pred.points.push_back($7);
            pred.points.push_back($9);
            pred.points.push_back($11);
            pred.points.push_back($13);
            pred.points.push_back($15);
            pred.points.push_back($17);
	}
|	EQRATIO LPAREN ID COMMA ID COMMA ID COMMA ID COMMA ID COMMA ID COMMA ID COMMA ID RPAREN
	{
	    pred.name = "eqratio";
	    pred.points = {};
            pred.points.push_back($3);
            pred.points.push_back($5);
            pred.points.push_back($7);
            pred.points.push_back($9);
            pred.points.push_back($11);
            pred.points.push_back($13);
            pred.points.push_back($15);
            pred.points.push_back($17);
	}
|	MIDP LPAREN ID COMMA ID COMMA ID RPAREN
	{
	    pred.name = "midp";
	    pred.points = {};
            pred.points.push_back($3);
            pred.points.push_back($5);
            pred.points.push_back($7);
	}
|	PARA LPAREN ID COMMA ID COMMA ID COMMA ID RPAREN
	{
	    pred.name = "para";
	    pred.points = {};
            pred.points.push_back($3);
            pred.points.push_back($5);
            pred.points.push_back($7);
            pred.points.push_back($9);
	}
|	PERP LPAREN ID COMMA ID COMMA ID COMMA ID RPAREN
	{
	    pred.name = "perp";
	    pred.points = {};
            pred.points.push_back($3);
            pred.points.push_back($5);
            pred.points.push_back($7);
            pred.points.push_back($9);
	}
|	SIMTRI LPAREN ID COMMA ID COMMA ID COMMA ID COMMA ID COMMA ID RPAREN
	{
	    pred.name = "simtri";
	    pred.points = {};
            pred.points.push_back($3);
            pred.points.push_back($5);
            pred.points.push_back($7);
            pred.points.push_back($9);
            pred.points.push_back($11);
            pred.points.push_back($13);
	}
;

consequence:
	CIRCLE LPAREN ID COMMA ID COMMA ID COMMA ID RPAREN
	{
	    ax.consequence.name = "circle";
	    ax.consequence.points = {};
            ax.consequence.points.push_back($3);
            ax.consequence.points.push_back($5);
            ax.consequence.points.push_back($7);
            ax.consequence.points.push_back($9);
	}
|	COLL LPAREN ID COMMA ID COMMA ID RPAREN
	{
	    ax.consequence.name = "coll";
	    ax.consequence.points = {};
            ax.consequence.points.push_back($3);
            ax.consequence.points.push_back($5);
            ax.consequence.points.push_back($7);
	}
|	CONG LPAREN ID COMMA ID COMMA ID COMMA ID RPAREN
	{
	    ax.consequence.name = "cong";
	    ax.consequence.points = {};
            ax.consequence.points.push_back($3);
            ax.consequence.points.push_back($5);
            ax.consequence.points.push_back($7);
            ax.consequence.points.push_back($9);
	}
|	CONTRI LPAREN ID COMMA ID COMMA ID COMMA ID COMMA ID COMMA ID RPAREN
	{
	    ax.consequence.name = "contri";
	    ax.consequence.points = {};
            ax.consequence.points.push_back($3);
            ax.consequence.points.push_back($5);
            ax.consequence.points.push_back($7);
            ax.consequence.points.push_back($9);
            ax.consequence.points.push_back($11);
            ax.consequence.points.push_back($13);
	}
|	CYCLIC LPAREN ID COMMA ID COMMA ID COMMA ID RPAREN
	{
	    ax.consequence.name = "cyclic";
	    ax.consequence.points = {};
            ax.consequence.points.push_back($3);
            ax.consequence.points.push_back($5);
            ax.consequence.points.push_back($7);
            ax.consequence.points.push_back($9);
	}
|	EQANGLE LPAREN ID COMMA ID COMMA ID COMMA ID COMMA ID COMMA ID COMMA ID COMMA ID RPAREN
	{
	    ax.consequence.name = "eqangle";
	    ax.consequence.points = {};
            ax.consequence.points.push_back($3);
            ax.consequence.points.push_back($5);
            ax.consequence.points.push_back($7);
            ax.consequence.points.push_back($9);
            ax.consequence.points.push_back($11);
            ax.consequence.points.push_back($13);
            ax.consequence.points.push_back($15);
            ax.consequence.points.push_back($17);
	}
|	EQRATIO LPAREN ID COMMA ID COMMA ID COMMA ID COMMA ID COMMA ID COMMA ID COMMA ID RPAREN
	{
	    ax.consequence.name = "eqratio";
	    ax.consequence.points = {};
            ax.consequence.points.push_back($3);
            ax.consequence.points.push_back($5);
            ax.consequence.points.push_back($7);
            ax.consequence.points.push_back($9);
            ax.consequence.points.push_back($11);
            ax.consequence.points.push_back($13);
            ax.consequence.points.push_back($15);
            ax.consequence.points.push_back($17);
	}
|	MIDP LPAREN ID COMMA ID COMMA ID RPAREN
	{
	    ax.consequence.name = "midp";
	    ax.consequence.points = {};
            ax.consequence.points.push_back($3);
            ax.consequence.points.push_back($5);
            ax.consequence.points.push_back($7);
	}
|	PARA LPAREN ID COMMA ID COMMA ID COMMA ID RPAREN
	{
	    ax.consequence.name = "para";
	    ax.consequence.points = {};
            ax.consequence.points.push_back($3);
            ax.consequence.points.push_back($5);
            ax.consequence.points.push_back($7);
            ax.consequence.points.push_back($9);
	}
|	PERP LPAREN ID COMMA ID COMMA ID COMMA ID RPAREN
	{
	    ax.consequence.name = "perp";
	    ax.consequence.points = {};
            ax.consequence.points.push_back($3);
            ax.consequence.points.push_back($5);
            ax.consequence.points.push_back($7);
            ax.consequence.points.push_back($9);
	}
|	SIMTRI LPAREN ID COMMA ID COMMA ID COMMA ID COMMA ID COMMA ID RPAREN
	{
	    ax.consequence.name = "simtri";
	    ax.consequence.points = {};
            ax.consequence.points.push_back($3);
            ax.consequence.points.push_back($5);
            ax.consequence.points.push_back($7);
            ax.consequence.points.push_back($9);
            ax.consequence.points.push_back($11);
            ax.consequence.points.push_back($13);
	}
;

%%

void yyerror(std::string error_msg)
{
    std::cerr << "[pg:ERROR] " << error_msg << std::endl;
}
