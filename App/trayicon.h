////////////////////////////////////////////////////////////////
//// CTrayIcon
//
	
#ifndef _TRAYICON_H
#define _TRAYICON_H

#include "resource.h"
#include <shellapi.h>
	
class CTrayIcon
{
protected:
	HMENU m_hMenu;
	NOTIFYICONDATA m_nid;
	UINT m_nDefMenuItem;
	HINSTANCE m_hinst;

public:	
	void CTrayIcon::InitTrayIcon( HINSTANCE hinst, UINT uID = ID_TRAY );
	CTrayIcon( );
	~CTrayIcon( );
	void SetDefault( UINT nItem );
	void CTrayIcon::CheckMenuItem( UINT nItem, BOOL bChecked );
	void SetNotificationWnd( HWND hNotifyWnd, UINT uCbMsg );
	BOOL SetIcon( UINT uID );
	BOOL SetIcon( HICON hicon, LPCSTR lpTip );
	BOOL SetIcon( LPCTSTR lpResName, LPCSTR lpTip )
		{ return SetIcon(lpResName ? 
			LoadIcon( m_hinst, lpResName ) : NULL, lpTip ); }		
	BOOL SetBitmaps( UINT nItem, HBITMAP bmUnChecked, HBITMAP bmChecked );
	BOOL SetBitmaps( UINT nItem, UINT nUnChecked, UINT nChecked );

	virtual LRESULT OnTrayNotification( WPARAM uID, LPARAM lEvent );	
};

#endif
