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


#include <iostream>
#include <set>
#include <string>

#include "axioms.hpp"
#include "generate.hpp"
#include "warnerr.hpp"

extern Axiom_list axioms;

int predicate_arity(std::string pred)
{
    if (pred == "circle")
	return 4;
    else if (pred == "coll")
	return 3;
    else if (pred == "cong")
	return 4;
    else if (pred == "contri")
	return 6;
    else if (pred == "cyclic")
	return 4;
    else if (pred == "eqangle")
	return 8;
    else if (pred == "eqratio")
	return 8;
    else if (pred == "midp")
	return 3;
    else if (pred == "para")
	return 4;
    else if (pred == "perp")
	return 4;
    else
	return 6;
}

std::string predicate_table(std::string pred)
{
    if (pred == "circle")
	return "Circle";
    else if (pred == "coll")
	return "Collinear";
    else if (pred == "cong")
	return "CongruentSegments";
    else if (pred == "contri")
	return "CongruentTriangles";
    else if (pred == "cyclic")
	return "Cyclic";
    else if (pred == "eqangle")
	return "EqualAngles";
    else if (pred == "eqratio")
	return "EqualRatios";
    else if (pred == "midp")
	return "Midpoint";
    else if (pred == "para")
	return "Parallel";
    else if (pred == "perp")
	return "Perpendicular";
    else
	return "SimilarTriangles";
}

std::string generate_rules_hpp()
{
    std::string rh = "";
    std::set<std::string> ants;

    for (Axiom ax : axioms) {
	ants = {};
	for (Predicate pred : ax.antecedents)
	    ants.insert(pred.name);
	for (std::string pred_name : ants) {
	    rh = rh + "\tDBinMemory " + ax.name + pred_name + "(DBinMemory";
	    for (int i = 1; i <= predicate_arity(pred_name); i++)
		rh = rh + ", std::string";
	    rh = rh + ");\n";
	}
    }
    return rh;
}

std::string generate_prover_hpp()
{
    std::string ph;

    ph = "#ifndef PROVER\n";
    ph = ph + "#define PROVER\n";
    ph = ph + "\n";
    ph = ph + "#include <string>\n";
    ph = ph + "\n";
    ph = ph + "#include \"dbRAM.hpp\"\n";
    ph = ph + "\n";
    ph = ph + "class DBinMemory;\n";
    ph = ph + "\n";
    ph = ph + "class Prover {\n";
    ph = ph + "private:\n";
    ph = ph + "\tvoid deriveNewColl(std::string,std::string,std::string);\n";
    ph = ph + "\n";
    ph = ph + "public:\n";
    ph = ph + "\tvoid saveFixedPoint(DBinMemory, std::string);\n";
    ph = ph + "\tDBinMemory fixedPoint(DBinMemory,clock_t *);\n";
    ph = ph + "\tbool proved(DBinMemory);\n";
    ph = ph + "\n";
    ph = ph + generate_rules_hpp();
    ph = ph + "\n";
    ph = ph + "\tvoid testDBim(DBinMemory);\n";
    ph = ph + "};\n";
    ph = ph + "\n";
    ph = ph + "#endif\n";
    return ph;
}

int position_point_list(std::string pt,  std::list<std::string> points)
{
    int pos = 0;

    for (std::string p : points) {
	pos++;
	if (p == pt)
	    break;
    }
    return pos;
}

std::string one2one(Axiom ax)
{
    std::string rc;
    
    rc = "DBinMemory Prover::" + ax.name + ax.antecedents.front().name + "(DBinMemory dbim";
    for (int i = 1; i <= predicate_arity(ax.antecedents.front().name); i++)
	rc = rc + ", std::string pt" + std::to_string(i);
    rc = rc + ")\n";
    rc = rc + "{\n";
    rc = rc + "\tbool correctTransaction;\n";
    rc = rc + "\tstd::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;\n";
    rc = rc + "\n";
    rc = rc + "\tinsertNewFact = \"INSERT INTO NewFact (typeGeoCmd) VALUES ('" + ax.consequence.name + "')\";\n";
    rc = rc + "\tlastInsertedRowId = \"SELECT last_insert_rowid()\";\n";
    rc = rc + "\tsqlite3_exec(dbim.db, \"begin;\", 0, 0, &(dbim.zErrMsg));\n";
    rc = rc + "\tcorrectTransaction = true;\n";
    rc = rc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertNewFact.c_str(), insertNewFact.size(), &(dbim.stmt), NULL);\n";
    rc = rc + "\tif (sqlite3_step(dbim.stmt) != SQLITE_DONE)\n";
    rc = rc + "\t\tcorrectTransaction = false;\n";
    rc = rc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(), lastInsertedRowId.size(), &(dbim.stmt), NULL);\n";
    rc = rc + "\tsqlite3_step(dbim.stmt);\n";
    rc = rc + "\tlstInsRwId = (char*) sqlite3_column_text(dbim.stmt, 0);\n";
    rc = rc + "\tinsertionPred = \"INSERT INTO " + predicate_table(ax.consequence.name) + " (typeGeoCmd, ";
    for (int i = 1; i <= predicate_arity(ax.consequence.name); i++)
	rc = rc + "point" + std::to_string(i) + ", ";
    rc = rc + "newFact) VALUES ('" + ax.consequence.name + "', '\" + ";
    for (std::string pt : ax.consequence.points)
	rc = rc + "pt" + std::to_string(position_point_list(pt, ax.antecedents.front().points)) + " + \"', '\" + ";
    rc = rc + "lstInsRwId + \"')\";\n";
    rc = rc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(), insertionPred.size(), &(dbim.stmt), NULL);\n";
    rc = rc + "\tif (sqlite3_step(dbim.stmt) != SQLITE_DONE)\n";
    rc = rc + "\t\tcorrectTransaction = false;\n";
    rc = rc + "\tif (correctTransaction) {\n";
    rc = rc + "\t\tsqlite3_exec(dbim.db, \"commit;\", 0, 0, 0);\n";
    rc = rc + "\t} else {\n";
    rc = rc + "\t\tsqlite3_exec(dbim.db, \"rollback;\", 0, 0, 0);\n";
    rc = rc + "\t}\n";
    rc = rc + "\treturn dbim;\n";
    rc = rc + "}\n";
    return rc;
}

