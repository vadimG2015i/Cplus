//
// wce_utime.cpp   _utime 関数
//////////////////////////////////////////////////////

#include "wcedef.h"

extern wchar_t* wce_AToW(const char* a);

// CE には Int32x32To64 がないんで、ここで定義
// __int64 は使えるが、LONGLONG は使えるのか？
#ifdef _WIN32_WCE
#if _WIN32_WCE < 300
  #define Int32x32To64(a, b) ((LONGLONG)((LONG)(a)) * (LONGLONG)((LONG)(b)))
//#define Int32x32To64(a, b) ((__int64)((LONG)(a)) * (__int64)((LONG)(b)))
#endif
#endif

int utime(const char *f, struct utimbuf *t)
{
	HANDLE h;
	FILETIME atime={0}, mtime={0};
	__int64 time64;
	BOOL rc;
	static wchar_t w[MAX_PATH*2];

	wce_AToW2(f, w);
	h = CreateFileW(w, GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, 0, 0);
//		NULL, OPEN_ALWAYS, 0, 0);

	if(h==INVALID_HANDLE_VALUE) return -1;

	time64 = Int32x32To64(t->actime, 10000000) + 116444736000000000;
	atime.dwLowDateTime  = (DWORD)time64;
	atime.dwHighDateTime = (DWORD)(time64 >> 32);
	time64 = Int32x32To64(t->modtime, 10000000) + 116444736000000000;
	mtime.dwLowDateTime  = (DWORD)time64;
	mtime.dwHighDateTime = (DWORD)(time64 >> 32);

	rc = SetFileTime(h, NULL, &atime, &mtime);
	return rc==0 ? -1 : 0 ;
}

