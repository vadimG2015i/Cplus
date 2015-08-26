// unpack.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#define _COMPILING_4AFD424F_87A7_419e_832C_F8FC0AA1BC2E

#include "unpack.h"
#include "unzip/unzip.h"
//#include "unlha/lha/lha.c"
//#include "untar/tar32.h"
//#include "untar/tarcmd.h"
//#include "uncabIS/i6comp.c"

#pragma warning(disable: 4786)
#include <list>
#include <string>
#include <strstream>
#include <fstream>
using namespace std;

#include "untar/fast_stl.h"

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




LIBSPEC HANDZIP	 Zip_OpenArchive(const char * ZipfileName)
{
	HANDZIP file;
	__try 
	   
   {
	file= unzOpen(ZipfileName);
	return file;

   }
    __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   { 
	   return NULL;
   }
  


};
LIBSPEC  BOOL Zip_CloseArchive(HANDZIP ZipFile)
{
	int err;
    __try
	{
	err=unzClose (	ZipFile);
    if (err ==0)return TRUE; else return FALSE;
	}
    __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   { 
	   return FALSE;
   }
  

}
;
LIBSPEC   BOOL  Zip_GetFirstFile(HANDZIP ZipFile,const char * Directory,char *FileReturn)
{
	const int  opt_withoutpath=0;	
    char *filename=(char*)malloc(256);
	char result[256]="";
	;
	int opt_owerwrite=1 ;
	int err;
 __try
	{
	if ((err=unzGoToFirstFile (ZipFile))==UNZ_OK)
	{
	   err=	do_extract_currentfile( ZipFile,
		Directory,filename,
		&opt_withoutpath,&opt_owerwrite);

    strcpy(result,filename);
   	if (filename) free (filename);

	if ( err==UNZ_OK)
	{
	strcpy(FileReturn,result);
	return TRUE;
	}

	}
	if (filename) free (filename);
	return FALSE;

	}//try
  __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   { 
	//if (filename) free (filename);
	return FALSE;
   }
   	

};
LIBSPEC  BOOL Zip_GetNextFile(HANDZIP ZipFile,const char * Directory,char *FileReturn)
{
const int  opt_withoutpath =0;	
	int opt_owerwrite=1 ;
	int err;
	char *filename=(char*)malloc(256);
	char result[256]="";
	
	err=unzGoToNextFile(ZipFile);
  __try

	{
	if (err==UNZ_OK)
	{
		err=do_extract_currentfile( ZipFile,
		Directory,filename,&opt_withoutpath,
		&opt_owerwrite);

	
	strcpy(result,filename);
		if (filename) free (filename);
	
	if (err==UNZ_OK)
	{
	strcpy(FileReturn,result);
	return TRUE;
	}

	}

    if (filename) free (filename);
	return FALSE;

  } //try 
  __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   { 

	   //	if (filename) free (filename);
	   return FALSE;
   }
    

};




///For unrar


LIBSPEC  HANDRAR Rar_OpenArchive(const char * ZipfileName,const char *Directory)
{
	

	char *argv[5];
	int argc;


	// by Vadim
	
	argv[1]="X";
	argv[2]="-o+";
	argv[3]=(char *) ZipfileName;
	argv[4]=(char*)malloc(strlen(Directory));
	strcpy(argv[4],Directory);
	argc=5;
	
  ErrHandler.SetSignalHandlers(true);

  RARInitData();
  //char ModuleName[NM];
  //GetModuleFileName(NULL,ModuleName,sizeof(ModuleName));
  SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);
  bool ShutdownOnClose;
  Cmd.Init();
  for (int I=1;I<argc;I++)
    Cmd.ParseArg(argv[I]);

    Cmd.ParseDone();

	 if (GetExt(Cmd.ArcName)==NULL)
    strcat(Cmd.ArcName,".rar");


  if (strchr("AFUMD",*Cmd.Command)==NULL)
  {
    StringList ArcMasks;
    ArcMasks.AddString(Cmd.ArcName);
    ScanTree Scan(&ArcMasks,Cmd.Recurse,Cmd.SaveLinks,SCAN_SKIPDIRS);
    FindData FindData;
    while (Scan.GetNext(&FindData)==SCAN_SUCCESS)
      Cmd.AddArcName(FindData.Name,FindData.NameW);
  }
  else
    Cmd.AddArcName(Cmd.ArcName,NULL);

    //Extract.DoExtract(this);
	Extract.DataIO.SetCurrentCommand(*Cmd.Command);
	struct FindData FD;
  while (Cmd.GetArcName(Cmd.ArcName,Cmd.ArcNameW,sizeof(Cmd.ArcName)))
    if (FindFile::FastFind(Cmd.ArcName,Cmd.ArcNameW,&FD))
      Extract.DataIO.TotalArcSize+=FD.Size;
  Cmd.ArcNames->Rewind();
   if (Cmd.GetArcName(Cmd.ArcName,Cmd.ArcNameW,sizeof(Cmd.ArcName)))
   {

	   //ExtractArchive
	 ArcR=Archive(&Cmd);
    if (!ArcR.WOpen(Cmd.ArcName,Cmd.ArcNameW))
    return(NULL)(EXTRACT_ARC_NEXT);//by v

	 if (ArcR.IsArchive(true))
	 {
   if (ArcR.Volume && ArcR.NotFirstVolume)
   {
    char FirstVolName[NM];

    VolNameToFirstName(Cmd.ArcName,FirstVolName,(ArcR.NewMhd.Flags & MHD_NEWNUMBERING));
    if (stricomp(Cmd.ArcName,FirstVolName)!=0 && FileExist(FirstVolName) &&
        Cmd.ArcNames->Search(FirstVolName,NULL,false))
      return(NULL)(EXTRACT_ARC_NEXT);
   }
		Extract.ExtractArchiveInit(&Cmd,ArcR);
        strcpy(Cmd.ExtrPath,Directory);
		HandRar* HNDR=new HandRar;
		HNDR->ArcR=ArcR;
		HNDR->Cmd=Cmd;
		HNDR->Extract=Extract;
        return HNDR;

	 }



   }

	return NULL;
}
;


LIBSPEC  BOOL Rar_CloseArchive(HANDRAR HNDR)
{
     
    ArcR= HNDR->ArcR; 
	
 //HNDR->ArcR.SkipClose=0;
 //HNDR->ArcR.Close();
//	ArcR.SkipClose=0;
	ArcR.Close();
//HNDR->ArcR.SkipClose=1;
//	ArcR.SkipClose=1;
//	delete(HNDR);
	File::RemoveCreated();
	return TRUE;
	
}
;

LIBSPEC  BOOL Rar_GetFirstFile(HANDRAR HNDR,char *FileReturn)
{


    
    ArcR= HNDR->ArcR;
	Cmd=HNDR->Cmd;
	Extract=HNDR->Extract;
    int Size=ArcR.ReadHeader();
/*	if (Size<0) 
	   {   success=TRUE;
		   return TRUE;
	   }*/
     Repeat=false;
	 success=false;
	char *Ret=NULL;
	const char *NEXT="NEXT";//for delphi 

	if (success=Extract.ExtractCurrentFile(&Cmd,ArcR,Size,Repeat))
	{
	if(int sizestr=strlen(Extract.DestFileName) < 1024)	   

	strcpy(FileReturn,Extract.DestFileName+strlen(Cmd.ExtrPath));
	
	}

	if(Repeat)	   

	strcpy(FileReturn,"");
	
	




        HNDR->ArcR=ArcR;
		HNDR->Cmd=Cmd;
		HNDR->Extract=Extract;
        
	return (success||Repeat) ;
};


