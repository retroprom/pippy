
#ifndef _DBMEMPUB_H_
#define _DBMEMPUB_H_ 1


#include "other/segment_defs.h"

#ifndef SEG_DBMEM_H
#define SEG_DBMEM_H
#endif

extern int dbmem_init();
extern int dbmem_fini();

extern void * dbmem_addentry(void *p, long size) SEG_DBMEM_H;
int  dbmem_size(void *s) SEG_DBMEM_H;

void *dbmem_createentry(UInt32 *indexP, long size) SEG_DBMEM_H;
void dbmem_deleteentry(UInt32 index, void *recP) SEG_DBMEM_H;
void dbmem_set( void *recP, UInt8 value, UInt32 count) SEG_DBMEM_H;
void dbmem_write( void *recP, UInt32 offset, void *srcP, UInt32 count) SEG_DBMEM_H;


#endif /* _DBMEMPUB_H_ */
