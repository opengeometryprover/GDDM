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
#include <map>
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

/*
 * Generation of the declarations of the predicate functions to be
 * included in 'prover.hpp'.
 */
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

/*
 * Converts a list of point to a set of points, thus removing duplicate points.
 */
std::set<std::string> points_list_to_set(std::list<std::string> pl)
{
	std::set<std::string> ps = {};

	for (std::string p : pl)
		ps.insert(p);
	return ps;
}

std::map<std::string, int> point_positions(Predicate pred)
{
	int pos = 0;
	std::map<std::string, int> pt_pos = {};


	for (std::string pt : pred.points) {
		pos++;
		pt_pos[pt] = pos;
	}
	return pt_pos;
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

std::string one_antecedent(Axiom ax)
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
	rc = rc + "\tif (correctTransaction)\n";
	rc = rc + "\t\tsqlite3_exec(dbim.db, \"commit;\", 0, 0, 0);\n";
	rc = rc + "\telse\n";
	rc = rc + "\t\tsqlite3_exec(dbim.db, \"rollback;\", 0, 0, 0);\n";
	rc = rc + "\treturn dbim;\n";
	rc = rc + "}\n";
	return rc;
}

/*
 * Generate rules with two antecedents.
 */
std::string two_antecedents(std::string pn, Axiom ax)
{
	int pos, nr_pt;
	bool first;
	std::string rc, select_pt, where_cond;
	std::set<std::string> pt_pred1;
	std::map<std::string, int> pt_pos;
	std::map<std::string, int> pt_pos_new = {};
	Predicate pred1, pred2;

	if (ax.antecedents.front().name == pn) {
		pred1 = ax.antecedents.front();
		pred2 = ax.antecedents.back();
	} else {
		pred1 = ax.antecedents.back();
		pred2 = ax.antecedents.front();
	}
	pt_pred1 = points_list_to_set(pred1.points);
	nr_pt = 0;
	pos = 0;
	first = true;
	for (std::string pt : pred2.points) {
		pos++;
		if (pt_pred1.count(pt) == 0) {
			nr_pt++;
			pt_pos_new.insert({pt, nr_pt});
			if (first) {
				select_pt = "point" + std::to_string(pos);
				first = false;
			} else
				select_pt = select_pt + ", point"
					+ std::to_string(pos);
		}
	}
	if (nr_pt == 0)
		select_pt = "*";
	pt_pos = point_positions(pred1);
	pos = 0;
	first = true;
	for (std::string pt : pred2.points) {
		pos++;
		if(pt_pred1.count(pt) > 0)
			if (first) {
				where_cond = "point" + std::to_string(pos)
					+ " = '\" + pt"
					+ std::to_string(pt_pos.at(pt));
				first = false;
			} else
				where_cond = where_cond + " + \"' AND point"
					+ std::to_string(pos) + " = '\" + pt"
					+ std::to_string(pt_pos.at(pt));
	}
	where_cond = where_cond + " + \"'\"";
	
	rc = "DBinMemory Prover::" + ax.name + pn + "(DBinMemory dbim";
	for (int i = 1; i <= predicate_arity(pn); i++)
		rc = rc + ", std::string pt" + std::to_string(i);
	rc = rc + ")\n";
	rc = rc + "{\n";
	rc = rc + "\tbool correctTransaction;\n";
	rc = rc + "\tstd::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;\n";
	rc = rc + "\tstd::string querySecondGeoCmdA, querySecondGeoCmdB;\n";
	if (nr_pt > 0) {
		rc = rc + "\tstd::string ";
		for (int i = 1; i <= nr_pt; i++) {
			if (i == 1)
				rc = rc + "newPoint" + std::to_string(i);
			else
				rc = rc + ", newPoint" + std::to_string(i);
		}
		rc = rc + ";\n";
	}
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
	rc = rc + "\tquerySecondGeoCmdA = \"SELECT " + select_pt + " FROM NewFact INNER JOIN " + predicate_table(pred2.name) + " ON (newFact = id) WHERE " + where_cond + ";\n";
	rc = rc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(), querySecondGeoCmdA.size(), &(dbim.stmt1), NULL);\n";
	rc = rc + "\tsqlite3_step(dbim.stmt1);\n";
	rc = rc + "\tquerySecondGeoCmdA = \"SELECT " + select_pt + " FROM Facts INNER JOIN " + predicate_table(pred2.name) + " ON (oldFact = id) WHERE " + where_cond + ";\n";
	rc = rc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, querySecondGeoCmdA.c_str(), querySecondGeoCmdA.size(), &(dbim.stmt2), NULL);\n";
	rc = rc + "\tsqlite3_step(dbim.stmt2);\n";
	rc = rc + "\tif (sqlite3_data_count(dbim.stmt1) == 0 && sqlite3_data_count(dbim.stmt2) == 0 ) {\n";
        rc = rc + "\t\tcorrectTransaction = false;\n";
	rc = rc + "\t} else {\n";
	rc = rc + "\t\tif (sqlite3_data_count(dbim.stmt1) != 0) {\n";
	for (int i = 1; i <= nr_pt; i++) {
		rc = rc + "\t\t\tnewPoint" + std::to_string(i)
			+ " = (char*) sqlite3_column_text(dbim.stmt1, "
			+ std::to_string(i - 1) + ");\n";	
	}
	rc = rc + "\t\t} else {\n";
	for (int i = 1; i <= nr_pt; i++) {
		rc = rc + "\t\t\tnewPoint" + std::to_string(i)
			+ " = (char*) sqlite3_column_text(dbim.stmt2, "
			+ std::to_string(i - 1) + ");\n";	
	}
	rc = rc + "\t\t}\n";
	rc = rc + "\t\tif (sqlite3_step(dbim.stmt) != SQLITE_DONE) {\n";
        rc = rc + "\t\t\tcorrectTransaction = false;\n";
	rc = rc + "\t\t} else {\n";
	rc = rc + "\t\t\tinsertionPred = \"INSERT INTO " + predicate_table(ax.consequence.name) + " (typeGeoCmd, ";
	for (int i = 1; i <= predicate_arity(ax.consequence.name); i++)
		rc = rc + "point" + std::to_string(i) + ", ";
	rc = rc + "newFact) VALUES ('" + ax.consequence.name + "', '\"";
	for (std::string pt : ax.consequence.points) {
		if (pt_pos.find(pt) == pt_pos.end())
			rc = rc + " + newPoint"
				+ std::to_string(pt_pos_new.at(pt));
		else
			rc = rc + " + pt" + std::to_string(pt_pos.at(pt));
		rc = rc + " + \"', '\"";
	}
	rc = rc + " + lstInsRwId + \"')\";\n";
	rc = rc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionPred.c_str(), insertionPred.size(), &(dbim.stmt), NULL);\n";
	rc = rc + "\t\t\tif (sqlite3_step(dbim.stmt) != SQLITE_DONE)\n";
	rc = rc + "\t\t\t\tcorrectTransaction = false;\n";
	rc = rc + "\t\t}\n";
	rc = rc + "\t}\n";
	rc = rc + "\tif (correctTransaction)\n";
	rc = rc + "\t\tsqlite3_exec(dbim.db, \"commit;\", 0, 0, 0);\n";
	rc = rc + "\telse\n";
	rc = rc + "\t\tsqlite3_exec(dbim.db, \"rollback;\", 0, 0, 0);\n";
	rc = rc + "\treturn dbim;\n";
	rc = rc + "}\n";
	return rc;
}

/*
 * Generate rules with three antecedents.
 */
std::string three_antecedents(std::string pn, Axiom ax)
{
	int nr_new_pt, pos;
	std::string code;
	Predicate pred1, pred2, pred3;

	pred2.name = "";
	for (Predicate p : ax.antecedents)
		if (p.name == pn)
			pred1 = p;
		else if (pred2.name.length() == 0)
			pred2 = p;
		else
			pred3 = p;
	

	code = "DBinMemory Prover::" + ax.name + pn + "(DBinMemory dbim";
	for (int i = 1; i <= predicate_arity(pn); i++)
		code = code + ", std::string pt" + std::to_string(i);
	code = code + ")\n";
	code = code + "{\n";
	code = code + "\tbool correctTransaction;\n";
	code = code + "\tstd::string insertionPred, insertNewFact, lastInsertedRowId, lstInsRwId;\n";
	code = code + "\tstd::string querySecondGeoCmdA, querySecondGeoCmdB;\n";
	code = code + "}\n";
	return code;
}

/*
 * Generate the code for the geometric rules/axioms.
 */
