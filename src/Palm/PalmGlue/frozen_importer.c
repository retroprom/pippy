#include "Python.h"
#include <PalmOS.h>
#include <PalmCompatibility.h>


static char db_name[] = "Python Modules-PyMd";
static int resource_base = 0x03e8;
static int resource_incr = 10;

static DmOpenRef dbref;

extern struct _frozen _PyImport_FrozenModules[];
extern VoidHand frozen_handles[];

/*  
    The pyc file for a given module is written to the database using the
    following structure.  Note that the first 8 bytes of the module are
    ignored, just as PyImport_FrozenModule likes it.
*/

typedef struct _frozen_record {
	Int32 size;
	char *name;
	unsigned char *code;
	VoidHand handle;
} frozen_record;

static frozen_record unpack_record( char *s ) {
	frozen_record rec;

	/* unpack the record */
	rec.size = *((Int32 *) s);
	s += 4;
	rec.name = (char *) s;
	s += StrLen((char *)s) + 1;
	rec.code = (unsigned char *) s;

	return rec;
}
	
typedef struct _frozen_res_entry {
  frozen_record rec;
  int used;
  int is_resource;
  struct _frozen_res_entry *next;
} *frozen_res_entry_t;
static frozen_res_entry_t all=NULL;

static frozen_record fromCache(char *name) 
{
    frozen_record null_rec = {0, NULL, NULL, 0};
    frozen_res_entry_t current;

    current = all;
    while (current) {
      if (StrCompare(current->rec.name, name)==0) {
	current->used = 1;
	return current->rec;
      }
      current = current->next;
    }
    return null_rec;
}


static void cleanupCache() 
{
  frozen_res_entry_t current, tmp;
  current = all;
  while (current) {
    tmp = current;
    if (!current->used) {
      MemHandleUnlock(current->rec.handle);
      if (current->is_resource) {
	DmReleaseResource(current->rec.handle);
      }
    }
    current = current->next;
    free(tmp);
  }
}

static frozen_record search_datamanager(DmOpenRef dbref, char *name) {
	
	/* search sequentially through the database for the requested
	   module name */

    static cached = 0;

    if (!cached) {
      UInt i;
      frozen_record null_rec = {0, NULL, NULL, 0};
      frozen_record rec = null_rec;
      char *s = NULL;
      VoidHand rec_hand;
      frozen_res_entry_t current;

      cached = 1;
      for (i=0; i<DmNumRecords(dbref); i++){
	rec_hand = DmQueryRecord(dbref, i);
	s = (char *) MemHandleLock(rec_hand);
	rec = unpack_record(s);
	rec.handle = rec_hand;
	current = (frozen_res_entry_t)malloc(sizeof(struct _frozen_res_entry));
	current->rec = rec;
	current->used = 0;
	current->is_resource = 0;
	current->next = all;
	all = current;
      }
    }
    return fromCache(name);
}

static frozen_record search_resources(char *name) {

    UInt i;
    frozen_record null_rec = {0, NULL, NULL, 0};
    frozen_record rec = null_rec;
    char *s = NULL;
    VoidHand rec_hand;
    static cached = 0;
    frozen_res_entry_t current;

    if (!cached) {
      cached = 1;
      for (i=resource_base; (rec_hand=DmGetResource('PyMd',i)) != 0; i+=resource_incr) {
	s = (char *) MemHandleLock(rec_hand);
	rec = unpack_record(s);
	rec.handle = rec_hand;
	current = (frozen_res_entry_t)malloc(sizeof(struct _frozen_res_entry));
	current->rec = rec;
	current->used = 0;
	current->is_resource = 1;
	current->next = all;
	all = current;

      }
    }
    return fromCache(name);
}


int initialize_FrozenModules()
	/* Load the _PyImport_FrozenModules structure with the compiled
	   code stored as a record in a Palm database (eg, pymods.pdb)
	   or as a resource in the .prc application file.
	*/
{
	struct _frozen *mod_struct;
	int i;
	frozen_record rec;
	LocalID dbid;
	int sts = 0;
	int search_db = 0;
	
	/* find and open the modules database */
	if ((dbid = DmFindDatabase(0, db_name))){
		dbref = DmOpenDatabase(0, dbid, dmModeReadOnly);
		if (dbref) 
			search_db = 1;
	}

	/* search for modules in need of importing */
	for (i=0; (mod_struct=_PyImport_FrozenModules + i)->name; i++) {
		if (mod_struct->code == NULL) { 
			/* try loading from data manager */
			if (search_db)
				rec = search_datamanager(dbref, mod_struct->name);
			if (rec.name == NULL ||  !search_db) 
				/* try loading from resources */
				rec = search_resources(mod_struct->name);

			/* if module has been found, then load the structure */
			if (rec.name != NULL){
				mod_struct->code = rec.code;

				/* the following will be a problem is the size > 2**15-1 */
				mod_struct->size = rec.size;
				frozen_handles[i] = rec.handle; /* save handle for future deletion */
				continue;
			}
			/* couldn't find it - ignore */
		}
	}
	sts = 1;
	cleanupCache();
	return sts;
}

int cleanup_FrozenModules(){
	Err err;
	int i;

	/* unlock the handles */
	for (i=0; _PyImport_FrozenModules[i].name != NULL; i++) {
		if (frozen_handles[i])
			MemHandleUnlock(frozen_handles[i]);
	}
	if (dbref) {
		err = DmCloseDatabase (dbref);
		if (err) return 0;
	}

	return 1;
}
