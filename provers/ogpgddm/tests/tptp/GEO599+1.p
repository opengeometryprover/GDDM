%------------------------------------------------------------------------------
% File     : GEO599+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 61
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 61.p [Qua20]

% Status   : Theorem
% Rating   : 0.53 v8.1.0, 0.56 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  301 (   1 equ)
%            Maximal formula atoms :   18 (   3 avg)
%            Number of connectives :  213 (   7   ~;   5   |; 106   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   19 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  532 ( 512   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL618061,conjecture,
    ! [A,B,C,D,E,F,G,NWPNT1,NWPNT2,NWPNT3] :
      ( ( circle(D,A,B,C)
        & midp(E,B,A)
        & coll(F,D,E)
        & circle(F,A,NWPNT1,NWPNT2)
        & coll(G,B,C)
        & circle(F,A,G,NWPNT3) )
     => ( eqangle(D,A,A,F,C,A,A,G)
        & ( eqangle(A,D,D,F,A,C,C,G)
          | eqangle(D,A,A,F,C,A,A,G) )
        & ( eqangle(A,D,D,F,A,G,G,C)
          | eqangle(D,A,A,F,A,C,C,G) )
        & ( eqangle(A,D,D,F,C,A,A,G)
          | eqangle(D,A,A,F,A,C,C,G) )
        & ( eqangle(A,D,D,F,A,G,G,C)
          | eqangle(D,A,A,F,A,G,G,C) )
        & ( eqangle(A,D,D,F,C,A,A,G)
          | eqangle(D,A,A,F,A,G,G,C) )
        & eqangle(A,D,D,F,A,C,C,G) ) ) ).

%------------------------------------------------------------------------------
