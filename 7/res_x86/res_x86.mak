# Microsoft Developer Studio Generated NMAKE File, Based on res_x86.dsp
!IF "$(CFG)" == ""
CFG=res_x86 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to res_x86 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "res_x86 - Win32 Release" && "$(CFG)" != "res_x86 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "res_x86.mak" CFG="res_x86 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "res_x86 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "res_x86 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "res_x86 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\Bin\res_x86.dll"


CLEAN :
	-@erase "$(INTDIR)\CfgFuncs.obj"
	-@erase "$(INTDIR)\res_x86.obj"
	-@erase "$(INTDIR)\res_x86.pch"
	-@erase "$(INTDIR)\resource.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\res_x86.exp"
	-@erase "$(OUTDIR)\res_x86.lib"
	-@erase "..\Bin\res_x86.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G6 /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RES_X86_EXPORTS" /Fp"$(INTDIR)\res_x86.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\resource.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\res_x86.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\res_x86.pdb" /machine:I386 /def:".\res_x86.def" /out:"../Bin/res_x86.dll" /implib:"$(OUTDIR)\res_x86.lib" 
DEF_FILE= \
	".\res_x86.def"
LINK32_OBJS= \
	"$(INTDIR)\CfgFuncs.obj" \
	"$(INTDIR)\res_x86.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\resource.res"

"..\Bin\res_x86.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "res_x86 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\Debug\res_x86.dll"


CLEAN :
	-@erase "$(INTDIR)\CfgFuncs.obj"
	-@erase "$(INTDIR)\res_x86.obj"
	-@erase "$(INTDIR)\res_x86.pch"
	-@erase "$(INTDIR)\resource.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\res_x86.exp"
	-@erase "$(OUTDIR)\res_x86.lib"
	-@erase "$(OUTDIR)\res_x86.pdb"
	-@erase "..\Debug\res_x86.dll"
	-@erase "..\Debug\res_x86.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RES_X86_EXPORTS" /Fp"$(INTDIR)\res_x86.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\resource.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\res_x86.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\res_x86.pdb" /debug /machine:I386 /def:".\res_x86.def" /out:"../Debug/res_x86.dll" /implib:"$(OUTDIR)\res_x86.lib" /pdbtype:sept 
DEF_FILE= \
	".\res_x86.def"
LINK32_OBJS= \
	"$(INTDIR)\CfgFuncs.obj" \
	"$(INTDIR)\res_x86.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\resource.res"

"..\Debug\res_x86.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("res_x86.dep")
!INCLUDE "res_x86.dep"
!ELSE 
!MESSAGE Warning: cannot find "res_x86.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "res_x86 - Win32 Release" || "$(CFG)" == "res_x86 - Win32 Debug"
SOURCE=.\CfgFuncs.cpp

"$(INTDIR)\CfgFuncs.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\res_x86.pch"


SOURCE=.\res_x86.cpp

"$(INTDIR)\res_x86.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\res_x86.pch"


SOURCE=.\resource.rc

"$(INTDIR)\resource.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "res_x86 - Win32 Release"

CPP_SWITCHES=/nologo /G6 /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RES_X86_EXPORTS" /Fp"$(INTDIR)\res_x86.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\res_x86.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "res_x86 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RES_X86_EXPORTS" /Fp"$(INTDIR)\res_x86.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\res_x86.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

