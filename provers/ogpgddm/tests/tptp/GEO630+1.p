%------------------------------------------------------------------------------
% File     : GEO630+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 93
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 93.p [Qua20]

% Status   : Theorem
% Rating   : 0.39 v8.1.0, 0.47 v7.5.0
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
fof(exemplo6GDDFULL8110993,conjecture,
    ! [A,B,C,O,D1,D,F,G,G1,F1,NWPNT1,NWPNT2] :
      ( ( circle(O,A,B,C)
        & circle(O,A,D1,NWPNT1)
        & circle(O,D1,D,NWPNT2)
        & coll(D,D1,O)
        & perp(F,D,A,C)
        & coll(F,A,C)
        & perp(G,D,A,B)
        & coll(G,A,B)
        & perp(G1,D1,A,B)
        & coll(G1,A,B)
        & perp(F1,D1,A,C)
        & coll(F1,A,C) )
     => perp(G1,F1,G,F) ) ).

%------------------------------------------------------------------------------
