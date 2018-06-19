#include <windows.h>
#include <commdlg.h>
#include <cstdlib>
#include<string>
using std::string;
#include<memory>
using std::shared_ptr;
#include<fstream>
using std::ofstream;
#include<sstream>
using std::ostringstream;
#include "poppad.h"
#include"PopFile.h"
#include"PopFind.h"
#include"PopFont.h"
#include"PopPRNT0.h"
#include"CompPad.h"
#include"Parser.h"
#include"Lexer.h"
#include "vld.h"
#define EDITID 1
#define UNTITLED "(untitled)"

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

namespace
{
	char szAppName[] = "CompPad";
	HWND hDlgModeless;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
PSTR szCmdLine, int iCmdShow)
{
MSG msg;
HWND hwnd;
HACCEL hAccel;
WNDCLASSEX wndclass;
wndclass.cbSize = sizeof(wndclass);
wndclass.style = CS_HREDRAW | CS_VREDRAW;
wndclass.lpfnWndProc = WndProc;
wndclass.cbClsExtra = 0;
wndclass.cbWndExtra = 0;
wndclass.hInstance = hInstance;
wndclass.hIcon = LoadIcon(hInstance, szAppName);
wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
wndclass.hbrBackground =(HBRUSH) GetStockObject(WHITE_BRUSH);
wndclass.lpszMenuName = szAppName;
wndclass.lpszClassName = szAppName;
wndclass.hIconSm = LoadIcon(hInstance, szAppName);
RegisterClassEx(&wndclass);

hwnd = CreateWindow(szAppName, NULL,
WS_OVERLAPPEDWINDOW,
CW_USEDEFAULT, CW_USEDEFAULT,
CW_USEDEFAULT, CW_USEDEFAULT,
NULL, NULL, hInstance, szCmdLine);
ShowWindow(hwnd, iCmdShow);
UpdateWindow(hwnd);
hAccel = LoadAccelerators(hInstance, szAppName);
while(GetMessage(&msg, NULL, 0, 0))
{
if(hDlgModeless == NULL || !IsDialogMessage(hDlgModeless, &msg))
{
if(!TranslateAccelerator(hwnd, hAccel, &msg))
{
TranslateMessage(&msg);
DispatchMessage(&msg);
}
}
}
return msg.wParam;
}

void DoCaption(HWND hwnd, const char *szTitleName)
{
	ostringstream outstr;
	outstr << szAppName << " - " << (szTitleName[0] ? szTitleName : UNTITLED);
	string szCaption = outstr.str();
SetWindowText(hwnd, szCaption.c_str());
}

void OkMessage(HWND hwnd, char *szMessage, const char *szTitleName)
{
	ostringstream outstr;
	outstr << szMessage << " " << (szTitleName[0] ? szTitleName : UNTITLED);
	string szBuffer = outstr.str();
	MessageBox(hwnd, szBuffer.c_str(), szAppName, MB_OK | MB_ICONEXCLAMATION);
}

