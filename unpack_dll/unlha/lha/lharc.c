/* ------------------------------------------------------------------------ */
/* LHa for UNIX    															*/
/*				lharc.c -- append to archive								*/
/*																			*/
/*		Copyright (C) MCMLXXXIX Yooichi.Tagawa								*/
/*		Modified          		Nobutaka Watazaki							*/
/*							Thanks to H.Yoshizaki. (MS-DOS LHarc)			*/
/*																			*/
/*  Ver. 0.00  Original							1988.05.23  Y.Tagawa		*/
/*  Ver. 0.01  Alpha Version (for 4.2BSD)		1989.05.28  Y.Tagawa		*/
/*  Ver. 0.02  Alpha Version Rel.2				1989.05.29  Y.Tagawa		*/
/*  Ver. 0.03  Release #3  Beta Version			1989.07.02  Y.Tagawa		*/
/*  Ver. 0.03a Debug							1989.07.03  Y.Tagawa		*/
/*  Ver. 0.03b Modified							1989.07.13  Y.Tagawa		*/
/*  Ver. 0.03c Debug (Thanks to void@rena.dit.junet)						*/
/*												1989.08.09  Y.Tagawa		*/
/*  Ver. 0.03d Modified (quiet and verbose)		1989.09.14  Y.Tagawa		*/
/*  V1.00  Fixed								1989.09.22  Y.Tagawa		*/
/*  V1.01  Bug Fixed							1989.12.25  Y.Tagawa		*/
/*																			*/
/*  DOS-Version Original LHx V C2.01 		(C) H.Yohizaki					*/
/*																			*/
/*  V2.00  UNIX Lharc + DOS LHx -> OSK LHx		1990.11.01  Momozou			*/
/*  V2.01  Minor Modified						1990.11.24  Momozou			*/
/*																			*/
/*  Ver. 0.02  LHx for UNIX						1991.11.18  M.Oki			*/
/*  Ver. 0.03  LHa for UNIX						1991.12.17  M.Oki			*/
/*  Ver. 0.04  LHa for UNIX	beta version		1992.01.20  M.Oki			*/
/*  Ver. 1.00  LHa for UNIX	Fixed				1992.03.19  M.Oki			*/
/*																			*/
/*  Ver. 1.10  for Symblic Link					1993.06.25  N.Watazaki		*/
/*  Ver. 1.11  for Symblic Link	Bug Fixed		1993.08.18  N.Watazaki		*/
/*  Ver. 1.12  for File Date Check				1993.10.28  N.Watazaki		*/
/*  Ver. 1.13  Bug Fixed (Idicator calcurate)	1994.02.21  N.Watazaki		*/
/*  Ver. 1.13a Bug Fixed (Sym. Link delete)		1994.03.11  N.Watazaki		*/
/*	Ver. 1.13b Bug Fixed (Sym. Link delete)		1994.07.29  N.Watazaki		*/
/*	Ver. 1.14  Source All chagned				1995.01.14	N.Watazaki		*/
/*	Ver. 1.14b,c  Bug Fixed                     1996.03.07  t.okamoto		*/
/*  Ver. 1.14d Version up                       1997.01.12  t.okamoto       */
/*  Ver. 1.14g Bug Fixed                        2000.05.06  t.okamoto       */
/*  Ver. 1.14i Modified                         2000.10.06  t.okamoto       */
/* ------------------------------------------------------------------------ */
#define LHA_MAIN_SRC

#include "lha.h"
//#include "..\unlhadlg.h"
//#include "..\wce\wcedef.h"

#ifdef WIN32
#  ifdef __BORLANDC__
#     include <string.h>
#     include <sys/stat.h>
#  endif /* __BORLANDC__ */
   /* function prototypes */
#ifndef _WIN32_WCE
   int main(int , char * []);
#endif
   extern void init_variable(void);
   static void print_tiny_usage_and_exit(void);
   static void message_1(char * , char * , char *);
#  ifdef __BORLANDC__
      static int sort_by_ascii();
#  else /* ! __BORLANDC__ */
      static int sort_by_ascii(const void * , const void *);
#  endif /* __BORLANDC__ */
   extern void sort_files(void);
   static boolean include_path_p(char * , char *);
   static void modify_filename_extention(char * , char *);
   static boolean open_old_archive_1(char * , FILE * *);

#endif /* WIN32 */

/* ------------------------------------------------------------------------ */
/*								PROGRAM										*/
/* ------------------------------------------------------------------------ */
static int      cmd = CMD_UNKNOWN;

/* 1996.8.13 t.okamoto */
#if 0
char          **cmd_filev;
int             cmd_filec;

char           *archive_name;
char            expanded_archive_name[FILENAME_LENGTH];
char            temporary_name[FILENAME_LENGTH];
char            backup_archive_name[FILENAME_LENGTH];
#endif

