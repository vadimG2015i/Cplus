///////////////////////////////////////////////////////////////////
// i6comp.c
//  
// InstallShield Compression and Maintenance util
// fOSSiL - 1999
//
// *Any use is authorized, granted the proper credit is given*
//
// No support will be provided for this code
//
// Changes by Morlac 27-dec-2000
// see changes.txt
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/utime.h>
#include <direct.h>


//#include "rsa/global.h"
//#include "rsa/md5.h"
#include "i6comp.h"
// Global Cab variables
 /*hCabHdr = 0;
//CABHEADER CabHdr;
// pCabDesc =(struct tagCABDESC *) NULL;
 //SetupTypes = NULL;
  cSetupTypes = 0;
//COMPONENTDESC* Components[CSCP_MAX];
 cComponents = 0;
//FILEGROUPDESC* FileGroups[CSFG_MAX];
 cFileGroups = 0;
// pCabPattern = NULL;
//Dirs = NULL;
//Files = NULL;

// Globals
 ver = ver60;
 //exename = NULL;
// pZBuf = NULL;
//pFileList = NULL;
//pDiskList = NULL;
// FileCount = 0;
 FileAttrVals[4] = {FILE_ATTR_ARCHIVE, FILE_ATTR_READONLY, FILE_ATTR_HIDDEN, FILE_ATTR_SYSTEM};
// FileAttrChars[] = "ARHS";

// Options
 /*optRecurseSubdirs = FALSE;
 optMatchWithDirs = FALSE;
 optPrintAll = TRUE;
 optFGasDir = FALSE;
 optVersion = 0;
 optFileGroup = (DWORD)-1;
// Morlac
 optVerbose = FALSE;*/

/*
void main(int argc, char* argv[])
{
	exename = argv[0];
	
	if (argc < 3) {
		StartMsg();
		Usage();
	}
	
	__try {
		DispatchCommand(argc - 1, argv + 1);
	} __except(1) {
		// catch all exceptions
		GeneralException();
	}
	CleanExit(0);
}*/

void StartMsg()
{
	/*if (optPrintAll)
	//	fprintf(stderr, "InstallShield 6.x Cabinet Util\n")
		;
		//fprintf(stderr, "Version 0.2 -] fOSSiL & Morlac - 2000 [-\n")
			;*/

}

void Usage()
{
	/*if (optPrintAll) {
		LPSTR pName;
		pName = strrchr(exename, '\\');
		pName = pName ? pName + 1 : exename;

		fprintf(stdout, "\nUsage: %s <cmd> [-opts] <cab> [file index|fmask|path] [disk fmask|path]\n", pName);
		fprintf(stdout, "Commands\n");
		fprintf(stdout, "\tl: list Files\n");
		fprintf(stdout, "\tg: list File Groups    c: list Components   t: list Setup Types\n");
		fprintf(stdout, "\ts: convert multi-volume cab to a single volume (not recommended)\n");
		fprintf(stdout, "\te: extract files (specify <cab index|mask> when specifying <disk path>)\n");
		fprintf(stdout, "\tx: same as 'e' with subdirs\n");
		fprintf(stdout, "\tr: replace files in cab (same syntax as 'e')\n");
		fprintf(stdout, "\td: delete files from cab (specify <cab index|mask>)\n");
		fprintf(stdout, "\ta: add files to cab (<cab path> is optional; must specify -g OR -f)\n");
		fprintf(stdout, "\tz: zero-size files (specify <cab index|mask>)\n");
		fprintf(stdout, "Options\n");
		fprintf(stdout, "\tv: verbose info mode\n");
		fprintf(stdout, "\tr: extract subdirs/recurse and store subdirs\n");
		fprintf(stdout, "\td: include directories in cab matches\n");
		fprintf(stdout, "\tg<name|index>: specifies File Group to work with\n");
		fprintf(stdout, "\to: suppress supplementary output (start msg, comments, etc.)\n");
		fprintf(stdout, "\tf: treat File Groups as directories (usefull for GUI, wrappers)\n");
	}*/
	CleanExit(100);
}

void CleanExit(int code)
{
	if (hCabHdr != 0)
		close(hCabHdr);

	if (Dirs != NULL)
		DirsArrayDestruct(Dirs);
	if (Files != 0) {
		long i;
		for (i = pCabDesc->cFiles - 1; i >= 0; i--) {
			if (!(Files[i].DescStatus & DESC_INVALID))
				free(Files[i].lpName);
		}
		Free(Files);
	}

	Free(pCabDesc);
	//Free(pCabPattern);
	Free(pZBuf);

	if (pFileList) {
		LPCABFILELIST pTemp;
		while (pFileList) {
			pTemp = pFileList->pNext;
			Free(pFileList);
			pFileList = pTemp;
		}
	}
	if (pDiskList) {
		LPDISKFILELIST pTemp;
		while (pDiskList) {
			pTemp = pDiskList->pNext;
			Free(pDiskList);
			pDiskList = pTemp;
		}
	}	
	//exit(code);
	//	_cexit();
}

void DispatchCommand(int argc, char* argv[])
{
	int arg = 0;
	char cmd;
	char* cabname;

	cmd = toupper(argv[arg][0]);
	arg++;
	while (arg < argc && argv[arg][0] == '-') {
		switch (toupper(argv[arg][1])) {
			case 'R':
				optRecurseSubdirs = TRUE;
				break;
			case 'D':
				optMatchWithDirs = TRUE;
				break;
			case 'V':
				optVerbose = TRUE;
				break;
			case 'G':
				optFileGroup = atolong(argv[arg] + 2);
				if (optFileGroup == (DWORD)-1)
					optFileGroup = (DWORD)(argv[arg] + 2);
				break;
			case 'O':
				optPrintAll = FALSE;
				break;
			case 'F':
				optFGasDir = TRUE;
				break;
			default:
				StartMsg();
				//fprintf(stderr, "\nUnknown option specified\n");
				Usage();
		}
		arg++;
	}
	if (arg >= argc) {
		StartMsg();
//		fprintf(stderr, "\nNo cab file specified\n");
		Usage();
	}
	
	StartMsg();

	cabname = argv[arg];
	arg++;

	switch (cmd) {
		case 'L':
			InitCabFile(cabname, O_RDONLY);
			TranslateFileGroup();
			CabBasedFileList(arg < argc ? argv[arg] : NULL, NULL);
			arg += 1;
			ListFiles();
			break;
		
		case 'T':
			InitCabFile(cabname, O_RDONLY);
			ListSetupTypes();
			break;
		
		case 'C':
			InitCabFile(cabname, O_RDONLY);
			ListComponents();
			break;
		
		case 'G':
			InitCabFile(cabname, O_RDONLY);
			TranslateFileGroup();
			ListFileGroups();
			break;
		
		case 'X':
			optRecurseSubdirs = TRUE;
		case 'E':
			InitCabFile(cabname, O_RDONLY);
			TranslateFileGroup();
			CabBasedFileList(arg < argc ? argv[arg] : NULL, arg + 1 < argc ? argv[arg + 1] : NULL);
			arg += 2;
			ExtractFiles();
			break;
		
		case 'S':
			InitCabFile(cabname, O_RDWR);
			ConvertToSingle();
			arg += 1;
			break;
		
		case 'R':
			InitCabFile(cabname, O_RDWR);
			TranslateFileGroup();
			CabBasedFileList(arg < argc ? argv[arg] : NULL, arg + 1 < argc ? argv[arg + 1] : NULL);
			arg += 2;
			ReplaceFiles();
			break;
		
		case 'D':
			InitCabFile(cabname, O_RDWR);
			TranslateFileGroup();
			if (arg < argc)
				CabBasedFileList(argv[arg], NULL);
			else
				NotEnoughParams("'d': ");
			arg += 1;
			DeleteFiles();
			break;

		case 'Z':
			InitCabFile(cabname, O_RDWR);
			TranslateFileGroup();
			if (arg < argc)
				CabBasedFileList(argv[arg], NULL);
			else
				NotEnoughParams("'d': ");
			arg += 1;
			ZeroSizeFiles();
			break;

		case 'A':
			InitCabFile(cabname, O_RDWR);
			TranslateFileGroup();
			switch (argc - arg) {
				case 1:
					DiskBasedFileList(NULL, argv[arg]);
					arg += 1;
					break;
				case 2:
					DiskBasedFileList(argv[arg], argv[arg + 1]);
					arg += 2;
					break;
				default:
					NotEnoughParams("'a': ");
			}
			AddFiles();
			break;

		default:
//			fprintf(stderr, "Unknown command specified\n");
			Usage();
	}
}

void ConvertToSingle()
{
	DWORD i;
	int vol = 1;
	FILEHANDLE hMainCab;
	FILEHANDLE hRead = 0;
	FILEDESC* pFile;
	FILESIZE ToCopy, FromOfs;
	CABHEADER curCabHdr;
	CABHEADER FirstVolHdr;
	DWORD Vol1 = 2;

	// we begin from the 2nd volume now
	hMainCab = OpenCabReadHeader(Vol1, "cab", O_RDWR, &FirstVolHdr);

	if (FirstVolHdr.LastFile == pCabDesc->cFiles - 1 &&
		Files[FirstVolHdr.LastFile].Volume == Vol1 &&
		FirstVolHdr.cbLastData == 0)
	{
		char buf[256];

		close(hMainCab);
		sprintf(buf, pCabPattern, 1, "cab");
//		fprintf(stderr, "Already a single-volume cab (%s contains only engine files)\n", buf);
		CleanExit(11);
	} 

	fileseek(hMainCab, 0, SEEK_END);

	for (i = FirstVolHdr.LastFile; i < pCabDesc->cFiles; i++)
	{
		FILESIZE ToTran;
		DWORD CurVol;
		FILESIZE CabPtr;

		pFile = Files + i;
		
		CabPtr = filetell(hMainCab);

		if ((pFile->DescStatus & DESC_INVALID) == 0 && pFile->ofsData != 0 && pFile->cbCompressed != 0)
		{
			// check if the file is a dupe of another file
			if (pFile->LinkFlags & LINK_PREV) {
				// it's a dupe, get correct data pointer
				CabPtr = Files[pFile->PrevDupe].ofsData;
				
				if (hRead != 0 && i == curCabHdr.LastFile) {
					close(hRead);
					hRead = 0;
					if (CurVol > Vol1) {
						// delete volume, if it's not 1st
						char cabname[256];
						sprintf(cabname, pCabPattern, CurVol, "cab");
						DeleteCab(cabname);
					}
					CurVol++;
				}

			} else {
				// either unique or first one of dupes
				// transfer data to 1st vol
				ToTran = pFile->cbCompressed;
				CurVol = pFile->Volume;

				if (CurVol == Vol1 && (FirstVolHdr.LinkFlags & LINK_NEXT) && i == FirstVolHdr.LastFile) {
					// last file in the 1st volume
					ToTran -= FirstVolHdr.cbLastData;
					CabPtr = FirstVolHdr.ofsLastData;
					CurVol++;
				}

				do {
					if (hRead == 0)
						hRead = OpenCabReadHeader(CurVol, "cab", O_RDONLY, &curCabHdr);
					
					// check if the file is split
					if (i == curCabHdr.FirstFile && (curCabHdr.LinkFlags & LINK_PREV)) {
						ToCopy = curCabHdr.cbFirstData;
						FromOfs = curCabHdr.ofsFirstData;

					} else if (i == curCabHdr.LastFile && (curCabHdr.LinkFlags & LINK_NEXT)) {
						ToCopy = curCabHdr.cbLastData;
						FromOfs = curCabHdr.ofsLastData;

					} else {
						ToCopy = pFile->cbCompressed;
						FromOfs = pFile->ofsData;
					}

					fileseek(hRead, FromOfs, SEEK_SET);
					TransferData(hRead, hMainCab, ToCopy, CRYPT_NONE, 0);
					ToTran -= ToCopy;

					if (i == curCabHdr.LastFile) {
						close(hRead);
						hRead = 0;
						if (CurVol > Vol1) {
							// delete volume, if it's not 1st
							char cabname[256];
							sprintf(cabname, pCabPattern, CurVol, "cab");
							DeleteCab(cabname);
						}
						CurVol++;
					}

				} while (ToTran > 0);
			}		
		}

		pFile->DescStatus &= ~(DWORD)DESC_SPLIT;
		pFile->ofsData = CabPtr;
		pFile->Volume = Vol1;
	}

	// Patch header to reflect the change to a single volume
	FirstVolHdr.LinkFlags &= ~LINK_NEXT;
	FirstVolHdr.LastFile = pCabDesc->cFiles - 1;
	FirstVolHdr.ofsLastData = 0;
	FirstVolHdr.cbLastExpanded = 0;
	FirstVolHdr.cbLastData = 0;

	{// Update FileGroups to reflect the change
		DWORD i;
		LPFILEGROUPDESC pFG;

		for (i=0; i < cFileGroups; i++) {
			pFG = FileGroups[i];
			//if (pFG->FirstVolume > 0) {
			//	pFG->FirstVolume = 1;
			//	pFG->LastVolume = 1;
			//}
		}
	}
	// save 1st volume header
	SaveCabHeader(hMainCab, &FirstVolHdr);
	close(hMainCab);

	// save .hdr
	SaveCabHeaders(TRUE);

	if (optPrintAll)
//		fprintf(stderr, "Successfully converted to single volume.\n")
        ;
}

