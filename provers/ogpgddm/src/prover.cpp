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


#include "prover.hpp"
#include "foftodb.hpp" // Show the BD

#include <iostream>
#include <map>

/*
 * Rule D1: coll(A, B, C) => coll(A, C, B)
 */
DBinMemory Prover::ruleD1(DBinMemory dbim, std::string point1,
			  std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('coll')";
    lastInsertedRowId = "SELECT last_insert_rowid()";

    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str() ,
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);

    insertionPred = "INSERT INTO Collinear(typeGeoCmd, point1, point2, point3, newFact) VALUES ('coll', '" + point1 + "', '" + point3 + "', '" + point2 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return dbim;
}

/*
 * Rule D2: coll(A, B, C) => coll(B, A, C)
 */
DBinMemory Prover::ruleD2(DBinMemory dbim, std::string point1,
			  std::string point2,std::string point3) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('coll')";
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

    insertionPred = "INSERT INTO Collinear(typeGeoCmd, point1, point2, point3, newFact) VALUES ('coll', '" + point2 + "', '" + point1 + "', '" + point3 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return dbim;
}

/*
 * Rule D3: coll(A, B, C) & coll(A, B, D) => coll(C, D, A)
 */
DBinMemory Prover::ruleD3(DBinMemory dbim, std::string point1,
			  std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint3;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('coll')";
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

    querySecondGeoCmdA = "SELECT point3 FROM NewFact INNER JOIN Collinear ON (newFact = id) WHERE point1 = '" + point1 + "' and point2 = '" + point2 + "' and point3 <> '" + point3 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);

    querySecondGeoCmdB = "SELECT point3 FROM Facts INNER JOIN Collinear ON (oldFact = id) WHERE point1 = '" + point1 + "' and point2 = '" + point2 + "' and point3 <> '" + point3 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction=false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	} else {
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO Collinear(typeGeoCmd, point1, point2, point3, newFact) VALUES ('coll', '" + point3 + "', '" + newPoint3 + "', '" + point1 + "', '" + lstInsRwId + "')";

	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    }
	}
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return dbim;
}

/*
 * Rule D4: para(A, B, C, D) => para(A, B, D, C)
 */
DBinMemory Prover::ruleD4(DBinMemory dbim, std::string point1,
			  std::string point2, std::string point3,
			  std::string point4) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('para')";
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
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionPred = "INSERT INTO Parallel(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('para', '" + point1 + "', '" + point2 + "', '" + point4 + "', '" + point3 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return dbim;
}

/*
 * Rule D5: para(A, B, C, D) => para(C, D, A, B)
 */
DBinMemory Prover::ruleD5(DBinMemory dbim, std::string point1,
			  std::string point2, std::string point3,
			  std::string point4) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('para')";
    lastInsertedRowId = "SELECT last_insert_rowid()";

    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg));
    correctTransaction=true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(),
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionPred = "INSERT INTO Parallel(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('para', '" + point3 + "', '" + point4 + "', '" + point1 + "', '" + point2 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return dbim;
}

/*
 * Rule D6: para(A, B, C, D) & para(C, D, E, F) => para(A, B, E, F)
 */
DBinMemory Prover::ruleD6(DBinMemory dbim, std::string point1,
			  std::string point2, std::string point3,
			  std::string point4) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint3, newPoint4;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('para')";
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

    querySecondGeoCmdA = "SELECT point3, point4 FROM NewFact INNER JOIN Parallel ON (newFact = id) WHERE point1 = '" + point3 + "' and point2 = '" + point4 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);

    querySecondGeoCmdB = "SELECT point3, point4 FROM Facts INNER JOIN Parallel ON (oldFact = id) WHERE point1 = '" + point3 + "' and point2 = '" + point4 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO Parallel(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('para', '" + point1 + "', '" + point2 + "', '" + newPoint3 + "', '" + newPoint4 + "', '" + lstInsRwId + "')";

	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    }
	}
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return dbim;
}

/*
 * Rule D7: perp(A, B, C, D) => perp(A, B, D, C)
 */
DBinMemory Prover::ruleD7(DBinMemory dbim, std::string point1,
			  std::string point2, std::string point3,
			  std::string point4) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('perp')";
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

    insertionPred = "INSERT INTO Perpendicular(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('perp', '" + point1 + "', '" + point2 + "', '" + point4 + "', '" + point3 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return dbim;
}

/*
 * Rule D8: perp(A, B, C, D) => perp(C, D, A, B)
 */
DBinMemory Prover::ruleD8(DBinMemory dbim, std::string point1,
			  std::string point2, std::string point3,
			  std::string point4) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('perp')";

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

    insertionPred = "INSERT INTO Perpendicular(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('perp', '" + point3 + "', '" + point4 + "', '" + point1 + "', '" + point2 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D9: perp(A, B, C, D) & perp(C, D, E, F) => para(A, B, E, F)
 */
DBinMemory Prover::ruleD9(DBinMemory dbim, std::string point1,
			  std::string point2, std::string point3,
			  std::string point4) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint3, newPoint4;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('para')";
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

    querySecondGeoCmdA = "SELECT point3, point4 FROM NewFact INNER JOIN Perpendicular ON (newFact=id) WHERE point1 = '" + point3 + "' and point2 = '" + point4 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    
    querySecondGeoCmdB = "SELECT point3, point4 FROM Facts INNER JOIN Perpendicular ON (oldFact = id) WHERE point1 = '" + point3 + "' and point2 = '" + point4 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction=false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO Parallel(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('para', '" + point1 + "', '" + point2 + "', '" + newPoint3 + "', '" + newPoint4 + "', '" + lstInsRwId + "')";

	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    }
	}
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
  return dbim;
}

/*
 * Rule D10: para(A, B, C, D) & perp(C, D, E, F) => perp(A, B, E, F)
 */
DBinMemory Prover::ruleD10(DBinMemory dbim, std::string point1,
			       std::string point2, std::string point3,
			       std::string point4) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint3, newPoint4;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('perp')";
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

    querySecondGeoCmdA = "SELECT point3, point4 FROM NewFact INNER JOIN Perpendicular ON (newFact=id) WHERE point1 = '" + point3 + "' and point2 = '" + point4 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    
    querySecondGeoCmdB = "SELECT point3, point4 FROM Facts INNER JOIN Perpendicular ON (oldFact = id) WHERE point1 = '" + point3 + "' and point2 = '" + point4 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction=false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO Perpendicular(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('perp', '" + point1 + "', '" + point2 + "', '" + newPoint3 + "', '" + newPoint4 + "', '" + lstInsRwId + "')";

	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    }
	}
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
  return dbim;
}

