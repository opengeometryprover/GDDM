%------------------------------------------------------------------------------
% File     : GEO605+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 67
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 67.p [Qua20]

% Status   : Theorem
% Rating   : 0.39 v8.1.0, 0.47 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  298 (   1 equ)
%            Maximal formula atoms :   15 (   3 avg)
%            Number of connectives :  210 (   7   ~;   0   |; 108   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   26 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  533 ( 513   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL618067,conjecture,
    ! [M,B,C,I,IC,IB,A,O,MIDPNT1,MIDPNT2,MIDPNT3] :
      ( ( eqangle(I,M,M,B,I,M,M,C)
        & eqangle(I,B,B,C,I,B,B,M)
        & eqangle(I,C,C,M,I,C,C,B)
        & perp(B,I,B,IC)
        & perp(C,I,C,IB)
        & coll(IC,C,I)
        & coll(IB,B,I)
        & midp(A,IB,IC)
        & midp(MIDPNT1,B,I)
        & perp(B,I,MIDPNT1,O)
        & midp(MIDPNT2,B,C)
        & perp(B,C,MIDPNT2,O)
        & midp(MIDPNT3,I,C)
        & perp(I,C,MIDPNT3,O) )
     => perp(A,B,B,O) ) ).

%------------------------------------------------------------------------------