void DeleteCab(LPSTR cabname)
{
	if (remove(cabname) != 0)
//		fprintf(stderr, "Could not delete %s, error code %d\n", cabname, errno)
		;
}

void EncryptBuffer(BYTE* Buf, DWORD dwTran, DWORD* pTotal)
{
	BYTE ts;
	for (; dwTran > 0; dwTran--, Buf++, (*pTotal)++) {
		ts = (BYTE)(*pTotal % 0x47) + *Buf;
		__asm { rol byte ptr ts, 2 };
		*Buf = ts ^ 0xd5;
	}
}

void DecryptBuffer(BYTE* Buf, DWORD dwTran, DWORD* pTotal)
{
	BYTE ts;
	for (; dwTran > 0; dwTran--, Buf++, (*pTotal)++) {
		ts = *Buf ^ 0xd5;
		__asm { ror byte ptr ts, 2 };
		*Buf = ts - (BYTE)(*pTotal % 0x47);
	}
}

void TransferData(FILEHANDLE hFrom, FILEHANDLE hTo, FILESIZE Length, DWORD Crypt, MD5_CTX* DigestCtx)
{
	long dwTran;
	BYTE* pBuf;
	long ToTran = 1048576;	// 1Meg
	DWORD Total = 0;

	pBuf = (unsigned char *)Alloc(ToTran);

	while (Length) {
		if (ToTran > Length)
			ToTran = (long)Length;
		
		dwTran = read(hFrom, pBuf, ToTran);
		if (dwTran != ToTran)
			CantReadFile(errno);
	
		// digest it if requested
		if (DigestCtx != 0)
			MD5Update(DigestCtx, pBuf, dwTran);

		if (Crypt == CRYPT_ENCRYPT)
			EncryptBuffer(pBuf, dwTran, &Total);
		else if (Crypt == CRYPT_DECRYPT)
			DecryptBuffer(pBuf, dwTran, &Total);

		if (write(hTo, pBuf, ToTran) != ToTran)
			CantWriteFile(errno);
		
		Length -= dwTran;
	}
	Free(pBuf);
}

void InitZData(void* pHnds, ZDProcess proc)
{
	ZDataSetup(2, 0xfa372dea, 0);
	pZBuf = Alloc(STD_DECOMP_BUFFER);
	ZDataSetInfo(INFO_BUFFER_PTR, (DWORD)pZBuf);
	ZDataSetInfo(INFO_BUFFER_SIZE, STD_DECOMP_BUFFER);
	ZDataSetInfo(INFO_CALLBACK_PARAM, (DWORD)pHnds);

	ZDataSetInfo(INFO_WRITE_CALLBACK, (DWORD)WriteData);

	if (proc == zdCompress) {
		ZDataSetInfo(INFO_READ_CALLBACK, (DWORD)ReadData);
	} else if (proc == zdDecompress) {
		ZDataSetInfo(INFO_READ_CALLBACK, (DWORD)ExtractRead);
	}
}

void ExtractData(EXTRACTINFO* pHnd, DWORD Crypt)
{
	BYTE* Buf;
	DWORD dwRead;
	DWORD Total = 0;

	Buf = (unsigned char *)Alloc(262144);
	dwRead = 262144;

	while (ExtractRead(Buf, &dwRead, pHnd) == 0 && dwRead != 0) {
		
		if (Crypt == CRYPT_DECRYPT)
			DecryptBuffer(Buf, dwRead, &Total);
		
		WriteData(Buf, &dwRead, (RWHANDLES*)pHnd);
		dwRead = 262144; // come again
	}
	
	Free(Buf);
}

void ExtractFiles()
{
	EXTRACTINFO rwHnd;
	LPCABFILELIST pCurrent;

	InitZData(&rwHnd, zdDecompress);
	
	for (pCurrent = pFileList; pCurrent != 0; pCurrent = pCurrent->pNext)
	{
		DWORD i = pCurrent->CabIndex;
		LPSTR pName = pCurrent->FileName;
		LPSTR pSlash;
		FILEDESC* pFile = Files + i;
		FILEDESC* pDupe;

		// check if the file is a dupe of another file and resolve
		for (pDupe = pFile;
			 pDupe->LinkFlags & LINK_PREV;
			 i = pDupe->PrevDupe, pDupe = Files + i)
			;

		rwHnd.FileIndex = i;		
		rwHnd.Volume = pDupe->Volume;
		rwHnd.ofsData = pDupe->ofsData;
		rwHnd.hRead = 0;
		rwHnd.BytesIn = pDupe->cbCompressed;
		rwHnd.BytesOut = 0;
		
		pSlash = strrchr(pName, '\\');
		if (pSlash) {
			*pSlash = '\0';
			if (!DirExists(pName))
				CreateDir(pName);
			*pSlash = '\\';
		}

		rwHnd.hWrite = OpenForWrite(pName, O_CREAT | O_TRUNC);
	
		if (pFile->DescStatus & DESC_COMPRESSED) {
			__try {
				DWORD Res;

				if (ZDataStart(0))
					CantDecompress(pName);
				
				if (Res=ZDataDecompress(0))
					CantDecompress(pName);
				else { 
					if (optPrintAll || rwHnd.BytesOut != pFile->cbExpanded)
						//fprintf(stdout, "%s", pName)
						;
					if (rwHnd.BytesOut != pFile->cbExpanded)
						NoMatchBytesOut();
					if (optPrintAll || rwHnd.BytesOut != pFile->cbExpanded)
						//fprintf(stdout, "\n")
						;
				}
				ZDataEnd();
			
			} __except(1) {
				ExceptInDecompress();
			}
		
		} else {
			ExtractData(&rwHnd, pFile->DescStatus & DESC_ENCRYPTED ? CRYPT_DECRYPT : CRYPT_NONE);
			if (optPrintAll)
				//fprintf(stdout, "%s\n", pName)
				;
		}
		close(rwHnd.hWrite);
		SetFileDosTime(pName, pFile->FatDate, pFile->FatTime);
		SetFileAttrs(pName, pFile->Attrs);
	}
}

LPFILEGROUPDESC GetGroupByFile(DWORD index)
{
	LPFILEGROUPDESC pFG;
	DWORD i;

	for (i = 0;
		 i < cFileGroups && (index < (pFG=FileGroups[i])->FirstFile || index > pFG->LastFile);
		 i++)
		;
	return i < cFileGroups ? pFG : NULL;
}

void CreateDir(LPSTR dirname)
{
	if (mkdir(dirname) == -1) {
		if (errno == ENOENT) {
			char* pSlash;
			pSlash = strrchr(dirname, '\\');
			if (pSlash) {
				*pSlash = '\0';
				CreateDir(dirname);
				*pSlash = '\\';
				CreateDir(dirname);
				return;
			}
		}
		if (errno == EACCES)
			return;

		//fprintf(stderr, "Can not create directory %s\n", dirname);
		CleanExit(9);
	}
}

int ZDCALLBACK ReadData(void* pbuf, DWORD* size, LPRWHANDLES pHnd)
{
	DWORD dwRead;

	if (!pHnd->BytesIn) {
		*size = 0;
		return 0;
	}
	if (pHnd->BytesIn < *size)
		*size = (DWORD)pHnd->BytesIn;

	dwRead = read(pHnd->hRead, pbuf, *size);
	if (dwRead != 0) {
		pHnd->BytesIn -= dwRead;
		*size = dwRead;

		if (pHnd->DigestCtx != 0)
			MD5Update(pHnd->DigestCtx,(unsigned char *) pbuf, dwRead);

		return 0;
	
	} else {
		return errno;
	}
}

int ZDCALLBACK WriteData(void* pbuf, DWORD* size, LPRWHANDLES pHnd)
{
	DWORD dwWritten;

	dwWritten = write(pHnd->hWrite, pbuf, *size);
	if (dwWritten != 0) {
		*size = dwWritten;
		pHnd->BytesOut += dwWritten;
		return 0;
	} else
		return errno;
}

