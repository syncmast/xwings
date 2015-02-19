//---------------------------------------------------------------------------
#include <stdio.h>
#include <X11/xpm.h>
#include <jpeglib.h>
#include <png.h>
#include "gif.h"
#include <setjmp.h>
#include "imgfmt.h"
#include "rtti.h"
#include "application.h"
#include "xstream.h"
#include "vxutil.h"
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
void SO_ENTRYPOINT initialization()
{
	RegisterClass(TXBMImage);
	RegisterClass(TXPMImage);
	RegisterClass(TJPGImage);
	RegisterClass(TBMPImage);
	RegisterClass(TGIFImage);
	RegisterClassIdent(TXBMImage, "xbm");
	RegisterClassIdent(TXPMImage, "xpm");
	RegisterClassIdent(TJPGImage, "jpg");
	RegisterClassIdent(TBMPImage, "bmp");
	RegisterClassIdent(TGIFImage, "gif");
}

void SO_EXITPOINT finalization()
{
}
//---------------------------------------------------------------------------
//  TXBMImage
//---------------------------------------------------------------------------
TXBMImage :: TXBMImage()
{

}
//---------------------------------------------------------------------------
TXBMImage :: TXBMImage(int width, int height, char *data)
{
   	Load(width, height, data);
}
//---------------------------------------------------------------------------
TXBMImage :: ~TXBMImage()
{
}
//---------------------------------------------------------------------------
void TXBMImage :: Load(const TString &filename)
{
	int x_hot_return, y_hot_return;
   	HBITMAP bitmap_return;
   	uint_t width_return, height_return;
   	int result;

   	if(!Empty)
    	Release();

   	result = XReadBitmapFile(Application.Handle, Screen.Root,
                     filename.c_str(), &width_return, &height_return,
                     &bitmap_return, &x_hot_return, &y_hot_return);
   	if(result == BitmapSuccess)
   	{
      	TBitmap :: LoadImage(bitmap_return);
      	HotSpot.x = x_hot_return;
      	HotSpot.y = y_hot_return;
   	}
}
//---------------------------------------------------------------------------
void TXBMImage :: Save(const TString &filename)
{
   int result;

   switch (HandleType)
   {
     case bmDIB:
		 HandleType = bmDDB;
     case bmDDB:
         result = XWriteBitmapFile(Application.Handle, filename.c_str(), m_Handle,
            					   m_Width, m_Height, HotSpot.x, HotSpot.y);
         break;
     case bmEmpty:
         break;
   }
}
//---------------------------------------------------------------------------
void TXBMImage :: Load(int width, int height, char *data)
{
   HBITMAP hbmp;

   if(!Empty)
      Release();

   hbmp = XCreateBitmapFromData(Application.Handle, Screen.Root,
   										 data, width, height);
   if(hbmp != None)
      	TBitmap :: LoadImage(hbmp);
}
//---------------------------------------------------------------------------
//  TXPMImage
//---------------------------------------------------------------------------
TXPMImage :: TXPMImage()
{
}
//---------------------------------------------------------------------------
TXPMImage :: TXPMImage(int width, int height, char **data)
{
   Load(width, height, data);
}
//---------------------------------------------------------------------------
TXPMImage :: ~TXPMImage()
{
}
//---------------------------------------------------------------------------
void TXPMImage :: Load(const TString &filename)
{
	int result;
   	HBITMAP bmp, mask;
   	XpmAttributes *attrib = new XpmAttributes();

   	if(!Empty)
    	Release();

   	result = XpmReadFileToPixmap(Application.Handle, Screen.Root,
   								(char*)filename.c_str(), &bmp, &mask, attrib);
   	if(result == XpmSuccess)
   	{
      	TBitmap :: LoadImage(bmp, mask);
      	HotSpot.x = attrib->x_hotspot;
      	HotSpot.y = attrib->y_hotspot;
      	XpmFreeAttributes(attrib);
      	XFreePixmap(Application.Handle, bmp);
      	if(mask)
      		XFreePixmap(Application.Handle, mask);
   	}
   	delete attrib;
}
//---------------------------------------------------------------------------
void TXPMImage :: Save(const TString &filename)
{
   XpmAttributes attrib;
   int result;

   attrib.valuemask = XpmHotspot | XpmSize | XpmDepth;
   attrib.x_hotspot = HotSpot.x;
   attrib.y_hotspot = HotSpot.y;
   attrib.width = m_Width;
   attrib.height = m_Height;
   attrib.depth = m_PixelFormat.Depth;
   switch (HandleType)
   {
     case bmDIB:
		 result = XpmWriteFileFromImage(Application.Handle, (char*)filename.c_str(),
		 		  m_Data, m_MaskData, &attrib);
		 break;
     case bmDDB:
         result = XpmWriteFileFromPixmap(Application.Handle, (char*)filename.c_str(),
         		  m_Handle, m_MaskHandle, &attrib);
         break;
     case bmEmpty:
         break;
   }
}
//---------------------------------------------------------------------------
void TXPMImage :: Load(int width, int height, char **data)
{
	int result;
   	HBITMAP bmp, mask;
   	XpmAttributes *attrib = new XpmAttributes();

   	if(!Empty)
    	Release();

   	result = XpmCreatePixmapFromData(Application.Handle, Screen.Root,
					 data, &bmp, &mask, attrib);
   	if(result == XpmSuccess)
   	{
      	TBitmap :: LoadImage(bmp, mask);
      	HotSpot.x = attrib->x_hotspot;
      	HotSpot.y = attrib->y_hotspot;
      	XpmFreeAttributes(attrib);
      	XFreePixmap(Application.Handle, bmp);
      	if(mask)
      		XFreePixmap(Application.Handle, mask);
   	}
   	delete attrib;
}
//---------------------------------------------------------------------------
//  TJPGImage
//---------------------------------------------------------------------------

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */
  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

