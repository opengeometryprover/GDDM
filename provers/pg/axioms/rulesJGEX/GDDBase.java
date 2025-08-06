/**
 * Created by IntelliJ IDEA.
 * User: Ye
 * Date: 2006-2-14
 * Time: 21:37:28
 * To change this template use File | Settings | File Templates.
 */
package gprover;
/**
 * Base class for geometric deduction database operations.
 */
public class GDDBase extends Gib {

    /**
     * Returns an index corresponding to the occurrence of a value among three candidates.
     *
     * @param p the value to match
     * @param p1 the first candidate value
     * @param p2 the second candidate value
     * @param p3 the third candidate value
     * @return 1 if p equals p1, 2 if p equals p2, 3 if p equals p3, otherwise 0
     */
    final int ind_3(int p, int p1, int p2, int p3) {
        if (p == p1) return (1);
        if (p == p2) return (2);
        if (p == p3) return (3);
        return (0);
    }

    /**
     * Returns an index corresponding to the occurrence of a value among the first three elements of an array.
     *
     * @param p the value to match
     * @param pp an array of candidate values (must contain at least three elements)
     * @return 1 if p equals pp[0], 2 if p equals pp[1], 3 if p equals pp[2], otherwise 0
     */
    final int ind_3(int p, int[] pp) {
        if (p == pp[0]) return (1);
        if (p == pp[1]) return (2);
        if (p == pp[2]) return (3);
        return (0);
    }

    /**
     * Determines whether the points identified by a, b, and c are collinear and registers the condition if true.
     *
     * @param a the first point
     * @param b the second point
     * @param c the third point
     * @return true if the points are collinear, false otherwise
     */
    final boolean ycoll(int a, int b, int c) {
        boolean i = xcoll(a, b, c);
        if (i) {
            add_codb(CO_COLL, a, b, c, 0, 0, 0, 0, 0);
        }
        return (i);
    }

    /**
     * Determines whether the lines defined by points (a, b) and (p, q) are perpendicular and records the condition if so.
     *
     * @param a the first point on the first line
     * @param b the second point on the first line
     * @param p the first point on the second line
     * @param q the second point on the second line
     * @return true if the lines are perpendicular, false otherwise
     */
    final boolean yperp(int a, int b, int p, int q) {
        boolean i = xperp(a, b, p, q);
        if (i) {
            add_codb(CO_PERP, a, b, p, q, 0, 0, 0, 0);
        }
        return (i);
    }

    /**
     * Checks whether the segments defined by (a, b) and (p, q) are congruent and registers the condition if they are.
     *
     * @param a the first point of the first segment
     * @param b the second point of the first segment
     * @param p the first point of the second segment
     * @param q the second point of the second segment
     * @return true if the segments are congruent, false otherwise
     */
    final boolean ycong(int a, int b, int p, int q) {
        boolean i = xcong(a, b, p, q);
        if (i) {
            add_codb(CO_CONG, a, b, p, q, 0, 0, 0, 0);
        }
        return (i);
    }

    /**
     * Checks if the given identifier is below the current depth or if the point registry is full.
     *
     * @param id the identifier to check
     * @return true if id is less than depth or the point registry is full, false otherwise
     */
    final boolean ch_dep(long id) {
        return id < depth || isPFull();
    }

    /**
     * Processes the provided circular arc by adding corresponding conditions for parallelism, cyclicity, and congruence.
     *
     * @param cr the circular arc used to derive conditions
     * @param m1 the first marker or identifier related to the arc
     * @param m2 the second marker or identifier related to the arc
     * @param p1 the first point used for condition registration
     * @param p2 the second point used for condition registration
     */
    final void add_cr_pn_as(ACir cr, int m1, int m2, int p1, int p2) {
        int lm = R_CR_P_EQARC;

        add_codb(CO_PARA, m1, m2, p1, p2, 0, 0, 0, 0);
        add_codb(CO_CYCLIC, 0, m1, m2, p1, p2, 0, 0, 0);
        add_ea_pt_t(lm, m1, p1, p2, p1, p2, m2);
        add_ea_pt_t(lm, m2, p1, p2, p1, p2, m1);
        add_cong(lm, 0, m1, p1, m2, p2);
        add_cong(lm, 0, m1, p2, m2, p1);

        if (cr.o != 0) {
            if (!ck_dr(m1, m2, p1, p2)) {
                int t = p1;
                p1 = p2;
                p2 = t;
            }
            add_ea_pt_t(lm, m1, cr.o, p1, p2, cr.o, m2);
            int t1, t2;
            for (int i = 0; i <= cr.no; i++)
                for (int j = 0; j <= cr.no; j++) {
                    t1 = cr.pt[i];
                    t2 = cr.pt[j];
                    if (t1 != p1 && t1 != m1 && t2 != p2 && t2 != m2)
                        add_ea_pt_t(lm, m1, t1, p1, p2, t2, m2);
                }
        }
        pop_codb();
        pop_codb();
    }

    /**
     * Applies type 3 processing by adding cyclic, midpoint, and angle conditions based on the given markers and points.
     *
     * @param m the primary marker
     * @param m2 the marker for the first midpoint condition
     * @param p2 the point used in cyclic and midpoint conditions
     * @param m3 the marker for the second midpoint condition
     * @param p3 the point used in cyclic and midpoint conditions
     */
    final void add_type3(int m, int m2, int p2, int m3, int p3) {
        int p1;
        add_codb(CO_CYCLIC, m, p2, p3, 0, 0, 0, 0, 0);
        add_codb(CO_MIDP, m2, m, p2, 0, 0, 0, 0, 0);
        add_codb(CO_MIDP, m3, m, p3, 0, 0, 0, 0, 0);
        add_cir4(61, 0, m2, m3, p2, p3);
        p1 = inter_ll(fd_pline(p2, m, p3), fd_pline(p3, m, p2));
        if (p1 != 0) {
            add_ea_pt_t(62, p2, p1, m2, m3, p1, p3);
            add_ea_pt_t(62, p2, m2, p1, p1, m3, p3);
            if (yperp(m, p2, m, p3)) {
                add_ea_pt_t(63, m, p2, m3, p2, p1, m2);
                add_ea_pt_t(63, p2, m3, m, p1, m2, p2);
                pop_codb();
            }
        }
        pop_codb();
        pop_codb();
        pop_codb();
    }

/* Lemmas  */

    /**
     * Searches for perpendicular and parallel relationships between a point-based
     * line and a t-line, and adds corresponding conditions.
     *
     * @param pn the point-based line
     * @param tn the t-line to be analyzed
     */
    final void search_pn_tn(PLine pn, TLine tn) {
        LLine ln, l1, l2;
        int i;
        if (on_pn(tn.l1, pn)) {
            l1 = tn.l2;
            l2 = tn.l1;

        } else if (on_pn(tn.l2, pn)) {
            l1 = tn.l1;
            l2 = tn.l2;
        } else {
            return;
        }
        for (i = 0; i <= pn.no; i++) {
            ln = pn.ln[i];
            if (ln != l2 && !ln_perp(l1, ln)) ;
            {
                add_codb(CO_PERP, l1.pt[0], l1.pt[1], l2.pt[0], l2.pt[1], 0, 0, 0, 0);
                if (ln == null) {
                    // TODO. Handle this.
                    System.err.println("ln is null");
                    return;
                }

                add_codb(CO_PARA, ln.pt[0], ln.pt[1], l2.pt[0], l2.pt[1], 0, 0, 0, 0);
                add_tline(R_PT_T, ln.pt[0], ln.pt[1], l1.pt[0], l1.pt[1]);
                pop_codb();
                pop_codb();
            }
        }
    }

    /**
     * Establishes a midpoint connection based on a midpoint object and two lines,
     * adding necessary conditions if valid.
     *
     * @param md the midpoint instance containing connection data
     * @param l1 the first line for connection
     * @param l2 the second line for connection
     */
    final void lm_md_connection(MidPt md, LLine l1, LLine l2) {
        int lm = R_MID_CONNECTION;
        if (!valid(lm)) return;

        int p1, p2, k, m1, m2;
        LLine n;
        if (on_ln(md.m, l1)) {
            n = l1;
            l1 = l2;
            l2 = n;
        }
        if (on_ln(md.m, l2)) {
            if (on_ln(md.a, l1)) {
                m1 = md.a;
                m2 = md.b;
            } else if (on_ln(md.b, l1)) {
                m1 = md.b;
                m2 = md.a;
            } else
                return;
            for (k = 0; k <= l2.no; k++) {
                p2 = l2.pt[k];
                if (p2 != md.m && (p1 = inter_ll(l1, fd_ln(p2, m2))) != 0 && p1 != p2) {
                    add_codb(CO_PARA, p1, m1, p2, md.m, 0, 0, 0, 0);
                    add_mid(lm, p2, p1, m2);
                    add_cong(lm, 0, p2, p1, p2, m2);
                    pop_codb();
                }
            }
        }
    }

    /**
     * Checks if the given integer meets connection criteria based on internal state.
     *
     * @param n the value to be checked
     * @return true if the condition is met, false otherwise
     */
    boolean ch_it(int n) {
        return ((d_base != 0) ? (n != 0) : ((n) == 1));

    }

    /**
     * Constructs a parallelogram configuration based on a midpoint and two lines,
     * adding conditions and lines if the configuration is valid.
     *
     * @param md the midpoint used for establishing the parallelogram
     * @param l1 the first line of the configuration
     * @param l2 the second line of the configuration
     */
    final void lm_parallelogram(MidPt md, LLine l1, LLine l2) {

        if (!valid(R_PARALLELOGRAM)) return;

        if (l1 == l2) return;

        LLine l0 = fd_ln(md.a, md.b);
        if (on_ln(md.a, l2)) {
            LLine n = l1;
            l1 = l2;
            l2 = n;
        }
        if (on_ln(md.a, l1) && on_ln(md.b, l2)) {

            {
                PLine pn = all_pn.nx;
                while (pn != null) {
                    if (!ch_it(pn.type) || pn.no == 0 || on_pn(l1, pn) || on_pn(l0, pn)) {
                        pn = pn.nx;
                        continue;
                    }
                    for (int k = 0; k <= pn.no; k++)
                        for (int l = k + 1; l <= pn.no; l++) {
                            LLine l3 = pn.ln[k];
                            LLine l4 = pn.ln[l];
                            if (on_ln(md.a, l4)) {
                                LLine n = l3;
                                l3 = l4;
                                l4 = n;
                            }
                            int p1, p2;
                            if (l3 != l4 && on_ln(md.a, l3) && on_ln(md.b, l4) &&
                                    (p1 = inter_ll(l2, l3)) != 0 && p1 != md.a && p1 != md.b &&
                                    (p2 = inter_ll(l1, l4)) != 0 && p2 != md.a && p2 != md.b && p1 != p2) {
                                add_codb(CO_PARA, p1, md.a, p2, md.b, 0, 0, 0, 0);
                                add_codb(CO_PARA, p1, md.b, p2, md.a, 0, 0, 0, 0);
                                add_line(R_PARALLELOGRAM, md.m, p1, p2);//  @@
                                add_mid(R_PARALLELOGRAM, md.m, p1, p2);
                                add_cong(R_PARALLELOGRAM, 0, md.m, p1, md.m, p2);
                                pop_codb();
                                pop_codb();
                            }
                        }
                    pn = pn.nx;
                }
            }
        }
    }

    /**
     * Establishes a ratio condition based on intersections of lines defined by points.
     *
     * @param lm the lemma identifier for the ratio condition
     * @param p1 the first point of the first line
     * @param p2 the second point of the first line
     * @param p3 the first point of the second line
     * @param p4 the second point of the second line
     */
    final void lm_RATIO(int lm, int p1, int p2, int p3, int p4) {
        if (!valid(R_RATIO)) return;

        int o;
        o = inter_ll(fd_ln(p1, p3), fd_ln(p2, p4));
        if (o != 0) {
            add_codb(CO_PARA, p1, p2, p3, p4, 0, 0, 0, 0);
            add_ratioo(lm, o, p1, p2, p3, p4);
            pop_codb();
        }
        o = inter_ll(fd_ln(p1, p4), fd_ln(p2, p3));
        if (o != 0) {
            add_codb(CO_PARA, p1, p2, p3, p4, 0, 0, 0, 0);
            add_ratioo(lm, o, p1, p2, p4, p3);
            pop_codb();
        }
    }

    /**
     * Determines whether two condition nodes are equivalent.
     *
     * @param n1 the first condition node
     * @param n2 the second condition node
     * @return true if both nodes are equivalent, false otherwise
     */
    boolean new_eq(Cond n1, Cond n2) {
        return (n1.pred == n2.pred && n1.u.equal(n2.u));
    }

    /**
     * Creates a new condition node with a given predicate value.
     *
     * @param pred the predicate identifier for the condition
     * @return a new condition node instance
     */
    final Cond new_pr(int pred) {
        Cond nd = new Cond();
        nd.pred = pred;
        nd.no = 0;
        nd.nx = null;
        for (int i = 0; i <= 7; i++)
            nd.p[i] = 0;

        if (all_nd.nx == null) {
            last_nd = all_nd;
        }
        last_nd.nx = nd;
        last_nd = nd;
        return nd;
    }

    /**
     * Adjusts the condition list by updating the last condition based on equivalence.
     */
    final void new_ot() {
        int i = 0;
        Cond nd1, nd = all_nd.nx;
        nd1 = null;
        if (nd == null) return;
        while (nd.nx != null) {
            if (new_eq(nd, last_nd)) i = 1;
            nd1 = nd;
            nd = nd.nx;
        }
        if (i == 1) {
            last_nd = nd1;
        }
    }

    /**
     * Creates and adds a new condition node with specified properties.
     *
     * @param n the predicate or condition type
     * @param p1 the first parameter
     * @param p2 the second parameter
     * @param p3 the third parameter
     * @param p4 the fourth parameter
     * @param p5 the fifth parameter
     * @param p6 the sixth parameter
     * @param p7 the seventh parameter
     * @param p8 the eighth parameter
     * @return the newly created condition node
     */
    Cond add_codb(int n, int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8) {

//        if(p1 == 1 && p2 ==4 && p3 == 1 && p4 == 9)
//        {
//            int k = 0;
//        }
        Cond co = new Cond();

        co.pred = n;
        co.no = 0;
        co.u.ln = null;
        co.p[0] = p1;
        co.p[1] = p2;
        co.p[2] = p3;
        co.p[3] = p4;
        co.p[4] = p5;
        co.p[5] = p6;
        co.p[6] = p7;
        co.p[7] = p8;

        if (n == CO_TANG && p3 == p4) {
            int k = 0;
        }

        if (n == CO_PARA && xcoll4(p1, p2, p3, p4)) n = CO_COLL;
        if (n == CO_COLL) {
            int i, j, k = 0;
            for (i = 1; i <= 5; i++) {
                boolean gt = false;
                for (j = 0; j <= k; j++)
                    if (co.p[j] == co.p[i]) {
                        gt = true;
                        j = 1;
                        break;
                    }

                if (gt != true)
                    if (co.p[i] != 0) {
                        k++;
                        co.p[k] = co.p[i];
                    }
                j = 1;
            }

            for (i = k + 1; i <= 5; i++) co.p[i] = 0;
            if (k <= 1) co.pred = 0;
        }

        co.nx = co_db.nx;
        if (co.pred != 0) {
            co.nx = co_db.nx;
            co_db.nx = co;
        }
        return (co);

    }

    /**
     * Removes the last added condition node from the condition database.
     */
    final void pop_codb() {
        Cond co = co_db.nx;
        if (co != null) {
            co_db.nx = co.nx;
        } /* free((cond *)co);  */
    }

    /**
     * Creates and adds a new coordinate condition node with the specified predicate.
     *
     * @param n the predicate for the coordinate condition
     * @return the newly created coordinate condition node
     */
    Cond add_coxy(int n) {
        Cond co = new Cond();
        co.pred = n;
        co.p[0] = 0;
        co.p[1] = 0;
        co.p[2] = 0;
        co.p[3] = 0;
        co.p[4] = 0;
        co.p[5] = 0;
        co.p[6] = 0;
        co.p[7] = 0;
        co.nx = co_xy.nx;
        co_xy.nx = co;
        return (co);
    }

    /**
     * Creates and adds a new parallel condition node associated with the given predicted line.
     *
     * @param pn the predicted line to be associated with the parallel condition
     */
    final void new_para(PLine pn) {
        Cond nd = new Cond();
        nd.pred = CO_PARA;
        nd.u.pn = pn;
        for (int i = 0; i <= 7; i++) nd.p[i] = 0;
        nd.nx = all_nd.nx;
        all_nd.nx = nd;
        if (nd.nx == null) {
            last_nd = nd;
        }
    }

    /**
     * Checks if the three specified points are collinear.
     *
     * @param p1 the first point
     * @param p2 the second point
     * @param p3 the third point
     * @return true if the points are collinear, false otherwise
     */
    final boolean xcoll(int p1, int p2, int p3) {
        LLine ln;

        if (!check_coll(p1, p2, p3)) return false;

        if (p1 == p2 || p1 == p3 || p2 == p3) return (true);
        ln = fo_ln3(p1, p2, p3);
        return (ln != null);
    }

    /**
     * Finds and returns the line that contains the three specified points, if such a line exists.
     *
     * @param p1 the first point
     * @param p2 the second point
     * @param p3 the third point
     * @return the line (LLine) containing the points, or null if none exists
     */
    final LLine fo_ln3(int p1, int p2, int p3) {
        LLine ln;
        ln = all_ln.nx;
        while (ln != null) {
            if (ln.no > 1 && on_ln(p1, ln) && on_ln(p2, ln) && on_ln(p3, ln)) {
                return (ln);
            }
            ln = ln.nx;
        }
        return (null);
    }

    /**
     * Finds and returns a line that passes through all specified non-zero points in the array.
     *
     * @param ps an array of points
     * @param n the number of indices to check in the array
     * @return the line (LLine) if found, or null otherwise
     */
    final LLine fo_ln(int ps[], int n)   ///xxxxx
    {
        LLine ln;
        int i;
        ln = all_ln.nx;
        while (ln != null) {
            if (ln.no <= 1) {
                ln = ln.nx;
                continue;
            }
            for (i = 0; i <= n; i++) {
                if (ps[i] != 0 && !on_ln(ps[i], ln))
                    break;
            }
            if (i == n + 1) return (ln);

            ln = ln.nx;
        }
        return (null);
    }

    /**
     * Checks if the four points satisfy collinearity conditions.
     * If the first two points are equal, it delegates to xcoll; otherwise, it ensures collinearity for both
     * subsets.
     *
     * @param p1 the first point
     * @param p2 the second point
     * @param p3 the third point
     * @param p4 the fourth point
     * @return true if the relevant points are collinear, false otherwise
     */
    final boolean xcoll4(int p1, int p2, int p3, int p4) {
        if (p1 == p2) return (xcoll(p2, p3, p4));
        return (xcoll(p1, p2, p3) && xcoll(p1, p2, p4));
    }

    /**
     * Determines whether the two lines defined by the point pairs (p1, p2) and (p3, p4) are parallel.
     *
     * @param p1 the first point of the first line
     * @param p2 the second point of the first line
     * @param p3 the first point of the second line
     * @param p4 the second point of the second line
     * @return true if the lines are parallel, false otherwise
     */
    final boolean xpara(int p1, int p2, int p3, int p4) {

        if (p1 == p2 || p3 == p4 || xcoll4(p1, p2, p3, p4)) return (true);
        return (fo_pn1(p1, p2, p3, p4) != null);
    }

    /**
     * Determines whether the two specified lines are parallel.
     *
     * @param l1 the first line
     * @param l2 the second line
     * @return true if the lines are parallel, false otherwise
     */
    final boolean ln_para(LLine l1, LLine l2) {
        PLine pn;
        if (l1 == l2) return (true);
        if (l1 == null) {
            // TODO. Handle this.
            System.err.println("l1 is null");
            return false;
        }
        if (l2 == null) {
            // TODO. Handle this.
            System.err.println("l2 is null");
            return false;
        }

        if ((l1.type == 0 || l2.type == 0) && xcoll_ln(l1, l2)) return true;
        if (l1.type == 0)
            l1 = fd_lnl(l1);
        if (l2.type == 0)
            l2 = fd_lnl(l2);
        pn = all_pn.nx;
        while (pn != null) {
            if (on_pn(l1, pn) && on_pn(l2, pn)) return (true);
            pn = pn.nx;
        }
        return (false);
    }

    /**
     * Checks if the two lines defined by the points (p1, p2) and (p3, p4) are perpendicular.
     *
     * @param p1 the first point of the first line
     * @param p2 the second point of the first line
     * @param p3 the first point of the second line
     * @param p4 the second point of the second line
     * @return true if the lines are perpendicular, false otherwise
     */
    final boolean xperp(int p1, int p2, int p3, int p4) {
        if (p1 == p2 || p3 == p4) return (true);
        return fo_tn1(p1, p2, p3, p4) != null;
    }

    /**
     * Determines whether the two specified lines are perpendicular.
     *
     * @param l1 the first line
     * @param l2 the second line
     * @return true if the lines are perpendicular, false otherwise
     */
    final boolean ln_perp(LLine l1, LLine l2) {
        if (l1 == l2) return false;
        if (l1 == null) {
            // TODO. Handle this.
            System.err.println("l1 is null");
            return false;
        }
        if (l1.type == 0)
            l1 = fd_lnl(l1);
        if (l2 == null) {
            // TODO. Handle this.
            System.err.println("l2 is null");
            return false;
        }

        if (l2.type == 0)
            l2 = fd_lnl(l2);

        TLine tn = all_tn.nx;
        while (tn != null) {
            if ((l1 == tn.l1 && l2 == tn.l2) || (l1 == tn.l2 && l2 == tn.l1)) return (true);
            tn = tn.nx;
        }
        return (false);
    }

    /**
     * Searches for an existing circle that is a sub-circle of the given circle.
     *
     * @param c1 the reference circle (ACir) to compare against
     * @return an existing circle that matches, or null if none is found
     */
    final ACir xcir(ACir c1) {
        ACir c2;
        c2 = all_cir.nx;
        while (c2 != null) {
            if (sub_cir(c1, c2)) return (c2);
            c2 = c2.nx;
        }
        return (null);
    }

    /**
     * Finds and returns a circle based on a specified property and four points.
     *
     * @param o the circle property or type indicator
     * @param p1 the first point
     * @param p2 the second point
     * @param p3 the third point
     * @param p4 the fourth point
     * @return the found circle (ACir) or null if none exists
     */
    final ACir fo_cr(int o, int p1, int p2, int p3, int p4) {
        test_c.o = o;
        test_c.no = 3;
        test_c.pt[0] = p1;
        test_c.pt[1] = p2;
        test_c.pt[2] = p3;
        test_c.pt[3] = p4;
        return (xcir(test_c));
    }

    /**
     * Checks for the existence of a circle defined by the given property and two points.
     *
     * @param o the circle property or type indicator
     * @param p1 the first point
     * @param p2 the second point
     * @return true if such a circle exists, false otherwise
     */
    final boolean xcir2(int o, int p1, int p2) {
        test_c.o = o;
        test_c.no = 1;
        test_c.pt[0] = p1;
        test_c.pt[1] = p2;
        return (xcir(test_c) != null);
    }

    /**
     * Checks for the existence of a circle defined by the given property and three points.
     *
     * @param o the circle property or type indicator
     * @param p1 the first point
     * @param p2 the second point
     * @param p3 the third point
     * @return true if such a circle exists, false otherwise
     */
    final boolean xcir3(int o, int p1, int p2, int p3) {
        test_c.o = o;
        test_c.no = 2;
        test_c.pt[0] = p1;
        test_c.pt[1] = p2;
        test_c.pt[2] = p3;
        return (xcir(test_c) != null);
    }

    /**
     * Checks for the existence of a circle defined by the given property and four points.
     *
     * @param o the circle property or type indicator
     * @param p1 the first point
     * @param p2 the second point
     * @param p3 the third point
     * @param p4 the fourth point
     * @return true if such a circle exists, false otherwise
     */
    final boolean xcir4(int o, int p1, int p2, int p3, int p4) {
        test_c.o = o;
        test_c.no = 3;
        test_c.pt[0] = p1;
        test_c.pt[1] = p2;
        test_c.pt[2] = p3;
        test_c.pt[3] = p4;
        return (xcir(test_c) != null);
    }

    /**
     * Checks if a midpoint exists for the specified segment within the given model.
     *
     * @param m the model identifier
     * @param a the first endpoint of the segment
     * @param b the second endpoint of the segment
     * @return true if a midpoint exists for the segment, false otherwise
     */
    final boolean xmid(int m, int a, int b) {
        return (fo_md(m, a, b) != null);
    }

    /**
     * Searches for and returns a midpoint corresponding to the specified model and endpoints.
     *
     * @param m the model identifier
     * @param a the first endpoint
     * @param b the second endpoint
     * @return the midpoint (MidPt) if found, or null otherwise
     */
    final MidPt fo_md(int m, int a, int b) {
        MidPt md = all_md.nx;
        while (md != null) {
            if (m == md.m &&
                    ((a == md.a && b == md.b) || (a == md.b && b == md.a))) {
                return (md);
            }
            md = md.nx;
        }
        return (null);
    }

    /**
     * Searches for and returns a midpoint defined by the two specified endpoints.
     *
     * @param a the first endpoint
     * @param b the second endpoint
     * @return the midpoint (MidPt) if one exists, or null otherwise
     */
    final MidPt fo_md(int a, int b) {
        MidPt md = all_md.nx;
        while (md != null) {
            if (((a == md.a && b == md.b) || (a == md.b && b == md.a))) {
                return (md);
            }
            md = md.nx;
        }
        return (null);
    }

    /**
     * Determines if the angles defined by points (a, b, c) and (p, q, r) are congruent.
     *
     * @param a the first point of the first angle
     * @param b the vertex of the first angle
     * @param c the third point of the first angle
     * @param p the first point of the second angle
     * @param q the vertex of the second angle
     * @param r the third point of the second angle
     * @return true if the angles are congruent, false otherwise
     */
    final boolean xacong(int a, int b, int c, int p, int q, int r) {
        if (!check_eqangle(a, b, c, p, q, r)) {
            return false;
        }

        LLine l1, l2, l3, l4;
        l1 = fd_ln(a, b);
        l2 = fd_ln(b, c);
        l3 = fd_ln(p, q);
        l4 = fd_ln(q, r);
        if (l1 == null || l2 == null || l3 == null || l4 == null) return (false);
        return (ln_acong(l1, l2, l3, l4));
    }

