/* ------------------------------------------------------------------------ */
/* LHa for UNIX    Archiver Driver											*/
/*																			*/
/*		Modified          		Nobutaka Watazaki							*/
/*																			*/
/*	Ver. 1.14 	Soruce All chagned				1995.01.14	N.Watazaki		*/
/*	Ver. 1.14i 	Modified and bug fixed			2000.10.06	t.okamoto   	*/
/* ------------------------------------------------------------------------ */
/*
	Included...
		lharc.h		interface.h		slidehuf.h
*/
#include <windows.h>
//#include "..\wce\wcedef.h"

#include <stdio.h>
#include <time.h>
#include <sys/utime.h>
#include <sys/stat.h>
#ifndef WIN32
#  include <sys/file.h>
#endif /* ! WIN32 */

#ifdef WIN32
   #ifndef _WIN32_WCE
      #include <setjmp.h>
   #endif
#endif /* WIN32 */

#include "win32cfg.h"
#include "lha_macro.h"
#include "unlha.h"

struct encode_option {
#if defined(__STDC__) || defined(AIX)
	void            (*output) ();
	void            (*encode_start) ();
	void            (*encode_end) ();
#else
	int             (*output) ();
	int             (*encode_start) ();
	int             (*encode_end) ();
#endif
};

struct decode_option {
	unsigned short  (*decode_c) ();
	unsigned short  (*decode_p) ();
#if defined(__STDC__) || defined(AIX)
	void            (*decode_start) ();
#else
	int             (*decode_start) ();
#endif
};

/* ------------------------------------------------------------------------ */
/*	LHa File Type Definition												*/
/* ------------------------------------------------------------------------ */
struct string_pool {
	int             used;
	int             size;
	int             n;
	char           *buffer;
};

typedef struct LzHeader {
	unsigned char   header_size;
	char            method[METHOD_TYPE_STRAGE];
	long            packed_size;
	long            original_size;
	long            last_modified_stamp;
	unsigned char   attribute;
	unsigned char   header_level;
	char            name[256];
	unsigned short  crc;
	boolean         has_crc;
	unsigned char   extend_type;
	unsigned char   minor_version;

	/* extend_type == EXTEND_UNIX  and convert from other type. */
	time_t          unix_last_modified_stamp;
	unsigned short  unix_mode;
	unsigned short  unix_uid;
	unsigned short  unix_gid;
}  LzHeader;

struct interfacing {
	FILE			*infile;
	FILE			*outfile;
	unsigned long   original;
	unsigned long   packed;
	int             dicbit;
	int             method;
};


/* ------------------------------------------------------------------------ */
/*	Option switch variable													*/
/* ------------------------------------------------------------------------ */
/* command line options (common options) */
EXTERN boolean  quiet;
EXTERN boolean  text_mode;
EXTERN boolean  verbose;
EXTERN boolean  noexec;		/* debugging option */
EXTERN boolean  force;
EXTERN boolean  prof;
EXTERN boolean  delete_after_append;
EXTERN boolean	compress_method;
EXTERN int		header_level;
/* EXTERN int		quiet_mode; */   /* 1996.8.13 t.okamoto */
#ifdef EUC
EXTERN boolean	euc_mode;
#endif

/* list command flags */
EXTERN boolean  verbose_listing;

/* extract/print command flags */
EXTERN boolean  output_to_stdout;

/* add/update/delete command flags */
EXTERN boolean  new_archive;
EXTERN boolean  update_if_newer;
EXTERN boolean  generic_format;

EXTERN boolean	remove_temporary_at_error;
EXTERN boolean	recover_archive_when_interrupt;
EXTERN boolean	remove_extracting_file_when_interrupt;
EXTERN boolean	get_filename_from_stdin;
EXTERN boolean	ignore_directory;
EXTERN boolean	verify_mode;

/* Indicator flag */
EXTERN int		quiet_mode;

/* ------------------------------------------------------------------------ */
/*	Globale Variable														*/
/* ------------------------------------------------------------------------ */
EXTERN char		**cmd_filev;
EXTERN int      cmd_filec;

