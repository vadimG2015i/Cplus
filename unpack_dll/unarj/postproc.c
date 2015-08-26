/*
 * $Id: postproc.c,v 1.1.1.1 2002/03/28 00:03:13 andrew_belov Exp $
 * ---------------------------------------------------------------------------
 * This  program  stores  the  CRC and  file  size of  ARJ.EXE  in it,  so  an
 * integrity check may be performed with ARJ i.
 *
 */

#include "arj.h"

/* Operations */

#define PP_NONE                    0    /* No action */
#define PP_DEFAULT                 1    /* Calculate and store the CRC */
#define PP_ARJSFX                  2    /* Remove LZEXE signature */

/* Errorlevels/return codes. If something went wrong, look here */

#define POSTPROC_ERL_SUCCESS       0
#define POSTPROC_ERL_WARNING       1    /* Non-fatal error */
#define POSTPROC_ERL_CANTOPEN      2    /* Can't open file */
#define POSTPROC_ERL_BAD_EXE       3    /* Malformed EXE file */
#define POSTPROC_ERL_NO_INTEGR     4    /* Integrity pattern not found */
#define POSTPROC_ERL_CANT_WRITE    5    /* Data was not written */

/* Patterns in EXE files */

static char reg_crc_pattern[]={0xB0, 0x01, 0xB0, 0x02, 0xB0, 0x03, 0xB0,
                               0x04, 0xB0, 0x05, 0};
static char encryption_pattern[]={0xB0, 0x02, 0xB0, 0x02, 0xB0, 0x03, 0xB0,
                                 0x04, 0xB0, 0x05, 0};
static char integrity_pattern[]={0xB0, 0x03, 0xB0, 0x02, 0xB0, 0x03, 0xB0,
                                 0x04, 0xB0, 0x05, 0};
static unsigned short self_check[]={0x9090, 0x9090, 0x138, 0x9090, 0x9090};

/* Processing buffer */

static char buf[PROC_BLOCK_SIZE];

/* Standard postprocessing for ARJ and REARJ */

static int pp_default(FILE *stream)
{
 int rp_len;
 long cur_pos, wr_pos;
 int bytes_read, byte_ctr;
 char *pb_ptr;
 long fsize;
 int c;

 fseek(stream, 0L, SEEK_END);
 fsize=ftell(stream);
 fseek(stream, 0L, SEEK_SET);
 rp_len=strlen(integrity_pattern);
 cur_pos=0L;
 while(1)
 {
  fseek(stream, cur_pos, SEEK_SET);
  if((bytes_read=fread(buf, 1, PROC_BLOCK_SIZE, stream))==0)
  {
   printf("Patch not found\n");
   return(POSTPROC_ERL_BAD_EXE);
  }
  bytes_read-=rp_len;
  pb_ptr=buf;
  byte_ctr=0;
  while(byte_ctr<bytes_read)
  {
   if(!memcmp(pb_ptr, integrity_pattern, rp_len))
    break;
   byte_ctr++;
   pb_ptr++;
  }
  if(byte_ctr<bytes_read)
   break;
  cur_pos+=(long)PROC_BLOCK_SIZE/2;     /* Dirty hack II */
 }
 wr_pos=(long)byte_ctr+cur_pos+rp_len;
 crc32term=CRC_MASK;
 fseek(stream, 0L, SEEK_SET);
 for(cur_pos=0L; cur_pos<wr_pos; cur_pos++)
 {
  if((c=fgetc(stream))==-1)
   break;
  crc32term=crc32_for_char(crc32term, (unsigned char)c);
 }
 cur_pos+=8L;
 fseek(stream, cur_pos, SEEK_SET);
 while(cur_pos<fsize)
 {
  if((c=fgetc(stream))==-1)
   break;
  crc32term=crc32_for_char(crc32term, (unsigned char)c);
  cur_pos++;
 }
 fsize+=2L;
 fseek(stream, wr_pos, SEEK_SET);
 fwrite(&crc32term, 1, 4, stream);
 fwrite(&fsize, 1, 4, stream);
 printf("Done\n");
 return(POSTPROC_ERL_SUCCESS);
}

