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
/* Modification history                                               */
/*                                                                    */
/* When?      What?                                              Who? */
/* -------------------------------------------------------------------*/
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

static Boolean   launchedExternally = true;

/* globals - move to an appropriate include file */
Boolean palmIII;
char  charEllipsis;
char  charNumSpace;
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
	Err err;

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
	list = (ListPtr)ptrFromObjID(IDC_PL_SYMS);
	if ((bltins = listFrom_tSTL(IDC_ST_SYMS, &num)))
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
	list = (ListPtr)ptrFromObjID(IDC_PL_MODS);
	if ((module_list = listFrom_tSTL(IDC_ST_MODS, &num)))
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


	/* open the VM library */

	GLib_PyLb_libref = GLibOpenSoft('PyLb',"Python VM", errmsg);
	if (!GLib_PyLb_libref)
		ErrFatalDisplayIf(GLib_PyLb_libref == NULL, "Cannot open Python VM GLib");


	/*------------------------------------------------------------------*/
	/* Check for PalmOS2                                                */
	/*------------------------------------------------------------------*/
	error = FtrGet(sysFtrCreator, sysFtrNumROMVersion, &ver);
	if (error || ver < 0x03000000)
	{
		FrmAlert(ERR_O4_OS_VERSION);
		return false;
	}
	
	/*------------------------------------------------------------------*/
	/* Version-specific settings                                        */
	/*------------------------------------------------------------------*/
	palmIII      = ver >= 0x03000000;
	charEllipsis = ver >= 0x03100000 ? 0x18 : 0x85;
	charNumSpace = ver >= 0x03100000 ? 0x19 : 0x80;

	startPanel = IDD_MainFrame;

	/* initialize stdout */
	{
		UInt16 t1 = IDD_MainFrame;
		UInt16 t2 = IDC_EF_OUTPUT;
		UInt16 t3 = IDC_SB_OUTPUT;
		UInt16 t4 =  OUTPUT_SIZE;
	}
	ioInit(IDD_MainFrame, IDC_EF_OUTPUT, IDC_SB_OUTPUT, OUTPUT_SIZE);

	/* load shared library */
	/* initialize Python interpreter */


	return true;
}

static void StopApp(void)
{
	python_finalize();

	FrmCloseAllForms();

	GLibClose(GLib_PyLb_libref);
	GLib_PyLb_libref = NULL;
	/* finalize Python interpreter */
	/* unload shared library */

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
		startPanel = IDD_MainFrame;
		mainForm   = FrmGetActiveForm();
		inField    = ptrFromObjID(IDC_EF_INPUT);
		outField   = ptrFromObjID(IDC_EF_OUTPUT);
		
		/*--------------------------------------------------------------*/
		/* Init handle for input field                                  */
		/*--------------------------------------------------------------*/
		oldHandle = FldGetTextHandle(inField);
		FldSetTextHandle(inField, inHandle);
		if (oldHandle)
			MemHandleFree(oldHandle);
		
		/*--------------------------------------------------------------*/
		/* Init handle for output field                                 */
		/*--------------------------------------------------------------*/
/* 		oldHandle = FldGetTextHandle(outField); */
/* 		FldSetTextHandle(outField, outHandle); */
/* 		if (oldHandle) */
/* 			MemHandleFree(oldHandle); */

		/*--------------------------------------------------------------*/
		/* Init other controls                                          */
		/*--------------------------------------------------------------*/
/* 		CtlSetLabel(ptrFromObjID(IDC_ST_SESSION), "label"); */
		if (running)
			disableButtons();
		enableCtls(true);
		
		FrmSetFocus(mainForm, FrmGetObjectIndex(mainForm, IDC_EF_INPUT));

		handled = true;
		break;
	}

	case frmCloseEvent:
		FldCompactText(inField); 
		FldSetTextHandle(inField, NULL);

		break;
	case ctlSelectEvent:
		printf("in MainFrameHandleEvent\n");
		switch (e->data.ctlSelect.controlID)
		{
		case IDC_PB_BREAK:
			/*----------------------------------------------------------*/
			/* Break excution                                           */
			/*----------------------------------------------------------*/
			enableButtons();
			{
				FormPtr frm;
				frm = FrmGetActiveForm();
				FrmSetFocus(frm,
				       FrmGetObjectIndex(frm, IDC_EF_INPUT));
			}
			handled = true;
			break;
		case IDC_PT_SYMS:
			fillBltinList();
			break;
		case IDC_PT_MODS:
			fillModuleList();
			break;
		case IDC_PB_EVAL:
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
				UInt16 index = FrmGetObjectIndex(frm, IDC_EF_INPUT);
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
		case IDM_EditClrIn:
			/*----------------------------------------------------------*/
			/* Clear input field                                        */
			/*----------------------------------------------------------*/
			FldDelete(inField, 0, FldGetTextLength(inField));
			handled = true;
			break;
			
		case IDM_EditClrOut:
			/*----------------------------------------------------------*/
			/* Clear output field                                       */
			/*----------------------------------------------------------*/
			ioClear();
/* 			FldDelete(outField, 0, FldGetTextLength(outField)); */
/* 			handled = true; */
			break;
		case IDM_HelpAbout:
			FrmAlert(IDA_ABOUT);
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
		case IDD_MainFrame:
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
	printf("in bogusEventHandler\n");
	printf("event type: %d\n", e->eType);
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

/*	_gdb_hook(); */

	switch (cmd)
	{
	case sysAppLaunchCmdNormalLaunch:
		cmdPBP = NULL;
		launchedExternally = false;
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
