# Microsoft Developer Studio Project File - Name="unpack" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=unpack - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "unpack.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "unpack.mak" CFG="unpack - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "unpack - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "unpack - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "unpack - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UNPACK_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I "./" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UNPACK_EXPORTS" /D "WINNT" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "unpack - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UNPACK_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /ZI /I "./" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UNPACK_EXPORTS" /D "WINNT" /FR /YX /FD /GZ /EHs /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "unpack - Win32 Release"
# Name "unpack - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "unzip"

# PROP Default_Filter "c,cpp"
# Begin Source File

SOURCE=.\unzip\adler32.c
# End Source File
# Begin Source File

SOURCE=.\unzip\compress.c
# End Source File
# Begin Source File

SOURCE=.\unzip\crc32.c
# End Source File
# Begin Source File

SOURCE=.\unzip\deflate.c
# End Source File
# Begin Source File

SOURCE=.\unzip\gvmat32c.c
# End Source File
# Begin Source File

SOURCE=.\unzip\gzio.c
# End Source File
# Begin Source File

SOURCE=.\unzip\infblock.c
# End Source File
# Begin Source File

SOURCE=.\unzip\infcodes.c
# End Source File
# Begin Source File

SOURCE=.\unzip\inffast.c
# End Source File
# Begin Source File

SOURCE=.\unzip\inflate.c
# End Source File
# Begin Source File

SOURCE=.\unzip\inftrees.c
# End Source File
# Begin Source File

SOURCE=.\unzip\infutil.c
# End Source File
# Begin Source File

SOURCE=.\unzip\ioapi.c
# End Source File
# Begin Source File

SOURCE=.\unzip\iowin32.c
# End Source File
# Begin Source File

SOURCE=.\unzip\trees.c
# End Source File
# Begin Source File

SOURCE=.\unzip\uncompr.c
# End Source File
# Begin Source File

SOURCE=.\unzip\unzip.c
# End Source File
# Begin Source File

SOURCE=.\unlha\wce\wce_atow.c
# End Source File
# Begin Source File

SOURCE=.\unzip\zutil.c
# End Source File
# End Group
# Begin Group "unrar"

# PROP Default_Filter "c,cpp"
# Begin Source File

SOURCE=.\unrar\archive.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\arcread.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\cmddata.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\consio.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\crc.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\crypt.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\encname.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\errhnd.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\extinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\extract.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\filcreat.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\file.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\filefn.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\filestr.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\find.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\getbits.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\global.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\int64.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\isnt.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\list.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\match.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\options.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\pathfn.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\rar.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\rarvm.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\rawread.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\rdwrfn.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\recvol.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\resource.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\rijndael.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\rs.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\savepos.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\scantree.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\sha1.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\strfn.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\strlist.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\system.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\timefn.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\ulinks.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\unicode.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\unpack.cpp
# End Source File
# Begin Source File

SOURCE=.\unrar\volume.cpp
# End Source File
# End Group
# Begin Group "unarj"

# PROP Default_Filter "c,cpp"
# Begin Source File

SOURCE=.\unarj\arj.c
# End Source File
# Begin Source File

SOURCE=.\unarj\arj_arcv.c
# End Source File
# Begin Source File

SOURCE=.\unarj\arj_file.c
# End Source File
# Begin Source File

SOURCE=.\unarj\arj_proc.c
# End Source File
# Begin Source File

SOURCE=.\unarj\arj_user.c
# End Source File
# Begin Source File

SOURCE=.\unarj\arjdata.c
# End Source File
# Begin Source File

SOURCE=.\unarj\arjsec_h.c
# End Source File
# Begin Source File

SOURCE=.\unarj\arjsec_l.c
# End Source File
# Begin Source File

SOURCE=.\unarj\arjtypes.c
# End Source File
# Begin Source File

SOURCE=.\unarj\crc16tab.c
# End Source File
# Begin Source File

SOURCE=.\unarj\crc32.c

!IF  "$(CFG)" == "unpack - Win32 Release"

# PROP Intermediate_Dir "Release2"

