# Microsoft Developer Studio Generated NMAKE File, Based on SimplyTransparent.dsp
!IF "$(CFG)" == ""
CFG=SimplyTransparent - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SimplyTransparent - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SimplyTransparent - Win32 Release" && "$(CFG)" != "SimplyTransparent - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SimplyTransparent.mak" CFG="SimplyTransparent - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SimplyTransparent - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SimplyTransparent - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SimplyTransparent - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\Bin\SimplyTransparent.exe"


CLEAN :
	-@erase "$(INTDIR)\_Button.obj"
	-@erase "$(INTDIR)\_Splash.obj"
	-@erase "$(INTDIR)\CfgFuncs.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\Options.obj"
	-@erase "$(INTDIR)\SimplyTransparent.obj"
	-@erase "$(INTDIR)\SimplyTransparent.pch"
	-@erase "$(INTDIR)\SimplyTransparent.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TransCtrl.obj"
	-@erase "$(INTDIR)\trayicon.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\Bin\SimplyTransparent.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G6 /ML /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\SimplyTransparent.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\SimplyTransparent.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SimplyTransparent.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib comctl32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\SimplyTransparent.pdb" /machine:I386 /out:"Bin/SimplyTransparent.exe" 
LINK32_OBJS= \
	"$(INTDIR)\_Button.obj" \
	"$(INTDIR)\_Splash.obj" \
	"$(INTDIR)\CfgFuncs.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\Options.obj" \
	"$(INTDIR)\SimplyTransparent.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TransCtrl.obj" \
	"$(INTDIR)\trayicon.obj" \
	"$(INTDIR)\SimplyTransparent.res"

".\Bin\SimplyTransparent.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SimplyTransparent - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\SimplyTransparent.exe"


CLEAN :
	-@erase "$(INTDIR)\_Button.obj"
	-@erase "$(INTDIR)\_Splash.obj"
	-@erase "$(INTDIR)\CfgFuncs.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\Options.obj"
	-@erase "$(INTDIR)\SimplyTransparent.obj"
	-@erase "$(INTDIR)\SimplyTransparent.pch"
	-@erase "$(INTDIR)\SimplyTransparent.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TransCtrl.obj"
	-@erase "$(INTDIR)\trayicon.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SimplyTransparent.exe"
	-@erase "$(OUTDIR)\SimplyTransparent.ilk"
	-@erase "$(OUTDIR)\SimplyTransparent.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\SimplyTransparent.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\SimplyTransparent.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SimplyTransparent.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comctl32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\SimplyTransparent.pdb" /debug /machine:I386 /out:"$(OUTDIR)\SimplyTransparent.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\_Button.obj" \
	"$(INTDIR)\_Splash.obj" \
	"$(INTDIR)\CfgFuncs.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\Options.obj" \
	"$(INTDIR)\SimplyTransparent.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TransCtrl.obj" \
	"$(INTDIR)\trayicon.obj" \
	"$(INTDIR)\SimplyTransparent.res"

"$(OUTDIR)\SimplyTransparent.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("SimplyTransparent.dep")
!INCLUDE "SimplyTransparent.dep"
!ELSE 
!MESSAGE Warning: cannot find "SimplyTransparent.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SimplyTransparent - Win32 Release" || "$(CFG)" == "SimplyTransparent - Win32 Debug"
SOURCE=.\_Button.cpp

"$(INTDIR)\_Button.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SimplyTransparent.pch"


SOURCE=.\_Splash.cpp

"$(INTDIR)\_Splash.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SimplyTransparent.pch"


SOURCE=.\CfgFuncs.cpp

"$(INTDIR)\CfgFuncs.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SimplyTransparent.pch"


SOURCE=.\main.cpp

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SimplyTransparent.pch"


SOURCE=.\Options.cpp

"$(INTDIR)\Options.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SimplyTransparent.pch"


SOURCE=.\SimplyTransparent.cpp

"$(INTDIR)\SimplyTransparent.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SimplyTransparent.pch"


SOURCE=.\SimplyTransparent.rc

"$(INTDIR)\SimplyTransparent.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "SimplyTransparent - Win32 Release"

CPP_SWITCHES=/nologo /G6 /ML /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\SimplyTransparent.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\SimplyTransparent.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "SimplyTransparent - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\SimplyTransparent.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\SimplyTransparent.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TransCtrl.cpp

"$(INTDIR)\TransCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SimplyTransparent.pch"


SOURCE=.\trayicon.cpp

"$(INTDIR)\trayicon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\SimplyTransparent.pch"



!ENDIF 

