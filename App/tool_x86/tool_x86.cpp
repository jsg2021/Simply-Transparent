// tool_x86.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "tool_x86.h"
#include <string>
using namespace std;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}






/*/////////////////////////////////////////////////////////////////////////////
///// Update( )
//
	make sure app_url is allocated and big enough!
*/
TOOL_X86_API BOOL Update( int major, int minor, char *app_url )
{
	int new_major, new_minor;
	BOOL ret = false;
	DWORD bytes_read;
	string filedata[2];
	
	const DWORD buffer_size = 65535;	
	LPSTR buffer = new char[buffer_size];

	string url[2] =
	{ 
		"http://www.jonathangrimes.com/versions.php?productID=SimplyTransparent",
		"http://www.jonathangrimes.com/versions.php?productID=SimplyTransparent&url=true"
	};

	HINTERNET file_session, inet_session = 
		InternetOpen( "JonathanGrimes: Simply Transparent Tool_x86",
			INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0 );

	if( inet_session )
	{
		for(int i=0; i<2; i++)
		{
			file_session = 
				InternetOpenUrl( inet_session, url[i].c_str( ), NULL, 0, 0, 0 );
		

			if( file_session )
			{
				bytes_read = 1;

				while( bytes_read > 0 )
				{
					bytes_read = 0;
					memset( buffer, 0, (buffer_size*sizeof(char)) );

					if( !InternetReadFile( file_session, buffer, buffer_size, &bytes_read ) )
						ErrorMessage( GetLastError( ) );
					else
					{
						if( bytes_read != 0 )						
							filedata[i] += buffer;
					}
				}
				

				InternetCloseHandle(  file_session );

			}
			else
			{
				MessageBox( 0, "Could not connect to the server", "SimplyTransparent : Tool_x86", 0 );
				break;
			}
		}

		InternetCloseHandle(  inet_session );

	}
	else
	{	
		MessageBox( 0, "Could not open an Internet session", "SimplyTransparent : Tool_x86", 0 );
	}

	delete buffer;

///////////////////////////////////////////////////////////////////
	//Do parsing here...or outsource to function
			

	if(!filedata[0].empty())
	{
		int dot = str_find( filedata[0], ".", 0);
		if( dot > -1)
		{
			string t = str_left( filedata[0], dot );
			new_major = atoi( t.c_str( ) );
			
			t = str_right( filedata[0], dot+1 );
			new_minor = atoi( t.c_str( ) );
			
		}
		else
		{
			new_major = atoi( filedata[0].c_str( ) );
			new_minor = 0;
		}

		if( new_major > major ) ret = true;
		else if( new_major == major && new_minor > minor ) ret = true;
		
	}

	if(!filedata[1].empty() && ret && app_url)
	{
		if(strlen( app_url ) >= filedata[1].length( ) )
			sprintf( app_url,"%s", filedata[1].c_str( ) );		
			
	}

///////////////////////////////////////////////////////////////////

	return ret;
}
