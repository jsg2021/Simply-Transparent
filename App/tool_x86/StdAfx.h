// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B430EE75_98B4_4FF6_8610_BD93C97ECC2F__INCLUDED_)
#define AFX_STDAFX_H__B430EE75_98B4_4FF6_8610_BD93C97ECC2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <wininet.h>
#include <string>
using namespace std;

#ifdef _DEBUG//////////////////////////////////////////////////////////////////
#include <crtdbg.h>
#define ASSERT(b)	_ASSERTE(b)
#else//////////////////////////////////////////////////////////////////////////
#define ASSERT(b) ( b )
#endif/////////////////////////////////////////////////////////////////////////

void ErrorMessage( DWORD error );

int str_find( string str, const char *c, int start );
int str_reversefind( string str, const char c );
string str_mid( string str, int first, int count = -1 );
string str_right( string str, int start );
string str_left( string str, int end );

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B430EE75_98B4_4FF6_8610_BD93C97ECC2F__INCLUDED_)
