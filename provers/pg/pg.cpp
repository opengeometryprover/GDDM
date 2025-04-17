/*
 * pg.cpp
 *
 * Given a set of geometric axioms in the first order form (FOF), as described
 * in TPTP, generates the corresponding C++ code of a GDDM prover.
 *
 * This file is part of 'pg', the OGP Provers-Generator (PG) for the Geometry
 * Deductive Database Method (GDDM).
 *
 * Copyright (C) 2025 Nuno, Pedro Quaresma
 * Distributed under GNU GPL 3.0 or later.
 */


#include <cstring>
#include <iostream>
#include <list>
#include <string>

#include "parser.h"

#include "pg.hpp"
#include "axioms.hpp"
#include "generate.hpp"
#include "warnerr.hpp"

Axiom_list axioms;

extern FILE *yyin;

/*
 * Given a set of geometric axioms in the first order form, as described
 * in TPTP, builds the corresponding C++ code.
 */
int main(int argc, char *argv[])
{
    std::string gentxt;
    FILE *outfile;

    if (argc < 2) {
	error(ERROR_MISSING_ARGUMENT, "");
    } else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
	std::cout << PG_ID;
	std::cout << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << "\tpg [option | axioms]" << std::endl;
	std::cout << "where option is one of:" << std::endl;
	std::cout << "\t-h | --help\tthis help" << std::endl;
	std::cout << "\t-V | --version\tbuilder's version" << std::endl;
	std::cout << "and" << std::endl;
	std::cout << "\taxioms\t\tfile with the list of axioms" << std::endl;
    } else if (!strcmp(argv[1], "-V") || !strcmp(argv[1], "--version")) {
	std::cout << PG_ID;
    } else {
	if (argc > 2)
	    warning(WARNING_EXCESS_ARGUMENTS, "");
	// Parse axiom file
	if ((yyin = fopen(argv[1], "r")) == NULL)
	    error(ERROR_UNABLE_OPEN_FILE_READ, argv[1]);
	yyparse();
	fclose(yyin);
	// Generate 'prover.hpp'
	gentxt = generate_prover_hpp();
	if ((outfile = fopen("prover.hpp", "w")) == NULL)
	    error(ERROR_UNABLE_OPEN_FILE_WRITE, "prover.hpp");
	fprintf(outfile, "%s", gentxt.c_str());
	fclose(outfile);
	// Generate 'prover.cpp'
	gentxt = generate_prover_cpp();
	if ((outfile = fopen("prover.cpp", "w")) == NULL)
	    error(ERROR_UNABLE_OPEN_FILE_WRITE, "prover.cpp");
	fprintf(outfile, "%s", gentxt.c_str());
	fclose(outfile);
	// Generate 'Makefile'
	gentxt = generate_makefile();
	if ((outfile = fopen("Makefile", "w")) == NULL)
	    error(ERROR_UNABLE_OPEN_FILE_WRITE, "Makefile");
	fprintf(outfile, "%s", gentxt.c_str());
	fclose(outfile);
	// Generate 'dbRAM.hpp'
	gentxt = generate_dbram_hpp();
	if ((outfile = fopen("dbRAM.hpp", "w")) == NULL)
	    error(ERROR_UNABLE_OPEN_FILE_WRITE, "dbRAM.hpp");
	fprintf(outfile, "%s", gentxt.c_str());
	fclose(outfile);
	// Generate 'foftodb.hpp'
	gentxt = generate_foftodb_hpp();
	if ((outfile = fopen("foftodb.hpp", "w")) == NULL)
	    error(ERROR_UNABLE_OPEN_FILE_WRITE, "foftodb.hpp");
	fprintf(outfile, "%s", gentxt.c_str());
	fclose(outfile);
	// Generate 'ogpgddm.cpp'
	gentxt = generate_ogpgddm();
	if ((outfile = fopen("ogpgddm.cpp", "w")) == NULL)
	    error(ERROR_UNABLE_OPEN_FILE_WRITE, "ogpgddm.cpp");
	fprintf(outfile, "%s", gentxt.c_str());
	fclose(outfile);
	// Generate 'parser.yy'
	gentxt = generate_parser();
	if ((outfile = fopen("parser.yy", "w")) == NULL)
	    error(ERROR_UNABLE_OPEN_FILE_WRITE, "parser.yy");
	fprintf(outfile, "%s", gentxt.c_str());
	fclose(outfile);
	// Generate 'scanner.ll'
	gentxt = generate_scanner();
	if ((outfile = fopen("scanner.ll", "w")) == NULL)
	    error(ERROR_UNABLE_OPEN_FILE_WRITE, "scanner.ll");
	fprintf(outfile, "%s", gentxt.c_str());
	fclose(outfile);
	// Generate 'strs.hpp'
	gentxt = generate_strs_hpp();
	if ((outfile = fopen("strs.hpp", "w")) == NULL)
	    error(ERROR_UNABLE_OPEN_FILE_WRITE, "strs.hpp");
	fprintf(outfile, "%s", gentxt.c_str());
	fclose(outfile);
	// Generate 'strs.cpp'
	gentxt = generate_strs_cpp();
	if ((outfile = fopen("strs.cpp", "w")) == NULL)
	    error(ERROR_UNABLE_OPEN_FILE_WRITE, "strs.cpp");
	fprintf(outfile, "%s", gentxt.c_str());
	fclose(outfile);
	// Generate 'version.hpp'
	gentxt = generate_version_hpp();
	if ((outfile = fopen("version.hpp", "w")) == NULL)
	    error(ERROR_UNABLE_OPEN_FILE_WRITE, "version.hpp");
	fprintf(outfile, "%s", gentxt.c_str());
	fclose(outfile);
    }
    return EXIT_SUCCESS;
}