LIBSPEC  BOOL Rar_GetNextFile(HANDRAR HNDR, char *FileReturn)
{

	
     
    ArcR= HNDR->ArcR;
	Cmd=HNDR->Cmd;
	Extract=HNDR->Extract;
	char *Ret=NULL;
	const char *NEXT="NEXT";//for delphi 

	if (Repeat&& !success)
	 {
	if (ArcR.Volume && ArcR.NotFirstVolume)
	{
    char FirstVolName[NM];

    VolNameToFirstName(Cmd.ArcName,FirstVolName,(ArcR.NewMhd.Flags & MHD_NEWNUMBERING));
    if (stricomp(Cmd.ArcName,FirstVolName)!=0 && FileExist(FirstVolName) &&
        Cmd.ArcNames->Search(FirstVolName,NULL,false))
      return FALSE;
	}

       Extract.ExtractArchiveInit(&Cmd,ArcR);
       int Size=ArcR.ReadHeader();
	   /*if (Size<0) 
	   {   success=TRUE;
		   return TRUE;
	   }*/
       Repeat=false;   
   
	   success=false;

	   if (success=Extract.ExtractCurrentFile(&Cmd,ArcR,Size,Repeat))
	   {

		strcpy(FileReturn,Extract.DestFileName+strlen(Cmd.ExtrPath));		   
	
	   }
	   	if(Repeat)	   

		strcpy(FileReturn,"");
	


	 }
	else 
	{

		 
       int Size=ArcR.ReadHeader();
	   /* if (Size<0) 
	   {   success=TRUE;
		   return TRUE;
	   }*/
       Repeat=false;   
   
	   success=false;

	   if (success=Extract.ExtractCurrentFile(&Cmd,ArcR,Size,Repeat))
	   {

    	
		strcpy(FileReturn,Extract.DestFileName+strlen(Cmd.ExtrPath));		   
	
	   }
	   	if(Repeat)	   strcpy(FileReturn,"");
	
	

 
	   }

	 if ((!Repeat)&& (!success))    return FALSE;

     
        HNDR->ArcR=ArcR;
		HNDR->Cmd=Cmd;
		HNDR->Extract=Extract;
	 return TRUE;

}
//For Arj



LIBSPEC  HANDARJ Arj_OpenArchive(const char * ArjfileName)
{
  __try 
	   
   {
	char *ArjName=(char*)malloc(strlen(ArjfileName));
	strcpy(ArjName, ArjfileName);
	char *arguments[]={"","l",ArjName};
	int count=3;
    listA= Create_List(12);
   
   datafile =new  FileRecord;
   if (!listA)      return NULL;
   ArjInitProc(count ,  arguments,"");
   int cou=6;
    HANDARJ openhand=new hand;
  openhand->arjlist= listA; 
  openhand->arjnode=listA->first;
  nodetmp=openhand->arjnode;
 while (nodetmp)
   {

	 datafile = (FileRecord *)Get_Data(openhand->arjlist, nodetmp);
	 //FileN=malloc(strlen(datafile->ArjFile));
	 timeN=datafile->timefile.unixtime;
	 openhand->arjnode=nodetmp->next;
	 //nodetmp= openhand->arjnode;
	// nodetmp=openhand->arjnode;
	 //openhand->arjnode = openhand->arjnode->next;
	 
	while  (openhand->arjnode)
    {
	 datafiletmp = (FileRecord *)Get_Data(openhand->arjlist, openhand->arjnode);
     timeP=datafiletmp->timefile.unixtime;
	 if ((timeP==timeN)&&(!strcmp(datafile->ArjFile,datafiletmp->ArjFile)))
	 {

	 Delete_Node(openhand->arjlist, openhand->arjnode);
	 openhand->arjnode=nodetmp->next;
	 } else 
	 openhand->arjnode = openhand->arjnode->next;	
	 
	 }

	 nodetmp=nodetmp->next;
	 //node = node->next;
	}
    openhand->arjnode=listA->first;
    openhand->ArjFileName=(char*)malloc(strlen( ArjfileName));
	strcpy((char*)openhand->ArjFileName,ArjfileName);
	return openhand;

	
	 }
   __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   { 
	   return NULL;
   }
}
;


LIBSPEC  BOOL Arj_CloseArchive(HANDARJ Cmd)
{
   	
	__try 
	   
    {
	 Delete_List(Cmd->arjlist);
	}

    __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   { 
	   return 0;
   }

return TRUE;
	
}
;

LIBSPEC   BOOL Arj_GetFirstFile(HANDARJ Cmd,const char * Directory,char * FileRet)
{
    
	
	__try 
	   
   {

	int count=4;
	if (!Cmd->arjlist)      return 0;
    Cmd->arjnode = Cmd->arjlist->first;
	datafile =(FileRecord *) Get_Data(Cmd->arjlist, Cmd->arjnode);
	//FileN=(char*)malloc(strlen(datafile->ArjFile));
	
	char *FileArj=(char*)malloc(strlen(Cmd->ArjFileName));
	strcpy(FileArj,Cmd->ArjFileName);
	
	char * File=(char*)malloc(strlen(datafile->ArjFile));
	strcpy(File,datafile->ArjFile);
	char *arguments[]={"","x",FileArj,File};
	//node = node->next;

    //if (!listA)      return 0;
   ArjInitProc(count , arguments,Directory);
   int cou=6;

  
    if (FileRet==NULL)
    FileRet=(char*)malloc(1024); 
	strcpy(FileRet,File);
//	free(File);
     return 1;

	 }
   __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   { 
	   return 0;
   }
	
};


LIBSPEC   BOOL Arj_GetNextFile(HANDARJ Cmd,const char * Directory,char * FileRet)
{


__try 
	   
   {int count=4;
	
    Cmd->arjnode = Cmd->arjnode->next;
	if (Cmd->arjnode==NULL) return 0;
	datafile =(FileRecord *) Get_Data(Cmd->arjlist, Cmd->arjnode);	
	char *FileArj=(char*)malloc(strlen(Cmd->ArjFileName));
	strcpy(FileArj,Cmd->ArjFileName);
	char *File=(char*)malloc(strlen(datafile->ArjFile));

	strcpy(File,datafile->ArjFile);
	char *arguments[]={"","x",FileArj,File};
	//node = node->next;

    //if (!list)      return NULL;
   ArjInitProc(count ,  arguments,Directory);
   	   
	if (FileRet==NULL)
    FileRet=(char*)malloc(1024); 
	strcpy(FileRet,File);
    return 1;

   }
  __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   { 
	   return 0;
   }
     //free(File);


}



//For unace




LIBSPEC  HANDACE Ace_OpenArchive(const char * AcefileName)
{
 __try 
 {
	   


	 int arg_cnt = 1;
     f_ovrall    = 1;
	 f_allvol_pr = 2;   
	  CHAR *s;

      init_unace();                              // initialize unace

      strcpy(aname, AcefileName);              // get archive name
      if (!(s = (CHAR *) strrchr(aname, DIRSEP)))
         s = aname;
      if (!strrchr(s, '.'))
         strcat(aname, ".ACE");

      if (!open_archive(1))    
		  return NULL;;

     return archan;
 } //try
   __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   {
	    return 0;
   }

   
}
;


LIBSPEC  BOOL Ace_CloseArchive(HANDACE Cmd)
{
   	
	__try 
	{

	   
     int success;
	 success=close(archan);
        
     done_unace();
	}
     __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   {
	    return 0;
   }


return success;
	
}
;

LIBSPEC   BOOL Ace_GetFirstFile(HANDACE Cmd,const char * Directory,char * FileRet)
{
    
	
	__try 
	{

	CHAR file[PATH_MAX];
	char * fileAceOem=(char*)malloc(PATH_MAX);
	char * fileAce=(char*)malloc(PATH_MAX);
    int nopath=1;
	int test=0;
	if (read_headerAce(1))
	{
	
  // {
      if (head.HEAD_TYPE == FILE_BLK)
      {
         comment_out("File comment:");   // show file comment
         ace_fname(file, &head, nopath); // get file name
     //    printf("\n%s", file);
         flush;
		 
		 strcpy(fileAceOem,Directory);
		 strcat(fileAceOem,file);
		 OemToChar(fileAceOem,fileAce);
         dcpr_init_file();               // initialize decompression of file

  ULARGE_INTEGER lpFreeBytesAvailable;//= (PULARGE_INTEGER) malloc(16) ;    // bytes available to caller
  ULARGE_INTEGER lpTotalNumberOfBytes;//=(PULARGE_INTEGER) malloc(16);    // bytes on disk
  ULARGE_INTEGER lpTotalNumberOfFreeBytes;//=(PULARGE_INTEGER) malloc(16) ;
  BOOL res=FALSE;

  res=GetDiskFreeSpaceEx(
  Directory,                 // directory name
  &lpFreeBytesAvailable,    // bytes available to caller
  &lpTotalNumberOfBytes,    // bytes on disk
  &lpTotalNumberOfFreeBytes );


  if( dcpr_size>4294967296) return 1;
  if (lpFreeBytesAvailable.QuadPart< dcpr_size) return 1;


         if (!f_err)
         {
            if (test || 
                (wrhan = create_dest_file(fileAce, (INT) fhead.ATTR))<0)
            {
               if (test || adat.sol)
                  analyze_file();        // analyze file
            }
            else
            {
               extract_file();           // extract it
                              // set file time
              // _dos_setftime(wrhan, (USHORT) (fhead.FTIME >> 16), (USHORT) fhead.FTIME);

               close(wrhan);
			                                // set file attributes
              // _dos_setfileattr(file, (UINT) fhead.ATTR);
               if (f_err)
               remove(file);
			    OemToChar(file,FileRet);
			   return 1;

            }

			  OemToChar(file,FileRet);
			 return 1;
         }
      }
   }
	else 
	{
	strcpy(FileRet,"");
	return 0;
	}
	   

	}//try


 __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   {
	    return 0;
   }
    return 0;
};


