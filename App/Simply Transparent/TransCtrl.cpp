///////////////////////////////////////////////////////////////////////////////
//// TransCtrl.cpp : implementation file
// 
//
#include "stdafx.h"
#include "SimplyTransparent.h"
#include "TransCtrl.h"
#include "_Button.h"

#include "getwnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
//// 
//
#define _STYLE_MASK		LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE \
						| LVS_EX_UNDERLINEHOT | LVS_EX_UNDERLINECOLD		
#define HoverEffect0	LVS_EX_TRACKSELECT
#define HoverEffectBase	HoverEffect0 |LVS_EX_ONECLICKACTIVATE
#define HoverEffect1	(HoverEffectBase | LVS_EX_UNDERLINEHOT)
#define HoverEffect2	(HoverEffectBase | LVS_EX_UNDERLINECOLD)


///////////////////////////////////////////////////////////////////////////////
//// 
//
static HWND * p_DeskListViewHwnd	= NULL;
static HWND * p_ButtonWnd			= NULL;


///////////////////////////////////////////////////////////////////////////////
//// 
//
static BOOL CALLBACK EnumWindowsProc( HWND hwnd, LPARAM lParam )
{

	BOOL bReturn = true;

	if( IsWindowVisible( hwnd ) && hwnd != GetDesktopWindow( ) 
		&& hwnd != *p_DeskListViewHwnd 
		&& hwnd != *p_ButtonWnd )
	{

		RECT r;
		if( GetWindowRect( hwnd, &r ) != 0 )
		{
			POINT *point = ( POINT * )lParam;
			if( ( point->x >= r.left )&&( point->x <= r.right ) )
			{

				if( ( point->y >= r.top )&&( point->y <= r.bottom ) )
				{
					bReturn = false;					
					char s[MAX_PATH];
					GetWindowText( hwnd, ( char * )&s, MAX_PATH );
					if( strcmp( "Program Manager", s ) != 0 )
						point->x = point->y = -1234;					
				}
			}
		}

	}

	return bReturn;
}




///////////////////////////////////////////////////////////////////////////////
//// 
//
static BOOL CheckPoint( POINT point )
{
	EnumWindows( EnumWindowsProc, ( LPARAM )&point );
	if( ( point.x == -1234 )&&( point.y == -1234 ) )
		return false;
	return true;
}

















///////////////////////////////////////////////////////////////////////////////
//// 
//
CTransCtrl::CTransCtrl( )
{
	hInst_res_x86		= LoadLibraryA( "RES_X86.DLL" );
	

	m_hhook				= NULL;
	p_DeskListViewHwnd	= &m_hDeskListViewWnd;
	p_ButtonWnd			= &m_hButtonWnd;
	m_hButtonWnd		= NULL;

	m_bHooked			= false;

	m_textcolor			= ReadRGBSetting( "res_x86", "textcolor", "255,255,255" );	

	__Comment( "Desktop Control Constructed" );
}




///////////////////////////////////////////////////////////////////////////////
//// 
//
CTransCtrl::~CTransCtrl( )
{ 
	__Comment( "Desktop Control: Button Destroyed" );
	DestroyButton( );

	if( m_bHooked )
		RemoveHook( ); 

	if( hInst_res_x86 )
		FreeLibrary( hInst_res_x86 );
	
	hInst_res_x86	= NULL;
	
	
	__Comment( "Desktop Control Destructed" );
}





























///////////////////////////////////////////////////////////////////////////////
//// 
//
BOOL CTransCtrl::InstallHook( )
{

	int nReturn =  false;

	WriteIntSetting( "res_x86", "installed", 0 );	
	
	
	DWORD (WINAPI *lpfProc)( void );

    if( hInst_res_x86 != NULL )
	{

		 // Get procedure address.
		lpfProc = (DWORD(WINAPI *)(void))
			GetProcAddress( hInst_res_x86, "StartHookThreadProc" );		

		if( lpfProc != NULL )
		{		
			DWORD err = lpfProc( );
			if( !err ){
				nReturn = true;
				m_hhook = (HHOOK)ReadIntSetting( "res_x86", "address", 0 );					
				__Comment( "Hook Installed!" );
			}		
			else
				__Error( err );
			
		}
		else __Comment( "function not found in resource library" );
		
	}
	else __Comment( "resource library not loaded" );

	/////////////////////////////////////////////////

	
	m_bHooked = nReturn;
		
	return nReturn;
}






