EXTERN char     current_directory[256]; // added by uema2.
EXTERN char		*archive_name;
EXTERN char     expanded_archive_name[FILENAME_LENGTH];
EXTERN char     temporary_name[FILENAME_LENGTH];
EXTERN char     backup_archive_name[FILENAME_LENGTH];

EXTERN char		*reading_filename, *writting_filename;

/* 1996.8.13 t.okamoto */
#if 0
EXTERN boolean  remove_temporary_at_error;
EXTERN boolean  recover_archive_when_interrupt;
EXTERN boolean  remove_extracting_file_when_interrupt;
#endif

EXTERN int      archive_file_mode;
EXTERN int      archive_file_gid;

EXTERN struct	interfacing interface;
EXTERN LHAnode		*next;
/* EXTERN unsigned short crc; */  /* 1996.8.13 t.okamoto */

EXTERN int      noconvertcase; /* 2000.10.6 */

/* slide.c */
EXTERN int      unpackable;
EXTERN unsigned long origsize, compsize;
EXTERN unsigned short dicbit;
EXTERN unsigned short maxmatch;
EXTERN unsigned long count;
EXTERN unsigned long loc;			/* short -> long .. Changed N.Watazaki */
EXTERN unsigned char *text;
EXTERN int		prev_char;

/* huf.c */
#ifndef LHA_MAIN_SRC  /* t.okamoto 96/2/20 */
EXTERN unsigned short left[], right[];
EXTERN unsigned char c_len[], pt_len[];
EXTERN unsigned short c_freq[], c_table[], c_code[];
EXTERN unsigned short p_freq[], pt_table[], pt_code[], t_freq[];
#endif

/* append.c */
#ifdef NEED_INCREMENTAL_INDICATOR
EXTERN long		indicator_count;
EXTERN long		indicator_threshold;
#endif

/* crcio.c */
EXTERN FILE		*infile, *outfile;
EXTERN unsigned short crc, bitbuf;
EXTERN int      dispflg;
EXTERN long		reading_size;

/* from dhuf.c */
EXTERN unsigned int n_max;

/* lhadd.c */
EXTERN FILE		*temporary_fp;

/* ------------------------------------------------------------------------ */
/*	Functions																*/
/* ------------------------------------------------------------------------ */
#ifndef WIN32
/* from lharc.c */
extern int		patmatch();

extern void		interrupt();

extern void		message();
extern void		warning();
extern void		LHAerror();
extern void		fatal_error();
extern void		fatal_error2();

extern boolean	need_file();
extern int		inquire();
extern FILE		*xfopen();

extern boolean	find_files();
extern void		free_files();

extern void		init_sp();
extern void		add_sp();
extern void		finish_sp();
extern void		free_sp();
extern void		cleaning_files();

extern void		build_temporary_name();
extern void		build_backup_file_name();
extern void		build_standard_archive_name();

extern FILE		*open_old_archive();
//extern void		init_header();
extern void init_header(char * , struct stat * , LzHeader *);
extern boolean	get_header();
extern boolean	archive_is_msdos_sfx1();
extern boolean	skip_msdos_sfx1_code();
extern void		LHAwrite_header();
extern void		write_archive_tail();
extern void		copy_old_one();
extern unsigned char *convdelim();
extern long		copyfile();

extern void		cmd_list(), cmd_extract(), cmd_add(), cmd_delete();

extern boolean	ignore_directory;
extern boolean	compress_method;
extern boolean	verify_mode;

extern char		*extract_directory;

/* from slide.c */

extern int		encode_alloc();
extern void		LHAencode();
extern void		LHAdecode();

/* from append.c */
extern void     start_indicator();
extern void     finish_indicator();
extern void     finish_indicator2();

/* slide.c */
extern void     output_st1();
extern unsigned char *alloc_buf();
extern void     encode_start_st1();
extern void     encode_end_st1();
extern unsigned short decode_c_st1();
extern unsigned short decode_p_st1();
extern void     decode_start_st1();