/*
 * Rule D11 : midp(M, B, A) => midp(M, A, B)
 */
DBinMemory Prover::ruleD11(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3){
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('midp')";
    lastInsertedRowId = "SELECT last_insert_rowid()";

    sqlite3_exec(dbim.db, "begin;", 0, 0, &(dbim.zErrMsg)); 
    correctTransaction = true;
    dbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str() ,
				 insertNewFact.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    dbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(),
				 lastInsertedRowId.size(), &(dbim.stmt), NULL);
    sqlite3_step(dbim.stmt);
    lstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);

    insertionPred = "INSERT INTO Midpoint(typeGeoCmd, point1, point2, point3, newFact) VALUES ('midp', '" + point1 + "', '" + point3 + "', '" + point2 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D12: cong(O, A, O, B) & perp(O, A, O, C) => circle(O, A, B, C)
 */
DBinMemory Prover::ruleD12(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string InsertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint4;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('circle')";
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

    querySecondGeoCmdA = "SELECT point4 FROM NewFact INNER JOIN CongruentSegments ON (newFact=id) WHERE point1 = '" + point1 + "' and point2 = '" + point2 + "' and point3 = '" + point3 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);
    
    querySecondGeoCmdB = "SELECT point4 FROM Facts INNER JOIN CongruentSegments ON (oldFact = id) WHERE point1 = '" + point1 + "' and point2 = '" + point2 + "' and point3 = '" + point3 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);

    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction=false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	} else {
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    InsertionPred = "INSERT INTO Circle(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('circle', '" + point1 + "', '" + point2 + "', '" + point4 + "', '" + newPoint4 + "', '" + lstInsRwId + "')";

	    dbim.rc = sqlite3_prepare_v2(dbim.db, InsertionPred.c_str(),
					 InsertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    }
	}
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
  return dbim;
}

/*
 * Rule D14: cyclic(A, B, C, D) => cyclic(A, B, D, C)
 */
DBinMemory Prover::ruleD14(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionCyclic;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('cyclic')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionCyclic = "INSERT INTO Cyclic(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('cyclic', '" + point1 + "', '" + point2 + "', '" + point4 + "', '" + point3 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionCyclic.c_str(),
				 insertionCyclic.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D15: cyclic(A, B, C, D) => cyclic(A, C, B, D)
 */
DBinMemory Prover::ruleD15(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionCyclic;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('cyclic')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionCyclic = "INSERT INTO Cyclic(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('cyclic', '" + point1 + "', '" + point3 + "', '" + point2 + "', '" + point4 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionCyclic.c_str(),
				 insertionCyclic.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}


/*
 * Rule D16: cyclic(A, B, C, D) => cyclic(B, A, C, D)
 */
DBinMemory Prover::ruleD16(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionCyclic;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('cyclic')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionCyclic = "INSERT INTO Cyclic(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('cyclic', '" + point2 + "', '" + point1 + "', '" + point3 + "', '" + point4 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionCyclic.c_str(),
				 insertionCyclic.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D17: cyclic(A, B, C, D) & cyclic(A, B, C, E) => cyclic(B, C, D, E)
 */
DBinMemory Prover::ruleD17(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionCyclic;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint4;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('cyclic')";
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

    querySecondGeoCmdA = "SELECT point4 FROM NewFact INNER JOIN Cyclic ON (newFact = id) WHERE point1 = '" + point1 + "' and point2 = '" + point2 + "' and point3 = '" + point3 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);

    querySecondGeoCmdB = "SELECT point4 FROM Facts INNER JOIN Cyclic ON (newFact = id) WHERE point1 = '" + point1 + "' and point2 = '" + point2 + "' and point3 = '" + point3 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	} else {
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionCyclic = "INSERT INTO Cyclic(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('cyclic', '" + point2 + "', '" + point3 + "', '" + point4 + "', '" + newPoint4 + "', '" + lstInsRwId + "')";

	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionCyclic.c_str(),
					 insertionCyclic.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    }
	}
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return dbim;
}

/*
 * Rule D18: eqangle(A, B, C, D, P, Q, U, V) => eqangle(B, A, C, D, P, Q, U, V)
 */
