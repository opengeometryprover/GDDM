%------------------------------------------------------------------------------
% File     : GEO561+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 22
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 22.p [Qua20]

% Status   : Theorem
% Rating   : 0.39 v8.1.0, 0.47 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  297 (   1 equ)
%            Maximal formula atoms :   14 (   3 avg)
%            Number of connectives :  209 (   7   ~;   0   |; 107   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   26 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  534 ( 514   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL214022,conjecture,
    ! [A,B,C,O,P,Q,N,M,E,D,NWPNT1,NWPNT2] :
      ( ( circle(O,A,B,C)
        & perp(P,O,A,C)
        & coll(P,A,C)
        & perp(Q,O,A,B)
        & coll(Q,A,B)
        & coll(N,O,P)
        & circle(O,A,N,NWPNT1)
        & coll(M,O,Q)
        & circle(O,A,M,NWPNT2)
        & coll(E,A,C)
        & coll(E,N,M)
        & coll(D,A,B)
        & coll(D,N,M) )
     => eqangle(A,D,D,E,D,E,E,A) ) ).

%------------------------------------------------------------------------------
