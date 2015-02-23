/*
 * Version 0.7.0
 * Copyright 2008 Clemens Eisserer
 * License: GPL3
 */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>


#include "glyphs.h"

Display *display;
int screen;
Window root, window;
XEvent event;

XRenderColor color_white={.red=0xffff, .green=0xffff, .blue=0xffff, .alpha=0xffff};
XRenderColor color_red={.red=0xffff, .green=0, .blue=0, .alpha=0x4fff};
XRenderColor color_yellow={.red=0xffff, .green=0xffff, .blue=0, .alpha=0xffff};
XRenderColor color_red2={.red=0xffff, .green=0, .blue=0, .alpha=0xffff};
XRenderColor color_blue={.red=0, .green=0, .blue=0xffff, .alpha=0xffff};

XRenderPictFormat *fmtA8;
Pixmap maskPixmap;
Picture alphaMask;
Picture setAlphaPict;
XImage *alphaImg;
XRenderColor clearAlpha = {.red=0, .green=0, .blue=0, .alpha=0};
XRenderColor setAlpha = {.red=0, .green=0, .blue=0, .alpha=0xffff};
GC alphaGC;
char* alphaData;

Pixmap linePixmap;
Picture linePicture;
GC lineSetGC;
GC lineClearGC;

Picture backBuffer;
Picture sourcePicture;
Picture sourceImage;
Picture texture_small;
Picture texture_large;
Picture textSource;

Picture gradCache;
Picture linearGradient;
Picture radialGradient;

GlyphSet gs12a8;
GlyphSet gs12argb32;
int num_only = 0;
int rt_screen = 0;


  char cmdArray[] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 5, 5, 5, 4, 4, 4, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 16, 16, 17, 17};

  int sizeArray[] = {15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 5, 15, 5, 15};

   /*Default tests*/
   char cmdArrayJ[] = {0, 0, 0, 2, 2, 2, 3, 3, 3, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 15, 15, 15, 16, 16, 17, 17};
   int sizeArrayJ[] = {15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 15, 75, 250, 5, 15, 5, 15};

  /*Execute only Java relevant benchmarks by default*/
  char *cmds = &cmdArrayJ[0];
  int *sizes = &sizeArrayJ[0];
  int cmdCnt = 34;


  char* names[] = {"rects (!)", "rects blended", "rects composition (!)", "put composition (!)", "lines composition (!)", "mixed composition (!)", "traps composition", "trapezoids with temporary surface composition", "Simple Blit (!)", "Transformed Blit Linear (!)", "Transformed Blit Billinear sharp edges (!)", "small transformed TexturePaint (10x10) (!)", "large transformed TexturePaint (256x256) (!)", "linear gradient blend", "radial gradient paint", "gradient+temp texture(!)", "12pt text grayscale", "12pt text lcd"};

Picture create_pen(int red, int green, int blue, int alpha, int width, int height, int depth, char* sm)
{
	XRenderColor color={.red=red, .green=green, .blue=blue, .alpha=alpha};
	XRenderPictFormat *fmt;
        if(depth == 24) {
            fmt = XRenderFindStandardFormat(display, PictStandardRGB24);
         }else {
            fmt = XRenderFindStandardFormat(display, PictStandardARGB32);
         }
	
	Pixmap pm = XCreatePixmap(display, window, width, height, depth);
	XRenderPictureAttributes pict_attr;
	pict_attr.repeat=RepeatNormal;
	Picture picture = XRenderCreatePicture(display, pm, fmt, CPRepeat, &pict_attr);
	XRenderFillRectangle(display, PictOpSrc, picture, &color, 0, 0, 256, 256);

        /*if(sm != NULL) {
          int bytes = width*height*(depth/8);
          char* heap = (char*) malloc(bytes);
          memcpy(heap, sm, bytes);

          XGCValues values;
          GC gc = XCreateGC(display, pm, 0, &values); 

          XImage* img = XCreateImage(display, (Visual*) &pm, depth, ZPixmap, 0, NULL, width, height, 32, 0); 
          XPutImage(display, pm, gc, img, 0, 0, 0, 0, width, height);
        }*/

	return picture;
}

