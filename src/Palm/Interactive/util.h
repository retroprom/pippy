/**********************************************************************/
/*                                                                    */
/* util.h:  LISPME utility functions needed everywhere                */
/*                                                                    */
/* LispMe System (c) FBI Fred Bayer Informatics                       */
/*                                                                    */
/* Distributed under the GNU General Public License;                  */
/* see the README file. This code comes with NO WARRANTY.             */
/*                                                                    */
/* Modification history                                               */
/*                                                                    */
/* When?      What?                                              Who? */
/* -------------------------------------------------------------------*/
/* 10.01.1999 New                                                FBI  */
/* 01.04.2000 Prepared for GCC 2.0 and SDK 3.5                   FBI  */
/*                                                                    */
/**********************************************************************/

#ifndef INC_UTIL_H
#define INC_UTIL_H


/**********************************************************************/
/* Includes                                                           */
/**********************************************************************/
#include <PalmOS.h>
#include "store.h"
/**********************************************************************/
/* Exported data                                                      */
/**********************************************************************/
extern  char* errInfo;
extern  Int16 expectedLen;

/**********************************************************************/
/* prototypes                                                         */
/**********************************************************************/
void*   ptrFromObjID(UInt16 obj);
void    GrabMem(void);
void    ReleaseMem(void);
void    enable(UInt16 id);
void    disable(UInt16 id);
void    disableButtons(void);
void    enableButtons(void);
void    displayError(UInt32 err);                 
void    enableCtls(Boolean enable);
void    updateScrollBar(void);                       

#endif
