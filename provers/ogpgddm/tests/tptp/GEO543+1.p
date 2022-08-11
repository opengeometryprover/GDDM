%------------------------------------------------------------------------------
% File     : GEO543+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 03
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 03.p [Qua20]

% Status   : Theorem
% Rating   : 0.44 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  295 (   1 equ)
%            Maximal formula atoms :   12 (   3 avg)
%            Number of connectives :  207 (   7   ~;   0   |; 105   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   22 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  532 ( 512   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL012003,conjecture,
    ! [A,B,C,D,E,F,H,A1,P,Q] :
      ( ( perp(D,A,B,C)
        & coll(D,B,C)
        & perp(E,B,A,C)
        & coll(E,A,C)
        & perp(F,C,A,B)
        & coll(F,A,B)
        & coll(H,B,E)
        & coll(H,C,F)
        & midp(A1,C,B)
        & midp(P,E,B)
        & midp(Q,F,C) )
     => cyclic(P,Q,H,D) ) ).

%------------------------------------------------------------------------------
