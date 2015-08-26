// 25beta.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#define _COMPILING_4AFD424F_87A7_419e_832C_F8FC0AA1BC2E
#include "25beta.h" 


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



LIBSPEC  HAND Rar_OpenArchive(const char * ZipfileName,const char *Directory)
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
        return &Cmd;

	 }



   }

	return NULL;
}
;


LIBSPEC  BOOL Rar_CloseArchive(HAND Cmd1)
{

	File::RemoveCreated();
	return ArcR.Close();
	
}
;

LIBSPEC   BOOL Rar_GetFirstFile(HAND Cmd1, char * FileReturn)
{




    int Size=ArcR.ReadHeader();
     Repeat=false;
	 success=false;
	char *Ret=NULL;
	const char *NEXT="NEXT";//for delphi 

	if (success=Extract.ExtractCurrentFile(&Cmd,ArcR,Size,Repeat))
	{
	strcpy(FileReturn,Extract.DestFileName+strlen(Cmd.ExtrPath));
	if (strlen(FileReturn)==0) strcpy(FileReturn,NEXT);//for delphi
	}





	return success;
};


LIBSPEC   BOOL  Rar_GetNextFile(HAND Cmd1, char * FileReturn)
{

	

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
      return 0;
	}

       Extract.ExtractArchiveInit(&Cmd,ArcR);
       int Size=ArcR.ReadHeader();
       Repeat=false;   
   
	   success=false;

	   if (success=Extract.ExtractCurrentFile(&Cmd,ArcR,Size,Repeat))
	   {

		strcpy(FileReturn,Extract.DestFileName+strlen(Cmd.ExtrPath));		   
		if (strlen(FileReturn)==0) strcpy(FileReturn,NEXT);//for delphi
	   }
	 }
	else 
	{

		 
       int Size=ArcR.ReadHeader();
       Repeat=false;   
   
	   success=false;

	   if (success=Extract.ExtractCurrentFile(&Cmd,ArcR,Size,Repeat))
	   {

    	
		strcpy(FileReturn,Extract.DestFileName+strlen(Cmd.ExtrPath));
    	if (strlen(FileReturn)==0) strcpy(FileReturn,NEXT);//for delphi
	   }
 
	   }

	 if (!Repeat&& !success)    return 0;


	 return success;

}

