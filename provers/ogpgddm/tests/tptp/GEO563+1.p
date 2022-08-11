%------------------------------------------------------------------------------
% File     : GEO563+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 24
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 24.p [Qua20]

% Status   : Theorem
% Rating   : 0.42 v8.1.0, 0.50 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  291 (   1 equ)
%            Maximal formula atoms :    8 (   3 avg)
%            Number of connectives :  203 (   7   ~;   0   |; 101   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   19 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  533 ( 513   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL214024,conjecture,
    ! [Q,R,P,O1,S,Y,O,X,I,NWPNT1,NWPNT2] :
      ( ( circle(O1,Q,R,P)
        & circle(O1,Q,S,NWPNT1)
        & coll(Y,Q,S)
        & circle(O,Y,P,Q)
        & circle(O,Q,X,NWPNT2)
        & coll(I,R,S)
        & coll(I,Y,X) )
     => eqangle(R,I,I,X,R,P,P,X) ) ).

%------------------------------------------------------------------------------
