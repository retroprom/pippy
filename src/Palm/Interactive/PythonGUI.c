/**********************************************************************/
/*                                                                    */
/* PythonGUI.c: Main driver                                           */
/*                                                                    */
/* Jeffery D. Collins                                                 */
/* (c) Endeavors Technology, Inc.                                     */
/*                                                                    */
/* Portions (c) FBI Fred Bayer Informatics                            */
/*                                                                    */
/* Distributed under the GNU General Public License;                  */
/* see the README file. This code comes with NO WARRANTY.             */
/*                                                                    */
/**********************************************************************/


#include "Python.h"
#include "PythonGUI.h"
#include "PalmOS.h"
#include "PalmCompatibility.h"
#include "set_a4.h"
#include "util.h"
#include "io.h"
#include "stdio.h"
#include "palmeventmodule.h"

#include "GLib_interface.h"
#include "GLib_stub.h"

extern struct LibRef *GLib_PyLb_libref = NULL;
int initialize_FrozenModules();
int cleanup_FrozenModules();
extern struct _frozen _PyImport_FrozenModules[];
int run_SimpleString(char *command);

void python_init();
void python_finalize();

char  *stackLimit;
UInt16 startPanel;


MemHandle    outHandle;
MemHandle    inHandle;
FieldPtr     inField;
FieldPtr     outField;
FormPtr      mainForm;
ScrollBarPtr scrollBar;

Boolean running;
#define STACK_AVAIL 16384   /* need the actual stack specified in prefs000.bin */

/* static forward */
static Boolean StartApp();
static void StopApp(void);
static Boolean appHandleEvent(EventType* e);

void _gdb_hook(void);

int 
python_loop_handler()
{
	/* the handler function must be wrapped in the 
	   callback macros */

	int not_done, interrupted;
	EventType e;

	SET_A4_FROM_A5;
	interrupted = 0;
	not_done = 1;
	if (EvtSysEventAvail (1)) {
		while (not_done) {
			EvtGetEvent(&e, 0);
			if (e.eType == nilEvent){
				not_done = 0; 
				continue;
			}
			switch (e.eType){
			case penDownEvent:
				not_done = 0;
				interrupted = 1;
				break;
			default:
				break;
			}
		}
	}
	RESTORE_A4;
	return interrupted;
}



static char** listFrom_tSTL(DmResID resID, UInt16 *num)
{
	MemHandle strRscH, arrH;
	char** res=NULL;
	char *p;

	strRscH = DmGetResource ('tSTL', resID);
	if (strRscH == NULL)
		goto finish;
	p = (char*) MemHandleLock(strRscH);
	if (p == NULL) {
		DmReleaseResource(strRscH);
		goto finish;
	}
	p += StrLen(p) + 1;
	MemMove(num, p, 2);
	p+=2;
	arrH = SysFormPointerArrayToStrings ( p, *num);
	if (arrH == NULL)
		goto finish;
	if (MemHandleUnlock(strRscH) != 0)
		goto finish;
	if (DmReleaseResource(strRscH) != errNone)
		goto finish;
/* 	if (MemHandleFree(strRscH) !=0 ) */
/* 		goto finish; */

	res = (char**)MemHandleLock(arrH);
  finish:
	return res;

}
/**********************************************************************/
/* Build list of known symbols                                        */
/**********************************************************************/
static char** bltins=NULL;
static void fillBltinList(void)
{
	static char* emergency[] = {"*** Not enough memory ***"};
	UInt16 num;
	ListPtr list;

	if (bltins) 
		return;
	list = (ListPtr)ptrFromObjID(BUILTIN_LIST);
	if ((bltins = listFrom_tSTL(BUILTIN_LIST_ITEMS, &num)))
		LstSetListChoices(list, bltins, num);
	else
		LstSetListChoices(list, emergency,1);
	list->attr.search = true;
}