Picture createMaskTile()
{
        XRenderColor color_black={.red=0, .green=0, .blue=0, .alpha=0xffff};
	XRenderPictureAttributes pict_attr;
	pict_attr.repeat=RepeatNone;

	maskPixmap = XCreatePixmap(display, window, 256, 256, 8);
        linePixmap = XCreatePixmap(display, window, 256, 256, 8);

	Picture picture = XRenderCreatePicture(display, maskPixmap, fmtA8, CPRepeat, &pict_attr);
        linePicture = XRenderCreatePicture(display, linePixmap, fmtA8, CPRepeat, &pict_attr);
        XRenderFillRectangle (display, PictOpClear, picture, &color_black, 0, 0, 256, 256);
        XRenderFillRectangle (display, PictOpClear, linePicture, &color_black, 0, 0, 256, 256);

        XGCValues values;
        values.graphics_exposures = 0;
        alphaGC = XCreateGC(display, maskPixmap, GCGraphicsExposures, &values);
        lineSetGC =  XCreateGC(display, linePixmap, 0, &values);
        lineClearGC =  XCreateGC(display, linePixmap, 0, &values);
        XSetForeground(display, lineSetGC, 255);
        XSetForeground(display, lineClearGC, 0);

        alphaData = malloc(256*256);
        alphaImg = XCreateImage(display, (Visual*) &maskPixmap, 8, ZPixmap, 0, alphaData, 256, 256, 8, 0); 
        int i;
        for(i=0; i < (256*256); i++)
        {
          alphaData[i] = i%5==0 ? 255 : 0;
        }

	return picture;
}

void create12GlpyhSetA8()
{
	XRenderPictFormat *fmt_a8 = XRenderFindStandardFormat(display, PictStandardA8);
	GlyphSet gs = XRenderCreateGlyphSet(display, fmt_a8);
	Glyph gid;
	XGlyphInfo ginfo={.x=0,.y=12,.width=12,.height=16,.xOff=12,.yOff=0};
	
	gid=0;
	XRenderAddGlyphs(display, gs, &gid, &ginfo, 1, glyph_data_e, 12*16);
	gid=1;
	XRenderAddGlyphs(display, gs, &gid, &ginfo, 1, glyph_data_l, 12*16);
	gid=2;
	XRenderAddGlyphs(display, gs, &gid, &ginfo, 1, glyph_data_o, 12*16);
	gid=3;
	XRenderAddGlyphs(display, gs, &gid, &ginfo, 1, glyph_data_space, 12*16);
	gid=4;
	XRenderAddGlyphs(display, gs, &gid, &ginfo, 1, glyph_data_W, 12*16);
	gid=5;
	XRenderAddGlyphs(display, gs, &gid, &ginfo, 1, glyph_data_r, 12*16);
	gid=6;
	XRenderAddGlyphs(display, gs, &gid, &ginfo, 1, glyph_data_d, 12*16);
	gid=7;
	XRenderAddGlyphs(display, gs, &gid, &ginfo, 1, glyph_data_excl, 12*16);

       gs12a8 = gs;
}

void create12GlpyhSetARGB32()
{
	XRenderPictFormat *fmt_argb32 = XRenderFindStandardFormat(display, PictStandardARGB32);
	GlyphSet gs = XRenderCreateGlyphSet(display, fmt_argb32);
	Glyph gid;
	XGlyphInfo ginfo={.x=0,.y=12,.width=10,.height=9,.xOff=12,.yOff=0};
        int i;

        for(i=0; i < 8; i++) {
	  gid=i;
	  XRenderAddGlyphs(display, gs, &gid, &ginfo, 1, glyph_data_H, 10*9*4);
        }
         
       gs12argb32 = gs;
}


