%------------------------------------------------------------------------------
% File     : GEO634+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 97
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 97.p [Qua20]

% Status   : Theorem
% Rating   : 0.39 v8.1.0, 0.44 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  296 (   1 equ)
%            Maximal formula atoms :   13 (   3 avg)
%            Number of connectives :  208 (   7   ~;   0   |; 106   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   25 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  534 ( 514   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL8110997,conjecture,
    ! [A,B,C,O,A1,P,Q,S,D,G,NWPNT1,NWPNT2] :
      ( ( circle(O,A,B,C)
        & midp(A1,C,B)
        & circle(O,A,P,NWPNT1)
        & coll(P,A,A1)
        & eqangle(Q,A,A,B,C,A,A,A1)
        & circle(O,A,Q,NWPNT2)
        & coll(S,B,C)
        & coll(S,A,Q)
        & perp(D,Q,B,C)
        & coll(D,B,C)
        & perp(G,Q,A,B)
        & coll(G,A,B) )
     => perp(D,G,P,A) ) ).

%------------------------------------------------------------------------------
