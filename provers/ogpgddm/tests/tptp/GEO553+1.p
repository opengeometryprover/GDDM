%------------------------------------------------------------------------------
% File     : GEO553+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 13
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 13.p [Qua20]

% Status   : Theorem
% Rating   : 0.36 v8.1.0, 0.44 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  294 (   1 equ)
%            Maximal formula atoms :   11 (   3 avg)
%            Number of connectives :  206 (   7   ~;   0   |; 104   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   19 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  530 ( 510   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL012013,conjecture,
    ! [A,B,C,D,E,F,G,H] :
      ( ( para(A,B,C,D)
        & para(A,C,B,D)
        & perp(E,B,A,C)
        & coll(E,A,C)
        & perp(F,A,B,D)
        & coll(F,B,D)
        & perp(G,D,A,C)
        & coll(G,A,C)
        & perp(H,C,B,D)
        & coll(H,B,D) )
     => para(E,F,G,H) ) ).

%------------------------------------------------------------------------------