DBinMemory Prover::ruleD18(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionEqangle;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('eqangle')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionEqangle = "INSERT INTO EqualAngles(typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8, newFact) VALUES ('eqangle', '" + point2 + "', '" + point1 + "', '" + point3 + "', '" + point4 + "', '" + point5 + "', '" + point6 + "', '" + point7 + "', '" + point8 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionEqangle.c_str(),
				 insertionEqangle.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D19: eqangle(A, B, C, D, P, Q, U, V) => eqangle(C, D, A, B, U, V, P, Q)
 */
DBinMemory Prover::ruleD19(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionEqangle;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('eqangle')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionEqangle = "INSERT INTO EqualAngles(typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8, newFact) VALUES ('eqangle', '" + point3 + "', '" + point4 + "', '" + point1 + "', '" + point2 + "', '" + point7 + "', '" + point8 + "', '" + point5 + "', '" + point6 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionEqangle.c_str(),
				 insertionEqangle.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D20: eqangle(A, B, C, D, P, Q, U, V) => eqangle(P, Q, U, V, A, B, C, D)
 */
DBinMemory Prover::ruleD20(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionEqangle;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('eqangle')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionEqangle = "INSERT INTO EqualAngles(typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8, newFact) VALUES ('eqangle', '" + point5 + "', '" + point6 + "', '" + point7 + "', '" + point8 + "', '" + point1 + "', '" + point2 + "', '" + point3 + "', '" + point4 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionEqangle.c_str(),
				 insertionEqangle.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D21: eqangle(A, B, C, D, P, Q, U, V) => eqangle(A, B, P, Q, C, D, U, V)
 */
DBinMemory Prover::ruleD21(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionEqangle;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('eqangle')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionEqangle = "INSERT INTO EqualAngles(typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8, newFact) VALUES ('eqangle', '" + point1 + "', '" + point2 + "', '" + point5 + "', '" + point6 + "', '" + point3 + "', '" + point4 + "', '" + point7 + "', '" + point8 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionEqangle.c_str(),
				 insertionEqangle.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D22: eqangle(A, B, C, D, P, Q, U, V) & eqangle(P, Q, U, V, E, F, G, H)
                 => eqangle(A, B, C, D, E, F, G, H)
 */
DBinMemory Prover::ruleD22(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionEqangle;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint5, newPoint6, newPoint7, newPoint8;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('eqangle')";
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

    querySecondGeoCmdA = "SELECT point5, point6, point7, point8 FROM NewFact INNER JOIN EqualAngles ON (newFact = id) WHERE point1 = '" + point5 + "' and point2 = '" + point6 + "' and point3 = '" + point7 + "' and point4 = '" + point8 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);

    querySecondGeoCmdB = "SELECT point5, point6, point7, point8 FROM Facts INNER JOIN EqualAngles ON (oldFact = id) WHERE point1 = '" + point5 + "' and point2 = '" + point6 + "' and point3 = '" + point7 + "' and point4 = '" + point8 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint5 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint6 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    newPoint7 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	    newPoint8 = (char*) sqlite3_column_text(dbim.stmt1, 3);
	} else {
	    newPoint5 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint6 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	    newPoint7 = (char*) sqlite3_column_text(dbim.stmt2, 2);
	    newPoint8 = (char*) sqlite3_column_text(dbim.stmt2, 3);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionEqangle = "INSERT INTO EqualAngles(typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8, newFact) VALUES ('eqangle', '" + point1 + "', '" + point2 + "', '" + point3 + "', '" + point4 + "', '" + newPoint5 + "', '" + newPoint6 + "', '" + newPoint7 + "', '" + newPoint8 + "', '" + lstInsRwId + "')";

	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionEqangle.c_str(),
					 insertionEqangle.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    }
	}
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return dbim;
}

/*
 * Rule D23: cong(A, B, C, D) => cong(A, B, D, C)
 */
DBinMemory Prover::ruleD23(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string InsertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('cong')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    InsertionPred = "INSERT INTO CongruentSegments(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('cong', '" + point1 + "', '" + point2 + "', '" + point4 + "', '" + point3 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, InsertionPred.c_str(),
				 InsertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D24: cong(A, B, C, D) => cong(C, D, A, B)
 */
DBinMemory Prover::ruleD24(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string InsertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('cong')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    InsertionPred = "INSERT INTO CongruentSegments(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('cong', '" + point3 + "', '" + point4 + "', '" + point1 + "', '" + point2 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, InsertionPred.c_str(),
				 InsertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D25: cong(A, B, C, D) & cong(C, D, E, F) => cong(A, B, E, F)
 */
DBinMemory Prover::ruleD25(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string InsertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint3, newPoint4;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('cong')";
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

    querySecondGeoCmdA = "SELECT point3, point4 FROM NewFact INNER JOIN CongruentSegments ON (newFact = id) WHERE point1 = '" + point3 + "' and point2 = '" + point4 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);

    querySecondGeoCmdB = "SELECT point3, point4 FROM Facts INNER JOIN CongruentSegments ON (oldFact = id) WHERE point1 = '" + point3 + "' and point2 = '" + point4 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    InsertionPred = "INSERT INTO CongruentSegments(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('cong', '" + point1 + "', '" + point2 + "', '" + newPoint3 + "', '" + newPoint4 + "', '" + lstInsRwId + "')";

	    dbim.rc = sqlite3_prepare_v2(dbim.db, InsertionPred.c_str(),
					 InsertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    }
	}
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return dbim;
}

/*
 * Rule D26: eqratio(A, B, C, D, P, Q, U, V) => eqratio(B, A, C, D, P, Q, U, V)
 */
DBinMemory Prover::ruleD26(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionEqratio;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('eqratio')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionEqratio = "INSERT INTO EqualRatios(typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8, newFact) VALUES ('eqratio', '" + point2 + "', '" + point1 + "', '" + point3 + "', '" + point4 + "', '" + point5 + "', '" + point6 + "', '" + point7 + "', '" + point8 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionEqratio.c_str(),
				 insertionEqratio.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D27: eqratio(A, B, C, D, P, Q, U, V) => eqratio(C, D, A, B, U, V, P, Q)
 */
DBinMemory Prover::ruleD27(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionEqratio;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('eqratio')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionEqratio = "INSERT INTO EqualRatios(typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8, newFact) VALUES ('eqratio', '" + point3 + "', '" + point4 + "', '" + point1 + "', '" + point2 + "', '" + point7 + "', '" + point8 + "', '" + point5 + "', '" + point6 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionEqratio.c_str(),
				 insertionEqratio.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D28: eqratio(A, B, C, D, P, Q, U, V) => eqratio(P, Q, U, V, A, B, C, D)
 */
DBinMemory Prover::ruleD28(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionEqratio;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('eqratio')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionEqratio = "INSERT INTO EqualRatios(typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8, newFact) VALUES ('eqratio', '" + point5 + "', '" + point6 + "', '" + point7 + "', '" + point8 + "', '" + point1 + "', '" + point2 + "', '" + point3 + "', '" + point4 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionEqratio.c_str(),
				 insertionEqratio.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D29: eqratio(A, B, C, D, P, Q, U, V) => eqratio(A, B, P, Q, C, D, U, V)
 */
DBinMemory Prover::ruleD29(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionEqratio;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('eqratio')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionEqratio = "INSERT INTO EqualRatios(typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8, newFact) VALUES ('eqratio', '" + point1 + "', '" + point2 + "', '" + point5 + "', '" + point6 + "', '" + point3 + "', '" + point4 + "', '" + point7 + "', '" + point8 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionEqratio.c_str(),
				 insertionEqratio.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D30: eqratio(A, B, C, D, P, Q, U, V) & eqratio(P, Q, U, V, E, F, G, H)
 *               => eqratio(A, B, C, D, E, F, G, H)
 */
DBinMemory Prover::ruleD30(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionEqratio;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint5, newPoint6, newPoint7, newPoint8;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('eqratio')";
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

    querySecondGeoCmdA = "SELECT point5, point6, point7, point8 FROM NewFact INNER JOIN EqualRatios ON (newFact = id) WHERE point1 = '" + point5 + "' and point2 = '" + point6 + "' and point3 = '" + point7 + "' and point4 = '" + point8 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);

    querySecondGeoCmdB = "SELECT point5, point6, point7, point8 FROM Facts INNER JOIN EqualRatios ON (oldFact = id) WHERE point1 = '" + point5 + "' and point2 = '" + point6 + "' and point3 = '" + point7 + "' and point4 = '" + point8 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint5 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint6 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    newPoint7 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	    newPoint8 = (char*) sqlite3_column_text(dbim.stmt1, 3);
	} else {
	    newPoint5 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint6 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	    newPoint7 = (char*) sqlite3_column_text(dbim.stmt2, 2);
	    newPoint8 = (char*) sqlite3_column_text(dbim.stmt2, 3);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionEqratio = "INSERT INTO EqualRatios(typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8, newFact) VALUES ('eqratio', '" + point1 + "', '" + point2 + "', '" + point3 + "', '" + point4 + "', '" + newPoint5 + "', '" + newPoint6 + "', '" + newPoint7 + "', '" + newPoint8 + "', '" + lstInsRwId + "')";

	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionEqratio.c_str(),
					 insertionEqratio.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    }
	}
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return dbim;
}

/*
 * Rule D31: simtri(A, C, B, P, R, Q) => simtri(A, B, C, P, Q, R)
 */
DBinMemory Prover::ruleD31(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionSimtri;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('simtri')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionSimtri = "INSERT INTO SimilarTriangles(typeGeoCmd, point1, point2, point3, point4, point5, point6, newFact) VALUES ('simtri', '" + point1 + "', '" + point3 + "', '" + point2 + "', '" + point4 + "', '" + point6 + "',  '" + point5 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionSimtri.c_str(),
				 insertionSimtri.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D32: simtri(B, A, C, Q, P, R) => simtri(A, B, C, P, Q, R)
 */
DBinMemory Prover::ruleD32(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionSimtri;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('simtri')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionSimtri = "INSERT INTO SimilarTriangles(typeGeoCmd, point1, point2, point3, point4, point5, point6, newFact) VALUES ('simtri', '" + point2 + "', '" + point1 + "', '" + point3 + "', '" + point5 + "', '" + point4 + "', '" + point6 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionSimtri.c_str(),
				 insertionSimtri.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D33: simtri(P, Q, R, A, B, C) => simtri(A, B, C, P, Q, R)
 */
DBinMemory Prover::ruleD33(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionSimtri;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('simtri')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionSimtri = "INSERT INTO SimilarTriangles(typeGeoCmd, point1, point2, point3, point4, point5, point6, newFact) VALUES ('simtri', '" + point4 + "', '" + point5 + "', '" + point6 + "', '" + point1 + "', '" + point2 + "', '" + point3 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionSimtri.c_str(),
				 insertionSimtri.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D34: simtri(A, B, C, E, F, G) & simtri(E, F, G, P, Q, R)
 *               => simtri(A, B, C, P, Q, R)
 */
DBinMemory Prover::ruleD34(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionSimtri;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint4, newPoint5, newPoint6;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('simtri')";
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

    querySecondGeoCmdA = "SELECT point4, point5, point6 FROM NewFact INNER JOIN SimilarTriangles ON (newFact = id) WHERE point1 = '" + point4 + "' and point2 = '" + point5 + "' and point3 = '" + point6 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);

    querySecondGeoCmdB = "SELECT point4, point5, point6 FROM Facts INNER JOIN SimilarTriangles ON (oldFact = id) WHERE point1 = '" + point4 + "' and point2 = '" + point5 + "' and point3 = '" + point6 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint5 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    newPoint6 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	} else {
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint5 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	    newPoint6 = (char*) sqlite3_column_text(dbim.stmt2, 2);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionSimtri = "INSERT INTO SimilarTriangles(typeGeoCmd, point1, point2, point3, point4, point5, point6, newFact) VALUES ('simtri', '" + point1 + "', '" + point2 + "', '" + point3 + "', '" + newPoint4 + "', '" + newPoint5 + "', '" + newPoint6 + "', '" + lstInsRwId + "')";

	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionSimtri.c_str(),
					 insertionSimtri.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    }
	}
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return dbim;
}

/*
 * Rule D35: contri(A, C, B, P, R, Q) => contri(A, B, C, P, Q, R)
 */
DBinMemory Prover::ruleD35(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('contri')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionPred = "INSERT INTO CongruentTriangles(typeGeoCmd, point1, point2, point3, point4, point5, point6, newFact) VALUES ('contri', '" + point1 + "', '" + point3 + "', '" + point2 + "', '" + point4 + "', '" + point6 + "',  '" + point5 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D36: contri(B, A, C, Q, P, R) => contri(A, B, C, P, Q, R)
 */
DBinMemory Prover::ruleD36(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('contri')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionPred = "INSERT INTO CongruentTriangles(typeGeoCmd, point1, point2, point3, point4, point5, point6, newFact) VALUES ('contri', '" + point2 + "', '" + point1 + "', '" + point3 + "', '" + point5 + "', '" + point4 + "', '" + point6 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D37: contri(P, Q, R, A, B, C) => contri(A, B, C, P, Q, R)
 */
DBinMemory Prover::ruleD37(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('contri')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionPred = "INSERT INTO CongruentTriangles(typeGeoCmd, point1, point2, point3, point4, point5, point6, newFact) VALUES ('contri', '" + point4 + "', '" + point5 + "', '" + point6 + "', '" + point1 + "', '" + point2 + "', '" + point3 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D38: contri(A, B, C, E, F, G) & contri(E, F, G, P, Q, R)
 *               => contri(A, B, C, P, Q, R)
 */
DBinMemory Prover::ruleD38(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint4, newPoint5, newPoint6;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('contri')";
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

    querySecondGeoCmdA = "SELECT point4, point5, point6 FROM NewFact INNER JOIN CongruentTriangles ON (newFact = id) WHERE point1 = '" + point4 + "' and point2 = '" + point5 + "' and point3 = '" + point6 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);

    querySecondGeoCmdB = "SELECT point4, point5, point6 FROM Facts INNER JOIN CongruentTriangles ON (oldFact = id) WHERE point1 = '" + point4 + "' and point2 = '" + point5 + "' and point3 = '" + point6 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction = false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint5 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	    newPoint6 = (char*) sqlite3_column_text(dbim.stmt1, 2);
	} else {
	    newPoint4 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint5 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	    newPoint6 = (char*) sqlite3_column_text(dbim.stmt2, 2);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO CongruentTriangles(typeGeoCmd, point1, point2, point3, point4, point5, point6, newFact) VALUES ('contri', '" + point1 + "', '" + point2 + "', '" + point3 + "', '" + newPoint4 + "', '" + newPoint5 + "', '" + newPoint6 + "', '" + lstInsRwId + "')";

	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    }
	}
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return dbim;
}

/*
 * Rule D39: eqangle(A, B, P, Q, C, D, P, Q) => para(A, B, C, D)
 */
DBinMemory Prover::ruleD39(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('para')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionPred = "INSERT INTO Parallel(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('para', '" + point1 + "', '" + point2 + "', '" + point5 + "', '" + point6 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D41: cyclic(A, B, P, Q) => eqangle(P, A, P, B, Q, A, Q, B)
 */
DBinMemory Prover::ruleD41(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionEqangle;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('eqangle')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionEqangle = "INSERT INTO EqualAngles(typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8, newFact) VALUES ('eqangle', '" + point3 + "', '" + point1 + "', '" + point3 + "', '" + point2 + "', '" + point4 + "', '" + point1 + "', '" + point4 + "', '" + point2 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionEqangle.c_str(),
				 insertionEqangle.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D46: cong(O, A, O, B) => eqangle(O, A, A, B, A, B, O, B)
 */
DBinMemory Prover::ruleD46(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionEqangle;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('eqangle')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionEqangle = "INSERT INTO EqualAngles(typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8, newFact) VALUES ('eqangle', '" + point1 + "', '" + point2 + "', '" + point2 + "', '" + point4 + "', '" + point2 + "', '" + point4 + "', '" + point1 + "', '" + point4 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionEqangle.c_str(),
				 insertionEqangle.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D59: simtri(A, B, C, P, Q; R) => eqratio(A, B, A, C, P, Q, P, R)
 */
DBinMemory Prover::ruleD59(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionEqratio;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('eqratio')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionEqratio = "INSERT INTO EqualRatios(typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8, newFact) VALUES ('eqratio', '" + point1 + "', '" + point2 + "', '" + point1 + "', '" + point3 + "', '" + point4 + "', '" + point5 + "', '" + point4 + "', '" + point6 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionEqratio.c_str(),
				 insertionEqratio.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D60: simtri(A, B, C, P, Q; R) => eqangle(A, B, B, C, P, Q, Q, R)
 */
DBinMemory Prover::ruleD60(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string insertionEqangle;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('eqangle')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionEqangle = "INSERT INTO EqualAngles(typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8, newFact) VALUES ('eqangle', '" + point1 + "', '" + point2 + "', '" + point2 + "', '" + point3 + "', '" + point4 + "', '" + point5 + "', '" + point5 + "', '" + point6 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionEqangle.c_str(),
				 insertionEqangle.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D62: contri(A, B, C, P, Q; R) => cong(A, B, P, Q)
 */
DBinMemory Prover::ruleD62(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6) {
    bool correctTransaction;
    std::string InsertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('cong')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    InsertionPred = "INSERT INTO CongruentSegments(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('cong', '" + point1 + "', '" + point2 + "', '" + point4 + "', '" + point5 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, InsertionPred.c_str(),
				 InsertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D63: midp(M, A, B) & midp(M, C, D) => para(A, C, B, D)
 */
DBinMemory Prover::ruleD63(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;
    std::string newPoint2, newPoint3;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('para')";
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

    querySecondGeoCmdA = "SELECT point2, point3 FROM NewFact INNER JOIN Midpoint ON (newFact = id) WHERE point1 = '" + point1 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);

    querySecondGeoCmdB = "SELECT point2, point3 FROM Facts INNER JOIN Midpoint ON (oldFact = id) WHERE point1 = '" + point1 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction=false;
    } else {
	if (sqlite3_data_count(dbim.stmt1) != 0) {
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt1, 0);
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt1, 1);
	} else {
	    newPoint2 = (char*) sqlite3_column_text(dbim.stmt2, 0);
	    newPoint3 = (char*) sqlite3_column_text(dbim.stmt2, 1);
	}
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO Parallel(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('para', '" + point2 + "', '" + newPoint2 + "', '" + point3 + "', '" + newPoint3 + "', '" + lstInsRwId + "')";

	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    }
	}
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return dbim;
}

/*
 * Rule D66: para(A, B, A, C) => coll(A, B, C)
 */
DBinMemory Prover::ruleD66(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('coll')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionPred = "INSERT INTO Collinear(typeGeoCmd, point1, point2, point3, newFact) VALUES ('coll', '" + point1 + "', '" + point2 + "', '" + point4 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D68: midp(A, B, C) => cong(A, B, A, C)
 */
DBinMemory Prover::ruleD68(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3) {
    bool correctTransaction;
    std::string InsertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('cong')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    InsertionPred = "INSERT INTO CongruentSegments(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('cong', '" + point1 + "', '" + point2 + "', '" + point1 + "', '" + point3 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, InsertionPred.c_str(),
				 InsertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D69: midp(A, B, C) => coll(A, B, C)
 */
DBinMemory Prover::ruleD69(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('coll')";
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
    sqlite3_step( dbim.stmt );
    lstInsRwId = (char*) sqlite3_column_text( dbim.stmt, 0 );

    insertionPred = "INSERT INTO Collinear(typeGeoCmd, point1, point2, point3, newFact) VALUES ('coll', '" + point1 + "', '" + point2 + "', '" + point3 + "', '" + lstInsRwId + "')";

    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
				 insertionPred.size(), &(dbim.stmt), NULL);
    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	correctTransaction = false;
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return(dbim);
}

/*
 * Rule D73: eqangle(A, B, C, D, P, Q, U, V) & para(P, Q, U, V)
 *               => para(A, B, C, D)
 */
DBinMemory Prover::ruleD73(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('para')";
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

    querySecondGeoCmdA = "SELECT point1 FROM NewFact INNER JOIN Parallel ON (newFact = id) WHERE point1 = '" + point5 + "' and point2 = '" + point6 + "' and point3 = '" + point7 + "' and point4 = '" + point8 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);

    querySecondGeoCmdB = "SELECT point1 FROM Facts INNER JOIN Parallel ON (oldFact = id) WHERE point1 = '" + point5 + "' and point2 = '" + point6 + "' and point3 = '" + point7 + "' and point4 = '" + point8 +"'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction=false;
    } else {
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO Parallel(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('para', '" + point1 + "', '" + point2 + "', '" + point3 + "', '" + point4 + "', '" + lstInsRwId + "')";

	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    }
	}
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return dbim;
}

/*
 * Rule D74: eqangle(A, B, C, D, P, Q, U, V) & perp(P, Q, U, V)
 *               => perp(A, B, C, D)
 */
DBinMemory Prover::ruleD74(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string insertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('perp')";
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

    querySecondGeoCmdA = "SELECT point1 FROM NewFact INNER JOIN Perpendicular ON (newFact = id) WHERE point1 = '" + point5 + "' and point2 = '" + point6 + "' and point3 = '" + point7 + "' and point4 = '" + point8 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);

    querySecondGeoCmdB = "SELECT point1 FROM Facts INNER JOIN Perpendicular ON (oldFact = id) WHERE point1 = '" + point5 + "' and point2 = '" + point6 + "' and point3 = '" + point7 + "' and point4 = '" + point8 +"'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction=false;
    } else {
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    insertionPred = "INSERT INTO Perpendicular(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('perp', '" + point1 + "', '" + point2 + "', '" + point3 + "', '" + point4 + "', '" + lstInsRwId + "')";

	    dbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(),
					 insertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    }
	}
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return dbim;
}