    /**
     * Determines if the angles defined by points (a, b, c, d) and (p, q, r, s) are congruent.
     *
     * @param a the first point of the first angle
     * @param b the second point of the first angle
     * @param c the third point of the first angle
     * @param d the fourth point of the first angle
     * @param p the first point of the second angle
     * @param q the second point of the second angle
     * @param r the third point of the second angle
     * @param s the fourth point of the second angle
     * @return true if the angles are congruent, false otherwise
     */
    final boolean xacong(int a, int b, int c, int d, int p, int q, int r, int s) {
        if (!check_eqangle(a, b, c, d, p, q, r, s)) return false;

        LLine l1, l2, l3, l4;
        l1 = fd_ln(a, b);
        l2 = fd_ln(c, d);
        l3 = fd_ln(p, q);
        l4 = fd_ln(r, s);
        if (l1 == null || l2 == null || l3 == null || l4 == null) return (false);
        return (ln_acong(l1, l2, l3, l4));
    }

    /**
     * Compares two lines based on their unique identifiers.
     *
     * @param l1 the first line to compare
     * @param l2 the second line to compare
     * @return 1 if l1 has a greater id than l2, 0 if they are equal, -1 if l1 has a smaller id than l2
     */
    final int line_compare(LLine l1, LLine l2) {
        if (l1 == null) {
            // TODO. Handle this.
            System.err.println("l1 is null");
            return 0;
        }
        if (l2 == null) {
            // TODO. Handle this.
            System.err.println("l2 is null");
            return 0;
        }
        if (l1.id > l2.id) return 1;
        if (l1.id == l2.id) return 0;
        if (l1.id < l2.id) return -1;
        return 0;
    }

    /**
     * Retrieves the angle set associated with the given four lines.
     * Depending on the current state, a different method is used to generate the angle set.
     *
     * @param l1 the first line
     * @param l2 the second line
     * @param l3 the third line
     * @param l4 the fourth line
     * @return the set of angles (Angles) if found, or null otherwise
     */
    final Angles fo_las(LLine l1, LLine l2, LLine l3, LLine l4) {
        Angles as;
        if (isPFull())
            as = fo_las0(l1, l2, l3, l4);
        else
            as = fo_las_t(l1, l2, l3, l4);
        return as;
    }

    /**
     * Determines the angle set for the provided four lines based on available line ordering and comparisons.
     *
     * @param l1 the first line
     * @param l2 the second line
     * @param l3 the third line
     * @param l4 the fourth line
     * @return the angle set (Angles) if found, or null otherwise
     */
    final Angles fo_las0(LLine l1, LLine l2, LLine l3, LLine l4)
    {
        LLine n1, n2;
        Angles as;
        if (l1 == l2 || l3 == l4) {
            LLine t = l2;
            l2 = l3;
            l3 = t;

        }
        if (l1 == l4) {
            LLine t = l1;
            l1 = l3;
            l3 = t;
            t = l2;
            l2 = l4;
            l4 = t;
        }
        if (l2 == l3) {
            if (line_compare(l1, l4) > 0) {
                n1 = l4;
                l4 = l1;
                l1 = n1;
            }
        } else if (l1 == l3) {
            if (line_compare(l2, l4) > 0) {
                n1 = l2;
                l2 = l4;
                l4 = n1;
            }
        } else if (l2 == l4) {
            n1 = l1;
            l1 = l2;
            l2 = n1;
            n1 = l3;
            l3 = l4;
            l4 = n1;
            if (line_compare(l2, l4) > 0) {
                n1 = l2;
                l2 = l4;
                l4 = n1;
            }
        } else {
            if (line_compare(l1, l2) < 0) n1 = l1;
            else n1 = l2;
            if (line_compare(l3, l4) < 0) n2 = l3;
            else n2 = l4;
            if (line_compare(n1, n2) > 0) {
                n1 = l3;
                l3 = l1;
                l1 = n1;
                n1 = l2;
                l2 = l4;
                l4 = n1;
            }
            if (line_compare(l1, l2) > 0) {
                n1 = l1;
                l1 = l2;
                l2 = n1;
                n1 = l3;
                l3 = l4;
                l4 = n1;
            }
        }
        as = all_as.nx;
        while (as != null) {
            if (l1 == as.l1 && l4 == as.l4 && ((l2 == as.l2 && l3 == as.l3) || (l2 == as.l3 && l3 == as.l2)))
                return (as);
            as = as.nx;
        }
        return (null);
    }

    /**
     * Determines if the angles defined by (l1, l2) and (l3, l4) are congruent.
     *
     * @param l1 first line of the first angle
     * @param l2 second line of the first angle
     * @param l3 first line of the second angle
     * @param l4 second line of the second angle
     * @return true if the angles are congruent, false otherwise
     */
    final boolean ln_acong(LLine l1, LLine l2, LLine l3, LLine l4) {
        if (l1 == null || l2 == null || l3 == null || l4 == null) {
            int k = 0;
        }
        if (l1 == l3 && l2 == l4) return (true);
        if (l1 == l2 || l3 == l4) return true;
        if (l1.type == 0)
            l1 = fd_lnl(l1);
        if (l2.type == 0)
            l2 = fd_lnl(l2);
        if (l3.type == 0)
            l3 = fd_lnl(l3);
        if (l4.type == 0)
            l4 = fd_lnl(l4);

        if (l1 == l3 && l2 == l4) return (true);
        if (l1 == l2 || l3 == l4) return true;
        if (ln_para(l1, l2) && ln_para(l3, l4)) return (true);
        if (ln_perp(l1, l2) && ln_perp(l3, l4)) return (true);
        return (fo_las(l1, l2, l3, l4) != null);
    }

    /**
     * Checks whether the segments defined by endpoints a, b and p, q are congruent.
     *
     * @param a first endpoint of the first segment
     * @param b second endpoint of the first segment
     * @param p first endpoint of the second segment
     * @param q second endpoint of the second segment
     * @return true if the segments are congruent, false otherwise
     */
    final boolean xcong(int a, int b, int p, int q) {
        if (!check_eqdistance(a, b, p, q)) return false;

        CongSeg cg;
        int o;
        if (a > b) {
            o = a;
            a = b;
            b = o;
        }
        if (p > q) {
            o = p;
            p = q;
            q = o;
        }
        if (a > p || (a == b && b > q)) {
            o = a;
            a = p;
            p = o;
            o = b;
            b = q;
            q = o;
        }

        if (a == p && b == q) return (true);

        cg = all_cg.nx;
        while (cg != null) {
            if (cg.type != 0 && on_cg(a, b, p, q, cg) && cg.t1 == cg.t2) return (true);
            cg = cg.nx;
        }

        return (false);
    }

    /**
     * Determines if the segments (a, b) and (p, q) are congruent using specific angle values.
     *
     * @param a first endpoint of the first segment
     * @param b second endpoint of the first segment
     * @param p first endpoint of the second segment
     * @param q second endpoint of the second segment
     * @param t1 first angle factor for congruency check
     * @param t2 second angle factor for congruency check
     * @return true if the segments are congruent based on the provided ratios, false otherwise
     */
    final boolean xcong1(int a, int b, int p, int q, int t1, int t2) {
        CongSeg cg;
        int o;
        if (a > b) {
            o = a;
            a = b;
            b = o;
        }
        if (p > q) {
            o = p;
            p = q;
            q = o;
        }
        if (a > p || (a == p && b > q)) {
            o = a;
            a = p;
            p = o;
            o = b;
            b = q;
            q = o;
            o = t1;
            t1 = t2;
            t2 = o;
        }

        if (a == p && b == q) return (true);

        cg = all_rg.nx;
        while (cg != null) {
            if (cg.type != 0 && on_cg(a, b, p, q, cg)) break;
            cg = cg.nx;
        }
        if (cg == null) return false;
        if (cg.p1 == a && cg.p2 == b || cg.p2 == a && cg.p1 == b)
            return cg.t1 * t2 == cg.t2 * t1;
        else
            return cg.t1 * t1 == cg.t2 * t2;
    }

    /**
     * Checks if the segments (a, b) and (c, d) are congruent using both default and extended methods.
     *
     * @param a first endpoint of the first segment
     * @param b second endpoint of the first segment
     * @param c first endpoint of the second segment
     * @param d second endpoint of the second segment
     * @return true if the segments are congruent by any method, false otherwise
     */
    final boolean xcong_all(int a, int b, int c, int d) {
        return xcong(a, b, c, d) || xcong1(a, b, c, d);
    }

    /**
     * Checks whether the segments defined by endpoints a, b and p, q are congruent using an alternative ratio approach.
     *
     * @param a first endpoint of the first segment
     * @param b second endpoint of the first segment
     * @param p first endpoint of the second segment
     * @param q second endpoint of the second segment
     * @return true if the segments are congruent, false otherwise
     */
    final boolean xcong1(int a, int b, int p, int q) {
        CongSeg cg;
        int o;
        if (a > b) {
            o = a;
            a = b;
            b = o;
        }
        if (p > q) {
            o = p;
            p = q;
            q = o;
        }
        if (a > p || (a == p && b > q)) {
            o = a;
            a = p;
            p = o;
            o = b;
            b = q;
            q = o;
        }

        if (a == p && b == q) return (true);

        cg = all_rg.nx;
        while (cg != null) {
            if (cg.type != 0 && on_cg(a, b, p, q, cg)) return (true);
            cg = cg.nx;
        }
        return (false);
    }

    /**
     * Searches for a congruent segment in the ratio group for segments (a, b) and (p, q).
     *
     * @param a first endpoint of the first segment
     * @param b second endpoint of the first segment
     * @param p first endpoint of the second segment
     * @param q second endpoint of the second segment
     * @return the congruent segment if found, null otherwise
     */
    final CongSeg fo_rg1(int a, int b, int p, int q) {
        CongSeg cg = all_rg.nx;
        while (cg != null) {
            if (cg.type != 0 && on_cg(a, b, p, q, cg)) return (cg);
            cg = cg.nx;
        }
        return null;
    }

    /**
     * Returns the first matching congruent segment from the ratio list based on endpoints (a, b) and (p, q).
     *
     * @param a first endpoint of the first segment
     * @param b second endpoint of the first segment
     * @param p first endpoint of the second segment
     * @param q second endpoint of the second segment
     * @return the congruent segment if present; null otherwise
     */
    final CongSeg fo_cg1(int a, int b, int p, int q) {
        CongSeg cg;
        int o;
        if (a > b) {
            o = a;
            a = b;
            b = o;
        }
        if (p > q) {
            o = p;
            p = q;
            q = o;
        }
        if (a == p && b == q) return (null);
        cg = all_rg.nx;
        while (cg != null) {
            if (on_cg(a, b, cg) && on_cg(p, q, cg)) return (cg);
            cg = cg.nx;
        }
        return (null);
    }

    /**
     * Retrieves a congruent segment from the common group based on endpoints (a, b) and (p, q).
     *
     * @param a first endpoint of the first segment
     * @param b second endpoint of the first segment
     * @param p first endpoint of the second segment
     * @param q second endpoint of the second segment
     * @return the congruent segment if found, null otherwise
     */
    final CongSeg fo_cg(int a, int b, int p, int q) {
        CongSeg cg;
        int o;
        if (a > b) {
            o = a;
            a = b;
            b = o;
        }
        if (p > q) {
            o = p;
            p = q;
            q = o;
        }
        if (a == p && b == q) return (null);
        cg = all_cg.nx;
        while (cg != null) {
            if (on_cg(a, b, cg) && on_cg(p, q, cg)) return (cg);
            cg = cg.nx;
        }
        return (null);
    }

    /**
     * Determines whether the triangles defined by points (a, b, c) and (p, q, r) are similar.
     *
     * @param a first vertex of the first triangle
     * @param b second vertex of the first triangle
     * @param c third vertex of the first triangle
     * @param p first vertex of the second triangle
     * @param q second vertex of the second triangle
     * @param r third vertex of the second triangle
     * @return true if the triangles are similar, false otherwise
     */
    boolean xsim_tri(int a, int b, int c, int p, int q, int r) {
        if (!check_simtri(a, b, c, p, q, r)) return false;

        return (fo_st(1, 0, a, b, c, p, q, r) != null);
    }

    /**
     * Determines whether the triangles defined by points (a, b, c) and (p, q, r) are congruent.
     *
     * @param a first vertex of the first triangle
     * @param b second vertex of the first triangle
     * @param c third vertex of the first triangle
     * @param p first vertex of the second triangle
     * @param q second vertex of the second triangle
     * @param r third vertex of the second triangle
     * @return true if the triangles are congruent, false otherwise
     */
    boolean xcon_tri(int a, int b, int c, int p, int q, int r) {
        if (!this.check_simtri(a, b, c, p, q, r)) return false;
        return (fo_st(0, 0, a, b, c, p, q, r) != null);
    }

    /**
     * Finds a similar or congruent triangle structure based on the provided vertices.
     *
     * @param xsim_2 flag to indicate extended similarity testing
     * @param xsim_1 flag to optionally bypass congruency type check
     * @param a first vertex of the first triangle
     * @param b second vertex of the first triangle
     * @param c third vertex of the first triangle
     * @param p first vertex of the second triangle
     * @param q second vertex of the second triangle
     * @param r third vertex of the second triangle
     * @return the triangle structure if found, null otherwise
     */
    SimTri fo_st(int xsim_2, int xsim_1, int a, int b, int c, int p, int q, int r) {
        SimTri st = (xsim_2 != 0) ? all_st.nx : all_ct.nx; //???
        while (st != null) {
            if ((xsim_1 != 0 || st.type != 0) &&
                    ((on_st(a, b, c, p, q, r, st))))
                return (st);
            st = st.nx;
        }
        return (null);
    }

    /**
     * Checks if the ratio of lengths between segments (a, b) and (c, d) equals that between (p, q) and (r, s).
     *
     * @param a first endpoint of the first segment
     * @param b second endpoint of the first segment
     * @param c first endpoint of the second segment
     * @param d second endpoint of the second segment
     * @param p first endpoint of the third segment
     * @param q second endpoint of the third segment
     * @param r first endpoint of the fourth segment
     * @param s second endpoint of the fourth segment
     * @return true if the ratios are equal, false otherwise
     */
    boolean xeq_ratio(int a, int b, int c, int d, int p, int q, int r, int s) {
        if ((xcong(a, b, p, q) && xcong(c, d, r, s)) ||
                (xcong(a, b, c, d) && xcong(p, q, r, s)) ||
                (xcong(a, b, r, s) && xcong(c, d, p, q)))
            return (true);
        return (fo_ra(a, b, c, d, p, q, r, s) != null);
    }

    /**
     * Finds and returns the ratio segment structure that matches the given endpoints.
     *
     * @param a first endpoint of the first segment
     * @param b second endpoint of the first segment
     * @param c first endpoint of the second segment
     * @param d second endpoint of the second segment
     * @param p first endpoint of the third segment
     * @param q second endpoint of the third segment
     * @param r first endpoint of the fourth segment
     * @param s second endpoint of the fourth segment
     * @return the matching ratio segment structure if found, null otherwise
     */
    RatioSeg fo_ra(int a, int b, int c, int d, int p, int q, int r, int s) {
        RatioSeg ra = all_ra.nx;
        for (; ra != null; ra = ra.nx) {
            if ((((xcong(a, b, ra.r[1], ra.r[2]) && xcong(r, s, ra.r[7], ra.r[8])) ||
                    (xcong(a, b, ra.r[7], ra.r[8]) && xcong(r, s, ra.r[1], ra.r[2]))) &&
                    ((xcong(c, d, ra.r[3], ra.r[4]) && xcong(p, q, ra.r[5], ra.r[6])) ||
                            (xcong(c, d, ra.r[5], ra.r[6]) && xcong(p, q, ra.r[3], ra.r[4])))) ||
                    (((xcong(a, b, ra.r[3], ra.r[4]) && xcong(r, s, ra.r[5], ra.r[6])) ||
                            (xcong(a, b, ra.r[5], ra.r[6]) && xcong(r, s, ra.r[3], ra.r[4]))) &&
                            ((xcong(c, d, ra.r[1], ra.r[2]) && xcong(p, q, ra.r[7], ra.r[8])) ||
                                    (xcong(c, d, ra.r[7], ra.r[8]) && xcong(p, q, ra.r[1], ra.r[2])))))
                return (ra);
        }
        return (null);
    }

    /**
     * Compares two lines based on their primary endpoints.
     *
     * @param l1 the first line to compare
     * @param l2 the second line to compare
     * @return true if l1 is considered less than l2, false otherwise
     */
    boolean ln_less(LLine l1, LLine l2) {
        if (l1 == l2) return (false);
        if (l1.pt[1] < l2.pt[1]) return (true);
        if (l1.pt[1] > l2.pt[1]) return (false);
        if (l1.pt[0] < l2.pt[0]) return (true);
        return (false);
    }

    /**
     * Compares two pairs of lines to determine order.
     *
     * @param l1 first line of the first pair
     * @param l2 second line of the first pair
     * @param l3 first line of the second pair
     * @param l4 second line of the second pair
     * @return true if the second pair is considered less than the first pair, false otherwise
     */
    boolean l2_less(LLine l1, LLine l2, LLine l3, LLine l4) {
        LLine ln;
        if (ln_less(l2, l1)) {
            ln = l1;
            l1 = l2;
            l2 = ln;
        }
        if (ln_less(l4, l3)) {
            ln = l3;
            l3 = l4;
            l4 = ln;
        }
        if (ln_less(l2, l4)) return (true);
        if (l2 == l4 && ln_less(l1, l3)) return (true);
        return (false);
    }

    /**
     * Returns the third point of circle c1 that is not equal to p1, p2, or p3.
     *
     * @param c1 the circle to examine
     * @param p1 the first point to exclude
     * @param p2 the second point to exclude
     * @param p3 the third point to exclude
     * @return the point number that is different from p1, p2, and p3, or 0 if not found
     */
    int get_cpt3(ACir c1, int p1, int p2, int p3) {
        char j;
        for (j = 0; j <= c1.no; j++) {
            if (c1.pt[j] != p1 && c1.pt[j] != p2 && c1.pt[j] != p3)
                return (c1.pt[j]);
        }
        return (0);
    }

    /**
     * Finds the intersection point of two lines if one exists.
     *
     * @param l1 the first line
     * @param l2 the second line
     * @return the intersection point number if found, 0 otherwise
     */
    int inter_ll(LLine l1, LLine l2) {
        if (l1 == null || l2 == null || l1 == l2) return (0);
        LLine ln1, ln2;
        ln1 = ln2 = null;

        if (l1.type == 0) ln1 = fd_ln(l1.pt[0], l1.pt[1]);
        if (l2.type == 0) ln2 = fd_ln(l2.pt[0], l2.pt[1]);
        if (l1 == l2) return 0;
        if (l1 == null || l2 == null) {
            int k = 0;
        }
        if (ln1 != null)
            l1 = ln1;
        if (ln2 != null)
            l2 = ln2;
        for (int i = 0; i <= l1.no; i++)
            for (int j = 0; j <= l2.no; j++) {
                if (l1.pt[i] == l2.pt[j]) return (l1.pt[i]);
            }
        return (0);
    }

    /**
     * Finds the intersection of two lines that is different from a given point.
     *
     * @param l1 the first line
     * @param l2 the second line
     * @param p1 the point to exclude from the intersection result
     * @return the intersection point number if found and not equal to p1, 0 otherwise
     */
    int inter_ll1(LLine l1, LLine l2, int p1) {
        char i, j;
        if (l1 == l2) return (0);
        if (l1.type == 0) l1 = fd_ln(l1.pt[0], l1.pt[1]);
        if (l2.type == 0) l2 = fd_ln(l2.pt[0], l2.pt[1]);
        for (i = 0; i <= l1.no; i++)
            for (j = 0; j <= l2.no; j++) {
                if (l1.pt[i] == l2.pt[j] && !meq_pt(l1.pt[i], p1)) return (l1.pt[i]);
            }
        return (0);
    }

    /**
     * Finds the intersection point between a line and a circle.
     *
     * @param l1 the line to check for intersection
     * @param c1 the circle to check for intersection
     * @return the intersection point number if found, 0 otherwise
     */
    int inter_lc(LLine l1, ACir c1) {
        char i, j;
        if (l1 == null || c1 == null) return (0);
        for (i = 0; i <= l1.no; i++)
            for (j = 0; j <= c1.no; j++) {
                if (l1.pt[i] == c1.pt[j]) return (l1.pt[i]);
            }
        return (0);
    }

    /**
     * Returns the intersection point between a line and a circle, excluding a given point.
     *
     * @param l1 the line to check
     * @param c1 the circle to check
     * @param p1 the point to exclude
     * @return the intersection point or 0 if none
     */
    int inter_lc1(LLine l1, ACir c1, int p1) {

        if (l1 == null || c1 == null) return (0);
        for (int i = 0; i <= l1.no; i++)
            for (int j = 0; j <= c1.no; j++) {
                if (l1.pt[i] == c1.pt[j] && !meq_pt(l1.pt[i], p1)) return (l1.pt[i]);
            }
        return (0);
    }

    /**
     * Returns the intersection point between two circles.
     *
     * @param c1 the first circle
     * @param c2 the second circle
     * @return the intersection point or 0 if none
     */
    int inter_cc(ACir c1, ACir c2) {
        char i, j;
        for (i = 0; i <= c1.no; i++)
            for (j = 0; j <= c2.no; j++) {
                if (c1.pt[i] == c2.pt[j]) return (c1.pt[i]);
            }
        return (0);
    }

    /**
     * Returns the intersection point between two circles, excluding a given point.
     *
     * @param c1 the first circle
     * @param c2 the second circle
     * @param p1 the point to exclude
     * @return the intersection point or 0 if none
     */
    int inter_cc1(ACir c1, ACir c2, int p1) {
        char i, j;
        for (i = 0; i <= c1.no; i++)
            for (j = 0; j <= c2.no; j++) {
                if (c1.pt[i] == c2.pt[j] && !meq_pt(c1.pt[i], p1)) return (c1.pt[i]);
            }
        return (0);
    }

    /**
     * Finds the midpoint index for the segment defined by two points.
     *
     * @param a the first point
     * @param b the second point
     * @return the midpoint index or 0 if not found
     */
    int fd_pt_md(int a, int b) {
        MidPt md = all_md.nx;
        while (md != null) {
            if ((a == md.a && b == md.b) || (a == md.b && b == md.a)) return (md.m);
            md = md.nx;
        }
        return (0);
    }

    /**
     * Adds a midpoint for the segment between two points if valid.
     *
     * @param lm the midpoint lemma or marker
     * @param pt the new midpoint
     * @param p1 the first endpoint
     * @param p2 the second endpoint
     */
    final void add_mid(int lm, int pt, int p1, int p2) {
        MidPt md;
        if (p1 == p2 || !check_mid(pt, p1, p2)) {
            this.add_checkError();
            return;
        }

        if (!xmid(pt, p1, p2)) {
            md = new MidPt();
            md.m = pt;
            md.a = p1;
            md.b = p2;
            md.type = 1;
            md.lemma = lm;
            md.nx = null;
            md.co = co_db.nx;
            last_md.nx = md;
            last_md = md;
            new_pr(CO_MIDP);
            last_nd.u.md = md;
        }
    }

    /**
     * Checks if a point is on the given line.
     *
     * @param p the point to check
     * @param ln the line to inspect
     * @return true if the point exists on the line; false otherwise
     */
    boolean on_ln(int p, LLine ln) {
        int i;
        if (ln == null) return (false);
        for (i = 0; i <= ln.no; i++) if (ln.pt[i] == p) return (true);
        return (false);
    }

    /**
     * Determines if all points of one line are contained within another line.
     *
     * @param l1 the line to check if it is a subline
     * @param l2 the line to be checked against
     * @return true if l1 is a subline of l2; false otherwise
     */
    boolean sub_ln(LLine l1, LLine l2) {
        int i;
        if (l1 == null) {
            // TODO. Handle this.
            System.err.println("l1 is null");
            return false;
        }
        for (i = 0; i <= l1.no; i++) if (!on_ln(l1.pt[i], l2)) return (false);
        return (true);
    }

    /**
     * Adds a point to the line in sorted order.
     *
     * @param p the point to add
     * @param ln the line to which the point is added
     */
    final void add_pt2l(int p, LLine ln) {
        int j, i = 0;
        while (i <= ln.no && ln.pt[i] < p) i++;
        if (i > ln.no) {
            ln.no += 1;
            ln.pt[ln.no] = p;
        } else {
            if (ln.pt[i] == p) return;
            for (j = ln.no; j >= i; j--) {
                ln.pt[j + 1] = ln.pt[j];
            }
            ln.pt[i] = p;
            ln.no += 1;
        }
    }

    /**
     * Finds a finite line that contains the two specified points.
     *
     * @param p1 the first point
     * @param p2 the second point
     * @return the line containing both points or null if not found
     */
    LLine fd_ln(int p1, int p2) {
        LLine ln = all_ln.nx;
        while (ln != null && !(ln.type != 0 && on_ln(p1, ln) && on_ln(p2, ln))) {
            ln = ln.nx;
        }
        return (ln);
    }

    /**
     * Finds a finite line that contains the three specified points.
     *
     * @param p1 the first point
     * @param p2 the second point
     * @param p3 the third point
     * @return the line containing the three points or null if not found
     */
    LLine fd_ln3(int p1, int p2, int p3) {
        LLine ln = all_ln.nx;
        while (ln != null) {
            if (ln.type != 0) {
                if (on_ln(p1, p2, ln) && on_ln(p3, ln))
                    return ln;
            }
            ln = ln.nx;
        }
        return (null);
    }

    /**
     * Retrieves a line from a parallel structure that contains the specified point.
     *
     * @param p1 the point to check
     * @param p2 the first reference point for the parallel line search
     * @param p3 the second reference point for the parallel line search
     * @return the line that contains the point or null if not found
     */
    LLine fd_pline(int p1, int p2, int p3) {
        int i;
        PLine pn;
        pn = fd_pn(p2, p3);
        if (pn == null) return (null);
        for (i = 0; i <= pn.no; i++)
            if (on_ln(p1, pn.ln[i])) return (pn.ln[i]);
        return (null);
    }

