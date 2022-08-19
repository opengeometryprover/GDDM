/*
 * points.hpp
 *
 * Conjecture's lists of strings
 *
 * This file is part of the OGP GDDM prover, which, in turn, is part of
 * the Open Geometry Prover Community Project (OGPCP)
 * <https://github.com/opengeometryprover>.
 *
 * Copyright (C) 2022 Nuno Baeta, Pedro Quaresma
 * Distributed under GNU GPL 3.0 or later
 */


#include <iostream>
#include <string>

#include "strs.hpp"

/*
 * Adds a point to the list, if it does not belong to the list.
 */
struct pointList *addPoint(std::string point, struct pointList *points) {
    bool pointExists;
    struct pointList *current, *newPt;

    pointExists = false;
    current = points;
    while (current != NULL && !pointExists) {
	pointExists = current->pt == point;
	current = current->next;
    }
    if (!pointExists) {
	newPt = new pointList;
	newPt->pt = point;
	newPt->next = points;
	points = newPt;
    }
    return points;
}

/*
 * For debugging purposes only.
 */
void showPoints(struct pointList *points) {
    struct pointList *aux;

    std::cout << "List of Points" << std::endl;
    std::cout << std::endl;
    aux = points;
    while (aux != NULL) {
	std::cout << "  " << aux->pt << std::endl;
	aux = aux->next;
    }
    std::cout << std::endl;
}
