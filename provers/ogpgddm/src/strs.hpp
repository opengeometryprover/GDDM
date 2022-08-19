/*
 * strs.hpp
 *
 * To deal with lists of strings, whether, e.g. point or predictes.
 *
 * This file is part of the OGP GDDM prover, which, in turn, is part of
 * the Open Geometry Prover Community Project (OGPCP)
 * <https://github.com/opengeometryprover>.
 *
 * Copyright (C) 2022 Nuno Baeta, Pedro Quaresma
 * Distributed under GNU GPL 3.0 or later
 */


#ifndef STRS
#define STRS

struct strsList {
    std::string str;
    struct strsList *next;
};

struct strsList *addStr(std::string, struct strsList *);
void showStrs(struct strsList *);

#endif
