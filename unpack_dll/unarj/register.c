/*
 * $Id: register.c,v 1.1.1.1 2002/03/28 00:03:24 andrew_belov Exp $
 * ---------------------------------------------------------------------------
 * This optional utility writes registration data to ARJ.EXE (or not .EXE).
 *
 */

#include "arj.h"

#include <stdlib.h>

/* Local definitions */

#define REG_BLOCK_SIZE          8192    /* Sequental processing block */

/* Global registration types */

#define REG_TYPE_DEFAULT           0    /* Registers a single user-specified
                                           file */
#define REG_TYPE_ARJ               1    /* Registers ARJ or ARJ/2 package */
#define REG_TYPE_ARJ32             2    /* Registers ARJ32 package */

DEBUGHDR(__FILE__)                      /* Debug information block */

/* Local variables */

static char reg_fname[]="REGISTER.DAT"; /* User registration data */
static char end_marker[]="";
static char *reg_list[]={"arj" EXE_EXTENSION,
                         "arjs" EXE_EXTENSION,
                         "dearj" EXE_EXTENSION,
                         "rearj" EXE_EXTENSION,
                          end_marker};
static char *reg_list_32[]={"arj32" EXE_EXTENSION,
                            "arjs32" EXE_EXTENSION,
                            "rearj32" EXE_EXTENSION,
                            end_marker};
static char integrity_pattern[]={0xB0, 0x03, 0xB0, 0x02, 0xB0, 0x03, 0xB0,
                                 0x04, 0xB0, 0x05, 0};
static char reg_pattern[]="aRj sOfTwArE";
static char reg_pad[STD_REG_LEN];
static char proc_block[REG_BLOCK_SIZE];
static unsigned int file_ctr;

/* Writes registration data to the given file */

static void write_reg_data(char *name)
{
 FILE *stream;
 int rp_len;
 long cur_pos, wr_pos;
 int bytes_read, byte_ctr;
 char *pb_ptr;
 long fsize;
 int c;

 if((stream=fopen(name, m_rbp))==NULL)
  error(M_CANTOPEN, name);
 rp_len=strlen(reg_pattern);
 cur_pos=0L;
 while(1)
 {
  fseek(stream, cur_pos, SEEK_SET);
  if((bytes_read=fread(proc_block, 1, REG_BLOCK_SIZE, stream))==0)
   error(M_PATCH_NOT_FOUND);
  bytes_read-=rp_len;
  pb_ptr=proc_block;
  byte_ctr=0;
  while(byte_ctr<bytes_read)
  {
   if(!memcmp(pb_ptr, reg_pattern, rp_len))
    break;
   byte_ctr++;
   pb_ptr++;
  }
  if(byte_ctr<bytes_read)
   break;
  cur_pos+=(long)REG_BLOCK_SIZE/2;      /* Dirty hack */
 }
 wr_pos=cur_pos+(long)byte_ctr+(long)REG_HDR_SHIFT;
 rp_len=STD_REG_LEN;
 if(fseek(stream, wr_pos, SEEK_SET))
  error(M_CANT_SEEK, name);
 if(fwrite(reg_pad, 1, rp_len, stream)!=rp_len)
  error(M_CANT_WRITE, name);
 fseek(stream, 0L, SEEK_END);
 fsize=ftell(stream);
 fseek(stream, 0L, SEEK_SET);
 rp_len=strlen(integrity_pattern);
 cur_pos=0L;
 while(1)
 {
  fseek(stream, cur_pos, SEEK_SET);
  if((bytes_read=fread(proc_block, 1, REG_BLOCK_SIZE, stream))==0)
   error(M_PATCH_NOT_FOUND);
  bytes_read-=rp_len;
  pb_ptr=proc_block;
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
  cur_pos+=(long)REG_BLOCK_SIZE/2;      /* Dirty hack II */
 }
 wr_pos=(long)byte_ctr+cur_pos+rp_len;
 crc32term=CRC_MASK;
 fseek(stream, 0L, SEEK_SET);
 for(cur_pos=0L; cur_pos<wr_pos; cur_pos++)
 {
  if((c=fgetc(stream))==-1)
   error(M_CANTREAD, name);
  crc32term=crc32_for_char(crc32term, (unsigned char)c);
 }
 cur_pos+=8L;
 fseek(stream, cur_pos, SEEK_SET);
 while(cur_pos<fsize)
 {
  if((c=fgetc(stream))==-1)
   error(M_CANTREAD, name);
  crc32term=crc32_for_char(crc32term, (unsigned char)c);
  cur_pos++;
 }
 fsize+=2L;
 if(fseek(stream, wr_pos, SEEK_SET))
  error(M_CANT_SEEK, name);
 if(fwrite(&crc32term, 1, 4, stream)!=4)
  error(M_CANT_WRITE, name);
 if(fwrite(&fsize, 1, 4, stream)!=4)
  error(M_CANT_WRITE, name);
 fclose(stream);
 msg_printf(M_REGISTER_STAMPED, name);
 msg_printf(M_VERIFY_REGISTRATION, name);
 printf("\n");
 file_ctr++;
}

