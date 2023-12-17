///////////////////////////////////////////////////////////////////////////////
//// stdafx.cpp
//
#include "stdafx.h"
#include "simplytransparent.h"

#include <winreg.h>
#include <shlobj.h>

UINT CALLBACK CustomMessageBoxProc( HWND hdlg, UINT msg, WPARAM wp, LPARAM lp );

#define USER_RUN_KEY        "Software\\Microsoft\\Windows\\CurrentVersion\\Run"//HKEY_CURRENT_USER

#define RECTHEIGHT( r )	(r.bottom - r.top)
#define RECTWIDTH( r )	(r.right - r.left)
#define ZERORECT( r )	memset( &r, 0, sizeof(RECT) )


///////////////////////////////////////
//// Logging definitions:
//
#define _COMMENT			_CRT_WARN+_CRT_ERROR+_CRT_ASSERT+101
#define _ERROR				_COMMENT+1
#define _LOG				_ERROR+1

///////////////////////////////////////
//// Loging variables
//
static HANDLE log = INVALID_HANDLE_VALUE;

///////////////////////////////////////////////////////////////////////////////
//// Logging Functions
//
static int AssertoinHandeler( int reportType, char *message, int *returnValue )
{
	if( log != INVALID_HANDLE_VALUE )
	{

		DWORD written;
		char mes[MAX_PATH];	 

		switch( reportType ){
		case _CRT_WARN:
			sprintf( mes, "WARNING: %s", message );		
			break;
		case _CRT_ERROR:
			sprintf( mes, "ERROR: %s", message );
			break;
		case _CRT_ASSERT:
			sprintf( mes, "ASSERT: %s", message );
			break;
		case _COMMENT:
			sprintf( mes, "Comment: %s\r\n", message );
			break;
		case _ERROR:
			sprintf( mes, "Run-Time Error: %s\r\n", message );		
			break;
		case _LOG:
			sprintf( mes, "%s\r\n", message );		
			break;
		default:
			sprintf( mes, "???: %s\r\n", message );
			break;

		}

		WriteFile( log, mes, strlen( mes ), &written, 0 );

		FlushFileBuffers( log );

	}

	return false;
}





///////////////////////////////////////////////////////////////////////////////
////
//
void _InitLogger( ){
		
	int n = 1;
	char buf1[MAX_PATH],
		buf2[MAX_PATH];	
	__GetSpecialFolderPath( GetMyWindowHandle( ), buf1, CSIDL_PERSONAL ,1 );
	sprintf( buf2, "%s\\Simply Transparent Log.txt", buf1 );

	do
	{
		log = CreateFile( buf2, GENERIC_WRITE, FILE_SHARE_READ, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, 0 );
		
		if( log == INVALID_HANDLE_VALUE )
			sprintf( buf2, "%s\\Simply Transparent Log( %d ).txt", buf1, n++ );		
	}
	while(log == INVALID_HANDLE_VALUE);
	
	GetTime( buf1 ); 
	sprintf( buf2, "Log opened: %s", buf1 );
	AssertoinHandeler( _LOG, buf2, NULL );


#ifdef _DEBUG	 
	
	__Comment( "InitLogger( ) called" );

	_CrtSetReportHook( AssertoinHandeler );


	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_WNDW );
	_CrtSetReportFile( _CRT_WARN, 0 );

	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_WNDW );
	_CrtSetReportFile( _CRT_ERROR, 0 );

	_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_WNDW );
	_CrtSetReportFile( _CRT_ASSERT, 0 );

	__Comment ( "Assertions enabled" );
#endif //_DEBUG

}




///////////////////////////////////////////////////////////////////////////////
////
//
int  _CheckLogger( ) { return ( log != INVALID_HANDLE_VALUE ) ? true : false; }




///////////////////////////////////////////////////////////////////////////////
////
//
void _CloseLogger()
{
	char buf1[50],
		buf2[50];	

	GetTime( buf1 ); 
	sprintf( buf2, "log closed: %s", buf1 );
	AssertoinHandeler( _LOG, buf2, NULL );
	
	FlushFileBuffers( log );
	CloseHandle( log );

	log = INVALID_HANDLE_VALUE;
}




///////////////////////////////////////////////////////////////////////////////
////
//
void __Comment ( const char *message )
{
	AssertoinHandeler( _COMMENT, (char *)message, NULL );
}