///////////////////////////////////////////////////////////////////////////////
//// 
//
void CTransCtrl::RemoveHook( )
{
	DWORD (WINAPI *lpfProc)( void );

	if( hInst_res_x86 != NULL )
	{

		 // Get procedure address.
		lpfProc = (DWORD(WINAPI *)(void))
			GetProcAddress( hInst_res_x86, "StopHookThreadProc" );		

		if( lpfProc != NULL )
		{
			DWORD err = lpfProc( );
			if( !err )
			{
				if( ReadIntSetting( "res_x86", "address", 0 ) != 0 )
					UnhookWindowsHookEx( m_hhook );
			}
			else __Error( err );
			
		}
		else __Comment( "function not found in resource library" );		 
	}
	else __Comment( "resource library not loaded" );

	/////////////////////////////////////////////////

		
	m_hhook = NULL;
	m_bHooked = false;

	__Comment( "Desktop Control: Hook Removal" );
	
}























///////////////////////////////////////////////////////////////////////////////
//// 
//
HWND CTransCtrl::GetDesktopListView( )
{
	HWND ret = NULL;
	
	ret = __GetDesktopListView( );
	
	return ret;
}






















///////////////////////////////////////////////////////////////////////////////
//// 
//
void CTransCtrl::SetTransparent( )
{
	TestDesktop( );
	ShowWindow( m_hDeskListViewWnd, SW_HIDE );
	SendMessage( m_hDeskListViewWnd, LVM_SETTEXTBKCOLOR, 0, CLR_NONE );
	//InvalidateRect( m_hDeskListViewWnd, NULL, true );
	UpdateWindow( m_hDeskListViewWnd );
	//WaitForSingleObject( GetCurrentProcess( ), 100 );	
	// Sleep( 100 );
	ShowWindow( m_hDeskListViewWnd, SW_SHOW );
}



















///////////////////////////////////////////////////////////////////////////////
//// 
//
COLORREF CTransCtrl::GetBGColor( )
{
	TestDesktop( );
	return ( COLORREF )SendMessage( m_hDeskListViewWnd, LVM_GETTEXTBKCOLOR, 0, 0 );
}
























///////////////////////////////////////////////////////////////////////////////
//// 
//
void CTransCtrl::SetTextColor( )
{
	TestDesktop( );

	DWORD dwCustClrs [16];
	for ( int i = 0; i < 16; i++ )		 
		dwCustClrs [i] = _WHITE;

	m_textcolor = ReadRGBSetting( "res_x86", "textcolor", "255,255,255");

	CHOOSECOLOR color;
	ZeroMemory( &color, sizeof( CHOOSECOLOR ) );
	color.lStructSize = sizeof( CHOOSECOLOR );
	color.Flags = CC_RGBINIT|CC_ENABLEHOOK;
	color.rgbResult = m_textcolor;
	color.hwndOwner = GetMyWindowHandle( );
	color.lpCustColors = ( LPDWORD )dwCustClrs;
	color.lCustData = 0L;
	color.lpfnHook = CCHookProc;

	if( ChooseColor( &color ) )
	{
		m_textcolor = color.rgbResult;
		
		WriteRGBSetting( "res_x86", "textcolor", m_textcolor );		
		SendMessage( __GetDesktopListView( ), LVM_SETTEXTCOLOR, 57, m_textcolor );
		InvalidateRect( __GetDesktopListView( ), NULL, false );
		UpdateWindow( __GetDesktopListView( ) );
	}	   
}




















///////////////////////////////////////////////////////////////////////////////
//// 
//
void CTransCtrl::SetTextColorEx2( COLORREF color )
{
	
	TestDesktop( );
	
	WriteRGBSetting( "res_x86", "textcolor", m_textcolor );

	SendMessage( __GetDesktopListView( ), LVM_SETTEXTCOLOR, 57, color );
	InvalidateRect( __GetDesktopListView( ), NULL, false );
	UpdateWindow( __GetDesktopListView( ) );
}



















