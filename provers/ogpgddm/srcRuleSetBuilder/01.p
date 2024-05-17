%----Include Geometry Deductive Database Method axioms
include('geometryDeductiveDatabaseMethod.ax').

fof(gddm01,conjecture,
    ! [ A,B,C,D,E,F,G ] :
    ( ( perp(D,C,A,B) & coll(D,A,B) & perp(E,B,A,C) &
    coll(E,A,C) & midp(F,C,B) & midp(G,D,E)
    ) 
    =>  perp(F,G,D,E) ) ).
