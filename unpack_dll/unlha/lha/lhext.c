/* ------------------------------------------------------------------------ */
/* LHa for UNIX    															*/
/*				lhext.c -- LHarc extract									*/
/*																			*/
/*		Copyright (C) MCMLXXXIX Yooichi.Tagawa								*/
/*		Modified          		Nobutaka Watazaki							*/
/*																			*/
/*  Ver. 0.00  Original								1988.05.23  Y.Tagawa	*/
/*  Ver. 1.00  Fixed								1989.09.22  Y.Tagawa	*/
/*  Ver. 0.03  LHa for UNIX							1991.12.17  M.Oki		*/
/*  Ver. 1.12  LHa for UNIX							1993.10.01	N.Watazaki	*/
/*  Ver. 1.13b Symbolic Link Update Bug Fix			1994.06.21	N.Watazaki	*/
/*	Ver. 1.14  Source All chagned					1995.01.14	N.Watazaki	*/
/*  Ver. 1.14e bugfix                               1999.04.30  T.Okamoto   */
/* ------------------------------------------------------------------------ */
#ifdef WIN32
#  ifdef _MSC_VER
//#     include <direct.h>
//#     include <sys/utime.h>
#  endif /* _MSC_VER */
#  ifdef __BORLANDC__
#     include <dir.h>
#     include <utime.h>
#  endif /* __BORLANDC__ */
#endif /* WIN32 */

#include "lha.h"
//#include "..\unlhadlg.h"

#ifdef WIN32
   /* function prototypes */
//   static boolean inquire_extract(char *);
   static boolean inquire_extract(char *, LzHeader *);
   static boolean make_parent_path(char *);
   static FILE * open_with_make_path(char *);
   static void adjust_info(char * , LzHeader *);
   extern void extract_one(FILE * , LzHeader *);

   extern BOOL compareFileName(int, char **, char *);//uema2.
#endif /* WIN32 */
/* ------------------------------------------------------------------------ */
static int      skip_flg = FALSE;	/* FALSE..No Skip , TRUE..Skip */
static char	   *methods[] =
{
	LZHUFF0_METHOD, LZHUFF1_METHOD, LZHUFF2_METHOD, LZHUFF3_METHOD,
	LZHUFF4_METHOD, LZHUFF5_METHOD, LZHUFF6_METHOD, LZHUFF7_METHOD,
	LARC_METHOD, LARC5_METHOD, LARC4_METHOD,
	LZHDIRS_METHOD,
	NULL
};

#ifdef WIN32
   /* function prototypes */
//   static boolean inquire_extract(char *);
   static boolean inquire_extract(char *, LzHeader *);
   static boolean make_parent_path(char *);
   static FILE * open_with_make_path(char *);
   static void adjust_info(char * , LzHeader *);
   extern void extract_one(FILE * , LzHeader *);
#endif /* WIN32 */

/* ------------------------------------------------------------------------ */
static boolean
inquire_extract(char *name, LzHeader *hdr)
{
	struct stat     stbuf;

	skip_flg = FALSE;
	if (stat(name, &stbuf) >= 0) {
		if (!is_regularfile(&stbuf)) {
			LHAerror("Already exist (not a file)", name);
			return FALSE;
		}

		if (noexec) {
//			printf("EXTRACT %s but file is exist.\n", name);
			return FALSE;
		}
		else if (!force) {
//			if (!isatty(0))  /* CE での動作上意味がない */
//				return FALSE;

//			switch (inquire("OverWrite ?(Yes/[No]/All/Skip)", name, "YyNnAaSs\n")) {
			switch (inquire(name, stbuf.st_mtime,
				hdr->unix_last_modified_stamp)) {
			case 0:
			case 1:/* Y/y */
				break;
			case 2:
			case 3:/* N/n */
			case 8:/* Return */
				return FALSE;
			case 4:
			case 5:/* A/a */
				force = TRUE;
				break;
			case 6:
			case 7:/* S/s */
				skip_flg = TRUE;
				break;
			}
		}
	}
//	if (noexec)
//		printf("EXTRACT %s\n", name);

	return TRUE;
}