/* static functions */
extern void     sort_files();
static void		print_version();

char		    *extract_directory = NULL;
char		  **xfilev;
int             xfilec = 257;

jmp_buf UnlhaEnd;
extern BOOL g_bCurrentDirectory;
BOOL g_bMakeBackupFile = FALSE;
BOOL g_bExtractMem     = FALSE;
int  g_errorCode = 0;

/* 1996.8.13 t.okamoto */
#if 0
char           *writting_filename;
char           *reading_filename;

int             archive_file_mode;
int             archive_file_gid;
#endif
/* ------------------------------------------------------------------------ */
extern void
init_variable()		/* Added N.Watazaki */
{
/* options */
	quiet			= FALSE;
	text_mode		= FALSE;
	verbose			= FALSE;
	noexec			= FALSE;	/* debugging option */
	force			= FALSE;
	prof			= FALSE;
#ifndef SUPPORT_LH7
	compress_method = LZHUFF5_METHOD_NUM;
#endif
#ifdef SUPPORT_LH7
	compress_method = LZHUFF7_METHOD_NUM;
#endif

	header_level	= HEADER_LEVEL1;
	quiet_mode		= 0;

#ifdef EUC
	euc_mode		= FALSE;
#endif

/* view command flags */
	verbose_listing = FALSE;

/* extract command flags */
	output_to_stdout = FALSE;

/* append command flags */
	new_archive			= FALSE;
	update_if_newer		= FALSE;
	delete_after_append = FALSE;
	generic_format		= FALSE;

	remove_temporary_at_error 				= FALSE;
	recover_archive_when_interrupt			= FALSE;
	remove_extracting_file_when_interrupt	= FALSE;
	get_filename_from_stdin					= FALSE;
	ignore_directory						= FALSE;
	verify_mode								= FALSE;

  	noconvertcase							= FALSE;

	extract_directory = NULL;
	xfilec = 257;
}

/* ------------------------------------------------------------------------ */
/* NOTES :			Text File Format										*/
/* GENERATOR		NewLine													*/
/* [generic]		0D 0A													*/
/* [MS-DOS]			0D 0A													*/
/* [OS9][MacOS]		0D														*/
/* [UNIX]			0A														*/
/* ------------------------------------------------------------------------ */
static void
print_tiny_usage_and_exit()
{
/*	printfToBuffer("\
LHarc    for UNIX  V 1.02  Copyright(C) 1989  Y.Tagawa\n\
LHx      for MSDOS V C2.01 Copyright(C) 1990  H.Yoshizaki\n\
LHx(arc) for OSK   V 2.01  Modified     1990  Momozou\n\
LHa      for UNIX  V 1.00  Copyright(C) 1992  Masaru Oki\n\
LHa      for UNIX  V 1.14  Modified     1995  Nobutaka Watazaki\n\
LHa      for UNIX  V 1.14i Modified     2000  Tsugio Okamoto\n\
");
#ifdef WIN32
    printfToBuffer("\
LHa      for Win32 V 1.14f Modified     1999  Takashi Fukui\n\
         based on patch for \"LHa 1.14c for Windows 95\" by Nobuyuki Koganemaru\n");
#endif
	printfToBuffer("\
usage: lha [-]{axelvudmcp[q[num]][vnfodizg012]}[w=<dir>] archive_file [file...]\n\
commands:                           options:\n\
 a   Add(or replace) to archive      q{num} quiet (num:quiet mode)\n\
 x,e EXtract from archive            v  verbose\n\
 l,v List / Verbose List             n  not execute\n\
 u   Update newer files to archive   f  force (over write at extract)\n\
 d   Delete from archive             t  FILES are TEXT file\n");
#ifdef SUPPORT_LH7
	printfToBuffer("\
 m   Move to archive (means 'ad')    o[567] compression method (a/u)\n\
");
#endif
#ifndef SUPPORT_LH7
	printfToBuffer("\
 m   Move to archive (means 'ad')    o  use LHarc compatible method (a/u)\n\
");
#endif
	printfToBuffer("\
 c   re-Construct new archive        w=<dir> specify extract directory (a/u/m/x/e)\n\
 p   Print to STDOUT from archive    d  delete FILES after (a/u/c)\n\
 t   Test file CRC in archive        i  ignore directory path (x/e)\n\
                                     z  files not compress (a/u)\n\
                                     g  Generic format (for compatibility)\n\
                                        or not convert case when extracting\n\
                                     0/1/2 header level (a/u)\n\
");
#ifdef EUC
	printfToBuffer("\
                                     e  TEXT code convert from/to EUC\n\
");

#endif
//	exit(1);*/
}

/* ------------------------------------------------------------------------ */

