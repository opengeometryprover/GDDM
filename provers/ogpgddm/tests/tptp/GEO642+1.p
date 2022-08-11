%------------------------------------------------------------------------------
% File     : GEO642+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 108
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 108.p [Qua20]

% Status   : Theorem
% Rating   : 0.33 v8.1.0, 0.39 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  297 (   1 equ)
%            Maximal formula atoms :   14 (   3 avg)
%            Number of connectives :  209 (   7   ~;   0   |; 107   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   31 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  539 ( 519   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL81109108,conjecture,
    ! [A,B,O,C,P,D,G,E,F,NWPNT1,NWPNT2,NWPNT3,NWPNT4,NWPNT5,NWPNT6,NWPNT7,NWPNT8] :
      ( ( midp(O,B,A)
        & circle(O,A,NWPNT1,NWPNT2)
        & circle(O,A,C,NWPNT3)
        & circle(O,A,P,NWPNT4)
        & circle(C,B,NWPNT5,NWPNT6)
        & circle(O,A,D,NWPNT7)
        & circle(C,B,D,NWPNT8)
        & coll(G,B,P)
        & coll(G,A,D)
        & coll(E,A,C)
        & coll(E,B,P)
        & coll(F,A,D)
        & coll(F,C,P) )
     => perp(E,F,A,D) ) ).

%------------------------------------------------------------------------------