std::string generate_rules_cpp()
{
	std::string code = "";
	std::list<std::string> ant_list;
	std::set<std::string> ant_set;

	for (Axiom ax : axioms) {
		ant_list = {};
		ant_set = {};
		for (Predicate pred : ax.antecedents) {
			ant_list.push_back(pred.name);
			ant_set.insert(pred.name);
		}
		switch (ant_list.size()) {
		case 1:
			// Rules/axioms with one antecedent
			code = code + "\n";
			code = code + one_antecedent(ax);
			break;
		case 2:
			// Rules/axioms with two antecedents
			for (std::string pred : ant_set) {
				code = code + "\n";
				code = code + two_antecedents(pred, ax);
			}
			break;
		case 3:
			// Rules/axioms with three antecedents
			for (std::string pred : ant_set) {
				code = code + "\n";
				code = code + three_antecedents(pred, ax);
			}
			break;
		default:
			error(ERROR_EXCESS_PREDICATES,
			      std::to_string(ant_list.size()));
			break;
		}
	}
	return code;
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

std::string generate_dbram_hpp()
{
	std::string dh;

	dh = dh + "#ifndef DBINMEMORY\n";
	dh = dh + "#define DBINMEMORY\n";
	dh = dh + "\n";
	dh = dh + "#include <sqlite3.h>\n";
	dh = dh + "\n";
	dh = dh + "#include <iostream>\n";
	dh = dh + "#include <map>\n";
	dh = dh + "\n";
	dh = dh + "class DBinMemory {\n";
	dh = dh + "\tfriend class Prover;\n";
	dh = dh + "\tfriend class FOFtoDB;\n";
	dh = dh + "protected:\n";
	dh = dh + "\tint rc;\n";
	dh = dh + "\tchar *zErrMsg = 0;\n";
	dh = dh + "\tsqlite3 *db;\n";
	dh = dh + "\tsqlite3_stmt *stmt, *stmt1, *stmt2;   \n";
	dh = dh + "\n";
	dh = dh + "\tint res = 0;\n";
	dh = dh + "\tstd::map<std::string, int> geoCmds = {{\"coll\", 1},\n";
	dh = dh + "\t\t\t\t\t      {\"para\", 2},\n";
	dh = dh + "\t\t\t\t\t      {\"perp\", 3},\n";
	dh = dh + "\t\t\t\t\t      {\"midp\", 4},\n";
	dh = dh + "\t\t\t\t\t      {\"circle\", 5},\n";
	dh = dh + "\t\t\t\t\t      {\"cong\", 6},\n";
	dh = dh + "\t\t\t\t\t      {\"contri\", 7},\n";
	dh = dh + "\t\t\t\t\t      {\"cyclic\", 8},\n";
	dh = dh + "\t\t\t\t\t      {\"eqangle\", 9},\n";
	dh = dh + "\t\t\t\t\t      {\"eqratio\", 10},\n";
	dh = dh + "\t\t\t\t\t      {\"simtri\", 11}};\n";
	dh = dh + "	\n";
	dh = dh + "public:\n";
	dh = dh + "\tvoid openInMemoryDB();\n";
	dh = dh + "\tvoid createDBforGDDM();\n";
	dh = dh + "\tvoid closeDB();\n";
	dh = dh + "\tint backupDb(const char *,void (*f)(int,int));\n";
	dh = dh + "};\n";
	dh = dh + "\n";
	dh = dh + "#endif\n";
	dh = dh + "\n";
	return dh;
}

std::string generate_dbram_cpp()
{
	std::string dc;

	dc = "#include <iostream>\n";
	dc = dc + "#include <string>\n";
	dc = dc + "\n";
	dc = dc + "#include \"dbRAM.hpp\"\n";
	dc = dc + "\n";
	dc = dc + "\n";
	dc = dc + "void DBinMemory::openInMemoryDB() {\n";
	dc = dc + "\trc = sqlite3_open(\":memory:\", &db);\n";
	dc = dc + "\tif (rc) {\n";
	dc = dc + "\t\tstd::cout << \"Can't open database: \" << sqlite3_errmsg(db) << std::endl;\n";
	dc = dc + "\t\texit(2);\n";
	dc = dc + "\t}\n";
	dc = dc + "}\n";
	dc = dc + "\n";
	dc = dc + "void DBinMemory::createDBforGDDM() {\n";
	dc = dc + "\tstd::string createTableNewFact;\n";
	dc = dc + "\tstd::string createTableFacts;\n";
	dc = dc + "\tstd::string createTableConsequent;\n";
	dc = dc + "\tstd::string createTableCollinear;\n";
	dc = dc + "\tstd::string createTableParallel;\n";
	dc = dc + "\tstd::string createTablePerpendicular;\n";
	dc = dc + "\tstd::string createTableMidpoint;\n";
	dc = dc + "\tstd::string createTableCircle;\n";
	dc = dc + "\tstd::string createTableCongruentSegments;\n";
	dc = dc + "\tstd::string createTableCongruentTriangles;\n";
	dc = dc + "\tstd::string createTableCyclic;\n";
	dc = dc + "\tstd::string createTableEqualAngles;\n";
	dc = dc + "\tstd::string createTableEqualRatios;\n";
	dc = dc + "\tstd::string createTableSimilarTriangles;\n";
	dc = dc + "\tstd::string deleteFromSQLiteSequence;\n";
	dc = dc + "\n";
	dc = dc + "\tsqlite3_prepare_v2(db, \"PRAGMA foreign_keys = OFF\", -1, &stmt, NULL);\n";
	dc = dc + "\tsqlite3_step(stmt);\n";
	dc = dc + "  \n";
	dc = dc + "\tcreateTableNewFact = \"CREATE TABLE IF NOT EXISTS 'NewFact' ('id' INTEGER NOT NULL, 'typeGeoCmd' TEXT NOT NULL, PRIMARY KEY ('id' AUTOINCREMENT))\";\n";
	dc = dc + "\tcreateTableFacts = \"CREATE TABLE IF NOT EXISTS 'Facts' ('id' INTEGER NOT NULL, 'typeGeoCmd' TEXT NOT NULL, PRIMARY KEY ('id'))\";\n";
	dc = dc + "\tcreateTableConsequent = \"CREATE TABLE IF NOT EXISTS 'Consequent' ('id' INTEGER NOT NULL, 'typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'point5' TEXT, 'point6' TEXT, 'point7' TEXT, 'point8' TEXT, PRIMARY KEY ('id' AUTOINCREMENT))\";\n";
	dc = dc + "\tcreateTableCollinear = \"CREATE TABLE IF NOT EXISTS 'Collinear' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY ('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY ('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY ('point1', 'point2', 'point3'))\";\n";
	dc = dc + "\tcreateTableParallel = \"CREATE TABLE IF NOT EXISTS 'Parallel' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY ('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY ('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY ('point1', 'point2', 'point3', 'point4'))\";\n";
	dc = dc + "\tcreateTablePerpendicular = \"CREATE TABLE IF NOT EXISTS 'Perpendicular' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY ('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY ('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY ('point1', 'point2', 'point3', 'point4'))\";\n";
	dc = dc + "\tcreateTableMidpoint = \"CREATE TABLE IF NOT EXISTS 'Midpoint' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY ('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY ('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY ('point1', 'point2', 'point3'))\";\n";
	dc = dc + "\tcreateTableCircle = \"CREATE TABLE IF NOT EXISTS 'Circle' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY ('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY ('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY ('point1', 'point2', 'point3', 'point4'))\";\n";
	dc = dc + "\tcreateTableCongruentSegments = \"CREATE TABLE IF NOT EXISTS 'CongruentSegments' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY ('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY ('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY ('point1', 'point2', 'point3', 'point4'))\";\n";
	dc = dc + "\tcreateTableCongruentTriangles = \"CREATE TABLE IF NOT EXISTS 'CongruentTriangles' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'point5' TEXT, 'point6' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY ('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY ('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY ('point1', 'point2', 'point3', 'point4', 'point5', 'point6'))\";\n";
	dc = dc + "\tcreateTableCyclic = \"CREATE TABLE IF NOT EXISTS 'Cyclic' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY ('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY ('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY ('point1', 'point2', 'point3', 'point4'))\";\n";
	dc = dc + "\tcreateTableEqualAngles = \"CREATE TABLE IF NOT EXISTS 'EqualAngles' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'point5' TEXT, 'point6' TEXT, 'point7' TEXT, 'point8' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY ('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY ('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY ('point1', 'point2', 'point3', 'point4', 'point5', 'point6', 'point7', 'point8'))\";\n";
	dc = dc + "\tcreateTableEqualRatios = \"CREATE TABLE IF NOT EXISTS 'EqualRatios' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'point5' TEXT, 'point6' TEXT, 'point7' TEXT, 'point8' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY ('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY ('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY ('point1', 'point2', 'point3', 'point4', 'point5', 'point6', 'point7', 'point8'))\";\n";
	dc = dc + "\tcreateTableSimilarTriangles = \"CREATE TABLE IF NOT EXISTS 'SimilarTriangles' ('typeGeoCmd' TEXT NOT NULL, 'point1' TEXT, 'point2' TEXT, 'point3' TEXT, 'point4' TEXT, 'point5' TEXT, 'point6' TEXT, 'newFact' INTEGER, 'oldFact' INTEGER, FOREIGN KEY ('oldFact') REFERENCES 'Facts'('id'), FOREIGN KEY ('newFact') REFERENCES 'NewFact'('id') ON DELETE SET NULL, PRIMARY KEY ('point1', 'point2', 'point3', 'point4', 'point5', 'point6'))\";\n";
	dc = dc + "\tdeleteFromSQLiteSequence = \"DELETE FROM sqlite_sequence\";\n";
	dc = dc + "\n";
	dc = dc + "\tsqlite3_exec(db, \"begin;\", 0, 0, &zErrMsg);\n";
	dc = dc + "\trc = sqlite3_prepare_v2(db, createTableNewFact.c_str(), createTableNewFact.size(), &stmt, NULL);\n";
	dc = dc + "\tsqlite3_step(stmt);\n";
	dc = dc + "\trc = sqlite3_prepare_v2(db, createTableFacts.c_str(), createTableFacts.size(), &stmt, NULL);\n";
	dc = dc + "\tsqlite3_step(stmt);\n";
	dc = dc + "\trc = sqlite3_prepare_v2(db, createTableConsequent.c_str(), createTableConsequent.size(), &stmt, NULL);\n";
	dc = dc + "\tsqlite3_step(stmt);\n";
	dc = dc + "\trc = sqlite3_prepare_v2(db, createTableCollinear.c_str(), createTableCollinear.size(), &stmt, NULL);\n";
	dc = dc + "\tsqlite3_step(stmt);\n";
	dc = dc + "\trc = sqlite3_prepare_v2(db, createTableParallel.c_str(), createTableParallel.size(), &stmt, NULL);\n";
	dc = dc + "\tsqlite3_step(stmt);\n";
	dc = dc + "\trc = sqlite3_prepare_v2(db, createTablePerpendicular.c_str(), createTablePerpendicular.size(), &stmt, NULL);\n";
	dc = dc + "\tsqlite3_step(stmt);\n";
	dc = dc + "\trc = sqlite3_prepare_v2(db, createTableMidpoint.c_str(), createTableMidpoint.size(), &stmt, NULL);\n";
	dc = dc + "\tsqlite3_step(stmt);\n";
	dc = dc + "\trc = sqlite3_prepare_v2(db, createTableCircle.c_str(), createTableCircle.size(), &stmt, NULL);\n";
	dc = dc + "\tsqlite3_step(stmt);\n";
	dc = dc + "\trc = sqlite3_prepare_v2(db, createTableCongruentSegments.c_str(), createTableCongruentSegments.size(), &stmt, NULL);\n";
	dc = dc + "\tsqlite3_step(stmt);\n";
	dc = dc + "\trc = sqlite3_prepare_v2(db, createTableCongruentTriangles.c_str(), createTableCongruentTriangles.size(), &stmt, NULL);\n";
	dc = dc + "\tsqlite3_step(stmt);\n";
	dc = dc + "\trc = sqlite3_prepare_v2(db, createTableCyclic.c_str(), createTableCyclic.size(), &stmt, NULL);\n";
	dc = dc + "\tsqlite3_step(stmt);\n";
	dc = dc + "\trc = sqlite3_prepare_v2(db, createTableEqualAngles.c_str(), createTableEqualAngles.size(), &stmt, NULL);\n";
	dc = dc + "\tsqlite3_step(stmt);\n";
	dc = dc + "\trc = sqlite3_prepare_v2(db, createTableEqualRatios.c_str(), createTableEqualRatios.size(), &stmt, NULL);\n";
	dc = dc + "\tsqlite3_step(stmt);\n";
	dc = dc + "\trc = sqlite3_prepare_v2(db, createTableSimilarTriangles.c_str(), createTableSimilarTriangles.size(), &stmt, NULL);\n";
	dc = dc + "\tsqlite3_step(stmt);\n";
	dc = dc + "\trc = sqlite3_prepare_v2(db, deleteFromSQLiteSequence.c_str(), deleteFromSQLiteSequence.size(), &stmt, NULL);\n";
	dc = dc + "\tsqlite3_step(stmt);\n";
	dc = dc + "\n";
	dc = dc + "\tsqlite3_exec(db, \"commit;\", 0, 0, 0);\n";
	dc = dc + "}\n";
	dc = dc + "\n";
	dc = dc + "void DBinMemory::closeDB() {\n";
	dc = dc + "\tsqlite3_close(db);\n";
	dc = dc + "}\n";
	dc = dc + "\n";
	dc = dc + "void seeProgress(int remaining, int pagecount){\n";
	dc = dc + "\tdouble completion;\n";
	dc = dc + "\tcompletion =  (double) (pagecount - remaining) / pagecount;\n";
	dc = dc + "\tstd:: cout << \"completion: \" << completion << std::endl;\n";
	dc = dc + "}\n";
	dc = dc + "\n";
	dc = dc + "int DBinMemory::backupDb(const char *zFilename, void(*xProgress)(int, int))\n";
	dc = dc + "{\n";
	dc = dc + "\tint rc;\n";
	dc = dc + "\tsqlite3 *pFile;\n";
	dc = dc + "\tsqlite3_backup *pBackup;\n";
	dc = dc + "\n";
	dc = dc + "\trc = sqlite3_open(zFilename, &pFile);\n";
	dc = dc + "\tif( rc==SQLITE_OK ){\n";
	dc = dc + "\n";
	dc = dc + "\t\tpBackup = sqlite3_backup_init(pFile, \"main\", db, \"main\");\n";
	dc = dc + "\t\tif( pBackup ){\n";
	dc = dc + "\t\t\tdo {\n";
	dc = dc + "\t\t\t\trc = sqlite3_backup_step(pBackup, 5);\n";
	dc = dc + "\t\t\t\txProgress(sqlite3_backup_remaining(pBackup), sqlite3_backup_pagecount(pBackup));\n";
	dc = dc + "\t\t\t\tif( rc==SQLITE_OK || rc==SQLITE_BUSY || rc==SQLITE_LOCKED ){\n";
	dc = dc + "\t\t\t\t\tsqlite3_sleep(250);\n";
	dc = dc + "\t\t\t\t}\n";
	dc = dc + "\t\t\t} while( rc==SQLITE_OK || rc==SQLITE_BUSY || rc==SQLITE_LOCKED );\n";
	dc = dc + "\n";
	dc = dc + "\t\t\t(void)sqlite3_backup_finish(pBackup);\n";
	dc = dc + "\t\t}\n";
	dc = dc + "\t\trc = sqlite3_errcode(pFile);\n";
	dc = dc + "\t}\n";
	dc = dc + "\n";
	dc = dc + "\t(void)sqlite3_close(pFile);\n";
	dc = dc + "\treturn rc;\n";
	dc = dc + "}\n";
	return dc;
}

std::string generate_foftodb_hpp()
{
	std::string fh;

	fh = fh + "#ifndef FOFTODB\n";
	fh = fh + "#define FOFTODB\n";
	fh = fh + "\n";
	fh = fh + "#include <map>\n";
	fh = fh + "\n";
	fh = fh + "#include \"parser.hpp\"\n";
	fh = fh + "\n";
	fh = fh + "\n";
	fh = fh + "class DBinMemory;\n";
	fh = fh + "\n";
	fh = fh + "#define YY_DECL yy::parser::symbol_type yylex (Driver& drv)\n";
	fh = fh + "YY_DECL;\n";
	fh = fh + "\n";
	fh = fh + "class Driver {\n";
	fh = fh + "public:\n";
	fh = fh + "\tDriver ();\n";
	fh = fh + "\n";
	fh = fh + "\tint numGeoCmd = 0;\n";
	fh = fh + "\tint antconcedent[500];\n";
	fh = fh + "\tstd::string typeGeoCmd[500];\n";
	fh = fh + "\tstd::string point1[500], point2[500], point3[500], point4[500];\n";
	fh = fh + "\tstd::string point5[500], point6[500], point7[500], point8[500];\n";
	fh = fh + "\tstd::map<std::string, int> variables;\n";
	fh = fh + "\tint result;\n";
	fh = fh + "\n";
	fh = fh + "\tint parse(const std::string& f);\n";
	fh = fh + "\tstd::string file;\n";
	fh = fh + "\tbool trace_parsing;\n";
	fh = fh + "\tvoid scan_begin();\n";
	fh = fh + "\tvoid scan_end();\n";
	fh = fh + "\tbool trace_scanning;\n";
	fh = fh + "\tyy::location location;\n";
	fh = fh + "};\n";
	fh = fh + "\n";
	fh = fh + "class FOFtoDB {\n";
	fh = fh + "private:\n";
	fh = fh + "\n";
	fh = fh + "public:\n";
	fh = fh + "\tDBinMemory readFileLoadDB(Driver, DBinMemory);\n";
	fh = fh + "\tvoid showDB(DBinMemory);\n";
	fh = fh + "};\n";
	fh = fh + "\n";
	fh = fh + "#endif\n";
	fh = fh + "\n";
	return fh;
}

std::string generate_foftodb_cpp()
{
	std::string fc;

	fc = "#include <sqlite3.h>\n";
	fc = fc + "\n";
	fc = fc + "#include \"dbRAM.hpp\"\n";
	fc = fc + "#include \"foftodb.hpp\"\n";
	fc = fc + "#include \"parser.hpp\"\n";
	fc = fc + "\n";
	fc = fc + "\n";
	fc = fc + "Driver::Driver () : trace_parsing (false), trace_scanning (false) {}\n";
	fc = fc + "\n";
	fc = fc + "int Driver::parse(const std::string &f)\n";
	fc = fc + "{\n";
	fc = fc + "\tint res;\n";
	fc = fc + "\n";
	fc = fc + "\tfile = f;\n";
	fc = fc + "\tlocation.initialize(&file);\n";
	fc = fc + "\tscan_begin();\n";
	fc = fc + "\tyy::parser parse(*this);\n";
	fc = fc + "\tparse.set_debug_level(trace_parsing);\n";
	fc = fc + "\tres = parse();\n";
	fc = fc + "\tscan_end();\n";
	fc = fc + "\treturn res;\n";
	fc = fc + "}\n";
	fc = fc + "\n";
	fc = fc + "DBinMemory FOFtoDB::readFileLoadDB(Driver drv, DBinMemory dbim) {\n";
	fc = fc + "\tint i;\n";
	fc = fc + "\tint numGeoCmd;\n";
	fc = fc + "\tbool correctTransaction;\n";
	fc = fc + "\tstd::string insertionColl, insertionPara, insertionPerp, insertionMidp;\n";
	fc = fc + "\tstd::string insertionCircle, insertionCong, insertionContri;\n";
	fc = fc + "\tstd::string insertionCyclic, insertionEqangle, insertionEqratio;\n";
	fc = fc + "\tstd::string insertionSimtri, insertionNewConsequent, insertionNewFact;\n";
	fc = fc + "\tstd::string lastInsertedRowId, lstInsRwId;\n";
	fc = fc + "\n";
	fc = fc + "\tnumGeoCmd = drv.numGeoCmd;\n";
	fc = fc + "\tfor (i = 0; i < numGeoCmd; i++) {\n";
	fc = fc + "\t\tif (drv.antconcedent[i] == 0) {\n";
	fc = fc + "\t\t\tinsertionNewFact = \"INSERT INTO NewFact (typeGeoCmd) VALUES ('\" + drv.typeGeoCmd[i] + \"')\";\n";
	fc = fc + "\t\t\tlastInsertedRowId = \"SELECT last_insert_rowid()\";\n";
	fc = fc + "\t\t\tsqlite3_exec(dbim.db, \"begin;\", 0, 0, &(dbim.zErrMsg)); \n";
	fc = fc + "\t\t\tcorrectTransaction = true; // What's the point?\n";
	fc = fc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionNewFact.c_str(), insertionNewFact.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\tif (sqlite3_step(dbim.stmt) != SQLITE_DONE)\n";
	fc = fc + "\t\t\t\tcorrectTransaction = false;\n";
	fc = fc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, lastInsertedRowId.c_str(), lastInsertedRowId.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\t\t\tlstInsRwId = (char*)sqlite3_column_text(dbim.stmt, 0);\n";
	fc = fc + "\t\t\tswitch (dbim.geoCmds[drv.typeGeoCmd[i]]) {\n";
	fc = fc + "\t\t\tcase 1:\n";
	fc = fc + "\t\t\t\tinsertionColl = \"INSERT INTO Collinear (typeGeoCmd, point1, point2, point3, newFact) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + lstInsRwId + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionColl.c_str(), insertionColl.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tif (sqlite3_step(dbim.stmt) != SQLITE_DONE)\n";
	fc = fc + "\t\t\t\t\tcorrectTransaction = false;\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 2:\n";
	fc = fc + "\t\t\t\tinsertionPara = \"INSERT INTO Parallel (typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + drv.point4[i] + \"', '\" + lstInsRwId + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionPara.c_str(), insertionPara.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tif (sqlite3_step(dbim.stmt) != SQLITE_DONE)\n";
	fc = fc + "\t\t\t\t\tcorrectTransaction = false;\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 3:\n";
	fc = fc + "\t\t\t\tinsertionPerp = \"INSERT INTO Perpendicular (typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + drv.point4[i] + \"', '\" + lstInsRwId + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionPerp.c_str(), insertionPerp.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tif (sqlite3_step(dbim.stmt) != SQLITE_DONE)\n";
	fc = fc + "\t\t\t\t\tcorrectTransaction = false;\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 4:\n";
	fc = fc + "\t\t\t\tinsertionMidp = \"INSERT INTO Midpoint (typeGeoCmd, point1, point2, point3, newFact) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + lstInsRwId + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionMidp.c_str(), insertionMidp.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tif (sqlite3_step(dbim.stmt) != SQLITE_DONE)\n";
	fc = fc + "\t\t\t\t\tcorrectTransaction = false;\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 5:\n";
	fc = fc + "\t\t\t\tinsertionCircle = \"INSERT INTO Circle (typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + drv.point4[i] + \"', '\" + lstInsRwId + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionCircle.c_str(), insertionCircle.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tif (sqlite3_step(dbim.stmt) != SQLITE_DONE)\n";
	fc = fc + "\t\t\t\t\tcorrectTransaction = false;\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 6:\n";
	fc = fc + "\t\t\t\tinsertionCong = \"INSERT INTO CongruentSegments (typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + drv.point4[i] + \"', '\" + lstInsRwId + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionCong.c_str(), insertionCong.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tif (sqlite3_step(dbim.stmt) != SQLITE_DONE)\n";
	fc = fc + "\t\t\t\t\tcorrectTransaction = false;\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 7:\n";
	fc = fc + "\t\t\t\tinsertionContri = \"INSERT INTO CongruentTriangles (typeGeoCmd, point1, point2, point3, point4, point5, point6, newFact) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + drv.point4[i] + \"', '\" + drv.point5[i] + \"', '\" + drv.point6[i] + \"', '\" + lstInsRwId + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionContri.c_str(), insertionContri.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tif (sqlite3_step(dbim.stmt) != SQLITE_DONE)\n";
	fc = fc + "\t\t\t\t\tcorrectTransaction = false;\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 8:\n";
	fc = fc + "\t\t\t\tinsertionCyclic = \"INSERT INTO Cyclic (typeGeoCmd, point1, point2, point3, point4, newFact) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + drv.point4[i] + \"', '\" + lstInsRwId + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionCyclic.c_str(), insertionCyclic.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tif (sqlite3_step(dbim.stmt) != SQLITE_DONE)\n";
	fc = fc + "\t\t\t\t\tcorrectTransaction = false;\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 9:\n";
	fc = fc + "\t\t\t\tinsertionEqangle = \"INSERT INTO EqualAngles (typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8, newFact) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + drv.point4[i] + \"', '\" + drv.point5[i] + \"', '\" + drv.point6[i] + \"', '\" + drv.point7[i] + \"', '\" + drv.point8[i] + \"', '\" + lstInsRwId + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionEqangle.c_str(), insertionEqangle.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tif (sqlite3_step(dbim.stmt) != SQLITE_DONE)\n";
	fc = fc + "\t\t\t\t\tcorrectTransaction = false;\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 10:\n";
	fc = fc + "\t\t\t\tinsertionEqratio = \"INSERT INTO EqualRatios (typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8, newFact) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + drv.point4[i] + \"', '\" + drv.point5[i] + \"', '\" + drv.point6[i] + \"', '\" + drv.point7[i] + \"', '\" + drv.point8[i] + \"', '\" + lstInsRwId + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionEqratio.c_str(), insertionEqratio.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tif (sqlite3_step(dbim.stmt) != SQLITE_DONE)\n";
	fc = fc + "\t\t\t\t\tcorrectTransaction = false;\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 11:\n";
	fc = fc + "\t\t\t\tinsertionSimtri = \"INSERT INTO SimilarTriangles (typeGeoCmd, point1, point2, point3, point4, point5, point6, newFact) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + drv.point4[i] + \"', '\" + drv.point5[i] + \"', '\" + drv.point6[i] + \"', '\" + lstInsRwId + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionSimtri.c_str(), insertionSimtri.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tif (sqlite3_step(dbim.stmt) != SQLITE_DONE)\n";
	fc = fc + "\t\t\t\t\tcorrectTransaction = false;\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tdefault:\n";
	fc = fc + "\t\t\t\tstd::cerr << \"Unknown geometric command!!!\" << std::endl;\n";
	fc = fc + "\t\t\t\texit(1);\n";
	fc = fc + "\t\t\t}\n";
	fc = fc + "\t\t\tif (correctTransaction)\n";
	fc = fc + "\t\t\t\tsqlite3_exec(dbim.db, \"commit;\", 0, 0, 0);\n";
	fc = fc + "\t\t\telse\n";
	fc = fc + "\t\t\t\tsqlite3_exec(dbim.db, \"rollback;\", 0, 0, 0);\n";
	fc = fc + "\t\t} else {\n";
	fc = fc + "\t\t\tswitch (dbim.geoCmds[drv.typeGeoCmd[i]]) {\n";
	fc = fc + "\t\t\tcase 1:\n";
	fc = fc + "\t\t\t\tinsertionNewConsequent = \"INSERT INTO Consequent (typeGeoCmd, point1, point2, point3) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"','\" + drv.point3[i] + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionNewConsequent.c_str(), insertionNewConsequent.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 2:\n";
	fc = fc + "\t\t\t\tinsertionNewConsequent = \"INSERT INTO Consequent (typeGeoCmd, point1, point2, point3, point4) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + drv.point4[i] + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionNewConsequent.c_str(), insertionNewConsequent.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 3:\n";
	fc = fc + "\t\t\t\tinsertionNewConsequent = \"INSERT INTO Consequent (typeGeoCmd, point1, point2, point3, point4) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + drv.point4[i] + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionNewConsequent.c_str(), insertionNewConsequent.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 4:\n";
	fc = fc + "\t\t\t\tinsertionNewConsequent = \"INSERT INTO Consequent (typeGeoCmd, point1, point2, point3) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionNewConsequent.c_str(), insertionNewConsequent.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 5:\n";
	fc = fc + "\t\t\t\tinsertionNewConsequent = \"INSERT INTO Consequent (typeGeoCmd, point1, point2, point3, point4) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + drv.point4[i] + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionNewConsequent.c_str(), insertionNewConsequent.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 6:\n";
	fc = fc + "\t\t\t\tinsertionNewConsequent = \"INSERT INTO Consequent (typeGeoCmd, point1, point2, point3, point4) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + drv.point4[i] + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionNewConsequent.c_str(), insertionNewConsequent.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 7:\n";
	fc = fc + "\t\t\t\tinsertionNewConsequent = \"INSERT INTO Consequent (typeGeoCmd, point1, point2, point3, point4, point5, point6) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + drv.point4[i] + \"', '\" + drv.point5[i] + \"', '\" + drv.point6[i] + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionNewConsequent.c_str(), insertionNewConsequent.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 8:\n";
	fc = fc + "\t\t\t\tinsertionNewConsequent = \"INSERT INTO Consequent (typeGeoCmd, point1, point2, point3, point4) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + drv.point4[i] + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionNewConsequent.c_str(), insertionNewConsequent.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 9:\n";
	fc = fc + "\t\t\t\tinsertionNewConsequent = \"INSERT INTO Consequent (typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + drv.point4[i] + \"', '\" + drv.point5[i] + \"', '\" + drv.point6[i] + \"', '\" + drv.point7[i] + \"', '\" + drv.point8[i] + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionNewConsequent.c_str(), insertionNewConsequent.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 10:\n";
	fc = fc + "\t\t\tinsertionNewConsequent = \"INSERT INTO Consequent (typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + drv.point4[i] + \"', '\" + drv.point5[i] + \"', '\" + drv.point6[i] + \"', '\" + drv.point7[i] + \"', '\" + drv.point8[i] + \"')\";\n";
	fc = fc + "\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionNewConsequent.c_str(), insertionNewConsequent.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\tcase 11:\n";
	fc = fc + "\t\t\t\tinsertionNewConsequent = \"INSERT INTO Consequent (typeGeoCmd, point1, point2, point3, point4, point5, point6) VALUES ('\" + drv.typeGeoCmd[i] + \"', '\" + drv.point1[i] + \"', '\" + drv.point2[i] + \"', '\" + drv.point3[i] + \"', '\" + drv.point4[i] + \"', '\" + drv.point5[i] + \"', '\" + drv.point6[i] + \"')\";\n";
	fc = fc + "\t\t\t\tdbim.rc = sqlite3_prepare_v2(dbim.db, insertionNewConsequent.c_str(), insertionNewConsequent.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\t\t\t\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\t\t\t\tbreak;\n";
	fc = fc + "\t\t\t}\n";
	fc = fc + "\t\t}\n";
	fc = fc + "\t}\n";
	fc = fc + "\treturn(dbim);\n";
	fc = fc + "}\n";
	fc = fc + "\n";
	fc = fc + "void FOFtoDB::showDB(DBinMemory dbim) {\n";
	fc = fc + "\tstd::string selectDB;\n";
	fc = fc + "\n";
	fc = fc + "\tstd::cout << std::endl;\n";
	fc = fc + "\tstd::cout << \"----------------------------\" << std::endl;\n";
	fc = fc + "\tstd::cout << \"  In memory facts database  \" << std::endl;\n";
	fc = fc + "\tstd::cout << \"----------------------------\" << std::endl;\n";
	fc = fc + "\tstd::cout << std::endl;\n";
	fc = fc + "\tstd::cout << \"Collinear facts\" << std::endl;\n";
	fc = fc + "\tselectDB = \"SELECT typeGeoCmd, point1, point2, point3 FROM Collinear\";\n";
	fc = fc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\twhile (sqlite3_step(dbim.stmt) != SQLITE_DONE) {\n";
	fc = fc + "\t\tstd::cout << \"  \" << sqlite3_column_text(dbim.stmt, 0) << \"(\" << sqlite3_column_text(dbim.stmt, 1) << \", \" << sqlite3_column_text(dbim.stmt, 2) << \", \" << sqlite3_column_text(dbim.stmt, 3) << \")\" << std::endl;\n";
	fc = fc + "\t}\n";
	fc = fc + "\tstd::cout << std::endl;\n";
	fc = fc + "\tstd::cout << \"Parallel facts\" << std::endl;\n";
	fc = fc + "\tselectDB = \"SELECT typeGeoCmd, point1, point2, point3, point4 FROM Parallel\";\n";
	fc = fc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\twhile (sqlite3_step(dbim.stmt) != SQLITE_DONE) {\n";
	fc = fc + "\t\tstd::cout << \"  \" << sqlite3_column_text(dbim.stmt, 0) << \"(\" << sqlite3_column_text(dbim.stmt, 1) << \", \" << sqlite3_column_text(dbim.stmt, 2) << \", \" << sqlite3_column_text(dbim.stmt, 3) << \", \" << sqlite3_column_text(dbim.stmt, 4) << \")\" << std::endl;\n";
	fc = fc + "\t}\n";
	fc = fc + "\tstd::cout << std::endl;\n";
	fc = fc + "\tstd::cout << \"Perpendicular facts\" << std::endl;\n";
	fc = fc + "\tselectDB = \"SELECT typeGeoCmd, point1, point2, point3, point4 FROM Perpendicular\";\n";
	fc = fc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\twhile (sqlite3_step(dbim.stmt) != SQLITE_DONE) {\n";
	fc = fc + "\t\tstd::cout << \"  \" << sqlite3_column_text(dbim.stmt, 0) << \"(\" << sqlite3_column_text(dbim.stmt, 1) << \", \" << sqlite3_column_text(dbim.stmt, 2) << \", \" << sqlite3_column_text(dbim.stmt, 3) << \", \" << sqlite3_column_text(dbim.stmt, 4) << \")\" << std::endl;\n";
	fc = fc + "\t}\n";
	fc = fc + "\tstd::cout << std::endl;\n";
	fc = fc + "\tstd::cout << \"Midpoint facts\" << std::endl;\n";
	fc = fc + "\tselectDB = \"SELECT typeGeoCmd, point1, point2, point3 FROM Midpoint\";\n";
	fc = fc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\twhile (sqlite3_step(dbim.stmt) != SQLITE_DONE) {\n";
	fc = fc + "\t\tstd::cout << \"  \" << sqlite3_column_text(dbim.stmt, 0) << \"(\" << sqlite3_column_text(dbim.stmt, 1) << \", \" << sqlite3_column_text(dbim.stmt, 2) << \", \" << sqlite3_column_text(dbim.stmt, 3) << \")\" << std::endl;\n";
	fc = fc + "\t}\n";
	fc = fc + "\tstd::cout << std::endl;\n";
	fc = fc + "\tstd::cout << \"Circle facts\" << std::endl;\n";
	fc = fc + "\tselectDB = \"SELECT typeGeoCmd, point1, point2, point3, point4 FROM Circle\";\n";
	fc = fc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\twhile (sqlite3_step(dbim.stmt) != SQLITE_DONE) {\n";
	fc = fc + "\t\tstd::cout << \"  \" << sqlite3_column_text(dbim.stmt, 0) << \"(\" << sqlite3_column_text(dbim.stmt, 1) << \", \" << sqlite3_column_text(dbim.stmt, 2) << \", \" << sqlite3_column_text(dbim.stmt, 3) << \", \" << sqlite3_column_text(dbim.stmt, 4) << \")\" << std::endl;\n";
	fc = fc + "\t}\n";
	fc = fc + "\tstd::cout << std::endl;\n";
	fc = fc + "\tstd::cout << \"Congruent Segments facts\" << std::endl;\n";
	fc = fc + "\tselectDB = \"SELECT typeGeoCmd, point1, point2, point3, point4 FROM CongruentSegments\";\n";
	fc = fc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\twhile (sqlite3_step(dbim.stmt) != SQLITE_DONE) {\n";
	fc = fc + "\t\tstd::cout << \"  \" << sqlite3_column_text(dbim.stmt, 0) << \"(\" << sqlite3_column_text(dbim.stmt, 1) << \", \" << sqlite3_column_text(dbim.stmt, 2) << \", \" << sqlite3_column_text(dbim.stmt, 3) << \", \" << sqlite3_column_text(dbim.stmt, 4) << \")\" << std::endl;\n";
	fc = fc + "\t}\n";
	fc = fc + "\tstd::cout << std::endl;\n";
	fc = fc + "\tstd::cout << \"Congruent Triangles facts\" << std::endl;\n";
	fc = fc + "\tselectDB = \"SELECT typeGeoCmd, point1, point2, point3, point4, point5, point6 FROM CongruentTriangles\";\n";
	fc = fc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\twhile (sqlite3_step(dbim.stmt) != SQLITE_DONE) {\n";
	fc = fc + "\t\tstd::cout << \"  \" << sqlite3_column_text(dbim.stmt, 0) << \"(\" << sqlite3_column_text(dbim.stmt, 1) << \", \" << sqlite3_column_text(dbim.stmt, 2) << \", \" << sqlite3_column_text(dbim.stmt, 3) << \", \" << sqlite3_column_text(dbim.stmt, 4) << \", \" << sqlite3_column_text(dbim.stmt, 5) << \", \" << sqlite3_column_text(dbim.stmt, 6) << \")\" << std::endl;\n";
	fc = fc + "\t}\n";
	fc = fc + "\tstd::cout << std::endl;\n";
	fc = fc + "\tstd::cout << \"Cyclic facts\" << std::endl;\n";
	fc = fc + "\tselectDB = \"SELECT typeGeoCmd, point1, point2, point3, point4 FROM Cyclic\";\n";
	fc = fc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\twhile (sqlite3_step(dbim.stmt) != SQLITE_DONE) {\n";
	fc = fc + "\t\tstd::cout << \"  \" << sqlite3_column_text(dbim.stmt, 0) << \"(\" << sqlite3_column_text(dbim.stmt, 1) << \", \" << sqlite3_column_text(dbim.stmt, 2) << \", \" << sqlite3_column_text(dbim.stmt, 3) << \", \" << sqlite3_column_text(dbim.stmt, 4) << \")\" << std::endl;\n";
	fc = fc + "\t}\n";
	fc = fc + "\tstd::cout << std::endl;\n";
	fc = fc + "\tstd::cout << \"Equal Angles facts\" << std::endl;\n";
	fc = fc + "\tselectDB = \"SELECT typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8 FROM EqualAngles\";\n";
	fc = fc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\twhile (sqlite3_step(dbim.stmt) != SQLITE_DONE) {\n";
	fc = fc + "\t\tstd::cout << \"  \" << sqlite3_column_text(dbim.stmt, 0) << \"(\" << sqlite3_column_text(dbim.stmt, 1) << \", \" << sqlite3_column_text(dbim.stmt, 2) << \", \" << sqlite3_column_text(dbim.stmt, 3) << \", \" << sqlite3_column_text(dbim.stmt, 4) << \", \" << sqlite3_column_text(dbim.stmt, 5) << \", \" << sqlite3_column_text(dbim.stmt, 6) << \", \" << sqlite3_column_text(dbim.stmt, 7) << \", \" << sqlite3_column_text(dbim.stmt, 8) << \")\" << std::endl;\n";
	fc = fc + "\t}\n";
	fc = fc + "\tstd::cout << std::endl;\n";
	fc = fc + "\tstd::cout << \"Equal Ratios facts\" << std::endl;\n";
	fc = fc + "\tselectDB = \"SELECT typeGeoCmd, point1, point2, point3, point4, point5, point6, point7, point8 FROM EqualRatios\";\n";
	fc = fc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\twhile (sqlite3_step(dbim.stmt) != SQLITE_DONE) {\n";
	fc = fc + "\t\tstd::cout << \"  \" << sqlite3_column_text(dbim.stmt, 0) << \"(\" << sqlite3_column_text(dbim.stmt, 1) << \", \" << sqlite3_column_text(dbim.stmt, 2) << \", \" << sqlite3_column_text(dbim.stmt, 3) << \", \" << sqlite3_column_text(dbim.stmt, 4) << \", \" << sqlite3_column_text(dbim.stmt, 5) << \", \" << sqlite3_column_text(dbim.stmt, 6) << \", \" << sqlite3_column_text(dbim.stmt, 7) << \", \" << sqlite3_column_text(dbim.stmt, 8) << \")\" << std::endl;\n";
	fc = fc + "\t}\n";
	fc = fc + "\tstd::cout << std::endl;\n";
	fc = fc + "\tstd::cout << \"Similar Triangles facts\" << std::endl;\n";
	fc = fc + "\tselectDB = \"SELECT typeGeoCmd, point1, point2, point3, point4, point5, point6 FROM SimilarTriangles\";\n";
	fc = fc + "\tdbim.rc = sqlite3_prepare_v2(dbim.db, selectDB.c_str(), selectDB.size(), &(dbim.stmt), NULL);\n";
	fc = fc + "\tsqlite3_step(dbim.stmt);\n";
	fc = fc + "\twhile (sqlite3_step(dbim.stmt) != SQLITE_DONE) {\n";
	fc = fc + "\t\tstd::cout << \"  \" << sqlite3_column_text(dbim.stmt, 0) << \"(\" << sqlite3_column_text(dbim.stmt, 1) << \", \" << sqlite3_column_text(dbim.stmt, 2) << \", \" << sqlite3_column_text(dbim.stmt, 3) << \", \" << sqlite3_column_text(dbim.stmt, 4) << \", \" << sqlite3_column_text(dbim.stmt, 5) << \", \" << sqlite3_column_text(dbim.stmt, 6) << \")\" << std::endl;\n";
	fc = fc + "\t}\n";
	fc = fc + "\tstd::cout << std::endl;\n";
	fc = fc + "\tstd::cout << \"----------------------------\" << std::endl;\n";
	fc = fc + "}\n";
	return fc;
}

std::string generate_ogpgddm()
{
	std::string o;
    
	o = "#include <cstdlib>\n";
	o = o + "#include <ctime>\n";
	o = o + "#include <iostream>\n";
	o = o + "#include <map>\n";
	o = o + "#include <string>\n";
	o = o + "\n";
	o = o + "#include <sqlite3.h>\n";
	o = o + "\n";
	o = o + "#include \"dbRAM.hpp\"\n";
	o = o + "#include \"foftodb.hpp\"\n";
	o = o + "#include \"parser.hpp\"\n";
	o = o + "#include \"prover.hpp\"\n";
	o = o + "#include \"strs.hpp\"\n";
	o = o + "#include \"version.hpp\"\n";
	o = o + "\n";
	o = o + "\n";
	o = o + "struct strsList *points = NULL;\n";
	o = o + "struct strsList *ndg = NULL;\n";
	o = o + "\n";
	o = o + "void errorMsg(int error) {\n";
	o = o + "\tstd::cerr << \"[OGPGDDM ERROR \" << error << \"] \";\n";
	o = o + "\tswitch (error) {\n";
	o = o + "\tcase 1:\n";
	o = o + "\t\tstd::cerr << \"Incorrect number of argumentos.\";\n";
	o = o + "\t\tstd::cerr << \" Use option '-h' for help.\" << std::endl;\n";
	o = o + "\t\tbreak;\n";
	o = o + "\t}\n";
	o = o + "\texit(error);\n";
	o = o + "}\n";
	o = o + "\n";
	o = o + "void xProgress(int remaining, int pagecount){\n";
	o = o + "\tdouble completion;\n";
	o = o + "\tcompletion =  (double) (pagecount - remaining) / pagecount*100; // 100% *\n";
	o = o + "\tstd:: cout << \"completion: \" << completion << \"%\" << std::endl;\n";
	o = o + "}\n";
	o = o + "\n";
	o = o + "int main(int argc, char *argv[]) {\n";
	o = o + "\tdouble time_spent;\n";
	o = o + "\tbool proved;\n";
	o = o + "\tclock_t start_t, end_t,proved_t;\n";
	o = o + "\tstd::string argone;\n";
	o = o + "\tDBinMemory dbim;\n";
	o = o + "\tDriver drv;\n";
	o = o + "\tFOFtoDB fdb;\n";
	o = o + "\tProver ogpgddm;\n";
	o = o + "\tconst char* zFilename = \"dbMemToFile.db\";\n";
	o = o + "\n";
	o = o + "\tstd::cout << \"OGP GDDM \" << VERSION << std::endl;\n";
	o = o + "\tstd::cout << \"Copyright (C) 2022 Nuno Baeta, Pedro Quaresma\" << std::endl;\n";
	o = o + "\tstd::cout << \"Distributed under GNU GPL 3.0 or later\" << std::endl;\n";
	o = o + "\n";
	o = o + "\tif (argc < 2) {\n";
	o = o + "\t\terrorMsg(1);\n";
	o = o + "\t} else {\n";
	o = o + "\t\tif (argv[1] == std::string(\"-h\") || argv[1] == std::string(\"--help\")) {\n";
	o = o + "\t\t\tstd::cout << std::endl;\n";
	o = o + "\t\t\tstd::cout << \"Usage:\" << std::endl;\n";
	o = o + "\t\t\tstd::cout << \"    ogpgddm [option | conjecture]\" << std::endl;\n";
	o = o + "\t\t\tstd::cout << \"where option is one of:\" << std::endl;\n";
	o = o + "\t\t\tstd::cout << \"    -h | --help     this help\" << std::endl;\n";
	o = o + "\t\t\tstd::cout << \"    -V | --version  OGP GDDM's version\" << std::endl;\n";
	o = o + "\t\t\texit(EXIT_SUCCESS);\n";
	o = o + "\t\t} else if (argv[1] == std::string(\"-V\")\n";
	o = o + "\t\t\t|| argv[1] == std::string(\"--version\")) {\n";
	o = o + "\t\t\t// Nothing to do, just exit with success.\n";
		o = o + "\t\t\texit(EXIT_SUCCESS);\n";
		o = o + "\t\t} else if (argv[1] == std::string(\"-p\")) {\n";
		o = o + "\t\t\tdrv.trace_parsing = true;\n";
		o = o + "\t\t} else if (argv[1] == std::string(\"-s\")) {\n";
		o = o + "\t\t\tdrv.trace_scanning = true;\n";
		o = o + "\t\t} else if (!drv.parse(argv[1])) {\n";
		o = o + "\t\t\t// Code to be executed when everything goes correctly\n";
		o = o + "\t\t} else {\n";
		o = o + "\t\t\t// Code to be executed when an error occurs while parsing\n";
		o = o + "\t\t\treturn 2;\n";
		o = o + "\t\t}\n";
		o = o + "\t}\n";
		o = o + "\n";
		o = o + "\tstart_t = clock();\n";
		o = o + "\n";
		o = o + "\tdbim.openInMemoryDB();\n";
		o = o + "\tdbim.createDBforGDDM();\n";
		o = o + "\n";
		o = o + "\tdbim = fdb.readFileLoadDB(drv, dbim);\n";
		o = o + "\n";
		o = o + "\tdbim = ogpgddm.fixedPoint(dbim,&proved_t );\n";
		o = o + "\n";
		o = o + "\tend_t = clock();\n";
		o = o + "\ttime_spent = ((double)(end_t - start_t))/CLOCKS_PER_SEC;\n";
		o = o + "\n";
		o = o + "\tproved = ogpgddm.proved(dbim);\n";
		o = o + "    \n";
		o = o + "\tif (proved) {\n";
		o = o + "\t\tstd::cout << \"Conjecture is PROVED, in: \";\n";
		o = o + "\t\tstd::cout << ((double)(proved_t - start_t))/CLOCKS_PER_SEC << \"s\" << std::endl;\n";
		o = o + "\t} else {\n";
		o = o + "\t\tstd::cout << \"Conjecture is: NOT PROVED\" << std::endl;;\n";
		o = o + "\t}\n";
		o = o + "\tstd::cout << \"Fix-point found, in: \";\n";
		o = o + "\tstd::cout << ((double)(end_t - start_t))/CLOCKS_PER_SEC << \"s\" << std::endl;\n";
		o = o + "\targone.assign(argv[1]);\n";
		o = o + "\togpgddm.saveFixedPoint(dbim, argone);\n";
		o = o + "\tdbim.closeDB();\n";
		o = o + "\n";
		o = o + "\texit(EXIT_SUCCESS);\n";
		o = o + "}\n";
		return o;
}

std::string generate_parser()
{
	std::string p;

	p = "%skeleton \"lalr1.cc\"\n";
	p = p + "%require \"3.7.5\"\n";
	p = p + "\n";
	p = p + "%defines\n";
	p = p + "\n";
	p = p + "%define api.token.raw\n";
	p = p + "%define api.token.constructor\n";
	p = p + "%define api.value.type variant\n";
	p = p + "%define parse.assert\n";
	p = p + "\n";
	p = p + "%code requires {\n";
	p = p + "\t# include <string>\n";
	p = p + "\tclass Driver;\n";
	p = p + "}\n";
	p = p + "\n";
	p = p + "%param { Driver& drv }\n";
	p = p + "\n";
	p = p + "%locations\n";
	p = p + "\n";
	p = p + "%define parse.trace\n";
	p = p + "%define parse.error detailed\n";
	p = p + "%define parse.lac full\n";
	p = p + "\n";
	p = p + "%code {\n";
	p = p + "#include \"foftodb.hpp\"\n";
	p = p + "#include \"strs.hpp\"\n";
	p = p + "\n";
	p = p + "extern struct strsList *points;\n";
	p = p + "}\n";
	p = p + "\n";
	p = p + "%define api.token.prefix {TOK_}\n";
	p = p + "%token\n";
	p = p + "\tMINUS                   \"-\"\n";
	p = p + "\tPLUS                    \"+\"\n";
	p = p + "\tSTAR                    \"*\"\n";
	p = p + "\tSLASH                   \"/\"\n";
	p = p + "\tLPAREN                  \"(\"\n";
	p = p + "\tRPAREN                  \")\"\n";
	p = p + "\tLRPAREN                 \"[\"\n";
	p = p + "\tRRPAREN                 \"]\"\n";
	p = p + "\tCOMMA                   \",\"\n";
	p = p + "\tPERIOD                  \".\"\n";
	p = p + "\tFOF                     \"fof\"\n";
	p = p + "\tCONJ                    \"conjecture\"\n";
	p = p + "\tCONJUNCTION             \"&\"\n";
	p = p + "\tFORALL                  \"!\"\n";
	p = p + "\tCOLON                   \":\"\n";
	p = p + "\tSYNTACTICCONSEQUENCE    \"=>\"\n";
	p = p + "\tCIRCLE                  \"circle\"\n";
	p = p + "\tCOLL                    \"coll\"\n";
	p = p + "\tCONG                    \"cong\"\n";
	p = p + "\tCONTRI                  \"contri\"\n";
	p = p + "\tCYCLIC                  \"cyclic\"\n";
	p = p + "\tEQANGLE                 \"eqangle\"\n";
	p = p + "\tEQRATIO                 \"eqratio\"\n";
	p = p + "\tMIDP                    \"midp\"\n";
	p = p + "\tPARA                    \"para\"\n";
	p = p + "\tPERP                    \"perp\"\n";
	p = p + "\tSIMTRI                  \"simtri\"\n";
	p = p + ";\n";
	p = p + "\n";
	p = p + "%token <std::string> IDENTIFIER \"identifier\"\n";
	p = p + "%token <int> NUMBER \"number\"\n";
	p = p + "\n";
	p = p + "%printer { yyo << $$; } <*>;\n";
	p = p + "\n";
	p = p + "%%\n";
	p = p + "\n";
	p = p + "%start unit;\n";
	p = p + "unit: fof {};\n";
	p = p + "\n";
	p = p + "fof:\n";
	p = p + "\t\"fof\" \"(\" \"identifier\" \",\" \"conjecture\" \",\" forall \":\" \"(\" \"(\" antecedents \")\" \"=>\" consequent \")\" \")\" \".\"\n";
	p = p + "\t{};\n";
	p = p + "\n";
	p = p + "forall:\n";
	p = p + "\t\"!\" \"[\" varList \"]\"\n";
	p = p + "\t{};\n";
	p = p + "\n";
	p = p + "antecedents:\n";
	p = p + "\t%empty {}\n";
	p = p + "\t| antecedents antecedent {}\n";
	p = p + "\t| antecedents antecedent \"&\" {}\n";
	p = p + "\n";
	p = p + "antecedent:\n";
	p = p + "\tgeocmd \n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.antconcedent[drv.numGeoCmd] = 0;\n";
	p = p + "\t\tdrv.numGeoCmd++;\n";
	p = p + "\t};\n";
	p = p + "\n";
	p = p + "consequent:\n";
	p = p + "\tgeocmdConsequent\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.antconcedent[drv.numGeoCmd] = 1;\n";
	p = p + "\t\tdrv.numGeoCmd++;\n";
	p = p + "\t};\n";
	p = p + "  \n";
	p = p + "geocmd:\n";
	p = p + "\tcircle {};\n";
	p = p + "\t| coll {};\n";
	p = p + "\t| cong {};\n";
	p = p + "\t| contri {};\n";
	p = p + "\t| cyclic {};\n";
	p = p + "\t| eqangle {};\n";
	p = p + "\t| eqratio {};\n";
	p = p + "\t| para {};\n";
	p = p + "\t| perp {};\n";
	p = p + "\t| midp {};\n";
	p = p + "\t| simtri {};\n";
	p = p + "\n";
	p = p + "circle:\n";
	p = p + "\t\"circle\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"circle\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd]=$3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd]=$5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd]=$7;\n";
	p = p + "\t\tdrv.point4[drv.numGeoCmd]=$9;\n";
	p = p + "\t\tpoints = addStr($3, points);\n";
	p = p + "\t\tpoints = addStr($5, points);\n";
	p = p + "\t\tpoints = addStr($7, points);\n";
	p = p + "\t\tpoints = addStr($9, points);\n";
	p = p + "\t}\n";
	p = p + "\n";
	p = p + "coll:\n";
	p = p + "\t\"coll\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"coll\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd] = $3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd] = $5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd] = $7;\n";
	p = p + "\t\tpoints = addStr($3, points);\n";
	p = p + "\t\tpoints = addStr($5, points);\n";
	p = p + "\t\tpoints = addStr($7, points);\n";
	p = p + "\t};\n";
	p = p + "\n";
	p = p + "cong:\n";
	p = p + "\t\"cong\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"cong\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd]=$3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd]=$5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd]=$7;\n";
	p = p + "\t\tdrv.point4[drv.numGeoCmd]=$9;\n";
	p = p + "\t\tpoints = addStr($3, points);\n";
	p = p + "\t\tpoints = addStr($5, points);\n";
	p = p + "\t\tpoints = addStr($7, points);\n";
	p = p + "\t\tpoints = addStr($9, points);\n";
	p = p + "\t}\n";
	p = p + "\n";
	p = p + "contri:\n";
	p = p + "\t\"contri\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"contri\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd]=$3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd]=$5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd]=$7;\n";
	p = p + "\t\tdrv.point4[drv.numGeoCmd]=$9;\n";
	p = p + "\t\tdrv.point5[drv.numGeoCmd]=$11;\n";
	p = p + "\t\tdrv.point6[drv.numGeoCmd]=$13;\n";
	p = p + "\t\tpoints = addStr($3, points);\n";
	p = p + "\t\tpoints = addStr($5, points);\n";
	p = p + "\t\tpoints = addStr($7, points);\n";
	p = p + "\t\tpoints = addStr($9, points);\n";
	p = p + "\t\tpoints = addStr($11, points);\n";
	p = p + "\t\tpoints = addStr($13, points);\n";
	p = p + "\t}\n";
	p = p + "\n";
	p = p + "cyclic:\n";
	p = p + "\t\"cyclic\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"cyclic\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd]=$3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd]=$5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd]=$7;\n";
	p = p + "\t\tdrv.point4[drv.numGeoCmd]=$9;\n";
	p = p + "\t\tpoints = addStr($3, points);\n";
	p = p + "\t\tpoints = addStr($5, points);\n";
	p = p + "\t\tpoints = addStr($7, points);\n";
	p = p + "\t\tpoints = addStr($9, points);\n";
	p = p + "\t}\n";
	p = p + "\n";
	p = p + "eqangle:\n";
	p = p + "\t\"eqangle\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\tdrv.typeGeoCmd[drv.numGeoCmd] = \"eqangle\";\n";
	p = p + "\tdrv.point1[drv.numGeoCmd]=$3;\n";
	p = p + "\tdrv.point2[drv.numGeoCmd]=$5;\n";
	p = p + "\tdrv.point3[drv.numGeoCmd]=$7;\n";
	p = p + "\tdrv.point4[drv.numGeoCmd]=$9;\n";
	p = p + "\tdrv.point5[drv.numGeoCmd]=$11;\n";
	p = p + "\tdrv.point6[drv.numGeoCmd]=$13;\n";
	p = p + "\tdrv.point7[drv.numGeoCmd]=$15;\n";
	p = p + "\tdrv.point8[drv.numGeoCmd]=$17;\n";
	p = p + "\tpoints = addStr($3, points);\n";
	p = p + "\tpoints = addStr($5, points);\n";
	p = p + "\tpoints = addStr($7, points);\n";
	p = p + "\tpoints = addStr($9, points);\n";
	p = p + "\tpoints = addStr($11, points);\n";
	p = p + "\tpoints = addStr($13, points);\n";
	p = p + "\tpoints = addStr($15, points);\n";
	p = p + "\tpoints = addStr($17, points);\n";
	p = p + "\t}\n";
	p = p + "\n";
	p = p + "eqratio:\n";
	p = p + "\t\"eqratio\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"eqratio\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd]=$3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd]=$5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd]=$7;\n";
	p = p + "\t\tdrv.point4[drv.numGeoCmd]=$9;\n";
	p = p + "\t\tdrv.point5[drv.numGeoCmd]=$11;\n";
	p = p + "\t\tdrv.point6[drv.numGeoCmd]=$13;\n";
	p = p + "\t\tdrv.point7[drv.numGeoCmd]=$15;\n";
	p = p + "\t\tdrv.point8[drv.numGeoCmd]=$17;\n";
	p = p + "\t\tpoints = addStr($3, points);\n";
	p = p + "\t\tpoints = addStr($5, points);\n";
	p = p + "\t\tpoints = addStr($7, points);\n";
	p = p + "\t\tpoints = addStr($9, points);\n";
	p = p + "\t\tpoints = addStr($11, points);\n";
	p = p + "\t\tpoints = addStr($13, points);\n";
	p = p + "\t\tpoints = addStr($15, points);\n";
	p = p + "\t\tpoints = addStr($17, points);\n";
	p = p + "\t}\n";
	p = p + "\n";
	p = p + "para:\n";
	p = p + "\t\"para\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"para\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd]=$3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd]=$5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd]=$7;\n";
	p = p + "\t\tdrv.point4[drv.numGeoCmd]=$9;\n";
	p = p + "\t\tpoints = addStr($3, points);\n";
	p = p + "\t\tpoints = addStr($5, points);\n";
	p = p + "\t\tpoints = addStr($7, points);\n";
	p = p + "\t\tpoints = addStr($9, points);\n";
	p = p + "\t};\n";
	p = p + "\n";
	p = p + "perp:\n";
	p = p + "\t\"perp\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\"  \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"perp\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd]=$3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd]=$5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd]=$7;\n";
	p = p + "\t\tdrv.point4[drv.numGeoCmd]=$9;\n";
	p = p + "\t\tpoints = addStr($3, points);\n";
	p = p + "\t\tpoints = addStr($5, points);\n";
	p = p + "\t\tpoints = addStr($7, points);\n";
	p = p + "\t\tpoints = addStr($9, points);\n";
	p = p + "\t};\n";
	p = p + "\n";
	p = p + "midp:\n";
	p = p + "\t\"midp\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"midp\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd] = $3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd] = $5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd] = $7;\n";
	p = p + "\t\tpoints = addStr($3, points);\n";
	p = p + "\t\tpoints = addStr($5, points);\n";
	p = p + "\t\tpoints = addStr($7, points);\n";
	p = p + "\t};\n";
	p = p + "\n";
	p = p + "simtri:\n";
	p = p + "\t\"simtri\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"simtri\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd]=$3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd]=$5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd]=$7;\n";
	p = p + "\t\tdrv.point4[drv.numGeoCmd]=$9;\n";
	p = p + "\t\tdrv.point5[drv.numGeoCmd]=$11;\n";
	p = p + "\t\tdrv.point6[drv.numGeoCmd]=$13;\n";
	p = p + "\t\tpoints = addStr($3, points);\n";
	p = p + "\t\tpoints = addStr($5, points);\n";
	p = p + "\t\tpoints = addStr($7, points);\n";
	p = p + "\t\tpoints = addStr($9, points);\n";
	p = p + "\t\tpoints = addStr($11, points);\n";
	p = p + "\t\tpoints = addStr($13, points);\n";
	p = p + "\t}\n";
	p = p + "\n";
	p = p + "geocmdConsequent:\n";
	p = p + "\tcircleConsequent {};\n";
	p = p + "\t| collConsequent {};\n";
	p = p + "\t| congConsequent {};\n";
	p = p + "\t| contriConsequent {};\n";
	p = p + "\t| cyclicConsequent {};\n";
	p = p + "\t| eqangleConsequent {};\n";
	p = p + "\t| eqratioConsequent {};\n";
	p = p + "\t| paraConsequent {};\n";
	p = p + "\t| perpConsequent {};\n";
	p = p + "\t| midpConsequent {};\n";
	p = p + "\t| simtriConsequent {};\n";
	p = p + "\n";
	p = p + "circleConsequent:\n";
	p = p + "\t\"circle\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"circle\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd]=$3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd]=$5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd]=$7;\n";
	p = p + "\t\tdrv.point4[drv.numGeoCmd]=$9;\n";
	p = p + "\t}\n";
	p = p + "\n";
	p = p + "collConsequent:\n";
	p = p + "\t\"coll\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"coll\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd] = $3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd] = $5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd] = $7;\n";
	p = p + "\t};\n";
	p = p + "\n";
	p = p + "congConsequent:\n";
	p = p + "\t\"cong\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"cong\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd]=$3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd]=$5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd]=$7;\n";
	p = p + "\t\tdrv.point4[drv.numGeoCmd]=$9;\n";
	p = p + "\t}\n";
	p = p + "\n";
	p = p + "contriConsequent:\n";
	p = p + "\t\"contri\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"contri\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd]=$3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd]=$5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd]=$7;\n";
	p = p + "\t\tdrv.point4[drv.numGeoCmd]=$9;\n";
	p = p + "\t\tdrv.point5[drv.numGeoCmd]=$11;\n";
	p = p + "\t\tdrv.point6[drv.numGeoCmd]=$13;\n";
	p = p + "\t}\n";
	p = p + "\n";
	p = p + "cyclicConsequent:\n";
	p = p + "\t\"cyclic\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"cyclic\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd]=$3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd]=$5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd]=$7;\n";
	p = p + "\t\tdrv.point4[drv.numGeoCmd]=$9;\n";
	p = p + "\t}\n";
	p = p + "\n";
	p = p + "eqangleConsequent:\n";
	p = p + "\t\"eqangle\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"eqangle\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd]=$3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd]=$5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd]=$7;\n";
	p = p + "\t\tdrv.point4[drv.numGeoCmd]=$9;\n";
	p = p + "\t\tdrv.point5[drv.numGeoCmd]=$11;\n";
	p = p + "\t\tdrv.point6[drv.numGeoCmd]=$13;\n";
	p = p + "\t\tdrv.point7[drv.numGeoCmd]=$15;\n";
	p = p + "\t\tdrv.point8[drv.numGeoCmd]=$17;\n";
	p = p + "\t}\n";
	p = p + "\n";
	p = p + "eqratioConsequent:\n";
	p = p + "\t\"eqratio\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"eqratio\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd]=$3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd]=$5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd]=$7;\n";
	p = p + "\t\tdrv.point4[drv.numGeoCmd]=$9;\n";
	p = p + "\t\tdrv.point5[drv.numGeoCmd]=$11;\n";
	p = p + "\t\tdrv.point6[drv.numGeoCmd]=$13;\n";
	p = p + "\t\tdrv.point7[drv.numGeoCmd]=$15;\n";
	p = p + "\t\tdrv.point8[drv.numGeoCmd]=$17;\n";
	p = p + "\t}\n";
	p = p + "\n";
	p = p + "paraConsequent:\n";
	p = p + "\t\"para\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"para\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd]=$3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd]=$5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd]=$7;\n";
	p = p + "\t\tdrv.point4[drv.numGeoCmd]=$9;\n";
	p = p + "\t};\n";
	p = p + "\n";
	p = p + "perpConsequent:\n";
	p = p + "\t\"perp\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\"  \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"perp\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd]=$3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd]=$5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd]=$7;\n";
	p = p + "\t\tdrv.point4[drv.numGeoCmd]=$9;\n";
	p = p + "\t};\n";
	p = p + "\n";
	p = p + "midpConsequent:\n";
	p = p + "\t\"midp\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"midp\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd] = $3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd] = $5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd] = $7;\n";
	p = p + "\t};\n";
	p = p + "\n";
	p = p + "simtriConsequent:\n";
	p = p + "\t\"simtri\" \"(\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \",\" \"identifier\" \")\"\n";
	p = p + "\t{\n";
	p = p + "\t\tdrv.typeGeoCmd[drv.numGeoCmd] = \"simtri\";\n";
	p = p + "\t\tdrv.point1[drv.numGeoCmd]=$3;\n";
	p = p + "\t\tdrv.point2[drv.numGeoCmd]=$5;\n";
	p = p + "\t\tdrv.point3[drv.numGeoCmd]=$7;\n";
	p = p + "\t\tdrv.point4[drv.numGeoCmd]=$9;\n";
	p = p + "\t\tdrv.point5[drv.numGeoCmd]=$11;\n";
	p = p + "\t\tdrv.point6[drv.numGeoCmd]=$13;\n";
	p = p + "\t}\n";
	p = p + "\n";
	p = p + "varList:\n";
	p = p + "\t%empty {}\n";
	p = p + "\t| varList \"identifier\" {}\n";
	p = p + "\t| varList \"identifier\" \",\" {}\n";
	p = p + "\n";
	p = p + "\n";
	p = p + "%left \"+\" \"-\";\n";
	p = p + "%left \"*\" \"/\";\n";
	p = p + "\n";
	p = p + "%%\n";
	p = p + "\n";
	p = p + "void yy::parser::error(const location_type& l, const std::string& m)\n";
	p = p + "{\n";
	p = p + "\tstd::cerr << l << \": \" << m << '\\n';\n";
	p = p + "}\n";
	p = p + "\n";
	return p;
}

