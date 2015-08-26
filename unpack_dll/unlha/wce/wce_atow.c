//
// wce_atow.cpp   char   -> wchar_t (AToW)
//               wchar_t -> char    (WToA)  ä÷êî
//////////////////////////////////////////////////////

//#include <windows.h>
#include "wcedef.h"

wchar_t* wce_AToW(const char* a)
{
	int length = MultiByteToWideChar(CP_ACP, 0, 
		a, -1,
		NULL, 0);
	wchar_t *w_buf = (wchar_t*)LocalAlloc(LPTR, (length+1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0,
		a, -1,
		w_buf, length);

	return w_buf;
}

char* wce_WToA(const wchar_t* w)
{
	DWORD charlength = WideCharToMultiByte(CP_ACP, 0, w,
						-1, NULL, 0, NULL, NULL);
	char* pChar = (char*)LocalAlloc(LPTR, charlength+1);
	WideCharToMultiByte(CP_ACP, 0, w,
		-1, pChar, charlength, NULL, NULL);

	return pChar;
}

int wce_AToW2(const char *a, wchar_t *w)
{
	int length = 0;
	length = MultiByteToWideChar(CP_ACP, 0, 
		a, -1, NULL, 0);

	if( w == NULL ) return length;

	MultiByteToWideChar(CP_ACP, 0,
		a, -1,
		w, length);

	return TRUE;
}

int wce_WToA2(const wchar_t *w, char *a)
{
	DWORD charlength;

	charlength = WideCharToMultiByte(CP_ACP, 0, w,
						-1, NULL, 0, NULL, NULL);

	if( a==NULL ) return charlength;

	WideCharToMultiByte(CP_ACP, 0, w,
		-1, a, charlength, NULL, NULL);

	return TRUE;
}

