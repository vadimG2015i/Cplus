/* wce_func.c */
#include "wcedef.h"
#pragma warning(disable:4047)

extern LPTSTR g_outBuff;
extern DWORD  g_outBuffSize;

//extern wchar_t* wce_AToW2(const char* a);

int _umask(int pmode)
{
	return 0;
}

int _chmod(const char* filename, int pmode)
{
	/* success   0 */
	/* fail     -1 */
	return 0;
}

int _isatty( int handle )
{
	/* _isatty はデバイスがキャラクタデバイスであると
	 0 以外の値を返します。
	 それ以外であると 0 を返します。 */
	/* 0==stdin? 1==stdout? */
	if(handle==0) return 0;
	return 0;
}

char *_mktemp( char *temp )
{
	// 一時ファイルのファイル名を
	// ランダムに決めます。
	char *p = temp;
	time_t t;

	time(&t);
	srand(t);
	while(*p!='\0'){
		if(*p == 'X')
			*p = 0x30 + (rand()%10);
		p++;
	}

	return temp;
}

#ifdef _WIN32_WCE
#ifdef _WIN32_WCE_EMULATION
void (* signal(int sig, void (*func)))
{
	return sig;
}
#else
void (* signal(int sig, void (__cdecl *func)(int)))(int)
{
	return sig;
}
#endif
#endif

void printfToBuffer(const char* a)
{
	// エラーなどの情報を
	// 呼び出し側のバッファに書き出します
	static int init = 0;
	TCHAR* w;

	if (g_outBuff == NULL || g_outBuffSize == 0)	// added by hanamaru
		return;

#ifdef _WIN32_WCE
	w = wce_AToW(a);
#else
	w = (TCHAR*)a;
#endif

	if(init==0){
		_tcscpy(g_outBuff, w);
		init = 1;
	}else{
		_tcscat(g_outBuff, w);
	}

#ifdef _WIN32_WCE
	LocalFree (w);
#endif
}

int rename( const char *oldname, const char *newname )
{
	// ファイル名を変える。
//	wchar_t *oldf, *newf;
	static wchar_t oldf[MAX_PATH*2], newf[MAX_PATH*2];
	BOOL rc;
	
	wce_AToW2(oldname, oldf);
	wce_AToW2(newname, newf);

	rc = MoveFileW(oldf, newf);

	return (rc==0) ? -1 : 0;
}

void perror( const char *string )
{	
	printfToBuffer(string);
}

#if _WIN32_WCE < 300
//#ifndef _WIN32_WCE_EMULATION
char *strrchr( const char *string, int c )
{
//	wchar_t *wstr, *wret;
	static wchar_t wstr[MAX_PATH*2];
	wchar_t *wret;
	char *ret;

	wce_AToW2(string, wstr);

	wret = wcsrchr(wstr, (unsigned short)c);	

	if(wret == NULL){
		return NULL;
	}else{
		ret = (char*)(string + (wret - wstr)/sizeof(wchar_t)*sizeof(char));
		return ret;
	}
}
//#endif
#endif


#ifdef _WIN32_WCE
#if _WIN32_WCE < 300
//#ifdef _WIN32_WCE
#ifndef _WIN32_WCE_EMULATION
#endif
int isupper( int c )
{
	return iswupper((wint_t)c);
}

int islower( int c )
{
	return iswlower((wint_t)c);
}
#endif /*_WIN32_WCE */
#endif

#ifdef _WIN32_WCE
char *CharNextA(const char* a)
{
	char *p=(char *)a;
	if(TRUE==IsDBCSLeadByteEx(CP_ACP, (BYTE)*a))
		p+=2;
	else
		p++;

	return p;
}

int _stricmp( const char *s1, const char *s2 )
{
//	int cmpResult;
    const char *p1 = s1;
	const char *p2 = s2;

	// 要するに、大文字小文字の区別がない strcmp だ。
    while( p1!='\0' && p2!='\0' ){
		if( *p1 != *p2 ){
			// p1 が大文字だったら、小文字に置き換えて計算
			if( 0x41 <= *p1 && *p1 <= 0x5A ){
				if( *p1+0x20 != p2 )
					return 1;
			}
			// p1 が小文字だったら、大文字に置き換えて計算
			else if( 0x61 <= *p1 && *p1 <= 0x7A ){
				if( *p1-0x20 != p2 )
					return -1;
			}
		}
		p1 = CharNextA(p1); p2 = CharNextA(p2);
		if( *p1 == '\0' && *p2 != '\0' ) return 1;
		if( *p1 != '\0' && *p2 == '\0' ) return -1;
	}
	return 0;
}
#endif /*_WIN32_WCE */

char *rindex (char *s, int c)	//	added by Hanamaru;
{
	int i,k;

	k = strlen (s);
	for (i = k ; i > k ; --i) {
		if (s [i] == c)
			return s + i;
	}
	return s;
}

char *index (char *s, int c)	//	added by Hanamaru;
{
	int i,k;

	k = strlen (s);
	for (i = 0 ; i < k ; ++i) {
		if (s [i] == c)
			return s + i;
	}
	return s;
}