static void my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}
//---------------------------------------------------------------------------
TJPGImage :: TJPGImage()
{
	Init();
}
//---------------------------------------------------------------------------
__fastcall TJPGImage :: TJPGImage(TJPGImage &img)
{
//	Init();
//	*this = img;
    Assign(img);
}
//---------------------------------------------------------------------------
__fastcall TJPGImage :: TJPGImage(TBitmap &img)
{
//	Init();
//	*this = img;
    Assign(img);
}
//---------------------------------------------------------------------------
__fastcall TJPGImage :: TJPGImage(byte_t *data, ulong_t count)
{
	Init();
   	read_jpg_file(NULL, data, count);
}
//---------------------------------------------------------------------------
TJPGImage :: ~TJPGImage()
{
}
//---------------------------------------------------------------------------
void TJPGImage :: Init()
{
    m_Quality = 80;
}
//---------------------------------------------------------------------------
__fastcall void TJPGImage :: Assign(const TJPGImage &img)
{
    m_Quality = img.m_Quality;
}
//---------------------------------------------------------------------------
__fastcall const TJPGImage & TJPGImage :: operator = ( const TJPGImage &img )
{
    if(this == &img)
		return *this;
	TBitmap :: operator=(img);
	Assign(img);
    return *this;
}
//---------------------------------------------------------------------------
void TJPGImage :: set_Quality(uint_t value)
{
	if(m_Quality == value)
		return;
	m_Quality = TRIMVAL(value, 10, 100);
}
//---------------------------------------------------------------------------
void TJPGImage :: Load( const TString &filename )
{
    read_jpg_file(filename.c_str(), NULL, 0);
}
//---------------------------------------------------------------------------
void TJPGImage :: Load( TStream &stream )
{
    long len = stream.Size;
    byte_t *buf = new byte_t[len];
    stream.Read(buf, len);
    read_jpg_file(NULL, buf, len);
    delete[] buf;
}
//---------------------------------------------------------------------------
void TJPGImage :: read_jpg_file( const char *filename, unsigned char *inbuffer, unsigned long insize )
{
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;

    FILE * infile;
    int depth;
    int row_stride;		/* physical row width in output buffer */

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    if (setjmp(jerr.setjmp_buffer))
    {
        jpeg_destroy_decompress(&cinfo);
        return;
    }
    jpeg_create_decompress(&cinfo);
    if( filename )
    {
        if ((infile = fopen(filename, "rb")) == NULL)
        {
            fprintf(stderr, "can't open %s\n", filename);
            jpeg_destroy_decompress(&cinfo);
            return;
        }
        jpeg_stdio_src(&cinfo, infile);
    }
    else
        jpeg_mem_src(&cinfo, inbuffer, insize);

  (void) jpeg_read_header(&cinfo, TRUE);
  (void) jpeg_start_decompress(&cinfo);
  /* JSAMPLEs per row in output buffer */
  row_stride = cinfo.output_width * cinfo.output_components;
  /* Make a one-row-high sample array that will go away when done with image */
//  buffer = (*cinfo.mem->alloc_sarray)
//		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
  depth = cinfo.output_components * 8;
  TPixelFormat a((TPixelSize)depth);
//  a.Depth = TPixelSize(depth);
  a.ByteOrder = MSBFirst;
  	Create(cinfo.output_width, cinfo.output_height, bmDIB, &a);
	byte_t *rp = (byte_t *)m_Data->data;
  while (cinfo.output_scanline < cinfo.output_height)
  {
    /* jpeg_read_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could ask for
     * more than one scanline at a time if that's more convenient.
     */
    (void) jpeg_read_scanlines(&cinfo, &rp, 1);
    rp += row_stride;
  }

  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  if( filename )
    fclose(infile);

  /* At this point you may want to check to see whether any corrupt-data
   * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
   */

  return;

/*
 * SOME FINE POINTS:
 *
 * In the above code, we ignored the return value of jpeg_read_scanlines,
 * which is the number of scanlines actually read.  We could get away with
 * this because we asked for only one line at a time and we weren't using
 * a suspending data source.  See libjpeg.doc for more info.
 *
 * We cheated a bit by calling alloc_sarray() after jpeg_start_decompress();
 * we should have done it beforehand to ensure that the space would be
 * counted against the JPEG max_memory setting.  In some systems the above
 * code would risk an out-of-memory error.  However, in general we don't
 * know the output image dimensions before jpeg_start_decompress(), unless we
 * call jpeg_calc_output_dimensions().  See libjpeg.doc for more about this.
 *
 * Scanlines are returned in the same order as they appear in the JPEG file,
 * which is standardly top-to-bottom.  If you must emit data bottom-to-top,
 * you can use one of the virtual arrays provided by the JPEG memory manager
 * to invert the data.  See wrbmp.c for an example.
 *
 * As with compression, some operating modes may require temporary files.
 * On some systems you may need to set up a signal handler to ensure that
 * temporary files are deleted if the program is interrupted.  See libjpeg.doc.
 */

}
//---------------------------------------------------------------------------
void TJPGImage :: Save( const TString &filename )
{
	TPixelFormat pf;
	pf.Depth = m_PixelFormat.Depth;
	pf.ByteOrder = MSBFirst;
	pf.BitOrder = m_PixelFormat.BitOrder;
	pf.ColorMode = m_PixelFormat.ColorMode;
	PixelFormat = pf;
   	switch (HandleType)
   	{
    	case bmDDB:
        	HandleType = bmDIB;
     	case bmDIB:
			write_jpg_file( filename.c_str(), NULL, NULL );
         	break;
     	case bmEmpty:
         	break;
   }
}
//---------------------------------------------------------------------------
void TJPGImage :: Save( TStream &stream )
{
	TPixelFormat pf;
	pf.Depth = m_PixelFormat.Depth;
	pf.ByteOrder = MSBFirst;
	pf.BitOrder = m_PixelFormat.BitOrder;
	pf.ColorMode = m_PixelFormat.ColorMode;
	PixelFormat = pf;
	byte_t *buf = NULL;
	ulong_t count = 0;
   	switch (HandleType)
   	{
    	case bmDDB:
        	HandleType = bmDIB;
     	case bmDIB:
			write_jpg_file( NULL, &buf, &count );
			stream.Write( buf, count );
			free( buf );
         	break;
     	case bmEmpty:
         	break;
   }
}
//---------------------------------------------------------------------------
void TJPGImage :: write_jpg_file( const char *filename, unsigned char **outbuffer, unsigned long *outsize )
{
	struct jpeg_compress_struct cinfo;
  	struct jpeg_error_mgr jerr;

  	FILE * outfile;
  	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
  	int row_stride;		/* physical row width in image buffer */

  	cinfo.err = jpeg_std_error(&jerr);
  	jpeg_create_compress(&cinfo);

    if( filename )
    {
        if( (outfile = fopen(filename, "wb") ) == NULL )
        {
            fprintf(stderr, "can't open %s\n", filename);
            return;
        }
        jpeg_stdio_dest(&cinfo, outfile);
    }
    else
    {
        jpeg_mem_dest(&cinfo, outbuffer, outsize);
    }

  /* First we supply a description of the input image.
   * Four fields of the cinfo struct must be filled in:
   */
  	cinfo.image_width = m_Width; 	/* image width and height, in pixels */
  	cinfo.image_height = m_Height;
  	cinfo.input_components = m_PixelFormat.Depth / 8; /* # of color components per pixel */
  	switch(cinfo.input_components)
  	{
  		case 1: cinfo.in_color_space = JCS_GRAYSCALE; break;
  		case 3: cinfo.in_color_space = JCS_RGB; break;
  		default: cinfo.in_color_space = JCS_GRAYSCALE;
  	}

  /* Now use the library's routine to set default compression parameters.
   * (You must set at least cinfo.in_color_space before calling this,
   * since the defaults depend on the source color space.)
   */
  	jpeg_set_defaults(&cinfo);
  /* Now you can set any non-default parameters you wish to.
   * Here we just illustrate the use of quality (quantization table) scaling:
   */
  	jpeg_set_quality(&cinfo, m_Quality, TRUE);

  /* TRUE ensures that we will write a complete interchange-JPEG file.
   * Pass TRUE unless you are very sure of what you're doing.
   */
  	jpeg_start_compress(&cinfo, TRUE);

  /* Here we use the library's state variable cinfo.next_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   * To keep things simple, we pass one scanline per call; you can pass
   * more if you wish, though.
   */
  	row_stride = cinfo.image_width * cinfo.input_components;/* JSAMPLEs per row in image_buffer */
	char *buffer = m_Data->data;
  	while (cinfo.next_scanline < cinfo.image_height)
  	{
    /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
    	row_pointer[0] = (JSAMPLE *)&buffer[cinfo.next_scanline * row_stride];
    	(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  	}
  	jpeg_finish_compress(&cinfo);
  	if( filename )
        fclose(outfile);
  	jpeg_destroy_compress(&cinfo);
}

/*
 * SOME FINE POINTS:
 *
 * In the above loop, we ignored the return value of jpeg_write_scanlines,
 * which is the number of scanlines actually written.  We could get away
 * with this because we were only relying on the value of cinfo.next_scanline,
 * which will be incremented correctly.  If you maintain additional loop
 * variables then you should be careful to increment them properly.
 * Actually, for output to a stdio stream you needn't worry, because
 * then jpeg_write_scanlines will write all the lines passed (or else exit
 * with a fatal error).  Partial writes can only occur if you use a data
 * destination module that can demand suspension of the compressor.
 * (If you don't know what that's for, you don't need it.)
 *
 * If the compressor requires full-image buffers (for entropy-coding
 * optimization or a multi-scan JPEG file), it will create temporary
 * files for anything that doesn't fit within the maximum-memory setting.
 * (Note that temp files are NOT needed if you use the default parameters.)
 * On some systems you may need to set up a signal handler to ensure that
 * temporary files are deleted if the program is interrupted.  See libjpeg.doc.
 *
 * Scanlines MUST be supplied in top-to-bottom order if you want your JPEG
 * files to be compatible with everyone else's.  If you cannot readily read
 * your data in that order, you'll need an intermediate array to hold the
 * image.  See rdtarga.c or rdbmp.c for examples of handling bottom-to-top
 * source data using the JPEG code's internal virtual-array mechanisms.
 */

/* This half of the example shows how to read data from the JPEG decompressor.
 * It's a bit more refined than the above, in that we show:
 *   (a) how to modify the JPEG library's standard error-reporting behavior;
 *   (b) how to allocate workspace using the library's memory manager.
 *
 * Just to make this example a little different from the first one, we'll
 * assume that we do not intend to put the whole image into an in-memory
 * buffer, but to send it line-by-line someplace else.  We need a one-
 * scanline-high JSAMPLE array as a work buffer, and we will let the JPEG
 * memory manager allocate it for us.  This approach is actually quite useful
 * because we don't need to remember to deallocate the buffer separately: it
 * will go away automatically when the JPEG object is cleaned up.
 */

/*
 * ERROR HANDLING:
 *
 * The JPEG library's standard error handler (jerror.c) is divided into
 * several "methods" which you can override individually.  This lets you
 * adjust the behavior without duplicating a lot of code, which you might
 * have to update with each future release.
 *
 * Our example here shows how to override the "error_exit" method so that
 * control is returned to the library's caller when a fatal error occurs,
 * rather than calling exit() as the standard error_exit method does.
 *
 * We use C's setjmp/longjmp facility to return control.  This means that the
 * routine which calls the JPEG library must first execute a setjmp() call to
 * establish the return point.  We want the replacement error_exit to do a
 * longjmp().  But we need to make the setjmp buffer accessible to the
 * error_exit routine.  To do this, we make a private extension of the
 * standard JPEG error handler object.  (If we were using C++, we'd say we
 * were making a subclass of the regular error handler.)
 *
 * Here's the extended error handler struct:
 */

//---------------------------------------------------------------------------
//   BMP Image
//---------------------------------------------------------------------------

#define BI_RGB  		0
#define BI_RLE8  		1
#define BI_RLE4  		2
#define BI_BITFIELDS	3

#pragma pack(push,1)

typedef struct tagBITMAPFILEHEADER {
	word_t	bfType;		//тип файла (для битового образа - BM)
	dword_t	bfSize;		//размер файла в dword
	word_t	bfReserved1;
	word_t	bfReserved2;
 	dword_t	bfOffbits;	//смещение данных битового образа от начала файла в байтах
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	dword_t	biSize;		//число байт, занимаемых структурой BITMAPINFOHEADER
 	dword_t	biWidth;	//ширина битового образа в пикселах
 	dword_t	biHeight;	//высота битового образа в пикселах
	word_t	biPlanes;	//число битовых плоскостей устройства
	word_t	biBitCount;	//число битов на пиксель
	dword_t	biCompression;	//тип сжатия
	dword_t	biSizeImage;	//размер картинки в байтах
	dword_t	biXPelsPerMeter;//горизонтальное разрешение устройства, пиксел/м
	dword_t	biYPelPerMeter;	//вертикальное разрешение устройства, пиксел/м
	dword_t	biClrUsed;	//число используемых цветов
	dword_t	biClrImportant; //число "важных" цветов
} BITMAPINFOHEADER;

#pragma pack(pop)
//---------------------------------------------------------------------------
static void __load_RLE4(FILE *infile, HDIB dib, byte_t *pixels)
{
	int EndOfPixLine, EndOfPicture, i, j, truewidth;
	long pos, line;
	byte_t count, value;
	byte_t  bArray[8];
	word_t value_w;
	byte_t **ImageLines;

    j = 8 - (dib->width - (dib->width /8) * 8);
    j = j * byte_t(j != 8);
    truewidth = dib->width + j;

    ImageLines = new byte_t *[dib->height];
    for(register int i = 0; i < dib->height; i++)
    {
    	ImageLines[i] = new byte_t[truewidth];
    };

    EndOfPicture = 0;
    line = 0;

    while (EndOfPicture == 0)
    {
    	pos = 0;
      	EndOfPixLine = 0;
      	while ((EndOfPixLine == 0) && (EndOfPicture == 0))
      	{
        	fread(&count, 1, 1, infile);
        	fread(&value, 1, 1, infile);
        	if (count > 0)
        	{
          		bArray[1] = value & 15;
          		bArray[0] = (value >> 4) & 15;
          		for(register int j = 0; j < count; j++)
          		{
            		ImageLines[line][pos] = bArray[j % 2];
            		pos = pos + 1;
          		};
        	}
        	else
        	{
          		switch(value)
          		{
            		case 0: EndOfPixLine = 1; break;
            		case 1: EndOfPicture = 1; break;
            		case 2:
                			fread(&count, 1, 1, infile);
        					fread(&value, 1, 1, infile);
                			pos = pos + count;
                			line = line + value;
                			break;
          			default:
              				for(register int j = 0; j < value; j++)
              				{
                				if((j % 4) == 0)
                				{
                  					fread(&value_w, 1, 2, infile);
                  					for(register int i = 0; i < 2; i++)
                  					{
                    					bArray[2 * i + 1] = value_w & 15;
                    					value_w = value_w >> 4;
                    					bArray[2 * i] = value_w & 15;
                    					value_w = value_w >> 4;
                  					};
                				};
  								ImageLines[line][pos] = bArray[j % 4];
                				pos = pos + 1;
              				};
          		};
        	};
        	if((pos - 1) > dib->width)
          		EndOfPixLine = 1;
      };

      line = line + 1;
      if (line >= dib->height)
		EndOfPicture = 1;
    };

    for(register int j = 0; j < dib->height; j++)
    {
      i = 0;
      while(i < truewidth)
      {
      	value = 0;
        value = (ImageLines[j][i] << 4) | (ImageLines[j][i + 1] & 15);
        *pixels++ = value;
        i = i + 2;
      };
    };

    for(register int i = 0; i < dib->height; i++)
    {
    	delete[] ImageLines[i];
    };
    delete[] ImageLines;
}
//---------------------------------------------------------------------------
static void __load_RLE8(FILE *infile, HDIB dib, byte_t *pixels)
{
	int count = 0;
  	int y;
  	register int i, x;
  	register byte_t *p, *q;
  	byte_t cbyte;

  	cbyte = 0;
  	x = 0;
  	p = pixels;
  	q = pixels + (size_t) dib->bytes_per_line * dib->height;
	for (y=0; y < dib->height; )
  	{
    	if ((p < pixels) || (p >= q))
      		break;
    	fread(&count, 1, 1, infile);
    	if(count == EOF)
      		break;
    	if(count > 0)
      	{
//          Encoded mode.
        	fread(&cbyte, 1, 1, infile);
            for (i = 0; i < count; i++)
              		*p++ = cbyte;
        	x += count;
      	}
    	else
      	{
//          Escape mode.
        	fread(&count, 1, 1, infile);
        	switch (count)
        	{
        		case 0x01: return;
          		case 0x00:
//              End of line.
            			x=0;
            			y++;
            			p = pixels + y * dib->bytes_per_line;
            			break;
          		case 0x02:
//              Delta mode.
            			fread(&cbyte, 1, 1, infile);
            			x += cbyte;
            			fread(&cbyte, 1, 1, infile);
            			y += cbyte;
            			p = pixels + y * dib->bytes_per_line + x;
            			break;
          		default:
//              Absolute mode.
              			for(i = 0; i < count; i++)
              			{
              				fread(&cbyte, 1, 1, infile);
                			*p++ = cbyte;
                		}
            			x += count;
//              Read pad byte.
                		if((count & 0x01) != 0)
                			fread(&cbyte, 1, 1, infile);
            			break;
        	}
      	}
  	}
}
//---------------------------------------------------------------------------
static void __load_bmp(FILE *infile, HDIB dib, dword_t compression)
{
    byte_t *rp;
  	int row_stride = dib->bytes_per_line;
  	int bsize = row_stride * dib->height;

  	if(compression == BI_RGB)
  	{
		rp = (byte_t *)dib->data + bsize - row_stride;
  		for (register int i = dib->height; i > 0; i--)
  		{
    		fread(rp, 1, row_stride, infile);
    		rp -= row_stride;
  		}
  		if(dib->depth == 1)
        {
            for(register int i = 0; i < bsize; i++)
                dib->data[i] = ~dib->data[i];
        }
  	}
  	else
  	{
  		byte_t *pp = new byte_t[bsize];
  		memset(pp, 255, bsize);
  		if(compression == BI_RLE4)
   			__load_RLE4(infile, dib, pp);
   		else if(compression == BI_RLE8)
   			__load_RLE8(infile, dib, pp);
		rp = (byte_t *)dib->data + bsize - row_stride;
  		byte_t *tp = pp;
  		for (register int i = dib->height; i > 0; i--)
  		{
    		memcpy(rp, tp, row_stride);
    		rp -= row_stride;
    		tp += row_stride;
  		}
  		delete [] pp;
  	}
}
//---------------------------------------------------------------------------
TBMPImage :: TBMPImage()
{
}
//---------------------------------------------------------------------------
TBMPImage :: TBMPImage(int width, int height, char *data)
{
   Load(width, height, data);
}
//---------------------------------------------------------------------------
TBMPImage :: ~TBMPImage()
{
}
//---------------------------------------------------------------------------
void TBMPImage :: Load(const TString &filename)
{
	BITMAPFILEHEADER finfo;
  	BITMAPINFOHEADER hinfo;
  	int bsize;
  	FILE *infile;
 // 	int row_stride;

  	if ((infile = fopen(filename.c_str(), "rb")) == NULL)
  	{
    	fprintf(stderr, "can't open %s\n", filename.c_str());
    	return;
  	}

	fread(&finfo.bfType, sizeof(finfo.bfType), 1, infile);
	if(finfo.bfType != 0x4D42)
	{
		fclose(infile);
		fprintf(stderr, "invalid file %s\n", filename.c_str());
		return;
	}
	fread(&finfo.bfSize, sizeof(finfo.bfSize), 1, infile);
	fseek(infile, sizeof(finfo.bfReserved1) + sizeof(finfo.bfReserved2), SEEK_CUR);
	fread(&finfo.bfOffbits, sizeof(finfo.bfOffbits), 1, infile);
	fread(&hinfo, 1, sizeof(BITMAPINFOHEADER), infile);
	bsize = finfo.bfOffbits - ftell(infile);
	if(bsize > 0)
	{
		m_Colormap = new RGBQUAD[bsize];
		fread(m_Colormap, 1, bsize, infile);
	}
  	fseek(infile,finfo.bfOffbits, SEEK_SET);

  	TPixelFormat a(TPixelSize(hinfo.biBitCount));
//  	a.Depth = TPixelSize(hinfo.biBitCount);
  	switch(TPixelSize(hinfo.biBitCount))
  	{
  		case pf1bit:
  			  	a.BitOrder = 1;
  				a.ByteOrder = 1;
  				break;
   		case pf4bit:
   		  		a.BitOrder = 0;
  				a.ByteOrder = 1;
  				break;
   		case pf8bit:
   		  		a.BitOrder = 0;
  				a.ByteOrder = 0;
  				break;
   		case pf16bit:
   		case pf24bit:
   		case pf32bit:
   		case pfNone: ;
  	}
  	Create(hinfo.biWidth, hinfo.biHeight, bmDIB, &a);
  	__load_bmp(infile, m_Data, hinfo.biCompression);
/*
  	row_stride = m_Data->bytes_per_line;
  	bsize = row_stride * hinfo.biHeight;

  	if(hinfo.biCompression == BI_RGB)
  	{
		rp = (byte_t *)m_Data->data + bsize - row_stride;
  		for (register int i = hinfo.biHeight; i > 0; i--)
  		{
    		fread(rp, 1, row_stride, infile);
    		rp -= row_stride;
  		}
  	}
  	else
  	{
  		byte_t *pp = new byte_t[bsize];
  		memset(pp, 255, bsize);
  		if(hinfo.biCompression == BI_RLE4)
   			__load_RLE4(infile, m_Data, pp);
   		else if(hinfo.biCompression == BI_RLE8)
   			__load_RLE8(infile, m_Data, pp);
		rp = (byte_t *)m_Data->data + bsize - row_stride;
  		byte_t *tp = pp;
  		for (register int i = hinfo.biHeight; i > 0; i--)
  		{
    		memcpy(rp, tp, row_stride);
    		rp -= row_stride;
    		tp += row_stride;
  		}
  		delete [] pp;
  	}
*/
  	fclose(infile);

	return;
}
//---------------------------------------------------------------------------
void TBMPImage :: Save(const TString &filename)
{
    if( m_PixelFormat.ByteOrder == 1 )
    {
        HandleType = bmDDB;
        m_PixelFormat.ByteOrder = 0;
    }
   	switch(HandleType)
   	{
    	case bmDDB:
        	HandleType = bmDIB;
     	case bmDIB:
        	write_bmp_file(filename.c_str());
         	break;
     	case bmEmpty:
        	break;
   	}
}
//---------------------------------------------------------------------------
void TBMPImage :: Load(int width, int height, char *data)
{
//   HBITMAP hbmp;

   if(!Empty)
      Release();
/*
   hbmp = XCreateBitmapFromData(Application.Handle, Application.ActiveScreen->Root,
   										 data, width, height);
   if(hbmp != None)
      CreateDDB(width, height, pf1bit, ptDefault, hbmp); */
}
//---------------------------------------------------------------------------
void TBMPImage :: write_bmp_file (const char *filename)
{
	BITMAPFILEHEADER finfo;
  	BITMAPINFOHEADER hinfo;
  	int row_stride, imgsize, hsize, cmsize;
  	byte_t *rp, *pp, *tp;
	FILE * outfile;

  	hinfo.biCompression = BI_RGB;
	row_stride = m_Data->bytes_per_line;
	imgsize = row_stride * m_Data->height;
	tp = new byte_t[imgsize];
	rp = (byte_t *)m_Data->data + imgsize - row_stride;
	pp = tp;
	for(register int i = m_Data->height; i > 0; i--)
	{
   		memcpy(pp, rp, row_stride);
   		rp -= row_stride;
   		pp += row_stride;
	}

	hsize = 14 + sizeof(BITMAPINFOHEADER);
	cmsize = m_Colormap ? NCOLORS(m_PixelFormat.Depth) * sizeof(RGBQUAD) : 0;

	finfo.bfType = 0x4D42;
	finfo.bfSize = hsize + cmsize + imgsize;  //размер файла в dword
	finfo.bfReserved1 = 0;
	finfo.bfReserved2 = 0;
 	finfo.bfOffbits = hsize + cmsize; //смещение данных битового образа от начала файла в байтах

	hinfo.biSize = sizeof(BITMAPINFOHEADER);
 	hinfo.biWidth = m_Data->width;
 	hinfo.biHeight = m_Data->height;
	hinfo.biPlanes = 1;	//число битовых плоскостей устройства
	hinfo.biBitCount = m_PixelFormat.Depth;	//число битов на пиксель
  	hinfo.biSizeImage = imgsize; //размер картинки в байтах
	hinfo.biXPelsPerMeter = Screen.Xdpi * 10000 / 254;//горизонтальное разрешение устройства, пиксел/м
	hinfo.biYPelPerMeter = Screen.Ydpi * 10000 / 254;	//вертикальное разрешение устройства, пиксел/м
	hinfo.biClrUsed = 0;	//число используемых цветов
	hinfo.biClrImportant = 0; //число "важных" цветов

	if ((outfile = fopen(filename, "wb")) == NULL)
  	{
    	fprintf(stderr, "can't open %s\n", filename);
    	delete tp;
    	return;
  	}

  	fwrite(&finfo.bfType, sizeof(finfo.bfType), 1, outfile);
	fwrite(&finfo.bfSize, sizeof(finfo.bfSize), 1, outfile);
  	fwrite(&finfo.bfReserved1, sizeof(finfo.bfReserved1), 1, outfile);
	fwrite(&finfo.bfReserved2, sizeof(finfo.bfReserved2), 1, outfile);
	fwrite(&finfo.bfOffbits, sizeof(finfo.bfOffbits), 1, outfile);
	fwrite(&hinfo, 1, sizeof(BITMAPINFOHEADER), outfile);
	if(m_Colormap)
		fwrite(m_Colormap, 1, cmsize, outfile);
	fwrite(tp, 1, imgsize, outfile);
	fclose(outfile);
	delete tp;
}
//---------------------------------------------------------------------------
//   ICO Image
//---------------------------------------------------------------------------
#define MAXICONS  1024

#pragma pack(push,1)

typedef struct tagICONENTRY
{
    byte_t width;
    byte_t height;
    byte_t colors;
    byte_t reserved;

    word_t planes;
    word_t bits_per_pixel;

    dword_t size;
    dword_t offset;
} ICONENTRY;

typedef struct tagICONFILEHEADER
{
    short reserved;
    short resource_type;
    short count;

//    ICONENTRY directory[MAXICONS];
} ICONFILEHEADER;

typedef struct tagICONINFO
{
    dword_t file_size;
    dword_t ba_offset;
    dword_t offset_bits;
    dword_t size;

    dword_t width;
    dword_t height;

    word_t planes;
    word_t bits_per_pixel;

    dword_t compression;
    dword_t image_size;
    dword_t x_pixels;
    dword_t y_pixels;
    dword_t number_colors;
    dword_t red_mask;
    dword_t green_mask;
    dword_t blue_mask;
    dword_t alpha_mask;
    dword_t colors_important;

    dword_t colorspace;
} ICONINFO;

#pragma pack(pop)

TICOImage :: TICOImage()
{
}
//---------------------------------------------------------------------------
TICOImage :: TICOImage(int width, int height, char *data)
{
   Load(width, height, data);
}
//---------------------------------------------------------------------------
TICOImage :: ~TICOImage()
{
}
//---------------------------------------------------------------------------
void TICOImage :: Load(const TString &filename)
{
	ICONFILEHEADER finfo;
  	ICONENTRY ientry;
    BITMAPINFOHEADER hinfo;
  	int bsize;
  	FILE *infile;
//  	int row_stride;
//  	byte_t *rp;

  	if ((infile = fopen(filename.c_str(), "rb")) == NULL)
  	{
    	fprintf(stderr, "can't open %s\n", filename.c_str());
    	return;
  	}

	fread(&finfo, sizeof(finfo), 1, infile);

	if(!(finfo.reserved == 0 && (finfo.resource_type == 1 || finfo.resource_type == 2) && finfo.count > 0))
	{
		fclose(infile);
		fprintf(stderr, "invalid file %s\n", filename.c_str());
		return;
	}
	fread(&ientry, sizeof(ientry), 1, infile);

    fseek(infile, ientry.offset, SEEK_SET);
    fread(&hinfo, 1, sizeof(hinfo), infile);
/*	fseek(infile, sizeof(finfo.bfReserved1) + sizeof(finfo.bfReserved2), SEEK_CUR);
	fread(&finfo.bfOffbits, sizeof(finfo.bfOffbits), 1, infile);
	fread(&hinfo, 1, sizeof(BITMAPINFOHEADER), infile); */
	bsize = ientry.colors * 4;
	if(bsize > 0)
	{
		m_Colormap = new RGBQUAD[bsize];
		fread(m_Colormap, 1, bsize, infile);
	}

  	TPixelFormat a(TPixelSize(hinfo.biBitCount));
//  	a.Depth = TPixelSize(hinfo.biBitCount);
  	switch(TPixelSize(hinfo.biBitCount))
  	{
  		case pf1bit:
  			  	a.BitOrder = 1;
  				a.ByteOrder = 1;
  				break;
   		case pf4bit:
   		  		a.BitOrder = 0;
  				a.ByteOrder = 1;
  				break;
   		case pf8bit:
   		  		a.BitOrder = 0;
  				a.ByteOrder = 0;
  				break;
   		case pf16bit:
   		case pf24bit:
   		case pf32bit:
   		case pfNone: ;
  	}
  	TPixelFormat b(pf1bit);
    b.BitOrder = 1;
  	b.ByteOrder = 1;
  	Create(hinfo.biWidth, hinfo.biHeight/2, bmDIB, &a, &b);
  	__load_bmp(infile, m_Data, hinfo.biCompression);
  	__load_bmp(infile, m_MaskData, 0);

/*  	row_stride = m_Data->bytes_per_line;
  	bsize = row_stride * hinfo.biHeight/2;

  	if(hinfo.biCompression == BI_RGB)
  	{
		rp = (byte_t *)m_Data->data + bsize - row_stride;
  		for (register int i = hinfo.biHeight/2; i > 0; i--)
  		{
    		fread(rp, 1, row_stride, infile);
    		rp -= row_stride;
  		}
  	}
  	else
  	{
  		byte_t *pp = new byte_t[bsize];
  		memset(pp, 255, bsize);
  		if(hinfo.biCompression == BI_RLE4)
   			__load_RLE4(infile, m_Data, pp);
   		else if(hinfo.biCompression == BI_RLE8)
   			__load_RLE8(infile, m_Data, pp);
		rp = (byte_t *)m_Data->data + bsize - row_stride;
  		byte_t *tp = pp;
  		for (register int i = hinfo.biHeight; i > 0; i--)
  		{
    		memcpy(rp, tp, row_stride);
    		rp -= row_stride;
    		tp += row_stride;
  		}
  		delete [] pp;
  	}
*/
  	fclose(infile);

	return;
}
//---------------------------------------------------------------------------
void TICOImage :: Save(const TString &filename)
{
    if( m_PixelFormat.ByteOrder == 1 )
    {
        HandleType = bmDDB;
        m_PixelFormat.ByteOrder = 0;
    }
   	switch(HandleType)
   	{
    	case bmDDB:
        	HandleType = bmDIB;
     	case bmDIB:
        	write_bmp_file(filename.c_str());
         	break;
     	case bmEmpty:
        	break;
   	}
}
//---------------------------------------------------------------------------
void TICOImage :: Load(int width, int height, char *data)
{
//   HBITMAP hbmp;

   if(!Empty)
      Release();
/*
   hbmp = XCreateBitmapFromData(Application.Handle, Application.ActiveScreen->Root,
   										 data, width, height);
   if(hbmp != None)
      CreateDDB(width, height, pf1bit, ptDefault, hbmp); */
}
//---------------------------------------------------------------------------
void TICOImage :: write_bmp_file (const char *filename)
{
	BITMAPFILEHEADER finfo;
  	BITMAPINFOHEADER hinfo;
  	int row_stride, imgsize, hsize, cmsize;
  	byte_t *rp, *pp, *tp;
	FILE * outfile;

  	hinfo.biCompression = BI_RGB;
	row_stride = m_Data->bytes_per_line;
	imgsize = row_stride * m_Data->height;
	tp = new byte_t[imgsize];
	rp = (byte_t *)m_Data->data + imgsize - row_stride;
	pp = tp;
	for(register int i = m_Data->height; i > 0; i--)
	{
   		memcpy(pp, rp, row_stride);
   		rp -= row_stride;
   		pp += row_stride;
	}

	hsize = 14 + sizeof(BITMAPINFOHEADER);
	cmsize = m_Colormap ? NCOLORS(m_PixelFormat.Depth) * sizeof(RGBQUAD) : 0;

	finfo.bfType = 0x4D42;
	finfo.bfSize = hsize + cmsize + imgsize;  //размер файла в dword
	finfo.bfReserved1 = 0;
	finfo.bfReserved2 = 0;
 	finfo.bfOffbits = hsize + cmsize; //смещение данных битового образа от начала файла в байтах

	hinfo.biSize = sizeof(BITMAPINFOHEADER);
 	hinfo.biWidth = m_Data->width;
 	hinfo.biHeight = m_Data->height;
	hinfo.biPlanes = 1;	//число битовых плоскостей устройства
	hinfo.biBitCount = m_PixelFormat.Depth;	//число битов на пиксель
  	hinfo.biSizeImage = imgsize; //размер картинки в байтах
	hinfo.biXPelsPerMeter = Screen.Xdpi * 10000 / 254;//горизонтальное разрешение устройства, пиксел/м
	hinfo.biYPelPerMeter = Screen.Ydpi * 10000 / 254;	//вертикальное разрешение устройства, пиксел/м
	hinfo.biClrUsed = 0;	//число используемых цветов
	hinfo.biClrImportant = 0; //число "важных" цветов

	if ((outfile = fopen(filename, "wb")) == NULL)
  	{
    	fprintf(stderr, "can't open %s\n", filename);
    	delete tp;
    	return;
  	}

  	fwrite(&finfo.bfType, sizeof(finfo.bfType), 1, outfile);
	fwrite(&finfo.bfSize, sizeof(finfo.bfSize), 1, outfile);
  	fwrite(&finfo.bfReserved1, sizeof(finfo.bfReserved1), 1, outfile);
	fwrite(&finfo.bfReserved2, sizeof(finfo.bfReserved2), 1, outfile);
	fwrite(&finfo.bfOffbits, sizeof(finfo.bfOffbits), 1, outfile);
	fwrite(&hinfo, 1, sizeof(BITMAPINFOHEADER), outfile);
	if(m_Colormap)
		fwrite(m_Colormap, 1, cmsize, outfile);
	fwrite(tp, 1, imgsize, outfile);
	fclose(outfile);
	delete tp;
}
//---------------------------------------------------------------------------
//   GIF Image
//---------------------------------------------------------------------------
TGIFImage :: TGIFImage()
{
}
//---------------------------------------------------------------------------
TGIFImage :: TGIFImage(int width, int height, char *data)
{
 //  Load(width, height, data);
}
//---------------------------------------------------------------------------
TGIFImage :: ~TGIFImage()
{
}
//---------------------------------------------------------------------------
void TGIFImage :: Load(const TString &filename)
{
	Gif *gif = read_gif_file(filename.c_str());
	printf("GIF: header = %s BlockCount = %d\n", gif->header, gif->block_count);
	printf("GIF: Screen size SWidth = %d SHeight = %d\n", gif->screen->width, gif->screen->height);
	print_gif(stdout, gif);

	GifScreen *screen;
	GifBlock *block;
	GifPicture *pic;
	GifPalette *cmap;
	int cmaplen;
	int pixval;
	TPixelFormat a(pf8bit);
//	a.Depth = pf8bit;
	a.BitOrder = 0;
	a.ByteOrder = 1;

	screen = gif->screen;
	printf(" GifScreen:\n");
	printf("  width      = %d\n", screen->width);
	printf("  height     = %d\n", screen->height);

	printf("  has_cmap   = %d\n", screen->has_cmap ? 1:0);
	printf("  color_res  = %d\n", screen->color_res);
	printf("  sorted     = %d\n", screen->sorted ? 1:0);
	printf("  cmap_depth = %d\n", screen->cmap_depth);

	printf("  bgcolour   = %02X\n", screen->bgcolour);
	printf("  aspect     = %d\n", screen->aspect);

	Create(screen->width, screen->height, bmDIB, &a);

	if (screen->has_cmap)
	{
		cmap = screen->cmap;
		cmaplen = cmap->length;
		printf("  GifPalette (length=%d):\n", cmap->length);
		SetColormap((TRGB *)(cmap->colours), cmap->length);
	}
	for (int i=0; i < gif->block_count; i++)
	{
		block = gif->blocks[i];
		if (block->pic)
		{
			pic = block->pic;
			printf(" GifPicture:\n");
			printf("  left       = %d\n", pic->left);
			printf("  top        = %d\n", pic->top);
			printf("  width      = %d\n", pic->width);
			printf("  height     = %d\n", pic->height);

			printf("  has_cmap   = %d\n", pic->has_cmap);
			printf("  interlace  = %d\n", pic->interlace);
			printf("  sorted     = %d\n", pic->sorted);
			printf("  reserved   = %d\n", pic->reserved);
			printf("  cmap_depth = %d\n", pic->cmap_depth);

			if (pic->has_cmap)
			{
				cmap = pic->cmap;
				cmaplen = cmap->length;
				printf("  GifPalette (length=%d):\n", cmap->length);
//				SetColormap((TRGB *)(cmap->colours), cmap->length);
			}

			for (int row = 0; row < pic->height; row++)
			{
				for (int col = 0; col < pic->width; col++)
				{
					pixval = pic->data[row][col];
					Pixels(col, row, pixval);
				}
			}
		}
	}

	del_gif(gif);
}
//---------------------------------------------------------------------------
//   PNG Image
//---------------------------------------------------------------------------
TPNGImage :: TPNGImage()
{
}
//---------------------------------------------------------------------------
TPNGImage :: TPNGImage(int width, int height, char *data)
{
 //  Load(width, height, data);
}
//---------------------------------------------------------------------------
TPNGImage :: ~TPNGImage()
{
}
//---------------------------------------------------------------------------
void TPNGImage :: Load(const TString &filename)
{
    read_png(filename.c_str());
}
//---------------------------------------------------------------------------
#define _POSIX_SOURCE 1
#define PNG_BYTES_TO_CHECK 4

void TPNGImage :: read_png(const char *filename)
{
    png_byte buf[PNG_BYTES_TO_CHECK];
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = PNG_BYTES_TO_CHECK;
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type;
    FILE *fp;

    int compression_type, filter_method;
    int row, col;
    png_bytep *row_pointers;
    int rowbytes;

    if ((fp = fopen(filename, "rb")) == NULL)
    {
        fprintf(stderr, "can't open %s\n", filename);
        return;
    }
    if (fread(buf, 1, PNG_BYTES_TO_CHECK, fp) != PNG_BYTES_TO_CHECK)
    {
		fclose(fp);
		fprintf(stderr, "invalid file %s\n", filename);
		return;
	}
    if(png_sig_cmp(buf, (png_size_t)0, PNG_BYTES_TO_CHECK))
    {
		fclose(fp);
		fprintf(stderr, "invalid file %s\n", filename);
		return;
	}
   /* Create and initialize the png_struct with the desired error handler
    * functions.  If you want to use the default stderr and longjump method,
    * you can supply NULL for the last three parameters.  We also supply the
    * the compiler header file version, so that we know if the application
    * was compiled with a compatible version of the library.  REQUIRED
    */
//    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, png_voidp user_error_ptr, user_error_fn, user_warning_fn);
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(png_ptr == NULL)
    {
        fclose(fp);
        return;
    }
    /* Allocate/initialize the memory for image information.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if(info_ptr == NULL)
    {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return;
    }

   /* Set error handling if you are using the setjmp/longjmp method (this is
    * the normal method of doing things with libpng).  REQUIRED unless you
    * set up your own error handlers in the png_create_read_struct() earlier.
    */
    if(setjmp(png_jmpbuf(png_ptr)))
    {
      /* Free all of the memory associated with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
      /* If we get here, we had a problem reading the file */
        return;
    }
    if(filename)
        png_init_io(png_ptr, fp);
//    else
//        png_set_read_fn(png_ptr, (void *)user_io_ptr, user_read_fn);
   /* where user_io_ptr is a structure you want available to the callbacks */

    /* If we have already read some of the signature */
    png_set_sig_bytes(png_ptr, sig_read);

   /*
    * If you have enough memory to read in the entire image at once,
    * and you need to specify only transforms that can be controlled
    * with one of the PNG_TRANSFORM_* bits (this presently excludes
    * quantizing, filling, setting background, and doing gamma
    * adjustment), then you can read the entire image (including
    * pixels) into the info structure with this call:
    */
//    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_BGR | PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_SWAP_ALPHA | PNG_TRANSFORM_INVERT_ALPHA, NULL);
png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_BGR, NULL);
    fclose(fp);

    const char *colorname;

    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, &compression_type, &filter_method);
    switch(color_type)
    {
        case PNG_COLOR_TYPE_GRAY: colorname = "PNG_COLOR_TYPE_GRAY"; break;
        case PNG_COLOR_TYPE_PALETTE: colorname = "PNG_COLOR_TYPE_PALETTE"; break;
        case PNG_COLOR_TYPE_RGB: colorname = "PNG_COLOR_TYPE_RGB"; break;
        case PNG_COLOR_TYPE_RGB_ALPHA: colorname = "PNG_COLOR_TYPE_RGB_ALPHA"; break;
        case PNG_COLOR_MASK_PALETTE: colorname = "PNG_COLOR_MASK_PALETTE"; break;
        case PNG_COLOR_MASK_ALPHA: colorname = "PNG_COLOR_MASK_ALPHA"; break;
    }