///////////////////////////////////////////////////////////////////////////////
////
//
int __Message ( const char *message, int type )
{
	__Comment( message );
	char s[2048];

	sprintf( s, "%d%s", type, message );

	return DialogBoxParam( GetMyInstanceHandle( ), (LPCTSTR)IDD_MESSAGE, GetMyWindowHandle(),
		(DLGPROC)CustomMessageBoxProc, (LPARAM)s );

}





///////////////////////////////////////////////////////////////////////////////
////
//
int __Message ( UINT message, int type )
{
	const DWORD buffer_size = 65535;	
	LPSTR buffer = new char[buffer_size];
	memset( buffer, 0, (buffer_size*sizeof(char)) );
	
	LoadString( GetMyInstanceHandle( ), message, buffer, buffer_size);
	int ret = __Message( buffer, type );

	delete buffer;
	return ret;
}






///////////////////////////////////////////////////////////////////////////////
////
//
void __Error( const char *message )
{
	AssertoinHandeler( _ERROR, (char *)message, NULL );
	//__Message( message );
}





///////////////////////////////////////////////////////////////////////////////
////
//
void __Error( DWORD error )
{
	LPVOID lpMsgBuf;
	
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);

	AssertoinHandeler( _ERROR, (char*)(LPCTSTR)lpMsgBuf, NULL );
	
	LocalFree( lpMsgBuf );
}




///////////////////////////////////////////////////////////////////////////////
////
//
void __ErrorMessage( const char *message )
{
	AssertoinHandeler( _ERROR, (char *)message, NULL );
	__Message( message );
}





///////////////////////////////////////////////////////////////////////////////
////
//
void __ErrorMessage( DWORD error )
{
	LPVOID lpMsgBuf;
	
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);

	__Message( (char *)(LPCTSTR)lpMsgBuf );
	AssertoinHandeler( _ERROR, (char*)(LPCTSTR)lpMsgBuf, NULL );
	
	LocalFree( lpMsgBuf );
}






///////////////////////////////////////////////////////////////////////////////
//// 
//
char * GetTime( char *ret )
{
	SYSTEMTIME time;

	GetLocalTime( &time );

	char *mo[] = { "January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December" };
	
	sprintf( ret, "%d:%d:%d %s  ( %s %d, %d )",
		(time.wHour>12)?time.wHour-12:time.wHour,
		time.wMinute,
		time.wSecond,
		(time.wHour >= (WORD)12) ? "PM" : "AM",		
		mo[time.wMonth-1],
		time.wDay,
		time.wYear		
		);
	return ret;
} 




///////////////////////////////////////////////////////////////////////////////
////	For now I don't want to use the registry run key...
//
BOOL __DoesStartWithWindows( LPCTSTR exe, LPCTSTR name,
								HWND hWnd, BOOL bUseRegistry )
{
	BOOL bReturn = TRUE;
	HKEY  key;
	

	if( bUseRegistry )
	{
		
		if( ERROR_SUCCESS == RegOpenKeyEx( HKEY_CURRENT_USER, 
											TEXT( USER_RUN_KEY ),0, 
											KEY_ALL_ACCESS, &key ) )
		{
		/*	
			char value[MAX_PATH];
			LONG value_size = MAX_PATH;

			if(RegQueryValueEx(key,TEXT(name),0, LPDWORD lpType,
								REG_SZ, value, value_size ) != ERROR_SUCCESS)
			{
				bReturn = FALSE;
			}					*/

			RegCloseKey( key );
		}
		else bReturn = FALSE;
	}
	else
	{
		char filename[MAX_PATH*2];
		char path[MAX_PATH];
		__GetSpecialFolderPath( hWnd, path, CSIDL_STARTUP, 1 );
		
		sprintf( filename, "%s\\%s.lnk", path, name );
	
		////////////////////////////////////////////////////////
		//// See if the file defined in szPath exists
		//
		FILE *Shortcut;
		if( ( Shortcut = fopen( filename, "r" ) ) == NULL )
			bReturn = FALSE;

		else fclose( Shortcut );
		////////////////////////////////////////////////////////		  

	}
	
	return bReturn;	
}





