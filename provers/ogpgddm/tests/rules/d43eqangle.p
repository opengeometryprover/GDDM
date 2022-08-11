fof(ruleD43eqangle, conjecture, ( ! [ A, B, C, P, Q, R ] : (
    ( eqangle(C, A, C, B, R, P, R, Q)
      & cyclic(A, B, C, P)
      & cyclic(A, B, C, Q)
      & cyclic(A, B, C, R) )
    =>
    ( cong(A, B, P, Q) )
))).
