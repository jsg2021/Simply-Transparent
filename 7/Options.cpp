///////////////////////////////////////////////////////////////////////////////
//// Options.cpp
//

#include "stdafx.h"
#include "SimplyTransparent.h"
#include "options.h"


LRESULT CALLBACK ChooseLockMethodProc(HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK SetAdvancedIconOptionsProc(HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK OptionsProc(HWND, UINT, WPARAM, LPARAM );

///////////////////////////////////////////////////////////////////////////////
//// Options Caller Definition
//
int Options( HWND hWnd )
{
	int nRet = DialogBox( GetMyInstanceHandle(), (LPCTSTR)IDD_OPTIONS,
						hWnd, (DLGPROC)OptionsProc);

	if( nRet == IDOK )__Comment ( "Options ended with IDOK" );
	else __Comment ( "Options ended with !IDOK...assumed IDCANCEL" );

	return nRet;
}



///////////////////////////////////////////////////////////////////////////////
///// Choose Lock Method Box Caller Definition
//
static int ChooseLockMethod(  HWND hWnd )
{
	int nRet = DialogBox( GetMyInstanceHandle(), (LPCTSTR)IDD_METHOD,
						hWnd, (DLGPROC)ChooseLockMethodProc );

	if( nRet == IDOK )__Comment ( "Choose Lock Method ended with IDOK" );
	else __Comment ( "Choose Lock Method ended with !IDOK...assumed IDCANCEL" );

	return nRet;
}



///////////////////////////////////////////////////////////////////////////////
///// Advanced Icon Options Box Caller Definition
//
static int SetAdvancedIconOptions(  HWND hWnd )
{
	int nRet = DialogBox( GetMyInstanceHandle(), (LPCTSTR)IDD_HI_ADVANCED,
							hWnd, (DLGPROC)SetAdvancedIconOptionsProc );
	
	if( nRet == IDOK )__Comment ( "Advanced Icon Options ended with IDOK" );
	else __Comment ( "Advanced Icon Options ended with !IDOK...assumed IDCANCEL" );

	return nRet;
}


static void EnableIconOptions( HWND hDlg, BOOL bEnable )
{
	EnableWindow( GetDlgItem( hDlg, ID_ICONHIDE_ADVANCE ), bEnable );
	EnableWindow( GetDlgItem( hDlg, IDC_ICON_HIDE_METHOD ),	bEnable );
	EnableWindow( GetDlgItem( hDlg, IDC_ICON_HIDE_METHOD2),	bEnable );
}

static void EnableHottrackOptions( HWND hDlg, BOOL bEnable )
{
	EnableWindow( GetDlgItem( hDlg, IDC_HOVER_TIME ), bEnable );
	EnableWindow( GetDlgItem( hDlg, IDC_TRACKMETHOD ), bEnable );
	EnableWindow( GetDlgItem( hDlg, IDC_TRACKMETHOD2 ), bEnable );
	EnableWindow( GetDlgItem( hDlg, IDC_TRACKMETHOD3 ), bEnable );
}

static void SetIconOption( HWND hDlg, unsigned int nOption )
{
	const unsigned int nFirst	= IDC_ICON_HIDE_METHOD;
	const unsigned int nLast	= IDC_ICON_HIDE_METHOD2;
	const unsigned int nTotal	= nLast - nFirst;
	
	//if( nOption <= nTotal ){
	
		CheckRadioButton( hDlg, nFirst, nLast,
			nFirst + nOption - 1 );

	//} 	
}

static int GetIconOption( HWND hDlg )
{
	const unsigned int nFirst	= IDC_ICON_HIDE_METHOD;
	const unsigned int nLast	= IDC_ICON_HIDE_METHOD2;

	for( unsigned int x = nFirst; x <= nLast; x++ )
	{
		if( IsDlgButtonChecked( hDlg, x ) )
			return ( x - nFirst + 1 );
	}

	return 0;
}


static void SetHotTrkOption( HWND hDlg, unsigned int nOption )
{
	const unsigned int nFirst	= IDC_TRACKMETHOD;
	const unsigned int nLast	= IDC_TRACKMETHOD3;
	const unsigned int nTotal	= nLast - nFirst;
	
	//if( nOption <= nTotal ){
	
		CheckRadioButton( hDlg, nFirst, nLast,
			nFirst + nOption - 1 );

	//}
}

static int GetHotTrkOption( HWND hDlg )
{
	const unsigned int nFirst	= IDC_TRACKMETHOD;
	const unsigned int nLast	= IDC_TRACKMETHOD3;
	
	for( unsigned int x = nFirst; x <= nLast; x++ )
	{
		if( IsDlgButtonChecked( hDlg, x ) )
			return ( x - nFirst + 1 );
	}

	return 0;
}



///////////////////////////////////////////////////////////////////////////////
///// Options Box Proc
//
LRESULT CALLBACK OptionsProc(HWND hDlg, UINT message, WPARAM wParam,
									LPARAM lParam)
{
	static COLORREF textcolor;

	switch (message)
	{
		case WM_INITDIALOG:
			{
			__Comment ( "Loading Options Box Resourses" );

			textcolor = ReadRGBSetting( "res_x86", "textcolor", "255,255,255" );

			char temp[MAX_PATH];
			
			sprintf( temp, "%d",
				ReadIntSetting( "res_x86", "HoverTime", HOVERTIME ) );
			SetWindowText( GetDlgItem( hDlg, IDC_HOVER_TIME ), temp );
			
			CheckDlgButton( hDlg, IDC_ICON_HIDE,
				( ReadIntSetting( "res_x86", "IHmethod", ICONHIDE_METHOD ) )
				? BST_CHECKED : BST_UNCHECKED );
			EnableIconOptions( hDlg, 
				IsDlgButtonChecked( hDlg, IDC_ICON_HIDE ) );
			

			
			CheckDlgButton( hDlg, IDC_HOT_TRACKING,
				( ReadIntSetting( "res_x86", "HoverEffect", HOVEREFFECT ) )
				? BST_CHECKED : BST_UNCHECKED );
			EnableHottrackOptions( hDlg, 
				IsDlgButtonChecked( hDlg, IDC_HOT_TRACKING ) );
			
			
			SetIconOption( hDlg, 
				ReadIntSetting( "res_x86", "IHmethod", ICONHIDE_METHOD ) );

			
			SetHotTrkOption( hDlg, 
				ReadIntSetting( "res_x86", "HoverEffect", HOVEREFFECT ) );
			
				
			CheckDlgButton( hDlg, ID_ADPOLL,
				( ReadIntSetting( "", "ADpoll", ACTIVEDESKTOP_POLLING ) )
				? BST_CHECKED : BST_UNCHECKED );

			CheckDlgButton( hDlg, IDC_LOGGING,
				( ReadIntSetting( "", "Logging", LOGGING ) )
				? BST_CHECKED : BST_UNCHECKED );

			CheckDlgButton( hDlg, IDC_SHOWICON,
				( ReadIntSetting( "", "TrayIcon", TRAYICON ) )
				? BST_CHECKED : BST_UNCHECKED );
						

			sprintf( temp, "Currently using: %s method",
				ReadIntSetting( "", "Lockmeth", LOCKMETHOD )
				? "Advanced" : "Brute Force" );
			SetWindowText( GetDlgItem( hDlg, IDC_METHODMESSAGE ), temp );

			EnableWindow( GetDlgItem( hDlg, IDC_LOCKMETHOD ), _IsRes_x86Loaded( ) );

			__AnimateWindow( hDlg, 200, AW_ACTIVATE|AW_BLEND );
			ShowWindow( hDlg, SW_SHOW );
			InvalidateRect( hDlg, 0, true );

			}
			
			return TRUE;


		case WM_COMMAND:

			switch( LOWORD( wParam ) )
			{
			case IDOK:
				{
				//Add save code here
					char time[MAX_PATH];
					GetWindowText( GetDlgItem( hDlg, IDC_HOVER_TIME ),
						time, MAX_PATH );

					int x = IsDlgButtonChecked( hDlg, IDC_ICON_HIDE );
					if( x )	x = GetIconOption( hDlg );
					else x = 0;
					WriteIntSetting( "res_x86", "IHmethod", x );
					
					x = IsDlgButtonChecked( hDlg, IDC_HOT_TRACKING );				
					if( x ) x = GetHotTrkOption( hDlg );
					else x = 0;
					WriteIntSetting( "res_x86", "HoverEffect", x );

					x = atoi( time );
					WriteIntSetting( "res_x86", "HoverTime", x );

					x = IsDlgButtonChecked( hDlg, ID_ADPOLL );
					WriteIntSetting( "", "ADpoll", x );

					x = IsDlgButtonChecked( hDlg, IDC_LOGGING );
					WriteIntSetting( "", "Logging", x );

					x = IsDlgButtonChecked( hDlg, IDC_SHOWICON );
					if( ReadIntSetting( "res_x86", "IHmethod", ICONHIDE_METHOD ) == 2 )
						x = 1;
					WriteIntSetting( "", "TrayIcon", x );

				}
			case IDCANCEL: 
				__Comment ( "Closing Options Box" );
				EndDialog(hDlg, LOWORD( wParam ));
				return TRUE;

			case IDC_LOCKMETHOD:
				if( ChooseLockMethod( hDlg ) == IDOK ){
					EnableWindow( GetDlgItem( hDlg, IDCANCEL ), false );

					char temp[MAX_PATH];					
					sprintf( temp, "Currently using: %s method",
						ReadIntSetting( "", "Lockmeth", LOCKMETHOD )
						? "Advanced" : "Brute Force" );
					SetWindowText( GetDlgItem( hDlg, IDC_METHODMESSAGE ), temp );

					EnableIconOptions( hDlg,						   
						( IsDlgButtonChecked( hDlg, IDC_ICON_HIDE ) ) );
					if( GetIconOption( hDlg ) == 1 )
						SetIconOption( hDlg, 0 );

				}
				break;
			case IDC_TEXTCOLOR:
				{
					DWORD dwCustClrs [16];
					for ( int i = 0; i < 16; i++ )		 
						dwCustClrs [i] = _WHITE;


					CHOOSECOLOR color;
					ZeroMemory( &color, sizeof( CHOOSECOLOR ) );
					color.lStructSize = sizeof( CHOOSECOLOR );
					color.Flags = CC_RGBINIT|CC_ENABLEHOOK;
					color.rgbResult = textcolor;
					color.hwndOwner = hDlg;
					color.lpCustColors = ( LPDWORD )dwCustClrs;
					color.lCustData = 0L;
					color.lpfnHook = CCHookProc;

					if( ChooseColor( &color ) )
					{
						textcolor = color.rgbResult;						
						WriteRGBSetting( "res_x86", "textcolor", textcolor );	
					}

				}
				break;

			case ID_ICONHIDE_ADVANCE:
				if( SetAdvancedIconOptions( hDlg ) == IDOK )
					EnableWindow( GetDlgItem( hDlg, IDCANCEL ), false );
				break;

			case IDC_ICON_HIDE:
				EnableIconOptions( hDlg,
					( IsDlgButtonChecked( hDlg, LOWORD( wParam ) ) ) );
				break;

			case IDC_HOT_TRACKING:
				EnableHottrackOptions( hDlg,
					( IsDlgButtonChecked( hDlg, LOWORD( wParam ) ) ) );
				break;

			case IDC_ICON_HIDE_METHOD2:
				CheckDlgButton( hDlg, IDC_SHOWICON, BST_CHECKED );
				break;

			}
			return TRUE;
		
	}
    return FALSE;
}	   



///////////////////////////////////////////////////////////////////////////////
///// Choose Lock Method Box Proc Definition
//
LRESULT CALLBACK ChooseLockMethodProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			{
			__Comment ( "Loading Lock Method Box Resourses" );
			
			//IDC_METHOD
			//IDC_METHOD2
			//IDC_TIMEOUT		

			char time[MAX_PATH];
			sprintf( time, "%d", 
				ReadIntSetting( "", "LockMeth1Time", LOCKMETHODTIME ) );
			SetWindowText( GetDlgItem( hDlg, IDC_TIMEOUT ), time );

			int meth = ReadIntSetting( "", "Lockmeth", LOCKMETHOD );
			CheckRadioButton( hDlg, IDC_METHOD, IDC_METHOD2,
				IDC_METHOD + meth );

			EnableWindow( GetDlgItem( hDlg, IDC_METHOD ), true );
			EnableWindow( GetDlgItem( hDlg, IDC_METHOD2 ), true );
			
			EnableWindow( GetDlgItem( hDlg, IDC_TIMEOUT ),
				( meth == 0 ) ? true : false );

			__AnimateWindow( hDlg, 200, AW_ACTIVATE|AW_BLEND );
			ShowWindow( hDlg, SW_SHOW );
			InvalidateRect( hDlg, 0, true );
			
			}
			return TRUE;


		case WM_COMMAND:

			switch( LOWORD(wParam) )
			{
			case IDOK:
				{
				char time[MAX_PATH];
				GetWindowText( GetDlgItem( hDlg, IDC_TIMEOUT ),
					time, MAX_PATH );

				int z = atoi( time );
				WriteSetting( "", "LockMeth1Time", &z );

				for( unsigned int x = IDC_METHOD; x <= IDC_METHOD2; x++ )
				{
					if( IsDlgButtonChecked( hDlg, x ) )
						z = ( x - IDC_METHOD );
				}
				WriteIntSetting( "", "Lockmeth", z );
				

				}
			case IDCANCEL: 
				__Comment ( "Closing Lock Method Box" );
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;

			case IDC_METHOD:
				EnableWindow( GetDlgItem( hDlg, IDC_TIMEOUT ), true );
				break;

			case IDC_METHOD2:
				EnableWindow( GetDlgItem( hDlg, IDC_TIMEOUT ), false );
				break;

			}	
			break;

		
	}
    return FALSE;
}
	   