/* ------------------------------------------------------------------------ */
static          boolean
make_parent_path(name)
	char           *name;
{
	char            path[FILENAME_LENGTH];
	struct stat     stbuf;
	register char  *p;

	/* make parent directory name into PATH for recursive call */
//	sprintf(path, "%s\\%s", cmd_filev[0], name);

//	strcpy(path, cmd_filev[0]);
//	strcat(path, name);
	strcpy(path, name);

	for (p = path + strlen(path); p > path; p--)
//		if (p[-1] == '/') {
		if (p[-1] == '/' || p[-1] == '\\') {
			*--p = '\0';
			break;
		}

	if (p == path) {
		message("Why?", "ROOT");
		return FALSE;	/* no more parent. */
	}

	if (GETSTAT(path, &stbuf) >= 0) {
		if (is_directory(&stbuf))
			return TRUE;
		LHAerror("Not a directory", path);
		return FALSE;
	}
	errno = 0;

	if (verbose)
		;
//		printf("Making directory \"%s\".\n", path);

#ifndef WIN32
	if (mkdir(path, 0777) >= 0)	/* try */
		return TRUE;	/* successful done. */
#else /* WIN32 */
#  if (defined(_MSC_VER) || defined(__BORLANDC__))
#     if (defined(__BORLANDC__) && (__BORLANDC__ < 0x520))
        if (0 <= mkdir(path)) /* try */
        {
           return TRUE; /* successful done. */
        }
#     else /* ! (__BORLANDC__ < 0x520) */ 
        if (0 <= _mkdir(path)) /* try */
        {
           return TRUE; /* successful done. */
        }
#     endif /* __BORLANDC__ < 0x520 */
#  endif /* _MSC_VER || __BORLANDC__ */
#endif /* ! WIN32 */
	errno = 0;

	if (!make_parent_path(path))
		return FALSE;

#ifndef WIN32
	if (mkdir(path, 0777) < 0) {	/* try again */
		message("Cannot make directory", path);
		return FALSE;
	}
#else /* WIN32 */
#  if (defined(_MSC_VER) || defined(__BORLANDC__))
#     if (defined(__BORLANDC__) && (__BORLANDC__ < 0x520))
        if (mkdir(path) < 0) /* try again */
        {
           message("Cannot make directory", path);
           return FALSE;
        }
#     else /* ! (__BORLANDC__ < 0x520) */ 
        if (_mkdir(path) < 0) /* try again */
        {
           message("Cannot make directory", path);
           return FALSE;
        }
#     endif /* __BORLANDC__ < 0x520 */
#  endif /* _MSC_VER || __BORLANDC__ */
#endif /* ! WIN32 */

	return TRUE;
}

/* ------------------------------------------------------------------------ */
static FILE    *
open_with_make_path(name)
	char           *name;
{
	FILE           *fp;
	char *p = name;
	static char temp[MAX_PATH*2];

//	sprintf(temp, "%s\\%s", cmd_filev[0], name);
	strcpy(temp, name);

	// '/' -> '\'
	while( *p!='\0' ){
		if( *p == '/' ) *p = '\\';
		p++;
	}

	if ((fp = fopen(temp, WRITE_BINARY)) == NULL) {
		errno = 0;
		if (!make_parent_path(temp) ||
		    (fp = fopen(temp, WRITE_BINARY)) == NULL)
			printfToBuffer("解凍に失敗しました");
//			error("Cannot extract", temp);
		errno = 0;
	}
/*
	if ((fp = fopen(name, WRITE_BINARY)) == NULL) {
		errno = 0;
		if (!make_parent_path(name) ||
		    (fp = fopen(name, WRITE_BINARY)) == NULL)
			error("Cannot extract", name);
		errno = 0;
	}
*/
	return fp;
}

/* ------------------------------------------------------------------------ */
static void
adjust_info(name, hdr)
	char           *name;
	LzHeader       *hdr;
{
#ifndef WIN32
	time_t          utimebuf[2];
#else /* WIN32 */
#  ifdef _MSC_VER
    struct _utimbuf utimebuf;
#  endif /* _MSC_VER */
#  ifdef __BORLANDC__
    struct utimbuf utimebuf;
#  endif /* __BORLANDC__ */
#endif /* ! WIN32 */

	/* adjust file stamp */
#ifndef WIN32
	utimebuf[0] = utimebuf[1] = hdr->unix_last_modified_stamp;
	if ((hdr->unix_mode & UNIX_FILE_TYPEMASK) != UNIX_FILE_SYMLINK)
		utime(name, utimebuf);
#else /* WIN32 */
    utimebuf.actime = utimebuf.modtime = hdr->unix_last_modified_stamp;
	if ((hdr->unix_mode & UNIX_FILE_TYPEMASK) != UNIX_FILE_SYMLINK)
	{
		utime(name, &utimebuf);
	}
#endif /* ! WIN32 */

	if (hdr->extend_type == EXTEND_UNIX
	    || hdr->extend_type == EXTEND_OS68K
	    || hdr->extend_type == EXTEND_XOSK) {
#ifdef NOT_COMPATIBLE_MODE
            /* Please need your modification in this space. */
#else
		if ((hdr->unix_mode & UNIX_FILE_TYPEMASK) != UNIX_FILE_SYMLINK)
			chmod(name, hdr->unix_mode);
#endif
#ifndef WIN32
		if (!getuid()) {
#  ifndef HAVE_NO_LCHOWN
			if ((hdr->unix_mode & UNIX_FILE_TYPEMASK) == UNIX_FILE_SYMLINK)
				lchown(name, hdr->unix_uid, hdr->unix_gid);
			else
#  endif /* HAVE_NO_LCHWON */
				chown(name, hdr->unix_uid, hdr->unix_gid);
		}
#endif /* ! WIN32 */
		errno = 0;
	}
}