/* from shuf.c */
extern void     decode_start_st0();
extern void     encode_p_st0( /* unsigned short j */ );
extern void     encode_start_fix();
extern void     decode_start_fix();
extern unsigned short decode_c_st0();
extern unsigned short decode_p_st0();

/* from dhuf.c */
extern void     start_c_dyn();
extern void     decode_start_dyn();
extern unsigned short decode_c_dyn();
extern unsigned short decode_p_dyn();
extern void     output_dyn( /* int code, unsigned int pos */ );
extern void     encode_end_dyn();

extern int      decode_lzhuf();

/* from larc.c */

extern unsigned short decode_c_lzs();
extern unsigned short decode_p_lzs();
extern unsigned short decode_c_lz5();
extern unsigned short decode_p_lz5();
extern void			  decode_start_lzs();
extern void			  decode_start_lz5();

extern void	make_table(	/* int nchar, uchar bitlen[], int tablebits,
							ushort table[] */ );

/* from maketree.c */
/*
 * void make_code(short n, uchar len[], ushort code[]); short make_tree(short
 * nparm, ushort freqparm[], uchar lenparm[], ushort codeparam[]);
 */
extern void		make_code( /* int n, uchar len[], ushort code[] */ );
extern short	make_tree( /* int nparm, ushort freqparm[], uchar lenparm[],
								ushort codeparam[] */ );

/* from crcio.c */
extern void				LHAmake_crctable();
extern unsigned short	calccrc( /* uchar *p, uint n */ );
extern void				LHAfillbuf( /* uchar n */ );
extern unsigned short	getbits( /* uchar n */ );
extern void				putcode( /* uchar n, ushort x */ );
extern void				LHAputbits( /* uchar n, ushort x */ );
extern int				LHAfread_crc( /* uchar *p, int n, FILE *f */ );
extern void				LHAfwrite_crc( /* uchar *p, int n, FILE *f */ );
extern void				init_getbits();
extern void				LHAinit_putbits();
extern void     		LHAmake_crctable();
extern unsigned 		short calccrc();

/* from lhadd.c */
extern int		encode_lzhuf();
extern int      encode_stored_crc();

#else /* WIN32 */
   /* from patmatch.c */
   extern int patmatch(register char * , register char * , int);

   /* from lharc.c */
   extern int unlhaMain(int argc, char *argv[]);
   extern void message(char * , char *);
   extern void warning(char * , char *);
   extern void LHAerror(char * , char *);
   extern void fatal_error(char *);
   extern void fatal_error2(char *, int);

   extern void write_error(void);
   extern void read_error(void);
   extern void interrupt(int);
   extern char * xmalloc(int);
   extern char * xrealloc(char * , int);
   extern void init_sp(struct string_pool *);
   extern void add_sp(struct string_pool * , char * , int);
   extern void finish_sp(register struct string_pool * , int * , char * * *);
   extern void free_sp(char * *);
   extern void cleaning_files(int * , char * * *);
   extern boolean find_files(char * , int * , char * * *);
   extern void free_files(int , char * *);
   extern void build_temporary_name(void);
   extern void build_backup_name(char * , char *);
   extern void build_standard_archive_name(char * , char *);
   extern boolean need_file(char *);
   extern FILE * xfopen(char * , char *);
   extern FILE * open_old_archive(void);
//   extern int inquire(char * , char * , char *);
   extern int inquire(char * , time_t, unsigned long);
   extern void write_archive_tail(FILE *);
   extern void copy_old_one(FILE * , FILE * , LzHeader *);

   /* from append.c */
   extern int encode_lzhuf(FILE * , FILE * , long , long * , long * , char *, char *);
   extern void start_indicator(char * , long , char * , long);
   extern void finish_indicator2(char * , char * , int);
   extern void finish_indicator(char * , char *);

   /* from crcio.c */
   extern void LHAmake_crctable(void);
   extern unsigned short calccrc(unsigned char * , unsigned int);
   extern void LHAfillbuf(unsigned char);
   extern unsigned short getbits(unsigned char);
   extern void putcode(unsigned char , unsigned short);
   extern void LHAputbits(unsigned char , unsigned short);
   extern int LHAfread_crc(unsigned char * , int , FILE *);
   extern void LHAfwrite_crc(unsigned char * , int , FILE *);
   extern void init_code_chache(void);
   extern void init_getbits(void);
   extern void LHAinit_putbits(void);