int LHAmain(int argc, char *argv[])
{
	char           *p, inpbuf[256];
	static char temp[256]={0};

	int i;


	char **av, *m;
	int  ac=4;
	argc = 4;//argc;



	init_variable();		/* Added N.Watazaki */
	temporary_fp = NULL;	// Added by Hanamaru
	g_bCurrentDirectory = TRUE;
	argv[1]="x";
	argv[2]="d:\\test.lzh";
	argv[3]="d:\\unlha";


	
	av = (char **)LocalAlloc( LPTR, sizeof(char*)*argc );
	if (av == NULL)
		return ERROR_ENOUGH_MEMORY;
//		fatal_error2("not enough memory\n",
//					ERROR_ENOUGH_MEMORY);
	for (i=0; i<argc; i++) {
	  if ((av[i] = strdup( argv[i] )) == NULL)
		return ERROR_ENOUGH_MEMORY;
//		fatal_error2("not enough memory\n",
//			ERROR_ENOUGH_MEMORY);
	}

	if (ac < 2){
		print_tiny_usage_and_exit();
		return ERROR_COMMAND_NAME;
	}

	if (strcmp(av[1], "--version") == 0) {
		//print_version();
		//exit(1);
		;
	}

	if (ac < 3) {
		cmd = CMD_LIST;
		av--; /* argv--; */ /* 1999.7.18 */
		ac++; /* argc++; */
		goto work;
	}
	
	m = av[1];

	if (m[0] == '-')
		m++;
	/* commands */
	switch (*m) {
	case 'x':
	case 'e':
		cmd = CMD_EXTRACT;
		break;

	case 'p':
		output_to_stdout = TRUE;
		g_bExtractMem = TRUE;
		cmd = CMD_EXTRACT;
		break;

	case 'c':
		new_archive = TRUE;
		cmd = CMD_ADD;
		break;

	case 'a':
		cmd = CMD_ADD;
		break;

	case 'd':
		cmd = CMD_DELETE;
		break;

	case 'u':
		update_if_newer = TRUE;
		cmd = CMD_ADD;
		break;

	case 'm':
		delete_after_append = TRUE;
		cmd = CMD_ADD;
		break;

	case 'v':
		verbose_listing = TRUE;
		cmd = CMD_LIST;
		break;

	case 'l':
		cmd = CMD_LIST;
		break;

	case 't':
		cmd = CMD_EXTRACT;
		verify_mode = TRUE;
		break;

	default:
		{
		print_tiny_usage_and_exit();
		return 1;
		}

	}

	/* options */
	/* p = &argv[1][1]; */
	p = m+1;
	while ( *p != 0 ) {
		switch ((*p++)) {
		case 'q':
			switch (*p) {
			case '0':
			case '1':
				quiet_mode = *p - '0';
				++p;
				break;
			case '2':
			default:
				quiet = TRUE;
				break;
			}
			break;
		case 'f':
			force = TRUE;
			break;
		case 'p':
			prof = TRUE;
			break;
		case 'v':
			verbose = TRUE;
			break;
		case 't':
			text_mode = TRUE;
			break;
		case 'b':
			g_bMakeBackupFile = TRUE;
			break;
#ifdef EUC
		case 'e':
			text_mode = TRUE;
			euc_mode = TRUE;
			break;
#endif
		case 'n':
			noexec = TRUE;
			break;
		case 'g':
			generic_format = TRUE;
			noconvertcase = TRUE;
            header_level = 0;
			break;
		case 'd':
			delete_after_append = TRUE;
			break;
		case 'o':
			switch (*p) {
			case 0:
				compress_method = LZHUFF1_METHOD_NUM;
				header_level = 0;
				break;
			case '5':
				compress_method = LZHUFF5_METHOD_NUM;
				p++;
				break;
#ifdef SUPPORT_LH7
            case '6':
				compress_method = LZHUFF6_METHOD_NUM;
				p++;
				break;
			case '7':
				compress_method = LZHUFF7_METHOD_NUM;
				p++;
				break;
#endif
			default:
				sprintf(temp, "LHa: error option o%c\n", p[-1]);
				printfToBuffer(temp);
				return 1;
			}
     		break;
		case 'z':
			compress_method = LZHUFF0_METHOD_NUM;	/* Changed N.Watazaki */
			break;
		case 'i':
			ignore_directory = TRUE;
			break;
		case 'w':
			if (*p == '=')
				p++;
			extract_directory = p;
			while (*p)
				p++;
			break;
		case '0':
			header_level = HEADER_LEVEL0;
			break;
		case '1':
			header_level = HEADER_LEVEL1;
			break;
		case '2':
			header_level = HEADER_LEVEL2;
			break;
		default:
			sprintf(temp, "LHa: Unknown option '%c'.\n", p[-1]);
			printfToBuffer(temp);
			return ERROR_COMMAND_NAME;
		}
	}

work:
	/* archive file name */
	archive_name = av[2];

/*	if (!strcmp(archive_name, "-")) {
		if (!isatty(1) && cmd == CMD_ADD)
			quiet = TRUE;
	}
	else {
		if (ac == 3 && !isatty(0)) {
//			get_filename_from_stdin = TRUE;
		}
	}*/

	/* target file name */
	if (0/*get_filename_from_stdin*/) {
		cmd_filec = 0;
		if ((xfilev = (char **)LocalAlloc( LPTR, sizeof(char *) * xfilec)) == NULL)
			fatal_error2("Virtual memory exhausted\n",
				ERROR_ENOUGH_MEMORY);
		while (fgets(inpbuf, sizeof(inpbuf), stdin)) {
		    /* delete \n if it exist */
            i=0; p=inpbuf;
			while (i < sizeof(inpbuf) && p != 0) {
			    if (*p == '\n') {
				    *p = 0;
					break;
				}
				p++; i++;
			}

			if (cmd_filec >= xfilec) {
				xfilec += 256;
				cmd_filev = (char **)LocalReAlloc(xfilev,
						   sizeof(char *) * xfilec, LMEM_ZEROINIT);
				if (cmd_filev == NULL)
					fatal_error2("Virtual memory exhausted\n", 
						ERROR_ENOUGH_MEMORY);
				xfilev = cmd_filev;
			}
			if (strlen(inpbuf) < 1)
				continue;
			if ((xfilev[cmd_filec++] = (char *) strdup(inpbuf)) == NULL)
				fatal_error2("Virtual memory exhausted\n", 
					ERROR_ENOUGH_MEMORY);
		}
		xfilev[cmd_filec] = NULL;
		cmd_filev = xfilev;
	} else {
		if( g_bCurrentDirectory == TRUE ){
//			current_directory = av[3];
			strcpy (current_directory , av[3]);
			cmd_filec = ac - 4;
			if( cmd_filec > 0 ) cmd_filev = av + 4;
		}else{
			cmd_filec = ac - 3;
			if( cmd_filec > 0 ) cmd_filev = av + 3;
		}
	}
	sort_files();

	/* make crc table */
	LHAmake_crctable();

	// fatal_error でここに戻ってくる
	if( setjmp(UnlhaEnd) != 0 ){
		// fatal_error なら戻り値は少なくとも 1
		cmd = CMD_UNKNOWN;
		if(g_errorCode == 0) g_errorCode=1;
	}

	switch (cmd) {
	case CMD_EXTRACT:
		if( g_bCurrentDirectory == TRUE )
			extract_directory=av[3];
		cmd_extract();
		break;
	case CMD_ADD:
		cmd_add();
		break;
	case CMD_LIST:
		cmd_list();
		break;
	case CMD_DELETE:
		cmd_delete();
		break;
	}


#ifndef AVIOD_MEMFREE
	//added by Mr.Hanamaru
	for (i=0; i<argc; i++) 
		if (av[i] != NULL){
			if(cmd_filev==&av[i]) cmd_filev = NULL;
			LocalFree (av[i]);
		}

	if (av != NULL){
		if(cmd_filev==av) cmd_filev = NULL;
		LocalFree(av);
	}

	free_hashbuf ();
	if (cmd_filev != NULL)
		LocalFree (cmd_filev);
#endif


#ifdef USE_PROF
	if (!prof)
		return 0;
//		exit(0);
#endif

	return g_errorCode;
}

