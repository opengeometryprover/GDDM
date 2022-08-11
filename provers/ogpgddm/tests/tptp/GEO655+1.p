%------------------------------------------------------------------------------
% File     : GEO655+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem E022-9
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : E022-9.p [Qua20]

% Status   : Theorem
% Rating   : 0.39 v8.1.0, 0.44 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  295 (   1 equ)
%            Maximal formula atoms :   12 (   3 avg)
%            Number of connectives :  207 (   7   ~;   0   |; 105   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   23 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  533 ( 513   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULLmoreE0229,conjecture,
    ! [A,B,C,D,E,F,G,MIDPNT1,MIDPNT2,MIDPNT3,NWPNT4] :
      ( ( midp(MIDPNT1,B,A)
        & perp(B,A,MIDPNT1,D)
        & midp(MIDPNT2,B,C)
        & perp(B,C,MIDPNT2,D)
        & midp(MIDPNT3,A,C)
        & perp(A,C,MIDPNT3,D)
        & coll(E,A,C)
        & eqangle(C,B,B,E,E,B,B,A)
        & coll(B,E,F)
        & circle(D,B,F,NWPNT4)
        & perp(D,F,F,G) )
     => para(F,G,A,C) ) ).

%------------------------------------------------------------------------------
