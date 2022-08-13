fof(ruleD57cyclic, conjecture, ! [ A, B, P, Q ] : (
    ( cyclic(A, B, P, Q)
      & cong(A, P, B, P)
      & cong(A, Q, B, Q) )
    =>
    perp(P, A, A, Q)
)).
