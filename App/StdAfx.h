// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers


// Windows Header Files:
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>



// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

#include <stdio.h>
#include <tchar.h>

#include <string>
using namespace std;

#include "CfgFuncs.h"
#include "options.h"

#ifdef MAX_PATH
#undef MAX_PATH
#define MAX_PATH 950//1560
#endif

#define WM_MY_TRAY_NOTIFICATION				WM_USER+101
#define WM_MY_BEGIN_EXIT_NOTIFICATION		WM_USER+102
#define WM_MY_EXIT_NOTIFICATION				WM_USER+103
#define WM_SIMPLYTRANSPARENT_SETTEXTCOLOR	WM_USER+106
#define WM_MY_RESTART_NOTIFICATION			WM_USER+110
#define WM_MY_SHOWTRAY_NOTIFICATION			WM_USER+111
#define WM_SHOW_OPTIONS						WM_APP+1
#define WM_SHOW_TEXTCOLOR					WM_APP+2
#define WM_GET_UPDATES						WM_APP+3
#define WM_SHOW_ABOUT						WM_APP+4

#ifndef AW_HOR_POSITIVE
#define AW_HOR_POSITIVE             0x00000001
#define AW_HOR_NEGATIVE             0x00000002
#define AW_VER_POSITIVE             0x00000004
#define AW_VER_NEGATIVE             0x00000008
#define AW_CENTER                   0x00000010
#define AW_HIDE                     0x00010000
#define AW_ACTIVATE                 0x00020000
#define AW_SLIDE                    0x00040000
#define AW_BLEND                    0x00080000
#endif //AW_HOR_POSITIVE

#define _WHITE				RGB( 255, 255, 255 )
#define _BLACK				RGB( 0, 0, 0 )

#ifdef _DEBUG//////////////////////////////////////////////////////////////////
#include <crtdbg.h>
#define ASSERT(b)	_ASSERTE(b)
#else//////////////////////////////////////////////////////////////////////////
#define ASSERT(b) ( b )
#define _CRT_WARN	1
#define _CRT_ERROR	2
#define _CRT_ASSERT	3	 
#endif/////////////////////////////////////////////////////////////////////////

void _InitLogger( );
int  _CheckLogger( );
void _CloseLogger( );
void __Comment ( const char * );
int __Message ( const char *message, int type = MB_OK );
int __Message ( UINT message, int type = MB_OK  );
void __Error( const char * );
void __Error( DWORD error );
void __ErrorMessage( const char *message );
void __ErrorMessage( DWORD error );


///////////////////////////////////////////////////////////////////////////////
//// Global Function Declarations
//
char * GetTime( char * );

BOOL __DoesStartWithWindows( LPCTSTR sExe, LPCTSTR sName,
								HWND hWnd = NULL, BOOL bRegistryStartup = false );

BOOL __StartWithWindows( BOOL bStart, LPCTSTR exe, LPCTSTR name,
						HWND hWnd = NULL, BOOL bUseRegistry = false );

HRESULT __CreateLink( LPCSTR, LPCSTR, LPCSTR, LPSTR, LPSTR );

HRESULT __GetSpecialFolderPath( HWND, LPTSTR, int, BOOL );

BOOL __AnimateWindow( HWND, DWORD, DWORD );

BOOL __ADenabled( BOOL );

///////////////////////////////////////////////////////////////////////////////
//// Color Dialog Hook proc...  used for centering...
//
UINT CALLBACK CCHookProc( HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);








///////////////////////////////////////////////////////////////////////////////
//// Tool Functions
//

inline BOOL IsWindowsNT( )
{
	BOOL ret = false;
	OSVERSIONINFO  osver;
	osver.dwOSVersionInfoSize = sizeof( osver );
	if( GetVersionEx( &osver ) )
		ret = ( osver.dwPlatformId == VER_PLATFORM_WIN32_NT );

	return ret;
}










