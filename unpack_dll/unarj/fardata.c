/*
 * $Id: fardata.c,v 1.1.1.1 2002/03/28 00:02:52 andrew_belov Exp $
 * ---------------------------------------------------------------------------
 * This file contains routines dealing with far data segment and CRC.
 *
 */

#include "arj.h"
#ifdef TILED
#include <dos.h>                        /* Weird, eh? */
#endif

DEBUGHDR(__FILE__)                      /* Debug information block */

#if SFX_LEVEL>=ARJ

/* Checks if the error can have an error code or not */

static int is_std_error(FMSG *errmsg)
{
 return(errmsg==M_DISK_FULL||errmsg==M_CANT_DELETE||errmsg==M_CANTOPEN||
        errmsg==M_CANTRENAME||errmsg==M_CANTREAD||errmsg==M_CANT_DELETE||
        errmsg==M_CANT_COPY_TEMP)?1:0;
}

#endif

/* Makes various cleanup depending on the error message received and quits. */

int error_proc(FMSG *errmsg, ...)
{
 char *tmp_errmsg;
 va_list marker;

 #if SFX_LEVEL>=ARJ
  /* Check if the message could have a standard error code */
  if(errno!=0&&is_std_error(errmsg))
  {
   msg_fprintf(new_stdout, lf);
   error_freport();
  }
 #endif
 #if SFX_LEVEL>=ARJSFXV
  if(quiet_mode==ARJ_QUIET1)
   freopen(dev_con, m_w, stdout);
 #endif
 #if SFX_LEVEL>=ARJ
  file_settype(stdout, ARJT_TEXT);
 #endif
 /* For SFX archives, don't forget to display our logo */
 #if SFX_LEVEL==ARJSFXV
  show_sfx_logo();
 #elif SFX_LEVEL==ARJSFX
  if(!logo_shown)
  {
   msg_printf(M_ARJSFX_BANNER, exe_name);
   msg_printf(M_PROCESSING_ARCHIVE, archive_name);
  }
 #endif
 #if SFX_LEVEL>=ARJ
  nputlf();
 #elif SFX_LEVEL>=ARJSFXV
  fputc(LF, new_stdout);
 #else
  fputc(LF, stdout);
 #endif
 /* Format and print the error message */
 tmp_errmsg=malloc_fmsg(errmsg);
 va_start(marker, errmsg);
 #if SFX_LEVEL>=ARJSFXV
  vfprintf(new_stdout, tmp_errmsg, marker);
 #else
  vprintf(tmp_errmsg, marker);
 #endif
 va_end(marker);
 free_fmsg(tmp_errmsg);
 #if SFX_LEVEL>=ARJ
  nputlf();
 #elif SFX_LEVEL>=ARJSFXV
  fputc(LF, new_stdout);
 #else
  fputc(LF, stdout);
 #endif
 /* Terminate the execution with a specific errorlevel */
 #if SFX_LEVEL>=ARJSFXV
  /* If there's no errorlevel yet, select errorlevel by message class */
  if(errorlevel==0)
   errorlevel=subclass_errors(errmsg);
  /* If the error was the lack of memory, display final memory statistics to
     find memory leaks */
  #if SFX_LEVEL>=ARJ
   if(errorlevel==ARJ_ERL_NO_MEMORY)
    mem_stats();
  #endif
  error_occured=1;
  return(errorlevel);
 #elif defined(REARJ)
  return(REARJ_ERL_WARNING);
 #elif defined(REGISTER)
  return(REGISTER_ERL_ERROR);
 #elif SFX_LEVEL>=ARJSFX
  return(ARJSFX_ERL_ERROR);
 #else
  return(1);
 #endif
 return(0);
}

#ifdef FMSG_ST

/* A printf() function for far strings */

int msg_printf(FMSG *fmt, ...)
{
 va_list marker;
 char *storage;
 int result;

 if ((storage=malloc_far_str(fmt))==NULL) 
 {  if ( (storage=malloc(strlen((const char*)fmt)))==NULL)
	 return 0 ;
 }
 va_start(marker, fmt);
 result=vfprintf(new_stdout, storage, marker);
 va_end(marker);
 free(storage);
 return(result);
}

/* A fprintf() function for far strings */

int msg_fprintf(FILE *stream, FMSG *fmt, ...)
{
 va_list marker;
 char *storage;
 int result;

 storage=malloc_far_str(fmt);
 va_start(marker, fmt);
 result=vfprintf(stream, storage, marker);
 va_end(marker);
 free(storage);
 return(result);
}

/* A sprintf() function for far strings */

int msg_sprintf(char *str, FMSG *fmt, ...)
{
 va_list marker;
 char *storage;
 int result;

 storage=(char *)malloc_far_str(fmt);
 va_start(marker, fmt);
 result=vsprintf(str, storage, marker);
 va_end(marker);
 free(storage);
 return(result);
}

#endif

#if SFX_LEVEL>=ARJSFX&&defined(TILED)

/* A model-independent movedata() function (it must go to ENVIRON.C) */

void far_memmove(char FAR *dest, char FAR *src, int length)
{
// movedata(FP_SEG(src), FP_OFF(src), FP_SEG(dest), FP_OFF(dest), length);
}

#endif

#if SFX_LEVEL>=ARJ

/* Initializes CRC32 subsystem (only used by main()) */

void init_crc()
{
 build_crc32_table();
}

/* Returns CRC32 for the given block */

void crc_for_block(char *block, unsigned int length)
{
 crc32_for_block(block, length);
}

/* Returns CRC32 for the given string */

void crc_for_string(char *str)
{
 crc32_for_string(str);
}

#endif
