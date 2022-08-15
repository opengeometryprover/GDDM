/*
 * points.hpp
 *
 * Conjecture's list of points.
 *
 * This file is part of the OGP GDDM prover, which, in turn, is part of
 * the Open Geometry Prover Community Project (OGPCP)
 * <https://github.com/opengeometryprover>.
 *
 * Copyright (C) 2022 Nuno Baeta, Pedro Quaresma
 * Distributed under GNU GPL 3.0 or later
 */


#ifndef POINTS
#define POINTS

struct pointList {
    std::string pt;
    struct pointList *next;
};

struct pointList *addPoint(std::string, struct pointList *);
void showPoints(struct pointList *);

#endif