LIBSPEC   BOOL Ace_GetNextFile(HANDACE Cmd,const char * Directory,char * FileRet)
{


__try 
{

	CHAR file[PATH_MAX];
	char * fileAceOem=(char*)malloc(PATH_MAX);
	char * fileAce=(char*)malloc(PATH_MAX);
    int nopath=0;
	int test=0;
	if (read_headerAce(1))
	{
	
  // {
      if (head.HEAD_TYPE == FILE_BLK)
      {
         comment_out("File comment:");   // show file comment
         ace_fname(file, &head, nopath); // get file name
//         printf("\n%s", file);
         flush;
		 strcpy(fileAceOem,Directory);
		 strcat(fileAceOem,file);
		 OemToChar(fileAceOem,fileAce);
         dcpr_init_file();               // initialize decompression of file

  ULARGE_INTEGER lpFreeBytesAvailable;//= (PULARGE_INTEGER) malloc(16) ;    // bytes available to caller
  ULARGE_INTEGER lpTotalNumberOfBytes;//=(PULARGE_INTEGER) malloc(16);    // bytes on disk
  ULARGE_INTEGER lpTotalNumberOfFreeBytes;//=(PULARGE_INTEGER) malloc(16) ;
  BOOL res=FALSE;

  res=GetDiskFreeSpaceEx(
  Directory,                 // directory name
  &lpFreeBytesAvailable,    // bytes available to caller
  &lpTotalNumberOfBytes,    // bytes on disk
  &lpTotalNumberOfFreeBytes );


  if( dcpr_size>4294967296) return 1;
  if (lpFreeBytesAvailable.QuadPart< dcpr_size) return 1;


         if (!f_err)
         {
            if (test || 
                (wrhan = create_dest_file(fileAce, (INT) fhead.ATTR))<0)
            {
               if (test || adat.sol)
                  analyze_file();        // analyze file
            }
            else
            {
               extract_file();           // extract it
                              // set file time
              // _dos_setftime(wrhan, (USHORT) (fhead.FTIME >> 16), (USHORT) fhead.FTIME);

               close(wrhan);
			   
                              // set file attributes
              // _dos_setfileattr(file, (UINT) fhead.ATTR);


               if (f_err)
                  remove(fileAce);
			    OemToChar(file,FileRet);
			   return 1;

            }

			  OemToChar(file,FileRet);
			 return 1;
         }
      }
   }
	else 
	{
	strcpy(FileRet,"");
	return 0;
	}

}//try
    __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   {
	    return 0;
   }
 return 0;
}




//FOR MS UNCAB 

LIBSPEC  HANDCABMS Cab_OpenArchive(const char * cabname)
{

  struct cabinet *basecab, *cab, *cab1, *cab2;
  struct file *filelist, *fi;
  HANDCABMS Cmd1;
  int quiet=0;
  int lower=0;
  int fix=0;

  /* has the list-mode header been seen before? */
  int viewhdr = 0;

 /* if (view || !quiet) {
    printf("%s cabinet: %s\n", view ? "Viewing" : "Extracting", cabname);
  }*/

  /* load the file requested */

  __try 
{
  basecab = find_cabs_in_file((char*)cabname);
  if (!basecab) return NULL;

  /* iterate over all cabinets found in that file */
  for (cab = basecab; cab; cab=cab->next) {

    /* bi-directionally load any spanning cabinets -- backwards */
    for (cab1 = cab; cab1->flags & cfheadPREV_CABINET; cab1 = cab1->prevcab) {
      //if (!quiet) printf("%s: extends backwards to %s (%s)\n", cabname,
		//	 cab1->prevname, cab1->previnfo);
      find_cabinet_file(&(cab1->prevname), (char*)cabname);
      if (!(cab1->prevcab = load_cab_offset(cab1->prevname, 0))) {
       //fprintf(stderr, "%s: can't read previous cabinet %s\n",
		//cabname, cab1->prevname)
		  ;
        break;
      }
      cab1->prevcab->nextcab = cab1;
    }

    /* bi-directionally load any spanning cabinets -- forwards */
    for (cab2 = cab; cab2->flags & cfheadNEXT_CABINET; cab2 = cab2->nextcab) {
      //if (!quiet) printf("%s: extends to %s (%s)\n", cabname,
		//	 cab2->nextname, cab2->nextinfo);
      find_cabinet_file(&(cab2->nextname), (char*)cabname);
      if (!(cab2->nextcab = load_cab_offset(cab2->nextname, 0))) {
        //fprintf(stderr, "%s: can't read next cabinet %s\n",
		//cabname, cab2->nextname)
		  ;
        break;
      }
      cab2->nextcab->prevcab = cab2;
    }

    Cmd1 = process_files(cab1);
    CAB(current) = NULL;
  }
	
	return Cmd1;
  }
  __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   {
	    return NULL;
   }


}
;


LIBSPEC  BOOL Cab_CloseArchive(HANDCABMS Cmd1)
{

	return TRUE;
}
;

LIBSPEC   BOOL Cab_GetFirstFile(HANDCABMS &Cmd1,const char * Directory,char *FileReturn)
{
  int lower=0;
  int fix=0;
  
  __try 
  {
	  if (Cmd1) {

        
  ULARGE_INTEGER lpFreeBytesAvailable;//= (PULARGE_INTEGER) malloc(16) ;    // bytes available to caller
  ULARGE_INTEGER lpTotalNumberOfBytes;//=(PULARGE_INTEGER) malloc(16);    // bytes on disk
  ULARGE_INTEGER lpTotalNumberOfFreeBytes;//=(PULARGE_INTEGER) malloc(16) ;
  BOOL res=FALSE;

  res=GetDiskFreeSpaceEx(
  Directory,                 // directory name
  &lpFreeBytesAvailable,    // bytes available to caller
  &lpTotalNumberOfBytes,    // bytes on disk
  &lpTotalNumberOfFreeBytes );


  if( Cmd1->length> 4294967296) return 1;
  if (lpFreeBytesAvailable.QuadPart< Cmd1->length) return 1;


		extract_fileMS(Cmd1, lower, fix,(char*) Directory);
		strcpy(FileReturn,Cmd1->filename);
        return TRUE;
		}
    strcpy(FileReturn,"");
    return FALSE;
  }
   __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   {
	   strcpy(FileReturn,"");
	   return FALSE ;
		
   }

  

};


LIBSPEC   BOOL  Cab_GetNextFile(HANDCABMS &Cmd1,const char * Directory,char *FileReturn)
{

        int lower=0;
        int fix=0;
		Cmd1 = Cmd1->next;
__try 
{
	if (Cmd1)	
		{

		      
  ULARGE_INTEGER lpFreeBytesAvailable;//= (PULARGE_INTEGER) malloc(16) ;    // bytes available to caller
  ULARGE_INTEGER lpTotalNumberOfBytes;//=(PULARGE_INTEGER) malloc(16);    // bytes on disk
  ULARGE_INTEGER lpTotalNumberOfFreeBytes;//=(PULARGE_INTEGER) malloc(16) ;
  BOOL res=FALSE;

  res=GetDiskFreeSpaceEx(
  Directory,                 // directory name
  &lpFreeBytesAvailable,    // bytes available to caller
  &lpTotalNumberOfBytes,    // bytes on disk
  &lpTotalNumberOfFreeBytes );


  if( Cmd1->length> 4294967296) return 1;
  if (lpFreeBytesAvailable.QuadPart< Cmd1->length) return 1;

		extract_fileMS(Cmd1, lower, fix, (char*)Directory);
        strcpy(FileReturn,Cmd1->filename);
		//Cmd1 = Cmd1->next;
        return TRUE;
		}
	    strcpy(FileReturn,"");
		return FALSE;
}
		__except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   {
		strcpy(FileReturn,"");
	    return FALSE ;
   }
	 

}



