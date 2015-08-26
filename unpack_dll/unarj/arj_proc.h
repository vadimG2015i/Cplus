/*
 * $Id: arj_proc.h,v 1.1.1.1 2002/03/28 00:01:52 andrew_belov Exp $
 * ---------------------------------------------------------------------------
 * Prototypes of the functions located in ARJ_PROC.C are declared here.
 *
 */

#ifndef ARJ_PROC_INCLUDED
#define ARJ_PROC_INCLUDED
#include<windows.h>
/* Helper macros */

#define mget_byte(p) (*(unsigned char FAR *)(p))
#define mput_byte(c, p) *(unsigned char FAR *)(p)=(unsigned char)(c)

/* Prototypes */

void garble_encode_stub(char *data, int len);
void garble_decode_stub(char *data, int len);
void append_curtime_proc();
void add_pathsep(char *path);
void convert_time_limits();
void parse_arj_sw(int cmd, char *arj_sw, char *dest);
void copy_bytes(unsigned long nbytes);
int translate_path(char *name);
void restart_proc(char *dest);
int search_for_extension(char *name, char *ext_list);
unsigned int get_volfree(unsigned int increment);
unsigned int check_multivolume(unsigned int increment);
void store();
void hollow_encode();
void convert_strtime(struct timestamp *dest, char *str);
int check_integrity(char *name);
void name_to_hdr(char *name);
char *format_filename(char *name);
void analyze_arg(char *sw);
void init();
int is_switch(char *arg);
int preprocess_cmdline(int argc, char *argv[]);
#if SFX_LEVEL>=ARJSFXV||defined(REARJ)
int split_name(char *name, char *pathname, char *filename);
#else
int split_name(char *name);
#endif
int subclass_errors(FMSG *errmsg);
void file_seek(FILE *stream, long offset, int whence);
void search_setup();
int calc_percentage(unsigned long partial, unsigned long total);
void smart_seek(unsigned long position, FILE *stream);
void alltrim(char *cmd);
void unstore(int action);
void hollow_decode(int action);
void pack_mem(struct mempack *mempack);
void unpack_mem(struct mempack *mempack);
void strip_lf(char *str);
char *ltrim(char *str);
unsigned int mget_word(char FAR *p);
unsigned long mget_dword(char FAR *p);
void mput_word(unsigned int w, char FAR *p);
void mput_dword(unsigned long d, char FAR *p);

#endif

