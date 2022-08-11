%------------------------------------------------------------------------------
% File     : GEO639+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 105
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 105.p [Qua20]

% Status   : Theorem
% Rating   : 0.47 v8.1.0, 0.56 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  299 (   1 equ)
%            Maximal formula atoms :   16 (   3 avg)
%            Number of connectives :  211 (   7   ~;   1   |; 108   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   31 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  538 ( 518   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL81109105,conjecture,
    ! [P,Q,A,N,D,O,B,C,E,NWPNT1,NWPNT2,NWPNT3,NWPNT4,NWPNT5,NWPNT6,NWPNT7] :
      ( ( ( cong(A,P,P,Q)
          | cong(A,P,A,Q) )
        & perp(P,A,P,N)
        & perp(Q,A,Q,N)
        & circle(N,P,NWPNT1,NWPNT2)
        & coll(D,A,N)
        & circle(N,P,D,NWPNT3)
        & midp(O,D,A)
        & circle(O,D,NWPNT4,NWPNT5)
        & coll(B,P,A)
        & circle(O,D,B,NWPNT6)
        & coll(C,Q,A)
        & circle(O,D,C,NWPNT7)
        & coll(E,P,Q)
        & coll(E,A,N) )
     => eqangle(A,B,B,E,E,B,B,C) ) ).

%------------------------------------------------------------------------------
