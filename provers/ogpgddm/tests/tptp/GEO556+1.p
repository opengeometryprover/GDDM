%------------------------------------------------------------------------------
% File     : GEO556+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 16
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 16.p [Qua20]

% Status   : Theorem
% Rating   : 0.39 v8.1.0, 0.44 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  299 (   1 equ)
%            Maximal formula atoms :   16 (   3 avg)
%            Number of connectives :  211 (   7   ~;   0   |; 109   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   27 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  533 ( 513   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL012016,conjecture,
    ! [A,B,O,M,P,Q,D,C,T,K,S] :
      ( ( coll(M,A,B)
        & perp(P,M,A,O)
        & coll(P,A,O)
        & perp(Q,M,B,O)
        & coll(Q,B,O)
        & perp(D,B,A,O)
        & coll(D,A,O)
        & perp(C,A,B,O)
        & coll(C,B,O)
        & perp(T,Q,A,O)
        & coll(T,A,O)
        & perp(K,P,B,O)
        & coll(K,B,O)
        & coll(S,Q,T)
        & coll(S,P,K) )
     => perp(O,S,P,Q) ) ).

%------------------------------------------------------------------------------
