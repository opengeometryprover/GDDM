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


#include <iostream>
#include <string>

#include "strs.hpp"

/*
 * Adds a string to the list, if it does not belong to the list.
 */
struct strsList *addStr(std::string theStr, struct strsList *strs) {
    bool strExists;
    struct strsList *current, *newStr;

    strExists = false;
    current = strs;
    while (current != NULL && !strExists) {
	strExists = current->str == theStr;
	current = current->next;
    }
    if (!strExists) {
	newStr = new strsList;
	newStr->str = theStr;
	newStr->next = strs;
	strs = newStr;
    }
    return strs;
}

/*
 * For debugging purposes only.
 */
void showStrs(struct strsList *strs) {
    struct strsList *current;

    std::cout << "List of Stringss" << std::endl;
    std::cout << std::endl;
    current = strs;
    while (current != NULL) {
	std::cout << "  " << current->str << std::endl;
	current = current->next;
    }
    std::cout << std::endl;
}
