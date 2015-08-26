/* wcedef.h */

#ifndef _WCEDEF_H_
#define _WCEDEF_H_

#include <windows.h>
#include <tchar.h>

int errno;
typedef ULONG time_t;

/* wce_time.c */
#ifndef _TM_DEFINED
struct tm {
        int tm_sec;     /* seconds after the minute - [0,59] */
        int tm_min;     /* minutes after the hour - [0,59] */
        int tm_hour;    /* hours since midnight - [0,23] */
        int tm_mday;    /* day of the month - [1,31] */
        int tm_mon;     /* months since January - [0,11] */
        int tm_year;    /* years since 1900 */
        int tm_wday;    /* days since Sunday - [0,6] */
        int tm_yday;    /* days since January 1 - [0,365] */
        int tm_isdst;   /* daylight savings time flag */
		int tm_gmtoff; /* ? */
        };
#define _TM_DEFINED
#endif

typedef struct {
		int  yr;        // year of interest
		int  yd;        // day of year 
		long ms;        // milli-seconds in the day 
} transitionTime;

/* wce_stat.c */
typedef unsigned int _dev_t;
typedef long _off_t;
typedef unsigned short _ino_t;

#ifndef _STAT_DEFINED

struct _stat {
        _dev_t st_dev;
        _ino_t st_ino;
        unsigned short st_mode;
        short st_nlink;
        short st_uid;
        short st_gid;
        _dev_t st_rdev;
        _off_t st_size;
        time_t st_atime;
        time_t st_mtime;
        time_t st_ctime;
        };
struct _stati64 {
        _dev_t st_dev;
        _ino_t st_ino;
        unsigned short st_mode;
        short st_nlink;
        short st_uid;
        short st_gid;
        _dev_t st_rdev;
        __int64 st_size;
//		long st_size;
        time_t st_atime;
        time_t st_mtime;
        time_t st_ctime;
        };

#endif /* _STAT_DEFINED */

#define stat _stat

#define _S_IFMT         0170000         /* file type mask */
#define _S_IFDIR        0040000         /* directory */
#define _S_IFCHR        0020000         /* character special */
#define _S_IFIFO        0010000         /* pipe */
#define _S_IFREG        0100000         /* regular */
#define _S_IREAD        0000400         /* read permission, owner */
#define _S_IWRITE       0000200         /* write permission, owner */
#define _S_IEXEC        0000100         /* execute/search permission, owner */

#define S_IFMT   _S_IFMT
#define S_IFDIR  _S_IFDIR
#define S_IFCHR  _S_IFCHR
#define S_IFREG  _S_IFREG
#define S_IREAD  _S_IREAD
#define S_IWRITE _S_IWRITE
#define S_IEXEC  _S_IEXEC

/* wce_utime.c */
struct utimbuf {
        time_t actime;        // 最終アクセス時間
        time_t modtime;       // 最終更新時間
        };

#define _utimbuf utimbuf

/* wce_func.c */
/* signal.h */
#define SIGINT          2       /* interrupt */
#define SIGILL          4       /* illegal instruction - invalid function image */
#define SIGFPE          8       /* floating point exception */
#define SIGSEGV         11      /* segment violation */
#define SIGTERM         15      /* Software termination signal from kill */
#define SIGBREAK        21      /* Ctrl-Break sequence */
#define SIGABRT         22      /* abnormal termination triggered by abort call */

/* signal action codes */
#define SIG_DFL (void (__cdecl *)(int))0
#define SIG_IGN (void (__cdecl *)(int))1
#define SIG_SGE (void (__cdecl *)(int))3
#define SIG_ACK (void (__cdecl *)(int))4

/* signal error value (returned by signal call on error) */
#define SIG_ERR (void (__cdecl *)(int))-1


/* 関数定義 */
wchar_t* wce_AToW(const char* a);
char* wce_WToA(const wchar_t* w);
int wce_AToW2(const char *a, wchar_t *w);
int wce_WToA2(const wchar_t *w, char *a);

char* strdup(const char* c);
int utime(const char *f, struct utimbuf *t);

#define _strdup strdup
#define _utime utime

int _mkdir(const char* dir);
int _rmdir(const char* dir);
int _unlink(const char* file);
#define mkdir _mkdir
#define unlink _unlink
#define rmdir  _rmdir

int _stati64(const char *, struct _stat *);
int _wstati64(const wchar_t *, struct _stati64 *);
time_t FILETIMETotime_t(FILETIME* ft);
int _stat(const char *filename, struct _stat *stat);

static __int64 wce_GetDeltaSecs(FILETIME f1, FILETIME f2);
static SYSTEMTIME wce_TmToSystemTime(struct tm *t);
static FILETIME wce_YearToFileTime(WORD wYear);
static FILETIME wce_Int64ToFileTime(__int64 iTime);
static time_t wce_SystemTimeToYDay(SYSTEMTIME s);
static struct tm wce_SystemTimeToTm(SYSTEMTIME *s);
static void wce_GetTZBias(int* pTZBiasSecs, int* pDSTBiasSecs);
long wce_GetMessageTime();
static void wce_cvtdate (int trantype, int year, int month, int week, int dayofweek,
			             int date, int hour, int min, int sec, int msec,
						 transitionTime* pDST);
time_t mktime(struct tm* pt);
time_t time( time_t *timer );
struct tm* localtime(const time_t *ptime);
char* wce_ctime( const time_t *ulSecsSince1970 );

char *_mktemp( char *temp );
int _isatty( int handle );
int _chmod(const char* filename, int pmode);
int _umask(int pmode);

#ifndef _WIN32_WCE_EMULATION
  void (* signal(int sig, void (__cdecl *func)(int)))(int);
#else
  void (* signal(int sig, void (*func)));
#endif

#ifdef _WIN32_WCE
  #if _WIN32_WCE < 300
//#ifdef _WIN32_WCE
    int isupper( int c );
    int islower( int c );
    char *CharNextA(const char* a);
  #endif /* _WIN32_WCE */
  int _stricmp( const char *s1, const char *s2 );
  #define stricmp _stricmp
#endif

int rename( const char *oldname, const char *newname );
void perror( const char *string );

#if _WIN32_WCE < 300
//#ifndef _WIN32_WCE_EMULATION
// 2.11   ではこの関数は必要
// 2.11em ではこの関数は必要
// 3.00   ではこの関数は不要
// 3.00em ではこの関数は不要
char *strrchr( const char *string, int c );
//#endif
#endif

char *getenv( const char *varname );

void printfToBuffer(const char* a);

char *rindex (char *s, int c);
char *index (char *s, int c);

#ifndef WCE_NO_MALLOC
#  define malloc(c)        LocalAlloc(LPTR, c)
#  define free(p)          LocalFree(p)
#  define realloc(p, size) LocalReAlloc(p, size, LMEM_ZEROINIT)
#  define WCE_NO_MALLOC
#endif

#endif /* _WCEDEF_H_ */
