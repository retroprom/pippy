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
#include "ctype.h"
#include "palmeventmodule.h"

#include "GLib_interface.h"
#include "GLib_stub.h"

extern struct LibRef *GLib_PyLb_libref = NULL;
int initialize_FrozenModules();
int cleanup_FrozenModules();
extern struct _frozen _PyImport_FrozenModules[];
int run_SimpleString(char *command);
int mkmodule(name, code);

void python_init();
void python_finalize();

char  *stackLimit;
UInt16 startPanel;


MemHandle    outHandle = NULL;
MemHandle    inHandle;
FieldPtr     inField;
FieldPtr     outField;
FormPtr      mainForm;
ScrollBarPtr scrollBar;

Boolean running;
#define STACK_AVAIL 16384   /* need the actual stack specified in prefs000.bin */
#define MAX_LINE_LEN 32

/* static forward */
static Boolean StartApp();
static void StopApp(void);
static Boolean appHandleEvent(EventType* e);
static void fillMemoList();
static void freeMemoList();
static char *getMemo(int index);

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
		case LOAD_BUTTON:
			FrmPopupForm(LOADFRAME);
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
 		if (module_list && e->data.popSelect.controlID == MODULE_LIST_TRIGGER)
		{
			UInt16 sel = e->data.popSelect.selection;
			char*  item = LstGetSelectionText((ListPtr)e->data.popSelect.listP,sel);
			FldInsert(inField, item, StrLen(item));
		}
		if (bltins && e->data.popSelect.controlID == BUILTIN_LIST_TRIGGER)
		{
			UInt16 sel = e->data.popSelect.selection;
			char*  item = LstGetSelectionText((ListPtr)e->data.popSelect.listP,sel);
			FldInsert(inField, item, StrLen(item));
			if (sel < KEYWORD_FUNCTION_SPLIT_INDEX)
				FldInsert(inField, " ", 1);
			else
				FldInsert(inField, "(", 1);
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

static Boolean LoadFrameHandleEvent(EventType *e)
{
	Boolean handled = false;
	int lines;
	ListPtr srcList;
	char *name, *code;
	Word index;

   	SET_A4_FROM_A5;


	srcList = ptrFromObjID(MEMOLIST);
	switch (e->eType)
	{
		case frmOpenEvent:
			fillMemoList();
			FrmDrawForm(FrmGetActiveForm());
			handled = true;
			break;
		case frmCloseEvent:
			freeMemoList();
			handled = true;
			break;
		case keyDownEvent:
			lines = LstGetVisibleItems(srcList)-1;
			switch (e->data.keyDown.chr)
			{
				case pageUpChr:
				  LstScrollList(srcList, winUp, lines);
				  handled = true;
				  break;

				case pageDownChr:
				  LstScrollList(srcList, winDown, lines);
				  handled = true;
				  break;
			}
			break;
		case ctlSelectEvent:
		{
			UInt16 id  = e->data.ctlSelect.controlID;
			switch (id)
			{

				case LOAD_OK_BUTTON:
					/* load selected memo */
					index = LstGetSelection(srcList);
					
					if (index != -1)
					{
						int begin, end, status;
						code = getMemo(index);
						ErrFatalDisplayIf(!code,"seltxt is null!");

						handled = true;
						freeMemoList();
						FrmReturnToForm(MAINFRAME);
						begin = 0;
						while (begin<StrLen(code) && (code[begin]=='#' ||
							code[begin]==' ' || code[begin]=='\t')) begin++;
						end = begin;
						while (end<StrLen(code) && (isalnum((int)code[end]) 
								|| code[end]=='_'))	end++;
						name = MemPtrNew((end-begin)+1);
						strncpy(name, code+begin, end-begin);
						name[end-begin] = '\0';

						disableButtons();
						status = mkmodule(name, code);
						enableButtons();
						
						MemPtrFree(code);
						MemPtrFree(name);
						break;
				}

				/* Load was OK, now clean up dialog (=fall thru to cancel)    */
				case LOAD_CANCEL_BUTTON:
					freeMemoList();
					FrmReturnToForm(MAINFRAME);
					// Eval selection may have changed the input field, so
					// redraw 
					//FrmUpdateForm(MAINFRAME, 0); 
					handled = true;
					break; 
			}
		}
		default:
	}
	RESTORE_A4;
	return handled;
}

/**********************************************************************/
/* Globals dealing with source memo lists                             */
/**********************************************************************/
static char**     ppSrc;
static SourceRef* srcArr;
static UInt16     numSrc;

static char *getMemo(int index)
{
	static DmOpenRef  memoRef; 
	VoidHand recHand;
	char *recPtr;
	char *txt;
	UInt16     pyCat;

	memoRef = DmOpenDatabase(0, srcArr[index].dbId, dmModeReadWrite);
	pyCat  = CategoryFind(memoRef, "Python");
	recHand = DmQueryRecord(memoRef, srcArr[index].recNr);
	recPtr = MemHandleLock(recHand);

	txt = MemPtrNew(StrLen(recPtr)+2);
	strncpy(txt, recPtr, StrLen(recPtr));
	txt[StrLen(recPtr)] = '\n';
	txt[StrLen(recPtr)+1] = '\0';
	
	MemHandleUnlock(recHand);
	DmCloseDatabase(memoRef);
	return txt;
}

/**********************************************************************/
/* Fill list box with memos/memos32/DOCs                              */
/**********************************************************************/
static void fillMemoList()
{
	static DmOpenRef  memoRef; 
	static char* noCategory[] = {"*** Category 'Python' not found ***"};
	static char* noMemo[]     = {"*** No memo in 'Python' found ***"};
	ListPtr    srcList  = ptrFromObjID(MEMOLIST);
	LocalID    memoId;
	Int16      i;
	Boolean    listOK = true;
	UInt16     pyCat;
	MemHandle  recHand;
	UInt16     recId   = 0;
	char charEllipsis;
	UInt32	   ver;
	SystemPreferencesType sysp;
	UInt16 memoDBMode = dmModeReadWrite;

  /*--------------------------------------------------------------*/
  /* Search memo DB and set access mode acc. to secret pref.      */
  /* MemoDB (and, if found Memo32DB) are kept open during the     */
  /* entire runtime.                                              */
  /*--------------------------------------------------------------*/
	if (!sysp.hideSecretRecords)
		memoDBMode |= dmModeShowSecret;
	memoId   = DmFindDatabase(0, "MemoDB");
	ErrFatalDisplayIf(!memoId,"No MemoDB?");
	PrefGetPreferences(&sysp);
	memoRef = DmOpenDatabase(0, memoId, memoDBMode);
	ErrFatalDisplayIf(!memoRef,"Can't open MemoDB?");

	charEllipsis = ver >= 0x03100000 ? 0x18 : 0x85;

	pyCat  = CategoryFind(memoRef, "Python");
	if (pyCat == dmAllCategories) {
	  listOK = false;
	  LstSetListChoices(srcList, noCategory, 1);
	}
	else if ((numSrc = DmNumRecordsInCategory(memoRef, pyCat)) == 0) {
	  listOK = false;
	  LstSetListChoices(srcList, noMemo, 1);
	}
    else 
    {
      ppSrc  = MemPtrNew(numSrc * sizeof(char*));
      srcArr = MemPtrNew(numSrc * sizeof(SourceRef));
      for (i=0; i<numSrc; ++i)
      {
        char  *recPtr, *s, *d;
        recHand         = DmQueryNextInCategory(memoRef, &recId, pyCat);
        recPtr          = MemHandleLock(recHand);
        ppSrc[i]        = MemPtrNew(MAX_LINE_LEN+2);
        srcArr[i].dbId  = memoId;
        srcArr[i].recNr = recId;

        /*--------------------------------------------------------------*/
        /* Copy beginning of memo text upto first \n or MAX_LINE_LEN    */
        /*--------------------------------------------------------------*/
        for (s = recPtr, d = ppSrc[i];
             s-recPtr < MAX_LINE_LEN && *s && *s != '\n';
             ++s, ++d)
          *d = *s;
        if (s-recPtr == MAX_LINE_LEN)
          *d++ = charEllipsis;
        *d = '\0';
        MemHandleUnlock(recHand);
        recId++;
      }
      LstSetListChoices(srcList, ppSrc, numSrc);
    } // memos found 
	DmCloseDatabase(memoRef);
}

/**********************************************************************/
/* Deallocate memo list structures                                    */
/**********************************************************************/
static void freeMemoList(void)
{
  Int16 i;
  for (i=0; i<numSrc; ++i)
    MemPtrFree(ppSrc[i]);
	if (numSrc) {
		MemPtrFree(ppSrc);
		MemPtrFree(srcArr);
		numSrc = 0;
	}
	numSrc = 0;
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
		case LOADFRAME:
		  	FrmSetEventHandler(form, LoadFrameHandleEvent);
			//FrmSetEventHandler(form, MainFrameHandleEvent);
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
