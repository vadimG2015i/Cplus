// Test.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Test.h"
#include "shellapi.h"
#include <Psapi.h>
#include <atlstr.h>
#include <Commdlg.h>
#pragma comment(lib, "psapi.lib")
// Gestures
#define GPRS_CUT				1	
#define GPRS_COPY			2
#define GPRS_PASTE			3
#define GPRS_BACKSPACE		4
#define GPRS_UNDO			5
#define GPRS_CASE			6
#define GPRS_CORRECTION		7
#define GPRS_BOLD			8
#define GPRS_UNDERLINE		9
#define GPRS_ITALICIZE		10
#define GPRS_LEFTJUSTIFY		11			
#define	GPRS_CENTREJUSTIFY	12		
#define	GPRS_RIGHTJUSTIFY	13

// Punctuation
#define	GPRS_SPACE			14
#define	GPRS_TAB				15
#define	GPRS_RETURN			16
#define GPRS_PERIOD			17
#define GPRS_COMMA			18
#define GPRS_SQUOTE			19
#define GPRS_DQUOTE			20
#define GPRS_APOS			21
#define GPRS_UNDERSCORE		22
#define GPRS_COLON			23
#define GPRS_SEMICOLON		24
#define GPRS_PERCENT         25
#define GPRS_QUESTION		26
#define GPRS_XMARK			27
#define GPRS_LPARENTHESIS	28
#define GPRS_RPARENTHESIS	29
#define GPRS_LESSTHAN		30
#define GPRS_GREATERTHAN		31
#define GPRS_LBRACKET		32
#define GPRS_RBRACKET		33
#define GPRS_LBRACE			34
#define GPRS_RBRACE			35
#define GPRS_BSLASH			36
#define GPRS_FSLASH			37
#define GPRS_TILDE			38
#define GPRS_AT				39
#define GPRS_NUM				40
#define GPRS_DOLLAR			41
#define GPRS_AMPERSAND		42
#define GPRS_PLUS			43
#define GPRS_CARET			44
#define GPRS_PIPE			45
#define GPRS_ASTERISK		46
#define GPRS_HYPHEN			47
#define GPRS_EQUALS			48


#define NOC_REQUEST  1
#define NOC_RESPONSE 2
#define NOC_REOPEN  3
#define NOC_ACTION   4
#define NOC_RESTORE  5
#define NOC_TIMEOUT  6
#define NOC_ENABLENIS 7
#define NOC_DISABLENIS 8
#define NOC_SAVEDOC   9
#define NOC_STOPTIMER   10

#define NOC_ACTION_SINLCLK  0
#define NOC_ACTION_SELTEXT  1
#define NOC_ACTION_DRAWBGW  2
#define NOC_ACTION_DRAWBPC  3
#define NOC_ACTION_DISP_SYM 4
#define NOC_ACTION_TXT_INS  5
#define NOC_TIMEOUT_HW_EXP  6  //Handwriting Expired  
#define NOC_TIMEOUT_HW_PRE  7  // Handwriting Pre-empted  
#define NOC_TIMEOUT_PC_EXP  8  // Punctuation Expired
#define NOC_TIMEOUT_PC_PRE  9  //Punctuation Pre-empted
#define NOC_TIMEOUT_NO     10 //  No Timeout
#define NOC_ACTION_SEND_CHAR  11
#define NOC_ACTION_SEND_GEST  12
#define NOC_ACTION_SEND_SYM   13
#define NOC_TIMEOUT_HW_ACT  14  // Handwriting Activated  
#define NOC_TIMEOUT_PC_ACT  15  // Punctuation  Acticvated 

#define WM_SENDNIS WM_USER +120
#define WM_SENDNOC_GEST WM_USER +121
#define WM_ENABLENIS	WM_USER + 388
#define WM_DISABLENIS	WM_USER + 389




