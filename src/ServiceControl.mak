# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=ServiceControl - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to ServiceControl - Win32\
 Debug.
!ENDIF 

!IF "$(CFG)" != "ServiceControl - Win32 Release" && "$(CFG)" !=\
 "ServiceControl - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "ServiceControl.mak" CFG="ServiceControl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ServiceControl - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ServiceControl - Win32 Debug" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "ServiceControl - Win32 Debug"
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "ServiceControl - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\ServiceControl.exe"

CLEAN : 
	-@erase "$(INTDIR)\ColumnsDlg.obj"
	-@erase "$(INTDIR)\ComputerNameDlg.obj"
	-@erase "$(INTDIR)\ConfigServiceDlg.obj"
	-@erase "$(INTDIR)\ListCtrlEx.obj"
	-@erase "$(INTDIR)\MiscUtil.obj"
	-@erase "$(INTDIR)\MultiString.obj"
	-@erase "$(INTDIR)\Registry.obj"
	-@erase "$(INTDIR)\ServiceControl.obj"
	-@erase "$(INTDIR)\ServiceControl.pch"
	-@erase "$(INTDIR)\ServiceControl.res"
	-@erase "$(INTDIR)\ServiceControlWnd.obj"
	-@erase "$(INTDIR)\ServiceProperty.obj"
	-@erase "$(INTDIR)\ServiceTracker.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\ServiceControl.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/ServiceControl.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x419 /fo"$(INTDIR)/ServiceControl.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ServiceControl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/ServiceControl.pdb" /machine:I386\
 /out:"$(OUTDIR)/ServiceControl.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ColumnsDlg.obj" \
	"$(INTDIR)\ComputerNameDlg.obj" \
	"$(INTDIR)\ConfigServiceDlg.obj" \
	"$(INTDIR)\ListCtrlEx.obj" \
	"$(INTDIR)\MiscUtil.obj" \
	"$(INTDIR)\MultiString.obj" \
	"$(INTDIR)\Registry.obj" \
	"$(INTDIR)\ServiceControl.obj" \
	"$(INTDIR)\ServiceControl.res" \
	"$(INTDIR)\ServiceControlWnd.obj" \
	"$(INTDIR)\ServiceProperty.obj" \
	"$(INTDIR)\ServiceTracker.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\ServiceControl.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ServiceControl - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\ServiceControl.exe"

CLEAN : 
	-@erase "$(INTDIR)\ColumnsDlg.obj"
	-@erase "$(INTDIR)\ComputerNameDlg.obj"
	-@erase "$(INTDIR)\ConfigServiceDlg.obj"
	-@erase "$(INTDIR)\ListCtrlEx.obj"
	-@erase "$(INTDIR)\MiscUtil.obj"
	-@erase "$(INTDIR)\MultiString.obj"
	-@erase "$(INTDIR)\Registry.obj"
	-@erase "$(INTDIR)\ServiceControl.obj"
	-@erase "$(INTDIR)\ServiceControl.pch"
	-@erase "$(INTDIR)\ServiceControl.res"
	-@erase "$(INTDIR)\ServiceControlWnd.obj"
	-@erase "$(INTDIR)\ServiceProperty.obj"
	-@erase "$(INTDIR)\ServiceTracker.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\ServiceControl.exe"
	-@erase "$(OUTDIR)\ServiceControl.ilk"
	-@erase "$(OUTDIR)\ServiceControl.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/ServiceControl.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x419 /fo"$(INTDIR)/ServiceControl.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ServiceControl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/ServiceControl.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/ServiceControl.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ColumnsDlg.obj" \
	"$(INTDIR)\ComputerNameDlg.obj" \
	"$(INTDIR)\ConfigServiceDlg.obj" \
	"$(INTDIR)\ListCtrlEx.obj" \
	"$(INTDIR)\MiscUtil.obj" \
	"$(INTDIR)\MultiString.obj" \
	"$(INTDIR)\Registry.obj" \
	"$(INTDIR)\ServiceControl.obj" \
	"$(INTDIR)\ServiceControl.res" \
	"$(INTDIR)\ServiceControlWnd.obj" \
	"$(INTDIR)\ServiceProperty.obj" \
	"$(INTDIR)\ServiceTracker.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\ServiceControl.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "ServiceControl - Win32 Release"
# Name "ServiceControl - Win32 Debug"

