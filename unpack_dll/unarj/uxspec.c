/*
 * $Id: uxspec.c,v 1.1.1.2 2002/03/28 00:03:28 andrew_belov Exp $
 * ---------------------------------------------------------------------------
 * This module handles the UNIX special files and the owner IDs.
 *
 */

#include "arj.h"

#if TARGET==UNIX
 #include <sys/stat.h>
 #include <pwd.h>
 #include <grp.h>
#endif

DEBUGHDR(__FILE__)                      /* Debug information block */

/* UXSPECIAL block types */

#define UXSB_FIFO               0x00
#define UXSB_HLNK               0x01
#define UXSB_LNK                0x02
#define UXSB_BLK                0x03
#define UXSB_CHR                0x04
#define UXSB_ID_BITS               3    /* Bits for block ID */
#define UXSB_SIZE_BITS (8-UXSB_ID_BITS) /* Bits for block size */
#define UXSB_SIZE_THRESHOLD ((1<<UXSB_SIZE_BITS)-1)  /* When to switch to 2-byte fmt. */
#define MK_UXSB(id, size) (((unsigned char)(id)<<UXSB_SIZE_BITS)|(unsigned char)(size))
#define UXSB_GET_ID(c) ((unsigned char)(c)>>UXSB_SIZE_BITS)
#define UXSB_GET_SIZE(c) ((unsigned char)(c)&((1<<UXSB_SIZE_BITS)-1))
#define UXSB_CALC_SIZE(s) (s+((s<UXSB_SIZE_THRESHOLD)?1:3))

/* Given a raw UXSPECIAL block, reports its size */

unsigned int get_uxspecial_size(char FAR *blk)
{
 int l;

 l=UXSB_GET_SIZE(blk[0]);
 if(l==UXSB_SIZE_THRESHOLD)
  return(mget_word(blk+1)+3);
 else
  return(l+1);
}

/* Fills in the header size fields, returning a pointer to the data area */

static char FAR *fill_hdr_size(char FAR *dest, int type, int size)
{
 if(size<UXSB_SIZE_THRESHOLD)
 {
  mput_byte(MK_UXSB(type, size), dest);
  return(dest+1);
 }
 else
 {
  mput_byte(MK_UXSB(type, UXSB_SIZE_THRESHOLD), dest);
  mput_word(size, dest+1);
  return(dest+3);
 }
}

/* Stores the UNIX special file data in archive fields */

#if SFX_LEVEL>=ARJ
int query_uxspecial(char FAR **dest, char *name, struct file_properties *props)
{
 #if TARGET==UNIX
  struct stat st;
  char tmp_name[FILENAME_MAX-1];
  int l;
  char FAR *dptr;
  int hardlink=0;

  if(lstat(name, &st)==-1)
   return(-1);
  if(!S_ISDIR(st.st_mode)&&st.st_nlink>1&&props->islink)
   hardlink=1;
  if(S_ISFIFO(st.st_mode))
   mput_byte(MK_UXSB(UXSB_FIFO, 0), (*dest=(char FAR *)farmalloc_msg(1)));
  else if(S_ISLNK(st.st_mode)||hardlink)
  {
   if(hardlink)
   {
    retrieve_entry(tmp_name, NULL, &flist_main, props->hl_search.ref);
    l=strlen(tmp_name);
   }
   else
   {
    if((l=readlink(name, tmp_name, sizeof(tmp_name)))<=0)
     return(-1);
   }
   *dest=(char FAR *)farmalloc_msg(UXSB_CALC_SIZE(l));
   dptr=fill_hdr_size(*dest, hardlink?UXSB_HLNK:UXSB_LNK, l);
   far_memmove(dptr, (char FAR *)tmp_name, l);
  }
  else if(S_ISCHR(st.st_mode)||S_ISBLK(st.st_mode))
  {
   *dest=(char FAR *)farmalloc_msg(UXSB_CALC_SIZE(sizeof(st.st_rdev)));
   dptr=fill_hdr_size(*dest, S_ISCHR(st.st_mode)?UXSB_CHR:UXSB_BLK, sizeof(st.st_rdev));
   far_memmove(dptr, (char FAR *)&st.st_rdev, sizeof(st.st_rdev));
  }
  else
   return(-1);                          /* Unusual file type, report warning */
 #else
  return(-1);
 #endif
 return(0);
}
#endif

/* Restores the UNIX special file data */

int set_uxspecial(char FAR *storage, char *name)
{
 #if TARGET==UNIX
  char FAR *dptr;
  int l, id;
  char tmp_name[FILENAME_MAX];
  int rc;

  unlink(name);
  l=UXSB_GET_SIZE(storage[0]);
  if(l==UXSB_SIZE_THRESHOLD)
   l=mget_word(storage+1);
  id=UXSB_GET_ID(storage[0]);
  dptr=storage+((UXSB_GET_SIZE(*storage)==UXSB_SIZE_THRESHOLD)?3:1);
  switch(id)
  {
   case UXSB_FIFO:
    rc=mkfifo(name, 0644);
    return(rc?UXSPEC_RC_ERROR:0);
   case UXSB_HLNK:
   case UXSB_LNK:
    #if SFX_LEVEL>=ARJ    
     if(id==UXSB_HLNK&&suppress_hardlinks)
      return(UXSPEC_RC_SUPPRESSED);
    #endif
    if(l>=sizeof(tmp_name))
     l=sizeof(tmp_name)-1;
    far_memmove((char FAR *)tmp_name, dptr, l);
    tmp_name[l]='\0';
    rc=(id==UXSB_HLNK)?link(tmp_name, name):symlink(tmp_name, name);
    if(!rc)
     return;
    return(errno==EPERM?UXSPEC_RC_NOLINK:UXSPEC_RC_ERROR);
   case UXSB_BLK:
   case UXSB_CHR:
    /* Check for platform mismatch */
    if(sizeof(dev_t)!=l)
     return(UXSPEC_RC_FOREIGN_OS);
    rc=mknod(name, 0644|((id==UXSB_BLK)?S_IFBLK:S_IFCHR), *(dev_t FAR *)dptr);
    return(rc?UXSPEC_RC_ERROR:0);
  }
 #else
  return(UXSPEC_RC_ERROR);
 #endif
}

