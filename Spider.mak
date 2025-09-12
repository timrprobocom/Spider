# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Spider - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Spider - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Spider - Win32 Release" && "$(CFG)" != "Spider - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Spider.mak" CFG="Spider - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Spider - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Spider - Win32 Debug" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "Spider - Win32 Debug"
RSC=rc.exe
MTL=mktyplib.exe
CPP=cl.exe

!IF  "$(CFG)" == "Spider - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\Spider.exe"

CLEAN : 
	-@erase ".\Release\Spider.exe"
	-@erase ".\Release\SPIDER.OBJ"
	-@erase ".\Release\FRAME.OBJ"
	-@erase ".\Release\SPIDER.res"
	-@erase ".\Release\Spider.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)/Spider.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/SPIDER.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Spider.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /map /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/Spider.pdb" /map:"$(INTDIR)/Spider.map" /machine:I386\
 /out:"$(OUTDIR)/Spider.exe" 
LINK32_OBJS= \
	"$(INTDIR)/SPIDER.OBJ" \
	"$(INTDIR)/FRAME.OBJ" \
	"$(INTDIR)/SPIDER.res"

"$(OUTDIR)\Spider.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Spider - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\Spider.exe"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\Spider.exe"
	-@erase ".\Debug\FRAME.OBJ"
	-@erase ".\Debug\SPIDER.OBJ"
	-@erase ".\Debug\SPIDER.res"
	-@erase ".\Debug\Spider.ilk"
	-@erase ".\Debug\Spider.pdb"
	-@erase ".\Debug\Spider.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)/Spider.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/SPIDER.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Spider.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /map /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/Spider.pdb" /map:"$(INTDIR)/Spider.map" /debug /machine:I386\
 /out:"$(OUTDIR)/Spider.exe" 
LINK32_OBJS= \
	"$(INTDIR)/FRAME.OBJ" \
	"$(INTDIR)/SPIDER.OBJ" \
	"$(INTDIR)/SPIDER.res"

"$(OUTDIR)\Spider.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

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

# Name "Spider - Win32 Release"
# Name "Spider - Win32 Debug"

!IF  "$(CFG)" == "Spider - Win32 Release"

!ELSEIF  "$(CFG)" == "Spider - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\SPIDER.RC
DEP_RSC_SPIDE=\
	".\spider.ico"\
	".\DIALOGS.H"\
	".\dialogs.dlg"\
	

"$(INTDIR)\SPIDER.res" : $(SOURCE) $(DEP_RSC_SPIDE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIDER.H

!IF  "$(CFG)" == "Spider - Win32 Release"

!ELSEIF  "$(CFG)" == "Spider - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SPIDER.CPP
DEP_CPP_SPIDER=\
	".\FRAME.H"\
	".\SPIDER.H"\
	".\DIALOGS.H"\
	".\CRD.H"\
	".\CDT.H"\
	

"$(INTDIR)\SPIDER.OBJ" : $(SOURCE) $(DEP_CPP_SPIDER) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\RESOURCE.H

!IF  "$(CFG)" == "Spider - Win32 Release"

!ELSEIF  "$(CFG)" == "Spider - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FRAME.H

!IF  "$(CFG)" == "Spider - Win32 Release"

!ELSEIF  "$(CFG)" == "Spider - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FRAME.CPP
DEP_CPP_FRAME=\
	".\FRAME.H"\
	".\SPIDER.H"\
	".\CRD.H"\
	".\DIALOGS.H"\
	".\CDT.H"\
	

"$(INTDIR)\FRAME.OBJ" : $(SOURCE) $(DEP_CPP_FRAME) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DIALOGS.H

!IF  "$(CFG)" == "Spider - Win32 Release"

!ELSEIF  "$(CFG)" == "Spider - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CRD.H

!IF  "$(CFG)" == "Spider - Win32 Release"

!ELSEIF  "$(CFG)" == "Spider - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CDT.H

!IF  "$(CFG)" == "Spider - Win32 Release"

!ELSEIF  "$(CFG)" == "Spider - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
