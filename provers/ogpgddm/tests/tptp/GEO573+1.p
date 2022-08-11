%------------------------------------------------------------------------------
% File     : GEO573+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 35
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 35.p [Qua20]

% Status   : Theorem
% Rating   : 0.39 v8.1.0, 0.44 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  293 (   1 equ)
%            Maximal formula atoms :   10 (   3 avg)
%            Number of connectives :  205 (   7   ~;   0   |; 103   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   19 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  531 ( 511   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL214035,conjecture,
    ! [A,B,C,O,D,E,K,H,NWPNT1] :
      ( ( circle(O,A,B,C)
        & perp(D,C,A,B)
        & coll(D,A,B)
        & perp(E,B,A,C)
        & coll(E,A,C)
        & circle(O,C,K,NWPNT1)
        & coll(K,C,D)
        & coll(H,C,D)
        & coll(H,B,E) )
     => cong(A,K,A,H) ) ).

%------------------------------------------------------------------------------
