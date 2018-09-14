// functions for mat chain

#include <stdlib.h>
#include <stdio.h>

#include "mat_chain.h"

// io functions
void print_rels(rels_t rls) {
    rel_t *rels=rls.rels;
    int n=rls.ne;
    for(int j=0; j<n; j++) {
        for(int i=0; i<n; i++) printf("%i", rel_rel(rels[i+n*j]));
        printf("\n");
    }
}

void print_rels_count(rels_t rels) {
    int nt, ns, nw;
    nt=rels_nsw(rels, &ns, &nw);
    printf("    total relations: %i\n", nt);
    printf("    strong relations: %i\n", ns);
    printf("    weak relations: %i\n", nw);
    printf("    strong and weak: %i\n", ns+nw-nt);

    nt=rels_ndiag(rels, &ns, &nw);
    printf("along diagonal line:\n");
    printf("    total relations: %i\n", nt);
    printf("    strong relations: %i\n", ns);
    printf("    weak relations: %i\n", nw);
    printf("    strong and weak: %i\n", ns+nw-nt);
}

void print_event_at(rels_t rels, int n) {
    event_t ev=rels_event_at(rels, n);
    int nlat=ev.lat,
        r=nlat/9,
        c=nlat%9,
        d=ev.num;
    printf("[(%i, %i), %i]", r+1, c+1, d);
}

void print_chain_between(rels_t rels, int e0, int e1, int rmark, char *indent) {
    // int nst=NCHAIN(REL_STRONG),
        // nwk=NCHAIN(REL_WEAK);
    char *rel_seps[2];
    rel_seps[NCHAIN(REL_STRONG)]=REL_SEP_STRONG;
    rel_seps[NCHAIN(REL_WEAK)]=REL_SEP_WEAK;

    // printf("strong: %s\n", rel_seps[nst]);
    // printf("weak: %s\n", rel_seps[nwk]);

    chain_t chain=rels_chain_between(rels, e0, e1, rmark);
    int len=chain_len(chain);
    // printf("chain len: %i\n", len);

    int nsep=NCHAIN(rmark);
    // printf("sep now: %s\n", rel_seps[nsep]);

    // printf("chain:\n");
    int *arr=chain.arr,
        ne_line=4;    // number of events printed in one line

    printf("%s  ", indent);
    print_event_at(rels, e0);
    for(int i=0; i<len; i++) {
        if((i+1)%ne_line==0) printf("\n%s", indent);   // count in e0
        printf("%s", rel_seps[nsep]);
        nsep=1-nsep;
        print_event_at(rels, arr[i]);
    }
    if((len+1)%ne_line==0) printf("\n%s", indent);   // count in e0
    printf("%s", rel_seps[nsep]);
    print_event_at(rels, e1);

    printf("\n");
}

// void print_bothchains_between(rels_t rels, int e0, int e1) {
//     print_chain_between(rels, e0, e1, REL_STRONG);
//     print_chain_between(rels, e0, e1, REL_WEAK);
// }

// functions for chain
void chain_init(chain_t *chn) {
    chn->n=0;
    chn->arr=NULL;
}

void chain_free(chain_t *chn) {
    if((chn->n>0)&&(chn->arr)) free(chn->arr);
    chain_init(chn);
}

void chain_add_arr(chain_t *chn, int *arr, int n) {
    // printf("chain add: arr len; %i\n", n);
    chn->n=n;
    chn->arr=arr;
}

int chain_len(chain_t chn) {
    return chn.n;
}

void chain_copy_to(chain_t chn, int *dst, int *pos, int reverse) {
    // printf("chain copy\n");
    if(chn.n<=0) return;

    // printf("begin to copy\n");
    int i0=0, i1=chn.n, shift=1;
    if(reverse) {
        i0=chn.n-1;
        i1=-1;
        shift=-1;
    }

    int n=*pos;
    for(; i0!=i1; i0+=shift) dst[n++]=chn.arr[i0];
    *pos=n;
}

// functions for rel
void rel_init(rel_t *rel) {
    rel->rel=0;
    chain_init(rel->chains);
    chain_init(rel->chains+1);
}

