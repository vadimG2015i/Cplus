// 25beta.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#define _COMPILING_4AFD424F_87A7_419e_832C_F8FC0AA1BC2E
#include "uncab.h"



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	// Perform actions based on the reason for calling.
    switch( ul_reason_for_call) 
    { 
        case DLL_PROCESS_ATTACH:
         // Initialize once for each new process.
         // Return FALSE to fail DLL load.
            break;

        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
         // Do thread-specific cleanup.
            break;

        case DLL_PROCESS_DETACH:
         // Perform any necessary cleanup.
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.



   
}



LIBSPEC  HAND Cab_OpenArchive(const char * cabname)
{

  struct cabinet *basecab, *cab, *cab1, *cab2;
  struct file *filelist, *fi;
  int quiet=0;
  int lower=0;
  int fix=0;

  /* has the list-mode header been seen before? */
  int viewhdr = 0;

 /* if (view || !quiet) {
    printf("%s cabinet: %s\n", view ? "Viewing" : "Extracting", cabname);
  }*/

  /* load the file requested */
  basecab = find_cabs_in_file((char*)cabname);
  if (!basecab) return NULL;

  /* iterate over all cabinets found in that file */
  for (cab = basecab; cab; cab=cab->next) {

    /* bi-directionally load any spanning cabinets -- backwards */
    for (cab1 = cab; cab1->flags & cfheadPREV_CABINET; cab1 = cab1->prevcab) {
      //if (!quiet) printf("%s: extends backwards to %s (%s)\n", cabname,
			 //cab1->prevname, cab1->previnfo);
      find_cabinet_file(&(cab1->prevname), (char*)cabname);
      if (!(cab1->prevcab = load_cab_offset(cab1->prevname, 0))) {
        //fprintf(stderr, "%s: can't read previous cabinet %s\n",
		//cabname, cab1->prevname);
        break;
      }
      cab1->prevcab->nextcab = cab1;
    }

    /* bi-directionally load any spanning cabinets -- forwards */
    for (cab2 = cab; cab2->flags & cfheadNEXT_CABINET; cab2 = cab2->nextcab) {
     // if (!quiet) printf("%s: extends to %s (%s)\n", cabname,
			// cab2->nextname, cab2->nextinfo);
      find_cabinet_file(&(cab2->nextname), (char*)cabname);
      if (!(cab2->nextcab = load_cab_offset(cab2->nextname, 0))) {
       // fprintf(stderr, "%s: can't read next cabinet %s\n",
	//	cabname, cab2->nextname);
        break;
      }
      cab2->nextcab->prevcab = cab2;
    }

    filelist = process_files(cab1);
    CAB(current) = NULL;
  }
	
	return filelist;

}
;


LIBSPEC  BOOL Cab_CloseArchive(HAND Cmd)
{

	return TRUE;
}
;

LIBSPEC   BOOL Cab_GetFirstFile(HAND &Cmd,const char * Directory,char *FileReturn)
{
  int lower=0;
  int fix=0;
  if (Cmd) {

		extract_fileMS(Cmd, lower, fix,(char*) Directory);
		strcpy(FileReturn,Cmd->filename);
        return TRUE;
		}
  return FALSE;

};


LIBSPEC   BOOL  Cab_GetNextFile(HAND &Cmd,const char * Directory,char *FileReturn)
{

        int lower=0;
        int fix=0;
		Cmd = Cmd->next;
		if (Cmd)	
		{
		extract_fileMS(Cmd, lower, fix, (char*)Directory);
        strcpy(FileReturn,Cmd->filename);
        return TRUE;
		}
	 return FALSE;

}