int ZDCALLBACK ExtractRead(void* pbuf, DWORD* size, EXTRACTINFO* pHnd)
{
	DWORD dwRead = 0;
	DWORD TotalRead = 0;
	DWORD ToTran = 0;
	DWORD dwWant;

	if (pHnd->BytesIn == 0) {
		*size = 0;
		return 0;
	}
	
	dwWant = *size;

	if (pHnd->BytesIn < dwWant)
		dwWant = (DWORD)pHnd->BytesIn;

	do {
		if (pHnd->hRead == 0) {
			CABHEADER curCabHdr;

			pHnd->hRead = OpenCabReadHeader(pHnd->Volume, "cab", O_RDONLY, &curCabHdr);
			if (pHnd->hRead==-1) return errno;
			if (curCabHdr.FirstFile == pHnd->FileIndex && (curCabHdr.LinkFlags & LINK_PREV)) {
				// first file in this volume
				pHnd->BytesHere = curCabHdr.cbFirstData;
				fileseek(pHnd->hRead, curCabHdr.ofsFirstData, SEEK_SET);

			} else if (curCabHdr.LastFile == pHnd->FileIndex && (curCabHdr.LinkFlags & LINK_NEXT)) {
				// last file in this volume
				pHnd->BytesHere = curCabHdr.cbLastData;
				fileseek(pHnd->hRead, curCabHdr.ofsLastData, SEEK_SET);

			} else {
				// all data in one volume
				pHnd->BytesHere = pHnd->BytesIn;
				fileseek(pHnd->hRead, pHnd->ofsData, SEEK_SET);
			}

			pHnd->Volume++;
		}

		ToTran = pHnd->BytesHere > dwWant ? dwWant : (DWORD)pHnd->BytesHere;
		
		dwRead = read(pHnd->hRead, (BYTE*)pbuf + TotalRead, ToTran);
		pHnd->BytesIn -= dwRead;
		pHnd->BytesHere -= dwRead;
		TotalRead += dwRead;
		dwWant -= dwRead;

		if (pHnd->BytesHere == 0) {
			// all data read from this volume
			close(pHnd->hRead);
			pHnd->hRead = 0;
		}

	} while (dwWant > 0 && dwRead != 0);

	if (TotalRead != 0) {
		*size = TotalRead;
		return 0;
	} else
		return errno;
}

FILEHANDLE OpenCabForAdd(DWORD FileIndex)
{
	FILEHANDLE hCab;
	DWORD Vol, i;
	CABHEADER curCabHdr;
	FILEDESC* pFile = Files + FileIndex;
	FILEGROUPDESC* pFG = GetGroupByFile(FileIndex);

	// find last volume referenced by this FileGroup
	// default Volume is 1
	for (i = pFG->FirstFile, Vol = 1; i <= pFG->LastFile; i++) {
		if (Files[i].Volume > Vol)
			Vol = Files[i].Volume;
	}

	if (Vol > 1)
		Vol = GetLastRefVolume();	// user files, add to last volume
		
	// seek to the first volume that doesn't have a split last file
	hCab = OpenCabReadHeader(Vol, "cab", O_RDWR, &curCabHdr);
	while (curCabHdr.LinkFlags & LINK_NEXT) {
		// split file, go to next volume
		close(hCab);
		Vol++;
		hCab = OpenCabReadHeader(Vol, "cab", O_RDWR, &curCabHdr);
	}
	
	// update cab header	
	curCabHdr.LastFile = FileIndex;
	SaveCabHeader(hCab, &curCabHdr);

	pFile->Volume = Vol;

	return hCab;
}

void AddFileBody(DWORD Index, char* FileName, RWHANDLES* pHnd)
{
	FILEDESC* pFile = Files + Index;
	FILEGROUPDESC* pFG = GetGroupByFile(Index);
	FILESIZE ofsNew, sizeNew;
	BOOL Store;
	ZCOMPRESSBUF ZParam;
	MD5_CTX DigestCtx;
	BOOL bCloseFile = FALSE;

	pHnd->DigestCtx = &DigestCtx;

	if (pHnd->hRead == 0) {
		pHnd->hRead = OpenForRead(FileName);
		bCloseFile = TRUE;
	}

	sizeNew = filelength(pHnd->hRead);

	pHnd->hWrite = OpenCabForAdd(Index);
	fileseek(pHnd->hWrite, 0, SEEK_END);
	ofsNew = filetell(pHnd->hWrite);
	
	if (pFile->DescStatus & DESC_COMPRESSED)
		Store = FALSE;
	else
		Store = TRUE;

	if (!Store) {
		pHnd->BytesIn = sizeNew;
		pHnd->BytesOut = 0;

		MD5Init(pHnd->DigestCtx);

		if (ZDataStart(0))
			CantCompressError(FileName);
		
		if (ZDataCompress(sizeof(ZParam), &ZParam) || sizeNew * 0.95 < pHnd->BytesOut) {
			CantCompressStore(FileName);
			pFile->DescStatus &= ~DESC_COMPRESSED;
			Store = TRUE;
			fileseek(pHnd->hWrite, ofsNew, SEEK_SET);
			fileseek(pHnd->hRead, 0, SEEK_SET);
		}
		ZDataEnd();
	}
	
	if (Store) {
		// either file wasn't compressed at all
		// or it could not be sufficiently compressed
		MD5Init(pHnd->DigestCtx);

		pHnd->BytesOut = sizeNew;
		TransferData(pHnd->hRead,
					pHnd->hWrite,
					sizeNew,
					pFile->DescStatus & DESC_ENCRYPTED ? CRYPT_ENCRYPT : CRYPT_NONE,
					pHnd->DigestCtx);
	}

	// store MD5 file digest
	MD5Final(pFile->MD5Sig, pHnd->DigestCtx);

	chsize(pHnd->hWrite, tell(pHnd->hWrite));
	close(pHnd->hWrite);

	if (bCloseFile) {
		// cleaup after ourselves
		close(pHnd->hRead);
		pHnd->hRead = 0;
	}

	pFile->cbExpanded = sizeNew;
	pFile->cbCompressed = pHnd->BytesOut;
	pFile->ofsData = ofsNew;
	pFile->DescStatus &= ~DESC_SPLIT;

	if (pFG) {
		pFG->cbExpanded += pFile->cbExpanded;
		pFG->cbCompressed += pFile->cbCompressed;
	}
}

void ReplaceFiles()
{
	LPCABFILELIST pCurrent;
	RWHANDLES rwHnd;

	InitZData(&rwHnd, zdCompress);

	for (pCurrent = pFileList; pCurrent != 0; pCurrent = pCurrent->pNext)
	{
		LPSTR pName = pCurrent->FileName;
		DWORD Index = pCurrent->CabIndex;

		// open file now, to make sure it exists
		rwHnd.hRead = OpenForRead(pName);

		RemoveFileBody(Index);

		AddFileBody(Index, pName, &rwHnd);

		close(rwHnd.hRead);
		
		if (optPrintAll)
			//fprintf(stdout, "%s\n", pName)
			;
	}
	
	SaveCabHeaders(TRUE);
}

DWORD GetFileAttrs(char* filename)
{
	struct stat s;
	DWORD Attrs = FILE_ATTR_ARCHIVE;

	if (stat(filename, &s) != -1) {
		if (!(s.st_mode & S_IWRITE))
			Attrs |= FILE_ATTR_READONLY;
	} else {
	//	fprintf(stderr, "Could not get file attributes of %s\n", filename)
		;
	}
	return Attrs;
}

void SetFileAttrs(char* filename, DWORD Attrs)
{
	int mode = S_IREAD;
	if (!(Attrs & FILE_ATTR_READONLY))
		mode |= S_IWRITE;
	chmod(filename, mode);
}

const char _MonthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void FileTimeToDosTime(time_t t, WORD* DosDate, WORD* DosTime)
{
	SYSTIME st;
	long i;
	
	st.wSecond = t % 60;
	t /= 60;
	st.wMinute = t % 60;
	t /= 60;
	st.wHour = t % 24;
	t /= 24;

	// compute the year - cauz of leap mess, cant be done directly
	st.wYear = t / 365;
	for (i = t - (st.wYear * 365 + (st.wYear + 1) / 4);
		 i < 0;
		 st.wYear--, i = t - (st.wYear * 365 + (st.wYear + 1) / 4)
	) ;
	st.wYear += 1970;
	t = i;

	for (i = 0; t >= _MonthDays[i]; t -= _MonthDays[i], i++)
		if (i == 1 && st.wYear % 4 == 0)
			t--;	// adjust for leap Feb.
	if (t < 0) {
		// leap Feb. underflow
		t = 28;
		i--;
	}
	st.wMonth = i + 1;
	st.wDay = t + 1;

	SysTimeToDosTime(&st, DosDate, DosTime);
}

time_t DosTimeToFileTime(WORD DosDate, WORD DosTime)
{
	SYSTIME st;
	int i;
	long t;

	DosTimeToSysTime(DosDate, DosTime, &st);
	// unix time equiv. - days since Jan-1-1970
	t = (st.wYear - 1970) * 365 + (st.wYear - 1969) / 4;

	for (i = 0; i < st.wMonth - 1; i++)
		t += _MonthDays[i];
	
	if (st.wYear % 4 == 0 && st.wMonth > 2)
		t += 1;

	t += st.wDay - 1;

	t = ((t * 24 + st.wHour) * 60 + st.wMinute) * 60 + st.wSecond;

	return t;
}

void SysTimeToDosTime(SYSTIME* systime, WORD* DosDate, WORD* DosTime)
{
	*DosDate = ((systime->wYear - 1980) << 9) | (systime->wMonth << 5) | systime->wDay;
	*DosTime = (systime->wHour << 11) | (systime->wMinute << 5) | (systime->wSecond / 2);
}

void DosTimeToSysTime(WORD DosDate, WORD DosTime, SYSTIME* systime)
{
	systime->wYear = ((DosDate & 0xfe00) >> 9) + 1980;
	systime->wMonth = (DosDate & 0x01e0) >> 5;
	systime->wDay = (DosDate & 0x001f);
	systime->wHour = (DosTime & 0xf800) >> 11;
	systime->wMinute = (DosTime & 0x07e0) >> 5;
	systime->wSecond = (DosTime & 0x001f) * 2;
}

void GetFileDosTime(char* filename, WORD* DosDate, WORD* DosTime)
{
	struct stat s;

	if (stat(filename, &s) != -1) {
		FileTimeToDosTime(s.st_mtime, DosDate, DosTime);
	} else {
		*DosDate = 0;
		*DosTime = 0;
//		fprintf(stderr, "Could not get modification time of %s", filename);
	}
}

void SetFileDosTime(char* filename, WORD DosDate, WORD DosTime)
{
	struct utimbuf ut;
	ut.actime = ut.modtime = DosTimeToFileTime(DosDate, DosTime);
	if (utime(filename, &ut) == -1)
//		fprintf(stderr, "Could not set modification time of %s", filename)
;
}

void DisplayGUID(GUID *pGuid)
{
	/*fprintf(stdout, "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}\n",
		pGuid->Data1, pGuid->Data2, pGuid->Data3,
		pGuid->Data4[0], pGuid->Data4[1],
		pGuid->Data4[2], pGuid->Data4[3],
		pGuid->Data4[4], pGuid->Data4[5],
		pGuid->Data4[6], pGuid->Data4[7]);*/
}

