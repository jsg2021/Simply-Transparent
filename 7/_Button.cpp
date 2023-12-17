///////////////////////////////////////////////////////////////////////////////
//// _Button.cpp
//

#include "stdafx.h"
#include "simplytransparent.h"
#include "_Button.h"


///////////////////////////////////////////////////////////////////////////////
///// Button Box Proc
//		  
LRESULT CALLBACK ButtonWndProc( HWND hWnd, UINT message,
							   WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc,hdc2;
	HBITMAP bm;
	HGDIOBJ obj;
	RECT rt;

	switch ( message ) 
	{
		case WM_INITDIALOG:
			InvalidateRect( hWnd, 0, true );
			return true;

		case WM_LBUTTONUP:
			ShowWindow( GetDeskListCtlWnd( ), SW_SHOW );
			__AnimateWindow( hWnd, 200, AW_HIDE|AW_BLEND );
			ShowWindow( hWnd, SW_HIDE );
			return true;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);			
			GetClientRect(hWnd, &rt);	
			bm = LoadBitmap( GetMyInstanceHandle( ),
				MAKEINTRESOURCE( IDB_BUTTON ) );

			hdc2 = CreateCompatibleDC(hdc);
			obj = SelectObject( hdc2, bm );

			BitBlt( hdc, 0, 0, 20, 18, hdc2, 0, 0, SRCCOPY); 

			//FillRect( hdc, &rt, CreateSolidBrush( 0x00ff00ff ) );

			SelectObject( hdc2, obj );
			DeleteDC( hdc2 );
			DeleteObject( bm );
			
			EndPaint(hWnd, &ps);

	}
	return DefWindowProc( hWnd, message, wParam, lParam );
}