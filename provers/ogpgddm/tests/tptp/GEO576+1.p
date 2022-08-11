%------------------------------------------------------------------------------
% File     : GEO576+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 38
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 38.p [Qua20]

% Status   : Theorem
% Rating   : 0.44 v8.1.0, 0.50 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  291 (   1 equ)
%            Maximal formula atoms :    8 (   3 avg)
%            Number of connectives :  203 (   7   ~;   0   |; 101   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   16 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  530 ( 510   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL214038,conjecture,
    ! [A,B,C,D,O,D1,P,NWPNT1] :
      ( ( perp(D,A,B,C)
        & coll(D,B,C)
        & circle(O,A,B,C)
        & circle(O,A,D1,NWPNT1)
        & coll(D1,A,D)
        & perp(A,C,D1,P)
        & coll(P,B,C) )
     => eqangle(O,A,A,D,C,A,A,P) ) ).

%------------------------------------------------------------------------------
