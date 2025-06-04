% −−−−Include Geometry Deductive Database Method axioms
include (’geometryDeductiveDatabaseMethod.ax ’) .
fof (geo0007,conjecture, ! [ A, B, C, D, E ] :
     ( ( midp(D, C,A) & midp(E , A, B) ) => para (B, C, E ,D) ) )  .