//FOR InstallShield  UNCAB

LIBSPEC  HANDCABIS CabIS_OpenArchive(const char * CabfileName,const char * Directory)
{


 hCabHdr = 0;
 pCabDesc = NULL;
 SetupTypes = NULL;
 cSetupTypes = 0;
 cComponents = 0;
 cFileGroups = 0;
 pCabPattern = NULL;
 Dirs = NULL;
 Files = NULL;

// Globals
 ver = ver60;
 exename = NULL;
 pZBuf = NULL;
 pFileList = NULL;
 pDiskList = NULL;
 FileCount = 0;
 DWORD FileAttrVals[4] = {FILE_ATTR_ARCHIVE, FILE_ATTR_READONLY, FILE_ATTR_HIDDEN, FILE_ATTR_SYSTEM};
 char FileAttrChars[]= "ARHS";

// Options
 optRecurseSubdirs = FALSE;
 optMatchWithDirs = FALSE;
 optPrintAll = TRUE;
 optFGasDir = FALSE;
 optVersion = 0;
 optFileGroup = (DWORD)-1;
// Morlac
 optVerbose = FALSE;

	optRecurseSubdirs = TRUE;
	
	char* cabname;

    if ((strstr(_strlwr((char*)CabfileName),"data1.")==NULL) &(strstr(_strlwr((char*)CabfileName),"data2.")==NULL)) return NULL;

	__try
	{
	InitCabFile((char*)CabfileName, O_RDONLY);
			TranslateFileGroup();
			CabBasedFileList("*", (char*)Directory);
			int arg =5;
    HandCab *CmdCab=new HandCab;
	CmdCab->pCurrent=pFileList;
	CmdCab->Path=(char*)malloc(strlen(Directory));
	strcpy(CmdCab->Path,Directory);
    

	InitZData(&CmdCab->rwHnd, zdDecompress);
    return CmdCab;

	}//try 
	 __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   {
	    return NULL ;
   }

}

LIBSPEC  BOOL CabIS_CloseArchive(HANDCABIS CmdCab)
{
	__try
	{
  if (!CmdCab) return FALSE;
  //if (!CmdCab->pCurrent) return FALSE;
 // CleanExit(101);
   //delete (CabFile);
  return TRUE;
	}
   __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   {
	    return FALSE ;
   }

  
}

LIBSPEC  BOOL  CabIS_GetFirstFile(HANDCABIS CmdCab,char *FileReturn)
{

	//	pCurrent = pFileList;

	if (!CmdCab) return FALSE;
	if (!CmdCab->pCurrent) return FALSE;
	//if (CmdCab->rwHnd==NULL) return FALSE;
		DWORD i = CmdCab->pCurrent->CabIndex;
		LPSTR pName = CmdCab->pCurrent->FileName;
		LPSTR pSlash;
		FILEDESC* pFile = Files + i;
		FILEDESC* pDupe;

		// check if the file is a dupe of another file and resolve
		for (pDupe = pFile;
			 pDupe->LinkFlags & LINK_PREV;
			 i = pDupe->PrevDupe, pDupe = Files + i)
			;

		CmdCab->rwHnd.FileIndex = i;		
		CmdCab->rwHnd.Volume = pDupe->Volume;
		CmdCab->rwHnd.ofsData = pDupe->ofsData;
		CmdCab->rwHnd.hRead = 0;
		CmdCab->rwHnd.BytesIn = pDupe->cbCompressed;
		CmdCab->rwHnd.BytesOut = 0;


  ULARGE_INTEGER lpFreeBytesAvailable;//= (PULARGE_INTEGER) malloc(16) ;    // bytes available to caller
  ULARGE_INTEGER lpTotalNumberOfBytes;//=(PULARGE_INTEGER) malloc(16);    // bytes on disk
  ULARGE_INTEGER lpTotalNumberOfFreeBytes;//=(PULARGE_INTEGER) malloc(16) ;
  BOOL res=FALSE;

  res=GetDiskFreeSpaceEx(
  CmdCab->Path,                 // directory name
  &lpFreeBytesAvailable,    // bytes available to caller
  &lpTotalNumberOfBytes,    // bytes on disk
  &lpTotalNumberOfFreeBytes );


  if( pDupe->cbExpanded> 4294967296) return 1;
  if (lpFreeBytesAvailable.QuadPart< pDupe->cbExpanded) return 1;

		
		pSlash = strrchr(pName, '\\');
		if (pSlash) {
			*pSlash = '\0';
			if (!DirExists(pName))
				CreateDir(pName);
			*pSlash = '\\';
		}

		CmdCab->rwHnd.hWrite = OpenForWrite(pName, O_CREAT | O_TRUNC);
	
		if (pFile->DescStatus & DESC_COMPRESSED) {
			__try {
				DWORD Res;

				if (ZDataStart(0))
					CantDecompress(pName);
				
				if (Res=ZDataDecompress(0))
					CantDecompress(pName);
				else { 
					if (optPrintAll || CmdCab->rwHnd.BytesOut != pFile->cbExpanded)
						//fprintf(stdout, "%s", pName)
						;
					if (CmdCab->rwHnd.BytesOut != pFile->cbExpanded)
						NoMatchBytesOut();
					if (optPrintAll || CmdCab->rwHnd.BytesOut != pFile->cbExpanded)
						//fprintf(stdout, "\n")
						;
				}
				ZDataEnd();
			
			} __except(1) {
				ExceptInDecompress();
				return FALSE;
			}
		
		} else {
			ExtractData(&CmdCab->rwHnd, pFile->DescStatus & DESC_ENCRYPTED ? CRYPT_DECRYPT : CRYPT_NONE);
			if (optPrintAll)
				//fprintf(stdout, "%s\n", pName)
				;
		}
		close(CmdCab->rwHnd.hWrite);
		strcpy(FileReturn,pName+strlen(CmdCab->Path));
		SetFileDosTime(pName, pFile->FatDate, pFile->FatTime);
		SetFileAttrs(pName, pFile->Attrs);


	return TRUE;
}

LIBSPEC  BOOL  CabIS_GetNextFile(HANDCABIS CmdCab,char *FileReturn)
{


        CmdCab->pCurrent =CmdCab->pCurrent->pNext ;
         if (!CmdCab->pCurrent) return FALSE;
		DWORD i = CmdCab->pCurrent->CabIndex;
		LPSTR pName = CmdCab->pCurrent->FileName;
		LPSTR pSlash;
		FILEDESC* pFile = Files + i;
		FILEDESC* pDupe;

		// check if the file is a dupe of another file and resolve
		for (pDupe = pFile;
			 pDupe->LinkFlags & LINK_PREV;
			 i = pDupe->PrevDupe, pDupe = Files + i)
			;

		CmdCab->rwHnd.FileIndex = i;		
		CmdCab->rwHnd.Volume = pDupe->Volume;
		CmdCab->rwHnd.ofsData = pDupe->ofsData;
		CmdCab->rwHnd.hRead = 0;
		CmdCab->rwHnd.BytesIn = pDupe->cbCompressed;
		CmdCab->rwHnd.BytesOut = 0;



  ULARGE_INTEGER lpFreeBytesAvailable;//= (PULARGE_INTEGER) malloc(16) ;    // bytes available to caller
  ULARGE_INTEGER lpTotalNumberOfBytes;//=(PULARGE_INTEGER) malloc(16);    // bytes on disk
  ULARGE_INTEGER lpTotalNumberOfFreeBytes;//=(PULARGE_INTEGER) malloc(16) ;
  BOOL res=FALSE;

  res=GetDiskFreeSpaceEx(
  CmdCab->Path,                  // directory name
  &lpFreeBytesAvailable,    // bytes available to caller
  &lpTotalNumberOfBytes,    // bytes on disk
  &lpTotalNumberOfFreeBytes );


  if( pDupe->cbExpanded> 4294967296) return 1;
  if (lpFreeBytesAvailable.QuadPart< pDupe->cbExpanded) return 1;
		
		pSlash = strrchr(pName, '\\');
		if (pSlash) {
			*pSlash = '\0';
			if (!DirExists(pName))
				CreateDir(pName);
			*pSlash = '\\';
		}

		CmdCab->rwHnd.hWrite = OpenForWrite(pName, O_CREAT | O_TRUNC);
	
		if (pFile->DescStatus & DESC_COMPRESSED) {
			__try {
				DWORD Res;

				if (ZDataStart(0))
					CantDecompress(pName);
				
				if (Res=ZDataDecompress(0))
				{
					CantDecompress(pName);
					return FALSE;
				}
				else { 
					if (optPrintAll || CmdCab->rwHnd.BytesOut != pFile->cbExpanded)
						//fprintf(stdout, "%s", pName)
						;
					if (CmdCab->rwHnd.BytesOut != pFile->cbExpanded)
						NoMatchBytesOut();
					if (optPrintAll || CmdCab->rwHnd.BytesOut != pFile->cbExpanded)
						//fprintf(stdout, "\n")
						;
				}
				ZDataEnd();
			
			} __except(1) {
				ExceptInDecompress();
				return FALSE;
			}
		
		} else {
			ExtractData(&CmdCab->rwHnd, pFile->DescStatus & DESC_ENCRYPTED ? CRYPT_DECRYPT : CRYPT_NONE);
			if (optPrintAll)
				//fprintf(stdout, "%s\n", pName)
				;
		}
		close(CmdCab->rwHnd.hWrite);
		strcpy(FileReturn,pName+strlen(CmdCab->Path));
		SetFileDosTime(pName, pFile->FatDate, pFile->FatTime);
		SetFileAttrs(pName, pFile->Attrs);






	return TRUE;
}


