/*
 * axioms.hpp
 *
 * Definition of types to describe the axioms and functions, mainly for
 * debugging purposes, to print a predicate, an axiom and a list of axioms.
 *
 * This file is part of 'pg', the OGP Provers-Generator (PG) for the Geometry
 * Deductive Database Method (GDDM).
 *
 * Copyright (C) 2025 Nuno, Pedro Quaresma
 * Distributed under GNU GPL 3.0 or later.
 */


#include <iostream>

#include "axioms.hpp"

void print_predicate(Predicate pred)
{
    bool comma = false;

    std::cout << pred.name << "(";
    for (std::string pt : pred.points) {
	if (comma) {
	    std::cout << ", ";
	}
	std::cout << pt;
	comma = true;
    }
    std::cout << ")";
}

void print_axiom(Axiom ax)
{
    bool ca;

    std::cout << "       Name: " + ax.name << std::endl;
    ca = false;
    std::cout << "     Points: ";
    for (std::string pt : ax.points) {
	if (ca) {
	    std::cout << ", ";
	}
	std::cout << pt;
	ca = true;
    }
    std::cout << std::endl;
    std::cout << "Antecedents: ";
    ca = false;
    for (Predicate p : ax.antecedents) {
	if (ca) {
	    std::cout << " & ";
	}
	print_predicate(p);
	ca = true;
    }
    std::cout << std::endl;
    std::cout << "Consequence: ";
    print_predicate(ax.consequence);
    std::cout << std::endl;
}

void print_axiom_list(Axiom_list axioms)
{
    for (Axiom ax : axioms) {
	std::cout << "-----  Axiom  -----" << std::endl;
	print_axiom(ax);
    }
}
