///////////////////////////////////////////////////////////////////////////////
//// SimplyTransparent.cpp : Defines the entry point
//

#include "stdafx.h"
#include "SimplyTransparent.h"
#include "main.h"
#include "_Splash.h"

#include <Windowsx.h>
#include "getwnd.h"

BOOL ExecuteCmdLineOptions( HINSTANCE hInstance, char* cmdline );
LRESULT CALLBACK TrayBoxProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

///////////////////////////////////////////////////////////////////////////////
//// The main app object... (one of the few global var's)
//
CMain *theApp = NULL;

///////////////////////////////////////////////////////////////////////////////
//// Main entry point
//
int APIENTRY WinMain( HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPSTR     lpCmdLine,
                      int       nCmdShow )
{

	if( ExecuteCmdLineOptions( hInstance, lpCmdLine ) )
		return 0;

 	int nReturn;

	
	if( ReadIntSetting( "", "Logging", LOGGING ) )
		_InitLogger( );//Enable log

	
	if( IsWindowsNT( ) )
		__Comment( "Running on NT" );

	//Splashscreen code...
	if( ReadIntSetting( "", "Splash", SPLASH )
		|| ReadIntSetting( "", "FirstRun", 1 ) )
		Splash( hInstance );
	
	
	// Must create an object for the pointer
	theApp = new CMain( hInstance, lpCmdLine );
	

	// initialize!
	if ( theApp->InitInstance ( ) )
	{
		//Load settings
		theApp->LoadSettings( );

		if( ReadIntSetting( "", "FirstRun", 1 ) == 1 )
		{
			theApp->LockInterface( CMain::lock );

			char message[1024];

			LoadString( hInstance, IDS_WELCOME, message, 1024 );

			//Welcome message
			__Message( message );

			//About
			theApp->OnAbout( );

			//Options
			theApp->OnOptions( );

			//OnOptions unlocks the interface on return
			theApp->LockInterface( CMain::lock );

			//Tray icon awareness message
			DialogBox( hInstance, (LPCTSTR)IDD_TRAYICON_AWARENESS,
				GetMyWindowHandle( ), (DLGPROC)TrayBoxProc );
			
			theApp->LockInterface( CMain::unlock );

			WriteIntSetting( "", "FirstRun", 0 );
		}
				 		
		nReturn = theApp->Run();// Main message loop:
	}
	else
	{
		__Comment ( "theApp->InitInstance failed...exiting" );	

		nReturn = false;
		
	}

	__Comment ( "Shutdown Begin" );
	// from this point on...is the beginning of the end... ;)

	__Comment ( "Deleting CMain Object..." );
	delete theApp;

	// flush the log & close it.
	_CloseLogger( );

	return nReturn;// woo hoo! we're done!
}