DWORD GetNewFileInGroup(DWORD iFileGroup)
{
	FILEGROUPDESC* pFG = FileGroups[iFileGroup];
	DWORD i, iFG, iFile;

	// try to find an unused file descriptor
	for (i = pFG->FirstFile; i <= pFG->LastFile && !(Files[i].DescStatus & DESC_INVALID); i++)
		;
	if (i <= pFG->LastFile) {
		// found one - clean it out, but keep the VolN
		FILEDESC* pFile = Files + i;
		DWORD Vol = pFile->Volume;
		memset(pFile, 0, sizeof(*pFile));
		pFile->Volume = Vol;

		return i;
	}

	// let's do it the hard way
	// insert a new descriptor
	Files = (FILEDESC*) ReAlloc(Files, (pCabDesc->cFiles + 1) * sizeof(FILEDESC));
	memmove(Files + i + 1, Files + i, (pCabDesc->cFiles - i) * sizeof(FILEDESC));
	pCabDesc->cFiles++;

	memset(Files + i, 0, sizeof(Files[0]));

	// update cabs to reflect the shift in indices
	ReNumberVolumes(1, i, '<=', 1);

	// update FileGroups to reflect the shift in indices
	for (iFG = 0; iFG < cFileGroups; iFG++) {
		if (FileGroups[iFG]->FirstFile >= i) {
			FileGroups[iFG]->FirstFile++;
			FileGroups[iFG]->LastFile++;
		}
	}

	// update file dupe links to reflect the shift in indices
	for (iFile = 0; iFile < pCabDesc->cFiles; iFile++) {
		if (Files[iFile].PrevDupe >= i)
			Files[iFile].PrevDupe++;
		if (Files[iFile].NextDupe >= i)
			Files[iFile].NextDupe++;
	}

	// and extend current FileGroup
	pFG->LastFile = i;

	return i;
}

void AddFiles()
{
	RWHANDLES rwHnd;
	LPDISKFILELIST pCurrent;
	FILEGROUPDESC* pFG;

	// add new files
	if (optFileGroup == -1) {
//		fprintf(stderr, "'a': Must specify File Group to add to with -g OR use -f\n");
		CleanExit(25);
	}

	pFG = FileGroups[optFileGroup];

	InitZData(&rwHnd, zdCompress);

	for (pCurrent = pDiskList; pCurrent != 0; pCurrent = pCurrent->pNext)
	{
		DWORD iFile;
		FILEDESC* pFile;
		char DName[MAX_PATH];
		
		// translate cabpath to dir index
		pCurrent->CabDirInd = DirsArrayAddDir(&Dirs, pCurrent->CabDir);

		// make sure file exists first
		strcat(strcpy(DName, pCurrent->DiskDir), pCurrent->FileName);
		rwHnd.hRead = OpenForRead(DName);

		// check if a file with such a name exists in this dir already
		for (iFile = pFG->FirstFile;
			 iFile <= pFG->LastFile &&
			 ( (Files[iFile].DescStatus & DESC_INVALID) ||
			   stricmp(Files[iFile].lpName, pCurrent->FileName) != 0 ||
			   Files[iFile].DirIndex != pCurrent->CabDirInd );
			 iFile++)
				 ;
		if (iFile <= pFG->LastFile) {
			// such a file exists - replace it
			RemoveFileBody(iFile);

		} else {
			// insert a new file
			iFile = GetNewFileInGroup(optFileGroup);
		}
		
		pFile = Files + iFile;
		// if version support needed
		// it can be added later -
		// I dont feel like adding PE format handler to this code
		pFile->dwVerMS = 0;
		pFile->dwVerLS = 0;

		//Morlac -> from definitions to bit fields
		if (pFG->Attrs.is_Compressed)
			pFile->DescStatus = DESC_COMPRESSED;
		else if (pFG->Attrs.is_Encrypted)
			pFile->DescStatus = DESC_ENCRYPTED;

		pFile->lpName = strdup(pCurrent->FileName);
		pFile->DirIndex = pCurrent->CabDirInd;
		
		AddFileBody(iFile, DName, &rwHnd);
		
		if (optPrintAll)
//			fprintf(stdout,	"%s\n", DName)
;
		
		close(rwHnd.hRead);

		GetFileDosTime(DName, &pFile->FatDate, &pFile->FatTime);
		pFile->Attrs = GetFileAttrs(DName);
	}
	
	SaveCabHeaders(TRUE);
}

DFTABLE RebuildDFT()
{
	DWORD i;
	DWORD NewSize;
	DWORD ofsNextName;
	DFTABLE NewDFT;
	FILEDESC* NewFiles;

	pCabDesc->cDirs = Dirs->Count;

	NewSize = Dirs->Count * sizeof(DWORD); // Dirs ofs table
	// position of first File Desc.
	pCabDesc->cbDirs = NewSize;
	pCabDesc->ofsFilesDFT = NewSize;
	
	NewSize += pCabDesc->cFiles * sizeof(FILEDESC);
	// position of first Name (Dir/File)
	ofsNextName = NewSize;
	
	// sum filename lengths
	for (i = 0; i < pCabDesc->cFiles; i++) {
		if (Files[i].DescStatus & DESC_INVALID)
			continue;
		NewSize += strlen(Files[i].lpName) + 1;
	}

	// sum dirname lengths
	for (i = 0; i < Dirs->Count; i++)
		NewSize += strlen(Dirs->Dirs[i]) + 1;
	
	// update headers and descriptors (yet again)
	CabHdr.ofsCompData = CabHdr.ofsCabDesc + pCabDesc->ofsDFT + NewSize;
	pCabDesc->cbDFT = NewSize;
	pCabDesc->cbDFT2 = NewSize;

	// allocate and build new DFT
	NewDFT =(unsigned long *) Alloc(NewSize);

	// copy dirs
	for (i = 0; i < Dirs->Count; i++) {
		NewDFT[i] = ofsNextName;
		strcpy(GetString(NewDFT, ofsNextName), Dirs->Dirs[i]);
		ofsNextName += strlen(Dirs->Dirs[i]) + 1;
	}

	// copy files and filenames
	NewFiles = (FILEDESC*) ((BYTE*)NewDFT + pCabDesc->ofsFilesDFT);
	memcpy(NewFiles, Files, sizeof(Files[0]) * pCabDesc->cFiles);

	for (i = 0; i < pCabDesc->cFiles; i++) {
		if (NewFiles[i].DescStatus & DESC_INVALID)
			continue;
		// copy filename
		strcpy(GetString(NewDFT, ofsNextName), Files[i].lpName);
		// convert ptr to offset
		NewFiles[i].ofsName = ofsNextName;
		ofsNextName += strlen(GetString(NewDFT, ofsNextName)) + 1;
	}

	return NewDFT;
}

LPDIRARRAY DirsArrayBuild(DFTABLE DFT)
{
	LPDIRARRAY pDs;
	DWORD i;

	pDs = (struct tagDIRARRAY *)Alloc(sizeof(DIRARRAY) + pCabDesc->cDirs * sizeof(LPSTR));
	for (i = 0; i < pCabDesc->cDirs; i++)
		pDs->Dirs[i] = strdup(GetString(DFT, DFT[i]));
	pDs->Count = pCabDesc->cDirs;

	return pDs;
}

long DirsArrayFind(LPDIRARRAY pDA, LPSTR DirName)
{
	DWORD i;

	i = 0;
	while (i < pDA->Count && _stricmp(pDA->Dirs[i], DirName) != 0)
		i++;
	if (i < pDA->Count)
		return i;
	else
		return -1;
}

long DirsArrayAddDir(LPDIRARRAY* ppDA, LPSTR DirName)
{
	LPDIRARRAY pDs = *ppDA;
	DWORD NewSize;
	DWORD Ind;

	Ind = DirsArrayFind(pDs, DirName);
	if (Ind != -1)
		return Ind;
	NewSize = sizeof(DIRARRAY) + (pDs->Count + 1) * sizeof(LPSTR);
	if (NewSize > _msize(pDs))
		pDs = (struct tagDIRARRAY *)ReAlloc(pDs, NewSize + 3 * sizeof(LPSTR));
	Ind = pDs->Count;
	pDs->Dirs[Ind] = strdup(DirName);
	pDs->Count++;
	*ppDA = pDs;

	return Ind;
}

void DirsArrayDestruct(LPDIRARRAY pDA)
{
	DWORD i;

	for (i = 0; i < pDA->Count; i++)
		free(pDA->Dirs[i]);
	Free(pDA);
}

DWORD GetLastRefVolume()
{
	DWORD i, Vol = 1;

	for (i = 0; i < pCabDesc->cFiles; i++)
		if (Files[i].Volume > Vol)
			Vol = Files[i].Volume;

	return Vol;
}

int CompareLongs(long Val1, long Val2, WORD Relation)
{
	long comp = Val1 - Val2;
	
	switch (Relation) {
		case '==':
			return comp == 0;
		case '!=':
			return comp != 0;
		case '<':
			return comp < 0;
		case '<=':
			return comp <= 0;
		case '>':
			return comp > 0;
		case '>=':
			return comp >= 0;
	}
	return 0;
}

void ReNumberVolumes(DWORD FromVol, DWORD Index, WORD IndexRelation, long IndexDelta)
{
	FILEHANDLE hCab;
	CABHEADER curCabHdr;
	DWORD LastVol = GetLastRefVolume();
	DWORD LastFile;

	do {
		hCab = OpenCabReadHeader(FromVol, "cab", O_RDWR, &curCabHdr);
		
		LastFile = curCabHdr.LastFile;	// for '==' early termination

		// check prev link
		if (CompareLongs(Index, curCabHdr.FirstFile, IndexRelation))
			curCabHdr.FirstFile += IndexDelta;

		// check next link
		if (CompareLongs(Index, curCabHdr.LastFile, IndexRelation))
			curCabHdr.LastFile += IndexDelta;

		// update header
		SaveCabHeader(hCab, &curCabHdr);
		close(hCab);
		
		// next volume
		FromVol++;

	} while ( (FromVol <= LastVol || (curCabHdr.LinkFlags & LINK_NEXT)) &&
			  (IndexRelation != '==' || ((curCabHdr.LinkFlags & LINK_NEXT) && LastFile == Index))
			);
}

