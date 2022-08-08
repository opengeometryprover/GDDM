/*
 * foftodb.cpp
 *
 * First Order Form to database manipulation: Driver, FOF parser, FOF
 * format to GDDM database FOFtoDB.
 *
 * This file is part of the OGP GDDM prover, which, in turn, is part of
 * the Open Geometry Prover Community Project (OGPCP)
 * <https://github.com/opengeometryprover>.
 *
 * Copyright (C) 2022 Nuno Baeta, Pedro Quaresma
 * Distributed under GNU GPL 3.0 or later
 */


#include <sqlite3.h>

#include "dbRAM.hpp"
#include "foftodb.hpp"
#include "parser.hpp"


Driver::Driver () : trace_parsing (false), trace_scanning (false) {
}

/*
 * Parser call.
 */
int Driver::parse(const std::string &f) {
    int res;

    file = f;
    location.initialize(&file);
    scan_begin();
    yy::parser parse(*this);
    parse.set_debug_level(trace_parsing);
    res = parse();
    scan_end();
    return res;
}

/*
 * Read (parse) the FOF conjecture (file) and populate the database.
 */
DBinMemory FOFtoDB::readFileLoadDB(Driver drv, DBinMemory dbim) {
    int i;
    int numGeoCmd;
    bool correctTransaction;
    std::string insertionColl;
    std::string insertionPara;
    std::string insertionPerp;
    std::string insertionMidp;
    std::string insertionCircle;
    std::string insertionCong;
    std::string insertionContri;
    std::string insertionCyclic;
    std::string insertionEqangle;
    std::string insertionEqratio;
    std::string insertionSimtri;
    std::string insertionNewConsequent;
    std::string insertionNewFact;
    std::string lastInsertedRowId;
    std::string lstInsRwId;

    // DEBUG
    std::cout << std::endl << "readFileLoadDB() : Entering..." << std::endl;
    
    // File parsed, now populate the database with the antecedents and
    // consequentes
    numGeoCmd = drv.numGeoCmd;

    // DEBUG
    std::cout << "    Nr. geometric commands : " << numGeoCmd << std::endl;
    
    for (i = 0; i < numGeoCmd; i++) {
	if (drv.antconcedent[i] == 0) {
	    // Antecendents...

	    // DEBUG
	    std::cout << "    Antecedent : " << i + 1 << " / ";
	    
	    // The SQL command
	    insertionNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('"
		+ drv.typeGeoCmd[i] + "')";
	    // Obtain value 'NewFact row id' through consultation
	    lastInsertedRowId = "SELECT last_insert_rowid()";

	    // Open transaction
	    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg)); 
	    correctTransaction = true; // What's the point?

	    // Insert the new fact into 'NewFacts'
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionNewFact.c_str(),
					 insertionNewFact.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    }
	    
	    // Get the last inserted row id
	    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
					 lastInsertedRowId.size(),
					 &(dbim.stmt), NULL);
	    // Execute the statement
	    sqlite3_step(dbim.stmt);
	    
	    lstInsRwId = (char*)sqlite3_column_text(dbim.stmt, 0);
	    switch (dbim.geoCmds[drv.typeGeoCmd[i]]) {
	    case 1:
                // Collinear
		// Insert the GeoCmd in the correct table connecting it to the new fact
		insertionColl = "INSERT INTO Collinear (typeGeoCmd, "
		    "point1, point2, point3, "
		    "newFact) VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + lstInsRwId + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionColl.c_str(),
					     insertionColl.size(),
					     &(dbim.stmt), NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		    correctTransaction = false;
		}

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ")" << std::endl;
		break;
	    case 2:
                // Parallel
		insertionPara = "INSERT INTO Parallel (typeGeoCmd, "
		    "point1, point2, point3, point4, "
		    "newFact) VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + drv.point4[i] + "', '"
		    + lstInsRwId + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPara.c_str(),
					     insertionPara.size(),
					     &(dbim.stmt), NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		    correctTransaction = false;
		}

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ", ";
		std::cout << drv.point4[i] + ")" << std::endl;
		break;
	    case 3:
                // Perpendicular
		insertionPerp = "INSERT INTO Perpendicular (typeGeoCmd, "
		    "point1, point2, point3, point4, "
		    "newFact) VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + drv.point4[i] + "', '"
		    + lstInsRwId + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPerp.c_str(),
					     insertionPerp.size(),
					     &(dbim.stmt), NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		    correctTransaction = false;
		}

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ", ";
		std::cout << drv.point4[i] + ")" << std::endl;
		break;
	    case 4:
                // Midpoint
		insertionMidp = "INSERT INTO Midpoint (typeGeoCmd, "
		    "point1, point2, point3, "
		    "newFact) VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + lstInsRwId + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionMidp.c_str(),
					     insertionMidp.size(),
					     &(dbim.stmt), NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		    correctTransaction = false;
		}

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ")" << std::endl;
		break;
	    case 5:
                // Circle
		insertionCircle = "INSERT INTO Circle (typeGeoCmd, "
		    "point1, point2, point3, point4, "
		    "newFact) VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + drv.point4[i] + "', '"
		    + lstInsRwId + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionCircle.c_str(),
					     insertionCircle.size(),
					     &(dbim.stmt), NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		    correctTransaction = false;
		}

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ", ";
		std::cout << drv.point4[i] + ")" << std::endl;
		break;
	    case 6:
		// CongruentSegments
		insertionCong = "INSERT INTO CongruentSegments (typeGeoCmd, "
		    "point1, point2, point3, point4, "
		    "newFact) VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + drv.point4[i] + "', '"
		    + lstInsRwId + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionCong.c_str(),
					     insertionCong.size(),
					     &(dbim.stmt), NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		    correctTransaction = false;
		}

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ", ";
		std::cout << drv.point4[i] + ")" << std::endl;
		break;
	    case 7:
		// CongruentTriangles
		insertionContri = "INSERT INTO CongruentTriangles (typeGeoCmd, "
		    "point1, point2, point3, point4, point5, point6, "
		    "newFact) VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + drv.point4[i] + "', '"
		    + drv.point5[i] + "', '"
		    + drv.point6[i] + "', '"
		    + lstInsRwId + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionContri.c_str(),
					     insertionContri.size(),
					     &(dbim.stmt), NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		    correctTransaction = false;
		}

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ", ";
		std::cout << drv.point4[i] + ", ";
		std::cout << drv.point5[i] + ", ";
		std::cout << drv.point6[i] + ")" << std::endl;
		break;
	    case 8:
		// Cyclic
		insertionCyclic = "INSERT INTO Cyclic (typeGeoCmd, "
		    "point1, point2, point3, point4, "
		    "newFact) VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + drv.point4[i] + "', '"
		    + lstInsRwId + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionCyclic.c_str(),
					     insertionCyclic.size(),
					     &(dbim.stmt), NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		    correctTransaction = false;
		}

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ", ";
		std::cout << drv.point4[i] + ")" << std::endl;
		break;
	    case 9:
		// EqualAngles
		insertionEqangle = "INSERT INTO EqualAngles (typeGeoCmd, "
		    "point1, point2, point3, point4, "
		    "point5, point6, point7, point8, "
		    "newFact) VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + drv.point4[i] + "', '"
		    + drv.point5[i] + "', '"
		    + drv.point6[i] + "', '"
		    + drv.point7[i] + "', '"
		    + drv.point8[i] + "', '"
		    + lstInsRwId + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionEqangle.c_str(),
					     insertionEqangle.size(),
					     &(dbim.stmt), NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		    correctTransaction = false;
		}

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ", ";
		std::cout << drv.point4[i] + ", ";
		std::cout << drv.point5[i] + ", ";
		std::cout << drv.point6[i] + ", ";
		std::cout << drv.point7[i] + ", ";
		std::cout << drv.point8[i] + ")" << std::endl;
		break;
	    case 10:
		// EqualRatios
		insertionEqratio = "INSERT INTO EqualRatios (typeGeoCmd, "
		    "point1, point2, point3, point4, "
		    "point5, point6, point7, point8, "
		    "newFact) VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + drv.point4[i] + "', '"
		    + drv.point5[i] + "', '"
		    + drv.point6[i] + "', '"
		    + drv.point7[i] + "', '"
		    + drv.point8[i] + "', '"
		    + lstInsRwId + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionEqratio.c_str(),
					     insertionEqratio.size(),
					     &(dbim.stmt), NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		    correctTransaction = false;
		}

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ", ";
		std::cout << drv.point4[i] + ", ";
		std::cout << drv.point5[i] + ", ";
		std::cout << drv.point6[i] + ", ";
		std::cout << drv.point7[i] + ", ";
		std::cout << drv.point8[i] + ")" << std::endl;
		break;
	    case 11:
		// SimilarTriangles
		insertionSimtri = "INSERT INTO SimilarTriangles (typeGeoCmd, "
		    "point1, point2, point3, point4, point5, point6, "
		    "newFact) VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + drv.point4[i] + "', '"
		    + drv.point5[i] + "', '"
		    + drv.point6[i] + "', '"
		    + lstInsRwId + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionSimtri.c_str(),
					     insertionSimtri.size(),
					     &(dbim.stmt), NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		    correctTransaction = false;
		}

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ", ";
		std::cout << drv.point4[i] + ", ";
		std::cout << drv.point5[i] + ", ";
		std::cout << drv.point6[i] + ")" << std::endl;
		break;
	    default:
                // ERROR : Necessary?  I don't _think_ so...
		std::cerr << "Unknown geometric command!!!" << std::endl;
		exit(1);
	    }
	    // close the transaction: commit or rollback
	    if (correctTransaction) {
		sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
	    } else {
		sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
	    }
	} else {
            // Consequent

	    // DEBUG
	    std::cout << "    Consequent : " << i + 1 << " / ";
	    
	    switch (dbim.geoCmds[drv.typeGeoCmd[i]]) {
	    case 1:
                // Collinear
		insertionNewConsequent = "INSERT INTO Consequent (typeGeoCmd, "
		    "point1, point2, point3"
		    ") VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "','"
		    + drv.point3[i] + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db,
					     insertionNewConsequent.c_str(),
					     insertionNewConsequent.size(),
					     &(dbim.stmt), NULL);
		sqlite3_step(dbim.stmt);

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ")" << std::endl;
		break;
	    case 2:
		// Parallel
		insertionNewConsequent = "INSERT INTO Consequent (typeGeoCmd, "
		    "point1, point2, point3, point4"
		    ") VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + drv.point4[i] + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db,
					     insertionNewConsequent.c_str(),
					     insertionNewConsequent.size(),
					     &(dbim.stmt), NULL);
		sqlite3_step(dbim.stmt);

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ", ";
		std::cout << drv.point4[i] + ")" << std::endl;
		break;
	    case 3:
                // Perpendicular
		insertionNewConsequent = "INSERT INTO Consequent (typeGeoCmd, "
		    "point1, point2, point3, point4"
		    ") VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + drv.point4[i] + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db,
					     insertionNewConsequent.c_str(),
					     insertionNewConsequent.size(),
					     &(dbim.stmt), NULL);
		sqlite3_step(dbim.stmt);

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ", ";
		std::cout << drv.point4[i] + ")" << std::endl;
		break;
	    case 4:
		// Midpoint
		insertionNewConsequent = "INSERT INTO Consequent (typeGeoCmd, "
		    "point1, point2, point3"
		    ") VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db,
					     insertionNewConsequent.c_str(),
					     insertionNewConsequent.size(),
					     &(dbim.stmt), NULL);
		sqlite3_step(dbim.stmt);

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ")" << std::endl;
		break;
	    case 5:
		// Circle
		insertionNewConsequent = "INSERT INTO Consequent (typeGeoCmd, "
		    "point1, point2, point3, point4"
		    ") VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + drv.point4[i] + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db,
					     insertionNewConsequent.c_str(),
					     insertionNewConsequent.size(),
					     &(dbim.stmt), NULL);
		sqlite3_step(dbim.stmt);

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ", ";
		std::cout << drv.point4[i] + ")" << std::endl;
		break;
	    case 6:
		// CongruentSegments
		insertionNewConsequent = "INSERT INTO Consequent (typeGeoCmd, "
		    "point1, point2, point3, point4"
		    ") VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + drv.point4[i] + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db,
					     insertionNewConsequent.c_str(),
					     insertionNewConsequent.size(),
					     &(dbim.stmt), NULL);
		sqlite3_step(dbim.stmt);

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ", ";
		std::cout << drv.point4[i] + ")" << std::endl;
		break;
	    case 7:
		// CongruentTriangles
		insertionNewConsequent = "INSERT INTO Consequent (typeGeoCmd, "
		    "point1, point2, point3, point4, point5, point6"
		    ") VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + drv.point4[i] + "', '"
		    + drv.point5[i] + "', '"
		    + drv.point6[i] + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db,
					     insertionNewConsequent.c_str(),
					     insertionNewConsequent.size(),
					     &(dbim.stmt), NULL);
		sqlite3_step(dbim.stmt);

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ", ";
		std::cout << drv.point4[i] + ", ";
		std::cout << drv.point5[i] + ", ";
		std::cout << drv.point6[i] + ")" << std::endl;
		break;
	    case 8:
		// Cyclic
		insertionNewConsequent = "INSERT INTO Consequent (typeGeoCmd, "
		    "point1, point2, point3, point4"
		    ") VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + drv.point4[i] + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db,
					     insertionNewConsequent.c_str(),
					     insertionNewConsequent.size(),
					     &(dbim.stmt), NULL);
		sqlite3_step(dbim.stmt);

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ", ";
		std::cout << drv.point4[i] + ")" << std::endl;
		break;
	    case 9:
		// EqualAngles
		insertionNewConsequent = "INSERT INTO Consequent (typeGeoCmd, "
		    "point1, point2, point3, point4, "
		    "point5, point6, point7, point8"
		    ") VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + drv.point4[i] + "', '"
		    + drv.point5[i] + "', '"
		    + drv.point6[i] + "', '"
		    + drv.point7[i] + "', '"
		    + drv.point8[i] + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db,
					     insertionNewConsequent.c_str(),
					     insertionNewConsequent.size(),
					     &(dbim.stmt), NULL);
		sqlite3_step(dbim.stmt);

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ", ";
		std::cout << drv.point4[i] + ", ";
		std::cout << drv.point5[i] + ", ";
		std::cout << drv.point6[i] + ", ";
		std::cout << drv.point7[i] + ", ";
		std::cout << drv.point8[i] + ")" << std::endl;
		break;
	    case 10:
		// EqualRatios
		insertionNewConsequent = "INSERT INTO Consequent (typeGeoCmd, "
		    "point1, point2, point3, point4, "
		    "point5, point6, point7, point8"
		    ") VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + drv.point4[i] + "', '"
		    + drv.point5[i] + "', '"
		    + drv.point6[i] + "', '"
		    + drv.point7[i] + "', '"
		    + drv.point8[i] + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db,
					     insertionNewConsequent.c_str(),
					     insertionNewConsequent.size(),
					     &(dbim.stmt), NULL);
		sqlite3_step(dbim.stmt);

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ", ";
		std::cout << drv.point4[i] + ", ";
		std::cout << drv.point5[i] + ", ";
		std::cout << drv.point6[i] + ", ";
		std::cout << drv.point7[i] + ", ";
		std::cout << drv.point8[i] + ")" << std::endl;
		break;
	    case 11:
		// SimilarTriangles
		insertionNewConsequent = "INSERT INTO Consequent (typeGeoCmd, "
		    "point1, point2, point3, point4, point5, point6"
		    ") VALUES ('"
		    + drv.typeGeoCmd[i] + "', '"
		    + drv.point1[i] + "', '"
		    + drv.point2[i] + "', '"
		    + drv.point3[i] + "', '"
		    + drv.point4[i] + "', '"
		    + drv.point5[i] + "', '"
		    + drv.point6[i] + "')";

		dbim.rc = sqlite3_prepare_v2(dbim.db,
					     insertionNewConsequent.c_str(),
					     insertionNewConsequent.size(),
					     &(dbim.stmt), NULL);
		sqlite3_step(dbim.stmt);

		// DEBUG
		std::cout << drv.typeGeoCmd[i] + "(";
		std::cout << drv.point1[i] + ", ";
		std::cout << drv.point2[i] + ", ";
		std::cout << drv.point3[i] + ", ";
		std::cout << drv.point4[i] + ", ";
		std::cout << drv.point5[i] + ", ";
		std::cout << drv.point6[i] + ")" << std::endl;
		break;
	    }
	}
    }

    // DEBUG
    std::cout << "readFileLoadDB() : Leaving..." << std::endl;

    return(dbim);
}

