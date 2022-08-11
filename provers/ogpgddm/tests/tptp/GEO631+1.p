%------------------------------------------------------------------------------
% File     : GEO631+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 94
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 94.p [Qua20]

% Status   : Theorem
% Rating   : 0.36 v8.1.0, 0.44 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  297 (   1 equ)
%            Maximal formula atoms :   14 (   3 avg)
%            Number of connectives :  209 (   7   ~;   0   |; 107   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   27 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  535 ( 515   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL8110994,conjecture,
    ! [A,B,C,O,D,P,F,G,H,E,I,NWPNT1,NWPNT2] :
      ( ( circle(O,A,B,C)
        & circle(O,A,D,NWPNT1)
        & circle(O,A,P,NWPNT2)
        & perp(F,P,A,D)
        & coll(F,A,D)
        & perp(G,P,A,B)
        & coll(G,A,B)
        & perp(H,P,B,C)
        & coll(H,B,C)
        & perp(E,P,C,D)
        & coll(E,C,D)
        & coll(I,F,G)
        & coll(I,H,E) )
     => cyclic(P,G,I,H) ) ).

%------------------------------------------------------------------------------
