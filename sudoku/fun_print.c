// print function

#include <stdlib.h>
#include <stdio.h>

#include "sudoku.h"

// print lattice
void print_lat(lattice *lat) {
    if(lat_isset(lat)) {
        printf("set\n");
        printf("number: %i\n", lat->number);
    } else {
        printf("unset\n");
        printf("number of possible: %i\n", lat_cand_num(lat));
        printf("candidate:");
        for(int i=1; i<=9; i++) {
            if(lat_iscand(lat, i)) printf(" %i", i);
        }
        printf("\n");
    }
}

// print sub-matrix
void print_sub(submat *sub) {
    if(sub->unset==0) {
        printf("sub-matrix is set down\n");
        return;
    }

    cand_t *nums=sub->nums;

    printf("%i set numbers:", 9-sub->unset);
    for(int i=0; i<9; i++) {
        if(cand_isset(nums+i)) printf(" %i", i+1);
    }
    printf("\n");
    for(int i=0; i<9; i++) {
        if(cand_isset(nums+i)) continue;
        printf("number %i: %i lat", i+1, cand_num(nums+i));
        if(!cand_isset(nums+i)) {
            for(int n=0; n<9; n++) {
                if(bit_has(&(nums[i].arr), n))
                    printf(" %i", n+1);
            }
            printf("\n");
        }
    }
}

// print arrray candidate using binary bit
void print_cnd_bin(cand_t cnd) {
    if(cand_isset(&cnd)) {
        printf("xxxxxxxxx\n");
        return;
    }
    bit_print_arrbin(cnd.arr, 9);
    printf("\n");
}

void print_cnds_bin(cand_t *cnds) {
    for(int i=0; i<9; i++) print_cnd_bin(cnds[i]);
}

// print matrix
void print_mat(matrix *mat) {
    lattice *lat=mat->lat;
    int n=0;
    for(int j=0; j<9; j++) {
        if(j==3 || j==6) printf("\n");
        printf("       ");
        for(int i=0; i<9; i++, n++) {
            if(i==3 || i==6) printf("  ");
            if(lat_isset(lat+n)) printf(" %i", lat[n].number);
            else printf(" x");
        }
        printf("\n");
    }
}

// print lattice update
void print_update(matrix *mat, int n, char *scan) {
    if(mat->verbose<PRINT_INFO) return;
    int r=n/9,
        c=n-9*r,
        d=lat_num(mat->lat+n);
    printf("scan %s found: lat (%i, %i), num %i\n",
                scan, r+1, c+1, d);
    printf("\n");
    print_mat(mat);
    printf("\n");
}

// print cross analysis
void print_cross(matrix *mat, int cross, int s0, int s1, int d) {
    if(mat->verbose<PRINT_INFO) return;

    char *sub0, *sub1;
    switch(cross) {
        case CROSS_BLK_ROW:
            sub0="blk"; sub1="row"; break;
        case CROSS_BLK_COL:
            sub0="blk"; sub1="col"; break;
        case CROSS_ROW_BLK:
            sub0="row"; sub1="blk"; break;
        case CROSS_COL_BLK:
            sub0="col"; sub1="blk"; break;
        default:
            printf("error for cross print\n");
            exit(-1);
    }

    printf("cross: %s %i, %s %i, num %i\n",
                sub0, s0+1, sub1, s1+1, d);
}

static char *names[]={"row", "col", "blk", "num"};

void print_group(matrix *mat, int b, barr_t pos0, barr_t pos1, int t0, int t1) {
    if(mat->verbose<PRINT_INFO) return;

    char *nameb, *name0, *name1=names[t1];
    if(t1==MARK_NUM) { // group in number-sub plane
        nameb=names[t0];
        if(t0==MARK_BLK) name0="lat";
        else name0=names[MARK_ROW+MARK_COL-t0];
    } else {
        nameb="num";
        name0=names[t0];
    }

    printf("group: %s %i, %ss ", nameb, b+1, name0);
    for(int i=0; i<9; i++) {
        if(pos0&(1<<i)) printf("%i", i+1);
    }
    printf(", %ss ", name1);
    for(int i=0; i<9; i++) {
        if(pos1&(1<<i)) printf("%i", i+1);
    }
    printf("\n");
}

void print_chain(matrix *mat, rels_t rels, int *result) {
    if(mat->verbose<PRINT_INFO) return;

    int ai=result[0],
        bi=result[1],
        mark=result[2];
    
    printf("found chain: %i %i", ai, bi);
    if(mark&REL_STRONG) printf(" strong");
    if(mark&REL_WEAK) printf(" weak");
    printf("\n");
    if(ai==bi) {
        event_t ev=rels_event_at(rels, ai);
        int nlat=ev.lat,
            r=nlat/9,
            c=nlat%9,
            d=ev.num;
        printf("    event: lat (%i, %i), num %i,", r+1, c+1, d);
        if(mark==REL_STRONG) printf(" true\n");
        else printf(" false\n");

        // print
        printf("    ");
        print_chain_between(rels, ai, bi, mark);
        if(mark==REL_STRONG) {
            printf("    update: lat (%i, %i), num %i\n", r+1, c+1, d);
        } else {
            printf("    delete: lat (%i, %i), num %i\n", r+1, c+1, d);
        }
    } else {
        printf("    ");
        print_chain_between(rels, ai, bi, REL_STRONG);
        printf("    ");
        print_chain_between(rels, ai, bi, REL_WEAK);

        event_t *evts=rels.events;
        printf("    del: ");
        for(int i=0; i<rels.ne; i++) {
            if(evts[i].mark!=3) continue;
            print_event_at(rels, i);
            printf(" ");
        }
        printf("\n");
    }
}

// print for debug
// void print_group_debug(matrix *mat, cand_t *sub, cand_t *trans) {
//     if(mat->verbose<PRINT_DEBUG) return;
//     printf("mat group debug\n");
//     print_cnds_bin(sub);
//     printf("\n");
//     print_cnds_bin(trans);
// }

// print finished matrix
void print_result(matrix *mat) {
    printf("solution %i:\n", ++nsol);
    printf("total try: %i\n", ntry);
    printf("level of try: %i\n", mat->ntry);
    print_mat(mat);
    printf("\n");
}