/* ------------------------------------------------------------------------ */
/* */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
static void
print_version()
{
	fprintf(stderr, "%s\n", LHA_VERSION);
}

/* ------------------------------------------------------------------------ */
static void
message_1(char *title, char *subject, char *name)
{
	//static char temp[128];
//	fprintf(stderr, "LHa: %s%s ", title, subject);
//	fflush(stderr);
//	sprintf(temp, "LHa: %s%s ", title, subject);
//	printfToBuffer(temp);

/*	if (errno == 0){
	//	printfToBuffer( name );
	//	printfToBuffer( "\n" );
//		fprintf(stderr, "%s\n", name);
	}else{
		//perror(name);
	}*/
}

/* ------------------------------------------------------------------------ */
void
message(char *subject, char *name)
{
	message_1("", subject, name);
}

/* ------------------------------------------------------------------------ */
void
warning(char *subject, char *name)
{
	message_1("Warning: ", subject, name);
}

/* ------------------------------------------------------------------------ */
void
LHAerror(char *subject, char *msg)
{
	message_1("Error: ", subject, msg);
}

/* ------------------------------------------------------------------------ */
void
fatal_error(char *msg)
{
	fatal_error2(msg, 0);
/*
	static char temp[256];

	if(quiet==TRUE){
		longjmp( UnlhaEnd,1 );
	}

	// setjmp が使えるならこの方法
	sprintf(temp, "Fatal error: %s\n", msg);
	printfToBuffer(temp);

	if (remove_temporary_at_error)
		unlink(temporary_name);

	longjmp( UnlhaEnd,1 );
*/
}