std::string generate_scanner()
{
	std::string s;

	s = "%{ \n";
	s = s + "# include <cerrno>\n";
	s = s + "# include <climits>\n";
	s = s + "# include <cstdlib>\n";
	s = s + "# include <cstring>\n";
	s = s + "# include <string>\n";
	s = s + "# include \"foftodb.hpp\"\n";
	s = s + "# include \"parser.hpp\"\n";
	s = s + "%}\n";
	s = s + "\n";
	s = s + "%{\n";
	s = s + "#if defined __clang__\n";
	s = s + "# define CLANG_VERSION (__clang_major__ * 100 + __clang_minor__)\n";
	s = s + "#endif\n";
	s = s + "\n";
	s = s + "#if defined __GNUC__ && !defined __clang__ && !defined __ICC\n";
	s = s + "# define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)\n";
	s = s + "#endif\n";
	s = s + "\n";
	s = s + "#if defined GCC_VERSION && 600 <= GCC_VERSION\n";
	s = s + "# pragma GCC diagnostic ignored \"-Wnull-dereference\"\n";
	s = s + "#endif\n";
	s = s + "\n";
	s = s + "#if defined CLANG_VERSION && 500 <= CLANG_VERSION\n";
	s = s + "# pragma clang diagnostic ignored \"-Wold-style-cast\"\n";
	s = s + "# pragma clang diagnostic ignored \"-Wzero-as-null-pointer-constant\"\n";
	s = s + "#elif defined GCC_VERSION && 407 <= GCC_VERSION\n";
	s = s + "# pragma GCC diagnostic ignored \"-Wold-style-cast\"\n";
	s = s + "# pragma GCC diagnostic ignored \"-Wzero-as-null-pointer-constant\"\n";
	s = s + "#endif\n";
	s = s + "\n";
	s = s + "#define FLEX_VERSION (YY_FLEX_MAJOR_VERSION * 100 + YY_FLEX_MINOR_VERSION)\n";
	s = s + "\n";
	s = s + "#if FLEX_VERSION < 206 && defined CLANG_VERSION\n";
	s = s + "# pragma clang diagnostic ignored \"-Wdocumentation\"\n";
	s = s + "#endif\n";
	s = s + "\n";
	s = s + "#if FLEX_VERSION < 206\n";
	s = s + "# if defined CLANG_VERSION && 600 <= CLANG_VERSION\n";
	s = s + "#  pragma clang diagnostic ignored \"-Wdeprecated-register\"\n";
	s = s + "# elif defined GCC_VERSION && 700 <= GCC_VERSION\n";
	s = s + "#  pragma GCC diagnostic ignored \"-Wregister\"\n";
	s = s + "# endif\n";
	s = s + "#endif\n";
	s = s + "\n";
	s = s + "#if FLEX_VERSION < 206\n";
	s = s + "# if defined CLANG_VERSION\n";
	s = s + "#  pragma clang diagnostic ignored \"-Wconversion\"\n";
	s = s + "#  pragma clang diagnostic ignored \"-Wdocumentation\"\n";
	s = s + "#  pragma clang diagnostic ignored \"-Wshorten-64-to-32\"\n";
	s = s + "#  pragma clang diagnostic ignored \"-Wsign-conversion\"\n";
	s = s + "# elif defined GCC_VERSION\n";
	s = s + "#  pragma GCC diagnostic ignored \"-Wconversion\"\n";
	s = s + "#  pragma GCC diagnostic ignored \"-Wsign-conversion\"\n";
	s = s + "# endif\n";
	s = s + "#endif\n";
	s = s + "\n";
	s = s + "#if defined GCC_VERSION && 900 <= GCC_VERSION\n";
	s = s + "# pragma GCC diagnostic ignored \"-Wuseless-cast\"\n";
	s = s + "#endif\n";
	s = s + "%}\n";
	s = s + "\n";
	s = s + "\n";
	s = s + "%option noyywrap nounput noinput batch debug\n";
	s = s + "\n";
	s = s + "\n";
	s = s + "%{\n";
	s = s + "\tyy::parser::symbol_type make_NUMBER(const std::string &s, const yy::parser::location_type& loc);\n";
	s = s + "%}\n";
	s = s + "\n";
	s = s + "id      [a-zA-Z][a-zA-Z_0-9]*\n";
	s = s + "int     [0-9]+\n";
	s = s + "blank   [ \\t\\r]\n";
	s = s + "\n";
	s = s + "%{\n";
	s = s + "# define YY_USER_ACTION  loc.columns(yyleng);\n";
	s = s + "%}\n";
	s = s + "\n";
	s = s + "%%\n";
	s = s + "\n";
	s = s + "%{\n";
	s = s + "\tyy::location& loc = drv.location;\n";
	s = s + "\tloc.step();\n";
	s = s + "%}\n";
	s = s + "\n";
	s = s + "%.*        loc.step();\n";
	s = s + "include.*  loc.step();\n";
	s = s + "{blank}+   loc.step();\n";
	s = s + "\\n+        loc.lines(yyleng); loc.step();\n";
	s = s + "\n";
	s = s + "\"-\"             return yy::parser::make_MINUS(loc);\n";
	s = s + "\"+\"             return yy::parser::make_PLUS(loc);\n";
	s = s + "\"*\"             return yy::parser::make_STAR(loc);\n";
	s = s + "\"/\"             return yy::parser::make_SLASH(loc);\n";
	s = s + "\"(\"             return yy::parser::make_LPAREN(loc);\n";
	s = s + "\")\"             return yy::parser::make_RPAREN(loc);\n";
	s = s + "\"[\"             return yy::parser::make_LRPAREN(loc);\n";
	s = s + "\"]\"             return yy::parser::make_RRPAREN(loc);\n";
	s = s + "\"&\"             return yy::parser::make_CONJUNCTION(loc);\n";
	s = s + "\".\"             return yy::parser::make_PERIOD(loc);\n";
	s = s + "\",\"             return yy::parser::make_COMMA(loc);\n";
	s = s + "\":\"             return yy::parser::make_COLON(loc);\n";
	s = s + "\"=>\"            return yy::parser::make_SYNTACTICCONSEQUENCE(loc);\n";
	s = s + "\"!\"             return yy::parser::make_FORALL(loc);\n";
	s = s + "\"conjecture\"    return yy::parser::make_CONJ(loc);\n";
	s = s + "\"fof\"           return yy::parser::make_FOF(loc);\n";
	s = s + "\"circle\"        return yy::parser::make_CIRCLE(loc);\n";
	s = s + "\"coll\"          return yy::parser::make_COLL(loc);\n";
	s = s + "\"cong\"	        return yy::parser::make_CONG(loc);\n";
	s = s + "\"contri\"        return yy::parser::make_CONTRI(loc);\n";
	s = s + "\"cyclic\"        return yy::parser::make_CYCLIC(loc);\n";
	s = s + "\"eqangle\"       return yy::parser::make_EQANGLE(loc);\n";
	s = s + "\"eqratio\"       return yy::parser::make_EQRATIO(loc);\n";
	s = s + "\"midp\"	        return yy::parser::make_MIDP(loc);\n";
	s = s + "\"para\"          return yy::parser::make_PARA(loc);\n";
	s = s + "\"perp\"          return yy::parser::make_PERP(loc);\n";
	s = s + "\"simtri\"        return yy::parser::make_SIMTRI(loc);\n";
	s = s + "\n";
	s = s + "{int}   return make_NUMBER(yytext, loc);\n";
	s = s + "{id}    return yy::parser::make_IDENTIFIER (yytext, loc);\n";
	s = s + ".          {\n";
	s = s + "               throw yy::parser::syntax_error(loc, \"invalid character: \" + std::string(yytext));\n";
	s = s + "           }\n";
	s = s + "<<EOF>>    return yy::parser::make_YYEOF (loc);\n";
	s = s + "\n";
	s = s + "%%\n";
	s = s + "\n";
	s = s + "yy::parser::symbol_type make_NUMBER(const std::string &s, const yy::parser::location_type& loc)\n";
	s = s + "{\n";
	s = s + "\terrno = 0;\n";
	s = s + "\tlong n = strtol(s.c_str(), NULL, 10);\n";
	s = s + "\tif (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))\n";
	s = s + "\t\tthrow yy::parser::syntax_error(loc, \"integer is out of range: \" + s);\n";
	s = s + "\treturn yy::parser::make_NUMBER((int) n, loc);\n";
	s = s + "}\n";
	s = s + "\n";
	s = s + "void Driver::scan_begin()\n";
	s = s + "{\n";
	s = s + "\tyy_flex_debug = trace_scanning;\n";
	s = s + "\tif (file.empty() || file == \"-\")\n";
	s = s + "\t\tyyin = stdin;\n";
	s = s + "\telse if (!(yyin = fopen(file.c_str (), \"r\")))\n";
	s = s + "\t{\n";
	s = s + "\t\tstd::cerr << \"cannot open \" << file << \": \" << strerror(errno) << '\\n';\n";
	s = s + "\t\texit (EXIT_FAILURE);\n";
	s = s + "\t}\n";
	s = s + "}\n";
	s = s + "\n";
	s = s + "void Driver::scan_end()\n";
	s = s + "{\n";
	s = s + "\tfclose(yyin);\n";
	s = s + "}\n";
	s = s + "\n";
	return s;
}

