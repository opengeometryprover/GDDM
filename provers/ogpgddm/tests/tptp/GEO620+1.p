%------------------------------------------------------------------------------
% File     : GEO620+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 82
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 82.p [Qua20]

% Status   : Theorem
% Rating   : 0.22 v8.1.0, 0.31 v7.5.0
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
fof(exemplo6GDDFULL8110982,conjecture,
    ! [A,B,C,O,I,E,J,L] :
      ( ( eqangle(O,A,A,B,O,A,A,C)
        & eqangle(O,B,B,C,O,B,B,A)
        & eqangle(O,C,C,A,O,C,C,B)
        & perp(I,C,A,O)
        & coll(I,A,O)
        & perp(A,O,A,E)
        & perp(J,C,A,E)
        & coll(J,A,E)
        & perp(L,C,B,O)
        & coll(L,B,O) )
     => coll(I,L,J) ) ).

%------------------------------------------------------------------------------
