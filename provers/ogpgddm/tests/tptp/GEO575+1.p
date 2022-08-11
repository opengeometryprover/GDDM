%------------------------------------------------------------------------------
% File     : GEO575+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 37
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 37.p [Qua20]

% Status   : Theorem
% Rating   : 0.42 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  292 (   1 equ)
%            Maximal formula atoms :    9 (   3 avg)
%            Number of connectives :  204 (   7   ~;   0   |; 102   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   18 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  531 ( 511   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL214037,conjecture,
    ! [A,B,C,O,H,C1,A1,NWPNT1,NWPNT2] :
      ( ( circle(O,A,B,C)
        & perp(A,B,C,H)
        & perp(A,C,B,H)
        & perp(B,C,A,H)
        & circle(O,C,C1,NWPNT1)
        & coll(C1,C,H)
        & circle(O,A,A1,NWPNT2)
        & coll(A1,A,H) )
     => cong(B,A1,B,C1) ) ).

%------------------------------------------------------------------------------
