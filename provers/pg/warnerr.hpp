/*
 * warnerr.hpp
 *
 * Errors and warnings management, including macros defining their
 * identification number.
 *
 * This file is part of 'pg', the OGP Provers-Generator (PG) for the Geometry
 * Deductive Database Method (GDDM).
 *
 * Copyright (C) 2025 Nuno, Pedro Quaresma
 * Distributed under GNU GPL 3.0 or later.
 */


#ifndef WARNERR
#define WARNERR

#define WARNING_EXCESS_ARGUMENTS 20

#define ERROR_MISSING_ARGUMENT 10
#define ERROR_UNABLE_OPEN_FILE_READ 11
#define ERROR_UNABLE_OPEN_FILE_WRITE 12
#define ERROR_UNABLE_ALLOCATE_MEMORY 13
#define ERROR_EXCESS_PREDICATES 14

#include <string>

void warning(int, std::string);
int error(int, std::string);

#endif
