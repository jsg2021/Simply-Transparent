// res_x86.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "res_x86.h"
#include "CfgFuncs.h"

#include "..\getwnd.h"

#pragma data_seg (".Res_x86App") 
	static HHOOK		g_hook					= NULL;
	static HWND			g_deskctlwnd			= NULL;
	static HINSTANCE	g_hmod					= NULL;		
	static WNDPROC		lpfDesktopWndProc		= NULL;
	static BOOL			g_inited				= false;
#pragma data_seg ()
#pragma comment(linker, "/section:.Res_x86App,rws")


#define HOVEREFFECT 0


///////////////////////////////////////////////////////////////////////////////
////
//
HINSTANCE GetMyInstanceHandle( ){ return g_hmod; }





///////////////////////////////////////////////////////////////////////////////
////
//
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
			g_hmod = (HINSTANCE)hModule;

		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}






///////////////////////////////////////////////////////////////////////////////
////
//
static void Init()
{	
	if( !g_deskctlwnd )
		g_deskctlwnd	= __GetDesktopListView( );
	
	if( !g_hook )
		g_hook			= (HHOOK)ReadIntSetting( "res_x86","address",0 );

	g_inited = true;
}




///////////////////////////////////////////////////////////////////////////////
////
//
static void TestData( )
{
	if( !g_inited ) Init( );

	if( !IsWindow( g_deskctlwnd ) )
		g_deskctlwnd = __GetDesktopListView( );
}




///////////////////////////////////////////////////////////////////////////////
///// Win9x hook:
//
LRESULT CALLBACK Win9xDesktopProc( int nCode, WPARAM wParam, LPARAM lParam ){

	LPCWPSTRUCT msg = (LPCWPSTRUCT)lParam;

	TestData( );

	if( ( nCode >= 0 ) && ( wParam == NULL ) )
	{	
			 
		switch( msg->message )
		{
						
		case LVM_SETTEXTBKCOLOR:			
			if( msg->hwnd == g_deskctlwnd )
				msg->lParam = ( LPARAM )( COLORREF )CLR_NONE;								
			break;
	 
		case LVM_SETTEXTCOLOR:					
			if( msg->hwnd == g_deskctlwnd )										
				if( wParam == 0 )
				{
					msg->lParam = ListView_GetTextColor( msg->hwnd );
					InvalidateRect( msg->hwnd, 0, false );
					UpdateWindow( msg->hwnd );
				}
				else wParam = 0;

			break;		

		case LVM_SETEXTENDEDLISTVIEWSTYLE:				
			if( msg->hwnd == g_deskctlwnd )					
				switch( ReadIntSetting(  "res_x86", "HoverEffect", HOVEREFFECT ) )
				{
				case 1:
					msg->wParam = (WPARAM) LVS_EX_TRACKSELECT;//Mask
					msg->lParam = (LPARAM) LVS_EX_TRACKSELECT;
					break;
				
				case 2:
					msg->wParam = (WPARAM) LVS_EX_TRACKSELECT
											|LVS_EX_ONECLICKACTIVATE
											|LVS_EX_UNDERLINEHOT;//Mask
					msg->lParam = (LPARAM) LVS_EX_TRACKSELECT
											|LVS_EX_ONECLICKACTIVATE
											|LVS_EX_UNDERLINEHOT;
					break;

				case 3:
					msg->wParam = (WPARAM) LVS_EX_TRACKSELECT
											|LVS_EX_ONECLICKACTIVATE
											|LVS_EX_UNDERLINECOLD;//Mask
					msg->lParam = (LPARAM) LVS_EX_TRACKSELECT
											|LVS_EX_ONECLICKACTIVATE
											|LVS_EX_UNDERLINECOLD;
					break;

				default:
					break;
				}
			break;

		case LVM_SETHOVERTIME:
			if( msg->hwnd == g_deskctlwnd )
			{
				if( ReadIntSetting( "res_x86", "HoverTime", 750 ) > 0 )
				{
					msg->lParam = (LPARAM)(DWORD)
						ReadIntSetting( "res_x86", "HoverTime", 1000 );
				}

				SetWindowPos( __GetDesktopListView( ), 0, 0, 0, 0, 0, 
					SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER );

				InvalidateRect( msg->hwnd, 0, true );
			}
			break;

		
		default:
			break;
		}		
	}
	return CallNextHookEx( g_hook, nCode, wParam, lParam );
}






















