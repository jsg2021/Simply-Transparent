// stdafx.cpp
#include "stdafx.h"


void ErrorMessage( DWORD error )
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

	MessageBox( 0, (char *)(LPCTSTR)lpMsgBuf, "tool error", MB_OK );
		
	LocalFree( lpMsgBuf );
}


int str_find( string str, const char *c, int start )
{
	int length = str.length( );
	if ( start > length)
		return -1;

	char *res = strstr( str.c_str( ) + start, c);

	
	return ( res == NULL ) ? -1 : (int)( res - str.c_str( ) );
}

int str_reversefind( string str, const char c )
{
	char *res = strrchr( str.c_str( ), c );

	return (res == NULL) ? -1 : (int)(res - str.c_str( ) );
}

string str_mid( string str, int first, int count )
{
	if (first < 0) first = 0;
	
	if ( unsigned( first + count ) > str.length( ) || (count < 0) )
		count = str.length( ) - first;

	if ( unsigned( first ) > str.length( ) )
		count = 0;

	ASSERT( first >= 0 );
	ASSERT( unsigned( first + count ) <= str.length( ) );

	int size = count;

	char *dest = new char[size+1];
	memset( dest, '\0', size+1 );
	memcpy(dest, str.c_str( )+first, size );

	string temp;
	temp = dest;
	delete dest;

	return temp;
}

string str_right( string str, int start )
{
	if ( start < 0 ) start = 0;
	if ( unsigned(start) >= str.length( ) )
		start = str.length( );		
	
	int size = str.length( ) - start;

	char *dest = new char[size+1];
	memset( dest, '\0', size+1 );
	memcpy( dest, str.c_str( )+start, size );

	string temp;
	temp = dest;
	delete dest;

	return temp;
}

string str_left( string str, int end )
{
	if ( end < 0 ) end = 0;
	if ( unsigned(end) >= str.length( ) )
		end = str.length( );

	char *dest = new char[end+1];
	memset( dest, '\0', end+1 );
	memcpy( dest, str.c_str( ), end );

	string temp;
	temp = dest;
	delete dest;

	return temp;
}