///////////////////////////////////////////////////////////////////////////////
///// Advanced Icon Options Box Proc Definition
//
LRESULT CALLBACK SetAdvancedIconOptionsProc(HWND hDlg, UINT message,
												   WPARAM wParam,
												   LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			{
			__Comment ( "Loading Advanced Icon Options Box Resourses" );
			
			CheckDlgButton( hDlg, IDC_SHOWBUTTON,
				( ReadIntSetting( "res_x86", "IHButton", ICONHIDE_BUTTON ) )
				? BST_CHECKED : BST_UNCHECKED );
			
			SendMessage( GetDlgItem( hDlg, IDC_RESPONSIVE ),
				TBM_SETRANGEMIN, 1, 1);
			
			SendMessage( GetDlgItem( hDlg, IDC_RESPONSIVE ),
				TBM_SETRANGEMAX, 1, 1000);
			
			
			SendMessage( GetDlgItem( hDlg, IDC_RESPONSIVE ),
				TBM_SETPOS, 1,
				ReadIntSetting( "res_x86", "IHRespon", ICONHIDE_RESPON ) );

			__AnimateWindow( hDlg, 200, AW_ACTIVATE|AW_BLEND );
			ShowWindow( hDlg, SW_SHOW );
			InvalidateRect( hDlg, 0, true );

			}
			return TRUE;


		case WM_COMMAND:

			switch( LOWORD(wParam) )
			{
			case IDOK:
				{

				int x = (int)SendMessage( GetDlgItem( hDlg, IDC_RESPONSIVE ),
						TBM_GETPOS, 0, 0l);
				WriteIntSetting( "res_x86", "IHRespon", x );
				

				x = IsDlgButtonChecked( hDlg, IDC_SHOWBUTTON );
				WriteIntSetting( "res_x86", "IHButton", x );
				

				}

			case IDCANCEL: 
				__Comment ( "Closing Advanced Icon Options Box" );
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;			

			}	
			break;

		
	}
    return FALSE;
}
