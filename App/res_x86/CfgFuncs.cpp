///////////////////////////////////////////////////////////////////////////////
//// Configuration Functions
//
#include "stdafx.h"
#include "CfgFuncs.h"



COLORREF rgb_from_text( string txt_rgb )
{
	
	int R,G,B;
	char color[15];
	char * buf;

	txt_rgb.copy( color, 15 );
	
	buf = strtok( color, "," );	
	R = atoi( buf );

	buf = strtok( 0, "," );
	G = atoi( buf );

	buf = strtok( 0, "," );
	B = atoi( buf );

	return RGB(R,G,B);
}












string rgb_to_text( COLORREF c )//char[15]
{
	char color_rgb[15];
	sprintf( color_rgb, "%d,%d,%d", GetRValue(c), GetGValue(c), GetBValue(c) );

	string color = color_rgb;

	return color;
}










DWORD ReadIntSetting( string section, string lable, DWORD def )
{	
	DWORD x = 0;
	if( ReadSetting( (char *)section.c_str(), (char *)lable.c_str(), &x, REG_DWORD ) == false )
		return def;

	return x;
}









void WriteIntSetting( string section, string lable, int data )
{
	WriteSetting( (char *)section.c_str(), (char *)lable.c_str(), &data );
}











DWORD ReadRGBSetting( string section, string lable, string def )
{	
	COLORREF x = rgb_from_text( def );
	DWORD s=2;
	char *n = new char[50];
	memset( n, 0, 50 );
	
	s = ReadSetting( (char *)section.c_str(), (char *)lable.c_str(), n, REG_SZ );
	if( s > 1 )
	{
		delete n;
		char *n = new char[s];
		memset( n, 0, 50 );
		s = ReadSetting( (char *)section.c_str(), (char *)lable.c_str(), n, REG_SZ );
	}
	

	if(s == TRUE)
		x = rgb_from_text( n );	

	return x;
}




