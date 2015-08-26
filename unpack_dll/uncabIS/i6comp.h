///////////////////////////////////////////////////////////////////
// i6comp.h
//  
// InstallShield Compression and Maintenance util
// fOSSiL - 1999
//
// *Any use is authorized, granted the proper credit is given*
//
// No support will be provided for this code
//

#ifndef _I6COMP_H
#define _I6COMP_H
#include "is6cab.h"
#include "rsa/md5.h"
#include  "zdatas.h"
typedef int FILEHANDLE;

#ifndef BOOL
	typedef int BOOL;
	#define TRUE	1
	#define FALSE	0
#endif

#ifndef MAX_PATH
	#define MAX_PATH	260
#endif

#if defined(_MSC_VER)
	#define	fileseek(h,p,o)		_lseeki64(h,p,o)
	#define	filetell(h)			_telli64(h)
	#define	filelen(h)			_filelengthi64(h)
#else
	#define fileseek(h,p,o)		lseek(h,p,o)
	#define	filetell(h)			tell(h)
	#define	filelen(h)			filelength(h)
#endif

typedef struct tagSYSTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
} SYSTIME, *LPSYSTIME;

typedef struct tagRWHANDLES {
	FILEHANDLE hRead;
	FILEHANDLE hWrite;
	FILESIZE BytesIn;
	FILESIZE BytesOut;
	MD5_CTX* DigestCtx;
} RWHANDLES, *LPRWHANDLES;

typedef struct tagEXTRACTINFO {
	FILEHANDLE hRead;
	FILEHANDLE hWrite;
	FILESIZE BytesIn;
	FILESIZE BytesOut;
	DWORD FileIndex;
	DWORD Volume;
	FILESIZE ofsData;
	FILESIZE BytesHere;
} EXTRACTINFO, *LPEXTRACTINFO;

typedef struct tagCABFILELIST {
	struct tagCABFILELIST* pNext;
	DWORD CabIndex;
	char FileName[];
} CABFILELIST, *LPCABFILELIST;

typedef struct tagDISKFILELIST {
	struct tagDISKFILELIST* pNext;
	DWORD CabDirInd;
	char* DiskDir;
	char* CabDir;
	char FileName[];
} DISKFILELIST, *LPDISKFILELIST;

typedef struct tagDIRARRAY {
	DWORD Count;
	char* Dirs[];
} DIRARRAY, *LPDIRARRAY;

typedef enum {
	ver50 = 0,
	ver51,
	ver55,
	ver60
} ISVersion;

typedef enum {
	zdCompress = 0,
	zdDecompress
} ZDProcess;

#define GetFileGroup(ptr, ofs) ( (LPFILEGROUPDESC) (((BYTE*)ptr) + ofs) )
#define GetComponent(ptr, ofs) ( (LPCOMPONENTDESC) (((BYTE*)ptr) + ofs) )


#define CSFG_MAX	512
#define CSCP_MAX	512

// IShield functions
void ConvertToSingle();
void ExtractFiles();
void ReplaceFiles();
void DeleteFiles();
void ZeroSizeFiles();
void AddFiles();
void ListFiles();
void ListSetupTypes();
void ListComponents();
void ListFileGroups();
void ReadCabHeader(FILEHANDLE hCab, LPCABHEADER pHdr);
void InitCabFile(LPSTR cabfile, int Access);
void SaveCabHeaders(int bCloseHandle);
void SaveCabHeader(FILEHANDLE hCabFile, CABHEADER* pHdr);
FILEHANDLE OpenCabReadHeader(int VolN, LPSTR ext, int Access, LPCABHEADER pHdr);

// ZData functions
int ZDCALLBACK ReadData(void* pbuf, DWORD* size, RWHANDLES* pHnd);
int ZDCALLBACK WriteData(void* pbuf, DWORD* size, RWHANDLES* pHnd);
int ZDCALLBACK ExtractRead(void* pbuf, DWORD* size, EXTRACTINFO* pHnd);


// Utils
void StartMsg();
void Usage();
void CleanExit(int code);
void DispatchCommand(int argc, char* argv[]);
void DeleteCab(LPSTR cabname);
void TransferData(FILEHANDLE hFrom, FILEHANDLE hTo, FILESIZE Length, DWORD Crypt, MD5_CTX* DigestCtx);
#define CRYPT_NONE		0
#define CRYPT_DECRYPT	1
#define CRYPT_ENCRYPT	2

