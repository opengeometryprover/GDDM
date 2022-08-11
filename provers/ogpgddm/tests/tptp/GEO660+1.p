%------------------------------------------------------------------------------
% File     : GEO660+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem E023-19
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : E023-19.p [Qua20]

% Status   : Theorem
% Rating   : 0.36 v8.1.0, 0.42 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  293 (   1 equ)
%            Maximal formula atoms :   10 (   3 avg)
%            Number of connectives :  205 (   7   ~;   0   |; 103   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   17 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  529 ( 509   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo7Book00EE02E02319,conjecture,
    ! [A,B,C,D,E,F,G] :
      ( ( perp(C,A,C,B)
        & perp(D,C,A,B)
        & coll(D,A,B)
        & coll(E,C,D)
        & coll(F,A,B)
        & eqangle(B,A,A,E,E,A,A,C)
        & eqangle(D,C,C,F,F,C,C,B)
        & coll(G,B,C)
        & coll(G,A,E) )
     => para(E,F,C,B) ) ).

%------------------------------------------------------------------------------
