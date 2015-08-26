//
// wce_direct.cpp   _mkdir, _rmdir, _unlink ä÷êî
//////////////////////////////////////////////////////

//#include <windows.h>
#include "wcedef.h"

extern wchar_t* wce_AToW(const char* a);

// success ->  0
// fail    -> -1

int _mkdir(const char* dir)
{
	static wchar_t wdir[MAX_PATH];
	BOOL rc;
//	wchar_t* wdir = wce_AToW(dir);
	wce_AToW2(dir, wdir);

	rc = CreateDirectoryW(wdir, NULL);

	return rc==0 ? -1 : 0 ;
}

int _rmdir(const char* dir)
{
	static wchar_t wdir[MAX_PATH];
	BOOL rc;

	wce_AToW2(dir, wdir);
	rc = RemoveDirectoryW(wdir);
	
	return rc==0 ? -1 : 0 ;
}

int _unlink(const char* file)
{
	static wchar_t wfile[MAX_PATH];
	BOOL rc;

	wce_AToW2(file, wfile);
	rc = DeleteFileW(wfile);

	return rc==0 ? -1 : 0 ;
}

#define mkdir _mkdir
#define unlink _unlink
#define rmdir  _rmdir