std::string generate_rules_cpp()
{
    std::string rc = "";
    std::set<std::string> ants;

    for (Axiom ax : axioms) {
	rc = rc + "\n";
	// std::cout << "Axiom: " << ax.name << std::endl;
	ants = {};
	for (Predicate pred : ax.antecedents)
	    ants.insert(pred.name);
	switch (ants.size()) {
	case 1:
	    rc = rc + one2one(ax);
	    break;
	default:
	    error(ERROR_EXCESS_PREDICATES, std::to_string(ants.size()));
	    break;
	}
    }
    return rc;
}

std::string use_rules_for_pred(std::string pred)
{
    std::string pc = "";
    std::set<std::string> ants;

    for (Axiom ax : axioms) {
	ants = {};
	for (Predicate pred : ax.antecedents)
	    ants.insert(pred.name);
	if (ants.find(pred) != ants.end()) {
	    pc = pc + "\t\t\t\tdbim = " + ax.name + pred + "(dbim";
	    for (int i = 1; i <= predicate_arity(pred); i++)
		pc = pc + ", point" + std::to_string(i);
	    pc = pc + ");\n";
	}
    }
    return pc;
}

std::string generate_prover_cpp()
{
    std::string pc;
    std::list<std::string> rules;

    pc = "#include <algorithm>\n";
    pc = pc + "#include <fstream>\n";
    pc = pc + "#include <iostream>\n";
    pc = pc + "#include <map>\n";
    pc = pc + "#include <vector>\n";
    pc = pc + "\n";
    pc = pc + "#include \"prover.hpp\"\n";
    pc = pc + "#include \"strs.hpp\"\n";
    pc = pc + "\n";
    pc = pc + "#include \"foftodb.hpp\"  // To show the BD\n";
    pc = pc + "\n";
    pc = pc + "extern struct strsList *points;\n";
    pc = pc + "extern struct strsList *ndg;\n";
    pc = pc + generate_rules_cpp();
    pc = pc + "\n";
    pc = pc + "DBinMemory Prover::fixedPoint(DBinMemory dbim, clock_t *proved_t)\n";
    pc = pc + "{\n";
    pc = pc + "\tint nrColl, nrPara, nrPerp, nrMidp, nrCircle, nrCong;\n";
    pc = pc + "\tint nrContri, nrCyclic, nrEqangle, nrEqratio, nrSimtri;\n";
    pc = pc + "\tbool correctTransaction;\n";
    pc = pc + "\tstd::string deleteNewFact, insertFact, newFact, newFactId;\n";
    pc = pc + "\tstd::string toDeriveGeoCmd, typeGeoCmd, updateGeoCmd;\n";
    pc = pc + "\tstd::string point1, point2, point3, point4, point5, point6, point7, point8;\n";
    pc = pc + "\tFOFtoDB fdb;\n";
    pc = pc + "\tint notproved;\n";
    pc = pc + "\n";
    pc = pc + "\tnrColl = 0;\n";
    pc = pc + "\tnrPara = 0;\n";
    pc = pc + "\tnrPerp = 0;\n";
    pc = pc + "\tnrMidp = 0;\n";
    pc = pc + "\tnrCircle = 0;\n";
    pc = pc + "\tnrCong = 0;\n";
    pc = pc + "\tnrContri = 0;\n";
    pc = pc + "\tnrCyclic = 0;\n";
    pc = pc + "\tnrEqangle = 0;\n";
    pc = pc + "\tnrEqratio = 0;\n";
    pc = pc + "\tnrSimtri = 0;\n";
    pc = pc + "\tsqlite3_prepare_v2(dbim.db, \"PRAGMA foreign_keys = ON\", -1, &(dbim.stmt), NULL);\n";
    pc = pc + "\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\tnewFact = \"SELECT id, typeGeoCmd FROM NewFact LIMIT 1\";\n";
    pc = pc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, newFact.c_str() , newFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\twhile (sqlite3_step(dbim.stmt) != SQLITE_DONE) {\n";
    pc = pc + "\t\tnewFactId = (char*) sqlite3_column_text(dbim.stmt, 0);\n";
    pc = pc + "\t\ttypeGeoCmd = (char*) sqlite3_column_text(dbim.stmt, 1);\n";
    pc = pc + "\t\tswitch (dbim.geoCmds[typeGeoCmd]) {\n";
    pc = pc + "\t\tcase 1:\n";
    pc = pc + "\t\t\tnrColl++;\n";
    pc = pc + "\t\t\ttoDeriveGeoCmd = \"SELECT point1, point2, point3 FROM Collinear WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(), toDeriveGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt, 2);\n";
    pc = pc + "\t\t\tinsertFact = \"INSERT INTO Facts (id, typeGeoCmd) VALUES (\" + newFactId + \",'\" + typeGeoCmd + \"')\";\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(), insertFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tupdateGeoCmd = \"UPDATE Collinear SET oldFact = \" + newFactId + \" WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(), updateGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tdeleteNewFact = \"DELETE FROM NewFact where id = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(), deleteNewFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 2:\n";
    pc = pc + "\t\t\tnrPara++;\n";
    pc = pc + "\t\t\ttoDeriveGeoCmd = \"SELECT point1, point2, point3, point4 FROM Parallel WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(), toDeriveGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt, 2);\n";
    pc = pc + "\t\t\tpoint4 = (char*) sqlite3_column_text(dbim.stmt, 3);\n";
    pc = pc + "\t\t\tinsertFact = \"INSERT INTO Facts (id, typeGeoCmd) VALUES (\" + newFactId + \",'\" + typeGeoCmd + \"')\";\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(), insertFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tupdateGeoCmd = \"UPDATE Parallel SET oldFact = \" + newFactId + \" WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(), updateGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tdeleteNewFact = \"DELETE FROM NewFact where id = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(), deleteNewFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 3:\n";
    pc = pc + "\t\t\tnrPerp++;\n";
    pc = pc + "\t\t\ttoDeriveGeoCmd = \"SELECT point1, point2, point3, point4 FROM Perpendicular WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(), toDeriveGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt, 2);\n";
    pc = pc + "\t\t\tpoint4 = (char*) sqlite3_column_text(dbim.stmt, 3);\n";
    pc = pc + "\t\t\tinsertFact = \"INSERT INTO Facts (id, typeGeoCmd) VALUES (\" + newFactId + \",'\" + typeGeoCmd + \"')\";\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(), insertFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tupdateGeoCmd = \"UPDATE Perpendicular SET oldFact = \" + newFactId + \" WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(), updateGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tdeleteNewFact = \"DELETE FROM NewFact where id = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(), deleteNewFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 4:\n";
    pc = pc + "\t\t\tnrMidp ++;\n";
    pc = pc + "\t\t\ttoDeriveGeoCmd = \"SELECT point1, point2, point3 FROM Midpoint WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(), toDeriveGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt, 2);\n";
    pc = pc + "\t\t\tinsertFact = \"INSERT INTO Facts (id, typeGeoCmd) VALUES (\" + newFactId + \",'\" + typeGeoCmd + \"')\";\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(), insertFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tupdateGeoCmd = \"UPDATE Midpoint SET oldFact = \" + newFactId + \" WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(), updateGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tdeleteNewFact = \"DELETE FROM NewFact where id = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(), deleteNewFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 5:\n";
    pc = pc + "\t\t\tnrCircle++;\n";
    pc = pc + "\t\t\ttoDeriveGeoCmd = \"SELECT point1, point2, point3, point4 FROM Circle WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(), toDeriveGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt, 2);\n";
    pc = pc + "\t\t\tpoint4 = (char*) sqlite3_column_text(dbim.stmt, 3);\n";
    pc = pc + "\t\t\tinsertFact = \"INSERT INTO Facts (id, typeGeoCmd) VALUES (\" + newFactId + \",'\" + typeGeoCmd + \"')\";\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(), insertFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tupdateGeoCmd = \"UPDATE Circle SET oldFact = \" + newFactId + \" WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(), updateGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tdeleteNewFact = \"DELETE FROM NewFact where id = \" + newFactId;\n";
											 pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(), deleteNewFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 6:\n";
    pc = pc + "\t\t\tnrCong++;\n";
    pc = pc + "\t\t\ttoDeriveGeoCmd = \"SELECT point1, point2, point3, point4 FROM CongruentSegments WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(), toDeriveGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt, 2);\n";
    pc = pc + "\t\t\tpoint4 = (char*) sqlite3_column_text(dbim.stmt, 3);\n";
    pc = pc + "\t\t\tinsertFact = \"INSERT INTO Facts (id, typeGeoCmd) VALUES (\" + newFactId + \",'\" + typeGeoCmd + \"')\";\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(), insertFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tupdateGeoCmd = \"UPDATE CongruentSegments SET oldFact = \" + newFactId + \" WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(), updateGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tdeleteNewFact = \"DELETE FROM NewFact where id = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(), deleteNewFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 7:\n";
    pc = pc + "\t\t\tnrContri++;\n";
    pc = pc + "\t\t\ttoDeriveGeoCmd = \"SELECT point1, point2, point3, point4, point5, point6 FROM CongruentTriangles WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(), toDeriveGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt, 2);\n";
    pc = pc + "\t\t\tpoint4 = (char*) sqlite3_column_text(dbim.stmt, 3);\n";
    pc = pc + "\t\t\tpoint5 = (char*) sqlite3_column_text(dbim.stmt, 4);\n";
    pc = pc + "\t\t\tpoint6 = (char*) sqlite3_column_text(dbim.stmt, 5);\n";
    pc = pc + "\t\t\tinsertFact = \"INSERT INTO Facts (id, typeGeoCmd) VALUES (\" + newFactId + \",'\" + typeGeoCmd + \"')\";\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(), insertFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tupdateGeoCmd = \"UPDATE CongruentTriangles SET oldFact = \" + newFactId + \" WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(), updateGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tdeleteNewFact = \"DELETE FROM NewFact where id = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(), deleteNewFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 8:\n";
    pc = pc + "\t\t\tnrCyclic++;\n";
    pc = pc + "\t\t\ttoDeriveGeoCmd = \"SELECT point1, point2, point3, point4 FROM Cyclic WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(), toDeriveGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt, 2);\n";
    pc = pc + "\t\t\tpoint4 = (char*) sqlite3_column_text(dbim.stmt, 3);\n";
    pc = pc + "\t\t\tinsertFact = \"INSERT INTO Facts (id, typeGeoCmd) VALUES (\" + newFactId + \",'\" + typeGeoCmd + \"')\";\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(), insertFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tupdateGeoCmd = \"UPDATE Cyclic SET oldFact = \" + newFactId + \" WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(), updateGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tdeleteNewFact = \"DELETE FROM NewFact where id = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(), deleteNewFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 9:\n";
    pc = pc + "\t\t\tnrEqangle++;\n";
    pc = pc + "\t\t\ttoDeriveGeoCmd = \"SELECT point1, point2, point3, point4, point5, point6, point7, point8 FROM EqualAngles WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(), toDeriveGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt, 2);\n";
    pc = pc + "\t\t\tpoint4 = (char*) sqlite3_column_text(dbim.stmt, 3);\n";
    pc = pc + "\t\t\tpoint5 = (char*) sqlite3_column_text(dbim.stmt, 4);\n";
    pc = pc + "\t\t\tpoint6 = (char*) sqlite3_column_text(dbim.stmt, 5);\n";
    pc = pc + "\t\t\tpoint7 = (char*) sqlite3_column_text(dbim.stmt, 6);\n";
    pc = pc + "\t\t\tpoint8 = (char*) sqlite3_column_text(dbim.stmt, 7);\n";
    pc = pc + "\t\t\tinsertFact = \"INSERT INTO Facts (id, typeGeoCmd) VALUES (\" + newFactId + \",'\" + typeGeoCmd + \"')\";\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(), insertFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tupdateGeoCmd = \"UPDATE EqualAngles SET oldFact = \" + newFactId + \" WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(), updateGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tdeleteNewFact = \"DELETE FROM NewFact where id = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(), deleteNewFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 10:\n";
    pc = pc + "\t\t\tnrEqratio++;\n";
    pc = pc + "\t\t\ttoDeriveGeoCmd = \"SELECT point1, point2, point3, point4, point5, point6, point7, point8 FROM EqualRatios WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(), toDeriveGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt, 2);\n";
    pc = pc + "\t\t\tpoint4 = (char*) sqlite3_column_text(dbim.stmt, 3);\n";
    pc = pc + "\t\t\tpoint5 = (char*) sqlite3_column_text(dbim.stmt, 4);\n";
    pc = pc + "\t\t\tpoint6 = (char*) sqlite3_column_text(dbim.stmt, 5);\n";
    pc = pc + "\t\t\tpoint7 = (char*) sqlite3_column_text(dbim.stmt, 6);\n";
    pc = pc + "\t\t\tpoint8 = (char*) sqlite3_column_text(dbim.stmt, 7);\n";
    pc = pc + "\t\t\tinsertFact = \"INSERT INTO Facts (id, typeGeoCmd) VALUES (\" + newFactId + \",'\" + typeGeoCmd + \"')\";\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(), insertFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tupdateGeoCmd = \"UPDATE EqualRatios SET oldFact = \" + newFactId + \" WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(), updateGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tdeleteNewFact = \"DELETE FROM NewFact where id = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(), deleteNewFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 11:\n";
    pc = pc + "\t\t\tnrSimtri++;\n";
    pc = pc + "\t\t\ttoDeriveGeoCmd = \"SELECT point1, point2, point3, point4, point5, point6 FROM SimilarTriangles WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, toDeriveGeoCmd.c_str(), toDeriveGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt, 2);\n";
    pc = pc + "\t\t\tpoint4 = (char*) sqlite3_column_text(dbim.stmt, 3);\n";
    pc = pc + "\t\t\tpoint5 = (char*) sqlite3_column_text(dbim.stmt, 4);\n";
    pc = pc + "\t\t\tpoint6 = (char*) sqlite3_column_text(dbim.stmt, 5);\n";
    pc = pc + "\t\t\tinsertFact = \"INSERT INTO Facts (id, typeGeoCmd) VALUES (\" + newFactId + \",'\" + typeGeoCmd + \"')\";\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertFact.c_str(), insertFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tupdateGeoCmd = \"UPDATE SimilarTriangles SET oldFact = \" + newFactId + \" WHERE newFact = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, updateGeoCmd.c_str(), updateGeoCmd.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tdeleteNewFact = \"DELETE FROM NewFact where id = \" + newFactId;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, deleteNewFact.c_str(), deleteNewFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tdefault:\n";
    pc = pc + "\t\t\tstd::cerr << \"Unknown geometric command!!!\" << std::endl;\n";
    pc = pc + "\t\t\texit(1);\n";
    pc = pc + "\t\t}\n";
    pc = pc + "\t\tswitch (dbim.geoCmds[typeGeoCmd]) {\n";
    pc = pc + "\t\tcase 1:\n";
    pc = pc + "\t\t\tif (point1 != point2 && point1 != point3 && point2 != point3) {\n";
    pc = pc + use_rules_for_pred("coll");
    pc = pc + "\t\t\t}\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 2:\n";
    pc = pc + "\t\t\tif (point1 != point2 && point3 != point4 && (point1 != point3 || point2 != point4) && (point1 != point4 || point2 != point3)) {\n";
    pc = pc + use_rules_for_pred("para");
    pc = pc + "\t\t\t}\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 3:\n";
    pc = pc + "\t\t\tif (point1 != point2 && point3 != point4 && (point1 != point3 || point2 != point4) && (point1 != point4 || point2 != point3)) {\n";
    pc = pc + use_rules_for_pred("perp");
    pc = pc + "\t\t\t}\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 4:\n";
    pc = pc + "\t\t\tif (point1 != point2 && point1 != point3 && point2 != point3) {\n";
    pc = pc + use_rules_for_pred("midp");
    pc = pc + "\t\t\t}\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 5:\n";
    pc = pc + "\t\t\tif (point1 != point2 && point1 != point3 && point1 != point4 && point2 != point3 && point2 != point4 && point3 != point4) {\n";
    pc = pc + use_rules_for_pred("circle");
    pc = pc + "\t\t\t}\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 6:\n";
    pc = pc + "\t\t\tif (point1 != point2 && point3 != point4 && (point1 != point3 || point2 != point4) && (point1 != point4 || point2 != point3)) {\n";
    pc = pc + use_rules_for_pred("cong");
    pc = pc + "\t\t\t}\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 7:\n";
    pc = pc + "\t\t\tif (point1 != point2 && point1 != point3 && point2 != point3 && point4 != point5 && point4 != point6 && point5 != point6 && (point1 != point4 || point2 != point5 || point3 != point6)) {\n";
    pc = pc + use_rules_for_pred("contri");
    pc = pc + "\t\t\t}\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 8:\n";
    pc = pc + "\t\t\tif (point1 != point2 && point1 != point3 && point1 != point4 && point2 != point3 && point2 != point4 && point3 != point4) {\n";
    pc = pc + use_rules_for_pred("cyclic");
    pc = pc + "\t\t\t}\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 9:\n";
    pc = pc + "\t\t\tif (point1 != point2 && point3 != point4 && point5 != point6 && point7 != point8 && (point1 != point3 || point2 != point4) && (point1 != point4 || point2 != point3) && (point5 != point7 || point6 != point8) && (point5 != point8 || point6 != point7)) {\n";
    pc = pc + use_rules_for_pred("eqangle");
    pc = pc + "\t\t\t}\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 10:\n";
    pc = pc + "\t\t\tif (point1 != point2 && point3 != point4 && point5 != point6 && point7 != point8 && (point1 != point5 || point2 != point6 || point3 != point7 || point4 != point8) && (point1 != point5 || point2 != point6 || point3 != point8 || point4 != point7) && (point1 != point6 || point2 != point5 || point3 != point7 || point4 != point8) && (point1 != point6 || point2 != point5 || point3 != point8 || point4 != point7)) {\n";
    pc = pc + use_rules_for_pred("eqratio");
    pc = pc + "\t\t\t}\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 11:\n";
    pc = pc + "\t\t\tif (point1 != point2 && point1 != point3 && point2 != point3 && point4 != point5 && point4 != point6 && point5 != point6 && (point1 != point4 || point2 != point5 || point3 != point6)) {\n";
    pc = pc + use_rules_for_pred("simtri");
    pc = pc + "\t\t\t}\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tdefault:\n";
    pc = pc + "\t\t\tstd::cerr << \"Unknown geometric command!!!\" << std::endl;\n";
    pc = pc + "\t\t\texit(1);\n";
    pc = pc + "\t\t}\n";
    pc = pc + "\t\tif (notproved && proved(dbim)) {\n";
    pc = pc + "\t\t\t(*proved_t) = clock();\n";
    pc = pc + "\t\t\tnotproved = 0;\n";
    pc = pc + "\t\t}\n";
    pc = pc + "\t\tnewFact = \"SELECT id, typeGeoCmd FROM NewFact LIMIT 1\";\n";
    pc = pc + "\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, newFact.c_str(), newFact.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t}\n";
    pc = pc + "\treturn dbim;\n";
    pc = pc + "}\n";
    pc = pc + "\n";
    pc = pc + "bool Prover::proved(DBinMemory dbim) {\n";
    pc = pc + "\tstd::string idConsequent;\n";
    pc = pc + "\tstd::string proving;\n";
    pc = pc + "\tstd::string sqlConsequent;\n";
    pc = pc + "\tstd::string typeGeoCmd;\n";
    pc = pc + "\t\n";
    pc = pc + "\tsqlConsequent = \"SELECT id, typeGeoCmd FROM Consequent\";\n";
    pc = pc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, sqlConsequent.c_str(), sqlConsequent.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\tidConsequent = (char*) sqlite3_column_text(dbim.stmt, 0);\n";
    pc = pc + "\ttypeGeoCmd = (char*) sqlite3_column_text(dbim.stmt, 1);\n";
    pc = pc + "\tswitch (dbim.geoCmds[typeGeoCmd]) {\n";
    pc = pc + "\tcase 1:\n";
    pc = pc + "\t\tproving = \"SELECT * FROM Consequent INNER JOIN Collinear USING (typeGeoCmd) WHERE Consequent.point1 = Collinear.point1 AND Consequent.point2 = Collinear.point2 AND Consequent.point3 = Collinear.point3\";\n";
    pc = pc + "\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\tif (sqlite3_data_count(dbim.stmt) == 0)\n";
    pc = pc + "\t\t\treturn false;\n";
    pc = pc + "\t\telse\n";
    pc = pc + "\t\t\treturn true;\n";
    pc = pc + "\t\tbreak;\n";
    pc = pc + "\tcase 2:\n";
    pc = pc + "\t\tproving = \"SELECT * FROM Consequent INNER JOIN Parallel USING (typeGeoCmd) WHERE Consequent.point1 = Parallel.point1 AND Consequent.point2 = Parallel.point2 AND Consequent.point3 = Parallel.point3 AND Consequent.point4 = Parallel.point4\";\n";
    pc = pc + "\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\tif (sqlite3_data_count(dbim.stmt) == 0)\n";
    pc = pc + "\t\t\treturn false;\n";
    pc = pc + "\t\telse\n";
    pc = pc + "\t\t\treturn true;\n";
    pc = pc + "\t\tbreak;\n";
    pc = pc + "\tcase 3:\n";
    pc = pc + "\t\tproving = \"SELECT * FROM Consequent INNER JOIN Perpendicular USING (typeGeoCmd) WHERE Consequent.point1 = Perpendicular.point1 AND Consequent.point2 = Perpendicular.point2 AND Consequent.point3 = Perpendicular.point3 AND Consequent.point4 = Perpendicular.point4\";\n";
    pc = pc + "\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\tif (sqlite3_data_count(dbim.stmt) == 0)\n";
    pc = pc + "\t\t\treturn false;\n";
    pc = pc + "\t\telse\n";
    pc = pc + "\t\t\treturn true;\n";
    pc = pc + "\t\tbreak;\n";
    pc = pc + "\tcase 4:\n";
    pc = pc + "\t\tproving = \"SELECT * FROM Consequent INNER JOIN Midpoint USING (typeGeoCmd) WHERE Consequent.point1 = Midpoint.point1 AND Consequent.point2 = Midpoint.point2 AND Consequent.point3 = Midpoint.point3\";\n";
    pc = pc + "\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\tif (sqlite3_data_count(dbim.stmt) == 0)\n";
    pc = pc + "\t\t\treturn false;\n";
    pc = pc + "\t\telse\n";
    pc = pc + "\t\t\treturn true;\n";
    pc = pc + "\t\tbreak;\n";
    pc = pc + "\tcase 5:\n";
    pc = pc + "\t\tproving = \"SELECT * FROM Consequent INNER JOIN Circle USING (typeGeoCmd) WHERE Consequent.point1 = Circle.point1 AND Consequent.point2 = Circle.point2 AND Consequent.point3 = Circle.point3 AND Consequent.point4 = Circle.point4\";\n";
    pc = pc + "\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\tif (sqlite3_data_count(dbim.stmt) == 0)\n";
    pc = pc + "\t\t\treturn false;\n";
    pc = pc + "\t\telse\n";
    pc = pc + "\t\t\treturn true;\n";
    pc = pc + "\t\tbreak;\n";
    pc = pc + "\tcase 6:\n";
    pc = pc + "\t\tproving = \"SELECT * FROM Consequent INNER JOIN CongruentSegments USING (typeGeoCmd) WHERE Consequent.point1 = CongruentSegments.point1 AND Consequent.point2 = CongruentSegments.point2 AND Consequent.point3 = CongruentSegments.point3 AND Consequent.point4 = CongruentSegments.point4\";\n";
    pc = pc + "\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\tif (sqlite3_data_count(dbim.stmt) == 0)\n";
    pc = pc + "\t\t\treturn false;\n";
    pc = pc + "\t\telse\n";
    pc = pc + "\t\t\treturn true;\n";
    pc = pc + "\t\tbreak;\n";
    pc = pc + "\tcase 7:\n";
    pc = pc + "\t\tproving = \"SELECT * FROM Consequent INNER JOIN CongruentTriangles USING (typeGeoCmd) WHERE Consequent.point1 = CongruentTriangles.point1 AND Consequent.point2 = CongruentTriangles.point2 AND Consequent.point3 = CongruentTriangles.point3 AND Consequent.point4 = CongruentTriangles.point4 AND Consequent.point5 = CongruentTriangles.point5 AND Consequent.point6 = CongruentTriangles.point6\";\n";
    pc = pc + "\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\tif (sqlite3_data_count(dbim.stmt) == 0)\n";
    pc = pc + "\t\t\treturn false;\n";
    pc = pc + "\t\telse\n";
    pc = pc + "\t\t\treturn true;\n";
    pc = pc + "\t\tbreak;\n";
    pc = pc + "\tcase 8:\n";
    pc = pc + "\t\tproving = \"SELECT * FROM Consequent INNER JOIN Cyclic USING (typeGeoCmd) WHERE Consequent.point1 = Cyclic.point1 AND Consequent.point2 = Cyclic.point2 AND Consequent.point3 = Cyclic.point3 AND Consequent.point4 = Cyclic.point4\";\n";
    pc = pc + "\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\tif (sqlite3_data_count(dbim.stmt) == 0)\n";
    pc = pc + "\t\t\treturn false;\n";
    pc = pc + "\t\telse\n";
    pc = pc + "\t\t\treturn true;\n";
    pc = pc + "\t\tbreak;\n";
    pc = pc + "\tcase 9:\n";
    pc = pc + "\t\tproving = \"SELECT * FROM Consequent INNER JOIN EqualAngles USING (typeGeoCmd) WHERE Consequent.point1 = EqualAngles.point1 AND Consequent.point2 = EqualAngles.point2 AND Consequent.point3 = EqualAngles.point3 AND Consequent.point4 = EqualAngles.point4 AND Consequent.point5 = EqualAngles.point5 AND Consequent.point6 = EqualAngles.point6 AND Consequent.point7 = EqualAngles.point7 AND Consequent.point8 = EqualAngles.point8\";\n";
    pc = pc + "\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\tif (sqlite3_data_count(dbim.stmt) == 0)\n";
    pc = pc + "\t\t\treturn false;\n";
    pc = pc + "\t\telse\n";
    pc = pc + "\t\t\treturn true;\n";
    pc = pc + "\t\tbreak;\n";
    pc = pc + "\tcase 10:\n";
    pc = pc + "\t\tproving = \"SELECT * FROM Consequent INNER JOIN EqualRatios USING (typeGeoCmd) WHERE Consequent.point1 = EqualRatios.point1 AND Consequent.point2 = EqualRatios.point2 AND Consequent.point3 = EqualRatios.point3 AND Consequent.point4 = EqualRatios.point4 AND Consequent.point5 = EqualRatios.point5 AND Consequent.point6 = EqualRatios.point6 AND Consequent.point7 = EqualRatios.point7 AND Consequent.point8 = EqualRatios.point8\";\n";
    pc = pc + "\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\tif (sqlite3_data_count(dbim.stmt) == 0)\n";
    pc = pc + "\t\t\treturn false;\n";
    pc = pc + "\t\telse\n";
    pc = pc + "\t\t\treturn true;\n";
    pc = pc + "\t\tbreak;\n";
    pc = pc + "\tcase 11:\n";
    pc = pc + "\t\tproving = \"SELECT * FROM Consequent INNER JOIN SimilarTriangles USING (typeGeoCmd) WHERE Consequent.point1 = SimilarTriangles.point1 AND Consequent.point2 = SimilarTriangles.point2 AND Consequent.point3 = SimilarTriangles.point3 AND Consequent.point4 = SimilarTriangles.point4 AND Consequent.point5 = SimilarTriangles.point5 AND Consequent.point6 = SimilarTriangles.point6\";\n";
    pc = pc + "\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, proving.c_str(), proving.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\t\tsqlite3_step(dbim.stmt);\n";
    pc = pc + "\t\tif (sqlite3_data_count(dbim.stmt) == 0)\n";
    pc = pc + "\t\t\treturn false;\n";
    pc = pc + "\t\telse\n";
    pc = pc + "\t\t\treturn true;\n";
    pc = pc + "\t\tbreak;\n";
    pc = pc + "\tdefault:\n";
    pc = pc + "\t\tstd::cerr << \"Unknown geometric command!!!\" << std::endl;\n";
    pc = pc + "\t\texit(1);\n";
    pc = pc + "\t}\n";
    pc = pc + "}\n";
    pc = pc + "\n";
    pc = pc + "void Prover::saveFixedPoint(DBinMemory dbim, std::string outFile)\n";
    pc = pc + "{\n";
    pc = pc + "\tstd::string sqlFacts, fixedPointFact, idFact, typeGeoCmd;\n";
    pc = pc + "\tstd::string point1, point2, point3, point4, point5, point6, point7, point8;\n";
    pc = pc + "\tstruct strsList *current;\n";
    pc = pc + "\tstd::size_t pos;\n";
    pc = pc + "\tstd::ofstream fout;\n";
    pc = pc + "\tsqlite3_stmt *stmt1; \n";
    pc = pc + "\n";
    pc = pc + "\tpos = outFile.find_last_of(\"/\");\n";
    pc = pc + "\tif (pos != std::string::npos)\n";
    pc = pc + "\t\toutFile = outFile.substr(pos + 1);\n";
    pc = pc + "\tpos = outFile.find_last_of(\".\");\n";
    pc = pc + "\tif (pos != std::string::npos)\n";
    pc = pc + "\t\toutFile.replace(outFile.begin() + pos + 1, outFile.end(), \"fp\");\n";
    pc = pc + "\telse\n";
    pc = pc + "\t\toutFile.append(\".fp\");\n";
    pc = pc + "\tfout.open(outFile);\n";
    pc = pc + "\tfout << \"Fixed Point\" << std::endl;\n";
    pc = pc + "\tfout << std::endl;\n";
    pc = pc + "\tsqlFacts = \"SELECT id, typeGeoCmd FROM Facts\";\n";
    pc = pc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, sqlFacts.c_str(), sqlFacts.size(), &(dbim.stmt), NULL);\n";
    pc = pc + "\twhile (sqlite3_step(dbim.stmt) == SQLITE_ROW) {\n";
    pc = pc + "\t\tidFact = (char*) sqlite3_column_text(dbim.stmt, 0);\n";
    pc = pc + "\t\ttypeGeoCmd = (char*) sqlite3_column_text(dbim.stmt, 1);\n";
    pc = pc + "\t\tswitch (dbim.geoCmds[typeGeoCmd]) {\n";
    pc = pc + "\t\tcase 1:\n";
    pc = pc + "\t\t\tfixedPointFact = \"SELECT point1, point2, point3 FROM Collinear WHERE oldFact = \" + idFact;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(), fixedPointFact.size(), &(dbim.stmt1), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt1);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);\n";
    pc = pc + "\t\t\tfout << \"    coll(\";\n";
    pc = pc + "\t\t\tfout << point1 << \", \";\n";
    pc = pc + "\t\t\tfout << point2 << \", \";\n";
    pc = pc + "\t\t\tfout << point3 << \")\" << std::endl;\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 2:\n";
    pc = pc + "\t\t\tfixedPointFact = \"SELECT point1, point2, point3, point4 FROM Parallel WHERE oldFact = \" + idFact;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(), fixedPointFact.size(), &(dbim.stmt1), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt1);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);\n";
    pc = pc + "\t\t\tpoint4 = (char*) sqlite3_column_text(dbim.stmt1, 3);\n";
    pc = pc + "\t\t\tfout << \"    para(\";\n";
    pc = pc + "\t\t\tfout << point1 << \", \";\n";
    pc = pc + "\t\t\tfout << point2 << \", \";\n";
    pc = pc + "\t\t\tfout << point3 << \", \";\n";
    pc = pc + "\t\t\tfout << point4 << \")\" << std::endl;\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 3:\n";
    pc = pc + "\t\t\tfixedPointFact = \"SELECT point1, point2, point3, point4 FROM Perpendicular WHERE oldFact = \" + idFact;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(), fixedPointFact.size(), &(dbim.stmt1), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt1);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);\n";
    pc = pc + "\t\t\tpoint4 = (char*) sqlite3_column_text(dbim.stmt1, 3);\n";
    pc = pc + "\t\t\tfout << \"    perp(\";\n";
    pc = pc + "\t\t\tfout << point1 << \", \";\n";
    pc = pc + "\t\t\tfout << point2 << \", \";\n";
    pc = pc + "\t\t\tfout << point3 << \", \";\n";
    pc = pc + "\t\t\tfout << point4 << \")\" << std::endl;\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 4:\n";
    pc = pc + "\t\t\tfixedPointFact = \"SELECT point1, point2, point3 FROM Midpoint WHERE oldFact = \" + idFact;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(), fixedPointFact.size(), &(dbim.stmt1), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt1);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);\n";
    pc = pc + "\t\t\tfout << \"    midp(\";\n";
    pc = pc + "\t\t\tfout << point1 << \", \";\n";
    pc = pc + "\t\t\tfout << point2 << \", \";\n";
    pc = pc + "\t\t\tfout << point3 << \")\" << std::endl;\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 5:\n";
    pc = pc + "\t\t\tfixedPointFact = \"SELECT point1, point2, point3, point4 FROM Circle WHERE oldFact = \" + idFact;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(), fixedPointFact.size(), &(dbim.stmt1), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt1);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);\n";
    pc = pc + "\t\t\tpoint4 = (char*) sqlite3_column_text(dbim.stmt1, 3);\n";
    pc = pc + "\t\t\tfout << \"    circle(\";\n";
    pc = pc + "\t\t\tfout << point1 << \", \";\n";
    pc = pc + "\t\t\tfout << point2 << \", \";\n";
    pc = pc + "\t\t\tfout << point3 << \", \";\n";
    pc = pc + "\t\t\tfout << point4 << \")\" << std::endl;\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 6:\n";
    pc = pc + "\t\t\tfixedPointFact = \"SELECT point1, point2, point3, point4 FROM CongruentSegments WHERE oldFact = \" + idFact;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(), fixedPointFact.size(), &(dbim.stmt1), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt1);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);\n";
    pc = pc + "\t\t\tpoint4 = (char*) sqlite3_column_text(dbim.stmt1, 3);\n";
    pc = pc + "\t\t\tfout << \"    cong(\";\n";
    pc = pc + "\t\t\tfout << point1 << \", \";\n";
    pc = pc + "\t\t\tfout << point2 << \", \";\n";
    pc = pc + "\t\t\tfout << point3 << \", \";\n";
    pc = pc + "\t\t\tfout << point4 << \")\" << std::endl;\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 7:\n";
    pc = pc + "\t\t\tfixedPointFact = \"SELECT point1, point2, point3, point4, point5, point6 FROM CongruentTriangles WHERE oldFact = \" + idFact;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(), fixedPointFact.size(), &(dbim.stmt1), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt1);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);\n";
    pc = pc + "\t\t\tpoint4 = (char*) sqlite3_column_text(dbim.stmt1, 3);\n";
    pc = pc + "\t\t\tpoint5 = (char*) sqlite3_column_text(dbim.stmt1, 4);\n";
    pc = pc + "\t\t\tpoint6 = (char*) sqlite3_column_text(dbim.stmt1, 5);\n";
    pc = pc + "\t\t\tfout << \"    contri(\";\n";
    pc = pc + "\t\t\tfout << point1 << \", \";\n";
    pc = pc + "\t\t\tfout << point2 << \", \";\n";
    pc = pc + "\t\t\tfout << point3 << \", \";\n";
    pc = pc + "\t\t\tfout << point4 << \", \";\n";
    pc = pc + "\t\t\tfout << point5 << \", \";\n";
    pc = pc + "\t\t\tfout << point6 << \")\" << std::endl;\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 8:\n";
    pc = pc + "\t\t\tfixedPointFact = \"SELECT point1, point2, point3, point4 FROM Cyclic WHERE oldFact = \" + idFact;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(), fixedPointFact.size(), &(dbim.stmt1), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt1);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);\n";
    pc = pc + "\t\t\tpoint4 = (char*) sqlite3_column_text(dbim.stmt1, 3);\n";
    pc = pc + "\t\t\tfout << \"    cyclic(\";\n";
    pc = pc + "\t\t\tfout << point1 << \", \";\n";
    pc = pc + "\t\t\tfout << point2 << \", \";\n";
    pc = pc + "\t\t\tfout << point3 << \", \";\n";
    pc = pc + "\t\t\tfout << point4 << \")\" << std::endl;\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 9:\n";
    pc = pc + "\t\t\tfixedPointFact = \"SELECT point1, point2, point3, point4, point5, point6, point7, point8 FROM EqualAngles WHERE oldFact = \" + idFact;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(), fixedPointFact.size(), &(dbim.stmt1), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt1);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);\n";
    pc = pc + "\t\t\tpoint4 = (char*) sqlite3_column_text(dbim.stmt1, 3);\n";
    pc = pc + "\t\t\tpoint5 = (char*) sqlite3_column_text(dbim.stmt1, 4);\n";
    pc = pc + "\t\t\tpoint6 = (char*) sqlite3_column_text(dbim.stmt1, 5);\n";
    pc = pc + "\t\t\tpoint7 = (char*) sqlite3_column_text(dbim.stmt1, 6);\n";
    pc = pc + "\t\t\tpoint8 = (char*) sqlite3_column_text(dbim.stmt1, 7);\n";
    pc = pc + "\t\t\tfout << \"    eqangle(\";\n";
    pc = pc + "\t\t\tfout << point1 << \", \";\n";
    pc = pc + "\t\t\tfout << point2 << \", \";\n";
    pc = pc + "\t\t\tfout << point3 << \", \";\n";
    pc = pc + "\t\t\tfout << point4 << \", \";\n";
    pc = pc + "\t\t\tfout << point5 << \", \";\n";
    pc = pc + "\t\t\tfout << point6 << \", \";\n";
    pc = pc + "\t\t\tfout << point7 << \", \";\n";
    pc = pc + "\t\t\tfout << point8 << \")\" << std::endl;\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 10:\n";
    pc = pc + "\t\t\tfixedPointFact = \"SELECT point1, point2, point3, point4, point5, point6, point7, point8 FROM EqualRatios WHERE oldFact = \" + idFact;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(), fixedPointFact.size(), &(dbim.stmt1), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt1);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);\n";
    pc = pc + "\t\t\tpoint4 = (char*) sqlite3_column_text(dbim.stmt1, 3);\n";
    pc = pc + "\t\t\tpoint5 = (char*) sqlite3_column_text(dbim.stmt1, 4);\n";
    pc = pc + "\t\t\tpoint6 = (char*) sqlite3_column_text(dbim.stmt1, 5);\n";
    pc = pc + "\t\t\tpoint7 = (char*) sqlite3_column_text(dbim.stmt1, 6);\n";
    pc = pc + "\t\t\tpoint8 = (char*) sqlite3_column_text(dbim.stmt1, 7);\n";
    pc = pc + "\t\t\tfout << \"    eqratio(\";\n";
    pc = pc + "\t\t\tfout << point1 << \", \";\n";
    pc = pc + "\t\t\tfout << point2 << \", \";\n";
    pc = pc + "\t\t\tfout << point3 << \", \";\n";
    pc = pc + "\t\t\tfout << point4 << \", \";\n";
    pc = pc + "\t\t\tfout << point5 << \", \";\n";
    pc = pc + "\t\t\tfout << point6 << \", \";\n";
    pc = pc + "\t\t\tfout << point7 << \", \";\n";
    pc = pc + "\t\t\tfout << point8 << \")\" << std::endl;\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tcase 11:\n";
    pc = pc + "\t\t\tfixedPointFact = \"SELECT point1, point2, point3, point4, point5, point6 FROM SimilarTriangles WHERE oldFact = \" + idFact;\n";
    pc = pc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, fixedPointFact.c_str(), fixedPointFact.size(), &(dbim.stmt1), NULL);\n";
    pc = pc + "\t\t\tsqlite3_step(dbim.stmt1);\n";
    pc = pc + "\t\t\tpoint1 = (char*) sqlite3_column_text(dbim.stmt1, 0);\n";
    pc = pc + "\t\t\tpoint2 = (char*) sqlite3_column_text(dbim.stmt1, 1);\n";
    pc = pc + "\t\t\tpoint3 = (char*) sqlite3_column_text(dbim.stmt1, 2);\n";
    pc = pc + "\t\t\tpoint4 = (char*) sqlite3_column_text(dbim.stmt1, 3);\n";
    pc = pc + "\t\t\tpoint5 = (char*) sqlite3_column_text(dbim.stmt1, 4);\n";
    pc = pc + "\t\t\tpoint6 = (char*) sqlite3_column_text(dbim.stmt1, 5);\n";
    pc = pc + "\t\t\tfout << \"    simtri(\";\n";
    pc = pc + "\t\t\tfout << point1 << \", \";\n";
    pc = pc + "\t\t\tfout << point2 << \", \";\n";
    pc = pc + "\t\t\tfout << point3 << \", \";\n";
    pc = pc + "\t\t\tfout << point4 << \", \";\n";
    pc = pc + "\t\t\tfout << point5 << \", \";\n";
    pc = pc + "\t\t\tfout << point6 << \")\" << std::endl;\n";
    pc = pc + "\t\t\tbreak;\n";
    pc = pc + "\t\tdefault:\n";
    pc = pc + "\t\t\tstd::cerr << \"Unknown geometric command!!!\" << std::endl;\n";
    pc = pc + "\t\t\texit(1);\n";
    pc = pc + "\t\t}\n";
    pc = pc + "\t}\n";
    pc = pc + "\tif (ndg != NULL) {\n";
    pc = pc + "\t\tfout << std::endl;\n";
    pc = pc + "\t\tfout << \"Nondegenerate Conditions\" << std::endl;\n";
    pc = pc + "\t\tfout << std::endl;\n";
    pc = pc + "\t\tcurrent = ndg;\n";
    pc = pc + "\t\twhile (current != NULL) {\n";
    pc = pc + "\t\t\tfout << \"    \" << current->str << std::endl;\n";
    pc = pc + "\t\t\tcurrent = current->next;\n";
    pc = pc + "\t\t}\n";
    pc = pc + "\t}\n";
    pc = pc + "\tfout.close();\n";
    pc = pc + "\tstd::cout << \"Fixed point saved to file '\" << outFile << \"'.\" << std::endl;\n";
    pc = pc + "}\n";
    return pc;
}

