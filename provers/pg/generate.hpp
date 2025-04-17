/*
 * generate.hpp
 *
 * Functions to generate code.
 *
 * This file is part of 'pg', the OGP Provers-Generator (PG) for the Geometry
 * Deductive Database Method (GDDM).
 *
 * Copyright (C) 2025 Nuno, Pedro Quaresma
 * Distributed under GNU GPL 3.0 or later.
 */


#ifndef GENERATE
#define GENERATE

std::string generate_prover_hpp();
std::string generate_prover_cpp();

std::string generate_makefile();
std::string generate_strs_hpp();
std::string generate_strs_cpp();
std::string generate_version_hpp();

#endif