LIBSPEC  HANDTAR Tar_OpenArchive(const char * TarfileName,const char * Directory)
{

	int ret;
	char szCmdLine[1000] = "";
    strcpy(szCmdLine," -x ");
	strcat(szCmdLine,TarfileName);
	strcat(szCmdLine," -o ");
	strcat(szCmdLine, Directory);


	int i;
	char buff[1001];
	
	int _dwSize=1000;
	HWND hwnd=NULL;
//	_CrtMemState memstate;
//	_CrtMemCheckpoint(&memstate);

	CTar32CmdInfo info(_szOutput, _dwSize-1);
	cmdinfo =info;

//	ret = tar_cmd(_hwnd,_szCmdLine,_szOutput,_dwSize);

	list<string> args;	// command line array
	{
		char **argv;
		argv = split_cmdline_with_response(szCmdLine);
		if(!argv){
			throw CTar32Exception("commandline split error", ERROR_COMMAND_NAME);
		}
		char **argv2 = argv;
		while(*argv2){
			args.push_back(*argv2);
			argv2++;
		}
	//	free(argv);
	}

	char command = 0;	// main command. ('x','c','l')
	string current_directory;
	list<string>::iterator argi = args.begin();

	while(argi != args.end()){
		string::iterator stri = (*argi).begin();
		if(argi==args.begin() || (*stri == '-' && *(stri+1) != '\0')){
			if(*stri == '-'){
				stri++;
			}
			if(*stri == '-' && *(stri+1) != '\0'){
				stri++;
				const string &long_option = (*argi).substr(stri - argi->begin());
				string key;
				string val = long_option;
				int len;
				if((len = long_option.find('=')) != string::npos){
					key = long_option.substr(0, len);
					val = long_option.substr(len + 1);
				}
				if(key == "use-directory"){
					cmdinfo.b_use_directory = (bool)atoi(val.c_str());
				}else if(key == "display-dialog"){
					cmdinfo.b_display_dialog = (bool)atoi(val.c_str());
				}else if(key == "message-loop"){
					cmdinfo.b_message_loop = (bool)atoi(val.c_str());
				}else if(key == "bzip2"){
					cmdinfo.archive_type = ARCHIVETYPE_BZ2;
					cmdinfo.compress_level = (bool)atoi(val.c_str());
				}else{
					/* igonore */;
				}
				argi++;
				continue;
			}
			list<string>::iterator cur_argi = argi;
			while(stri != (*cur_argi).end()){
				switch(*stri){
				case 'x':
					command = 'x';break;
				case 'c':
					command = 'c';break;
				case 'l':
				case 't': // 
					command = 'l';break;
				case 'p':
					command = 'x';
					cmdinfo.b_print = true;
					break;
				case 'f':
					if(++argi == args.end()){
						throw CTar32Exception("'f' follow no directory name", ERROR_COMMAND_NAME);
					}
					cmdinfo.arcfile = *argi;
					break;
				case 'o':
					if(++argi == args.end()){
						throw CTar32Exception("'o' follow no directory name", ERROR_COMMAND_NAME);
					}
					current_directory = *argi;
					// stri = argi->end()-1;
					break;
				case 'z':
					cmdinfo.archive_type = ARCHIVETYPE_GZ;
					if(isdigit(*(stri+1))){
						stri++;
						cmdinfo.compress_level = ((char)*stri) - '0';
					}else{
						cmdinfo.compress_level = 6;
					}
					break;
				case 'B':
					cmdinfo.archive_type = ARCHIVETYPE_BZ2;
					if(isdigit(*(stri+1))){
						stri++;
						cmdinfo.compress_level = ((char)*stri) - '0';
					}else{
						cmdinfo.compress_level = 9;
					}
					break;
				case 'Z':
					cmdinfo.archive_type = ARCHIVETYPE_Z;
					if(isdigit(*(stri+1))){
						stri++;
						cmdinfo.compress_level = ((char)*stri) - '0';
					}else{
						cmdinfo.compress_level = 6;
					}
					break;
				case 'G':
					cmdinfo.b_archive_tar = false;
					if(cmdinfo.archive_type == ARCHIVETYPE_NORMAL){
						cmdinfo.archive_type = ARCHIVETYPE_GZ;
						cmdinfo.compress_level = 6;
					}
					break;
				case 'a':
				case 'v':
				case 'V':
				case 'I':
				case 'i':
				case 'e':
				case 'g':
				case 'S':
				case 'R':	// added by tsuneo 2001.05.14
					/*ignore*/
					break;
				case 'A':
					stri++;
					/*ignore*/
					break;
				case 'b':
				case 'N':
					argi++;
					stri = argi->end()-1;
					/*ignore*/
					break;

				case 'U':
					if(isdigit(*(stri+1))){
						stri++;
					}
					/*ignore*/
					break;
				default:
					throw CTar32Exception("Invalid Arguments", ERROR_COMMAND_NAME);
				}
				stri++;
			}
			argi++;
		}else{
			const char *file = (*argi).c_str();
			if(*file && ((char*)_mbsrchr((const unsigned char*)file, '\\') == file + strlen(file) - 1)){
				current_directory = (char*)file;
			}else if(cmdinfo.arcfile.length() == 0){
				cmdinfo.arcfile = *argi;
			}else{
				string file = *argi;
				cmdinfo.argfiles.push_back(CTar32CmdInfo::CArgs(file,current_directory));
			}
			argi++;
		}
	}
	if(cmdinfo.arcfile.empty()){
		throw CTar32Exception("Archive File is not specified.", ERROR_NOT_ARC_FILE);
	}
	if(cmdinfo.argfiles.empty()){
		if(command == 'x' || command == 'l'){
			// If no file to extract/create is specified, I assume as all file is specified.
			cmdinfo.argfiles.push_back(CTar32CmdInfo::CArgs("*",current_directory));
		}else{
			throw CTar32Exception("no file to archive is specified.", ERROR_NOT_ARC_FILE);
		}
	}
	if(cmdinfo.b_archive_tar){
		switch(cmdinfo.archive_type){
		case ARCHIVETYPE_NORMAL:
			cmdinfo.archive_type = ARCHIVETYPE_TAR;break;
		case ARCHIVETYPE_GZ:
			cmdinfo.archive_type = ARCHIVETYPE_TARGZ;break;
		case ARCHIVETYPE_Z:
			cmdinfo.archive_type = ARCHIVETYPE_TARZ;break;
		case ARCHIVETYPE_BZ2:
			cmdinfo.archive_type = ARCHIVETYPE_TARBZ2;break;
		}
	}
	//string arcfile = *argi++;
	//list<string> files(argi,args.end());
	
	//CTar32CmdInfo cmdinfo;
	//cmdinfo.arcfile = arcfile;
	//cmdinfo.files = files;

	/*
	CTar32StatusDialog dlg;
	if(cmdinfo.b_display_dialog){
		cmdinfo.hTar32StatusDialog = dlg.Create(hwnd);
	}
	switch(command){
	case 'x':
		cmd_extract(cmdinfo);
		break;
	case 'c':
		cmd_create(cmdinfo);
		break;
	case 'l':
		cmd_list(cmdinfo);
		break;
	default:
		throw CTar32Exception("Command not specified.", ERROR_COMMAND_NAME);
	}
	*/
	cmdinfo.command = command;
	cmdinfo.hParentWnd = hwnd;
	int func_ret = 0;
	// extern static void _cdecl tar_cmd_main_thread(LPVOID param);
	//DWORD process_id,thread_id;
	//thread_id = GetWindowThreadProcessId(hwnd,&process_id);
	if(cmdinfo.b_message_loop && cmdinfo.hParentWnd /*&& process_id == GetCurrentProcessId() && thread_id == GetCurrentThreadId()*/){
		cmdinfo.wm_main_thread_end = RegisterWindowMessage("wm_main_thread_end");
		cmdinfo.idMessageThread = GetCurrentThreadId();

//	CTar32CmdInfo *pCmdInfo = (CTar32CmdInfo*)param;
	//CTar32CmdInfo &cmdinfo = *pCmdInfo;

		EXTRACTINGINFOEX extractinfo;
		memset(&extractinfo,0,sizeof(extractinfo));
		strncpy(extractinfo.exinfo.szSourceFileName, cmdinfo.arcfile.c_str() ,FNAME_MAX32);
		//int ret = SendArcMessage(cmdinfo, ARCEXTRACT_BEGIN, &extractinfo);
	}
	
	//CTar32 tarfile;
	//int ret;
	ret = tarfile.open(cmdinfo.arcfile.c_str(), "rb");
/*	if(!ret){
	throw CTar32Exception("can't open archive file", ERROR_ARC_FILE_OPEN);
	}*/

    HandTar *Cmd=(struct HandTar*)malloc(sizeof(HandTar));
//	Cmd->cmd=&cmdinfo;
	//tCmd->tar=&tarfile;
	strcpy(Cmd->direct,Directory);
	//Cmd.cmd=cmdinfo;
	
//	_CrtMemDumpAllObjectsSince(&memstate);	// check memoryleaks. (the first time call happen memory-leak becaus of C/C++ runtime.)
	
    if (ret) 	return Cmd;
	return NULL;
};
LIBSPEC  BOOL  Tar_CloseArchive(HANDTAR TarFile)
{
   // delete(TarFile);//->tar.close();
	__try{

	tarfile.close();
	//TarFile->tar->close();
    free(TarFile);
	

/*	if (TarFile)
	delete(TarFile);*/
	return TRUE;
	}

	 __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   {
	    return FALSE ;
   }

}
;
LIBSPEC  BOOL  Tar_GetFirstFile(HANDTAR TarFile,char *FileReturn)
{


	if (!TarFile) return FALSE;
    // CTar32CmdInfo *cmdinfo=TarFile->cmd; 
//	CTar32 * tarfile=TarFile->tar;

		CTar32FileStatus stat;

		bool bret = tarfile.readdir(&stat);
		if(!bret){return FALSE;}
/*
  ULARGE_INTEGER lpFreeBytesAvailable;//= (PULARGE_INTEGER) malloc(16) ;    // bytes available to caller
  ULARGE_INTEGER lpTotalNumberOfBytes;//=(PULARGE_INTEGER) malloc(16);    // bytes on disk
  ULARGE_INTEGER lpTotalNumberOfFreeBytes;//=(PULARGE_INTEGER) malloc(16) ;
  BOOL res=FALSE;
  const char *dir=TarFile->direct;
  
  res=GetDiskFreeSpaceEx(
  dir ,                  // directory name
  &lpFreeBytesAvailable,    // bytes available to caller
  &lpTotalNumberOfBytes,    // bytes on disk
  &lpTotalNumberOfFreeBytes );


  if( tarfile.m_currentfile_status.original_size> 4294967296) return 1;
  if (lpFreeBytesAvailable.QuadPart< tarfile.m_currentfile_status.original_size) return 1;

*/






		
		string file_internal = stat.filename;
		string file_external;
		{
			const list<CTar32CmdInfo::CArgs> &args = cmdinfo.argfiles;
			list<CTar32CmdInfo::CArgs>::const_iterator filei;
			for(filei = args.begin();filei!=args.end();filei++){
				if(::is_regexp_match_dbcs(filei->file.c_str(), file_internal.c_str())){
					string file_internal2 = file_internal;
					if(!cmdinfo.b_use_directory){
						file_internal2 = get_filename(file_internal.c_str());
					}
					file_external = make_pathname(filei->current_dir.c_str(), file_internal2.c_str());
					break;
				}
			}
		}
		if(file_external.empty()){
			bret = tarfile.readskip();
		}else{
			bool bret2 = extract_fileTar(cmdinfo,&tarfile,file_external.c_str(),TarFile->direct);
		}
       //TarFile->cmd=&cmdinfo; 
      // TarFile->tar=tarfile;
       strcpy(FileReturn,file_external.c_str());
		return bret;
}
;
LIBSPEC  BOOL  Tar_GetNextFile(HANDTAR TarFile,char *FileReturn)
{


	if (!TarFile) return FALSE;

	//CTar32CmdInfocmdinfo=TarFile->cmd; 
	//CTar32 * tarfile=TarFile->tar;

		CTar32FileStatus stat;

		bool bret = tarfile.readdir(&stat);
		if(!bret){return FALSE;}

	/*	ULARGE_INTEGER lpFreeBytesAvailable;//= (PULARGE_INTEGER) malloc(16) ;    // bytes available to caller
  ULARGE_INTEGER lpTotalNumberOfBytes;//=(PULARGE_INTEGER) malloc(16);    // bytes on disk
  ULARGE_INTEGER lpTotalNumberOfFreeBytes;//=(PULARGE_INTEGER) malloc(16) ;
  BOOL res=FALSE;
  const char *dir=TarFile->direct;
  
  res=GetDiskFreeSpaceEx(
  dir ,                  // directory name
  &lpFreeBytesAvailable,    // bytes available to caller
  &lpTotalNumberOfBytes,    // bytes on disk
  &lpTotalNumberOfFreeBytes );


  if( tarfile.m_currentfile_status.original_size> 4294967296) return 1;
  if (lpFreeBytesAvailable.QuadPart< tarfile.m_currentfile_status.original_size) return 1;


*/






		
		string file_internal = stat.filename;
		string file_external;
		{
			const list<CTar32CmdInfo::CArgs> &args = cmdinfo.argfiles;
			list<CTar32CmdInfo::CArgs>::const_iterator filei;
			for(filei = args.begin();filei!=args.end();filei++){
				if(::is_regexp_match_dbcs(filei->file.c_str(), file_internal.c_str())){
					string file_internal2 = file_internal;
					if(!cmdinfo.b_use_directory){
						file_internal2 = get_filename(file_internal.c_str());
					}
					file_external = make_pathname(filei->current_dir.c_str(), file_internal2.c_str());
					break;
				}
			}
		}
		if(file_external.empty()){
			bret = tarfile.readskip();
		}else{
			bool bret2 = extract_fileTar(cmdinfo,&tarfile,file_external.c_str(),TarFile->direct);
		}
     //  TarFile->cmd=&cmdinfo; 
    //	TarFile->tar=tarfile;
		//delete(tarfile)

        strcpy(FileReturn,file_external.c_str());  
		return bret;
}
;


