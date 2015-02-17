#include "xstream.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------------
//  TFileStream
//---------------------------------------------------------------------------------
TFileStream :: TFileStream() : TStream( this ), fstream()
{
}
//---------------------------------------------------------------------------------
TFileStream :: TFileStream( const char * filename, int mode ) :  TStream( this ), fstream( filename, (ios::openmode)mode )
{
}
//---------------------------------------------------------------------------------
//  TStringStream
//---------------------------------------------------------------------------------
TStringStream :: TStringStream( int mode ) : TStream( this ), stringstream( (ios::openmode)(mode & ~omBinary) )
{
}
//---------------------------------------------------------------------------------
TStringStream :: TStringStream( const TString &str, int mode ) : TStream( this ), stringstream( str, (ios::openmode)(mode & ~omBinary) )
{
}
//---------------------------------------------------------------------------------
TString TStringStream :: ReadString( long count, char delim )
{
    TString ret;
    if( count )
    {
        char *c = new char[count + 1];
        if ( c == NULL )
            return ret;
        if( delim )
            getline ( c, count, delim );
        else
            getline ( c, count );
        ret = c;
        delete [] c;
    }
    else
        *this >> ret;
    return ret;
};
//---------------------------------------------------------------------------------
//  TMemoryStream
//---------------------------------------------------------------------------------
TMemoryStream :: TMemoryStream( int mode ) : TStream( this ), stringstream( (ios::openmode)(mode | omBinary) )
{
}
//---------------------------------------------------------------------------------
TMemoryStream :: TMemoryStream( const void *mem, int len, int mode ) : TStream( this ), stringstream( (ios::openmode)(mode | omBinary) )
{
    write ( (const char *)mem, len );
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------------
