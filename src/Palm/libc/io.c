/**********************************************************************/
/*                                                                    */
/* io.c: input/output subsystem                                       */
/*                                                                    */
/* Jeffery D. Collins                                                 */
/* (c) Endeavors Technology, Inc.                                     */
/*                                                                    */
/**********************************************************************/

#include "io.h"
#include "PalmCompatibility.h"
#include "set_a4.h"
#include "GLib_interface.h"

static void *io_GetObjectPtr(UInt16 objectID) SEG_LIBC;
static void  io_FormInit(FormPtr frm) SEG_LIBC;
static void  io_FormClose(FormPtr frm) SEG_LIBC;
static void  io_FormUpdateScroller(FormPtr frm) SEG_LIBC;
static void  io_FieldScroll(Int16 linesToScroll) SEG_LIBC;
static void  io_FieldPageScroll(WinDirectionType direction) SEG_LIBC;

static MemHandle gTextH = NULL;
static UInt16 gFormID = 0;
static UInt16 gFieldID = 0;
static UInt16 gScrollerID = 0;
static UInt16 gMaxChars = 0;


Err 
ioInit(UInt16 formID, UInt16 fieldID, UInt16 scrollerID, UInt16 maxChars)
{
	gFormID = formID;
	gFieldID = fieldID;
	gScrollerID = scrollerID;
	gMaxChars = maxChars;

	return 0;
}

static void *
io_GetObjectPtr(UInt16 objectID)
{
	FormPtr	frm;
	void *obj;
	
	frm = FrmGetActiveForm();
	obj = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, objectID));

	return obj;
}

static MemHandle
initBuffer(UInt16 size) {
	Char *textP;
	MemHandle textH;

	textH = MemHandleNew(size);
	if (textH == NULL)
		return NULL;
	textP = MemHandleLock(textH);
	MemSet(textP, size, '\0');
	MemPtrUnlock(textP);

	return textH;
}

static UInt16 
pushToBuffer(MemHandle textH, Char *s)
{
	Char *textP, *str=s;
	UInt16 res, lenstr, textLen;
	
	if ((lenstr=StrLen(str)) > gMaxChars) {
	  str += lenstr - gMaxChars + 1;
	  lenstr = StrLen(str);
	}

	textP = MemHandleLock(textH);

	if ((textLen = StrLen(textP)) + lenstr >= gMaxChars) {
		Char *p;
		UInt16 factor = max(gMaxChars/4, lenstr), len;
			
		/* Cut off the top */
		p = textP + factor;
		len = StrLen(p);

		/* Move the text up */
		MemMove(textP, p, len);
		MemSet(textP + len, factor, '\0');
		textLen = StrLen(textP);
	}

	/* Add the new text */
	StrCat(textP, str);
	res = textLen + lenstr;
	MemPtrUnlock(textP);
	
	return res;
}

static void 
io_FormInit(FormPtr frm)
{
	FieldAttrType attr;
	FieldPtr fldP;
	Int16 obj_index; /* for debugging purposes */
	
	obj_index = FrmGetObjectIndex(frm, gFieldID);
	fldP = FrmGetObjectPtr(frm, obj_index);
	if (!gTextH)
		gTextH = initBuffer(gMaxChars);
	
	FldGetAttributes(fldP, &attr);
	attr.hasScrollBar = true;
	FldSetAttributes(fldP, &attr);
	
	FldSetTextHandle(fldP, gTextH);
	
	gTextH = NULL;
	
}

static void 
io_FormClose(FormPtr frm)
{
	FieldPtr fldP;
	Int16 obj_index; /* for debugging purposes */
	
	obj_index = FrmGetObjectIndex(frm, gFieldID);
	fldP = FrmGetObjectPtr(frm, obj_index);
	
	if (gTextH) MemHandleFree(gTextH);
	gTextH = FldGetTextHandle(fldP);
	
	FldSetTextHandle(fldP, NULL);
}

static void 
io_FormUpdateScroller(FormPtr frm)
{
	UInt16 scrollPos, textHeight, fieldHeight;
	Int16 maxValue;
	FieldPtr fld;
	ScrollBarPtr bar;
	
	fld = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, gFieldID));
	bar = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, gScrollerID));
	
	FldGetScrollValues(fld, &scrollPos, &textHeight,  &fieldHeight);
	
	if (textHeight > fieldHeight)
		maxValue = textHeight - fieldHeight;
	else
		maxValue = 0;
	
	SclSetScrollBar (bar, scrollPos, 0, maxValue, fieldHeight-1);
}