void createGradients()
{
        XLinearGradient grad;
        grad.p1.x = 0;
        grad.p1.y = 0;
        grad.p2.x = 75<<16;
        grad.p2.y = 75<<16;

        XRadialGradient rgrad;
        rgrad.inner.x = 0;
        rgrad.inner.y = 0;
        rgrad.inner.radius = 75<<16;
        rgrad.outer.x = 0;
        rgrad.outer.y = 0;
        rgrad.outer.radius = 75<<16;

        /*TODO optimized & malloc check*/
        XRenderColor colors[3];
        XFixed stops[3];

        stops[0] = XDoubleToFixed(0.2);
        stops[1] = XDoubleToFixed(0.5);
        stops[2] = XDoubleToFixed(1.0);
        colors[0] = color_red;
        colors[1] = color_yellow;
        colors[2] = color_blue;

        linearGradient = XRenderCreateLinearGradient(display, &grad, stops, colors, 3);
        radialGradient = XRenderCreateRadialGradient(display, &rgrad, stops, colors, 3);

	XRenderPictureAttributes pict_attr;
        pict_attr.repeat = RepeatReflect;
        XRenderChangePicture (display, linearGradient, CPRepeat, &pict_attr);

        pict_attr.repeat = RepeatNormal;
        XRenderChangePicture (display, radialGradient, CPRepeat, &pict_attr);
}


void fillRectMaskTile(int area, int x, int y, int composite)
{
  if(composite > 0) {
    XRenderFillRectangle (display, PictOpClear, alphaMask, &clearAlpha, x, y, area, area);
  }

  int i;
  XRectangle rects[area];
  for(i=0; i < area; i++)
  {
     rects[i].x =      i + x;
     rects[i].y =      i + y;
     rects[i].width  = area / 2;  
     rects[i].height = 1;
  }

  XRenderFillRectangles (display, PictOpSrc, alphaMask, &setAlpha, &rects[0], area);

  if(composite > 0)
  XRenderComposite(display, PictOpOver, sourcePicture, alphaMask, backBuffer, 0, 0, x, y, x, y, area, area);
}


void fillRects(int area, int x, int y, int alpha)
{
  int i;
  XRectangle rects[area];
  for(i=0; i < area; i++)
  {
     rects[i].x =      i + x;
     rects[i].y =      i + y;
     rects[i].width  = area / 2;  
     rects[i].height = 1;
  }

  XRenderFillRectangles (display, PictOpOver, backBuffer, alpha > 0 ? &color_red : &color_yellow, &rects[0], area);
}


/*void fillRects(int area, int x, int y, int alpha)
{
  XRectangle rect;
  rect.x = x;
  rect.y = y;
  rect.width = area;
  rect.height = area;
  XRenderFillRectangles (display, PictOpOver, backBuffer, alpha > 0 ? &color_red : &color_yellow, &rects, 1);
}*/

void fillTrapMaskTile(int area, int x, int y)
{
  XRenderFillRectangle (display, PictOpClear, alphaMask, &clearAlpha, x, y, area, area);

  int trapCnt = (area - area/10)/3;
  int i;
  XTrap traps[trapCnt];
  for(i=0; i < trapCnt; i++)  {
     int top = y << 16;
     int bottom = (area + y) << 16;
     int left = (i*3 + x) << 16;
     int right = (i*3 + (area/10) + x) << 16;

     traps[i].top.left = left;
     traps[i].top.right = left + (1<<16);
     traps[i].top.y = top; 
     traps[i].bottom.left = right;
     traps[i].bottom.right =  right + (1<<16);
     traps[i].bottom.y = bottom;
  }

  XRenderAddTraps (display, alphaMask, 0, 0, &traps[0], trapCnt);
  XRenderComposite(display, PictOpOver, sourcePicture, alphaMask, backBuffer, 0, 0, x, y, x, y, area, area);
}

void fillTrapezoidMaskTile(int area, int x, int y)
{
  XRenderFillRectangle (display, PictOpClear, alphaMask, &clearAlpha, x, y, area, area);

  int trapCnt = (area - area/10)/3;
  int i;
  XTrapezoid traps[trapCnt];
  for(i=0; i < trapCnt; i++)  {                  
     int top = y << 16;
     int bottom = (area+y) << 16;
     int left = (i*3 + x) << 16;
     int right = (i*3 + (area/10) + x) << 16;

     traps[i].top= top;
     traps[i].bottom=bottom;
     traps[i].left.p1.x=left;
     traps[i].left.p1.y=top;
     traps[i].left.p2.x=right;
     traps[i].left.p2.y=bottom;

     traps[i].right.p1.x=left + (1<<16);
     traps[i].right.p1.y=top;
     traps[i].right.p2.x=right + (1<<16);
     traps[i].right.p2.y=bottom;
  }

  XRenderCompositeTrapezoids (display, PictOpSrc, setAlphaPict, alphaMask, fmtA8, 0, 0, &traps[0], trapCnt);
  XRenderComposite(display, PictOpOver, sourcePicture, alphaMask, backBuffer, 0, 0, x, y, x, y, area, area);
}

