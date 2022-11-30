
%----Include Geometry Deductive Database Method axioms
include('geometryDeductiveDatabaseMethod.ax').

fof(exemploeulerLineGEO0315a,conjecture,( ! [ A,B,C,D,E,F,G,H,I,J,K,L,M ] :
((midp(D,B,C) & midp(E,B,A) & midp(F,A,C) & 
coll(G,B,F) & coll(G,C,E) & 
perp(H,A,B,C) & coll(H,B,C) & 
perp(I,C,A,B) & coll(I,A,B) & 
perp(J,B,A,C) & coll(J,A,C) & 
coll(K,A,H) & coll(K,C,I) & 
perp(B,A,M,E) & 
perp(C,B,L,D) & 
perp(C,A,L,F) & 
coll(M,D,L)
)
=>
(
coll(L,G,K)
)
)
)).