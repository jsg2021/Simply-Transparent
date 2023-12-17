///////////////////////////////////////////////////////////////////////////////
//// _Button.cpp
//

#include "stdafx.h"
#include "simplytransparent.h"
#include "_Splash.h"


static BOOL CALLBACK SplashWndProc(HWND, UINT, WPARAM, LPARAM );

///////////////////////////////////////////////////////////////////////////////
///// Splash Window Proc
//		  
void Splash( HINSTANCE hinstance )
{
	DialogBox( hinstance, (LPCTSTR)IDD_SPLASH,
				NULL, (DLGPROC)SplashWndProc );
}

static BOOL CALLBACK SplashWndProc(HWND hWnd, UINT message,
							   WPARAM wParam, LPARAM lParam )
{
	switch ( message ) 
	{
		case WM_INITDIALOG:
			__AnimateWindow( hWnd, 1000, AW_ACTIVATE|AW_BLEND );
			ShowWindow( hWnd, SW_SHOW );			
			InvalidateRect( hWnd, 0, true );
			SetTimer( hWnd, 1, 1500, NULL );
			return true;

		case WM_CLOSE:
		case WM_TIMER:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			KillTimer( hWnd, 1 );
			
			EndDialog(hWnd, IDOK );
			return true;
	}
	return false;
}