/* These describe the color_type field in png_info. */
/* color type masks */
/*
#define PNG_COLOR_MASK_PALETTE    1
#define PNG_COLOR_MASK_COLOR      2
#define PNG_COLOR_MASK_ALPHA      4
*/
/* color types.  Note that not all combinations are legal */
/*
#define PNG_COLOR_TYPE_GRAY 0
#define PNG_COLOR_TYPE_PALETTE  (PNG_COLOR_MASK_COLOR | PNG_COLOR_MASK_PALETTE)
#define PNG_COLOR_TYPE_RGB        (PNG_COLOR_MASK_COLOR)
#define PNG_COLOR_TYPE_RGB_ALPHA  (PNG_COLOR_MASK_COLOR | PNG_COLOR_MASK_ALPHA)
#define PNG_COLOR_TYPE_GRAY_ALPHA (PNG_COLOR_MASK_ALPHA)
*/
/* aliases */
/*
#define PNG_COLOR_TYPE_RGBA  PNG_COLOR_TYPE_RGB_ALPHA
#define PNG_COLOR_TYPE_GA  PNG_COLOR_TYPE_GRAY_ALPHA
 */
    printf ("Ширина = %lu, высота = %lu\n", width, height);
    printf ("Тип цвета = %d (%s), глубина цвета = %d %d\n", color_type, colorname, bit_depth, info_ptr->pixel_depth);
    printf ("Количество байт в строке = %lu\n", png_get_rowbytes(png_ptr, info_ptr));

    rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    row_pointers = png_get_rows(png_ptr, info_ptr);

  	TPixelFormat ap(TPixelSize(info_ptr->pixel_depth));
