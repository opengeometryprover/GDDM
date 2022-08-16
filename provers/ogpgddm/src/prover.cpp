/*
 * prover.cpp
 *
 * GDDM's core: deduction rules and point fixed construction.
 *
 * This file is part of the OGP GDDM prover, which, in turn, is part of
 * the Open Geometry Prover Community Project (OGPCP)
 * <https://github.com/opengeometryprover>.
 *
 * Copyright (C) 2022 Nuno Baeta, Pedro Quaresma
 * Distributed under GNU GPL 3.0 or later
 */


#include <iostream>
#include <map>

#include "prover.hpp"
#include "points.hpp"

#include "foftodb.hpp" // To show the BD.

/*
 * Rule D1: coll(A, B, C) -> coll(A, C, B)
 */
DBinMemory Prover::ruleD01(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('coll')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str() ,
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"Collinear (typeGeoCmd, point1, point2, point3, newFact) "
	"VALUES "
	"('coll', '" + point1 + "', '" + point3 + "', '" + point2 + "', '"
	+ lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D2: coll(A, B, C) -> coll(B, A, C)
 */
DBinMemory Prover::ruleD02(DBinMemory dbim, std::string point1,
			   std::string point2,std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('coll')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	" Collinear (typeGeoCmd, point1, point2, point3, newFact) "
	"VALUES "
	"('coll', '" + point2 + "', '" + point1 + "', '" + point3 + "', '"
	+ lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D3: coll(A, B, C) & coll(A, B, D) -> coll(C, D, A)
 */
DBinMemory Prover::ruleD03(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('coll')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point3 "
	"FROM NewFact "
	"INNER JOIN Collinear "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 <> '" + point3 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point3 "
	"FROM Facts "
	"INNER JOIN Collinear "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 <> '" + point3 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0)
	    newPoint = (char*) sqlite3_column_text(dbim.stmt1, 0);
	else
	    newPoint = (char*) sqlite3_column_text(dbim.stmt2, 0);
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Collinear (typeGeoCmd, point1, point2, point3, newFact) "
		"VALUES "
		"('coll', '" + point3 + "', '" + newPoint + "', '" + point1
		+ "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D4: para(A, B, C, D) -> para(A, B, D, C)
 */
DBinMemory Prover::ruleD04(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('para')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
     	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"Parallel (typeGeoCmd, point1, point2, point3, point4, newFact) "
	"VALUES "
	"('para', '" + point1 + "', '" + point2 + "', '" + point4 + "', '"
	+ point3 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D5: para(A, B, C, D) -> para(C, D, A, B)
 */
DBinMemory Prover::ruleD05(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('para')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction=true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"Parallel (typeGeoCmd, point1, point2, point3, point4, newFact) "
	" VALUES "
	"('para', '" + point3 + "', '" + point4 + "', '" + point1 + "', '"
	+ point2 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D6: para(A, B, C, D) & para(C, D, E, F) -> para(A, B, E, F)
 */
DBinMemory Prover::ruleD06(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('para')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg)); 
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point3, point4 "
	"FROM NewFact "
	"INNER JOIN Parallel "
	"ON (newFact = id) "
	"WHERE point1 = '" + point3 + "' AND point2 = '" + point4
	+ "' AND NOT (point3 = '" + point1 + "' AND point4 = '" + point2
	+ "') AND NOT (point3 = '" + point2 + "' AND point4 = '" + point1
	+ "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point3, point4 "
	"FROM Facts "
	"INNER JOIN Parallel "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point3 + "' AND point2 = '" + point4
	+ "' AND NOT (point3 = '" + point1 + "' AND point4 = '" + point2
	+ "') AND NOT (point3 = '" + point2 + "' AND point4 = '" + point1
	+ "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Parallel (typeGeoCmd, point1, point2, point3, point4,"
		"newFact) "
		"VALUES "
		"('para', '" + point1 + "', '" + point2 + "', '" + newPoint1
		+ "', '" + newPoint2 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D7: perp(A, B, C, D) -> perp(A, B, D, C)
 */
DBinMemory Prover::ruleD07(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('perp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"Perpendicular (typeGeoCmd, point1, point2, point3, point4, newFact) "
	"VALUES "
	"('perp', '" + point1 + "', '" + point2 + "', '" + point4 + "', '"
	+ point3 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D8: perp(A, B, C, D) -> perp(C, D, A, B)
 */
DBinMemory Prover::ruleD08(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('perp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"Perpendicular (typeGeoCmd, point1, point2, point3, point4, newFact) "
	"VALUES "
	"('perp', '" + point3 + "', '" + point4 + "', '" + point1 + "', '"
	+ point2 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D9: perp(A, B, C, D) & perp(C, D, E, F) -> para(A, B, E, F)
 */
DBinMemory Prover::ruleD09(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('para')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg)); 
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point3, point4 "
	"FROM NewFact "
	"INNER JOIN Perpendicular "
	"ON (newFact = id) "
	"WHERE point1 = '" + point3 + "' AND point2 = '" + point4
	+ "' AND NOT (point3 = '" + point1 + "' AND point4 = '" + point2
	+ "') AND NOT (point3 = '" + point2 + "' AND point4 = '" + point1
	+ "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    
    querySecondGeoCmdB = "SELECT point3, point4 "
	"FROM Facts "
	"INNER JOIN Perpendicular "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point3 + "' AND point2 = '" + point4
	+ "' AND NOT (point3 = '" + point1 + "' AND point4 = '" + point2
	+ "') AND NOT (point3 = '" + point2 + "' AND point4 = '" + point1
	+ "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Parallel (typeGeoCmd, point1, point2, point3, point4, newFact)"
		"VALUES "
		"('para', '" + point1 + "', '" + point2 + "', '" + newPoint1
		+ "', '" + newPoint2 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
  return dbim;
}

/*
 * Rule D10: para(A, B, C, D) & perp(C, D, E, F) -> perp(A, B, E, F)
 *
 * Function's argument is para(A, B, C, D) and searches for perp(C, D, E, F).
 */
DBinMemory Prover::ruleD10para(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3,
			       std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('perp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg)); 
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
     	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point3, point4 "
	"FROM NewFact "
	"INNER JOIN Perpendicular "
	"ON (newFact = id) "
	"WHERE point1 = '" + point3 + "' AND point2 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point3, point4 "
	"FROM Facts "
	"INNER JOIN Perpendicular "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point3 + "' AND point2 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Perpendicular (typeGeoCmd, point1, point2, point3, point4, "
		"newFact) "
		"VALUES "
		"('perp', '" + point1 + "', '" + point2 + "', '" + newPoint1
		+ "', '" + newPoint2 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
  return dbim;
}

/*
 * Rule D10: para(A, B, C, D) & perp(C, D, E, F) -> perp(A, B, E, F)
 *
 * Function's argument is perp(C, D, E, F) and searches for para(A, B, C, D).
 */
DBinMemory Prover::ruleD10perp(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3,
			       std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('perp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg)); 
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
     	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point1, point2 "
	"FROM NewFact "
	"INNER JOIN Parallel "
	"ON (newFact = id) "
	"WHERE point3 = '" + point1 + "' AND point4 = '" + point2 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point1, point2 "
	"FROM Facts "
	"INNER JOIN Parallel "
	"ON (oldFact = id) "
	"WHERE point3 = '" + point1 + "' AND point4 = '" + point2 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Perpendicular (typeGeoCmd, point1, point2, point3, point4, "
		"newFact) "
		"VALUES "
		"('perp', '" + newPoint1 + "', '" + newPoint2 + "', '"
		+ point3 + "', '" + point4 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
  return dbim;
}

/*
 * Rule D11 : midp(M, B, A) -> midp(M, A, B)
 */
DBinMemory Prover::ruleD11(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3){
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('midp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg)); 
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str() ,
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"Midpoint (typeGeoCmd, point1, point2, point3, newFact) "
	"VALUES ('midp', '" + point1 + "', '" + point3 + "', '" + point2
	+ "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D12: cong(O, A, O, B) & perp(O, A, O, C) -> circle(O, A, B, C)
 */
DBinMemory Prover::ruleD12(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('circle')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg)); 
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point4 "
	"FROM NewFact "
	"INNER JOIN CongruentSegments "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point3
	+ "' AND point4 <> '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point4 "
	"FROM Facts "
	"INNER JOIN CongruentSegments "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
        + "' AND point3 = '" + point3
	+ "' AND point4 <> '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0)
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	else
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Circle (typeGeoCmd, point1, point2, point3, point4, newFact)"
		"VALUES "
		"('circle', '" + point1 + "', '" + point2 + "', '" + point4
		+ "', '" + newPoint1 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D13: cong(O, A, O, B) & cong(O, A, O, C) & cong(O, A, O, D)
 *               -> cyclic(A, B, C, D)
 */
DBinMemory Prover::ruleD13(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cyclic')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg)); 
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point4 "
	"FROM NewFact "
	"INNER JOIN CongruentSegments "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point3
	+ "' AND point4 <> '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point4 "
	"FROM Facts "
	"INNER JOIN CongruentSegments "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
        + "' AND point3 = '" + point3
	+ "' AND point4 <> '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0)
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	else
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	querySecondGeoCmdA = "SELECT point4 "
	    "FROM NewFact "
	    "INNER JOIN CongruentSegments "
	    "ON (newFact = id) "
	    "WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	    + "' AND point3 = '" + point3
	    + "' AND point4 NOT IN ('" + point4 + "', '" + newPoint1 + "')";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				     querySecondGeoCmdA.size(),
				     &(dbim.stmt1), NULL);
	sqlite3_step(dbim.stmt1);
	querySecondGeoCmdB = "SELECT point4 "
	    "FROM Facts "
	    "INNER JOIN CongruentSegments "
	    "ON (oldFact = id) "
	    "WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	    + "' AND point3 = '" + point3
	    + "' AND point4 NOT IN ('" + point4 + "', '" + newPoint1 + "')";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				     querySecondGeoCmdB.size(),
					 &(dbim.stmt2), NULL);
	sqlite3_step(dbim.stmt2);
	if (sqlite3_data_count(dbim.stmt1) == 0
	    && sqlite3_data_count(dbim.stmt2) == 0 ) {
	    correctTransaction = false;
	} else {
	    if (sqlite3_data_count(dbim.stmt1) != 0)
		newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    else
		newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    } else {
		insertionPred = "INSERT INTO "
		    "Cyclic (typeGeoCmd, point1, point2, point3, point4, "
		    "newFact)"
		    "VALUES "
		    "('cyclic', '" + point2 + "', '" + point4 + "', '"
		    + newPoint1 + "', '" + newPoint2 + "', '" + lstInsRwId
		    + "')";
		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					     insertionPred.size(),
					     &(dbim.stmt), NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		    correctTransaction = false;
	    }
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
  return dbim;
}

/*
 * Rule D14: cyclic(A, B, C, D) -> cyclic(A, B, D, C)
 */
DBinMemory Prover::ruleD14(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cyclic')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"Cyclic (typeGeoCmd, point1, point2, point3, point4, newFact) "
	"VALUES "
	"('cyclic', '" + point1 + "', '" + point2 + "', '" + point4 + "', '"
	+ point3 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D15: cyclic(A, B, C, D) -> cyclic(A, C, B, D)
 */
DBinMemory Prover::ruleD15(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cyclic')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"Cyclic (typeGeoCmd, point1, point2, point3, point4, newFact) "
	"VALUES "
	"('cyclic', '" + point1 + "', '" + point3 + "', '" + point2 + "', '"
	+ point4 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}


/*
 * Rule D16: cyclic(A, B, C, D) -> cyclic(B, A, C, D)
 */
DBinMemory Prover::ruleD16(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cyclic')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"Cyclic (typeGeoCmd, point1, point2, point3, point4, newFact) "
	"VALUES "
	"('cyclic', '" + point2 + "', '" + point1 + "', '" + point3 + "', '"
	+ point4 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D17: cyclic(A, B, C, D) & cyclic(A, B, C, E) -> cyclic(B, C, D, E)
 */
DBinMemory Prover::ruleD17(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cyclic')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg)); 
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point4 "
	"FROM NewFact "
	"INNER JOIN Cyclic "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point3
	+ "' AND point4 <> '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point4 "
	"FROM Facts "
	"INNER JOIN Cyclic "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point3
	+ "' AND point4 <> '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0)
	    newPoint = (char*) sqlite3_column_text(dbim.stmt1, 0);
	else
	    newPoint = (char*) sqlite3_column_text(dbim.stmt2, 0);
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Cyclic (typeGeoCmd, point1, point2, point3, point4, newFact) "
		"VALUES "
		"('cyclic', '" + point2 + "', '" + point3 + "', '" + point4
		+ "', '" + newPoint + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D18: eqangle(A, B, C, D, P, Q, U, V) -> eqangle(B, A, C, D, P, Q, U, V)
 */
DBinMemory Prover::ruleD18(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqangle')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"EqualAngles (typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6, point7, point8, newFact) "
	"VALUES "
	"('eqangle', '" + point2 + "', '" + point1 + "', '" + point3 + "', '"
	+ point4 + "', '" + point5 + "', '" + point6 + "', '" + point7 + "', '"
	+ point8 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D19: eqangle(A, B, C, D, P, Q, U, V) -> eqangle(C, D, A, B, U, V, P, Q)
 */
DBinMemory Prover::ruleD19(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqangle')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"EqualAngles (typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6, point7, point8, newFact) "
	"VALUES "
	"('eqangle', '" + point3 + "', '" + point4 + "', '" + point1 + "', '"
	+ point2 + "', '" + point7 + "', '" + point8 + "', '" + point5 + "', '"
	+ point6 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D20: eqangle(A, B, C, D, P, Q, U, V) -> eqangle(P, Q, U, V, A, B, C, D)
 */
DBinMemory Prover::ruleD20(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqangle')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"EqualAngles (typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6, point7, point8, newFact) "
	"VALUES "
	"('eqangle', '" + point5 + "', '" + point6 + "', '" + point7 + "', '"
	+ point8 + "', '" + point1 + "', '" + point2 + "', '" + point3 + "', '"
	+ point4 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D21: eqangle(A, B, C, D, P, Q, U, V) -> eqangle(A, B, P, Q, C, D, U, V)
 */
DBinMemory Prover::ruleD21(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqangle')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"EqualAngles (typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6, point7, point8, newFact) "
	"VALUES "
	"('eqangle', '" + point1 + "', '" + point2 + "', '" + point5 + "', '"
	+ point6 + "', '" + point3 + "', '" + point4 + "', '" + point7 + "', '"
	+ point8 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D22: eqangle(A, B, C, D, P, Q, U, V) & eqangle(P, Q, U, V, E, F, G, H)
                 -> eqangle(A, B, C, D, E, F, G, H)
 */
DBinMemory Prover::ruleD22(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2, newPoint3, newPoint4;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqangle')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg)); 
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point5, point6, point7, point8 "
	"FROM NewFact "
	"INNER JOIN EqualAngles "
	"ON (newFact = id) "
	"WHERE point1 = '" + point5 + "' AND point2 = '" + point6
	+ "' AND point3 = '" + point7 + "' AND point4 = '" + point8
	+ "' AND NOT (point5 = '" + point1 + "' AND point6 = '" + point2
	+ "' AND point7 = '" + point3 + "' AND point8 = '" + point4
	+ "') AND NOT (point5 = '" + point1 + "' AND point6 = '" + point2
	+ "' AND point7 = '" + point4 + "' AND point8 = '" + point3
	+ "') AND NOT (point5 = '" + point2 + "' AND point6 = '" + point1
	+ "' AND point7 = '" + point3 + "' AND point8 = '" + point4
	+ "') AND NOT (point5 = '" + point2 + "' AND point6 = '" + point2
	+ "' AND point7 = '" + point4 + "' AND point8 = '" + point3
	+ "') AND NOT (point5 = '" + point3 + "' AND point6 = '" + point4
	+ "' AND point7 = '" + point1 + "' AND point8 = '" + point2
	+ "') AND NOT (point5 = '" + point3 + "' AND point6 = '" + point4
	+ "' AND point7 = '" + point2 + "' AND point8 = '" + point1
	+ "') AND NOT (point5 = '" + point4 + "' AND point6 = '" + point3
	+ "' AND point7 = '" + point1 + "' AND point8 = '" + point2
	+ "') AND NOT (point5 = '" + point4 + "' AND point6 = '" + point3
	+ "' AND point7 = '" + point2 + "' AND point8 = '" + point1 + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point5, point6, point7, point8 "
	"FROM Facts "
	"INNER JOIN EqualAngles "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point5 + "' AND point2 = '" + point6
	+ "' AND point3 = '" + point7 + "' AND point4 = '" + point8
	+ "' AND NOT (point5 = '" + point1 + "' AND point6 = '" + point2
	+ "' AND point7 = '" + point3 + "' AND point8 = '" + point4
	+ "') AND NOT (point5 = '" + point1 + "' AND point6 = '" + point2
	+ "' AND point7 = '" + point4 + "' AND point8 = '" + point3
	+ "') AND NOT (point5 = '" + point2 + "' AND point6 = '" + point1
	+ "' AND point7 = '" + point3 + "' AND point8 = '" + point4
	+ "') AND NOT (point5 = '" + point2 + "' AND point6 = '" + point2
	+ "' AND point7 = '" + point4 + "' AND point8 = '" + point3
	+ "') AND NOT (point5 = '" + point3 + "' AND point6 = '" + point4
	+ "' AND point7 = '" + point1 + "' AND point8 = '" + point2
	+ "') AND NOT (point5 = '" + point3 + "' AND point6 = '" + point4
	+ "' AND point7 = '" + point2 + "' AND point8 = '" + point1
	+ "') AND NOT (point5 = '" + point4 + "' AND point6 = '" + point3
	+ "' AND point7 = '" + point1 + "' AND point8 = '" + point2
	+ "') AND NOT (point5 = '" + point4 + "' AND point6 = '" + point3
	+ "' AND point7 = '" + point2 + "' AND point8 = '" + point1 + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt1, 3);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt2, 2);
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt2, 3);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"EqualAngles (typeGeoCmd, point1, point2, point3, point4, "
		"point5, point6, point7, point8, newFact) "
		"VALUES "
		"('eqangle', '" + point1 + "', '" + point2 + "', '" + point3
		+ "', '" + point4 + "', '" + newPoint1 + "', '" + newPoint2
		+ "', '" + newPoint3 + "', '" + newPoint4 + "', '" + lstInsRwId
		+ "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D23: cong(A, B, C, D) -> cong(A, B, D, C)
 */
DBinMemory Prover::ruleD23(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cong')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"CongruentSegments (typeGeoCmd, point1, point2, point3, point4, "
	"newFact) "
	"VALUES "
	"('cong', '" + point1 + "', '" + point2 + "', '" + point4 + "', '"
	+ point3 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D24: cong(A, B, C, D) -> cong(C, D, A, B)
 */
DBinMemory Prover::ruleD24(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cong')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"CongruentSegments (typeGeoCmd, point1, point2, point3, point4, "
	"newFact) "
	"VALUES "
	"('cong', '" + point3 + "', '" + point4 + "', '" + point1 + "', '"
	+ point2 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D25: cong(A, B, C, D) & cong(C, D, E, F) -> cong(A, B, E, F)
 */
DBinMemory Prover::ruleD25(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cong')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg)); 
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point3, point4 "
	"FROM NewFact "
	"INNER JOIN CongruentSegments "
	"ON (newFact = id) "
	"WHERE point1 = '" + point3 + "' AND point2 = '" + point4
	+ "' AND NOT (point3 = '" + point1 + "' AND point4 = '" + point2
	+ "') AND NOT (point3 = '" + point2 + "' AND point4 = '" + point1
	+ "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point3, point4 "
	"FROM Facts "
	"INNER JOIN CongruentSegments "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point3 + "' AND point2 = '" + point4 +
	+ "' AND NOT (point3 = '" + point1 + "' AND point4 = '" + point2
	+ "') AND NOT (point3 = '" + point2 + "' AND point4 = '" + point1
	+ "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"CongruentSegments (typeGeoCmd, point1, point2, point3, "
		"point4, newFact) "
		"VALUES "
		"('cong', '" + point1 + "', '" + point2 + "', '" + newPoint1
		+ "', '" + newPoint2 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D26: eqratio(A, B, C, D, P, Q, U, V) -> eqratio(B, A, C, D, P, Q, U, V)
 */
DBinMemory Prover::ruleD26(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqratio')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"EqualRatios (typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6, point7, point8, newFact) "
	"VALUES "
	"('eqratio', '" + point2 + "', '" + point1 + "', '" + point3 + "', '"
	+ point4 + "', '" + point5 + "', '" + point6 + "', '" + point7 + "', '"
	+ point8 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D27: eqratio(A, B, C, D, P, Q, U, V) -> eqratio(C, D, A, B, U, V, P, Q)
 */
DBinMemory Prover::ruleD27(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqratio')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"EqualRatios (typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6, point7, point8, newFact) "
	"VALUES "
	"('eqratio', '" + point3 + "', '" + point4 + "', '" + point1 + "', '"
	+ point2 + "', '" + point7 + "', '" + point8 + "', '" + point5 + "', '"
	+ point6 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D28: eqratio(A, B, C, D, P, Q, U, V) -> eqratio(P, Q, U, V, A, B, C, D)
 */
DBinMemory Prover::ruleD28(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqratio')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"EqualRatios (typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6, point7, point8, newFact) "
	"VALUES "
	"('eqratio', '" + point5 + "', '" + point6 + "', '" + point7 + "', '"
	+ point8 + "', '" + point1 + "', '" + point2 + "', '" + point3 + "', '"
	+ point4 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D29: eqratio(A, B, C, D, P, Q, U, V) -> eqratio(A, B, P, Q, C, D, U, V)
 */
DBinMemory Prover::ruleD29(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqratio')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"EqualRatios (typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6, point7, point8, newFact) "
	"VALUES "
	"('eqratio', '" + point1 + "', '" + point2 + "', '" + point5 + "', '"
	+ point6 + "', '" + point3 + "', '" + point4 + "', '" + point7 + "', '"
	+ point8 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D30: eqratio(A, B, C, D, P, Q, U, V) & eqratio(P, Q, U, V, E, F, G, H)
 *               -> eqratio(A, B, C, D, E, F, G, H)
 */
DBinMemory Prover::ruleD30(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2, newPoint3, newPoint4;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqratio')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg)); 
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point5, point6, point7, point8 "
	"FROM NewFact "
	"INNER JOIN EqualRatios "
	"ON (newFact = id) "
	"WHERE point1 = '" + point5 + "' AND point2 = '" + point6
	+ "' AND point3 = '" + point7 + "' AND point4 = '" + point8
	+ "' AND NOT (point5 = '" + point1 + "' AND point6 = '" + point2
	+ "' AND point7 = '" + point3 + "' AND point8 = '" + point4
	+ "') AND NOT (point5 = '" + point1 + "' AND point6 = '" + point2
	+ "' AND point7 = '" + point4 + "' AND point8 = '" + point3
	+ "') AND NOT (point5 = '" + point2 + "' AND point6 = '" + point1
	+ "' AND point7 = '" + point3 + "' AND point8 = '" + point4
	+ "') AND NOT (point5 = '" + point2 + "' AND point6 = '" + point2
	+ "' AND point7 = '" + point4 + "' AND point8 = '" + point3
	+ "') AND NOT (point5 = '" + point3 + "' AND point6 = '" + point4
	+ "' AND point7 = '" + point1 + "' AND point8 = '" + point2
	+ "') AND NOT (point5 = '" + point3 + "' AND point6 = '" + point4
	+ "' AND point7 = '" + point2 + "' AND point8 = '" + point1
	+ "') AND NOT (point5 = '" + point4 + "' AND point6 = '" + point3
	+ "' AND point7 = '" + point1 + "' AND point8 = '" + point2
	+ "') AND NOT (point5 = '" + point4 + "' AND point6 = '" + point3
	+ "' AND point7 = '" + point2 + "' AND point8 = '" + point1 + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point5, point6, point7, point8 "
	"FROM Facts "
	"INNER JOIN EqualRatios "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point5 + "' AND point2 = '" + point6
	+ "' AND point3 = '" + point7 + "' AND point4 = '" + point8
	+ "' AND NOT (point5 = '" + point1 + "' AND point6 = '" + point2
	+ "' AND point7 = '" + point3 + "' AND point8 = '" + point4
	+ "') AND NOT (point5 = '" + point1 + "' AND point6 = '" + point2
	+ "' AND point7 = '" + point4 + "' AND point8 = '" + point3
	+ "') AND NOT (point5 = '" + point2 + "' AND point6 = '" + point1
	+ "' AND point7 = '" + point3 + "' AND point8 = '" + point4
	+ "') AND NOT (point5 = '" + point2 + "' AND point6 = '" + point2
	+ "' AND point7 = '" + point4 + "' AND point8 = '" + point3
	+ "') AND NOT (point5 = '" + point3 + "' AND point6 = '" + point4
	+ "' AND point7 = '" + point1 + "' AND point8 = '" + point2
	+ "') AND NOT (point5 = '" + point3 + "' AND point6 = '" + point4
	+ "' AND point7 = '" + point2 + "' AND point8 = '" + point1
	+ "') AND NOT (point5 = '" + point4 + "' AND point6 = '" + point3
	+ "' AND point7 = '" + point1 + "' AND point8 = '" + point2
	+ "') AND NOT (point5 = '" + point4 + "' AND point6 = '" + point3
	+ "' AND point7 = '" + point2 + "' AND point8 = '" + point1 + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt1, 3);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt2, 2);
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt2, 3);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"EqualRatios (typeGeoCmd, point1, point2, point3, point4, "
		"point5, point6, point7, point8, newFact) "
		"VALUES "
		"('eqratio', '" + point1 + "', '" + point2 + "', '" + point3
		+ "', '" + point4 + "', '" + newPoint1 + "', '" + newPoint2
		+ "', '" + newPoint3 + "', '" + newPoint4 + "', '" + lstInsRwId
		+ "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D31: simtri(A, C, B, P, R, Q) -> simtri(A, B, C, P, Q, R)
 */
DBinMemory Prover::ruleD31(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('simtri')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"SimilarTriangles (typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6, newFact) "
	"VALUES "
	"('simtri', '" + point1 + "', '" + point3 + "', '" + point2 + "', '"
	+ point4 + "', '" + point6 + "',  '" + point5 + "', '" + lstInsRwId
	+ "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D32: simtri(B, A, C, Q, P, R) -> simtri(A, B, C, P, Q, R)
 */
DBinMemory Prover::ruleD32(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('simtri')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"SimilarTriangles (typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6, newFact) "
	"VALUES "
	"('simtri', '" + point2 + "', '" + point1 + "', '" + point3 + "', '"
	+ point5 + "', '" + point4 + "', '" + point6 + "', '" + lstInsRwId
	+ "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D33: simtri(P, Q, R, A, B, C) -> simtri(A, B, C, P, Q, R)
 */
DBinMemory Prover::ruleD33(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('simtri')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"SimilarTriangles (typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6, newFact) "
	"VALUES "
	"('simtri', '" + point4 + "', '" + point5 + "', '" + point6+ "', '"
	+ point1 + "', '" + point2 + "', '" + point3 + "', '" + lstInsRwId
	+ "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D34: simtri(A, B, C, E, F, G) & simtri(E, F, G, P, Q, R)
 *               -> simtri(A, B, C, P, Q, R)
 */
DBinMemory Prover::ruleD34(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2, newPoint3;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('simtri')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg)); 
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point4, point5, point6 "
	"FROM NewFact "
	"INNER JOIN SimilarTriangles "
	"ON (newFact = id) "
        "WHERE point1 = '" + point4 + "' AND point2 = '" + point5
	+ "' AND point3 = '" + point6
	+ "' AND NOT (point4 = '" + point1 + "' AND point5 = '" + point2
	+ "' AND point6 = '" + point3
	+ "') AND NOT (point4 = '" + point1 + "' AND point5 = '" + point3
	+ "' AND point6 = '" + point2
	+ "') AND NOT (point4 = '" + point2 + "' AND point5 = '" + point1
	+ "' AND point6 = '" + point3
	+ "') AND NOT (point4 = '" + point2 + "' AND point5 = '" + point3
	+ "' AND point6 = '" + point1
	+ "') AND NOT (point4 = '" + point3 + "' AND point5 = '" + point1
	+ "' AND point6 = '" + point2
	+ "') AND NOT (point4 = '" + point3 + "' AND point5 = '" + point2
	+ "' AND point6 = '" + point1 + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point4, point5, point6 "
	"FROM Facts "
	"INNER JOIN SimilarTriangles "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point4 + "' AND point2 = '" + point5
	+ "' AND point3 = '" + point6
	+ "' AND NOT (point4 = '" + point1 + "' AND point5 = '" + point2
	+ "' AND point6 = '" + point3
	+ "') AND NOT (point4 = '" + point1 + "' AND point5 = '" + point3
	+ "' AND point6 = '" + point2
	+ "') AND NOT (point4 = '" + point2 + "' AND point5 = '" + point1
	+ "' AND point6 = '" + point3
	+ "') AND NOT (point4 = '" + point2 + "' AND point5 = '" + point3
	+ "' AND point6 = '" + point1
	+ "') AND NOT (point4 = '" + point3 + "' AND point5 = '" + point1
	+ "' AND point6 = '" + point2
	+ "') AND NOT (point4 = '" + point3 + "' AND point5 = '" + point2
	+ "' AND point6 = '" + point1 + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt2, 2);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"SimilarTriangles (typeGeoCmd, point1, point2, point3, point4, "
		"point5, point6, newFact) "
		"VALUES "
		"('simtri', '" + point1 + "', '" + point2 + "', '" + point3
		+ "', '" + newPoint1 + "', '" + newPoint2 + "', '" + newPoint3
		+ "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D35: contri(A, C, B, P, R, Q) -> contri(A, B, C, P, Q, R)
 */
DBinMemory Prover::ruleD35(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('contri')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"CongruentTriangles (typeGeoCmd, point1, point2, point3, point4, "
	"point5, point6, newFact) "
	"VALUES "
	"('contri', '" + point1 + "', '" + point3 + "', '" + point2 + "', '"
	+ point4 + "', '" + point6 + "',  '" + point5 + "', '" + lstInsRwId
	+ "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D36: contri(B, A, C, Q, P, R) -> contri(A, B, C, P, Q, R)
 */
DBinMemory Prover::ruleD36(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('contri')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"CongruentTriangles (typeGeoCmd, point1, point2, point3, point4, "
	"point5, point6, newFact) "
	"VALUES "
	"('contri', '" + point2 + "', '" + point1 + "', '" + point3 + "', '"
	+ point5 + "', '" + point4 + "', '" + point6 + "', '" + lstInsRwId
	+ "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D37: contri(P, Q, R, A, B, C) -> contri(A, B, C, P, Q, R)
 */
DBinMemory Prover::ruleD37(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('contri')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"CongruentTriangles (typeGeoCmd, point1, point2, point3, point4, "
	"point5, point6, newFact) "
	"VALUES "
	"('contri', '" + point4 + "', '" + point5 + "', '" + point6 + "', '"
	+ point1 + "', '" + point2 + "', '" + point3 + "', '" + lstInsRwId
	+ "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D38: contri(A, B, C, E, F, G) & contri(E, F, G, P, Q, R)
 *               -> contri(A, B, C, P, Q, R)
 */
DBinMemory Prover::ruleD38(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2, newPoint3;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('contri')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg)); 
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point4, point5, point6 "
	"FROM NewFact "
	"INNER JOIN CongruentTriangles "
	"ON (newFact = id) "
	"WHERE point1 = '" + point4 + "' AND point2 = '" + point5
	+ "' AND point3 = '" + point6
        + "' AND NOT (point4 = '" + point1 + "' AND point5 = '" + point2
	+ "' AND point6 = '" + point3
	+ "') AND NOT (point4 = '" + point1 + "' AND point5 = '" + point3
	+ "' AND point6 = '" + point2
	+ "') AND NOT (point4 = '" + point2 + "' AND point5 = '" + point1
	+ "' AND point6 = '" + point3
	+ "') AND NOT (point4 = '" + point2 + "' AND point5 = '" + point3
	+ "' AND point6 = '" + point1
	+ "') AND NOT (point4 = '" + point3 + "' AND point5 = '" + point1
	+ "' AND point6 = '" + point2
	+ "') AND NOT (point4 = '" + point3 + "' AND point5 = '" + point2
	+ "' AND point6 = '" + point1 + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point4, point5, point6 "
	"FROM Facts "
	"INNER JOIN CongruentTriangles "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point4 + "' AND point2 = '" + point5
	+ "' AND point3 = '" + point6
	+ "' AND NOT (point4 = '" + point1 + "' AND point5 = '" + point2
	+ "' AND point6 = '" + point3
	+ "') AND NOT (point4 = '" + point1 + "' AND point5 = '" + point3
	+ "' AND point6 = '" + point2
	+ "') AND NOT (point4 = '" + point2 + "' AND point5 = '" + point1
	+ "' AND point6 = '" + point3
	+ "') AND NOT (point4 = '" + point2 + "' AND point5 = '" + point3
	+ "' AND point6 = '" + point1
	+ "') AND NOT (point4 = '" + point3 + "' AND point5 = '" + point1
	+ "' AND point6 = '" + point2
	+ "') AND NOT (point4 = '" + point3 + "' AND point5 = '" + point2
	+ "' AND point6 = '" + point1 + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt2, 2);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"CongruentTriangles (typeGeoCmd, point1, point2, point3, "
		"point4, point5, point6, newFact) "
		"VALUES "
		"('contri', '" + point1 + "', '" + point2 + "', '" + point3
		+ "', '" + newPoint1 + "', '" + newPoint2 + "', '" + newPoint3
		+ "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D39: eqangle(A, B, P, Q, C, D, P, Q) -> para(A, B, C, D)
 */
DBinMemory Prover::ruleD39(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('para')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"Parallel (typeGeoCmd, point1, point2, point3, point4, newFact) "
	"VALUES "
	"('para', '" + point1 + "', '" + point2 + "', '" + point5 + "', '"
	+ point6 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D40: para(A, B, C, D) -> eqangle(A, B, P, Q, C, D, P, Q)
 */
DBinMemory Prover::ruleD40(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    struct pointList *ptP, *ptQ;
    extern struct pointList *points;

    ptP = points;
    while (ptP != NULL) {
	if (ptP->pt != point1 && ptP->pt != point2 && ptP->pt != point3
	    && ptP->pt != point4) {
	    ptQ = ptP->next;
	    while (ptQ != NULL) {
		if (ptQ->pt != point1 && ptQ->pt != point2 && ptQ->pt != point3
		    && ptQ->pt != point4) {
		    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) "
			"VALUES ('eqangle')";
		    lastInsertedRowId = "SELECT last_insert_rowid()";
		    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
		    correctTransaction = true;
		    dbim.rc = sqlite3_prepare_v2(dbim.db,
						 insertNewFact.c_str() ,
						 insertNewFact.size(),
						 &(dbim.stmt), NULL);
		    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
			correctTransaction = false;
		    dbim.rc = sqlite3_prepare_v2(dbim.db,
						 lastInsertedRowId.c_str(),
						 lastInsertedRowId.size(),
						 &(dbim.stmt), NULL);
		    sqlite3_step(dbim.stmt);
		    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
		    insertionPred = "INSERT INTO "
			"EqualAngles (typeGeoCmd, point1, point2, point3, "
			"point4, point5, point6, point7, point8, newFact) "
			"VALUES "
			"('eqangle', '" + point1 + "', '" + point2 + "', '"
			+ ptP->pt + "', '" + ptQ->pt + "', '" + point3 + "', '"
			+ point4 + "', '" + ptP->pt + "', '" + ptQ->pt + "', '"
			+ lstInsRwId + "')";
		    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
						 insertionPred.size(),
						 &(dbim.stmt), NULL);
		    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
			correctTransaction = false;
		    if (correctTransaction)
			sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
		    else
			sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
		}
		ptQ = ptQ->next;
	    }
	}
	ptP = ptP->next;
    }
    return dbim;
}

/*
 * Rule D41: cyclic(A, B, P, Q) -> eqangle(P, A, P, B, Q, A, Q, B)
 */
DBinMemory Prover::ruleD41(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqangle')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"EqualAngles (typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6, point7, point8, newFact) "
	"VALUES "
	"('eqangle', '" + point3 + "', '" + point1 + "', '" + point3 + "', '"
	+ point2 + "', '" + point4 + "', '" + point1 + "', '" + point4 + "', '"
	+ point2 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D42: eqangle(P, A, P, B, Q, A, Q, B) & ~ coll(P, Q, A, B)
 *               -> cyclic(A, B, P, Q)
 *
 * Function's argument is eqangle(P, A, P, B, Q, A, Q, B) and verifies if
 * ~ coll(P, Q, A, B).
 */
DBinMemory Prover::ruleD42(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cyclic')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN Collinear "
	"ON (newFact = id) "
	"WHERE point1 IN ('" + point1 + "', '" + point5 + "', '" + point2
	+ "') AND point2 IN ('" + point1 + "', '" + point5 + "', '" + point2
	+ "') AND point2 <> point1 AND point3 IN ('" + point1 + "', '" + point5
	+ "', '" + point2 + "') AND point3 NOT IN (point1, point2)";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN Collinear "
	"ON (oldFact = id) "
	"WHERE point1 IN ('" + point1 + "', '" + point5 + "', '" + point2
	+ "') AND point2 IN ('" + point1 + "', '" + point5 + "', '" + point2
	+ "') AND point2 <> point1 AND point3 IN ('" + point1 + "', '" + point5
	+ "', '" + point2 + "') AND point3 NOT IN (point1, point2)";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) != 0
	|| sqlite3_data_count(dbim.stmt2) != 0 ) {
	correctTransaction = false;
    } else {
	querySecondGeoCmdA = "SELECT * "
	    "FROM NewFact "
	    "INNER JOIN Collinear "
	    "ON (newFact = id) "
	    "WHERE point1 IN ('" + point1 + "', '" + point5 + "', '" + point4
	    + "') AND point2 IN ('" + point1 + "', '" + point5 + "', '" + point4
	    + "') AND point2 <> point1 AND point3 IN ('" + point1 + "', '"
	    + point5 + "', '" + point4
	    + "') AND point3 NOT IN (point1, point2)";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				     querySecondGeoCmdA.size(), &(dbim.stmt1),
				     NULL);
	sqlite3_step(dbim.stmt1);
	querySecondGeoCmdB = "SELECT * "
	    "FROM Facts "
	    "INNER JOIN Collinear "
	    "ON (oldFact = id) "
	    "WHERE point1 IN ('" + point1 + "', '" + point5 + "', '" + point4
	    + "') AND point2 IN ('" + point1 + "', '" + point5 + "', '" + point4
	    + "') AND point2 <> point1 AND point3 IN ('" + point1 + "', '"
	    + point5 + "', '" + point4
	    + "') AND point3 NOT IN (point1, point2)";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				     querySecondGeoCmdB.size(), &(dbim.stmt2),
				     NULL);
	sqlite3_step(dbim.stmt2);
	if (sqlite3_data_count(dbim.stmt1) != 0
	    || sqlite3_data_count(dbim.stmt2) != 0 ) {
	    correctTransaction = false;
	} else {
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    } else {
		insertionPred = "INSERT INTO "
		    "Cyclic (typeGeoCmd, point1, point2, point3, point4, "
		    "newFact) "
		    "VALUES "
		    "('cyclic', '" + point2 + "', '" + point4 + "', '" + point1
		    + "', '" + point5 + "', '" + lstInsRwId + "')";
		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					     insertionPred.size(), &(dbim.stmt),
					     NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		    correctTransaction = false;
	    }
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D43: cyclic(A, B, C, P, Q, R) & eqangle(C, A, C, B, R, P, R, Q)
 *               -> cong(A, B, P, Q)
 *
 * Function's argument is cyclic(A, B, C, P, Q, R) and searches for 
 * eqangle(C, A, C, B, R, P, R, Q).
 */
DBinMemory Prover::ruleD43cyclic(DBinMemory dbim, std::string point1,
				 std::string point2, std::string point3,
				 std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cong')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point4 "
	"FROM NewFact "
	"INNER JOIN Cyclic "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point1
	+ "' AND point3 = '" + point3
	+ "' AND point4 <> '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point4 "
	"FROM Facts "
	"INNER JOIN Cyclic "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point1
	+ "' AND point3 = '" + point3
	+ "' AND point4 <> '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0)
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	else
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	querySecondGeoCmdA = "SELECT point4 "
	    "FROM NewFact "
	    "INNER JOIN Cyclic "
	    "ON (newFact = id) "
	    "WHERE point1 = '" + point1 + "' AND point2 = '" + point1
	    + "' AND point3 = '" + point3
	    + "' AND point4 NOT IN ('" + point4 + "', '" + newPoint1 + "')";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				     querySecondGeoCmdA.size(), &(dbim.stmt1),
				     NULL);
	sqlite3_step(dbim.stmt1);
	querySecondGeoCmdB = "SELECT point4 "
	    "FROM Facts "
	    "INNER JOIN Cyclic "
	    "ON (oldFact = id) "
	    "WHERE point1 = '" + point1 + "' AND point2 = '" + point1
	    + "' AND point3 = '" + point3
	    + "' AND point4 NOT IN ('" + point4 + "', '" + newPoint1 + "')";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				     querySecondGeoCmdB.size(), &(dbim.stmt2),
				     NULL);
	sqlite3_step(dbim.stmt2);
	if (sqlite3_data_count(dbim.stmt1) == 0
	    && sqlite3_data_count(dbim.stmt2) == 0 ) {
	    correctTransaction = false;
	} else {
	    if (sqlite3_data_count(dbim.stmt1) != 0)
		newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    else
		newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    querySecondGeoCmdA = "SELECT * "
		"FROM NewFact "
		"INNER JOIN EqualAngles "
		"ON (newFact = id) "
		"WHERE point1 = '" + point3 + "' AND point2 = '" + point1
		+ "' AND point3 = '" + point3 + "' AND point4 = '" + point2
		+ "' AND point5 = '" + newPoint2 + "' AND point6 = '" + point4
		+ "' AND point7 = '" + newPoint2 + "' AND point8 = '"
		+ newPoint1 + "'";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
					 querySecondGeoCmdA.size(),
					 &(dbim.stmt1), NULL);
	    sqlite3_step(dbim.stmt1);
	    querySecondGeoCmdB = "SELECT * "
		"FROM Facts "
		"INNER JOIN EqualAngles "
		"ON (oldFact = id) "
		"WHERE point1 = '" + point3 + "' AND point2 = '" + point1
		+ "' AND point3 = '" + point3 + "' AND point4 = '" + point2
		+ "' AND point5 = '" + newPoint2 + "' AND point6 = '" + point4
		+ "' AND point7 = '" + newPoint2 + "' AND point8 = '"
		+ newPoint1 + "'";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
					 querySecondGeoCmdB.size(),
					 &(dbim.stmt2), NULL);
	    sqlite3_step(dbim.stmt2);
	    if (sqlite3_data_count(dbim.stmt1) == 0
		&& sqlite3_data_count(dbim.stmt2) == 0 ) {
		correctTransaction = false;
	    } else {
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		    correctTransaction = false;
		} else {
		    insertionPred = "INSERT INTO "
			"CongruentSegments (typeGeoCmd, point1, point2, "
			"point3, point4, newFact) "
			"VALUES "
			"('cong', '" + point1 + "', '" + point2 + "', '"
			+ point4 + "', '" + newPoint1 + "', '" + lstInsRwId
			+ "')";
		    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
						 insertionPred.size(),
						 &(dbim.stmt), NULL);
		    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
			correctTransaction = false;
		}
	    }
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D43: cyclic(A, B, C, P, Q, R) & eqangle(C, A, C, B, R, P, R, Q)
 *               -> cong(A, B, P, Q)
 *
 * Function's argument is eqangle(C, A, C, B, R, P, R, Q) and searches for 
 * cyclic(A, B, C, P, Q, R).
 */
DBinMemory Prover::ruleD43eqangle(DBinMemory dbim, std::string point1,
				  std::string point2, std::string point3,
				  std::string point4, std::string point5,
				  std::string point6, std::string point7,
				  std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cong')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN Cyclic "
	"ON (newFact = id) "
	"WHERE point1 = '" + point2 + "' AND point2 = '" + point4
	+ "' AND point3 = '" + point1 + "' AND point4 = '" + point6 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN Cyclic "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point2 + "' AND point2 = '" + point4
	+ "' AND point3 = '" + point1 + "' AND point4 = '" + point6 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	querySecondGeoCmdA = "SELECT * "
	    "FROM NewFact "
	    "INNER JOIN Cyclic "
	    "ON (newFact = id) "
	    "WHERE point1 = '" + point2 + "' AND point2 = '" + point4
	    + "' AND point3 = '" + point1 + "' AND point4 = '" + point8 + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				     querySecondGeoCmdA.size(), &(dbim.stmt1),
				     NULL);
	sqlite3_step(dbim.stmt1);
	querySecondGeoCmdB = "SELECT * "
	    "FROM Facts "
	    "INNER JOIN Cyclic "
	    "ON (oldFact = id) "
	    "WHERE point1 = '" + point2 + "' AND point2 = '" + point4
	    + "' AND point3 = '" + point1 + "' AND point4 = '" + point8 + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				     querySecondGeoCmdB.size(), &(dbim.stmt2),
				     NULL);
	sqlite3_step(dbim.stmt2);
	if (sqlite3_data_count(dbim.stmt1) == 0
	    && sqlite3_data_count(dbim.stmt2) == 0 ) {
	    correctTransaction = false;
	} else {
	    querySecondGeoCmdA = "SELECT * "
		"FROM NewFact "
		"INNER JOIN Cyclic "
		"ON (newFact = id) "
		"WHERE point1 = '" + point2 + "' AND point2 = '" + point4
		+ "' AND point3 = '" + point1 + "' AND point4 = '" + point5
		+ "'";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
					 querySecondGeoCmdA.size(),
					 &(dbim.stmt1), NULL);
	    sqlite3_step(dbim.stmt1);
	    querySecondGeoCmdB = "SELECT * "
		"FROM Facts "
		"INNER JOIN Cyclic "
		"ON (oldFact = id) "
		"WHERE point1 = '" + point2 + "' AND point2 = '" + point4
		+ "' AND point3 = '" + point1 + "' AND point4 = '" + point5
		+ "'";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
					 querySecondGeoCmdB.size(),
					 &(dbim.stmt2), NULL);
	    sqlite3_step(dbim.stmt2);
	    if (sqlite3_data_count(dbim.stmt1) == 0
		&& sqlite3_data_count(dbim.stmt2) == 0 ) {
		correctTransaction = false;
	    } else {
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		    correctTransaction = false;
		} else {
		    insertionPred = "INSERT INTO "
			"CongruentSegments (typeGeoCmd, point1, point2, "
			"point3, point4, newFact) "
			"VALUES "
			"('cong', '" + point2 + "', '" + point4 + "', '"
			+ point6 + "', '" + point8 + "', '" + lstInsRwId
			+ "')";
		    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
						 insertionPred.size(),
						 &(dbim.stmt), NULL);
		    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
			correctTransaction = false;
		}
	    }
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D44: midp(E, A, B) & midp(F, A, C) -> para(E, F, B, C)
 */
DBinMemory Prover::ruleD44(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('para')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point1, point3 "
	"FROM NewFact "
	"INNER JOIN Midpoint "
	"ON (newFact = id) "
	"WHERE point2 = '" + point2 + "' AND point1 <> '" + point1 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point1, point3 "
	"FROM Facts "
	"INNER JOIN Midpoint "
	"ON (oldFact = id) "
	"WHERE point2 = '" + point2 + "' AND point1 <> '" + point1 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Parallel (typeGeoCmd, point1, point2, point3, point4, "
		"newFact) "
		"VALUES "
		"('para', '" + point1 + "', '" + newPoint1 + "', '"
		+ point3 + "', '" + newPoint2 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D45: midp(E, A, B) & para(E, F, B, C) & coll(F, A, C) -> midp(F, A, C)
 *
 * Function's argument is coll(F, A, C) and searches for midp(E, A, B) and
 * para(E, F, B, C).
 */
DBinMemory Prover::ruleD45coll(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('midp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point1, point3 "
	"FROM NewFact "
	"INNER JOIN Midpoint "
	"ON (newFact = id) "
	"WHERE point2 = '" + point2 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point1, point3 "
	"FROM Facts "
	"INNER JOIN Midpoint "
	"ON (oldFact = id) "
	"WHERE point2 = '" + point2 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	querySecondGeoCmdA = "SELECT * "
	    "FROM NewFact "
	    "INNER JOIN Parallel "
	    "ON (newFact = id) "
	    "WHERE point1 = '" + newPoint1 + "' AND point2 = '" + point1
	    + "' AND point3 = '" + newPoint2 + "' AND point4 = '" + point3
	    + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				     querySecondGeoCmdA.size(), &(dbim.stmt1),
				     NULL);
	sqlite3_step(dbim.stmt1);
	querySecondGeoCmdB = "SELECT * "
	    "FROM Facts "
	    "INNER JOIN Parallel "
	    "ON (oldFact = id) "
	    "WHERE point1 = '" + newPoint1 + "' AND point2 = '" + point1
	    + "' AND point3 = '" + newPoint2 + "' AND point4 = '" + point3
	    + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				     querySecondGeoCmdB.size(), &(dbim.stmt2),
				     NULL);
	sqlite3_step(dbim.stmt2);
	if (sqlite3_data_count(dbim.stmt1) == 0
	    && sqlite3_data_count(dbim.stmt2) == 0 ) {
	    correctTransaction = false;
	} else {
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    } else {
		insertionPred = "INSERT INTO "
		    "Midpoint (typeGeoCmd, point1, point2, point3, newFact) "
		    "VALUES "
		    "('midp', '" + point1 + "', '" + point2 + "', '" + point3
		    + "', '" + lstInsRwId + "')";
		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					     insertionPred.size(), &(dbim.stmt),
					     NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		    correctTransaction = false;
	    }
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D45: midp(E, A, B) & para(E, F, B, C) & coll(F, A, C) -> midp(F, A, C)
 *
 * Function's argument is midp(E, A, B) and searches for coll(F, A, C) and
 * para(E, F, B, C).
 */
DBinMemory Prover::ruleD45midp(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('midp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point1, point3 "
	"FROM NewFact "
	"INNER JOIN Collinear "
	"ON (newFact = id) "
	"WHERE point2 = '" + point2 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point1, point3 "
	"FROM Facts "
	"INNER JOIN Collinear "
	"ON (oldFact = id) "
	"WHERE point2 = '" + point2 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	querySecondGeoCmdA = "SELECT * "
	    "FROM NewFact "
	    "INNER JOIN Parallel "
	    "ON (newFact = id) "
	    "WHERE point1 = '" + point1 + "' AND point2 = '" + newPoint1
	    + "' AND point3 = '" + point3 + "' AND point4 = '" + newPoint2
	    + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				     querySecondGeoCmdA.size(), &(dbim.stmt1),
				     NULL);
	sqlite3_step(dbim.stmt1);
	querySecondGeoCmdB = "SELECT * "
	    "FROM Facts "
	    "INNER JOIN Parallel "
	    "ON (oldFact = id) "
	    "WHERE point1 = '" + point1 + "' AND point2 = '" + newPoint1
	    + "' AND point3 = '" + point3 + "' AND point4 = '" + newPoint2
	    + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				     querySecondGeoCmdB.size(), &(dbim.stmt2),
				     NULL);
	sqlite3_step(dbim.stmt2);
	if (sqlite3_data_count(dbim.stmt1) == 0
	    && sqlite3_data_count(dbim.stmt2) == 0 ) {
	    correctTransaction = false;
	} else {
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    } else {
		insertionPred = "INSERT INTO "
		    "Midpoint (typeGeoCmd, point1, point2, point3, newFact) "
		    "VALUES "
		    "('midp', '" + newPoint1 + "', '" + point2 + "', '"
		    + newPoint2 + "', '" + lstInsRwId + "')";
		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					     insertionPred.size(), &(dbim.stmt),
					     NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		    correctTransaction = false;
	    }
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D45: midp(E, A, B) & para(E, F, B, C) & coll(F, A, C) -> midp(F, A, C)
 *
 * Function's argument is para(E, F, B, C) and searches for coll(F, A, C) and
 * midp(E, A, B).
 */
DBinMemory Prover::ruleD45para(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3,
			       std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('midp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point2 "
	"FROM NewFact "
	"INNER JOIN Collinear "
	"ON (newFact = id) "
	"WHERE point1 = '" + point2 + "' AND point3 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point2 "
	"FROM Facts "
	"INNER JOIN Collinear "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point2 + "' AND point3 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0)
	    newPoint = (char*) sqlite3_column_text(dbim.stmt1, 0);
	else
	    newPoint = (char*) sqlite3_column_text(dbim.stmt2, 0);
	querySecondGeoCmdA = "SELECT * "
	    "FROM NewFact "
	    "INNER JOIN Midpoint "
	    "ON (newFact = id) "
	    "WHERE point1 = '" + point1 + "' AND point2 = '" + newPoint
	    + "' AND point3 = '" + point3 + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				     querySecondGeoCmdA.size(), &(dbim.stmt1),
				     NULL);
	sqlite3_step(dbim.stmt1);
	querySecondGeoCmdB = "SELECT * "
	    "FROM Facts "
	    "INNER JOIN Midpoint "
	    "ON (oldFact = id) "
	    "WHERE point1 = '" + point1 + "' AND point2 = '" + newPoint
	    + "' AND point3 = '" + point3 + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				     querySecondGeoCmdB.size(), &(dbim.stmt2),
				     NULL);
	sqlite3_step(dbim.stmt2);
	if (sqlite3_data_count(dbim.stmt1) == 0
	    && sqlite3_data_count(dbim.stmt2) == 0 ) {
	    correctTransaction = false;
	} else {
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    } else {
		insertionPred = "INSERT INTO "
		    "Midpoint (typeGeoCmd, point1, point2, point3, newFact) "
		    "VALUES "
		    "('midp', '" + point2 + "', '" + newPoint + "', '" + point4
		    + "', '" + lstInsRwId + "')";
		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					     insertionPred.size(), &(dbim.stmt),
					     NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		    correctTransaction = false;
	    }
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D46: cong(O, A, O, B) -> eqangle(O, A, A, B, A, B, O, B)
 */
DBinMemory Prover::ruleD46(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqangle')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"EqualAngles (typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6, point7, point8, newFact) "
	"VALUES "
	"('eqangle', '" + point1 + "', '" + point2 + "', '" + point2 + "', '"
	+ point4 + "', '" + point2 + "', '" + point4 + "', '" + point1 + "', '"
	+ point4 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D47: eqangle(O, A, A, B, A, B, O, B) & ~ coll(O, A, B)
 *               -> cong(O, A, O, B)
 *
 * Function's argument is eqangle(A, B, C, D, C, D, A, B) and verifies if
 * ~ perp(A, B, C, D).
 */
DBinMemory Prover::ruleD47(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cong')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN Collinear "
	"ON (newFact = id) "
	"WHERE point1 IN ('" + point1 + "', '" + point2 + "', '" + point4
	+ "') AND point2 IN ('" + point1 + "', '" + point2 + "', '" + point4
	+ "') AND point2 <> point1 AND point3 IN ('" + point1 + "', '" + point2
	+ "', '" + point4 + "') AND point3 NOT IN (point1, point2)";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN Collinear "
	"ON (oldFact = id) "
	"WHERE point1 IN ('" + point1 + "', '" + point2 + "', '" + point4
	+ "') AND point2 IN ('" + point1 + "', '" + point2 + "', '" + point4
	+ "') AND point2 <> point1 AND point3 IN ('" + point1 + "', '" + point2
	+ "', '" + point4 + "') AND point3 NOT IN (point1, point2)";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) != 0
	|| sqlite3_data_count(dbim.stmt2) != 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"CongruentSegments (typeGeoCmd, point1, point2, point3, point4,"
		"newFact) "
		"VALUES "
		"('cong', '" + point1 + "', '" + point2 + "', '" + point1
		+ "', '" + point4 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D48: circle(O, A, B, C) & perp(O, A, A, X)
 *               -> eqangle(A, X, A, B, C, A, C, B)
 *
 * Function's argument is circle(O, A, B, C) and searches for perp(O, A, A, X).
 */
DBinMemory Prover::ruleD48circle(DBinMemory dbim, std::string point1,
				 std::string point2, std::string point3,
				 std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqangle')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point4 "
	"FROM NewFact "
	"INNER JOIN Perpendicular "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point2 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point4 "
	"FROM Facts "
	"INNER JOIN Perpendicular "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point2 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0)
	    newPoint = (char*) sqlite3_column_text(dbim.stmt1, 0);
	else
	    newPoint = (char*) sqlite3_column_text(dbim.stmt2, 0);
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"EqualAngles (typeGeoCmd, point1, point2, point3, point4, "
		"point5, point6, point7, point8, newFact) "
		"VALUES "
		"('eqangle', '" + point2 + "', '" + newPoint + "', '" + point2
		+ "', '" + point3 + "', '" + point4 + "', '" + point2 + "', '"
		+ point4 + "', '" + point3 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D48: circle(O, A, B, C) & perp(O, A, A, X)
 *               -> eqangle(A, X, A, B, C, A, C, B)
 *
 * Function's argument is perp(O, A, A, X) and searches for circle(O, A, B, C).
 */
DBinMemory Prover::ruleD48perp(DBinMemory dbim, std::string point1,
				 std::string point2, std::string point3,
				 std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqangle')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point3, point4 "
	"FROM NewFact "
	"INNER JOIN Circle "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point3, point4 "
	"FROM Facts "
	"INNER JOIN Circle "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"EqualAngles (typeGeoCmd, point1, point2, point3, point4, "
		"point5, point6, point7, point8, newFact) "
		"VALUES "
		"('eqangle', '" + point2 + "', '" + point4 + "', '" + point2
		+ "', '" + newPoint1 + "', '" + newPoint2 + "', '" + point2
		+ "', '" + newPoint2 + "', '" + newPoint1 + "', '" + lstInsRwId
		+ "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D49: circle(O, A, B, C) & eqangle(A, X, A, B, C, A, C, B)
 *               -> perp(O, A, A, X)
 *
 * Function's argument is circle(O, A, B, C) and searches for
 * eqangle(A, X, A, B, C, A, C, B).
 */
DBinMemory Prover::ruleD49circle(DBinMemory dbim, std::string point1,
				 std::string point2, std::string point3,
				 std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('perp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point2 "
	"FROM NewFact "
	"INNER JOIN EqualAngles "
	"ON (newFact = id) "
	"WHERE point1 = '" + point2 + "' AND point3 = '" + point2
	+ "' AND point4 = '" + point3 + "' AND point5 = '" + point4
	+ "' AND point6 = '" + point2 + "' AND point7 = '" + point4
	+ "' AND point8 = '" + point3 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point2 "
	"FROM Facts "
	"INNER JOIN EqualAngles "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point2 + "' AND point3 = '" + point2
	+ "' AND point4 = '" + point3 + "' AND point5 = '" + point4
	+ "' AND point6 = '" + point2 + "' AND point7 = '" + point4
	+ "' AND point8 = '" + point3 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0)
	    newPoint = (char*) sqlite3_column_text(dbim.stmt1, 0);
	else
	    newPoint = (char*) sqlite3_column_text(dbim.stmt2, 0);
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Perpendicular (typeGeoCmd, point1, point2, point3, point4, "
		"newFact) "
		"VALUES "
		"('perp', '" + point1 + "', '" + point2 + "', '" + point2
		+ "', '" + newPoint + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D49: circle(O, A, B, C) & eqangle(A, X, A, B, C, A, C, B)
 *               -> perp(O, A, A, X)
 *
 * Function's argument is eqangle(A, X, A, B, C, A, C, B) and searches for
 * circle(O, A, B, C).
 */
DBinMemory Prover::ruleD49eqangle(DBinMemory dbim, std::string point1,
				  std::string point2, std::string point3,
				  std::string point4, std::string point5,
				  std::string point6, std::string point7,
				  std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('perp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point1 "
	"FROM NewFact "
	"INNER JOIN Circle "
	"ON (newFact = id) "
	"WHERE point2 = '" + point1 + "' AND point3 = '" + point4
	+ "' AND point4 = '" + point5 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point1 "
	"FROM Facts "
	"INNER JOIN Circle "
	"ON (oldFact = id) "
	"WHERE point2 = '" + point1 + "' AND point3 = '" + point4
	+ "' AND point4 = '" + point5 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0)
	    newPoint = (char*) sqlite3_column_text(dbim.stmt1, 0);
	else
	    newPoint = (char*) sqlite3_column_text(dbim.stmt2, 0);
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Perpendicular (typeGeoCmd, point1, point2, point3, point4, "
		"newFact) "
		"VALUES "
		"('perp', '" + newPoint + "', '" + point1 + "', '" + point1
		+ "', '" + point2 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D50: circle(O, A, B, C) & midp(M, B, C)
 *               -> eqangle(A, B, A, C, O, B, O, M)
 *
 * Function's argument is circle(O, A, B, C) and searches for midp(M, B, C).
 */
DBinMemory Prover::ruleD50circle(DBinMemory dbim, std::string point1,
				 std::string point2, std::string point3,
				 std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqangle')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point1 "
	"FROM NewFact "
	"INNER JOIN Midpoint "
	"ON (newFact = id) "
	"WHERE point2 = '" + point3 + "' AND point3 = '" + point4
	+ "' AND point1 <> '" + point1 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point1 "
	"FROM Facts "
	"INNER JOIN Midpoint "
	"ON (oldFact = id) "
	"WHERE point2 = '" + point3 + "' AND point3 = '" + point4
	+ "' AND point1 <> '" + point1 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0)
	    newPoint = (char*) sqlite3_column_text(dbim.stmt1, 0);
	else
	    newPoint = (char*) sqlite3_column_text(dbim.stmt2, 0);
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"EqualAngles (typeGeoCmd, point1, point2, point3, point4, "
		"point5, point6, point7, point8, newFact) "
		"VALUES "
		"('eqangle', '" + point2 + "', '" + point3 + "', '" + point2
		+ "', '" + point4 + "', '" + point1 + "', '" + point3 + "', '"
		+ point1 + "', '" + newPoint + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D50: circle(O, A, B, C) & midp(M, B, C)
 *               -> eqangle(A, B, A, C, O, B, O, M)
 *
 * Function's argument is midp(M, B, C) and searches for circle(O, A, B, C).
 */
DBinMemory Prover::ruleD50midp(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqangle')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point1, point2 "
	"FROM NewFact "
	"INNER JOIN Circle "
	"ON (newFact = id) "
	"WHERE point3 = '" + point2 + "' AND point4 = '" + point3
	+ "' AND point1 <> '" + point1 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point1, point2 "
	"FROM Facts "
	"INNER JOIN Circle "
	"ON (oldFact = id) "
	"WHERE point3 = '" + point2 + "' AND point4 = '" + point3
	+ "' AND point1 <> '" + point1 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"EqualAngles (typeGeoCmd, point1, point2, point3, point4, "
		"point5, point6, point7, point8, newFact) "
		"VALUES "
		"('eqangle', '" + newPoint2 + "', '" + point2 + "', '"
		+ newPoint2 + "', '" + point3 + "', '" + newPoint1 + "', '"
		+ point2 + "', '" + newPoint1 + "', '" + point1 + "', '"
		+ lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D51: circle(O, A, B, C) & coll(M, B, C) 
 *               & eqangle(A, B, A, C, O, B, O, M) -> midp(M, B, C)
 *
 * Function's argument is circle(O, A, B, C) and searches for coll(M, B, C)
 * and eqangle(A, B, A, C, O, B, O, M).
 */
DBinMemory Prover::ruleD51circle(DBinMemory dbim, std::string point1,
				 std::string point2, std::string point3,
				 std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('midp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point1 "
	"FROM NewFact "
	"INNER JOIN Collinear "
	"ON (newFact = id) "
	"WHERE point2 = '" + point3 + "' AND point3 = '" + point4
	+ "' AND point1 <> '" + point1 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point1 "
	"FROM Facts "
	"INNER JOIN Collinear "
	"ON (oldFact = id) "
	"WHERE point2 = '" + point3 + "' AND point3 = '" + point4
	+ "' AND point1 <> '" + point1 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0)
	    newPoint = (char*) sqlite3_column_text(dbim.stmt1, 0);
	else
	    newPoint = (char*) sqlite3_column_text(dbim.stmt2, 0);
	querySecondGeoCmdA = "SELECT * "
	    "FROM NewFact "
	    "INNER JOIN EqualAngles "
	    "ON (newFact = id) "
	    "WHERE point1 = '" + point2 + "' AND point2 = '" + point3
	    + "' AND point3 = '" + point2 + "' AND point4 = '" + point4
	    + "' AND point5 = '" + point1 + "' AND point6 = '" + point3
	    + "' AND point7 = '" + point1 + "' AND point8 = '" + newPoint + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				     querySecondGeoCmdA.size(), &(dbim.stmt1),
				     NULL);
	sqlite3_step(dbim.stmt1);
	querySecondGeoCmdB = "SELECT * "
	    "FROM Facts "
	    "INNER JOIN EqualAngles "
	    "ON (oldFact = id) "
	    "WHERE point1 = '" + point2 + "' AND point2 = '" + point3
	    + "' AND point3 = '" + point2 + "' AND point4 = '" + point4
	    + "' AND point5 = '" + point1 + "' AND point6 = '" + point3
	    + "' AND point7 = '" + point1 + "' AND point8 = '" + newPoint + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				     querySecondGeoCmdB.size(), &(dbim.stmt2),
				     NULL);
	sqlite3_step(dbim.stmt2);
	if (sqlite3_data_count(dbim.stmt1) == 0
	    && sqlite3_data_count(dbim.stmt2) == 0 ) {
	    correctTransaction = false;
	} else {
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    } else {
		insertionPred = "INSERT INTO "
		    "Midpoint (typeGeoCmd, point1, point2, point3, newFact) "
		    "VALUES "
		    "('midp', '" + newPoint + "', '" + point3 + "', '" + point4
		    + "', '" + lstInsRwId + "')";
		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					     insertionPred.size(), &(dbim.stmt),
					     NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		    correctTransaction = false;
	    }
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D51: circle(O, A, B, C) & coll(M, B, C) 
 *               & eqangle(A, B, A, C, O, B, O, M) -> midp(M, B, C)
 *
 * Function's argument is coll(M, B, C) and searches for circle(O, A, B, C)
 * and eqangle(A, B, A, C, O, B, O, M).
 */
DBinMemory Prover::ruleD51coll(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('midp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point1, point2 "
	"FROM NewFact "
	"INNER JOIN Circle "
	"ON (newFact = id) "
	"WHERE point3 = '" + point2 + "' AND point4 = '" + point3
	+ "' AND point1 <> '" + point1 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point1, point2 "
	"FROM Facts "
	"INNER JOIN Circle "
	"ON (oldFact = id) "
	"WHERE point3 = '" + point2 + "' AND point4 = '" + point3
	+ "' AND point1 <> '" + point1 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	querySecondGeoCmdA = "SELECT * "
	    "FROM NewFact "
	    "INNER JOIN EqualAngles "
	    "ON (newFact = id) "
	    "WHERE point1 = '" + newPoint2 + "' AND point2 = '" + point2
	    + "' AND point3 = '" + newPoint2 + "' AND point4 = '" + point3
	    + "' AND point5 = '" + newPoint1 + "' AND point6 = '" + point2
	    + "' AND point7 = '" + newPoint1 + "' AND point8 = '" + point1
	    + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				     querySecondGeoCmdA.size(), &(dbim.stmt1),
				     NULL);
	sqlite3_step(dbim.stmt1);
	querySecondGeoCmdB = "SELECT * "
	    "FROM Facts "
	    "INNER JOIN EqualAngles "
	    "ON (oldFact = id) "
	    "WHERE point1 = '" + newPoint2 + "' AND point2 = '" + point2
	    + "' AND point3 = '" + newPoint2 + "' AND point4 = '" + point3
	    + "' AND point5 = '" + newPoint1 + "' AND point6 = '" + point2
	    + "' AND point7 = '" + newPoint1 + "' AND point8 = '" + point1
	    + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				     querySecondGeoCmdB.size(), &(dbim.stmt2),
				     NULL);
	sqlite3_step(dbim.stmt2);
	if (sqlite3_data_count(dbim.stmt1) == 0
	    && sqlite3_data_count(dbim.stmt2) == 0 ) {
	    correctTransaction = false;
	} else {
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    } else {
		insertionPred = "INSERT INTO "
		    "Midpoint (typeGeoCmd, point1, point2, point3, newFact) "
		    "VALUES "
		    "('midp', '" + point1 + "', '" + point2 + "', '" + point3
		    + "', '" + lstInsRwId + "')";
		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					     insertionPred.size(), &(dbim.stmt),
					     NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		    correctTransaction = false;
	    }
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D51: circle(O, A, B, C) & coll(M, B, C) 
 *               & eqangle(A, B, A, C, O, B, O, M) -> midp(M, B, C)
 *
 * Function's argument is eqangle(A, B, A, C, O, B, O, M) and searches for
 * coll(M, B, C) and circle(O, A, B, C).
 */
DBinMemory Prover::ruleD51eqangle(DBinMemory dbim, std::string point1,
				  std::string point2, std::string point3,
				  std::string point4, std::string point5,
				  std::string point6, std::string point7,
				  std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('midp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN Circle "
	"ON (newFact = id) "
	"WHERE point1 = '" + point5 + "' AND point2 = '" + point1
	+ "' AND point3 = '" + point2 + "' AND point4 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN Circle "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point5 + "' AND point2 = '" + point1
	+ "' AND point3 = '" + point2 + "' AND point4 = '" + point4 + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	querySecondGeoCmdA = "SELECT * "
	    "FROM NewFact "
	    "INNER JOIN Collinear "
	    "ON (newFact = id) "
	    "WHERE point1 = '" + point8 + "' AND point2 = '" + point2
	    + "' AND point3 = '" + point4 + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				     querySecondGeoCmdA.size(), &(dbim.stmt1),
				     NULL);
	sqlite3_step(dbim.stmt1);
	querySecondGeoCmdB = "SELECT * "
	    "FROM Facts "
	    "INNER JOIN Collinear "
	    "ON (oldFact = id) "
	    "WHERE point1 = '" + point8 + "' AND point2 = '" + point2
	    + "' AND point3 = '" + point4 + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				     querySecondGeoCmdB.size(), &(dbim.stmt2),
				     NULL);
	sqlite3_step(dbim.stmt2);
	if (sqlite3_data_count(dbim.stmt1) == 0
	    && sqlite3_data_count(dbim.stmt2) == 0 ) {
	    correctTransaction = false;
	} else {
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    } else {
		insertionPred = "INSERT INTO "
		    "Midpoint (typeGeoCmd, point1, point2, point3, newFact) "
		    "VALUES "
		    "('midp', '" + point8 + "', '" + point2 + "', '" + point4
		    + "', '" + lstInsRwId + "')";
		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					     insertionPred.size(), &(dbim.stmt),
					     NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		    correctTransaction = false;
	    }
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D52: perp(A, B, B, C) & midp(M, A, C)
 *               -> cong(A, M, B, M)
 *
 * Function's argument is midp(M, A, C) and searches for perp(A, B, B, C).
 */
DBinMemory Prover::ruleD52midp(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cong')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point2 "
	"FROM NewFact "
	"INNER JOIN Perpendicular "
	"ON (newFact = id) "
	"WHERE point1 = '" + point2 + "' AND point4 = '" + point3
	+ "' AND point2 = point3";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point2 "
	"FROM Facts "
	"INNER JOIN Perpendicular "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point2 + "' AND point4 = '" + point3
	+ "' AND point2 = point3";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0)
	    newPoint = (char*) sqlite3_column_text(dbim.stmt1, 0);
	else
	    newPoint = (char*) sqlite3_column_text(dbim.stmt2, 0);
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"CongruentSegments (typeGeoCmd, point1, point2, point3, "
		"point4, newFact) "
		"VALUES "
		"('cong', '" + point2 + "', '" + point1 + "', '" + newPoint
		+ "', '" + point1 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D52: perp(A, B, B, C) & midp(M, A, C)
 *               -> cong(A, M, B, M)
 *
 * Function's argument is perp(A, B, B, C) and searches for midp(M, A, C).
 */
DBinMemory Prover::ruleD52perp(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3,
			       std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cong')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point1 "
	"FROM NewFact "
	"INNER JOIN Midpoint "
	"ON (newFact = id) "
	"WHERE point2 = '" + point1 + "' AND point3 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point1 "
	"FROM Facts "
	"INNER JOIN Midpoint "
	"ON (oldFact = id) "
	"WHERE point2 = '" + point1 + "' AND point3 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0)
	    newPoint = (char*) sqlite3_column_text(dbim.stmt1, 0);
	else
	    newPoint = (char*) sqlite3_column_text(dbim.stmt2, 0);
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"CongruentSegments (typeGeoCmd, point1, point2, point3, "
		"point4, newFact) "
		"VALUES "
		"('cong', '" + point1 + "', '" + newPoint + "', '" + point2
		+ "', '" + newPoint + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D53: circle(O, A, B, C) & coll(O, A, C) -> perp(A, B, B, A)
 *
 * Function's argument is circle(O, A, B, C) and searches for coll(O, A, C).
 */
DBinMemory Prover::ruleD53circle(DBinMemory dbim, std::string point1,
				 std::string point2, std::string point3,
				 std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('perp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN Collinear "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN Collinear "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Perpendicular (typeGeoCmd, point1, point2, point3, point4, "
		"newFact) "
		"VALUES "
		"('perp', '" + point2 + "', '" + point3 + "', '" + point3
		+ "', '" + point4 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D53: circle(O, A, B, C) & coll(O, A, C) -> perp(A, B, B, A)
 *
 * Function's argument is coll(O, A, C) and searches for circle(O, A, B, C).
 */
DBinMemory Prover::ruleD53coll(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('perp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point3 "
	"FROM NewFact "
	"INNER JOIN Circle "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point4 = '" + point3 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point3 "
	"FROM Facts "
	"INNER JOIN Circle "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point4 = '" + point3 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0)
	    newPoint = (char*) sqlite3_column_text(dbim.stmt1, 0);
	else
	    newPoint = (char*) sqlite3_column_text(dbim.stmt2, 0);
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Perpendicular (typeGeoCmd, point1, point2, point3, point4, "
		"newFact) "
		"VALUES "
		"('perp', '" + point2 + "', '" + newPoint + "', '" + newPoint
		+ "', '" + point3 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D54: cyclic(A, B, C, D) & para(A, B, C, D)
 *               -> eqangle(A, D, C, D, C, D, C, B)
 *
 * Function's argument is cyclic(A, B, C, D) and searches for para(A, B, C, D).
 */
DBinMemory Prover::ruleD54cyclic(DBinMemory dbim, std::string point1,
				 std::string point2, std::string point3,
				 std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqangle')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN Parallel "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point3 + "' AND point4 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN Parallel "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point3 + "' AND point4 = '" + point4 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"EqualAngles (typeGeoCmd, point1, point2, point3, point4, "
		"point5, point6, point7, point8, newFact) "
		"VALUES "
		"('eqangle', '" + point1 + "', '" + point4 + "', '" + point3
		+ "', '" + point4 + "', '" + point3 + "', '" + point4 + "', '"
		+ point3 + "', '" + point2 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D54: cyclic(A, B, C, D) & para(A, B, C, D)
 *               -> eqangle(A, D, C, D, C, D, C, B)
 *
 * Function's argument is para(A, B, C, D) and searches for cyclic(A, B, C, D).
 */
DBinMemory Prover::ruleD54para(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3,
			       std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqangle')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN Cyclic "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point3 + "' AND point4 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN Cyclic "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point3 + "' AND point4 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"EqualAngles (typeGeoCmd, point1, point2, point3, point4, "
		"point5, point6, point7, point8, newFact) "
		"VALUES "
		"('eqangle', '" + point1 + "', '" + point4 + "', '" + point3
		+ "', '" + point4 + "', '" + point3 + "', '" + point4 + "', '"
		+ point3 + "', '" + point2 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D55: midp(M, A, B) & perp(O, M, A, B) -> cong(O, A, O, B)
 *
 * Function's argument is midp(M, A, B) and searches for perp(O, M, A, B).
 */
DBinMemory Prover::ruleD55midp(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cong')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point1 "
	"FROM NewFact "
	"INNER JOIN Perpendicular "
	"ON (newFact = id) "
	"WHERE point2 = '" + point1 + "' AND point3 = '" + point2
	+ "' AND point4 = '" + point3 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point1 "
	"FROM Facts "
	"INNER JOIN Perpendicular "
	"ON (oldFact = id) "
	"WHERE point2 = '" + point1 + "' AND point3 = '" + point2
	+ "' AND point4 = '" + point3 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0)
	    newPoint = (char*) sqlite3_column_text(dbim.stmt1, 0);
	else
	    newPoint = (char*) sqlite3_column_text(dbim.stmt2, 0);
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"CongruentSegments (typeGeoCmd, point1, point2, point3, point4,"
		"newFact) "
		"VALUES "
		"('cong', '" + newPoint + "', '" + point2 + "', '" + newPoint
		+ "', '" + point3 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D55: midp(M, A, B) & perp(O, M, A, B) -> cong(O, A, O, B)
 *
 * Function's argument is perp(O, M, A, B) and searches for midp(M, A, C).
 */
DBinMemory Prover::ruleD55perp(DBinMemory dbim, std::string point1,
				 std::string point2, std::string point3,
				 std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cong')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN Midpoint "
	"ON (newFact = id) "
	"WHERE point1 = '" + point2 + "' AND point2 = '" + point3
	+ "' AND point3 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN Midpoint "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point2 + "' AND point2 = '" + point3
	+ "' AND point3 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"CongruentSegments (typeGeoCmd, point1, point2, point3, point4,"
		"newFact) "
		"VALUES "
		"('cong', '" + point1 + "', '" + point3 + "', '" + point1
		+ "', '" + point4 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D56: cong(A, P, B, P) & cong(A, Q, B, Q) -> perp(A, B, P, Q)
 */
DBinMemory Prover::ruleD56(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('perp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point2 "
	"FROM NewFact "
	"INNER JOIN CongruentSegments "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point3 = '" + point3
	+ "' AND point2 <> '" + point2 + "' AND point2 = point4";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point2 "
	"FROM Facts "
	"INNER JOIN CongruentSegments "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point3 = '" + point3
	+ "' AND point2 <> '" + point2 + "' AND point2 = point4";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0)
	    newPoint = (char*) sqlite3_column_text(dbim.stmt1, 0);
	else
	    newPoint = (char*) sqlite3_column_text(dbim.stmt2, 0);
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Perpendicular (typeGeoCmd, point1, point2, point3, point4, "
		"newFact) "
		"VALUES "
		"('perp', '" + point1 + "', '" + point3 + "', '" + point2
		+ "', '" + newPoint + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D57: cong(A, P, B, P) & cong(A, Q, B, Q) & cyclic(A, B, P, Q)
 *               -> perp(P, A, A, Q)
 *
 * Function's argument is cong(A, P, B, P) and searches for cong(A, Q, B, Q)
 * and cyclic(A, B, P, Q).
 */
DBinMemory Prover::ruleD57cong(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3,
			       std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('perp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point2 "
	"FROM NewFact "
	"INNER JOIN CongruentSegments "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point3 = '" + point3
	+ "' AND point2 <> '" + point2 + "' AND point2 = point4";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point2 "
	"FROM Facts "
	"INNER JOIN CongruentSegments "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point3 = '" + point3
	+ "' AND point2 <> '" + point2 + "' AND point2 = point4";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0)
	    newPoint = (char*) sqlite3_column_text(dbim.stmt1, 0);
	else
	    newPoint = (char*) sqlite3_column_text(dbim.stmt2, 0);
	querySecondGeoCmdA = "SELECT * "
	    "FROM NewFact "
	    "INNER JOIN Cyclic "
	    "ON (newFact = id) "
	    "WHERE point1 = '" + point1 + "' AND point2 = '" + point3
	    + "' AND point3 = '" + point2 + "' AND point4 = '" + newPoint
	    + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				     querySecondGeoCmdA.size(), &(dbim.stmt1),
				     NULL);
	sqlite3_step(dbim.stmt1);
	querySecondGeoCmdB = "SELECT * "
	    "FROM Facts "
	    "INNER JOIN Cyclic "
	    "ON (oldFact = id) "
	    "WHERE point1 = '" + point1 + "' AND point2 = '" + point3
	    + "' AND point3 = '" + point2 + "' AND point4 = '" + newPoint
	    + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				     querySecondGeoCmdB.size(), &(dbim.stmt2),
				     NULL);
	sqlite3_step(dbim.stmt2);
	if (sqlite3_data_count(dbim.stmt1) == 0
	    && sqlite3_data_count(dbim.stmt2) == 0 ) {
	    correctTransaction = false;
	} else {
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    } else {
		insertionPred = "INSERT INTO "
		    "Perpendicular (typeGeoCmd, point1, point2, point3, point4,"
		    "newFact) "
		    "VALUES "
		    "('perp', '" + point2 + "', '" + point1 + "', '" + point1
		    + "', '" + newPoint + "', '" + lstInsRwId + "')";
		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					     insertionPred.size(), &(dbim.stmt),
					     NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		    correctTransaction = false;
	    }
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D57: cong(A, P, B, P) & cong(A, Q, B, Q) & cyclic(A, B, P, Q)
 *               -> perp(P, A, A, Q)
 *
 * Function's argument is cyclic(A, B, P, Q) and searches for cong(A, P, B, P)
 * and cong(A, Q, B, Q).
 */
DBinMemory Prover::ruleD57cyclic(DBinMemory dbim, std::string point1,
				 std::string point2, std::string point3,
				 std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('perp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN CongruentSegments "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point3
	+ "' AND point3 = '" + point2 + "', AND point4 = '" + point3 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN CongruentSegments "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point3
	+ "' AND point3 = '" + point2 + "', AND point4 = '" + point3 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	querySecondGeoCmdA = "SELECT * "
	    "FROM NewFact "
	    "INNER JOIN CongruentSegments "
	    "ON (newFact = id) "
	    "WHERE point1 = '" + point1 + "' AND point2 = '" + point4
	    + "' AND point3 = '" + point2 + "' AND point4 = '" + point4 + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				     querySecondGeoCmdA.size(), &(dbim.stmt1),
				     NULL);
	sqlite3_step(dbim.stmt1);
	querySecondGeoCmdB = "SELECT * "
	    "FROM Facts "
	    "INNER JOIN CongruentSegments "
	    "ON (oldFact = id) "
	    "WHERE point1 = '" + point1 + "' AND point2 = '" + point4
	    + "' AND point3 = '" + point2 + "' AND point4 = '" + point4 + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				     querySecondGeoCmdB.size(), &(dbim.stmt2),
				     NULL);
	sqlite3_step(dbim.stmt2);
	if (sqlite3_data_count(dbim.stmt1) == 0
	    && sqlite3_data_count(dbim.stmt2) == 0 ) {
	    correctTransaction = false;
	} else {
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    } else {
		insertionPred = "INSERT INTO "
		    "Perpendicular (typeGeoCmd, point1, point2, point3, point4,"
		    "newFact) "
		    "VALUES "
		    "('perp', '" + point3 + "', '" + point1 + "', '" + point1
		    + "', '" + point4 + "', '" + lstInsRwId + "')";
		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					     insertionPred.size(), &(dbim.stmt),
					     NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		    correctTransaction = false;
	    }
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D58: eqangle(A, B, B, C, P, Q, Q, R) & eqangle(A, C, B, C, P, R, Q, R)
 *               & ~ coll(A, B, C) -> simtri(A, B, C, P, Q, R)
 *
 * Function's argument is eqangle(A, B, B, C, P, Q, Q, R), verifies if
 * ~ perp(A, B, C, D) and, eventually, seraches for
 * eqangle(A, C, B, C, P, R, Q, R).
 */
DBinMemory Prover::ruleD58(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('simtri')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN Collinear "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN Collinear "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point5 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) != 0
	|| sqlite3_data_count(dbim.stmt2) != 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    querySecondGeoCmdA = "SELECT * "
		"FROM NewFact "
		"INNER JOIN EqualAngles "
		"ON (newFact = id) "
		"WHERE point1 = '" + point1 + "' AND point2 = '" + point4
		+ "' AND point3 = '" + point2 + "' AND point4 = '" + point4
		+ "' AND point5 = '" + point5 + "' AND point6 = '" + point8
		+ "' AND point7 = '" + point6 + "' AND point8 = '" + point8
		+ "'";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
					 querySecondGeoCmdA.size(),
					 &(dbim.stmt1), NULL);
	    sqlite3_step(dbim.stmt1);
	    querySecondGeoCmdB = "SELECT * "
		"FROM Facts "
		"INNER JOIN EqualAngles "
		"ON (oldFact = id) "
		"WHERE point1 = '" + point1 + "' AND point2 = '" + point4
		+ "' AND point3 = '" + point2 + "' AND point4 = '" + point4
		+ "' AND point5 = '" + point5 + "' AND point6 = '" + point8
		+ "' AND point7 = '" + point6 + "' AND point8 = '" + point8
		+ "'";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
					 querySecondGeoCmdB.size(),
					 &(dbim.stmt2), NULL);
	    sqlite3_step(dbim.stmt2);
	    if (sqlite3_data_count(dbim.stmt1) == 0
		&& sqlite3_data_count(dbim.stmt2) == 0) {
		correctTransaction = false;
	    } else {
		insertionPred = "INSERT INTO "
		    "SimilarTriangles (typeGeoCmd, point1, point2, point3, "
		    "point4, point5, point6, newFact) "
		    "VALUES "
		    "('simtri', '" + point1 + "', '" + point2 + "', '" + point4
		    + "', '" + point5 + "', '" + point6 + "', '" + point8
		    + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	    }
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D59: simtri(A, B, C, P, Q; R) -> eqratio(A, B, A, C, P, Q, P, R)
 */
DBinMemory Prover::ruleD59(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqratio')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"EqualRatios (typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6, point7, point8, newFact) "
	"VALUES "
	"('eqratio', '" + point1 + "', '" + point2 + "', '" + point1 + "', '"
	+ point3 + "', '" + point4 + "', '" + point5 + "', '" + point4 + "', '"
	+ point6 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D60: simtri(A, B, C, P, Q; R) -> eqangle(A, B, B, C, P, Q, Q, R)
 */
DBinMemory Prover::ruleD60(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqangle')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"EqualAngles (typeGeoCmd, point1, point2, point3, point4, point5, "
	"point6, point7, point8, newFact) "
	"VALUES "
	"('eqangle', '" + point1 + "', '" + point2 + "', '" + point2 + "', '"
	+ point3 + "', '" + point4 + "', '" + point5 + "', '" + point5 + "', '"
	+ point6 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D61: simtri(A, B, C, P, Q, R) & AB = PQ -> contri(A, B, C, P, Q, R)
 *
 * Function's argument is cong(A, B, P, Q) and searches for
 * simtri(A, B, C, P, Q, R).
 */
DBinMemory Prover::ruleD61cong(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3,
			       std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('contri')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point3, point6 "
	"FROM NewFact "
	"INNER JOIN SimilarTriangles "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point4 = '" + point3 + "' AND point5 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point3, point6 "
	"FROM Facts "
	"INNER JOIN SimilarTriangles "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point4 = '" + point3 + "' AND point5 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"CongruentTriangles (typeGeoCmd, point1, point2, point3, "
		"point4, point5, point6, newFact) "
		"VALUES "
		"('contri', '" + point1 + "', '" + point2 + "', '" + newPoint1
		+ "', '" + point3 + "', '" + point4 + "', '" + newPoint2
		+ "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D61: simtri(A, B, C, P, Q, R) & AB = PQ -> contri(A, B, C, P, Q, R)
 *
 * Function's argument is simtri(A, B, C, P, Q, R) and searches for
 * cong(A, B, P, Q).
 */
DBinMemory Prover::ruleD61simtri(DBinMemory dbim, std::string point1,
				 std::string point2, std::string point3,
				 std::string point4, std::string point5,
				 std::string point6) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('contri')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN CongruentSegments "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point4 + "' AND point4 = '" + point5 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN CongruentSegments "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point4 + "' AND point4 = '" + point5 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"CongruentTriangles (typeGeoCmd, point1, point2, point3, "
		"point4, point5, point6, newFact) "
		"VALUES "
		"('contri', '" + point1 + "', '" + point2 + "', '" + point3
		+ "', '" + point4 + "', '" + point4 + "', '" + point6 + "', '"
		+ lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D62: contri(A, B, C, P, Q, R) -> cong(A, B, P, Q)
 */
DBinMemory Prover::ruleD62(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cong')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"CongruentSegments (typeGeoCmd, point1, point2, point3, point4, "
	"newFact) "
	"VALUES "
	"('cong', '" + point1 + "', '" + point2 + "', '" + point4 + "', '"
	+ point5 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D63: midp(M, A, B) & midp(M, C, D) -> para(A, C, B, D)
 */
DBinMemory Prover::ruleD63(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('para')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point2, point3 "
	"FROM NewFact "
	"INNER JOIN Midpoint "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1
	+ "' AND NOT (point2 = '" + point2 + "' AND point3 = '" + point3
	+ "') AND NOT (point2 = '" + point3 + "' AND point3 = '" + point2
	+ "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point2, point3 "
	"FROM Facts "
	"INNER JOIN Midpoint "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1
	+ "' AND NOT (point2 = '" + point2 + "' AND point3 = '" + point3
	+ "') AND NOT (point2 = '" + point3 + "' AND point3 = '" + point2
	+ "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Parallel (typeGeoCmd, point1, point2, point3, point4, "
		"newFact) "
		"VALUES "
		"('para', '" + point2 + "', '" + newPoint1 + "', '" + point3
		+ "', '" + newPoint2 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D64: midp(M, A, B) & para(A, C, B, D) & para(A, D, B, C)
 *               -> midp(M, C, D)
 *
 * Function's argument is midp(M, A, B) and searches for para(A, C, B, D) and
 * para(A, D, B, C).
 */
DBinMemory Prover::ruleD64midp(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('midp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point2, point4 "
	"FROM NewFact "
	"INNER JOIN Parallel "
	"ON (newFact = id) "
	"WHERE point1 = '" + point2 + "' AND point3 = '" + point3
	+ "' AND point2 <> '" + point1 + "' AND point4 <> '" + point1
	+ "' AND point2 <> point4";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point2, point4 "
	"FROM Facts "
	"INNER JOIN Parallel "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point2 + "' AND point3 = '" + point3
	+ "' AND point2 <> '" + point1 + "' AND point4 <> '" + point1
	+ "' AND point2 <> point4";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	querySecondGeoCmdA = "SELECT * "
	    "FROM NewFact "
	    "INNER JOIN Parallel "
	    "ON (newFact = id) "
	    "WHERE point1 = '" + point2 + "' AND point2 = '" + newPoint2
	    + "' AND point3 = '" + point3 + "' AND point4 = '" + newPoint1
	    + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				     querySecondGeoCmdA.size(), &(dbim.stmt1),
				     NULL);
	sqlite3_step(dbim.stmt1);
	querySecondGeoCmdB = "SELECT * "
	    "FROM Facts "
	    "INNER JOIN Parallel "
	    "ON (oldFact = id) "
	    "WHERE point1 = '" + point2 + "' AND point2 = '" + newPoint2
	    + "' AND point3 = '" + point3 + "' AND point4 = '" + newPoint1
	    + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				     querySecondGeoCmdB.size(), &(dbim.stmt2),
				     NULL);
	sqlite3_step(dbim.stmt2);
	if (sqlite3_data_count(dbim.stmt1) == 0
	    && sqlite3_data_count(dbim.stmt2) == 0 ) {
	    correctTransaction = false;
	} else {
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    } else {
		insertionPred = "INSERT INTO "
		    "Midpoint (typeGeoCmd, point1, point2, point3, newFact) "
		    "VALUES "
		    "('midp', '" + point1 + "', '" + newPoint1 + "', '"
		    + newPoint2	+ "', '" + lstInsRwId + "')";
		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					     insertionPred.size(), &(dbim.stmt),
					     NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		    correctTransaction = false;
	    }
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D64: midp(M, A, B) & para(A, C, B, D) & para(A, D, B, C)
 *               -> midp(M, C, D)
 *
 * Function's argument is para(A, C, B, D) and searches for para(A, D, B, C)
 * and midp(M, A, B).
 */
DBinMemory Prover::ruleD64para(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3,
			       std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('midp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN Parallel "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point4
	+ "' AND point3 = '" + point3 + "' AND point4 = '" + point2 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN Parallel "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point4
	+ "' AND point3 = '" + point3 + "' AND point4 = '" + point2 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	querySecondGeoCmdA = "SELECT point1 "
	    "FROM NewFact "
	    "INNER JOIN Midpoint "
	    "ON (newFact = id) "
	    "WHERE point2 = '" + point1 + "' AND point3 = '" + point3
	    + "' AND point1 NOT IN ('" + point1 + "', '" + point2 + "', '"
	    + point3 + "', '" + point4 + "')";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				     querySecondGeoCmdA.size(), &(dbim.stmt1),
				     NULL);
	sqlite3_step(dbim.stmt1);
	querySecondGeoCmdB = "SELECT point1 "
	    "FROM Facts "
	    "INNER JOIN Midpoint "
	    "ON (oldFact = id) "
	    "WHERE point2 = '" + point1 + "' AND point3 = '" + point3
	    + "' AND point1 NOT IN ('" + point1 + "', '" + point2 + "', '"
	    + point3 + "', '" + point4 + "')";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				     querySecondGeoCmdB.size(), &(dbim.stmt2),
				     NULL);
	sqlite3_step(dbim.stmt2);
	if (sqlite3_data_count(dbim.stmt1) == 0
	    && sqlite3_data_count(dbim.stmt2) == 0 ) {
	    correctTransaction = false;
	} else {
	    if (sqlite3_data_count(dbim.stmt1) != 0)
		newPoint = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    else
		newPoint = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    } else {
		insertionPred = "INSERT INTO "
		    "Midpoint (typeGeoCmd, point1, point2, point3, newFact) "
		    "VALUES "
		    "('midp', '" + newPoint + "', '" + point2 + "', '" + point4
		    + "', '" + lstInsRwId + "')";
		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					     insertionPred.size(), &(dbim.stmt),
					     NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		    correctTransaction = false;
	    }
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D65: para(A, B, C, D) & coll(O, A, C) & coll(O, B, D)
 *               -> eqratio(O, A, A, C, O, B, B, D)
 *
 * Function's argument is coll(O, A, C) and searches for coll(O, B, D) and
 * para(A, B, C, D).
 */
DBinMemory Prover::ruleD65coll(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqratio')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point2, point3 "
	"FROM NewFact "
	"INNER JOIN Collinear "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1
	+ "' AND NOT (point2 = '" + point2 + "' AND point3 = '" + point3
	+ "') AND NOT (point2 = '" + point3 + "' AND point3 = '" + point2
	+ "')"
	+ " AND point2 <> point3";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point2, point3 "
	"FROM Facts "
	"INNER JOIN Collinear "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1
	+ "' AND NOT (point2 = '" + point2 + "' AND point3 = '" + point3
	+ "') AND NOT (point2 = '" + point3 + "' AND point3 = '" + point2
	+ "')"
	+ " AND point2 <> point3";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	querySecondGeoCmdA = "SELECT * "
	    "FROM NewFact "
	    "INNER JOIN Parallel "
	    "ON (newFact = id) "
	    "WHERE point1 = '" + point1 + "' AND point2 = '" + newPoint1
	    + "' AND point3 = '" + point3 + "' AND point4 = '" + newPoint2
	    + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				     querySecondGeoCmdA.size(), &(dbim.stmt1),
				     NULL);
	sqlite3_step(dbim.stmt1);
	querySecondGeoCmdB = "SELECT * "
	    "FROM Facts "
	    "INNER JOIN Collinear "
	    "ON (oldFact = id) "
	    "WHERE point1 = '" + point1 + "' AND point2 = '" + newPoint1
	    + "' AND point3 = '" + point3 + "' AND point4 = '" + newPoint2
	    + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				     querySecondGeoCmdB.size(), &(dbim.stmt2),
				     NULL);
	sqlite3_step(dbim.stmt2);
	if (sqlite3_data_count(dbim.stmt1) == 0
	    && sqlite3_data_count(dbim.stmt2) == 0 ) {
	    correctTransaction = false;
	} else {
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    } else {
		insertionPred = "INSERT INTO "
		    "EqualRatios (typeGeoCmd, point1, point2, point3, point4, "
		    "point5, point6, point7, point8, newFact) "
		    "VALUES "
		    "('eqratio', '" + point1 + "', '" + point2 + "', '"
		    + point2 + "', '" + point3 + "', '" + point1 + "', '"
		    + newPoint1 + "', '" + newPoint1 + "', '" + newPoint2
		    + "', '" + lstInsRwId + "')";
		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					     insertionPred.size(), &(dbim.stmt),
					     NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		    correctTransaction = false;
	    }
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D65: para(A, B, C, D) & coll(O, A, C) & coll(O, B, D)
 *               -> eqratio(O, A, A, C, O, B, B, D)
 *
 * Function's argument is para(A, B, C, D) and searches for coll(O, A, C) and
 * coll(O, B, D).
 */
DBinMemory Prover::ruleD65para(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3,
			       std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqratio')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point1 "
	"FROM NewFact "
	"INNER JOIN Collinear "
	"ON (newFact = id) "
	"WHERE point2 = '" + point1 + "' AND point3 = '" + point3
	+ "' AND point1 NOT IN ('" + point1 + "', '" + point2 + "', '"
	+ point3 + "', '" + point4 + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point1 "
	"FROM Facts "
	"INNER JOIN Collinear "
	"ON (oldFact = id) "
	"WHERE point2 = '" + point1 + "' AND point3 = '" + point3
	+ "' AND point1 NOT IN ('" + point1 + "', '" + point2 + "', '"
	+ point3 + "', '" + point4 + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0)
	    newPoint = (char*) sqlite3_column_text(dbim.stmt1, 0);
	else
	    newPoint = (char*) sqlite3_column_text(dbim.stmt2, 0);
	querySecondGeoCmdA = "SELECT * "
	    "FROM NewFact "
	    "INNER JOIN Collinear "
	    "ON (newFact = id) "
	    "WHERE point1 = '" + newPoint + "' AND point2 = '" + point2
	    + "' AND point3 = '" + point4 + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				     querySecondGeoCmdA.size(), &(dbim.stmt1),
				     NULL);
	sqlite3_step(dbim.stmt1);
	querySecondGeoCmdB = "SELECT * "
	    "FROM Facts "
	    "INNER JOIN Collinear "
	    "ON (oldFact = id) "
	    "WHERE point1 = '" + newPoint + "' AND point2 = '" + point2
	    + "' AND point3 = '" + point4 + "'";
	dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				     querySecondGeoCmdB.size(), &(dbim.stmt2),
				     NULL);
	sqlite3_step(dbim.stmt2);
	if (sqlite3_data_count(dbim.stmt1) == 0
	    && sqlite3_data_count(dbim.stmt2) == 0 ) {
	    correctTransaction = false;
	} else {
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    } else {
		insertionPred = "INSERT INTO "
		    "EqualRatios (typeGeoCmd, point1, point2, point3, point4, "
		    "point5, point6, point7, point8, newFact) "
		    "VALUES "
		    "('eqratio', '" + newPoint + "', '" + point1 + "', '"
		    + point1 + "', '" + point3 + "', '" + newPoint + "', '"
		    + point2 + "', '" + point2 + "', '" + point4 + "', '"
		    + lstInsRwId + "')";
		dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					     insertionPred.size(), &(dbim.stmt),
					     NULL);
		if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		    correctTransaction = false;
	    }
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D66: para(A, B, A, C) -> coll(A, B, C)
 */
DBinMemory Prover::ruleD66(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('coll')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"Collinear (typeGeoCmd, point1, point2, point3, newFact) "
	"VALUES "
	"('coll', '" + point1 + "', '" + point2 + "', '" + point4 + "', '"
	+ lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D67: cong(A, B, A, C) & coll(A, B, C) -> midp(A, B, C)
 *
 * Function's argument is coll(A, B, C) and searches for cong(A, B, A, C).
 */
DBinMemory Prover::ruleD67coll(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('midp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN CongruentSegments "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point1 + "' AND point4 = '" + point3 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN CongruentSegments "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point1 + "' AND point4 = '" + point3 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Midpoint (typeGeoCmd, point1, point2, point3, newFact) "
		"VALUES "
		"('midp', '" + point1 + "', '" + point2 + "', '" + point3
		+ "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D67: cong(A, B, A, C) & coll(A, B, C) -> midp(A, B, C)
 *
 * Function's argument is cong(A, B, A, C) and searches for coll(A, B, C).
 */
DBinMemory Prover::ruleD67cong(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3,
			       std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('midp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN Collinear "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN Collinear "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Midpoint (typeGeoCmd, point1, point2, point3, newFact) "
		"VALUES "
		"('midp', '" + point1 + "', '" + point2 + "', '" + point4
		+ "', '" + lstInsRwId + "')";

	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D68: midp(A, B, C) -> cong(A, B, A, C)
 */
DBinMemory Prover::ruleD68(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cong')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"CongruentSegments (typeGeoCmd, point1, point2, point3, point4, "
	"newFact) "
	"VALUES "
	"('cong', '" + point1 + "', '" + point2 + "', '" + point1 + "', '"
	+ point3 + "', '" + lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D69: midp(A, B, C) -> coll(A, B, C)
 */
DBinMemory Prover::ruleD69(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('coll')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    insertionPred = "INSERT INTO "
	"Collinear (typeGeoCmd, point1, point2, point3, newFact) "
	"VALUES "
	"('coll', '" + point1 + "', '" + point2 + "', '" + point3 + "', '"
	+ lstInsRwId + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D70: midp(M, A, B) & midp(N, C, D) -> eqratio(M, A, A, B, N, C, C, D)
 */
DBinMemory Prover::ruleD70(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2, newPoint3;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('eqratio')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point1, point2, point3 "
	"FROM NewFact "
	"INNER JOIN Midpoint "
	"ON (newFact = id) "
	"WHERE NOT (point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point3
	+ "') AND NOT (point1 = '" + point1 + "' AND point2 = '" + point3
	+ "' AND point3 = '" + point2 + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point1, point2, point3 "
	"FROM Facts "
	"INNER JOIN Midpoint "
	"ON (oldFact = id) "
	"WHERE NOT (point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point3
	+ "') AND NOT (point1 = '" + point1 + "' AND point2 = '" + point3
	+ "' AND point3 = '" + point2 + "')";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt2, 2);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"EqualRatios (typeGeoCmd, point1, point2, point3, point4, "
		"point5, point6, point7, point8, newFact) "
		"VALUES "
		"('eqratio', '" + point1 + "', '" + point2 + "', '" + point2
		+ "', '" + point3 + "', '" + newPoint1 + "', '" + newPoint2
		+ "', '" + newPoint2 + "', '" + newPoint3 + "', '" + lstInsRwId
		+ "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D71: eqangle(A, B, C, D, C, D, A, B) & ~ para(A, B, C, D)
 *               -> para(A, B, C, D)
 *
 * Function's argument is eqangle(A, B, C, D, C, D, A, B) and verifies if
 * ~ para(A, B, C, D).
 */
DBinMemory Prover::ruleD71(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('perp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN Parallel "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point3 + "' AND point4 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN Parallel "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point5 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point3 + "' AND point4 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) != 0
	|| sqlite3_data_count(dbim.stmt2) != 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Perpendicular (typeGeoCmd, point1, point2, point3, point4, "
		"newFact) "
		"VALUES "
		"('perp', '" + point1 + "', '" + point2 + "', '" + point3
		+ "', '" + point4 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D72: eqangle(A, B, C, D, C, D, A, B) & ~ perp(A, B, C, D)
 *               -> para(A, B, C, D)
 *
 * Function's argument is eqangle(A, B, C, D, C, D, A, B) and verifies if
 * ~ perp(A, B, C, D).
 */
DBinMemory Prover::ruleD72(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('para')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN Perpendicular "
	"ON (newFact = id) "
	"WHERE point1 = '" + point1 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point3 + "' AND point4 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN Perpendicular "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point5 + "' AND point2 = '" + point2
	+ "' AND point3 = '" + point3 + "' AND point4 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) != 0
	|| sqlite3_data_count(dbim.stmt2) != 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Parallel (typeGeoCmd, point1, point2, point3, point4, "
		"newFact) "
		"VALUES "
		"('para', '" + point1 + "', '" + point2 + "', '" + point3
		+ "', '" + point4 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D73: eqangle(A, B, C, D, P, Q, U, V) & para(P, Q, U, V)
 *               -> para(A, B, C, D)
 *
 * Function's argument is eqangle(A, B, C, D, P, Q, U, V) and searches for
 * para(P, Q, U, V).
 */
DBinMemory Prover::ruleD73eqangle(DBinMemory dbim, std::string point1,
				  std::string point2, std::string point3,
				  std::string point4, std::string point5,
				  std::string point6, std::string point7,
				  std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('para')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN Parallel "
	"ON (newFact = id) "
	"WHERE point1 = '" + point5 + "' AND point2 = '" + point6
	+ "' AND point3 = '" + point7 + "' AND point4 = '" + point8 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN Parallel "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point5 + "' AND point2 = '" + point6
	+ "' AND point3 = '" + point7 + "' AND point4 = '" + point8 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Parallel (typeGeoCmd, point1, point2, point3, point4, "
		"newFact) "
		"VALUES "
		"('para', '" + point1 + "', '" + point2 + "', '" + point3
		+ "', '" + point4 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D73: eqangle(A, B, C, D, P, Q, U, V) & para(P, Q, U, V)
 *               -> para(A, B, C, D)
 *
 * Function's argument is para(P, Q, U, V) and searches for
 * eqangle(A, B, C, D, P, Q, U, V).
 *
 */
DBinMemory Prover::ruleD73para(DBinMemory dbim, std::string point1,
				  std::string point2, std::string point3,
				  std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2, newPoint3, newPoint4;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('para')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point1, point2, point3, point4 "
	"FROM NewFact "
	"INNER JOIN EqualAngles "
	"ON (newFact = id) "
	"WHERE point5 = '" + point1 + "' AND point6 = '" + point2
	+ "' AND point7 = '" + point3 + "' AND point8 = '" + point4
	+ "' AND NOT (point1 = point3 AND point2 = point4) "
	+ " AND NOT (point1 = point4 AND point2 = point3)";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point1, point2, point3, point4 "
	"FROM Facts "
	"INNER JOIN EqualAngles "
	"ON (oldFact = id) "
	"WHERE point5 = '" + point1 + "' AND point6 = '" + point2
	+ "' AND point7 = '" + point3 + "' AND point8 = '" + point4
	+ "' AND NOT (point1 = point3 AND point2 = point4) "
	+ " AND NOT (point1 = point4 AND point2 = point3)";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt1, 3);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt2, 2);
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt2, 3);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Parallel (typeGeoCmd, point1, point2, point3, point4, "
		"newFact) "
		"VALUES "
		"('para', '" + newPoint1 + "', '" + newPoint2 + "', '"
		+ newPoint3 +  "', '" + newPoint4 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D74: eqangle(A, B, C, D, P, Q, U, V) & perp(P, Q, U, V)
 *               -> perp(A, B, C, D)
 *
 * Function's argument is eqangle(A, B, C, D, P, Q, U, V) and searches for
 * perp(P, Q, U, V).
 */
DBinMemory Prover::ruleD74eqangle(DBinMemory dbim, std::string point1,
				  std::string point2, std::string point3,
				  std::string point4, std::string point5,
				  std::string point6, std::string point7,
				  std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('perp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN Perpendicular "
	"ON (newFact = id) "
	"WHERE point1 = '" + point5 + "' AND point2 = '" + point6
	+ "' AND point3 = '" + point7 + "' AND point4 = '" + point8 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN Perpendicular "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point5 + "' AND point2 = '" + point6
	+ "' AND point3 = '" + point7 + "' AND point4 = '" + point8 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Perpendicular (typeGeoCmd, point1, point2, point3, point4, "
		"newFact) "
		"VALUES "
		"('perp', '" + point1 + "', '" + point2 + "', '" + point3
		+ "', '" + point4 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D74: eqangle(A, B, C, D, P, Q, U, V) & perp(P, Q, U, V)
 *               -> perp(A, B, C, D)
 *
 * Function's argument is perp(P, Q, U, V) and searches for
 * eqangle(A, B, C, D, P, Q, U, V).
 * 
 */
DBinMemory Prover::ruleD74perp(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3,
			       std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2, newPoint3, newPoint4;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('perp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point1, point2, point3, point4 "
	"FROM NewFact "
	"INNER JOIN EqualAngles "
	"ON (newFact = id) "
	"WHERE point5 = '" + point1 + "' AND point6 = '" + point2
	+ "' AND point7 = '" + point3 + "' AND point8 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point1, point2, point3, point4 "
	"FROM Facts "
	"INNER JOIN EqualAngles "
	"ON (oldFact = id) "
	"WHERE point5 = '" + point1 + "' AND point6 = '" + point2
	+ "' AND point7 = '" + point3 + "' AND point8 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt1, 3);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt2, 2);
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt2, 3);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"Perpendicular (typeGeoCmd, point1, point2, point3, point4, "
		"newFact) "
		"VALUES "
		"('perp', '" + newPoint1 + "', '" + newPoint2 + "', '"
		+ newPoint3 + "', '" + newPoint4 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D75: eqratio(A, B, C, D, P, Q, U, V) & cong(P, Q, U, V)
 *               -> cong(A, B, C, D)
 *
 * Function's argument is eqratio(A, B, C, D, P, Q, U, V) and searches for
 * cong(P, Q, U, V).
 */
DBinMemory Prover::ruleD75eqratio(DBinMemory dbim, std::string point1,
				  std::string point2, std::string point3,
				  std::string point4, std::string point5,
				  std::string point6, std::string point7,
				  std::string point8) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cong')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT * "
	"FROM NewFact "
	"INNER JOIN CongruentSegments "
	"ON (newFact = id) "
	"WHERE point1 = '" + point5 + "' AND point2 = '" + point6
	+ "' AND point3 = '" + point7 + "' AND point4 = '" + point8 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT * "
	"FROM Facts "
	"INNER JOIN CongruentSegments "
	"ON (oldFact = id) "
	"WHERE point1 = '" + point5 + "' AND point2 = '" + point6
	+ "' AND point3 = '" + point7 + "' AND point4 = '" + point8 +"'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"CongruentSegments (typeGeoCmd, point1, point2, point3, "
		"point4, newFact) "
		"VALUES "
		"('cong', '" + point1 + "', '" + point2 + "', '" + point3
		+ "', '" + point4 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Rule D75: eqratio(A, B, C, D, P, Q, U, V) & cong(P, Q, U, V)
 *               -> cong(A, B, C, D)
 *
 * Function's argument is cong(P, Q, U, V) and searches for
 * eqratio(A, B, C, D, P, Q, U, V).
 * 
 */
DBinMemory Prover::ruleD75cong(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3,
			       std::string point4) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint1, newPoint2, newPoint3, newPoint4;

    insertNewFact = "INSERT INTO NewFact (typeGeoCmd) VALUES ('cong')";
    lastInsertedRowId = "SELECT last_insert_rowid()";
    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
	correctTransaction = false;
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);
    querySecondGeoCmdA = "SELECT point1, point2, point3, point4 "
	"FROM NewFact "
	"INNER JOIN EqualRatios "
	"ON (newFact = id) "
	"WHERE point5 = '" + point1 + "' AND point6 = '" + point2
	+ "' AND point7 = '" + point3 + "' AND point8 = '" + point4 + "'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    querySecondGeoCmdB = "SELECT point1, point2, point3, point4 "
	"FROM Facts "
	"INNER JOIN EqualRatios "
	"ON (oldFact = id) "
	"WHERE point5 = '" + point1 + "' AND point6 = '" + point2
	+ "' AND point7 = '" + point3 + "' AND point8 = '" + point4 +"'";
    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt1, 3);
	} else {
	    newPoint1 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt2, 2);
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt2, 3);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO "
		"CongruentSegments (typeGeoCmd, point1, point2, point3, "
		"point4, newFact) "
		"VALUES "
		"('cong', '" + newPoint1 + "', '" + newPoint2 + "', '"
		+ newPoint3 + "', '" + newPoint4 + "', '" + lstInsRwId + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE)
		correctTransaction = false;
	}
    }
    if (correctTransaction)
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    else
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    return dbim;
}

/*
 * Calcute de fixed point.
 */
DBinMemory Prover::fixedPoint(DBinMemory dbim) {
    int nrColl, nrPara, nrPerp, nrMidp, nrCircle, nrCong;
    int nrContri, nrCyclic, nrEqangle, nrEqratio, nrSimtri;
    bool correctTransaction;
    std::string deleteNewFact, insertFact, newFact, newFactId;
    std::string toDeriveGeoCmd, typeGeoCmd, updateGeoCmd;
    std::string point1, point2, point3, point4, point5, point6, point7, point8;
    FOFtoDB fdb;

    // DEBUG START
    std::cout << std::endl << "fixedPoint() : Entering.." << std::endl;
    // DEBUG STOP
    nrColl = 0;
    nrPara = 0;
    nrPerp = 0;
    nrMidp = 0;
    nrCircle = 0;
    nrCong = 0;
    nrContri = 0;
    nrCyclic = 0;
    nrEqangle = 0;
    nrEqratio = 0;
    nrSimtri = 0;
    // Set foreign keys ON
    sqlite3_prepare_v2(dbim.db, "PRAGMA foreign_keys = ON", -1, &(dbim.stmt),
		       NULL);
    sqlite3_step(dbim.stmt);  
    newFact = "SELECT id, typeGeoCmd FROM NewFact LIMIT 1";
    dbim.rc = sqlite3_prepare_v2(dbim.db, newFact.c_str() , newFact.size(),
				 &(dbim.stmt), NULL);
    // While this search is successfull do...
    // rc = sqlite3_step(stmt);
    while (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	newFactId = (char*) sqlite3_column_text(dbim.stmt, 0);
	typeGeoCmd = (char*) sqlite3_column_text(dbim.stmt, 1);
	switch (dbim.geoCmds[typeGeoCmd]) {
	case 1:
            // Collinear
	    nrColl++;
	    // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3 "
		"FROM Collinear "
		"WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(),
					 toDeriveGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    point1 = (char*) sqlite3_column_text(dbim.stmt, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt, 2);
	    // Insert a new fact in table 'Facts'
	    insertFact = "INSERT INTO Facts (id, typeGeoCmd) VALUES ("
		+ newFactId + ",'" + typeGeoCmd + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(),
					 insertFact.size(), &(dbim.stmt), NULL);
	    sqlite3_step(dbim.stmt);
	    // Update table 'Collinear' with the 'Facts' foreign key
	    updateGeoCmd = "UPDATE Collinear SET oldFact = " + newFactId
		+ " WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(),
					 updateGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // Delete that line from table 'NewFacts'
	    deleteNewFact = "DELETE FROM NewFact where id = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(),
					 deleteNewFact.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // DEBUG START
	    std::cout << "  Collinear : " << nrColl << " / ";
	    std::cout << "coll(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ")" << std::endl;
	    // DEBUG STOP
	    break;
	case 2:
            // Parallel
	    nrPara++;
	    // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3, point4 "
		"FROM Parallel "
		"WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(),
					 toDeriveGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    point1 = (char*) sqlite3_column_text(dbim.stmt, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt, 2);
	    point4 = (char*) sqlite3_column_text(dbim.stmt, 3);
	    // Insert a new fact in table 'Facts'
	    insertFact = "INSERT INTO Facts (id, typeGeoCmd) VALUES ("
		+ newFactId + ",'" + typeGeoCmd + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(),
					 insertFact.size(), &(dbim.stmt), NULL);
	    sqlite3_step(dbim.stmt);
	    // Update table 'Parallel' with the 'Facts' foreign key
	    updateGeoCmd = "UPDATE Parallel SET oldFact = " + newFactId
		+ " WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(),
					 updateGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // Delete that line of table 'NewFacts'
	    deleteNewFact = "DELETE FROM NewFact where id = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(),
					 deleteNewFact.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // DEBUG START
	    std::cout << "  Parallel : " << nrPara << " / ";
	    std::cout << "para(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ")" << std::endl;
	    // DEBUG STOP
	    break;
	case 3:
            // Perpendicular
	    nrPerp++;
            // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3, point4 "
		"FROM Perpendicular "
		"WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(),
					 toDeriveGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    point1 = (char*) sqlite3_column_text(dbim.stmt, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt, 2);
	    point4 = (char*) sqlite3_column_text(dbim.stmt, 3);
	    // Insert a new fact in table Facts
	    insertFact = "INSERT INTO Facts (id, typeGeoCmd) VALUES ("
		+ newFactId + ",'" + typeGeoCmd + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(),
					 insertFact.size(), &(dbim.stmt), NULL);
	    sqlite3_step(dbim.stmt);
	    // Update table 'Perpendicular' with the 'Facts' foreign key
	    updateGeoCmd = "UPDATE Perpendicular SET oldFact = " + newFactId
		+ " WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(),
					 updateGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // Delete that line of table 'NewFacts'
	    deleteNewFact = "DELETE FROM NewFact where id = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(),
					 deleteNewFact.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // DEBUG START
	    std::cout << "  Perpendicular : " << nrPerp << " / ";
	    std::cout << "perp(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ")" << std::endl;
	    // DEBUG STOP
	    break;
	case 4:
            // Midpoint
	    nrMidp ++;
	    // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3 "
		"FROM Midpoint "
		"WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(),
					 toDeriveGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    point1 = (char*) sqlite3_column_text(dbim.stmt, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt, 2);
	    // Insert a new fact in table 'Facts'
	    insertFact = "INSERT INTO Facts (id, typeGeoCmd) VALUES ("
		+ newFactId + ",'" + typeGeoCmd + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(),
					 insertFact.size(), &(dbim.stmt), NULL);
	    sqlite3_step(dbim.stmt);
	    // Update table 'Midpoint' with the 'Facts' foreign key
	    updateGeoCmd = "UPDATE Midpoint SET oldFact = " + newFactId
		+ " WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(),
					 updateGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // Delete that line of table 'NewFacts'
	    deleteNewFact = "DELETE FROM NewFact where id = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(),
					 deleteNewFact.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // DEBUG START
	    std::cout << "  Midpoint : " << nrMidp << " / ";
	    std::cout << "midp(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ")" << std::endl;
	    // DEBUG STOP
	    break;
	case 5:
	    // Circle
	    nrCircle++;
            // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3, point4 "
		"FROM Circle "
		"WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(),
					 toDeriveGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    point1 = (char*) sqlite3_column_text(dbim.stmt, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt, 2);
	    point4 = (char*) sqlite3_column_text(dbim.stmt, 3);
	    // Insert a new fact in table Facts
	    insertFact = "INSERT INTO Facts (id, typeGeoCmd) VALUES ("
		+ newFactId + ",'" + typeGeoCmd + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(),
					 insertFact.size(), &(dbim.stmt), NULL);
	    sqlite3_step(dbim.stmt);
	    // Update table 'Circle' with the 'Facts' foreign key
	    updateGeoCmd = "UPDATE Circle SET oldFact = " + newFactId
		+ " WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(),
					 updateGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // Delete that line of table 'NewFacts'
	    deleteNewFact = "DELETE FROM NewFact where id = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(),
					 deleteNewFact.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // DEBUG START
	    std::cout << "  Circle : " << nrCircle << " / ";
	    std::cout << "circle(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ")" << std::endl;
	    // DEBUG STOP
	    break;
	case 6:
	    // Congruent Segments
	    nrCong++;
	    // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3, point4 "
		"FROM CongruentSegments "
		"WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(),
					 toDeriveGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    point1 = (char*) sqlite3_column_text(dbim.stmt, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt, 2);
	    point4 = (char*) sqlite3_column_text(dbim.stmt, 3);
	    // Insert a new fact in table 'Facts'
	    insertFact = "INSERT INTO Facts (id, typeGeoCmd) VALUES ("
		+ newFactId + ",'" + typeGeoCmd + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(),
					 insertFact.size(), &(dbim.stmt), NULL);
	    sqlite3_step(dbim.stmt);
	    // Update table 'CongruentSegments' with the 'Facts' foreign key
	    updateGeoCmd = "UPDATE CongruentSegments SET oldFact = " + newFactId
		+ " WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(),
					 updateGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // Delete that line of table 'NewFacts'
	    deleteNewFact = "DELETE FROM NewFact where id = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(),
					 deleteNewFact.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // DEBUG START
	    std::cout << "  Congruent Segments : " << nrCong << " / ";
	    std::cout << "cong(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ")" << std::endl;
	    // DEBUG STOP
	    break;
	case 7:
	    // Congruent Triangles
	    nrContri++;
	    // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3, "
		"point4, point5, point6 "
		"FROM CongruentTriangles "
		"WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(),
					 toDeriveGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    point1 = (char*) sqlite3_column_text(dbim.stmt, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt, 2);
	    point4 = (char*) sqlite3_column_text(dbim.stmt, 3);
	    point5 = (char*) sqlite3_column_text(dbim.stmt, 4);
	    point6 = (char*) sqlite3_column_text(dbim.stmt, 5);
	    // Insert a new fact in table 'Facts'
	    insertFact = "INSERT INTO Facts (id, typeGeoCmd) VALUES ("
		+ newFactId + ",'" + typeGeoCmd + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(),
					 insertFact.size(), &(dbim.stmt), NULL);
	    sqlite3_step(dbim.stmt);
	    // Update table 'CongruentTriangles' with the 'Facts' foreign key
	    updateGeoCmd = "UPDATE CongruentTriangles SET oldFact = "
		+ newFactId + " WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(),
					 updateGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // Delete that line of table 'NewFacts'
	    deleteNewFact = "DELETE FROM NewFact where id = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(),
					 deleteNewFact.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // DEBUG START
	    std::cout << "  Congruent Triangles : " << nrContri << " / ";
	    std::cout << "contri(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ", ";
	    std::cout << point5 << ", ";
	    std::cout << point6 << ")" << std::endl;
	    // DEBUG STOP
	    break;
	case 8:
	    // Cyclic
	    nrCyclic++;
	    // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3, point4 "
		"FROM Cyclic "
		"WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(),
					 toDeriveGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    point1 = (char*) sqlite3_column_text(dbim.stmt, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt, 2);
	    point4 = (char*) sqlite3_column_text(dbim.stmt, 3);
	    // Insert a new fact in table 'Facts'
	    insertFact = "INSERT INTO Facts (id, typeGeoCmd) VALUES ("
		+ newFactId + ",'" + typeGeoCmd + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(),
					 insertFact.size(), &(dbim.stmt), NULL);
	    sqlite3_step(dbim.stmt);
	    // Update table 'Cyclic' with the 'Facts' foreign key
	    updateGeoCmd = "UPDATE Cyclic SET oldFact = " + newFactId
		+ " WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(),
					 updateGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // Delete that line of table 'NewFacts'
	    deleteNewFact = "DELETE FROM NewFact where id = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(),
					 deleteNewFact.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // DEBUG START
	    std::cout << "  Cyclic : " << nrCyclic << " / ";
	    std::cout << "cyclic(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ")" << std::endl;
	    // DEBUG STOP
	    break;
	case 9:
	    // Equal Angles
	    nrEqangle++;
	    // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3, point4, "
		"point5, point6, point7, point8 "
		"FROM EqualAngles "
		"WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(),
					 toDeriveGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    point1 = (char*) sqlite3_column_text(dbim.stmt, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt, 2);
	    point4 = (char*) sqlite3_column_text(dbim.stmt, 3);
	    point5 = (char*) sqlite3_column_text(dbim.stmt, 4);
	    point6 = (char*) sqlite3_column_text(dbim.stmt, 5);
	    point7 = (char*) sqlite3_column_text(dbim.stmt, 6);
	    point8 = (char*) sqlite3_column_text(dbim.stmt, 7);
	    // Insert a new fact in table 'Facts'
	    insertFact = "INSERT INTO Facts (id, typeGeoCmd) VALUES ("
		+ newFactId + ",'" + typeGeoCmd + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(),
					 insertFact.size(), &(dbim.stmt), NULL);
	    sqlite3_step(dbim.stmt);
	    // Update table 'EqualAngles' with the 'Facts' foreign key
	    updateGeoCmd = "UPDATE EqualAngles SET oldFact = "
		+ newFactId + " WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(),
					 updateGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // Delete that line of table 'NewFacts'
	    deleteNewFact = "DELETE FROM NewFact where id = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(),
					 deleteNewFact.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // DEBUG START
	    std::cout << "  Equal Angles : " << nrEqangle << " / ";
	    std::cout << "eqangle(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ", ";
	    std::cout << point5 << ", ";
	    std::cout << point6 << ", ";
	    std::cout << point7 << ", ";
	    std::cout << point8 << ")" << std::endl;
	    // DEBUG STOP
	    break;
	case 10:
	    // Equal Ratios
	    nrEqratio++;
	    // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3, point4, "
		"point5, point6, point7, point8 "
		"FROM EqualRatios "
		"WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(),
					 toDeriveGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    point1 = (char*) sqlite3_column_text(dbim.stmt, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt, 2);
	    point4 = (char*) sqlite3_column_text(dbim.stmt, 3);
	    point5 = (char*) sqlite3_column_text(dbim.stmt, 4);
	    point6 = (char*) sqlite3_column_text(dbim.stmt, 5);
	    point7 = (char*) sqlite3_column_text(dbim.stmt, 6);
	    point8 = (char*) sqlite3_column_text(dbim.stmt, 7);
	    // Insert a new fact in table 'Facts'
	    insertFact = "INSERT INTO Facts (id, typeGeoCmd) VALUES ("
		+ newFactId + ",'" + typeGeoCmd + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(),
					 insertFact.size(), &(dbim.stmt), NULL);
	    sqlite3_step(dbim.stmt);
	    // Update table 'EqualRatios' with the 'Facts' foreign key
	    updateGeoCmd = "UPDATE EqualRatios SET oldFact = "
		+ newFactId + " WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(),
					 updateGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // Delete that line of table 'NewFacts'
	    deleteNewFact = "DELETE FROM NewFact where id = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(),
					 deleteNewFact.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // DEBUG START
	    std::cout << "  Equal Ratios : " << nrEqratio << " / ";
	    std::cout << "eqratio(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ", ";
	    std::cout << point5 << ", ";
	    std::cout << point6 << ", ";
	    std::cout << point7 << ", ";
	    std::cout << point8 << ")" << std::endl;
	    // DEBUG STOP
	    break;
	case 11:
	    // Similar Triangles
	    nrSimtri++;
	    // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3, "
		"point4, point5, point6 "
		"FROM SimilarTriangles "
		"WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(),
					 toDeriveGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    point1 = (char*) sqlite3_column_text(dbim.stmt, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt, 2);
	    point4 = (char*) sqlite3_column_text(dbim.stmt, 3);
	    point5 = (char*) sqlite3_column_text(dbim.stmt, 4);
	    point6 = (char*) sqlite3_column_text(dbim.stmt, 5);
	    // Insert a new fact in table 'Facts'
	    insertFact = "INSERT INTO Facts (id, typeGeoCmd) VALUES ("
		+ newFactId + ",'" + typeGeoCmd + "')";
	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(),
					 insertFact.size(), &(dbim.stmt), NULL);
	    sqlite3_step(dbim.stmt);
	    // Update table 'SimilarTriangles' with the 'Facts' foreign key
	    updateGeoCmd = "UPDATE SimilarTriangles SET oldFact = "
		+ newFactId + " WHERE newFact = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(),
					 updateGeoCmd.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // Delete that line of table 'NewFacts'
	    deleteNewFact = "DELETE FROM NewFact where id = " + newFactId;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(),
					 deleteNewFact.size(), &(dbim.stmt),
					 NULL);
	    sqlite3_step(dbim.stmt);
	    // DEBUG START
	    std::cout << "  Similar Triangles : " << nrSimtri << " / ";
	    std::cout << "simtri(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ", ";
	    std::cout << point5 << ", ";
	    std::cout << point6 << ")" << std::endl;
	    // DEBUG STOP
	    break;
	default:
	    // ERROR : Necessary?  I don't _think_ so...
	    std::cerr << "Unknown geometric command!!!" << std::endl;
	    exit(1);
	}
	/*
	 * Derive new facts 
	 * --from the 'newFact' that has been selected, try to prove a new fact 
	 */
	switch (dbim.geoCmds[typeGeoCmd]) {
	case 1:
            // Collinear
	    dbim = ruleD01(dbim, point1, point2, point3);
	    dbim = ruleD02(dbim, point1, point2, point3);
	    dbim = ruleD03(dbim, point1, point2, point3);
	    dbim = ruleD45coll(dbim, point1, point2, point3);
	    dbim = ruleD51coll(dbim, point1, point2, point3);
	    dbim = ruleD53coll(dbim, point1, point2, point3);
	    // dbim = ruleD65coll(dbim, point1, point2, point3);
	    // dbim = ruleD67coll(dbim, point1, point2, point3);
	    break;
	case 2:
            // Parallel
	    dbim = ruleD04(dbim, point1, point2, point3, point4);
	    dbim = ruleD05(dbim, point1, point2, point3, point4);
	    dbim = ruleD06(dbim, point1, point2, point3, point4);
	    dbim = ruleD10para(dbim, point1, point2, point3, point4);
	    dbim = ruleD40(dbim, point1, point2, point3, point4);
	    dbim = ruleD45para(dbim, point1, point2, point3, point4);
	    dbim = ruleD54para(dbim, point1, point2, point3, point4);
	    // if (point3 != point4)
	    // 	dbim = ruleD64para(dbim, point1, point2, point3, point4);
	    // dbim = ruleD65para(dbim, point1, point2, point3, point4);
	    // // if (point1 != point3 && point2 != point4)
	    // // 	dbim = ruleD65para(dbim, point1, point2, point3, point4);
	    // if (point1 == point3)
	    // 	dbim = ruleD66(dbim, point1, point2, point3, point4);
	    // dbim = ruleD73para(dbim, point1, point2, point3, point4);
	    break;
	case 3:
            // Perpendicular
	    dbim = ruleD07(dbim, point1, point2, point3, point4);
	    dbim = ruleD08(dbim, point1, point2, point3, point4);
	    dbim = ruleD09(dbim, point1, point2, point3, point4);
	    dbim = ruleD10perp(dbim, point1, point2, point3, point4);
	    if (point2 == point3)
		dbim = ruleD48perp(dbim, point1, point2, point3, point4);
	    if (point2 == point3)
		dbim = ruleD52perp(dbim, point1, point2, point3, point4);
	    dbim = ruleD55perp(dbim, point1, point2, point3, point4);
	    // dbim = ruleD74perp(dbim, point1, point2, point3, point4);
	    break;
	case 4:
            // Midpoint
	    dbim = ruleD11(dbim, point1, point2, point3);
	    dbim = ruleD44(dbim, point1, point2, point3);
	    dbim = ruleD45midp(dbim, point1, point2, point3);
	    dbim = ruleD50midp(dbim, point1, point2, point3);
	    dbim = ruleD52midp(dbim, point1, point2, point3);
	    dbim = ruleD55midp(dbim, point1, point2, point3);
	    // dbim = ruleD63(dbim, point1, point2, point3);
	    // dbim = ruleD64midp(dbim, point1, point2, point3);
	    // dbim = ruleD68(dbim, point1, point2, point3);
	    // dbim = ruleD69(dbim, point1, point2, point3);
	    // dbim = ruleD70(dbim, point1, point2, point3);
	    break;
	case 5:
	    // Circle
	    dbim = ruleD48circle(dbim, point1, point2, point3, point4);
	    dbim = ruleD49circle(dbim, point1, point2, point3, point4);
	    dbim = ruleD50circle(dbim, point1, point2, point3, point4);
	    dbim = ruleD51circle(dbim, point1, point2, point3, point4);
	    dbim = ruleD53circle(dbim, point1, point2, point3, point4);
	    break;
	case 6:
	    // Congruent Segments
	    if (point1 == point3)
		dbim = ruleD12(dbim, point1, point2, point3, point4);
	    if (point1 == point3)
		dbim = ruleD13(dbim, point1, point2, point3, point4);
	    dbim = ruleD23(dbim, point1, point2, point3, point4);
	    dbim = ruleD24(dbim, point1, point2, point3, point4);
	    dbim = ruleD25(dbim, point1, point2, point3, point4);
	    if (point1 == point3)
		dbim = ruleD46(dbim, point1, point2, point3, point4);
	    if (point2 == point4)
		dbim = ruleD56(dbim, point1, point2, point3, point4);
	    if (point2 == point4)
		dbim = ruleD57cong(dbim, point1, point2, point3, point4);
	    dbim = ruleD61cong(dbim, point1, point2, point3, point4);
	    // if (point1 == point3)
	    //  dbim = ruleD67cong(dbim, point1, point2, point3, point4);
	    // dbim = ruleD75cong(dbim, point1, point2, point3, point4);
	    break;
	case 7:
	    // Congruent Triangles
	    dbim = ruleD35(dbim, point1, point2, point3,
			   point4, point5, point6);
	    dbim = ruleD36(dbim, point1, point2, point3,
			   point4, point5, point6);
	    dbim = ruleD37(dbim, point1, point2, point3,
			   point4, point5, point6);
	    dbim = ruleD38(dbim, point1, point2, point3,
			   point4, point5, point6);
	    dbim = ruleD62(dbim, point1, point2, point3,
			   point4, point5, point6);
	    break;
	case 8:
	    // Cyclic
	    dbim = ruleD14(dbim, point1, point2, point3, point4);
	    dbim = ruleD15(dbim, point1, point2, point3, point4);
	    dbim = ruleD16(dbim, point1, point2, point3, point4);
	    dbim = ruleD17(dbim, point1, point2, point3, point4);
	    dbim = ruleD41(dbim, point1, point2, point3, point4);
	    dbim = ruleD43cyclic(dbim, point1, point2, point3, point4);
	    dbim = ruleD54cyclic(dbim, point1, point2, point3, point4);
	    dbim = ruleD57cyclic(dbim, point1, point2, point3, point4);
	    break;
	case 9:
	    // Equal Angles
	    dbim = ruleD18(dbim, point1, point2, point3, point4,
			   point5, point6, point7, point8);
	    dbim = ruleD19(dbim, point1, point2, point3, point4,
			   point5, point6, point7, point8);
	    dbim = ruleD20(dbim, point1, point2, point3, point4,
			   point5, point6, point7, point8);
	    dbim = ruleD21(dbim, point1, point2, point3, point4,
			   point5, point6, point7, point8);
	    dbim = ruleD22(dbim, point1, point2, point3, point4,
			   point5, point6, point7, point8);
	    if (point3 == point7 && point4 == point8
		&& !(point1 == point5 && point2 == point6)
		&& !(point1 == point6 && point2 == point5))
		dbim = ruleD39(dbim, point1, point2, point3, point4,
			       point5, point6, point7, point8);
	    if (point1 == point3 && point2 == point6 && point4 == point8
		&& point5 == point7
		&& point1 != point5 && point2 != point4)
		dbim = ruleD42(dbim, point1, point2, point3, point4,
			       point5, point6, point7, point8);
	    if (point1 == point3 && point5 == point7
		&& point2 != point4 && point6 != point8
		&& !(point2 == point6 && point4 == point8)
		&& !(point2 == point8 && point4 == point6))
		dbim = ruleD43eqangle(dbim, point1, point2, point3, point4,
				      point5, point6, point7, point8);
	    if (point1 == point7 && point2 == point3 && point2 == point5
		&& point4 == point6 && point4 == point8)
		dbim = ruleD47(dbim, point1, point2, point3, point4,
			       point5, point6, point7, point8);
	    if (point1 == point3 && point1 == point6 && point4 == point8
		&& point5 == point7)
		dbim = ruleD49eqangle(dbim, point1, point2, point3, point4,
				      point5, point6, point7, point8);
	    if (point1 == point3 && point2 == point6 && point5 == point7
		&& point2 != point4 && point2 != point8)
		dbim = ruleD51eqangle(dbim, point1, point2, point3, point4,
				      point5, point6, point7, point8);
	    // if (point2 == point3 && point6 == point7)
	    // 	dbim = ruleD58(dbim, point1, point2, point3, point4,
	    // 		       point5, point6, point7, point8);
	    // if (point1 == point7 && point2 == point8 && point3 == point5
	    // 	&& point4 == point6)
	    // 	dbim = ruleD71(dbim, point1, point2, point3, point4,
	    // 		       point5, point6, point7, point8);
	    // if (point1 == point7 && point2 == point8 && point3 == point5
	    // 	&& point4 == point6)
	    // 	dbim = ruleD72(dbim, point1, point2, point3, point4,
	    // 		       point5, point6, point7, point8);
	    // // if (!(point1 == point3 && point2 == point4)
	    // // 	&& !(point1 == point4 && point2 == point3))
	    // // 	dbim = ruleD73eqangle(dbim, point1, point2, point3, point4,
	    // // 			      point5, point6, point7, point8);
	    // dbim = ruleD73eqangle(dbim, point1, point2, point3, point4,
	    // 			  point5, point6, point7, point8);
	    // dbim = ruleD74eqangle(dbim, point1, point2, point3, point4,
	    // 			  point5, point6, point7, point8);
	    break;
	case 10:
	    // Equal Ratios
	    dbim = ruleD26(dbim, point1, point2, point3, point4,
			   point5, point6, point7, point8);
	    dbim = ruleD27(dbim, point1, point2, point3, point4,
			   point5, point6, point7, point8);
	    dbim = ruleD28(dbim, point1, point2, point3, point4,
			   point5, point6, point7, point8);
	    dbim = ruleD29(dbim, point1, point2, point3, point4,
			   point5, point6, point7, point8);
	    dbim = ruleD30(dbim, point1, point2, point3, point4,
			   point5, point6, point7, point8);
	    // // if (!(point1 == point3 && point2 == point4)
	    // // 	&& !(point1 == point4 && point2 == point3))
	    // // 	dbim = ruleD75eqratio(dbim, point1, point2, point3, point4,
	    // // 			      point5, point6, point7, point8);
	    // dbim = ruleD75eqratio(dbim, point1, point2, point3, point4,
	    // 			  point5, point6, point7, point8);
	    break;
	case 11:
	    // Similar Triangles
	    dbim = ruleD31(dbim, point1, point2, point3,
			   point4, point5, point6);
	    dbim = ruleD32(dbim, point1, point2, point3,
			   point4, point5, point6);
	    dbim = ruleD33(dbim, point1, point2, point3,
			   point4, point5, point6);
	    dbim = ruleD34(dbim, point1, point2, point3,
			   point4, point5, point6);
	    dbim = ruleD59(dbim, point1, point2, point3,
			   point4, point5, point6);
	    dbim = ruleD60(dbim, point1, point2, point3,
			   point4, point5, point6);
	    dbim = ruleD61simtri(dbim, point1, point2, point3,
				 point4, point5, point6);
	    break;
	default:
            // ERROR : Necessary?  I don't _thin_ so...
	    std::cerr << "Unknown geometric command!!!" << std::endl;
	    exit(1);
	}
	// new query to restart the cycle
	newFact = "SELECT id, typeGeoCmd FROM NewFact LIMIT 1";
	dbim.rc = sqlite3_prepare_v2(dbim.db, newFact.c_str(), newFact.size(),
				     &(dbim.stmt), NULL);
    }
    // DEBUG START
    // fdb.showDB(dbim);
    std::cout << "fixedPoint() : Leaving..." << std::endl << std::endl;
    // DEBUG STOP
    return dbim;
}

/*
 * Checks if conjecture has been proved.
 * Assumes that conjecture has a single consequent.
 */
bool Prover::proved(DBinMemory dbim) {
    std::string idConsequent;
    std::string proving;
    std::string sqlConsequent;
    std::string typeGeoCmd;

    sqlConsequent = "SELECT id, typeGeoCmd FROM Consequent";
    dbim.rc = sqlite3_prepare_v2(dbim.db, sqlConsequent.c_str(),
				 sqlConsequent.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    idConsequent = (char*) sqlite3_column_text(dbim.stmt, 0);
    typeGeoCmd = (char*) sqlite3_column_text(dbim.stmt, 1);
    switch (dbim.geoCmds[typeGeoCmd]) {
    case 1:
        // Collinear
	proving = "SELECT * "
	    "FROM Consequent "
	    "INNER JOIN Collinear "
	    "USING (typeGeoCmd) "
	    "WHERE Consequent.point1 = Collinear.point1 "
	    "AND Consequent.point2 = Collinear.point2 "
	    "AND Consequent.point3 = Collinear.point3";
	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0)
	    return false;
	else
	    return true;
	break;
    case 2:
        // Parallel
	proving = "SELECT * "
	    "FROM Consequent "
	    "INNER JOIN Parallel "
	    "USING (typeGeoCmd) "
	    "WHERE Consequent.point1 = Parallel.point1 "
	    "AND Consequent.point2 = Parallel.point2 "
	    "AND Consequent.point3 = Parallel.point3 "
	    "AND Consequent.point4 = Parallel.point4";
	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0)
	    return false;
	else
	    return true;
	break;
    case 3:
        // Perpendicular
	proving = "SELECT * "
	    "FROM Consequent "
	    "INNER JOIN Perpendicular "
	    "USING (typeGeoCmd) "
	    "WHERE Consequent.point1 = Perpendicular.point1 "
	    "AND Consequent.point2 = Perpendicular.point2 "
	    "AND Consequent.point3 = Perpendicular.point3 "
	    "AND Consequent.point4 = Perpendicular.point4";
	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0)
	    return false;
	else
	    return true;
	break;
    case 4:
	// Midpoint
	proving = "SELECT * "
	    "FROM Consequent "
	    "INNER JOIN Midpoint "
	    "USING (typeGeoCmd) "
	    "WHERE Consequent.point1 = Midpoint.point1 "
	    "AND Consequent.point2 = Midpoint.point2 "
	    "AND Consequent.point3 = Midpoint.point3";
	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0)
	    return false;
	else
	    return true;
	break;
    case 5:
	// Circle
	proving = "SELECT * "
	    "FROM Consequent "
	    "INNER JOIN Circle "
	    "USING (typeGeoCmd) "
	    "WHERE Consequent.point1 = Circle.point1 "
	    "AND Consequent.point2 = Circle.point2 "
	    "AND Consequent.point3 = Circle.point3 "
	    "AND Consequent.point4 = Circle.point4";
	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0)
	    return false;
	else
	    return true;
	break;
    case 6:
	// Congruent Segments
	proving = "SELECT * "
	    "FROM Consequent "
	    "INNER JOIN CongruentSegments "
	    "USING (typeGeoCmd) "
	    "WHERE Consequent.point1 = CongruentSegments.point1 "
	    "AND Consequent.point2 = CongruentSegments.point2 "
	    "AND Consequent.point3 = CongruentSegments.point3 "
	    "AND Consequent.point4 = CongruentSegments.point4";
	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0)
	    return false;
	else
	    return true;
	break;
    case 7:
	// Congruent Triangles
	proving = "SELECT * "
	    "FROM Consequent "
	    "INNER JOIN CongruentTriangles "
	    "USING (typeGeoCmd) "
	    "WHERE Consequent.point1 = CongruentTriangles.point1 "
	    "AND Consequent.point2 = CongruentTriangles.point2 "
	    "AND Consequent.point3 = CongruentTriangles.point3 "
	    "AND Consequent.point4 = CongruentTriangles.point4 "
	    "AND Consequent.point5 = CongruentTriangles.point5 "
	    "AND Consequent.point6 = CongruentTriangles.point6";
	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0)
	    return false;
	else
	    return true;
	break;
    case 8:
	// Cyclic
	proving = "SELECT * "
	    "FROM Consequent "
	    "INNER JOIN Cyclic "
	    "USING (typeGeoCmd) "
	    "WHERE Consequent.point1 = Cyclic.point1 "
	    "AND Consequent.point2 = Cyclic.point2 "
	    "AND Consequent.point3 = Cyclic.point3 "
	    "AND Consequent.point4 = Cyclic.point4";
	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0)
	    return false;
	else
	    return true;
	break;
    case 9:
	// Equal Angles
	proving = "SELECT * "
	    "FROM Consequent "
	    "INNER JOIN EqualAngles "
	    "USING (typeGeoCmd) "
	    "WHERE Consequent.point1 = EqualAngles.point1 "
	    "AND Consequent.point2 = EqualAngles.point2 "
	    "AND Consequent.point3 = EqualAngles.point3 "
	    "AND Consequent.point4 = EqualAngles.point4 "
	    "AND Consequent.point5 = EqualAngles.point5 "
	    "AND Consequent.point6 = EqualAngles.point6 "
	    "AND Consequent.point7 = EqualAngles.point7 "
	    "AND Consequent.point8 = EqualAngles.point8";
	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0)
	    return false;
	else
	    return true;
	break;
    case 10:
	// Equal Ratios
	proving = "SELECT * "
	    "FROM Consequent "
	    "INNER JOIN EqualRatios "
	    "USING (typeGeoCmd) "
	    "WHERE Consequent.point1 = EqualRatios.point1 "
	    "AND Consequent.point2 = EqualRatios.point2 "
	    "AND Consequent.point3 = EqualRatios.point3 "
	    "AND Consequent.point4 = EqualRatios.point4 "
	    "AND Consequent.point5 = EqualRatios.point5 "
	    "AND Consequent.point6 = EqualRatios.point6 "
	    "AND Consequent.point7 = EqualRatios.point7 "
	    "AND Consequent.point8 = EqualRatios.point8";
	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0)
	    return false;
	else
	    return true;
	break;
    case 11:
	// Similar Triangles
	proving = "SELECT * "
	    "FROM Consequent "
	    "INNER JOIN SimilarTriangles "
	    "USING (typeGeoCmd) "
	    "WHERE Consequent.point1 = SimilarTriangles.point1 "
	    "AND Consequent.point2 = SimilarTriangles.point2 "
	    "AND Consequent.point3 = SimilarTriangles.point3 "
	    "AND Consequent.point4 = SimilarTriangles.point4 "
	    "AND Consequent.point5 = SimilarTriangles.point5 "
	    "AND Consequent.point6 = SimilarTriangles.point6";
	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0)
	    return false;
	else
	    return true;
	break;
    default:
	// ERROR : Necessary?  I don't _think_ so...
	std::cerr << "Unknown geometric command!!!" << std::endl;
	exit(1);
    }
}

/*
 * Show fixed point - displays 'Facts' table after findind the fixed point.
 */
void Prover::showFixedPoint(DBinMemory dbim) {
    std::string sqlFacts, fixedPointFact;
    std::string idFact, typeGeoCmd;
    std::string point1, point2, point3, point4, point5, point6, point7, point8;
    sqlite3_stmt *stmt1; 

    std::cout << std::endl;
    std::cout << "Fixed Point" << std::endl;
    std::cout << std::endl;
    sqlFacts = "SELECT id, typeGeoCmd FROM Facts";
    dbim.rc = sqlite3_prepare_v2(dbim.db, sqlFacts.c_str(), sqlFacts.size(),
				 &(dbim.stmt), NULL);
    while (sqlite3_step(dbim.stmt) == SQLITE_ROW) {
	idFact = (char*) sqlite3_column_text(dbim.stmt, 0);
	typeGeoCmd = (char*) sqlite3_column_text(dbim.stmt, 1);
	switch (dbim.geoCmds[typeGeoCmd]) {
	case 1:
            // Collinear
	    // Save geometric command for the derivation of new facts
	    fixedPointFact = "SELECT point1, point2, point3 "
		"FROM Collinear "
		"WHERE oldFact = " + idFact;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(),
					 fixedPointFact.size(), &(dbim.stmt1),
					 NULL);
	    sqlite3_step(dbim.stmt1);
	    point1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	    std::cout << "  coll(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ")" << std::endl;
	    break;
	case 2:
            // Parallel
	    fixedPointFact = "SELECT point1, point2, point3, point4 "
		"FROM Parallel "
		"WHERE oldFact = " + idFact;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(),
					 fixedPointFact.size(), &(dbim.stmt1),
					 NULL);
	    sqlite3_step(dbim.stmt1);
	    point1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	    point4 = (char*) sqlite3_column_text(dbim.stmt1, 3);
	    std::cout << "  para(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ")" << std::endl;
	    break;
	case 3:
            // Perpendicular
	    fixedPointFact = "SELECT point1, point2, point3, point4 "
		"FROM Perpendicular "
		"WHERE oldFact = " + idFact;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(),
					 fixedPointFact.size(), &(dbim.stmt1),
					 NULL);
	    sqlite3_step(dbim.stmt1);
	    point1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	    point4 = (char*) sqlite3_column_text(dbim.stmt1, 3);
	    std::cout << "  perp(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ")" << std::endl;
	    break;
	case 4:
            // Midpoint
	    fixedPointFact = "SELECT point1, point2, point3 "
		"FROM Midpoint "
		"WHERE oldFact = " + idFact;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(),
					 fixedPointFact.size(), &(dbim.stmt1),
					 NULL);
	    sqlite3_step(dbim.stmt1);
	    point1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	    std::cout << "  midp(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ")" << std::endl;
	    break;
	case 5:
	    // Circle
	    fixedPointFact = "SELECT point1, point2, point3, point4 "
		"FROM Circle "
		"WHERE oldFact = " + idFact;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(),
					 fixedPointFact.size(), &(dbim.stmt1),
					 NULL);
	    sqlite3_step(dbim.stmt1);
	    point1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	    point4 = (char*) sqlite3_column_text(dbim.stmt1, 3);
	    std::cout << "  circle(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ")" << std::endl;
	    break;
	case 6:
	    // Congruent Segments
	    fixedPointFact = "SELECT point1, point2, point3, point4 "
		"FROM CongruentSegments "
		"WHERE oldFact = " + idFact;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(),
					 fixedPointFact.size(), &(dbim.stmt1),
					 NULL);
	    sqlite3_step(dbim.stmt1);
	    point1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	    point4 = (char*) sqlite3_column_text(dbim.stmt1, 3);
	    std::cout << "  cong(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ")" << std::endl;
	    break;
	case 7:
	    // Congruent Triangles
	    fixedPointFact = "SELECT point1, point2, point3, "
		"point4, point5, point6 "
		"FROM CongruentTriangles "
		"WHERE oldFact = " + idFact;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(),
					 fixedPointFact.size(), &(dbim.stmt1),
					 NULL);
	    sqlite3_step(dbim.stmt1);
	    point1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	    point4 = (char*) sqlite3_column_text(dbim.stmt1, 3);
	    point5 = (char*) sqlite3_column_text(dbim.stmt1, 4);
	    point6 = (char*) sqlite3_column_text(dbim.stmt1, 5);
	    std::cout << "  contri(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ", ";
	    std::cout << point5 << ", ";
	    std::cout << point6 << ")" << std::endl;
	    break;
	case 8:
	    // Cyclic
	    fixedPointFact = "SELECT point1, point2, point3, point4 "
		"FROM Cyclic "
		"WHERE oldFact = " + idFact;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(),
					 fixedPointFact.size(), &(dbim.stmt1),
					 NULL);
	    sqlite3_step(dbim.stmt1);
	    point1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	    point4 = (char*) sqlite3_column_text(dbim.stmt1, 3);
	    std::cout << "  cyclic(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ")" << std::endl;
	    break;
	case 9:
	    // Equal Angles
	    fixedPointFact = "SELECT point1, point2, point3, point4, "
		"point5, point6, point7, point8 "
		"FROM EqualAngles "
		"WHERE oldFact = " + idFact;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(),
					 fixedPointFact.size(), &(dbim.stmt1),
					 NULL);
	    sqlite3_step(dbim.stmt1);
	    point1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	    point4 = (char*) sqlite3_column_text(dbim.stmt1, 3);
	    point5 = (char*) sqlite3_column_text(dbim.stmt1, 4);
	    point6 = (char*) sqlite3_column_text(dbim.stmt1, 5);
	    point7 = (char*) sqlite3_column_text(dbim.stmt1, 6);
	    point8 = (char*) sqlite3_column_text(dbim.stmt1, 7);
	    std::cout << "  eqangle(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ", ";
	    std::cout << point5 << ", ";
	    std::cout << point6 << ", ";
	    std::cout << point7 << ", ";
	    std::cout << point8 << ")" << std::endl;
	    break;
	case 10:
	    // Equal Ratios
	    fixedPointFact = "SELECT point1, point2, point3, point4, "
		"point5, point6, point7, point8 "
		"FROM EqualRatios "
		"WHERE oldFact = " + idFact;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(),
					 fixedPointFact.size(), &(dbim.stmt1),
					 NULL);
	    sqlite3_step(dbim.stmt1);
	    point1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	    point4 = (char*) sqlite3_column_text(dbim.stmt1, 3);
	    point5 = (char*) sqlite3_column_text(dbim.stmt1, 4);
	    point6 = (char*) sqlite3_column_text(dbim.stmt1, 5);
	    point7 = (char*) sqlite3_column_text(dbim.stmt1, 6);
	    point8 = (char*) sqlite3_column_text(dbim.stmt1, 7);
	    std::cout << "  eqratio(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ", ";
	    std::cout << point5 << ", ";
	    std::cout << point6 << ", ";
	    std::cout << point7 << ", ";
	    std::cout << point8 << ")" << std::endl;
	    break;
	case 11:
	    // Similar Triangles
	    fixedPointFact = "SELECT point1, point2, point3, "
		"point4, point5, point6 "
		"FROM SimilarTriangles "
		"WHERE oldFact = " + idFact;
	    dbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(),
					 fixedPointFact.size(), &(dbim.stmt1),
					 NULL);
	    sqlite3_step(dbim.stmt1);
	    point1 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    point2 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    point3 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	    point4 = (char*) sqlite3_column_text(dbim.stmt1, 3);
	    point5 = (char*) sqlite3_column_text(dbim.stmt1, 4);
	    point6 = (char*) sqlite3_column_text(dbim.stmt1, 5);
	    std::cout << "  simtri(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ", ";
	    std::cout << point5 << ", ";
	    std::cout << point6 << ")" << std::endl;
	    break;
	default:
            // ERROR : Necessary? I don't _think_ so...
	    std::cerr << "Unknown geometric command!!!" << std::endl;
	    exit(1);
	}
    }
    std::cout << std::endl;
}