short AskAboutSave(HWND hwnd, const char *szTitleName)
{
	ostringstream outstr;
	outstr << "Save current changes in " << (szTitleName[0] ? szTitleName : UNTITLED) << "?";
	string szBuffer = outstr.str();
	int iReturn = MessageBox(hwnd, szBuffer.c_str(), szAppName,
		MB_YESNOCANCEL | MB_ICONQUESTION);
if(iReturn == IDYES)
if(!SendMessage(hwnd, WM_COMMAND, IDM_SAVE, 0L))
iReturn = IDCANCEL;
return iReturn;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
static BOOL bNeedSave = FALSE;
static char szFileName[_MAX_PATH];
static char szTitleName[_MAX_FNAME + _MAX_EXT];
static HINSTANCE hInst;
static HWND hwndEdit, hCode;
static int iOffset;
static UINT iMsgFindReplace;
int iSelBeg, iSelEnd, iEnable;
LPFINDREPLACE pfr;
switch(iMsg)
{
case WM_CREATE :
hInst =((LPCREATESTRUCT) lParam) -> hInstance;
// Create the edit control child window
hwndEdit = CreateWindow("edit", NULL,
WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
WS_BORDER | ES_LEFT | ES_MULTILINE |
ES_NOHIDESEL | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
0, 0, 0, 0,
hwnd,(HMENU) EDITID, hInst, NULL);
SendMessage(hwndEdit, EM_LIMITTEXT, 32000, 0L);
hCode = CreateWindow("edit", NULL,
	WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
	WS_BORDER | ES_LEFT | ES_MULTILINE |
	ES_NOHIDESEL | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
	0, 0, 0, 0,
	hwnd, (HMENU)IDD_CODE, hInst, NULL);

// Initialize common dialog box stuff
PopFileInitialize(hwnd);
PopFontInitialize(hwndEdit);
iMsgFindReplace = RegisterWindowMessage(FINDMSGSTRING);
// Process command line
lstrcpy(szFileName,(PSTR)
(((LPCREATESTRUCT) lParam)->lpCreateParams));
if(strlen(szFileName) > 0)
{
GetFileTitle(szFileName, szTitleName,
sizeof(szTitleName));
if(!PopFileRead(hwndEdit, szFileName))
OkMessage(hwnd, "File cannot be read!",
szTitleName);
}
DoCaption(hwnd, szTitleName);
return 0;
case WM_SETFOCUS :
SetFocus(hwndEdit);
return 0;
case WM_SIZE :
MoveWindow(hwndEdit, 0, 0, LOWORD(lParam)/ 2,
HIWORD(lParam), TRUE);
MoveWindow(hCode, LOWORD(lParam) / 2, 0, LOWORD(lParam),
	HIWORD(lParam), TRUE);
return 0;
case WM_INITMENUPOPUP :
switch(lParam)
{
case 0 :
	EnableMenuItem((HMENU) wParam, IDM_SAVE, (bNeedSave ? MF_ENABLED : MF_GRAYED));		
	EnableMenuItem((HMENU) wParam, IDM_SAVEAS, (bNeedSave ? MF_ENABLED : MF_GRAYED));
	break;
case 1 : // Edit menu
// Enable Undo if edit control can do it
EnableMenuItem((HMENU) wParam, IDM_UNDO,
SendMessage(hwndEdit, EM_CANUNDO, 0, 0L) ?
MF_ENABLED : MF_GRAYED);
// Enable Paste if text is in the clipboard
EnableMenuItem((HMENU) wParam, IDM_PASTE,
IsClipboardFormatAvailable(CF_TEXT) ?
MF_ENABLED : MF_GRAYED);
// Enable Cut, Copy, and Del if text is selected
SendMessage(hwndEdit, EM_GETSEL,(WPARAM) &iSelBeg,
(LPARAM) &iSelEnd);
iEnable = iSelBeg != iSelEnd ? MF_ENABLED : MF_GRAYED;
EnableMenuItem((HMENU) wParam, IDM_CUT, iEnable);
EnableMenuItem((HMENU) wParam, IDM_COPY, iEnable);
EnableMenuItem((HMENU) wParam, IDM_CLEAR, iEnable);
break;
case 2 : // Search menu
// Enable Find, Next, and Replace if modeless
// dialogs are not already active
iEnable = hDlgModeless == NULL ?
MF_ENABLED : MF_GRAYED;
EnableMenuItem((HMENU) wParam, IDM_FIND, iEnable);
EnableMenuItem((HMENU) wParam, IDM_NEXT, iEnable);
EnableMenuItem((HMENU) wParam, IDM_REPLACE, iEnable);
break;
}
return 0;
case WM_COMMAND :
// Messages from edit control
if(lParam && LOWORD(wParam) == EDITID)
{
switch(HIWORD(wParam))
{
case EN_UPDATE :
bNeedSave = TRUE;
return 0;
case EN_ERRSPACE :
case EN_MAXTEXT :
MessageBox(hwnd, "Edit control out of space.",
szAppName, MB_OK | MB_ICONSTOP);
return 0;
}
break;
}
switch (LOWORD(wParam))
{
	// Messages from File menu
case IDM_NEW:
	if (bNeedSave && IDCANCEL ==
		AskAboutSave(hwnd, szTitleName))
		return 0;
	SetWindowText(hwndEdit, "\0");
	SetWindowText(hCode, "\0");
	szFileName[0] = '\0';
	szTitleName[0] = '\0';
	DoCaption(hwnd, szTitleName);
	bNeedSave = FALSE;
	return 0;
case IDM_OPEN:
	if (bNeedSave && IDCANCEL ==
		AskAboutSave(hwnd, szTitleName))
		return 0;
	if (PopFileOpenDlg(hwnd, szFileName, szTitleName))
	{
		if (!PopFileRead(hwndEdit, szFileName))
		{
			OkMessage(hwnd, "Could not read file!",
				szTitleName);
			szFileName[0] = '\0';
			szTitleName[0] = '\0';
		}
	}
	DoCaption(hwnd, szTitleName);
	SetWindowText(hCode, "\0");
	bNeedSave = TRUE;
	return 0;
case IDM_SAVE:
	if (szFileName[0])
	{
		if (PopFileWrite(hwndEdit, szFileName))
		{
			bNeedSave = FALSE;
			return 1;
		}
		else
			OkMessage(hwnd, "Could not write file",
				szTitleName);
		return 0;
	}
	// fall through
case IDM_SAVEAS:
	if (PopFileSaveDlg(hwnd, szFileName, szTitleName))
	{
		DoCaption(hwnd, szTitleName);
		if (PopFileWrite(hwndEdit, szFileName))
		{
			bNeedSave = FALSE;
			return 1;
		}
		else
			OkMessage(hwnd, "Could not write file",
				szTitleName);
	}
	return 0;
case IDM_PRINT:
	if (!PopPrntPrintFile(hInst, hwnd, hwndEdit,
		szTitleName))
		OkMessage(hwnd, "Could not print file",
			szTitleName);
	return 0;
case IDM_EXIT:
	SendMessage(hwnd, WM_CLOSE, 0, 0);
	return 0;
	// Messages from Edit menu
case IDM_UNDO:
	SendMessage(hwndEdit, WM_UNDO, 0, 0);
	return 0;
case IDM_CUT:
	SendMessage(hwndEdit, WM_CUT, 0, 0);
	return 0;
case IDM_COPY:
	SendMessage(hwndEdit, WM_COPY, 0, 0);
	return 0;
case IDM_PASTE:
	SendMessage(hwndEdit, WM_PASTE, 0, 0);
	return 0;
case IDM_CLEAR:
	SendMessage(hwndEdit, WM_CLEAR, 0, 0);
	return 0;
case IDM_SELALL:
	SendMessage(hwndEdit, EM_SETSEL, 0, -1);
	return 0;
	// Messages from Search menu
case IDM_FIND:
	SendMessage(hwndEdit, EM_GETSEL, NULL,
		(LPARAM)&iOffset);
	hDlgModeless = PopFindFindDlg(hwnd);
	return 0;
case IDM_NEXT:
	SendMessage(hwndEdit, EM_GETSEL, NULL,
		(LPARAM)&iOffset);
	if (PopFindValidFind())
		PopFindNextText(hwndEdit, &iOffset);
	else
		hDlgModeless = PopFindFindDlg(hwnd);
	return 0;
case IDM_REPLACE:
	SendMessage(hwndEdit, EM_GETSEL, NULL,
		(LPARAM)&iOffset);
	hDlgModeless = PopFindReplaceDlg(hwnd);
	return 0;
case IDM_FONT:
	if (PopFontChooseFont(hwnd))
		PopFontSetFont(hwndEdit);
	return 0;
	// Messages from Help menu
case IDM_HELP:
	OkMessage(hwnd, "Help not yet implemented!", "\0");
	return 0;
case IDM_ABOUT:
	DialogBox(hInst, "AboutBox", hwnd, AboutDlgProc);
	return 0;
	//start of compiler work
case IDM_COMP:
	ofstream Code("code.txt", std::ios::out);
	string yourText;
	yourText = writeInString(hwndEdit);
	shared_ptr<Test>test = std::make_shared<Test>(yourText);
	shared_ptr<Lexer>lexer = std::make_shared<Lexer>(test);
	shared_ptr<Parser>parse = std::make_shared<Parser>(lexer, hwnd);
	parse->program();
	yourText.clear();
	parse.reset();
	if (!PopFileRead(hCode, "code.txt"))
	{
		OkMessage(hCode, "Could not read file!",
			"code.txt");                                              
	}
	return 0;
}
break;
case WM_CLOSE :
if(!bNeedSave || IDCANCEL != AskAboutSave(hwnd, szTitleName))
DestroyWindow(hwnd);
return 0;
case WM_QUERYENDSESSION :
if(!bNeedSave || IDCANCEL != AskAboutSave(hwnd, szTitleName))
return 1;
return 0;
case WM_DESTROY :
PopFontDeinitialize();
PostQuitMessage(0);
return 0;
default:
// Process "Find-Replace" iMsgs
if(iMsg == iMsgFindReplace)
{
pfr =(LPFINDREPLACE) lParam;
if(pfr->Flags & FR_DIALOGTERM)
hDlgModeless = NULL;
if(pfr->Flags & FR_FINDNEXT)
if(!PopFindFindText(hwndEdit, &iOffset, pfr))
OkMessage(hwnd, "Text not found!", "\0");
if(pfr->Flags & FR_REPLACE ||
pfr->Flags & FR_REPLACEALL)
if(!PopFindReplaceText(hwndEdit, &iOffset, pfr))
OkMessage(hwnd, "Text not found!", "\0");
if(pfr->Flags & FR_REPLACEALL)
while(PopFindReplaceText(hwndEdit, &iOffset, pfr));
return 0;
}
break;
}
return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
switch(iMsg)
{
case WM_INITDIALOG :
return TRUE;

	
case WM_COMMAND :
switch(LOWORD(wParam))
{
case IDOK :
EndDialog(hDlg, 0);
return TRUE;
}
break;
}
return FALSE;
}

