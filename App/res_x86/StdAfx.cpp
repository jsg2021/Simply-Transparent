// stdafx.cpp : source file that includes just the standard includes
//	res_x86.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "resource.h"







///////////////////////////////////////////////////////////////////////////////
////
//
void ErrorMessage( DWORD code ){
	LPVOID lpMsgBuf;
	
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);

	MessageBox( 0, (LPCTSTR)lpMsgBuf, "Simply Transparet: res_x86 Error", MB_OK );

	LocalFree( lpMsgBuf ); 
}










///////////////////////////////////////////////////////////////////////////////
//// SendQuit( ) - Sends the Quit message to the main application WindProc
//
void SendQuit( )
{
	char class_s[MAX_PATH];
	char title_s[MAX_PATH];

	LoadString( GetMyInstanceHandle( ),
		IDS_APP_CLASS, class_s, MAX_PATH);
	LoadString( GetMyInstanceHandle( ),
		IDS_APP_TITLE, title_s, MAX_PATH);

	for( unsigned int x = 0; x<strlen( class_s ); x++ )
		class_s[x] = tolower( class_s[x] );

	HWND hwnd = FindWindow( class_s, title_s );
	if( hwnd != NULL)
		SendMessage( hwnd, WM_QUIT, 0, 0 );
}
