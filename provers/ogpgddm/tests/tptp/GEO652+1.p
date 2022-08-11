%------------------------------------------------------------------------------
% File     : GEO652+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem E022-13
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : E022-13.p [Qua20]

% Status   : Theorem
% Rating   : 0.42 v8.1.0, 0.47 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  298 (   1 equ)
%            Maximal formula atoms :   15 (   3 avg)
%            Number of connectives :  210 (   7   ~;   0   |; 108   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   37 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  544 ( 524   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULLmoreE02213,conjecture,
    ! [A,B,C,D,E,F,G,H,I,J,NWPNT1,NWPNT2,NWPNT3,NWPNT4,NWPNT5,NWPNT6,NWPNT7,NWPNT8,NWPNT9,NWPNT01,NWPNT11,NWPNT21] :
      ( ( circle(A,B,NWPNT1,NWPNT2)
        & circle(C,B,NWPNT3,NWPNT4)
        & circle(A,B,D,NWPNT5)
        & circle(C,B,D,NWPNT6)
        & circle(C,B,E,NWPNT7)
        & circle(C,B,F,NWPNT8)
        & coll(D,E,G)
        & circle(A,D,G,NWPNT9)
        & coll(B,F,H)
        & circle(A,B,H,NWPNT01)
        & coll(D,H,I)
        & circle(C,D,I,NWPNT11)
        & coll(B,G,J)
        & circle(C,B,J,NWPNT21) )
     => para(E,F,I,J) ) ).

%------------------------------------------------------------------------------
