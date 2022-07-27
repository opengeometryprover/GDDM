fof(exemplo6GDDFULL618077,conjecture,( ! [ A,B,C,D,E,F,G,NWPNT1,NWPNT2,NWPNT3,NWPNT4,NWPNT5 ] :
((
eqangle(D,A,A,B,D,A,A,C) & eqangle(D,B,B,C,D,B,B,A) & eqangle(D,C,C,A,D,C,C,B) & 
midpoint(NWPNT1,B,D) & perp(B,D,NWPNT1,E) & 
perp(A,B,B,E) & 
circle(E,B,NWPNT2,NWPNT3) & 
coll(F,A,C) & 
circle(E,B,F,NWPNT4) & 
circle(E,F,G,NWPNT5) & 
coll(G,F,A)
)
=>
(
eqangle(F,D,D,C,C,D,D,G)
)
)
)).