!ELSEIF  "$(CFG)" == "unpack - Win32 Debug"

# PROP Intermediate_Dir "Debug2"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\unarj\decode.c
# End Source File
# Begin Source File

SOURCE=.\unarj\ea_mgr.c
# End Source File
# Begin Source File

SOURCE=.\unarj\enc_gwy.c
# End Source File
# Begin Source File

SOURCE=.\unarj\encode.c
# End Source File
# Begin Source File

SOURCE=.\unarj\environ.c
# End Source File
# Begin Source File

SOURCE=.\unarj\exe_sear.c
# End Source File
# Begin Source File

SOURCE=.\unarj\ext_hdr.c
# End Source File
# Begin Source File

SOURCE=.\unarj\externs.c
# End Source File
# Begin Source File

SOURCE=.\unarj\fardata.c
# End Source File
# Begin Source File

SOURCE=.\unarj\file_reg.c
# End Source File
# Begin Source File

SOURCE=.\unarj\filelist.c
# End Source File
# Begin Source File

SOURCE=.\unarj\filemode.c
# End Source File
# Begin Source File

SOURCE=.\unarj\list.c

!IF  "$(CFG)" == "unpack - Win32 Release"

# PROP Intermediate_Dir "Release2"

!ELSEIF  "$(CFG)" == "unpack - Win32 Debug"

# PROP Intermediate_Dir "Debug2"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\unarj\misc.c
# End Source File
# Begin Source File

SOURCE=.\unarj\recovery.c
# End Source File
# Begin Source File

SOURCE=.\unarj\uxspec.c
# End Source File
# End Group
# Begin Group "unace"

# PROP Default_Filter "c,cpp"
# Begin Source File

SOURCE=.\unace\globals.c
# End Source File
# Begin Source File

SOURCE=.\unace\uac_comm.c
# End Source File
# Begin Source File

SOURCE=.\unace\uac_crc.c
# End Source File
# Begin Source File

SOURCE=.\unace\uac_crt.c
# End Source File
# Begin Source File

SOURCE=.\unace\uac_dcpr.c
# End Source File
# Begin Source File

SOURCE=.\unace\uac_sys.c
# End Source File
# Begin Source File

SOURCE=.\unace\unace.c
# End Source File
# End Group
# Begin Group "uncabMSCF"

# PROP Default_Filter "c,cpp"
# End Group
# Begin Group "uncabIS"

# PROP Default_Filter "c,cpp"
# Begin Source File

SOURCE=.\uncabIS\i6comp.c
# End Source File
# Begin Source File

SOURCE=.\uncabIS\Rsa\md5c.c
# End Source File
# End Group
# Begin Group "untar"

# PROP Default_Filter "c,cpp"
# Begin Source File

SOURCE=.\untar\ARCBZ2.CPP
# End Source File
# Begin Source File

SOURCE=.\untar\ARCFILE.CPP
# End Source File
# Begin Source File

SOURCE=.\untar\ARCGZ.CPP
# End Source File
# Begin Source File

SOURCE=.\untar\ARCZ.CPP
# End Source File
# Begin Source File

SOURCE=.\unbz2\blocksort.c
# End Source File
# Begin Source File

SOURCE=.\unbz2\bzip2.c
# End Source File
# Begin Source File

SOURCE=.\unbz2\bzlib.c
# End Source File
# Begin Source File

SOURCE=.\untar\CMDLINE.C
# End Source File
# Begin Source File

SOURCE=.\unbz2\compress.c

!IF  "$(CFG)" == "unpack - Win32 Release"

# PROP Intermediate_Dir "Release2"

!ELSEIF  "$(CFG)" == "unpack - Win32 Debug"

# PROP Intermediate_Dir "Debug2"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\unbz2\crctable.c
# End Source File
# Begin Source File

SOURCE=.\unbz2\decompress.c
# End Source File
# Begin Source File

SOURCE=.\unbz2\huffman.c
# End Source File
# Begin Source File

SOURCE=.\untar\NORMAL.CPP
# End Source File
# Begin Source File