//  	a.Depth = TPixelSize(info_ptr->pixel_depth);
  	ap.BitOrder = 0;
    ap.ByteOrder = 0;

      	TPixelFormat bp(pf8bit);
  	bp.BitOrder = 0;
    bp.ByteOrder = 0;
  	Create(info_ptr->width, info_ptr->height, bmDIB, &ap, &bp);

  		byte_t *rp = (byte_t *)m_Data->data;
  		for (register uint_t i = 0; i < info_ptr->height; i++)
  		{
    		memcpy(rp, row_pointers[i], rowbytes);
    		rp += m_Data->bytes_per_line;
  		}

		byte_t *tp, *tm;
		for(register int j = 0; j < m_Data->height; j++)
		{
			tm = (byte_t *)m_Data->data + j * m_Data->bytes_per_line;
			tp = (byte_t *)m_MaskData->data + j * m_MaskData->bytes_per_line;
			for(register int i = 0; i < m_Data->width; i++)
				memcpy(tp + i, tm + i * 4 + 3, 1);
		}

/*
		int nbytes = m_MaskData->bytes_per_line * m_MaskData->height;
		int r, g, b, a;

			for(register int i = 0; i < nbytes; i++)
			{
				r = *(m_Data->data + i * 4);
				g = *(m_Data->data + i * 4 + 1);
				b = *(m_Data->data + i * 4 + 2);
				a = *(m_Data->data + i * 4 + 3);
				*(m_MaskData->data + i) = a;
			}
*/
//  /*--- Получение негатива исходного изображения ---*/
//    int pixval;
//    for (row = 0; row < height; row++)
//    {
//        for (col = 0; col < width; col++)
//        {
//            pixval = row_pointers[row][col];
//            Pixels(col, row, pixval);
//      row_pointers[row][col] ^= 0xFF;
//        };
//    };

   /* Clean up after the read, and free any memory allocated - REQUIRED */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return;
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------

