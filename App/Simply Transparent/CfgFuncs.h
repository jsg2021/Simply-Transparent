///////////////////////////////////////////////////////////////////////////////
//// Configuration Functions header file (CfgFuncs.h)
//

#ifndef ____CFG_FUNCS_H___
#define ____CFG_FUNCS_H___

#include "resource.h"
#include <winreg.h>

COLORREF rgb_from_text( string txt_rgb );
string rgb_to_text( COLORREF c );

DWORD ReadIntSetting( string section, string lable, DWORD def );
void WriteIntSetting( string section, string lable, int data );

DWORD ReadRGBSetting( string section, string lable, string def );
void WriteRGBSetting( string section, string lable, COLORREF data );

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
		
		__ErrorMessage( "WriteSetting( )" );

	}

	return ret;
}













template <class T>
inline T * ReadSetting( char section[], char lable[], T *def, T *data, bool &alloced )
{
	LONG res = 0;

	try {
	
	const unsigned int string_size = 50;

	DWORD disposition, size, type = ( sizeof( *def ) == sizeof( char ) ) ? REG_SZ : REG_DWORD;
	
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
			REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE, 0, &key, &disposition );
		if( ERROR_SUCCESS == res )
		{	
			res = RegQueryValueEx(key, lable, 0, &type, 0, &size );
			if( ERROR_SUCCESS == res )
			{
				if( type == REG_SZ )
				{
					alloced = true;
					data = new T[size/sizeof(T)];
				}
				
				res = RegQueryValueEx(key, lable, 0, &type, (BYTE*)data,
					&size );

				res = ( ERROR_SUCCESS == res ) ? 1 : 0;

				if(res != 1)
				{
					if(type == REG_SZ)
						delete data;

					data = def;
				}					
			}
			else data = def;
		}
		else data = def;		
	}
	else data = def;

	RegCloseKey( softkey );
	RegCloseKey( key );

	}catch(...){
		
		__ErrorMessage( "T * ReadSetting( )" );

	}

	return (res == 1) ? data : def;
}

#endif //____CFG_FUNCS_H___