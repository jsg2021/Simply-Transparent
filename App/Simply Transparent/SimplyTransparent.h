
#if !defined(AFX_SIMPLYTRANSPARENT_H__D7EAE86E_B0E6_4DE1_928D_789EC089B807__INCLUDED_)
#define AFX_SIMPLYTRANSPARENT_H__D7EAE86E_B0E6_4DE1_928D_789EC089B807__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "resource.h"


#define MAX_LOADSTRING		100
#define MAX_STRING			400


LRESULT CALLBACK About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	WndProc( HWND, UINT, WPARAM, LPARAM );
BOOL UpdateCheck( int major, int minor, BOOL silent );
HINSTANCE			GetMyInstanceHandle();
HWND				GetMyWindowHandle();
HWND				GetDeskListCtlWnd( );
string				GetMyName( );
BOOL				_IsRes_x86Loaded( );


#endif // !defined(AFX_SIMPLYTRANSPARENT_H__D7EAE86E_B0E6_4DE1_928D_789EC089B807__INCLUDED_)
