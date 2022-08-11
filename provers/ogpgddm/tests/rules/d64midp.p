fof(ruleD64midp, conjecture, ! [ A, B, C, D, M ] : (
    ( midp(M, A, B)
      & para(A, C, B, D)
      & para(A, D, B, C) )
    =>
    midp(M, C, D)
)).