std::string generate_strs_hpp()
{
	std::string sh;

	sh = sh + "#ifndef STRS\n";
	sh = sh + "#define STRS\n";
	sh = sh + "\n";
	sh = sh + "struct strsList {\n";
	sh = sh + "\tstd::string str;\n";
	sh = sh + "\tstruct strsList *next;\n";
	sh = sh + "};\n";
	sh = sh + "\n";
	sh = sh + "struct strsList *addStr(std::string, struct strsList *);\n";
	sh = sh + "void showStrs(struct strsList *);\n";
	sh = sh + "\n";
	sh = sh + "#endif\n";
	return sh;
}

std::string generate_strs_cpp()
{
	std::string sc;

	sc = sc + "#include <iostream>\n";
	sc = sc + "#include <string>\n";
	sc = sc + "\n";
	sc = sc + "#include \"strs.hpp\"\n";
	sc = sc + "\n";
	sc = sc + "struct strsList *addStr(std::string theStr, struct strsList *strs) {\n";
	sc = sc + "\tbool strExists;\n";
	sc = sc + "\tstruct strsList *current, *newStr;\n";
	sc = sc + "\n";
	sc = sc + "\tstrExists = false;\n";
	sc = sc + "\tcurrent = strs;\n";
	sc = sc + "\twhile (current != NULL && !strExists) {\n";
	sc = sc + "\t\tstrExists = current->str == theStr;\n";
	sc = sc + "\t\tcurrent = current->next;\n";
	sc = sc + "\t}\n";
	sc = sc + "\tif (!strExists) {\n";
	sc = sc + "\t\tnewStr = new strsList;\n";
	sc = sc + "\t\tnewStr->str = theStr;\n";
	sc = sc + "\t\tnewStr->next = strs;\n";
	sc = sc + "\t\tstrs = newStr;\n";
	sc = sc + "\t}\n";
	sc = sc + "\treturn strs;\n";
	sc = sc + "}\n";
	sc = sc + "\n";
	sc = sc + "void showStrs(struct strsList *strs) {\n";
	sc = sc + "\tstruct strsList *current;\n";
	sc = sc + "\n";
	sc = sc + "\tcurrent = strs;\n";
	sc = sc + "\twhile (current != NULL) {\n";
	sc = sc + "\t\tstd::cout << \"    \" << current->str << std::endl;\n";
	sc = sc + "\t\tcurrent = current->next;\n";
	sc = sc + "\t}\n";
	sc = sc + "}\n";
	sc = sc + "\n";
	return sc;
}

std::string generate_version_hpp()
{
	std::string vh;

	vh = "#ifndef OGPGDDMVERSION\n";
	vh = vh + "#define OGPGDDMVERSION\n";
	vh = vh + "\n";
	vh = vh + "#define VERSION \"- PG Generated\"\n";
	vh = vh + "\n";
	vh = vh + "#endif\n";
	return vh;
}