void fillLineMaskTile(int area, int x, int y, int composite)
{
  int lineCnt = (area - area/10)/3;
  int i;
  XSegment segs[lineCnt];
  for(i=0; i < lineCnt; i++) {
    segs[i].x1 = (short) (i*3) + x;
    segs[i].y1 = y;
    segs[i].x2 = (short) (i*3 + (area/10) + x);
    segs[i].y2 = (short) (area + y);
  }
 
  XDrawSegments(display, linePixmap, lineSetGC, &segs[0], lineCnt);

  if(composite)
  {
    XRenderComposite(display, PictOpOver, sourcePicture, linePicture, backBuffer, 0, 0, x, y, x, y, area, area);

    /* EXA does not even accalerate diagonal lines *arrg*
    * To eliminate readbacks/migration XRenderFillRect would trigger,
    * we delete lines, by rendering them again with 0 alpha.*/ 
    XDrawSegments(display, linePixmap, lineClearGC, &segs[0], lineCnt);
  }
}

void fillPutImage(int area, int x, int y, int composite)
{
  /*We don't need to clean everything, the used mask-area is overwritten completly*/
  XPutImage(display, maskPixmap, alphaGC, alphaImg, 0, 0, x, y, area, area);
  
  if(composite)
  XRenderComposite(display, PictOpOver, sourcePicture, alphaMask, backBuffer, 0, 0, x, y, x, y, area, area);
}

void mixed(int area, int x, int y)
{  
   fillLineMaskTile(area, x, y, 0);

   XCopyArea(display, linePixmap, maskPixmap, alphaGC, 0, 0, area, area, 0, 0);
   //XRenderComposite(display, PictOpSrc, linePicture, None, alphaMask, 0, 0, 0, 0, 0, 0, area, area);

   fillPutImage(area/2, x, y, 0);
   fillRectMaskTile(area, x, y, 0);

   XRenderComposite(display, PictOpOver, sourcePicture, alphaMask, backBuffer, 0, 0, x, y, x, y, area, area);
}

void resetPictureSettings(Picture pic)
{
   XTransform xf;
   xf.matrix[0][0] = XDoubleToFixed(1); 
   xf.matrix[0][1] = XDoubleToFixed(0); 
   xf.matrix[0][2] = XDoubleToFixed(0); 
   xf.matrix[1][0] = XDoubleToFixed(0); 
   xf.matrix[1][1] = XDoubleToFixed(1); 
   xf.matrix[1][2] = XDoubleToFixed(0);
   xf.matrix[2][0] = 0; 
   xf.matrix[2][1] = 0; 
   xf.matrix[2][2] = 1<<16;
   XRenderSetPictureTransform (display, pic, &xf);

   XRenderSetPictureFilter(display, pic, "fast", NULL, 0);

   XRenderPictureAttributes pict_attr;
   pict_attr.repeat = RepeatNone;
   XRenderChangePicture (display, pic, CPRepeat, &pict_attr);
}

void setupSimpleBlit()
{
   resetPictureSettings(sourceImage);
}


void simpleBlit(int area, int x, int y) 
{
  XRenderComposite(display, PictOpOver, sourceImage, None, backBuffer, 0, 0, 0, 0, x, y, area, area);
}

void setupLinearTransformedBlit()
{
   resetPictureSettings(sourceImage);

   XTransform xf;
  xf.matrix[0][0] = XDoubleToFixed(1.194170611407008); 
   xf.matrix[0][1] = XDoubleToFixed(0.369400258326674); 
   xf.matrix[0][2] = XDoubleToFixed(0); 
   xf.matrix[1][0] = XDoubleToFixed(-0.369400258326674); 
   xf.matrix[1][1] = XDoubleToFixed(1.194170611407008); 
   xf.matrix[1][2] = XDoubleToFixed(0);
   xf.matrix[2][0] = 0; 
   xf.matrix[2][1] = 0; 
   xf.matrix[2][2] = 1<<16;
   XRenderSetPictureTransform (display, sourceImage, &xf);
}