///////////////////////////////////////////////////////////////////////////////
///// Main Box Proc
//		  
LRESULT CALLBACK WndProc( HWND hWnd, UINT message,
						  WPARAM wParam, LPARAM lParam )
{
	int wmId, wmEvent;

	switch ( message )
	{
		case WM_COMMAND:
			wmId    = LOWORD( wParam ); 
			wmEvent = HIWORD( wParam ); 
			//__Comment( "WM_COMMAND" );
			// Parse the menu selections:
			switch ( wmId )
			{
				case IDM_ABOUT:
					theApp->OnAbout( );
					break;

				case ID_HELP:
					theApp->OnHelp( );
					break;

				case ID_UPDATE:
					theApp->OnUpdateCheck( );
					break;

				case ID_SET_TEXT_COLOR:
					theApp->OnSettextcolor( );
					break;

				case ID_OPTIONS:
					theApp->OnOptions( );
					break;

				case ID_SHOWSPLASH:
					theApp->OnShowSplash( );
					break;

				case ID_STARTUP:
					theApp->OnStartup( );
					break;

				case ID_BEGINNERMODE:
					theApp->OnBeginnerMode( );
					break;

				case IDM_EXIT:					
					if( theApp->OnExit( ) )
						DestroyWindow( hWnd );
					break;

				

				default:
					return DefWindowProc( hWnd, message, wParam, lParam );
			}
			break;

		case WM_CLOSE:
		case WM_QUIT:		
		case WM_ENDSESSION:
			DestroyWindow(hWnd);
			break;

		case WM_QUERYENDSESSION:
			return theApp->OnQueryEndSession( );
			break;
		
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;

		case WM_MY_BEGIN_EXIT_NOTIFICATION:
			theApp->OnBeginExitNotification( wParam, lParam );
			break;

		case WM_MY_EXIT_NOTIFICATION:
			theApp->OnExitNotification( wParam, lParam );
			break;

		case WM_SIMPLYTRANSPARENT_SETTEXTCOLOR:
			theApp->OnRemoteSetTextColor( wParam, lParam );
			break;

		case WM_MY_RESTART_NOTIFICATION:
			theApp->OnRestart( wParam, lParam );
			break;
		
		case WM_MY_SHOWTRAY_NOTIFICATION:
			theApp->OnShowTrayNotification( wParam, lParam );				
			break;
	
		case WM_SHOW_OPTIONS:
		case WM_SHOW_TEXTCOLOR:
		case WM_SHOW_ABOUT:
		case WM_GET_UPDATES:
			if( wParam == 577 && lParam == (LPARAM)hWnd )
			{
				switch( message )
				{
					case WM_SHOW_OPTIONS:
						theApp->OnOptions( );
						break;
					case WM_SHOW_TEXTCOLOR:
						theApp->OnSettextcolor( );
						break;
					case WM_SHOW_ABOUT:
						theApp->OnAbout( );
						break;
					case WM_GET_UPDATES:
						theApp->OnUpdateCheck( );
						break;
					default:break;
				}
			}
			break;

		case WM_MY_TRAY_NOTIFICATION:
			theApp->OnTrayNotification( wParam, lParam );
			break;

		case WM_TIMER:
			theApp->OnTimer( wParam );
			break;

		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
   }
   return 0;
}






