///////////////////////////////////////////////////////////////////
// is6cab.h
//  
// InstallShield cabinet definitions
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

#ifndef ISHIELD6_INC
#define ISHIELD6_INC

#pragma pack( push, 1 )
#include "../stdafx.h"
/*#ifndef DWORD
	typedef unsigned int DWORD;
#endif
#ifndef WORD
	typedef unsigned short WORD;
#endif*/
#ifndef BYTE
	typedef unsigned char BYTE;
#endif
#ifndef LPSTR
	typedef char* LPSTR;
#endif
/*#ifndef GUID
	typedef struct _GUID {
		unsigned long Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char Data4[8];
	} GUID;
#endif /* GUID */

#if defined(__GNUC__) || defined(__MWERKS__)
	// gcc
	typedef long long FILESIZE;

#elif defined(_MSC_VER) || defined(__BCPLUSPLUS__)
	// MS and Borland
	typedef __int64 FILESIZE;

#else
	// for other compilers
	typedef struct tagFILESIZE {
		DWORD dwLow;
		DWORD dwHigh;
	} FILESIZE;
#endif

typedef struct tagCABHEADER {
	DWORD Signature;			// 0
	DWORD Version;				// 4
	DWORD LinkFlags;			// 8
	DWORD ofsCabDesc;			// c
	DWORD cbCabDesc;			// 10
	FILESIZE ofsCompData;		// 14
	DWORD FirstFile;			// 1c		
	DWORD LastFile;				// 20		
	FILESIZE ofsFirstData;		// 24
	FILESIZE cbFirstExpanded;	// 2c
	FILESIZE cbFirstData;		// 34
	FILESIZE ofsLastData;		// 3c
	FILESIZE cbLastExpanded;	// 44
	FILESIZE cbLastData;		// 4c
	// Morlac
	// Array containing password at offs:54 password size limit unknown yet
} CABHEADER, *LPCABHEADER;	// size=54

#define CAB_SIG		0x28635349
#define LINK_NONE	0
#define LINK_PREV	1
#define LINK_NEXT	2
#define LINK_BOTH	3

#define CFG_MAX		0x47

typedef struct tagCABDESC {
	DWORD ofsStrings;				// 0
	DWORD junk11;					// 4
	DWORD ofsCompList;				// 8
	DWORD ofsDFT;					// c
	DWORD junk21;					// 10
	DWORD cbDFT;					// 14
	DWORD cbDFT2;					// 18
	WORD cDirs;						// 1c
	DWORD phpDirs;					// 1e
	WORD Pad01;						// 22
	DWORD cbDirs;					// 24
	DWORD cFiles;					// 28
	DWORD ofsFilesDFT;				// 2c
	WORD cCompTabInfo;				// 30
	DWORD ofsCompTab;				// 32
	DWORD junk51, junk52;			// 36
	DWORD ofsFileGroup[CFG_MAX];	// 3e
	DWORD ofsComponent[CFG_MAX];	// 15a
	DWORD ofsSTypes;				// 276
	DWORD ofsSTab;					// 27a
} CABDESC, *LPCABDESC;				// size=4e

typedef DWORD* DFTABLE;

typedef struct tagFILEDESC {
	WORD DescStatus;
	FILESIZE cbExpanded;
	FILESIZE cbCompressed;
	FILESIZE ofsData;
	BYTE MD5Sig[16];
	DWORD dwVerMS;
	DWORD dwVerLS;
	DWORD junk1[2];
	union {
		DWORD ofsName;
		LPSTR lpName;
	};
	WORD DirIndex;
	DWORD Attrs;
	WORD FatDate, FatTime;
	DWORD junk2;
	DWORD PrevDupe;
	DWORD NextDupe;
	BYTE LinkFlags;
	WORD Volume;
} FILEDESC, *LPFILEDESC;	// size=57

#define FILE_ATTR_READONLY             0x00000001  
#define FILE_ATTR_HIDDEN               0x00000002  
#define FILE_ATTR_SYSTEM               0x00000004  
#define FILE_ATTR_DIRECTORY            0x00000010  
#define FILE_ATTR_ARCHIVE              0x00000020  

#define DESC_SPLIT			1L
#define DESC_ENCRYPTED		2L
#define DESC_COMPRESSED		4L
#define DESC_INVALID		8L

typedef struct tagCOMPFILEENTRY {
	DWORD ofsName;		// 0
	DWORD ofsDesc;		// 4
	DWORD ofsNext;		// 8, size=C
} COMPONENTENTRY, *LPCOMPONENTENTRY, FILEGROUPENTRY, *LPFILEGROUPENTRY;

typedef DWORD* COMPONENTTABLE;
typedef DWORD* FILEGROUPTABLE;

//Morlac - Needed component
typedef struct tagCompStatus{
	WORD is_CRITICAL	:1;//#define COMP_STAT_CRITICAL		0x1
	WORD is_RECOMMEND	:1;//#define COMP_STAT_RECOMMEND	0x2
	WORD is_STANDARD	:1;//#define COMP_STAT_STANDARD		0x4
	WORD ignore			:13;
} CompStatus;
//Morlac - Components attibutes
typedef struct tagCompAttr {
	DWORD is_LOCKED			:1;//#define COMP_ATTR_LOCKED			0x1L
	DWORD unknown			:2;
	DWORD is_DATA_AS_FILES	:1;//#define COMP_ATTR_DATA_AS_FILES	0x8L
	DWORD is_VISIBLE		:1;//#define COMP_ATTR_VISIBLE			0x10L
	DWORD is_ENCRYPTED		:1;//#define COMP_ATTR_ENCRYPTION		0x20L
	DWORD is_VOLATILE		:1;//#define COMP_ATTR_VOLATILE			0x40L
	DWORD unknown2			:25;
} CompAttrs;