/* LZEXE signature removal from ARJSFX */

static int pp_arjsfx(FILE *stream)
{
 int p_len;
 int sig_found;

 p_len=32;
 sig_found=0;
 if(fread(buf, 1, p_len, stream)<p_len)
 {
  printf("Malformed executable, exiting");
  return(POSTPROC_ERL_BAD_EXE);
 }
 if(buf[28]=='L'&&buf[29]=='Z')
 {
  if(buf[30]=='0'&&buf[31]=='9')
   printf("LZEXE v 0.90");
  else if(buf[30]=='9'&&buf[31]=='0')
   printf("LZEXE v 0.90 (Russian hack-up)");
  else if(buf[30]=='9'&&buf[31]=='1')
   printf("LZEXE v 0.91");
  else
  {
   printf("No replaceable signature found (normal for non-LZEXE packers).\n");
   return(POSTPROC_ERL_SUCCESS);
  }
  printf(" signature found - ");
  memcpy(buf+28, "RJSX", 4);
  sig_found=1;
 }
 if(sig_found)
 {
  fseek(stream, 0L, SEEK_SET);
  fwrite(buf, 1, p_len, stream);
  printf("replaced\n");
 }
 return(POSTPROC_ERL_SUCCESS);
}

/* ARJSFX packer */

#ifndef NP_SFX
static void pack_sfx(char *name)
{
 char *p;
 #if TARGET==DOS
  char *e;
 #endif
 char path[CCHMAXPATH];
 char cmd_text[CMDLINE_MAX];

 strcpy(path, name);
 if((p=strrchr(path, PATHSEP_DEFAULT))!=NULL)
  *p='\0';
 else
 {
  p=path;
  p[0]='\0';                            /* Weird case, don't care about it */
 }
 p++;
 #if TARGET==DOS
  sprintf(cmd_text, "LZEXE %s", name);
 #elif TARGET==OS2&&defined(LIBC)
  /* LXLITE must run at regular priority and flush the data to STDOUT */
  sprintf(cmd_text, "LXLITE /CS /BDX- /I- /MF3 %s", name);
 #else
  cmd_text[0]='\0';
 #endif
 if(cmd_text[0]!='\0')
  system(cmd_text);
 /* Relocation and cleanup for LZEXE-packed files */
 #if TARGET==DOS
  sprintf(cmd_text, "copy %s %s", p, name);
  system(cmd_text);
  unlink(p);
  strcpy(path, name);
  e=strrchr(path, '.');
  if(e!=NULL)
   strcpy(e, ".OLD");
  unlink(path);
 #endif
}
#endif

/* Main routine */

void main(int argc, char **argv)
{
 FILE *stream;
 int pp_type=PP_NONE;
 int rc;

 printf("POSTPROC v 1.22a  [27/08/2001]  Not a part of any binary package!\n\n");
 if(argc<2)
 {
  printf("Usage: POSTPROC <target> [-sfx],\n"
         "Where: <target> is the EXE name to patch,\n"
         "           -sfx does ARJSFX postprocessing (packing and signing)\n"
         "       e.g, to patch ARJ.EXE, type POSTPROC ARJ.EXE\n");
  exit(POSTPROC_ERL_WARNING);
 }
 build_crc32_table();
 /* Determine the type of post-processing */
 if(argc==2)
  pp_type=PP_DEFAULT;
 else if(!strcmp(argv[2], "-sfx"))
  pp_type=PP_ARJSFX;
 /* Pack the SFX */
 #ifndef NP_SFX
  if(pp_type==PP_ARJSFX)
   pack_sfx(argv[1]);
 #endif
 if((stream=fopen(argv[1], m_rbp))==NULL)
 {
  printf("Can't open %s\n", argv[1]);
  exit(POSTPROC_ERL_CANTOPEN);
 }
 /* Run the corresponding routine */
 switch(pp_type)
 {
  case PP_DEFAULT:  rc=pp_default(stream); break;
  case PP_ARJSFX:   rc=pp_arjsfx(stream); break;
  default:
   printf("No postprocessing action specified\n");
   rc=POSTPROC_ERL_WARNING;
 }
 fclose(stream);
 exit(rc);
}