///////////////////////////////////////////////////////////////////////////////
///// About Box Proc
//
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	const int about_msg_size = 400;
	static BOOL bClick = false;
	static BOOL bHover = false;
	static HCURSOR old = LoadCursor( 0, IDC_ARROW ), 
		hand = LoadCursor( 0, IDC_HAND );
	static DWORD hoverText = 0;
	
	TCHAR about_message[about_msg_size];
	TCHAR paypal_message[(about_msg_size/2)];

	PAINTSTRUCT ps;
	HDC hdc;
	HFONT norm, under;
	HGDIOBJ hObj;
	COLORREF oldtc;
	UINT id[4] = { IDC_CONTACTME, IDC_VISITME, IDC_PRODUCTHOME, IDC_HELPME };
	int x;
	
	register POINT p;
	register RECT rCon, rVis, rDon, rPro, rHelp, r;

	switch (message)
	{
		case WM_INITDIALOG:
			__Comment ( "Loading AboutBox Resourses" );
			LoadString( theApp->GetInstanceHandle(), IDS_ABOUT, about_message,
				about_msg_size );
			SetWindowText( GetDlgItem( hDlg, IDC_EDIT1 ) , about_message );

			LoadString( theApp->GetInstanceHandle(), IDS_PAYPAL, paypal_message,
				(about_msg_size/2) );
			SetWindowText( GetDlgItem( hDlg, IDC_PAYPAL_DONTATE ) , paypal_message );

			UpdateWindow( hDlg );
			__AnimateWindow( hDlg, 200, AW_ACTIVATE|AW_BLEND );
			ShowWindow( hDlg, SW_SHOW );
			InvalidateRect( hDlg, 0, true );
			return TRUE;
		
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
			case IDOK: 
			case IDCANCEL:			
				__Comment ( "Closing AboutBox" );
				KillTimer( hDlg, 1 );
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;

			case IDC_CONTACTME:
				bClick = true;
				
				ShellExecute( GetMyWindowHandle( ), "open", "http://www.jonathangrimes.com/contact.php", 0, 0, SW_SHOWNORMAL );				
				
				Sleep( 300 );				
				bClick = false;
				break;

			case IDC_VISITME:
				bClick = true;
				ShellExecute( GetMyWindowHandle( ), "open", "http://www.jonathangrimes.com", 0, 0, SW_SHOWNORMAL );
				
				Sleep( 300 );
				bClick = false;
				break;

			case IDC_PRODUCTHOME:
				bClick = true;
				ShellExecute( GetMyWindowHandle( ), "open", "http://www.jonathangrimes.com/projects.php&name=simply%20transparent", 0, 0, SW_SHOWNORMAL );
				
				Sleep( 300 );
				bClick = false;
				break;

			case IDC_HELPME:
				bClick = true;
				theApp->OnHelp( );
				
				Sleep( 300 );
				bClick = false;
				break;

			case IDC_PAY:
				bClick = true;
				ShellExecute( GetMyWindowHandle( ), "open", "http://www.jonathangrimes.com/donate.php", 0, 0, SW_SHOWNORMAL );

				Sleep( 300 );
				bClick = false;
				break;
			}

			break;


		case WM_PAINT:
			hdc = BeginPaint(hDlg, &ps);

			oldtc = GetTextColor( hdc );

			

			norm = CreateFont(
				-MulDiv( 8, GetDeviceCaps(hdc, LOGPIXELSY), 72),
				0,
				0,
				0,
				400,
				false, //italic
				false, //underline
				false, //strickout
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, //PROOF_QUALITY
				DEFAULT_PITCH|FF_SWISS,
				"Tahoma" );//"MS Sans Serif"

			under = CreateFont(
				-MulDiv( 8, GetDeviceCaps(hdc, LOGPIXELSY), 72),
				0,
				0,
				0,
				800,
				false, //italic
				true, //underline
				false, //strickout
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, //PROOF_QUALITY
				DEFAULT_PITCH|FF_SWISS,
				"Tahoma" );//"MS Sans Serif"

				
			hObj = SelectObject( hdc, norm );
			SetBkMode( hdc, TRANSPARENT );
						

			for( x = 0; x<4; x++ )
			{
				GetWindowRect( GetDlgItem( hDlg, id[x] ), &r );
				ScreenToClient( hDlg, &r );
				
				if(hoverText == id[x] )
				{
					SetTextColor( hdc, RGB( 102, 102, 255 ) );
					SelectObject( hdc, under );
				}
				else
					SetTextColor( hdc, RGB( 0, 51, 255 ) );

				char text[50];
				LoadString( GetMyInstanceHandle( ), id[x], text, 50 );

				TextOut( hdc, r.left, r.top, text, strlen( text ) );				
			}
			
				
			SetTextColor( hdc, oldtc );

			SelectObject( hdc, hObj );

			DeleteObject( norm );
			DeleteObject( under );

			EndPaint(hDlg, &ps);
			break;


		case WM_MOUSELEAVE:
			if( hoverText == IDC_CONTACTME)
				InvalidateRect( hDlg, &rCon, true );
			if( hoverText == IDC_VISITME)
				InvalidateRect( hDlg, &rVis, true );
			if( hoverText == IDC_PRODUCTHOME )
				InvalidateRect( hDlg, &rPro, true );
			if( hoverText == IDC_HELPME )
				InvalidateRect( hDlg, &rHelp, true );
			
			hoverText = 0;
			break;

		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		case WM_MOUSEHOVER:
			
			bHover = true;

			p.x = GET_X_LPARAM(lParam); 
			p.y = GET_Y_LPARAM(lParam);
			
			GetWindowRect( GetDlgItem( hDlg, IDC_CONTACTME ), &rCon );
			ScreenToClient( hDlg, &rCon );

			GetWindowRect( GetDlgItem( hDlg, IDC_VISITME ), &rVis );
			ScreenToClient( hDlg, &rVis );

			GetWindowRect( GetDlgItem( hDlg, IDC_PRODUCTHOME ), &rPro );
			ScreenToClient( hDlg, &rPro );

			GetWindowRect( GetDlgItem( hDlg, IDC_HELPME ), &rHelp );
			ScreenToClient( hDlg, &rHelp );

			GetWindowRect( GetDlgItem( hDlg, IDC_PAY ), &rDon );
			ScreenToClient( hDlg, &rDon );
			

			if( IsPintInRect( &p, &rCon ) )
			{	
				if( hoverText != IDC_CONTACTME )
				{
					hoverText = IDC_CONTACTME;
					InvalidateRect( hDlg, &rCon, true );
				}

				SetCursor( hand );
				
				if( message == WM_LBUTTONUP )
					SendMessage( hDlg, WM_COMMAND, IDC_CONTACTME,0 );
				
			}

			else if( IsPintInRect( &p, &rVis ) )
			{	
				if( hoverText != IDC_VISITME )
				{
					hoverText = IDC_VISITME;				
					InvalidateRect( hDlg, &rVis, true );
				}
				SetCursor( hand );	
				
				if( message == WM_LBUTTONUP )
					SendMessage( hDlg, WM_COMMAND, IDC_VISITME,0 );
			}

			else if( IsPintInRect( &p, &rPro ) )
			{	
				if( hoverText != IDC_PRODUCTHOME )
				{
					hoverText = IDC_PRODUCTHOME;				
					InvalidateRect( hDlg, &rPro, true );
				}

				SetCursor( hand );

				if( message == WM_LBUTTONUP )
					SendMessage( hDlg, WM_COMMAND, IDC_PRODUCTHOME,0 );
			}

			else if( IsPintInRect( &p, &rHelp ) )
			{	
				if( hoverText != IDC_HELPME )
				{
					hoverText = IDC_HELPME;
					InvalidateRect( hDlg, &rHelp, true );
				}
				SetCursor( hand );

				if( message == WM_LBUTTONUP )
					SendMessage( hDlg, WM_COMMAND, IDC_HELPME,0 );
			}

			else 
			{
				
				if( hoverText == IDC_CONTACTME )
					InvalidateRect( hDlg, &rCon, true );
				if( hoverText == IDC_VISITME )
					InvalidateRect( hDlg, &rVis, true );
				if( hoverText == IDC_PRODUCTHOME )
					InvalidateRect( hDlg, &rPro, true );
				if( hoverText == IDC_HELPME )
					InvalidateRect( hDlg, &rHelp, true );

				hoverText = 0;

				if( IsPintInRect( &p, &rDon ) )
				{
					SetCursor( hand );	
					
					if( message == WM_LBUTTONUP )
						SendMessage( hDlg, WM_COMMAND, IDC_PAY,0 );
				}
				else
				{
					bHover = false;			
					SetCursor( old );
				}
			}


			return true;

		case WM_SETCURSOR:

			if(bHover)
			{
				SetCursor( hand );
				return true;
			}


			
			break;
			
	}
    return FALSE;
}








