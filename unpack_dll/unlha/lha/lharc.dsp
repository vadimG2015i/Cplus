# Microsoft Developer Studio Project File - Name="lharc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=lharc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "lharc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "lharc.mak" CFG="lharc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "lharc - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "lharc - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "lharc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "lharc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "lharc - Win32 Release"
# Name "lharc - Win32 Debug"
# Begin Source File

SOURCE=.\append.c
# End Source File
# Begin Source File

SOURCE=.\crcio.c
# End Source File
# Begin Source File

SOURCE=.\dhuf.c
# End Source File
# Begin Source File

SOURCE=.\extract.c
# End Source File
# Begin Source File

SOURCE=.\header.c
# End Source File
# Begin Source File

SOURCE=.\huf.c
# End Source File
# Begin Source File

SOURCE=.\larc.c
# End Source File
# Begin Source File

SOURCE=.\lha.h
# End Source File
# Begin Source File

SOURCE=.\lha_macro.h
# End Source File
# Begin Source File

SOURCE=.\lhadd.c
# End Source File
# Begin Source File

SOURCE=.\lharc.c
# End Source File
# Begin Source File

SOURCE=.\lhdir.h
# End Source File
# Begin Source File

SOURCE=.\lhext.c
# End Source File
# Begin Source File

SOURCE=.\lhlist.c
# End Source File
# Begin Source File

SOURCE=.\maketbl.c
# End Source File
# Begin Source File

SOURCE=.\maketree.c
# End Source File
# Begin Source File

SOURCE=.\patmatch.c
# End Source File
# Begin Source File

SOURCE=.\shuf.c
# End Source File
# Begin Source File

SOURCE=.\slide.c
# End Source File
# Begin Source File

SOURCE=.\unlha.c
# End Source File
# Begin Source File

SOURCE=.\unlha.h
# End Source File
# Begin Source File

SOURCE=.\util.c
# End Source File
# Begin Source File

SOURCE=..\wce\wce_atow.c
# End Source File
# Begin Source File

SOURCE=..\wce\wce_direct.c
# End Source File
# Begin Source File

SOURCE=..\wce\wce_func.c
# End Source File
# Begin Source File

SOURCE=..\wce\wce_stat.c
# End Source File
# Begin Source File

SOURCE=..\wce\wce_strdup.c
# End Source File
# Begin Source File

SOURCE=..\wce\wce_time.c
# End Source File
# Begin Source File

SOURCE=..\wce\wce_utime.c
# End Source File
# Begin Source File

SOURCE=..\wce\wcedef.h
# End Source File
# Begin Source File

SOURCE=.\win32cfg.h
# End Source File
# End Target
# End Project
