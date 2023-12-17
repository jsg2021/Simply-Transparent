# Microsoft Developer Studio Generated NMAKE File, Based on tool_x86.dsp
!IF "$(CFG)" == ""
CFG=tool_x86 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to tool_x86 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "tool_x86 - Win32 Release" && "$(CFG)" != "tool_x86 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tool_x86.mak" CFG="tool_x86 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tool_x86 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "tool_x86 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "tool_x86 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\Bin\tool_x86.dll"


CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\tool_x86.obj"
	-@erase "$(INTDIR)\tool_x86.pch"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\tool_x86.exp"
	-@erase "$(OUTDIR)\tool_x86.lib"
	-@erase "..\Bin\tool_x86.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TOOL_X86_EXPORTS" /Fp"$(INTDIR)\tool_x86.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tool_x86.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\tool_x86.pdb" /machine:I386 /def:".\tool_x86.def" /out:"../Bin/tool_x86.dll" /implib:"$(OUTDIR)\tool_x86.lib" 
DEF_FILE= \
	".\tool_x86.def"
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\tool_x86.obj"

"..\Bin\tool_x86.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tool_x86 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\Debug\tool_x86.dll"


CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\tool_x86.obj"
	-@erase "$(INTDIR)\tool_x86.pch"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\tool_x86.exp"
	-@erase "$(OUTDIR)\tool_x86.lib"
	-@erase "$(OUTDIR)\tool_x86.pdb"
	-@erase "..\Debug\tool_x86.dll"
	-@erase "..\Debug\tool_x86.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TOOL_X86_EXPORTS" /Fp"$(INTDIR)\tool_x86.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tool_x86.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\tool_x86.pdb" /debug /machine:I386 /def:".\tool_x86.def" /out:"../Debug/tool_x86.dll" /implib:"$(OUTDIR)\tool_x86.lib" /pdbtype:sept 
DEF_FILE= \
	".\tool_x86.def"
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\tool_x86.obj"

"..\Debug\tool_x86.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("tool_x86.dep")
!INCLUDE "tool_x86.dep"
!ELSE 
!MESSAGE Warning: cannot find "tool_x86.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "tool_x86 - Win32 Release" || "$(CFG)" == "tool_x86 - Win32 Debug"
SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "tool_x86 - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TOOL_X86_EXPORTS" /Fp"$(INTDIR)\tool_x86.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\tool_x86.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "tool_x86 - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TOOL_X86_EXPORTS" /Fp"$(INTDIR)\tool_x86.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\tool_x86.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\tool_x86.cpp

"$(INTDIR)\tool_x86.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\tool_x86.pch"



!ENDIF 