void rel_free(rel_t *rel) {
    rel->rel=0;
    chain_free(rel->chains);
    chain_free(rel->chains+1);
}

void rel_add_rel(rel_t *rel, int mark) {
    rel->rel|=mark;
}

void rel_add_chain(rel_t *rel, int mark, int *chain, int n) {
    chain_add_arr(rel->chains+NCHAIN(mark), chain, n);
}

void rel_copychain_to(rel_t rel, int mark, int *dst, int *pos, int reverse) {
    // printf("rel copy chain with mark: %i\n", mark);
    // printf("    reverse: %i\n", reverse);
    chain_copy_to(rel.chains[NCHAIN(mark)], dst, pos, reverse);
}

int rel_norel(rel_t rel) {
    return rel.rel==0;
}

int rel_isrel(rel_t rel, int mark) {
    return rel.rel&mark;
}

int rel_isstrong(rel_t rel) {
    return rel.rel&REL_STRONG;
}

int rel_isweak(rel_t rel) {
    return rel.rel&REL_WEAK;
}

int rel_rel(rel_t rel) {
    return rel.rel;
}

chain_t rel_chain(rel_t rel, int mark) {
    return rel.chains[NCHAIN(mark)];
}

int rel_lenchain(rel_t rel, int mark) {
    // not include two ends
    return chain_len(rel.chains[NCHAIN(mark)]);
}

// function for rels
void rels_init(rels_t *rels, int n) {
    rels->ne=n;
    rels->map=MALLOC(9*81, int);
    rels->events=MALLOC(n, event_t);
    rels->rels=MALLOC(n*n, rel_t);

    for(int i=0; i<9*81; i++) rels->map[i]=-1;

    for(int i=0; i<n*n; i++) rel_init(rels->rels+i);
}

void rels_set_event_at(rels_t *rels, int n, int lat, int d) {
    rels->events[n].lat=lat;
    rels->events[n].num=d;
    rels->map[d-1+9*lat]=n;
}

void rels_free(rels_t *rels) {
    free(rels->map);
    free(rels->events);

    int n=rels->ne;
    for(int j=0; j<n; j++) {
        for(int i=j; i<n; i++) rel_free(rels->rels+i+j*n);
    }
    free(rels->rels);
}

void rels_add_rel(rels_t *rls, int e0, int e1, int rmark) {
    rel_t *rels=rls->rels;
    int ne=rls->ne;

    rel_add_rel(rels+e0+ne*e1, rmark);
    rel_add_rel(rels+e1+ne*e0, rmark);
}

void rels_add_relchain(rels_t *rls, int e0, int e1,
                                    int rmark, int *chain, int n) {
    // printf("add relchain between: %i %i\n", e0, e1);
    rel_t *rels=rls->rels;
    int ne=rls->ne;

    rel_add_rel(rels+e0+ne*e1, rmark);
    rel_add_rel(rels+e1+ne*e0, rmark);

    rel_add_chain(rels+e0+ne*e1, rmark, chain, n);
    rel_add_chain(rels+e1+ne*e0, rmark, chain, n);
    // printf("\n");
}


void rels_copy_chainbetween_to(rels_t rels, int e0, int e1, int rmark,
                                    int *dst, int *pos) {
    // printf("copy chain from %i to %i\n", e0, e1);
    // printf("    rmark: %i\n", rmark);
    int ne=rels.ne,
        reverse=0;
    if(e0>e1) reverse=1;
    return rel_copychain_to(rels.rels[e1+e0*ne], rmark, dst, pos, reverse);
}

