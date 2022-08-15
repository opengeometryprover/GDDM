fof(pappus, conjecture, ! [ A, B, C, D, E, F, P, Q, R ] : (
    ( coll(A, B, C)
      & coll(D, E, F)
      & coll(A, P, E)
      & coll(A, Q, F)
      & coll(B, P, D)
      & coll(B, R, F)
      & coll(C, Q, D)
      & coll(C, R, E) )
    =>
    coll(P, Q, R)
)).