SOURCE=.\unbz2\randtable.c
# End Source File
# Begin Source File

SOURCE=.\untar\RPM.CPP
# End Source File
# Begin Source File

SOURCE=.\untar\TAR32.CPP
# End Source File
# Begin Source File

SOURCE=.\untar\TAR32API.CPP
# End Source File
# Begin Source File

SOURCE=.\untar\TAR32DLL.CPP
# End Source File
# Begin Source File

SOURCE=.\untar\TARCMD.CPP
# End Source File
# Begin Source File

SOURCE=.\untar\UTIL.CPP
# End Source File
# End Group
# Begin Group "unlha"

# PROP Default_Filter "c,cpp"
# Begin Source File

SOURCE=.\unlha\lha\append.c
# End Source File
# Begin Source File

SOURCE=.\unlha\lha\crcio.c
# End Source File
# Begin Source File

SOURCE=.\unlha\lha\dhuf.c
# End Source File
# Begin Source File

SOURCE=.\unlha\lha\extract.c

!IF  "$(CFG)" == "unpack - Win32 Release"

# PROP Intermediate_Dir "Release2"

!ELSEIF  "$(CFG)" == "unpack - Win32 Debug"

# PROP Intermediate_Dir "Debug2"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\unlha\lha\header.c
# End Source File
# Begin Source File

SOURCE=.\unlha\lha\huf.c
# End Source File
# Begin Source File

SOURCE=.\unlha\lha\larc.c
# End Source File
# Begin Source File

SOURCE=.\unlha\lha\lhadd.c
# End Source File
# Begin Source File

SOURCE=.\unlha\lha\lharc.c
# End Source File
# Begin Source File

SOURCE=.\unlha\lha\lhext.c
# End Source File
# Begin Source File

SOURCE=.\unlha\lha\lhlist.c
# End Source File
# Begin Source File

SOURCE=.\unlha\lha\maketbl.c
# End Source File
# Begin Source File

SOURCE=.\unlha\lha\maketree.c
# End Source File
# Begin Source File

SOURCE=.\unlha\lha\patmatch.c
# End Source File
# Begin Source File

SOURCE=.\unlha\lha\shuf.c
# End Source File
# Begin Source File

SOURCE=.\unlha\lha\slide.c
# End Source File
# Begin Source File

SOURCE=.\unlha\lha\unlha.c
# End Source File
# Begin Source File

SOURCE=.\unlha\lha\util.c

!IF  "$(CFG)" == "unpack - Win32 Release"

# PROP Intermediate_Dir "Release2"

!ELSEIF  "$(CFG)" == "unpack - Win32 Debug"

# PROP Intermediate_Dir "Debug2"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\unlha\wce\wce_func.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\unpack.cpp

!IF  "$(CFG)" == "unpack - Win32 Release"

# PROP Intermediate_Dir "Release2"

!ELSEIF  "$(CFG)" == "unpack - Win32 Debug"

# PROP Intermediate_Dir "Debug2"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "unzip_headers"

# PROP Default_Filter "h,hpp"
# Begin Source File

SOURCE=.\unzip\deflate.h
# End Source File
# Begin Source File

SOURCE=.\unzip\infblock.h
# End Source File
# Begin Source File

SOURCE=.\unzip\infcodes.h
# End Source File
# Begin Source File

SOURCE=.\unzip\inffast.h
# End Source File
# Begin Source File

SOURCE=.\unzip\inffixed.h
# End Source File
# Begin Source File

SOURCE=.\unzip\inftrees.h
# End Source File
# Begin Source File

SOURCE=.\unzip\infutil.h
# End Source File
# Begin Source File

SOURCE=.\unzip\ioapi.h
# End Source File
# Begin Source File

SOURCE=.\unzip\iowin32.h
# End Source File
# Begin Source File

SOURCE=.\unzip\resource.h
# End Source File
# Begin Source File

SOURCE=.\unzip\trees.h
# End Source File
# Begin Source File

