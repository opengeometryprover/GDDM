/*
 * dbRAM.hpp
 *
 * Open, create and close the (in memory) SQLite database.
 *
 * This file is part of the OGP GDDM prover, which, in turn, is part of
 * the Open Geometry prover Community Project (OGPCP)
 * <https://github.com/opengeometryprover>.
 *
 * Copyright (C) 2022 Nuno Baeta, Pedro Quaresma
 * Distributed under GNU GPL 3.0 or later
 */


#ifndef DBINMEMORY
#define DBINMEMORY

#include <sqlite3.h>

#include <iostream>
#include <map>

class DBinMemory {
    friend class Prover;
    friend class FOFtoDB;
protected:
    // SQLite variables
    int rc;
    char *zErrMsg = 0;
    sqlite3 *db;
    sqlite3_stmt *stmt, *stmt1, *stmt2;   

    int res = 0;
    std::map<std::string, int> geoCmds = {{"coll", 1},
					  {"para", 2},
					  {"perp", 3},
					  {"midp", 4},
					  {"circle", 5},
					  {"cong", 6},
					  {"contri", 7},
                                          {"cyclic", 8},
                                          {"eqangle", 9},
                                          {"eqratio", 10},
                                          {"simtri", 11}};
	
public:
  void openInMemoryDB();	// Open database (in memory)
  void createDBforGDDM();// Create database (in memory)
  void closeDB();		// Close (in memory) database
  int backupDb(const char *,void (*f)(int,int)); // backup DB in file 
};

#endif
