//
// wce_stat.cpp   _stati64 関数
//////////////////////////////////////////////////////

//#include <windows.h>
//#include "wce_atow.c"
//#include "wce_time.c"
#include "wcedef.h"

extern wchar_t *wce_AToW(const char *a);


//int _stati64(const char *filename, struct _stati64 *stat)
//int _stati64(const char *filename, struct _stat *stat)
int _stat(const char *filename, struct _stat *stat)
{
	DWORD dwAttribute;
	HANDLE h;
	DWORD dwSizeLow=0, dwSizeHigh=0, dwError=0;
	WIN32_FIND_DATAW fd;
	static wchar_t w_filename[MAX_PATH*2];

	wce_AToW2(filename, w_filename);

	dwAttribute = GetFileAttributesW(w_filename);	
	if(dwAttribute==0xFFFFFFFF) return -1;

	stat->st_mode = 0;
	if((dwAttribute & FILE_ATTRIBUTE_DIRECTORY) != 0)
		stat->st_mode += S_IFDIR;
	else
		stat->st_mode += S_IFREG;

	// とりあえず初期化。
	stat->st_atime = 0;
    stat->st_mtime = 0;
    stat->st_ctime = 0;
	stat->st_size  = 0;

	h = FindFirstFileW(w_filename, &fd);
	if(h == INVALID_HANDLE_VALUE){
		if(w_filename[wcslen(w_filename)-1]	== L'\\'){
			w_filename[wcslen(w_filename)-1] = L'\0';
			h = FindFirstFileW(w_filename, &fd);
			if(h == INVALID_HANDLE_VALUE) return 0;
		}else{
			return 0;
		}
	}

	// FILETIME -> time_t 変換が必要。
	stat->st_atime = FILETIMETotime_t(&fd.ftLastAccessTime);
    stat->st_mtime = FILETIMETotime_t(&fd.ftLastWriteTime);
    stat->st_ctime = FILETIMETotime_t(&fd.ftCreationTime);
	stat->st_size  = fd.nFileSizeLow;

	// ドライブ
	stat->st_dev = 0;

	FindClose( h );
	return 0;
}

/*
int _wstati64(const wchar_t *, struct _stati64 *)
{
	return -1;
}
*/
/*
time_t FILETIMETotime_t(FILETIME* pft)
{
	FILETIME   ft;
	SYSTEMTIME st;
	tm t;
	int nDST = -1;

//	if(!FileTimeToLocalFileTime(pft, &ft) || 
//		!FileTimeToSystemTime(&ft, &st)){
	if(!FileTimeToSystemTime(pft, &st))
		return 0;

	t.tm_sec   = st.wSecond;
	t.tm_min   = st.wMinute;
	t.tm_hour  = st.wHour;
	t.tm_mday  = st.wDay;
	t.tm_mon   = st.wMonth - 1;
	t.tm_year  = st.wYear - 1900;
	t.tm_isdst = nDST;

	return wce_mktime(t);
}
*/

time_t FILETIMETotime_t(FILETIME* pft)
{
	__int64 time, highTime;

	time = pft->dwLowDateTime;
	highTime = (__int64)pft->dwHighDateTime << 32;
	highTime -= 116444736000000000;
	time += highTime;

	if(time < 0) return 0;
	else         return (time_t)(time/10000000);
}

