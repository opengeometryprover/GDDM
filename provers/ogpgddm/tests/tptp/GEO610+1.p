%------------------------------------------------------------------------------
% File     : GEO610+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 72
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 72.p [Qua20]

% Status   : Theorem
% Rating   : 0.47 v8.1.0, 0.53 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  301 (   1 equ)
%            Maximal formula atoms :   18 (   3 avg)
%            Number of connectives :  213 (   7   ~;   5   |; 106   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   17 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  530 ( 510   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL618072,conjecture,
    ! [A,B,C,D,E,F,G,NWPNT1] :
      ( ( circle(D,A,B,C)
        & circle(D,A,E,NWPNT1)
        & perp(F,E,A,C)
        & coll(F,A,C)
        & perp(G,E,A,B)
        & coll(G,A,B) )
     => ( eqangle(F,E,E,G,C,E,E,B)
        & ( eqangle(E,F,F,G,E,C,C,B)
          | eqangle(F,E,E,G,C,E,E,B) )
        & ( eqangle(E,F,F,G,E,B,B,C)
          | eqangle(F,E,E,G,E,C,C,B) )
        & ( eqangle(E,F,F,G,C,E,E,B)
          | eqangle(F,E,E,G,E,C,C,B) )
        & ( eqangle(E,F,F,G,E,B,B,C)
          | eqangle(F,E,E,G,E,B,B,C) )
        & ( eqangle(E,F,F,G,C,E,E,B)
          | eqangle(F,E,E,G,E,B,B,C) )
        & eqangle(E,F,F,G,E,C,C,B) ) ) ).

%------------------------------------------------------------------------------