/*
 * Show database status
 */
void FOFtoDB::showDB(DBinMemory dbim) {
    std::string selectDB;
    
    std::cout << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::cout << "  In memory facts database  " << std::endl;
    std::cout << "----------------------------" << std::endl;

    // Collinear facts
    std::cout << std::endl;
    std::cout << "Collinear facts" << std::endl;
    selectDB = "SELECT typeGeoCmd, point1, point2, point3 "
	"FROM Collinear";
    dbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(),
				 &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    while (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	std::cout << "  "
		  << sqlite3_column_text(dbim.stmt, 0) << "("
		  << sqlite3_column_text(dbim.stmt, 1) << ", "
		  << sqlite3_column_text(dbim.stmt, 2) << ", "
		  << sqlite3_column_text(dbim.stmt, 3) << ")"
		  << std::endl;
    }

    // Parallel facts
    std::cout << std::endl;
    std::cout << "Parallel facts" << std::endl;
    selectDB = "SELECT typeGeoCmd, point1, point2, point3, point4 "
	"FROM Parallel";
    dbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(),
				 &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    while (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	std::cout << "  "
		  << sqlite3_column_text(dbim.stmt, 0) << "("
		  << sqlite3_column_text(dbim.stmt, 1) << ", "
		  << sqlite3_column_text(dbim.stmt, 2) << ", "
		  << sqlite3_column_text(dbim.stmt, 3) << ", "
		  << sqlite3_column_text(dbim.stmt, 4) << ")"
		  << std::endl;
    }

    // Perpendicular facts
    std::cout << std::endl;
    std::cout << "Perpendicular facts" << std::endl;
    selectDB = "SELECT typeGeoCmd, point1, point2, point3, point4 "
	"FROM Perpendicular";
    dbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(),
				 &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    while (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	std::cout << "  "
		  << sqlite3_column_text(dbim.stmt, 0) << "("
		  << sqlite3_column_text(dbim.stmt, 1) << ", "
		  << sqlite3_column_text(dbim.stmt, 2) << ", "
		  << sqlite3_column_text(dbim.stmt, 3) << ", "
		  << sqlite3_column_text(dbim.stmt, 4) << ")"
		  << std::endl;
    }

    // Midpoint facts
    std::cout << std::endl;
    std::cout << "Midpoint facts" << std::endl;
    selectDB = "SELECT typeGeoCmd, point1, point2, point3 "
	"FROM Midpoint";
    dbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(),
				 &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    while (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	std::cout << "  "
		  << sqlite3_column_text(dbim.stmt, 0) << "("
		  << sqlite3_column_text(dbim.stmt, 1) << ", "
		  << sqlite3_column_text(dbim.stmt, 2) << ", "
		  << sqlite3_column_text(dbim.stmt, 3) << ")"
		  << std::endl;
    }

    // Circle facts
    std::cout << std::endl;
    std::cout << "Circle facts" << std::endl;
    selectDB = "SELECT typeGeoCmd, point1, point2, point3, point4 "
	"FROM Circle";
    dbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(),
				 &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    while (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	std::cout << "  "
		  << sqlite3_column_text(dbim.stmt, 0) << "("
		  << sqlite3_column_text(dbim.stmt, 1) << ", "
		  << sqlite3_column_text(dbim.stmt, 2) << ", "
		  << sqlite3_column_text(dbim.stmt, 3) << ", "
		  << sqlite3_column_text(dbim.stmt, 4) << ")"
		  << std::endl;
    }

    // Congruent Segments facts
    std::cout << std::endl;
    std::cout << "Congruent Segments facts" << std::endl;
    selectDB = "SELECT typeGeoCmd, point1, point2, point3, point4 "
	"FROM CongruentSegments";
    dbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(),
				 &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    while (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	std::cout << "  "
		  << sqlite3_column_text(dbim.stmt, 0) << "("
		  << sqlite3_column_text(dbim.stmt, 1) << ", "
		  << sqlite3_column_text(dbim.stmt, 2) << ", "
		  << sqlite3_column_text(dbim.stmt, 3) << ", "
		  << sqlite3_column_text(dbim.stmt, 4) << ")"
		  << std::endl;
    }

    // Congruent Triangles facts
    std::cout << std::endl;
    std::cout << "Congruent Triangles facts" << std::endl;
    selectDB = "SELECT typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6 "
	"FROM CongruentTriangles";
    dbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(),
				 &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    while (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	std::cout << "  "
		  << sqlite3_column_text(dbim.stmt, 0) << "("
		  << sqlite3_column_text(dbim.stmt, 1) << ", "
		  << sqlite3_column_text(dbim.stmt, 2) << ", "
		  << sqlite3_column_text(dbim.stmt, 3) << ", "
		  << sqlite3_column_text(dbim.stmt, 4) << ", "
		  << sqlite3_column_text(dbim.stmt, 5) << ", "
		  << sqlite3_column_text(dbim.stmt, 6) << ")"
		  << std::endl;
    }

    // Cyclic facts
    std::cout << std::endl;
    std::cout << "Cyclic facts" << std::endl;
    selectDB = "SELECT typeGeoCmd, point1, point2, point3, point4 "
	"FROM Cyclic";
    dbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(),
				 &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    while (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	std::cout << "  "
		  << sqlite3_column_text(dbim.stmt, 0) << "("
		  << sqlite3_column_text(dbim.stmt, 1) << ", "
		  << sqlite3_column_text(dbim.stmt, 2) << ", "
		  << sqlite3_column_text(dbim.stmt, 3) << ", "
		  << sqlite3_column_text(dbim.stmt, 4) << ")"
		  << std::endl;
    }

    // Equal Angles facts
    std::cout << std::endl;
    std::cout << "Equal Angles facts" << std::endl;
    selectDB = "SELECT typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6, point7, point8 "
	"FROM EqualAngles";
    dbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(),
				 &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    while (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	std::cout << "  "
		  << sqlite3_column_text(dbim.stmt, 0) << "("
		  << sqlite3_column_text(dbim.stmt, 1) << ", "
		  << sqlite3_column_text(dbim.stmt, 2) << ", "
		  << sqlite3_column_text(dbim.stmt, 3) << ", "
		  << sqlite3_column_text(dbim.stmt, 4) << ", "
		  << sqlite3_column_text(dbim.stmt, 5) << ", "
		  << sqlite3_column_text(dbim.stmt, 6) << ", "
		  << sqlite3_column_text(dbim.stmt, 7) << ", "
		  << sqlite3_column_text(dbim.stmt, 8) << ")"
		  << std::endl;
    }

    // Equal Ratios facts
    std::cout << std::endl;
    std::cout << "Equal Ratios facts" << std::endl;
    selectDB = "SELECT typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6, point7, point8 "
	"FROM EqualRatios";
    dbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(),
				 &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    while (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	std::cout << "  "
		  << sqlite3_column_text(dbim.stmt, 0) << "("
		  << sqlite3_column_text(dbim.stmt, 1) << ", "
		  << sqlite3_column_text(dbim.stmt, 2) << ", "
		  << sqlite3_column_text(dbim.stmt, 3) << ", "
		  << sqlite3_column_text(dbim.stmt, 4) << ", "
		  << sqlite3_column_text(dbim.stmt, 5) << ", "
		  << sqlite3_column_text(dbim.stmt, 6) << ", "
		  << sqlite3_column_text(dbim.stmt, 7) << ", "
		  << sqlite3_column_text(dbim.stmt, 8) << ")"
		  << std::endl;
    }

    // Similar Triangles facts
    std::cout << std::endl;
    std::cout << "Similar Triangles facts" << std::endl;
    selectDB = "SELECT typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6 "
	"FROM SimilarTriangles";
    dbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(),
				 &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    while (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	std::cout << "  "
		  << sqlite3_column_text(dbim.stmt, 0) << "("
		  << sqlite3_column_text(dbim.stmt, 1) << ", "
		  << sqlite3_column_text(dbim.stmt, 2) << ", "
		  << sqlite3_column_text(dbim.stmt, 3) << ", "
		  << sqlite3_column_text(dbim.stmt, 4) << ", "
		  << sqlite3_column_text(dbim.stmt, 5) << ", "
		  << sqlite3_column_text(dbim.stmt, 6) << ")"
		  << std::endl;
    }

    std::cout << std::endl;
    std::cout << "----------------------------" << std::endl;
}
