/**********************************************************************/
/*                                                                    */
/* PythonGUI.h: Resource definitions for Python                       */
/*                                                                    */
/* Jeffery D. Collins                                                 */
/* Endeavors Technology, Inc.                                         */
/*                                                                    */
/* Portions (c) FBI Fred Bayer Informatics                            */
/*                                                                    */
/* Distributed under the GNU General Public License;                  */
/* see the README file. This code comes with NO WARRANTY.             */
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
	disable(EVAL_BUTTON);
	enable(BREAK_BUTTON);
}

void enableButtons(void)
{
	disable(BREAK_BUTTON);
	enable(EVAL_BUTTON);
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
                    FrmGetObjectIndex(mainForm,BUILTIN_LIST_TRIGGER)),
    enable);
  if (enable)
  {
    FrmEraseForm(mainForm);
    updateScrollBar();
    FrmUpdateForm(MAINFRAME, frmRedrawUpdateCode);
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
  FieldPtr     field = ptrFromObjID(OUTPUT_FIELD);
  ScrollBarPtr sbar  = ptrFromObjID(OUTPUT_SCROLLBAR);
  UInt16       scrollPos;
  UInt16       textHeight;
  UInt16       fieldHeight;

  FldGetScrollValues(field, &scrollPos, &textHeight, &fieldHeight);
  if (textHeight > fieldHeight)
    SclSetScrollBar(sbar, scrollPos, 0,textHeight-fieldHeight, fieldHeight);
  else
    SclSetScrollBar(sbar, scrollPos,0, 0, fieldHeight);
}