typedef struct tagNISDATA
{
   

	DWORD Ydiff;				 // The difference between two Y coordinate values.
	DWORD Ybottom;				 // The Y coordinate location of the bottom of the Document Window.
	DWORD Xtip, Ytip;			 // The X and Y coordinate locations of the Text Insertion Point.
	DWORD Xbpc, Ybpc;			 // The X and Y coordinate locations of the Text Insertion Point
	DWORD Xgd , Ygd ;			 // The X and Y coordinate locations of the Gesture Diagram
	DWORD Xoffset, Yoffset;		 // The X and Y values for the offset of the Gesture Diagram.
	DWORD Xstart , Ystart;		 // The X,Y coordinate location of the starting position of the selected block of text.
	DWORD Xend, Yend;			 //	The X,Y coordinate location of the end position of the selected block of text.
	DWORD Xbgw , Ybgw ;			 // The X,Y coordinate location of the end position of the selected block of text
	DWORD SelStart,SelEnd;		 // The Start,End position  selected text
	DWORD Xpenup;				 // The X coordinate location of the Pen Up Event.
	DWORD Ypenup;				 // The Y coordinate location of the Pen Up Event.
	BOOL Display_Diagram ;		 // Indicates if the diagram is to be displayed (True) or NOT displayed (False).
	char Diagram_Name[40];		 // Contains the name of the gesture.
	char Action_Text_Box[40];	 //  Contains the contents to be displayed in the Action Text Box.
	char Timeout_Text_Box[40];   //Contains the contents to be displayed in the Timeout Text Box.
	DWORD NOC_Mode ;			 //Contains the mode of Natural Input Software (Selection, BPC Edit or BGW Edit).
	char Output[512];			     // The results of Microsoft’s recognition engine.

	DWORD Available_Height_for_Display ;//The available height for display as defined as the difference 
	//between Ybottom and Ytip OR the difference between Ybottom and Yend.

	DWORD Min_Height_for_Display;//The minimum height needed to fully display the Black Punctuation
	//Circle OR the Blue Gesture Window and/or displayed diagram.

	DWORD Height_of_Circle_or_Square;//The height of the Black Punctuation Circle or Blue Gesture Window.
	DWORD Height_of_Diagram ;//The height of the diagram to be displayed (determine from file properties).




} NISDATA;
COPYDATASTRUCT NISCDS;
NISDATA  NisMsg;
NISDATA  storeNisMsg;
PCOPYDATASTRUCT pNISCDS;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND hWndEditAction;
HWND hWndEditTime;
HWND hWndEnable;
HWND hWndTest;
HANDLE _word_handle;
HWND word_hwnd;
DWORD  processId = 0;
BOOL   m_Disabled=FALSE;
BOOL   m_DialogOk=FALSE;
int XOFF=0;
int YOFF=0;
WCHAR  NameFileDiagram[512];

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
HWND                GetAcitveWordWindowHandle();
BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam); 
BOOL				SendMessageToNis(HWND, int);
BOOL				RecvMessageFromNis(LPARAM);
BOOL				OpenWordDoc(CString NameWordDoc);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
SHELLEXECUTEINFO SHExecInfo;


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL; //LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;// MAKEINTRESOURCE(IDC_TEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;/// LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd,hWndDialog;

   hInst = hInstance; // Store ,instance handle in our global variable
   

   hWnd = CreateWindowEx(WS_EX_DLGMODALFRAME|WS_EX_TOPMOST, szWindowClass, szTitle, WS_SYSMENU,
	   CW_USEDEFAULT, CW_USEDEFAULT, 600, 100, NULL, NULL, hInstance, 0);

   HMENU hMenu = GetSystemMenu(hWnd, FALSE);
   DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
   DeleteMenu(hMenu, SC_SIZE, MF_BYCOMMAND);
   DeleteMenu(hMenu, SC_MOVE, MF_BYCOMMAND);
   DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
   DeleteMenu(hMenu, SC_RESTORE, MF_BYCOMMAND);
   RECT rc;
   GetWindowRect(hWnd, &rc);
   int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
   int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;
   SetWindowPos(hWnd, 0, xPos, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
   SendMessage(hWnd, WM_SETICON, 0, 0);

   if (!hWnd)
   {
      return FALSE;
   }
   hWndTest=hWnd;
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	WCHAR exe_path[2048] = {};
	ZeroMemory(&SHExecInfo, sizeof(SHExecInfo));
	TCHAR curDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, curDir);
	CString fullpathname;
	
    HWND hWndDialog;
	SHExecInfo.cbSize = sizeof(SHExecInfo);
	SHExecInfo.hwnd = hWnd;
    SHExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	SHExecInfo.nShow = SW_SHOWNORMAL;
	SHExecInfo.lpFile = _T("Sample_Document1.doc");
	SHExecInfo.lpDirectory = curDir;
	fullpathname.Append(curDir);
	fullpathname.Append(_T("\\"));
	fullpathname.Append( _T("Sample_Document1.doc"));
	
	switch (message)
	{
	case WM_CREATE:
	{
				   	  DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		 			  SetFileAttributes( fullpathname,  
                      GetFileAttributes(fullpathname) & ~FILE_ATTRIBUTE_READONLY);
					  SetCurrentDirectory(curDir);
					
					  if (!ShellExecuteEx(&SHExecInfo)) {
						  //error
					  }
					 
					

					
					  CreateWindow(TEXT("BUTTON"), TEXT("Reset"),
						  WS_VISIBLE | WS_BORDER | WS_CHILD | BS_FLAT,
						  5, 8, 100, 25,
						  hWnd, (HMENU)TB_REOPEN, NULL, NULL
						  );


					   CreateWindow(TEXT("BUTTON"), TEXT("Stop"),
						  WS_VISIBLE | WS_BORDER | WS_CHILD | BS_FLAT,
						  5, 38, 100, 25,
						  hWnd, (HMENU)TB_STOP, NULL, NULL
						  );

					  hWndEnable= CreateWindow(TEXT("BUTTON"), TEXT("Disable"),
						  WS_VISIBLE | WS_BORDER | WS_CHILD | BS_FLAT,
						  110, 38, 100, 25,
						  hWnd, (HMENU)TB_ENABLE, NULL, NULL
						  );
					   CreateWindow(TEXT("BUTTON"), TEXT("Save"),
						  WS_VISIBLE | WS_BORDER | WS_CHILD | BS_FLAT,
						  215, 38, 100, 25,
						  hWnd, (HMENU)TB_SAVE, NULL, NULL
						  );



					  CreateWindow(TEXT("BUTTON"), TEXT("Snapshot"),
						  WS_VISIBLE | WS_BORDER | WS_CHILD | BS_FLAT,
						  110, 8, 100, 25,
						  hWnd, (HMENU)TB_TEST2, NULL, NULL
						  );
					  CreateWindow(TEXT("BUTTON"), TEXT("Display"),
						  WS_VISIBLE | WS_BORDER | WS_CHILD | BS_FLAT,
						  215, 8, 100, 25,
						  hWnd, (HMENU)TB_TEST3, NULL, NULL
						  );

					  CreateWindow(TEXT("STATIC"), TEXT("Action"),
						  WS_VISIBLE | WS_CHILD | BS_CENTER | BS_RIGHT,
						  325, 8, 60, 25,
						  hWnd, (HMENU)TB_TEST4, NULL, NULL
						  );

					hWndEditAction=  CreateWindow(TEXT("EDIT"), TEXT("NULL"),
						  WS_VISIBLE | WS_BORDER | WS_CHILD | BS_CENTER,
						  385, 8, 170, 25,
						  hWnd, (HMENU)TB_EDIT1, NULL, NULL
						  );

					   CreateWindow(TEXT("STATIC"), TEXT("Timer"),
						  WS_VISIBLE | WS_CHILD | BS_CENTER,
						  325, 38, 60, 25,
						  hWnd, (HMENU)TB_TEST5, NULL, NULL
						  );

					 hWndEditTime= CreateWindow(TEXT("EDIT"), TEXT("NULL"),
						  WS_VISIBLE | WS_BORDER | WS_CHILD | BS_CENTER,
						  385, 38, 170, 25,
						  hWnd, (HMENU)TB_EDIT2, NULL, NULL
						  );

					  break;
	}
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case TB_REOPEN:
		
			SetFileAttributes( fullpathname,  
            GetFileAttributes(fullpathname) & ~FILE_ATTRIBUTE_READONLY);
			SendMessageToNis( hWnd, NOC_REOPEN);
			
			break;

		case TB_TEST2:
			SendMessageToNis( hWnd, NOC_REQUEST);
			break;

		case TB_TEST3:
			SendMessageToNis( hWnd, NOC_RESTORE);
			break;

		case TB_ENABLE:
			if (m_Disabled)
			{
				SetWindowText( hWndEnable, TEXT("Disable")); //#define NOC_ENABLENIS 7 #define NOC_DISABLENIS 8  NOC_DISABLENIS
				m_Disabled=FALSE;
				SendMessageToNis( hWnd,NOC_ENABLENIS);
				
			}
			else 
			{
				SetWindowText( hWndEnable, TEXT("Enable"));
				m_Disabled=TRUE;
				SendMessageToNis( hWnd, NOC_DISABLENIS);
				
			}
			break;

			case TB_SAVE:
			
		    SetFileAttributes( fullpathname,  
            GetFileAttributes(fullpathname) & ~FILE_ATTRIBUTE_READONLY);
			SendMessageToNis( hWnd, NOC_SAVEDOC); 

			break;

			
			case TB_STOP:

			SendMessageToNis( hWnd, NOC_STOPTIMER); 

			break;
  
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(0, 0, 0));
		SetBkColor(hdcStatic, RGB(255, 255, 255));
		return (INT_PTR)CreateSolidBrush(RGB(255, 255, 255));
	}

	case WM_COPYDATA:
		  
	    RecvMessageFromNis(lParam);
		break;

	case WM_SENDNIS: 
		{
		  int action= lParam;
		  if (action ==NOC_ACTION_SINLCLK)
			  SetWindowText( hWndEditAction, TEXT("Single Clicked")); 
		  if (action ==NOC_ACTION_SELTEXT)
			  SetWindowText( hWndEditAction, TEXT("Selected Text")); 
		   if (action ==NOC_ACTION_DRAWBGW)
			  SetWindowText( hWndEditAction, TEXT(" Name of the Gesture Drawn BGW")); 
		  if (action ==NOC_ACTION_DRAWBPC)
			  SetWindowText( hWndEditAction, TEXT("Name of the Gesture Drawn BPC")); 
		  if (action ==NOC_ACTION_TXT_INS )
			  SetWindowText( hWndEditAction, TEXT("Text Inserted")); 
		    //Timeout edit NOC_ACTION_SEND_GEST
		 
		  if (action ==NOC_TIMEOUT_HW_EXP )
			  SetWindowText( hWndEditTime, TEXT("Handwriting Expired")); 
		  if (action ==NOC_TIMEOUT_HW_PRE )
			 SetWindowText( hWndEditTime, TEXT("Handwriting Pre-empted")); 

		  if (action ==NOC_TIMEOUT_PC_EXP )
			 SetWindowText( hWndEditTime, TEXT("Punctuation Expired")); 

		 if (action ==NOC_TIMEOUT_PC_PRE )
			 SetWindowText( hWndEditTime, TEXT("Punctuation Pre-empted")); 

		 if (action ==NOC_TIMEOUT_NO )
			 SetWindowText( hWndEditTime, TEXT("NO Timeout ")); //

		  if (action ==NOC_TIMEOUT_PC_ACT )
			 SetWindowText( hWndEditTime, TEXT("Punctuation Activated")); 

		  if (action ==NOC_TIMEOUT_HW_ACT )
			 SetWindowText( hWndEditTime, TEXT("Handwriting Activated ")); 


		 
		}
		break;

		case WM_SENDNOC_GEST: 
		{
		  int action= lParam;
		 
		  switch  (action)
		  {
			  case  GPRS_CUT:
				 SetWindowText( hWndEditAction,  L"CUT" ); 
			     break;
			  case  GPRS_COPY:
				 SetWindowText( hWndEditAction,  L"COPY" ); 
				 break;
			case  GPRS_PASTE:
				 SetWindowText( hWndEditAction,  L"PASTE" ); 
				 break;
			case  GPRS_BACKSPACE:
				 SetWindowText( hWndEditAction,  L"BACKSPACE" ); 
				 break;
			case  GPRS_UNDO:
				 SetWindowText( hWndEditAction,  L"UNDO" ); 
				 break;
			case  GPRS_CASE:
				 SetWindowText( hWndEditAction,  L"CASE" ); 
				 break;
			case  GPRS_CORRECTION:
				 SetWindowText( hWndEditAction,  L"CORRECTION" ); 
				 break;
			 case  GPRS_BOLD:
				 SetWindowText( hWndEditAction,  L"BOLD" ); 
				 break;
			 case  GPRS_UNDERLINE:
				 SetWindowText( hWndEditAction,  L"UNDERLINE" ); 
				 break;
			 case  GPRS_ITALICIZE:
				 SetWindowText( hWndEditAction,  L"ITALICIZE" ); 
				 break;
			 case  GPRS_LEFTJUSTIFY:
				 SetWindowText( hWndEditAction,  L"LEFTJUSTIFY" ); 
				 break;
			case  GPRS_CENTREJUSTIFY:
				 SetWindowText( hWndEditAction,  L"CENTREJUSTIFY" ); 
				 break;
			case  GPRS_RIGHTJUSTIFY:
				 SetWindowText( hWndEditAction,  L"RIGHTJUSTIFY" ); 
				 break;
			case  GPRS_SPACE:
				 SetWindowText( hWndEditAction,  L"SPACE" ); 
				 break;
			case  GPRS_TAB:
				 SetWindowText( hWndEditAction,  L"TAB" ); 
				 break;
			case  GPRS_RETURN:
				 SetWindowText( hWndEditAction,  L"RETURN" ); 
				 break;
			case  GPRS_PERIOD:
				 SetWindowText( hWndEditAction,  L"PERIOD" ); 
				 break;
			case  GPRS_COMMA:
				 SetWindowText( hWndEditAction,  L"COMMA" ); 
				 break;
			case  GPRS_SQUOTE:
				 SetWindowText( hWndEditAction,  L"SQUOTE" ); 
				 break;
			case  GPRS_DQUOTE:
				 SetWindowText( hWndEditAction,  L"DQUOTE" ); 
				 break;
			case  GPRS_APOS:
				 SetWindowText( hWndEditAction,  L"APOS" ); 
				 break;
			case  GPRS_UNDERSCORE:
				 SetWindowText( hWndEditAction,  L"UNDERSCORE" ); 
				 break;
			case  GPRS_COLON:
				 SetWindowText( hWndEditAction,  L"COLON" ); 
				 break;
			case  GPRS_SEMICOLON:
				 SetWindowText( hWndEditAction,  L"SEMICOLON" ); 
				 break;
		    case  GPRS_PERCENT:
				 SetWindowText( hWndEditAction,  L"PERCENT" ); 
				 break;
			case  GPRS_QUESTION:
				 SetWindowText( hWndEditAction,  L"QUESTION" ); 
				 break;
			case  GPRS_XMARK:
				 SetWindowText( hWndEditAction,  L"XMARK" ); 
				 break;
			case  GPRS_LPARENTHESIS:
				 SetWindowText( hWndEditAction,  L"LPARENTHESIS" ); 
				 break;
			case  GPRS_RPARENTHESIS:
				 SetWindowText( hWndEditAction,  L"RPARENTHESIS" ); 
				 break;
			case  GPRS_LESSTHAN:
				 SetWindowText( hWndEditAction,  L"LESSTHAN" ); 
				 break;
			case  GPRS_GREATERTHAN:
				 SetWindowText( hWndEditAction,  L"GREATERTHAN" ); 
				 break;
			case  GPRS_LBRACKET:
				 SetWindowText( hWndEditAction,  L"LBRACKET" ); 
				 break;
			case  GPRS_RBRACKET:
				 SetWindowText( hWndEditAction,  L"RBRACKET" ); 
				 break;
			case  GPRS_LBRACE:
				 SetWindowText( hWndEditAction,  L"LBRACE" ); 
				 break;
			 case  GPRS_RBRACE:
				 SetWindowText( hWndEditAction,  L"RBRACE" ); 
				 break;
			case  GPRS_BSLASH:
				 SetWindowText( hWndEditAction,  L"BSLASH" ); 
				 break;
			case  GPRS_FSLASH:
				 SetWindowText( hWndEditAction,  L"FSLASH" ); 
				 break;
			case  GPRS_TILDE:
				 SetWindowText( hWndEditAction,  L"TILDE" ); 
				 break;
			case  GPRS_AT:
				 SetWindowText( hWndEditAction,  L"AT" ); 
				 break;
			case  GPRS_NUM:
				 SetWindowText( hWndEditAction,  L"NUM" ); 
				 break;
			case  GPRS_DOLLAR:
				 SetWindowText( hWndEditAction,  L"DOLLAR" ); 
				 break;
			case  GPRS_AMPERSAND:
				 SetWindowText( hWndEditAction,  L"AMPERSAND" ); 
				 break;
			case  GPRS_PLUS:
				 SetWindowText( hWndEditAction,  L"PLUS" ); 
				 break;
			case  GPRS_CARET:
				 SetWindowText( hWndEditAction,  L"CARET" ); 
				 break;
			case  GPRS_PIPE:
				 SetWindowText( hWndEditAction,  L"PIPE" ); 
				 break;
			case  GPRS_ASTERISK:
				 SetWindowText( hWndEditAction,  L"ASTERISK" ); 
				 break;
			case  GPRS_HYPHEN:
				 SetWindowText( hWndEditAction,  L"HYPHEN" ); 
				 break;
			case  GPRS_EQUALS:
				 SetWindowText( hWndEditAction,  L"EQUALS" ); 
				 break;
				 




		  }

		

		 
		}
		break;
	    
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	OPENFILENAME ofn;       // common dialog box structure
	WCHAR szFile[260]=_T("");       // buffer for file name
	WCHAR filter [160] = _T("All\0*.*\0Text\0*.TXT\0");
	
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hInstance=hInst;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter =filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
	//	if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		//{
			if (LOWORD(wParam) == IDOK )

			{
				if (GetOpenFileName(&ofn)==TRUE) 
				{
					SetDlgItemText(hDlg, IDC_FILENAME,	 ofn.lpstrFile  );
				}
				int res= CommDlgExtendedError();
			}
			

			if (LOWORD(wParam) == IDCANCEL )
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}

			if (LOWORD(wParam) == IDC_CANCEL )
			{
				GetDlgItemText(hDlg, IDC_FILENAME,NameFileDiagram,260);
				wcstombs(storeNisMsg.Output,NameFileDiagram,260);
			
				XOFF=GetDlgItemInt(hDlg,IDC_XOFF,NULL,TRUE);
				YOFF=GetDlgItemInt(hDlg,IDC_YOFF,NULL,TRUE);
				storeNisMsg.Xoffset=XOFF;
				storeNisMsg.Yoffset=YOFF;
				m_DialogOk=TRUE;
				EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			}
	//	}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL	SendMessageToNis(HWND hWnd, int NisCmd)
	{
			LRESULT res=NULL;
			HWND hwDispatch;
			if ((NisCmd == NOC_ENABLENIS) || (NisCmd== NOC_DISABLENIS )) //
			{
				hwDispatch = FindWindow( NULL, _T("NUS") );

				if (NisCmd == NOC_ENABLENIS)
				res = ::SendMessage( hwDispatch,
                   WM_ENABLENIS,
                   (WPARAM)(HWND) hWnd,
                   (LPARAM) NULL );
				
				if (NisCmd == NOC_DISABLENIS)
				res = ::SendMessage( hwDispatch,
                   WM_DISABLENIS,
                   (WPARAM)(HWND) hWnd,
                   (LPARAM) NULL );
				
				return TRUE;

			}

			hwDispatch = FindWindow(  NULL,_T("NUSWnd") );
			
		  
			// Fill the COPYDATA structure
			NisMsg.Ybottom=storeNisMsg.Ybottom;

			NisMsg.SelStart=storeNisMsg.SelStart;
			NisMsg.SelEnd=storeNisMsg.SelEnd;

			NisMsg.Xpenup=storeNisMsg.Xpenup;
			
			if (m_DialogOk)
			{
			NisMsg.Xoffset = storeNisMsg.Xoffset;
			NisMsg.Yoffset = storeNisMsg.Yoffset;
			strcpy(NisMsg.Output,storeNisMsg.Output);
			m_DialogOk=FALSE;
			}
			
			//wcstombs(NisMsg.Output,NameFileDiagram,260);

			NisMsg.Ybottom=storeNisMsg.Ybottom;
			NisMsg.NOC_Mode=storeNisMsg.NOC_Mode;
			NisMsg.Xtip=storeNisMsg.Xtip;
			NisMsg.Ytip=storeNisMsg.Ytip;
			NisMsg.Xstart=storeNisMsg.Xstart;
			NisMsg.Xend=storeNisMsg.Xend;
			NisMsg.Ystart=storeNisMsg.Ystart;
			NisMsg.Yend=storeNisMsg.Yend;
		    NISCDS.dwData=NisCmd;           // function identifier
			NISCDS.cbData = sizeof( NisMsg );  // size of data
			NISCDS.lpData = &NisMsg;           // data structure
			//
			
					
			
            if( hwDispatch != NULL )
                res = SendMessage( hwDispatch,
                   WM_COPYDATA,
                   (WPARAM)(HWND) hWnd,
                   (LPARAM) (LPVOID) &NISCDS );
		    if ((res == 0) && (GetLastError() != 0)) return FALSE;

		    return TRUE;
}
;
BOOL	RecvMessageFromNis(LPARAM _lparam)
	{
		  pNISCDS = (PCOPYDATASTRUCT) _lparam;
		   switch( pNISCDS->dwData )
		    {
		     case NOC_RESPONSE:

               // example store data from NIS
				 if (  (((NISDATA*)pNISCDS->lpData)->NOC_Mode==NOC_ACTION_SEND_SYM ) || (((NISDATA*)pNISCDS->lpData)->NOC_Mode==NOC_ACTION_SEND_CHAR)  )
				 {
					 strcpy(storeNisMsg.Output,((NISDATA*)pNISCDS->lpData)->Output);
					 WCHAR    symbol[512];
					 MultiByteToWideChar(0, 0, storeNisMsg.Output, 5, symbol, 512);
					 SetWindowText( hWndEditAction, symbol); 
					 break;
				 }
				storeNisMsg.Ybottom=((NISDATA*)pNISCDS->lpData)->Ybottom;

				storeNisMsg.SelStart=((NISDATA*)pNISCDS->lpData)->SelStart;
			    storeNisMsg.SelEnd=((NISDATA*)pNISCDS->lpData)->SelEnd;

				storeNisMsg.Xpenup=((NISDATA*)pNISCDS->lpData)->Xpenup;
				storeNisMsg.NOC_Mode=((NISDATA*)pNISCDS->lpData)->NOC_Mode;
				//storeNisMsg.NOC_Mode=((NISDATA*)pNISCDS->lpData)->NOC_Mode;

				storeNisMsg.Xtip=((NISDATA*)pNISCDS->lpData)->Xtip;
				storeNisMsg.Ytip=((NISDATA*)pNISCDS->lpData)->Ytip;

				storeNisMsg.Xstart=((NISDATA*)pNISCDS->lpData)->Xstart;
				storeNisMsg.Xend=((NISDATA*)pNISCDS->lpData)->Xend;
				storeNisMsg.Ystart=((NISDATA*)pNISCDS->lpData)->Ystart;
				storeNisMsg.Yend=((NISDATA*)pNISCDS->lpData)->Yend;
		     break;
		   
		     case NOC_ACTION:
				// SetWindowText( hWndEditAction, TEXT("TEST 23523")); 
			 break;

			 case NOC_TIMEOUT:
				//  SetWindowText( hWndEditTime, TEXT("TIME 23523")); 
		     break;

		    }
		   return TRUE;

    };

HWND GetAcitveWordWindowHandle()
{
	HWND hWnd = NULL;

	EnumWindows(EnumWindowsProc, (LPARAM)&hWnd);

	return hWnd;
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, long lParam) 
{ 
    TCHAR buff[255]; 
 
    if (IsWindowVisible(hWnd)) 
	{ 
        GetWindowText(hWnd, buff, 254); 
       

		CString strWinText(buff);
		if((strWinText.Find(_T("Microsoft Word")) >= 0) && (strWinText.Find( _T("Sample_Document1")) >= 0))
		{
			(*(HWND*)lParam) = hWnd;
			return FALSE;
		}
    } 
    return TRUE; 
} 