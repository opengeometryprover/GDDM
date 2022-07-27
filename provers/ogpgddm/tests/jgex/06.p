
%----Include Geometry Deductive Database Method axioms
include('geometryDeductiveDatabaseMethod.ax').

fof(exemplo6GDDFULL012006,conjecture,( ! [ A,B,C,E,F,H ] :
((
perp(E,C,A,B) & coll(E,A,B) & 
perp(F,A,B,C) & coll(F,B,C) & 
coll(H,C,E) & 
coll(H,A,F)
)
=>
(
perp(B,H,A,C)
)
)
)).