void RemoveFileBody(DWORD Index)
{
	LPFILEDESC pFile;
	LPFILEGROUPDESC pFG;
	DWORD MoveVol;
	FILESIZE ofsData, cbData;
	FILEHANDLE hCab;
	CABHEADER curCabHdr;
	DWORD DupeLinks;

	pFile = Files + Index;
	pFG = GetGroupByFile(Index);

	// update FileGroup statistics
	if (pFG) {
		pFG->cbExpanded -= pFile->cbExpanded;
		pFG->cbCompressed -= pFile->cbCompressed;
	}
	
	// check if it's a duped file
	// and unlink
	DupeLinks = pFile->LinkFlags & LINK_BOTH;
	// file will be unlinked
	pFile->LinkFlags = 0;
	
	switch (DupeLinks)
	{
		case LINK_NEXT:
			// it's the first file in the list
			// it actually contains the data
			Files[pFile->NextDupe].ofsData = pFile->ofsData;
			// important - copy vol#, since Next file becomes the first
			Files[pFile->NextDupe].Volume = pFile->Volume;
			// unlink now
			Files[pFile->NextDupe].PrevDupe = 0;
			Files[pFile->NextDupe].LinkFlags &= ~LINK_PREV;
			// if it's a split file, it is a mess
			if (pFile->DescStatus & DESC_SPLIT)
				ReNumberVolumes(pFile->Volume, Index, '==', pFile->NextDupe - Index);
			pFile->NextDupe = 0;
			return;

		case LINK_PREV:
			// it's the last file in the list - simply unlink
			Files[pFile->PrevDupe].NextDupe = 0;
			Files[pFile->PrevDupe].LinkFlags &= ~LINK_NEXT;
			pFile->PrevDupe = 0;
			return;
	
		case LINK_BOTH:
			// the file is in the middle of the list - cross unlink
			Files[pFile->PrevDupe].NextDupe = pFile->NextDupe;
			Files[pFile->NextDupe].PrevDupe = pFile->PrevDupe;
			pFile->NextDupe = 0;
			pFile->PrevDupe = 0;
			return;
	}

	// open first volume
	MoveVol = pFile->Volume;
	ofsData = pFile->ofsData;
	cbData = pFile->cbCompressed;
	hCab = OpenCabReadHeader(MoveVol, "cab", O_RDWR, &curCabHdr);

	while (curCabHdr.LastFile == Index && (curCabHdr.LinkFlags & LINK_NEXT)) {
		// it's the last file in volume and it's split
		// remove file data
		chsize(hCab, (long)curCabHdr.ofsLastData);
		// update header
		curCabHdr.LinkFlags &= ~LINK_NEXT;
		curCabHdr.ofsLastData = 0;
		curCabHdr.cbLastData = 0;
		curCabHdr.cbLastExpanded = 0;
		SaveCabHeader(hCab, &curCabHdr);
		close(hCab);
		
		// open next volume
		MoveVol++;
		hCab = OpenCabReadHeader(MoveVol, "cab", O_RDWR, &curCabHdr);
	}

	if (curCabHdr.FirstFile == Index && (curCabHdr.LinkFlags & LINK_PREV)) {
		// it's the last file in volume and it's split
		ofsData = curCabHdr.ofsFirstData;
		cbData = curCabHdr.cbFirstData;

		// update header
		curCabHdr.LinkFlags &= ~LINK_PREV;
		curCabHdr.ofsFirstData = 0;
		curCabHdr.cbFirstData = 0;
		curCabHdr.cbFirstExpanded = 0;
	}

	{// Clean out the old file from cab
		FILESIZE MoveSize, MoveShift;
		DWORD i;
		FILESIZE ofsTop, ofsBottom;

		ofsTop = ofsBottom = ofsData + cbData;

		// Find file right after the one removing and the last file
		for (i = 0; i < pCabDesc->cFiles; i++) {
			FILESIZE cbCurFile;
			FILEDESC* pF = Files + i;

			// ignore files in other volumes
			if ((pF->DescStatus & DESC_INVALID) || pF->Volume != MoveVol)
				continue;
			
			cbCurFile = pF->cbCompressed;
			if (curCabHdr.LastFile == i && (curCabHdr.LinkFlags & LINK_NEXT))
				cbCurFile = curCabHdr.cbLastData;	// last file in volume

			// just in case
			if (pF->ofsData > ofsData && pF->ofsData < ofsTop)
				ofsTop = pF->ofsData;
			
			if (pF->ofsData > ofsData && pF->ofsData + cbCurFile > ofsBottom)
				ofsBottom = pF->ofsData + cbCurFile;
		}
		MoveSize = ofsBottom - ofsTop;
		MoveShift = ofsTop - ofsData;
	
		fileseek(hCab, ofsData, SEEK_SET);
		
		// Shift data up if necessary
		if (MoveSize > 0) {
			FILEHANDLE hCabRead = OpenCabForAccess(MoveVol, "cab", O_RDONLY);
		
			fileseek(hCabRead, ofsTop, SEEK_SET);
			TransferData(hCabRead, hCab, MoveSize, CRYPT_NONE, 0);

			close(hCabRead);

			// Update file descriptors to reflect the change in file position
			for (i = 0; i < pCabDesc->cFiles; i++) {
				FILEDESC* pF = Files + i;
				
				// ignore files in other volumes
				if ((pF->DescStatus & DESC_INVALID) || pF->Volume != MoveVol)
					continue;
				
				if (pF->ofsData > ofsData)
					pF->ofsData -= MoveShift;
			
				if (curCabHdr.LastFile == i && (curCabHdr.LinkFlags & LINK_NEXT))
					curCabHdr.ofsLastData = pF->ofsData;	// last file in volume
			}
		}
	}

	chsize(hCab, tell(hCab));
	SaveCabHeader(hCab, &curCabHdr);
	close(hCab);
}

void DeleteFiles()
{
	LPCABFILELIST pCurrent;

	for (pCurrent = pFileList; pCurrent != 0; pCurrent = pCurrent->pNext)
	{
		FILEDESC* pFile = Files + pCurrent->CabIndex;

		RemoveFileBody(pCurrent->CabIndex);
		
		if (optPrintAll) {
			LPSTR pDir = Dirs->Dirs[pFile->DirIndex];
			if (*pDir != '\0')
			//	fprintf(stderr, "%s\\", pDir);
			//fprintf(stderr, "%s\n", pFile->lpName)
			;
		}
			
		pFile->DescStatus = DESC_INVALID;
		pFile->cbCompressed = 0;
		pFile->cbExpanded = 0;
		pFile->ofsData = 0;
		Free(pFile->lpName);
		pFile->lpName = 0;
	}
	
	SaveCabHeaders(TRUE);
}

void ZeroSizeFiles()
{
	LPCABFILELIST pCurrent;

	for (pCurrent = pFileList; pCurrent != 0; pCurrent = pCurrent->pNext)
	{
		FILEDESC* pFile = Files + pCurrent->CabIndex;

		RemoveFileBody(pCurrent->CabIndex);
		
		if (optPrintAll) {
			LPSTR pDir = Dirs->Dirs[pFile->DirIndex];
			if (*pDir != '\0')
//				fprintf(stderr, "%s\\", pDir);
		//	fprintf(stderr, "%s\n", pFile->lpName)
		;
		}
			
		pFile->DescStatus &= ~DESC_COMPRESSED;
		pFile->cbCompressed = 0;
		pFile->cbExpanded = 0;
	}
	
	SaveCabHeaders(TRUE);
}

void ListFiles()
{
	DWORD i;
	LPFILEDESC pFile;
/*	int files = 0;
	FILESIZE osize = 0, csize = 0;
	SYSTIME systime;
	LPSTR pDir;
	char attrs[5];
	int ia;
	LPCABFILELIST pCurrent = pFileList;

	if (optPrintAll) {
		fprintf(stderr, "\n");
		fprintf(stderr, "Date       Time   OrigSize  Attr  CompSize   Ind FileName\n");
		fprintf(stderr, "========== ===== ========== ==== ========== ==== =================\n");
	}

/*	if (optFGasDir) {
		for (i = 0; i < cFileGroups; i++) {
			LPFILEGROUPDESC pFG = FileGroups[i];
			fprintf(stdout, "01-01-2000 12:00 %10d ____ %10d %4d %s\\\n",
				(long)pFG->cbExpanded, (long)pFG->cbCompressed, -1, GetString(pCabDesc, pFG->ofsName));
		}
	}
*/
	/*while (pCurrent) {
		i = pCurrent->CabIndex;

		pFile = Files + i;

		DosTimeToSysTime(pFile->FatDate, pFile->FatTime, &systime);
		fprintf(stdout, "%02d-%02d-%04d %02d:%02d", systime.wMonth, systime.wDay, systime.wYear, systime.wHour, systime.wMinute);
	
		strcpy(attrs, FileAttrChars);
		for (ia=0; ia < 4; ia++)
			if (!(pFile->Attrs & FileAttrVals[ia]))
				attrs[ia] = '_';
		fprintf(stdout, " %10d %s %10d %4d ", (long)pFile->cbExpanded, attrs, (long)pFile->cbCompressed, i);
	
		pDir = Dirs->Dirs[pFile->DirIndex];

		if (optFGasDir)
			fprintf(stdout, "%s\\", GetGroupNameByFile(i));
		if (*pDir != '\0')
			fprintf(stdout, "%s\\", pDir);
		fprintf(stdout, "%s\n", pFile->lpName);

		files++;
		osize += pFile->cbExpanded;
		csize += pFile->cbCompressed;

		pCurrent = pCurrent->pNext;
	}

	if (optPrintAll) {
		fprintf(stderr, "                 ----------      ---------- -------------------\n");
		fprintf(stderr, "                 %10d      %10d %4d file(s) total\n", (long)osize, (long)csize, files);
	}*/
}

LPSTR GetGroupNameByFile(DWORD Index)
{
	LPFILEGROUPDESC pFG = GetGroupByFile(Index);
	return GetString(pCabDesc, pFG->ofsName);
}

//Morlac - List Setup components
void ListSetupTypes()
{
	DWORD i, j;
	LPSETUPTYPEDESC pST;
	
	if (optPrintAll)
//		fprintf(stderr, "\nSetup Types\n===============\n")
;

/*	for (i=0; i < cSetupTypes; i++) {
		pST = GetSetupTypeDesc(pCabDesc, SetupTypes, i);
		if (optPrintAll) fprintf(stderr, "Name            : ");
		fprintf(stdout, "%s\n", GetString(pCabDesc, pST->ofsName));
		if (optPrintAll) fprintf(stderr, "Display Name    : ");
		fprintf(stdout, "%s\n", GetString(pCabDesc, pST->ofsDispName));
		if (optPrintAll) fprintf(stderr, "Description     : ");
		fprintf(stdout, "%s\n", GetString(pCabDesc, pST->ofsDescription));
		if (optVerbose) {
			if (optPrintAll) fprintf(stderr, "# of Components : ");
			fprintf(stdout, "%d\n", pST->STHeader.cSTypes);
			if (optPrintAll) fprintf(stderr, "Components      :\n");
			for (j=0; j < pST->STHeader.cSTypes; j++) {
				fprintf(stdout, "                  %s\n",
				GetString(pCabDesc, ((DWORD*)((BYTE*)pCabDesc+pST->STHeader.ofsSTypeTab))[j]));
			}
		}
		if (optPrintAll) fprintf(stderr, "\n");
	}*/
}


/*
 * Morlac: Cahnges: Print all component data
 * Needed: Test with encypted and password potected components
 */
