///////////////////////////////////////////////////////////////////////////////
//// Configuration Functions header file (CfgFuncs.h)
//

#ifndef ____CFG_FUNCS_H___
#define ____CFG_FUNCS_H___

#include "resource.h"
#include <winreg.h>
#include <string>
using namespace std;

COLORREF rgb_from_text( string txt_rgb );
string rgb_to_text( COLORREF c );

DWORD ReadIntSetting( string section, string lable, DWORD def );
void WriteIntSetting( string section, string lable, int data );

DWORD ReadRGBSetting( string section, string lable, string def );

template <class T>
inline int WriteSetting( char section[], char lable[], T *data )
{
	int ret = 1;

	try{	

	const unsigned int string_size = 50;

	DWORD disposition;
	LONG res = 0;
	HKEY softkey = NULL, key = NULL;
	char subkey[MAX_PATH];
	char appname[string_size];
	char author[string_size];

	LoadString( GetMyInstanceHandle( ), IDS_APP_TITLE,
		appname, string_size );

	LoadString( GetMyInstanceHandle( ), IDS_REGISTRY_KEY,
		author, string_size );
	
	sprintf( subkey, "%s\\%s\\%s", author, appname, section );

	res = RegOpenKeyEx( HKEY_CURRENT_USER, "software", 0, KEY_WRITE, 
		&softkey );
	if( ERROR_SUCCESS == res )
	{
		res = RegCreateKeyEx( softkey, subkey, 0, "key",
			REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &key, &disposition );
		if( ERROR_SUCCESS == res )
		{	
			res = RegSetValueEx(key, lable, 0, 
				( sizeof( *data ) == sizeof( char ) ) ? REG_SZ : REG_DWORD,
				(BYTE*)data, ( sizeof( *data ) == sizeof( char ) ) ? strlen( (const char *) data ) : sizeof( data ) );
			if( ERROR_SUCCESS == res )
			{
				ret = 0;
			}
		}			
	}

	RegCloseKey( softkey );
	RegCloseKey( key );

	}catch(...){
		
		__Message( "WriteSetting( )" );

	}

	return ret;
}













template <class T>
inline DWORD ReadSetting( char section[], char lable[], T *data, DWORD type )
{
	LONG res = 0;	
	const unsigned int string_size = 100;
	DWORD size = sizeof(data);	
	HKEY key = NULL;
	char subkey[MAX_PATH];
	static char appname[string_size] = "Simply Transparent";
	static char author[string_size] = "JonathanGrimes";
	/*static bool loaded_strings = false;
	if( !loaded_strings )
	{
		LoadString( GetMyInstanceHandle( ), IDS_APP_TITLE, appname, string_size );
		LoadString( GetMyInstanceHandle( ), IDS_REGISTRY_KEY, author, string_size );
		loaded_strings = true;
	}*/

	/*
	sprintf( subkey, "Software\\%s\\%s\\%s", author, appname, section );
	
	char debug[512];
	sprintf( debug, "%s\n%s\n%s", section, lable, subkey );*/
	
	

	
	res = RegOpenKeyEx( HKEY_CURRENT_USER, subkey, 0, KEY_QUERY_VALUE, &key );
	if( res != ERROR_SUCCESS )
		return false;

	res = RegQueryValueEx(key, lable, 0, &type, (BYTE*)data, &size );
	RegCloseKey( key );
	if(res != ERROR_SUCCESS)
	{
		if( res == ERROR_MORE_DATA )
			return size;

		//ErrorMessage( res );		
		//MessageBox( 0, debug, " ", MB_OK );
	
		return false;			
	}
	
	return true;
}

#endif //____CFG_FUNCS_H___