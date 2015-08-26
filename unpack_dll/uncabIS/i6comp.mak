# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=i6comp - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to i6comp - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "i6comp - Win32 Release" && "$(CFG)" != "i6comp - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "i6comp.mak" CFG="i6comp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "i6comp - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "i6comp - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "i6comp - Win32 Debug"
RSC=rc.exe
CPP=xicl4.exe

!IF  "$(CFG)" == "i6comp - Win32 Release"

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
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\i6comp.exe" ".\none"

CLEAN : 
	-@erase "$(INTDIR)\i6comp.obj"
	-@erase "$(INTDIR)\md5c.obj"
	-@erase "$(OUTDIR)\i6comp.exe"
	-@erase ".\none"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_CONSOLE"\
 /Fp"$(INTDIR)/i6comp.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/i6comp.bsc" 
BSC32_SBRS= \
	
LINK32=xilink4.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib advapi32.lib shell32.lib zdata\zdata.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /nodefaultlib
LINK32_FLAGS=kernel32.lib advapi32.lib shell32.lib zdata\zdata.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)/i6comp.pdb" /machine:I386\
 /out:"$(OUTDIR)/i6comp.exe" 
LINK32_OBJS= \
	"$(INTDIR)\i6comp.obj" \
	"$(INTDIR)\md5c.obj"

"$(OUTDIR)\i6comp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying file to Utilz dir
InputPath=.\Release\i6comp.exe
SOURCE=$(InputPath)

"none" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   copy $(InputPath) c:\utils

# End Custom Build

!ELSEIF  "$(CFG)" == "i6comp - Win32 Debug"

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
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\i6comp.exe" ".\none"

CLEAN : 
	-@erase "$(INTDIR)\i6comp.obj"
	-@erase "$(INTDIR)\md5c.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\i6comp.exe"
	-@erase "$(OUTDIR)\i6comp.ilk"
	-@erase "$(OUTDIR)\i6comp.pdb"
	-@erase ".\none"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE"\
 /Fp"$(INTDIR)/i6comp.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/i6comp.bsc" 
BSC32_SBRS= \
	
LINK32=xilink4.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib advapi32.lib shell32.lib zdata\zdata.lib /nologo /subsystem:console /debug /machine:I386
# SUBTRACT LINK32 /nodefaultlib
LINK32_FLAGS=kernel32.lib advapi32.lib shell32.lib zdata\zdata.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)/i6comp.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/i6comp.exe" 
LINK32_OBJS= \
	"$(INTDIR)\i6comp.obj" \
	"$(INTDIR)\md5c.obj"

"$(OUTDIR)\i6comp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

# Begin Custom Build - Copying file to Utilz dir
InputPath=.\Debug\i6comp.exe
SOURCE=$(InputPath)

"none" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   copy $(InputPath) c:\utils

# End Custom Build

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "i6comp - Win32 Release"
# Name "i6comp - Win32 Debug"

!IF  "$(CFG)" == "i6comp - Win32 Release"

!ELSEIF  "$(CFG)" == "i6comp - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\i6comp.c
DEP_CPP_I6COM=\
	".\i6comp.h"\
	{$(INCLUDE)}"\is6cab.h"\
	{$(INCLUDE)}"\rsa\global.h"\
	{$(INCLUDE)}"\rsa\md5.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	{$(INCLUDE)}"\sys\utime.h"\
	{$(INCLUDE)}"\zdatas.h"\
	

"$(INTDIR)\i6comp.obj" : $(SOURCE) $(DEP_CPP_I6COM) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Rsa\md5c.c
DEP_CPP_MD5C_=\
	{$(INCLUDE)}"\rsa\global.h"\
	{$(INCLUDE)}"\rsa\md5.h"\
	

"$(INTDIR)\md5c.obj" : $(SOURCE) $(DEP_CPP_MD5C_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
