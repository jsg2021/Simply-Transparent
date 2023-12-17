///////////////////////////////////////////////////////////////////////////////
//// main.cpp : Defines the Simply Transparent class
//
#include "stdafx.h"
#include "SimplyTransparent.h"
#include "main.h"


CMain::CMain( HINSTANCE hInstance, char * sCmdLine )
{
	m_updating			= false;
	m_bShutdownReady	= false;
	m_bRunning			= false;
	m_bRes_loaded		= false;

///////////////////////////////////////////////////////////////////////////////
	__Comment( "Creating Desktop Control Object" );
	m_icons = new CTransCtrl( );

	if( !m_icons->IsRes_x86Loaded( ) )
	{
		

		__Comment( "===[ res_x86 library failed to load ]===" );


		//disable advances method
		WriteIntSetting( "", "Lockmeth", 0 );

		__Comment( "---{ Falling back to Brute Force method }---" );
	}
	else m_bRes_loaded = true;
///////////////////////////////////////////////////////////////////////////////
	
	m_tray = new CTrayIcon( );

	LockInterface( unlock );
	
	char string[MAX_PATH];

	m_bFirstInstance	= false;
	m_ihevent			= false;
	m_lockevent			= false;
	
	ASSERT( hInstance != NULL );
	m_hInst = hInstance;

	LoadString(hInstance, IDS_APP_TITLE, m_sTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDS_APP_CLASS, m_sClass, MAX_PATH);
		
	if( sCmdLine )
		sprintf( m_sCmdLine, "%s", sCmdLine );
	else memset( m_sCmdLine , 0, MAX_PATH );
	___makelower( m_sCmdLine );

	GetModuleFileName( hInstance, m_sApppath, MAX_PATH );
	___makelower( m_sApppath );
	sprintf( string, "Path: %s", m_sApppath );
	__Comment( string );

	
	___makelower( m_sClass );	
	sprintf( string, "Window Class: %s", m_sClass );
	__Comment( string );


	SetCurrentDirectory( ___left( m_sApppath,
			___reversefind( m_sApppath, '\\') ).c_str( ) );
		
	__Comment ( "CMain constructed" );
}





CMain::~CMain( )
{
	__Comment ( "Stopping timers" );
	KillTimer( m_hWnd, IDC_METHOD );
	KillTimer( m_hWnd, IDC_ICON_HIDE );
	KillTimer( m_hWnd, ID_TRAY );
	KillTimer( m_hWnd, ID_ADPOLL );


	m_icons->DestroyButton();
	WriteIntSetting( "res_x86", "IHvis", 1 );
		

	if( m_bFirstInstance )
	{
		m_icons->RemoveHook( );	

		__Comment( "Restoring default colors..." );
		RestoreOriginalColors( );
	


		__Comment( "Window unregistered..." );
		UnregisterClass( m_sClass, m_hInst );
	
	}

	delete m_icons;
	delete m_tray;

	__Comment ( "CMain destructed" );
	
}
///////////////////////////////////////////////////////////////////////////




void CMain::Restart( string flag )
{
	char flags[MAX_STRING];
	sprintf( flags, "/nosplash /restart %s", flag.c_str( ) );
	ShellExecute( NULL, "open", m_sApppath, flags, 0, SW_SHOW);

	DestroyWindow( m_hWnd );

}




BOOL CMain::CheckCmdLine( string flag )
{
	BOOL ret = false;
	char cmdl[MAX_STRING];
	char *buf;

	sprintf( cmdl, "%s", m_sCmdLine );
	
	buf = strtok( cmdl, " /" );
	while( buf )
	{
		if( strcmp( flag.c_str( ), buf ) == 0 )
			ret = true;
		
		buf = strtok( 0, " /" );			
	}

	return ret;
}


BOOL CMain::FirstInstance( )
{
	BOOL ret = true;

	HWND win = FindWindow( m_sClass, m_sTitle );
	if(win != NULL)
	{
		__Comment( "program already running..." );
		ret = false;
	}

	m_bFirstInstance = ret;

	return ret;
}




ATOM CMain::MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance,
							(LPCTSTR)IDI_SIMPLYTRANSPARENT);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_SIMPLYTRANSPARENT;
	wcex.lpszClassName	= m_sClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)ID_TRAY);

	return RegisterClassEx(&wcex);
}




