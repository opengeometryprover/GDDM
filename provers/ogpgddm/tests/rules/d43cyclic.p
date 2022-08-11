fof(ruleD43cyclic, conjecture, ( ! [ A, B, C, P, Q, R ] : (
    ( cyclic(A, B, C, P)
      & cyclic(A, B, C, Q)
      & cyclic(A, B, C, R)
      & eqangle(C, A, C, B, R, P, R, Q) )
    =>
    ( cong(A, B, P, Q) )
))).
