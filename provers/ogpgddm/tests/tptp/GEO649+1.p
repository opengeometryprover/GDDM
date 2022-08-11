%------------------------------------------------------------------------------
% File     : GEO649+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem E022-10
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : E022-10.p [Qua20]

% Status   : Theorem
% Rating   : 0.44 v8.1.0, 0.56 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  297 (   1 equ)
%            Maximal formula atoms :   14 (   3 avg)
%            Number of connectives :  209 (   7   ~;   0   |; 107   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   30 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  538 ( 518   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULLmoreE02210,conjecture,
    ! [A,B,C,D,E,F,G,H,NWPNT1,NWPNT2,NWPNT3,NWPNT4,NWPNT5,MIDPNT6,MIDPNT7,MIDPNT8] :
      ( ( circle(A,B,NWPNT1,NWPNT2)
        & circle(A,B,C,NWPNT3)
        & circle(A,B,D,NWPNT4)
        & circle(A,B,E,NWPNT5)
        & coll(F,C,E)
        & coll(F,B,D)
        & midp(MIDPNT6,E,F)
        & perp(E,F,MIDPNT6,G)
        & midp(MIDPNT7,E,B)
        & perp(E,B,MIDPNT7,G)
        & midp(MIDPNT8,F,B)
        & perp(F,B,MIDPNT8,G)
        & perp(F,G,F,H) )
     => para(H,F,C,D) ) ).

%------------------------------------------------------------------------------