#if 0 /* in case someone actually tries to compile this */

#ifdef hilevel
   /*
    * If you have enough memory to read in the entire image at once,
    * and you need to specify only transforms that can be controlled
    * with one of the PNG_TRANSFORM_* bits (this presently excludes
    * quantizing, filling, setting background, and doing gamma
    * adjustment), then you can read the entire image (including
    * pixels) into the info structure with this call:
    */
   png_read_png(png_ptr, info_ptr, png_transforms, NULL);

#else
   /* OK, you're doing it the hard way, with the lower-level functions */

   /* The call to png_read_info() gives us all of the information from the
    * PNG file before the first IDAT (image data chunk).  REQUIRED
    */
   png_read_info(png_ptr, info_ptr);

   png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
       &interlace_type, NULL, NULL);

   /* Set up the data transformations you want.  Note that these are all
    * optional.  Only call them if you want/need them.  Many of the
    * transformations only work on specific types of images, and many
    * are mutually exclusive.
    */

   /* Tell libpng to strip 16 bit/color files down to 8 bits/color.
    * Use accurate scaling if it's available, otherwise just chop off the
    * low byte.
    */
#ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
    png_set_scale_16(png_ptr);
#else
   png_set_strip_16(png_ptr);
#endif

   /* Strip alpha bytes from the input data without combining with the
    * background (not recommended).
    */
   png_set_strip_alpha(png_ptr);

   /* Extract multiple pixels with bit depths of 1, 2, and 4 from a single
    * byte into separate bytes (useful for paletted and grayscale images).
    */
   png_set_packing(png_ptr);

   /* Change the order of packed pixels to least significant bit first
    * (not useful if you are using png_set_packing). */
   png_set_packswap(png_ptr);

   /* Expand paletted colors into true RGB triplets */
   if (color_type == PNG_COLOR_TYPE_PALETTE)
      png_set_palette_to_rgb(png_ptr);

   /* Expand grayscale images to the full 8 bits from 1, 2, or 4 bits/pixel */
   if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
      png_set_expand_gray_1_2_4_to_8(png_ptr);

   /* Expand paletted or RGB images with transparency to full alpha channels
    * so the data will be available as RGBA quartets.
    */
   if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
      png_set_tRNS_to_alpha(png_ptr);

   /* Set the background color to draw transparent and alpha images over.
    * It is possible to set the red, green, and blue components directly
    * for paletted images instead of supplying a palette index.  Note that
    * even if the PNG file supplies a background, you are not required to
    * use it - you should use the (solid) application background if it has one.
    */

   png_color_16 my_background, *image_background;

   if (png_get_bKGD(png_ptr, info_ptr, &image_background))
      png_set_background(png_ptr, image_background,
                         PNG_BACKGROUND_GAMMA_FILE, 1, 1.0);
   else
      png_set_background(png_ptr, &my_background,
                         PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);

   /* Some suggestions as to how to get a screen gamma value
    *
    * Note that screen gamma is the display_exponent, which includes
    * the CRT_exponent and any correction for viewing conditions
    */
   if (/* We have a user-defined screen gamma value */)
   {
      screen_gamma = user-defined screen_gamma;
   }
   /* This is one way that applications share the same screen gamma value */
   else if ((gamma_str = getenv("SCREEN_GAMMA")) != NULL)
   {
      screen_gamma = atof(gamma_str);
   }
   /* If we don't have another value */
   else
   {
      screen_gamma = 2.2;  /* A good guess for a PC monitor in a dimly
                              lit room */
      screen_gamma = 1.7 or 1.0;  /* A good guess for Mac systems */
   }

   /* Tell libpng to handle the gamma conversion for you.  The final call
    * is a good guess for PC generated images, but it should be configurable
    * by the user at run time by the user.  It is strongly suggested that
    * your application support gamma correction.
    */

   int intent;

   if (png_get_sRGB(png_ptr, info_ptr, &intent))
      png_set_gamma(png_ptr, screen_gamma, 0.45455);
   else
   {
      double image_gamma;
      if (png_get_gAMA(png_ptr, info_ptr, &image_gamma))
         png_set_gamma(png_ptr, screen_gamma, image_gamma);
      else
         png_set_gamma(png_ptr, screen_gamma, 0.45455);
   }

