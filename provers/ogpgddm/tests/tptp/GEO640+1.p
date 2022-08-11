%------------------------------------------------------------------------------
% File     : GEO640+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 106
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 106.p [Qua20]

% Status   : Theorem
% Rating   : 0.50 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  309 (   1 equ)
%            Maximal formula atoms :   26 (   3 avg)
%            Number of connectives :  221 (   7   ~;   0   |; 119   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   56 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  552 ( 532   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL81109106,conjecture,
    ! [Q4,Q1,Q3,Q0,Q2,P0,P1,P2,P3,P4,O0,O4,O3,O2,O1,M0,M4,M3,M2,M1,NWPNT1,NWPNT2,NWPNT3,NWPNT4,NWPNT5,NWPNT6,NWPNT7,NWPNT8,NWPNT9,NWPNT01] :
      ( ( coll(P0,Q4,Q1)
        & coll(P0,Q0,Q2)
        & coll(P1,Q1,Q3)
        & coll(P1,Q0,Q2)
        & coll(P2,Q1,Q3)
        & coll(P2,Q4,Q2)
        & coll(P3,Q3,Q0)
        & coll(P3,Q4,Q2)
        & coll(P4,Q4,Q1)
        & coll(P4,Q3,Q0)
        & circle(O0,Q0,P0,P4)
        & circle(O4,P4,P3,Q4)
        & circle(O3,P3,P2,Q3)
        & circle(O2,P1,P2,Q2)
        & circle(O1,P1,Q1,P0)
        & circle(O0,Q0,M0,NWPNT1)
        & circle(O1,Q1,M0,NWPNT2)
        & circle(O0,Q0,M4,NWPNT3)
        & circle(O4,Q4,M4,NWPNT4)
        & circle(O4,Q4,M3,NWPNT5)
        & circle(O3,Q3,M3,NWPNT6)
        & circle(O3,Q3,M2,NWPNT7)
        & circle(O2,Q2,M2,NWPNT8)
        & circle(O2,Q2,M1,NWPNT9)
        & circle(O1,Q1,M1,NWPNT01) )
     => cyclic(M4,M3,M2,M1) ) ).

%------------------------------------------------------------------------------