void linearTransformedBlit(int area, int x, int y)
{
  XRenderComposite(display, PictOpOver, sourceImage, None, backBuffer, 0, 0, 0, 0, x, y, area, area);
}

void billinearTransformedBlitSetup()
{
   /*Setup source Image*/
   resetPictureSettings(sourceImage);

   XRenderPictureAttributes pict_attr;
   pict_attr.repeat = RepeatPad;
   XRenderChangePicture (display, sourceImage, CPRepeat, &pict_attr);

   XRenderSetPictureFilter(display, sourceImage, "good", NULL, 0);

   /*Clear mask setup*/
   XRenderFillRectangle (display, PictOpClear, alphaMask, &clearAlpha, 0, 0, 32767, 32767);
}

void billinearTransformedBlit(int area, int x, int y)
{
  int maskSize = area/8;
  maskSize = maskSize == 0 ? 1 : area;
  area = maskSize*8;

  XRenderFillRectangle (display, PictOpSrc, alphaMask, &setAlpha, 0, 0, maskSize, maskSize);

  XTransform xf;
  xf.matrix[0][0] = XDoubleToFixed(1.194170611407008); 
   xf.matrix[0][1] = XDoubleToFixed(0.369400258326674); 
   xf.matrix[0][2] = XDoubleToFixed(0); 
   xf.matrix[1][0] = XDoubleToFixed(-0.369400258326674); 
   xf.matrix[1][1] = XDoubleToFixed(1.194170611407008); 
   xf.matrix[1][2] = XDoubleToFixed(0);
   xf.matrix[2][0] = 0; 
   xf.matrix[2][1] = 0; 
   xf.matrix[2][2] = 1<<16;
  XRenderSetPictureTransform (display, sourceImage, &xf);

  xf.matrix[0][0] = XDoubleToFixed(0.149271326425876); 
  xf.matrix[0][1] = XDoubleToFixed(0.046175032290834); 
  xf.matrix[1][0] = XDoubleToFixed(-0.046175032290834); 
  xf.matrix[1][1] = XDoubleToFixed(0.149271326425876); 
  XRenderSetPictureTransform (display, alphaMask, &xf);

  XRenderComposite(display, PictOpOver, sourceImage, alphaMask, backBuffer, 0, 0, 0, 0, x, y, area, area);  

  /*Restore mask properties*/
  XRenderFillRectangle (display, PictOpClear, alphaMask, &setAlpha, 0, 0, maskSize, maskSize);
  
  xf.matrix[0][0] = XDoubleToFixed(1); 
  xf.matrix[0][1] = XDoubleToFixed(0); 
  xf.matrix[1][0] = XDoubleToFixed(0); 
  xf.matrix[1][1] = XDoubleToFixed(1); 
  XRenderSetPictureTransform (display, alphaMask, &xf);
}



void transformedTexturePaintSetup(Picture texture)
{
   /*Setup source Image*/
   resetPictureSettings(texture);

   XRenderPictureAttributes pict_attr;
   pict_attr.repeat = RepeatNormal;
   XRenderChangePicture (display, texture, CPRepeat, &pict_attr);

   XTransform xf;
   xf.matrix[0][0] = XDoubleToFixed(0.318445496375202); 
   xf.matrix[0][1] = XDoubleToFixed(0.09850673555378); 
   xf.matrix[0][2] = XDoubleToFixed(0); 
   xf.matrix[1][0] = XDoubleToFixed(-0.09850673555378); 
   xf.matrix[1][1] = XDoubleToFixed(0.318445496375202); 
   xf.matrix[1][2] = XDoubleToFixed(0);
   xf.matrix[2][0] = 0; 
   xf.matrix[2][1] = 0; 
   xf.matrix[2][2] = 1<<16;
   XRenderSetPictureTransform (display, texture, &xf);

   XRenderSetPictureFilter(display, texture, "good", NULL, 0);

   /*Clear mask setup*/
   XRenderFillRectangle (display, PictOpClear, alphaMask, &clearAlpha, 0, 0, 32767, 32767);
}

