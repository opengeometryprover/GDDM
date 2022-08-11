%------------------------------------------------------------------------------
% File     : GEO615+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 77
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 77.p [Qua20]

% Status   : Theorem
% Rating   : 0.44 v8.1.0, 0.50 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  295 (   1 equ)
%            Maximal formula atoms :   12 (   3 avg)
%            Number of connectives :  207 (   7   ~;   0   |; 105   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   24 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   13 (  12 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  534 ( 514   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL618077,conjecture,
    ! [A,B,C,D,E,F,G,NWPNT1,NWPNT2,NWPNT3,NWPNT4,NWPNT5] :
      ( ( eqangle(D,A,A,B,D,A,A,C)
        & eqangle(D,B,B,C,D,B,B,A)
        & eqangle(D,C,C,A,D,C,C,B)
        & midpoint(NWPNT1,B,D)
        & perp(B,D,NWPNT1,E)
        & perp(A,B,B,E)
        & circle(E,B,NWPNT2,NWPNT3)
        & coll(F,A,C)
        & circle(E,B,F,NWPNT4)
        & circle(E,F,G,NWPNT5)
        & coll(G,F,A) )
     => eqangle(F,D,D,C,C,D,D,G) ) ).

%------------------------------------------------------------------------------
