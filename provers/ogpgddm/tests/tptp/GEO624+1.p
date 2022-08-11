%------------------------------------------------------------------------------
% File     : GEO624+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 88
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 88.p [Qua20]

% Status   : Theorem
% Rating   : 0.39 v8.1.0, 0.47 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  296 (   1 equ)
%            Maximal formula atoms :   13 (   3 avg)
%            Number of connectives :  208 (   7   ~;   0   |; 106   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   23 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  532 ( 512   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL8110988,conjecture,
    ! [O,X,L,A,Y,B,P,Q,Q1,P1] :
      ( ( perp(A,L,O,X)
        & coll(A,O,X)
        & perp(B,L,O,Y)
        & coll(B,O,Y)
        & coll(L,A,P)
        & cong(L,A,L,P)
        & coll(L,B,Q)
        & cong(L,B,L,Q)
        & coll(Q1,L,A)
        & coll(Q1,O,Y)
        & coll(P1,O,X)
        & coll(P1,L,B) )
     => cyclic(O,P,Q,P1) ) ).

%------------------------------------------------------------------------------