/* ------------------------------------------------------------------------ */
void
fatal_error2(char *msg, int error)
{
	static char temp[256];

/*	if(quiet==TRUE){
		longjmp( UnlhaEnd,1 );
	}*/

	// setjmp が使えるならこの方法
	sprintf(temp, "Fatal error: %s\n", msg);
//	printfToBuffer(temp);

	if (remove_temporary_at_error)
		unlink(temporary_name);

	g_errorCode = error;

	//longjmp( UnlhaEnd,1 );
}

/* ------------------------------------------------------------------------ */
void
write_error()
{
	fatal_error2(writting_filename, 
		ERROR_CANNOT_WRITE);
}

/* ------------------------------------------------------------------------ */
void
read_error()
{
	fatal_error(reading_filename);
}

/* ------------------------------------------------------------------------ */
void
interrupt(int signo)
{
	errno = 0;
	message("Interrupted\n", "");

	if (temporary_fp)
		fclose(temporary_fp);
	unlink(temporary_name);
	if (recover_archive_when_interrupt)
		rename(backup_archive_name, archive_name);
	if (remove_extracting_file_when_interrupt) {
		errno = 0;
		message("Removing", writting_filename);
		unlink(writting_filename);
	}
	//signal(SIGINT, SIG_DFL);
	//signal(SIGHUP, SIG_DFL);
#ifndef WIN32
	//kill(getpid(), signo);
#else /* WIN32 */
        return;
#endif /* ! WIN32 */
}

/* ------------------------------------------------------------------------ */
/*																			*/
/* ------------------------------------------------------------------------ */
static int
sort_by_ascii(char **a, char **b)
{
	register char  *p, *q;
	register int    c1, c2;

	p = *a, q = *b;
	if (generic_format) {
		do {
			c1 = *(unsigned char *) p++;
			c2 = *(unsigned char *) q++;
			if (!c1 || !c2)
				break;
			if (islower(c1))
				c1 = toupper(c1);
			if (islower(c2))
				c2 = toupper(c2);
		}
		while (c1 == c2);
		return c1 - c2;
	}
	else {
		while (*p == *q && *p != '\0')
			p++, q++;
		return *(unsigned char *) p - *(unsigned char *) q;
	}
}

/* ------------------------------------------------------------------------ */

extern void
sort_files()
{
#ifndef _WIN32_WCE_EMULATION
	if (cmd_filec > 1)
		qsort(cmd_filev, cmd_filec, sizeof(char *), sort_by_ascii);
#endif
}


/* ------------------------------------------------------------------------ */
char *
xmalloc(int size)
{
	char  *p = (char *)LocalAlloc( LPTR, size);
	if (!p)
		fatal_error2("Not enough memory",
			ERROR_ENOUGH_MEMORY);
	return p;
}

/* ------------------------------------------------------------------------ */
char *
xrealloc(char *old, int size)
{
	char *p = (char *)LocalReAlloc(old, size, LMEM_ZEROINIT);
	if (!p)
		fatal_error2("Not enough memory",
			ERROR_ENOUGH_MEMORY);
	return p;
}

/* ------------------------------------------------------------------------ */
/*								STRING POOL									*/
/* ------------------------------------------------------------------------ */
/*
  string pool :
	+-------------+-------------+------+-------------+----------+
	| N A M E 1 \0| N A M E 2 \0| .... | N A M E n \0|			|
	+-------------+-------------+------+-------------+----------+
	  ^ ^		 ^ buffer+0 buffer+used buffer+size

  vector :
	+---------------+---------------+------------- -----------------+
	| pointer to	| pointer to	| pointer to   ...  pointer to	|
	|  stringpool	|  N A M E 1	|  N A M E 2   ...   N A M E n	|
	+---------------+---------------+-------------     -------------+
	^ malloc base      returned
*/

/* ------------------------------------------------------------------------ */
void
init_sp(struct string_pool *sp)
{
	sp->size = 1024 - 8;	/* any ( >=0 ) */
	sp->used = 0;
	sp->n = 0;
	sp->buffer = (char *) xmalloc(sp->size * sizeof(char));
}

/* ------------------------------------------------------------------------ */
void
add_sp(struct string_pool *sp, char* name, int len)
{
	while (sp->used + len > sp->size) {
		sp->size *= 2;
		sp->buffer = (char *)xrealloc(sp->buffer, 
			sp->size * sizeof(char));
	}
	bcopy(name, sp->buffer + sp->used, len);
	sp->used += len;
	sp->n++;
}

