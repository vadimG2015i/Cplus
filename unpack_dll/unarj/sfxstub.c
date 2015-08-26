/*
 * $Id: sfxstub.c,v 1.1.1.2 2002/03/28 00:03:24 andrew_belov Exp $
 * ---------------------------------------------------------------------------
 * This is a portable version of the SFX stub.
 *
 */

#include "arj.h"

DEBUGHDR(__FILE__)                      /* Debug information block */

static char strform[]="%s";

/* Main routine */

void main()
{
 printf(strform, M_SFXSTUB_BANNER);
 printf(strform, M_SFXSTUB_BLURB_1);
 printf(strform, M_SFXSTUB_BLURB_2);
}
