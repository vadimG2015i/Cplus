/* ------------------------------------------------------------------------ */
/* LHa for UNIX    Directory access routine									*/
/*																			*/
/*		Modified          		Nobutaka Watazaki							*/
/* Emulate opendir(), readdir(), closedir() function for LHa				*/
/*																			*/
/*	Ver. 1.14 	Soruce All chagned				1995.01.14	N.Watazaki		*/
/* ------------------------------------------------------------------------ */

#ifndef DIRBLKSIZ
#define DIRBLKSIZ	512
#endif

/* ------------------------------------------------------------------------ */
/*	Type Definition															*/
/* ------------------------------------------------------------------------ */
struct direct {
	int             d_ino;
	int             d_namlen;
	char            d_name[256];
};

typedef struct {
	int             dd_fd;
	int             dd_loc;
	int             dd_size;
	char            dd_buf[DIRBLKSIZ];
}               DIR;

/* ------------------------------------------------------------------------ */
/*	Functions																*/
/* ------------------------------------------------------------------------ */
#ifdef WIN32
   DIR * opendir(char *);
   struct direct * readdir(register DIR *);
   void closedir(DIR *);
#else /* ! WIN32 */
   extern DIR     		 *opendir();
   extern struct direct *readdir();
   extern int			 closedir();
#endif /* WIN32 */