#ifdef PNG_READ_QUANTIZE_SUPPORTED
   /* Quantize RGB files down to 8 bit palette or reduce palettes
    * to the number of colors available on your screen.
    */
   if (color_type & PNG_COLOR_MASK_COLOR)
   {
      int num_palette;
      png_colorp palette;

      /* This reduces the image to the application supplied palette */
      if (/* We have our own palette */)
      {
         /* An array of colors to which the image should be quantized */
         png_color std_color_cube[MAX_SCREEN_COLORS];

         png_set_quantize(png_ptr, std_color_cube, MAX_SCREEN_COLORS,
            MAX_SCREEN_COLORS, NULL, 0);
      }
      /* This reduces the image to the palette supplied in the file */
      else if (png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette))
      {
         png_uint_16p histogram = NULL;

         png_get_hIST(png_ptr, info_ptr, &histogram);

         png_set_quantize(png_ptr, palette, num_palette,
                        max_screen_colors, histogram, 0);
      }
   }
#endif /* PNG_READ_QUANTIZE_SUPPORTED */

   /* Invert monochrome files to have 0 as white and 1 as black */
   png_set_invert_mono(png_ptr);

   /* If you want to shift the pixel values from the range [0,255] or
    * [0,65535] to the original [0,7] or [0,31], or whatever range the
    * colors were originally in:
    */
   if (png_get_valid(png_ptr, info_ptr, PNG_INFO_sBIT))
   {
      png_color_8p sig_bit_p;

      png_get_sBIT(png_ptr, info_ptr, &sig_bit_p);
      png_set_shift(png_ptr, sig_bit_p);
   }

   /* Flip the RGB pixels to BGR (or RGBA to BGRA) */
   if (color_type & PNG_COLOR_MASK_COLOR)
      png_set_bgr(png_ptr);

   /* Swap the RGBA or GA data to ARGB or AG (or BGRA to ABGR) */
   png_set_swap_alpha(png_ptr);

   /* Swap bytes of 16 bit files to least significant byte first */
   png_set_swap(png_ptr);

   /* Add filler (or alpha) byte (before/after each RGB triplet) */
   png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

