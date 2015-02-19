//---------------------------------------------------------------------------
#include <stdio.h>
#include "application.h"
#include "cursor.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
extern HDISPLAY XDpy;
//---------------------------------------------------------------------------
HCURSOR __fastcall CreateCursor(int shape)
{
    return XCreateFontCursor(XDpy, shape);
}
//---------------------------------------------------------------------------
HCURSOR __fastcall CreateCursor(TFont *src, TFont *mask, int src_chr, int mask_chr)
{
    HCURSOR c;
    XColor fg, bg;
    fg.pixel = clCursorForeground;
    bg.pixel = clCursorBackground;
    XQueryColor(XDpy, Screen.Colormap, &fg);
    XQueryColor(XDpy, Screen.Colormap, &bg);
    c = XCreateGlyphCursor(XDpy, src->Font->fid, mask->Font->fid,
                           src_chr, mask_chr, &fg, &bg);
    return c;
}
//---------------------------------------------------------------------------
HCURSOR __fastcall CreateCursor(TBitmap *src, int hot_x, int hot_y)
{
    HCURSOR c;

    TPixelFormat pf = src->PixelFormat;
    if(pf.Depth == pf1bit)
    {
        XColor fg, bg;
        fg.pixel = clCursorForeground;
        bg.pixel = clCursorBackground;
        XQueryColor(Application.Handle, Screen.Colormap, &fg);
        XQueryColor(Application.Handle, Screen.Colormap, &bg);
        c = XCreatePixmapCursor(XDpy, src->Handle, src->MaskHandle,
                                &fg, &bg, hot_x, hot_y);
    }
    else
    {
        XRenderPictureAttributes pa;
        unsigned long valuemask = CPClipMask;
        pa.clip_mask = src->MaskHandle;
        XRenderChangePicture (XDpy, src->Canvas->Picture, valuemask, &pa);
        c = XRenderCreateCursor(XDpy, src->Canvas->Picture, hot_x, hot_y);
    }
    return c;
}
//---------------------------------------------------------------------------
HCURSOR __fastcall CreateCursor(TCursorList &lst, int delay)
{
    HCURSOR c;
    int	ncursor = lst.Count;
    XAnimCursor	*cur;
    XAnimCursor	*cursors = new XAnimCursor[ncursor];
    for(register int i = 0; i < ncursor; i++)
    {
        cur = &cursors[i];
        cur->cursor = lst[i];
        cur->delay = delay;
    }
    c = XRenderCreateAnimCursor(XDpy, ncursor, cursors);
    delete [] cursors;
    return c;
}
//---------------------------------------------------------------------------
HCURSOR __fastcall CreateCursor(TImageList *lst, int hot_x, int hot_y, int delay)
{
    TCursorList lcur;
    TBitmap bmp;
    uint_t w, h;
    TPixelFormat pf = lst->PixelFormat;
    XQueryBestCursor(XDpy, Screen.Root, lst->ImageWidth, lst->ImageHeight, &w, &h);
    bmp.Create(w, h, lst->HandleType, &pf);
    TRect rect = TRect(0, 0, w, h);
    int	ncursor = lst->Count;
    for(register int i = 0; i < ncursor; i++)
    {
        lst->DrawRect(bmp.Canvas, rect, 0, 0, i, alClient);
        lcur[i] = CreateCursor(&bmp, hot_x, hot_y);
    }
    return CreateCursor(lcur, delay);
}
//---------------------------------------------------------------------------
void __fastcall DestroyCursor(HCURSOR hcur)
{
    XFreeCursor(XDpy, hcur);
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