void transformedTexturePaint(Picture texture, int area, int x, int y)
{
  XRenderFillRectangle (display, PictOpSrc, alphaMask, &setAlpha, 0, 0, area, area);

  XRenderComposite(display, PictOpOver, texture, alphaMask, backBuffer, 0, 0, 0, 0, x, y, area, area);

  XRenderFillRectangle (display, PictOpClear, alphaMask, &setAlpha, 0, 0, area, area);
}

void gradientPaint(int area, int x, int y, Picture grad)
{
  XRenderFillRectangle (display, PictOpSrc, alphaMask, &setAlpha, 0, 0, area, area);

  XRenderComposite(display, PictOpOver, grad, alphaMask, backBuffer, 0, 0, 0, 0, x, y, area, area);

  XRenderFillRectangle (display, PictOpClear, alphaMask, &setAlpha, 0, 0, area, area);
}

void gradientPaintTemp(int area, int x, int y)
{
  XRenderFillRectangle (display, PictOpSrc, alphaMask, &setAlpha, 0, 0, area, area);

  XRenderComposite(display, PictOpSrc, linearGradient, None, gradCache, 0, 0, 0, 0, 0, 0, area, area);
  XRenderComposite(display, PictOpOver, gradCache, alphaMask, backBuffer, 0, 0, 0, 0, x, y, area, area);

  XRenderFillRectangle (display, PictOpClear, alphaMask, &setAlpha, 0, 0, area, area);
}

void text(GlyphSet gs, int chars, int x, int y)
{
  int i;
  char gids[chars]; 
  
  for(i=0; i < chars; i++) {
   gids[i] = i%8;
  }

  XGlyphElt8 elt[1];
  elt[0].glyphset = gs;
  elt[0].chars = (const char*) &gids;
  elt[0].nchars = chars;
  elt[0].xOff = x;
  elt[0].yOff = y+16;

  XRenderCompositeText8(display, PictOpOver, textSource, backBuffer, 0, 0, 0, 0, 0, &elt[0], 1);
}


long usec()
{
  struct timeval tv;
  struct timezone tz;
  struct tm *tm;
  gettimeofday(&tv, &tz);
  tm=localtime(&tv.tv_sec);
  return ((long) tv.tv_sec)*750 + tv.tv_usec/750;
}

int executeSeries(char cmd, int maskSize, int size, int repititions, int benchs)
{ 
  int run, reps, i;
  int times[benchs];

  XSync(display, 0);

  switch(cmd)
  {
    case 8:
      setupSimpleBlit();
      break;

    case 9:
     setupLinearTransformedBlit();
     break;

    case 10:
      billinearTransformedBlitSetup();
      break;

    case 11:
      transformedTexturePaintSetup(texture_small);
      break;

    case 12:
      transformedTexturePaintSetup(texture_large);
      break;
  }


  char* names[] = {"rects (!)", "rects blended", "rects composition (!)", "put composition (!)", "lines composition (!)", "mixed composition (!)", "traps composition", "trapezoids with temporary surface composition", "Simple Blit (!)", "Transformed Blit Linear (!)", "Transformed Blit Billinear sharp edges (!)", "small transformed TexturePaint (10x10) (!)", "large transformed TexturePaint (256x256) (!)", "linear gradient blend", "radial gradient paint", "gradient+temp texture(!)", "12pt text grayscale", "12pt text lcd"};

  for(run=0; run < benchs; run++)
  {
    XSync(display, 0);

    long stime = usec();
    for(reps=0; reps < repititions; reps++)
    {
        int x = 0; //i % (maskSize - size);
        int y = x;

        switch(cmd) //Yes I know, there are function pointers in C ;)
        {
          case 0:
            fillRects(size, x, y, 0);
            break;

          case 1:
            fillRects(size, x, y, 1);
            break;

          case 2:
            fillRectMaskTile(size, x, y, 1);
            break;

           case 3:
            fillPutImage(size, x, y, 1);
            break;

           case 4:
            fillLineMaskTile(size, x, y, 1);
            break;

          case 5:
            mixed(size, x, y);
            break;

          case 6:
            fillTrapMaskTile(size, x, y);
            break;

          case 7:
            fillTrapezoidMaskTile(size, x, y);
            break;

          case 8:
            simpleBlit(size, x, y);
            break;

          case 9:
           linearTransformedBlit(size, x, y);
           break;

          case 10:
            billinearTransformedBlit(size, x, y);
            break;

          case 11:
            transformedTexturePaint(texture_small, size, x, y);
            break;

          case 12:
            transformedTexturePaint(texture_large, size, x, y);
            break;
        
           case 13:
            gradientPaint(size, x, y, linearGradient);
            break;

          case 14:
            gradientPaint(size, x, y, radialGradient);
            break;

          case 15:
            gradientPaintTemp(size, x, y);
            break;

          case 16:
             text(gs12a8, size, x, y);
             break;

          case 17:
             text(gs12argb32, size, x, y);
             break;
                        
          default:
            printf("Wrong command-opcode!");
        }
      }

     XSync(display, 0);
     long etime = usec();
     times[run] = etime - stime;
  }

  /*arithemtisches mittel*/
  int arith = 0;
  for(run = 0; run < benchs; run++)
  {
   arith += times[run];
  }
  arith /= benchs;

  return arith;
}