/* The registration procedure itself */

static void register_proc(char *block)
{
 char *nptr;
 int i;

 strip_lf(block);
 nptr=block;
 for(i=0; i<8; i++)
 {
  nptr=ltrim(nptr);
  *(long *)&reg_pad[i*4]=strtoul(nptr, &nptr, 10);
 }
 nptr=ltrim(nptr);
 for(i=0; *nptr!=' '&&*nptr!='\0'&&i<REG_KEY1_LEN; i++)
  reg_pad[REG_KEY1_SHIFT-REG_HDR_SHIFT+i]=*nptr++;
 reg_pad[REG_KEY1_SHIFT-REG_HDR_SHIFT+i]='\0';
 nptr=ltrim(nptr);
 for(i=0; *nptr!=' '&&*nptr!='\0'&&i<REG_KEY2_LEN; i++)
  reg_pad[REG_KEY2_SHIFT-REG_HDR_SHIFT+i]=*nptr++;
 reg_pad[REG_KEY2_SHIFT-REG_HDR_SHIFT+i]='\0';
 nptr=ltrim(nptr);
 for(i=0; *nptr!='\0'&&i<REG_NAME_LEN; i++)
  reg_pad[REG_NAME_SHIFT-REG_HDR_SHIFT+i]=*nptr++;
 reg_pad[REG_NAME_SHIFT-REG_HDR_SHIFT+i]='\0';
 alltrim(reg_pad+REG_KEY1_SHIFT-REG_HDR_SHIFT);
 if(reg_pad[REG_KEY1_SHIFT-REG_HDR_SHIFT]=='\0')
  error(M_IMPROPER_REG_FMT);
}

/* Main routine */

void main(int argc, char **argv)
{
 int global_reg;
 FILE *stream;
 char reg_source[200];
 int i;

 msg_printf(M_REGISTER_BANNER);
 build_crc32_table();
 if(argc!=2)
  error(M_REGISTER_HELP);
 global_reg=REG_TYPE_DEFAULT;
 file_ctr=0;
 if(!stricmp(argv[1], "-arj"))
  global_reg=REG_TYPE_ARJ;
 else if(!stricmp(argv[1], "-arj32"))
  global_reg=REG_TYPE_ARJ32;
 if((stream=fopen(reg_fname, m_r))==NULL)
  error(M_MISSING_REG_FILE);
 if(fgets(reg_source, sizeof(reg_source), stream)==NULL)
  error(M_CANTREAD, reg_fname);
 fclose(stream);
 register_proc(reg_source);
 if(global_reg==REG_TYPE_DEFAULT)
  write_reg_data(argv[1]);
 else if(global_reg==REG_TYPE_ARJ)
 {
  for(i=0; reg_list[i][0]!='\0'; i++)
  {
   if(file_exists(reg_list[i]))
    write_reg_data(reg_list[i]);
  }
 }
 else if(global_reg==REG_TYPE_ARJ32)
 {
  for(i=0; reg_list_32[i][0]!='\0'; i++)
  {
   if(file_exists(reg_list_32[i]))
    write_reg_data(reg_list_32[i]);
  }
 }
 if(file_ctr==0)
  error(M_REG_FAILED);
 if(file_ctr>1)
  msg_printf(M_REG_TOTALS, file_ctr);
 exit(REGISTER_ERL_SUCCESS);
}