int CMain::Run ( )
{
	__Comment ( "Entered main loop" );
	
	m_bRunning = true;

	if( CheckCmdLine( "options" ) )
		PostMessage( m_hWnd, WM_SHOW_OPTIONS,	577, (LPARAM)m_hWnd );
	else if( CheckCmdLine( "textcolor" ) )
		PostMessage( m_hWnd, WM_SHOW_TEXTCOLOR,	577, (LPARAM)m_hWnd );
	else if( CheckCmdLine( "about" ) )
		PostMessage( m_hWnd, WM_SHOW_ABOUT,	577, (LPARAM)m_hWnd );
	else if( CheckCmdLine( "update" ) )
		PostMessage( m_hWnd, WM_GET_UPDATES,	577, (LPARAM)m_hWnd );

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
	}

	__Comment ( "Exited main loop" );
	
	return msg.wParam;
}




BOOL CMain::InitInstance( )
{

///////////////////////////////////////////////////////////////////////////////

	if( !FirstInstance( ) )
	{
		if( !CheckCmdLine( "restart" ) )
			return false;
		else{
			do{
				for( int x=0; ( !FirstInstance( ) && x<30 ); x++ )
					Sleep( 1500 );

				if( __Message( IDS_WAITING, MB_YESNO ) != IDYES )
					break;
			}
			while( !FirstInstance( ) );

			if(!FirstInstance( ))return false;
		}
		
	}



///////////////////////////////////////////////////////////////////////////////


	if( ReadIntSetting( "", "ADpoll", ACTIVEDESKTOP_POLLING ) )
		if( __ADenabled( true ) )
			return false;


///////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////
	if( MyRegisterClass( m_hInst ) == 0 )
		__Error( "Could not register class" );

	RECT r_screen;
	GetWindowRect( GetDesktopWindow(), &r_screen);

	//create the window (hidden of course) to the same size of the desktop
	//so we always get messages
	m_hWnd = CreateWindow(m_sClass, m_sTitle, WS_CAPTION|WS_SYSMENU,
		0, 0, r_screen.right, r_screen.bottom, NULL, NULL, m_hInst, NULL);
	
	char wnd[30];
	sprintf( wnd, "Window Handle: 0x%X", m_hWnd );
	__Comment( wnd );

	if (!m_hWnd)			
		return false;
	

	m_icons->SetTransparent( );
	m_icons->SetTextColorEx( ReadRGBSetting( "res_x86", "textcolor", "255,255,255" ) );
	
	
	WriteIntSetting( "res_x86", "deftxtclr",
		( m_icons->GetTextColor() == _BLACK ) ? 0 : 1 );

	
	//create tray icon...and show it(unless user prefers not to)
	m_tray->InitTrayIcon( m_hInst, ID_TRAY );

	m_tray->SetNotificationWnd( m_hWnd, WM_MY_TRAY_NOTIFICATION );
	
	if( ReadIntSetting( "", "TrayIcon", TRAYICON ) != 0 )
		m_tray->SetIcon( ID_TRAY );

	m_tray->SetDefault( IDM_ABOUT );
	

	UpdateWindow(m_hWnd);


	__Comment( "Setting IDLE priority..." );
	if( !SetPriorityClass( GetCurrentProcess( ), IDLE_PRIORITY_CLASS ) )
		__Error( GetLastError( ) );
	
	__Comment ( "InitInstance Success..." );
   return TRUE;
}




void CMain::LoadSettings( )
{
	__Comment( "Loading Application Settings" );

	m_tray->CheckMenuItem( ID_STARTUP, 
		__DoesStartWithWindows( m_sApppath, m_sTitle, m_hWnd ) );

	m_tray->CheckMenuItem( ID_BEGINNERMODE, 
		ReadIntSetting( "", "NewUser", NEWUSER ) );

	m_tray->CheckMenuItem( ID_SHOWSPLASH, 
		ReadIntSetting( "", "Splash", SPLASH ) );
	
	
	//////////////////////////////////////////////////////////////////////
	
	if( ReadIntSetting( "", "Lockmeth", LOCKMETHOD ) == 1 )
	{
		__Comment ( "Installing Hook" );		   
		
		if( !m_icons->InstallHook( ) )
		{

			__Comment( "===[ Hook Failed! ]===" );


			//Hook failed...switch modes
			WriteIntSetting( "", "Lockmeth", 0 );

			//brute force! :P
			SetTimer( m_hWnd, IDC_METHOD, 
				ReadIntSetting( "", "LockMeth1Time", LOCKMETHODTIME ), NULL );

				__Comment( "--{ falling back to Brute Force mode }---" );
		}

	}
	else
		SetTimer( m_hWnd, IDC_METHOD, 
			ReadIntSetting( "", "LockMeth1Time", LOCKMETHODTIME ), NULL );

	SetOptions( );

}