///////////////////////////////////////////////////////////////////////////////
//// 
//
void CTransCtrl::SetTextColorEx( COLORREF color )
{
	TestDesktop( );
	SendMessage( __GetDesktopListView( ), LVM_SETTEXTCOLOR, 57, color );
	InvalidateRect( __GetDesktopListView( ), NULL, false );
	UpdateWindow( __GetDesktopListView( ) );
}






















///////////////////////////////////////////////////////////////////////////////
//// 
//
COLORREF CTransCtrl::GetTextColor( )
{
	TestDesktop( );
	return ( COLORREF )SendMessage( __GetDesktopListView( ), LVM_GETTEXTCOLOR, 0, 0);
}


















///////////////////////////////////////////////////////////////////////////////
//// 
//
void CTransCtrl::UndoTransparent( COLORREF color )
{
	TestDesktop( );
			
	ShowWindow( m_hDeskListViewWnd, SW_HIDE );
	SendMessage( m_hDeskListViewWnd, LVM_SETTEXTBKCOLOR, 0, color );
	//InvalidateRect( m_hDeskListViewWnd, NULL, true );
	UpdateWindow( m_hDeskListViewWnd );
	//WaitForSingleObject( GetCurrentProcess(), 100 );
		//Sleep( 100 );
	HotTracking( false );
	ShowWindow( m_hDeskListViewWnd, SW_SHOW );
}
























///////////////////////////////////////////////////////////////////////////////
//// 
//
void CTransCtrl::HotTracking( BOOL install, int hovertime )
{
	if( TestDesktop( ) == 0 )
	{
		if( install )
		{
			HotTracking( false );
			switch( ReadIntSetting( "res_x86", "HoverEffect", HOVEREFFECT ) )
			{
			case 1:
				ListView_SetExtendedListViewStyleEx( m_hDeskListViewWnd,
					_STYLE_MASK, HoverEffect0 );
				break;
			
			case 2:
				ListView_SetExtendedListViewStyleEx( m_hDeskListViewWnd,
					_STYLE_MASK, HoverEffect1 );
				break;

			case 3:
				ListView_SetExtendedListViewStyleEx( m_hDeskListViewWnd,
					_STYLE_MASK, HoverEffect2 );
				break;

			default:
				break;		
			}

			if( hovertime > 0 )
			{
				ListView_SetHoverTime( m_hDeskListViewWnd, hovertime );
			}
		}
		else
			ListView_SetExtendedListViewStyleEx( m_hDeskListViewWnd,
				_STYLE_MASK, 0 );				
	}
}






















  
///////////////////////////////////////////////////////////////////////////////
//// 
//
void CTransCtrl::CheckExtStyles( )
{
	if( TestDesktop( ) == 0 )
	{
		DWORD style = SendMessage( m_hDeskListViewWnd,
			LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0 );
		
		int time = SendMessage( m_hDeskListViewWnd,
				LVM_GETHOVERTIME, 0, 0 );


		switch( ReadIntSetting( "res_x86", "HoverEffect", HOVEREFFECT ) )
		{
			case 1:
				if( ( style & HoverEffect0 ) != HoverEffect0 )
					ListView_SetExtendedListViewStyleEx( m_hDeskListViewWnd,
						_STYLE_MASK, HoverEffect0 );
				break;
			
			case 2:
				if( (style & HoverEffect1 ) != HoverEffect1 )
					ListView_SetExtendedListViewStyleEx( m_hDeskListViewWnd,
						_STYLE_MASK, HoverEffect1 );
				break;

			case 3:
				if( (style & HoverEffect2 ) != HoverEffect2 )
					ListView_SetExtendedListViewStyleEx( m_hDeskListViewWnd,
						_STYLE_MASK, HoverEffect2 );
				break;

			default:
				break;		
		}
		
		if( time != ReadIntSetting( "res_x86", "HoverTime", HOVERTIME ) )
			ListView_SetHoverTime( m_hDeskListViewWnd, 
				ReadIntSetting( "res_x86", "HoverTime", HOVERTIME ) );
	}
}

























