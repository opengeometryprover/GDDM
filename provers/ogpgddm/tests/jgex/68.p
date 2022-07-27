fof(exemplo6GDDFULL618068,conjecture,( ! [ A,B,P,D,E,I,F,G,NWPNT1,NWPNT2,NWPNT3,NWPNT4,NWPNT5,NWPNT6,NWPNT7,NWPNT8,NWPNT9,NWPNT01 ] :
((
perp(P,A,P,B) & 
circle(A,P,NWPNT1,NWPNT2) & 
circle(B,P,NWPNT3,NWPNT4) & 
circle(A,P,D,NWPNT5) & 
circle(A,D,E,NWPNT6) & 
coll(E,D,A) & 
circle(A,P,I,NWPNT7) & 
circle(B,P,I,NWPNT8) & 
coll(F,D,I) & 
circle(B,P,F,NWPNT9) & 
circle(B,F,G,NWPNT01) & 
coll(G,F,B)
)
=>
(
perp(G,F,D,E)
)
)
)).
