%------------------------------------------------------------------------------
% File     : GEO638+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 104
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 104.p [Qua20]

% Status   : Theorem
% Rating   : 0.33 v8.1.0, 0.36 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  293 (   1 equ)
%            Maximal formula atoms :   10 (   3 avg)
%            Number of connectives :  205 (   7   ~;   0   |; 103   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   21 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  533 ( 513   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL81109104,conjecture,
    ! [A,B,E,G,D,C,F,H,NWPNT1,NWPNT2,NWPNT3] :
      ( ( midp(E,A,B)
        & circle(E,A,NWPNT1,NWPNT2)
        & circle(E,A,G,NWPNT3)
        & perp(E,G,G,D)
        & para(A,D,C,B)
        & coll(C,G,D)
        & midp(F,C,D)
        & perp(H,F,A,B)
        & coll(H,A,B) )
     => perp(C,H,D,H) ) ).

%------------------------------------------------------------------------------
