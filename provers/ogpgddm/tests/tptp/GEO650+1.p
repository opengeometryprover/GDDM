%------------------------------------------------------------------------------
% File     : GEO650+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem E022-11
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : E022-11.p [Qua20]

% Status   : Theorem
% Rating   : 0.44 v8.1.0, 0.47 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  296 (   1 equ)
%            Maximal formula atoms :   13 (   3 avg)
%            Number of connectives :  208 (   7   ~;   0   |; 106   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   23 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  532 ( 512   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULLmoreE02211,conjecture,
    ! [A,B,C,D,E,F,G,MIDPNT1,MIDPNT2,MIDPNT3] :
      ( ( midp(MIDPNT1,A,B)
        & perp(A,B,MIDPNT1,D)
        & midp(MIDPNT2,A,C)
        & perp(A,C,MIDPNT2,D)
        & midp(MIDPNT3,B,C)
        & perp(B,C,MIDPNT3,D)
        & perp(C,D,C,E)
        & perp(B,D,B,E)
        & perp(F,D,A,B)
        & coll(F,A,B)
        & coll(G,D,F)
        & coll(G,A,C) )
     => para(G,E,A,B) ) ).

%------------------------------------------------------------------------------