typedef struct tagCOMPONENTDESC {
	DWORD ofsName;						// 0
	DWORD ofsDescription;				// 4
	DWORD ofsStatusText;				// 8
	CompStatus  Status;					// c Morlac : Changed from Attrs to Status
	DWORD ofsPassword;					// e
	DWORD ofsMisc;						// 12
	WORD Index;							// 16
	DWORD ofsDispName;					// 18
	DWORD ofsCDRomFolder;				// 1c
	DWORD ofsHTTPLocation;				// 20
	DWORD ofsFTPLocation;				// 24
	GUID Guid[2];						// 28
	DWORD ofsUnkStr1;					// 48
	BYTE junk41[3];						// 4c
	CompAttrs Attrs;					// 4f Morlac: junk51[8] -> junk51[7];junk51[0]->Attrs
	DWORD junk51[7];
	WORD cFileGroups;					// 6f
	DWORD ofsFileGroups;				// 71
	WORD cDepends;						// 75
	DWORD ofsDepends;					// 77
	WORD cSubComps;						// 7b
	DWORD ofsSubComps;					// 7d
	DWORD ofsNextComponent;				// 81
	DWORD ofsOnInstalling;				// 85
	DWORD ofsOnInstalled;				// 89
	DWORD ofsOnUninstalling;			// 8d
	DWORD ofsOnUninstalled;				// 91
} COMPONENTDESC, *LPCOMPONENTDESC;		// size=95

//Morlac - overwrite attributes
typedef struct tagFG_overwrite {
	DWORD is_ALWAYS_OVERWRITE	:1;//#define FGDESC_ALWAYS_OVERWRITE	0x001L
	DWORD is_NEVER_OVERWRITE	:1;//#define FGDESC_NEVER_OVERWRITE		0x002L
	DWORD pad					:3;
	DWORD is_NEWER_DATE			:1;//#define FGDESC_NEWER_DATE			0x020L
	DWORD pad2					:3;
	DWORD is_NEWER_VERSION		:1;//#define FGDESC_NEWER_VERSION		0x200L
	DWORD pad3					:22;
} FG_Flags;
//Morlac - Group attributes
typedef struct tagFG_Attrs {
	WORD	is_Shared				:1;//1#define FGATTR_SHARED			0x01
	WORD	is_Encrypted			:1;//2#define FGATTR_ENCRYPTED		0x02
	WORD	is_Compressed			:1;//3#define FGATTR_COMPRESSED		0x04
	WORD	is_u1					:1;//4
	WORD	is_SelfRegistering		:1;//5#define FGATTR_SELFREGISTER	0x10
	WORD	is_PotentiallyLocked	:1;//6#define FGATTR_LOCKED			0x20
	WORD	is_Uninstall			:1;//7------Active Low-----------
	WORD	is_u2					:9;//8
} FG_Attrs;

typedef struct tagFILEGROUPDESC {
	DWORD		ofsName;
	FILESIZE	cbExpanded;
	FILESIZE	cbCompressed;
	FG_Attrs	Attrs;
	DWORD		FirstFile;
	DWORD		LastFile;
	DWORD		ofsUnkStr1;
	DWORD		ofsOperatingSystem;
	DWORD		ofsLanguage;
	DWORD		ofsUnkStr2;
	DWORD		ofsHTTPLocation;
	DWORD		ofsFTPLocation;
	DWORD		ofsMisc;
	DWORD		ofsTargetDir;
	FG_Flags	OverwriteFlags;		// Morlac -> Changed to a bitfield
	DWORD		junk1[4];
} FILEGROUPDESC, *LPFILEGROUPDESC;


typedef struct tagSETUPTYPEHEADER {
	DWORD cSTypes;
	DWORD ofsSTypeTab;
} SETUPTYPEHEADER, *LPSETUPTYPEHEADER;

typedef DWORD* SETUPTYPETABLE;

typedef struct tagSETUPTYPEDESC {
	DWORD			ofsName;
	DWORD			ofsDescription;
	DWORD			ofsDispName;
	SETUPTYPEHEADER	STHeader;		// Morlac -> 2 DWORDS to a struct
} SETUPTYPEDESC, *LPSETUPTYPEDESC;

#pragma pack( pop )

#define GetString(ptr, ofs) ( (LPSTR) (((BYTE*)ptr) + ((DWORD)ofs)) )

#define GetCompEntry(ptr, ofs) ( (LPCOMPONENTENTRY) (((BYTE*)ptr) + ofs) )

#define GetFileGroupEntry(ptr, ofs) ( (LPFILEGROUPENTRY) (((BYTE*)ptr) + ofs) )

#define GetSetupTypeDesc(ptr, stt, i) ( (LPSETUPTYPEDESC) (((BYTE*)ptr) + ((SETUPTYPETABLE)stt)[i]) )

#define GetCompDesc(ptr, ct, i) ( (LPCOMPONENTDESC) (((BYTE*)ptr) + ((COMPONENTTABLE)ct)[i]) )

#define GetFileGroupDesc(ptr, fgt, i) ( (LPFILEGROUPDESC) (((BYTE*)ptr) + ((FILEGROUPTABLE)fgt)[i]) )

#define GetFileDesc(ptr, dft, i) ( ((LPFILEDESC) (((BYTE*)dft) + ptr->ofsFilesDFT) ) + i )


#endif // ISHIELD6_INC
