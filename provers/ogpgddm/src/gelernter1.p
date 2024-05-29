% −−−−Include Geometry Deductive Database Method axioms
include (’geometryDeductiveDatabaseMethod.ax ’) .
fof(gelernterT1,conjecture, ! [ A,B,C,D,E,F,G,H ] :
   ( ( midp(E,A,B) & midp(F,A,C) & midp(G,C,D) & midp(H,B,D) ) =>  para(E,F,G,H) ) ) .
