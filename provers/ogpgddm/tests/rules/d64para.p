fof(ruleD64para, conjecture, ! [ A, B, C, D, M ] : (
    ( para(A, C, B, D)
      & para(A, D, B, C)
      & midp(M, A, B) )
    =>
    midp(M, C, D)
)).
