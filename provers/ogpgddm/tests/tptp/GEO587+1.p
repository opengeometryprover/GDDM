%------------------------------------------------------------------------------
% File     : GEO587+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 49
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 49.p [Qua20]

% Status   : Theorem
% Rating   : 0.42 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  291 (   1 equ)
%            Maximal formula atoms :    8 (   3 avg)
%            Number of connectives :  203 (   7   ~;   0   |; 101   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   17 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  531 ( 511   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL416049,conjecture,
    ! [A,C,B,D,P,Q,R,S,O] :
      ( ( perp(A,C,B,D)
        & midp(P,B,A)
        & midp(Q,C,B)
        & midp(R,D,C)
        & midp(S,A,D)
        & coll(O,P,R)
        & coll(O,Q,S) )
     => cong(O,S,O,R) ) ).

%------------------------------------------------------------------------------
