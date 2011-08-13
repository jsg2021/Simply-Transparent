///////////////////////////////////////////////////////////////////////////////
///// main.h
///

#ifndef ____MAIN_H__________
#define ____MAIN_H__________

#include "trayicon.h"
#include "TransCtrl.h"


class CMain
{
public:
	enum Lock { lock, unlock, nutral };

	CMain( HINSTANCE hInstance = NULL, char * sCmdLine = NULL );
	~CMain();	

	BOOL FirstInstance( );
	BOOL InitInstance( );	
	BOOL CheckCmdLine( string flag );


	ATOM MyRegisterClass(HINSTANCE hInstance);
	
	int Run ( );
	
	void LoadSettings( );
	void SetOptions( );
	void Restart( string flag );

	HINSTANCE	GetInstanceHandle( )	{ return m_hInst; }
	HWND		GetWindowHandle( )		{ return m_hWnd; }
	HWND		GetDLCWnd( )			{ return m_icons->GetWnd( ); }
	string		GetName( )				{ return m_sTitle; }

	BOOL		IsRes_Loaded( )			{ return m_bRes_loaded; }
	BOOL		IsInterfaceLocked( )	{ return m_bInterfaceLock; }
	void		LockInterface( Lock l = nutral )
	{
		switch( l )
		{
		case nutral:
			m_bInterfaceLock = !m_bInterfaceLock;
			break;
		case lock:
			m_bInterfaceLock = true;
			break;
		case unlock:
			m_bInterfaceLock = false;
			break;
		default:break;
		}
	}

	
	

///////////////////////////////////////////////////////////////////////////////
//// External Comm event handlers
//
	LRESULT OnRestart( WPARAM AccessCode, LPARAM Color );
	LRESULT OnRemoteSetTextColor( WPARAM AccessCode, LPARAM Color );
	LRESULT OnBeginExitNotification( WPARAM uID, LPARAM lEvent );
	LRESULT OnExitNotification( WPARAM uID, LPARAM lEvent );
	LRESULT OnTrayNotification( WPARAM uID, LPARAM lEvent );
	LRESULT OnShowTrayNotification( WPARAM wp, LPARAM lp );
	LRESULT OnQueryEndSession( );

///////////////////////////////////////////////////////////////////////////////
//// Misc. Event Handlers
//
	void OnTimer( DWORD event );

///////////////////////////////////////////////////////////////////////////////
//// Menu Event Handlers
//
	void OnAbout( );
	void OnSettextcolor( );
	void OnOptions( );
	void OnHelp( );
	void OnUpdateCheck( );
	void OnShowSplash( );
	void OnStartup( );
	void OnBeginnerMode( );
	int OnExit( );

///////////////////////////////////////////////////////////////////////////////
////  Functions
//
	void ShowhideIcons( );
	void RestoreOriginalColors( );




protected:
	BOOL		m_updating;
	BOOL		m_bInterfaceLock;	

	BOOL		m_ihevent;
	BOOL		m_lockevent;
	BOOL		m_bFirstInstance;
	BOOL		m_bRunning;
	BOOL		m_bShutdownReady;
	BOOL		m_bRes_loaded;
	char		m_sCmdLine[MAX_PATH];
	char		m_sClass[MAX_PATH];
	char		m_sApppath[MAX_PATH];
	char		m_sTitle[MAX_LOADSTRING];
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	CTrayIcon*	m_tray;
	CTransCtrl*	m_icons;

};


#endif //____MAIN_H__________