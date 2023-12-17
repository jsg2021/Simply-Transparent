////////////////////////////////////////////////////////////////
// CTrayIcon 

#include "stdafx.h"
#include "trayicon.h"




///////////////////////////////////////////////////////////////////////////////
//// Construct
//
CTrayIcon::CTrayIcon()
{

}



///////////////////////////////////////////////////////////////////////////////
//// Destruct
//
CTrayIcon::~CTrayIcon( )
{
	UINT count = GetMenuItemCount( m_hMenu );
	for( UINT x = 0; x>count; x++ )
	{
		MENUITEMINFO info;
		GetMenuItemInfo( m_hMenu, x, true, &info );
		if(info.hbmpChecked != NULL)
			DeleteObject(info.hbmpChecked);
		if(info.hbmpUnchecked != NULL)
			DeleteObject(info.hbmpUnchecked);
	}

	SetIcon( 0 ); // remove icon from system tray
}




///////////////////////////////////////////////////////////////////////////////
//// Init
//
void CTrayIcon::InitTrayIcon( HINSTANCE hinst, UINT uID )
{
		// Initialize NOTIFYICONDATA
	memset( &m_nid, 0 , sizeof( m_nid ) );
	m_nid.cbSize = sizeof( m_nid );
	m_nid.uID = uID;// never changes after construction

	ASSERT( hinst != NULL );
	m_hinst = hinst;

	// Use resource string as tip if there is one
	LoadString( m_hinst, uID, m_nid.szTip, sizeof( m_nid.szTip ) );

	m_nDefMenuItem = 0;	
}



///////////////////////////////////////////////////////////////////////////////
//// Set notification window
//
void CTrayIcon::SetNotificationWnd(HWND hNotifyWnd, UINT uCbMsg)
{
	// If the following assert fails, you're probably
	// calling me before you created your window. Oops.
	ASSERT( hNotifyWnd==NULL || ::IsWindow( hNotifyWnd ) );
	m_nid.hWnd = hNotifyWnd;

	ASSERT( uCbMsg==0 || uCbMsg>=WM_USER );
	m_nid.uCallbackMessage = uCbMsg;

	m_hMenu = LoadMenu( m_hinst, MAKEINTRESOURCE( m_nid.uID ) );
	ASSERT( m_hMenu );
	if ( !m_hMenu )
		__Message( "Menu Faild to load" );

	 
}





///////////////////////////////////////////////////////////////////////////////
//// Main Method for adding icon
//
BOOL CTrayIcon::SetIcon(UINT uID)
{ 
	HICON hicon = NULL;
	if ( uID )
	{
		LoadString( m_hinst, uID, m_nid.szTip, sizeof( m_nid.szTip ) );
		hicon = LoadIcon( m_hinst, MAKEINTRESOURCE( uID ) );
	}
	return SetIcon( hicon, NULL );
}





///////////////////////////////////////////////////////////////////////////////
//// Base fucntion for all overloads
//
BOOL CTrayIcon::SetIcon(HICON hicon, LPCSTR lpTip) 
{
	UINT msg;
	m_nid.uFlags = 0;

	// do icon
	if ( hicon )
	{
		if( !m_nid.hIcon )
			__Comment( "Initializing Tray-icon..." );

		// Add or replace icon in system tray
		msg = m_nid.hIcon ? NIM_MODIFY : NIM_ADD;
		m_nid.hIcon = hicon;
		m_nid.uFlags |= NIF_ICON;
	}
	else
	{ // remove icon from tray
		__Comment( "Removing Tray icon..." );
		if ( m_nid.hIcon==NULL )
			return TRUE;		// already deleted
		msg = NIM_DELETE;
	}

	// Use the tip
	if ( lpTip )
		strncpy( m_nid.szTip, lpTip, sizeof( m_nid.szTip ) );
	if ( m_nid.szTip[0] )
		m_nid.uFlags |= NIF_TIP;

	// Use callback
	if ( m_nid.uCallbackMessage && m_nid.hWnd )
		m_nid.uFlags |= NIF_MESSAGE;

	// Go!
	BOOL bRet = Shell_NotifyIcon( msg, &m_nid );
	if ( msg==NIM_DELETE || !bRet )
	{
		DestroyIcon( m_nid.hIcon );
		m_nid.hIcon = NULL;	// failed
	}
	
	return bRet;
}





///////////////////////////////////////////////////////////////////////////////
//// Event Handle
//
LRESULT CTrayIcon::OnTrayNotification( WPARAM wID, LPARAM lEvent )
{
	if ( wID!=m_nid.uID ||
		( lEvent!=WM_RBUTTONUP && lEvent!=WM_LBUTTONDBLCLK ) )
		return 0;

	HMENU pSubMenu = GetSubMenu( m_hMenu , 0 );	
	ASSERT( pSubMenu );
	if ( !pSubMenu ) 
		return 0;

	if( m_nDefMenuItem == 0 )
	{
		m_nDefMenuItem = GetMenuItemID( m_hMenu, 0 );
	}

	if ( lEvent==WM_RBUTTONUP )
	{

		// Make 'm_nDefMenuItem' menu item the default (bold font)
		ASSERT( SetMenuDefaultItem( pSubMenu, m_nDefMenuItem, FALSE ) != 0 );


		POINT mouse;
		GetCursorPos( &mouse );
		SetForegroundWindow( m_nid.hWnd );	
		TrackPopupMenu( pSubMenu, 0, mouse.x, mouse.y, 0, m_nid.hWnd, NULL );

	}
	else  // execute default menu item
		SendMessage( m_nid.hWnd, WM_COMMAND, m_nDefMenuItem, 0 );

	return 1; // handled
}





///////////////////////////////////////////////////////////////////////////////
//// Adds or removes a checkmark next to the menu item
//
void CTrayIcon::CheckMenuItem( UINT nItem, BOOL bChecked )
{
	::CheckMenuItem( m_hMenu, nItem, bChecked ? MF_CHECKED : MF_UNCHECKED );
}





///////////////////////////////////////////////////////////////////////////////
//// Adds bitmaps next to the menu item
//
BOOL CTrayIcon::SetBitmaps( UINT nItem, HBITMAP bmUnChecked, HBITMAP bmChecked )
{
	return SetMenuItemBitmaps( m_hMenu, nItem, MF_BYCOMMAND, bmUnChecked, bmChecked );
}





///////////////////////////////////////////////////////////////////////////////
//// Toggles between the active state and disabled state bitmaps
//
BOOL CTrayIcon::SetBitmaps(UINT nItem, UINT nUnChecked, UINT nChecked){

	return SetBitmaps( nItem,
		LoadBitmap( m_hinst, MAKEINTRESOURCE( nUnChecked ) ),
		LoadBitmap( m_hinst, MAKEINTRESOURCE( nChecked ) ) );

}





///////////////////////////////////////////////////////////////////////////////
//// Set the defualt action on the menu (also bolds the item)
//
void CTrayIcon::SetDefault( UINT nItem )
{
	m_nDefMenuItem = nItem;	
}



