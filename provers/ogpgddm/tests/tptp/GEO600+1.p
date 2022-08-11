%------------------------------------------------------------------------------
% File     : GEO600+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 62
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 62.p [Qua20]

% Status   : Theorem
% Rating   : 0.42 v8.1.0, 0.50 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  298 (   1 equ)
%            Maximal formula atoms :   15 (   3 avg)
%            Number of connectives :  210 (   7   ~;   0   |; 108   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   38 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  545 ( 525   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL618062,conjecture,
    ! [A,B,C,D,E,F,G,H,I,J,K,NWPNT1,NWPNT2,NWPNT3,NWPNT4,NWPNT5,NWPNT6,NWPNT7,NWPNT8,NWPNT9,NWPNT01,NWPNT11,NWPNT21] :
      ( ( circle(D,A,B,C)
        & circle(D,A,E,NWPNT1)
        & midp(F,C,E)
        & midp(G,A,E)
        & circle(F,E,NWPNT2,NWPNT3)
        & circle(G,E,NWPNT4,NWPNT5)
        & circle(F,E,H,NWPNT6)
        & circle(G,E,H,NWPNT7)
        & coll(I,A,B)
        & circle(G,E,I,NWPNT8)
        & midp(J,B,E)
        & circle(J,B,NWPNT9,NWPNT01)
        & circle(F,E,K,NWPNT11)
        & circle(J,B,K,NWPNT21) )
     => coll(I,H,K) ) ).

%------------------------------------------------------------------------------