#ifdef PNG_READ_INTERLACING_SUPPORTED
   /* Turn on interlace handling.  REQUIRED if you are not using
    * png_read_image().  To see how to handle interlacing passes,
    * see the png_read_row() method below:
    */
   number_passes = png_set_interlace_handling(png_ptr);
#else
   number_passes = 1;
#endif /* PNG_READ_INTERLACING_SUPPORTED */


   /* Optional call to gamma correct and add the background to the palette
    * and update info structure.  REQUIRED if you are expecting libpng to
    * update the palette for you (ie you selected such a transform above).
    */
   png_read_update_info(png_ptr, info_ptr);

   /* Allocate the memory to hold the image using the fields of info_ptr. */

   /* The easiest way to read the image: */
   png_bytep row_pointers[height];

   /* Clear the pointer array */
   for (row = 0; row < height; row++)
      row_pointers[row] = NULL;

   for (row = 0; row < height; row++)
      row_pointers[row] = png_malloc(png_ptr, png_get_rowbytes(png_ptr,
         info_ptr));

   /* Now it's time to read the image.  One of these methods is REQUIRED */
#ifdef entire /* Read the entire image in one go */
   png_read_image(png_ptr, row_pointers);

#else no_entire /* Read the image one or more scanlines at a time */
   /* The other way to read images - deal with interlacing: */

   for (pass = 0; pass < number_passes; pass++)
   {
#ifdef single /* Read the image a single row at a time */
      for (y = 0; y < height; y++)
      {
         png_read_rows(png_ptr, &row_pointers[y], NULL, 1);
      }

#else no_single /* Read the image several rows at a time */
      for (y = 0; y < height; y += number_of_rows)
      {
#ifdef sparkle /* Read the image using the "sparkle" effect. */
         png_read_rows(png_ptr, &row_pointers[y], NULL,
            number_of_rows);
#else no_sparkle /* Read the image using the "rectangle" effect */
         png_read_rows(png_ptr, NULL, &row_pointers[y],
            number_of_rows);
#endif no_sparkle /* Use only one of these two methods */
      }

      /* If you want to display the image after every pass, do so here */
#endif no_single /* Use only one of these two methods */
   }
#endif no_entire /* Use only one of these two methods */

   /* Read rest of file, and get additional chunks in info_ptr - REQUIRED */
   png_read_end(png_ptr, info_ptr);
#endif hilevel

   /* At this point you have read the entire image */

   /* Clean up after the read, and free any memory allocated - REQUIRED */
   png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

   /* Close the file */
   fclose(fp);

   /* That's it */
   return (OK);
}

/* Progressively read a file */

int
initialize_png_reader(png_structp *png_ptr, png_infop *info_ptr)
{
   /* Create and initialize the png_struct with the desired error handler
    * functions.  If you want to use the default stderr and longjump method,
    * you can supply NULL for the last three parameters.  We also check that
    * the library version is compatible in case we are using dynamically
    * linked libraries.
    */
   *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
       png_voidp user_error_ptr, user_error_fn, user_warning_fn);

   if (*png_ptr == NULL)
   {
      *info_ptr = NULL;
      return (ERROR);
   }

   *info_ptr = png_create_info_struct(png_ptr);

   if (*info_ptr == NULL)
   {
      png_destroy_read_struct(png_ptr, info_ptr, NULL);
      return (ERROR);
   }

   if (setjmp(png_jmpbuf((*png_ptr))))
   {
      png_destroy_read_struct(png_ptr, info_ptr, NULL);
      return (ERROR);
   }

   /* This one's new.  You will need to provide all three
    * function callbacks, even if you aren't using them all.
    * If you aren't using all functions, you can specify NULL
    * parameters.  Even when all three functions are NULL,
    * you need to call png_set_progressive_read_fn().
    * These functions shouldn't be dependent on global or
    * static variables if you are decoding several images
    * simultaneously.  You should store stream specific data
    * in a separate struct, given as the second parameter,
    * and retrieve the pointer from inside the callbacks using
    * the function png_get_progressive_ptr(png_ptr).
    */
   png_set_progressive_read_fn(*png_ptr, (void *)stream_data,
      info_callback, row_callback, end_callback);

   return (OK);
}

int
process_data(png_structp *png_ptr, png_infop *info_ptr,
   png_bytep buffer, png_uint_32 length)
{
   if (setjmp(png_jmpbuf((*png_ptr))))
   {
      /* Free the png_ptr and info_ptr memory on error */
      png_destroy_read_struct(png_ptr, info_ptr, NULL);
      return (ERROR);
   }

   /* This one's new also.  Simply give it chunks of data as
    * they arrive from the data stream (in order, of course).
    * On segmented machines, don't give it any more than 64K.
    * The library seems to run fine with sizes of 4K, although
    * you can give it much less if necessary (I assume you can
    * give it chunks of 1 byte, but I haven't tried with less
    * than 256 bytes yet).  When this function returns, you may
    * want to display any rows that were generated in the row
    * callback, if you aren't already displaying them there.
    */
   png_process_data(*png_ptr, *info_ptr, buffer, length);
   return (OK);
}

info_callback(png_structp png_ptr, png_infop info)
{
   /* Do any setup here, including setting any of the transformations
    * mentioned in the Reading PNG files section.  For now, you _must_
    * call either png_start_read_image() or png_read_update_info()
    * after all the transformations are set (even if you don't set
    * any).  You may start getting rows before png_process_data()
    * returns, so this is your last chance to prepare for that.
    */
}

row_callback(png_structp png_ptr, png_bytep new_row,
   png_uint_32 row_num, int pass)
{
   /*
    * This function is called for every row in the image.  If the
    * image is interlaced, and you turned on the interlace handler,
    * this function will be called for every row in every pass.
    *
    * In this function you will receive a pointer to new row data from
    * libpng called new_row that is to replace a corresponding row (of
    * the same data format) in a buffer allocated by your application.
    *
    * The new row data pointer "new_row" may be NULL, indicating there is
    * no new data to be replaced (in cases of interlace loading).
    *
    * If new_row is not NULL then you need to call
    * png_progressive_combine_row() to replace the corresponding row as
    * shown below:
    */

   /* Get pointer to corresponding row in our
    * PNG read buffer.
    */
   png_bytep old_row = ((png_bytep *)our_data)[row_num];

#ifdef PNG_READ_INTERLACING_SUPPORTED
   /* If both rows are allocated then copy the new row
    * data to the corresponding row data.
    */
   if ((old_row != NULL) && (new_row != NULL))
   png_progressive_combine_row(png_ptr, old_row, new_row);

   /*
    * The rows and passes are called in order, so you don't really
    * need the row_num and pass, but I'm supplying them because it
    * may make your life easier.
    *
    * For the non-NULL rows of interlaced images, you must call
    * png_progressive_combine_row() passing in the new row and the
    * old row, as demonstrated above.  You can call this function for
    * NULL rows (it will just return) and for non-interlaced images
    * (it just does the png_memcpy for you) if it will make the code
    * easier.  Thus, you can just do this for all cases:
    */

   png_progressive_combine_row(png_ptr, old_row, new_row);

   /* where old_row is what was displayed for previous rows.  Note
    * that the first pass (pass == 0 really) will completely cover
    * the old row, so the rows do not have to be initialized.  After
    * the first pass (and only for interlaced images), you will have
    * to pass the current row as new_row, and the function will combine
    * the old row and the new row.
    */
#endif /* PNG_READ_INTERLACING_SUPPORTED */
}

end_callback(png_structp png_ptr, png_infop info)
{
   /* This function is called when the whole image has been read,
    * including any chunks after the image (up to and including
    * the IEND).  You will usually have the same info chunk as you
    * had in the header, although some data may have been added
    * to the comments and time fields.
    *
    * Most people won't do much here, perhaps setting a flag that
    * marks the image as finished.
    */
}