void CMain::SetOptions( )
{
	KillTimer( m_hWnd, IDC_ICON_HIDE );
	KillTimer( m_hWnd, ID_ADPOLL );
	KillTimer( m_hWnd, ID_TRAY );

	m_icons->HotTracking(
		ReadIntSetting( "res_x86", "HoverEffect", HOVEREFFECT ),
		ReadIntSetting( "res_x86", "HoverTime", HOVERTIME ) );

	if( ReadIntSetting( "res_x86","IHmethod", ICONHIDE_METHOD ) > 0 )
		SetTimer( m_hWnd, IDC_ICON_HIDE,
			ReadIntSetting( "res_x86", "IHRespon", ICONHIDE_RESPON ), NULL );
	else
	{
		WriteIntSetting( "res_x86", "IHvis", 1 );		
		m_icons->ShowIcons( );
	}
	
	if( ReadIntSetting( "", "ADpoll", ACTIVEDESKTOP_POLLING ) )
		SetTimer( m_hWnd, ID_ADPOLL, 5000, NULL );

	m_icons->SetTextColorEx( ReadRGBSetting( "res_x86", "textcolor", "255,255,255" ) );

	///////////////////////////////////////////////////////////////////////
	//// Enable - Disable options
	//

	if( ReadIntSetting( "", "TrayIcon", TRAYICON ) != 0 )
	{
		m_tray->SetIcon( ID_TRAY );
		SetTimer( m_hWnd, ID_TRAY, 5000, NULL );
	}
	else
	{
		m_tray->SetIcon( NULL );

		if( m_bRunning && ReadIntSetting( "", "TrayIcon-Changed", 1 ) )
			__Message( "You have choosen to hide the tray icon\nTo get the icon back, rerun Simply Transparent" );
	}

	if( ReadIntSetting( "", "Logging", LOGGING ) )
	{
		if ( !_CheckLogger( ) )
			_InitLogger( );
	}
	else
		if ( _CheckLogger( ) )
			_CloseLogger( );


	if( ReadIntSetting( "res_x86", "IHButton", ICONHIDE_BUTTON ) &&
		ReadIntSetting( "res_x86","IHmethod", ICONHIDE_METHOD ) == 1 )
		m_icons->CreateButton( );
	else
		m_icons->DestroyButton( );
}



///////////////////////////////////////////////////////////////////////////////
//// External Comm event handlers
//

LRESULT CMain::OnRestart(WPARAM AccessCode, LPARAM Color){
	LRESULT lRetrun = 0;		
		
		Restart("/winkeyd");	

	return lRetrun;
}





LRESULT CMain::OnRemoteSetTextColor(WPARAM AccessCode, LPARAM Color){
	LRESULT lRetrun = 0;

	if(AccessCode == 0){
		m_icons->SetTextColorEx2((COLORREF)Color);
		lRetrun = Color;
	}
	
	return lRetrun;
}





LRESULT CMain::OnBeginExitNotification(WPARAM uID, LPARAM lEvent){
	LRESULT lRetrun = 0;

	if((uID == 321)&(lEvent == 147)){
		lRetrun = 123;			
	}
	return lRetrun;
}




LRESULT CMain::OnExitNotification(WPARAM uID, LPARAM lEvent){
	
	LRESULT lRetrun = 0;

	if((uID == 321)&(lEvent == 147)){
		DestroyWindow( m_hWnd );
	}

	return lRetrun;
}




