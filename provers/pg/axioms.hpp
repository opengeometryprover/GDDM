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


#ifndef AXIOMS
#define AXIOMS

#include <list>
#include <set>

typedef struct predicate {
    std::string name;
    std::list<std::string> points;
} Predicate;

typedef struct axiom {
    std::string name;
    std::set<std::string> points;
    std::list<predicate> antecedents;
    predicate consequence;
} Axiom;

typedef std::list<Axiom> Axiom_list;

void print_predicate(Predicate);
void print_axiom(Axiom);
void print_axiom_list(Axiom_list);

#endif
