fof(exemplo6GDDFULL618061,conjecture,( ! [ A,B,C,D,E,F,G,NWPNT1,NWPNT2,NWPNT3 ] :
((circle(D,A,B,C) & midp(E,B,A) & 
coll(F,D,E) & 
circle(F,A,NWPNT1,NWPNT2) & 
coll(G,B,C) & 
circle(F,A,G,NWPNT3)
)
=>
(
eqangle(D,A,A,F,C,A,A,G) & (eqangle(A,D,D,F,A,C,C,G) |
eqangle(D,A,A,F,C,A,A,G)) & (eqangle(A,D,D,F,A,G,G,C) |
eqangle(D,A,A,F,A,C,C,G)) & (eqangle(A,D,D,F,C,A,A,G) |
eqangle(D,A,A,F,A,C,C,G)) & (eqangle(A,D,D,F,A,G,G,C) |
eqangle(D,A,A,F,A,G,G,C)) & (eqangle(A,D,D,F,C,A,A,G) |
eqangle(D,A,A,F,A,G,G,C)) & eqangle(A,D,D,F,A,C,C,G)
)
)
)).
