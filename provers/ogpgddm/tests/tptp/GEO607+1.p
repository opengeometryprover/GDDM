%------------------------------------------------------------------------------
% File     : GEO607+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 69
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 69.p [Qua20]

% Status   : Theorem
% Rating   : 0.33 v8.1.0, 0.44 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  288 (   1 equ)
%            Maximal formula atoms :    8 (   3 avg)
%            Number of connectives :  200 (   7   ~;   0   |;  98   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   15 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  528 ( 508   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL618069,conjecture,
    ! [A,B,C,D,E,F] :
      ( ( midp(C,B,A)
        & perp(D,A,D,B)
        & circle(E,A,C,D)
        & circle(F,B,D,C) )
     => perp(E,D,D,F) ) ).

%------------------------------------------------------------------------------