int rels_link_chains(rels_t *rels, int *events, int n, int rmark) {
    // among events array, all neighbors are linked with chains
    // but chains are disconnected
    // printf("link chains rmark: %i\n", rmark);
    // printf("     events:%i %i %i %i\n", events[0], events[1], events[2], events[3]);
    int start=events[0], end=events[n-1];
    if(rels_isrel_between(*rels, start, end, rmark)) return REL_SCAN_NONE;

    // printf("got it: unset before\n");
    int len=n-2;
    // printf("len: %i\n", len);
    for(int i=0; i<n-1; i++)
        len+=rels_lenchain_between(*rels, events[i], events[i+1], rmark);

    // printf("len: %i\n", len);
    int *chain=MALLOC(len, int);

    int ei=0, estop=n-2, shift=1;
    if(start>end) {
        ei=n-1;
        estop=1;
        shift=-1;
    }

    // printf("eistop, shift: %i %i %i\n", ei, estop, shift);

    start=events[ei];
    int pos=0;
    for(; ei!=estop; ei+=shift) {
        // printf("ei: %i\n", ei);
        // printf("chain: %i %i\n", events[ei], events[ei+shift]);
        // printf("pos: %i\n", pos);
        rels_copy_chainbetween_to(*rels, events[ei], events[ei+shift], rmark, 
                                        chain, &pos);
        // printf("pos: %i\n", pos);
        // printf("ei: %i\n", ei);
        chain[pos++]=events[ei+shift];
    }
    rels_copy_chainbetween_to(*rels, events[ei], events[ei+shift], rmark,
                                        chain, &pos);
    end=events[ei+shift];

    rels_add_relchain(rels, start, end, rmark, chain, pos);
    return REL_SCAN_CHANGE;
}

int rels_events_weak_union_between(rels_t *rels, int e0, int e1) {
    // find events having weak relation with both e0 and e1
    event_t *events=rels->events;
    int ne=rels->ne;

    for(int i=0; i<ne; i++) events[i].mark=0;

    for(int i=0; i<ne; i++) {
        // if(i==e1) continue;
        if(!rels_isrel_between(*rels, e0, i, REL_WEAK)) continue;
        events[i].mark|=1;
    }

    int nu=0;
    for(int i=0; i<ne; i++) {
        // if(i==e0) continue;
        if(!rels_isrel_between(*rels, e1, i, REL_WEAK)) continue;
        events[i].mark|=2;
        if(events[i].mark==3) nu++;
    }

    return nu;
}

int rels_isrel_between(rels_t rels, int e0, int e1, int rmark) {
    int ne=rels.ne;
    return rel_isrel(rels.rels[e1+e0*ne], rmark);
}

chain_t rels_chain_between(rels_t rels, int e0, int e1, int rmark) {
    int ne=rels.ne;
    return rel_chain(rels.rels[e1+e0*ne], rmark);
}

int rels_rel_between(rels_t rels, int e0, int e1) {
    int ne=rels.ne;
    return rel_rel(rels.rels[e1+e0*ne]);
}

int rels_lenchain_between(rels_t rels, int e0, int e1, int rmark) {
    int ne=rels.ne;
    return rel_lenchain(rels.rels[e1+e0*ne], rmark);
}

int rels_map_latnum(rels_t rels, int n, int d) {
    return rels.map[d-1+9*n];
}

event_t rels_event_at(rels_t rels, int n) {
    return rels.events[n];
}

int rels_nsw(rels_t rels, int *ns, int *nw) {
    int ne=rels.ne;

    int nt=0;  // number of total relation
    *ns=*nw=0;
    for(int j=0; j<ne; j++) {
        for(int i=j; i<ne; i++) {
            if(rel_norel(rels.rels[i+j*ne])) continue;

            nt++;
            if(rel_isstrong(rels.rels[i+j*ne])) (*ns)++;
            if(rel_isweak(rels.rels[i+j*ne])) (*nw)++;
        }
    }

    return nt;
}

int rels_ndiag(rels_t rels, int *ns, int *nw) {
    // relations along diagonal
    int ne=rels.ne;

    int nt=0;  // number of total relation
    *ns=*nw=0;
    for(int i=0; i<ne; i++) {
        if(rel_norel(rels.rels[i+i*ne])) continue;

        nt++;
        if(rel_isstrong(rels.rels[i+i*ne])) (*ns)++;
        if(rel_isweak(rels.rels[i+i*ne])) (*nw)++;
    }

    return nt;
}

