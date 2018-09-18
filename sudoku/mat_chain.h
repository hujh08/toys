
// structure of event--fill a lattice with a number
typedef struct
{
    int lat, num;
    int mark;   // used to mark true/false
} event_t;

typedef struct
{
	int n,        // number of points in the chain except the two ends
		*arr;
} chain_t;

typedef struct
{
	int rel;
	chain_t chains[2];
} rel_t;

typedef struct
{
	int ne,    // number of events
		// len,   // chains shorter than len all found
		*map;
	event_t *events;
	rel_t *rels;
} rels_t;

// relation between two events
#define REL_WEAK 1
#define REL_STRONG 2
#define REL_BOTH 3   // both weak and strong
#define REL_MARK 3   // mark the relation bits

#define REL_REVERSE(mark) ((~(mark))&REL_MARK)
#define NCHAIN(rel) ((rel)-1)  // id in chains for weak/strong relation

#define REL_SCAN_NONE 0
#define REL_SCAN_CHANGE 1
#define REL_SCAN_FOUND 2

// seperator for weak/strong relation
#define REL_SEP_WEAK "--"
#define REL_SEP_STRONG "=="

// malloc
#define MALLOC(size, type) ((type *) malloc((size)*sizeof(type)))

// io functions
extern void print_rels(rels_t );
extern void print_rels_count(rels_t );
extern void print_event_at(rels_t , int );
extern void print_chain_between(rels_t , int , int , int , char *);
// extern void print_bothchains_between(rels_t , int , int );

// functions for chain
extern void chain_init(chain_t *);
extern void chain_free(chain_t *);
extern void chain_add_arr(chain_t *, int *, int );
extern void chain_copy_to(chain_t , int *, int *, int );
extern int chain_len(chain_t );

// functions for rel
extern void rel_init(rel_t *);
extern void rel_free(rel_t *);
extern void rel_add_rel(rel_t *, int );
extern void rel_add_chain(rel_t *, int , int *, int n);
extern void rel_copychain_to(rel_t , int , int *, int *, int );
extern int rel_norel(rel_t );
extern int rel_isstrong(rel_t );
extern int rel_isweak(rel_t );
extern int rel_isrel(rel_t , int );
extern chain_t rel_chain(rel_t , int );
extern int rel_rel(rel_t );

// functions for rels
extern void rels_init(rels_t *, int );
extern void rels_set_event_at(rels_t *, int , int , int );
extern void rels_free(rels_t *);
extern void rels_add_rel(rels_t *, int , int , int );
extern void rels_add_chain(rels_t *, int , int , int , int *, int );
extern void rels_copy_chainbetween_to(rels_t , int , int , int , int *, int *);
extern int rels_events_weak_union_between(rels_t *, int , int );
extern int rels_link_chains(rels_t *, int *, int , int );
extern int rels_isrel_between(rels_t , int , int , int );
extern chain_t rels_chain_between(rels_t , int , int , int );
extern int rels_rel_between(rels_t , int , int );
extern int rels_lenchain_between(rels_t , int , int , int );
extern event_t rels_event_at(rels_t , int );
extern int rels_map_latnum(rels_t , int , int );
extern int rels_nsw(rels_t , int *, int *);
extern int rels_ndiag(rels_t , int *, int *);