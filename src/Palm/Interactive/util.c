/**********************************************************************/
/*                                                                    */
/* util.c:  LISPME utility functions needed everywhere                */
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

/**********************************************************************/
/* Includes                                                           */
/**********************************************************************/
#include "util.h"
#include "PythonGUI.h"

/**********************************************************************/
/* Global data                                                        */
/**********************************************************************/
char* errInfo;
Int16 expectedLen;

/**********************************************************************/
/* ptr from ID                                                        */
/**********************************************************************/
void* ptrFromObjID(UInt16 obj)
{
  return FrmGetObjectPtr(FrmGetActiveForm(),
           FrmGetObjectIndex(FrmGetActiveForm(),obj));
}

static Boolean grabbed = false;


/**********************************************************************/
/* Disable/enable command buttons during execution                    */
/**********************************************************************/
void enable(UInt16 id)
{
	CtlShowControl((ControlPtr)FrmGetObjectPtr(mainForm,
                 FrmGetObjectIndex(mainForm,id)));
}

void disable(UInt16 id)
{
  CtlHideControl((ControlPtr)FrmGetObjectPtr(mainForm,
                 FrmGetObjectIndex(mainForm,id)));
}

/**********************************************************************/
/* Disable/enable command buttons during execution                    */
/**********************************************************************/
void disableButtons(void)
{
/* 	disable(IDC_PB_IMPORT); */
/* 	disable(IDC_PB_MEMO); */
/* 	disable(IDC_PB_LOG); */
	disable(IDC_PB_EVAL);
	enable(IDC_PB_BREAK);
}

void enableButtons(void)
{
	disable(IDC_PB_BREAK);
/* 	enable(IDC_PB_IMPORT); */
/* 	enable(IDC_PB_MEMO); */
/* 	enable(IDC_PB_LOG); */
	enable(IDC_PB_EVAL);
}


/**********************************************************************/
/* Disable/enable main form controls for GUI                          */
/**********************************************************************/
void enableCtls(Boolean enable)
{
  FldSetUsable(outField, enable);
  FldSetUsable(inField, enable);
  CtlSetUsable((ControlPtr)
    FrmGetObjectPtr(mainForm,
                    FrmGetObjectIndex(mainForm,IDC_PT_SYMS)),
    enable);
  if (enable)
  {
    FrmEraseForm(mainForm);
    updateScrollBar();
    FrmUpdateForm(IDD_MainFrame, frmRedrawUpdateCode);
  }
  else
  {
    SclSetScrollBar(scrollBar, 0, 0, 0, 1);
    FldReleaseFocus(outField);
    FldReleaseFocus(inField);
  }
}

/**********************************************************************/
/* Update the scroll bar associated with a field                      */
/**********************************************************************/
void updateScrollBar(void)
{
  FieldPtr     field = ptrFromObjID(IDC_EF_OUTPUT);
  ScrollBarPtr sbar  = ptrFromObjID(IDC_SB_OUTPUT);
  UInt16       scrollPos;
  UInt16       textHeight;
  UInt16       fieldHeight;

  FldGetScrollValues(field, &scrollPos, &textHeight, &fieldHeight);
  if (textHeight > fieldHeight)
    SclSetScrollBar(sbar, scrollPos, 0,textHeight-fieldHeight, fieldHeight);
  else
    SclSetScrollBar(sbar, scrollPos,0, 0, fieldHeight);
}
