#ifndef PGEN_C_H
#define PGEN_C_H

#include "proto_defs.h"
/* previously forward declarations stripped from object.c */

static void compile_rhs Py_PROTO((labellist *ll,
			       nfa *nf, node *n, int *pa, int *pb)) SEG_PGEN_C;
static void compile_alt Py_PROTO((labellist *ll,
			       nfa *nf, node *n, int *pa, int *pb)) SEG_PGEN_C;
static void compile_item Py_PROTO((labellist *ll,
				nfa *nf, node *n, int *pa, int *pb)) SEG_PGEN_C;
static void compile_atom Py_PROTO((labellist *ll,
				nfa *nf, node *n, int *pa, int *pb)) SEG_PGEN_C;
static int addnfastate Py_PROTO((nfa *nf)) SEG_PGEN_C;
static void addnfaarc Py_PROTO((nfa *nf, int, int, int)) SEG_PGEN_C;
static nfa * newnfa Py_PROTO((char *name)) SEG_PGEN_C;
static void compile_rule Py_PROTO((nfagrammar *gr, node *n)) SEG_PGEN_C;

static nfagrammar *newnfagrammar Py_PROTO(()) SEG_PGEN_C;
static nfa *addnfa Py_PROTO((nfagrammar *gr, char *name)) SEG_PGEN_C;
static nfagrammar *metacompile Py_PROTO((node *n)) SEG_PGEN_C;
static void compile_rule Py_PROTO((nfagrammar *gr, node *n)) SEG_PGEN_C;
static void compile_rhs Py_PROTO((labellist *ll,nfa *nf,node *n, int *pa, int *pb)) SEG_PGEN_C;
static void compile_alt Py_PROTO((labellist *ll, nfa *nf,node *n,int *pa, int *pb)) SEG_PGEN_C;
static void compile_item Py_PROTO((labellist *ll, nfa *nf,node *n, int *pa, int *pb)) SEG_PGEN_C;
static void compile_atom Py_PROTO((labellist *ll, nfa *nf,node *n, int *pa, int *pb)) SEG_PGEN_C;
static void dumpstate Py_PROTO((labellist *ll, nfa *nf, int istate)) SEG_PGEN_C;
static void dumpnfa Py_PROTO((labellist *ll, nfa *nf)) SEG_PGEN_C;
static void addclosure Py_PROTO((bitset ss, nfa *nf, int istate)) SEG_PGEN_C;
static void printssdfa Py_PROTO((int xx_nstates, ss_state *xx_state, int nbits,
			      labellist *ll, char *msg)) SEG_PGEN_C;
static void simplify Py_PROTO((int xx_nstates, ss_state *xx_state)) SEG_PGEN_C;
static void convert Py_PROTO((dfa *d, int xx_nstates, ss_state *xx_state)) SEG_PGEN_C;
static void makedfa Py_PROTO((nfagrammar *gr,nfa *nf,dfa *d)) SEG_PGEN_C;
static void printssdfa Py_PROTO((int xx_nstates, ss_state *xx_state,int nbits,labellist *ll,char *msg)) SEG_PGEN_C;
static int samestate Py_PROTO((ss_state *s1, ss_state *s2)) SEG_PGEN_C;
static void simplify Py_PROTO((int xx_nstates, ss_state *xx_state)) SEG_PGEN_C;
static void renamestates Py_PROTO((int xx_nstates,ss_state *xx_state,int from, int to)) SEG_PGEN_C;
static void convert Py_PROTO((dfa *d, int xx_nstates,ss_state *xx_state)) SEG_PGEN_C;
static grammar *maketables Py_PROTO((nfagrammar *gr)) SEG_PGEN_C;

#endif /* PGEN_C_H */
