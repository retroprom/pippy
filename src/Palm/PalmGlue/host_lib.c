
/*
  Logging using the PalmOS Host* services.
*/

#include <PalmOS.h>
#include "stdio.h"

static HostFILE *logfile = NULL;

void host_printf(char *message)
{
        if (logfile == NULL) {
                logfile = HostFOpen("palm_intern_strings.log", "w");
                if (logfile == NULL){
                        printf("problem opening log file\n");
                        return;
                }
        }
        HostFPrintF(logfile, message);
        HostFPrintF(logfile, "\n");
        HostFFlush (logfile);
}