///////////////////////////////////////////////////////////////////////////////
//// Win2k Desktop Proc
//
LRESULT CALLBACK DeskWndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	TestData( );

	switch( message )
	{

	case LVM_SETTEXTBKCOLOR:				
		lParam = CLR_NONE;
		break;

 
	case LVM_SETTEXTCOLOR:
		if( wParam == 0 )
		{
			lParam = ListView_GetTextColor( hwnd );
			
			InvalidateRect( hwnd, 0, false );

			SetWindowPos( __GetDesktopListView( ), 0, 0, 0, 0, 0, 
				SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER );

			InvalidateRect( hwnd, 0, false );
			UpdateWindow( hwnd );
		}
		else wParam = 0;

		break;		


	case LVM_SETEXTENDEDLISTVIEWSTYLE:

		switch( ReadIntSetting(  "res_x86", "HoverEffect", HOVEREFFECT ) )
		{
		case 1:
			wParam = (WPARAM) LVS_EX_TRACKSELECT;//Mask
			lParam = (LPARAM) LVS_EX_TRACKSELECT;
			break;
		
		case 2:
			wParam = (WPARAM) LVS_EX_TRACKSELECT
									|LVS_EX_ONECLICKACTIVATE
									|LVS_EX_UNDERLINEHOT;//Mask
			lParam = (LPARAM) LVS_EX_TRACKSELECT
									|LVS_EX_ONECLICKACTIVATE
									|LVS_EX_UNDERLINEHOT;
			break;

		case 3:
			wParam = (WPARAM) LVS_EX_TRACKSELECT
									|LVS_EX_ONECLICKACTIVATE
									|LVS_EX_UNDERLINECOLD;//Mask
			lParam = (LPARAM) LVS_EX_TRACKSELECT
									|LVS_EX_ONECLICKACTIVATE
									|LVS_EX_UNDERLINECOLD;
			break;

		default:
			break;
		}

		SetWindowPos( __GetDesktopListView( ), 0, 0, 0, 0, 0, 
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER );

		InvalidateRect( hwnd, 0, true );
		break;

	case LVM_SETHOVERTIME:
	
		if( ReadIntSetting( "res_x86", "HoverTime", 750 ) > 0 )
		{
			lParam = (LPARAM)(DWORD)
				ReadIntSetting( "res_x86", "HoverTime", 1000 );
		}

		SetWindowPos( __GetDesktopListView( ), 0, 0, 0, 0, 0, 
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER );

		InvalidateRect( hwnd, 0, true );
		break;

	default:
		break;
	}

	return CallWindowProc( lpfDesktopWndProc, hwnd, message, wParam, lParam );
}



void SubclassDesktop( )
{

	lpfDesktopWndProc = (WNDPROC)GetWindowLongPtr( __GetDesktopListView( ), GWLP_WNDPROC );
	if( lpfDesktopWndProc == 0)
		ErrorMessage( GetLastError( ) );
	else
	{
		if( SetWindowLongPtr( __GetDesktopListView( ), GWLP_WNDPROC, (LONG_PTR)(WNDPROC)DeskWndProc ) == 0 )
			ErrorMessage( GetLastError( ) );
		else
		{
			//Success		
			SetWindowPos( __GetDesktopListView( ), 0, 0, 0, 0, 0, 
				SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER );

			SetFocus( __GetDesktopListView( ) );
		}
	}
}


void UnSubclassDesktop( )
{

	if( SetWindowLongPtr( __GetDesktopListView( ), GWLP_WNDPROC, (LONG_PTR)lpfDesktopWndProc ) == 0 )
		ErrorMessage( GetLastError( ) );
	else
	{
		//Success		
		SetWindowPos( __GetDesktopListView( ), 0, 0, 0, 0, 0, 
			SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER );	
		
		SetFocus( __GetDesktopListView( ) );
	}
	
}




LRESULT CALLBACK SubclassMangr( int nCode, WPARAM wParam, LPARAM lParam ){

	LPCWPSTRUCT msg = (LPCWPSTRUCT)lParam;

	TestData( );

	if( g_deskctlwnd == msg->hwnd )
	{
		switch( msg->message )
		{
		case WM_USER+10:
			SubclassDesktop( );
			break;

		case WM_USER+20:
			UnSubclassDesktop( );			
			break;

		default:break;
		}
	}


	return CallNextHookEx( g_hook, nCode, wParam, lParam );
}




RES_x86API DWORD WINAPI StartHookThreadProc( void )
{
	if( ReadIntSetting( "res_x86", "installed", 0 ) )
		return ERROR_ACCESS_DENIED;
	
	Init( );

	if( IsWindowsNT( ) )
	{
		g_hook = SetWindowsHookEx( WH_CALLWNDPROC,
					SubclassMangr, GetMyInstanceHandle( ), 0 );
	}	
	else
	{
		g_hook = SetWindowsHookEx( WH_CALLWNDPROC,
					Win9xDesktopProc, GetMyInstanceHandle( ), 0 );
	}


	if( !g_hook )
		return GetLastError( );
	else
	{
		WriteIntSetting( "res_x86", "address", (DWORD)g_hook );
		WriteIntSetting( "res_x86", "installed", 1 );	

		SendMessage ( __GetDesktopListView( ), WM_USER+10, 0, 0 );
	}

	return 0;
}





RES_x86API DWORD WINAPI StopHookThreadProc( void )
{
	Init( );
	
	SendMessage( __GetDesktopListView( ), WM_USER+20, 0, 0 );



	if( !UnhookWindowsHookEx( g_hook ) )
		return GetLastError( );		
	else
	{
		WriteIntSetting( "res_x86", "address", (DWORD)0 );
		WriteIntSetting( "res_x86", "installed", 0 );
		g_hook = NULL;
	}

	return 0;
}