/*
 * $Id: gost_t.c,v 1.1.1.1 2002/03/28 00:03:01 andrew_belov Exp $
 * ---------------------------------------------------------------------------
 * Provides GOST seed/pattern storage space.
 *
 */

/* GOST source seed and pattern storage space */

unsigned char seed[8][16]={0x01, 0x0F, 0x0D, 0x00, 0x05, 0x07, 0x0A, 0x04,
                           0x09, 0x02, 0x03, 0x0E, 0x06, 0x0B, 0x08, 0x0C,
                           0x0D, 0x0B, 0x04, 0x01, 0x03, 0x0F, 0x05, 0x09,
                           0x00, 0x0A, 0x0E, 0x07, 0x06, 0x08, 0x02, 0x0C,
                           0x04, 0x0B, 0x0A, 0x00, 0x07, 0x02, 0x01, 0x0D,
                           0x03, 0x06, 0x08, 0x05, 0x09, 0x0C, 0x0F, 0x0E,
                           0x06, 0x0C, 0x07, 0x01, 0x05, 0x0F, 0x0D, 0x08,
                           0x04, 0x0A, 0x09, 0x0E, 0x00, 0x03, 0x0B, 0x02,
                           0x07, 0x0D, 0x0A, 0x01, 0x00, 0x08, 0x09, 0x0F,
                           0x0E, 0x04, 0x06, 0x0C, 0x0B, 0x02, 0x05, 0x03,
                           0x05, 0x08, 0x01, 0x0D, 0x0A, 0x03, 0x04, 0x02,
                           0x0E, 0x0F, 0x0C, 0x07, 0x06, 0x00, 0x09, 0x0B,
                           0x0E, 0x0B, 0x04, 0x0C, 0x06, 0x0D, 0x0F, 0x0A,
                           0x02, 0x03, 0x08, 0x01, 0x00, 0x07, 0x05, 0x09,
                           0x04, 0x0A, 0x09, 0x02, 0x0D, 0x08, 0x00, 0x0E,
                           0x06, 0x0B, 0x01, 0x0C, 0x07, 0x0F, 0x05, 0x03};
unsigned char pattern[4][256]={0};