    /**
     * Retrieves a transversal line associated with the given points.
     *
     * @param p1 the point to check
     * @param p2 the first reference point for the transversal search
     * @param p3 the second reference point for the transversal search
     * @return the transversal line containing the point or null if not found
     */
    LLine fd_tline(int p1, int p2, int p3) {
        LLine ln;
        TLine tn;
        ln = fd_ln(p2, p3);
        if (ln == null) return (null);
        tn = all_tn.nx;
        while (tn != null) {
            if (tn.type != 0) //??
            {
                if (tn.l1 == ln && on_ln(p1, tn.l2)) return (tn.l2);
                if (tn.l2 == ln && on_ln(p1, tn.l1)) return (tn.l1);
            }
            tn = tn.nx;
        }
        return (null);
    }

    /**
     * Searches for a line within a parallel structure that contains the given point.
     *
     * @param pn the parallel line structure
     * @param p the point to check
     * @return the line containing the point or null if not found
     */
    LLine fd_ln_pn1(PLine pn, int p) {
        int i;
        if (pn == null) return (null);
        for (i = 0; i <= pn.no; i++) if (on_ln(p, pn.ln[i])) return (pn.ln[i]);
        return (null);
    }

    /**
     * Finds a finite line associated with the given line by checking for shared intersection points.
     *
     * @param l1 the reference line
     * @return the associated finite line or null if not found
     */
    LLine fd_lnl(LLine l1) {
        LLine ln = all_ln.nx;
        int p1, p2;
        while (ln != null) {
            if (ln != l1 && ln.type != 0) {
                p1 = inter_ll(l1, ln);
                p2 = inter_ll1(l1, ln, p1);
                if (p1 != 0 && p2 != 0) return (ln);
            }
            ln = ln.nx;
        }
        return (null);
    }

    /**
     * Retrieves or constructs a refined line related to the given line based on an exclusion point.
     *
     * @param l1 the original line
     * @param p the exclusion point for refining the line
     * @return the refined line or the original line if no refinement is needed
     */
    LLine fd_ln_rl(LLine l1, int p) {
        if (l1.type != 0) return l1;

        LLine l = fd_lnl(l1);
        if (l == l1) return l;

        test_ln.cp_ln(l1);
        int p1 = get_lpt1(l1, p);
        for (int i = 0; i <= l.no; i++) {
            if (!on_ln(l.pt[i], test_ln) && ck_dr(p, p1, p, l.pt[i]))
                add_pt2l(l.pt[i], test_ln);
        }
        if (test_ln.no == l.no) return l;

        LLine ln = all_ln.nx;
        while (ln != null) {
            if (ln != null)
                if (sub_ln(test_ln, ln) && test_ln.no == ln.no)
                    return ln;
            ln = ln.nx;
        }
        ln = add_ln(test_ln.pt[0], test_ln.pt[1]);
        for (int i = 2; i <= test_ln.no; i++)
            add_pt2l(test_ln.pt[i], ln);
        ln.type = 0;
        return ln;


    }

    /**
     * Creates a new line with the two specified points.
     *
     * @param p1 the first point
     * @param p2 the second point
     * @return the newly created line or null if the points are identical
     */
    LLine add_ln(int p1, int p2) {

        if (p1 == p2) {
            int k = 0;
            return null;
        }
        LLine ln;
        ln = new LLine();
        ln.type = 1;
        ln.co = null;
        ln.no = 1;
        if (p1 < p2) {
            ln.pt[0] = p1;
            ln.pt[1] = p2;
        } else {
            ln.pt[0] = p2;
            ln.pt[1] = p1;
        }
        ln.nx = null;
        last_ln.nx = ln;
        last_ln = ln;
        return (ln);
    }

    /**
     * Combines two lines to create a new line that contains points from both.
     *
     * @param l1 the first line
     * @param l2 the second line
     * @return the new combined line or null if the combined line is a subline of an existing line
     */
    LLine add_ln2l(LLine l1, LLine l2) {
        test_ln.cp_ln(l1);
        LLine ln = test_ln;
        for (int i = 0; i <= l2.no; i++)
            add_pt2l(l2.pt[i], ln);

        LLine l = all_ln.nx;
        while (l != null) {
            if (l.type != 0 && sub_ln(ln, l))
                return null;
            l = l.nx;
        }

        ln = cp_ln(test_ln);
        ln.type = 2;
        return ln;
    }

    /**
     * Finds an existing line containing the two points or creates a new one if not found.
     *
     * @param p1 the first point
     * @param p2 the second point
     * @return the found or newly created line
     */
    LLine fadd_ln(int p1, int p2) {
        LLine ln;
        ln = fd_ln(p1, p2);
        if (ln != null) return ln;
        return (add_ln(p1, p2));
    }

    /**
     * Creates a copy of the given line by duplicating its point data.
     * The new copy is linked as the last line.
     *
     * @param ln the line to copy
     * @return a new line object that is a copy of ln
     */
    LLine cp_ln(LLine ln) {
        LLine ln1;
        ln1 = new LLine();
        ln1.no = ln.no;
        for (int i = 0; i <= ln.no; i++) {
            ln1.pt[i] = ln.pt[i];
        }
        last_ln.nx = ln1;
        last_ln = ln1;
        return (ln1);
    }

    /**
     * Replaces occurrences of ln1 with ln2 in parallel, perpendicular, and angle relations.
     * Updates related structures accordingly.
     *
     * @param ln1 the original line to be replaced
     * @param ln2 the new line to substitute for ln1
     */
    final void ch_ln(LLine ln1, LLine ln2) {

        LLine l1, l2, l3, l4;

        PLine pn = all_pn.nx;
        while (pn != null) {
            if (pn.type != 0 && on_pn(ln1, pn)) {
                PLine pn1 = cp_pn(pn);
                pn.type = 0;
                if (on_pn(ln2, pn1)) {
                    int i = 0;
                    while (pn1.ln[i] != ln1) i++;
                    for (int j = i; j <= pn1.no; j++)
                        pn1.ln[j] = pn1.ln[j + 1];

                    pn1.no--;
                } else {
                    for (int i = 0; i <= pn1.no; i++) if (pn1.ln[i] == ln1) pn1.ln[i] = ln2;
                }
                {
                    co_xy.nx = null;
                    Cond co = add_coxy(CO_PARA);
                    co.u.pn = pn;
                    co = add_coxy(CO_COLL);
                    co.u.ln = ln2;
                    pn1.co = co_xy.nx;
                }
                if (pn1.no == 0) {
                    pn1.type = 0;
                } else {
                    new_para(pn1);
                }
            }
            pn = pn.nx;
        }
        TLine tn = all_tn.nx;
        while (tn != null) {
            if (tn.type != 0 && on_tn(ln1, tn)) {

                if (tn.l1 == ln1) {
                    l1 = ln2;
                    l2 = tn.l2;
                } else if (tn.l2 == ln1) {
                    l1 = tn.l1;
                    l2 = ln2;
                } else
                    l1 = l2 = null;

                if (l1 != null && l2 != null) {
                    TLine tn1 = add_tline(402, l1, l2);
                    tn.type = 0;
                    if (tn1 != null) {
                        co_xy.nx = null;
                        Cond co = add_coxy(CO_PERP);
                        co.u.tn = tn;
                        co = add_coxy(CO_COLL);
                        co.u.ln = ln2;
                        tn1.co = co_xy.nx;
                        new_pr(CO_PERP);
                        last_nd.u.tn = tn1;
                        new_ot();
                    }

                }
            }
            tn = tn.nx;
        }
        Angles as = all_as.nx;
        while (as != null) {
            if (as.type != 0) {
                int i = 0;
                l1 = as.l1;
                l2 = as.l2;
                l3 = as.l3;
                l4 = as.l4;

                if (as.l1 == ln1) {
                    l1 = ln2;
                    i = 1;
                }
                if (as.l2 == ln1) {
                    l2 = ln2;
                    i = 1;
                }
                if (as.l3 == ln1) {
                    l3 = ln2;
                    i = 1;
                }
                if (as.l4 == ln1) {
                    l4 = ln2;
                    i = 1;
                }
                if (i == 1) {
                    Angles as1 = add_ea_ln(0, l1, l2, l3, l4);
                    as.type = 0;
                    if (as1 != null) {
                        co_xy.nx = null;
                        Cond co = add_coxy(CO_ACONG);
                        co.u.as = as;
                        co = add_coxy(CO_COLL);
                        co.u.ln = ln2;
                        as1.co = co_xy.nx;

                        new_pr(CO_ACONG);
                        last_nd.u.as = as1; /*  new_ot();*/
                    }
                }
            }
            as = as.nx;
        }

        AngTn atn = all_atn.nx;
        co_db.nx = null;

        while (atn != null) {

            if (atn.type == 0 || atn.ln1 != ln1 && atn.ln2 != ln1 && atn.ln3 != ln1 && atn.ln4 != ln1) {
                atn = atn.nx;
                continue;
            }
            ch_ln_atn(ln1, ln2, atn);
            atn = atn.nx;
        }
    }

    /**
     * Adds a line based on three given point identifiers.
     * Validates collinearity and either creates a new line or updates an existing one.
     *
     * @param lm the lemma identifier used for processing
     * @param p1 identifier for the first point
     * @param p2 identifier for the second point
     * @param p3 identifier for the third point
     */
    final void add_line(int lm, int p1, int p2, int p3) {
        LLine ln1;
        if (xcoll(p1, p2, p3)) return;

        if (p1 == 0 || p2 == 0 || p3 == 0) {
            int k = 0;
            return;
        }
        if (!check_coll(p1, p2, p3)) {
            add_checkError();
            return;
        }
        ln1 = fd_ln3(p1, p2, p3);
        if (ln1 != null) return;

        if (false || co_db.nx == null && (ln1 = fd_ln3(p1, p2, p3)) != null && ln1.co == null) {
            add_pt2l(p1, ln1);
            add_pt2l(p2, ln1);
            add_pt2l(p3, ln1);
        } else {
            ln1 = add_ln(p1, p2);
            add_pt2l(p3, ln1);
            ln1.co = co_db.nx;
            ln1.lemma = lm;
        }
        ch_lns(ln1);
    }

    /**
     * Adds a line for hypothesis purposes, defined by three point identifiers.
     * If the line exists, it updates the structure; otherwise, it creates a new one.
     *
     * @param lm the lemma identifier used for processing
     * @param p1 identifier for the first point
     * @param p2 identifier for the second point
     * @param p3 identifier for the third point
     */
    final void add_line1(int lm, int p1, int p2, int p3) {      // for hypothesis only.
        if (!check_coll(p1, p2, p3)) {
            add_checkError();
            return;
        }
        LLine ln1 = fd_ln(p2, p3);
        if (ln1 != null) {
            add_pt2l(p1, ln1);
            ch_lns(ln1);
            return;
        }
        add_line(lm, p1, p2, p3);
    }

    /**
     * Processes and adjusts collections of lines.
     * Iterates through sub-line relationships to update and merge lines as needed.
     *
     * @param ln1 the initial line to process and potentially replace with merged data
     */
    public void ch_lns(LLine ln1) {
        LLine ln2, ln3, ln4;
        ln2 = ln1;
        while ((ln3 = fd_lnl(ln2)) != null) {
            if (sub_ln(ln2, ln3)) {
                ch_ln(ln2, ln3);
                ln2.type = 0;
                ln2 = ln3;
            } else if (sub_ln(ln3, ln2)) {
                ch_ln(ln3, ln2);
                ln3.type = 0;

            } else {
                ln4 = cp_ln(ln2);

                ln4.type = 1;
                ln4.lemma = 0;
                for (int i = 0; i <= ln3.no; i++) {
                    add_pt2l(ln3.pt[i], ln4);
                }
                if (ln2.co == null && ln3.co == null) {
                    ln4.co = null;
                } else {
                    co_xy.nx = null;
                    Cond co = add_coxy(CO_COLL);
                    co.u.ln = ln2;
                    co = add_coxy(CO_COLL);
                    co.u.ln = ln3;
                    ln4.co = co_xy.nx;
                }
                ch_ln(ln2, ln4);
                ch_ln(ln3, ln4);
                ln2.type = 0;
                ln3.type = 0;
                ln2 = ln4;
            }
        }
        new_pr(CO_COLL);
        last_nd.u.ln = ln2;
    }

    /**
     * Checks if the given line is contained within the specified parallel line container.
     *
     * @param ln the line to check for membership
     * @param pn the parallel line container to search
     * @return true if ln is part of pn; otherwise, false
     */
    boolean on_pn(LLine ln, PLine pn) {
        int i;
        for (i = 0; i <= pn.no; i++) if (pn.ln[i] == ln) return (true);
        return (false);
    }

    /**
     * Finds the parallel line container associated with the specified line.
     *
     * @param ln the line for which to locate its parallel container
     * @return the parallel line container if found; otherwise, null
     */
    PLine fd_pnl(LLine ln) {
        PLine pn;
        pn = all_pn.nx;
        while (pn != null && !(pn.type == 1 && on_pn(ln, pn))) {
            pn = pn.nx;
        }
        return (pn);
    }

    /**
     * Searches within the parallel line container for a line that contains the specified point.
     *
     * @param p the point identifier to look for
     * @param ln the parallel line container in which to search
     * @return the line containing the point if found; otherwise, null
     */
    LLine fd_lpp2(int p, LLine ln) {
        PLine pn;
        pn = all_pn.nx;
        while (pn != null && !(pn.type == 1 && on_pn(ln, pn))) {
            pn = pn.nx;
        }
        if (pn == null) return null;
        for (int i = 0; i <= pn.no; i++)
            if (on_ln(p, pn.ln[i]))
                return pn.ln[i];
        return (null);
    }

    /**
     * Retrieves the parallel line container corresponding to the line defined by two points.
     *
     * @param p1 identifier for the first point
     * @param p2 identifier for the second point
     * @return the associated parallel line container if it exists; otherwise, null
     */
    PLine fd_pn(int p1, int p2) {
        LLine ln = fd_ln(p1, p2);
        if (ln == null) return (null);
        return (fd_pnl(ln));
    }

    /**
     * Finds an alternative parallel line container (different from the given one) that includes the specified line.
     *
     * @param pn the current parallel line container
     * @param ln the line to search for in other containers
     * @return another parallel line container if found; otherwise, null
     */
    PLine fd_pnp(PLine pn, LLine ln) {
        PLine pn1;
        pn1 = all_pn.nx;
        while (pn1 != null)
            if (pn1 != pn && pn1.type != 0 && on_pn(ln, pn1)) return (pn1);
            else pn1 = pn1.nx;
        return (null);
    }

    /**
     * Creates a copy of the given parallel line container.
     * Duplicates its type, line array, and other associated properties.
     *
     * @param pn the parallel line container to copy
     * @return a new parallel line container that is a copy of pn
     */
    PLine cp_pn(PLine pn) {
        PLine pn1;
        char i;
        pn1 = new PLine();
        pn1.type = pn.type;
        pn1.nx = null;
        pn1.co = null;
        pn1.no = pn.no;
        for (i = 0; i <= pn.no; i++) {
            pn1.ln[i] = pn.ln[i];
        }
        last_pn.nx = pn1;
        last_pn = pn1;
        return (pn1);
    }

    /**
     * Merges two parallel line containers by uniting the lines from pn2 into pn1.
     *
     * @param pn1 the primary parallel line container to update
     * @param pn2 the source container whose lines will be merged into pn1
     */
    final void pn_un(PLine pn1, PLine pn2) {
        for (int i = 0; i <= pn2.no; i++)
            if (!on_pn(pn2.ln[i], pn1)) {
                pn1.no += 1;
                pn1.ln[pn1.no] = pn2.ln[i];
                if (pn1.ln[0] == null) {
                    // TODO. Handle this.
                    System.err.println("pn1.ln[0] is null");
                    return;
                }
                if (!check_para(pn1.ln[0].pt[0], pn1.ln[0].pt[1], pn2.ln[i].pt[0], pn2.ln[i].pt[1])) {
                    int k = 0;
                }
            }
    }

    /**
     * Adds a parallel line structure using four point identifiers.
     * Validates the input and creates a new parallel line container to represent the relation.
     *
     * @param lm the lemma identifier used for processing
     * @param p1 identifier for the first point of the first line
     * @param p2 identifier for the second point of the first line
     * @param p3 identifier for the first point of the second line
     * @param p4 identifier for the second point of the second line
     */
    final void add_pline(int lm, int p1, int p2, int p3, int p4) {
        if (!valid(lm)) return;

        PLine pn;
        if (xpara(p1, p2, p3, p4)) return;
        if (!check_para(p1, p2, p3, p4)) {
            add_checkError();
            return;
        }
        {
            if (p1 == p3 || p1 == p4 || p2 == p3 || p2 == p4) {
                int k = 0;
            }

            pn = new PLine();
            pn.type = 2;
            pn.lemma = lm;
            pn.co = co_db.nx;
            pn.no = 1;
            LLine ln1 = fadd_ln(p1, p2);
            LLine ln2 = fadd_ln(p3, p4);
            if (ln1.pt[0] < ln2.pt[0] || (ln1.pt[0] == ln2.pt[0] && ln1.pt[1] < ln2.pt[1])) {
                pn.ln[0] = ln1;
                pn.ln[1] = ln2;
            } else {
                pn.ln[0] = ln2;
                pn.ln[1] = ln1;
            }

            pn.nx = null;
            last_pn.nx = pn;
            last_pn = pn;
        }
        new_pr(CO_PARA);
        last_nd.u.pn = pn;
        //adj_pn(pn);
    }

    /**
     * Adds a parallel connection between two lines.
     * Creates a new parallel line container if the given lines are not already parallel.
     *
     * @param lm the lemma identifier used for processing
     * @param ln1 the first line in the connection
     * @param ln2 the second line in the connection
     * @return the newly created parallel line container if the connection is established; otherwise, null
     */
    PLine add_px(int lm, LLine ln1, LLine ln2) {
        if (!valid(lm)) return null;

        PLine pn;
        if (ln_para(ln1, ln2)) return (null);
        {
            pn = new PLine();
            pn.type = 2;
            pn.lemma = lm;
            pn.nx = null;
            pn.co = co_xy.nx;
            pn.no = 1;
            pn.ln[0] = ln1;
            pn.ln[1] = ln2;
            last_pn.nx = pn;
            last_pn = pn;
        }
        new_pr(CO_PARA);
        last_nd.u.pn = pn;
        return (pn);
    }

    /**
     * Checks if the given LLine is one of the defining lines of the specified TLine.
     *
     * @param ln the line to check
     * @param tn the TLine whose defining lines are examined
     * @return true if ln equals tn.l1 or tn.l2, false otherwise
     */
    boolean on_tn(LLine ln, TLine tn) {
        if ((ln == tn.l1) || (ln == tn.l2)) return (true);
        return (false);
    }

    /**
     * Finds and returns the first non-zero type TLine that uses the given LLine.
     *
     * @param ln the LLine to search for in TLines
     * @return the found TLine, or null if none exists
     */
    TLine fd_tn(LLine ln) {
        TLine tn;
        tn = all_tn.nx;
        while (tn != null) {
            if (tn.type != 0 && (tn.l1 == ln || tn.l2 == ln))
                return tn;
            tn = tn.nx;
        }


        return (tn);
    }

    /**
     * Creates a new TLine from two given LLines.
     *
     * @param ln1 the first LLine component
     * @param ln2 the second LLine component
     * @return the newly created TLine
     */
    TLine add_tn(LLine ln1, LLine ln2) {
        TLine tn = new TLine();
        tn.l1 = ln1;
        tn.l2 = ln2;
        tn.type = 2;
        tn.co = null;
        tn.nx = null;
        last_tn.nx = tn;
        last_tn = tn;
        return (tn);
    }

    /**
     * Adds a TLine based on two LLines if they are valid and not perpendicular.
     *
     * @param lm the lemma (identifier) associated with the TLine
     * @param l1 the first LLine component
     * @param l2 the second LLine component
     * @return the added TLine, or null if invalid conditions are met
     */
    final TLine add_tline(int lm, LLine l1, LLine l2) {
        if (!valid(lm)) return null;

        if (l1 == null || l2 == null) return null;
        if (ln_perp(l1, l2)) return null;
        if (!check_perp(l1.pt[0], l1.pt[1], l2.pt[0], l2.pt[1])) {
            int k = 0;
        }
        TLine tn1 = add_tn(l1, l2);

        tn1.lemma = lm;
        tn1.co = co_db.nx;
        new_pr(CO_PERP);
        last_nd.u.tn = tn1;
        return tn1;
    }

    /**
     * Adds a TLine using point indices by converting them to LLines.
     *
     * @param lm the lemma (identifier) for the TLine
     * @param p1 the first point of the first LLine
     * @param p2 the second point of the first LLine
     * @param p3 the first point of the second LLine
     * @param p4 the second point of the second LLine
     */
    final void add_tline(int lm, int p1, int p2, int p3, int p4) {
        if (!valid(lm)) return;

        LLine ln1, ln2;
        TLine tn1;
        if (xperp(p1, p2, p3, p4)) return;
        if (!check_perp(p1, p2, p3, p4)) {
            int k = 0;
        }
        if (search_only_exists_ln() && (fd_ln(p1, p2) == null || fd_ln(p3, p4) == null))
            return;

        ln1 = fadd_ln(p1, p2);
        ln2 = fadd_ln(p3, p4);
        if (ln1 == null || ln2 == null) return;

        tn1 = add_tn(ln1, ln2);
        tn1.lemma = lm;
        tn1.co = co_db.nx;
        new_pr(CO_PERP);
        last_nd.u.tn = tn1;
    }

    /**
     * Adds a TLine from two LLines with a different coordinate source.
     *
     * @param lm the lemma (identifier) for the TLine
     * @param l1 the first LLine component
     * @param l2 the second LLine component
     * @return the newly added TLine, or null if preconditions fail
     */
    final TLine add_tx(int lm, LLine l1, LLine l2) {
        if (!valid(lm)) return null;

        TLine tn1;
        if (l1 == null || l2 == null) return null;
        if (ln_perp(l1, l2)) return null;

        tn1 = add_tn(l1, l2);
//        if (l1.type == 0 || l2.type == 0)
//            tn1.type = 0;
        tn1.lemma = lm;
        tn1.co = co_xy.nx;
        new_pr(CO_PERP);
        last_nd.u.tn = tn1;
        return tn1;
    }

    /**
     * Adds a TLine in test mode using input point indices.
     *
     * @param lm the lemma (identifier) for the TLine
     * @param p1 the first point of the first LLine
     * @param p2 the second point of the first LLine
     * @param p3 the first point of the second LLine
     * @param p4 the second point of the second LLine
     */
    public void add_tline_t(int lm, int p1, int p2, int p3, int p4) {
        if (!valid(lm)) return;

        if (xperp(p1, p2, p3, p4)) return;
        if (!check_perp(p1, p2, p3, p4)) {
            add_checkError();
            return;
        }
        if (search_only_exists_ln() && (fd_ln(p1, p2) == null || fd_ln(p3, p4) == null))
            return;

        LLine ln1 = fadd_ln_t(p1, p2);
        LLine ln2 = fadd_ln_t(p3, p4);

        if (ln1 == null || ln2 == null) return;

        TLine tn1 = add_tn(ln1, ln2);
//        if (ln1.type == 0 || ln2.type == 0)
//            tn1.type = 0;

        tn1.lemma = lm;
        tn1.co = co_db.nx;
        new_pr(CO_PERP);
        last_nd.u.tn = tn1;
    }

    /**
     * Checks if a given point is contained in the specified circle.
     *
     * @param a the point to check; zero is always considered valid
     * @param cr the circle in which to check for the point
     * @return true if the point is found in the circle, or if a is zero; false otherwise
     */
    boolean on_cir(int a, ACir cr) {
        char i;
        if (a == 0) return (true);
        for (i = 0; i <= cr.no; i++) if (cr.pt[i] == a) return (true);
        return (false);
    }

    /**
     * Determines whether all points of the first circle are contained in the second circle.
     *
     * @param c1 the circle whose points are to be checked
     * @param c2 the circle to check against
     * @return true if c1 is a subset of c2, false otherwise
     */
    boolean sub_cir(ACir c1, ACir c2) {
        char i;
        if (c1.o == c2.o || c1.o == 0) {
            for (i = 0; i <= c1.no; i++) {
                if (!on_cir(c1.pt[i], c2)) return (false);
            }
            return (true);
        }
        return (false);
    }

    /**
     * Searches for a circle that contains all three specified points.
     *
     * @param p1 the first point
     * @param p2 the second point
     * @param p3 the third point
     * @return the identifier of the found circle if all points belong to it; otherwise, zero
     */
    int fd_co(int p1, int p2, int p3) {
        ACir c2;
        c2 = all_cir.nx;
        while (c2 != null) {
            if (c2.o != 0 && on_cir(p1, c2) && on_cir(p2, c2) && on_cir(p3, c2)) return (c2.o);
            c2 = c2.nx;
        }
        return (0);
    }

    /**
     * Finds a circle with the specified center identifier that contains a given point.
     *
     * @param o the center identifier to match
     * @param p the point to check for within the circle
     * @return the found circle, or null if none match
     */
    ACir fd_cr_op(int o, int p) {
        ACir c2 = all_cir.nx;
        while (c2 != null) {
            if (c2.type != 0 && c2.o == o && on_cir(p, c2)) return (c2);
            c2 = c2.nx;
        }
        return (null);
    }

    /**
     * Retrieves a circle that contains three specified points.
     *
     * @param p1 the first point
     * @param p2 the second point
     * @param p3 the third point
     * @return the found circle containing all three points, or null if none exists
     */
    ACir fd_cr_p3(int p1, int p2, int p3) {
        ACir c2 = all_cir.nx;
        while (c2 != null) {
            if (c2.type != 0 && on_cir(p1, c2) && on_cir(p2, c2) && on_cir(p3, c2)) return (c2);
            c2 = c2.nx;
        }
        return (null);
    }

    /**
     * Finds and returns a circle that is similar to the given circle by matching common points.
     *
     * @param c1 the circle to compare
     * @return a matching circle based on shared center or common points, or null if none is found
     */
    ACir fd_cir(ACir c1) {
        ACir c2;
        char i, j;
        c2 = all_cir.nx;
        while (c2 != null) {
            if (c2 != c1 && c2.type != 0)
                if (c2.o == c1.o && c1.o != 0) {
                    for (i = 0; i <= c1.no; i++) if (on_cir(c1.pt[i], c2)) return (c2);
                } else {
                    j = 0;
                    for (i = 0; i <= c1.no; i++) if (on_cir(c1.pt[i], c2)) j++;
                    if (j > 2) return (c2);
                }
            c2 = c2.nx;
        }
        return (null);
    }

