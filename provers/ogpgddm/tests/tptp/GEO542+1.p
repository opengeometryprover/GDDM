%------------------------------------------------------------------------------
% File     : GEO542+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem 02
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : 02.p [Qua20]

% Status   : Theorem
% Rating   : 0.33 v8.1.0, 0.39 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  288 (   1 equ)
%            Maximal formula atoms :    8 (   3 avg)
%            Number of connectives :  200 (   7   ~;   0   |;  98   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   15 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  529 ( 509   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULL012002,conjecture,
    ! [A,B,C,A1,B1,C1,O] :
      ( ( midp(A1,C,B)
        & midp(B1,C,A)
        & midp(C1,B,A)
        & circle(O,A,B,C) )
     => perp(O,A1,B1,C1) ) ).

%------------------------------------------------------------------------------