/* ------------------------------------------------------------------------ */
void
finish_sp(register struct string_pool *sp,
		  int *v_count, char ***v_vector)
{
	int             i;
	register char  *p;
	char          **v;

	v = (char **) xmalloc((sp->n + 1) * sizeof(char *));
	*v++ = sp->buffer;
	*v_vector = v;
	*v_count = sp->n;
	p = sp->buffer;
	for (i = sp->n; i; i--) {
		*v++ = p;
		if (i - 1)
			p += strlen(p) + 1;
	}
}

/* ------------------------------------------------------------------------ */
void
free_sp(char **vector)
{
	vector--;
	LocalFree(*vector);		/* free string pool */
	LocalFree(vector);
}


/* ------------------------------------------------------------------------ */
/*							READ DIRECTORY FILES							*/
/* ------------------------------------------------------------------------ */
static          boolean
include_path_p(char *path, char *name)
{
	char           *n = name;
	while (*path)
		if (*path++ != *n++)
			return (path[-1] == '/' && *n == '\0');
	return (*n == '/' || (n != name && path[-1] == '/' && n[-1] == '/'));
}

/* ------------------------------------------------------------------------ */
void
cleaning_files(int *v_filec, char ***v_filev)
{
	char           *flags;
	struct stat     stbuf;

	register char **filev = *v_filev;
	register int    filec = *v_filec;
	register char  *p;
	register int    i, j;

	if (filec == 0)
		return;

	flags = xmalloc(filec * sizeof(char));

	/* flags & 0x01 :	1: ignore */
	/* flags & 0x02 :	1: directory, 0 : regular file */
	/* flags & 0x04 :	1: need delete */

	
	for (i = 0; i < filec; i++)
		if (GETSTAT(filev[i], &stbuf) < 0) {
			flags[i] = 0x04;
//			fprintf(stderr,
//			 "LHa: Cannot access \"%s\", ignored.\n", filev[i]);
		}
		else {
			if (is_regularfile(&stbuf))
				flags[i] = 0x00;
			else if (is_directory(&stbuf))
				flags[i] = 0x02;
#ifdef S_IFLNK
			else if (is_symlink(&stbuf)) /* t.okamoto */
				flags[i] = 0x00;
#endif			
			else {
				flags[i] = 0x04;
//				fprintf(stderr,
//					"LHa: Cannot archive \"%s\", ignored.\n", filev[i]);
			}
		}
	errno = 0;

	for (i = 0; i < filec; i++) {
		p = filev[i];
		/* regular file, not deleted/ignored */
		if ((flags[i] & 0x07) == 0x00) {	
			for (j = i + 1; j < filec; j++) {

				/* regular file, not deleted/ignored */
				if ((flags[j] & 0x07) == 0x00) {

					if (STREQU(p, filev[j]))
						flags[j] = 0x04; /* delete */
				}
			}
		}
		/* directory, not deleted/ignored */
		else if ((flags[i] & 0x07) == 0x02) {

			for (j = i + 1; j < filec; j++) {
				/* regular file, not deleted/ignored */
				if ((flags[j] & 0x07) == 0x00) {
					if (include_path_p(p, filev[j]))
						flags[j] = 0x04; /* delete */
				}
				/* directory, not deleted/ignored */
				else if ((flags[j] & 0x07) == 0x02) {
					if (include_path_p(p, filev[j]))
						flags[j] = 0x04; /* delete */
				}
			}
		}
	}

	for (i = j = 0; i < filec; i++) {
		if ((flags[i] & 0x04) == 0) {
			if (i != j)
				filev[j] = filev[i];
			j++;
		}
	}
	*v_filec = j;

	LocalFree(flags);
}

/* ------------------------------------------------------------------------ */
#ifdef NODIRECTORY
/* please need your imprementation */
boolean
find_files(char *name, int *v_filec, char ***v_filev)
{
	return FALSE;		/* DUMMY */
}