    /**
     * Adds a point to the specified circle in sorted order if it does not already exist.
     *
     * @param p the point to add
     * @param cr the circle to which the point will be added
     */
    final void add_pt2c(int p, ACir cr) {
        int j, i = 0;
        if (p <= 0) return;


        while (i <= cr.no && cr.pt[i] < p) i++;
        if (i > cr.no) {
            cr.no += 1;
            cr.pt[cr.no] = p;
        } else {
            if (cr.pt[i] == p) return;
            for (j = cr.no; j >= i; j--) {
                cr.pt[j + 1] = cr.pt[j];
            }
            cr.pt[i] = p;
            cr.no += 1;
        }
    }

    /**
     * Creates and adds a new circle with the given lemma, center, and initial two points.
     *
     * @param lm the lemma (identifier) associated with the circle
     * @param o the center identifier of the circle
     * @param a the first point defining the circle's boundary
     * @param b the second point defining the circle's boundary
     * @return the newly created circle, or null if the lemma is invalid
     */
    ACir add_c2(int lm, int o, int a, int b) {
        if (!valid(lm)) return null;

        ACir cr = new ACir();
        cr.type = 2;
        cr.lemma = lm;
        cr.co = null;
        cr.o = o;
        cr.no = 1;
        cr.nx = null;
        if (a < b) {
            cr.pt[0] = a;
            cr.pt[1] = b;
        } else {
            cr.pt[0] = b;
            cr.pt[1] = a;
        }
        last_cir.nx = cr;
        last_cir = cr;
        return (cr);
    }

    /**
     * Creates a copy of the specified circle with the same properties and points.
     *
     * @param cir the circle to copy
     * @return a new circle object that is a copy of cir
     */
    ACir cp_cr(ACir cir) {
        ACir cir1;
        char i;
        cir1 = new ACir();
        cir1.type = cir.type;
        cir1.o = cir.o;
        cir1.co = cir.co;
        cir1.no = cir.no;
        for (i = 0; i <= cir.no; i++) {
            cir1.pt[i] = cir.pt[i];
        }
        last_cir.nx = cir1;
        last_cir = cir1;
        return (cir1);
    }

    /**
     * Adds a cyclic circle using the specified lemma, origin and two point identifiers.
     *
     * @param lm the lemma identifier
     * @param o  the origin or center point identifier
     * @param a  the first point identifier on the circle
     * @param b  the second point identifier on the circle
     */
    final void add_cir2(int lm, int o, int a, int b) {
        if (!valid(lm)) return;

        if (xcir_n(o, a, b, 0, 0, 0)) return;
        ACir cr = add_c2(lm, o, a, b);
        cr.co = co_db.nx;
        new_pr(CO_CYCLIC);
        last_nd.u.cr = cr;
    }

    /**
     * Adds a cyclic circle using the specified lemma, origin, two point identifiers and an additional point.
     *
     * @param lm the lemma identifier
     * @param o  the origin or center point identifier
     * @param a  the first point identifier on the circle
     * @param b  the second point identifier on the circle
     * @param c  the additional point identifier to be added to the circle
     */
    final void add_cir3(int lm, int o, int a, int b, int c) {
        if (!valid(lm)) return;

        ACir cr;
        if (xcir_n(o, a, b, c, 0, 0)) return;
        {
            cr = add_c2(lm, o, a, b);
            add_pt2c(c, cr);
            cr.co = co_db.nx;
        }
        new_pr(CO_CYCLIC);
        last_nd.u.cr = cr;
    }

    /**
     * Adds a cyclic circle using the specified lemma, origin, two point identifiers and two additional points.
     *
     * @param lm the lemma identifier
     * @param o  the origin or center point identifier
     * @param a  the first point identifier on the circle
     * @param b  the second point identifier on the circle
     * @param c  the first additional point identifier to be added to the circle
     * @param d  the second additional point identifier to be added to the circle
     */
    final void add_cir4(int lm, int o, int a, int b, int c, int d) {
        if (!valid(lm)) return;

        ACir cr;
        //if (xcir4(o, a, b, c, d)) return;
        if (xcir_n(o, a, b, c, d, 0)) return;
        {
            cr = add_c2(lm, o, a, b);
            add_pt2c(c, cr);
            add_pt2c(d, cr);
            cr.co = co_db.nx;
        }
        //adj_cir(cr);
        new_pr(CO_CYCLIC);
        last_nd.u.cr = cr;
    }

    /**
     * Checks if the given pair of lines match the sides defined in the specified angles structure.
     *
     * @param l1 the first line
     * @param l2 the second line
     * @param as the angles structure to compare against
     * @return true if the lines match either the first or second half of the angles structure; false otherwise
     */
    boolean onl_as(LLine l1, LLine l2, Angles as) {
        if (l1 == as.l1 && l2 == as.l2) return (true);
        if (l1 == as.l3 && l2 == as.l4) return (true);
        return (false);
    }

    /**
     * Creates a new angles object from the given lemma and four lines representing two pairs of corresponding sides.
     * Performs necessary comparisons and adjustments between the lines.
     *
     * @param lm the lemma identifier
     * @param l1 the first line of the first angle pair
     * @param l2 the second line of the first angle pair
     * @param l3 the first line of the second angle pair
     * @param l4 the second line of the second angle pair
     * @return the created angles object, or null if the parameters are invalid
     */
    Angles add_as0(int lm, LLine l1, LLine l2, LLine l3, LLine l4) {
        if (!valid(lm)) return null;

        LLine n1, n2;
        Angles as;

        if (line_compare(l1, l4) == 0) {
            n1 = l3;
            l3 = l1;
            l1 = n1;
            n1 = l2;
            l2 = l4;
            l4 = n1;
        }
        if (line_compare(l2, l3) == 0) {
            if (line_compare(l1, l4) > 0) {
                n1 = l4;
                l4 = l1;
                l1 = n1;
            }
        } else if (line_compare(l1, l3) == 0) {
            if (line_compare(l2, l4) > 0) {
                n1 = l2;
                l2 = l4;
                l4 = n1;
            }
        } else if (line_compare(l2, l4) == 0) {
            n1 = l1;
            l1 = l2;
            l2 = n1;
            n1 = l3;
            l3 = l4;
            l4 = n1;
            if (line_compare(l2, l4) > 0) {
                n1 = l2;
                l2 = l4;
                l4 = n1;
            }
        } else {
            if (line_compare(l1, l2) < 0) n1 = l1;
            else n1 = l2;
            if (line_compare(l3, l4) < 0) n2 = l3;
            else n2 = l4;
            if (line_compare(n1, n2) > 0) {
                n1 = l3;
                l3 = l1;
                l1 = n1;
                n1 = l2;
                l2 = l4;
                l4 = n1;
            }
            if (line_compare(l1, l2) > 0) {
                n1 = l1;
                l1 = l2;
                l2 = n1;
                n1 = l3;
                l3 = l4;
                l4 = n1;
            }
        }
        if (l1 == l2 && l3 == l4 || l1 == l3 && l2 == l4) return null;
        //if (!valid(R_AG_ALL) && !valid(R_AG_NO_INTER) && (inter_lls(l1, l2) == 0 || inter_lls(l3, l4) == 0)) return null;

//        if (!search_only_exists_ln(l1, l2, l3, l4)) return null;
//        if (!check_eqangle(l1.pt[0], l1.pt[1], l2.pt[0], l2.pt[1], l3.pt[0], l3.pt[1], l4.pt[0], l4.pt[1])) {
//            int k = 0;
//        }
//        if (check_perp(l1.pt[0], l1.pt[1], l2.pt[0], l2.pt[1])) {
//            int k = 0;
//        }

        {
            as = new Angles();
            as.type = 2;
            as.lemma = lm;
            as.co = co_db.nx;
            as.sa = 0;
            as.nx = null;
            as.l1 = l1;
            as.l2 = l2;
            as.l3 = l3;
            as.l4 = l4;
            last_as.nx = as;
            last_as = as;
            new_pr(CO_ACONG);
            last_nd.u.as = as;


            if (l1 == l2 || l3 == l4 || l1 == l3 && l2 == l4 || l1 == l4 && l2 == l3) {
                int k = 0;
            }
            if (l1.type == 0 || l2.type == 0 || l3.type == 0 || l4.type == 0)
                as.type = 0;
        }
        return (as);
    }

    /**
     * Checks and adjusts the given angles structure based on the provided four lines.
     * Modifies the angles type and associates new conditions if necessary.
     *
     * @param as the angles structure to check and adjust
     * @param l1 the first line of the first angle pair
     * @param l2 the second line of the first angle pair
     * @param l3 the first line of the second angle pair
     * @param l4 the second line of the second angle pair
     */
    final void ck_as(Angles as, LLine l1, LLine l2, LLine l3, LLine l4) {
        co_xy.nx = null;
        if (ln_para(l1, l2) && l3 != l4) {
            as.type = 0;
            PLine pn = add_px(165, l3, l4);
            if (pn != null) {
                pn.co = add_acoxy(new Angles(l1, l2, l3, l4));
            }
        } else if (ln_perp(l1, l2)) {
            as.type = 0;
            TLine tn = add_tx(145, l3, l4);
            if (tn != null) {
                tn.co = add_acoxy(new Angles(l1, l2, l3, l4));
            }
        } else if (ln_para(l1, l3) && l2 != l4) {
            PLine pn = add_px(166, l2, l4);
            if (pn != null) {
                pn.co = add_acoxy(new Angles(l1, l2, l3, l4));
            }
        } else if (false && ln_perp(l1, l3)) {
            TLine tn = add_tx(146, l2, l4);
            if (tn != null) {
                tn.co = add_acoxy(new Angles(l1, l2, l3, l4));
            }
        }
        co_xy.nx = null;
    }

    /**
     * Creates and returns a new condition object associated with the given angles structure.
     *
     * @param as the angles structure for which the condition is created
     * @return the newly created condition object
     */
    final Cond add_acoxy(Angles as) {
        co_xy.nx = null;
        Cond c = add_coxy(CO_ACONG);
        c.u.as = as;
        return c;
    }

    /**
     * Adjusts the specified angles structure based on the provided type flag.
     * Iterates over existing angles objects and applies necessary adjustments.
     *
     * @param t  the type flag indicating the adjustment mode
     * @param as the angles structure to adjust
     */
    final void adj_as(int t, Angles as) {
        if (as == null) return;
        if (as.type == 0) return;


        if (t == 1)
            for (Angles r1 = all_as.nx; (r1 != null && ch_dep(r1.dep)); r1 = r1.nx) {
                if (r1 == as) continue;
                if (r1.type == 0) continue;
                adj_as1(r1.l1, r1.l2, r1.l3, r1.l4, as, r1);
                adj_as1(r1.l3, r1.l4, r1.l1, r1.l2, as, r1);
            }
        else if (t == 0) {
            if (R_AG_ALL && isPFull()) {
                for (Angles r1 = all_as.nx; (r1 != null && r1 != as && ch_dep(r1.dep)); r1 = r1.nx) {
                    if (r1.type == 0) continue;
                    if (r1 == as) continue;
                    adj_as1(r1.l1, r1.l3, r1.l2, r1.l4, as, r1);
                    adj_as1(r1.l2, r1.l4, r1.l1, r1.l3, as, r1);
                    adj_as2(r1.l1, r1.l4, r1.l2, r1.l3, as, r1);
                    adj_as2(r1.l2, r1.l3, r1.l1, r1.l4, as, r1);
                }
            } else if (!isPFull()) {
                for (Angles r1 = all_as.nx; (r1 != null && r1 != as && ch_dep(r1.dep)); r1 = r1.nx) {
                    if (r1.type == 0) continue;
                    if (r1 == as) continue;
                    adj_as_plus(r1.l1, r1.l2, r1.l3, r1.l4, as);
                    adj_as_plus(r1.l2, r1.l1, r1.l4, r1.l3, as);
                }
            }
        }
    }

    /**
     * Adjusts tangency relationships for the specified angles structure by comparing
     * the tangency lines associated with its component lines using the given markers.
     *
     * @param m1 a marker or point identifier used for tangency adjustment
     * @param m2 a marker or point identifier used for tangency adjustment
     * @param as the angles structure whose tangency relationships are to be adjusted
     */
    public void adj_as_tn(int m1, int m2, Angles as) {

        if (m1 == 0 || m2 == 0) return;
        TLine tn1, tn2, tn3, tn4;
        tn1 = fd_tn(as.l1);
        tn2 = fd_tn(as.l2);
        tn3 = fd_tn(as.l3);
        tn4 = fd_tn(as.l4);
        search_as_tn1(tn1, tn3, as);
        search_as_tn1(tn1, tn4, as);
        search_as_tn1(tn2, tn3, as);
        search_as_tn1(tn2, tn4, as);

    }

    /**
     * Searches and adjusts transversal angles based on the provided t-lines.
     *
     * @param tn1 the first transversal line
     * @param tn2 the second transversal line
     * @param as the angle set to update
     */
    public void search_as_tn1(TLine tn1, TLine tn2, Angles as) {
        if (tn1 == null || tn2 == null) return;

        adj_as_plus(tn1.l1, tn1.l2, tn2.l1, tn2.l2, as);
        adj_as_plus(tn1.l1, tn1.l2, tn2.l2, tn2.l1, as);
        adj_as_plus(tn2.l1, tn2.l2, tn1.l2, tn1.l1, as);
        adj_as_plus(tn2.l1, tn2.l2, tn1.l2, tn1.l1, as);
    }

    /**
     * Adjusts angles based on the line configuration in the given angle sets.
     *
     * @param l1 the first line in the configuration
     * @param l2 the second line in the configuration
     * @param l3 the third line in the configuration
     * @param l4 the fourth line in the configuration
     * @param r1 the primary angle set
     * @param r2 the secondary angle set
     */
    final void adj_as1(LLine l1, LLine l2, LLine l3, LLine l4, Angles r1, Angles r2) {
        Cond co;
        LLine t1, t2;

        if (l1 == r1.l1 && l2 == r1.l2) {
            t1 = r1.l3;
            t2 = r1.l4;
        } else if (l1 == r1.l2 && l2 == r1.l1) {
            t1 = r1.l4;
            t2 = r1.l3;
        } else if (l1 == r1.l3 && l2 == r1.l4) {
            t1 = r1.l1;
            t2 = r1.l2;
        } else if (l1 == r1.l4 && l2 == r1.l3) {
            t1 = r1.l2;
            t2 = r1.l1;
        }
//maked for orthocenter 97,7
        else if (isPFull()) {
            if (l1 == r1.l1 && l2 == r1.l3) {
                t1 = r1.l2;
                t2 = r1.l4;
            } else if (l1 == r1.l3 && l2 == r1.l1) {
                t1 = r1.l4;
                t2 = r1.l2;
            } else if (l1 == r1.l2 && l2 == r1.l4) {
                t1 = r1.l1;
                t2 = r1.l3;
            } else if (l1 == r1.l4 && l2 == r1.l2) {
                t1 = r1.l3;
                t2 = r1.l1;
            } else
                return;
        } else
            return;

        if (t1 == t2 || l3 == l4 || t1 == l3 && t2 == l4 || t1 == l4 && t2 == l3) return;
//        if (ln_acong(t1, t2, l3, l4)) return;

        Angles as = add_ea_ln(181, t1, t2, l3, l4);
        if (as != null) {
            co_xy.nx = null;
            co = add_coxy(CO_ACONG);
            co.u.as = r2;
            co = add_coxy(CO_ACONG);
            co.u.as = r1;
            as.co = co_xy.nx;
        }
    }

    /**
     * Adjusts angles using an alternative line pairing.
     *
     * @param l1 the first line in the configuration
     * @param l2 the second line in the configuration
     * @param l3 the third line in the configuration
     * @param l4 the fourth line in the configuration
     * @param r1 the primary angle set
     * @param r2 the secondary angle set
     */
    final void adj_as2(LLine l1, LLine l2, LLine l3, LLine l4, Angles r1, Angles r2) {
        Cond co;
        LLine t1, t2;

        if ((l1 == r1.l1 && l2 == r1.l4) || (l1 == r1.l4 && l2 == r1.l1)) {
            t1 = r1.l2;
            t2 = r1.l3;
        } else if ((l1 == r1.l2 && l2 == r1.l3) || (l1 == r1.l3 && l2 == r1.l2)) {
            t1 = r1.l1;
            t2 = r1.l4;
        } else
            return;
        if (t1 == l3 || l4 == t2 || t1 == l4 && l3 == t2 || t1 == t2 && l3 == l4) return;
        if (ln_acong(t1, l3, l4, t2)) return;

        Angles as = add_ea_ln(182, t1, l3, l4, t2);
        if (as != null) {
            co_xy.nx = null;
            co = add_coxy(CO_ACONG);
            co.u.as = r2;
            co = add_coxy(CO_ACONG);
            co.u.as = r1;
            last_as.co = co_xy.nx;
        }
    }

    /**
     * Attempts to combine line segments and adjust angles based on additional angle information.
     *
     * @param l1 the first line in the initial pair
     * @param l2 the second line in the initial pair
     * @param l3 the first line in the alternate pair
     * @param l4 the second line in the alternate pair
     * @param r1 the reference angle set
     */
    final void adj_as_plus(LLine l1, LLine l2, LLine l3, LLine l4, Angles r1) {


        LLine t1, t2, ln1, ln2, ln3, ln4;
        ln1 = ln2 = ln3 = ln4 = null;

        t1 = get_82l0(l1, l2, r1.l1, r1.l2);
        t2 = get_82l0(l3, l4, r1.l3, r1.l4);

        if (t1 != null && t2 != null) {
            if (l2 == r1.l1) {
                ln1 = l1;
                ln2 = r1.l2;
            } else if (l1 == r1.l2) {
                ln1 = r1.l1;
                ln2 = l2;
            }

            if (l4 == r1.l3) {
                ln3 = l3;
                ln4 = r1.l4;
            } else if (l3 == r1.l4) {
                ln3 = r1.l3;
                ln4 = l4;
            }
        } else {
            t1 = get_82l0(l1, l2, r1.l3, r1.l4);
            t2 = get_82l0(l3, l4, r1.l1, r1.l2);

            if (t1 != null && t2 != null) {
                if (l2 == r1.l3) {
                    ln1 = l1;
                    ln2 = r1.l4;
                } else if (l1 == r1.l4) {
                    ln1 = r1.l3;
                    ln2 = l2;
                }

                if (l4 == r1.l1) {
                    ln3 = l3;
                    ln4 = r1.l2;
                } else if (l3 == r1.l2) {
                    ln3 = r1.l1;
                    ln4 = l4;
                }
            }
        }
        if (t1 == null || t2 == null) return;

        if (ln1 == ln3 && ln2 == ln4) return;


        int m1 = inter_lls(ln1, t1);
        int m2 = inter_lls(t1, ln2);
        int m3 = inter_lls(ln1, ln2);

        int p1 = inter_lls(ln3, t2);
        int p2 = inter_lls(t2, ln4);
        int p3 = inter_lls(ln3, ln4);
        if (p1 == 0 || p2 == 0 || p3 == 0 || m1 == 0 || m2 == 0 || m3 == 0)
            return;

        if (m1 != m2 && m1 != m3 && m3 != m2) {
            ln1 = fadd_ln_t(m1, m3);
            ln2 = fadd_ln_t(m2, m3);
        }
        if (p1 != p2 && p1 != p3 && p2 != p3) {
            ln3 = fadd_ln_t(p1, p3);
            ln4 = fadd_ln_t(p2, p3);
        }
        if (ln1 == ln2 || ln3 == ln4) return;

        TLine tn = null;
        Angles as = null;
        if (xcoll_ln(ln1, ln2) || xcoll_ln(ln3, ln4)) return;

        if (ln1 == ln4 && ln2 == ln3) {
            if (!check_para(ln1, ln2))
                tn = add_tline(142, ln1, ln2);
        } else
            as = add_ea_ln(183, ln1, ln2, ln3, ln4);

        if (tn != null || as != null) {
            co_xy.nx = null;
            Cond co = add_coxy(CO_ACONG);
            co.u.as = new Angles(l1, l2, l3, l4);
            co = add_coxy(CO_ACONG);
            co.u.as = r1;
            if (tn != null)
                tn.co = co_xy.nx;
            if (as != null)
                as.co = co_xy.nx;
        }

    }

    /**
     * Retrieves a line that matches the endpoints of two given line pairs.
     *
     * @param l1 the first line of the first pair
     * @param l2 the second line of the first pair
     * @param l3 the first line of the second pair
     * @param l4 the second line of the second pair
     * @return the matching line if found; otherwise, null
     */
    public LLine get_82l0(LLine l1, LLine l2, LLine l3, LLine l4) {
        if (l1 == l4) return l1;
        if (l2 == l3) return l2;
        return null;
    }

    /**
     * Creates an angle point using the intersection of line segments.
     *
     * @param lm lemma identifier
     * @param a first endpoint of the first line
     * @param b second endpoint of the first line
     * @param c additional parameter for the first line
     * @param p first endpoint of the second line
     * @param q second endpoint of the second line
     * @param r additional parameter for the second line
     */
    final void add_ea_pt(int lm, int a, int b, int c, int p, int q, int r) {
        add_ea_ln(lm, fadd_ln(a, b), fadd_ln(b, c), fadd_ln(p, q), fadd_ln(q, r));
    }

    /**
     * Adds an angle by combining four lines if the configuration is valid.
     *
     * @param lm lemma identifier
     * @param l1 the first line of the first pair
     * @param l2 the second line of the first pair
     * @param l3 the first line of the second pair
     * @param l4 the second line of the second pair
     * @return the constructed angle set or null if invalid
     */
    final Angles add_ea_ln(int lm, LLine l1, LLine l2, LLine l3, LLine l4) {

        if (d_base == 1) return null;
        if (l1 == null || l2 == null || l3 == null || l4 == null) return null;
        if (ln_acong(l1, l2, l3, l4)) return null;
        Angles as = add_as(lm, l1, l2, l3, l4);
        if (l1 == l4 && l2 == l3) return null;

        return as;
    }

    /**
     * Determines if a set of points belongs to the specified similar triangle configuration.
     *
     * @param p1 first point of the first triangle
     * @param p2 second point of the first triangle
     * @param p3 third point of the first triangle
     * @param p first point of the second triangle
     * @param q second point of the second triangle
     * @param r third point of the second triangle
     * @param st the similar triangle to compare against
     * @return true if the points match the triangle configuration; false otherwise
     */
    boolean on_st(int p1, int p2, int p3, int p, int q, int r, SimTri st) {                         //??????????????????
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (i != j)
                    for (int k = 0; k < 3; k++)
                        if (k != i && k != j) {
                            if (p1 == st.p1[i] && p2 == st.p1[j] && p3 == st.p1[k] && p == st.p2[i] && q == st.p2[j] && r == st.p2[k])
                                return true;
                            if (p1 == st.p2[i] && p2 == st.p2[j] && p3 == st.p2[k] && p == st.p1[i] && q == st.p1[j] && r == st.p1[k])
                                return true;
                        }
        return (false);
    }

    /**
     * Adds a pair of triangles based on provided vertex configurations.
     *
     * @param lm lemma identifier
     * @param dr direction flag
     * @param a first vertex of the first triangle
     * @param b second vertex of the first triangle
     * @param c third vertex of the first triangle
     * @param p first vertex of the second triangle
     * @param q second vertex of the second triangle
     * @param r third vertex of the second triangle
     */
    final void add_stri(int lm, int dr, int a, int b, int c, int p, int q, int r) {
        if (!valid(lm)) return;

        if (a == p && b == q && c == r) return;

        if (tri_type == 0 && !valid(R_STRI)) return;
        if (tri_type == 1 && !valid(R_CTRI)) return;

        if (same_tri(a, b, c, p, q, r)) return;

        if (search_only_exists_ln(a, b, b, c, a, c)) return;
        if (search_only_exists_ln(p, q, p, r, q, r)) return;

        if (tri_type == 0 && (xsim_tri(a, b, c, p, q, r) || xcon_tri(a, b, c, p, q, r))) return;
        if (tri_type == 1 && xcon_tri(a, b, c, p, q, r)) return;


        if (tri_type == 0 && !check_simtri(a, b, c, p, q, r)) {
            this.add_checkError();
            return;
        }

        if (tri_type == 1 && !check_congtri(a, b, c, p, q, r)) {
            this.add_checkError();
            return;
        }

        SimTri st = new SimTri();
        st.type = 2;
        st.lemma = lm;
        st.co = co_db.nx;
        st.nx = null;
        st.dr = dr;
        st.p1[0] = a;
        st.p1[1] = b;
        st.p1[2] = c;
        st.p2[0] = p;
        st.p2[1] = q;
        st.p2[2] = r;
        st.st = tri_type;

        if (tri_type == 0) {
            last_st.nx = st;
            last_st = st;
        } else {
            last_ct.nx = st;
            last_ct = st;
        }
        if (tri_type == 0)
            new_pr(CO_STRI);
        else
            new_pr(CO_CTRI);
        last_nd.u.st = st;

        return;
    }

    /**
     * Checks if a line defined by two points is part of the provided congruency segment.
     *
     * @param p1 the first point of the line
     * @param p2 the second point of the line
     * @param cg the congruency segment to check
     * @return true if the line is on the congruency segment; false otherwise
     */
    final boolean on_cg(int p1, int p2, CongSeg cg) {
        int i;
        if (p2 < p1) {
            i = p1;
            p1 = p2;
            p2 = i;
        }
        if (p1 == cg.p1 && p2 == cg.p2 || p1 == cg.p3 && p2 == cg.p4) return (true);
        return (false);
    }

    /**
     * Checks if the lines defined by two pairs of points form the provided congruency segment.
     *
     * @param p1 the first point of the first line
     * @param p2 the second point of the first line
     * @param p3 the first point of the second line
     * @param p4 the second point of the second line
     * @param cg the congruency segment to check
     * @return true if the points form the congruency segment; false otherwise
     */
    final boolean on_cg(int p1, int p2, int p3, int p4, CongSeg cg) {
        int i;
        if (p2 < p1) {
            i = p1;
            p1 = p2;
            p2 = i;
        }
        if (p4 < p3) {
            i = p3;
            p3 = p4;
            p4 = i;
        }
        if (p1 == cg.p1 && p2 == cg.p2 && p3 == cg.p3 && p4 == cg.p4) return (true);
        if (p1 == cg.p3 && p2 == cg.p4 && p3 == cg.p1 && p4 == cg.p2) return (true);
        return (false);
    }

