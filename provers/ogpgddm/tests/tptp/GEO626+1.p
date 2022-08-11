%------------------------------------------------------------------------------
% File     : GEO626+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 90
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 90.p [Qua20]

% Status   : Theorem
% Rating   : 0.42 v8.1.0, 0.50 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  294 (   1 equ)
%            Maximal formula atoms :   11 (   3 avg)
%            Number of connectives :  206 (   7   ~;   0   |; 104   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   23 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  534 ( 514   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL8110990,conjecture,
    ! [A,B,C,O,D,G,F,C1,B1,NWPNT1,NWPNT2,NWPNT3] :
      ( ( circle(O,A,B,C)
        & circle(O,A,D,NWPNT1)
        & perp(G,D,A,B)
        & coll(G,A,B)
        & perp(F,D,A,C)
        & coll(F,A,C)
        & circle(O,D,C1,NWPNT2)
        & coll(C1,D,G)
        & circle(O,D,B1,NWPNT3)
        & coll(B1,D,F) )
     => para(C1,C,B1,B) ) ).

%------------------------------------------------------------------------------