!IF  "$(CFG)" == "ServiceControl - Win32 Release"

!ELSEIF  "$(CFG)" == "ServiceControl - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ServiceControl.cpp
DEP_CPP_SERVI=\
	".\ListCtrlEx.h"\
	".\ServiceControl.h"\
	".\ServiceControlWnd.h"\
	".\ServiceTracker.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ServiceControl.obj" : $(SOURCE) $(DEP_CPP_SERVI) "$(INTDIR)"\
 "$(INTDIR)\ServiceControl.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "ServiceControl - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/ServiceControl.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\ServiceControl.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "ServiceControl - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/ServiceControl.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\ServiceControl.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ServiceControl.rc
DEP_RSC_SERVIC=\
	".\res\ServiceControl.ico"\
	".\res\ServiceControl.rc2"\
	".\res\toolbar1.bmp"\
	

"$(INTDIR)\ServiceControl.res" : $(SOURCE) $(DEP_RSC_SERVIC) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ListCtrlEx.cpp
DEP_CPP_LISTC=\
	".\ListCtrlEx.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ListCtrlEx.obj" : $(SOURCE) $(DEP_CPP_LISTC) "$(INTDIR)"\
 "$(INTDIR)\ServiceControl.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ServiceControlWnd.cpp
DEP_CPP_SERVICE=\
	".\ColumnsDlg.h"\
	".\ComputerNameDlg.h"\
	".\ConfigServiceDlg.h"\
	".\ListCtrlEx.h"\
	".\MiscUtil.h"\
	".\MultiString.h"\
	".\ServiceControl.h"\
	".\ServiceControlWnd.h"\
	".\ServiceProperty.h"\
	".\ServiceTracker.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ServiceControlWnd.obj" : $(SOURCE) $(DEP_CPP_SERVICE) "$(INTDIR)"\
 "$(INTDIR)\ServiceControl.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ColumnsDlg.cpp
DEP_CPP_COLUM=\
	".\ColumnsDlg.h"\
	".\ServiceControl.h"\
	".\ServiceProperty.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ColumnsDlg.obj" : $(SOURCE) $(DEP_CPP_COLUM) "$(INTDIR)"\
 "$(INTDIR)\ServiceControl.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ServiceProperty.cpp
DEP_CPP_SERVICEP=\
	".\ServiceProperty.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ServiceProperty.obj" : $(SOURCE) $(DEP_CPP_SERVICEP) "$(INTDIR)"\
 "$(INTDIR)\ServiceControl.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ComputerNameDlg.cpp
DEP_CPP_COMPU=\
	".\ComputerNameDlg.h"\
	".\ServiceControl.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ComputerNameDlg.obj" : $(SOURCE) $(DEP_CPP_COMPU) "$(INTDIR)"\
 "$(INTDIR)\ServiceControl.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ServiceTracker.cpp
DEP_CPP_SERVICET=\
	".\ServiceTracker.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ServiceTracker.obj" : $(SOURCE) $(DEP_CPP_SERVICET) "$(INTDIR)"\
 "$(INTDIR)\ServiceControl.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ConfigServiceDlg.cpp
DEP_CPP_CONFI=\
	".\ConfigServiceDlg.h"\
	".\MiscUtil.h"\
	".\MultiString.h"\
	".\ServiceControl.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ConfigServiceDlg.obj" : $(SOURCE) $(DEP_CPP_CONFI) "$(INTDIR)"\
 "$(INTDIR)\ServiceControl.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MiscUtil.cpp
DEP_CPP_MISCU=\
	".\MiscUtil.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MiscUtil.obj" : $(SOURCE) $(DEP_CPP_MISCU) "$(INTDIR)"\
 "$(INTDIR)\ServiceControl.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MultiString.cpp
DEP_CPP_MULTI=\
	".\MultiString.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MultiString.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"\
 "$(INTDIR)\ServiceControl.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Registry.cpp
DEP_CPP_REGIS=\
	".\ListCtrlEx.h"\
	".\MultiString.h"\
	".\ServiceControlWnd.h"\
	".\ServiceProperty.h"\
	".\ServiceTracker.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Registry.obj" : $(SOURCE) $(DEP_CPP_REGIS) "$(INTDIR)"\
 "$(INTDIR)\ServiceControl.pch"


# End Source File
# End Target
# End Project
################################################################################
