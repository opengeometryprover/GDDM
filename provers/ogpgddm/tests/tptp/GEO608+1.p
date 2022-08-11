%------------------------------------------------------------------------------
% File     : GEO608+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 70
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 70.p [Qua20]

% Status   : Theorem
% Rating   : 0.33 v8.1.0, 0.42 v7.5.0
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
fof(exemplo6GDDFULL618070,conjecture,
    ! [P,A,B,O,A1,B1,O1,NWPNT1,NWPNT2,NWPNT3,NWPNT4] :
      ( ( midp(O,B,A)
        & circle(O,A,NWPNT1,NWPNT2)
        & coll(A1,P,A)
        & circle(O,A,A1,NWPNT3)
        & coll(B1,P,B)
        & circle(O,A,B1,NWPNT4)
        & circle(O1,P,A1,B1) )
     => perp(O,A1,A1,O1) ) ).

%------------------------------------------------------------------------------