static bool extract_fileTar(CTar32CmdInfo &cmdinfo, CTar32 *pTarfile, const char *fname,const char *dir)
{
	CTar32FileStatus &stat = pTarfile->m_currentfile_status;
	string fname2 = fname;

	EXTRACTINGINFOEX extractinfo;
	{
		memset(&extractinfo,0,sizeof(extractinfo));
		extractinfo.exinfo.dwFileSize = stat.original_size;
		extractinfo.exinfo.dwWriteSize = 0;
		strncpy(extractinfo.exinfo.szSourceFileName, pTarfile->get_arc_filename().c_str(),FNAME_MAX32+1);
		strncpy(extractinfo.exinfo.szDestFileName, fname2.c_str(), FNAME_MAX32+1);
		extractinfo.dwCompressedSize = stat.compress_size;
		extractinfo.dwCRC = stat.chksum;
		extractinfo.uOSType = 0;
		extractinfo.wRatio = extractinfo.exinfo.dwFileSize ? (1000 * extractinfo.dwCompressedSize / extractinfo.exinfo.dwFileSize) : 0;
		extractinfo.wDate = GetARCDate(stat.mtime);
		extractinfo.wTime = GetARCTime(stat.mtime);
		GetARCAttribute(stat.mode, extractinfo.szAttribute,sizeof(extractinfo.szAttribute));
		GetARCMethod(pTarfile->m_archive_type, extractinfo.szMode, sizeof(extractinfo.szMode));
		int ret = 0;//SendArcMessage(cmdinfo, ARCEXTRACT_BEGIN, &extractinfo);
	//	if(ret){throw CTar32Exception("Cancel button was pushed.",ERROR_USER_CANCEL);}
		fname2 = extractinfo.exinfo.szDestFileName;
	}

	int filesize = pTarfile->m_currentfile_status.original_size;


  ULARGE_INTEGER lpFreeBytesAvailable;//= (PULARGE_INTEGER) malloc(16) ;    // bytes available to caller
  ULARGE_INTEGER lpTotalNumberOfBytes;//=(PULARGE_INTEGER) malloc(16);    // bytes on disk
  ULARGE_INTEGER lpTotalNumberOfFreeBytes;//=(PULARGE_INTEGER) malloc(16) ;
  BOOL res=FALSE;
  //const char *dir=TarFile->direct;
  
  res=GetDiskFreeSpaceEx(
  dir ,                  // directory name
  &lpFreeBytesAvailable,    // bytes available to caller
  &lpTotalNumberOfBytes,    // bytes on disk
  &lpTotalNumberOfFreeBytes );


  if( pTarfile->m_currentfile_status.original_size> 4294967294) return 1;
  if (lpFreeBytesAvailable.QuadPart< pTarfile->m_currentfile_status.original_size) return 1;




	CTar32InternalFile file; file.open(pTarfile);


	// ofstream fs_w;
	fast_fstream fs_w;
	if(!cmdinfo.b_print){
		mkdir_recursive(get_dirname(fname2.c_str()).c_str());
		fs_w.open(fname2.c_str(), ios::out|ios::binary);
		if(fs_w.fail()){return false;}
	}

	int readsize = 0;
	while(filesize ==-1 || readsize<filesize){
		char buf[65536];
		int nextreadsize;
		if(filesize == -1){ // case ".gz",".Z",".bz2"
			nextreadsize = sizeof(buf);
		}else{
			nextreadsize = min((int)filesize-readsize,(int)sizeof(buf));
		}
		int n = file.read(buf,nextreadsize);
		readsize += n;
		if(cmdinfo.b_print){
			cmdinfo.output.write(buf,n);
		}else{
			fs_w.write(buf,n);
			if(fs_w.fail()){return false;}
		}
		if(n != nextreadsize){
			if(filesize == -1){ // case .gz/.Z/.bz2"
				break;
			}else{
				return false;
			}
		}
		if(cmdinfo.hTar32StatusDialog){
			extractinfo.exinfo.dwWriteSize = readsize;
			int ret = 0;//SendArcMessage(cmdinfo, ARCEXTRACT_INPROCESS, &extractinfo);
		//	if(ret){throw CTar32Exception("Cancel button was pushed.",ERROR_USER_CANCEL);}
		}
	}
	if(!cmdinfo.b_print){
		fs_w.close();
		struct _utimbuf ut;
		ut.actime = (stat.atime ? stat.atime : time(NULL));
		ut.modtime = (stat.mtime ? stat.mtime : time(NULL));
		int ret;
		ret = _utime(fname2.c_str(), &ut);
		ret = _chmod(fname2.c_str(), stat.mode);
	}
	if(cmdinfo.hTar32StatusDialog){
		extractinfo.exinfo.dwWriteSize = readsize;
		int ret = 0;//SendArcMessage(cmdinfo, ARCEXTRACT_END, &extractinfo);
	//	if(ret){throw CTar32Exception("Cancel button was pushed.",ERROR_USER_CANCEL);}
	}
	return true;
}








