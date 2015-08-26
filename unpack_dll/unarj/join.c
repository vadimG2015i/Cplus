/*
 * $Id: join.c,v 1.1.1.1 2002/03/28 00:03:01 andrew_belov Exp $
 * ---------------------------------------------------------------------------
 * This program writes overlay data to ARJ executables.
 *
 */

#include "arj.h"

static char buffer[PROC_BLOCK_SIZE];

void main(int argc, char **argv)
{
 FILE *V1, *V2;
 unsigned long b;
 int i;

 printf("JOIN v 1.20  [29/10/2000]  Not a part of any binary package!\r\n\r\n");
 if(argc>=3)
 {
  if((V1=fopen(argv[1], m_abp))!=NULL)
  {
   if((V2=fopen(argv[2], m_rb))!=NULL)
   {
    fseek(V1, 0, SEEK_END);
    b=ftell(V1);
    fgetc(V1);
    fwrite("ARJ_SFX", 1, 8, V1); fwrite(&b, 4, 1, V1);
    fseek(V2, 0, SEEK_END); b=ftell(V2); fseek(V2, 0, SEEK_SET);
    fwrite(&b, 4, 1, V1);
    /* Now simply copy the file */
    printf("Copying ");
    while((i=fread(buffer, 1, sizeof(buffer), V2))!=0)
    {
     fwrite(buffer, 1, i, V1);
     printf(".");
    }
    printf(" done!\r\n");
    fclose(V2);
   }
   else
    printf("Can't open %s\r\n", argv[1]);
   fclose(V1);
  }
  else
   printf("Can't open ARJ.EXE\r\n");
 }
 else
  printf("Usage: JOIN <target> <overlay>,\r\n"
         "       e.g, to append HELP.ARJ to ARJ.EXE, type JOIN ARJ.EXE HELP.ARJ\r\n");
}
