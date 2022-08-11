%------------------------------------------------------------------------------
% File     : GEO606+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 68
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 68.p [Qua20]

% Status   : Theorem
% Rating   : 0.42 v8.1.0, 0.44 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  296 (   1 equ)
%            Maximal formula atoms :   13 (   3 avg)
%            Number of connectives :  208 (   7   ~;   0   |; 106   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   31 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  540 ( 520   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL618068,conjecture,
    ! [A,B,P,D,E,I,F,G,NWPNT1,NWPNT2,NWPNT3,NWPNT4,NWPNT5,NWPNT6,NWPNT7,NWPNT8,NWPNT9,NWPNT01] :
      ( ( perp(P,A,P,B)
        & circle(A,P,NWPNT1,NWPNT2)
        & circle(B,P,NWPNT3,NWPNT4)
        & circle(A,P,D,NWPNT5)
        & circle(A,D,E,NWPNT6)
        & coll(E,D,A)
        & circle(A,P,I,NWPNT7)
        & circle(B,P,I,NWPNT8)
        & coll(F,D,I)
        & circle(B,P,F,NWPNT9)
        & circle(B,F,G,NWPNT01)
        & coll(G,F,B) )
     => perp(G,F,D,E) ) ).

%------------------------------------------------------------------------------