int containsArgument (char argument[], int argc, char* argv[])
{
  int i;
  for(i=1; i < argc; i++) {
     if(strcmp(argv[i], &argument[0]) == 0) {
       return 1;
     }
  }
  
  return 0;
}

void parseParameters(int argc, char *argv[])
{
  int help = containsArgument("-help\0", argc, argv);
  int all = containsArgument("-all\0", argc, argv);
  num_only = containsArgument("-num\0", argc, argv);
  rt_screen = containsArgument("-screen\0", argc, argv);

  int extraArgs = help + all + num_only + rt_screen;

  all = 0;

   if(!help) {
      if(all) {
          cmds = &cmdArray[0];
          sizes = &sizeArray[0];
          cmdCnt = 51;
      }else
      {
          int i;
          int start = extraArgs + 1;
          int cnt = (argc - extraArgs)/2;
          if(cnt > 0) {
            cmdCnt = cnt;
            sizes = (int*) malloc(sizeof(int) * cmdCnt);
            cmds = (void*) malloc(sizeof(char) * cmdCnt);
        
              for(i=start; i < argc; i+=2) {
                cmds[(i - start) / 2] = atoi(argv[i]);
                sizes[(i - start) / 2] = atoi(argv[i + 1]);
              }
          }
      }
   }else {
      int i=0;
      printf("JXRenderMark 0.7\n");
      printf("parameters: opType sopSize(10-250)\n\n");
      printf("The following operations are available:\n");
      for(i=0; i < 16; i++) {
          printf("Opcode %d: %s\n", i, names[i]);
      }
      printf("\nExample: ./putperf 0 128 1 64 3 256 ....\n");
      printf("means: 1. rectangles 128x128, 2. putImage 64x64, 3. ...\n");
      printf("Example: ./putperf; run only tests currently used by the XRender pipeline (Default)\n");
      printf("Example: ./putperf -num; only print numbers\n");
      printf("Example: ./putperf -screen; renders directly to screen instead of an backbuffer\n");
      //printf("Example: ./putperf -all; run all tests; -all includes functionality we really would *like* to use in later releases, but can't now because its broken\n");

      exit(1);
  }
}

 #define RUN_TARGET 750

int estimateRunCount(char cmd, int mask_size, int size) 
{
   int tries = 5;
   int lastTook = 0;
   while(lastTook < (RUN_TARGET*0.75)) {
     lastTook =  executeSeries(cmd, mask_size, size, tries, 1);
     if(lastTook > 10) {
        tries = (((double) tries)/lastTook)*RUN_TARGET;
     }else {
        tries *= (RUN_TARGET/10);
     }
   }

   return tries;
}