/* Write a png file */
void write_png(char *file_name /* , ... other image information ... */)
{
   FILE *fp;
   png_structp png_ptr;
   png_infop info_ptr;
   png_colorp palette;

   /* Open the file */
   fp = fopen(file_name, "wb");
   if (fp == NULL)
      return (ERROR);

   /* Create and initialize the png_struct with the desired error handler
    * functions.  If you want to use the default stderr and longjump method,
    * you can supply NULL for the last three parameters.  We also check that
    * the library version is compatible with the one used at compile time,
    * in case we are using dynamically linked libraries.  REQUIRED.
    */
   png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
      png_voidp user_error_ptr, user_error_fn, user_warning_fn);

   if (png_ptr == NULL)
   {
      fclose(fp);
      return (ERROR);
   }

   /* Allocate/initialize the image information data.  REQUIRED */
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL)
   {
      fclose(fp);
      png_destroy_write_struct(&png_ptr,  NULL);
      return (ERROR);
   }

   /* Set error handling.  REQUIRED if you aren't supplying your own
    * error handling functions in the png_create_write_struct() call.
    */
   if (setjmp(png_jmpbuf(png_ptr)))
   {
      /* If we get here, we had a problem writing the file */
      fclose(fp);
      png_destroy_write_struct(&png_ptr, &info_ptr);
      return (ERROR);
   }

   /* One of the following I/O initialization functions is REQUIRED */

#ifdef streams /* I/O initialization method 1 */
   /* Set up the output control if you are using standard C streams */
   png_init_io(png_ptr, fp);

#else no_streams /* I/O initialization method 2 */
   /* If you are using replacement write functions, instead of calling
    * png_init_io() here you would call
    */
   png_set_write_fn(png_ptr, (void *)user_io_ptr, user_write_fn,
      user_IO_flush_function);
   /* where user_io_ptr is a structure you want available to the callbacks */
#endif no_streams /* Only use one initialization method */

#ifdef hilevel
   /* This is the easy way.  Use it if you already have all the
    * image info living in the structure.  You could "|" many
    * PNG_TRANSFORM flags into the png_transforms integer here.
    */
   png_write_png(png_ptr, info_ptr, png_transforms, NULL);

#else
   /* This is the hard way */

   /* Set the image information here.  Width and height are up to 2^31,
    * bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
    * the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
    * PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
    * or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
    * PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
    * currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
    */
   png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, PNG_COLOR_TYPE_???,
      PNG_INTERLACE_????, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

   /* Set the palette if there is one.  REQUIRED for indexed-color images */
   palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH
             * png_sizeof(png_color));
   /* ... Set palette colors ... */
   png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
   /* You must not free palette here, because png_set_PLTE only makes a link to
    * the palette that you malloced.  Wait until you are about to destroy
    * the png structure.
    */

   /* Optional significant bit (sBIT) chunk */
   png_color_8 sig_bit;

   /* If we are dealing with a grayscale image then */
   sig_bit.gray = true_bit_depth;

   /* Otherwise, if we are dealing with a color image then */
   sig_bit.red = true_red_bit_depth;
   sig_bit.green = true_green_bit_depth;
   sig_bit.blue = true_blue_bit_depth;

   /* If the image has an alpha channel then */
   sig_bit.alpha = true_alpha_bit_depth;

   png_set_sBIT(png_ptr, info_ptr, &sig_bit);


   /* Optional gamma chunk is strongly suggested if you have any guess
    * as to the correct gamma of the image.
    */
   png_set_gAMA(png_ptr, info_ptr, gamma);

   /* Optionally write comments into the image */
   {
      png_text text_ptr[3];

      char key0[]="Title";
      char text0[]="Mona Lisa";
      text_ptr[0].key = key0;
      text_ptr[0].text = text0;
      text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
      text_ptr[0].itxt_length = 0;
      text_ptr[0].lang = NULL;
      text_ptr[0].lang_key = NULL;

      char key1[]="Author";
      char text1[]="Leonardo DaVinci";
      text_ptr[1].key = key1;
      text_ptr[1].text = text1;
      text_ptr[1].compression = PNG_TEXT_COMPRESSION_NONE;
      text_ptr[1].itxt_length = 0;
      text_ptr[1].lang = NULL;
      text_ptr[1].lang_key = NULL;

      char key2[]="Description";
      char text2[]="<long text>";
      text_ptr[2].key = key2;
      text_ptr[2].text = text2;
      text_ptr[2].compression = PNG_TEXT_COMPRESSION_zTXt;
      text_ptr[2].itxt_length = 0;
      text_ptr[2].lang = NULL;
      text_ptr[2].lang_key = NULL;

      png_set_text(write_ptr, write_info_ptr, text_ptr, 3);
   }

   /* Other optional chunks like cHRM, bKGD, tRNS, tIME, oFFs, pHYs */

   /* Note that if sRGB is present the gAMA and cHRM chunks must be ignored
    * on read and, if your application chooses to write them, they must
    * be written in accordance with the sRGB profile
    */

   /* Write the file header information.  REQUIRED */
   png_write_info(png_ptr, info_ptr);

   /* If you want, you can write the info in two steps, in case you need to
    * write your private chunk ahead of PLTE:
    *
    *   png_write_info_before_PLTE(write_ptr, write_info_ptr);
    *   write_my_chunk();
    *   png_write_info(png_ptr, info_ptr);
    *
    * However, given the level of known- and unknown-chunk support in 1.2.0
    * and up, this should no longer be necessary.
    */

   /* Once we write out the header, the compression type on the text
    * chunks gets changed to PNG_TEXT_COMPRESSION_NONE_WR or
    * PNG_TEXT_COMPRESSION_zTXt_WR, so it doesn't get written out again
    * at the end.
    */

   /* Set up the transformations you want.  Note that these are
    * all optional.  Only call them if you want them.
    */

   /* Invert monochrome pixels */
   png_set_invert_mono(png_ptr);

   /* Shift the pixels up to a legal bit depth and fill in
    * as appropriate to correctly scale the image.
    */
   png_set_shift(png_ptr, &sig_bit);

   /* Pack pixels into bytes */
   png_set_packing(png_ptr);

   /* Swap location of alpha bytes from ARGB to RGBA */
   png_set_swap_alpha(png_ptr);

   /* Get rid of filler (OR ALPHA) bytes, pack XRGB/RGBX/ARGB/RGBA into
    * RGB (4 channels -> 3 channels). The second parameter is not used.
    */
   png_set_filler(png_ptr, 0, PNG_FILLER_BEFORE);

   /* Flip BGR pixels to RGB */
   png_set_bgr(png_ptr);

   /* Swap bytes of 16-bit files to most significant byte first */
   png_set_swap(png_ptr);

   /* Swap bits of 1, 2, 4 bit packed pixel formats */
   png_set_packswap(png_ptr);

   /* Turn on interlace handling if you are not using png_write_image() */
   if (interlacing)
      number_passes = png_set_interlace_handling(png_ptr);

   else
      number_passes = 1;

   /* The easiest way to write the image (you may have a different memory
    * layout, however, so choose what fits your needs best).  You need to
    * use the first method if you aren't handling interlacing yourself.
    */
   png_uint_32 k, height, width;

   /* In this example, "image" is a one-dimensional array of bytes */
   png_byte image[height*width*bytes_per_pixel];

   png_bytep row_pointers[height];

   if (height > PNG_UINT_32_MAX/png_sizeof(png_bytep))
     png_error (png_ptr, "Image is too tall to process in memory");

   /* Set up pointers into your "image" byte array */
   for (k = 0; k < height; k++)
     row_pointers[k] = image + k*width*bytes_per_pixel;

   /* One of the following output methods is REQUIRED */

#ifdef entire /* Write out the entire image data in one call */
   png_write_image(png_ptr, row_pointers);

   /* The other way to write the image - deal with interlacing */

#else no_entire /* Write out the image data by one or more scanlines */

   /* The number of passes is either 1 for non-interlaced images,
    * or 7 for interlaced images.
    */
   for (pass = 0; pass < number_passes; pass++)
   {
      /* Write a few rows at a time. */
      png_write_rows(png_ptr, &row_pointers[first_row], number_of_rows);

      /* If you are only writing one row at a time, this works */
      for (y = 0; y < height; y++)
         png_write_rows(png_ptr, &row_pointers[y], 1);
   }
#endif no_entire /* Use only one output method */

   /* You can write optional chunks like tEXt, zTXt, and tIME at the end
    * as well.  Shouldn't be necessary in 1.2.0 and up as all the public
    * chunks are supported and you can use png_set_unknown_chunks() to
    * register unknown chunks into the info structure to be written out.
    */

   /* It is REQUIRED to call this to finish writing the rest of the file */
   png_write_end(png_ptr, info_ptr);
#endif hilevel

   /* If you png_malloced a palette, free it here (don't free info_ptr->palette,
    * as recommended in versions 1.0.5m and earlier of this example; if
    * libpng mallocs info_ptr->palette, libpng will free it).  If you
    * allocated it with malloc() instead of png_malloc(), use free() instead
    * of png_free().
    */
   png_free(png_ptr, palette);
   palette = NULL;

   /* Similarly, if you png_malloced any data that you passed in with
    * png_set_something(), such as a hist or trans array, free it here,
    * when you can be sure that libpng is through with it.
    */
   png_free(png_ptr, trans);
   trans = NULL;
   /* Whenever you use png_free() it is a good idea to set the pointer to
    * NULL in case your application inadvertently tries to png_free() it
    * again.  When png_free() sees a NULL it returns without action, thus
    * avoiding the double-free security problem.
    */

   /* Clean up after the write, and free any memory allocated */
   png_destroy_write_struct(&png_ptr, &info_ptr);

   /* Close the file */
   fclose(fp);

   /* That's it */
   return (OK);
}
#endif /* if 0 */
