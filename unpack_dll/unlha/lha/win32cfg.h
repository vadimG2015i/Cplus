/*******************************************************************
 * LHa for Win32 based on LHa for UNIX version 1.14f configuration *
 *                          by T.Fukui, 1999                       *
 *******************************************************************/

// Win 上で free すると
// なぜか落ちるんで
// これをdefine するとfreeしない(極悪)
//#define AVIOD_MEMFREE

#ifndef WIN32CFG_H
#  define WIN32CFG_H
#  ifdef WIN32
      /* 機種非依存のマクロ */
#     define NEED_INCREMENTAL_INDICATOR
#     undef EUC
#     undef ARCHIVENAME_EXTENTION
#     undef BACKUPNAME_EXTENTION
#     define TMP_FILENAME_TEMPLATE "lhXXXXXX"
#     define SUPPORT_LH7

      /* 機種依存マクロ */
#     define USG
#     undef SYSTIME_HAS_NO_TM

      /* 文字列関係 */
#     define NOBSTRING
//#     define NOINDEX
#     undef NOINDEX
#     undef NOSTRDUP
#     undef USESTRCASECMP
#     undef NOMEMSET
#     define MULTIBYTE_CHAR

      /* timezone 情報の取得関係 */
#     define FTIME
#     undef GETTIMEOFDAY
#     define MKTIME
#     undef TIMELOCAL
#     define TZSET
#     undef TIMEZONE_HOOK

      /* ディレクトリ操作関係 */
#     undef NONSYSTEM_DIR_LIBRARY
#     undef SYSV_SYSTEM_DIR
#     define NODIRECTORY

      /* その他 */
#     define NOFTRUNCATE
#     undef NOMKDIR
#     undef MKDIRPATH
#     undef RMDIRPATH
#     define NOT_COMPATIBLE_MODE

#     define NO_INODE
#     define MSDOS 1

#     ifdef _MSC_VER
#        undef strdup
#        define strdup(s) _strdup((s))
#        undef isatty
#        define isatty(h) _isatty((h))
#        undef mktemp
#        define mktemp(t) _mktemp((t))
#        undef umask
#        define umask(p) _umask((p))
#        undef chmod
#        define chmod(f, p) _chmod((f), (p))
#        undef ftime
#        define ftime(t) _ftime((t))
#        undef tzset
#        define tzset _tzset

#        undef S_IFMT
#        undef S_IFREG
#        undef S_IFDIR
#        define S_IFMT _S_IFMT
#        define S_IFREG _S_IFREG
#        define S_IFDIR _S_IFDIR

//#        undef stat
//#        define stat _stat
#     endif /* _MSC_VER */

#     if (defined(_MSC_VER) || (0x520 <= __BORLANDC__))
#        undef unlink
#        define unlink(f) _unlink((f))
#        undef rmdir
#        define rmdir(d) _rmdir((d))
#        undef utime
#        define utime(f, t) _utime((f), (t))
#     endif /* _MSC_VER || (0x520 <= __BORLANDC__) */

#     undef SIGHUP
#     define SIGHUP SIGSEGV
#     undef interface
#     define interface interface_

#ifndef WCE_NO_MALLOC
#  define malloc(c)        LocalAlloc(LPTR, c+1)
#  define free(p)          LocalFree(p)
#  define realloc(p, size) LocalReAlloc(p, size, LMEM_ZEROINIT)
#  define WCE_NO_MALLOC
#endif

#  endif /* WIN32 */
#endif /* ! WIN32CFG_H */
