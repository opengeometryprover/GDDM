%------------------------------------------------------------------------------
% File     : GEO659+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem E023-17
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : E023-17.p [Qua20]

% Status   : Theorem
% Rating   : 0.47 v8.1.0, 0.56 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  298 (   1 equ)
%            Maximal formula atoms :   15 (   3 avg)
%            Number of connectives :  210 (   7   ~;   0   |; 108   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   28 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  535 ( 515   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULLmoreE02317,conjecture,
    ! [A,B,C,D,E,F,G,H,MIDPNT1,MIDPNT2,MIDPNT3,NWPNT4,NWPNT5] :
      ( ( perp(D,A,B,C)
        & coll(D,B,C)
        & perp(E,B,A,C)
        & coll(E,A,C)
        & midp(MIDPNT1,A,C)
        & perp(A,C,MIDPNT1,F)
        & midp(MIDPNT2,A,B)
        & perp(A,B,MIDPNT2,F)
        & midp(MIDPNT3,C,B)
        & perp(C,B,MIDPNT3,F)
        & coll(B,E,G)
        & circle(F,B,G,NWPNT4)
        & coll(A,D,H)
        & circle(F,A,H,NWPNT5) )
     => para(D,E,G,H) ) ).

%------------------------------------------------------------------------------