#  ifdef EUC
      extern void putc_euc(int , FILE *);
#  endif /* EUC */
   extern int fwrite_txt(unsigned char * , int , FILE *);
   extern int fread_txt(unsigned char * , int , FILE *);
   extern unsigned short calc_header_crc(unsigned char * , unsigned int);

   /* from dhuf.c */
   extern void start_c_dyn(void);
   extern void decode_start_dyn(void);
   extern unsigned short decode_c_dyn(void);
   extern unsigned short decode_p_dyn(void);
   extern void output_dyn(unsigned int , unsigned int);
   extern void encode_end_dyn(void);

   /* from extract.c */
   extern int decode_lzhuf(FILE * , FILE * , long , long , char * , int);

   /* from header.c */
   extern int calc_sum(register char * , register int);
   extern boolean get_header(FILE * , register LzHeader *);
   extern void init_header(char * , struct stat * , LzHeader *);
   extern void LHAwrite_header(FILE * , LzHeader *);

   /* from huf.c */
   extern void output_st1(unsigned short , unsigned short);
   extern unsigned char * alloc_buf(void);
   extern void free_buf(void);
   extern void encode_start_st1(void);
   extern void encode_end_st1(void);
   extern unsigned short decode_c_st1(void);
   extern unsigned short decode_p_st1(void);
   extern void decode_start_st1(void);

   /* from larc.c */
   extern unsigned short decode_c_lzs(void);
   extern unsigned short decode_p_lzs(void);
   extern void decode_start_lzs(void);
   extern unsigned short decode_c_lz5(void);
   extern unsigned short decode_p_lz5(void);
   extern void decode_start_lz5(void);

   /* from lhadd.c */
   extern FILE * append_it(char * , FILE * , FILE *);
   extern boolean temporary_to_new_archive_file(long);
   extern void cmd_add(void);
   extern void cmd_delete(void);
   extern int strcmp_filename(char * , char *);

   /* from lhext.c */
   extern void cmd_extract(void);

   /* from lhlist.c */
   extern void cmd_list(void);

   /* from maketbl.c */
   extern void make_table(short , unsigned char [] , short , unsigned short []);

   /* from maketree.c */
   extern void make_code(int , unsigned char [] , unsigned short []);
   extern short make_tree(int , unsigned short [] , unsigned char [] , unsigned short []);

   /* from shuf.c */
   extern void decode_start_st0(void);
   extern void encode_p_st0(unsigned short);
   extern void encode_start_fix(void);
   extern void decode_start_fix(void);
   extern unsigned short decode_c_st0(void);
   extern unsigned short decode_p_st0(void);

   /* from slide.c */
   extern int encode_alloc(int);
   extern void LHAencode(struct interfacing *);
   extern void LHAdecode(struct interfacing *);
   extern void free_hashbuf(void);

   /* from util.c */
   extern long copyfile(FILE * , FILE * , long , int);
   extern int encode_stored_crc(FILE * , FILE * , long , long * , long *);
   extern unsigned char * convdelim(unsigned char * , unsigned char);
   extern boolean archive_is_msdos_sfx1(char *);
   extern boolean skip_msdos_sfx1_code(FILE *);
   extern int strucmp(register char * , register char *);
   /* extern int rename(char * , char *); */

   extern boolean ignore_directory;
   extern boolean compress_method;
   extern boolean verify_mode;
   extern char *  extract_directory;
   extern void init_variable()	;
   extern void sort_files();   
   extern  BOOL compareFileName(int ac, char **av, char *name);
   extern  void extract_one(FILE           *afp,	/* archive file */
	LzHeader       *hdr);
	

#endif /* ! WIN32 */

/* Local Variables: */
/* mode:c */
/* tab-width:4 */
/* End: */
