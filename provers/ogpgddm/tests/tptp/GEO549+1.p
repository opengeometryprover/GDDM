%------------------------------------------------------------------------------
% File     : GEO549+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 09
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 09.p [Qua20]

% Status   : Theorem
% Rating   : 0.44 v8.1.0, 0.53 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  300 (   1 equ)
%            Maximal formula atoms :   17 (   3 avg)
%            Number of connectives :  212 (   7   ~;   0   |; 110   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   33 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  538 ( 518   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL012009,conjecture,
    ! [A,B,D,O,C,P,F,P1,F2,F3,F1,L,M,NWPNT1,NWPNT2,NWPNT3] :
      ( ( circle(O,A,B,D)
        & circle(O,A,C,NWPNT1)
        & circle(O,A,P,NWPNT2)
        & perp(F,P,A,C)
        & coll(F,A,C)
        & circle(O,A,P1,NWPNT3)
        & perp(F2,P1,A,C)
        & coll(F2,A,C)
        & perp(F3,P1,A,D)
        & coll(F3,A,D)
        & perp(F1,P,A,D)
        & coll(F1,A,D)
        & perp(L,P,A,B)
        & coll(L,A,B)
        & perp(M,P1,A,B)
        & coll(M,A,B) )
     => eqangle(F,L,L,F1,F2,M,M,F3) ) ).

%------------------------------------------------------------------------------