int main(int argc, char *argv[])
{
  parseParameters(argc, argv);
	
  display=XOpenDisplay(NULL);


	int render_event_base, render_error_base;
	int render_present=XRenderQueryExtension(display, &render_event_base, &render_error_base);
	if (!render_present) {
		fprintf(stderr, "RENDER extension missing!\n");
		abort();
	}
	
	/* obtain a few parameters */
	XRenderPictFormat *fmt=XRenderFindStandardFormat(display, PictStandardRGB24);
	screen=DefaultScreen(display);
	root=DefaultRootWindow(display);
	
	/* create window and all related resources */
	window = XCreateWindow(display, root, 0, 0, 640, 480, 0,
		DefaultDepth(display, screen), InputOutput,
		DefaultVisual(display, screen), 
		0, NULL);
	XRenderPictureAttributes pict_attr;
	
	/* set picture attributes for anti-aliased edges; default is sharp edges */
	pict_attr.poly_edge=PolyEdgeSmooth;//
	pict_attr.poly_mode=PolyModeImprecise;
	Picture picture=XRenderCreatePicture(display, window, fmt, CPPolyEdge|CPPolyMode, &pict_attr);
	
	XSelectInput(display, window, KeyPressMask|KeyReleaseMask|ExposureMask
		|ButtonPressMask|StructureNotifyMask);
	

	sourcePicture = create_pen(0,0,0xffff,0xdfff, 1, 1, 32, NULL);
	textSource = create_pen(0,0,0,0xffff, 1, 1, 24, NULL);
        sourceImage = create_pen(0xffff, 0xffff,0,0xffff, 510, 510, 24, NULL); //&smiley512RGB[0]);
        texture_small = create_pen(0xffff, 0xffff,0, 0x4fff, 10, 10, 32, NULL);
        texture_large = create_pen(0xffff, 0xffff,0, 0x4fff, 256, 256, 32, NULL);		
        gradCache = create_pen(0, 0, 0, 0, 512, 512, 32, NULL);

	/* now make the window visible */
	XMapWindow(display, window);
	fmtA8 = XRenderFindStandardFormat(display, PictStandardA8);

        Pixmap pmf = XCreatePixmap(display, window, 768, 768, 24);
	XRenderPictureAttributes pict_attrf;
	pict_attrf.repeat=RepeatNone;
	backBuffer = XRenderCreatePicture(display, pmf, fmt, CPRepeat, &pict_attrf);

        setAlphaPict = create_pen(0xffff,0xffff,0,0xefff, 1, 1, 32, NULL);
        alphaMask = createMaskTile();

        createGradients();
        create12GlpyhSetARGB32();
        create12GlpyhSetA8();

        if(rt_screen) {
          backBuffer = picture;
          XRectangle rect;
          rect.x = 0;
          rect.y = 0;
          rect.width = 256;
          rect.height = 256;
          XRenderSetPictureClipRectangles (display, backBuffer, 0, 0, &rect, 1);
        }

	while(1) {
	   XNextEvent(display, &event);
		
		switch(event.type) {
			case Expose:
     {
				/* no partial repaints */
				XRenderFillRectangle(display, PictOpOver,
					picture, &color_white, 0, 0, 750, 750);

      XRenderFillRectangle (display, PictOpSrc, backBuffer, &color_white, 0, 0, 256, 256);

      int x;
      int m, i;
      for(i=0; i < cmdCnt; i++)
      {
          XRenderFillRectangle (display, PictOpSrc, backBuffer, &color_white, 0, 0, 256, 256);
          int size = sizes[i];
          char cmd = cmds[i];

          int reps = estimateRunCount(cmd, 256, size);
 
          int took = executeSeries(cmd, 256, size, reps, 5);
          double opsPerSec = reps / (((double) took) / 750);

          if(!rt_screen) {
            XRenderComposite(display, PictOpSrc, backBuffer, None, picture, 0, 0, 0, 0, 30, 30, 256, 256);
          }
          XFlush(display);

          if(num_only) {
            printf("%lf\n", opsPerSec);
          }else {
            printf("%lf Ops/s; %s; %dx%d\n", opsPerSec, names[cmd], size, size);
          }
      }
      exit(0);
    
      fflush(stdout);
    }
				break;
			case DestroyNotify:
				return 0;
		}
	}
	
	return 0;
}