/* ------------------------------------------------------------------------ */
extern void
extract_one(afp, hdr)
	FILE           *afp;	/* archive file */
	LzHeader       *hdr;
{
	FILE           *fp;	/* output file */
	struct stat     stbuf;
	static char     name[257];
	int             crc;
	int             method;
	boolean         save_quiet, save_verbose, up_flag;
	char           *q = hdr->name, c;

	if (ignore_directory && rindex(hdr->name, '/')) {
		q = (char *) rindex(hdr->name, '/') + 1;
	}
	else {
		if (*q == '/') {
			q++;
			/*
			 * if OSK then strip device name
			 */
			if (hdr->extend_type == EXTEND_OS68K
			    || hdr->extend_type == EXTEND_XOSK) {
				do
					c = (*q++);
				while (c && c != '/');
				if (!c || !*q)
					q = ".";	/* if device name only */
			}
		}
	}

	if (extract_directory){
//		sprintf(name, "%s/%s", extract_directory, q);
		// ディレクトリ名の最後に \ がなければつける
		if(extract_directory[strlen(extract_directory)-1] != '\\')
			sprintf(name, "%s\\%s", extract_directory, q);
		else
			sprintf(name, "%s%s", extract_directory, q);
	}else{
		if(q[0]=='\\') strcpy(name, q);
		else           sprintf(name, "\\%s", q);
	}


	/* LZHDIRS_METHODを持つヘッダをチェックする */
	/* 1999.4.30 t.okamoto */
	for (method = 0;; method++) {
		if (methods[method] == NULL) {
			LHAerror("Unknown method skiped ...", name);
			return;
		}
		if (bcmp(hdr->method, methods[method], 5) == 0)
			break;
	}

	if ((hdr->unix_mode & UNIX_FILE_TYPEMASK) == UNIX_FILE_REGULAR
		&& method != LZHDIRS_METHOD_NUM) {
#if 0
		for (method = 0;; method++) {
			if (methods[method] == NULL) {
				LHAerror("Unknown method skiped ...", name);
				return;
			}
			if (bcmp(hdr->method, methods[method], 5) == 0)
				break;
		}
#endif

		reading_filename = archive_name;
		writting_filename = name;
//		output_to_stdout = 1; //uema2
		if (output_to_stdout || verify_mode) {
			if (noexec) {
//				printf("%s %s\n", verify_mode ? "VERIFY" : "EXTRACT", name);
				if (afp == stdin) {
					int             i = hdr->packed_size;
					while (i--)
						fgetc(afp);
				}
				return;
			}

			save_quiet = quiet;
			save_verbose = verbose;
			if (!quiet && output_to_stdout) {
//				printf("::::::::\n%s\n::::::::\n", name);
				quiet = TRUE;
				verbose = FALSE;
			}
			else if (verify_mode) {
				quiet = FALSE;
				verbose = TRUE;
			}

			crc = decode_lzhuf
				(afp, stdout, hdr->original_size, hdr->packed_size, name, method);
			quiet = save_quiet;
			verbose = save_verbose;
		}
		else {
			if (skip_flg == FALSE)  {
				up_flag = inquire_extract(name, hdr);
				if (up_flag == FALSE && force == FALSE) {
					return;
				}
			}

			if (skip_flg == TRUE) {	/* if skip_flg */
				if (stat(name, &stbuf) == 0 && force != TRUE) {
					if (stbuf.st_mtime >= hdr->unix_last_modified_stamp) {
//						if (quiet != TRUE)
//							printf("%s : Skipped...\n", name);
						return;
					}
				}
			}
			if (noexec) {
				if (afp == stdin) {
					int i = hdr->packed_size;
					while (i--)
						fgetc(afp);
				}
				return;
			}

//			signal(SIGINT, interrupt);
//			signal(SIGHUP, interrupt);

			unlink(name);
			errno = 0;
			remove_extracting_file_when_interrupt = TRUE;

			if ((fp = open_with_make_path(name)) != NULL) {
				crc = decode_lzhuf
					(afp, fp, hdr->original_size, hdr->packed_size, name, method);
				fclose(fp);
			}
			remove_extracting_file_when_interrupt = FALSE;
//			signal(SIGINT, SIG_DFL);
//			signal(SIGHUP, SIG_DFL);

			if (!fp)
				return;
		}

		errno = 0;
		if (hdr->has_crc && crc != hdr->crc)
			LHAerror("CRC error", name);
	}
	else if ((hdr->unix_mode & UNIX_FILE_TYPEMASK) == UNIX_FILE_DIRECTORY
			 || (hdr->unix_mode & UNIX_FILE_TYPEMASK) == UNIX_FILE_SYMLINK
			 || method == LZHDIRS_METHOD_NUM) {
		/* ↑これで、Symblic Link は、大丈夫か？ */
		if (!ignore_directory && !verify_mode) {
			if (noexec) {
				if (quiet != TRUE)
					;
//					printf("EXTRACT %s (directory)\n", name);
				return;
			}
			/* NAME has trailing SLASH '/', (^_^) */
			if ((hdr->unix_mode & UNIX_FILE_TYPEMASK) == UNIX_FILE_SYMLINK) {
				char            buf[256], *bb1, *bb2;
//				int             l_code;
				strcpy(buf, name);
				bb1 = strtok(buf, "|");
				bb2 = strtok(NULL, "|");

#ifdef S_IFLNK
				if (skip_flg == FALSE)  {
					up_flag = inquire_extract(name, hdr);
					if (up_flag == FALSE && force == FALSE) {
						return;
					}
				} else {
					if (GETSTAT(bb1, &stbuf) == 0 && force != TRUE) {
						if (stbuf.st_mtime >= hdr->unix_last_modified_stamp) {
//							if (quiet != TRUE)
//								printf("%s : Skipped...\n", bb1);
							return;
						}
					}
				}

				unlink(bb1);
				l_code = symlink(bb2, bb1);
				if (l_code < 0) {
					if (quiet != TRUE)
						warning("Can't make Symbolic Link : ");
				}
				if (quiet != TRUE) {
//					printf("Symbolic Link %s -> %s\n", bb1, bb2);
				}
				strcpy(name, bb1);	/* Symbolic's name set */
#else
				sprintf(buf, "%s -> %s", bb1, bb2);
				warning("Can't make Symbolic Link", buf);
				return;
#endif
			} else { /* make directory */
				if (!output_to_stdout && !make_parent_path(name))
					return;
			}
		}
	}
	else {
		LHAerror("Unknown information", name);
	}

	if (!output_to_stdout)
		adjust_info(name, hdr);
}

