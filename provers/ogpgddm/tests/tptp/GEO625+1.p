%------------------------------------------------------------------------------
% File     : GEO625+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 89
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 89.p [Qua20]

% Status   : Theorem
% Rating   : 0.47 v8.1.0, 0.56 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  293 (   1 equ)
%            Maximal formula atoms :   10 (   3 avg)
%            Number of connectives :  205 (   7   ~;   0   |; 103   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   20 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  532 ( 512   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL8110989,conjecture,
    ! [A,B,C,I,C1,A1,B1,P,Q,N] :
      ( ( eqangle(I,A,A,B,I,A,A,C)
        & eqangle(I,B,B,C,I,B,B,A)
        & eqangle(I,C,C,A,I,C,C,B)
        & midp(C1,B,A)
        & midp(A1,C,B)
        & midp(B1,A,C)
        & midp(P,I,B)
        & midp(Q,C,I)
        & circle(N,P,A1,Q) )
     => eqangle(B1,A1,A1,N,N,A1,A1,C1) ) ).

%------------------------------------------------------------------------------
