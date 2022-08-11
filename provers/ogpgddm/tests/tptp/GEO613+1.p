%------------------------------------------------------------------------------
% File     : GEO613+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 75y
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 75y.p [Qua20]

% Status   : Theorem
% Rating   : 0.47 v8.1.0, 0.53 v7.5.0
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
fof(exemplo6GDDFULL618075y,conjecture,
    ! [A,B,C,D,E,F,G,H,NWPNT1,NWPNT2,NWPNT3,NWPNT4,NWPNT5,NWPNT6,NWPNT7,NWPNT8] :
      ( ( circle(A,B,NWPNT1,NWPNT2)
        & circle(C,B,NWPNT3,NWPNT4)
        & circle(A,B,D,NWPNT5)
        & circle(C,B,D,NWPNT6)
        & midp(E,D,B)
        & circle(A,B,F,NWPNT7)
        & circle(C,B,G,NWPNT8)
        & coll(G,B,F)
        & coll(H,A,F)
        & coll(H,C,G) )
     => eqangle(F,D,D,G,A,H,H,C) ) ).

%------------------------------------------------------------------------------
