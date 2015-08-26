#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
//#include <crtdbg.h> 

#include "lha.h"
//#include "../wce/wcedef.h"
#include "unlha.h"

LPTSTR g_outBuff;
DWORD  g_outBuffSize;
HINSTANCE g_hInst;
BOOL g_bRunning;
HWND g_hWnd;
BOOL g_bCurrentDirectory=FALSE;

static LPBYTE g_buffer;
static DWORD g_bufSize;
static DWORD g_dwWriteSize;

void writeToMem(unsigned char *p, unsigned long lc)
{
	if( g_buffer==NULL || g_dwWriteSize==g_bufSize ){
		return;
	}

	if( g_bufSize - g_dwWriteSize < lc ){
		// “r’†‚Ü‚Å‘‚­
		memcpy(g_buffer, p, 
			g_bufSize - g_dwWriteSize);
		g_buffer += g_bufSize - g_dwWriteSize;
	}else{
		// ‘S•”‘‚­
		memcpy(g_buffer, p, lc);
		g_buffer += lc;
		g_dwWriteSize += lc;
	}
}