///////////////////////////////////////////////////////////////////////////////
///// About Box Proc
//
LRESULT CALLBACK TrayBoxProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	TCHAR msg[400];

	switch( message )
	{
	case WM_INITDIALOG:		
		LoadString( theApp->GetInstanceHandle(), IDS_TRAY_AWARNESS, msg,
				400 );
			SetWindowText( GetDlgItem( hDlg, IDC_TRAYMESSAGE ) , msg );

		__AnimateWindow( hDlg, 200, AW_ACTIVATE|AW_BLEND );
		ShowWindow( hDlg, SW_SHOW );
		InvalidateRect( hDlg, 0, true );
		break;
	
	case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				__AnimateWindow( hDlg, 200, AW_HIDE|AW_BLEND );
				__Comment ( "Closing AboutBox" );
				KillTimer( hDlg, 1 );
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
	return false;
}







///////////////////////////////////////////////////////////////////////////////
///// if ST already running...tell it to execute the option
//
BOOL ExecuteCmdLineOptions( HINSTANCE hInstance, char* cmdline )
{
	UINT message = 0;
	BOOL ret = false;
	char class_s[MAX_PATH];
	char title_s[MAX_LOADSTRING];

	string cmds = ___changeCase( cmdline );

	LoadString(hInstance, IDS_APP_CLASS, class_s, MAX_PATH);
	LoadString(hInstance, IDS_APP_TITLE, title_s, MAX_LOADSTRING);
	___makelower( class_s );

	if( FindWindow( class_s, title_s ) != NULL)
	{
		if( cmds.length( ) == 0 )//no commandline arguments
		{
			if( !ReadIntSetting( "", "TrayIcon", TRAYICON ) )
				message = WM_MY_SHOWTRAY_NOTIFICATION;
		}
		else
		{
		
			if( cmds.find( "/options" ) != -1 )
				message = WM_SHOW_OPTIONS;
			else if( cmds.find( "/textcolor" ) != -1 )
				message = WM_SHOW_TEXTCOLOR;
			else if( cmds.find( "/about" ) != -1 )
				message = WM_SHOW_ABOUT;
			else if( cmds.find( "/update" ) != -1 )
				message = WM_GET_UPDATES;
					
		}


		if(message != 0)
		{
			PostMessage( FindWindow( class_s, title_s ), 
				message,
				577,// number symbols for ST7
				//recieving session will test this to see if it matches itself
				(LONG)FindWindow( class_s, title_s )
			);

			ret = true;
		}

	}

	return ret;
}








