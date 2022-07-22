# OGP Geometry Deductive Database Method (GDDM)

**Notice.** This prover is in a pre-alpha stage.  The authors are actively developing this prover, updating it (almost) daily.  Until a stable version is made avalable, undesired behaviour may happen.

---

This prover implements the deductive database approach as described in

Shang-Ching Chou, Xiao-Shan Gao, Jing-Zhong Zhang; *A Deductive Database Approach to Automated Geometry Theorem proving and Discovering*; Journal of Automated Reasoning 25: 219--246, 2000.


**Compilation and Running**

The prover is being developed in a GNU/Linux environment.  The tools used to compile it are:
[GNU Make](https://www.gnu.org/software/make/), [GNU C++ Compiler](https://gcc.gnu.org/), [flex](https://github.com/westes/flex) and [Bison](https://www.gnu.org/software/bison/).  To compile it type

    $ cd src
    $ make

To use the prover type

    $ ./ogpgddm CONJECTURE 

where `CONJECTURE` is a file with a conjecture written in [TPTP](https://tptp.org/) First Order Format.

**Authors**

- Nuno Baeta ([GitHub](https://github.com/nmsbaeta));
- Pedro Quaresma ([GitHub](https://github.com/GeoTiles) | [Homepage](http://www.mat.uc.pt/~pedro/)).