LIBSPEC  HANDLHA LHA_OpenArchive(const char * LHAfileName,const char * Directory)

{


	
	__try
	{

	char           *p, inpbuf[256];
	static char temp[256]={0};

	int i;


	char **av, *m;
	int  ac=4;
	int argc = 4;//argc;
    char *argv[4];//=new char[4]; 
	int cmd;

	char		  **xfilev;
	int             xfilec = 257;

	jmp_buf UnlhaEnd;
	int  g_errorCode = 0;


    BOOL	g_bExtractMem=FALSE;
	init_variable();		/* Added N.Watazaki */
	temporary_fp = NULL;	// Added by Hanamaru
	BOOL g_bCurrentDirectory = TRUE;
	argv[0]=new char;
	argv[1]=new char;
	argv[2]=new char;
	argv[3]=new char;
	strcpy(argv[1],"x");
	strcpy(argv[2], LHAfileName);
	strcpy(argv[3], Directory);


	
	av = (char **)LocalAlloc( LPTR, sizeof(char*)*argc );
	if (av == NULL)
		return NULL;//ERROR_ENOUGH_MEMORY;
//		fatal_error2("not enough memory\n",
//					ERROR_ENOUGH_MEMORY);
	for (i=0; i<argc; i++) {
	  if ((av[i] = strdup( argv[i] )) == NULL)
		return NULL;//ERROR_ENOUGH_MEMORY;
//		fatal_error2("not enough memory\n",
//			ERROR_ENOUGH_MEMORY);
	}

/*	if (ac < 2){
		//print_tiny_usage_and_exit();
		return NULL;//ERROR_COMMAND_NAME;
	}*/

/*	if (strcmp(av[1], "--version") == 0) {
		//print_version();
	return NULL;
	}*/

	if (ac < 3) {
		cmd = CMD_LIST;
		av--; /* argv--; */ /* 1999.7.18 */
		ac++; /* argc++; */
		goto work;
	}
	
	m = av[1];

	if (m[0] == '-')
		m++;
	/* commands */
//	switch (*m) {
//	case 'x':
//	case 'e':
		cmd = CMD_EXTRACT;
	//	break;

/*	case 'p':
		output_to_stdout = TRUE;
		g_bExtractMem = TRUE;
		cmd = CMD_EXTRACT;
		break;

	case 'c':
		new_archive = TRUE;
		cmd = CMD_ADD;
		break;

	case 'a':
		cmd = CMD_ADD;
		break;

	case 'd':
		cmd = CMD_DELETE;
		break;

	case 'u':
		update_if_newer = TRUE;
		cmd = CMD_ADD;
		break;

	case 'm':
		delete_after_append = TRUE;
		cmd = CMD_ADD;
		break;

	case 'v':
		verbose_listing = TRUE;
		cmd = CMD_LIST;
		break;

	case 'l':
		cmd = CMD_LIST;
		break;

	case 't':
		cmd = CMD_EXTRACT;
		verify_mode = TRUE;
		break;*/

/*	default:
		{
		//print_tiny_usage_and_exit();
		return NULL;
		}

	}*/

	/* options */
	/* p = &argv[1][1]; */
	p = m+1;
	while ( *p != 0 ) {
		switch ((*p++)) {
		case 'q':
			switch (*p) {
			case '0':
			case '1':
				quiet_mode = *p - '0';
				++p;
				break;
			case '2':
			default:
				quiet = TRUE;
				break;
			}
			break;
		case 'f':
			force = TRUE;
			break;
		case 'p':
			prof = TRUE;
			break;
		case 'v':
			verbose = TRUE;
			break;
		case 't':
			text_mode = TRUE;
			break;
		case 'b':
			//g_bMakeBackupFile = TRUE;
			break;
#ifdef EUC
		case 'e':
			text_mode = TRUE;
			euc_mode = TRUE;
			break;
#endif
		case 'n':
			noexec = TRUE;
			break;
		case 'g':
			generic_format = TRUE;
			noconvertcase = TRUE;
            header_level = 0;
			break;
		case 'd':
			delete_after_append = TRUE;
			break;
		case 'o':
			switch (*p) {
			case 0:
				compress_method = LZHUFF1_METHOD_NUM;
				header_level = 0;
				break;
			case '5':
				compress_method = LZHUFF5_METHOD_NUM;
				p++;
				break;
#ifdef SUPPORT_LH7
            case '6':
				compress_method = LZHUFF6_METHOD_NUM;
				p++;
				break;
			case '7':
				compress_method = LZHUFF7_METHOD_NUM;
				p++;
				break;
#endif
			default:
				sprintf(temp, "LHa: error option o%c\n", p[-1]);
				//printfToBuffer(temp);
				return NULL;
			}
     		break;
		case 'z':
			compress_method = LZHUFF0_METHOD_NUM;	/* Changed N.Watazaki */
			break;
		case 'i':
			ignore_directory = TRUE;
			break;
		case 'w':
			if (*p == '=')
				p++;
			extract_directory = p;
			while (*p)
				p++;
			break;
		case '0':
			header_level = HEADER_LEVEL0;
			break;
		case '1':
			header_level = HEADER_LEVEL1;
			break;
		case '2':
			header_level = HEADER_LEVEL2;
			break;
		default:
		//	sprintf(temp, "LHa: Unknown option '%c'.\n", p[-1]);
			//printfToBuffer(temp);
			return NULL;//ERROR_COMMAND_NAME;
		}
	}

work:
	/* archive file name */
	archive_name = av[2];

	if (!strcmp(archive_name, "-")) {
		if (!isatty(1) && cmd == CMD_ADD)
			quiet = TRUE;
	}
	else {
		if (ac == 3 && !isatty(0)) { /* 1999.7.18 */
//			get_filename_from_stdin = TRUE;
		}
	}

	/* target file name */
	if (get_filename_from_stdin) {
		cmd_filec = 0;
		if ((xfilev = (char **)LocalAlloc( LPTR, sizeof(char *) * xfilec)) == NULL)
			/*fatal_error2("Virtual memory exhausted\n",
				ERROR_ENOUGH_MEMORY)*/;
		while (fgets(inpbuf, sizeof(inpbuf), stdin)) {
		    /* delete \n if it exist */
            i=0; p=inpbuf;
			while (i < sizeof(inpbuf) && p != 0) {
			    if (*p == '\n') {
				    *p = 0;
					break;
				}
				p++; i++;
			}

			if (cmd_filec >= xfilec) {
				xfilec += 256;
				cmd_filev = (char **)LocalReAlloc(xfilev,
						   sizeof(char *) * xfilec, LMEM_ZEROINIT);
				if (cmd_filev == NULL)
					fatal_error2("Virtual memory exhausted\n", 
						ERROR_ENOUGH_MEMORY);
				xfilev = cmd_filev;
			}
			if (strlen(inpbuf) < 1)
				continue;
			if ((xfilev[cmd_filec++] = (char *) strdup(inpbuf)) == NULL)
				fatal_error2("Virtual memory exhausted\n", 
					ERROR_ENOUGH_MEMORY);
		}
		xfilev[cmd_filec] = NULL;
		cmd_filev = xfilev;
	} else {
		if( g_bCurrentDirectory == TRUE ){
//			current_directory = av[3];
			strcpy (current_directory , av[3]);
			cmd_filec = ac - 4;
			if( cmd_filec > 0 ) cmd_filev = av + 4;
		}else{
			cmd_filec = ac - 3;
			if( cmd_filec > 0 ) cmd_filev = av + 3;
		}
	}
	sort_files();

	/* make crc table */
	LHAmake_crctable();

	// fatal_error ‚Å‚±‚±‚É–ß‚Á‚Ä‚­‚é
	if( setjmp(UnlhaEnd) != 0 ){
		// fatal_error ‚È‚ç–ß‚è’l‚Í­‚È‚­‚Æ‚à 1
		cmd = CMD_UNKNOWN;
		if(g_errorCode == 0) g_errorCode=1;
	}

///	switch (cmd) {
//	case CMD_EXTRACT:
		if( g_bCurrentDirectory == TRUE )
			extract_directory=av[3];

//	{
     HANDLHA lha=new LHA;
	LzHeader  hdr;
	long      pos;
	FILE     *afp;

	/* open archive file */
	if ((afp = open_old_archive()) == NULL)
		fatal_error2(archive_name, ERROR_ARC_FILE_OPEN);

	if (archive_is_msdos_sfx1(archive_name))
		skip_msdos_sfx1_code(afp);
	lha->hdr=hdr;
	lha->pos=pos;
	lha->afp=afp;




    return lha;

	//	}
		}
   __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   { 
	   return NULL;
   }
     //free(File);
	

    

	
	
//	}
/*

#ifndef AVIOD_MEMFREE
	//added by Mr.Hanamaru
	for (i=0; i<argc; i++) 
		if (av[i] != NULL){
			if(cmd_filev==&av[i]) cmd_filev = NULL;
			LocalFree (av[i]);
		}

	if (av != NULL){
		if(cmd_filev==av) cmd_filev = NULL;
		LocalFree(av);
	}

	free_hashbuf ();
	if (cmd_filev != NULL)
		LocalFree (cmd_filev);
#endif


#ifdef USE_PROF
	if (!prof)
		return 0;
//		exit(0);
#endif

	return NULL;*/

	//return NULL;
}
;
LIBSPEC  BOOL  LHA_CloseArchive(HANDLHA LHAFile)