/*
 * Rule D75: eqratio(A, B, C, D, P, Q, U, V) & cong(P, Q, U, V)
 *               => cong(A, B, C, D)
 */
DBinMemory Prover::ruleD75(DBinMemory dbim, std::string point1,
			   std::string point2, std::string point3,
			   std::string point4, std::string point5,
			   std::string point6, std::string point7,
			   std::string point8) {
    bool correctTransaction;
    std::string InsertionPred;
    std::string insertNewFact, lastInsertedRowId, lstInsRwId;
    std::string querySecondGeoCmdA, querySecondGeoCmdB;

    insertNewFact = "INSERT INTO NewFact(typeGeoCmd) VALUES ('cong')";
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

    querySecondGeoCmdA = "SELECT point1 FROM NewFact INNER JOIN CongruentSegmenta ON (newFact = id) WHERE point1 = '" + point5 + "' and point2 = '" + point6 + "' and point3 = '" + point7 + "' and point4 = '" + point8 + "'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(),
				 querySecondGeoCmdA.size(), &(dbim.stmt1),
				 NULL);
    sqlite3_step(dbim.stmt1);

    querySecondGeoCmdB = "SELECT point1 FROM Facts INNER JOIN CongruentSegments ON (oldFact = id) WHERE point1 = '" + point5 + "' and point2 = '" + point6 + "' and point3 = '" + point7 + "' and point4 = '" + point8 +"'";

    dbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdB.c_str(),
				 querySecondGeoCmdB.size(), &(dbim.stmt2),
				 NULL);
    sqlite3_step(dbim.stmt2);
    if (sqlite3_data_count(dbim.stmt1) == 0
	&& sqlite3_data_count(dbim.stmt2) == 0 ) {
	correctTransaction=false;
    } else {
	if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
	    correctTransaction = false;
	} else {
	    InsertionPred = "INSERT INTO CongruentSegments(typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('cong', '" + point1 + "', '" + point2 + "', '" + point3 + "', '" + point4 + "', '" + lstInsRwId + "')";

	    dbim.rc = sqlite3_prepare_v2(dbim.db, InsertionPred.c_str(),
					 InsertionPred.size(), &(dbim.stmt),
					 NULL);
	    if (sqlite3_step(dbim.stmt) != SQLITE_DONE) {
		correctTransaction = false;
	    }
	}
    }
    if (correctTransaction) {
	sqlite3_exec(dbim.db, "commit;", 0, 0, 0);
    } else {
	sqlite3_exec(dbim.db, "rollback;", 0, 0, 0);
    }
    return dbim;
}

