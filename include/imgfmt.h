//---------------------------------------------------------------------------
#ifndef imagesfmtH
#define imagesfmtH

#include "xstream.h"
#include "graphics.h"
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
class TXBMImage : public TBitmap
{
	DECLARE_DYNAMIC_CLASS(TXBMImage);

private:

protected:

public:
   __fastcall TXBMImage();
   __fastcall TXBMImage(int width, int height, char *data);
   virtual ~TXBMImage();
   virtual void Load(const TString &filename);
           void Load(int width, int height, char *data);
   virtual void Save(const TString &filename);
};
//---------------------------------------------------------------------------
class TXPMImage : public TBitmap
{
	DECLARE_DYNAMIC_CLASS(TXPMImage);

private:

protected:

public:
   __fastcall TXPMImage();
   __fastcall TXPMImage(int width, int height, char **data);
   virtual ~TXPMImage();
   virtual void Load(const TString &filename);
           void Load(int width, int height, char **data);
   virtual void Save(const TString &filename);
};
//---------------------------------------------------------------------------
class TJPGImage : public TBitmap
{
	DECLARE_DYNAMIC_CLASS( TJPGImage );
	DECLARE_PROPERTY( TJPGImage );

private:
    void Init();
    __fastcall void Assign(const TJPGImage &bmp);
	uint_t m_Quality;
	void set_Quality(uint_t value);
	void read_jpg_file( const char *filename, unsigned char *inbuffer, unsigned long insize );
	void write_jpg_file( const char *filename, byte_t **outbuffer, ulong_t *outsize );

protected:

public:
	__fastcall TJPGImage();
   	__fastcall TJPGImage(TJPGImage &img);
   	__fastcall TJPGImage(TBitmap &img);
   	__fastcall TJPGImage(byte_t *data, ulong_t count);
   	virtual ~TJPGImage();
   	__fastcall const TJPGImage & operator = ( const TJPGImage &img );
   	const TJPGImage & operator = ( TBitmap &value )
   	{
   		Assign(value);
   		return *this;
   	};

   virtual void Load( const TString &filename );
   virtual void Load( TStream &stream );
    inline void Load( byte_t *data, ulong_t count ) { read_jpg_file(NULL, data, count); };
   virtual void Save( const TString &filename );
    virtual void Save( TStream &stream );

   property (m, uint_t, Quality);
};
//---------------------------------------------------------------------------
class TBMPImage : public TBitmap
{
	DECLARE_DYNAMIC_CLASS(TBMPImage);

private:
	void write_bmp_file (const char *filename);

protected:

public:
   __fastcall TBMPImage();
   __fastcall TBMPImage(int width, int height, char *data);
   virtual ~TBMPImage();
   virtual void Load(const TString &filename);
           void Load(int width, int height, char *data);
   virtual void Save(const TString &filename);
};
//---------------------------------------------------------------------------
class TICOImage : public TBitmap
{
	DECLARE_DYNAMIC_CLASS(TICOImage);

private:
	void write_bmp_file (const char *filename);

protected:

public:
   __fastcall TICOImage();
   __fastcall TICOImage(int width, int height, char *data);
   virtual ~TICOImage();
   virtual void Load(const TString &filename);
           void Load(int width, int height, char *data);
   virtual void Save(const TString &filename);
};
//---------------------------------------------------------------------------
//   GIF Image
//---------------------------------------------------------------------------
class TGIFImage : public TBitmap
{
	DECLARE_DYNAMIC_CLASS(TGIFImage);

private:
//	void write_bmp_file (const char *filename);

protected:

public:
   TGIFImage();
   __fastcall TGIFImage(int width, int height, char *data);
   virtual ~TGIFImage();
   virtual void Load(const TString &filename);
//           void Load(int width, int height, char *data);
//   virtual void Save(const TString &filename);
};
//---------------------------------------------------------------------------
//   PNG Image
//---------------------------------------------------------------------------
class TPNGImage : public TBitmap
{
	DECLARE_DYNAMIC_CLASS(TPNGImage);

private:
    void read_png(const char *filename);

protected:

public:
   TPNGImage();
   __fastcall TPNGImage(int width, int height, char *data);
   virtual ~TPNGImage();
   virtual void Load(const TString &filename);
//           void Load(int width, int height, char *data);
//   virtual void Save(const TString &filename);
};
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
#endif
