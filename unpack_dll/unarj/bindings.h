/*
 * $Id: bindings.h,v 1.1.1.1 2002/03/28 00:02:01 andrew_belov Exp $
 * ---------------------------------------------------------------------------
 * This file defines basic constants depending on the given SFX_LEVEL
 *
 */

#ifndef BINDINGS_INCLUDED
#define BINDINGS_INCLUDED

#define C_DEFS_INCLUDED
#include <ctype.h>
//#define _WIN32_WINNT 0x0400

//#define __cplusplus
#include <windows.h>
#undef FAR
#undef NEAR
#define FAR
#define NEAR

/* If the SFX_LEVEL is not given, default to the lowest one ever possible */

#ifndef SFX_LEVEL
 #ifdef SFL
  #define SFX_LEVEL              SFL
 #else
  #define SFX_LEVEL                4//by vadim
 #endif
#endif

/* Bindings (SFX_LEVEL grades) */

#define ARJ                        4
#define ARJSFXV                    3
#define ARJSFX                     2
#define ARJSFXJR                   1

#if SFX_LEVEL>=ARJ
 #define FMSG_ST
 #define FARDATA                 FAR
 #define FARCODE                        /* Just indicates the far code model */
 #define EXTR_LEVEL  ARJ_X_SUPPORTED
#else
 #define FARDATA
 #define EXTR_LEVEL        ARJ_X_SFX
#endif

/* Debug information record. */

#ifdef DEBUG
 #define DEBUGHDR(fname) static char dbg_cur_file[]=fname; \
                         static int dbg_dummy;
#else
 #define DEBUGHDR(fname)
#endif

#endif