static void 
io_FieldScroll(Int16 linesToScroll)
{

	UInt16 blankLines;
	Int16 min, max, value, pageSize;
	FieldPtr fld;
	ScrollBarPtr bar;
	
	fld = io_GetObjectPtr(gFieldID);

	if (linesToScroll < 0) {
		blankLines = FldGetNumberOfBlankLines(fld);
		FldScrollField(fld, -linesToScroll, winUp);
		
		if (blankLines) {
			bar = io_GetObjectPtr(gScrollerID);
			SclGetScrollBar(bar, &value, &min, &max, &pageSize);
			if (blankLines > -linesToScroll)
				max += linesToScroll;
			else
				max -= blankLines;
			SclSetScrollBar(bar, value, min, max, pageSize);
		}
	}
	
	else if (linesToScroll > 0)
		FldScrollField(fld, linesToScroll, winDown);
	
}


void ioPutS(Char *s)
{	
	FieldPtr fldP;
	UInt16 pos;
	MemHandle textH;
	Char *str = s;

	/* If the incoming string is larger than the buffer size,
	   then grab the tail portion of the string plus some extra */

	if (FrmGetActiveFormID() == gFormID) {
		fldP =(FieldPtr)io_GetObjectPtr(gFieldID);

		textH = FldGetTextHandle(fldP);

		pos = pushToBuffer(textH, str);

		FldSetTextHandle(fldP, textH);
		FldSetScrollPosition(fldP, pos);
		FldDrawField(fldP);
		io_FormUpdateScroller(FrmGetActiveForm());
	}
	else  {	
		if (!gTextH)
			gTextH = initBuffer(gMaxChars);
		pushToBuffer(gTextH, str);
	}
}

void ioClear(void)
{
	FieldPtr fldP;

	if (FrmGetActiveFormID() == gFormID) {
		fldP =(FieldPtr)io_GetObjectPtr(gFieldID);
		FldFreeMemory(fldP);  /* this should free the memory in gTextH */
		FldEraseField(fldP);
		FldDrawField(fldP);
		io_FormUpdateScroller(FrmGetActiveForm());

		gTextH = initBuffer(gMaxChars);
		FldSetTextHandle(fldP, gTextH);
		gTextH = NULL;
	}
	else {
		if (gTextH) MemHandleFree(gTextH);
		gTextH = NULL;
		}
}

static void io_FieldPageScroll(WinDirectionType direction)
{
	Int16 value, min, max, pageSize;
	UInt16 linesToScroll;
	FieldPtr fld;
	ScrollBarPtr bar;

	fld = io_GetObjectPtr(gFieldID);
	
	if (FldScrollable(fld, direction)) {
		linesToScroll = FldGetVisibleLines(fld) - 1;
		FldScrollField(fld, linesToScroll, direction);
		
		bar = io_GetObjectPtr(gScrollerID);
		SclGetScrollBar(bar, &value, &min, &max, &pageSize);
		
		if (direction == winUp)
			value -= linesToScroll;
		else
			value += linesToScroll;
		
		SclSetScrollBar(bar, value, min, max, pageSize);
		return;
	}
}


Boolean ioHandleEvent(EventPtr event)
{
	FormPtr frmP;
	Boolean handled;
	
	SET_A4_FOR_GLIB('PyLb');

	handled = false;

	switch (event->eType) {
	case keyDownEvent:
		if (event->data.keyDown.chr == pageUpChr) {
			io_FieldPageScroll(winUp);
			handled = true;
		}
		else if (event->data.keyDown.chr == pageDownChr) {
			io_FieldPageScroll(winDown);
			handled = true;
		}
		break;
	case sclRepeatEvent:
		io_FieldScroll(event->data.sclRepeat.newValue - 
				event->data.sclRepeat.value);
		break;
	case frmUpdateEvent:
		io_FormUpdateScroller(FrmGetActiveForm());
		break;
	case fldChangedEvent:
		io_FormUpdateScroller(FrmGetActiveForm());
		handled = true;
		break;
	case frmOpenEvent:
		frmP = FrmGetActiveForm();
		io_FormInit(frmP);
		io_FormUpdateScroller(frmP);
		break;
	case frmCloseEvent:
		frmP = FrmGetActiveForm();
		io_FormClose(frmP);
		break;
	default:
		break;
	}

	RESTORE_A4;
	return handled;
}
