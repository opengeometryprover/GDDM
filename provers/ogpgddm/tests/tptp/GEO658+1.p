%------------------------------------------------------------------------------
% File     : GEO658+1 : TPTP v8.1.0. Released v7.5.0.
% Domain   : Geometry
% Problem  : JGEX problem E023-16
% Version  : [CGZ00] axioms.
% English  :

% Refs     : [CGZ00] Chou et al. (2000), A Deductive Database Approach to A
%          : [YCG08] Ye et al. (2008), An Introduction to Java Geometry Exp
%          : [Qua20] Quaresma (2020), Email to Geoff Sutcliffe
% Source   : [Qua20]
% Names    : E023-16.p [Qua20]

% Status   : Theorem
% Rating   : 0.39 v8.1.0, 0.42 v7.5.0
% Syntax   : Number of formulae    :   95 (   0 unt;   0 def)
%            Number of atoms       :  295 (   1 equ)
%            Maximal formula atoms :   12 (   3 avg)
%            Number of connectives :  207 (   7   ~;   0   |; 105   &)
%                                         (   0 <=>;  95  =>;   0  <=;   0 <~>)
%            Maximal formula depth :   21 (   9 avg)
%            Maximal term depth    :    1 (   1 avg)
%            Number of predicates  :   12 (  11 usr;   0 prp; 2-8 aty)
%            Number of functors    :    0 (   0 usr;   0 con; --- aty)
%            Number of variables   :  531 ( 511   !;  20   ?)
% SPC      : FOF_THM_RFO_SEQ

% Comments : Taken from JGEX [YCG08], converted by Pedro Quaresma.
%------------------------------------------------------------------------------
include('Axioms/GEO012+0.ax').
%------------------------------------------------------------------------------
fof(exemplo6GDDFULLmoreE02316,conjecture,
    ! [A,B,C,D,E,F,G,H,I] :
      ( ( para(A,B,C,D)
        & para(A,C,B,D)
        & coll(E,A,C)
        & perp(F,E,C,D)
        & coll(F,C,D)
        & perp(G,E,B,C)
        & coll(G,B,C)
        & coll(H,E,G)
        & coll(H,A,D)
        & coll(I,E,F)
        & coll(I,A,B) )
     => para(G,F,H,I) ) ).

%------------------------------------------------------------------------------