static char** module_list=NULL;
static void fillModuleList(void)
{
	static char* emergency[] = {"*** Not enough memory ***"};
	UInt16 num;
	ListPtr list;

	if (module_list) 
		return;
	list = (ListPtr)ptrFromObjID(MODULE_LIST);
	if ((module_list = listFrom_tSTL(MODULE_LIST_ITEMS, &num)))
		LstSetListChoices(list, module_list, num);
	else
		LstSetListChoices(list, emergency,1);
	list->attr.search = true;
}



/**********************************************************************/
/* init all subsystems                                                */
/**********************************************************************/
static Boolean StartApp()
{
	Err     error;
	UInt32  ver;
	char errmsg[80];


	/* check for recent PalmOS revision */
	error = FtrGet(sysFtrCreator, sysFtrNumROMVersion, &ver);
	if (error || ver < 0x03500000)
	{
		UInt16 res;
		res = FrmAlert(OS_VERSION_ERROR);
		if (res == 1) return false;
	}
	

	/* open the VM library */
#ifndef NO_GLIB
	GLib_PyLb_libref = GLibOpenSoft('PyLb',"Python VM", errmsg);
	if (!GLib_PyLb_libref)
		ErrFatalDisplayIf(GLib_PyLb_libref == NULL, "Cannot open Python VM GLib");
#endif
	startPanel = MAINFRAME;

	/* initialize stdout */
	ioInit(MAINFRAME, OUTPUT_FIELD, OUTPUT_SCROLLBAR, OUTPUT_SIZE);

	return true;
}

static void StopApp(void)
{
	python_finalize();

	FrmCloseAllForms();

#ifndef NO_GLIB
	GLibClose(GLib_PyLb_libref);
	GLib_PyLb_libref = NULL;
#endif
}

static Boolean MainFrameHandleEvent(EventType *e)
{
	Boolean handled = false;
	
   	SET_A4_FROM_A5;
	
	// See if stdout
	if (ioHandleEvent(e)) {
		RESTORE_A4;
		return true;
	}

	switch (e->eType)
	{
	case frmOpenEvent:
	{
		MemHandle oldHandle;
		startPanel = MAINFRAME;
		mainForm   = FrmGetActiveForm();
		inField    = ptrFromObjID(INPUT_FIELD);
		outField   = ptrFromObjID(OUTPUT_FIELD);
		
		/*--------------------------------------------------------------*/
		/* Init handle for input field                                  */
		/*--------------------------------------------------------------*/
		oldHandle = FldGetTextHandle(inField);
		FldSetTextHandle(inField, inHandle);
		if (oldHandle)
			MemHandleFree(oldHandle);
		
		/*--------------------------------------------------------------*/
		/* Init other controls                                          */
		/*--------------------------------------------------------------*/
		if (running)
			disableButtons();
		enableCtls(true);
		
		FrmSetFocus(mainForm, FrmGetObjectIndex(mainForm, INPUT_FIELD));

		handled = true;
		break;
	}

	case frmCloseEvent:
		FldCompactText(inField); 
		FldSetTextHandle(inField, NULL);

		break;
	case ctlSelectEvent:
		switch (e->data.ctlSelect.controlID)
		{
		case BREAK_BUTTON:
			/*----------------------------------------------------------*/
			/* Break excution                                           */
			/*----------------------------------------------------------*/
			enableButtons();
			{
				FormPtr frm;
				frm = FrmGetActiveForm();
				FrmSetFocus(frm,
				       FrmGetObjectIndex(frm, INPUT_FIELD));
			}
			handled = true;
			break;
		case BUILTIN_LIST_TRIGGER:
			fillBltinList();
			break;
		case MODULE_LIST_TRIGGER:
			fillModuleList();
			break;
		case EVAL_BUTTON:
			/*----------------------------------------------------------*/
			/* Evaluate entered expression:                             */
			/*   Parse and compile and set state to running             */
			/*----------------------------------------------------------*/
			ErrTry {
				char *in = FldGetTextPtr(inField);
				if (in != NULL && StrLen(in) > 0) {
					int res;
					disableButtons();
					/* run python command here */
					if ( (in+StrLen(in))[0] != '\n')
						printf("%s\n", in);
					else
						printf(in);
					res = run_SimpleString(in);
					enableButtons();
				}
				else {
					handled = true;
					break;
				}
			}
			ErrCatch(err) {
				/* we want to place the pointer at the point
				   in the string where the error occurred
				   and retain that input text */

			} ErrEndCatch;
			/* for now, flush stdin */
			
			{
				FormPtr frm = FrmGetActiveForm();
				UInt16 index = FrmGetObjectIndex(frm, INPUT_FIELD);
				FieldPtr fldP = FrmGetObjectPtr(frm, index);
				       
				FldFreeMemory(fldP);
				FldEraseField( fldP );
				FldDrawField( fldP );

			}
			handled = true;
			break;
			
		default:
			break;
		}
		break;

	case popSelectEvent:
		/*--------------------------------------------------------------*/
		/* Insert selected symbol into input field                      */
		/*--------------------------------------------------------------*/
 		if (bltins || module_list)
		{
			UInt16 sel = e->data.popSelect.selection;
			char*  item = LstGetSelectionText((ListPtr)e->data.popSelect.listP,sel);
			FldInsert(inField, item, StrLen(item));
		}
		handled = true;
		break;
	case menuEvent:
		switch (e->data.menu.itemID)
		{
		case MENU_ClearInput:
			/*----------------------------------------------------------*/
			/* Clear input field                                        */
			/*----------------------------------------------------------*/
			FldDelete(inField, 0, FldGetTextLength(inField));
			handled = true;
			break;
			
		case MENU_ClearOutput:
			/*----------------------------------------------------------*/
			/* Clear output field                                       */
			/*----------------------------------------------------------*/
			ioClear();
			break;
		case MENU_HelpAbout:
			FrmAlert(ABOUT);
			handled = true;
			break;
		}
		break;
	default:
		handled = false;
		break;
	}
 
	RESTORE_A4;
	return handled;
}