///////////////////////////////////////////////////////////////////////////////
///// Global function checks for updates
//
BOOL UpdateCheck( int major, int minor, BOOL silent )
{
	BOOL ret = false;
	const int url_size = 65535;
	LPSTR url = new char[url_size];
	memset( url, 'A', ( url_size*(sizeof(char) ) ) );

	BOOL (*lpfProc)( int,int,char * );

	__Comment( "Loading Library: tool_x86.dll" );
	HINSTANCE hInst_tool_x86 = LoadLibraryA( "TOOL_X86.DLL" );
    if( hInst_tool_x86 != NULL )
	{
		__Comment( "Load Success" );

		 // Get procedure address.
		lpfProc = (BOOL(*)(int,int,char *))
			GetProcAddress( hInst_tool_x86, "Update" );

		if( lpfProc != NULL )
		{
			__Comment( "Call Update proc" );

			if( lpfProc( major,minor, url ) )
			{
				__Comment( "Update found..." );
				if( strlen( url ) )
				{
					__Comment( url );
					if( __Message( IDS_UPDATE, MB_YESNO ) == IDYES )
					ShellExecute( GetMyWindowHandle( ), "open", url,0,0,SW_SHOW );
				}
			
			}
			else
			{
				if(!silent)
					__Message( "You have the latest version of Simply Transparent" );

				__Comment( "no updates found" );
			}
		}		

		FreeLibrary( hInst_tool_x86 );
		__Comment( "Library Free Success" );
	}
	else __Message( "Tool module is not present, or corrupted." );

	delete url;

	return ret;
}








///////////////////////////////////////////////////////////////////////////////
///// Global function that returns the instance handle of this program
//		(A replacement for a global variable)
HINSTANCE GetMyInstanceHandle()
{
	return (theApp == NULL) ? NULL : theApp->GetInstanceHandle( );
}


///////////////////////////////////////////////////////////////////////////////
///// Global function that returns the main window handle of this program
//		(A replacement for a global variable)
HWND GetMyWindowHandle()
{
	return (theApp == NULL) ? NULL : theApp->GetWindowHandle( );
}



///////////////////////////////////////////////////////////////////////////////
///// Global function that returns the main title of this program
//		(A replacement for a global variable)
string GetMyName()
{
	return (theApp == NULL) ? NULL : theApp->GetName( );
}



///////////////////////////////////////////////////////////////////////////////
///// Global function that returns the Desktop ListView Control handle
//		(A replacement for a global variable)
HWND GetDeskListCtlWnd( )
{
	return (theApp == NULL) ? NULL : theApp->GetDLCWnd( );
}




///////////////////////////////////////////////////////////////////////////////
/////
//
BOOL _IsRes_x86Loaded( )
{
	return ( theApp == NULL ) ? false : theApp->IsRes_Loaded( );
}
