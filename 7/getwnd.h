


#ifndef __get_wnd_h_____
#define __get_wnd_h_____





///////////////////////////////////////////////////////////////////////////////
////
//
inline HWND __GetProgramManager( )
{
	HWND   hwnd = GetDesktopWindow( );
	hwnd = FindWindowEx( hwnd, 0, "Progman", "Program Manager" );
	return hwnd;
}





///////////////////////////////////////////////////////////////////////////////
////
//
inline HWND __GetShellDefView( )
{
	HWND   hwnd = __GetProgramManager( );
	if( hwnd )
	{

	// look among progman's children for the shell default view
		hwnd = FindWindowEx( hwnd, 0, "SHELLDLL_DefView", NULL );
		
		if( !hwnd )
		{
			#ifdef _SHOW_DEBUG_MESSAGES
			__Message( "Can't Find SHELLDLL_DefView" );
			#endif	
		}
	}

	return hwnd;
}





///////////////////////////////////////////////////////////////////////////////
////
//
inline HWND __GetTrayWnd( )
{
	HWND   hwnd = GetDesktopWindow( );
	hwnd = FindWindowEx( hwnd, 0, "Shell_TrayWnd", "" );
	return hwnd;
}








///////////////////////////////////////////////////////////////////////////////
////
//
inline HWND __GetDesktopListView( )
{
	HWND   hwnd = __GetShellDefView();
	if(hwnd!=NULL)
	{
		// then, get the listview
		hwnd = FindWindowEx( hwnd, 0, "SysListView32", NULL );
	}	
	
#ifdef _SHOW_DEBUG_MESSAGES
	if( !hwnd )
		__Message( "Simply Transparent can not find the Desktop's Handle!" );
#endif

	return hwnd;
}







#endif //__get_wnd_h_____