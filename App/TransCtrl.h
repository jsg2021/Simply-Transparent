#if !defined(AFX_TRANSCTRL_H__F0476EFB_2791_4112_92CF_D046E996934C__INCLUDED_)
#define AFX_TRANSCTRL_H__F0476EFB_2791_4112_92CF_D046E996934C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransCtrl.h : header file
//


class CTransCtrl{
// Construction
public:
	int DoDesktopLock( );
	BOOL InstallHook( );
	void RemoveHook( );
	void ShowButton( BOOL bShow );
	void DestroyButton( );
	void CreateButton( );
	void CheckExtStyles( );
	void Hide( );
	void ShowIcons( );	
	void HotTracking( BOOL install = true, int hovertime = 0 );
	void UndoTransparent( COLORREF color );
	void SetTextColor( );
	void SetTextColorEx( COLORREF color );
	void SetTextColorEx2( COLORREF color );
	void SetTransparent( );	
	int TestDesktop( );
	int DoHitTest( int nShowButton );	
	COLORREF GetBGColor( );
	COLORREF GetTextColor( );	
	HWND GetDesktopListView( );
	HWND GetWnd( ) { return m_hDeskListViewWnd; }

	BOOL IsRes_x86Loaded(){ return ( hInst_res_x86 != NULL ); }
	
	CTransCtrl( );
	virtual ~CTransCtrl( );
protected:
	BOOL		m_bHooked;
	COLORREF	m_textcolor;
	HWND		m_hDeskListViewWnd;
	HWND		m_hButtonWnd;
	HHOOK		m_hhook;

private:
	HINSTANCE hInst_res_x86;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSCTRL_H__F0476EFB_2791_4112_92CF_D046E996934C__INCLUDED_)
