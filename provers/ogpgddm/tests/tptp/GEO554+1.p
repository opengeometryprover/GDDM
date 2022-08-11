%------------------------------------------------------------------------------
% File     : GEO554+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 14
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 14.p [Qua20]

% Status   : Theorem
% Rating   : 0.42 v8.1.0, 0.44 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  294 (   1 equ)
%            Maximal formula atoms :   11 (   3 avg)
%            Number of connectives :  206 (   7   ~;   0   |; 104   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   27 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  538 ( 518   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL012014,conjecture,
    ! [A,B,C,D,E,F,G,NWPNT1,NWPNT2,NWPNT3,NWPNT4,NWPNT5,NWPNT6,NWPNT7,NWPNT8,NWPNT9] :
      ( ( perp(C,A,C,B)
        & circle(A,C,NWPNT1,NWPNT2)
        & circle(B,C,NWPNT3,NWPNT4)
        & circle(A,C,D,NWPNT5)
        & coll(C,D,E)
        & circle(B,C,E,NWPNT6)
        & coll(B,E,F)
        & circle(B,C,F,NWPNT7)
        & circle(A,C,G,NWPNT8)
        & circle(B,C,G,NWPNT9) )
     => coll(D,F,G) ) ).

%------------------------------------------------------------------------------