LRESULT CMain::OnTrayNotification(WPARAM uID, LPARAM lEvent)
{
	if( ( ReadIntSetting( "res_x86","IHmethod", ICONHIDE_METHOD ) == 2 )
		&& (
		   lEvent == WM_LBUTTONDBLCLK
		|| lEvent == WM_MBUTTONDBLCLK
		|| lEvent == WM_MBUTTONUP
		|| lEvent == WM_LBUTTONUP
		) )
		{		
			ShowhideIcons();
			return 0;
	}
	
	if( IsInterfaceLocked( ) )return 0;		

	return m_tray->OnTrayNotification(uID, lEvent);
}




LRESULT CMain::OnShowTrayNotification( WPARAM wp, LPARAM lp )
{
	
	if( wp == 577 && lp == (LONG)m_hWnd )
	{
		WriteIntSetting( "", "TrayIcon", TRAYICON );
		SetOptions( );
	}

	
	return 0;
}

	

///////////////////////////////////////////////////////////////////////////////
//// Misc. Event Handlers
//
void CMain::OnTimer( DWORD event )
{
	int n;

	switch( event)
	{
	case IDC_METHOD:

		// prevent re-entry
		if( m_lockevent )
			break;

		m_lockevent = true;
		

		if( !m_icons->DoDesktopLock( ) )
			KillTimer( m_hWnd, IDC_METHOD );
					

		m_lockevent = false;

		break;
	
	case IDC_ICON_HIDE:
		
		//Prevent re-entry
		if( m_ihevent )
			break;

		m_ihevent = true;

		n = ReadIntSetting( "res_x86","IHmethod", ICONHIDE_METHOD );
		
		if( n == 1 )
		{
			n = ReadIntSetting( "res_x86", "IHButton", ICONHIDE_BUTTON );
			n = m_icons->DoHitTest( n );
			if( n == 1 )
				m_icons->ShowIcons( );
			else if( n == -1 )
				KillTimer( m_hWnd, IDC_ICON_HIDE );
		}
		else KillTimer( m_hWnd, IDC_ICON_HIDE );

		m_ihevent = false;
		
		break;

	//Keep the tray icon accessable.
	case ID_TRAY: 
		if( ReadIntSetting( "", "TrayIcon", TRAYICON ) != 0 )
			m_tray->SetIcon( ID_TRAY );
		break;

	case ID_ADPOLL:
		//check to see if Active Desktop is enabled.
		if( __ADenabled( false ) )
		{	
			__Comment( "Active Desktop's Web View was enabled!...exiting" );

			KillTimer( m_hWnd, ID_ADPOLL );


			char mes[MAX_STRING];
			LoadString( m_hInst, IDS_ERROR_ACTIVE_DESKTOP_ENABLED,
				mes, MAX_STRING );
			__Message( mes );
			DestroyWindow( m_hWnd );
			
		}			
		break;
	}

}




///////////////////////////////////////////////////////////////////////////////
//// Menu Event Handlers
//

void CMain::OnAbout( )
{
	
	LockInterface( lock );
	
	__Comment ( "Opening AboutBox..." );
	DialogBox( m_hInst, (LPCTSTR)IDD_ABOUTBOX,
				m_hWnd, (DLGPROC)About );

	
	LockInterface( unlock );
}






void CMain::OnSettextcolor( )
{
	
	LockInterface( lock );
	
	m_icons->SetTextColor( ); 
	
	LockInterface( unlock );
}




void CMain::OnOptions( )
{	
	
	LockInterface( lock );

	int now, prev = ReadIntSetting( "", "Lockmeth", LOCKMETHOD );
	int ti_now, ti_prev = ReadIntSetting( "", "TrayIcon", TRAYICON );

	Options( m_hWnd );

	now = ReadIntSetting( "", "Lockmeth", LOCKMETHOD );
	ti_now = ReadIntSetting( "", "TrayIcon", TRAYICON );

	if(ti_now != ti_prev)
		WriteIntSetting( "", "TrayIcon-Changed", 1 );
	else
		WriteIntSetting( "", "TrayIcon-Changed", 0 );

	SetOptions( );



	if( now != prev )
	{
		Restart( "/changedlock" );
	}

	LockInterface( unlock );
}




void CMain::OnHelp( )
{
	char file[MAX_PATH];

	sprintf( file, "%s.chm",
		___left( m_sApppath, ___reversefind( m_sApppath, '.' ) ).c_str( ) );

	string s = "Help File: ";
	s += file;

	__Comment( s.c_str( ) );

	ShellExecute( this->m_hWnd, "open", "hh.exe", file, 0, SW_SHOW );
}




