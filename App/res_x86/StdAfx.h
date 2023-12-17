// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__FEB60DA1_F089_4692_963C_23967E771EE2__INCLUDED_)
#define AFX_STDAFX_H__FEB60DA1_F089_4692_963C_23967E771EE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>

HINSTANCE GetMyInstanceHandle( );
void ErrorMessage( DWORD code );
void SendQuit( );

///////////////////////////////////////////////////////////////////////////////
//// My MessageBox wrapper
//
inline int __Message( char* message )
	{ return MessageBox( 0, message, "res_x86", MB_OK ); }

inline BOOL IsWindowsNT( )
{
	BOOL ret = false;
	OSVERSIONINFO  osver;
	osver.dwOSVersionInfoSize = sizeof( osver );
	if( GetVersionEx( &osver ) )
		ret = ( osver.dwPlatformId == VER_PLATFORM_WIN32_NT );

	return ret;
}


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__FEB60DA1_F089_4692_963C_23967E771EE2__INCLUDED_)
