fof(simson, conjecture, ( ! [ A, B, C, D, E, F, G, AUX1, AUX2 ] : (
    ( circle(AUX1, A, B, C)
      & circle(AUX1, A, D, AUX2)
      & perp(A, B, D, E)
      & coll(A, B, E)
      & perp(A, C, D, F)
      & coll(A, C, F)
      & perp(B, C, D, G)
      & coll(B, C, G) )
    =>
    ( coll(E, F, G) )
))).
