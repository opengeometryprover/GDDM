%------------------------------------------------------------------------------
% File     : GEO602+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 64
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 64.p [Qua20]

% Status   : Theorem
% Rating   : 0.39 v8.1.0, 0.47 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  295 (   1 equ)
%            Maximal formula atoms :   12 (   3 avg)
%            Number of connectives :  207 (   7   ~;   0   |; 105   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   29 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  539 ( 519   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL618064,conjecture,
    ! [A,B,E,F,D,M,E1,F1,NWPNT1,NWPNT2,NWPNT3,NWPNT4,NWPNT5,NWPNT6,NWPNT7,NWPNT8,NWPNT9] :
      ( ( circle(B,E,NWPNT1,NWPNT2)
        & circle(A,E,NWPNT3,NWPNT4)
        & circle(B,E,F,NWPNT5)
        & circle(A,E,F,NWPNT6)
        & coll(D,A,B)
        & coll(D,E,F)
        & circle(B,E,M,NWPNT7)
        & circle(A,E,E1,NWPNT8)
        & coll(E1,E,M)
        & coll(F1,F,M)
        & circle(A,E,F1,NWPNT9) )
     => perp(E1,F1,M,B) ) ).

%------------------------------------------------------------------------------