SOURCE=.\unzip\unzip.h
# End Source File
# Begin Source File

SOURCE=.\unzip\zconf.h
# End Source File
# Begin Source File

SOURCE=.\uncabIS\zdatas.h
# End Source File
# Begin Source File

SOURCE=.\unzip\zip.h
# End Source File
# Begin Source File

SOURCE=.\unzip\zlib.h
# End Source File
# Begin Source File

SOURCE=.\unzip\zutil.h
# End Source File
# End Group
# Begin Group "unrar_headers"

# PROP Default_Filter "h,hpp"
# Begin Source File

SOURCE=.\unrar\archive.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\array.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\cmddata.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\coder.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\compress.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\consio.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\crc.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\crypt.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\dll.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\encname.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\errhnd.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\extinfo.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\extract.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\filcreat.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\file.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\filefn.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\filestr.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\find.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\getbits.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\global.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\headers.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\int64.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\isnt.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\list.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\loclang.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\match.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\model.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\options.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\os.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\pathfn.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\rar.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\rardefs.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\rarfn.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\rarlang.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\raros.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\rartypes.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\rarvm.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\rawread.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\rdwrfn.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\recvol.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\resource.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\rijndael.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\rs.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\savepos.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\scantree.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\sha1.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\smallfn.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\unrar\strfn.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\strlist.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\suballoc.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\system.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\timefn.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\ulinks.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\unicode.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\unpack.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\version.hpp
# End Source File
# Begin Source File

SOURCE=.\unrar\volume.hpp
# End Source File
# End Group
# Begin Group "unarj_headers"

# PROP Default_Filter "h,hpp"
# Begin Source File

SOURCE=.\unarj\arj.h
# End Source File
# Begin Source File

SOURCE=.\unarj\arj_arcv.h
# End Source File
# Begin Source File

SOURCE=.\unarj\arj_file.h
# End Source File
# Begin Source File

SOURCE=.\unarj\arj_proc.h
# End Source File
# Begin Source File

SOURCE=.\unarj\arj_user.h
# End Source File
# Begin Source File

SOURCE=.\unarj\arjdata.h
# End Source File
# Begin Source File

SOURCE=.\unarj\arjsec_h.h
# End Source File
# Begin Source File

SOURCE=.\unarj\arjsec_l.h
# End Source File
# Begin Source File

SOURCE=.\unarj\arjtypes.h
# End Source File
# Begin Source File

SOURCE=.\unarj\bindings.h
# End Source File
# Begin Source File

SOURCE=.\unarj\crc16tab.h
# End Source File
# Begin Source File

SOURCE=.\unarj\crc32.h
# End Source File
# Begin Source File

SOURCE=.\unarj\decode.h
# End Source File
# Begin Source File

SOURCE=.\unarj\defines.h
# End Source File
# Begin Source File

SOURCE=.\unarj\ea_mgr.h
# End Source File
# Begin Source File

SOURCE=.\unarj\enc_gwy.h
# End Source File
# Begin Source File

SOURCE=.\unarj\encode.h
# End Source File
# Begin Source File

SOURCE=.\unarj\environ.h
# End Source File
# Begin Source File

SOURCE=.\unarj\exe_sear.h
# End Source File
# Begin Source File

SOURCE=.\unarj\ext_hdr.h
# End Source File
# Begin Source File

SOURCE=.\unarj\externs.h
# End Source File
# Begin Source File

SOURCE=.\unarj\fardata.h
# End Source File
# Begin Source File

SOURCE=.\unarj\file_reg.h
# End Source File
# Begin Source File

SOURCE=.\unarj\filelist.h
# End Source File
# Begin Source File

SOURCE=.\unarj\filemode.h
# End Source File
# Begin Source File

SOURCE=.\unarj\list.h
# End Source File
# Begin Source File

SOURCE=.\unarj\misc.h
# End Source File
# Begin Source File

SOURCE=.\unarj\recovery.h
# End Source File
# Begin Source File

SOURCE=.\unarj\resource2.h
# End Source File
# Begin Source File