    /**
     * Adjusts congruency segments by comparing the specified congruency with others.
     *
     * @param c flag indicating which congruency list to traverse
     * @param cg the reference congruency segment
     */
    final void adj_cg(int c, CongSeg cg) {
        CongSeg cg1 = (c == 1) ? all_cg.nx : all_rg.nx;

        while (cg1 != null && ch_dep(cg1.dep)) {
            if (cg.p1 == cg1.p1 && cg.p2 == cg1.p2) {
                add_codb(CO_CONG, cg1.p1, cg1.p2, cg1.p3, cg1.p4, cg1.t1, cg1.t2, 0, 0);
                add_codb(CO_CONG, cg.p1, cg.p2, cg.p3, cg.p4, cg.t1, cg.t2, 0, 0);
                add_cong(0, 0, cg.p3, cg.p4, cg1.p3, cg1.p4, cg.t2 * cg1.t1, cg1.t2 * cg.t1);
                pop_codb();
                pop_codb();
            } else if (cg.p1 == cg1.p3 && cg.p2 == cg1.p4) {
                add_codb(CO_CONG, cg1.p1, cg1.p2, cg1.p3, cg1.p4, cg1.t1, cg1.t2, 0, 0);
                add_codb(CO_CONG, cg.p1, cg.p2, cg.p3, cg.p4, cg.t1, cg.t2, 0, 0);
                add_cong(0, 0, cg.p3, cg.p4, cg1.p1, cg1.p2, cg.t2 * cg1.t2, cg.t1 * cg1.t1);
                pop_codb();
                pop_codb();
            } else if (cg.p3 == cg1.p1 && cg.p4 == cg1.p2) {
                add_codb(CO_CONG, cg1.p1, cg1.p2, cg1.p3, cg1.p4, cg1.t1, cg1.t2, 0, 0);
                add_codb(CO_CONG, cg.p1, cg.p2, cg.p3, cg.p4, cg.t1, cg.t2, 0, 0);
                add_cong(0, 0, cg.p1, cg.p2, cg1.p3, cg1.p4, cg.t1 * cg1.t1, cg.t2 * cg1.t2);
                pop_codb();
                pop_codb();

            } else if (cg.p3 == cg1.p3 && cg.p4 == cg1.p4) {
                add_codb(CO_CONG, cg1.p1, cg1.p2, cg1.p3, cg1.p4, cg1.t1, cg1.t2, 0, 0);
                add_codb(CO_CONG, cg.p1, cg.p2, cg.p3, cg.p4, cg.t1, cg.t2, 0, 0);
                add_cong(0, 0, cg.p1, cg.p2, cg1.p1, cg1.p2, cg.t1 * cg1.t2, cg.t2 * cg1.t1);
                pop_codb();
                pop_codb();
            }

            if (cg.t1 * cg1.t2 == cg.t2 * cg1.t1)
                search_cg3(cg.p1, cg.p2, cg.p3, cg.p4, cg1.p1, cg1.p2, cg1.p3, cg1.p4, cg.t1, cg.t2, cg1.t1, cg1.t2);
            if (cg.t1 * cg1.t1 == cg.t2 * cg1.t2)
                search_cg3(cg.p1, cg.p2, cg.p3, cg.p4, cg1.p3, cg1.p4, cg1.p1, cg1.p2, cg.t1, cg.t2, cg1.t2, cg1.t1);
            cg1 = cg1.nx;
        }
    }

    /**
     * Searches for and creates a congruency by comparing endpoints and ratios of two segments.
     *
     * @param a first endpoint of the first segment
     * @param b second endpoint of the first segment
     * @param c third endpoint of the first segment
     * @param d fourth endpoint of the first segment
     * @param a1 first endpoint of the second segment
     * @param b1 second endpoint of the second segment
     * @param c1 third endpoint of the second segment
     * @param d1 fourth endpoint of the second segment
     * @param k1 first ratio component of the first segment
     * @param k2 second ratio component of the first segment
     * @param k3 first ratio component of the second segment
     * @param k4 second ratio component of the second segment
     * @return true if a valid congruency is found or established; false otherwise
     */
    final boolean search_cg3(int a, int b, int c, int d, int a1, int b1, int c1, int d1, int k1, int k2, int k3, int k4) {

        int t1, t2, t3, t4, m1, m2;
        if (a == a1) {
            t1 = b;
            t2 = b1;
            m1 = a;
        } else if (a == b1) {
            t1 = b;
            t2 = a1;
            m1 = a;
        } else if (b == a1) {
            t1 = a;
            t2 = b1;
            m1 = b;
        } else if (b == b1) {
            t1 = a;
            t2 = a1;
            m1 = b;
        } else
            return false;

        if (c == c1) {
            t3 = d;
            t4 = d1;
            m2 = c;
        } else if (c == d1) {
            t3 = d;
            t4 = c1;
            m2 = c;
        } else if (d == c1) {
            t3 = c;
            t4 = d1;
            m2 = d;
        } else if (d == d1) {
            t3 = c;
            t4 = c1;
            m2 = d;
        } else
            return false;

        if (check_eqdistance(t1, t2, t3, t4) && xcoll(t1, t2, m1) && xcoll(t3, t4, m2)
                && !xcong(t1, t2, t3, t4)) {
            add_codb(CO_CONG, a, b, c, d, k1, k2, 0, 0);
            add_codb(CO_CONG, a1, b1, c1, d1, k3, k4, 0, 0);
            add_cong(0, 0, t1, t2, t3, t4);
            pop_codb();
            pop_codb();
        }
        return true;
    }

    /**
     * Adds a congruency segment based on two lines and their ratio components.
     *
     * @param lm lemma identifier
     * @param l_no line number flag
     * @param a the first point of the first line
     * @param b the second point of the first line
     * @param p the first point of the second line
     * @param q the second point of the second line
     * @param t1 first ratio component
     * @param t2 second ratio component
     */
    final void add_cong(int lm, int l_no, int a, int b, int p, int q, int t1, int t2) {
        CongSeg cg;
        if (!valid(lm)) return;

        if (a == b || p == q) return;

        if (t1 == t2 && xcong(a, b, p, q))
            return;

        if (xcong1(a, b, p, q)) return;

        if (search_only_exists_ln(a, b, p, q)) return;

        if (!check_eqdistance(a, b, p, q, t1, t2)) {
            add_checkError();
            return;
        }

        cg = new CongSeg();
        cg.co = co_db.nx;
        if (l_no != 0) {
            Cond co = cg.co;
            if (co != null) co.nx = null;
        }
        cg.type = 2;
        cg.lemma = lm;
        cg.nx = null;
        if (a < b) {
            cg.p1 = a;
            cg.p2 = b;
        } else {
            cg.p1 = b;
            cg.p2 = a;
        }
        if (p < q) {
            cg.p3 = p;
            cg.p4 = q;
        } else {
            cg.p3 = q;
            cg.p4 = p;
        }
        if (cg.p1 > cg.p3) {
            int t = cg.p1;
            cg.p1 = cg.p3;
            cg.p3 = t;
            t = cg.p2;
            cg.p2 = cg.p4;
            cg.p4 = t;
            t = t1;
            t1 = t2;
            t2 = t;
        }

        int g = (int) gcd(t1, t2);

        if (g == 0) {
            // TODO. Handle this.
            System.err.println("Division by zero: " + this.toString());
            return;
        }

        cg.t1 = t1 / g;
        cg.t2 = t2 / g;
        if (t1 == 0 || t2 == 0) {
            int k = 0;
        }
        if (t1 == t2 && xcong(a, b, p, q))
            return;

        if (t1 == t2) {
            last_cg.nx = cg;
            last_cg = cg;
            add_cgs(cg.p1, cg.p2, cg.p3, cg.p4);
        } else {
            last_rg.nx = cg;
            last_rg = cg;
        }

        new_pr(CO_CONG);
        last_nd.u.cg = cg;
    }

    /**
     * Adds a congruency segment for two lines without specifying ratio components.
     *
     * @param lm lemma identifier
     * @param l_no line number flag
     * @param a the first point of the first line
     * @param b the second point of the first line
     * @param p the first point of the second line
     * @param q the second point of the second line
     */
    final void add_cong(int lm, int l_no, int a, int b, int p, int q) {
        CongSeg cg;
        if (a == b || p == q) return;
        if (!valid(lm)) return;

        if (search_only_exists_ln(a, b, p, q)) return;
        if (!check_eqdistance(a, b, p, q)) {
            this.add_checkError();
            return;
        }

        if (xcong(a, b, p, q)) return;
        {
            cg = new CongSeg();
            cg.co = co_db.nx;
            if (l_no != 0) {
                Cond co = cg.co;
                if (co != null) co.nx = null;
            }
            cg.type = 2;
            cg.lemma = lm;
            cg.nx = null;
            if (a < b) {
                cg.p1 = a;
                cg.p2 = b;
            } else {
                cg.p1 = b;
                cg.p2 = a;
            }
            if (p < q) {
                cg.p3 = p;
                cg.p4 = q;
            } else {
                cg.p3 = q;
                cg.p4 = p;
            }
            if (cg.p1 > cg.p3) {
                int t1 = cg.p1;
                cg.p1 = cg.p3;
                cg.p3 = t1;
                t1 = cg.p2;
                cg.p2 = cg.p4;
                cg.p4 = t1;
            }
            if (cg.p3 >= 5) {
                int k = 0;
            }
            cg.t1 = cg.t2 = 1;
            last_cg.nx = cg;
            last_cg = cg;
        }
        add_cgs(cg.p1, cg.p2, cg.p3, cg.p4);
        new_pr(CO_CONG);
        last_nd.u.cg = cg;
    }

    /**
     * Adds a congruence segment based on the provided point indices and lemma.
     *
     * @param lm the lemma identifier
     * @param p1 first point index of the first segment
     * @param p2 second point index of the first segment
     * @param p3 first point index of the second segment
     * @param p4 second point index of the second segment
     * @param a first point index of the first congruence pair
     * @param b second point index of the first congruence pair
     * @param p first point index of the second congruence pair
     * @param q second point index of the second congruence pair
     */
    final void add_cong1(int lm, int p1, int p2, int p3, int p4, int a, int b, int p, int q) {
        CongSeg cg;
        Cond co;

        if (!valid(lm)) return;
        if (search_only_exists_ln(a, b, p, q)) return;
        if (!check_eqdistance(a, b, p, q)) {
            this.add_checkError();
            return;
        }
        if (xcong(a, b, p, q)) return;
        {
            cg = new CongSeg();
            cg.type = 2;
            cg.lemma = lm;
            cg.nx = null;
            if (a < b) {
                cg.p1 = a;
                cg.p2 = b;
            } else {
                cg.p1 = b;
                cg.p2 = a;
            }
            if (p < q) {
                cg.p3 = p;
                cg.p4 = q;
            } else {
                cg.p3 = q;
                cg.p4 = p;
            }
            if (cg.p1 > cg.p3) {
                int t1 = cg.p1;
                cg.p1 = cg.p3;
                cg.p3 = t1;
                t1 = cg.p2;
                cg.p2 = cg.p4;
                cg.p4 = t1;
            }
            cg.t1 = cg.t2 = 1;
            last_cg.nx = cg;
            last_cg = cg;

            add_cgs(cg.p1, cg.p2, cg.p3, cg.p4);
        }
        if (p2 > p1) {
            lm = p1;
            p1 = p2;
            p2 = lm;
            //ex_ch(lm, p1, p2)
        }
        if (p4 > p3) {
            lm = p3;
            p3 = p4;
            p4 = lm;
            //ex_ch(lm, p3, p4)
        }
        if (p1 != p3 || p2 != p4) {
            co = add_coxy(CO_CONG);
            co.p[0] = p1;
            co.p[1] = p2;
            co.p[2] = p3;
            co.p[3] = p4;
        }
        cg.co = co_xy.nx;
        //adj_cg(cg);
        new_pr(CO_CONG);
        last_nd.u.cg = cg;
    }

    /**
     * Determines the index for a ratio segment based on point congruence.
     *
     * @param a first point index
     * @param b second point index
     * @param ra the ratio segment reference
     * @param m mode indicator affecting the returned index
     * @return an integer representing the determined index
     */
    int ind_ra(int a, int b, RatioSeg ra, int m) {
        int c = 0;
        if (xcong(a, b, ra.r[1], ra.r[2])) {
            c = 1;
            if (m == 1 && xcong(a, b, ra.r[7], ra.r[8])) c = 4;
        } else if (xcong(a, b, ra.r[3], ra.r[4])) {
            c = 2;
            if (m == 1 && xcong(a, b, ra.r[5], ra.r[6])) c = 3;
        } else if (xcong(a, b, ra.r[5], ra.r[6]))
            c = 3;
        else if (xcong(a, b, ra.r[7], ra.r[8])) c = 4;
        return (c);
    }

    /**
     * Adds a ratio segment if the ratio parameters pass the validity checks.
     *
     * @param lm the lemma identifier
     * @param o an origin or operator parameter
     * @param a first point index of the first ratio pair
     * @param b second point index of the first ratio pair
     * @param c first point index of the second ratio pair
     * @param d second point index of the second ratio pair
     * @param p first point index of the third ratio pair
     * @param q second point index of the third ratio pair
     * @param r first point index of the fourth ratio pair
     * @param s second point index of the fourth ratio pair
     * @return the created RatioSeg object, or null if the check fails
     */
    RatioSeg add_ra(int lm, int o, int a, int b, int c, int d, int p, int q, int r, int s) {
        RatioSeg ra;
        Cond co;
        if (!valid(lm) || !valid(R_RATIO)) return null;


        if (!check_ratio(a, b, c, d, p, q, r, s)) {
            this.add_checkError();
            return null;
        }
        {
            ra = new RatioSeg();
            ra.type = 2;
            ra.lemma = lm;
            ra.r[0] = o;
            ra.co = co_db.nx;
            ra.r[1] = a;
            ra.r[2] = b;
            ra.r[3] = c;
            ra.r[4] = d;
            ra.r[5] = p;
            ra.r[6] = q;
            ra.r[7] = r;
            ra.r[8] = s;
            ra.nx = null;
            last_ra.nx = ra;
            last_ra = ra;
            new_pr(CO_PROD);
            last_nd.u.ra = ra;
        }
        co_xy.nx = null;
        if (xcong(a, b, p, q)) {
            co = add_coxy(CO_PROD);
            co.u.ra = ra;
            ra.type = 0;
            add_cong1(0, a, b, p, q, c, d, r, s);
        } else if (xcong(c, d, r, s)) {
            co = add_coxy(CO_PROD);
            co.u.ra = ra;
            ra.type = 0;
            add_cong1(0, c, d, r, s, a, b, p, q);
        } else if (xcong(a, b, c, d)) {
            co = add_coxy(CO_PROD);
            co.u.ra = ra;
            ra.type = 0;
            add_cong1(0, a, b, c, d, p, q, r, s);
        } else if (xcong(p, q, r, s)) {
            co = add_coxy(CO_PROD);
            co.u.ra = ra;
            ra.type = 0;
            add_cong1(0, p, q, r, s, a, b, c, d);
        } else if (xcong(a, b, r, s) && xcong(c, d, p, q)) {
            co = add_coxy(CO_PROD);
            co.u.ra = ra;
            ra.type = 0;
            add_cong1(0, a, b, r, s, c, d, p, q);
        }
        return (ra);
    }

    /**
     * Adjusts the ratio segments by comparing with all existing segments and reconciling differences.
     *
     * @param ra the starting RatioSeg to adjust
     */
    final void adj_ra(RatioSeg ra) {
        RatioSeg r1;
        int i1, i2, i3, i4;
        while (ra != null) {
            if (ra.type == 0) {
                ra = ra.nx;
                continue;
            }
            r1 = all_ra.nx;

            while (r1 != null && r1 != ra) {
                if (r1.type == 0) {
                    r1 = r1.nx;
                    continue;
                }
                i1 = ind_ra(r1.r[1], r1.r[2], ra, 0);
                i2 = ind_ra(r1.r[3], r1.r[4], ra, 0);
                i3 = ind_ra(r1.r[5], r1.r[6], ra, 1);
                i4 = ind_ra(r1.r[7], r1.r[8], ra, 1);

                if (i1 != 0 && i2 != 0 && i1 != i2 && i1 + i2 != 5) {
                    adj_ra1(r1.r[5], r1.r[6], r1.r[7], r1.r[8], i1, i2, ra, r1);
                } else if (i3 != 0 && i4 != 0 && i3 != i4 && i3 + i4 != 5) {
                    adj_ra1(r1.r[1], r1.r[2], r1.r[3], r1.r[4], i3, i4, ra, r1);
                }

                if (i1 != 0 && i3 != 0 && i1 != i3 && i1 + i3 != 5) {
                    adj_ra1(r1.r[3], r1.r[4], r1.r[7], r1.r[8], i1, i3, ra, r1);
                } else if (i2 != 0 && i4 != 0 && i2 != i4 && i2 + i4 != 5) {
                    adj_ra1(r1.r[1], r1.r[2], r1.r[5], r1.r[6], i2, i4, ra, r1);
                }

                if (i1 != 0 && i4 != 0 && i1 + i4 == 5) {
                    adj_ra2(r1.r[3], r1.r[4], r1.r[5], r1.r[6], i1, ra, r1);
                } else if (i2 != 0 && i3 != 0 && i2 + i3 == 5) {
                    adj_ra2(r1.r[1], r1.r[2], r1.r[7], r1.r[8], i2, ra, r1);
                }
                m1:
                r1 = r1.nx;
            }
            m2:
            ra = ra.nx;
        }
        test_ra = last_ra;
    }

    /**
     * Adjusts a ratio segment relation using the specified ratio parameters and indices.
     *
     * @param a first point index of the first ratio pair
     * @param b second point index of the first ratio pair
     * @param c first point index of the second ratio pair
     * @param d second point index of the second ratio pair
     * @param i1 indicator for the first ratio selection in r1
     * @param i2 indicator for the second ratio selection in r1
     * @param r1 the ratio segment to be adjusted
     * @param r2 a related ratio segment used for adjustment context
     */
    final void adj_ra1(int a, int b, int c, int d, int i1, int i2, RatioSeg r1, RatioSeg r2) {
        int p, q, r, s;
        Cond co;
        RatioSeg ra;
        p = q = r = s = 0;

        switch (5 - i2) {
            case 1:
                p = r1.r[1];
                q = r1.r[2];
                break;
            case 2:
                p = r1.r[3];
                q = r1.r[4];
                break;
            case 3:
                p = r1.r[5];
                q = r1.r[6];
                break;
            case 4:
                p = r1.r[7];
                q = r1.r[8];
                break;
            default:
                Cm.print("adj_ra1");
                break; /// exit
        }
        switch (5 - i1) {
            case 1:
                r = r1.r[1];
                s = r1.r[2];
                break;
            case 2:
                r = r1.r[3];
                s = r1.r[4];
                break;
            case 3:
                r = r1.r[5];
                s = r1.r[6];
                break;
            case 4:
                r = r1.r[7];
                s = r1.r[8];
                break;
            default:
                Cm.print("adj_ra11");
                break;
        }
        if (xeq_ratio(a, b, c, d, p, q, s, r)) return;
        ra = add_ra(0, 0, a, b, c, d, p, q, r, s);
        if (ra == null) {
        } else if (r1.co == null && r2.co == null)
            last_ra.co = null;
        else {
            co_xy.nx = null;
            co = add_coxy(CO_PROD);
            co.u.ra = r2;
            co = add_coxy(CO_PROD);
            co.u.ra = r1;
            last_ra.co = co_xy.nx;
        }
    }

    /**
     * Adjusts a ratio segment relation based on a single indicator and updates connection information.
     *
     * @param a first point index of the first ratio pair
     * @param b second point index of the first ratio pair
     * @param c first point index of the second ratio pair
     * @param d second point index of the second ratio pair
     * @param i1 indicator dictating the selection of ratio components in r1
     * @param r1 the ratio segment to be adjusted
     * @param r2 a related ratio segment for context
     */
    final void adj_ra2(int a, int b, int c, int d, int i1, RatioSeg r1, RatioSeg r2) {
        int p, q, r, s;
        Cond co;
        RatioSeg ra;

        if (i1 == 1 || i1 == 4) {
            p = r1.r[3];
            q = r1.r[4];
            r = r1.r[5];
            s = r1.r[6];
        } else {
            p = r1.r[1];
            q = r1.r[2];
            r = r1.r[7];
            s = r1.r[8];
        }

        if (xeq_ratio(a, b, p, q, r, s, c, d)) return;
        ra = add_ra(0, 0, a, b, p, q, r, s, c, d);
        if (ra == null) {
        } else if (r1.co == null && r2.co == null)
            last_ra.co = null;
        else {
            co_xy.nx = null;
            co = add_coxy(CO_PROD);
            co.u.ra = r2;
            co = add_coxy(CO_PROD);
            co.u.ra = r1;
            last_ra.co = co_xy.nx;
        }
    }

    /**
     * Adds a ratio segment if no equivalent ratio exists.
     *
     * @param lm the lemma identifier
     * @param o an origin or operator parameter
     * @param a first point index of the first ratio pair
     * @param b second point index of the first ratio pair
     * @param c first point index of the second ratio pair
     * @param d second point index of the second ratio pair
     * @param p first point index of the third ratio pair
     * @param q second point index of the third ratio pair
     * @param r first point index of the fourth ratio pair
     * @param s second point index of the fourth ratio pair
     */
    final void add_ratio(int lm, int o, int a, int b, int c, int d, int p, int q, int r, int s) {
        if (xeq_ratio(a, b, c, d, p, q, r, s)) return;
        add_ra(lm, o, a, b, c, d, p, q, r, s);
    }

    /**
     * Adds a ratio segment using only four points by inferring the remaining ratio components.
     *
     * @param lm the lemma identifier
     * @param o an origin or operator parameter
     * @param a first point index
     * @param b second point index
     * @param c third point index
     * @param d fourth point index
     */
    final void add_ratioo(int lm, int o, int a, int b, int c, int d) {
        if (!xeq_ratio(o, a, o, c, o, b, o, d))
            add_ra(lm, 1, o, a, o, c, o, b, o, d);

        if (!xeq_ratio(o, a, o, c, a, b, c, d))
            add_ra(lm, 1, o, a, o, c, a, b, c, d);

        if (!xeq_ratio(o, b, o, d, a, b, c, d))
            add_ra(lm, 1, o, b, o, d, a, b, c, d);

        if (!xeq_ratio(o, a, a, c, o, b, b, d))
            add_ra(lm, 1, o, a, a, c, o, b, b, d);

        if (!xeq_ratio(o, c, c, a, o, d, d, b))
            add_ra(lm, 1, o, c, c, a, o, d, d, b);
    }

    /**
     * Checks if two points are equal.
     *
     * @param p1 the first point index
     * @param p2 the second point index
     * @return true if points are equal, false otherwise
     */
    boolean meq_pt(int p1, int p2) {
        return p1 == p2;
    }

    /**
     * Processes and validates all conclusions.
     *
     * @return true if all conclusions are processed successfully
     */
    public boolean sbase() {
        for (int i = 1; i <= cns_no; i++) {
            if (!isConclusion(i))
                do_i_cons(i);
        }

        return true;
    }

    /**
     * Sets the example context for the theorem by initializing various parameters.
     *
     * @param tm the geometric term containing the example information
     */
    final public void setExample(GTerm tm) {
        gt = tm;
        cons_no = tm.getCons_no();
        Cond tco = tm.getConc();
        setConc(tco);
        tm.setAllpts(allpts);
        cns_no = tm.setAllcons(allcns);
        pts_no = tm.getPointsNum();
    }

    /**
     * Sets the conclusion condition.
     *
     * @param tco the condition object containing conclusion data
     */
    final public void setConc(Cond tco) {
        if (tco == null) return;

        conc.pred = tco.pred;
        for (int i = 0; i < Cond.MAX_GEO; i++)
            conc.p[i] = tco.p[i];
        conc.no = tco.no;
        if (conc.pred == CO_COLL) {
            for (int i = 0; i < 3; i++)
                for (int j = i + 1; j < 3; j++) {
                    if (conc.p[i] < conc.p[j]) {
                        int t = conc.p[i];
                        conc.p[i] = conc.p[j];
                        conc.p[j] = t;
                    }
                }
        } else if (conc.pred == CO_CYCLIC) {
            if (conc.p[0] != 0 && conc.p[4] == 0) {
                for (int i = 4; i >= 1; i--)
                    conc.p[i] = conc.p[i - 1];
                conc.p[0] = 0;
            }
        }
    }

    /**
     * Processes the individual conclusion for the specified index.
     *
     * @param ptn the conclusion index
     * @return true after processing the conclusion
     */
    public boolean do_i_cons(int ptn) {
        do_pd(ptn, allcns[ptn].type, allcns[ptn].ps);
        return true;
    }

