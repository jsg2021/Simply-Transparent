///////////////////////////////////////////////////////////////////////////////
//// Configuration Functions
//
#include "stdafx.h"
#include "SimplyTransparent.h"
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

	try{

		DWORD *n = NULL;
		bool bAlloced = false;
		
		n = (DWORD *)ReadSetting( (char *)section.c_str(), (char *)lable.c_str(), &def, &x, bAlloced );
		
		x = *n;
		

	}
	catch(...){
		 __ErrorMessage( "ReadIntSetting( )" );
	}

	return x;
}









void WriteIntSetting( string section, string lable, int data )
{
	WriteSetting( (char *)section.c_str(), (char *)lable.c_str(), &data );
}











DWORD ReadRGBSetting( string section, string lable, string def )
{	
	COLORREF x;
	
	try{

		char *n = NULL;
		bool bAlloced = false;

		n = ReadSetting( (char *)section.c_str(), (char *)lable.c_str(), (char *)def.c_str(), n, bAlloced );


		x = rgb_from_text( n );
		
		
		if(bAlloced)
			delete n;

	}
	catch(...){
		__ErrorMessage( "ReadRGBSetting( )" );
	}

	return x;
}








void WriteRGBSetting( string section, string lable, COLORREF data )
{	
	try{
		string rgb = rgb_to_text( data );

		WriteSetting( (char *)section.c_str(), (char *)lable.c_str(), rgb.c_str( ) );

	}
	catch(...){
		__ErrorMessage( "ReadRGBSetting( )" );
	}
}