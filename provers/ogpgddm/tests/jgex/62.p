fof(exemplo6GDDFULL618062,conjecture,( ! [ A,B,C,D,E,F,G,H,I,J,K,NWPNT1,NWPNT2,NWPNT3,NWPNT4,NWPNT5,NWPNT6,NWPNT7,NWPNT8,NWPNT9,NWPNT01,NWPNT11,NWPNT21 ] :
((circle(D,A,B,C) & 
circle(D,A,E,NWPNT1) & midp(F,C,E) & midp(G,A,E) & 
circle(F,E,NWPNT2,NWPNT3) & 
circle(G,E,NWPNT4,NWPNT5) & 
circle(F,E,H,NWPNT6) & 
circle(G,E,H,NWPNT7) & 
coll(I,A,B) & 
circle(G,E,I,NWPNT8) & midp(J,B,E) & 
circle(J,B,NWPNT9,NWPNT01) & 
circle(F,E,K,NWPNT11) & 
circle(J,B,K,NWPNT21)
)
=>
(
coll(I,H,K)
)
)
)).
