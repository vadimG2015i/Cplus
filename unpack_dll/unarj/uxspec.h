/*
 * $Id: uxspec.h,v 1.1.1.2 2002/03/28 00:03:28 andrew_belov Exp $
 * ---------------------------------------------------------------------------
 * Prototypes of the functions located in UXSPEC.C are declared here.
 *
 */

#ifndef UXSPEC_INCLUDED
#define UXSPEC_INCLUDED

#define UXSTATS_SHORT              0    /* Short statistics */
#define UXSTATS_LONG               1    /* Long statistics (for ARJ L) */

/* set_uxspecial() return codes */

#define UXSPEC_RC_ERROR            1    /* Misc. errors */
#define UXSPEC_RC_FOREIGN_OS       2    /* Reports structure mismatches */
#define UXSPEC_RC_NOLINK           3    /* Links not supported */
#define UXSPEC_RC_SUPPRESSED       4    /* Attribute suppressed */

unsigned int get_uxspecial_size(char FAR *blk);
int query_uxspecial(char FAR **dest, char *name, struct file_properties *props);
int set_uxspecial(char FAR *storage, char *name);
void uxspecial_stats(char FAR *storage, int format);
unsigned int get_owner_size(char FAR *blk);
int query_owner(char FAR **dest, char *name, int resolve);
int set_owner(char FAR *storage, char *name, int resolve);
void owner_stats(char FAR *storage, int resolve);

#endif