///////////////////////////////////////////////////////////////////////////////
//// 
//
int CTransCtrl::DoHitTest( int nShowButton )
{
	int test = TestDesktop( );
	if( test != 0 )  return test;

	POINT mouse;	

	GetCursorPos( &mouse );
	if( SendMessage( m_hDeskListViewWnd, WM_NCHITTEST, 0,
		MAKELPARAM( mouse.x, mouse.y) ) != HTNOWHERE )
	{		
		
		if( CheckPoint( mouse ) )
		{	
			if( !IsWindowVisible( m_hDeskListViewWnd ) )
			{
				if( nShowButton )
				{
					ShowButton( true ); 
				}
				else ShowWindow( m_hDeskListViewWnd, SW_SHOWNA );
			}				
		}
		else
		{
			if( nShowButton )
			{
				ShowButton( false );
			}
			if( IsWindowVisible( m_hDeskListViewWnd ) )
				ShowWindow( m_hDeskListViewWnd, SW_HIDE );
		}
	}	 
	else{
		if( nShowButton )
		{
			ShowButton( false );
		}
		if( IsWindowVisible( m_hDeskListViewWnd ) )
		{
				ShowWindow( m_hDeskListViewWnd, SW_HIDE );
		}
	}

	return 0;
}

















///////////////////////////////////////////////////////////////////////////////
//// 
//
void CTransCtrl::ShowIcons( )
{
	if( TestDesktop( ) == 0 )
	{
		ShowWindow( m_hDeskListViewWnd, SW_SHOWNOACTIVATE );
	}
	
	ShowWindow( __GetTrayWnd( ), SW_SHOWNORMAL );	

}





















///////////////////////////////////////////////////////////////////////////////
//// 
//
void CTransCtrl::Hide( )
{
	if( TestDesktop( ) == 0 )
	{
		if( IsWindowVisible( m_hDeskListViewWnd ) )
			ShowWindow( m_hDeskListViewWnd, SW_HIDE );
	}

	ShowWindow( __GetTrayWnd( ), SW_SHOWNORMAL );
}












///////////////////////////////////////////////////////////////////////////////
//// 
//
int CTransCtrl::DoDesktopLock( )
{
	if( TestDesktop( ) < 0 )
		return 0;

	if( GetTextColor() != m_textcolor )
		SetTextColorEx( m_textcolor );
	
	if( GetBGColor() != CLR_NONE )
		SetTransparent();
	
	CheckExtStyles();

	return 1;
}


















///////////////////////////////////////////////////////////////////////////////
//// 
//
int CTransCtrl::TestDesktop( )
{
	
	HWND temp_DeskListViewHwnd = GetDesktopListView();
	
	temp_DeskListViewHwnd = GetDesktopListView( );
	

	if( !temp_DeskListViewHwnd )
	{

		if( !IsWindowVisible( m_hDeskListViewWnd ) )
			ShowWindow( m_hDeskListViewWnd, SW_SHOWNA );
		
		return 1;
	}

	m_hDeskListViewWnd = temp_DeskListViewHwnd;	
	
	return 0;
}


















void CTransCtrl::CreateButton()
{
	if( m_hButtonWnd )
		return;

	DLGTEMPLATE *dt = (DLGTEMPLATE*)GlobalAlloc( GMEM_ZEROINIT|GMEM_FIXED, sizeof(DLGTEMPLATE) );
	dt->x = 1;
	dt->y = 1;
	dt->cx = 10;
	dt->cy = 9;
	dt->style = DS_ABSALIGN|WS_POPUP;
	dt->dwExtendedStyle = WS_EX_TOOLWINDOW|WS_EX_TOPMOST;
	dt->cdit = 0;
	
	__Comment( "Creating Button" );
	m_hButtonWnd = CreateDialogIndirect( GetMyInstanceHandle(),
		dt, GetMyWindowHandle( ), ButtonWndProc );
			
	GlobalFree( (HGLOBAL)dt );
}














void CTransCtrl::DestroyButton()
{
	if( !m_hButtonWnd )
		return;

	__Comment( "Removing Button" );
	DestroyWindow( m_hButtonWnd );
	m_hButtonWnd = NULL;
}










void CTransCtrl::ShowButton( BOOL bShow )
{
	if( !m_hButtonWnd )
		return;

	if( bShow )
	{		
		if( !__AnimateWindow( m_hButtonWnd, 200, AW_ACTIVATE|AW_BLEND ) )
			ShowWindow( m_hButtonWnd, SW_SHOWNOACTIVATE );
		else
			InvalidateRect( m_hButtonWnd, 0, 1 );
		
	}
	else
	{
		ShowWindow( m_hButtonWnd, SW_HIDE );
	}	
}