SOURCE=.\unarj\uxspec.h
# End Source File
# End Group
# Begin Group "unace_headers"

# PROP Default_Filter "h,hpp"
# Begin Source File

SOURCE=.\unace\acestruc.h
# End Source File
# Begin Source File

SOURCE=.\unace\declare.h
# End Source File
# Begin Source File

SOURCE=.\unace\globals.h
# End Source File
# Begin Source File

SOURCE=.\unace\os.h
# End Source File
# Begin Source File

SOURCE=.\unace\portable.h
# End Source File
# Begin Source File

SOURCE=.\unace\uac_comm.h
# End Source File
# Begin Source File

SOURCE=.\unace\uac_crc.h
# End Source File
# Begin Source File

SOURCE=.\unace\uac_crt.h
# End Source File
# Begin Source File

SOURCE=.\unace\uac_dcpr.h
# End Source File
# Begin Source File

SOURCE=.\unace\uac_sys.h
# End Source File
# Begin Source File

SOURCE=.\unace\unace.h
# End Source File
# End Group
# Begin Group "uncabMSCF_headers"

# PROP Default_Filter "h,hpp"
# Begin Source File

SOURCE=.\uncabMSCF\cabextract.h
# End Source File
# Begin Source File

SOURCE=.\uncabMSCF\getopt.h
# End Source File
# Begin Source File

SOURCE=.\uncabMSCF\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\uncabMSCF\uncab.h
# End Source File
# End Group
# Begin Group "uncabIS_headers"

# PROP Default_Filter "h,hpp"
# Begin Source File

SOURCE=.\uncabIS\Rsa\global.h
# End Source File
# Begin Source File

SOURCE=.\uncabIS\i6comp.h
# End Source File
# Begin Source File

SOURCE=.\uncabIS\is6cab.h
# End Source File
# Begin Source File

SOURCE=.\uncabIS\Rsa\md5.h
# End Source File
# Begin Source File

SOURCE=.\uncabIS\StdAfx.h
# End Source File
# End Group
# Begin Group "untar_headers"

# PROP Default_Filter "h,hpp"
# Begin Source File

SOURCE=.\untar\AR.H
# End Source File
# Begin Source File

SOURCE=.\untar\ARCBZ2.H
# End Source File
# Begin Source File

SOURCE=.\untar\ARCFILE.H
# End Source File
# Begin Source File

SOURCE=.\untar\ARCGZ.H
# End Source File
# Begin Source File

SOURCE=.\untar\ARCZ.H
# End Source File
# Begin Source File

SOURCE=.\untar\CMDLINE.H
# End Source File
# Begin Source File

SOURCE=.\untar\CPIO.H
# End Source File
# Begin Source File

SOURCE=.\untar\FAST_STL.H
# End Source File
# Begin Source File

SOURCE=.\untar\NORMAL.H
# End Source File
# Begin Source File

SOURCE=.\untar\RPM.H
# End Source File
# Begin Source File

SOURCE=.\untar\STDAFX.H
# End Source File
# Begin Source File

SOURCE=.\untar\TAR.H
# End Source File
# Begin Source File

SOURCE=.\untar\TAR32.H
# End Source File
# Begin Source File

SOURCE=.\untar\TAR32API.H
# End Source File
# Begin Source File

SOURCE=.\untar\TAR32DLL.H
# End Source File
# Begin Source File

SOURCE=.\untar\TAR32RES.H
# End Source File
# Begin Source File

SOURCE=.\untar\TARCMD.H
# End Source File
# Begin Source File

SOURCE=.\untar\UTIL.H
# End Source File
# End Group
# Begin Group "unlha_headers"

# PROP Default_Filter "h,hpp"
# Begin Source File

SOURCE=.\unlha\lha\lha.h
# End Source File
# Begin Source File

SOURCE=.\unlha\lha\lha_macro.h
# End Source File
# Begin Source File

SOURCE=.\unlha\lha\unlha.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\unpack.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\uncabIS\ZData\zdata.lib
# End Source File
# End Group
# End Target
# End Project