/* ------------------------------------------------------------------------ */
/* EXTRACT COMMAND MAIN														*/
/* ------------------------------------------------------------------------ */
void
cmd_extract()
{
	LzHeader  hdr;
	long      pos;
	FILE     *afp;

	/* open archive file */
	if ((afp = open_old_archive()) == NULL)
		fatal_error2(archive_name, ERROR_ARC_FILE_OPEN);

	if (archive_is_msdos_sfx1(archive_name))
		skip_msdos_sfx1_code(afp);

//	dlgSetArchiveName(archive_name);

	/* extract each files */
	while (get_header(afp, &hdr)) {
		if (need_file(hdr.name)) {
			pos = ftell(afp);
//			extract_one(afp, &hdr);	
			// 引数のファイル名と比較
			// 引数がなければ全解凍だから無条件 TRUE
			// 引数がある場合、比較して一致したら TRUE
			if(TRUE == compareFileName(cmd_filec, 
							cmd_filev, hdr.name)){
				extract_one(afp, &hdr);	
			}
			fseek(afp, pos + hdr.packed_size, SEEK_SET);
		}
		else {
			if (afp != stdin)
				fseek(afp, hdr.packed_size, SEEK_CUR);
			else {
				int i = hdr.packed_size;
				while (i--)
					fgetc(afp);
			}
		}
	}

	/* close archive file */
	fclose(afp);

	return;
}

extern BOOL compareFileName(int ac, char **av, char *name)
{
	int i=0;
	char tmp[MAX_PATH+1];
	char *p=NULL;

	if(ac == 0) return TRUE;

	for(i=0; i<ac; i++){
		// コピー
		strcpy(tmp, av[i]);
		// \ -> /
		p = tmp;
		while(1){
			int j=0;
			if( *p == '\\' ) *p = '/';
			p = CharNextA( p );
			if( *p == '\0' ) break;
		}
		if(0==stricmp(tmp, name)) return TRUE;
	}
	return FALSE;
}

/* Local Variables: */
/* mode:c */
/* tab-width:4 */
/* End: */