///////////////////////////////////////////////////////////////////////////////
////
//
BOOL __StartWithWindows( BOOL bStart,	LPCTSTR exe,
					  LPCTSTR name, HWND hWnd, BOOL bUseRegistry )
{
	BOOL bReturn = TRUE;
	HKEY  key;
	char filename[MAX_PATH*2];
	string exe_s = exe;
	
	{
		char path[MAX_PATH];

		__GetSpecialFolderPath( hWnd, path, CSIDL_STARTUP ,1 );

		sprintf( filename, "%s\\%s.lnk", path, name );
	}	
	
	if( bStart )
	{
		if( bUseRegistry )
		{
			if( ERROR_SUCCESS == RegOpenKeyEx( HKEY_CURRENT_USER, 
				TEXT( USER_RUN_KEY ), 0, KEY_ALL_ACCESS, &key ) )
			{
				RegSetValueEx( key, TEXT( name ), 0, REG_SZ,
					(BYTE*)(unsigned char *)LPCSTR( exe ), 512 );

				RegCloseKey( key );
			}
		}
		else
		{

			string folder = ___left( exe_s, ___reversefind( exe_s, '\\' ) );
			__CreateLink(
				exe,
				folder.c_str( ),
				"",
				(char *)LPCTSTR( filename ),
				(char *)LPCTSTR( name )
				
				);
		}
	}
	else
	{
		DeleteFile( filename );
		
		if( ERROR_SUCCESS == RegOpenKeyEx( HKEY_CURRENT_USER,
			TEXT( USER_RUN_KEY ),0, KEY_ALL_ACCESS, &key ) )
		{
			RegDeleteValue( key, TEXT( name ) );
			RegCloseKey( key );
		}
	}

	return bReturn;	
}
 




///////////////////////////////////////////////////////////////////////////////
////
//
HRESULT __CreateLink( LPCSTR pszShortcutFile, LPCSTR pszWorkDir,
				   LPCSTR pszCmdArgs, LPSTR pszLink, LPSTR pszDesc )
{
	HRESULT hres;
	IShellLink *psl;

	CoInitialize( NULL );

	// Create an IShellLink object and get a pointer to the IShellLink 
	// interface (returned from CoCreateInstance).
	hres = CoCreateInstance ( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
		IID_IShellLink, (void **)&psl );

	if( hres > -1 )
	{
		
		IPersistFile *ppf;

		// Query IShellLink for the IPersistFile interface for 
		// saving the shortcut in persistent storage.
		hres = psl->QueryInterface( IID_IPersistFile, (void **)&ppf );
		if( hres > -1 )
		{
			
			WORD wsz [MAX_PATH]; // buffer for Unicode string

			// Set the path to the shortcut target.
			hres = psl->SetPath( pszShortcutFile );
			hres = psl->SetWorkingDirectory( pszWorkDir );
			hres = psl->SetArguments( pszCmdArgs );

			//hres = psl->SetIconLocation( "", 0 );

			// Set the description of the shortcut.
			hres = psl->SetDescription( pszDesc );
			if( hres != NOERROR )
				__Error( "__CreateLink( ): SetDescription( ) failed!" );

			// Ensure that the string consists of ANSI characters.
			MultiByteToWideChar( CP_ACP, 0, pszLink, -1, wsz, MAX_PATH );
			// Save the shortcut via the IPersistFile::Save member function.
			hres = ppf->Save ( wsz, TRUE );
			if( hres == E_FAIL )
			{
				__Error( "__CreateLink( ): Save failed!" );
			}

			// Release the pointer to IPersistFile.
			ppf->Release( );
			}		
		else
			__Error( "__CreateLink( ): QueryInterface( ) failed!" );

		// Release the pointer to IShellLink.
		psl->Release( );
		}
	else
		__Error( "__CreateLink( ): CoCreateInstance( ) failed!" );
		
	
	CoUninitialize( );

	return hres;
}





///////////////////////////////////////////////////////////////////////////////
////
//
HRESULT __GetSpecialFolderPath( HWND hwndOwner, LPTSTR lpszPath,
							   int nFolder, BOOL fCreate )
{
	HRESULT hReturn = !NOERROR;

	LPITEMIDLIST pidl;
    LPMALLOC pmalloc = NULL;

	SHGetMalloc( &pmalloc );

	hReturn = SHGetSpecialFolderLocation( hwndOwner, nFolder, &pidl );

	if( pidl )
	{
		SHGetPathFromIDList( pidl, lpszPath );

		pmalloc->Free( pidl );
	}
	else hReturn = !NOERROR;

	pmalloc->Release( );

	return hReturn;
}




