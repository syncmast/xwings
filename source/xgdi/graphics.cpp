//---------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include "graphics.h"
#include "rtti.h"
#include "application.h"
#include "vxutil.h"

#define RadToDeg(x) x * 180 / M_PI
#define DegToRad(x) x * M_PI / 180
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
HDISPLAY XDpy;
HWND RootWnd;
//---------------------------------------------------------------------------
TPoint __fastcall Point(int x, int y)
{
    TPoint ret;
    ret.x = x;
    ret.y = y;
    return ret;
}
//---------------------------------------------------------------------------
//  TRect
//---------------------------------------------------------------------------
bool __fastcall TRect :: inside(const TRect &rect)
{
    int l, t, r, b;
    l = rect.left;
    t = rect.top;
    r = rect.right();
    b = rect.bottom();
    if( inside(l, t) &&
            inside(r, t) &&
            inside(l, b) &&
            inside(r, b) )
        return true;
    return false;
}
//---------------------------------------------------------------------------
TPoint TRect :: center()
{
    TPoint ret;
    ret.x = width / 2 + left;
    ret.y = height / 2 + top;
    return ret;
}
//---------------------------------------------------------------------------
bool __fastcall TRect :: intersect(TRect &r2, TRect &rect)
{
    int l, t, r, b;
    rect.rx = rect.ry = 0;
    l = (r2.left > left)? r2.left : left;
    t = (r2.top > top)? r2.top : top;
    r = (r2.right() > right())? right() : r2.right();
    b = (r2.bottom() > bottom())? bottom() : r2.bottom();
    rect.bounds(l,t,r,b);
    bool result = !rect.isempty();
    if(!result) rect.erase();
    return result;
}
//---------------------------------------------------------------------------
bool __fastcall TRect :: unionrect(TRect &r2, TRect &rect)
{
    int l, t, r, b;
    if(!r2.isempty())
    {
        l = (r2.left <= left)? r2.left : left;
        t = (r2.top <= top)? r2.top : top;
        r = (r2.right() >= right())? r2.right() : right();
        b = (r2.bottom() >= bottom())? r2.bottom() : bottom();
    }
    rect.bounds(l,t,r,b);
    bool result = !rect.isempty();
    if(!result) rect.erase();
    return result;
}
//---------------------------------------------------------------------------
TRect __fastcall TRect :: align(const TRect &rect, TAlign value)
{
    TRect ret;
    ret.rx = rect.rx;
    ret.ry = rect.ry;
    switch (value)
    {
    case alNone:
        ret.left = rect.left;
        ret.top = rect.top;
        ret.width = rect.width;
        ret.height = rect.height;
        break;
    case alClient:
        ret.left = left;
        ret.top = top;
        ret.width = width;
        ret.height = height;
        break;
    case alLeft:
        ret.left = left;
        ret.top = top;
        ret.width = rect.width;
        ret.height = height;
        break;
    case alTop:
        ret.left = left;
        ret.top = top;
        ret.width = width;
        ret.height = rect.height;
        break;
    case alRight:
        ret.left = right() - rect.width + 1;
        ret.top = top;
        ret.width = rect.width;
        ret.height = height;
        break;
    case alBottom:
        ret.left = left;
        ret.top = bottom() - rect.height + 1;
        ret.width = width;
        ret.height = rect.height;
        break;
    case alLeftTop:
        ret.left = left;
        ret.top = top;
        ret.width = rect.width;
        ret.height = rect.height;
        break;
    case alCenterTop:
        ret.left = left + (width - rect.width) / 2;
        ret.top = top;
        ret.width = rect.width;
        ret.height = rect.height;
        break;
    case alRightTop:
        ret.left = right() - rect.width + 1;
        ret.top = top;
        ret.width = rect.width;
        ret.height = rect.height;
        break;
    case alLeftCenter:
        ret.left = left;
        ret.top = top + (height - rect.height) / 2;
        ret.width = rect.width;
        ret.height = rect.height;
        break;
    case alCenter:
        ret.left = left + (width - rect.width) / 2;
        ret.top = top + (height - rect.height) / 2;
        ret.width = rect.width;
        ret.height = rect.height;
        break;
    case alRightCenter:
        ret.left = right() - rect.width + 1;
        ret.top = top + (height - rect.height) / 2;
        ret.width = rect.width;
        ret.height = rect.height;
        break;
    case alLeftBottom:
        ret.left = left;
        ret.top = bottom() - rect.height + 1;
        ret.width = rect.width;
        ret.height = rect.height;
        break;
    case alCenterBottom:
        ret.left = left + (width - rect.width) / 2;
        ret.top = bottom() - rect.height + 1;
        ret.width = rect.width;
        ret.height = rect.height;
        break;
    case alRightBottom:
        ret.left = right() - rect.width + 1;
        ret.top = bottom() - rect.height + 1;
        ret.width = rect.width;
        ret.height = rect.height;
        break;
    case alVCenter:
        ret.left = left;
        ret.top = top + (height - rect.height) / 2;
        ret.width = width;
        ret.height = rect.height;
        break;
    case alHCenter:
        ret.left = left + (width - rect.width) / 2;
        ret.top = top;
        ret.width = rect.width;
        ret.height = height;
        break;
    case alWidth:
        ret.left = 0;
        ret.top = rect.top;
        ret.width = width;
        ret.height = rect.height;
        break;
    case alHeight:
        ret.left = rect.left;
        ret.top = 0;
        ret.width = rect.width;
        ret.height = height;
        break;
    }
    return ret;
}
//---------------------------------------------------------------------------
//  TEllipse
//---------------------------------------------------------------------------
bool __fastcall TEllipse :: inside(int x, int y)
{
    int x1, y1, c1;
    x1 = b * (x - cx);
    y1 = a * (y - cy);
    c1 = a * b * a * b;
    if( (x1 * x1 + y1 * y1) <= c1 )
        return true;
    return false;
}
//---------------------------------------------------------------------------
//  Region
//---------------------------------------------------------------------------
TRegion :: TRegion()
{
    m_Handle = XCreateRegion();
}
//---------------------------------------------------------------------------
__fastcall TRegion :: TRegion(const TPoint *points, int n)
{
    m_Handle = XPolygonRegion((XPoint *)points, n, EvenOddRule);
}
//---------------------------------------------------------------------------
__fastcall TRegion :: TRegion(const TRect &r)
{
    TPoint points[4];
    points[0].x = r.left;
    points[0].y = r.top;
    points[1].x = r.right() + 1;
    points[1].y = r.top;
    points[2].x = r.right() + 1;
    points[2].y = r.bottom() + 1;
    points[3].x = r.left;
    points[3].y = r.bottom() + 1;
    m_Handle = XPolygonRegion((XPoint *)points, 4, EvenOddRule);
}
//---------------------------------------------------------------------------
__fastcall TRegion :: TRegion(const TRegion &r)
{
    m_Handle = XCreateRegion();
    XUnionRegion(m_Handle, r.Handle, m_Handle);
}
//---------------------------------------------------------------------------
TRegion :: ~TRegion()
{
    XDestroyRegion(m_Handle);
}
//---------------------------------------------------------------------------
TRegion __fastcall const & TRegion :: operator=(const TRegion &r)
{
    Clear();
    XUnionRegion(m_Handle, r.Handle, m_Handle);
    return *this;
}
//---------------------------------------------------------------------------
TRect TRegion :: get_ClipRect()
{
    XRectangle rect_return;
    XClipBox(m_Handle, &rect_return);
    TRect rect = TRect(rect_return);
    return rect;
}
//---------------------------------------------------------------------------
void TRegion :: set_ClipRect(TRect rect)
{
    if(IsEmpty())
    {
        *this += rect;
        return;
    }

    TRect r = get_ClipRect();
    if(r != rect)
    {
        int dx, dy, dw, dh;
        dx = rect.left - r.left;
        dy = rect.top - r.top;
        dw = rect.width - r.width;
        dh = rect.height - r.height;
        Offset(dx, dy);
        Expand(dw, dh);
    }
}
//---------------------------------------------------------------------------
void TRegion :: Clear()
{
    XDestroyRegion(m_Handle);
    m_Handle = XCreateRegion();
}
//---------------------------------------------------------------------------
bool TRegion :: IsEmpty()
{
    return XEmptyRegion(m_Handle);
}
//---------------------------------------------------------------------------
bool __fastcall TRegion :: InRegion(int x, int y)
{
    return XPointInRegion(m_Handle, x, y);
}
//---------------------------------------------------------------------------
TRegionLocate __fastcall TRegion :: InRegion(const TRect &r)
{
    return (TRegionLocate)XRectInRegion(m_Handle, r.left, r.top, r.width, r.height);
}
//---------------------------------------------------------------------------
void __fastcall TRegion :: Expand(int dx, int dy)
{
    TRect r = get_ClipRect();
    int x = dx >> 1, y = dy >> 1;
    XShrinkRegion(m_Handle, -x, -y);
    if(IsEmpty())
    {
        x += r.left;
        y += r.top;
    }
    XOffsetRegion(m_Handle, x, y);
}
//---------------------------------------------------------------------------
void __fastcall TRegion :: Offset(int dx, int dy)
{
    XOffsetRegion(m_Handle, dx, dy);
}
//---------------------------------------------------------------------------
TRegion __fastcall TRegion :: Intersect(const TRegion &r)
{
    TRegion result;
    XIntersectRegion(m_Handle, r.Handle, result.Handle);
    return result;
}
//---------------------------------------------------------------------------
TRegion __fastcall TRegion :: Xor(const TRegion &r)
{
    TRegion result;
    XXorRegion(m_Handle, r.Handle, result.Handle);
    return result;
}
//---------------------------------------------------------------------------
TRegion __fastcall TRegion :: Union(const TRegion &r)
{
    TRegion result;
    XUnionRegion(m_Handle, r.Handle, result.Handle);
    return result;
}
//---------------------------------------------------------------------------
TRegion __fastcall TRegion :: Subtract(const TRegion &r)
{
    TRegion result;
    XSubtractRegion(m_Handle, r.Handle, result.Handle);
    return result;
}
//---------------------------------------------------------------------------
bool __fastcall TRegion :: operator==(const TRegion &r)
{
    return XEqualRegion(m_Handle, r.Handle);
}
//---------------------------------------------------------------------------
void __fastcall TRegion :: operator+=(const TRegion &r)
{
    XUnionRegion(m_Handle, r.Handle, m_Handle);
}
//---------------------------------------------------------------------------
TRegion __fastcall TRegion :: operator+(const TRect &r)
{
    TRegion result;
    XRectangle rectangle = const_cast<TRect &>(r).xrect();

//	rectangle.x = r.left;
//	rectangle.y = r.top;
//	rectangle.width = r.width;
//	rectangle.height = r.height;

    XUnionRectWithRegion(&rectangle, m_Handle, result.Handle);
    return result;
}
//---------------------------------------------------------------------------
void __fastcall TRegion :: operator+=(const TRect &r)
{
    XRectangle rectangle = const_cast<TRect &>(r).xrect();

//	rectangle.x = r.left;
//	rectangle.y = r.top;
//	rectangle.width = r.width;
//	rectangle.height = r.height;

    XUnionRectWithRegion(&rectangle, m_Handle, m_Handle);
}
//---------------------------------------------------------------------------
void __fastcall TRegion :: operator-=(const TRegion &r)
{
    XSubtractRegion(m_Handle, r.Handle, m_Handle);
}
//---------------------------------------------------------------------------
void __fastcall TRegion :: operator -=(const TRect &r)
{
    XRectangle rectangle = const_cast<TRect &>(r).xrect();;

//	rectangle.x = r.left;
//	rectangle.y = r.top;
//	rectangle.width = r.width;
//	rectangle.height = r.height;

    Region tmp = XCreateRegion();
    XUnionRectWithRegion(&rectangle, tmp, tmp);
    XSubtractRegion(m_Handle, tmp, m_Handle);
    XDestroyRegion(tmp);
}
//---------------------------------------------------------------------------
static void __fastcall __convert_region(const Region reg, const XserverRegion xreg)
{
    int nrects;
    XRectangle *rects = XFixesFetchRegion(XDpy, xreg, &nrects);
    for(register int i = 0; i < nrects; i++)
    {
        XUnionRectWithRegion(&rects[i], reg, reg);
    }
    XFree(rects);
    XFixesDestroyRegion(XDpy, xreg);
}
//---------------------------------------------------------------------------
void __fastcall SetRegion(const TRegion &reg, const TBitmap &bmp)
{
    XserverRegion xreg = XFixesCreateRegionFromBitmap(XDpy, bmp.Handle);
    __convert_region(reg.Handle, xreg);
}
//---------------------------------------------------------------------------
void __fastcall SetRegion(const TRegion &reg, const TCanvas &can)
{
    XserverRegion xreg = XFixesCreateRegionFromGC(XDpy, can.Pen->Handle);
    __convert_region(reg.Handle, xreg);
}
//---------------------------------------------------------------------------
void __fastcall SetRegion(const TRegion &reg, const TWinControl &win, int kind)
{
    XserverRegion xreg = XFixesCreateRegionFromWindow(XDpy, win.Handle, kind);
    __convert_region(reg.Handle, xreg);
}
//---------------------------------------------------------------------------
//  TGraphicsObject
//---------------------------------------------------------------------------
TGraphicsObject :: TGraphicsObject()
{
    CreateHandle(NULL);
}
//---------------------------------------------------------------------------
TGraphicsObject :: TGraphicsObject(const TCanvas *owner)
{
    CreateHandle(owner);
}
//---------------------------------------------------------------------------
TGraphicsObject :: ~TGraphicsObject()
{
    if(m_Handle)
        XFreeGC(XDpy, m_Handle);
}
//---------------------------------------------------------------------------
__fastcall TGraphicsObject :: TGraphicsObject(const TGraphicsObject &a) : TObject(a)
{
    CreateHandle(a.m_Canvas);
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TGraphicsObject & TGraphicsObject :: operator=(const TGraphicsObject &a)
{
    if(this == &a)
        return *this;
    TObject :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
/*
void TGraphicsObject :: Init()
{
    m_Handle = NULL;
    m_Canvas = NULL;
}
*/
//---------------------------------------------------------------------------
void __fastcall TGraphicsObject :: Assign(const TGraphicsObject &obj)
{
    XCopyGC(XDpy, obj.m_Handle, 0xFFFFFFFF, m_Handle);
}
//---------------------------------------------------------------------------
void __fastcall TGraphicsObject :: CreateHandle(const TCanvas *owner)
{
    m_Canvas = owner;
    XGCValues values;
    unsigned long valuemask = GCFillRule | GCGraphicsExposures;
    values.fill_rule = WindingRule;
    values.graphics_exposures = false;
    m_Handle = XCreateGC( XDpy, owner ? owner->Handle : (Screen.Handle)? *Screen.Root : DefaultRootWindow(XDpy),
                          valuemask, &values );
}
//---------------------------------------------------------------------------
TColor TGraphicsObject :: get_Color()
{
    XGCValues values;
    unsigned long valuemask = GCForeground;
    XGetGCValues(XDpy, m_Handle, valuemask, &values);
    return values.foreground;
}
//---------------------------------------------------------------------------
void TGraphicsObject :: set_Color(TColor value)
{
    TColor ctmp = get_Color();
    if(ctmp != value)
    {
        if(value == clNone)
            XSetFunction(XDpy, m_Handle, GXnoop);
        else if(ctmp == clNone)
            XSetFunction(XDpy, m_Handle, GXcopy);
        XSetForeground(XDpy, m_Handle, value);
//        Changed();
    }
}
//---------------------------------------------------------------------------
TSubwindowMode TGraphicsObject :: get_SubwindowMode()
{
    XGCValues values;
    unsigned long valuemask = GCSubwindowMode;
    XGetGCValues(XDpy, m_Handle, valuemask, &values);
    return TSubwindowMode(values.subwindow_mode);
}
//---------------------------------------------------------------------------
void TGraphicsObject :: set_SubwindowMode(TSubwindowMode value)
{
    XSetSubwindowMode(XDpy, m_Handle, (int)value);
}
//---------------------------------------------------------------------------
/*
void TGraphicsObject :: Changed()
{
    if(OnChange)
        OnChange(this);
}
*/
//---------------------------------------------------------------------------
void TGraphicsObject :: Apply()
{
    XFlushGC(XDpy, m_Handle);
//    Changed();
}
//---------------------------------------------------------------------------
//  TPen
//---------------------------------------------------------------------------
TPen :: TPen()
{
    Init();
}
//---------------------------------------------------------------------------
TPen :: ~TPen()
{
}
//---------------------------------------------------------------------------
__fastcall TPen :: TPen(const TCanvas *owner) : TGraphicsObject(owner)
{
    Init();
}
//---------------------------------------------------------------------------
__fastcall TPen :: TPen(const TPen &a) : TGraphicsObject(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TPen & TPen :: operator=(const TPen &a)
{
    if(this == &a)
        return *this;
    TGraphicsObject :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TPen :: Init()
{
    m_Style = psSolid;
}
//---------------------------------------------------------------------------
void __fastcall TPen :: Assign(const TPen &obj)
{
    m_Style = obj.m_Style;
}
//---------------------------------------------------------------------------
TPenMode TPen :: get_Mode()
{
    XGCValues values;
    unsigned long valuemask = GCFunction;
    XGetGCValues(XDpy, m_Handle, valuemask, &values);
    return TPenMode(values.function);
}
//---------------------------------------------------------------------------
void TPen :: set_Mode(TPenMode value)
{
    if( value == get_Mode() )
        return;
    XSetFunction(XDpy, m_Handle, value);
//    Changed();
}
//---------------------------------------------------------------------------
static void __fastcall __set_pen_style(HGC gc, TPenStyle value, int width)
{
    XGCValues values;
    char dash[6];
    int n = 0;
    int style = LineOnOffDash;
    if(width == 0)
        width = 1;
    switch (value)
    {
    case psDash:
        dash[0] = 6;
        dash[1] = 3;
        n = 2;
        break;
    case psDot:
        dash[0] = 1;
        dash[1] = 2;
        n = 2;
        break;
    case psDashDot:
        dash[0] = 6;
        dash[1] = 2;
        dash[2] = 2;
        dash[3] = 2;
        n = 4;
        break;
    case psDashDotDot:
        dash[0] = 6;
        dash[1] = 2;
        dash[2] = 1;
        dash[3] = 2;
        dash[4] = 1;
        dash[5] = 2;
        n = 6;
        break;
    default:
        style = LineSolid;
    }

    values.line_style = style;
    XChangeGC(XDpy, gc, GCLineStyle, &values);
    if(style == LineOnOffDash)
    {
        for(int i = 0; i < n; i++)
            dash[i] *= width;
        XSetDashes(XDpy, gc, 0, dash, n);
    }
}

void TPen :: set_Style(TPenStyle value)
{
    if(m_Style != value)
    {
        TPenStyle ps;
        if( value == psClear )
        {
            ps = psSolid;
            XSetFunction(XDpy, m_Handle, GXnoop);
        }
        else
        {
            ps = value;
            XSetFunction(XDpy, m_Handle, GXcopy);
        }
        __set_pen_style(m_Handle, ps, get_Width());
        m_Style = value;
//        Changed();
    }
}
//---------------------------------------------------------------------------
short TPen :: get_Width()
{
    XGCValues values;
    unsigned long valuemask = GCLineWidth;
    XGetGCValues(XDpy, m_Handle, valuemask, &values);
    return values.line_width;
}
//---------------------------------------------------------------------------
void TPen :: set_Width(short value)
{
    if( value == get_Width() )
        return;
    XGCValues values;
    values.line_width = value;
    XChangeGC(XDpy, m_Handle, GCLineWidth, &values);
    if(m_Style != psSolid)
        set_Style(m_Style);
//    else
//        Changed();
}
//---------------------------------------------------------------------------
TCapStyle TPen :: get_CapStyle()
{
    XGCValues values;
    unsigned long valuemask = GCCapStyle;
    XGetGCValues(XDpy, m_Handle, valuemask, &values);
    return TCapStyle(values.cap_style);
}
//---------------------------------------------------------------------------
void TPen :: set_CapStyle(TCapStyle value)
{
    if( value == get_CapStyle() )
        return;
    XGCValues values;
    values.cap_style = value;
    XChangeGC(XDpy, m_Handle, GCCapStyle, &values);
//    Changed();
}
//---------------------------------------------------------------------------
TJoinStyle TPen :: get_JoinStyle()
{
    XGCValues values;
    unsigned long valuemask = GCJoinStyle;
    XGetGCValues(XDpy, m_Handle, valuemask, &values);
    return TJoinStyle(values.join_style);
}
//---------------------------------------------------------------------------
void TPen :: set_JoinStyle(TJoinStyle value)
{
    if( value == get_JoinStyle() )
        return;
    XGCValues values;
    values.join_style = value;
    XChangeGC(XDpy, m_Handle, GCJoinStyle, &values);
//    Changed();
}
//---------------------------------------------------------------------------
//  TBrush
//---------------------------------------------------------------------------
TBrush :: TBrush()
{
    Init();
}
//---------------------------------------------------------------------------
TBrush :: ~TBrush()
{
}
//---------------------------------------------------------------------------
__fastcall TBrush :: TBrush(const TCanvas *owner) : TGraphicsObject(owner)
{
    Init();
}
//---------------------------------------------------------------------------
__fastcall TBrush :: TBrush(const TBrush &a) : TGraphicsObject(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TBrush & TBrush :: operator=(const TBrush &a)
{
    if(this == &a)
        return *this;
    TGraphicsObject :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TBrush :: Init()
{
    m_Bitmap = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TBrush :: Assign(const TBrush &obj)
{
}
//---------------------------------------------------------------------------
void TBrush :: set_Bitmap(TBitmap *value)
{
    if(m_Bitmap != value)
    {
        if(value)
        {
            if((*value->PixelFormat).Depth == pf1bit)
            {
                XSetStipple(XDpy, m_Handle, value->Handle);
            }
            else
            {
                XSetTile(XDpy, m_Handle, value->Handle);
            }
        }
        m_Bitmap = value;
//        Changed();
    }
}
//---------------------------------------------------------------------------
TBrushStyle TBrush :: get_Style()
{
    XGCValues values;
    TBrushStyle ret;
    unsigned long valuemask = GCFillStyle | GCFunction;
    XGetGCValues(XDpy, m_Handle, valuemask, &values);
    if( values.function == GXnoop )
        ret = bsClear;
    else
        ret = TBrushStyle(values.fill_style);
    return ret;
}
//---------------------------------------------------------------------------
void TBrush :: set_Style(TBrushStyle value)
{
    if( value == get_Style() )
        return;
    if( value == bsClear )
    {
        XSetFunction(XDpy, m_Handle, GXnoop);
        XSetFillStyle(XDpy, m_Handle, FillSolid);
    }
    else
    {
        XSetFunction(XDpy, m_Handle, GXcopy);
        XSetFillStyle(XDpy, m_Handle, value);
    }
//    Changed();
}
//---------------------------------------------------------------------------
void TBrush :: set_Color(TColor value)
{
    GC gc1, gc2;

    if(m_Canvas)
    {
        gc1 = m_Canvas->Font->Handle;
        gc2 = m_Canvas->Pen->Handle;
    }
    else
    {
        gc1 = Screen.Font.Handle;
        gc2 = Screen.Pen.Handle;
    }

    XSetBackground(XDpy, m_Handle, value);
    XCopyGC(XDpy, m_Handle, GCBackground, gc1);
    XCopyGC(XDpy, m_Handle, GCBackground, gc2);
    TGraphicsObject :: set_Color(value);
}
//---------------------------------------------------------------------------
//  TFont
//---------------------------------------------------------------------------
TFont :: TFont()
{
    Init();
}
//---------------------------------------------------------------------------
TFont :: ~TFont()
{
    if(m_system == m_Font)
    {
//        XUnloadFont(XDpy, m_Font->fid);
        XFreeFontInfo(NULL, m_Font, 0);
    }
    else
        XFreeFont(XDpy, m_Font);
}
//---------------------------------------------------------------------------
__fastcall TFont :: TFont(const TCanvas *owner) : TGraphicsObject(owner)
{
    Init();
}
//---------------------------------------------------------------------------
__fastcall TFont :: TFont(const TFont &a) : TGraphicsObject(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TFont & TFont :: operator=(const TFont &a)
{
    if(this == &a)
        return *this;
    TGraphicsObject :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TFont :: Init()
{
    m_Font = XQueryFont(XDpy, XGContextFromGC(m_Handle));
//	if(m_Font == NULL)
//	{
//	    m_Font = XQueryFont(XDpy, XGContextFromGC(CScreen.Font.Handle));
//	}
    m_system = m_Font;
    m_Style = 0;
    m_PenStyle = psSolid;
    FullName = "-monotype-arial-medium-r-normal--0-80-0-0-p-0-iso8859-5";
    printf("FONT 1 %s\n", (*FullName).c_str());
//	SetName();
}
//---------------------------------------------------------------------------
void __fastcall TFont :: Assign(const TFont &obj)
{
}
//---------------------------------------------------------------------------
void TFont :: set_PenStyle(TPenStyle value)
{
    if(m_PenStyle != value)
    {
        __set_pen_style(m_Handle, value, 0);
        m_PenStyle = value;
//        Changed();
    }
}
//---------------------------------------------------------------------------
static const char *__weightnormal[] =
{
    "medium",
    "book",
    "semilight",
    "light"
};
static const char *__weightbold[] =
{
    "bold",
    "demibold",
    "demi bold",
    "dark"
};

void TFont :: set_Style(int value)
{
    if(m_Style == value)
        return;
    HFONT tmp = m_Font;
    if((m_Style & fsBold) != (value & fsBold))
    {
        if(value & fsBold)
        {
            for(register uint_t i = 0; i < SIZEOFARRAY(__weightbold); i++)
            {
                set_Weight(__weightbold[i]);
                if(tmp != m_Font)
                    break;
            }
        }
        else
        {
            for(register uint_t i = 0; i < SIZEOFARRAY(__weightnormal); i++)
            {
                set_Weight(__weightnormal[i]);
                if(tmp != m_Font)
                    break;
            }
        }
        tmp = m_Font;
    }
    if((m_Style & fsItalic) != (value & fsItalic))
    {
        if(value & fsItalic)
        {
            set_Slant(fslItalic);
            if(tmp == m_Font)
                set_Slant(fslOblique);
        }
        else
        {
            set_Slant(fslRoman);
        }
    }
    m_Style = value;
}
//---------------------------------------------------------------------------
TString TFont :: get_Foundry()
{
    return GetProperty(FS_FOUNDRY);
}
//---------------------------------------------------------------------------
void TFont :: set_Foundry(TString value)
{
    TString str;
    str = '-' + value +
          "-*-" + GetProperty(FS_WEIGHT_NAME) +
          '-' + GetProperty(FS_SLANT) +
          "-*-*-" + str.printf("%lu", GetMetric(FM_PIXEL_SIZE)) +
          "-*-*-*-*-*-" + get_Charset();
    set_FullName(str);
}
//---------------------------------------------------------------------------
TString TFont :: get_FullName()
{
    return GetProperty(FS_FONT);
}
//---------------------------------------------------------------------------
void TFont :: set_FullName(TString value)
{
    HFONT fnt = XLoadQueryFont(XDpy, value.c_str());
    if(fnt != NULL)
    {
        if(m_system != m_Font)
            XFreeFont(XDpy, m_Font);
        m_Font = fnt;
        XSetFont(XDpy, m_Handle, m_Font->fid);
//        Changed();
    }
}
//---------------------------------------------------------------------------
TString TFont :: get_Family()
{
    return GetProperty(FS_FAMILY_NAME);
}
//---------------------------------------------------------------------------
void TFont :: set_Family(TString value)
{
    TString str;
    str = "-*-" + value +
          '-' + GetProperty(FS_WEIGHT_NAME) +
          '-' + GetProperty(FS_SLANT) +
          "-*-*-" + str.printf("%lu", GetMetric(FM_PIXEL_SIZE)) +
          "-*-*-*-*-*-" + get_Charset();
    set_FullName(str);
}
//---------------------------------------------------------------------------
short TFont :: get_Resolution()
{
    return (short)GetMetric(FM_RESOLUTION_X);
}
//---------------------------------------------------------------------------
short TFont :: get_Points()
{
    return (short)GetMetric(FM_POINT_SIZE);
}
//---------------------------------------------------------------------------
void TFont :: set_Points(short value)
{
    TString str;
    TString s[2];
    str = '-' + GetProperty(FS_FOUNDRY) +
          '-' + GetProperty(FS_FAMILY_NAME) +
          '-' + GetProperty(FS_WEIGHT_NAME) +
          '-' + GetProperty(FS_SLANT) +
          '-' + GetProperty(FS_SETWIDTH_NAME) +
          '-' + '*' +
          '-' + '*' +
          '-' + str.printf("%d", value) +
          '-' + s[0].printf("%lu", GetMetric(FM_RESOLUTION_X)) +
          '-' + s[1].printf("%lu", GetMetric(FM_RESOLUTION_Y)) +
          '-' + GetProperty(FS_SPACING) +
          '-' + '*' + '-' + get_Charset();
    set_FullName(str);
}
//---------------------------------------------------------------------------
short TFont :: get_Pixels()
{
    return (short)GetMetric(FM_PIXEL_SIZE);
}
//---------------------------------------------------------------------------
void TFont :: set_Pixels(short value)
{
    TString str;
    TString s[2];
    str = '-' + GetProperty(FS_FOUNDRY) +
          '-' + GetProperty(FS_FAMILY_NAME) +
          '-' + GetProperty(FS_WEIGHT_NAME) +
          '-' + GetProperty(FS_SLANT) +
          '-' + GetProperty(FS_SETWIDTH_NAME) +
          '-' + '*' +
          '-' + str.printf("%d", value) +
          '-' + '*' +
          '-' + s[0].printf("%lu", GetMetric(FM_RESOLUTION_X)) +
          '-' + s[1].printf("%lu", GetMetric(FM_RESOLUTION_Y)) +
          '-' + GetProperty(FS_SPACING) +
          '-' + '*' + '-' + get_Charset();
    set_FullName(str);
}
//---------------------------------------------------------------------------
TFontSpacing TFont :: get_Spacing()
{
    TFontSpacing ret;
    TString str = GetProperty(FS_SPACING);
    if(str[0] == 'm' || str[0] == 'M')
        ret = fsMonospaced;
    else if(str[0] == 'p' || str[0] == 'P')
        ret = fsProportional;
    else
        ret = fsCharCell;
    return ret;
}
//---------------------------------------------------------------------------
void TFont :: set_Spacing(TFontSpacing value)
{
    TString str;
    switch(value)
    {
    case fsMonospaced:
        str = 'm';
        break;
    case fsProportional:
        str = 'p';
        break;
    case fsCharCell:
    default:
        str = 'c';
    }
    TString s[2];
    str = "-*-*-" + GetProperty(FS_WEIGHT_NAME) +
          '-' + GetProperty(FS_SLANT) +
          '-' + GetProperty(FS_SETWIDTH_NAME) +
          '-' + '*' +
          '-' + s[0].printf("%lu", GetMetric(FM_PIXEL_SIZE)) +
          '-' + '*' +
          '-' + '*' +
          '-' + '*' +
          '-' + str +
          '-' + s[1].printf("%lu", GetMetric(FM_AVERAGE_WIDTH)) +
          '-' + get_Charset();
    set_FullName(str);
}
//---------------------------------------------------------------------------
TFontSlant TFont :: get_Slant()
{
    TFontSlant ret;
    TString str = GetProperty(FS_SLANT);
    if(str == "r" || str == "R")
        ret = fslRoman;
    else if(str == "o" || str == "O")
        ret = fslOblique;
    else if(str == "i" || str == "I")
        ret = fslItalic;
    else if(str == "ro" || str == "RO")
        ret = fslReverseOblique;
    else if(str == "ri" || str == "RO")
        ret = fslReverseItalic;
    else
        ret = fslOther;
    return ret;
}
//---------------------------------------------------------------------------
void TFont :: set_Slant(TFontSlant value)
{
    if(get_Slant() == value)
        return;
    TString str, s1;
    switch(value)
    {
    case fslRoman:
        str = "-r-";
        break;
    case fslOblique:
        str = "-o-";
        break;
    case fslItalic:
        str = "-i-";
        break;
    case fslReverseOblique:
        str = "-ro-";
        break;
    case fslReverseItalic:
        str = "-ri-";
        break;
    case fslOther:
    default:
        str = "-ot-";
    }
    str = '-' + GetProperty(FS_FOUNDRY) +
          '-' + GetProperty(FS_FAMILY_NAME) +
          '-' + GetProperty(FS_WEIGHT_NAME) + str + GetProperty(FS_SETWIDTH_NAME) +
          "-*-" + s1.printf("%lu", GetMetric(FM_PIXEL_SIZE)) +
          "-*-*-*-" + GetProperty(FS_SPACING) +
          "-*-" + get_Charset();

    set_FullName(str);
}
//---------------------------------------------------------------------------
TString TFont :: get_Charset()
{
    return GetProperty(FS_CHARSET_REGISTRY) + '-' +
           GetProperty(FS_CHARSET_ENCODING);
}
//---------------------------------------------------------------------------
void TFont :: set_Charset(TString value)
{
    TString str;
    str = '-' + GetProperty(FS_FOUNDRY) +
          '-' + GetProperty(FS_FAMILY_NAME) +
          '-' + GetProperty(FS_WEIGHT_NAME) +
          '-' + GetProperty(FS_SLANT) +
          '-' + GetProperty(FS_SETWIDTH_NAME) +
          '-' + '*' +
          '-' + str.printf("%lu", GetMetric(FM_PIXEL_SIZE)) +
          '-' + '*' +
          '-' + '*' +
          '-' + '*' +
          '-' + GetProperty(FS_SPACING) +
          '-' + '*' +
          '-' + value;
    set_FullName(str);
}
//---------------------------------------------------------------------------
TString TFont :: get_Weight()
{
    return GetProperty(FS_WEIGHT_NAME);
}
//---------------------------------------------------------------------------
void TFont :: set_Weight(TString value)
{
    TString str;
    str = '-' + GetProperty(FS_FOUNDRY) +
          '-' + GetProperty(FS_FAMILY_NAME) +
          '-' + value +
          '-' + GetProperty(FS_SLANT) +
          '-' + GetProperty(FS_SETWIDTH_NAME) +
          "-*-" + str.printf("%lu", GetMetric(FM_PIXEL_SIZE)) +
          "-*-*-*-*-*-" + get_Charset();
    set_FullName(str);
}
//---------------------------------------------------------------------------
TString TFont :: get_Setwidth()
{
    return GetProperty(FS_SETWIDTH_NAME);
}
//---------------------------------------------------------------------------
void TFont :: set_Setwidth(TString value)
{
    TString str;
    str = "-*-*-" + GetProperty(FS_WEIGHT_NAME) +
          '-' + GetProperty(FS_SLANT) +
          '-' + value +
          '-' + '*' +
          '-' + str.printf("%lu", GetMetric(FM_PIXEL_SIZE)) +
          '-' + '*' +
          '-' + '*' +
          '-' + '*' +
          '-' + GetProperty(FS_SPACING) +
          '-' + '*' + '-' + get_Charset();
    set_FullName(str);
}
//---------------------------------------------------------------------------
TString __fastcall TFont :: GetProperty(const char *name)
{
    TString ret;
    char *str = NULL;
    unsigned long value = 0;
    Atom atom;

    atom = XInternAtom(XDpy, name, false);
    if(XGetFontProperty(m_Font, atom, &value))
    {
        str = XGetAtomName(XDpy, value);
        ret = str;
        XFree(str);
    }
    else
        ret = "";
    return ret;
}
//---------------------------------------------------------------------------
ulong_t __fastcall TFont :: GetMetric(const char *name)
{
    unsigned long value = 0, v;
    Atom atom;

    atom = XInternAtom(XDpy, name, true);
    XGetFontProperty(m_Font, atom, &value);
    if(value != 0)
        return value;
    if(!strcmp(name, FM_MIN_SPACE))
    {
        value = GetMetric(FM_NORM_SPACE);
        value = (75 * value) / 100;
    }
    else if(!strcmp(name, FM_NORM_SPACE))
    {
        value = (33 * GetMetric(FM_RESOLUTION_X) * GetMetric(FM_POINT_SIZE)) / 72270;
    }
    else if(!strcmp(name, FM_MAX_SPACE))
    {
        value = (15 * GetMetric(FM_NORM_SPACE)) / 10;
    }
    else if(!strcmp(name, FM_END_SPACE))
    {
        value = GetMetric(FM_NORM_SPACE);
    }
    else if(!strcmp(name, FM_AVG_CAPITAL_WIDTH))
    {
        value = GetMetric(FM_AVERAGE_WIDTH);
    }
    else if(!strcmp(name, FM_AVG_LOWERCASE_WIDTH))
    {
        value = GetMetric(FM_AVERAGE_WIDTH);
    }
    else if(!strcmp(name, FM_FIGURE_WIDTH))
    {
        value = GetMetric(FM_AVERAGE_WIDTH);
    }
    else if(!strcmp(name, FM_SUPERSCRIPT_X))
    {
        value = (4 * GetMetric(FM_CAP_HEIGHT)) / 10;
    }
    else if(!strcmp(name, FM_SUPERSCRIPT_Y))
    {
        value = (4 * GetMetric(FM_CAP_HEIGHT)) / 10;
    }
    else if(!strcmp(name, FM_SUBSCRIPT_X))
    {
        value = (4 * GetMetric(FM_CAP_HEIGHT)) / 10;
    }
    else if(!strcmp(name, FM_SUBSCRIPT_Y))
    {
        value = (4 * GetMetric(FM_CAP_HEIGHT)) / 10;
    }
    else if(!strcmp(name, FM_SUPERSCRIPT_SIZE))
    {
        value = (6 * GetMetric(FM_PIXEL_SIZE)) / 10;
    }
    else if(!strcmp(name, FM_SUBSCRIPT_SIZE))
    {
        value = (6 * GetMetric(FM_PIXEL_SIZE)) / 10;
    }
    else if(!strcmp(name, FM_SMALL_CAP_SIZE))
    {
        value = GetMetric(FM_X_HEIGHT);
        v = GetMetric(FM_CAP_HEIGHT);
        value = round(GetMetric(FM_PIXEL_SIZE) * ((value + (double(v - value) / 3.)) / v));
    }
    else if(!strcmp(name, FM_UNDERLINE_POSITION))
    {
        value = m_Font->descent / 2;
    }
    else if(!strcmp(name, FM_UNDERLINE_THICKNESS))
    {
        value =  1;
    }
    else if(!strcmp(name, FM_STRIKEOUT_ASCENT))
    {
        value = m_Font->ascent;
    }
    else if(!strcmp(name, FM_STRIKEOUT_DESCENT))
    {
        value = m_Font->descent;
    }
    else if(!strcmp(name, FM_ITALIC_ANGLE))
    {
        value = 90 * 64;
    }
    else if(!strcmp(name, FM_CAP_HEIGHT))
    {
        int dummy;
        XCharStruct overall;
        XTextExtents(m_Font, "X", 1, &dummy, &dummy, &dummy, &overall);
        value = overall.ascent;
    }
    else if(!strcmp(name, FM_X_HEIGHT))
    {
        int dummy;
        XCharStruct overall;
        XTextExtents(m_Font, "x", 1, &dummy, &dummy, &dummy, &overall);
        value = overall.ascent;
    }
    else if(!strcmp(name, FM_RELATIVE_SETWIDTH))
    {
        value = 50;
    }
    else if(!strcmp(name, FM_RELATIVE_WEIGHT))
    {
        value = 50;
    }
    else if(!strcmp(name, FM_WEIGHT))
    {
        value = (1 * 1000) / GetMetric(FM_CAP_HEIGHT);
    }
    return value;
}
//---------------------------------------------------------------------------
TString __fastcall TFont :: ConvertCharset(const TString &text)
{
    TString tmp = get_Charset();
    const char *cs = tmp.c_str();
    const char *acs = Application.Charset;
    if(!strcasecmp(acs, cs))
        return text;
    char *s = (char *)text.c_str();
    int n = text.size();
    int len = convert_charset(acs, cs, s, n, NULL);
    char *os = (char *)malloc(len + 1);
    convert_charset(acs, cs, s, n, os);
    TString ret = os;
    free(os);
    return ret;
}
//---------------------------------------------------------------------------
void TFont :: Reset()
{
    if(m_Font != m_system)
    {
        XFreeFont(XDpy, m_Font);
        m_Font = m_system;
        XSetFont(XDpy, m_Handle, m_Font->fid);
    }
}
//---------------------------------------------------------------------------
/*
void TFont :: Changed()
{
    /*   TMessage msg;
       msg.Msg = CM_FONTCHANGED;
       msg.Params[0] = (long)this;
       SendMessage(NULL, msg);
    TGraphicsObject :: Changed();
}
*/
//---------------------------------------------------------------------------
void __fastcall TFont :: GetInfo(TFontInfo *info)
{
    info->Foundry = GetProperty(FS_FOUNDRY);
    info->FamilyName = GetProperty(FS_FAMILY_NAME);
    info->WeightName = GetProperty(FS_WEIGHT_NAME);
    info->Slant = GetProperty(FS_SLANT);
    info->SetWidthName = GetProperty(FS_SETWIDTH_NAME);
    info->AddStyleName = GetProperty(FS_ADD_STYLE_NAME);
    info->Spacing = GetProperty(FS_SPACING);
    info->CharsetRegistry = GetProperty(FS_CHARSET_REGISTRY);
    info->CharsetEncoding = GetProperty(FS_CHARSET_ENCODING);
    info->Font = GetProperty(FS_FONT);
    info->FaceName = GetProperty(FS_FACE_NAME);
    info->Copyright = GetProperty(FS_COPYRIGHT);
    info->Notice = GetProperty(FS_NOTICE);
    info->FontType = GetProperty(FS_FONT_TYPE);
    info->FontVersion = GetProperty(FS_FONT_VERSION);
    info->RasterizerName = GetProperty(FS_RASTERIZER_NAME);
    info->RasterizerVersion = GetProperty(FS_RASTERIZER_VERSION);
}
//---------------------------------------------------------------------------
void TFont :: SetName()
{
    TString s;
    TFontInfo fi;

//	FullName = "-monotype-arial-medium-r-normal--0-80-0-0-p-0-iso8859-5";
//	FullName = "-Misc-Fixed-Medium-R-SemiCondensed--13-120-75-75-C-60-ISO8859-5";
//	FullName = "-microsoft-verdana-medium-r-normal--0-90-100-100-p-0-iso8859-5";
//	Foundry = "monotype";
//	Family = "arial";
//	Charset = "iso8859-5";
//	Weight = "bold";

    GetInfo(&fi);

    printf("FONT 2 %s\n", (*FullName).c_str());
    printf("COPYRIGHT %s\n", fi.Copyright.c_str());
    printf("CHARSET %s\n", (*Charset).c_str());
    printf("FOUNDRY %s\n", (*Foundry).c_str());
    printf("FAMILY %s\n", (*Family).c_str());
    printf("FACE_NAME [%s]\n", fi.FaceName.c_str());
    printf("FONT_TYPE [%s]\n", fi.FontType.c_str());
    printf("POINT_SIZE %d\n", *Points);
    printf("PIXEL_SIZE %d\n", *Pixels);
    printf("SLANT %d\n", *Slant);
    printf("RESOLUTION_X %d\n", *Resolution);
    printf("SPACING %d\n",*Spacing);
    printf("WEIGHT_NAME %s\n", (*Weight).c_str());
    printf("SETWIDTH_NAME %s\n", (*Setwidth).c_str());
    printf("ADD_STYLE_NAME [%s]\n", fi.AddStyleName.c_str());
    printf("NOTICE [%s]\n", fi.Notice.c_str());
    printf("FONT_VERSION [%s]\n", fi.FontVersion.c_str());
    printf("RASTERIZER_NAME [%s]\n", fi.RasterizerName.c_str());
    printf("RASTERIZER_VERSION [%s]\n", fi.RasterizerVersion.c_str());
    printf("============================================\n");

    printf("AVERAGE_WIDTH %lu \n",GetMetric(FM_AVERAGE_WIDTH));
    printf("X_HEIGHT %lu\n", GetMetric(FM_X_HEIGHT));
    printf("MIN_SPACE %lu\n", GetMetric(FM_MIN_SPACE));
    printf("NORM_SPACE %lu\n", GetMetric(FM_NORM_SPACE));
    printf("MAX_SPACE %lu\n", GetMetric(FM_MAX_SPACE));
    printf("END_SPACE %lu\n", GetMetric(FM_END_SPACE));
    printf("SUPERSCRIPT_X %lu\n", GetMetric(FM_SUPERSCRIPT_X));
    printf("SUPERSCRIPT_Y %lu\n", GetMetric(FM_SUPERSCRIPT_Y));
    printf("SUBSCRIPT_X %lu\n", GetMetric(FM_SUBSCRIPT_X));
    printf("SUBSCRIPT_Y %lu\n", GetMetric(FM_SUBSCRIPT_Y));
    printf("UNDERLINE_POSITION %lu\n", GetMetric(FM_UNDERLINE_POSITION));
    printf("UNDERLINE_THICKNESS %lu\n", GetMetric(FM_UNDERLINE_THICKNESS));
    printf("STRIKEOUT_ASCENT %lu\n", GetMetric(FM_STRIKEOUT_ASCENT));
    printf("STRIKEOUT_DESCENT %lu\n", GetMetric(FM_STRIKEOUT_DESCENT));
    printf("ITALIC_ANGLE %lu\n", GetMetric(FM_ITALIC_ANGLE));
    printf("CAP_HEIGHT %lu\n", GetMetric(FM_CAP_HEIGHT));
    printf("WEIGHT %lu\n", GetMetric(FM_WEIGHT));
    printf("RELATIVE_WEIGHT %lu\n", GetMetric(FM_RELATIVE_WEIGHT));
    printf("RELATIVE_SETWIDTH %lu\n", GetMetric(FM_RELATIVE_SETWIDTH));
    printf("AVG_CAPITAL_WIDTH %lu \n",GetMetric(FM_AVG_CAPITAL_WIDTH));
    printf("AVG_LOWERCASE_WIDTH %lu \n",GetMetric(FM_AVG_LOWERCASE_WIDTH));
    printf("FIGURE_WIDTH %lu \n",GetMetric(FM_FIGURE_WIDTH));
    printf("SUPERSCRIPT_SIZE %lu \n",GetMetric(FM_SUPERSCRIPT_SIZE));
    printf("SUBSCRIPT_SIZE %lu \n",GetMetric(FM_SUBSCRIPT_SIZE));
    printf("SMALL_CAP_SIZE %lu \n",GetMetric(FM_SMALL_CAP_SIZE));
    printf("DESTINATION %lu \n",GetMetric(FM_DESTINATION));
    printf("RAW_ASCENT %lu \n",GetMetric(FM_RAW_ASCENT));
    printf("RAW_DESCENT %lu \n",GetMetric(FM_RAW_DESCENT));
    printf("============================================\n");

//  53. RAW_*
//  54. AXIS_NAMES
//  55. AXIS_LIMITS
//  56. AXIS_TYPES

}
//---------------------------------------------------------------------------
// TCanvas
//---------------------------------------------------------------------------
DYNAMIC_CLASS( TCanvas );
//---------------------------------------------------------------------------
TCanvas :: TCanvas()
{
    m_Handle = Screen.Root;
//   	m_owner = NULL;
    Init();
}
//---------------------------------------------------------------------------
TCanvas :: TCanvas(HDC owner)
{
    m_Handle = owner;
//   	m_owner = NULL;
    Init();
}
//---------------------------------------------------------------------------
TCanvas :: TCanvas(TWinControl *owner)
{
    m_Handle = owner->Handle;
//   	m_owner = owner;
    Init();
}
//---------------------------------------------------------------------------
TCanvas :: TCanvas(TBitmap *owner)
{
    m_Handle = owner->Handle;
//   	m_owner = owner;
    Init();
}
//---------------------------------------------------------------------------
TCanvas :: ~TCanvas()
{
//	delete Font;
//	delete Pen;
//	delete Brush;
//	Font = NULL;
//	Pen = NULL;
//	Brush = NULL;
    if(m_Picture)
        XRenderFreePicture(XDpy, m_Picture);		//  XRender
}
//---------------------------------------------------------------------------
__fastcall TCanvas :: TCanvas(const TCanvas &a) : TObject(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TCanvas & TCanvas :: operator=(const TCanvas &a)
{
    if(this == &a)
        return *this;
    TObject :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
__fastcall void TCanvas :: Assign(const TCanvas &obj)
{
}
//---------------------------------------------------------------------------
void TCanvas :: Init()
{
//	PenPos.x = PenPos.y = 0;
//    Font = new TFont(this);
//	Pen = new TPen(this);
//	Brush = new TBrush(this);
    Font = &Screen.Font;
    Pen = &Screen.Pen;
    Brush = &Screen.Brush;
    m_Picture = 0;

    /*
    	XRenderColor  color;
    	color.red = 65535;
        color.green = 65535;
        color.blue = 65535;
        color.alpha = 0;
    	XRenderFillRectangle (XDpy, PictOpSrc, m_Picture, &color, 0, 0, 24, 24); */
}
//---------------------------------------------------------------------------
TPixelSize TCanvas :: get_Depth()
{
    HWND root;
    int x, y;
    unsigned int width, height;
    unsigned int border_width;
    unsigned int depth_return;

    XGetGeometry(XDpy, m_Handle, &root, &x, &y, &width,
                 &height, &border_width, &depth_return);
    return (TPixelSize)depth_return;
}
//---------------------------------------------------------------------------
HPICTURE TCanvas :: get_Picture()
{
    if(m_Picture == None)
    {
        TPixelSize depth = get_Depth();				//  XRender
        int	format;
        XRenderPictFormat *pictformat;
        switch(depth)
        {
        case pf1bit:
            format = PictStandardA1;
            break;
        case pf4bit:
            format = PictStandardA4;
            break;
        case pf8bit:
            format = PictStandardA8;
            break;
        case pf16bit:
            format = PictStandardA8;
            break;
        case pf24bit:
            format = PictStandardRGB24;
            break;
        case pf32bit:
            format = PictStandardARGB32;
            break;
        default:
            format = PictStandardARGB32;
        }
        pictformat = XRenderFindStandardFormat(XDpy, format);
        m_Picture = XRenderCreatePicture(XDpy, m_Handle, pictformat, 0, NULL);
    }
    return m_Picture;
}
//---------------------------------------------------------------------------
void TCanvas :: set_ClipOrigin(TPoint value)
{
    m_ClipOrigin = value;

    XSetClipOrigin(XDpy, Pen->Handle, m_ClipOrigin.x + Origin.x, m_ClipOrigin.y + Origin.y);
//	XSetClipOrigin(XDpy, Pen->Handle, m_ClipOrigin.x, m_ClipOrigin.y);
    XCopyGC(XDpy, Pen->Handle, GCClipXOrigin | GCClipYOrigin, Brush->Handle);
    XCopyGC(XDpy, Pen->Handle, GCClipXOrigin | GCClipYOrigin, Font->Handle);
    /*
    	unsigned long	valuemask;
    	XRenderPictureAttributes pa;
    	valuemask = CPClipXOrigin | CPClipYOrigin;    //  XRender
    	pa.clip_x_origin = m_ClipOrigin.x + m_Origin.x;
        pa.clip_y_origin = m_ClipOrigin.y + m_Origin.y;
    	XRenderChangePicture(Application.Handle, m_Picture, valuemask, &pa); */
}
//---------------------------------------------------------------------------
void TCanvas :: set_ClipMask(TRegion aregion)
{
    m_ClipMask = aregion;
    aregion.Offset(Origin.x, Origin.y);
    XSetRegion(XDpy, Pen->Handle, aregion.Handle);
    XCopyGC(XDpy, Pen->Handle, GCClipMask, Brush->Handle);
    XCopyGC(XDpy, Pen->Handle, GCClipMask, Font->Handle);

//	XRenderSetPictureClipRegion(XDpy, m_Picture, aregion.Handle);   //  XRender
}
//---------------------------------------------------------------------------
TCopyMode TCanvas :: get_CopyMode()
{
    XGCValues values;
    unsigned long valuemask = GCFunction;
    XGetGCValues(XDpy, Brush->Handle, valuemask, &values);
    return TCopyMode(values.function);
}
//---------------------------------------------------------------------------
void TCanvas :: set_CopyMode(TCopyMode value)
{
    XSetFunction(XDpy, Brush->Handle, value);
}
//---------------------------------------------------------------------------
void TCanvas :: set_Handle(HDC value)
{
    m_Handle = value;
}
//---------------------------------------------------------------------------
void TCanvas :: Arc(int x, int y, unsigned int width, unsigned int height,
                    int angle1, int angle2)
{
//	if(Pen->Color != clNone)
    XDrawArc(XDpy, m_Handle, Pen->Handle, Origin.x + x, Origin.y + y, width, height, angle1*64, angle2*64);
}
//---------------------------------------------------------------------------
void TCanvas :: Arc(int X1, int Y1, int X2, int Y2, int X3, int Y3, int X4, int Y4)
{
    int width, height;
    int centerX, centerY;
    double theta;
    double theta2;

    width = X2-X1;
    height = Y2-Y1;

    centerX = X1 + width / 2;
    centerY = Y1 + height / 2;
    theta = RadToDeg(atan2(-1*(Y3-centerY), X3-centerX));
    theta2 = RadToDeg(atan2(-1*(Y4-centerY), X4-centerX));
    Arc(X1, Y1, width, height, int(theta) << 4, int(theta2 - theta) << 4);
}
//---------------------------------------------------------------------------
void TCanvas :: Ellipse(int x, int y, unsigned int width, unsigned int height)
{
//	if(Pen->Color != clNone)
    XDrawArc(XDpy, m_Handle, Pen->Handle, Origin.x + x, Origin.y + y, width, height, 0, 360 * 64);
}
//---------------------------------------------------------------------------
void TCanvas :: Ellipse(const TRect &rect)
{
//	if(Pen->Color != clNone)
    XDrawArc(XDpy, m_Handle, Pen->Handle, Origin.x + rect.left, Origin.y + rect.top, rect.width,
             rect.height, 0, 360 * 64);
}
//---------------------------------------------------------------------------
void TCanvas :: Chord(int x, int y, unsigned int width, unsigned int height,
                      int angle1, int angle2)
{
    XSetArcMode(XDpy, Brush->Handle, ArcChord);
//   	if(Brush->Color != clNone)
    XFillArc(XDpy, m_Handle, Brush->Handle, Origin.x + x, Origin.y + y, width, height, angle1*64, angle2*64);
}
//---------------------------------------------------------------------------
void TCanvas :: Chord(int X1, int Y1, int X2, int Y2, int X3, int Y3, int X4, int Y4)
{
    int width, height;
    int centerX, centerY;
    double theta;
    double theta2;

    width = X2 - X1;
    height = Y2 - Y1;

    centerX = X1 + width / 2;
    centerY = Y1 + height / 2;
    theta = RadToDeg(atan2(-1*(Y3-centerY), X3-centerX));
    theta2 = RadToDeg(atan2(-1*(Y4-centerY), X4-centerX));
    Chord(X1, Y1, width, height, int(theta) << 4, int(theta2 - theta) << 4);
}
//---------------------------------------------------------------------------
void TCanvas :: Pie(int x, int y, unsigned int width, unsigned int height,
                    int angle1, int angle2)
{
    XSetArcMode(XDpy, Brush->Handle, ArcPieSlice);
//   	if(Brush->Color != clNone)
    XFillArc(XDpy, m_Handle, Brush->Handle, Origin.x + x, Origin.y + y, width, height, angle1*64, angle2*64);
}
//---------------------------------------------------------------------------
void TCanvas :: Pie(int X1, int Y1, int X2, int Y2, int X3, int Y3, int X4, int Y4)
{
    int width, height;
    int centerX, centerY;
    double theta;
    double theta2;

    width = X2 - X1;
    height = Y2 - Y1;

    centerX = X1 + width / 2;
    centerY = Y1 + height / 2;
    theta = RadToDeg(atan2(-1*(Y3-centerY), X3-centerX));
    theta2 = RadToDeg(atan2(-1*(Y4-centerY), X4-centerX));
    Pie(X1, Y1, width, height, int(theta) << 4, int(theta2 - theta) << 4);
}
//---------------------------------------------------------------------------
void TCanvas :: Highlight(TRect &rect, int level)
{
    XImage *img = XGetImage(XDpy, m_Handle, Origin.x + rect.left, Origin.y + rect.top, rect.width,
                            rect.height, AllPlanes, ZPixmap);
    if(img == NULL)
        return;
    int val = abs(TRIMVAL(level,-0xFF,0xFF));
    val = RGB(val, val, val);
    XAddPixel(img, (level > 0) ? val : -val);
    XPutImage(XDpy, m_Handle, Pen->Handle, img, 0, 0, Origin.x + rect.left, Origin.y + rect.top,
              rect.width, rect.height);
    XDestroyImage(img);
}
//---------------------------------------------------------------------------
TColor TCanvas :: LightenColor(TColor color, int level)
{
    level = TRIMVAL(level,-0xFF,0xFF);
    int r, g, b;
    r = R(color) + level;
    g = G(color) + level;
    b = B(color) + level;
    r = TRIMVAL(r, 0, 0xFF);
    g = TRIMVAL(g, 0, 0xFF);
    b = TRIMVAL(b, 0, 0xFF);
    return RGB(r, g, b);
}
//---------------------------------------------------------------------------
void TCanvas :: Point(int x, int y)
{
//	if(Pen->Color != clNone)
    XDrawPoint(XDpy, m_Handle, Pen->Handle, Origin.x + x, Origin.y + y);
}
//---------------------------------------------------------------------------
void TCanvas :: Points(TPoint *points, int npoints, int mode)
{
//	if(Pen->Color != clNone)
//    {
    TPoint *ptmp = new TPoint[npoints];
    for(register int i = 0; i < npoints; i++)
    {
        ptmp[i].x = points[i].x + Origin.x;
        ptmp[i].y = points[i].y + Origin.y;
    }
    XDrawPoints(XDpy, m_Handle, Pen->Handle, ptmp, npoints, mode);
    delete [] ptmp;
//   }
}
//---------------------------------------------------------------------------
TColor TCanvas :: Pixels(int x, int y)
{
    TColor ret;

    XImage *img = XGetImage(XDpy, m_Handle, Origin.x + x, Origin.y + y, 1, 1, AllPlanes, ZPixmap);
    if(img == NULL)
        return clNone;
    ret = XGetPixel(img, 0, 0);
    XDestroyImage(img);
    return ret;
}
//---------------------------------------------------------------------------
void TCanvas :: Pixels(int x, int y, TColor color)
{
    Pen->Color = color;
    Point(x, y);
}
//---------------------------------------------------------------------------
void TCanvas :: Rectangle(int x, int y, uint_t width, uint_t height)
{
//	if(Pen->Color != clNone)
    XDrawRectangle(XDpy, m_Handle, Pen->Handle, Origin.x + x, Origin.y + y, width - 1, height - 1);
}
//---------------------------------------------------------------------------
void TCanvas :: Rectangle(const TRect &rect)
{
    if(rect.rx > 0 && rect.ry > 0)
        RoundRect(rect.left, rect.top, rect.width, rect.height,
                  rect.rx, rect.ry);
    else
        Rectangle(rect.left, rect.top, rect.width, rect.height);
}
//---------------------------------------------------------------------------
void TCanvas :: RoundRect(int x, int y, uint_t width, uint_t height, int rx, int ry)
{
    int dx, dy;
    dx = rx * 2;
    dy = ry * 2;

    Arc(x, y, dx, dy, 90, 90);
    Line(x + rx, y, x + width - rx, y);
    Arc(x + width - dx, y, dx, dy, 0, 90);
    Line(x + width, y + ry, x + width, y + height - ry);
    Arc(x + width - dx, y + height - dy, dx, dy, 0, -90);
    Line(x + rx, y + height, x + width - rx, y + height);
    Arc(x, y + height - dy, dx, dy, -90, -90);
    Line(x, y + height - ry, x, y + ry);
}
//---------------------------------------------------------------------------
void TCanvas :: Rectangles(TRect *rects, const int nrects)
{
//	if(Pen->Color == clNone)
//		return;
    XRectangle *rectangles;
    rectangles = new XRectangle[nrects];
    for(register int i = 0; i < nrects; i++)
    {
        if(rects[i].rx > 0 && rects[i].ry > 0)
            RoundRect(rects[i].left, rects[i].top, rects[i].width, rects[i].height,
                      rects[i].rx, rects[i].ry);
        else
        {
            rects[i].offset(Origin.x, Origin.y);
            rectangles[i] = rects[i].xrect();
        }
    }
    XDrawRectangles(XDpy, m_Handle, Pen->Handle, rectangles, nrects);
    delete rectangles;
}
//---------------------------------------------------------------------------
void TCanvas :: FillRect(int x, int y, uint_t width, uint_t height)
{
//	if(Brush->Color != clNone)
    XFillRectangle(XDpy, m_Handle, Brush->Handle, Origin.x + x, Origin.y + y, width, height);
}
//---------------------------------------------------------------------------
void TCanvas :: FillRect(const TRect &rect)
{
    if(rect.rx > 0 && rect.ry > 0)
        FillRoundRect(rect.left, rect.top, rect.width, rect.height,
                      rect.rx, rect.ry);
    else
        FillRect(rect.left, rect.top, rect.width, rect.height);
}
//---------------------------------------------------------------------------
void TCanvas :: FillRoundRect(int x, int y, uint_t width,
                              uint_t height, int dx, int dy)
{
    int rx, ry;
    rx = dx * 2;
    ry = dy * 2;

    FillRect(x + dx, y, width - 2 * dx, height);
    FillRect(x, y + dy, width, height - 2 * dy);

    Pie(x, y, rx, ry, 90, 90);
    Pie(x + width - rx, y, rx, ry, 0, 90);
    Pie(x + width - rx, y + height - ry, rx, ry, 0, -90);
    Pie(x, y + height - ry, rx, ry, -90, -90);
}
//---------------------------------------------------------------------------
void TCanvas :: FillRegion(const TRegion &rgn)
{
    TRegion trgn;
    if(m_ClipMask.IsEmpty())
        trgn = rgn;
    else
        trgn = m_ClipMask.Intersect(rgn);

    trgn.Offset(Origin.x, Origin.y);
    XSetRegion(XDpy, Brush->Handle, trgn.Handle);

    FillRect(rgn.ClipRect);
    XCopyGC(XDpy, Pen->Handle, GCClipMask, Brush->Handle);

    /*
        int nrectangles;
        XserverRegion xreg = XFixesCreateRegionFromGC(XDpy, Brush->Handle);
        XRectangle *rectangles = XFixesFetchRegion(XDpy, xreg, &nrectangles);
        XFillRectangles(XDpy, m_Handle, Brush->Handle, rectangles, nrectangles);
        XFree(rectangles);

        XFixesDestroyRegion(XDpy, xreg);
    */
}
//---------------------------------------------------------------------------
void TCanvas :: Draw(int x, int y, TGraphic &g)
{
    if(!g.Empty)
        g.Draw(this, Origin.x + x, Origin.y + y, TRect(0, 0, g.Width, g.Height));
}
//---------------------------------------------------------------------------
void TCanvas :: StretchDraw(const TRect &rect, TGraphic &g)
{
    if(!g.Empty)
        g.StretchDraw(this, rect);
}
//---------------------------------------------------------------------------
void TCanvas :: TiledDraw(const TRect &rect, TGraphic &g)
{
    if(!g.Empty)
        g.TiledDraw(this, rect);
}
//---------------------------------------------------------------------------
void TCanvas :: Line(int x1, int y1, int x2, int y2)
{
//	if(Pen->Color != clNone)
    XDrawLine(XDpy, m_Handle, Pen->Handle, Origin.x + x1, Origin.y + y1, Origin.x + x2, Origin.y + y2);
}
//---------------------------------------------------------------------------
void TCanvas :: LineTo(int x, int y)
{
    Line(PenPos.x, PenPos.y, x, y);
    PenPos.x = x;
    PenPos.y = y;
}
//---------------------------------------------------------------------------
void TCanvas :: MoveTo(int x, int y)
{
    PenPos.x = x;
    PenPos.y = y;
}
//---------------------------------------------------------------------------
void TCanvas :: CopyRect(const TRect &Dest, const TCanvas* Canvas, const TRect &Source)
{
    XCopyArea(XDpy, Canvas->Handle, m_Handle, Brush->Handle,
              Source.left + Canvas->Origin.x, Source.top + Canvas->Origin.y, Source.width, Source.height,
              Dest.left + Origin.x, Dest.top + Origin.y);
}
//---------------------------------------------------------------------------
int TCanvas :: TextWidth(const TString &text)
{
    TString out = Font->ConvertCharset(text);
    int ret = XTextWidth(Font->Font, out.c_str(), out.size());
    return ret;
}
//---------------------------------------------------------------------------
int TCanvas :: TextHeight(const TString &text)
{
    return TextExtents(text).height;
}
//---------------------------------------------------------------------------
TRect TCanvas :: TextExtents(const TString &text)
{
    TRect ret;
    int dummy, ascent, descent;
    XCharStruct overall;

    TString out = Font->ConvertCharset(text);
    XTextExtents(Font->Font, out.c_str(), out.size(),
                 &dummy, &ascent, &descent, &overall);
    ret.width = overall.width;
    ret.height = ascent + descent;
    return ret;
}
//---------------------------------------------------------------------------
void TCanvas :: TextOut(int x0, int y0, const TString &text)
{
    int x = x0 + Origin.x;
    int y = y0 + Origin.y;
    TString out = Font->ConvertCharset(text);
    int outy = y + Font->Ascent;
    int outsize = out.size();
    if(Brush->Style == bsClear)
        XDrawString(XDpy, m_Handle, Font->Handle, x, outy, out.c_str(), outsize);
    else
        XDrawImageString(XDpy, m_Handle, Font->Handle, x, outy, out.c_str(), outsize);

    if((Font->Style & fsUnderline) || (Font->Style & fsStrikeOut))
    {
        XGCValues values;
        ulong_t metric;
        int x1, y1;

        metric = Font->GetMetric(FM_UNDERLINE_THICKNESS);
        values.line_width = metric;
        XChangeGC(XDpy, Font->Handle, GCLineWidth, &values);
        x1 = x + XTextWidth(Font->Font, out.c_str(), outsize);

        if(Font->Style & fsUnderline)
        {
            metric = Font->GetMetric(FM_UNDERLINE_POSITION);
            y1 = outy + metric;
            XDrawLine(XDpy, m_Handle, Font->Handle, x, y1, x1, y1);
        }
        if(Font->Style & fsStrikeOut)
        {
            y1 = outy - Font->GetMetric(FM_STRIKEOUT_ASCENT) + 3;
            metric = Font->GetMetric(FM_STRIKEOUT_DESCENT) - 3;
            XDrawLine(XDpy, m_Handle, Font->Handle, x, y1, x1, outy + metric);
        }
        values.line_width = 0;
        XChangeGC(XDpy, Font->Handle, GCLineWidth, &values);
    }
}
//---------------------------------------------------------------------------
void TCanvas :: TextRect(const TRect &rect, int x, int y, const TString &text, TAlign flags)
{
    TRect tmp, r, r1;
    int ox, oy;

    r = TextExtents(text);
    r1 = rect;
    tmp = r1.align(r, flags);
    ox = x + tmp.left;
    oy = y + tmp.top;
    TRegion rgn;
    if(m_ClipMask.IsEmpty())
        rgn = TRegion(rect);
    else
        rgn = m_ClipMask.Intersect(TRegion(rect));
    rgn.Offset(Origin.x, Origin.y);

    XSetRegion(XDpy, Font->Handle, rgn.Handle);
    TextOut(ox, oy, text);
    XCopyGC(XDpy, Pen->Handle, GCClipMask, Font->Handle);
}
//---------------------------------------------------------------------------
static void __draw_prefix(TCanvas *canvas, TRect &rect, TRect &srect,
                          TString &out, TAlign flags, const TFormatText *format)
{
    TRect trect;
    int tmpw, x;
    const char *amp = "&";
    TString::size_type pos1, pos2;
    pos2 = out.find('&');
    if(pos2 != TString::npos)
    {
        TString tmp;
        list<TString::size_type> li;
        tmpw = XTextWidth(canvas->Font->Font, amp, 1);
        do
        {
            if(out[pos2 + 1] != '&')
                li.push_back(pos2);
            out.erase(pos2, 1);
            pos1 = pos2 + 1;
            pos2 = out.find('&', pos1);
        }
        while(pos2 != TString::npos);
        trect = rect.align(srect, flags);
        pos1 = 0;
        x = trect.left;
        for(list<TString::size_type>::iterator from = li.begin();
                from != li.end();
                ++from)
        {
            pos2 = *from;
            tmp = out.substr(pos1, pos2 - pos1);
            tmpw = XTextWidth(canvas->Font->Font, tmp.c_str(), tmp.size());
            canvas->TextOut(x, srect.top, tmp);

            tmp = out[pos2];
            if(tmp[0] != '\0')
            {
                TColor oldcolor = canvas->Font->Color;
                uint_t oldstyle = canvas->Font->Style;
                canvas->Font->Style |= (format->PrefixStyle & (pmBold | pmUnderline));
                if(format->PrefixStyle & pmColor)
                    canvas->Font->Color = format->PrefixColor;
                x += tmpw;
                canvas->TextOut(x, srect.top, tmp);
                tmpw = XTextWidth(canvas->Font->Font, tmp.c_str(), 1);
                x += tmpw;
                canvas->Font->Style = oldstyle;
                canvas->Font->Color = oldcolor;
            }
            pos1 = pos2 + 1;
        }
        if(tmp[0] != '\0')
        {
            tmp = out.substr(pos2 + 1);
            canvas->TextOut(x, srect.top, tmp);
        }
    }
    else
    {
        trect = rect.align(srect, flags);
        canvas->TextOut(trect.left, srect.top, out);
    }
}

static int __draw_len(TCanvas *canvas, TString &str, const TFormatText *format)
{
    int tmpw, ret = 0;
    const char *amp = "&";
    TString::size_type pos1, pos2;
    TString tmp, out = str;
    pos2 = out.find('&');
    if(pos2 != TString::npos)
    {
        list<TString::size_type> li;
        tmpw = XTextWidth(canvas->Font->Font, amp, 1);
        do
        {
            if(out[pos2 + 1] != '&')
                li.push_back(pos2);
            out.erase(pos2, 1);
            pos1 = pos2 + 1;
            pos2 = out.find('&', pos1);
        }
        while(pos2 != TString::npos);
        pos1 = 0;
        for(list<TString::size_type>::iterator from = li.begin();
                from != li.end();
                ++from)
        {
            pos2 = *from;
            tmp = out.substr(pos1, pos2 - pos1);
            tmpw = XTextWidth(canvas->Font->Font, tmp.c_str(), tmp.size());
            ret += tmpw;

            tmp = out[pos2];
            if(tmp[0] != '\0')
            {
                TColor oldcolor = canvas->Font->Color;
                uint_t oldstyle = canvas->Font->Style;
                canvas->Font->Style |= (format->PrefixStyle & (pmBold | pmUnderline));
                if(format->PrefixStyle & pmColor)
                    canvas->Font->Color = format->PrefixColor;
                tmpw = XTextWidth(canvas->Font->Font, tmp.c_str(), 1);
                ret += tmpw;
                canvas->Font->Style = oldstyle;
                canvas->Font->Color = oldcolor;
            }
            pos1 = pos2 + 1;
        }
        if(tmp[0] != '\0')
        {
            tmp = out.substr(pos2 + 1);
            tmpw = XTextWidth(canvas->Font->Font, tmp.c_str(), tmp.size());
            ret += tmpw;
        }
    }
    else
    {
        ret = XTextWidth(canvas->Font->Font, str.c_str(), str.size());
    }
    return ret;
}

static void __add_ellipsis(TCanvas *canvas, TString &str, int maxlen)
{
    int clen = 0;
    int plen = 0;
    const char *dots = "...";
    int dlen = canvas->TextWidth(dots);
    const char *p = str.c_str();

    while(plen + dlen < maxlen)
    {
        plen += XTextWidth(canvas->Font->Font, p, 1);
        p++;
        clen++;
        if(*p == '\0')
            return;
    };
    str.resize(clen);
    str.append(dots);
}

static void __draw_format_text(TCanvas *canvas, TRect &rect, TRect &srect,
                               TString &out, TAlign flags, const TFormatText *format)
{
    TRect trect;
    if(format->Options & DT_NOPREFIX)
    {
        trect = rect.align(srect, flags);
        if( !(format->Options & DT_CALCRECT) )
            canvas->TextOut(trect.left, srect.top, out);
    }
    else if( !(format->Options & DT_CALCRECT) )
        __draw_prefix(canvas, rect, srect, out, flags, format);
    srect.top += srect.height;
    out.erase();
}

static void __draw_text(TCanvas *canvas, TRect &rect, const TString &text, const TFormatText *format)
{
    const char *wb = " `~!@#$%^*()-+=|\\\n\t/.,<>;:[]}{\"'";
    TAlign flags;
    int ch = 0, swidth, space;
    uint_t w = 0, h = 0;
    TRect srect;
    bool final = true;
    TString::size_type pos1, pos2, tsize;
    TString tmp, out;
    switch(format->Justify)
    {
    case taLeftJustify:
        flags = alLeft;
        break;
    case taCenter:
        flags = alCenter;
        break;
    case taRightJustify:
        flags = alRight;
        break;
    case taFull:
        break;
    }
    srect.left = rect.left;
    srect.top = rect.top;
    srect.height = canvas->Font->Font->ascent + canvas->Font->Font->descent + format->Leading;
    space = XTextWidth(canvas->Font->Font, " ", 1);
    pos1 = 0;
    do
    {
        pos2 = text.find_first_of(wb, pos1);
        if(pos2 != TString::npos)
        {
            ch = text[pos2];
            tmp = text.substr(pos1, pos2 - pos1 + 1);
        }
        else
        {
            tmp = text.substr(pos1);
            final = false;
        }
        if(ch == '\t')
        {
            if(format->Options & DT_EXPANDTABS)
                swidth = format->Tab;
            else
                swidth = 1;
            tmp.replace(tmp.size() - 1, 1, swidth, ' ');
        }
        else if(ch == '\n')// && !(format->Options & DT_WORDBREAK))
        {
//   			tmp.replace(tmp.size() - 1, 1, 1, ' ');
            tmp.erase(tmp.size() - 1, 1);
//   			ch = ' ';
        }
        tsize = tmp.size();
        swidth = __draw_len(canvas, tmp, format);
        if(format->Options & DT_WORDBREAK && srect.width + swidth > rect.width + 1)
        {
            if(srect.width == 0)
            {
                pos1 += tsize;
                out = tmp;
                srect.width = swidth;
            }
            if(*out.rbegin() == ' ')
            {
                out.erase(out.size() - 1);
                srect.width -= space;
            }
            __draw_format_text(canvas, rect, srect, out, flags, format);
            srect.width = 0;
            h += srect.height;
            final = true;
        }
        else
        {
            out += tmp;
            srect.width += swidth;
            pos1 = pos2 + 1;
            if(ch == '\n' || final == false)
            {
                if(format->Options & DT_END_ELLIPSIS)
                    if(srect.width > rect.width)
                        __add_ellipsis(canvas, out, rect.width);
                __draw_format_text(canvas, rect, srect, out, flags, format);
                if(final != false)
                    srect.width = 0;
                h += srect.height;
            }
        }
        if(w < srect.width)
            w = srect.width;
    }
    while(final);
    if(format->Options & DT_CALCRECT)
        rect = TRect(rect.left, rect.top, w, h);
}

void TCanvas :: DrawText(TRect &rect, const TString &text, TFormatText *format)
{
    TFormatText defformat;
    if(format == NULL)
        format = &defformat;
    if(format->Options & DT_CALCRECT)
    {
        __draw_text(this, rect, text, format);
        return;
    }
    bool clipping = false;
    if(!(format->Options & DT_NOCLIP))
    {
        TRegion rgn;
        if(m_ClipMask.IsEmpty())
            rgn = TRegion(rect);
        else
            rgn = m_ClipMask.Intersect(TRegion(rect));
        rgn.Offset(Origin.x, Origin.y);
        XSetRegion(XDpy, Font->Handle, rgn.Handle);
        clipping = true;
    }
    if(format->Layout == tlTop)
        __draw_text(this, rect, text, format);
    else
    {
        TAlign flags;
        TRect tmprect = rect;
        format->Options = BIT_ON(format->Options, DT_CALCRECT);
        __draw_text(this, tmprect, text, format);
        format->Options = BIT_OFF(format->Options, DT_CALCRECT);
        tmprect.left = rect.left;
        tmprect.top = rect.top;
        if(format->Layout == tlCenter)
            flags = alVCenter;
        else if(format->Layout == tlBottom)
            flags = alBottom;
        else
            flags = alTop;
        tmprect = rect.align(tmprect, flags);
        __draw_text(this, tmprect, text, format);
    }
    if(clipping)
    {
        XCopyGC(XDpy, Pen->Handle, GCClipMask, Font->Handle);
    }
}
//---------------------------------------------------------------------------
void TCanvas :: TextLine(int x, int y, int delta, TStrings &text)
{
    XTextItem *items;
    int count = text.Count;
    TStrings out;
    out.Count = count;
    items = new XTextItem[count];
    for(register int i = 0; i < count; i++)
    {
        out[i] = Font->ConvertCharset(text[i]);
        items[i].chars = (char *)out[i].c_str();
        items[i].nchars = out[i].size();
        items[i].delta = delta;
        items[i].font = None;
    }
    XDrawText(XDpy, m_Handle, Font->Handle, x + Origin.x, y + Origin.y, items, count);
    delete items;
}
//---------------------------------------------------------------------------
void TCanvas :: SetClipMask(HBITMAP abitmap)
{
    XSetClipMask(XDpy, Pen->Handle, abitmap);
//    if(abitmap != None)
    XSetClipOrigin(XDpy, Pen->Handle, m_ClipOrigin.x + Origin.x, m_ClipOrigin.y + Origin.y);

    XCopyGC(XDpy, Pen->Handle, GCClipMask | GCClipXOrigin | GCClipYOrigin, Brush->Handle);
    XCopyGC(XDpy, Pen->Handle, GCClipMask | GCClipXOrigin | GCClipYOrigin, Font->Handle);

    XserverRegion xreg = XFixesCreateRegionFromBitmap(XDpy, abitmap);
    __convert_region(m_ClipMask.Handle, xreg);
    /*
    	unsigned long	valuemask;				//  XRender
    	XRenderPictureAttributes pa;
    	valuemask = CPClipMask | CPClipXOrigin | CPClipYOrigin;
    	pa.clip_mask = abitmap;
    	pa.clip_x_origin = m_ClipOrigin.x + m_Origin.x;
    	pa.clip_y_origin = m_ClipOrigin.y + m_Origin.y;
    	XRenderChangePicture(Application.Handle, m_Picture, valuemask, &pa); */
}
//---------------------------------------------------------------------------
void TCanvas :: SetClipMask(const TBitmap &abitmap)
{
    SetClipMask(abitmap.Handle);
}
//---------------------------------------------------------------------------
//void TCanvas :: SetClipMask(const TRect &rect)
//{
//	XRectangle rectangles[1];
//	rectangles[0].x = rect.left;
//	rectangles[0].y = rect.top;
//	rectangles[0].width = rect.width;
//	rectangles[0].height = rect.height;
//	XSetClipRectangles(XDpy, Pen->Handle, 0, 0, rectangles, 1, Unsorted);
//	XCopyGC(XDpy, Pen->Handle, GCClipMask, Brush->Handle);
//	XCopyGC(XDpy, Pen->Handle, GCClipMask, Font->Handle);
//}
//---------------------------------------------------------------------------
void TCanvas :: RemoveClipMask()
{
    XSetClipMask(XDpy, Pen->Handle, None);
    XSetClipMask(XDpy, Brush->Handle, None);
    XSetClipMask(XDpy, Font->Handle, None);
    m_ClipMask.Clear();

//	unsigned long	valuemask;
//	XRenderPictureAttributes pa;
//	valuemask = CPClipMask;
//	pa.clip_mask = None;
//	XRenderChangePicture(Application.Handle, m_Picture, valuemask, &pa);
}
//---------------------------------------------------------------------------
void TCanvas :: Polygon(TPoint *points, const int npoints)
{
//	if(Brush->Color != clNone)
//    {
    TPoint *ptmp = new TPoint[npoints];
    for(register int i = 0; i < npoints; i++)
    {
        ptmp[i].x = points[i].x + Origin.x;
        ptmp[i].y = points[i].y + Origin.y;
    }
    XFillPolygon(XDpy, m_Handle, Brush->Handle, ptmp, npoints, Complex, CoordModeOrigin);
    delete [] ptmp;
//    }
}
//---------------------------------------------------------------------------
void TCanvas :: Polyline(TPoint *points, const int npoints)
{
//	if(Pen->Color != clNone)
//    {
    TPoint *ptmp = new TPoint[npoints];
    for(register int i = 0; i < npoints; i++)
    {
        ptmp[i].x = points[i].x + Origin.x;
        ptmp[i].y = points[i].y + Origin.y;
    }
    XDrawLines(XDpy, m_Handle, Pen->Handle, ptmp, npoints, CoordModeOrigin);
    delete [] ptmp;
//   }
}
//---------------------------------------------------------------------------
void TCanvas :: Lock()
{
    XLockDisplay(XDpy);
}
//---------------------------------------------------------------------------
void TCanvas :: Unlock()
{
    XUnlockDisplay(XDpy);
}
//---------------------------------------------------------------------------
void TCanvas :: Composite(int op, const TRect &dest, const TCanvas* canvas, const TRect &source)
{
//	XRenderComposite(XDpy, op, canvas->Picture, None, m_Picture,
//					source.left, source.top, 0, 0, dest.left, dest.top,
//					source.width, source.height);
}
//---------------------------------------------------------------------------
//  TGraphic
//---------------------------------------------------------------------------
TGraphic :: TGraphic()
{
    Init();
}
//---------------------------------------------------------------------------
__fastcall TGraphic :: TGraphic(const TGraphic &a) : TObject(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TGraphic & TGraphic :: operator=(const TGraphic &a)
{
    if(this == &a)
        return *this;
    TObject :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TGraphic :: Init()
{
    m_Width = m_Height = 0;
    HotSpot.x = HotSpot.y = -1;
    ResizeMode = rmCrop;
}
//---------------------------------------------------------------------------
void __fastcall TGraphic :: Assign(const TGraphic &obj)
{
    m_Width = obj.m_Width;
    m_Height = obj.m_Height;
    HotSpot = obj.HotSpot;
    ResizeMode = obj.ResizeMode;
}
//---------------------------------------------------------------------------
void TGraphic :: set_Height(uint_t value)
{
    if(m_Height == value  || value == 0)
        return;
    if(m_Width)
        SetSize(m_Width, value);
    else
        m_Height = value;
}
//---------------------------------------------------------------------------
void TGraphic :: set_Width(uint_t value)
{
    if(m_Width == value || value == 0)
        return;
    if(m_Height)
        SetSize(value, m_Height);
    else
        m_Width = value;
}
//---------------------------------------------------------------------------
void TGraphic :: TiledDraw(TCanvas *canvas, const TRect &rect)
{
    int x, y;
    TRect r;

    x = rect.left;
    while(x < rect.right())
    {
        y = rect.top;
        r.left = x;
        r.width = m_Width;
        while(y < rect.bottom())
        {
            r.top = y;
            r.height = m_Height;
            Draw(canvas, 0, 0, r);
            y = r.bottom();
        }
        x = r.right();
    }
}
//---------------------------------------------------------------------------
void TGraphic :: Progress(TObject *Sender, TProgressStage Stage, byte_t PercentDone,
                          bool RedrawNow, const TRect &R, const TString &Msg)
{
    if(OnProgress)
        OnProgress(Sender, Stage, PercentDone, RedrawNow, R, Msg);
}
//---------------------------------------------------------------------------
/*

{ TGraphic }

procedure TGraphic.Changed(Sender: TObject);
begin
  FModified := True;
  if Assigned(FOnChange) then FOnChange(Self);
end;

procedure TGraphic.DefineProperties(Filer: TFiler);

  function DoWrite: Boolean;
  begin
    if Filer.Ancestor <> nil then
      Result := not (Filer.Ancestor is TGraphic) or
        not Equals(TGraphic(Filer.Ancestor))
    else
      Result := not Empty;
  end;

begin
  Filer.DefineBinaryProperty('Data', ReadData, WriteData, DoWrite);
end;

function TGraphic.Equals(Graphic: TGraphic): Boolean;
var
  MyImage, GraphicsImage: TMemoryStream;
begin
  Result := (Graphic <> nil) and (ClassType = Graphic.ClassType);
  if Empty or Graphic.Empty then
  begin
    Result := Empty and Graphic.Empty;
    Exit;
  end;
  if Result then
  begin
    MyImage := TMemoryStream.Create;
    try
      WriteData(MyImage);
      GraphicsImage := TMemoryStream.Create;
      try
        Graphic.WriteData(GraphicsImage);
        Result := (MyImage.Size = GraphicsImage.Size) and
          CompareMem(MyImage.Memory, GraphicsImage.Memory, MyImage.Size);
      finally
        GraphicsImage.Free;
      end;
    finally
      MyImage.Free;
    end;
  end;
end;

function TGraphic.GetTransparent: Boolean;
begin
  Result := FTransparent;
end;

procedure TGraphic.LoadFromFile(const Filename: string);
var
  Stream: TStream;
begin
  Stream := TFileStream.Create(Filename, fmOpenRead or fmShareDenyWrite);
  try
    LoadFromStream(Stream);
  finally
    Stream.Free;
  end;
end;

procedure TGraphic.ReadData(Stream: TStream);
begin
  LoadFromStream(Stream);
end;

procedure TGraphic.SaveToFile(const Filename: string);
var
  Stream: TStream;
begin
  Stream := TFileStream.Create(Filename, fmCreate);
  try
    SaveToStream(Stream);
  finally
    Stream.Free;
  end;
end;

procedure TGraphic.SetModified(Value: Boolean);
begin
  if Value then
    Changed(Self) else
    FModified := False;
end;

procedure TGraphic.SetTransparent(Value: Boolean);
begin
  if Value <> FTransparent then
  begin
    FTransparent := Value;
    Changed(Self);
  end;
end;

procedure TGraphic.WriteData(Stream: TStream);
begin
  SaveToStream(Stream);
end;


*/
//---------------------------------------------------------------------------
//  TPixelFormat
//---------------------------------------------------------------------------
TPixelFormat :: TPixelFormat()
{
    BitOrder = BitmapBitOrder(XDpy);
    ByteOrder = ImageByteOrder(XDpy);
    Depth = (TPixelSize)DefaultDepthOfScreen(Screen.Handle);
    switch(Depth)
    {
    case pfNone:
        ColorMode = cmNone;
        break;
    case pf1bit:
        ColorMode = cmBW;
        break;
    case pf4bit:
        ColorMode = cmIndexed;
        break;
    case pf8bit:
        ColorMode = cmGrayScale;
        break;
    case pf16bit:
    case pf24bit:
    case pf32bit:
        ColorMode = cmColor;
        break;
    };
}
//---------------------------------------------------------------------------
TPixelFormat :: TPixelFormat(const TPixelFormat &pf)
{
    BitOrder = pf.BitOrder;
    ByteOrder = pf.ByteOrder;
    Depth = pf.Depth;
    ColorMode = pf.ColorMode;
}
//---------------------------------------------------------------------------
TPixelFormat :: TPixelFormat(TPixelSize depth)
{
    BitOrder = BitmapBitOrder(XDpy);
    ByteOrder = ImageByteOrder(XDpy);
    Depth = depth;
    switch(Depth)
    {
    case pfNone:
        ColorMode = cmNone;
        break;
    case pf1bit:
        ColorMode = cmBW;
        break;
    case pf4bit:
        ColorMode = cmIndexed;
        break;
    case pf8bit:
        ColorMode = cmGrayScale;
        break;
    case pf16bit:
    case pf24bit:
    case pf32bit:
        ColorMode = cmColor;
        break;
    };
}
//---------------------------------------------------------------------------
//  TBitmap
//---------------------------------------------------------------------------
static HDIB create_dib_handle(uint_t w, uint_t h, const TPixelFormat &pf)
{
    XImage *img;

//   	img = new XImage();
//   	memset(img, 0, sizeof(XImage));
    img = (XImage *)calloc(1, sizeof(XImage));
    img->width = w;
    img->height = h;
    img->xoffset = 0;
    img->format = ZPixmap;
    img->bitmap_unit = BitmapUnit(XDpy);
    img->bitmap_pad = BitmapPad(XDpy);
    img->bytes_per_line = 0;
    img->bitmap_bit_order = pf.BitOrder;
    img->depth = pf.Depth;
    img->byte_order = pf.ByteOrder;
    img->bits_per_pixel = img->depth;
    XInitImage(img);

//	img->data = new char[img->bytes_per_line * h];
//	memset(img->data, 0, img->bytes_per_line * h);
    img->data = (char *)calloc(h, img->bytes_per_line);
    return img;
}
//---------------------------------------------------------------------------
static TPixelFormat getpf_dib(HDIB src)
{
    TPixelFormat pf((TPixelSize)src->depth);

    pf.ByteOrder = src->byte_order;
    pf.BitOrder = src->bitmap_bit_order;
    return pf;
}
//---------------------------------------------------------------------------
static HBITMAP dib2ddb(HDIB src)
{
    HBITMAP ret;
    GC gc;

    ret = XCreatePixmap(XDpy, Screen.Root, src->width, src->height, src->depth);
    gc = XCreateGC(XDpy, ret, 0, NULL);
    XFillRectangle(XDpy, ret, gc, 0, 0, src->width, src->height);
    XPutImage(XDpy, ret, gc, src, 0, 0, 0, 0, src->width, src->height);
    XFreeGC(XDpy, gc);
    return ret;
}
//---------------------------------------------------------------------------
#define ROTPOINTX(x, y)  	(x - cx) * cAngle - (y - cy) * sAngle + cx
#define ROTPOINTY(x, y)   	(x - cx) * sAngle + (y - cy) * cAngle + cy

static HDIB rotate_dib(HDIB src, int angle, bool resize, TColor fill)
{
    double cAngle, sAngle, alf;
    int xDiff, yDiff, ifx, ify, px, py, x, y, cx, cy, d;
    ulong_t pu;
    byte_t *P1, *P3;
    TPoint points[4], srcp[4];
    HDIB dst;

    d = src->depth;
    cx = src->width >> 1;
    cy = src->height >> 1;
    alf = angle * M_PI / 180;
    sAngle = sin(alf);
    cAngle = cos(alf);
    TPixelFormat pf(getpf_dib(src));
//	pf = getpf_dib(src);
    if(resize)
    {
        srcp[0] = Point(0, 0);
        srcp[1] = Point(src->width, 0);
        srcp[2] = Point(src->width, src->height);
        srcp[3] = Point(0, src->height);
        for(register int i = 0; i < 4; i++)
        {
            x = srcp[i].x;
            y = srcp[i].y;
            points[i].x = ROTPOINTX(x, y);
            points[i].y = ROTPOINTY(x, y);
        }
        TRegion rgn(points,4);
        TRect rect = rgn.ClipRect;
        cx = rect.width >> 1;
        cy = rect.height >> 1;
        dst = create_dib_handle(rect.width, rect.height, pf);
    }
    else
        dst = create_dib_handle(src->width, src->height, pf);

    xDiff = (dst->width - src->width - 1) / 2;
    yDiff = (dst->height - src->height - 1) / 2;
    for(y = 0; y < dst->height; y++)
    {
        P3 = (byte_t *)(dst->data + y * dst->bytes_per_line);
        py = ((y - cy) << 1) + 1;
        for(x = 0; x < dst->width; x++)
        {
            px = ((x - cx) << 1) + 1;
            ifx = (((int(px * cAngle - py * sAngle) - 1) >> 1) + cx) - xDiff;
            ify = (((int(px * sAngle + py * cAngle) - 1) >> 1) + cy) - yDiff;

            if((ifx > -1) && (ifx < src->width) && (ify > -1) && (ify < src->height))
            {
                P1 = (byte_t *)(src->data + ify * src->bytes_per_line);
                switch(d)
                {
                case pf1bit:
                case pf4bit:
                    pu = XGetPixel(src, ifx, ify);
                    XPutPixel(dst, x, y, pu);
                    break;
                case pf8bit:
                    P3[x] = P1[ifx];
                    break;
                case pf16bit:
                    P3[x * 2 + 1] = P1[ifx * 2 + 1];
                    P3[x * 2] = P1[ifx * 2];
                    break;
                case pf24bit:
                    P3[x * 3 + 2] = P1[ifx * 3 + 2];
                    P3[x * 3 + 1] = P1[ifx * 3 + 1];
                    P3[x * 3] = P1[ifx * 3];
                    break;
                case pf32bit:
                    P3[x * 4 + 3] = P1[ifx * 4 + 3];
                    P3[x * 4 + 2] = P1[ifx * 4 + 2];
                    P3[x * 4 + 1] = P1[ifx * 4 + 1];
                    P3[x * 4] = P1[ifx * 4];
                    break;

                }
            }
            else if(fill != clBlack)
                XPutPixel(dst, x, y, fill);
        };
    };
    return dst;
}
#undef ROTPOINTX
#undef ROTPOINTY
//---------------------------------------------------------------------------
static HDIB stretch_dib(HDIB src, uint_t w, uint_t h)
{
    int xP, yP, yP2, xP2;
    byte_t *Read;
    int t, d, v;
    byte_t *pc;
    HDIB dst;
    TPixelFormat pf(getpf_dib(src));
    ulong_t pu;

//	pf = getpf_dib(src);
    dst = create_dib_handle(w, h, pf);

    d = src->depth;

    xP2 = ((src->width) << 15) / dst->width;
    yP2 = ((src->height) << 15) / dst->height;

    yP = 0;
    for(register int y = 0; y < dst->height; y++)
    {
        xP = 0;
        v = yP >> 15;
        Read = (byte_t *)(src->data + v * src->bytes_per_line);
        pc = (byte_t *)(dst->data + y * dst->bytes_per_line);
        for(register int  x = 0; x < dst->width; x++)
        {
            t = xP >> 15;
            switch(d)
            {
            case pf1bit:
            case pf4bit:
                pu = XGetPixel(src, t, v);
                XPutPixel(dst, x, y, pu);
                break;
            case pf8bit:
                pc[x] = Read[t];
                break;
            case pf16bit:
                pc[x * 2 + 1] = Read[t * 2 + 1];
                pc[x * 2] = Read[t * 2];
                break;
            case pf24bit:
                pc[x * 3 + 2] = Read[t * 3 + 2];
                pc[x * 3 + 1] = Read[t * 3 + 1];
                pc[x * 3] = Read[t * 3];
                break;
            case pf32bit:
                pc[x * 4 + 3] = Read[t * 4 + 3];
                pc[x * 4 + 2] = Read[t * 4 + 2];
                pc[x * 4 + 1] = Read[t * 4 + 1];
                pc[x * 4] = Read[t * 4];
                break;

            }
            xP += xP2;
        };
        yP += yP2;
    };
    return dst;
}
//---------------------------------------------------------------------------
static void mirror_vert_dib(HDIB src)
{
    byte_t *sp, *dp, *tp;
    int n, m, k;
    n = src->height / 2;
    m = src->bytes_per_line;
    k = src->height - 1;
    tp = new byte_t[m];
    for(register int i = 0; i < n; i++)
    {
        sp = (byte_t *)(src->data + i * m);
        dp = (byte_t *)(src->data + (k - i) * m);
        memcpy(tp, dp, m);
        memcpy(dp, sp, m);
        memcpy(sp, tp, m);
    }
    delete tp;
}
//---------------------------------------------------------------------------
static void mirror_horz_dib(HDIB src)
{
    ulong_t tmp1, tmp2;
    int n, k;
    n = src->height;
    k = src->width;
    for(register int j = 0; j < n; j++)
    {
        for(register int i = 0; i < k / 2; i++)
        {
            tmp1 = XGetPixel(src, i, j);
            tmp2 = XGetPixel(src, k-1-i, j);
            XPutPixel(src, k-1-i, j, tmp1);
            XPutPixel(src, i, j, tmp2);
        }
    }
}
//---------------------------------------------------------------------------
static HDIB convert_depth(HDIB in, TPixelSize depth, RGBQUAD *colormap)
{
    int nbytes;
    HDIB out;
    TPixelFormat pf(getpf_dib(in));
    pf.Depth = depth;

    out = create_dib_handle(in->width, in->height, pf);
    printf("CONVERTDEPTH from %d to %d\n",in->depth, out->depth);

    if(in->depth == pf32bit && out->depth == pf24bit)
    {
        clock_t cl1 = clock();
        byte_t *tp, *tm;
        for(register int j = 0; j < in->height; j++)
        {
            tm = (byte_t *)in->data + j * in->bytes_per_line;
            tp = (byte_t *)out->data + j * out->bytes_per_line;
            for(register int i = 0; i < in->width; i++)
                memcpy(tp + i * 3, tm + i * 4, 3);
        }
        printf("TIME 32->24 %ld  %ld\n", clock() - cl1, CLOCKS_PER_SEC);
        return out;
    }

    if(in->depth == pf24bit && out->depth == pf32bit)
    {
        clock_t cl1 = clock();
        byte_t *tp, *tm;
        for(register int j = 0; j < in->height; j++)
        {
            tm = (byte_t *)in->data + j * in->bytes_per_line;
            tp = (byte_t *)out->data + j * out->bytes_per_line;
            for(register int i = 0; i < in->width; i++)
                memcpy(tp + i * 4, tm + i * 3, 3);
        }
        printf("TIME 24->32 %ld  %ld\n", clock() - cl1, CLOCKS_PER_SEC);
        return out;
    }

    if(in->depth == pf8bit && out->depth == pf24bit)
    {
        clock_t cl1 = clock();
        nbytes = in->bytes_per_line * in->height;
        if(colormap == NULL)
        {
            for(register int i = 0; i < nbytes; i++)
                memset(out->data + i * 3, *(in->data + i), 3);
        }
        else
        {
            byte_t idx;
            byte_t *op;
            byte_t *ip;
            for(register int j = 0; j < in->height; j++)
            {
                op = (byte_t *)out->data + j * out->bytes_per_line;
                ip = (byte_t *)in->data + j * in->bytes_per_line;
                for(register int i = 0; i < in->bytes_per_line; i++)
                {
                    idx = *(ip + i);
                    memcpy(op, &colormap[idx], 3);
                    op += 3;
                }
            }
        }
        printf("TIME 8->24 %ld  %ld\n", clock() - cl1, CLOCKS_PER_SEC);
        return out;
    }

    if(in->depth == pf24bit && out->depth == pf8bit)
    {
        clock_t cl1 = clock();
        nbytes = out->bytes_per_line * out->height;
        int r, g, b, intens;

        if(colormap == NULL)
        {
            for(register int i = 0; i < nbytes; i++)
            {
                r = *(in->data + i * 3);
                g = *(in->data + i * 3 + 1);
                b = *(in->data + i * 3 + 2);
                intens = (30 * r + 59 * g + 11 * b) / 100;
                *(out->data + i) = intens;
            }
        }
        else
        {
            byte_t idx;
            RGBQUAD u1, u2;

            for(register int i = 0; i < nbytes; i++)
            {
                u1.p = *(in->data + i * 3);
                u1.a = 0;
                for(register int j = 0; j < 256; j++)
                {
                    u2 = colormap[j];
                    if(u1.p == u2.p)
                    {
                        idx = j;
                        break;
                    }
                }
                *(out->data + i) = idx;
            }
        }
        printf("TIME 24->8 %ld  %ld\n", clock() - cl1, CLOCKS_PER_SEC);
        return out;
    }

    if(in->depth == pf1bit && out->depth == pf8bit)
    {
        unsigned long p1;
        clock_t cl1 = clock();
        for(register int j = 0; j < in->height; j++)
            for(register int i = 0; i < in->width; i++)
            {
                p1 = XGetPixel(in, i, j);
                XPutPixel(out, i, j, p1 ? 0xFF : 0x00);
            }
        printf("TIME 1->8 %ld  %ld\n", clock() - cl1, CLOCKS_PER_SEC);
        return out;
    }

    if(in->depth == pf8bit && out->depth == pf1bit)
    {
        unsigned long p1;
        clock_t cl1 = clock();
        for(register int j = 0; j < in->height; j++)
            for(register int i = 0; i < in->width; i++)
            {
                p1 = XGetPixel(in, i, j);
                XPutPixel(out, i, j, (p1 >= (0xFF >> 1)) ? 1 : 0);
            }
        printf("TIME 8->1 %ld  %ld\n", clock() - cl1, CLOCKS_PER_SEC);
        return out;
    }

    if(in->depth == pf4bit && out->depth == pf24bit)
    {
        clock_t cl1 = clock();
        unsigned long p1;
        RGBQUAD u;

        for(register int j = 0; j < in->height; j++)
            for(register int i = 0; i < in->width; i++)
            {
                p1 = XGetPixel(in, i, j);
                u = colormap[p1];
                XPutPixel(out, i, j, u.p);
            }
        printf("TIME 4->24 %ld  %ld\n", clock() - cl1, CLOCKS_PER_SEC);
        return out;
    }

    if(in->depth == pf24bit && out->depth == pf4bit)
    {
        clock_t cl1 = clock();
        RGBQUAD u1, u2;

        for(register int j = 0; j < in->height; j++)
            for(register int i = 0; i < in->width; i++)
            {
                u1.p = XGetPixel(in, i, j);
                for(register int k = 0; k < 16; k++)
                {
                    u2 = colormap[k];
                    if(u1.p == u2.p)
                    {
                        XPutPixel(out, i, j, k);
                        break;
                    }
                }
            }
        printf("TIME 24->4 %ld  %ld\n", clock() - cl1, CLOCKS_PER_SEC);
        return out;
    }

    if(in->depth == pf16bit && out->depth == pf24bit)
    {
        clock_t cl1 = clock();
        RGBQUAD rgb;
        word_t w;
        byte_t *tp = (byte_t *)out->data;
        byte_t *tm;
        nbytes = in->width << 1;
        for(register int j = 0; j < in->height; j++)
        {
            tm = (byte_t *)in->data + j * in->bytes_per_line;
            for(register int i = 0; i < nbytes; i += 2)
            {
                w = *(word_t *)(tm + i) ;
                rgb.r = (w & 0x1F) * 0xFF / 0x1F;
                rgb.g = ((w >> 5) & 0x1F) * 0xFF / 0x1F;
                rgb.b = ((w >> 10) & 0x1F) * 0xFF / 0x1F;
                memcpy(tp, &rgb, 3);
                tp += 3;
            }
        }
        printf("TIME 16->24 %ld  %ld\n", clock() - cl1, CLOCKS_PER_SEC);
        return out;
    }

    if(in->depth == pf24bit && out->depth == pf16bit)
    {
        clock_t cl1 = clock();
        RGBQUAD rgb;
        word_t w;
        word_t *tp = (word_t *)out->data;
        byte_t *tm;
        nbytes = in->width * 3;
        for(register int j = 0; j < in->height; j++)
        {
            tm = (byte_t *)in->data + j * in->bytes_per_line;
            for(register int i = 0; i < nbytes; i += 3)
            {
                memcpy(&rgb, tm + i, 3);
                w = (rgb.r * 0x1F / 0xFF) | ((rgb.g * 0x1F / 0xFF) << 5) |
                    ((rgb.b * 0x1F / 0xFF) << 10);
                *tp++ = w;
            }
        }
        printf("TIME 24->16 %ld  %ld\n", clock() - cl1, CLOCKS_PER_SEC);
        return out;
    }

    XDestroyImage(out);
    HDIB tmp = convert_depth(in, pf24bit, colormap);
    out = convert_depth(tmp, depth, colormap);
    XDestroyImage(tmp);
    return out;
}
//---------------------------------------------------------------------------
inline static int get_std_pict_format(TPixelSize ps)
{
    int format;
    switch(ps)
    {
    case pf1bit:
        format = PictStandardA1;
        break;
    case pf4bit:
        format = PictStandardA4;
        break;
    case pf8bit:
        format = PictStandardA8;
        break;
    case pf16bit:
        format = PictStandardA8;
        break;
    case pf24bit:
        format = PictStandardRGB24;
        break;
    case pf32bit:
        format = PictStandardARGB32;
        break;
    default:
        format = PictStandardRGB24;
    }
    return format;
}
//---------------------------------------------------------------------------
static void combine_mask(TCanvas *canvas, HBITMAP mask, int x, int y)
{
    XserverRegion xreg1 = XFixesCreateRegionFromGC(XDpy, canvas->Brush->Handle);
    XserverRegion xreg2 = XFixesCreateRegionFromBitmap(XDpy, mask);
    XFixesTranslateRegion(XDpy, xreg2, x, y);
    XFixesIntersectRegion(XDpy, xreg1, xreg1, xreg2); //  XFixesChangeSaveSet()
    XFixesSetGCClipRegion(XDpy, canvas->Pen->Handle, 0, 0, xreg1);
//    XFlushGC(XDpy, canvas->Pen->Handle);
//    XFillRectangle(XDpy,canvas->Handle, canvas->Pen->Handle, 0, 0, 600, 600);
    XFixesDestroyRegion(XDpy, xreg1);
    XFixesDestroyRegion(XDpy, xreg2);
}
//---------------------------------------------------------------------------
static void draw_picture_ddb(HBITMAP src, HBITMAP mask, TCanvas *canvas, int x, int y, const TRect &rect)
{
    int	format;
    XRenderPictFormat *pictformat, *pfmask;
    Picture pic1, pic2, pic3;

    format = get_std_pict_format(canvas->Depth);
    pictformat = XRenderFindStandardFormat(XDpy, format);
    pfmask = XRenderFindStandardFormat(XDpy, PictStandardA8);

    /*	unsigned long	valuemask;				//  XRender
    	XRenderPictureAttributes pa;
    	valuemask = CPClipMask;// | CPClipXOrigin | CPClipYOrigin;
    	pa.clip_mask = None;
    //	pa.clip_x_origin = m_ClipOrigin.x + m_Origin.x;
    //	pa.clip_y_origin = m_ClipOrigin.y + m_Origin.y;
    	XRenderChangePicture(Application.Handle, m_Picture, valuemask, &pa); */

    pic1 = XRenderCreatePicture(Application.Handle, mask, pfmask, 0, NULL);
    pic2 = XRenderCreatePicture(Application.Handle, src, pictformat, 0, NULL);
    pic3 = XRenderCreatePicture(Application.Handle, canvas->Handle, pictformat, 0, NULL);

    XserverRegion xreg = XFixesCreateRegionFromGC(XDpy, canvas->Pen->Handle);   // 1-variant
    XFixesSetPictureClipRegion(XDpy, pic3, 0, 0, xreg);
    XFixesDestroyRegion(XDpy, xreg);

//TRegion rgn = canvas->ClipMask;                             // 2 variant
//rgn.Offset(canvas->Origin.x, canvas->Origin.y);
//XRenderSetPictureClipRegion(XDpy, pic3, rgn.Handle);

    int	op = PictOpOver;
    XRenderComposite(Application.Handle, op, pic2, pic1, pic3,
                     rect.left, rect.top, 0, 0, x, y, rect.width, rect.height);

    XRenderFreePicture(Application.Handle, pic1);
    XRenderFreePicture(Application.Handle, pic2);
    XRenderFreePicture(Application.Handle, pic3);
}
//---------------------------------------------------------------------------
static void draw_picture_dib(HDIB src, HDIB mask, TCanvas *canvas, int x, int y, const TRect &rect)
{
    HBITMAP sbmp, mbmp;
    sbmp = dib2ddb(src);
    mbmp = dib2ddb(mask);
    draw_picture_ddb(sbmp, mbmp, canvas, x, y, rect);
    XFreePixmap(XDpy, sbmp);
    XFreePixmap(XDpy, mbmp);
}
//---------------------------------------------------------------------------
static void draw_dib(HDIB src, HDIB mask, TCanvas *canvas, int x, int y, const TRect &rect, RGBQUAD *colormap)
{
    HDIB tmpout;
    HBITMAP bmp;
    GC gc;

    if(mask)
    {
        if(mask->depth == pf1bit)
        {
            bmp = XCreatePixmap(XDpy, canvas->Handle, rect.width, rect.height, mask->depth);
            gc = XCreateGC(XDpy, bmp, 0, NULL);
//            XSetForeground(XDpy, gc, 1);
//            XFillRectangle(XDpy, bmp, gc, 0, 0, rect.width, rect.height);
            XPutImage(XDpy, bmp, gc, mask, rect.left, rect.top, 0, 0, rect.width, rect.height);
            XFreeGC(XDpy, gc);

            if((*canvas->ClipMask).IsEmpty())
            {
                XSetClipOrigin(XDpy, canvas->Pen->Handle, x, y);
                XSetClipMask(XDpy, canvas->Pen->Handle, bmp);
            }
            else
                combine_mask(canvas, bmp, x, y);
            XFreePixmap(XDpy, bmp);
        }
        else   //   mask->depth == pf8bit
        {
            if(canvas->Depth == src->depth)
                draw_picture_dib(src, mask, canvas, x, y, rect);
            else
            {
                tmpout = convert_depth(src, canvas->Depth, colormap);
                draw_picture_dib(tmpout, mask, canvas, x, y, rect);
                XDestroyImage(tmpout);
            }
            return;
        }
    };
    if(canvas->Depth == src->depth)
    {
        XPutImage(XDpy, canvas->Handle, canvas->Pen->Handle, src,
                  rect.left, rect.top, x, y, rect.width, rect.height);
    }
    else
    {
        if(src->depth == pf1bit)
        {
            bmp = XCreatePixmap(XDpy, canvas->Handle,
                                rect.width, rect.height, pf1bit);
            gc = XCreateGC(XDpy, bmp, 0, NULL);
            XPutImage(XDpy, bmp, gc, src, rect.left, rect.top, 0, 0, rect.width, rect.height);
            XFreeGC(XDpy, gc);
            XCopyPlane(XDpy, bmp, canvas->Handle,
                       canvas->Pen->Handle, rect.left, rect.top, rect.width, rect.height,
                       x, y, pf1bit);
            XFreePixmap(XDpy, bmp);
        }
        else
        {
            tmpout = convert_depth(src, canvas->Depth, colormap);
            XPutImage(XDpy, canvas->Handle, canvas->Pen->Handle, tmpout,
                      rect.left, rect.top, x, y, rect.width, rect.height);
            XDestroyImage(tmpout);
        }
    };
        if(mask)
        {
     //    	XSetClipOrigin(XDpy, canvas->Pen->Handle, 0, 0);
    //    	XSetClipMask(XDpy, canvas->Pen->Handle, None);
        	XCopyGC(XDpy, canvas->Brush->Handle, GCClipMask, canvas->Pen->Handle);
        }
}
//---------------------------------------------------------------------------
TBitmap :: TBitmap()
{
    Init();
}
//---------------------------------------------------------------------------
__fastcall TBitmap :: TBitmap(TBitmap &obj)
{
    /*  	m_Data = NULL;
      	m_Handle = 0;
      	m_MaskData = NULL;
      	m_MaskHandle = 0;
      	m_Canvas = NULL;
      	m_Colormap = NULL;
      	m_TransparentColor = clBlack;
    *this = obj;
    */
    Assign(obj);
}
//---------------------------------------------------------------------------
__fastcall TBitmap :: TBitmap(uint_t w, uint_t h)
{
    Create(w, h);
}
//---------------------------------------------------------------------------
TBitmap :: ~TBitmap()
{
    Release();
}
//---------------------------------------------------------------------------
void TBitmap :: Init()
{
    m_Data = NULL;
    m_Handle = 0;
    m_MaskData = NULL;
    m_MaskHandle = 0;
    m_Canvas = NULL;
    m_Colormap = NULL;
    m_TransparentColor = clBlack;
}
//---------------------------------------------------------------------------
//void TBitmap :: Assign(TObject &obj)
__fastcall void TBitmap :: Assign(const TBitmap &bmp)
{
//	TBitmap &bmp = dynamic_cast<TBitmap &>(obj);
//	if(&bmp == 0)
//		return;
    if(!Empty)
        Release();
    switch (bmp.HandleType)
    {
    case bmDIB:
        LoadImage(bmp.Data, bmp.MaskData);
        break;
    case bmDDB:
        LoadImage(bmp.Handle, bmp.MaskHandle);
        break;
    case bmEmpty:
        break;
    }
    RGBQUAD *cm = const_cast<TBitmap &>(bmp).GetColormap();
    if(cm)
    {
        m_Colormap = new RGBQUAD[NCOLORS(m_PixelFormat.Depth)];
        memcpy(m_Colormap, cm, NCOLORS(m_PixelFormat.Depth) * sizeof(RGBQUAD));
    }
}
//---------------------------------------------------------------------------
__fastcall const TBitmap & TBitmap :: operator = ( const TBitmap &bmp )
{
    if(this == &bmp)
        return *this;
    TGraphic :: operator=(bmp);
    Assign(bmp);
    return *this;
}
//---------------------------------------------------------------------------
bool TBitmap :: get_Empty()
{
    return (m_Data == NULL) && (m_Handle == 0);
}
//---------------------------------------------------------------------------
void TBitmap :: set_TransparentColor(TColor value)
{
    if(m_TransparentColor == value)
        return;
    Mask(value);
    m_TransparentColor = value;
}
//---------------------------------------------------------------------------
TBitmapHandleType TBitmap :: get_HandleType()
{
    TBitmapHandleType ret = bmEmpty;

    if(m_Handle)
        ret = bmDDB;
    else if(m_Data)
        ret = bmDIB;
    return ret;
}
//---------------------------------------------------------------------------
void TBitmap :: set_HandleType(TBitmapHandleType value)
{
    if(HandleType == value)
        return;
    switch (value)
    {
    case bmDIB:
        convert_to_dib();
        break;
    case bmDDB:
        convert_to_ddb();
        break;
    case bmEmpty:
        Release();
        break;
    }
}
//---------------------------------------------------------------------------
void TBitmap :: set_PixelFormat(TPixelFormat value)
{
    if(m_PixelFormat == value)
        return;

    HDIB tmp;
    if(m_PixelFormat.Depth != value.Depth || m_PixelFormat.ColorMode != value.ColorMode)
    {
        switch (HandleType)
        {
        case bmDDB:
            convert_to_dib();
        case bmDIB:
            tmp = convert_depth(m_Data, value.Depth, m_Colormap);
            XDestroyImage(m_Data);
            m_Data = tmp;
            if(value.Depth != pf4bit && value.Depth != pf8bit)
            {
                delete [] m_Colormap;
                m_Colormap = NULL;
            }
            break;
        case bmEmpty:
            break;
        }
    }
    if(m_PixelFormat.ByteOrder != value.ByteOrder ||
            m_PixelFormat.BitOrder != value.BitOrder)
    {
        if(HandleType == bmDIB)
            convert_to_ddb();
    }
    m_PixelFormat = value;
}
//---------------------------------------------------------------------------
byte_t * TBitmap :: get_ScanLine(int n)
{
    HandleType = bmDIB;
    if(n > m_Data->height - 1)
        n = m_Data->height - 1;
    return (byte_t *)(m_Data->data + n * m_Data->bytes_per_line);
}
//---------------------------------------------------------------------------
void TBitmap :: convert_to_dib()
{
    if(m_Handle == 0)
        return;
    m_Data = create_dib_handle(m_Width, m_Height, m_PixelFormat);
    XGetSubImage(XDpy, m_Handle, 0, 0, m_Width, m_Height,
                 AllPlanes, ZPixmap, m_Data, 0, 0);
    delete m_Canvas;
    m_Canvas = NULL;
    XFreePixmap(XDpy, m_Handle);
    m_Handle = 0;
    if(m_MaskHandle)
    {
        m_MaskData = XGetImage(XDpy, m_MaskHandle, 0, 0,
                               m_Width, m_Height, AllPlanes, ZPixmap);
        XFreePixmap(XDpy, m_MaskHandle);
        m_MaskHandle = 0;
    }
}
//---------------------------------------------------------------------------
void TBitmap :: convert_to_ddb()
{
    if(m_Data)
    {
        m_Handle = dib2ddb(m_Data);
        XDestroyImage(m_Data);
        m_Data = NULL;
    }
    else
    {
        m_Handle = XCreatePixmap(XDpy, Screen.Root,
                                 m_Width, m_Height, m_PixelFormat.Depth);
    }
    if(m_MaskData)
    {
        m_MaskHandle = dib2ddb(m_MaskData);
        XDestroyImage(m_MaskData);
        m_MaskData = NULL;
    }

    m_Canvas = new TCanvas(this);
    m_Canvas->Brush->Color = m_TransparentColor;
}
//---------------------------------------------------------------------------
void TBitmap :: Create(uint_t w, uint_t h, TBitmapHandleType type, const TPixelFormat *pf, const TPixelFormat *pfmask)
{
    if(!Empty)
        Release();
    if(pf == NULL)
        m_PixelFormat = TPixelFormat();
    else
        m_PixelFormat = *pf;

    switch(type)
    {
    case bmDDB:
        m_Handle = XCreatePixmap(XDpy, Screen.Root, w, h, m_PixelFormat.Depth);
        if(pfmask)
            m_MaskHandle = XCreatePixmap(XDpy, Screen.Root, w, h, pfmask->Depth);
        m_Canvas = new TCanvas(this);
        break;
    case bmDIB:
        m_Data = create_dib_handle(w, h, m_PixelFormat);
        if(pfmask)
            m_MaskData = create_dib_handle(w, h, *pfmask);
        break;
    case bmEmpty:
        w = h = 0;
        break;
    }
    m_Width = w;
    m_Height = h;
}
//---------------------------------------------------------------------------
void TBitmap :: Load(const TString &filename)
{
    TString ext = filename.substr(filename.rfind('.') + 1);
    TBitmap *obj = (TBitmap *)TClass::Create(ext);
    obj->Load(filename);
//	Assign(*obj);
    *this = *obj;
    delete obj;
}
//---------------------------------------------------------------------------
void TBitmap :: Save(const TString &filename)
{
    TString ext = filename.substr(filename.rfind('.') + 1);
    TBitmap *obj = (TBitmap *)TClass::Create(ext);
//	obj->Assign(*this);
    *obj = *this;
    obj->Save(filename);
    delete obj;
}
//---------------------------------------------------------------------------
void TBitmap :: LoadImage(HDC hdc, HBITMAP hmask)
{
    GC gc;

    if(!Empty)
        Release();
    if(hdc == 0)
        return;
    Window root;
    int x, y;
    unsigned int width, height, border_width, depth;
    if(XGetGeometry(XDpy, hdc, &root, &x, &y, &width,
                    &height, &border_width, &depth))
    {
        m_Handle = XCreatePixmap(XDpy, hdc, width, height, depth);
        gc = XCreateGC(XDpy, m_Handle, 0, NULL);
        XCopyArea(XDpy, hdc, m_Handle, gc,
                  0, 0, width, height, 0, 0);
        XFreeGC(XDpy, gc);
        m_PixelFormat.Depth = (TPixelSize)depth;
        m_Width = width;
        m_Height = height;
        m_Canvas = new TCanvas(this);
    }

    if(hmask == 0)
        return;
    if(XGetGeometry(XDpy, hmask, &root, &x, &y, &width,
                    &height, &border_width, &depth))
    {
        m_MaskHandle = XCreatePixmap(XDpy, hmask,
                                     width, height, depth);
        gc = XCreateGC(XDpy, m_MaskHandle, 0, NULL);
        XCopyArea(XDpy, hmask, m_MaskHandle, gc,
                  0, 0, width, height, 0, 0);
        XFreeGC(XDpy, gc);
    }
}
//---------------------------------------------------------------------------
void TBitmap :: LoadImage(HDIB hbmp, HDIB hmask)
{
    if(!Empty)
        Release();
    if(hbmp == 0)
        return;
    m_Data = XSubImage(hbmp, 0, 0, hbmp->width, hbmp->height);
    m_Width = hbmp->width;
    m_Height = hbmp->height;
    m_PixelFormat = getpf_dib(hbmp);

    if(hmask == 0)
        return;
    m_MaskData = XSubImage(hmask, 0, 0, hmask->width, hmask->height);
}
//---------------------------------------------------------------------------
void TBitmap :: SetColormap(TRGB *cmap, int ncolor)
{
    if(m_Colormap)
        delete m_Colormap;
    int colorcount = NCOLORS(m_PixelFormat.Depth);
    if(colorcount > 256)
        colorcount = 256;
    if(ncolor > colorcount)
        ncolor = colorcount;

    m_Colormap = new RGBQUAD[colorcount];
    byte_t *p, *q;
    p = (byte_t *)m_Colormap;
    q = (byte_t *)cmap;
    for(register int i = 0; i < ncolor; i++)
    {
        memcpy(p, q, sizeof(TRGB));
        p += sizeof(RGBQUAD);
        q += sizeof(TRGB);
    }
}
//---------------------------------------------------------------------------
void TBitmap :: Rotate(int angle)
{
    HDIB hdib;
    bool resize = (ResizeMode == rmCrop) ? false : true;
    TColor fill = m_TransparentColor;

    switch (HandleType)
    {
    case bmDDB:
        convert_to_dib();
    case bmDIB:
        hdib = rotate_dib(m_Data, angle, resize, fill);
        XDestroyImage(m_Data);
        m_Data = hdib;
        if(m_MaskData)
        {
            hdib = rotate_dib(m_MaskData, angle, resize, fill);
            XDestroyImage(m_MaskData);
            m_MaskData = hdib;
        }
        m_Width = hdib->width;
        m_Height = hdib->height;
        break;
    case bmEmpty:
        break;
    }
}
//---------------------------------------------------------------------------
void TBitmap :: SetSize(uint_t w, uint_t h)
{
    HBITMAP htmp, bmp;
    HDIB hdib;
    uint_t tw, th;
    HGC gc;

    if(ResizeMode == rmStretch)
    {
        switch (HandleType)
        {
        case bmDDB:
            convert_to_dib();
        case bmDIB:
            hdib = stretch_dib(m_Data, w, h);
            XDestroyImage(m_Data);
            m_Data = hdib;
            if(m_MaskData)
            {
                hdib = stretch_dib(m_MaskData, w, h);
                XDestroyImage(m_MaskData);
                m_MaskData = hdib;
            }
            m_Width = w;
            m_Height = h;
            break;
        case bmEmpty:
            Create(w, h);
            break;
        }
    }
    else
    {
        tw = (w > m_Width)? m_Width : w;
        th = (h > m_Height)? m_Height : h;
        switch (HandleType)
        {
        case bmDIB:
            convert_to_ddb();
        case bmDDB:
            htmp = XCreatePixmap(XDpy, m_Handle, w, h, m_PixelFormat.Depth);
            //     	XFillRectangle(XDpy, htmp, m_Canvas->Brush->Handle, 0, 0, w, h);
            XFillRectangle(XDpy, htmp, m_Canvas->Pen->Handle, 0, 0, w, h);
            XCopyArea(XDpy, m_Handle, htmp, m_Canvas->Pen->Handle,
                      0, 0, tw, th, 0, 0);
            XFreePixmap(XDpy, m_Handle);
            m_Handle = htmp;

            if(m_MaskHandle)
            {
                Window root;
                int x, y;
                unsigned int width, height, border_width, depth;
                XGetGeometry(XDpy, m_MaskHandle, &root, &x, &y, &width,
                             &height, &border_width, &depth);
                bmp = XCreatePixmap(XDpy, m_MaskHandle,	w, h, depth);
                gc = XCreateGC(XDpy, bmp, 0, NULL);
                XFillRectangle(XDpy, bmp, gc, 0, 0, w, h);
                XCopyArea(XDpy, m_MaskHandle, bmp, gc, 0, 0, tw, th, 0, 0);
                XFreeGC(XDpy, gc);
                XFreePixmap(XDpy, m_MaskHandle);
                m_MaskHandle = bmp;
            }
            m_Width = w;
            m_Height = h;

            delete m_Canvas;
            m_Canvas = new TCanvas(this);

            break;
        case bmEmpty:
            Create(w, h);
            break;
        }
    }
}
//---------------------------------------------------------------------------
void TBitmap :: StretchDraw(TCanvas *canvas, const TRect &rect)
{
    HDIB hdib, hmask = NULL;

    switch (HandleType)
    {
    case bmDDB:
        convert_to_dib();
    case bmDIB:
        if(m_MaskData)
            hmask = stretch_dib(m_MaskData, rect.width, rect.height);
        hdib = stretch_dib(m_Data, rect.width, rect.height);
        draw_dib(hdib, hmask, canvas, canvas->Origin.x + rect.left, canvas->Origin.y + rect.top, TRect(0, 0, rect.width, rect.height), m_Colormap);
//			draw_dib(hdib, hmask, canvas, canvas->Origin.x, canvas->Origin.y, rect, m_Colormap);
        XDestroyImage(hdib);
        if(hmask)
            XDestroyImage(hmask);
        break;
    case bmEmpty:
        break;
    }
}
//---------------------------------------------------------------------------
void TBitmap :: Release()
{
    if(m_Canvas)
        delete m_Canvas;
    if(m_Data)
        XDestroyImage(m_Data);
    if(m_MaskData)
        XDestroyImage(m_MaskData);
    if(m_Handle)
        XFreePixmap(XDpy, m_Handle);
    if(m_MaskHandle)
        XFreePixmap(XDpy, m_MaskHandle);
    if(m_Colormap)
        delete [] m_Colormap;
    m_Colormap = NULL;
    m_Width = m_Height = 0;
    m_Data = NULL;
    m_Handle = 0;
    m_MaskData = NULL;
    m_MaskHandle = 0;
    m_Canvas = NULL;
    m_PixelFormat = TPixelFormat();
    HotSpot.x = HotSpot.y = -1;
    m_TransparentColor = clBlack;
}
//---------------------------------------------------------------------------
void TBitmap :: ReleaseMask()
{
    if(m_MaskData)
        XDestroyImage(m_MaskData);
    if(m_MaskHandle)
    {
        m_Canvas->RemoveClipMask();
        XFreePixmap(XDpy, m_MaskHandle);
    }
    m_MaskData = NULL;
    m_MaskHandle = 0;
}
//---------------------------------------------------------------------------
void TBitmap :: Draw(TCanvas *canvas, int x, int y, const TRect &rect)
{
    HDIB tmpin, tmpout;

    switch(HandleType)
    {
    case bmDIB:
        draw_dib(m_Data, m_MaskData, canvas, x, y, rect, m_Colormap);
        break;
    case bmDDB:
        if(m_MaskHandle)
        {
            Window root;
            int x1, y1;
            unsigned int width, height, border_width, depth;
            XGetGeometry(XDpy, m_MaskHandle, &root, &x1, &y1, &width,
                         &height, &border_width, &depth);
            if(depth == pf1bit)
            {
                if((*canvas->ClipMask).IsEmpty())
                {
                    XSetClipOrigin(XDpy, canvas->Pen->Handle, x, y);
                    XSetClipMask(XDpy, canvas->Pen->Handle, m_MaskHandle);
                }
                else
                    combine_mask(canvas, m_MaskHandle, x, y);

//                    XSetClipOrigin(XDpy, canvas->Pen->Handle, x - rect.left, y - rect.top);
//                    XSetClipOrigin(XDpy, canvas->Pen->Handle, x, y);
//                    XSetClipMask(XDpy, canvas->Pen->Handle, m_MaskHandle);
            }
            else
            {
                draw_picture_ddb(m_Handle, m_MaskHandle, canvas, x, y, rect);
                break;
            }
        }
        if(canvas->Depth == m_PixelFormat.Depth)
        {
            XCopyArea(XDpy, m_Handle, canvas->Handle, canvas->Pen->Handle,
                      rect.left, rect.top, rect.width, rect.height, x, y);
        }
        else
        {
            if(m_PixelFormat.Depth == pf1bit)
                XCopyPlane(XDpy, m_Handle, canvas->Handle, canvas->Pen->Handle,
                           rect.left, rect.top, rect.width, rect.height, x, y, pf1bit);
            else
            {
                tmpin = create_dib_handle(m_Width, m_Height, m_PixelFormat);
                XGetSubImage(XDpy, m_Handle, 0, 0, m_Width, m_Height,
                             AllPlanes, ZPixmap, tmpin, 0, 0);
                tmpout = convert_depth(tmpin, canvas->Depth, m_Colormap);
                XPutImage(XDpy, canvas->Handle, canvas->Pen->Handle, tmpout,
                          rect.left, rect.top, x, y, rect.width, rect.height);
                XDestroyImage(tmpout);
                XDestroyImage(tmpin);
            }
        }
//			if(m_MaskHandle)
//			{
//                XSetClipOrigin(XDpy, canvas->Pen->Handle, 0, 0);
//                XSetClipMask(XDpy, canvas->Pen->Handle, None);
//     		}
        break;
    case bmEmpty:
        break;
    }
}
//---------------------------------------------------------------------------
void TBitmap :: Clear()
{
    switch (HandleType)
    {
    case bmDIB:
        memset(m_Data->data, 0, m_Data->bytes_per_line * m_Data->height);
        break;
    case bmDDB:
        m_Canvas->FillRect(0, 0, m_Width, m_Height);
        break;
    case bmEmpty:
        break;
    }
}
//---------------------------------------------------------------------------
TColor TBitmap :: Pixels(int x, int y)
{
    TColor ret;

    switch (HandleType)
    {
    case bmDIB:
        ret = XGetPixel(m_Data, x, y);
        break;
    case bmDDB:
        ret = m_Canvas->Pixels(x, y);
        break;
    case bmEmpty:
        ret = clNone;
        break;
    }
    return ret;
}
//---------------------------------------------------------------------------
void TBitmap :: Pixels(int x, int y, TColor color)
{
    switch (HandleType)
    {
    case bmDIB:
        XPutPixel(m_Data, x, y, color);
        break;
    case bmDDB:
        m_Canvas->Pixels(x, y, color);
        break;
    case bmEmpty:
        break;
    }
}
//---------------------------------------------------------------------------
void TBitmap :: Mask(TColor maskcolor, uint_t threshold)
{
    if(HandleType == bmEmpty)
        return;
    HandleType = bmDIB;
    if(m_MaskData == NULL)
    {
        TPixelFormat a(pf1bit);
//		a.Depth = pf1bit;
//		a.ByteOrder = MSBFirst;
//		a.BitOrder = MSBFirst;
        m_MaskData = create_dib_handle(m_Width, m_Height, a);
    }
    else
        memset(m_MaskData->data, 0, m_MaskData->bytes_per_line * m_MaskData->height);

    TColor tmp, mcolor = 0;
    uint_t ui;
    if(m_Colormap)
    {
        threshold = 0;
        for(register int i = 0; i < (2 << m_PixelFormat.Depth); i++)
        {
            memcpy(&ui, &m_Colormap[i], sizeof(RGBQUAD));
            if(ui == maskcolor)
            {
                mcolor = TColor(i);
                break;
            }
        }
    }
    else
    {
        if(m_PixelFormat.Depth == pf8bit)
            mcolor = maskcolor & 0xFF;
        else
            mcolor = maskcolor;
    }

    if(threshold > 0)
    {
        uint_t rmin, rmax;
        uint_t gmin, gmax;
        uint_t bmin, bmax;
        rmin = TRIMVAL( R(maskcolor) - threshold, 0, 0xFF);
        rmax = TRIMVAL( R(maskcolor) + threshold, 0, 0xFF);
        gmin = TRIMVAL( G(maskcolor) - threshold, 0, 0xFF);
        gmax = TRIMVAL( G(maskcolor) + threshold, 0, 0xFF);
        bmin = TRIMVAL( B(maskcolor) - threshold, 0, 0xFF);
        bmax = TRIMVAL( G(maskcolor) + threshold, 0, 0xFF);
        for(register uint_t j = 0; j < m_Height; j++)
            for(register uint_t i = 0; i < m_Width; i++)
            {
                tmp = XGetPixel(m_Data, i, j);
                if(!(INRANGE( R(tmp), rmin, rmax) &&
                        INRANGE( G(tmp), gmin, gmax) &&
                        INRANGE( B(tmp), bmin, bmax)))
                    XPutPixel(m_MaskData, i, j, 1);
            }
    }
    else
    {
        for(register uint_t j = 0; j < m_Height; j++)
            for(register uint_t i = 0; i < m_Width; i++)
            {
                tmp = XGetPixel(m_Data, i, j);
                if(tmp != mcolor)
                    XPutPixel(m_MaskData, i, j, 1);
            }
    }
}
//---------------------------------------------------------------------------
void TBitmap :: Mirror(TMirrorType value)
{
    switch (HandleType)
    {
    case bmDDB:
        convert_to_dib();
    case bmDIB:
        if(value == mtMirrorVert)
            mirror_vert_dib(m_Data);
        else
            mirror_horz_dib(m_Data);
        if(m_MaskData)
        {
            if(value == mtMirrorVert)
                mirror_vert_dib(m_MaskData);
            else
                mirror_horz_dib(m_MaskData);
        }
        break;
    case bmEmpty:
        break;
    }
}
//---------------------------------------------------------------------------
//  TImageList
//---------------------------------------------------------------------------
TImageList :: TImageList()
{
    ImageHeight = 16;
    ImageWidth = 16;
}
//---------------------------------------------------------------------------
void TImageList :: Draw(TCanvas* canvas, int x, int y, int index)
{
    int ix, iy, i, j;
    i = index % Cols;
    j = index / Cols;
    ix = i * ImageWidth;
    iy = j * ImageHeight;
    TRect rect(ix, iy, ImageWidth, ImageHeight);
    TBitmap :: Draw(canvas, x, y, rect);
}
//---------------------------------------------------------------------------
void TImageList :: DrawRect(TCanvas *canvas, const TRect &rect, int x, int y, int index, TAlign flags)
{
    TRect tmp, r(0, 0, ImageWidth, ImageHeight), r1;
    int ox, oy;

    r1 = rect;
    tmp = r1.align(r, flags);
    ox = x + tmp.left;
    oy = y + tmp.top;
    Draw(canvas, ox, oy, index);
}
//---------------------------------------------------------------------------
int __fastcall TImageList :: GetIndex(uint_t col, uint_t row)
{
    int ret;
    ret = col + (row * Cols);
    if(((uint_t)ret >= Cols * Rows) || Empty)
        ret = -1;
    return ret;
}
//---------------------------------------------------------------------------
void __fastcall TImageList :: GetBitmap(int index, TBitmap* image)
{
    image->Clear();
    image->Create(ImageWidth, ImageHeight, HandleType, &m_PixelFormat);
    TImageList :: Draw(image->Canvas, 0, 0, index);
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