/* Statistics report */

void uxspecial_stats(char FAR *storage, int format)
{
 #if TARGET==UNIX
  FMSGP fm;
 #endif
 #if SFX_LEVEL>=ARJ
  char tmp[FILENAME_MAX-1];
  char FAR *dptr;
  int i, l, m, id;
 #endif

 if(format==UXSTATS_SHORT)
 {
  /* Only relevant under UNIX when extracting the files */
#if TARGET==UNIX  
  switch(UXSB_GET_ID(storage[0]))
  {
   case UXSB_FIFO:
    fm=M_UXSPECIAL_FIFO;
    break;
   case UXSB_HLNK:
    fm=M_UXSPECIAL_HLNK;
    break;
   case UXSB_LNK:
    fm=M_UXSPECIAL_LNK;
    break;
   case UXSB_CHR:
    fm=M_UXSPECIAL_CHR;
    break;
   case UXSB_BLK:
    fm=M_UXSPECIAL_BLK;
    break;
   default:
    return;
  }
  msg_fprintf(new_stdout, fm);
  fputc(' ', new_stdout);
#endif
 }
#if SFX_LEVEL>=ARJ
 else
 {
  l=UXSB_GET_SIZE(storage[0]);
  if(l==UXSB_SIZE_THRESHOLD)
   l=mget_word(storage+1);
  id=UXSB_GET_ID(storage[0]);
  dptr=storage+((UXSB_GET_SIZE(*storage)==UXSB_SIZE_THRESHOLD)?3:1);
  switch(id)
  {
   case UXSB_FIFO:
    arj_user_msg(M_UXLIST_FIFO);
    break;
   case UXSB_HLNK:
   case UXSB_LNK:
    if(l>=sizeof(tmp))
     l=sizeof(tmp)-1;
    far_memmove((char FAR *)tmp, dptr, l);
    tmp[l]='\0';
    msg_sprintf(misc_buf, (id==UXSB_HLNK)?M_UXLIST_HLNK:M_UXLIST_LNK, tmp);
    arj_user_msg(misc_buf);
    break;
   case UXSB_BLK:
   case UXSB_CHR:
    m=0;
    tmp[0]='\0';
    for(i=0; i<l&&m<sizeof(tmp)-4; i++)
     m+=sprintf(tmp+m, "%02x ", (unsigned char)dptr[i]);
    if(m>0)
     tmp[m-1]='\0';
    msg_sprintf(misc_buf, (id==UXSB_BLK)?M_UXLIST_BLK:M_UXLIST_CHR, tmp);
    arj_user_msg(misc_buf);
    break;
  }
 }
#endif
}

/* Given a raw UXSPECIAL block, reports its size */

unsigned int get_owner_size(char FAR *blk)
{
 return(*(unsigned char *)blk+1);
}

/* Queries the file owner */

#if SFX_LEVEL>=ARJ
int query_owner(char FAR **dest, char *name, int resolve)
{
 #if TARGET==UNIX
  struct passwd *pw;
  struct stat st;
  unsigned int l;

  if(lstat(name, &st)==-1)
   return(-1);
  if(resolve)
  {
   if((pw=getpwuid(st.st_uid))==NULL)
    return(-1);
   l=strlen(pw->pw_name);
   if(l>=256)
    return(-1);
   *dest=(char FAR *)farmalloc_msg(l+1);
    *dest[0]=(unsigned char)l;
   far_memmove(*dest+1, (char FAR *)pw->pw_name, l);
  }
  else
  {
   *dest=(char FAR *)farmalloc_msg(9);
   *dest[0]=8;
   mput_dword((unsigned long)st.st_uid, *dest+1);
   mput_dword((unsigned long)st.st_gid, *dest+5);
  }
  return(0);
 #else
  return(-1);
 #endif
}
#endif

/* Restores the file properties */

int set_owner(char FAR *storage, char *name, int resolve)
{
 #if TARGET==UNIX
  struct passwd *pw;
  char tmp[256];
  int l;

  l=*(unsigned char FAR *)storage;
  if(resolve)
  {
   far_memmove((char FAR *)tmp, storage+1, l);
   tmp[l]='\0';
   if((pw=getpwnam(tmp))==NULL)
    return(-1);
   return(lchown(name, pw->pw_uid, pw->pw_gid));
  }
  else
  {
   if(l!=8)
    return(-1);
   return(lchown(name, mget_dword(storage+1), mget_dword(storage+5)));
  }
 #else
  return(-1);
 #endif
}

/* Report the owner */

#if SFX_LEVEL>=ARJ
void owner_stats(char FAR *storage, int resolve)
{
 char tmp[256];
 int l;

 l=*(unsigned char FAR *)storage;
 if(resolve)
 {
  far_memmove((char FAR *)tmp, storage+1, l);
  tmp[l]='\0';
 }
 else
 {
  if(l==8)
   msg_sprintf(tmp, M_OWNER_ID, mget_dword(storage+1), mget_dword(storage+5));
  else
   strcpy(tmp, "???");
 }
 msg_sprintf(misc_buf, M_OWNER_LIST, tmp);
 arj_user_msg(misc_buf);
 
}
#endif
