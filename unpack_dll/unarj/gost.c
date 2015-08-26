/*
 * $Id: gost.c,v 1.1.1.1 2002/03/28 00:02:55 andrew_belov Exp $
 * ---------------------------------------------------------------------------
 * This file  contains basic  GOST encryption-related initialization  data and
 * code.
 *
 */

#include "arj.h"

DEBUGHDR(__FILE__)                      /* Debug information block */

/* Makes a 1024-byte pattern for encryption */

void calc_gost_pattern()
{
 int i;

 for(i=0; i<256; i++)
 {
  pattern[0][i]=(seed[0][i>>4]<<4)|(seed[1][i&0x0F]);
  pattern[1][i]=(seed[2][i>>4]<<4)|(seed[3][i&0x0F]);
  pattern[2][i]=(seed[4][i>>4]<<4)|(seed[5][i&0x0F]);
  pattern[3][i]=(seed[6][i>>4]<<4)|(seed[7][i&0x0F]);
 }
}

/* Basic step of GOST encoding */

unsigned long gost_term(unsigned long data)
{
 unsigned char *dptr;
 unsigned short p1, p2;
 unsigned short hi, lo;

 dptr=(unsigned char *)&data;
 p1=(pattern[0][dptr[3]]<<8)+pattern[1][dptr[2]];
 p2=(pattern[2][dptr[1]]<<8)+pattern[3][dptr[0]];
 hi=(p1<<GSH_BITS)|(p2>>(16-GSH_BITS));
 lo=(p2<<GSH_BITS)|(p1>>(16-GSH_BITS));
 return(((unsigned long)hi<<16)+(unsigned long)lo);
}