void ListComponents()
{
	DWORD i, fgi;
	LPCOMPONENTDESC pComp;

/*	if (optPrintAll)
		fprintf(stderr, "\nComponents\n===============\n");

	for (i=0; i < cComponents; i++) {
		pComp = Components[i];
		if (optPrintAll) fprintf(stderr, "Name          : ");
		fprintf(stdout, "%s\n", GetString(pCabDesc, pComp->ofsName));
		if (optPrintAll) fprintf(stderr, "Description   : ");
		fprintf(stdout, "%s\n", GetString(pCabDesc, pComp->ofsDescription));
		if (optPrintAll) fprintf(stderr, "Status Text   : ");
		fprintf(stdout, "%s\n", GetString(pCabDesc, pComp->ofsStatusText));
		if (optPrintAll) fprintf(stderr, "Display Name  : ");
		fprintf(stdout, "%s\n", GetString(pCabDesc, pComp->ofsDispName));
		if (optPrintAll) fprintf(stderr, "File Need     : ");
		fprintf(stdout, "%s\n", (pComp->Status.is_CRITICAL)?"Citical":
								(pComp->Status.is_RECOMMEND)?"Recommended":
								(pComp->Status.is_STANDARD)?"Standard":"UNKOWN");
		if (optPrintAll) fprintf(stderr, "Password      : ");
				fprintf(stdout, "%s\n", GetString(pCabDesc, pComp->ofsPassword));
		if(optVerbose) {
			if (optPrintAll) fprintf(stderr, "Visible       : ");
			fprintf(stdout, "%s\n", (pComp->Attrs.is_VISIBLE)?"Yes":"No");
			if (optPrintAll) fprintf(stderr, "Data as Files : ");
			fprintf(stdout, "%s\n", (pComp->Attrs.is_DATA_AS_FILES)?"Yes":"No");
			if (optPrintAll) fprintf(stderr, "Encryption    : ");
			fprintf(stdout, "%s\n", (pComp->Attrs.is_ENCRYPTED)?"Yes":"No");
			if (optPrintAll) fprintf(stderr, "CD-ROM Folder : ");
			fprintf(stdout, "%s\n", GetString(pCabDesc, pComp->ofsCDRomFolder));
			if (optPrintAll) fprintf(stderr, "FTP Location  : ");
			fprintf(stdout, "%s\n", GetString(pCabDesc, pComp->ofsFTPLocation));
			if (optPrintAll) fprintf(stderr, "HTTP Location : ");
			fprintf(stdout, "%s\n", GetString(pCabDesc, pComp->ofsHTTPLocation));
			if (optPrintAll) fprintf(stderr, "Miscellaneous : ");
			fprintf(stdout, "%s\n", GetString(pCabDesc, pComp->ofsMisc));
			if (optPrintAll) fprintf(stderr, "Depends on    : ");
			for	(fgi=0; fgi < pComp->cDepends; fgi++) {
				fprintf(stdout, "%s", GetString(pCabDesc, ((DWORD*)((BYTE*)pCabDesc + pComp->ofsDepends))[fgi]));
				if (fgi + 1 != pComp->cDepends)
					fprintf(stdout, ", ");
			}
			fprintf(stdout, "\n");
			if (optPrintAll) fprintf(stderr, "File Groups   : ");
			for (fgi=0; fgi < pComp->cFileGroups; fgi++) {
				fprintf(stdout, "%s", GetString(pCabDesc, ((DWORD*)((BYTE*)pCabDesc + pComp->ofsFileGroups))[fgi]));
				if (fgi + 1 != pComp->cFileGroups)
					fprintf(stdout, ", ");
			}
			fprintf(stdout, "\n");
			if (optPrintAll) fprintf(stderr, "Sub Comp      : ");
			for (fgi=0; fgi < pComp->cSubComps; fgi++) {
				fprintf(stdout, "%s", GetString(pCabDesc, ((DWORD*)((BYTE*)pCabDesc + pComp->ofsSubComps))[fgi]));
				if (fgi + 1 != pComp->cSubComps)
					fprintf(stdout, ", ");
			}
			fprintf(stdout, "\n");
			if (optPrintAll) fprintf(stderr, "GUID          : ");
			DisplayGUID(pComp->Guid);
			if (optPrintAll) fprintf(stderr, "OnInstalling  : ");
			fprintf(stdout, "%s\n", GetString(pCabDesc, pComp->ofsOnInstalling));
			if (optPrintAll) fprintf(stderr, "OnInstalled   : ");
			fprintf(stdout, "%s\n", GetString(pCabDesc, pComp->ofsOnInstalled));
			if (optPrintAll) fprintf(stderr, "OnUnInstalling: ");
			fprintf(stdout, "%s\n", GetString(pCabDesc, pComp->ofsOnUninstalling));
			if (optPrintAll) fprintf(stderr, "OnUnInstalled : ");
			fprintf(stdout, "%s\n", GetString(pCabDesc, pComp->ofsOnUninstalled));
			if (optPrintAll) fprintf(stderr, "Volatile      : ");
			fprintf(stdout, "%s\n", (pComp->Attrs.is_VOLATILE)?"Yes":"No");
			if (optPrintAll) fprintf(stderr, "Locked        : ");
			fprintf(stdout, "%s\n", (pComp->Attrs.is_LOCKED)?"Yes":"No");
		}
		fprintf(stdout, "\n");
	}*/
}

//Morlac - Display all known data
void ListFileGroups()
{
	DWORD i;
	LPFILEGROUPDESC pFG;
	DWORD Start, End;

	/*if (optPrintAll) {
		fprintf(stderr, "\n");
		fprintf(stderr, "FF - Index of the first file in the group (0-based)\n");
		fprintf(stderr, "LF - Index of the last file in the group\n");
		fprintf(stderr, "\n");
	}

	if (optFileGroup == -1)
		Start = 0, End = cFileGroups;
	else
		Start = optFileGroup, End = optFileGroup + 1;
	for (i = Start; i < End; i++) {
		pFG = FileGroups[i];
		if (optPrintAll) fprintf(stderr, "Name               : ");
		fprintf(stdout, "%s\n", GetString(pCabDesc, pFG->ofsName));
		if (optPrintAll) fprintf(stderr, "Destination        : ");
		fprintf(stdout, "%s\n", GetString(pCabDesc, pFG->ofsTargetDir));
		if (optPrintAll) fprintf(stderr, "Overwite           : ");
		if (pFG->OverwriteFlags.is_ALWAYS_OVERWRITE)
			fprintf(stdout,"ALWAYS_OVERWITE ");
		else if (pFG->OverwriteFlags.is_NEVER_OVERWRITE)
			fprintf(stdout,"NEVER_OVERWITE ");
		else if (pFG->OverwriteFlags.is_NEWER_VERSION)
			fprintf(stdout,"NEWER_VERSION ");
		if (pFG->OverwriteFlags.is_NEWER_DATE)
			fprintf(stdout,"/ NEWER_DATE");
		fprintf(stdout,"\n");
		if (optVerbose) {
			if (optPrintAll) fprintf(stderr, "FTP Location       : ");
			fprintf(stdout, "%s\n", GetString(pCabDesc, pFG->ofsFTPLocation));
			if (optPrintAll) fprintf(stderr, "HTTP Location      : ");
			fprintf(stdout, "%s\n", GetString(pCabDesc, pFG->ofsHTTPLocation));
			if (optPrintAll) fprintf(stderr, "Miscellaneous      : ");
			fprintf(stdout, "%s\n", GetString(pCabDesc, pFG->ofsMisc));
			if (optPrintAll) fprintf(stderr, "Shared             : ");
			fprintf(stdout, "%s\n", (pFG->Attrs.is_Shared)?"Yes":"No");
			if (optPrintAll) fprintf(stderr, "UnInstall          : ");
			fprintf(stdout, "%s\n", (pFG->Attrs.is_Uninstall)?"No":"Yes");//Uninstall bit is active low
			if (optPrintAll) fprintf(stderr, "Self-Registering   : ");
			fprintf(stdout, "%s\n", (pFG->Attrs.is_SelfRegistering)?"Yes":"No");
			if (optPrintAll) fprintf(stderr, "Potentially Locked : ");
			fprintf(stdout, "%s\n", (pFG->Attrs.is_PotentiallyLocked)?"Yes":"No");
			if (optPrintAll) fprintf(stderr, "Compressed         : ");
			fprintf(stdout, "%s\n", (pFG->Attrs.is_Compressed)?"Yes":"No");
			fprintf(stderr, "                     OrigSize   CompSize   FF   LF  Ind\n");
			fprintf(stderr, "                    ========== ========== ==== ==== ===\n");
			fprintf(stdout, "                    %10d %10d %4d %4d %3d\n",
			(long)pFG->cbExpanded, (long)pFG->cbCompressed,
			pFG->FirstFile, pFG->LastFile, i);
		}
		fprintf(stdout,"\n");
	}*/
}


void CantReadFile(int code)
{
	//fprintf(stderr, "\nCan not read from file, error code %d\n", code);
	//CleanExit(3);
}

void CantWriteFile(int code)
{
	//fprintf(stderr, "\nCan not write to file, error code %d\n", code);
//	CleanExit(12);
}

void CantOpenFile(LPSTR filename)
{
//	fprintf(stderr, "\nCould not open %s\n", filename);
//	CleanExit(2);
}

void NotIShield6()
{
	//fprintf(stderr, "\nThis does not look like IShield 6 cab =)\nAre u sure u know what u are doing ?\n");
//	CleanExit(4);
}

void InvalidCab()
{
//	fprintf(stderr, "\nInvalid or unsupported cab file\n");
//	CleanExit(9);
}

void NoMemory()
{
//	fprintf(stderr, "\nCan not grab enough memory, giving up =)\nGo get a better 'puter !");
//	CleanExit(6);
}

void CantDecompress(LPSTR filename)
{
//	fprintf(stderr, "SJiT! Can not decompress %s. ZData returned an error\n", filename);
}

void CantCompressError(LPSTR filename)
{
//	fprintf(stderr, "SJiT! Can not compress %s. ZData returned an error\n", filename);
//	CleanExit(13);
}

void CantCompressStore(LPSTR filename)
{
	//fprintf(stderr, "SJiT! Can not compress %s. Storing...\n", filename);
}

void NotInAnyGroups(LPSTR filename)
{
	//fprintf(stderr, "\n%s is not in any FileGroups\n", filename);
}

void InvalidFileIndex()
{
//	fprintf(stderr, "\nInvalid file index specified\n");
//	CleanExit(12);
}

void BadFileGroup()
{
//	fprintf(stderr, "Invalid FileGroup specified\n");
	CleanExit(22);
}

void NotEnoughParams(LPSTR str)
{
///	fprintf(stderr, "%sNot enough parameters\n", str);
	CleanExit(24);
}

void NoFilesMatched()
{
//	fprintf(stderr, "\nNo files matched the criteria, duh !\n");
	CleanExit(0);
}

void ExceptInDecompress()
{
	//fprintf(stderr, "\nException in decompression routine\nPossibly new compression algos\n");
	CleanExit(26);
}

void GeneralException()
{
//	fprintf(stderr, "\nAn exception occured\nThis might be a new version of InstallShield unsupported at this moment\n");
	CleanExit(27);
}

void TooManyEntries(LPSTR str)
{
//	fprintf(stderr, "\nToo many %s, recompile with a larger buffer\n", str);
	CleanExit(28);
}

void NoMatchBytesOut()
{
	//fprintf(stderr, " WARNING: file size does not match File Descriptor in cab");
}

FILEHANDLE OpenForRead(LPSTR filename)
{
	FILEHANDLE hnd;

	hnd = open(filename, O_RDONLY | O_BINARY | O_RANDOM, S_IREAD | S_IWRITE);
	
	if (hnd == -1)
		CantOpenFile(filename);

	return hnd;
}

FILEHANDLE OpenForWrite(LPSTR filename, int Flags)
{
	FILEHANDLE hnd;

	if (!(Flags & (O_RDWR | O_APPEND)))
		Flags |= O_WRONLY;
	
	hnd = open(filename, Flags | O_BINARY | O_RANDOM, S_IREAD | S_IWRITE);

	if (hnd == -1)
		CantOpenFile(filename);

	return hnd;
}

