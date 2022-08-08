/*
 * prover.hpp
 *
 * GDDM's core: deduction rules and point fixed construction.
 *
 * This file is part of the OGP GDDM prover, which, in turn, is part of
 * the Open Geometry prover Community Project (OGPCP)
 * <https://github.com/opengeometryprover>.
 *
 * Copyright (C) 2022 Nuno Baeta, Pedro Quaresma
 * Distributed under GNU GPL 3.0 or later 
 */


#ifndef PROVER
#define PROVER

#include <string>

#include "dbRAM.hpp"

class DBinMemory;

class Prover {

private:
    void deriveNewColl(std::string,std::string,std::string);
  
public:
    // Show Fixed Point - displays 'Facts' table after findind a fixed-point.
    void showFixedPoint(DBinMemory);
  
    /*
     * Find the fixed point of the antecedents
     * ---> assume the database of the method, already opened and populated
     * <--- databased modified, adding all the new facts derived from the
     * antecedents
     */
    DBinMemory fixedPoint(DBinMemory);

    /*
     * Verify if the conjecture was proved, or not
     * ---> database with all the facts in the fixed point
     * <--- true the consequent is in the facts table, false in the othe case
     */
    bool proved(DBinMemory);

    DBinMemory ruleD01(DBinMemory, std::string, std::string, std::string);
    DBinMemory ruleD02(DBinMemory, std::string, std::string, std::string);
    DBinMemory ruleD03(DBinMemory, std::string, std::string, std::string);
    DBinMemory ruleD04(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD05(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD06(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD07(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD08(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD09(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD10para(DBinMemory,std::string, std::string, std::string,
			   std::string);
    DBinMemory ruleD10perp(DBinMemory,std::string, std::string, std::string,
			   std::string);
    DBinMemory ruleD11(DBinMemory, std::string, std::string, std::string);
    DBinMemory ruleD12(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD13(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD14(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD15(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD16(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD17(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD18(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD19(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD20(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD21(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD22(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD23(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD24(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD25(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD26(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD27(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD28(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD29(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD30(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD31(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string);
    DBinMemory ruleD32(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string);
    DBinMemory ruleD33(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string);
    DBinMemory ruleD34(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string);
    DBinMemory ruleD35(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string);
    DBinMemory ruleD36(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string);
    DBinMemory ruleD37(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string);
    DBinMemory ruleD38(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string);
    DBinMemory ruleD39(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD40(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD41(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD42(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD43(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD44(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD45(DBinMemory, std::string, std::string, std::string);
    DBinMemory ruleD46(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD47(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD48(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD49(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD50(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD51(DBinMemory, std::string, std::string, std::string);
    DBinMemory ruleD52(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD53(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD54(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD55(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD56(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD57(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD58(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string);
    DBinMemory ruleD59(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string);
    DBinMemory ruleD60(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string);
    DBinMemory ruleD61(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string);
    DBinMemory ruleD62(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string);
    DBinMemory ruleD63(DBinMemory, std::string, std::string, std::string);
    DBinMemory ruleD64(DBinMemory, std::string, std::string, std::string);
    DBinMemory ruleD65(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD66(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD67(DBinMemory, std::string, std::string, std::string);
    DBinMemory ruleD68(DBinMemory, std::string, std::string, std::string);
    DBinMemory ruleD69(DBinMemory, std::string, std::string, std::string);
    DBinMemory ruleD70(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD71(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD72(DBinMemory, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD73(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD74(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);
    DBinMemory ruleD75(DBinMemory, std::string, std::string, std::string,
		       std::string, std::string, std::string, std::string,
		       std::string);

    void testDBim(DBinMemory);
};

#endif