std::string generate_makefile()
{
    std::string m;

    m = "BASE = ogpgddm\n";
    m = m + "CPP = g++\n";
    m = m + "CPPFLAGS = \n";
    m = m + "CXX = g++\n";
    m = m + "FLEX = flex\n";
    m = m + "FLEXFLAGS =\n";
    m = m + "BISON = bison\n";
    m = m + "BISONFLAGS = -Wcounterexamples\n";
    m = m + "INSTALLDIR = /usr/local/bin\n";
    m = m + "\n";
    m = m + ".PHONY : clean all\n";
    m = m + "\n";
    m = m + "all: $(BASE)\n";
    m = m + "\n";
    m = m + "%.cpp: %.ll\n";
    m = m + "\t$(FLEX) $(FLEXFLAGS) -o $@ $<\n";
    m = m + "\n";
    m = m + "%.cpp %.hpp: %.yy\n";
    m = m + "\t$(BISON) $(BISONFLAGS) -o $*.cpp $<\n";
    m = m + "\n";
    m = m + "%.o: %.cpp %.hpp\n";
    m = m + "\t$(CPP) $(CPPFLAGS) -c -o $@ $<\n";
    m = m + "\n";
    m = m + "\n";
    m = m + "$(BASE): $(BASE).o scanner.o parser.o dbRAM.o foftodb.o prover.o strs.o\n";
    m = m + "\t$(CPP) -o $@ ogpgddm.cpp foftodb.o parser.o scanner.o prover.o dbRAM.o strs.o -lsqlite3\n";
    m = m + "\n";
    m = m + "$(BASE).o: parser.hpp\n";
    m = m + "parser.o: parser.hpp strs.hpp strs.cpp\n";
    m = m + "scanner.o: parser.hpp\n";
    m = m + "\n";
    m = m + "CLEANFILES = $(BASE) *.o parser.hpp parser.cpp location.hh scanner.cpp\n";
    m = m + "\n";
    m = m + "clean:\n";
    m = m + "\trm -f $(CLEANFILES) $(BASE)\n";
    m = m + "\n";
    m = m + "install:\n";
    m = m + "\tcp $(BASE) $(INSTALLDIR)\n";
    m = m + "\n";
    return m;
}

std::string generate_version_hpp()
{
    std::string vh;

    vh = "#ifndef OGPGDDMVERSION\n";
    vh = vh + "#define OGPGDDMVERSION\n";
    vh = vh + "\n";
    vh = vh + "#define VERSION \"* PG Generated\"\n";
    vh = vh + "\n";
    vh = vh + "#endif\n";
    return vh;
}