/* ------------------------------------------------------------------------ */
void
free_files(int filec, char **filev)
{
	/* do nothing */
}
/* ------------------------------------------------------------------------ */
#else
boolean
find_files(char *name, int *v_filec, char ***v_filev)
{
	struct string_pool sp;
	char            newname[FILENAME_LENGTH];
	int             len, n;
	DIR            *dirp;
	DIRENTRY       *dp;
	struct stat     tmp_stbuf, arc_stbuf, fil_stbuf;

	strcpy(newname, name);
	len = strlen(name);
	if (len > 0 && newname[len - 1] != '/')
		newname[len++] = '/';

	dirp = opendir(name);
	if (!dirp)
		return FALSE;

	init_sp(&sp);

	GETSTAT(temporary_name, &tmp_stbuf);
	GETSTAT(archive_name, &arc_stbuf);

	for (dp = readdir(dirp); dp != NULL; dp = readdir(dirp)) {
		n = NAMLEN(dp);
		strncpy(newname + len, dp->d_name, n);
		newname[len + n] = '\0';
		if (GETSTAT(newname, &fil_stbuf) < 0)
			continue;
#ifdef NO_INODE
		if ( dp->d_name[0] != '.' ||
			(n != 1 &&
			 (dp->d_name[1] != '.' ||
			  n != 2))  ) {
			add_sp(&sp, newname, len+n+1);
		}
#else		
		if ((dp->d_ino != 0) &&
		/* exclude '.' and '..' */
		    ((dp->d_name[0] != '.') ||
		     ((n != 1) &&
		      ((dp->d_name[1] != '.') ||
		       (n != 2)))) &&
		    ((tmp_stbuf.st_dev != fil_stbuf.st_dev ||
		      tmp_stbuf.st_ino != fil_stbuf.st_ino) &&
		     (arc_stbuf.st_dev != fil_stbuf.st_dev ||
		      arc_stbuf.st_ino != fil_stbuf.st_ino))) {
			add_sp(&sp, newname, len + n + 1);
		}
#endif
	}
	closedir(dirp);
	finish_sp(&sp, v_filec, v_filev);
	if (*v_filec > 1)
		qsort(*v_filev, *v_filec, sizeof(char *), sort_by_ascii);
	cleaning_files(v_filec, v_filev);

	//added by Mr.Hanamaru
	LocalFree(dirp);
	LocalFree(sp.buffer);
	return TRUE;
}

/* ------------------------------------------------------------------------ */
void
free_files(int filec, char **filev)
{
	free_sp(filev);
}
#endif
/* ------------------------------------------------------------------------ */
/*																			*/
/* ------------------------------------------------------------------------ */
/* Build temporary file name and store to TEMPORARY_NAME */
void
build_temporary_name()
{
#ifndef WIN32
#  ifdef TMP_FILENAME_TEMPLATE
	/* "/tmp/lhXXXXXX" etc. */
	if (extract_directory == NULL) {
		strcpy(temporary_name, TMP_FILENAME_TEMPLATE);
	}
	else {
		sprintf(temporary_name, "%s/lhXXXXXX", extract_directory);
	}
#ifdef MKSTEMP
	mkstemp(temporary_name);
#else
	mktemp(temporary_name);
#endif

#  else /* ! TMP_FILENAME_TEMPLATE */
	char           *p, *s;

	strcpy(temporary_name, archive_name);
	for (p = temporary_name, s = (char *) 0; *p; p++)
		if (*p == '/')
			s = p;
	strcpy((s ? s + 1 : temporary_name), "lhXXXXXX");
#ifdef MKSTEMP
	mkstemp(temporary_name);
#else
	mktemp(temporary_name);
#endif

#  endif /* TMP_FILENAME_TEMPLATE */

#else /* WIN32 */
        /* making temporary file in extract_directory, %TEMP%, %TMP% or
           current directory */
#ifdef _WIN32_WCE
 	   char * tmppath = "";
#else
       char * tmppath = getenv("TEMP");
#endif
        memset(temporary_name, '\0', FILENAME_LENGTH);
        if (NULL == extract_directory)
        {
           if (NULL == tmppath)
           {
#ifdef _WIN32_WCE
         	   char * tmppath = "";
#else
               char * tmppath = getenv("TEMP");
#endif
           }
           if (NULL == tmppath)
           {
              strcpy(temporary_name, TMP_FILENAME_TEMPLATE);
           }
           else
           {
              sprintf(temporary_name, "%s\\%s", tmppath,
                      TMP_FILENAME_TEMPLATE);
           }
        }
        else
        {
           sprintf(temporary_name, "%s\\%s", extract_directory,
                   TMP_FILENAME_TEMPLATE);
        }
        mktemp(temporary_name);
        return;
#endif /* ! WIN32 */
}

/* ------------------------------------------------------------------------ */
static void
modify_filename_extention(char *buffer, char *ext)
{
	register char  *p, *dot;

	for (p = buffer, dot = (char *) 0; *p; p++) {
		if (*p == '.')
			dot = p;
		else if (*p == '/')
			dot = (char *) 0;
	}

	if (dot)
		p = dot;

	strcpy(p, ext);
}

/* ------------------------------------------------------------------------ */
/* build backup file name */
void
build_backup_name(char *buffer, char *original)
{
	strcpy(buffer, original);
	/* ".bak" */
	modify_filename_extention(buffer, BACKUPNAME_EXTENTION);

}

/* ------------------------------------------------------------------------ */
void
build_standard_archive_name(char *buffer, char *orginal)
{
	strcpy(buffer, orginal);
	/* ".lzh" */
	modify_filename_extention(buffer, ARCHIVENAME_EXTENTION);
}

