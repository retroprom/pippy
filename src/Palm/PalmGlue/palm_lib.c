#include "palm_lib.h"

static UInt serialRefNum;

void printSerial(char *mesg) {
	Err errP, err;
	SerSend (serialRefNum, mesg, StrLen(mesg), &errP);
	SerSend (serialRefNum, "\r", StrLen("\r"), &errP);
	err = SerSendWait (serialRefNum, -1);
}

void openSerial() {
	
	Err err;
	err = SysLibFind("Serial Library", &serialRefNum);
	if (err == sysErrLibNotFound) {
		ErrNonFatalDisplayIf( 1, "Library Not Found");
	}
	else if (err !=0) {
		ErrNonFatalDisplayIf( 1, "Something else");
	}
	err = SerOpen (serialRefNum, 0, 19200);
}

void closeSerial() {
	SerClose(serialRefNum);
}