void InitZData(void* pHnds, ZDProcess proc);
void CreateDir(LPSTR dirname);
FILEHANDLE OpenForRead(LPSTR filename);
FILEHANDLE OpenForWrite(LPSTR filename, int Flags);
FILEHANDLE OpenForAccess(LPSTR filename, int Access);
FILEHANDLE OpenCabForAccess(int VolN, LPSTR ext, int Access);
BOOL TranslatePathToGroup(LPSTR* ppPath, LPSTR* ppFGName);
void CabBasedFileList(LPSTR CabParam, LPSTR DiskParam);
void DiskBasedFileList(LPSTR CabParam, LPSTR DiskParam);
BOOL WildMatch(LPSTR str, LPSTR wildcard);
long strtolong(char* str);
long atolong(char* str);
BOOL IsMask(LPSTR str);
LPFILEGROUPDESC GetGroupByFile(DWORD index);
LPSTR GetGroupNameByFile(DWORD Index);
DWORD GetGroupIndexByName(LPSTR GroupName);
void TranslateFileGroup();
BOOL DirExists(LPSTR DirName);
void RecurseBuildDiskList(LPSTR BegDP, LPSTR CurDP, LPSTR Mask, LPSTR CabPath);
void Free(void* ptr);
void* Alloc(DWORD NewSize);
void* ReAlloc(void* Ptr, DWORD NewSize);
LPDIRARRAY DirsArrayBuild(DFTABLE DFT);
void DirsArrayDestruct(LPDIRARRAY pDA);
long DirsArrayFind(LPDIRARRAY pDA, LPSTR DirName);
long DirsArrayAddDir(LPDIRARRAY* ppDA, LPSTR DirName);
DFTABLE RebuildDFT();
void RemoveFileBody(DWORD Index);
void AddFileBody(DWORD Index, char* FileName, RWHANDLES* pHnd);
DWORD GetLastRefVolume();
int CompareLongs(long Val1, long Val2, WORD Relation);
void ReNumberVolumes(DWORD FromVol, DWORD Index, WORD IndexRelation, long IndexDelta);
DWORD GetNewFileInGroup(DWORD iFileGroup);

// Win32 routine replacements
void SetFileAttrs(char* filename, DWORD Attrs);
void FileTimeToDosTime(time_t t, WORD* DosDate, WORD* DosTime);
time_t DosTimeToFileTime(WORD DosDate, WORD DosTime);
void GetFileDosTime(char* filename, WORD* DosDate, WORD* DosTime);
void SetFileDosTime(char* filename, WORD DosDate, WORD DosTime);
void SysTimeToDosTime(SYSTIME* systime, WORD* DosDate, WORD* DosTime);
void DosTimeToSysTime(WORD DosDate, WORD DosTime, SYSTIME* systime);
void DisplayGUID(GUID *pGuid);
DWORD GetFileAttrs(char* filename);

// Errors
void CantReadFile(int code);
void CantWriteFile(int code);
void CantOpenFile(LPSTR filename);
void InvalidCab();
void NotIShield6();
void NoMemory();
void InvalidFileIndex();
void CantDecompress(LPSTR filename);
void CantCompressError(LPSTR filename);
void CantCompressStore(LPSTR filename);
void NotInAnyGroups(LPSTR filename);
void NotEnoughParams(LPSTR str);
void NoFilesMatched();
void ExceptInDecompress();
void NoMatchBytesOut();
void GeneralException();
void TooManyEntries(LPSTR str);
void BadFileGroup();

// Global Cab variables
FILEHANDLE hCabHdr ;
CABHEADER CabHdr;
LPCABDESC pCabDesc ;
SETUPTYPETABLE SetupTypes ;
DWORD cSetupTypes ;
COMPONENTDESC* Components[CSCP_MAX];
DWORD cComponents ;
FILEGROUPDESC* FileGroups[CSFG_MAX];
DWORD cFileGroups ;
LPSTR pCabPattern;
DIRARRAY* Dirs;
FILEDESC* Files;

// Globals
ISVersion ver ;
LPSTR exename ;
void* pZBuf;
LPCABFILELIST pFileList;
LPDISKFILELIST pDiskList ;
DWORD FileCount;
//DWORD FileAttrVals[4] = {FILE_ATTR_ARCHIVE, FILE_ATTR_READONLY, FILE_ATTR_HIDDEN, FILE_ATTR_SYSTEM};
//char FileAttrChars[]= "ARHS";

// Options
BOOL optRecurseSubdirs ;
BOOL optMatchWithDirs ;
BOOL optPrintAll ;
BOOL optFGasDir ;
DWORD optVersion ;
DWORD optFileGroup ;//= (DWORD)-1;
// Morlac
BOOL optVerbose ;

void ExtractData(EXTRACTINFO* pHnd, DWORD Crypt);





#endif // _I6COMP_H