/* ------------------------------------------------------------------------ */
/*																			*/
/* ------------------------------------------------------------------------ */
boolean
need_file(char *name)
{
	int             i;

	if (cmd_filec == 0)
		return TRUE;

	for (i = 0; i < cmd_filec; i++) {
		if (patmatch(cmd_filev[i], name, 0))
			return TRUE;
	}

	return FALSE;
}

FILE *xfopen(char *name, char *mode)
{
	FILE           *fp;

	if ((fp = fopen(name, mode)) == NULL)
		fatal_error(name);

	return fp;
}

/* ------------------------------------------------------------------------ */
/*																			*/
/* ------------------------------------------------------------------------ */
static boolean
open_old_archive_1(char *name, FILE **v_fp)
{
	FILE           *fp;
	struct stat     stbuf;

	if (stat(name, &stbuf) >= 0 &&
	    is_regularfile(&stbuf) &&
	    (fp = fopen(name, READ_BINARY)) != NULL) {
		*v_fp = fp;
		archive_file_gid = stbuf.st_gid;
		archive_file_mode = stbuf.st_mode;
		return TRUE;
	}

	*v_fp = NULL;
	archive_file_gid = -1;
	return FALSE;
}

/* ------------------------------------------------------------------------ */
FILE *open_old_archive()
{
	FILE           *fp;
	char           *p;

	if (!strcmp(archive_name, "-")) {
		if (cmd == CMD_EXTRACT || cmd == CMD_LIST)
			return stdin;
		else
			return NULL;
	}
	if (p = (char *) rindex(archive_name, '.')) {
		if (strucmp(".LZH", p) == 0
		    || strucmp(".LZS", p) == 0
		    || strucmp(".COM", p) == 0	/* DOS SFX */
		    || strucmp(".EXE", p) == 0
		    || strucmp(".X", p) == 0	/* HUMAN SFX */
		    || strucmp(".BAK", p) == 0) {	/* for BackUp */
			open_old_archive_1(archive_name, &fp);
			return fp;
		}
	}

	if (open_old_archive_1(archive_name, &fp))
		return fp;
	sprintf(expanded_archive_name, "%s.lzh", archive_name);
	if (open_old_archive_1(expanded_archive_name, &fp)) {
//		archive_name = expanded_archive_name;
		strcpy (archive_name , expanded_archive_name);
		return fp;
	}
	/*
	 * if ( (errno&0xffff)!=E_PNNF ) { archive_name =
	 * expanded_archive_name; return NULL; }
	 */
	sprintf(expanded_archive_name, "%s.lzs", archive_name);
	if (open_old_archive_1(expanded_archive_name, &fp)) {
//		archive_name = expanded_archive_name;
		strcpy (archive_name , expanded_archive_name);
		return fp;
	}
	/*
	 * if ( (errno&0xffff)!=E_PNNF ) { archive_name =
	 * expanded_archive_name; return NULL; }
	 */
	/*
	 * sprintf( expanded_archive_name , "%s.lzh",archive_name);
	 * archive_name = expanded_archive_name;
	 */
	return NULL;
}

/* ------------------------------------------------------------------------ */
//inquire(msg, name, selective)
//	char           *msg, *name, *selective;
int inquire(char *name, time_t t, unsigned long unixtime)
{
//	char            buffer[1024];
//	char           *p;
/*
	for (;;) {
//		fprintf(stderr, "%s %s ", name, msg);
		fflush(stderr);

		fgets(buffer, 1024, stdin);

		for (p = selective; *p; p++)
			if (buffer[0] == *p)
				return p - selective;
	}
*/
	/* 実質上書き確認だけなのでメッセージを固定 */
	return 1;//createInquireDialog(name, t, unixtime);

	/* NOTREACHED */
	
}

/* ------------------------------------------------------------------------ */
void
write_archive_tail(FILE *nafp)
{
	putc(0x00, nafp);
}

/* ------------------------------------------------------------------------ */
void
copy_old_one(FILE *oafp, FILE *nafp, LzHeader *hdr)
{
	if (noexec) {
		fseek(oafp, (long) (hdr->header_size + 2) + hdr->packed_size, SEEK_CUR);
	}
	else {
		reading_filename = archive_name;
		writting_filename = temporary_name;
		if (hdr->header_level != 2) {
			copyfile(oafp, nafp,
					 (long) (hdr->header_size + 2) + hdr->packed_size, 0);
		} else {
			copyfile(oafp, nafp,
					 (long) (hdr->header_size) + hdr->packed_size, 0);
		}
    }
}

/* Local Variables: */
/* mode:c */
/* tab-width:4 */
/* compile-command:"gcc -c lharc.c" */
/* End: */
