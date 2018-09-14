/*
    Chaining strategies
    see http://www.sudokuwiki.org/Strategy_Families for details

    It is a complicated but universal framework to discribe solver

    The code could be implemented as following:
        relation between two events is represented using a matrix
*/

#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"

// initiate map and events
void rels_init_mapevns(rels_t *rels, lattice *lats) {
    int n=0;
    for(int i=0; i<81; i++) {
        if(lat_isset(lats+i)) continue;
        for(int d=1; d<=9; d++) {
            if(!lat_iscand(lats+i, d)) continue;
            // printf("event %i: lat (%i, %i), num %i\n", n, i/9+1, i%9+1, d);
            rels_set_event_at(rels, n++, i, d);
        }
    }
    // return n;
}

// update relations using a sub-matrix
void update_rels_sub(rels_t *rels, submat *sub, fun_id f) {
    for(int b=0; b<9; b++) {
        if(sub_isset(sub+b)) continue;

        cand_t *nums=sub[b].nums;
        for(int d=0; d<9; d++) {
            if(cand_isset(nums+d)) continue;

            int reli=REL_WEAK;
            if(cand_num(nums+d)==2) reli|=REL_STRONG;

            barr_t bins=cand_bins(nums+d);
            int i0, i1=cand_1stcand(nums+d),
                n0, n1;
            // printf("di1: %i %i\n", d, i1);
            // printf("i1: %i\n", i1);
            for(; i1<8; i1++) {
                if(!(bins&(1<<i1))) continue;
                n1=rels_map_latnum(*rels, f(b, i1), d+1);
                for(i0=i1+1; i0<9; i0++) {
                    if(!(bins&(1<<i0))) continue;
                    n0=rels_map_latnum(*rels, f(b, i0), d+1);
                    // printf("update %i %i\n", n1, n0);

                    rels_add_rel(rels, n0, n1, reli);
                    // printf("rel: %i %i %i\n", n0, n1, reli);
                }
            }
        }
    }
}


// functions to update scans: --: weak relation; ==: strong relation
// figure out all ai--a==b--bi/ai==a--b==bi given middle relation a==/--b
//    which is equivalent to ai--bi/ai==bi
int rels_update_midr(rels_t *rels, int a, int b, int rmark0, int *buff) {
    // if found chance to reduce candidates, result is also put in buff
    // printf("update middle remark: %i\n", rmark0);
    // printf("       middle ab: %i %i\n", a, b);
    int ne=rels->ne;
    // printf("ne: %i\n", ne);

    int rmark=(~rmark0)&REL_MARK;

    int *buffa=buff,
        *buffb=buff+ne,
        na=0, nb=0;
    // find two sides relation
    for(int i=0; i<ne; i++) {
        if(rels_isrel_between(*rels, i, a, rmark)) buffa[na++]=i;
    }
    for(int i=0; i<ne; i++) {
        if(rels_isrel_between(*rels, b, i, rmark)) buffb[nb++]=i;
    }

    // printf("nab: %i %i\n", na, nb);
    if(na==0 || nb==0) return REL_SCAN_NONE;

    // printf("begin to link chains\n");
    int change=REL_SCAN_NONE,
        ai, bi, events[4],
        *result=buff;
    events[1]=a;
    events[2]=b;
    for(int j=0; j<na; j++) {
        events[0]=ai=buffa[j];
        for(int i=0; i<nb; i++) {
            events[3]=bi=buffb[i];
            int chi=rels_link_chains(rels, events, 4, rmark);
            if(chi==REL_SCAN_NONE) continue;
            // if(change==REL_SCAN_CHANGE)
            // printf("events: %i %i change\n", j, i);
            change=REL_SCAN_CHANGE;
            if(ai==bi) {
                result[0]=ai;
                return REL_SCAN_FOUND;
            }
            // find event having weak relations with two strong-connected events
            //      is equal to before
            // if(rels_rel_between(*rels, ai, bi)!=REL_BOTH) continue;

            // event_t eva=rels_event_at(*rels, ai),
            //         evb=rels_event_at(*rels, bi);

            // int numa=eva.num, numb=evb.num;

            // if(numa!=numb) continue;

            // int nu=rels_events_weak_union_between(rels, ai, bi);
            // if(nu==0) continue;

            // result[0]=ai;
            // result[1]=bi;
            // return REL_SCAN_FOUND;
        }
    }

    return change;
}

