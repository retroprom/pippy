#ifndef PALMGLUE_H
#define PALMGLUE_H

void set_flags Py_PROTO(( int f_dbg, int f_verbose, int f_interact, int f_nosite,
			  int f_except, int f_frozen, int f_optimize )) SEG_MAIN;

void set_frozen_tab( struct _frozen *f) SEG_MAIN;
int testcomplete(char *code) SEG_MAIN;
int run_SimpleString( char *command) SEG_MAIN;
int mkmodule(char *name, char *code) SEG_MAIN;

#endif /* PALMGLUE_H */
