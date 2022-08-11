%------------------------------------------------------------------------------
% File     : GEO654+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem E022-8
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : E022-8.p [Qua20]

% Status   : Theorem
% Rating   : 0.42 v8.1.0, 0.44 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  294 (   1 equ)
%            Maximal formula atoms :   11 (   3 avg)
%            Number of connectives :  206 (   7   ~;   0   |; 104   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   28 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  539 ( 519   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULLmoreE0228,conjecture,
    ! [A,B,C,D,E,F,G,H,NWPNT1,NWPNT2,NWPNT3,NWPNT4,NWPNT5,NWPNT6,NWPNT7,NWPNT8,NWPNT9] :
      ( ( circle(A,B,NWPNT1,NWPNT2)
        & circle(C,B,NWPNT3,NWPNT4)
        & circle(A,B,D,NWPNT5)
        & circle(C,B,D,NWPNT6)
        & circle(C,B,E,NWPNT7)
        & coll(D,E,F)
        & circle(A,D,F,NWPNT8)
        & coll(B,E,G)
        & circle(A,B,G,NWPNT9)
        & perp(C,E,E,H) )
     => para(H,E,G,F) ) ).

%------------------------------------------------------------------------------