    /**
     * Processes the predicate for a given conclusion based on its type and parameters.
     *
     * @param ptn the conclusion index
     * @param t the type of predicate
     * @param p an array of parameters associated with the predicate
     * @return true if the predicate is processed successfully; false otherwise
     */
    public boolean do_pd(int ptn, int t, int[] p) {
        if (t == 0) return true;
        switch (t) {
            case C_POINT:
                break;
            case C_CONSTANT:
                break;
            case C_MIDPOINT:
                add_line1(0, p[0], p[1], p[2]);
                add_mid(0, p[0], p[1], p[2]);
                break;
            case C_FOOT:    // foot
                add_line1(0, p[0], p[2], p[3]);
                add_tline(0, p[0], p[1], p[2], p[3]);
                break;
            case C_O_C: {
                ACir cr = fd_cr_op(p[1], p[2]);

                if (cr == null)
                    add_cir2(0, p[1], p[0], p[2]);
                else {
                    for (int i = 0; i <= cr.no; i++)
                        add_cong(0, 0, cr.o, cr.pt[i], cr.o, p[0]);
                    add_pt2c(p[0], cr);
                }
                add_cong(0, 0, p[0], p[1], p[1], p[2]);
            }
            break;
            case C_O_L:
                add_line1(0, p[0], p[1], p[2]);
                break;
            case C_O_T:
                add_tline(0, p[0], p[1], p[2], p[3]);
                break;
            case C_O_P:
                add_pline(0, p[0], p[1], p[2], p[3]);
                break;
            case C_EQANGLE:
            case C_O_A:
                add_ea_pt(0, p[0], p[1], p[2], p[3], p[4], p[5]);
                break;
            case C_O_R:
                add_cong(0, 0, p[0], p[1], p[2], p[3]);
                break;
            case C_O_B:
                add_cir2(0, p[0], p[1], p[2]);
                break;
            case C_NSQUARE: {
                add_tline(0, p[0], p[1], p[1], p[2]);
                add_cir2(0, p[1], p[0], p[2]);
                add_cong(0, 0, p[0], p[1], p[1], p[2]);
            }
            break;
            case C_PSQUARE: {
                add_tline(0, p[0], p[1], p[1], p[2]);
                add_cir2(0, p[1], p[0], p[2]);
                add_cong(0, 0, p[0], p[1], p[1], p[2]);
            }
            break;
            case C_I_CC:
                add_cir2(0, p[1], p[0], p[2]);
                add_cir2(0, p[3], p[0], p[4]);
                break;
            case C_CIRCUM:
                add_cir3(0, p[0], p[1], p[2], p[3]);
                break;
            case C_ORTH:
                add_tline(0, p[0], p[1], p[2], p[3]);
                add_tline(0, p[0], p[2], p[1], p[3]);
                add_tline(0, p[0], p[3], p[1], p[2]);
                break;
            case C_PETRIANGLE:
                add_cir2(0, p[0], p[1], p[2]);
                add_cir2(0, p[1], p[2], p[0]);
                add_cir2(0, p[2], p[1], p[0]);
                add_cong(0, 0, p[0], p[1], p[0], p[2]);
                add_cong(0, 0, p[1], p[0], p[1], p[2]);
                add_cong(0, 0, p[2], p[1], p[2], p[0]);
                add_ea_pt(0, p[0], p[1], p[2], p[1], p[2], p[0]);
                add_ea_pt(0, p[0], p[1], p[2], p[2], p[0], p[1]);
                add_ea_pt(0, p[1], p[2], p[0], p[2], p[0], p[1]);
                break;
            case C_NETRIANGLE:
                add_cir2(0, p[0], p[1], p[2]);
                add_cir2(0, p[1], p[2], p[0]);
                add_cir2(0, p[2], p[1], p[0]);
                add_ea_pt(0, p[0], p[1], p[2], p[1], p[2], p[0]);
                add_ea_pt(0, p[0], p[1], p[2], p[2], p[0], p[1]);
                add_ea_pt(0, p[1], p[2], p[0], p[2], p[0], p[1]);
                break;
            case C_REF:  //Point wrpt Point
                add_line1(0, p[0], p[1], p[2]);
                add_mid(0, p[2], p[1], p[0]);
                break;
            case C_SYM:  // Point wrpt LINE
                fadd_ln(p[0], p[2]);
                fadd_ln(p[0], p[3]);
                fadd_ln(p[1], p[2]);
                fadd_ln(p[1], p[3]);
                fadd_ln(p[0], p[1]);
                fadd_ln(p[2], p[3]);
                add_cir2(0, p[2], p[0], p[1]);
                add_cir2(0, p[3], p[0], p[1]);
                add_cong(0, 0, p[2], p[0], p[2], p[1]);
                add_cong(0, 0, p[3], p[0], p[3], p[1]);

                add_tline(0, p[0], p[1], p[2], p[3]);
                break;
            case C_ICENT:
                add_ea_pt(0, p[2], p[1], p[0], p[0], p[1], p[3]);
                add_ea_pt(0, p[1], p[3], p[0], p[0], p[3], p[2]);
                add_ea_pt(0, p[3], p[2], p[0], p[0], p[2], p[1]);
                break;
            case C_PRATIO:
                add_pline(0, p[0], p[1], p[2], p[3]);
                add_ratio(p[0], p[1], p[2], p[3], p[4], p[5]);
                break;
            case C_TRATIO:
                add_tline(0, p[0], p[1], p[2], p[3]);
                add_ratio(p[0], p[1], p[2], p[3], p[4], p[5]);
                break;
            case C_EQDISTANCE:
                add_cong(0, 0, p[0], p[1], p[2], p[3]);
                break;
            case C_SANGLE:
                add_at(0, p[0], p[1], p[2], p[3] * A_TIME);
                break;
            case C_LRATIO:
                add_laratio(p[0], p[1], p[2], p[3], p[4], p[5]);
                break;
            case C_RATIO:
                add_ratio(0, 0, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
                break;
            case C_NRATIO:
                add_ratio(p[0], p[1], p[2], p[3], p[4], p[5]);
                break;
            case C_LINE:
                break;
            case C_TRIANGLE:
                break;
            case C_ISO_TRI:
                add_pg_ln(p);
                add_cong(0, 0, p[0], p[1], p[0], p[2]);
                add_cir2(0, p[0], p[1], p[2]);
                break;
            case C_R_TRI:
                add_tline(0, p[0], p[1], p[0], p[2]);
                break;
            case C_EQ_TRI:
                add_e_triangle(ptn, p);
                break;
            case C_QUADRANGLE:
                break;
            case C_PENTAGON:
                break;
            case C_POLYGON:
                break;
            case C_TRAPEZOID:
                add_pline(0, p[0], p[1], p[2], p[3]);
                break;
            case C_R_TRAPEZOID:
                add_r_trapezoid(p);
                break;
            case C_PARALLELOGRAM:
                add_parallelogram(p);
                break;
            case C_LOZENGE:
                add_lozenge(p);
                break;
            case C_RECTANGLE:
                add_rectangle(p);
                break;
            case C_SQUARE:
                add_square(p);
                break;
            case C_ANGLE_BISECTOR:
                add_ea_pt(0, p[1], p[2], p[0], p[0], p[2], p[3]);
                break;
            case C_CIRCLE:
                add_cir_n(p);
                break;
            case C_LC_TANGENT:
                add_tline(0, p[0], p[1], p[1], p[2]);
                break;

            case CO_CONG:
                add_cong(0, 0, p[0], p[1], p[2], p[3]);
                break;
            case CO_COLL:
                add_line(0, p[0], p[1], p[2]);
                break;
            case CO_PARA:
                add_pline(0, p[0], p[1], p[2], p[3]);
                break;
            case CO_PERP:
                add_tline(0, p[0], p[1], p[2], p[3]);
                break;
            case CO_MIDP:
                add_line1(0, p[0], p[1], p[2]);
                add_mid(0, p[0], p[1], p[2]);
                break;
            case CO_ACONG:
                add_ea_pt(0, p[0], p[1], p[2], p[3], p[4], p[5]);
                break;
            case CO_PBISECT:
                add_cong(0, 0, p[0], p[1], p[0], p[2]);
                break;
            default:
                do_pred2(p, t, ptn);
                return false;
        }
        return true;

    }

    /**
     * Adds a circle defined by the provided parameters.
     *
     * @param p an array containing circle parameters; p[2] must be non-zero
     */
    public void add_cir_n(int p[]) {
        if (p[2] == 0) return;

        add_cir2(0, p[0], p[1], p[2]);
        ACir c = fo_cr(p[0], p[1], p[2], 0, 0);
        for (int i = 1; i < p.length; i++) {
            if (p[i] != 0)
                add_pt2c(p[i], c);
        }
        for (int i = 0; i <= c.no; i++)
            for (int j = 0; j <= c.no; j++) {
                if (i != j)
                    add_cong(0, 0, c.o, c.pt[i], c.o, c.pt[j]);
            }
    }

    /**
     * Adds a line angle ratio by drawing lines between the provided endpoints and establishing
     * squared ratio relationships.
     *
     * @param p1 the first point index
     * @param p2 the second point index
     * @param p3 the third point index
     * @param p4 the fourth point index
     * @param t1 the first ratio value (to be squared)
     * @param t2 the second ratio value (to be squared)
     */
    public void add_laratio(int p1, int p2, int p3, int p4, int t1, int t2) {
        add_line1(0, p1, p2, p3);
        add_line1(0, p1, p2, p4);
        add_cong(0, 0, p1, p2, p3, p4, t1 * t1, t2 * t2);
    }

    /**
     * Adds a congruence relation between segments based on the provided endpoints and squared ratio values.
     *
     * @param p1 the first point index of the first segment
     * @param p2 the second point index of the first segment
     * @param p3 the first point index of the second segment
     * @param p4 the second point index of the second segment
     * @param t1 the first ratio value (to be squared)
     * @param t2 the second ratio value (to be squared)
     */
    public void add_ratio(int p1, int p2, int p3, int p4, int t1, int t2) {
        add_cong(0, 0, p1, p2, p3, p4, t1 * t1, t2 * t2);
    }

    /**
     * Adds a polygon line by connecting the series of points in a loop.
     *
     * @param p an array of point indices representing the polygon vertices; a zero value indicates termination
     */
    public void add_pg_ln(int[] p) {
        int t = p[0];
        if (t == 0) return;

        for (int i = 1; i < p.length && p[i] != 0; i++) {
            fadd_ln(t, p[i]);
            t = p[i];
        }
        fadd_ln(p[0], t);
    }

    /**
     * Adds a trapezoid by drawing the primary lines for its shape.
     *
     * @param p an array of 4 vertex indices representing the trapezoid
     */
    public void add_r_trapezoid(int[] p) {
        add_pline(0, p[0], p[1], p[2], p[3]);
        add_tline(0, p[0], p[1], p[0], p[3]);
        add_tline(0, p[0], p[3], p[2], p[3]);
    }

    /**
     * Adds a parallelogram by drawing its outline, congruencies, and structural elements.
     *
     * @param p an array of 4 vertex indices representing the parallelogram
     */
    public void add_parallelogram(int[] p) {
        add_pline(0, p[0], p[1], p[2], p[3]);
        add_pline(0, p[0], p[3], p[1], p[2]);
        add_cong(0, 0, p[0], p[1], p[2], p[3]);
        add_cong(0, 0, p[0], p[3], p[1], p[2]);
        tri_type = 1;
        add_stri(0, 1, p[0], p[1], p[2], p[2], p[3], p[0]);
        add_stri(0, 1, p[0], p[1], p[3], p[2], p[3], p[1]);
    }

    /**
     * Adds a lozenge by drawing its lines and congruencies.
     *
     * @param p an array of 4 vertex indices representing the lozenge
     */
    public void add_lozenge(int[] p) {
        add_pline(0, p[0], p[1], p[2], p[3]);
        add_pline(0, p[0], p[3], p[1], p[2]);
        for (int i = 0; i < 4; i++)
            for (int j = i + 1; j < 4 && j != i; j++) {
                add_cong(0, 0, p[i], p[(i + 1) % 4], p[j], p[(j + 1) % 4]);
            }

        tri_type = 1;
        add_stri(0, 1, p[0], p[1], p[2], p[2], p[3], p[0]);
        add_stri(0, 1, p[0], p[1], p[3], p[2], p[3], p[1]);
    }

    /**
     * Adds a rectangle by drawing its outline, tangents, congruencies, and structural elements.
     *
     * @param p an array of 4 vertex indices representing the rectangle
     */
    public void add_rectangle(int[] p) {
        add_pline(0, p[0], p[1], p[2], p[3]);
        add_pline(0, p[0], p[3], p[2], p[1]);
        add_tline(0, p[0], p[1], p[0], p[3]);
        add_tline(0, p[0], p[1], p[1], p[2]);
        add_tline(0, p[0], p[3], p[2], p[3]);
        add_tline(0, p[2], p[3], p[2], p[1]);
        add_cong(0, 0, p[0], p[1], p[2], p[3]);
        add_cong(0, 0, p[0], p[3], p[1], p[2]);
        tri_type = 1;
        add_stri(0, 1, p[0], p[1], p[2], p[2], p[3], p[0]);
        add_stri(0, 1, p[0], p[1], p[3], p[2], p[3], p[1]);
    }

    /**
     * Adds a square by drawing its lines, tangents, congruencies, and angle structures.
     *
     * @param p an array of 4 vertex indices representing the square
     */
    public void add_square(int[] p) {
        add_pline(0, p[0], p[1], p[2], p[3]);
        add_pline(0, p[0], p[3], p[2], p[1]);
        add_tline(0, p[0], p[1], p[0], p[3]);
        add_tline(0, p[0], p[1], p[1], p[2]);
        add_tline(0, p[0], p[3], p[2], p[3]);
        add_tline(0, p[2], p[3], p[2], p[1]);

        for (int i = 0; i < 4; i++)
            for (int j = i + 1; j < 4 && j != i; j++) {
                add_cong(0, 0, p[i], p[(i + 1) % 4], p[j], p[(j + 1) % 4]);
            }


        for (int i = 0; i < 4; i++) {
            int i1 = i;
            int i2 = (i + 1) % 4;
            int i3 = (i + 2) % 4;
            add_at0(0, p[i2], p[i1], p[i3], A_45);
            add_at0(0, p[i2], p[i3], p[i1], A_45);
            add_at0(0, p[i1], p[i2], p[i3], A_90);
        }
        tri_type = 1;
        for (int i = 0; i < 4; i++) {
            int a = p[i];
            int b = p[(i + 1) % 4];
            int c = p[(i + 3) % 4];
            add_ea_pt_t(0, a, b, c, b, c, a);
            for (int j = i + 1; j < 4; j++) {
                int a1 = p[j];
                int b1 = p[(j + 1) % 4];
                int c1 = p[(j + 3) % 4];
                add_stri(0, 1, a, b, c, a1, b1, c1);
                add_ea_pt_t(0, a, b, c, a1, b1, c1);
                add_ea_pt_t(0, b, c, a, b1, c1, a1);
                add_ea_pt_t(0, a, b, c, b1, c1, a1);
                add_ea_pt_t(0, b, c, a, a1, b1, c1);

            }
        }


    }

    /**
     * Adds an auxiliary point based on the provided ProPoint type.
     *
     * @param pt the ProPoint object containing point and type information
     * @return true if the auxiliary point is added successfully; false otherwise
     */
    public boolean add_auxpt(ProPoint pt) {

        int[] p = pt.ps;
        int mk = pt.type;
        switch (mk) {
            case C_MIDPOINT:
                add_mid(0, p[0], p[1], p[2]);
                add_line(0, p[0], p[1], p[2]);
                break;
            case C_I_LL:
                add_line(0, p[0], p[1], p[2]);
                add_line(0, p[0], p[3], p[4]);
                break;
            case C_FOOT:
                add_line(0, p[0], p[2], p[3]);
                add_tline(0, p[0], p[1], p[2], p[3]);
                break;
            case C_I_LC:
                add_line(0, p[0], p[1], p[2]);
                add_cir2(0, p[3], p[0], p[4]);
                break;
            case C_I_LP:
                add_line(0, p[0], p[1], p[2]);
                add_pline(0, p[0], p[3], p[4], p[5]);
                break;
            case C_I_LT:
                add_line(0, p[0], p[1], p[2]);
                add_tline(0, p[0], p[3], p[4], p[5]);
                break;
            case C_I_PC:
                add_pline(0, p[0], p[1], p[2], p[3]);
                add_cir2(0, p[4], p[0], p[5]);
                break;
            case C_CIRCUM:
                break;
            default:
                break;
        }

        return true;
    }

    /**
     * Performs a predicate operation based on the given type.
     *
     * @param p1 the array of points and parameters (first element is used as predicate target)
     * @param mk the predicate type code
     * @param ptn the predicate target (overwritten by the first element of p1)
     * @return true if the predicate action is executed successfully, false otherwise
     */
    public boolean do_pred2(int[] p1, int mk, int ptn) {
        ptn = p1[0];
        int[] p = new int[p1.length];

        for (int i = 1; i < p.length; i++)
            p[i - 1] = p1[i];

        switch (mk) {
            case C_I_LL:
                add_line(0, ptn, p[0], p[1]);
                add_line(0, ptn, p[2], p[3]);
                break;
            case C_FOOT:    // foot
                add_line(0, ptn, p[1], p[2]);
                add_tline(0, ptn, p[0], p[1], p[2]);
                break;
            case C_I_LT:    // perp D A B C
                add_line(0, ptn, p[0], p[1]);
                add_tline(0, ptn, p[2], p[3], p[4]);
                break;
            case C_I_LP:    //p line
                add_line(0, ptn, p[0], p[1]);
                add_pline(0, ptn, p[2], p[3], p[4]);
                break;
            case C_I_PP:   // pp
                add_pline(0, ptn, p[0], p[1], p[2]);
                add_pline(0, ptn, p[3], p[4], p[5]);
                break;
            case C_I_PT:   //pt
                add_pline(0, ptn, p[0], p[1], p[2]);
                add_tline(0, ptn, p[3], p[4], p[5]);
                break;
            case C_I_TT:
                add_tline(0, ptn, p[0], p[1], p[2]);
                add_tline(0, ptn, p[3], p[4], p[5]);
                break;
            case C_I_LB:
                add_line(0, ptn, p[0], p[1]);
                add_cir2(0, ptn, p[2], p[3]);
                break;
            case C_I_LC:
                add_line(0, ptn, p[0], p[1]);
                add_cir2(0, p[2], ptn, p[3]);
                if (p[0] == p[2] && p[1] == p[3]) {
                    add_mid(0, p[2], ptn, p[3]);
                } else if (p[1] == p[2] && p[0] == p[3]) {
                    add_mid(0, p[2], ptn, p[3]);
                }
                break;

            case C_I_PC:
                add_pline(0, ptn, p[0], p[1], p[2]);
                add_cir2(0, p[3], ptn, p[4]);
                break;
            case C_I_PB:
                add_pline(0, ptn, p[0], p[1], p[2]);
                add_cir2(0, ptn, p[3], p[4]);
                break;
            case C_I_TC:
                add_tline(0, ptn, p[0], p[1], p[2]);
                add_cir2(0, p[3], ptn, p[4]);
                break;
            case C_I_BC:
                add_cir2(0, ptn, p[0], p[1]);
                add_cir2(0, p[2], ptn, p[3]);
                break;
            case C_I_BB:
                add_cong(0, 0, ptn, p[0], ptn, p[1]);
                add_cong(0, 0, ptn, p[2], ptn, p[3]);
                break;
            case C_I_TB:
                add_tline(0, ptn, p[0], p[1], p[2]);
                add_cir2(0, ptn, p[3], p[4]);
                break;
            case C_I_LR:// L RCircle
                add_line(0, ptn, p[0], p[1]);
                add_cong(0, 0, p[2], ptn, p[3], p[4]);
                break;
            case C_I_TR:
                add_tline(0, ptn, p[0], p[1], p[2]);
                add_cong(0, 0, ptn, p[3], p[3], p[4]);
                break;
            case C_I_CC:
                add_cir2(0, p[0], ptn, p[1]);
                add_cir2(0, p[2], ptn, p[3]);
                break;
            case C_I_CR:
                add_cir2(0, p[0], ptn, p[1]);
                add_cong(0, 0, p[2], ptn, p[3], p[4]);
                break;
            case C_I_RR:
                add_cong(0, 0, p[0], ptn, p[1], p[2]);
                add_cong(0, 0, p[3], ptn, p[4], p[5]);
                break;
            case C_I_LA:
                this.add_line(0, p1[0], p1[1], p1[2]);
                this.add_ea_pt(0, p1[0], p1[3], p1[4], p1[5], p1[6], p1[7]);
                break;
            case C_I_PA:
                this.add_pline(0, p1[0], p1[1], p1[2], p1[3]);
                this.add_ea_pt(0, p1[4], p1[5], p1[6], p1[7], p1[8], p1[9]);
                break;
            case C_I_TA:
                this.add_tline(0, p1[0], p1[1], p1[2], p1[3]);
                this.add_ea_pt(0, p1[4], p1[5], p1[6], p1[7], p1[8], p1[9]);
                break;
            case C_I_BA:
                this.add_cong(0, 0, p1[0], p1[1], p1[0], p1[2]);
                this.add_ea_pt(0, p1[0], p1[3], p1[4], p1[5], p1[6], p1[7]);
                break;
            case C_I_AA:
                this.add_ea_pt(0, p1[0], p1[1], p1[2], p1[3], p1[4], p1[5]);
                this.add_ea_pt(0, p1[0], p1[6], p1[7], p1[8], p1[9], p1[10]);
                break;
            case C_NSQUARE:
                add_tline(0, ptn, p[0], p[0], p[1]);
                add_cir2(0, p[0], ptn, p[1]);
                break;
            case C_PSQUARE:
                add_tline(0, ptn, p[0], p[0], p[1]);
                add_cir2(0, p[0], ptn, p[1]);
                break;
            case C_ICENT:
            case CO_INCENT:
                add_ea_pt(0, p[1], p[0], p[2], p[2], p[0], ptn);
                add_ea_pt(0, p[0], p[1], p[2], p[2], p[1], ptn);
                add_ea_pt(0, p[0], ptn, p[2], p[2], ptn, p[1]);
                break;
            case C_ICENT1:
                add_ea_pt(0, p[1], p[0], ptn, ptn, p[0], p[2]);
                add_ea_pt(0, p[0], p[1], ptn, ptn, p[1], p[2]);
                add_ea_pt(0, p[1], p[2], ptn, ptn, p[2], p[0]);
                break;
            case C_CENT:
                break;
            case C_ORTH:
                add_tline(0, ptn, p[0], p[1], p[2]);
                add_tline(0, ptn, p[1], p[0], p[2]);
                add_tline(0, ptn, p[2], p[1], p[0]);
                break;
            case C_PETRIANGLE:
                add_cir2(0, ptn, p[0], p[1]);
                add_cir2(0, p[0], p[1], ptn);
                add_cir2(0, p[1], p[0], ptn);
                add_ea_pt(0, ptn, p[0], p[1], p[0], p[1], ptn);
                add_ea_pt(0, ptn, p[0], p[1], p[1], ptn, p[0]);
                add_ea_pt(0, p[0], p[1], ptn, p[1], ptn, p[0]);
                break;
            case C_NETRIANGLE:
                add_cir2(0, ptn, p[0], p[1]);
                add_cir2(0, p[0], p[1], ptn);
                add_cir2(0, p[1], p[0], ptn);
                add_ea_pt(0, ptn, p[0], p[1], p[0], p[1], ptn);
                add_ea_pt(0, ptn, p[0], p[1], p[1], ptn, p[0]);
                add_ea_pt(0, p[0], p[1], ptn, p[1], ptn, p[0]);
                break;
            case C_I_SS: {
                int pt1 = fd_co(p[0], p[1], p[2]);
                int pt2 = fd_co(p[3], p[4], p[5]);
                add_cir4(0, pt1, ptn, p[0], p[1], p[2]);
                add_cir4(0, pt2, ptn, p[3], p[4], p[5]);
            }
            break;
            default: {
                Cm.print("Error input type: " + mk);
                return false;
            }

        }
        return true;
    }

    /**
     * Adds an equilateral triangle using the specified points.
     *
     * @param ptn the identifier or index used for the triangle
     * @param ps  an array containing the triangle's vertex points
     */
    final public void add_e_triangle(int ptn, int[] ps) {
        int a = ps[0];
        int b = ps[1];
        int c = ps[2];

        add_at0(0, a, b, c, A_60);
        add_at0(0, b, c, a, A_60);
        add_at0(0, c, a, b, A_60);
        add_cong(0, 0, a, b, a, c);
        add_cong(0, 0, a, b, b, c);
        add_cong(0, 0, a, c, b, c);
        for (int i = 1; true; i++) {
            if (i == ptn) break;
            Cons p1 = (Cons) allcns[i];
            if (p1.type != C_EQ_TRI) continue;

            int d = check_tri_dr(ps[0], ps[1], ps[2], p1.ps[0], p1.ps[1], p1.ps[2]);
            int[] pp = new int[3];
            if (d > 0) {
                pp[0] = ps[0];
                pp[1] = ps[1];
                pp[2] = ps[2];
            } else {
                pp[0] = ps[0];
                pp[1] = ps[2];
                pp[2] = ps[1];
            }
            add_ea_pt(0, p1.ps[0], p1.ps[1], p1.ps[2], pp[0], pp[1], pp[2]);
            add_ea_pt(0, p1.ps[0], p1.ps[1], p1.ps[2], pp[2], pp[0], pp[1]);
            add_ea_pt(0, p1.ps[0], p1.ps[1], p1.ps[2], pp[1], pp[2], pp[0]);

            add_ea_pt(0, p1.ps[2], p1.ps[0], p1.ps[1], pp[0], pp[1], pp[2]);
            add_ea_pt(0, p1.ps[2], p1.ps[0], p1.ps[1], pp[2], pp[0], pp[1]);
            add_ea_pt(0, p1.ps[2], p1.ps[0], p1.ps[1], pp[1], pp[2], pp[0]);

            add_ea_pt(0, p1.ps[1], p1.ps[2], p1.ps[0], pp[0], pp[1], pp[2]);
            add_ea_pt(0, p1.ps[1], p1.ps[2], p1.ps[0], pp[2], pp[0], pp[1]);
            add_ea_pt(0, p1.ps[1], p1.ps[2], p1.ps[0], pp[1], pp[2], pp[0]);
        }
    }

    /**
     * Checks whether a triangle with vertices a, b, and c exists in the given STris,
     * considering all permutations of the vertices.
     *
     * @param a  first vertex
     * @param b  second vertex
     * @param c  third vertex
     * @param st the STris instance to search
     * @return the index of the matching triangle if found; otherwise, -1
     */
    final public int on_sts1(int a, int b, int c, STris st) {
        int p, q, r;
        for (int i = 0; i <= st.no; i++) {
            p = st.p1[i];
            q = st.p2[i];
            r = st.p3[i];
            if (a == p && b == q && c == r ||
                    a == p && b == r && c == q ||
                    a == q && b == p && c == r ||
                    a == q && b == r && c == p ||
                    a == r && b == p && c == q ||
                    a == r && b == q && c == p) {
                return i;
            }
        }
        return -1;
    }

    /**
     * Checks for the existence of a triangle with vertices a, b, and c, in the given order, within a STris.
     *
     * @param a  first vertex
     * @param b  second vertex
     * @param c  third vertex
     * @param st the STris instance to search
     * @return the index of the triangle if it exists; otherwise, -1
     */
    final public int on_sts(int a, int b, int c, STris st) {
        int p, q, r;
        for (int i = 0; i <= st.no; i++) {
            p = st.p1[i];
            q = st.p2[i];
            r = st.p3[i];
            if (a == p && b == q && c == r) {
                return i;
            }
        }
        return -1;
    }

    /**
     * Adds a triangle with vertices a, b, and c to the specified STris if it does not already exist.
     *
     * @param d  a multiplier or descriptor value for the triangle
     * @param a  first vertex
     * @param b  second vertex
     * @param c  third vertex
     * @param st the STris instance to which the triangle will be added
     */
    final public void add_to_sts(int d, int a, int b, int c, STris st) {

        if (on_sts1(a, b, c, st) >= 0) return;

        int n = ++st.no;
        st.dr[n] = d;
        st.p1[n] = a;
        st.p2[n] = b;
        st.p3[n] = c;
    }

    /**
     * Merges triangle data from the secondary STris into the primary one, based on a specified index.
     *
     * @param s  the primary STris instance
     * @param s1 the secondary STris instance to merge from
     * @param t  the index within the secondary STris used for merging
     */
    final public void cb_sts(STris s, STris s1, int t) {
        s1.type = 0;

        int o = s.no;
        while (true) {
            if (s.p1[o] == s1.p2[t]) {
                for (int i = 0; i <= s1.no; i++) {
                    int x = s1.p1[i];
                    s1.p1[i] = s1.p2[i];
                    s1.p2[i] = x;
                }
            } else if (s.p1[o] == s1.p3[t]) {
                for (int i = 0; i <= s1.no; i++) {
                    int x = s1.p1[i];
                    s1.p1[i] = s1.p3[i];
                    s1.p3[i] = x;
                }
            } else if (s.p2[o] == s1.p3[t]) {
                for (int i = 0; i <= s1.no; i++) {
                    int x = s1.p2[i];
                    s1.p2[i] = s1.p3[i];
                    s1.p3[i] = x;
                }
            } else
                break;
        }
        for (int i = 0; i <= s1.no; i++) {
            if (i != t && (on_sts1(s1.p1[i], s1.p2[i], s1.p3[i], s) < 0)) {
                s.no++;
                s.p1[s.no] = s1.p1[i];
                s.p2[s.no] = s1.p2[i];
                s.p3[s.no] = s1.p3[i];
                s.dr[s.no] = s1.dr[i];
            }
        }


    }

    /**
     * Attempts to add a SimTri to an existing STris structure.
     *
     * @param t the SimTri object representing the triangle to be added
     * @return true if the list was updated successfully, false otherwise
     */
    final public boolean add_sts1(SimTri t) {
        STris st = null;

        if (tri_type == 0)
            st = all_sts.nx;
        else
            st = all_cts.nx;

        int o1, o2;
        STris st1;
        o1 = o2 = -1;
        st1 = null;

        while (st != null) {
            if (st.type != 0)
                for (int k = 0; k < 3; k++) {
                    for (int m = 0; m < 3; m++) {
                        if (k != m)
                            for (int n = 0; n < 3; n++)
                                if (k != n && m != n) {
                                    int t1 = on_sts(t.p1[k], t.p1[m], t.p1[n], st);
                                    int t2 = on_sts(t.p2[k], t.p2[m], t.p2[n], st);

                                    if (t1 >= 0 && t2 >= 0) {
                                        o1 = t1;
                                        o2 = t2;
                                        st1 = st;
                                        return true;
                                    } else if (t1 >= 0) {
                                        o1 = t1;
                                        add_to_sts(t.dr * st.dr[o1], t.p2[k], t.p2[m], t.p2[n], st);
                                        st1 = st;
                                        break;
                                    } else if (t2 >= 0) {
                                        o2 = t2;
                                        add_to_sts(t.dr * st.dr[o2], t.p1[k], t.p1[m], t.p1[n], st);
                                        st1 = st;
                                        break;
                                    }
                                }
                        if (st == st1) break;
                    }
                    if (st == st1) break;
                }
            if (st == st1) break;
            st = st.nx;
        }

        if (st1 == null) return false;

        int a, b, c;
        a = st1.p1[st1.no];
        b = st1.p2[st1.no];
        c = st1.p3[st1.no];

        int nd = -1;
        while (st != null) {
            if (st.type != 0 && st != st1 && (nd = on_sts1(a, b, c, st)) >= 0) {
                cb_sts(st1, st, nd);
            }
            st = st.nx;
        }

        return true;
    }

    /**
     * Adds a SimTri object to the appropriate STris structure or creates a new STris if necessary.
     *
     * @param st the SimTri object representing the triangle to add
     */
    final public void add_sts(SimTri st) {

        if (add_sts1(st)) return;

        STris sts = new STris();
        sts.st = tri_type;

        if (tri_type == 0) {
            last_sts.nx = sts;
            last_sts = sts;
        } else {
            last_cts.nx = sts;
            last_cts = sts;
        }
        sts.type = 2;
        sts.no = 1;
        sts.dr[0] = 1;
        sts.dr[1] = st.dr;
        sts.p1[0] = st.p1[0];
        sts.p2[0] = st.p1[1];
        sts.p3[0] = st.p1[2];
        sts.p1[1] = st.p2[0];
        sts.p2[1] = st.p2[1];
        sts.p3[1] = st.p2[2];
//        sts.dep[0] = sts.dep[1] = depth;
        return;
    }

    /**
     * Collects and organizes triangles from all SimTri objects into STris structures.
     */
    public void collect_sts() {
        SimTri st = all_st.nx;
        tri_type = 0;
        while (st != null) {
            if (st.type != 0)
                add_sts(st);
            st = st.nx;
        }

        st = all_ct.nx;
        tri_type = 1;
        while (st != null) {
            if (st.type != 0)
                add_sts(st);
            st = st.nx;
        }
    }

    /**
     * Adds a connection between two points into the specified CSegs.
     *
     * @param p1 the first point
     * @param p2 the second point
     * @param cg the CSegs instance where the connection is added
     */
    public void add_to_cg(int p1, int p2, CSegs cg) {
        cg.no++;
        cg.p1[cg.no] = p1;
        cg.p2[cg.no] = p2;
    }

    /**
     * Merges connections from one CSegs instance into another.
     *
     * @param cg  the primary CSegs instance
     * @param cg1 the secondary CSegs instance to merge from
     */
    public void cb_cgs(CSegs cg, CSegs cg1) {
        cg1.type = 0;
        for (int i = 0; i <= cg1.no; i++) {
            if (!on_cgs(cg1.p1[i], cg1.p2[i], cg))
                add_to_cg(cg1.p1[i], cg1.p2[i], cg);
        }
    }

    /**
     * Checks whether a connection between two points exists within the given CSegs.
     *
     * @param p1  the first point
     * @param p2  the second point
     * @param cgs the CSegs instance to check
     * @return true if the connection exists, false otherwise
     */
    public boolean on_cgs(int p1, int p2, CSegs cgs) {
        for (int i = 0; i <= cgs.no; i++)
            if (p1 == cgs.p1[i] & p2 == cgs.p2[i]) return true;
        return false;
    }

    /**
     * Adds a connection between two pairs of points, merging with existing connections if applicable.
     *
     * @param p1 the first point of the first pair
     * @param p2 the second point of the first pair
     * @param p3 the first point of the second pair
     * @param p4 the second point of the second pair
     */
    public void add_cgs(int p1, int p2, int p3, int p4) {
        CSegs cg = all_cgs.nx;
        boolean t1, t2;
        t1 = t2 = false;
        while (cg != null) {
            if (cg.type != 0) {
                for (int i = 0; i <= cg.no; i++) {
                    if (p1 == cg.p1[i] && p2 == cg.p2[i])
                        t1 = true;
                    else if (p3 == cg.p1[i] && p4 == cg.p2[i])
                        t2 = true;
                    if (t1 && t2) return;
                }
                if (t1 && t2) return;
                if (t1) {
                    add_to_cg(p3, p4, cg);
                    break;
                } else if (t2) {
                    add_to_cg(p1, p2, cg);
                    break;
                }
            }
            cg = cg.nx;
        }

        if (t1) {
        } else if (t2) {
            int t = p1;
            p1 = p3;
            p3 = t;
            t = p2;
            p2 = p4;
            p4 = t;
        } else {
            CSegs c = new CSegs();
            c.no = 1;
            c.type = 2;
            c.p1[0] = p1;
            c.p2[0] = p2;
            c.p1[1] = p3;
            c.p2[1] = p4;
            last_cgs.nx = c;
            last_cgs = c;
            return;
        }
        CSegs cg1 = cg.nx;
        while (cg1 != null) {
            if (cg1.type != 0 && on_cgs(p3, p4, cg1))
                cb_cgs(cg, cg1);
            cg1 = cg1.nx;
        }

    }

    /**
     * Finds a PLine object that contains both line segments defined by the given points.
     *
     * @param p the first point of the first line segment
     * @param q the second point of the first line segment
     * @param r the first point of the second line segment
     * @param s the second point of the second line segment
     * @return the matching PLine if found; otherwise, null
     */
    final PLine fo_pn1(int p, int q, int r, int s) {
        PLine pn = all_pn.nx;
        boolean r1, r2;

        while (pn != null) {
            r1 = r2 = false;
            for (int i = 0; i <= pn.no; i++) {
                if (on_ln(p, q, pn.ln[i])) r1 = true;
                if (on_ln(r, s, pn.ln[i])) r2 = true;
            }
            if (r1 && r2) return pn;
            pn = pn.nx;
        }
        return null;

    }

    /**
     * Finds a TLine object that matches the given line segments.
     *
     * If any parameter is zero, this method returns null.
     *
     * @param p1 the first point of the first line segment
     * @param p2 the second point of the first line segment
     * @param p3 the first point of the second line segment
     * @param p4 the second point of the second line segment
     * @return the matching TLine if found; otherwise, null
     */
    final TLine fo_tn1(int p1, int p2, int p3, int p4) {
        if (p1 == 0 || p2 == 0 || p3 == 0 || p4 == 0) return null;

        TLine tn = all_tn.nx;
        while (tn != null) {
            if (on_ln(p1, p2, tn.l1) && on_ln(p3, p4, tn.l2) ||
                    on_ln(p1, p2, tn.l2) && on_ln(p3, p4, tn.l1))
                return (tn);
            tn = tn.nx;
        }
        return tn;
    }

    /**
     * Finds an Angles object matching the given configuration of lines.
     *
     * This method checks various combinations of lines in the Angles object using on_ln.
     * Additional checks are applied when isPFull() returns true.
     *
     * @param a the first point for the first line
     * @param b the second point for the first line
     * @param c the first point for the second line
     * @param d the second point for the second line
     * @param p the first point for the third line
     * @param q the second point for the third line
     * @param r the first point for the fourth line
     * @param s the second point for the fourth line
     * @return the matching Angles object if found; otherwise, null
     */
    final Angles fo_as1(int a, int b, int c, int d, int p, int q, int r, int s) {
        Angles as = all_as.nx;
        while (as != null) {
            if (on_ln(a, b, as.l1) && on_ln(c, d, as.l2) && on_ln(p, q, as.l3) && on_ln(r, s, as.l4)) return as;
            if (on_ln(a, b, as.l2) && on_ln(c, d, as.l1) && on_ln(p, q, as.l4) && on_ln(r, s, as.l3)) return as;
            if (on_ln(a, b, as.l3) && on_ln(c, d, as.l4) && on_ln(p, q, as.l1) && on_ln(r, s, as.l2)) return as;
            if (on_ln(a, b, as.l4) && on_ln(c, d, as.l3) && on_ln(p, q, as.l2) && on_ln(r, s, as.l1)) return as;

            if (isPFull()) {
                if (on_ln(a, b, as.l1) && on_ln(c, d, as.l3) && on_ln(p, q, as.l2) && on_ln(r, s, as.l4)) return as;
                if (on_ln(a, b, as.l3) && on_ln(c, d, as.l1) && on_ln(p, q, as.l4) && on_ln(r, s, as.l2)) return as;
                if (on_ln(a, b, as.l2) && on_ln(c, d, as.l4) && on_ln(p, q, as.l1) && on_ln(r, s, as.l3)) return as;
                if (on_ln(a, b, as.l4) && on_ln(c, d, as.l2) && on_ln(p, q, as.l3) && on_ln(r, s, as.l1)) return as;
            }
            as = as.nx;
        }
        return null;
    }

    /**
     * Determines whether the given LLine contains exactly the two specified points.
     *
     * This method counts the occurrences of the points in the LLine.
     *
     * @param p the first point
     * @param q the second point
     * @param ln the LLine to check
     * @return true if the line contains both points exactly; otherwise, false
     */
    boolean on_ln(int p, int q, LLine ln) {

        int i, n;
        n = 0;
        if (ln == null) return (false);
        for (i = 0; i <= ln.no; i++)
            if (ln.pt[i] == p || ln.pt[i] == q) n++;

        return (n == 2);
    }

    /**
     * Searches for an LLine that is a super-line of the given line and contains the specified additional point.
     *
     * The method expects the found LLine to match the required point count.
     *
     * @param ln the base LLine to be contained
     * @param p the additional point to be present in the super-line
     * @return the matching LLine if found; otherwise, null
     */
    public LLine fd_ln_lp(LLine ln, int p) {  // a line contian ln, and p , no others.
        int n = 0;
        if (on_ln(p, ln))
            n = ln.no;
        else
            n = ln.no + 1;
        LLine l1 = all_ln.nx;
        while (l1 != null) {
            if (sub_ln(ln, l1) && on_ln(p, l1) && l1.no == n)
                return l1;
            l1 = l1.nx;
        }
        return null;
    }

    /**
     * Checks whether a circle defined by the given set of points exists.
     *
     * This method initializes test parameters using the provided points and verifies the existence
     * of a corresponding circle.
     *
     * @param o a parameter for the circle (e.g. origin)
     * @param a the first point
     * @param b the second point
     * @param c the third point
     * @param d the fourth point
     * @param e the fifth point
     * @return true if the circle exists; otherwise, false
     */
    final public boolean xcir_n(int o, int a, int b, int c, int d, int e) {
        test_c.no = -1;
        test_c.o = o;
        add_pt2c(a, test_c);
        add_pt2c(b, test_c);
        add_pt2c(c, test_c);
        add_pt2c(d, test_c);
        add_pt2c(e, test_c);
        ACir cr = xcir(test_c);
        if (cr != null)
            return true;
        return false;
    }

    /**
     * Adjusts the specified ACir object based on sub-circle or overlapping conditions.
     *
     * This method iterates through related circles and updates their types and associated properties
     * when sub-circle relationships are detected.
     *
     * @param cr the ACir object to adjust
     */
    final void adj_cir1(ACir cr) {
        ACir cr1, cr2, cr3;
        Cond co;
        char i;
        cr1 = cr;
        while ((cr2 = fd_cir(cr1)) != null) {
            if (cr2 == cr1) break;

            if (sub_cir(cr2, cr1)) {
                cr2.type = 0;
            } else if (sub_cir(cr1, cr2)) {
                cr1.type = 0;
                cr1 = cr2;
            } else {
                cr3 = cp_cr(cr1);
                cr1.type = 0;
                cr2.type = 0;
                cr3.type = 2;
                if (cr3.o == 0) cr3.o = cr2.o;
                for (i = 0; i <= cr2.no; i++) add_pt2c(cr2.pt[i], cr3);
                if (cr1.co == null && cr2.co == null)
                    cr3.co = null;
                else {
                    co_xy.nx = null;
                    cr3.lemma = 301;
                    co = add_coxy(CO_CYCLIC);
                    co.u.cr = cr1;
                    co = add_coxy(CO_CYCLIC);
                    co.u.cr = cr2;
                    cr3.co = co_xy.nx;
                }
                cr1 = cr3;
                new_pr(CO_CYCLIC);
                last_nd.u.cr = cr1;
            }
        }
    }

    /**
     * Searches for an LLine that exactly contains the two specified points and has exactly one segment.
     *
     * @param p1 the first point
     * @param p2 the second point
     * @return the matching LLine if found; otherwise, null
     */
    public LLine fd_ln1(int p1, int p2) {
        LLine ln = all_ln.nx;
        while (ln != null) {

            if (on_ln(p1, p2, ln) && ln.no == 1)
                return ln;
            ln = ln.nx;
        }
        return ln;
    }

    /**
     * Creates and adds a PLine connecting two line segments defined by the given points.
     *
     * This method validates the parameters, checks for necessary conditions such as non-collinearity,
     * updates or creates required LLine objects, and constructs a new PLine based on the provided points.
     *
     * @param lm the lemma or identifier for the PLine
     * @param p1 the first point of the first segment
     * @param p2 the second point of the first segment
     * @param p3 the first point of the second segment
     * @param p4 the second point of the second segment
     */
    final void add_pline1(int lm, int p1, int p2, int p3, int p4) {
        PLine pn;
        if (!valid(lm)) return;
        if (xpara(p1, p2, p3, p4)) return;
        if (p1 == p2 || p3 == p4) return;

        if (!check_para(p1, p2, p3, p4)) {
            add_checkError();
            return;
        }
        {
            if (p1 == p3 || p1 == p4 || p2 == p3 || p2 == p4) {
                int k = 0;
            }
            if (p1 > p2) {
                int k = p1;
                p1 = p2;
                p2 = k;
            }
            if (p3 > p4) {
                int k = p3;
                p3 = p4;
                p4 = k;
            }
            if (p1 > p3) {
                int k = p1;
                p1 = p3;
                p3 = k;
                k = p2;
                p2 = p4;
                p4 = k;
            }

            LLine ln1 = fd_ln1(p1, p2);
            LLine ln2 = fd_ln1(p3, p4);

            LLine l1 = fd_ln(p1, p2);
            LLine l2 = fd_ln(p3, p4);
            if ((l1 == null || l2 == null) && search_only_exists_ln())
                return;

            pn = new PLine();
            pn.type = 2;
            pn.lemma = lm;
            pn.co = co_db.nx;
            pn.no = 1;

            if (ln1 == null)
                ln1 = add_ln(p1, p2);
            if (ln2 == null)
                ln2 = add_ln(p3, p4);

            if (l1 != null && l1 != ln1)
                ln1.type = 0;
            if (l2 != null && l2 != ln2)
                ln2.type = 0;

            if (ln1.pt[0] < ln2.pt[0] || (ln1.pt[0] == ln2.pt[0] && ln1.pt[1] < ln2.pt[1])) {
                pn.ln[0] = ln1;
                pn.ln[1] = ln2;
            } else {
                pn.ln[0] = ln2;
                pn.ln[1] = ln1;
            }

            pn.nx = null;
            last_pn.nx = pn;
            last_pn = pn;
            new_pr(CO_PARA);
            last_nd.u.pn = pn;
            //adj_pn(pn);
        }

    }

    /**
     * Adjusts the configuration of the specified PLine based on its constituent lines.
     *
     * This method refines the composition of the PLine by evaluating its associated LLine segments,
     * updating their states, and readjusting the PLine properties if necessary.
     *
     * @param pn the PLine to adjust
     */
    final void adj_pn(PLine pn) {
        PLine pn1, pn2, pn3;
        Cond co;
        if (pn.no < 1 || pn.type == 0) return;

        if (pn.no == 1) {
            LLine l1 = pn.ln[0];
            LLine l2 = pn.ln[1];
            pn1 = fd_pnp(pn, l1);
            pn2 = fd_pnp(pn, l2);
            if (l1 == null) {
                // TODO. Handle this.
                System.err.println("l1 is null");
                return;
            }
            if (l2 == null) {
                // TODO. Handle this.
                System.err.println("l2 is null");
                return;
            }

            if (l1.type == 0 || l2.type == 0) {
                LLine ln1 = l1;
                LLine ln2 = l2;
                if (l1.type == 0)
                    ln1 = fd_lnl(ln1);
                if (ln2.type == 0)
                    ln2 = fd_lnl(ln2);
                if (!sub_ln(l1, ln1) || !sub_ln(l2, ln2)) {
                    int k = 0;
                }
                PLine pnn = add_px(188, ln1, ln2);
                if (pnn != null) {
                    Cond c = add_coxy(CO_PARA);
                    c.u.pn = new PLine(l1, l2);

                    pnn.co = c;
                }
                pn.type = 0;
            }

            if (pn1 == null && pn2 == null)
                return;

            if (pn1 == null) {
                pn3 = pn1;
                pn1 = pn2;
                pn2 = pn3;
            }

            co_xy.nx = null;
            pn3 = cp_pn(pn1);
            pn1.type = 0;
            pn_un(pn3, pn);
            pn.type = 0;
            pn3.type = 2;
            pn3.lemma = 2;
            co = add_coxy(CO_PARA);
            co.u.pn = pn1;
            co = add_coxy(CO_PARA);
            co.u.pn = pn;
            pn3.co = co_xy.nx;//(pn.co == null) ? co_xy.nx : pn.co;
            if (pn2 != null) {
                co_xy.nx = null;
                pn1 = cp_pn(pn3);
                pn3.type = 0;
                pn_un(pn1, pn2);
                pn2.type = 0;
                pn1.lemma = 2;
                co = add_coxy(CO_PARA);
                co.u.pn = pn3;
                co = add_coxy(CO_PARA);
                co.u.pn = pn2;
                pn1.co = co_xy.nx;//(pn.co == null) ? null : co_xy.nx;
                pn3 = pn1;
            }
            new_para(pn3);
        }
    }

    /**
     * Creates an Angles object using the four provided lines.
     *
     * @param lm the lemma identifier
     * @param l1 the first line
     * @param l2 the second line
     * @param l3 the third line
     * @param l4 the fourth line
     * @return the constructed Angles object, or null if the input is invalid
     */
    Angles add_as_t(int lm, LLine l1, LLine l2, LLine l3, LLine l4) {
        if (l1 == l2 && l3 == l4) return null;
        if (!valid(lm)) return null;

        LLine n1;
        if (line_compare(l1, l2) > 0) {
            n1 = l1;
            l1 = l2;
            l2 = n1;
            n1 = l3;
            l3 = l4;
            l4 = n1;
        }

        if (line_compare(l1, l3) > 0) {
            n1 = l1;
            l1 = l3;
            l3 = n1;
            n1 = l2;
            l2 = l4;
            l4 = n1;
        }

        if (l1 == l2 && l3 == l4 || l1 == l3 && l2 == l4) return null;

        if (!search_only_exists_ln(l1, l2, l3, l4)) return null;

        if (!check_eqangle(l1.pt[0], l1.pt[1], l2.pt[0], l2.pt[1], l3.pt[0], l3.pt[1], l4.pt[0], l4.pt[1])) {
            int k = 0;
        }
        if (check_perp(l1.pt[0], l1.pt[1], l2.pt[0], l2.pt[1])) {
            int k = 0;
        }

        if (inter_lls(l1, l2) == 0 || inter_lls(l3, l4) == 0) {
            int k = 0;
        }

        Angles as = new Angles();
        as.type = 2;
        as.lemma = lm;
        as.co = co_db.nx;
        as.sa = 0;
        as.nx = null;
        as.l1 = l1;
        as.l2 = l2;
        as.l3 = l3;
        as.l4 = l4;
        as.atp = 0;
        last_as.nx = as;
        last_as = as;

        new_pr(CO_ACONG);
        last_nd.u.as = as;
        if (l1 == l2 || l3 == l4 || l1 == l3 && l2 == l4 || l1 == l4 && l2 == l3) {
            int k = 0;
        }
        if (l1.type == 0 || l2.type == 0 || l3.type == 0 || l4.type == 0)
            as.type = 0;
        return (as);
    }

    /**
     * Checks whether the given sets of three points satisfy the congruence conditions.
     *
     * @param a first point of the first segment
     * @param b second point of the first segment
     * @param c third point of the first segment
     * @param p first point of the second segment
     * @param q second point of the second segment
     * @param r third point of the second segment
     * @return true if the conditions for congruence hold; false otherwise
     */
    final boolean xacongt(int a, int b, int c, int p, int q, int r) {
        LLine l1, l2, l3, l4;
        l1 = fd_ln(a, b);
        l2 = fd_ln(b, c);
        l3 = fd_ln(p, q);
        l4 = fd_ln(q, r);

        if (l1 == null || l2 == null || l3 == null || l4 == null) return (false);
        if (xcoll4(a, b, p, q) && xcoll4(b, c, q, r)) return true;
        if (xpara(a, b, b, c) && xpara(p, q, q, r)) return (true);
        if (xperp(a, b, b, c) && xperp(p, q, q, r)) return (true);
        return (this.fo_as1_t(a, b, b, c, p, q, q, r) != null);
    }

    /**
     * Finds and returns an Angles object that matches the specified endpoints from four lines.
     *
     * @param a the first endpoint of the first line
     * @param b the second endpoint of the first line
     * @param c the first endpoint of the second line
     * @param d the second endpoint of the second line
     * @param p the first endpoint of the third line
     * @param q the second endpoint of the third line
     * @param r the first endpoint of the fourth line
     * @param s the second endpoint of the fourth line
     * @return the matching Angles object, or null if none is found
     */
    final Angles fo_as1_t(int a, int b, int c, int d, int p, int q, int r, int s) {

        Angles as = all_as.nx;
        while (as != null) {
            if (on_ln(a, b, as.l1) && on_ln(c, d, as.l2) && on_ln(p, q, as.l3) && on_ln(r, s, as.l4)) return as;
            if (on_ln(a, b, as.l2) && on_ln(c, d, as.l1) && on_ln(p, q, as.l4) && on_ln(r, s, as.l3)) return as;
            if (on_ln(a, b, as.l3) && on_ln(c, d, as.l4) && on_ln(p, q, as.l1) && on_ln(r, s, as.l2)) return as;
            if (on_ln(a, b, as.l4) && on_ln(c, d, as.l3) && on_ln(p, q, as.l2) && on_ln(r, s, as.l1)) return as;
            as = as.nx;
        }
        return null;
    }

    /**
     * Retrieves an Angles object using four specified lines.
     *
     * @param l1 the first line
     * @param l2 the second line
     * @param l3 the third line
     * @param l4 the fourth line
     * @return the matching Angles object, or null if no match is found
     */
    final Angles fo_las_t(LLine l1, LLine l2, LLine l3, LLine l4) {
        Angles as = all_as.nx;
        while (as != null) {
            if (l1 == as.l1 && l2 == as.l2 && l3 == as.l3 && l4 == as.l4) return as;
            if (l1 == as.l2 && l2 == as.l1 && l3 == as.l4 && l4 == as.l3) return as;
            if (l1 == as.l3 && l2 == as.l4 && l3 == as.l1 && l4 == as.l2) return as;
            if (l1 == as.l4 && l2 == as.l3 && l3 == as.l2 && l4 == as.l1) return as;
            as = as.nx;
        }
        return (null);
    }

    /**
     * Adds an Angles object from the provided lines based on the project state.
     *
     * @param lm the lemma identifier
     * @param l1 the first line
     * @param l2 the second line
     * @param l3 the third line
     * @param l4 the fourth line
     * @return the added Angles object
     */
    public Angles add_as(int lm, LLine l1, LLine l2, LLine l3, LLine l4) {
        if (isPFull())
            return add_as0(lm, l1, l2, l3, l4);
        else
            return add_as_t(lm, l1, l2, l3, l4);
    }

    /**
     * Adds an inscribed angle point using the provided parameters.
     *
     * @param lm the lemma identifier
     * @param a the first point on the first line
     * @param b the second point on the first line
     * @param c the third point on the first line
     * @param p the first point on the second line
     * @param q the second point on the second line
     * @param r the third point on the second line
     */
    final void add_ea_pt_t(int lm, int a, int b, int c, int p, int q, int r) {        // for inscrible angle.

        if (d_base == 1) return;
        if (!valid(lm)) return;
        if (this.isPFull()) {
            this.add_ea_pt(lm, a, b, c, p, q, r);
            return;
        }
        if (xacongt(a, b, c, p, q, r)) return;
        LLine l1 = fadd_ln_t(a, b);
        LLine l2 = fadd_ln_t(b, c);
        LLine l3 = fadd_ln_t(p, q);
        LLine l4 = fadd_ln_t(q, r);
        if (l1 == null || l2 == null || l3 == null || l4 == null) return;
        if (ln_acong(l1, l2, l3, l4)) return;
        Angles as = add_as(lm, l1, l2, l3, l4);
    }

    /**
     * Adds a new line segment with endpoints a and b.
     *
     * @param a the first endpoint
     * @param b the second endpoint
     * @return the new or existing LLine instance, or null if the input is invalid
     */
    final LLine add_ln_t0(int a, int b) {
        if (a == 0 || b == 0) return null;

        LLine ln = fd_ln1(a, b);
        if (ln != null) return ln;

        LLine ln1 = fd_ln(a, b);

        if (ln1 == null && search_only_exists_ln()) return null;

        ln = add_ln(a, b);

        if (ln1 != null)
            ln.type = 0;
        return ln;
    }

    /**
     * Fast-adds a line segment using the given endpoints.
     *
     * @param p1 the first endpoint
     * @param p2 the second endpoint
     * @return the added LLine object
     */
    LLine fadd_ln_t(int p1, int p2) {
        return add_ln_t0(p1, p2);
    }

    /**
     * Returns the anti midpoint from a line, excluding the specified endpoints.
     *
     * @param ln the line to search
     * @param p one of the endpoints
     * @param p1 the other endpoint
     * @return the internal point on the line between p and p1, or 0 if not found
     */
    public int get_anti_pt(LLine ln, int p, int p1) { // p is imdpoint of x and p1
        for (int i = 0; i <= ln.no; i++) {
            if (ln.pt[i] != p && ln.pt[i] != p1 && (x_inside(p, ln.pt[i], p1)))
                return ln.pt[i];
        }
        return 0;
    }

    /**
     * Determines whether two lines are collinear.
     *
     * @param l1 the first line
     * @param l2 the second line
     * @return true if l1 and l2 are collinear; false otherwise
     */
    boolean xcoll_ln(LLine l1, LLine l2) {
        if (l1.type == 0 && l2.type == 0) {
            int r = 0;
        }
        if (l1 == l2) return true;

        if (l1.type != 0 && l2.type != 0)
            return false;

        if (l1.type == 0)
            l1 = fd_lnl(l1);
        if (l2.type == 0)
            l2 = fd_lnl(l2);
        return l1 == l2;
    }

    /**
     * Adds an angle (AngleT) by creating it from line segments corresponding to points a, b, and c.
     *
     * @param lemma the lemma identifier
     * @param a the first point of the angle
     * @param b the vertex point of the angle
     * @param c the third point of the angle
     * @param v the angle value
     * @return the created AngleT object
     */
    public AngleT add_at(int lemma, int a, int b, int c, int v) {

        return add_at(lemma, fadd_ln_t(a, b), fadd_ln_t(b, c), v);
    }

    /**
     * Adds an angle (AngleT) by creating it using line segments looked up directly.
     *
     * @param lemma the lemma identifier
     * @param a the first point of the angle
     * @param b the vertex point of the angle
     * @param c the third point of the angle
     * @param v the angle value
     * @return the created AngleT object
     */
    public AngleT add_at0(int lemma, int a, int b, int c, int v) {

        return add_at(lemma, fd_ln(a, b), fd_ln(b, c), v);
    }

    /**
     * Creates and adds an AngleT instance with the given lemma, lines, and angle value.
     * Returns the created AngleT if successful, or null otherwise.
     *
     * @param lemma the lemma identifier
     * @param l1 the first line
     * @param l2 the second line
     * @param v the angle value
     * @return the created AngleT instance or null
     */
    public AngleT add_at(int lemma, LLine l1, LLine l2, int v) {
        if (isPFull()) return null;
        if (!valid(R_AG_SPECIAL)) return null;
        if (!valid(lemma)) return null;


        if (l1 == null || l2 == null) return null;

        if (xatcong(l1, l2)) return null;

        int p = inter_lls(l1, l2);
        if (p == 0) return null;

        if (line_compare(l1, l2) > 0) {
            LLine t = l1;
            l1 = l2;
            l2 = t;
            v = -v;
        }
        if (v > A_180)
            v = v - A_360;
        else if (v <= -A_180)
            v = v + A_360;

        if (!check_ll_dr(p, l1, l2, v))
            v = -v;


        if (!check_ateq(get_lpt1(l1, p), p, get_lpt1(l2, p), v)) {
            int k = 0;
        }

        AngleT at = new AngleT();
        at.type = 1;
        at.l1 = l1;
        at.l2 = l2;
        at.p = p;
        at.v = v;
        at.lemma = lemma;
        at.co = co_db.nx;
        last_at.nx = at;
        last_at = at;
        new_pr(CO_TANG);
        last_nd.u.at = at;
        return at;
    }

    /**
     * Checks whether the angle formed by points a, b, and c is congruent to the specified angle value.
     *
     * @param a the first point
     * @param b the second point
     * @param c the third point
     * @param v the angle value to compare
     * @return true if the angle is congruent; false otherwise
     */
    public boolean xatcong(int a, int b, int c, int v) {
        AngleT at = fo_at(a, b, c);
        return check_ll_dr(at.p, at.l1, at.l2, v);
    }

    /**
     * Determines if the angle formed by the two lines is congruent.
     *
     * @param l1 the first line
     * @param l2 the second line
     * @return true if the lines form a congruent angle or are collinear; false otherwise
     */
    public boolean xatcong(LLine l1, LLine l2) {
        if (l1 == l2 || xcoll_ln(l1, l2)) return true;

        return fd_at(l1, l2) != null;
    }

    /**
     * Finds an existing AngleT instance based on three points, mapping these points to corresponding lines.
     *
     * @param p1 the first point
     * @param p2 the second point
     * @param p3 the third point
     * @return the found AngleT instance, or null if none exists
     */
    public AngleT fd_at(int p1, int p2, int p3) {
        return fd_at(fd_ln(p1, p2), fd_ln(p2, p3));
    }

    /**
     * Searches for the first AngleT instance that matches the line segments determined by the given points.
     *
     * @param p1 the first point
     * @param p2 the second point
     * @param p3 the third point
     * @return the found AngleT instance, or null if not found
     */
    public AngleT fo_at(int p1, int p2, int p3) {
        AngleT at = all_at.nx;
        while (at != null) {
            if (on_ln(p1, p2, at.l1) && on_ln(p2, p3, at.l2) || on_ln(p1, p2, at.l2) && on_ln(p2, p3, at.l1)) return at;
            at = at.nx;
        }
        return null;
    }

    /**
     * Finds an AngleT instance defined by the two specified lines.
     *
     * @param l1 the first line
     * @param l2 the second line
     * @return the found AngleT instance, or null if it does not exist
     */
    public AngleT fd_at(LLine l1, LLine l2) {
        if (l1 == null || l2 == null) return null;

        if (l1 == l2 || xcoll_ln(l1, l2)) return null;

        AngleT at = all_at.nx;
        while (at != null) {
            if (at.l1 == l1 && at.l2 == l2 || at.l2 == l1 && at.l1 == l2) return at;
            at = at.nx;
        }
        return null;
    }

    /**
     * Checks if the angle between the two lines at a specific point approximates the given angle measure.
     *
     * @param p the vertex point
     * @param l1 the first line
     * @param l2 the second line
     * @param d the expected angle in degrees
     * @return true if the measured angle is approximately equal to d degrees; false otherwise
     */
    public boolean check_ll_dr(int p, LLine l1, LLine l2, int d) {
        double r = getAngleValue(p, l1, l2);
        double x = Math.abs(r * A_180 / Math.PI - d);
        return x < ZERO || Math.abs(x - A_180) < ZERO || Math.abs(x + A_180) < ZERO;
    }

    /**
     * Checks if two angle measures are equal or supplementary.
     *
     * @param t1 the first angle measure
     * @param t2 the second angle measure
     * @return true if the angles are equal or differ by 180 degrees; false otherwise
     */
    public boolean check_at_eq(int t1, int t2) {
        return t1 == t2 || Math.abs(Math.abs(t1 - t2) - A_180) < ZERO;
    }

    /**
     * Searches for an AngTn instance with the specified set of four lines.
     * The search considers both direct and reverse orders.
     *
     * @param l1 the first line
     * @param l2 the second line
     * @param l3 the third line
     * @param l4 the fourth line
     * @return the found AngTn instance, or null if not found
     */
    public AngTn fo_atn(LLine l1, LLine l2, LLine l3, LLine l4) {
        AngTn atn = all_atn.nx;
        while (atn != null) {

            if (atn.ln1 == l1 && atn.ln2 == l2 && atn.ln3 == l3 && atn.ln4 == l4) return atn;
            if (atn.ln1 == l3 && atn.ln2 == l4 && atn.ln3 == l1 && atn.ln4 == l2) return atn;
            if (atn.ln1 == l2 && atn.ln2 == l1 && atn.ln3 == l4 && atn.ln4 == l3) return atn;
            if (atn.ln1 == l4 && atn.ln2 == l3 && atn.ln3 == l2 && atn.ln4 == l1) return atn;

            atn = atn.nx;
        }
        return null;
    }

    /**
     * Searches for an AngTn instance matching the provided point-derived line parameters.
     * Various order combinations are evaluated.
     *
     * @param a the first point of the first line
     * @param b the second point of the first line
     * @param c the first point of the second line
     * @param d the second point of the second line
     * @param a1 the first point of the third line
     * @param b1 the second point of the third line
     * @param c1 the first point of the fourth line
     * @param d1 the second point of the fourth line
     * @return the matching AngTn instance, or null if not found
     */
    public AngTn fo_atn(int a, int b, int c, int d, int a1, int b1, int c1, int d1) {
        AngTn atn = all_atn.nx;
        while (atn != null) {
            if ((on_ln(a, b, atn.ln1) && on_ln(c, d, atn.ln2) && on_ln(a1, b1, atn.ln3) && on_ln(c1, d1, atn.ln4)))
                return atn;
            if (on_ln(a, b, atn.ln2) && on_ln(c, d, atn.ln1) && on_ln(a1, b1, atn.ln4) && on_ln(c1, d1, atn.ln3))
                return atn;
            if ((on_ln(a, b, atn.ln3) && on_ln(c, d, atn.ln4) && on_ln(a1, b1, atn.ln1) && on_ln(c1, d1, atn.ln2)))
                return atn;
            if ((on_ln(a, b, atn.ln4) && on_ln(c, d, atn.ln3) && on_ln(a1, b1, atn.ln2) && on_ln(c1, d1, atn.ln1)))
                return atn;
            atn = atn.nx;
        }
        return null;
    }

    /**
     * Adjusts the given angle tangent node if necessary.
     *
     * @param atn the AngTn node to adjust
     */
    public void adj_atn(AngTn atn) {
        if (atn.type == 0) return;
        LLine l1 = atn.ln1;
        LLine l2 = atn.ln2;
        LLine l3 = atn.ln3;
        LLine l4 = atn.ln4;

        if (l1.type != 0 && l2.type != 0 && l3.type != 0 && l4.type != 0)
            return;


        int p1 = inter_lls(l1, l2);
        int p2 = inter_lls(l3, l4);

        if (atn.ln1.type == 0)
            l1 = fd_ln_rl(l1, p1);
        else
            l1 = atn.ln1;
        if (atn.ln2.type == 0)
            l2 = fd_ln_rl(l2, p1);
        else
            l2 = atn.ln2;
        if (atn.ln3.type == 0)
            l3 = fd_ln_rl(l3, p2);
        else
            l3 = atn.ln3;
        if (atn.ln4.type == 0)
            l4 = fd_ln_rl(l4, p2);
        else
            l4 = atn.ln4;

        if (l1 == atn.ln1 && l2 == atn.ln2 && l3 == atn.ln3 && l4 == atn.ln4)
            return;
        AngTn atn1 = add_atn(188, l1, l2, l3, l4);
        if (atn1 != null) {
            co_xy.nx = null;
            Cond c = add_coxy(CO_ATNG);
            c.u.atn = atn;
            atn1.co = c;
        }
        atn.type = 0;
    }

    /**
     * Adjusts a specific line segment within the angle tangent node based on a change.
     *
     * @param ln1 the modified line
     * @param ln2 an associated line for adjustment
     * @param atn the AngTn node to update
     */
    final void ch_ln_atn(LLine ln1, LLine ln2, AngTn atn) {
        if (atn.type == 0) return;
        LLine l1 = atn.ln1;
        LLine l2 = atn.ln2;
        LLine l3 = atn.ln3;
        LLine l4 = atn.ln4;

        int p1 = atn.t1;//inter_lls(l1, l2);
        int p2 = atn.t2;//inter_lls(l3, l4);

        if (l1 == ln1)
            l1 = fd_ln_rl(l1, p1);
        else
            l1 = atn.ln1;
        if (l2 == ln1)
            l2 = fd_ln_rl(l2, p1);
        else
            l2 = atn.ln2;
        if (l3 == ln1)
            l3 = fd_ln_rl(l3, p2);
        else
            l3 = atn.ln3;
        if (l4 == ln1)
            l4 = fd_ln_rl(l4, p2);
        else
            l4 = atn.ln4;

        if (l1 == atn.ln1 && l2 == atn.ln2 && l3 == atn.ln3 && l4 == atn.ln4)
            return;
        AngTn atn1 = add_atn(188, l1, l2, l3, l4);
        if (atn1 != null) {
            co_xy.nx = null;
            Cond c = add_coxy(CO_ATNG);
            c.u.atn = atn;
            atn1.co = c;
        }
        atn.type = 0;
    }

    /**
     * Adds a new angle tangent node using four line objects.
     *
     * @param lemma the lemma identifier
     * @param l1 the first line in the angle configuration
     * @param l2 the second line in the angle configuration
     * @param l3 the third line in the angle configuration
     * @param l4 the fourth line in the angle configuration
     * @return the new AngTn object, or null if creation fails
     */
    public AngTn add_atn(int lemma, LLine l1, LLine l2, LLine l3, LLine l4) {
        if (isPFull()) return null;
        if (!valid(R_AG_ATN)) return null;
        if (!valid(lemma)) return null;

        if (l1 == null || l2 == null || l3 == null || l4 == null) return null;
        if (xcoll_ln(l1, l2) || xcoll_ln(l3, l4)) return null;
        if (fo_atn(l1, l2, l3, l4) != null) return null;

        int p1 = inter_lls(l1, l2);
        int p2 = inter_lls(l3, l4);
        if (p1 == 0 || p2 == 0) return null;
        int dr = check_atn_dr(get_lpt1(l1, p1), p1, get_lpt1(l2, p1), get_lpt1(l3, p2), p2, get_lpt1(l4, p2));
        switch (dr) {
            case-1:
                return null;
            case 0:
                break;
            case 1: {
                LLine t = l1;
                l1 = l2;
                l2 = t;
                t = l3;
                l3 = l4;
                l4 = t;
            }
            break;
            case 2: {
                LLine t = l3;
                l3 = l4;
                l4 = t;
            }
            break;
            case 3: {
                LLine t = l1;
                l1 = l2;
                l2 = t;
            }
            break;
        }
        if (l1 == l3 && l2 == l4 || l1 == l2 || l3 == l4) return null;
        if (!check_atn(get_lpt1(l1, p1), p1, get_lpt1(l2, p1), get_lpt1(l3, p2), p2, get_lpt1(l4, p2))) {
            int l = 0;
        }

        if (fo_atn(l1, l2, l3, l4) != null) return null;

//        if (true)
//          return null;
        AngTn atn = new AngTn();
        atn.lemma = lemma;
        atn.ln1 = l1;
        atn.ln2 = l2;
        atn.ln3 = l3;
        atn.ln4 = l4;
        atn.t1 = inter_lls(l1, l2);
        atn.t2 = inter_lls(l3, l4);
        atn.type = 1;
        atn.co = co_db.nx;
        last_atn.nx = atn;
        last_atn = atn;
        new_pr(CO_ATNG);
        last_nd.u.atn = atn;
        return atn;
    }

    /**
     * Checks the directional relationship between two angles defined by three points each.
     *
     * @param a the first point of the first angle
     * @param b the vertex point of the first angle
     * @param c the second point of the first angle
     * @param a1 the first point of the second angle
     * @param b1 the vertex point of the second angle
     * @param c1 the second point of the second angle
     * @return 0, 1, 2, or 3 for specific directional configurations, or -1 if false
     */
    public int check_atn_dr(int a, int b, int c, int a1, int b1, int c1) {
        double t1 = getAngleValue(a, b, c);
        double t2 = getAngleValue(a1, b1, c1);
        double t = t1 + t2;
        if (Math.abs(t - Math.PI / 2) < ZERO) return 0; //positive
        if (Math.abs(t + Math.PI / 2) < ZERO) return 1; //negtive
        if (Math.abs(t1 - t2 - Math.PI / 2) < ZERO) return 2; //negtive
        if (Math.abs(t1 - t2 + Math.PI / 2) < ZERO) return 3; //negtive
        return -1; // false
    }

    /**
     * Retrieves the adjusted angle value based on an AngleT object.
     *
     * @param a the first point for angle calculation
     * @param b the vertex point for angle calculation
     * @param c the second point for angle calculation
     * @param at the AngleT object containing the original angle value
     * @return the adjusted angle value
     */
    public int getAtv(int a, int b, int c, AngleT at) {
        int v = at.get_val(a, c);
        return getAtv(a, b, c, v);
    }

    /**
     * Computes the adjusted angle value using a given angle value.
     *
     * @param a the first point for angle calculation
     * @param b the vertex point for angle calculation
     * @param c the second point for angle calculation
     * @param v the initial angle value
     * @return the adjusted angle value, or 0 if no adjustment is needed
     */
    public int getAtv(int a, int b, int c, int v) {
        double r = getAngleValue(a, b, c) * A_180 / Math.PI;
        if (v > A_180)
            v -= A_360;
        if (v <= -A_180)
            v += A_360;
        double t = r - v;
        if (Math.abs(t) < ZERO) return v;
        if (Math.abs(t - A_180) < ZERO) return A_180 + v;
        if (Math.abs(t + A_180) < ZERO) return -A_180 + v;
        return 0;
    }

    /**
     * Determines if only existing lines should be searched.
     *
     * @return true if only existing lines are considered, false otherwise
     */
    public boolean search_only_exists_ln() {
        return (!R_SEARCH_ALL_LN);// && depth != 0);
    }

    /**
     * Checks whether lines defined by two pairs of points exist.
     *
     * @param a the first point of the first line
     * @param b the second point of the first line
     * @param c the first point of the second line
     * @param d the second point of the second line
     * @return true if at least one corresponding line does not exist, false otherwise
     */
    public boolean search_only_exists_ln(int a, int b, int c, int d) {
        return (!R_SEARCH_ALL_LN) && (fd_ln(a, b) == null || fd_ln(c, d) == null);
    }

    /**
     * Checks whether lines defined by three pairs of points exist.
     *
     * @param a the first point of the first line
     * @param b the second point of the first line
     * @param c the first point of the second line
     * @param d the second point of the second line
     * @param e the first point of the third line
     * @param f the second point of the third line
     * @return true if at least one corresponding line does not exist, false otherwise
     */
    public boolean search_only_exists_ln(int a, int b, int c, int d, int e, int f) {
        return (!R_SEARCH_ALL_LN) && (fd_ln(a, b) == null || fd_ln(c, d) == null || fd_ln(e, f) == null);
    }

    /**
     * Checks if the set of four lines satisfies the search criteria based on intersections.
     *
     * @param l1 the first line
     * @param l2 the second line
     * @param l3 the third line
     * @param l4 the fourth line
     * @return true if the lines meet the search criteria, false otherwise
     */
    public boolean search_only_exists_ln(LLine l1, LLine l2, LLine l3, LLine l4) {
        if (!R_AG_ALL && (inter_lls(l1, l2) == 0 || inter_lls(l3, l4) == 0))
            return false;
        return true;
    }

    /**
     * Processes an array of integers to add nodes for a conclusion.
     * Each group of three integers represents properties of one node.
     * If an extra integer exists after grouping, it is processed as a separate node.
     *
     * @param p the array of integers representing node properties (zero-terminated)
     */
    public void add_nodes(int[] p) { // for conclusion;

        LList d = new LList();
        Mnde m = null;
        int n = 0;
        while (n < p.length && p[n] != 0)
            n++;
        int num = n / 3;
        for (int i = 0; i < num; i++) {
            Mnde m1 = new Mnde();
            int a = p[i * 3];
            int b = p[3 * i + 1];
            int c = p[3 * i + 2];
            if (a != 0 && b != 0 && c != 0) {
                m1.tr = fadd_tr(b, a, c, fadd_ln(a, b), fadd_ln(b, c));
                m1.t = 1;
                m1.type = LList.ANGLE;
                d.add_md(m1);
            }
        }
        if (n == num * 3 + 1) {
            m = new Mnde();
            m.type = LList.ANGLE;
            m.tr = null;
            m.t = p[n - 1];
            d.add_mf(m);
        }
        d.type = LList.ANGLE;
        last_ns.nx = d;
        last_ns = d;
    }

    /**
     * Creates and returns a copy of the given node list.
     *
     * @param ns the original node list to copy
     * @return a new node list copied from the provided list
     */
    public LList cp_nodes(LList ns) {
        LList d = new LList();
        d.cp(ns);
        return d;
    }

    /**
     * Adds the specified node list to the global node list if it is not already present.
     *
     * @param d the node list to add
     */
    public void add_nodes(LList d) {
        if (d == null) return;
        if (x_list(d)) return;

        last_ns.nx = d;
        last_ns = d;
    }

    /**
     * Checks whether an equivalent node list already exists in the collection.
     *
     * @param ls the node list to check
     * @return true if an equivalent list exists; false otherwise
     */
    public boolean x_list(LList ls) {
        if (ls.nd == 0 && ls.nf == 0) return false;

        LList ls1 = all_ns.nx;
        while (ls1 != null) {
            if (eq_list(ls1, ls)) return true;
            ls1 = ls1.nx;
        }
        return false;
    }

    /**
     * Compares two node lists for equality based on their node and factor arrays.
     *
     * @param ls the first node list
     * @param ls1 the second node list
     * @return true if both lists are equal; false otherwise
     */
    public boolean eq_list(LList ls, LList ls1) {
        if (ls.nd != ls1.nd) return false;
        for (int i = 0; i < ls.nd; i++) {
            if (!eq_mnde(ls.md[i], ls1.md[i])) return false;
        }

        if (ls.nf != ls1.nf) return false;

        for (int i = 0; i < ls.nf; i++) {
            if (!eq_mnde(ls.mf[i], ls1.mf[i])) return false;
        }
        return true;
    }

    /**
     * Determines if two Mnde objects are equivalent.
     * Equivalence is based on the type value and, if present, the associated transformation.
     *
     * @param m1 the first Mnde object
     * @param m2 the second Mnde object
     * @return true if both Mnde objects are considered equal; false otherwise
     */
    public boolean eq_mnde(Mnde m1, Mnde m2) {
        if (m1.t != m2.t) return false;
        if (m1.tr == null && m2.tr == null) return true;
        if (m1.tr == null && m2.tr != null || m1.tr != null && m2.tr == null) return false;

        if (m1.tr.l1 == m2.tr.l1 && m1.tr.l2 == m2.tr.l2) return true;
        return false;
    }

    /**
     * Retrieves an existing AngTr object matching the specified parameters,
     * or creates a new one if none exists.
     *
     * @param v the angle value
     * @param t1 the first transformation parameter
     * @param t2 the second transformation parameter
     * @param l1 the first line
     * @param l2 the second line
     * @return an existing or newly created AngTr object based on the parameters
     */
    public AngTr fadd_tr(int v, int t1, int t2, LLine l1, LLine l2) {
        AngTr tr = fd_tr(v, l1, l2);
        if (tr != null) return tr;

        AngTr t = add_tr(v, l1, l2);
        t.t1 = t1;
        t.t2 = t2;
        return t;
    }

    /**
     * Creates and returns a new AngTr object with the specified angle value and lines.
     *
     * @param v the angle value
     * @param l1 the first line
     * @param l2 the second line
     * @return a new AngTr object initialized with the provided parameters
     */
    public AngTr add_tr(int v, LLine l1, LLine l2) {
        AngTr tr = new AngTr();
        tr.v = v;
        tr.l1 = l1;
        tr.l2 = l2;
        last_tr.nx = tr;
        last_tr = tr;
        return tr;
    }

    /**
     * Searches for an existing AngTr object that matches the specified angle value and lines.
     *
     * @param v the angle value
     * @param l1 the first line
     * @param l2 the second line
     * @return the matching AngTr object if found; otherwise, null
     */
    public AngTr fd_tr(int v, LLine l1, LLine l2) {
        AngTr tr = all_tr.nx;
        while (tr != null) {
            if (tr.l1 == l1 && tr.l2 == l2) return tr;
            tr = tr.nx;
        }
        return null;
    }

}