void CMain::OnUpdateCheck( )
{
	if(!m_updating)
	{
		m_updating = true;

		UpdateCheck( 7, 1, false );

		m_updating = false;
	}
	else
	{
		LockInterface( lock );
		__Message( "Simply Transparent is already checking for an update" );
		LockInterface( unlock );
	}
}




void CMain::OnShowSplash( )
{
	BOOL spl = ReadIntSetting( "", "Splash", SPLASH );
	
	WriteIntSetting( "", "Splash", !spl );
	
	m_tray->CheckMenuItem( ID_SHOWSPLASH, !spl );
}




void CMain::OnStartup( )
{
	
	LockInterface( lock );

	char warn[MAX_STRING];
	LoadString( m_hInst, IDS_STARTUPW, warn, MAX_STRING);

	BOOL bContinue = true,
		bStart = __DoesStartWithWindows( m_sApppath, m_sTitle, m_hWnd );

	if( ReadIntSetting( "", "NewUser", NEWUSER ) )
	{
		if( bStart )
			bContinue = 
				( __Message( warn, MB_YESNO ) == IDYES )
				? true : false;
	}

	if(bContinue)
	{
		if( !__StartWithWindows( !bStart, m_sApppath, m_sTitle, m_hWnd ) )
		{
			sprintf( warn, "Unable to make %s Start with Windows.",
				m_sTitle );
			__Message( warn );
		}
		else
			bStart = !bStart;

		m_tray->CheckMenuItem( ID_STARTUP, bStart );
	}

	LockInterface( unlock );
}




void CMain::OnBeginnerMode( )
{	
	
	LockInterface( lock );

	BOOL begin = ReadIntSetting( "", "NewUser", NEWUSER ),
		bContinue = true;
	
	char warn[MAX_STRING];
	LoadString( m_hInst, IDS_NEWUSERW, warn, MAX_STRING);
	
	if( begin )
		bContinue = 
			( __Message( warn, MB_YESNO ) == IDYES )
			? true : false;		

	if( bContinue ){
		begin = !begin;
		WriteIntSetting( "", "NewUser", begin );
	}

	m_tray->CheckMenuItem( ID_BEGINNERMODE, begin );

	LockInterface( unlock );
}





int CMain::OnExit( )
{
	LockInterface( lock );

	__Comment ( "User Selected to Exit..." );

	BOOL begin = ReadIntSetting( "", "NewUser", NEWUSER ),
		bContinue = true;
	
	char warn[MAX_STRING];
	LoadString( m_hInst, IDS_EXITW, warn, MAX_STRING);
	
	if( begin )
		bContinue = 
			( __Message( warn, MB_YESNO ) == IDYES )
			? true : false;		

	
	LockInterface( unlock );

	if( bContinue )
		return 1;

	__Comment ( "User aborted the Exit..." );
	
	return 0;
}






LRESULT CMain::OnQueryEndSession( )
{

	if( !m_bShutdownReady )
	{
		__Comment( "Windows is Shutting down...MUST exit now!" );

		KillTimer( m_hWnd, IDC_METHOD );
		KillTimer( m_hWnd, IDC_ICON_HIDE );
		KillTimer( m_hWnd, ID_TRAY );
		KillTimer( m_hWnd, ID_ADPOLL );

		m_icons->RemoveHook( );

		m_bShutdownReady = true;

		DestroyWindow( m_hWnd );

		
	//	return 0;
	}
	
	return 1;
}




///////////////////////////////////////////////////////////////////////////////
////  Functions
//
void CMain::ShowhideIcons( ) 
{
	BOOL bShow = ReadIntSetting( "res_x86", "IHvis", 1);
	WriteIntSetting( "res_x86", "IHvis", !bShow );

	if( bShow )
		m_icons->ShowIcons( );
	else
		m_icons->Hide( );
}




void CMain::RestoreOriginalColors( )
{
	m_icons->SetTextColorEx( 
		ReadIntSetting( "res_x86", "deftxtclr", 1 ) ? _WHITE : _BLACK );
	
	m_icons->UndoTransparent( GetSysColor( COLOR_DESKTOP ) );		
}