/*
 * $Id: arj.h,v 1.1.1.1 2002/03/28 00:01:13 andrew_belov Exp $
 * ---------------------------------------------------------------------------
 * This file must be included FIRST in all modules. It defines the system-wide
 * equates and types, as well as it does some compiler-specific job.
 *
 */

#ifndef ARJ_INCLUDED
#define ARJ_INCLUDED
//#include "../unpack.h"
#include <windows.h>
#include "list.h"
#include "resource2.h"
#include "bindings.h"
#include "environ.h"
#include "defines.h"
#include "date_sig.h"
#include "arjtypes.h"
#include "filemode.h"
#if SFX_LEVEL!=ARJSFXJR

#include "file_reg.h"
#include "encode.h"
#include "decode.h"
#include "enc_gwy.h"
#include "fardata.h"
#include "arj_user.h"
#include "ext_hdr.h"
#include "arj_arcv.h"
#include "arj_file.h"
#endif
#include "crc32.h"
#if SFX_LEVEL!=ARJSFXJR
#include "exe_sear.h"
#include "chk_fmsg.h"
#include "filelist.h"
#include "arjsec_h.h"
#include "arjsec_l.h"
#include "debug.h"
#include "misc.h"
#include "arj_proc.h"
#include "ea_mgr.h"
#include "uxspec.h"
#include "garble.h"
#endif
#if SFX_LEVEL>=ARJSFXV||defined(ARJDISP)
#include "scrnio.h"
#endif
#if SFX_LEVEL!=ARJSFXJR
#include "ansi.h"
#include "recovery.h"
#include "crc16tab.h"
#include "gost.h"
#include "gost_t.h"
#include "gost40.h"
#endif

#if SFX_LEVEL==ARJ
//#include <msg_arj.h>
#elif SFX_LEVEL==ARJSFXV
#include <msg_sfv.h>
#elif SFX_LEVEL==ARJSFX
//#include <msg_sfx.h>
#elif SFX_LEVEL==ARJSFXJR
#include <msg_sfj.h>
#elif defined(REARJ)
#include <msg_rej.h>
#elif defined(REGISTER)
#include <msg_reg.h>
#elif defined(ARJDISP)
#include <msg_adi.h>
#elif defined(SFXSTUB)
#include <msg_stb.h>
#endif

#if SFX_LEVEL<=ARJSFXV&&SFX_LEVEL>=ARJSFX
#include "arjsfx.h"
#endif

#if SFX_LEVEL>=ARJSFX||defined(REARJ)||defined(REGISTER)
#include "externs.h"
#endif

#ifdef REARJ
#include "rearj.h"
#endif

#if SFX_LEVEL>=ARJ

/* Prototypes */

/*#ifdef __cplusplus  
extern "C" {                                     
#endif // */
struct FileRecord
{
  struct timestamp timefile;
  char * ArjFile;
} *datafile,*datafiletmp;
struct List *listA;
struct Node *node;
struct Node *nodetmp;

void arj_exec_validation();
int ArjInitProc(int argc, char *argv[],const char * Directory);
void create_excl_list(char *names);
void exec_cmd(char *cmd);                                                            


// ...                   
//#undef LIBSPEC   //
                                                                
/*#ifdef __cplusplus    
  }                                                   
#endif // __cplusplus  */



#endif

#endif