///////////////////////////////////////////////////////////////////////////////
////
//
BOOL __AnimateWindow( HWND hwnd, DWORD dwTime, DWORD dwFlags )
{
	if(!IsWindowsNT( ))return false;

	BOOL bReturn = false;

	BOOL ( WINAPI *lpfAnimateWindow )( HWND, DWORD, DWORD );

    HINSTANCE hInstLib = LoadLibraryA( "USER32.DLL" );
	if( hInstLib != NULL )
	{
		 // Get procedure address.
		 lpfAnimateWindow = ( BOOL(WINAPI *)(HWND,DWORD,DWORD) )
			GetProcAddress( hInstLib, "AnimateWindow" );

		 if( lpfAnimateWindow != NULL )
		 {
			 bReturn = lpfAnimateWindow( hwnd, dwTime, dwFlags );
		 }
		 else
		 {
			 //function not found
		 }
	}	

	FreeLibrary( hInstLib ) ;

	return bReturn;
}





///////////////////////////////////////////////////////////////////////////////
////
//
BOOL __ADenabled( BOOL bWarn )
{

	BOOL ret = false;
	HINSTANCE h_Shell32 = LoadLibrary( "shell32.dll" );
		if( h_Shell32 )
		{
			void ( WINAPI *pfn_SHGetSettings )
				( LPSHELLFLAGSTATE, DWORD );//function declaration
			
			pfn_SHGetSettings = 
				(void(WINAPI *)(LPSHELLFLAGSTATE,DWORD))
				GetProcAddress( h_Shell32, "SHGetSettings" );

			if( pfn_SHGetSettings )
			{
				SHELLFLAGSTATE shellstateOptions;
				pfn_SHGetSettings( &shellstateOptions, SSF_DESKTOPHTML );
				if( shellstateOptions.fDesktopHTML != 0 )
				{

					if( bWarn )
					{
						__Message( IDS_ACTIVEDESKTOP_WARN );
					}

					ret = true;
					

				}// end if: is Active desktop enabled?

			}// end if: function pointer null?

		}// end if: h_Shell32 valid?

		FreeLibrary(h_Shell32);

		return ret;
}

















///////////////////////////////////////////////////////////////////////////////
//// Color Dialog Hook proc...  used for centering...
//
UINT CALLBACK CCHookProc( HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if( uiMsg == WM_INITDIALOG ){

		RECT dlg;
		RECT center;
		POINT p;
		GetWindowRect( hdlg, &dlg );
		ScreenToClient( GetParent( hdlg ), &dlg );
		GetWindowRect( GetParent( hdlg ), &center );//GetDesktopWindow( )
		ScreenToClient( GetParent( hdlg ), &center );

		p.x = ( center.left + center.right ) / 2
			- ( dlg.left + dlg.right ) / 2;
		p.y = ( center.top + center.bottom ) / 2
			- (dlg.top + dlg.bottom ) / 2;

		ClientToScreen( GetParent( hdlg ), &p );

		SetWindowPos( hdlg, NULL, p.x, p.y, -1, -1,		
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );

		SetWindowText( hdlg, "Set Text Color" );

		__AnimateWindow( hdlg, 200, AW_ACTIVATE|AW_BLEND );
		ShowWindow( hdlg, SW_SHOW );
		InvalidateRect( hdlg, 0, true );
	}

	return 0;
}