// scan rels to update it
int rels_update_scan(rels_t *rels, int *result) {
    // printf("rels_update_scan\n");
    int ne=rels->ne;

    int change=0, //ai, bi,
        *buff=MALLOC(2*ne, int);
    for(int j=0; j<ne-1; j++) {
        for(int i=j+1; i<ne; i++) {
            if(rel_norel(rels->rels[i+j*ne])) continue;

            // printf("relation between %i %i: %i\n", j, i, 
                        // rel_rel(rels->rels[i+j*ne]));

            int chi;
            if(rel_isstrong(rels->rels[i+j*ne])) {
                chi=rels_update_midr(rels, j, i, REL_STRONG, buff);
                if(chi==REL_SCAN_CHANGE) change=chi; 
                else if(chi==REL_SCAN_FOUND) {
                    result[0]=buff[0];
                    result[1]=REL_WEAK;

                    free(buff);
                    return REL_SCAN_FOUND;
                }
            }
            if(rel_isweak(rels->rels[i+j*ne]))  {
                chi=rels_update_midr(rels, j, i, REL_WEAK, buff);
                if(chi==REL_SCAN_CHANGE) change=chi;
                else if(chi==REL_SCAN_FOUND) {
                    result[0]=buff[0];
                    result[1]=REL_STRONG;

                    free(buff);
                    return REL_SCAN_FOUND;
                }
            }
            // printf("\n");
        }
    }

    free(buff);
    return change;
}

// main function for chain scan
int mat_chain(matrix *mat) {
    // printf("mat chain\n");
    lattice *lats=mat->lat;
    submat  *rows=mat->rows,
            *cols=mat->cols,
            *blks=mat->blks;

    int ncnd=0;
    for(int i=0; i<81; i++) {
        if(lat_isset(lats+i)) continue;

        ncnd+=lat_cand_num(lats+i);
    }

    // printf("total candidates: %i\n", ncnd);

    rels_t rels;
    rels_init(&rels, ncnd);

    rels_init_mapevns(&rels, lats);
    // printf("got events: %i\n", n);

    // initial relations
    // lattices
    // printf("scan lats\n");
    for(int i=0; i<81; i++) {
        if(lat_isset(lats+i)) continue;

        // relation in this lattice
        int reli=REL_WEAK;
        if(lat_cand_num(lats+i)==2) reli|=REL_STRONG;

        int d0, d1=lat_1stcand(lats+i)-1,
            n0, n1;
        barr_t bins=lat_bins(lats+i);
        for(; d1<8; d1++) {
            if(!(bins&(1<<d1))) continue;
            n1=rels_map_latnum(rels, i, d1+1);
            for(d0=d1+1; d0<9; d0++) {
                if(!(bins&(1<<d0))) continue;
                n0=rels_map_latnum(rels, i, d0+1);
                rels_add_rel(&rels, n0, n1, reli);
                // printf("rel: %i %i %i\n", n0, n1, reli);
            }
        }
    }
    // printf("relations after scaning lats\n");
    // print_rels(rels);
    // print_rels_count(rels);
    // printf("\n");

    // scan sub-matrix
    // printf("scan submats\n");
    // printf("chain scan rows\n");
    update_rels_sub(&rels, rows, fid_row);
    // printf("chain scan cols\n");
    update_rels_sub(&rels, cols, fid_col);
    // printf("chain scan blks\n");
    update_rels_sub(&rels, blks, fid_blk);

    // printf("relations after scaning submats\n");
    // print_rels(rels);
    // print_rels_count(rels);
    // printf("\n");

    int found=0, result[3];
    for(int i=0; i<ncnd; i++) { // longest chain is with length=nevents
        found=rels_update_scan(&rels, result);

        // if(i%100==0) {
        //     printf("after %i trips scan\n", i+1);
        //     print_rels_count(rels);
        //     printf("\n");
        // }

        if(found==REL_SCAN_NONE) {
            printf("maxium chain scan: %i\n", i+1);
            printf("chain can't change any more\n");
            break;
        } else if(found==REL_SCAN_FOUND) {
            int ai=result[0],
                mark=result[1];
            // printf("found chain: %i %i", ai, bi);
            // if(mark&REL_STRONG) printf(" strong");
            // if(mark&REL_WEAK) printf(" weak");
            // printf("\n");
            
            event_t ev=rels_event_at(rels, ai);
            int nlat=ev.lat,
                // r=nlat/9,
                // c=nlat%9,
                d=ev.num;
            // printf("    event: lat (%i, %i), num %i,", r+1, c+1, d);
            // if(mark==REL_STRONG) printf(" true\n");
            // else printf(" false\n");

            // print
            // printf("    ");
            // print_chain_between(rels, ai, bi, mark);

            int len=rels_lenchain_between(rels, ai, ai, mark);
            if(len>lchn) lchn=len;

            if(mark==REL_STRONG) {
                // printf("    will update\n");
                mat_update(mat, nlat, d);
                // print_update(mat, nlat, "lats");
                print_chain(mat, rels, result);
            } else {
                // printf("    del lat (%i, %i), num %i\n", r+1, c+1, d);
                mat_del_latnum(mat, nlat, d);
                print_chain(mat, rels, result);
            }
            rels_free(&rels);
            return SCAN_SUCC;
        }
    }
    // printf("after %i trips scan\n", ncnd);
    // print_rels_count(rels);
    // printf("\n");
    // print_rels(rels);


    // printf("begin to free\n");
    rels_free(&rels);

    // printf("\n");
    return SCAN_NONE;
}