FILEHANDLE OpenForAccess(LPSTR filename, int Access)
{
	FILEHANDLE hnd;

	hnd = open(filename, Access | O_BINARY | O_RANDOM, S_IREAD | S_IWRITE);
	
	if (hnd == -1)
		CantOpenFile(filename);

	return hnd;
}

FILEHANDLE OpenCabForAccess(int VolN, LPSTR ext, int Access)
{
	char namebuf[256];
	
	sprintf(namebuf, pCabPattern, VolN, ext);
	return OpenForAccess(namebuf, Access);
}

void ReadCabHeader(FILEHANDLE hCab, LPCABHEADER pHdr)
{
	fileseek(hCab, 0, SEEK_SET);
	if (read(hCab, pHdr, sizeof(CABHEADER)) != sizeof(CABHEADER))
		CantReadFile(errno);
	if (CabHdr.Signature != CAB_SIG)
		NotIShield6();
	if (!(CabHdr.Version & 0x01000000))
		NotIShield6();
	//if (!IsFirstVolume()) {
	//	fprintf(stderr, "Can only operate on the first cab volume\n");
	//	CleanExit(5);
	//}
}

FILEHANDLE OpenCabReadHeader(int VolN, LPSTR ext, int Access, LPCABHEADER pHdr)
{
	FILEHANDLE hFile;

	hFile = OpenCabForAccess(VolN, ext, Access);
	ReadCabHeader(hFile, pHdr);

	return hFile;
}

int CompareFileGroups(const void * pFG1, const void * pFG2)
{
	return (*(const FILEGROUPDESC**)pFG1)->FirstFile - (*(const FILEGROUPDESC**)pFG2)->FirstFile;
}

void InitCabFile(LPSTR cabfile, int Access)
{
	// generate Cab filename pattern
	{
		LPSTR pdot;
		int pos;

		pCabPattern =(char *)Alloc(strlen(cabfile) + 2);
		strcpy(pCabPattern, cabfile);
		
		pdot = strrchr(cabfile, '.');
		pos = pdot ? pdot - cabfile : strlen(cabfile);
		// nuke all digits
		for (pos--; isdigit(pCabPattern[pos]); pos--)
			pCabPattern[pos] = '\0';

		//FIX: Crashed if filename had no digits in it.
		pCabPattern[pos+1] = '\0';
		strcat(pCabPattern, "%d.%s");
	}

	hCabHdr = OpenCabReadHeader(1, "hdr", Access, &CabHdr);

	pCabDesc = (struct tagCABDESC *)Alloc(CabHdr.cbCabDesc);

	// Read Cab Descriptor and all descriptor data
	fileseek(hCabHdr, CabHdr.ofsCabDesc, SEEK_SET);
	if (read(hCabHdr, pCabDesc, CabHdr.cbCabDesc) != (int)CabHdr.cbCabDesc)
		CantReadFile(errno);
	
	{// Calculate Setup Types table location
		LPSETUPTYPEHEADER pSTH;
	
		pSTH = (LPSETUPTYPEHEADER) ((BYTE*)pCabDesc + pCabDesc->ofsSTypes);
		cSetupTypes = pSTH->cSTypes;
		SetupTypes = (SETUPTYPETABLE) ((BYTE*)pCabDesc + pSTH->ofsSTypeTab);
	}

	{// Calculate Components & File Groups tables location
		DWORD ofsEntry;
		long i;

		// BUGFIX001 File Groups and Components are inited from their lists

		// init File Groups array
		for (i = 0; i < CFG_MAX; i++) {
			ofsEntry = pCabDesc->ofsFileGroup[i];
			while (ofsEntry != 0) {
				FILEGROUPENTRY* pFGE;
				
				pFGE = GetFileGroupEntry(pCabDesc, ofsEntry);
				FileGroups[cFileGroups++] = GetFileGroup(pCabDesc, pFGE->ofsDesc);
				
				ofsEntry = pFGE->ofsNext;
				
				if (cFileGroups >= CSFG_MAX)
					TooManyEntries("File Groups");
			}
		}
		// sort File Groups by file indices
		qsort(FileGroups, cFileGroups, sizeof(FileGroups[0]), CompareFileGroups);

		// init Components array
		for (i = 0; i < CFG_MAX; i++) {
			ofsEntry = pCabDesc->ofsComponent[i];
			while (ofsEntry != 0) {
				COMPONENTENTRY* pCE;
				
				pCE = GetCompEntry(pCabDesc, ofsEntry);
				Components[cComponents++] = GetComponent(pCabDesc, pCE->ofsDesc);
				
				ofsEntry = pCE->ofsNext;

				if (cComponents >= CSCP_MAX)
					TooManyEntries("Components");
			}
		}

	}

	// Read Dirs & Files table
	{
		DWORD* DFT;
		DWORD i;

		DFT =(unsigned long *) Alloc(pCabDesc->cbDFT);
		
		fileseek(hCabHdr, CabHdr.ofsCabDesc + pCabDesc->ofsDFT, SEEK_SET);
		if (read(hCabHdr, DFT, pCabDesc->cbDFT) != (int)pCabDesc->cbDFT)
			CantReadFile(errno);

		Dirs = DirsArrayBuild(DFT);

		// alloc and copy Files
		Files = (FILEDESC*) Alloc(pCabDesc->cFiles * sizeof(FILEDESC));
		memcpy(Files, GetFileDesc(pCabDesc, DFT, 0), pCabDesc->cFiles * sizeof(FILEDESC));
		
		// copy filename strings
		for (i = 0; i < pCabDesc->cFiles; i++) {
			if (!(Files[i].DescStatus & DESC_INVALID))
				Files[i].lpName = strdup(GetString(DFT, Files[i].ofsName));
		}

		Free(DFT);
	}
}

void SaveCabHeaders(int bCloseHandle)
{
	DWORD* DFT = RebuildDFT();

	fileseek(hCabHdr, 0, SEEK_SET);
	if (write(hCabHdr, &CabHdr, sizeof(CabHdr)) != sizeof(CabHdr))
		CantWriteFile(errno);
	
	fileseek(hCabHdr, CabHdr.ofsCabDesc, SEEK_SET);
	if (write(hCabHdr, pCabDesc, CabHdr.cbCabDesc) != (int)CabHdr.cbCabDesc)
		CantWriteFile(errno);

	fileseek(hCabHdr, CabHdr.ofsCabDesc + pCabDesc->ofsDFT, SEEK_SET);
	if (write(hCabHdr, DFT, pCabDesc->cbDFT) != (int)pCabDesc->cbDFT)
		CantWriteFile(errno);

	Free(DFT);

	chsize(hCabHdr, (long)filetell(hCabHdr));
	
	if (bCloseHandle) {
		close(hCabHdr);
		hCabHdr = 0;
	}
}

void SaveCabHeader(FILEHANDLE hCabFile, CABHEADER* pHdr)
{
	fileseek(hCabFile, 0, SEEK_SET);
	if (write(hCabFile, pHdr, sizeof(CABHEADER)) != sizeof(CABHEADER))
		CantWriteFile(errno);
}

void TranslateChars(char* str, char* sFrom, char* sTo)
{
	for (; *str != 0; str++) {
		char* set;
		
		for (set = sFrom; *set != 0 && *str != *set; set++)
			;
		if (*str == *set)
			*str = sTo[set - sFrom];
	}
}

BOOL TranslatePathToGroup(LPSTR* ppPath, LPSTR* ppFGName)
{
	LPSTR pSlash;
	DWORD i;
	char GroupName[MAX_PATH];

	*ppFGName = NULL;

	if (*ppPath == NULL || !optFGasDir)
		return FALSE;

	if (**ppPath == '\\')
		*ppPath++;

	// attemp to parse out FG Name
	pSlash = strchr(*ppPath, '\\');
	// slash found: either a dir or FG name
	if (pSlash)
		*pSlash = 0;

	// map to internal naming
	TranslateChars(strcpy(GroupName, *ppPath), "[]", "<>");
	i = GetGroupIndexByName(GroupName);

	if (i != -1) {		// it's an FG name
		optFileGroup = i;
		*ppFGName = *ppPath;
		// map to filesys naming
		TranslateChars(*ppFGName, "<>", "[]");

		if (pSlash) {	// there is more of path/mask in a param
			*ppPath = pSlash + 1;
		} else			// it was FG only
			*ppPath = NULL;
		
		return TRUE;
	}
	
	// it wasn't an FG name
	if (pSlash)	// restore the path
		*pSlash = '\\';
	
	return FALSE;
}

// both params may be NULL
void CabBasedFileList(LPSTR CabParam, LPSTR DiskParam)
{
	long CabInd;
	BOOL CabByName = FALSE;
	BOOL CabIsMask = FALSE;
	BOOL DiskIsDir = FALSE;
	char dparam[MAX_PATH];
	char buf[MAX_PATH];
	LPFILEDESC pFile;
	LPSTR pDir;
	LPSTR pName;
	LPCABFILELIST pCurrent;
	DWORD NewSize;
	DWORD i, Start, End;
	LPSTR pFGName = NULL;

	// check if file-index 
	CabInd = strtolong(CabParam);
	
	if (CabInd == -1) {	// not an index
		TranslatePathToGroup(&CabParam, &pFGName);
		CabByName = TRUE;
		CabIsMask = IsMask(CabParam);

	} else if (CabInd < 0 || CabInd > (long)pCabDesc->cFiles - 1)
		InvalidFileIndex();
	
	if (CabIsMask || (!DiskParam) || DiskParam[strlen(DiskParam) - 1] == '\\')
		DiskIsDir = TRUE;
	
	if (DiskParam) {
		strcpy(dparam, DiskParam);
		if (DiskIsDir && DiskParam[strlen(DiskParam) - 1] != '\\')
			strcat(dparam, "\\");
	} else
		dparam[0] = '\0';

	if (!CabByName) {
		if (optFileGroup != -1)
		//	fprintf(stderr, "WARNING: FileGroup specified when referencing file by index, option ignored\n")
		;

		pFile = Files + CabInd;
		if (pFile->DescStatus & DESC_INVALID)
			InvalidFileIndex();
		
		buf[0] = '\0';
		if (DiskIsDir) {
			strcat(buf, dparam);
			pDir = Dirs->Dirs[pFile->DirIndex];
			
			if (optRecurseSubdirs) {
				//Morlac
				if (optFGasDir)
					pFGName = GetGroupNameByFile(CabInd);
				if (pFGName != NULL) {
					strcat(strcat(buf, pFGName), "\\");
				}//Morlac: End
				if (*pDir != '\0')
					strcat(strcat(buf, pDir), "\\");
			}
			strcat(buf, pFile->lpName);
		} else
			strcat(buf, dparam);
		
		pFileList = (struct tagCABFILELIST *)Alloc(sizeof(CABFILELIST) + strlen(buf) + 1);
		pFileList->CabIndex = CabInd;
		pFileList->pNext = NULL;
		strcpy(pFileList->FileName, buf);
		FileCount = 1;

		return;
	}

	if (optFileGroup == -1) {
		Start = 0;
		End = pCabDesc->cFiles;
	} else {
		LPFILEGROUPDESC pFG = FileGroups[optFileGroup];
		Start = pFG->FirstFile;
		End = pFG->LastFile + 1;
	}
	for (i = Start; i < End; i++) {
		pFile = Files + i;
		if ((pFile->DescStatus & DESC_INVALID) || pFile->ofsData == 0)
			continue;
		
		buf[0] = '\0';
		pDir = Dirs->Dirs[pFile->DirIndex];
		pName = pFile->lpName;
		
		if (optMatchWithDirs && *pDir != '\0')
			strcat(strcat(buf, pDir), "\\");

		strcat(buf, pName);

		if (!WildMatch(buf, CabParam))
			continue;

		buf[0] = '\0';
		if (DiskIsDir) {
			strcat(buf, dparam);
			if (optRecurseSubdirs) {
				//Morlac
				if (optFGasDir)
					pFGName = GetGroupNameByFile(i);
				if (pFGName != NULL)
					strcat(strcat(buf, pFGName), "\\");
				//Morlac:End
				if (*pDir != '\0')
					strcat(strcat(buf, pDir), "\\");
			} else {
				//Morlac
				if (optFGasDir)
					pFGName = GetGroupNameByFile(i);
				if (pFGName != NULL)
					strcat(strcat(buf, pFGName), "\\");
				//Morlac:End
			}
			strcat(buf, pName);
		} else
			strcat(buf, dparam);
		
		NewSize = sizeof(CABFILELIST) + strlen(buf) + 1;
		if (pFileList) {
			pCurrent->pNext =(struct tagCABFILELIST *) Alloc(NewSize);
			pCurrent = pCurrent->pNext;
		} else {
			pFileList =(struct tagCABFILELIST *) Alloc(NewSize);
			pCurrent = pFileList;
		}
		pCurrent->CabIndex = i;
		pCurrent->pNext = NULL;
		strcpy(pCurrent->FileName, buf);
		FileCount++;
	}
	if (!FileCount)
		NoFilesMatched();
}

