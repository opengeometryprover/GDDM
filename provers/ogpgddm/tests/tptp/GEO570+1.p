%------------------------------------------------------------------------------
% File     : GEO570+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 32
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 32.p [Qua20]

% Status   : Theorem
% Rating   : 0.44 v8.1.0, 0.47 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  292 (   1 equ)
%            Maximal formula atoms :    9 (   3 avg)
%            Number of connectives :  204 (   7   ~;   0   |; 102   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   18 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  531 ( 511   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL214032,conjecture,
    ! [B,C,R,O,S,A,M,N,NWPNT1] :
      ( ( circle(O,B,C,R)
        & circle(O,B,S,NWPNT1)
        & coll(A,B,R)
        & coll(A,C,S)
        & perp(M,A,R,S)
        & coll(M,R,S)
        & perp(N,A,B,C)
        & coll(N,B,C) )
     => eqangle(B,A,A,M,N,A,A,C) ) ).

%------------------------------------------------------------------------------