{
	__try
	{

		/* close archive file */
	fclose(LHAFile->afp);
	delete(LHAFile);
	}
   __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   { 
	   return 0;
   }
     //free(File);
	

	return TRUE;
};
LIBSPEC  BOOL  LHA_GetFirstFile(HANDLHA LHAFile,char *FileReturn)

{

		__try
	{

	if (get_header(LHAFile->afp, &LHAFile->hdr))
	{
		if (need_file(LHAFile->hdr.name)) {
			LHAFile->pos = ftell(LHAFile->afp);
//			extract_one(afp, &hdr);	
			// ˆø”‚Ìƒtƒ@ƒCƒ‹–¼‚Æ”äŠr
			// ˆø”‚ª‚È‚¯‚ê‚Î‘S‰ð“€‚¾‚©‚ç–³ðŒ TRUE
			// ˆø”‚ª‚ ‚éê‡A”äŠr‚µ‚Äˆê’v‚µ‚½‚ç TRUE
			if(TRUE == compareFileName(cmd_filec, 
							cmd_filev,LHAFile->hdr.name)){
				extract_one(LHAFile->afp, &LHAFile->hdr);
				strcpy(FileReturn,LHAFile->hdr.name);
			}
			fseek(LHAFile->afp, LHAFile->pos + LHAFile->hdr.packed_size, SEEK_SET);
		}
		else {
			if (LHAFile->afp != stdin)
				fseek(LHAFile->afp, LHAFile->hdr.packed_size, SEEK_CUR);
			else {
				int i =LHAFile->hdr.packed_size;
				while (i--)
					fgetc(LHAFile->afp);
			}
		}
	
			return TRUE;
	}

	}//try
 __except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   { 
	   return FALSE;
   }

	return FALSE;
}
;
LIBSPEC  BOOL  LHA_GetNextFile(HANDLHA LHAFile,char *FileReturn)


{
		__try
	{
	if (get_header(LHAFile->afp, &LHAFile->hdr))
	{
		if (need_file(LHAFile->hdr.name)) {
			LHAFile->pos = ftell(LHAFile->afp);
//			extract_one(afp, &hdr);	
			// ˆø”‚Ìƒtƒ@ƒCƒ‹–¼‚Æ”äŠr
			// ˆø”‚ª‚È‚¯‚ê‚Î‘S‰ð“€‚¾‚©‚ç–³ðŒ TRUE
			// ˆø”‚ª‚ ‚éê‡A”äŠr‚µ‚Äˆê’v‚µ‚½‚ç TRUE
			if(TRUE == compareFileName(cmd_filec, 
							cmd_filev,LHAFile->hdr.name)){
				extract_one(LHAFile->afp, &LHAFile->hdr);
				strcpy(FileReturn,LHAFile->hdr.name);
			}
			fseek(LHAFile->afp, LHAFile->pos + LHAFile->hdr.packed_size, SEEK_SET);
		}
		else {
			if (LHAFile->afp != stdin)
				fseek(LHAFile->afp, LHAFile->hdr.packed_size, SEEK_CUR);
			else {
				int i =LHAFile->hdr.packed_size;
				while (i--)
					fgetc(LHAFile->afp);
			}
		}
	
			return TRUE;
	}
}//try

	__except(GetExceptionCode() == STATUS_INTEGER_OVERFLOW,1 )
   { 
	   return FALSE;
   }

	
	return FALSE;
};
