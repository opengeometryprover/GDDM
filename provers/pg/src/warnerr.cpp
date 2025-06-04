/*
 * warnerr.cpp
 *
 * Errors and warnings management.
 *
 * This file is part 'pg', the OGP Provers-Generator (PG) for the Geometry
 * Deductive Database Method (GDDM).
 *
 * Copyright (C) 2025 Nuno, Pedro Quaresma
 * Distributed under GNU GPL 3.0 or later.
 */


#include <iostream>

#include "warnerr.hpp"

void warning(int warning_nr, std::string warning_msg)
{
	std::cerr << "[pg:WARNING:" + std::to_string(warning_nr) + "] ";
	switch (warning_nr) {
	case WARNING_EXCESS_ARGUMENTS:
		std::cerr << "Too many arguments. Arguments in excess ignored.";
	}
	std::cerr << std::endl;
}

int error(int error_nr, std::string error_msg)
{
	std::cerr << "[pg:ERROR:" + std::to_string(error_nr) + "] ";
	switch (error_nr) {
	case ERROR_MISSING_ARGUMENT:
		std::cerr << "Argument missing. Type 'builder -h' for help.";
		break;
	case ERROR_UNABLE_OPEN_FILE_READ:
		std::cerr << "Unable to open file '" + error_msg + "' for reading.";
		break;
	case ERROR_UNABLE_OPEN_FILE_WRITE:
		std::cerr << "Unable to open file '" + error_msg + "' for writing.";
		break;
	case ERROR_UNABLE_ALLOCATE_MEMORY:
		std::cerr << "Unable to allocate memory.";
		break;
	case ERROR_EXCESS_PREDICATES:
		std::cerr << "Code generation for antecedents with " + error_msg;
		std::cerr << " predicates not implemented.";
		break;
	}
	std::cerr << std::endl;
	exit(error_nr);
}