/*
 * Calcute de fixed point.
 */
DBinMemory Prover::fixedPoint(DBinMemory dbim) {
    int nrColl, nrPara, nrPerp, nrMidp, nrCircle, nrCong;
    int nrContri, nrCyclic, nrEqangle, nrEqratio, nrSimtri;
    bool correctTransaction;
    std::string newFact;
    std::string typeGeoCmd;
    std::string newFactId;
    std::string deleteNewFact;
    std::string insertFact;
    std::string updateGeoCmd;
    std::string toDeriveGeoCmd;
    std::string insertNewFact;
    std::string querySecondGeoCmdA;
    std::string querySecondGeoCmdB;
    std::string point1, point2, point3, point4, point5, point6, point7, point8;
    std::string newPoint3;
    std::string lastInsertedRowId;
    std::string lstInsRwId;
    std::string insertionPred;
    std::string insertionNewFact;
    FOFtoDB fdb;

    // DEBUG
    std::cout << std::endl << "fixedPoint() : Entering.." << std::endl;

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
	    toDeriveGeoCmd = "SELECT point1, point2, point3 FROM Collinear WHERE newFact = " + newFactId;

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

	    // DEBUG
	    std::cout << "  Collinear : " << nrColl << " / ";
	    std::cout << "coll(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ")" << std::endl;
	    break;
	case 2:
            // Parallel
	    nrPara++;

	    // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3, point4 FROM Parallel WHERE newFact = " + newFactId;
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

	    // DEBUG
	    std::cout << "  Parallel : " << nrPara << " / ";
	    std::cout << "para(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ")" << std::endl;
	    break;
	case 3:
            // Perpendicular
	    nrPerp++;

            // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3, point4 FROM Perpendicular WHERE newFact = " + newFactId;
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

	    // DEBUG
	    std::cout << "  Perpendicular : " << nrPerp << " / ";
	    std::cout << "perp(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ")" << std::endl;
	    break;
	case 4:
            // Midpoint
	    nrMidp ++;

	    // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3 FROM Midpoint WHERE newFact = " + newFactId;

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
	    sqlite3_step( dbim.stmt );

	    // DEBUG
	    std::cout << "  Midpoint : " << nrMidp << " / ";
	    std::cout << "midp(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ")" << std::endl;
	    break;
	case 5:
	    // Circle
	    nrCircle++;

            // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3, point4 FROM Circle WHERE newFact = " + newFactId;
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

	    // DEBUG
	    std::cout << "  Circle : " << nrCircle << " / ";
	    std::cout << "circle(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ")" << std::endl;
	    break;
	case 6:
	    // Congruent Segments
	    nrCong++;

	    // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3, point4 FROM CongruentSegments WHERE newFact = " + newFactId;
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

	    // DEBUG
	    std::cout << "  Congruent Segments : " << nrCong << " / ";
	    std::cout << "cong(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ")" << std::endl;
	    break;
	case 7:
	    // Congruent Triangles
	    nrContri++;

	    // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3, point4, point5, point6 FROM CongruentTriangles WHERE newFact = " + newFactId;
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

	    // DEBUG
	    std::cout << "  Congruent Triangles : " << nrContri << " / ";
	    std::cout << "contri(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ", ";
	    std::cout << point5 << ", ";
	    std::cout << point6 << ")" << std::endl;
	    break;
	case 8:
	    // Cyclic
	    nrCyclic++;

	    // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3, point4 FROM Cyclic WHERE newFact = " + newFactId;
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

	    // DEBUG
	    std::cout << "  Cyclic : " << nrCyclic << " / ";
	    std::cout << "cyclic(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ")" << std::endl;
	    break;
	case 9:
	    // Equal Angles
	    nrEqangle++;

	    // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3, point4, point5, point6, point7, point8 FROM EqualAngles WHERE newFact = " + newFactId;
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

	    // DEBUG
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
	    break;
	case 10:
	    // Equal Ratios
	    nrEqratio++;

	    // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3, point4, point5, point6, point7, point8 FROM EqualRatios WHERE newFact = " + newFactId;
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

	    // DEBUG
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
	    break;
	case 11:
	    // Similar Triangles
	    nrSimtri++;

	    // Save the geometric command for the derivation of new facts
	    toDeriveGeoCmd = "SELECT point1, point2, point3, point4, point5, point6 FROM SimilarTriangles WHERE newFact = " + newFactId;
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

	    // DEBUG
	    std::cout << "  Similar Triangles : " << nrSimtri << " / ";
	    std::cout << "simtri(";
	    std::cout << point1 << ", ";
	    std::cout << point2 << ", ";
	    std::cout << point3 << ", ";
	    std::cout << point4 << ", ";
	    std::cout << point5 << ", ";
	    std::cout << point6 << ")" << std::endl;
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
	    dbim = ruleD1(dbim, point1, point2, point3);
	    dbim = ruleD2(dbim, point1, point2, point3);
	    dbim = ruleD3(dbim, point1, point2, point3);
	    break;
	case 2:
            // Parallel
	    dbim = ruleD4(dbim, point1, point2, point3, point4);
	    dbim = ruleD5(dbim, point1, point2, point3, point4);
	    dbim = ruleD6(dbim, point1, point2, point3, point4);
	    dbim = ruleD10(dbim, point1, point2, point3, point4);
	    if (point1 == point3) {
		dbim = ruleD66(dbim, point1, point2, point3, point4);
	    }
	    break;
	case 3:
            // Perpendicular
	    dbim = ruleD7(dbim, point1, point2, point3, point4);
	    dbim = ruleD8(dbim, point1, point2, point3, point4);
	    dbim = ruleD9(dbim, point1, point2, point3, point4);
	    break;
	case 4:
            // Midpoint
	    dbim = ruleD11(dbim, point1, point2, point3);
	    dbim = ruleD63(dbim, point1, point2, point3);
	    dbim = ruleD68(dbim, point1, point2, point3);
	    dbim = ruleD69(dbim, point1, point2, point3);
	    break;
	case 5:
	    // Circle
	    break;
	case 6:
	    // Congruent Segments
	    if (point1 == point3) {
		dbim = ruleD12(dbim, point1, point2, point3, point4);
	    }
	    dbim = ruleD23(dbim, point1, point2, point3, point4);
	    dbim = ruleD24(dbim, point1, point2, point3, point4);
	    dbim = ruleD25(dbim, point1, point2, point3, point4);
	    if (point1 == point3) {	
		dbim = ruleD46(dbim, point1, point2, point3, point4);
	    }
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
	    if (point3 == point7 && point4 == point8) {
		dbim = ruleD39(dbim, point1, point2, point3, point4,
			       point5, point6, point7, point8);
	    }
	    dbim = ruleD73(dbim, point1, point2, point3, point4,
			   point5, point6, point7, point8);
	    dbim = ruleD74(dbim, point1, point2, point3, point4,
			   point5, point6, point7, point8);
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
	    dbim = ruleD75(dbim, point1, point2, point3, point4,
			   point5, point6, point7, point8);
	    break;
	case 11:
	    // Similar Triangles
	    dbim = ruleD31(dbim, point1, point2, point3,
			   point4, point5, point6);
	    dbim = ruleD32(dbim, point1, point2, point3,
			   point4, point5, point6);
	    dbim = ruleD33(dbim, point1, point2, point3,
			   point4, point5, point6);
	    dbim = ruleD59(dbim, point1, point2, point3,
			   point4, point5, point6);
	    dbim = ruleD60(dbim, point1, point2, point3,
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

    // DEBUG
    // fdb.showDB(dbim);
    std::cout << "fixedPoint() : Leaving..." << std::endl << std::endl;

    return(dbim);
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
	proving = "SELECT * FROM Consequent INNER JOIN Collinear USING(typeGeoCmd) WHERE Consequent.point1 = Collinear.point1 and Consequent.point2 = Collinear.point2 and Consequent.point3 = Collinear.point3";

	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0) {
	    return false;
	} else {
	    return true;
	}
	break;
    case 2:
        // Parallel
	proving = "SELECT * FROM Consequent INNER JOIN Parallel USING(typeGeoCmd) WHERE Consequent.point1 = Parallel.point1 and Consequent.point2 = Parallel.point2 and Consequent.point3 = Parallel.point3 and Consequent.point4 = Parallel.point4";

	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0) {
	    return false;
	} else {
	    return true;
	}
	return false;
	break;
    case 3:
        // Perpendicular
	proving = "SELECT * FROM Consequent INNER JOIN Perpendicular USING(typeGeoCmd) WHERE Consequent.point1 = Perpendicular.point1 and Consequent.point2 = Perpendicular.point2 and Consequent.point3 = Perpendicular.point3 and Consequent.point4 = Perpendicular.point4";

	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0) {
	    return false;
	} else {
	    return true;
	}
	break;
    case 4:
	// Midpoint
	proving = "SELECT * FROM Consequent INNER JOIN Midpoint USING(typeGeoCmd) WHERE Consequent.point1 = Midpoint.point1 and Consequent.point2 = Midpoint.point2 and Consequent.point3 = Midpoint.point3";

	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0) {
	    return false;
	} else {
	    return true;
	}
	break;
    case 5:
	// Circle
	proving = "SELECT * FROM Consequent INNER JOIN Circle USING(typeGeoCmd) WHERE Consequent.point1 = Circle.point1 and Consequent.point2 = Circle.point2 and Consequent.point3 = Circle.point3 and Consequent.point4 = Circle.point4";

	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0) {
	    return false;
	} else {
	    return true;
	}
	break;
    case 6:
	// Congruent Segments
	proving = "SELECT * FROM Consequent INNER JOIN CongruentSegments USING(typeGeoCmd) WHERE Consequent.point1 = CongruentSegments.point1 and Consequent.point2 = CongruentSegments.point2 and Consequent.point3 = CongruentSegments.point3 and Consequent.point4 = CongruentSegments.point4";

	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0) {
	    return false;
	} else {
	    return true;
	}
	break;
    case 7:
	// Congruent Triangles
	proving = "SELECT * FROM Consequent INNER JOIN CongruentTriangles USING(typeGeoCmd) WHERE Consequent.point1 = CongruentTriangles.point1 and Consequent.point2 = CongruentTriangles.point2 and Consequent.point3 = CongruentTriangles.point3 and Consequent.point4 = CongruentTriangles.point4 and Consequent.point5 = CongruentTriangles.point5 and Consequent.point6 = CongruentTriangles.point6";

	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0) {
	    return false;
	} else {
	    return true;
	}
	break;
    case 8:
	// Cyclic
	proving = "SELECT * FROM Consequent INNER JOIN Cyclic USING(typeGeoCmd) WHERE Consequent.point1 = Cyclic.point1 and Consequent.point2 = Cyclic.point2 and Consequent.point3 = Cyclic.point3 and Consequent.point4 = Cyclic.point4";

	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0) {
	    return false;
	} else {
	    return true;
	}
	break;
    case 9:
	// Equal Angles
	proving = "SELECT * FROM Consequent INNER JOIN EqualAngles USING(typeGeoCmd) WHERE Consequent.point1 = EqualAngles.point1 and Consequent.point2 = EqualAngles.point2 and Consequent.point3 = EqualAngles.point3 and Consequent.point4 = EqualAngles.point4 and Consequent.point5 = EqualAngles.point5 and Consequent.point6 = EqualAngles.point6 and Consequent.point7 = EqualAngles.point7 and Consequent.point8 = EqualAngles.point8";

	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0) {
	    return false;
	} else {
	    return true;
	}
	break;
    case 10:
	// Equal Ratios
	proving = "SELECT * FROM Consequent INNER JOIN EqualRatios USING(typeGeoCmd) WHERE Consequent.point1 = EqualRatios.point1 and Consequent.point2 = EqualRatios.point2 and Consequent.point3 = EqualRatios.point3 and Consequent.point4 = EqualRatios.point4 and Consequent.point5 = EqualRatios.point5 and Consequent.point6 = EqualRatios.point6 and Consequent.point7 = EqualRatios.point7 and Consequent.point8 = EqualRatios.point8";

	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0) {
	    return false;
	} else {
	    return true;
	}
	break;
    case 11:
	// Similar Triangles
	proving = "SELECT * FROM Consequent INNER JOIN SimilarTriangles USING(typeGeoCmd) WHERE Consequent.point1 = SimilarTriangles.point1 and Consequent.point2 = SimilarTriangles.point2 and Consequent.point3 = SimilarTriangles.point3 and Consequent.point4 = SimilarTriangles.point4 and Consequent.point5 = SimilarTriangles.point5 and Consequent.point6 = SimilarTriangles.point6";

	dbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(),
				     &(dbim.stmt), NULL);
	sqlite3_step(dbim.stmt);
	if (sqlite3_data_count(dbim.stmt) == 0) {
	    return false;
	} else {
	    return true;
	}
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
	    fixedPointFact = "SELECT point1, point2, point3 FROM Collinear WHERE oldFact = " + idFact;

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
	    // Save geometric command for the derivation of new facts
	    fixedPointFact = "SELECT point1, point2, point3, point4 FROM Parallel WHERE oldFact = " + idFact;

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
	    // Save geometric command for the derivation of new facts
	    fixedPointFact = "SELECT point1, point2, point3, point4 FROM Perpendicular WHERE oldFact = " + idFact;

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
	    // Save geometric command for the derivation of new facts
	    fixedPointFact = "SELECT point1, point2, point3 FROM Midpoint WHERE oldFact = " + idFact;
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
	    fixedPointFact = "SELECT point1, point2, point3, point4 FROM Circle WHERE oldFact = " + idFact;
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
	    fixedPointFact = "SELECT point1, point2, point3, point4 FROM CongruentSegments WHERE oldFact = " + idFact;
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
	    fixedPointFact = "SELECT point1, point2, point3, point4, point5, point6 FROM CongruentTriangles WHERE oldFact = " + idFact;
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
	    fixedPointFact = "SELECT point1, point2, point3, point4 FROM Cyclic WHERE oldFact = " + idFact;
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
	    fixedPointFact = "SELECT point1, point2, point3, point4, point5, point6, point7, point8 FROM EqualAngles WHERE oldFact = " + idFact;
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
	    fixedPointFact = "SELECT point1, point2, point3, point4, point5, point6, point7, point8 FROM EqualRatios WHERE oldFact = " + idFact;
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
	    fixedPointFact = "SELECT point1, point2, point3, point4, point5, point6 FROM SimilarTriangles WHERE oldFact = " + idFact;
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
