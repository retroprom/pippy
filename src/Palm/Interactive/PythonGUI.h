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

#ifndef _PYTHONGUI_H_
#define _PYTHONGUI_H_ 1

#define MENU_HelpAbout        1001
#define MENU_ClearInput       1002
#define MENU_ClearOutput      1003
#define MENU_RedrawScreen     1004

#define MAINFRAME             1010

#define OUTPUT_SCROLLBAR      1020
#define OUTPUT_FIELD          1030
#define INPUT_FIELD           1040

#define LOAD_BUTTON           1050
#define EVAL_BUTTON           1060
#define BREAK_BUTTON          1070

#define BUILTIN_LIST_TRIGGER  1080
#define BUILTIN_LIST          1090
#define BUILTIN_LIST_ITEMS    1100

#define MODULE_LIST           1110
#define MODULE_LIST_ITEMS     1120
#define MODULE_LIST_TRIGGER   1130

#define OS_VERSION_ERROR      1140

#define ABOUT                 1150

#define LOADFRAME             1160
#define MEMOLIST              1170
#define LOAD_OK_BUTTON        1180
#define LOAD_CANCEL_BUTTON    1190


#define OUTPUT_SIZE           1024
#define INPUT_SIZE            4096
#define READ_SIZE               64

#define KEYWORD_FUNCTION_SPLIT_INDEX 28

#endif /* _PYTHONGUI_H_ */