/**********************************************************************/
/* Application event handling                                         */
/**********************************************************************/
static Boolean appHandleEvent(EventType* e)
{
	if (e->eType == frmLoadEvent)
	{
		Int16   formId = e->data.frmLoad.formID;
		FormPtr form   = FrmInitForm(formId);
		FrmSetActiveForm(form);
		
		switch(formId)
		{
		case MAINFRAME:
			FrmSetEventHandler(form, MainFrameHandleEvent);
			break;
		default:
			return false;
			break;
		}
		return true;
	}
	return false;
}
Boolean bogusEventHandler(EventType *e)
{
	Boolean res;
	SET_A4_FROM_A5;
	/* A bogus event handler to test genericEventHandler */
	res = MainFrameHandleEvent(e);
	RESTORE_A4;
	return res;

}
/**********************************************************************/
/* Main entry                                                         */
/**********************************************************************/
UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	EventType e;
	short err;

/* 	_gdb_hook(); */

	switch (cmd)
	{
	case sysAppLaunchCmdNormalLaunch:
		cmdPBP = NULL;
		if (!StartApp())
			return 0;

		/*--------------------------------------------------------------*/
		/* Determine stack limit                                        */
		/*--------------------------------------------------------------*/
		stackLimit = ((char*)&e) - STACK_AVAIL;
		FrmGotoForm(startPanel);
		python_init();
		PyOS_SetInterruptChecker(python_loop_handler);
/* 		registerGenericEventHandler(MainFrameHandleEvent); */
		registerGenericEventHandler(bogusEventHandler);
		
		do {
			EvtGetEvent(&e, running ? 0 : -1);
			if (!SysHandleEvent(&e))
				if (!MenuHandleEvent(0, &e, &err))
					if (!appHandleEvent(&e))
						FrmDispatchEvent(&e);
		} while (e.eType != appStopEvent);
		StopApp();
	}
	return 0;
}