// DiskParam may not be NULL
void DiskBasedFileList(LPSTR CabParam, LPSTR DiskParam)
{
	char cpath[MAX_PATH] = "";
	char begdp[MAX_PATH] = "";
	char curdp[MAX_PATH] = "";
	char mask[MAX_PATH];
	char tmpdp[MAX_PATH] = "";
	char nulldp[] = "";
	long tmp;
	LPSTR pFGName = NULL;


	if (CabParam) {	// we were passed a Cab Path
		// parse out the path
		TranslatePathToGroup(&CabParam, &pFGName);

		if (CabParam) {
			strcpy(cpath, CabParam);
			tmp = strlen(cpath);
			if (cpath[tmp - 1] == '\\')
				cpath[tmp - 1] = '\0';
		}
	}

	if (pFGName == NULL) {	// there was no Group in CabParam
		if (TranslatePathToGroup(&DiskParam, &pFGName)) {
			// path resolved to FileGroup name
			strcat(strcpy(begdp, pFGName), "\\");
			if (DiskParam == NULL)
				DiskParam = nulldp;
		}
	}

	tmp = strlen(DiskParam);
	if (IsMask(DiskParam)) {
		// we were passed a mask parse out starting dir and file mask
		LPSTR pSlash = strrchr(DiskParam, '\\');
		if (pSlash) {
			tmp = pSlash - DiskParam + 1;
			strncpy(tmpdp, DiskParam, tmp);
			tmpdp[tmp] = '\0';
			pSlash++;
		
		} else {
			pSlash = DiskParam;
		}
		
		strcpy(mask, pSlash);

	} else if (tmp == 0 || DiskParam[tmp - 1] == '\\' || DirExists(DiskParam)) {
		// we were passed a dir - parse it
		// or it was a Group name
		LPSTR pSlash;
		
		strcpy(tmpdp, DiskParam);
		if (DiskParam[tmp - 1] == '\\')
			tmpdp[tmp - 1] = '\0';
		
		pSlash = strrchr(tmpdp, '\\');
		if (!pSlash)
			pSlash = tmpdp - 1;
		pSlash++;
		
		strcpy(curdp, pSlash);
		
		*pSlash = '\0';
		strcpy(mask, "*");
	
	} else { // we were passed a file
		LPSTR pSlash;
		
		strcpy(tmpdp, DiskParam);
		
		pSlash = strrchr(tmpdp, '\\');
		if (!pSlash)
			pSlash = tmpdp - 1;
		pSlash++;
		
		strcpy(mask, pSlash);
		*pSlash = '\0';
	}
	
	if (*tmpdp != 0)
		strcat(strcat(begdp, tmpdp), "\\");

	RecurseBuildDiskList(begdp, curdp, mask, cpath);
	if (!FileCount)
		NoFilesMatched();
}

void RecurseBuildDiskList(LPSTR BegDP, LPSTR CurDP, LPSTR Mask, LPSTR CabPath)
{
	int hFind;
	struct _finddata_t fd;
	char buf[MAX_PATH];
	LPSTR pSubDir;
	static LPDISKFILELIST pCurrent;

	strcpy(buf, BegDP);
	strcat(buf, CurDP);
	if (*CurDP != '\0')
		strcat(buf, "\\");
	strcat(buf, "*");

	hFind = _findfirst(buf, &fd);
	if (hFind == -1)
		return;
	
	// setup CurDirPath for subsequent recursive calls
	strcpy(buf, CurDP);
	pSubDir = strchr(buf, '\0');
	if (pSubDir != buf)
		*(pSubDir++) = '\\';

	do {
		if (fd.attrib & FILE_ATTR_DIRECTORY) {
			if (optRecurseSubdirs &&
					strcmp(fd.name, ".") != 0 &&
					strcmp(fd.name, "..") != 0) {
				strcpy(pSubDir, fd.name);
				RecurseBuildDiskList(BegDP, buf, Mask, CabPath);
			}
		} else if (WildMatch(fd.name, Mask) && 
					!(CurDP[0] == '\0' && _stricmp(pCabPattern, fd.name) == 0) ) { // matched the wildcard
			DWORD Size;
			DWORD ofsDisk, ofsCab;
			Size = sizeof(DISKFILELIST) + strlen(fd.name) + 1; // size of struct + size of FileName
			ofsDisk = Size;
			Size += strlen(BegDP) + strlen(CurDP) + 2; // size of disk path
			ofsCab = Size;
			Size += strlen(CabPath) + 2;
			if (optRecurseSubdirs)
				Size += strlen(CurDP);
			
			if (pDiskList) {
				pCurrent->pNext =(struct tagDISKFILELIST *) Alloc(Size);
				pCurrent = pCurrent->pNext;
			} else {
				pDiskList =(struct tagDISKFILELIST *) Alloc(Size);
				pCurrent = pDiskList;
			}
			strcpy(pCurrent->FileName, fd.name);
			
			pCurrent->DiskDir = (LPSTR)pCurrent + ofsDisk;
			strcpy(pCurrent->DiskDir, BegDP);
			strcat(pCurrent->DiskDir, CurDP);
			if (CurDP[0] != '\0')
				strcat(pCurrent->DiskDir, "\\");
			
			pCurrent->CabDir = (LPSTR)pCurrent + ofsCab;
			strcpy(pCurrent->CabDir, CabPath);
			if (optRecurseSubdirs && CurDP[0] != '\0') {
				if (CabPath[0] != '\0')
					strcat(pCurrent->CabDir, "\\");
				strcat(pCurrent->CabDir, CurDP);
			}
			
			pCurrent->pNext = NULL;
			FileCount++;
			//fprintf(stderr, "%s, %s, %s\n", pCurrent->FileName, pCurrent->DiskDir, pCurrent->CabDir);
		}
	} while (_findnext(hFind, &fd) == 0);

}

BOOL DirExists(LPSTR DirName)
{
	struct _finddata_t fd;
	long fh;

	TranslateChars(DirName, "<>", "[]");
	fh = _findfirst(DirName, &fd);
	if (fh == -1)
		return FALSE;
	_findclose(fh);
	
	if (fd.attrib & FILE_ATTR_DIRECTORY)
		return TRUE;
	else
		return FALSE;
}

void TranslateFileGroup()
{
	if (optFileGroup == -1)
		return;
	if (optFileGroup > 0x1000)
		optFileGroup = GetGroupIndexByName((LPSTR)optFileGroup);
	if (optFileGroup < 0 || optFileGroup > cFileGroups - 1) {
//		fprintf(stderr, "Invalid FileGroup specified\n")
		;
		CleanExit(22);
	}
}

DWORD GetGroupIndexByName(LPSTR GroupName)
{
	LPFILEGROUPDESC pFG;
	DWORD i;

	for (i = 0; i<cFileGroups &&
		(stricmp(GroupName,GetString(pCabDesc,(pFG=FileGroups[i])->ofsName)) != 0);
		 i++)
			 ;
	return i < cFileGroups ? i : -1;
}

BOOL IsMask(LPSTR str)
{
	return (!str) || strchr(str, '*') || strchr(str, '?');
}

BOOL WildMatch(LPSTR str, LPSTR wildcard)
{
	LPSTR tstr;

	if (!wildcard)
		return TRUE;
	if (!str)
		return FALSE;

	while (*wildcard != '\0' && (toupper(*str) == toupper(*wildcard) || *wildcard == '*' || *wildcard == '?')) {
		if (*wildcard == '?') {
			wildcard++;
			if (*str == '\0')
				return FALSE;
			str++;
		} else if (*wildcard == '*') {
			wildcard++;
			if (*wildcard == '\0')
				return TRUE;
			tstr = str;
			while (*tstr != '\0' && !WildMatch(tstr, wildcard))
				tstr++;
			if (*tstr != '\0')
				str = tstr;
		} else {
			*wildcard++;
			*str++;
		}
	}
	return toupper(*str) == toupper(*wildcard);
}

// accepts part of the string
// returns -1 if no conversion possible
long strtolong(char* str)
{
	char* pend;
	long retval;

	if (!str)
		return -1;

	retval = strtol(str, &pend, 10);
	if (!(*pend == '\0' || *pend == ',' || *pend == '-'))
		retval = -1;

	return retval;
}

// must convert the entire the string
// returns -1 if no conversion possible
long atolong(char* str)
{
	char* pend;
	long retval;

	if (!str)
		return -1;

	retval = strtol(str, &pend, 10);
	if (*pend != '\0')
		retval = -1;

	return retval;
}

void Free(void* ptr)
{
	if (ptr != NULL)
		free(ptr);
}

void* Alloc(DWORD NewSize)
{
	void* tempPtr = malloc(NewSize);
		if (tempPtr==0)
	{
		NoMemory();
		//return NULL;
	}
	return tempPtr;
}

void* ReAlloc(void* Ptr, DWORD NewSize)
{
	void* tempPtr = realloc(Ptr, NewSize);
		if (!tempPtr)
	{
		NoMemory();
		//return NULL;
	}
	
	return tempPtr;
}