///////////////////////////////////////////////////////////////////////////////
//// Custom message box proc...
//
UINT CALLBACK CustomMessageBoxProc( HWND hdlg, UINT msg, WPARAM wp, LPARAM lp )
{
	static string sHead;
	static string sText;

	RECT rWindow = {0,0,0,0},
		rIcon = {0,0,0,0},
		rMessage = {0,0,0,0},
		rHeading = {0,0,0,0},
		rDesktop = {0,0,0,0},
		rTemp = {0,0,0,0};

	char *t;
	int nButtonType,
		width = 0,
		width_head = 0,
		width_mess = 0,
		height = 0,
		height_head = 0,
		height_mess = 0,
		caption;

	PAINTSTRUCT ps;
	HDC hdc;
	HFONT font;
	HGDIOBJ hObj;

	switch( msg )
	{

	case WM_COMMAND:
		if (LOWORD(wp) == IDOK || LOWORD(wp) == IDCANCEL
			|| LOWORD(wp) == IDYES || LOWORD(wp) == IDNO) 
		{
			sHead.erase();
			sText.erase();
			__AnimateWindow( hdlg, 200, AW_HIDE|AW_BLEND );
			EndDialog(hdlg, LOWORD(wp));
			return TRUE;
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hdlg, &ps);
		
		font = CreateFont(
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

		GetClientRect( hdlg, &rWindow );
		GetWindowRect( GetDlgItem( hdlg, IDC_IMAGE ), &rIcon );
		ScreenToClient( hdlg, &rIcon );		

		rMessage		=  rWindow;
		rMessage.top	=  rWindow.top+rIcon.top;
		rMessage.left	=  rWindow.left+rIcon.right+10;
		rMessage.right  -= rIcon.left;

		SetTextColor( hdc, RGB(0,0,0) );
		SetBkMode( hdc, TRANSPARENT );	

		if( sHead.length() != 0 )
		{
			rHeading.top	= rWindow.top+5;//rIcon.top;
			rHeading.right	= rWindow.right;
			rHeading.left	= rWindow.left+rIcon.right;
			rHeading.bottom	= rWindow.bottom;

			height = DrawText( hdc, sHead.c_str(), sHead.length(), &rHeading, DT_SINGLELINE|DT_CALCRECT );
			DrawText( hdc, sHead.c_str(), sHead.length(), &rHeading,  DT_SINGLELINE );
			
			rMessage.top	= rHeading.bottom;
		}
		
		//rMessage.left	+= 10;

		if( sText.length() != 0 )
		{
			hObj = SelectObject( hdc, font );
			DrawText( hdc, sText.c_str(), sText.length(), &rMessage, DT_WORDBREAK );
			SelectObject( hdc, hObj );
		}

		DeleteObject( font );

		EndPaint(hdlg, &ps);
		break;







	case WM_INITDIALOG:
		__Comment( "init message" );

///////////////////////////////////////////////////////////////////////////////
///// Parse message string

		t = (char *)lp;

		nButtonType = t[0]-'0';

		t++;

		if( ___find( t, "\n", 0 ) != -1 )
		{
			sHead = ___left( t, ___find( t, "\n", 0 ) );
			sText = ___mid( t, ___find( t, "\n", 0 )+1 );
		}
		else sText = t;

///////////////////////////////////////////////////////////////////////////////	
		hdc = GetDC( hdlg );

		font = CreateFont(
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

///////////////////////////////////////////////////////////////////////////////

		GetWindowRect( GetDesktopWindow(), &rDesktop );		
		GetWindowRect( hdlg, &rWindow );
		GetClientRect( hdlg, &rTemp );
		ClientToScreen( hdlg, &rTemp );

		caption			= rTemp.top - rWindow.top;
				
		GetWindowRect( GetDlgItem( hdlg, IDC_IMAGE ), &rIcon );
		ScreenToClient( hdlg, &rIcon );
	
		rMessage		=  rWindow;
		rMessage.top	=  rWindow.top+rIcon.top;
		rMessage.left	=  rWindow.left+rIcon.right+10;

		if( sHead.length() != 0 )
		{
			rHeading.top	= rWindow.top+rIcon.top;
			rHeading.right	= rWindow.right;
			rHeading.left	= rWindow.left+rIcon.right;
			rHeading.bottom	= rWindow.bottom;

			height = DrawText( hdc, sHead.c_str(), sHead.length(), &rHeading, DT_SINGLELINE|DT_CALCRECT);

			rHeading.bottom	 = rHeading.top		+ height;
			rMessage.top	+= rHeading.bottom;			
		}
		
		hObj = SelectObject( hdc, font );

		height = DrawText( hdc, sText.c_str(), sText.length(), &rMessage, DT_CALCRECT|DT_SINGLELINE);
	
		SelectObject( hdc, hObj );
		DeleteObject( font );

		if( ((RECTWIDTH(rDesktop)/10)*6) < RECTWIDTH(rMessage) )
		{
			int desiredWidth = ((RECTWIDTH(rDesktop)/10)*6);
			int lines = RECTHEIGHT(rMessage)/height;				
				lines += RECTWIDTH(rMessage)/desiredWidth;
				lines++;

			rMessage.right	= rMessage.left+desiredWidth;
			rMessage.bottom	= rMessage.top + (lines*height);				
			
		}

		///////////////////
		width_head	= rHeading.right	- rHeading.left;
		width_mess	= rMessage.right	- rMessage.left+10;

		height_head	= rHeading.bottom	- rHeading.top;
		height_mess	= rMessage.bottom	- rMessage.top;

		//			v--includes gutter
		//		  __border__|icon_________|______text width_________________________________________|__border__
		width	= rIcon.left+rIcon.right + ( ( width_head > width_mess ) ? width_head : width_mess )+rIcon.left;

		if( width < 200 )width = 200;
		

		//		  caption__|top gutter_|____body height__________|_btn__|_gutter__
		height	= caption + rIcon.top + height_head + height_mess + 35 + rIcon.top;

		if( height < 75 )height = 75;


		rWindow.left	=	(rDesktop.right/2)	-	(width/2);
		rWindow.right	=	(rDesktop.right/2)	+	(width/2);
		rWindow.top		=	(rDesktop.bottom/2)	-	(height/2);
		rWindow.bottom	=	(rDesktop.bottom/2)	+	(height/2);

		MoveWindow( hdlg, rWindow.left, rWindow.top, RECTWIDTH( rWindow ), RECTHEIGHT( rWindow ), true );
		
		ScreenToClient( hdlg, &rWindow );

		if( nButtonType > -1)
		{
			RECT r;
			switch( nButtonType )
			{
			case MB_OK:
				
				r.top = rWindow.bottom-rIcon.top-25;
				r.bottom = rWindow.bottom-rIcon.top;

				r.left = ( RECTWIDTH( rWindow )/2)-50;
				r.right = ( RECTWIDTH( rWindow )/2)+50;
				
				MoveWindow( GetDlgItem( hdlg, IDOK ),
					r.left, r.top, RECTWIDTH( r ), RECTHEIGHT( r ), true );

				SetWindowLong( GetDlgItem( hdlg, IDOK ), GWL_STYLE,
					BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP );
				break;

			case MB_YESNO:
				
				r.top = rWindow.bottom-rIcon.top-25;
				r.bottom = rWindow.bottom-rIcon.top;

				r.left = ( RECTWIDTH( rWindow )/2)-105;
				r.right = ( RECTWIDTH( rWindow )/2)-5;
				
				MoveWindow( GetDlgItem( hdlg, IDYES ),
					r.left, r.top, RECTWIDTH( r ), RECTHEIGHT( r ), true );

				SetWindowLong( GetDlgItem( hdlg, IDYES ), GWL_STYLE,
					BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP );

				r.left = ( RECTWIDTH( rWindow )/2 )+5;
				r.right = ( RECTWIDTH( rWindow )/2 )+105;
				
				MoveWindow( GetDlgItem( hdlg, IDNO ),
					r.left, r.top, RECTWIDTH( r ), RECTHEIGHT( r ), true );
				SetWindowLong( GetDlgItem( hdlg, IDNO ), GWL_STYLE,
					BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP);
				break;
			case MB_YESNOCANCEL:

				r.top = rWindow.bottom-rIcon.top-25;
				r.bottom = rWindow.bottom-rIcon.top;

				r.left = ( RECTWIDTH( rWindow )/2)-160;
				r.right = ( RECTWIDTH( rWindow )/2)-60;
				
				MoveWindow( GetDlgItem( hdlg, IDYES ),
					r.left, r.top, RECTWIDTH( r ), RECTHEIGHT( r ), true );

				SetWindowLong( GetDlgItem( hdlg, IDYES ), GWL_STYLE,
					BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP );

				r.left = ( RECTWIDTH( rWindow )/2 )-50;
				r.right = ( RECTWIDTH( rWindow )/2 )+50;
				
				MoveWindow( GetDlgItem( hdlg, IDNO ),
					r.left, r.top, RECTWIDTH( r ), RECTHEIGHT( r ), true );
				SetWindowLong( GetDlgItem( hdlg, IDNO ), GWL_STYLE,
					BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP);


				r.left = ( RECTWIDTH( rWindow )/2 )+60;
				r.right = ( RECTWIDTH( rWindow )/2 )+160;
				
				MoveWindow( GetDlgItem( hdlg, IDCANCEL ),
					r.left, r.top, RECTWIDTH( r ), RECTHEIGHT( r ), true );
				SetWindowLong( GetDlgItem( hdlg, IDCANCEL ), GWL_STYLE,
					BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP);


				break;
			}
		}

		
		{
			char name[30];
			LoadString( GetMyInstanceHandle(), IDS_APP_TITLE, name, 30 );
			SetWindowText( hdlg, name );
		}


		UpdateWindow( hdlg );
		
		__AnimateWindow( hdlg, 200, AW_ACTIVATE|AW_BLEND );
		__Comment( "show message" );
		ShowWindow( hdlg, SW_SHOW );
		InvalidateRect( hdlg, 0, true );

		break;
	}

	return 0;

}