inline BOOL ScreenToClient( HWND hWnd, LPRECT lpRect )
{
	BOOL ret;
	POINT t;
	
	int width = lpRect->right - lpRect->left;
	int height = lpRect->bottom - lpRect->top;

	t.x = lpRect->left;
	t.y = lpRect->top;
		
	ret = ScreenToClient( hWnd, &t );
	if(ret != 0 )
	{
		lpRect->left	= t.x;
		lpRect->top		= t.y;
		
		lpRect->right	= t.x+width;
		lpRect->bottom	= t.y+height;
	}

	return ret;
}










inline BOOL ClientToScreen( HWND hWnd, LPRECT lpRect )
{
	BOOL ret;
	POINT t;
	
	int width = lpRect->right - lpRect->left;
	int height = lpRect->bottom - lpRect->top;

	t.x = lpRect->left;
	t.y = lpRect->top;
		
	ret = ClientToScreen( hWnd, &t );
	if(ret != 0 )
	{
		lpRect->left	= t.x;
		lpRect->top		= t.y;
		
		lpRect->right	= t.x+width;
		lpRect->bottom	= t.y+height;
	}

	return ret;
}






inline BOOL IsPintInRect( LPPOINT lpPoint, LPRECT lpRect )
{
	BOOL ret = false;


	if( ( lpPoint->x >= lpRect->left && lpPoint->x <= lpRect->right )
		&& ( lpPoint->y >= lpRect->top && lpPoint->y <= lpRect->bottom ) )
			ret = true;

	return ret;
}







inline int ___find( string str, const char *c, int start )
{
	int length = str.length( );
	if ( start > length)
		return -1;

	char *res = strstr( str.c_str( ) + start, c);

	
	return ( res == NULL ) ? -1 : (int)( res - str.c_str( ) );
}

inline int ___reversefind( string str, const char c )
{
	char *res = strrchr( str.c_str( ), c );

	return (res == NULL) ? -1 : (int)(res - str.c_str( ) );
}

inline string ___mid( string str, int first, int count = -1 )
{
	if (first < 0) first = 0;
	
	if ( unsigned( first + count ) > str.length( ) || (count < 0) )
		count = str.length( ) - first;

	if ( unsigned( first ) > str.length( ) )
		count = 0;

	ASSERT( first >= 0 );
	ASSERT( unsigned( first + count ) <= str.length( ) );

	int size = count;

	char *dest = new char[size+1];
	memset( dest, '\0', size+1 );
	memcpy(dest, str.c_str( )+first, size );

	string temp;
	temp = dest;
	delete dest;

	return temp;
}

inline string ___right( string str, int start )
{
	if ( start < 0 ) start = 0;
	if ( unsigned(start) >= str.length( ) )
		start = str.length( );		
	
	int size = str.length( ) - start;

	char *dest = new char[size+1];
	memset( dest, '\0', size+1 );
	memcpy( dest, str.c_str( )+start, size );

	string temp;
	temp = dest;
	delete dest;

	return temp;
}

inline string ___left( string str, int end )
{
	if ( end < 0 ) end = 0;
	if ( unsigned(end) >= str.length( ) )
		end = str.length( );

	char *dest = new char[end+1];
	memset( dest, '\0', end+1 );
	memcpy( dest, str.c_str( ), end );

	string temp;
	temp = dest;
	delete dest;

	return temp;
}

inline void ___makelower( char *str )
{
	for( unsigned int x = 0; x<strlen( str ); x++ )
		str[x] = tolower( str[x] );
}

inline void ___makeupper( char *str )
{
	for( unsigned int x = 0; x<strlen( str ); x++ )
		str[x] = toupper( str[x] );
}

inline string ___changeCase( const char *str, bool blower = true )
{
	string out;
	out.resize( strlen( str ) );
	
	for( unsigned int x = 0; x< strlen( str ); x++ )
	{
		if(blower)
			out[x] = (str[x] <= 90 && str[x] >= 65 )?(str[x]+32):str[x];
		else
			out[x] = (str[x] <= 122 && str[x] >= 97 )?(str[x]-32):str[x];
	}

	return out;
}


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
