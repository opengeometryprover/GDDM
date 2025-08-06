# OGP Provers-Generator (PG) for the Geometry Deductive Database Method (GDDM)

Given a set of rules/axioms, OGP prover-generator (PG) will generate the code of a Geometry Deductive Database Method (GDDM) prover, approach described in

Shang-Ching Chou, Xiao-Shan Gao, Jing-Zhong Zhang; *A Deductive Database Approach to Automated Geometry Theorem proving and Discovering*; Journal of Automated Reasoning 25: 219--246, 2000.


**Compilation and Running**

PG is being developed in a GNU/Linux environment.  The tools used to compile it are:
[GNU Make](https://www.gnu.org/software/make/), [GNU C++ Compiler](https://gcc.gnu.org/), [flex](https://github.com/westes/flex) and [Bison](https://www.gnu.org/software/bison/).  To compile it type:

    $ make

To use PG, copy the executable `pg` and the axiom set, e.g. `RULES`, to a new directory - otherwise some of the source code of PG will be overwritten -, and run it:

    $ ./pg RULES

The source code of a new GATP will be produced.


**Authors**

- Nuno Baeta ([GitHub](https://github.com/nmsbaeta) | [Homepage](https://nmsb.codeberg.page/));
- Pedro Quaresma ([GitHub](https://github.com/GeoTiles) | [Homepage](http://www.mat.uc.pt/~pedro/)).
