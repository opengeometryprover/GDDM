%------------------------------------------------------------------------------
% File     : GEO601+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 63f
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 63f.p [Qua20]

% Status   : Theorem
% Rating   : 0.36 v8.1.0, 0.47 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  288 (   1 equ)
%            Maximal formula atoms :    8 (   3 avg)
%            Number of connectives :  200 (   7   ~;   0   |;  98   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   15 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  529 ( 509   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL618063f,conjecture,
    ! [A,B,C,D,E,F,G] :
      ( ( coll(D,B,C)
        & circle(E,A,D,C)
        & circle(F,A,D,B)
        & circle(G,B,A,C) )
     => cyclic(A,F,G,E) ) ).

%------------------------------------------------------------------------------
