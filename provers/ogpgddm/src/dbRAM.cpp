/*
 * dbRAM.cpp
 *
 * Open, create and close the (in memory) SQLite database.
 *
 * This file is part of the OGP GDDM prover, which, in turn, is part of
 * the OGPCP <https://github.com/opengeometryprover>.
 *
 * Copyright (C) 2022 Nuno Baeta, Pedro Quaresma
 * Distributed under GNU GPL 3.0 or later
 */


#include <iostream>
#include <string>

#include "dbRAM.hpp"


/*
 * Open database (in memory).
 */
void DBinMemory::openInMemoryDB() {
    rc = sqlite3_open(":memory:", &db);
    if (rc) {
	std::cout << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
	exit(2);
    }
}

/*
 * Create database (in memory).
 */
void DBinMemory::createDBforGDDM() {
    std::string createTableNewFact;
    std::string createTableFacts;
    std::string createTableConsequent;
    std::string createTableCollinear;
    std::string createTableParallel;
    std::string createTablePerpendicular;
    std::string createTableMidpoint;
    std::string createTableCircle;
    std::string createTableCongruentSegments;
    std::string createTableCongruentTriangles;
    std::string createTableCyclic;
    std::string createTableEqualAngles;
    std::string createTableEqualRatios;
    std::string createTableSimilarTriangles;
    std::string deleteFromSQLiteSequence;
  
    // To be able to re-create the database, without any violation issue.
    sqlite3_prepare_v2(db, "PRAGMA foreign_keys = OFF", -1, &stmt, NULL);
    sqlite3_step(stmt);
  
    // The CREATE TABLE statements
    createTableNewFact = "CREATE TABLE IF NOT EXISTS 'NewFact' ('id' INTEGER NOT NULL, 'typeGeoCmd' TEXT NOT NULL, PRIMARY KEY('id' AUTOINCREMENT))";
    createTableFacts = "CREATE TABLE IF NOT EXISTS 'Facts' ('id' INTEGER NOT NULL, 'typeGeoCmd' TEXT NOT NULL, PRIMARY KEY('id'))";
    createTableConsequent = "CREATE TABLE IF NOT EXISTS 'Consequent' ('id' INTEGER NOT NULL, 'typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'point5' TEXT, 'point6' TEXT, 'point7' TEXT, 'point8' TEXT, PRIMARY KEY('id' AUTOINCREMENT))";
    createTableCollinear = "CREATE TABLE IF NOT EXISTS 'Collinear' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY('point1', 'point2', 'point3'))";
    createTableParallel = "CREATE TABLE IF NOT EXISTS 'Parallel' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER,FOREIGN KEY('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY('point1', 'point2', 'point3', 'point4'))";
    createTablePerpendicular = "CREATE TABLE IF NOT EXISTS 'Perpendicular' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'newFact' INTEGER,'oldFact' INTEGER, FOREIGN KEY('oldFact') REFERENCES 'Facts'('id'), PRIMARY KEY('point1', 'point2', 'point3', 'point4'), FOREIGN KEY('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL)";
    createTableMidpoint = "CREATE TABLE IF NOT EXISTS 'Midpoint' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY('point1', 'point2', 'point3'))";
    createTableCircle = "CREATE TABLE IF NOT EXISTS 'Circle' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY('point1', 'point2', 'point3', 'point4'))";
    createTableCongruentSegments = "CREATE TABLE IF NOT EXISTS 'CongruentSegments' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY('point1', 'point2', 'point3', 'point4'))";
    createTableCongruentTriangles = "CREATE TABLE IF NOT EXISTS 'CongruentTriangles' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'point5' TEXT, 'point6' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY('point1', 'point2', 'point3', 'point4', 'point5', 'point6'))";
    createTableCyclic = "CREATE TABLE IF NOT EXISTS 'Cyclic' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY('point1', 'point2', 'point3', 'point4'))";
    createTableEqualAngles = "CREATE TABLE IF NOT EXISTS 'EqualAngles' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'point5' TEXT, 'point6' TEXT, 'point7' TEXT, 'point8' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY('point1', 'point2', 'point3', 'point4', 'point5', 'point6', 'point7', 'point8'))";
    createTableEqualRatios = "CREATE TABLE IF NOT EXISTS 'EqualRatios' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'point5' TEXT, 'point6' TEXT, 'point7' TEXT, 'point8' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY('point1', 'point2', 'point3', 'point4', 'point5', 'point6', 'point7', 'point8'))";
    createTableSimilarTriangles = "CREATE TABLE IF NOT EXISTS 'SimilarTriangles' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'point5' TEXT, 'point6' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY('point1', 'point2', 'point3', 'point4', 'point5', 'point6'))";
    deleteFromSQLiteSequence = "DELETE FROM sqlite_sequence";
    
    // Open transactions
    sqlite3_exec(db, "begin;", 0, 0, &zErrMsg);
    rc = sqlite3_prepare_v2(db, createTableNewFact.c_str(),
			    createTableNewFact.size(), &stmt, NULL);
    sqlite3_step(stmt);
    rc = sqlite3_prepare_v2(db, createTableFacts.c_str(),
			    createTableFacts.size(), &stmt, NULL);
    sqlite3_step(stmt);
    rc = sqlite3_prepare_v2(db, createTableConsequent.c_str(),
			    createTableConsequent.size(), &stmt, NULL);
    sqlite3_step(stmt);
    rc = sqlite3_prepare_v2(db, createTableCollinear.c_str(),
			    createTableCollinear.size(), &stmt, NULL);
    sqlite3_step(stmt);
    rc = sqlite3_prepare_v2(db, createTableParallel.c_str(),
			    createTableParallel.size(), &stmt, NULL);
    sqlite3_step(stmt);
    rc = sqlite3_prepare_v2(db, createTablePerpendicular.c_str(),
			    createTablePerpendicular.size(), &stmt, NULL);
    sqlite3_step(stmt);
    rc = sqlite3_prepare_v2(db, createTableMidpoint.c_str(),
			    createTableMidpoint.size(), &stmt, NULL);
    sqlite3_step(stmt);
    rc = sqlite3_prepare_v2(db, createTableCircle.c_str(),
			    createTableCircle.size(), &stmt, NULL);
    sqlite3_step(stmt);
    rc = sqlite3_prepare_v2(db, createTableCongruentSegments.c_str(),
			    createTableCongruentSegments.size(), &stmt, NULL);
    sqlite3_step(stmt);
    rc = sqlite3_prepare_v2(db, createTableCongruentTriangles.c_str(),
			    createTableCongruentTriangles.size(), &stmt, NULL);
    sqlite3_step(stmt);
    rc = sqlite3_prepare_v2(db, createTableCyclic.c_str(),
			    createTableCyclic.size(), &stmt, NULL);
    sqlite3_step(stmt);
    rc = sqlite3_prepare_v2(db, createTableEqualAngles.c_str(),
			    createTableEqualAngles.size(), &stmt, NULL);
    sqlite3_step(stmt);
    rc = sqlite3_prepare_v2(db, createTableEqualRatios.c_str(),
			    createTableEqualRatios.size(), &stmt, NULL);
    sqlite3_step(stmt);
    rc = sqlite3_prepare_v2(db, createTableSimilarTriangles.c_str(),
			    createTableSimilarTriangles.size(), &stmt, NULL);
    sqlite3_step(stmt);
    rc = sqlite3_prepare_v2(db, deleteFromSQLiteSequence.c_str(),
			    deleteFromSQLiteSequence.size(), &stmt, NULL);
    sqlite3_step(stmt);
  
    // Close the transaction
    sqlite3_exec(db, "commit;", 0, 0, 0);  
}

/*
 * Close (in memory) database.
 */
void DBinMemory::closeDB() {
    sqlite3_close(db);
}
