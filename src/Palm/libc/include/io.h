/**********************************************************************/
/*                                                                    */
/* io.h: input/output subsystem definitions                           */
/*                                                                    */
/* Jeffery D. Collins                                                 */
/* (c) Endeavors Technology, Inc.                                     */
/*                                                                    */
/* Distributed under the GNU General Public License;                  */
/* see the README file. This code comes with NO WARRANTY.             */
/*                                                                    */
/* Modification history                                               */
/*                                                                    */
/* When?      What?                                              Who? */
/* -------------------------------------------------------------------*/
/*                                                                    */
/**********************************************************************/

#ifndef IO_H
#define IO_H

/**********************************************************************/
/* Includes                                                           */
/**********************************************************************/
#include <PalmOS.h>
#include "libc_segments.h"

/**********************************************************************/
/* Prototypes                                                         */
/**********************************************************************/

/* cannot put these into segments - they must be exposed to callers
   of the library */
Boolean ioHandleEvent (EventPtr event);
void ioPutS (Char* s);
Err ioInit(UInt16 formID, UInt16 fieldID, UInt16 scrollerID, UInt16 maxChars);
void ioClear(void);

#endif
