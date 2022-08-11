%------------------------------------------------------------------------------
% File     : GEO545+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 05
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 05.p [Qua20]

% Status   : Theorem
% Rating   : 0.61 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  311 (   1 equ)
%            Maximal formula atoms :   28 (   3 avg)
%            Number of connectives :  223 (   7   ~;   0   |; 121   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   48 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  542 ( 522   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL012005,conjecture,
    ! [A,B,C,H,O,C1,B1,A1,MIDPNT1,MIDPNT2,MIDPNT3,MIDPNT4,MIDPNT5,MIDPNT6,MIDPNT7,MIDPNT8,MIDPNT9,MIDPNT01,MIDPNT11,MIDPNT21] :
      ( ( perp(A,B,C,H)
        & perp(A,C,B,H)
        & perp(B,C,A,H)
        & midp(MIDPNT1,A,B)
        & perp(A,B,MIDPNT1,O)
        & midp(MIDPNT2,A,C)
        & perp(A,C,MIDPNT2,O)
        & midp(MIDPNT3,B,C)
        & perp(B,C,MIDPNT3,O)
        & midp(MIDPNT4,A,B)
        & perp(A,B,MIDPNT4,C1)
        & midp(MIDPNT5,A,H)
        & perp(A,H,MIDPNT5,C1)
        & midp(MIDPNT6,B,H)
        & perp(B,H,MIDPNT6,C1)
        & midp(MIDPNT7,A,H)
        & perp(A,H,MIDPNT7,B1)
        & midp(MIDPNT8,A,C)
        & perp(A,C,MIDPNT8,B1)
        & midp(MIDPNT9,H,C)
        & perp(H,C,MIDPNT9,B1)
        & midp(MIDPNT01,B,H)
        & perp(B,H,MIDPNT01,A1)
        & midp(MIDPNT11,B,C)
        & perp(B,C,MIDPNT11,A1)
        & midp(MIDPNT21,H,C)
        & perp(H,C,MIDPNT21,A1) )
     => perp(A1,O,B1,C1) ) ).

%------------------------------------------------------------------------------
