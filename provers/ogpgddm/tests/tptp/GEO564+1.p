%------------------------------------------------------------------------------
% File     : GEO564+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 26
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 26.p [Qua20]

% Status   : Theorem
% Rating   : 0.67 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  322 (   1 equ)
%            Maximal formula atoms :   39 (   3 avg)
%            Number of connectives :  234 (   7   ~;   5   |; 127   &)
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
fof(exemplo6GDDFULL214026,conjecture,
    ! [A,B,C,H,O,OC,OA,OB,MIDPNT1,MIDPNT2,MIDPNT3,MIDPNT4,MIDPNT5,MIDPNT6,MIDPNT7,MIDPNT8,MIDPNT9,MIDPNT01,MIDPNT11,MIDPNT21] :
      ( ( perp(A,B,C,H)
        & perp(A,C,B,H)
        & perp(B,C,A,H)
        & midp(MIDPNT1,A,B)
        & perp(A,B,MIDPNT1,O)
        & midp(MIDPNT2,A,C)
        & perp(A,C,MIDPNT2,O)
        & midp(MIDPNT3,B,C)
        & perp(B,C,MIDPNT3,O)
        & midp(MIDPNT4,A,H)
        & perp(A,H,MIDPNT4,OC)
        & midp(MIDPNT5,A,B)
        & perp(A,B,MIDPNT5,OC)
        & midp(MIDPNT6,H,B)
        & perp(H,B,MIDPNT6,OC)
        & midp(MIDPNT7,B,H)
        & perp(B,H,MIDPNT7,OA)
        & midp(MIDPNT8,B,C)
        & perp(B,C,MIDPNT8,OA)
        & midp(MIDPNT9,H,C)
        & perp(H,C,MIDPNT9,OA)
        & midp(MIDPNT01,C,H)
        & perp(C,H,MIDPNT01,OB)
        & midp(MIDPNT11,C,A)
        & perp(C,A,MIDPNT11,OB)
        & midp(MIDPNT21,H,A)
        & perp(H,A,MIDPNT21,OB) )
     => ( eqangle(B,A,A,C,OB,OA,OA,OC)
        & ( eqangle(A,B,B,C,OA,OB,OB,OC)
          | eqangle(B,A,A,C,OB,OA,OA,OC) )
        & ( eqangle(A,B,B,C,OA,OC,OC,OB)
          | eqangle(B,A,A,C,OA,OB,OB,OC) )
        & ( eqangle(A,B,B,C,OB,OA,OA,OC)
          | eqangle(B,A,A,C,OA,OB,OB,OC) )
        & ( eqangle(A,B,B,C,OA,OC,OC,OB)
          | eqangle(B,A,A,C,OA,OC,OC,OB) )
        & ( eqangle(A,B,B,C,OB,OA,OA,OC)
          | eqangle(B,A,A,C,OA,OC,OC,OB) )
        & eqangle(A,B,B,C,OA,OB,OB,OC) ) ) ).

%------------------------------------------------------------------------------
