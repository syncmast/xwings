//---------------------------------------------------------------------------
#ifndef graphicsH
#define graphicsH

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/Xrender.h>
#include "tobj.h"
//---------------------------------------------------------------------------
typedef Display* 	   		HDISPLAY;
typedef Screen*      		HSCREEN;
typedef Screen*      		HMONITOR;
typedef Region       		HREGION;
typedef GC			   		HGC;
typedef GC			   		HBRUSH;
typedef GC			   		HPEN;
typedef Drawable	   		HDC;
typedef Window		   		HWND;
typedef Pixmap		   		HBITMAP;
typedef XImage*      		HDIB;
typedef Cursor       		HCURSOR;
typedef Colormap     		HCOLORMAP;
typedef XFontStruct* 		HFONT;
typedef Visual*      		HVISUAL;
typedef Picture				HPICTURE;
typedef unsigned int       	TColor;

typedef Screen XScreen;
static const int XKeyPress = KeyPress;
static const uint_t XButton1 = Button1;
static const uint_t XButton2 = Button2;
static const uint_t XButton3 = Button3;
static const uint_t XButton4 = Button4;
static const uint_t XButton5 = Button5;
#undef  KeyPress
#undef  Button1
#undef  Button2
#undef  Button3
#undef  Button4
#undef  Button5

static const TColor clScrollBar = 0x00dfdfcf;
static const TColor clBackground = 0x00a56e3a;
static const TColor clActiveCaption = 0x000a246a;
static const TColor clInactiveCaption = 0x00808080;
static const TColor clMenu = 0x00c8d0d4;
static const TColor clWindow = 0x00ffffff;
static const TColor clWindowFrame = 0x0;
static const TColor clMenuText = 0x0;
static const TColor clWindowText = 0x0;
static const TColor clCaptionText = 0x00ffffff;
static const TColor clActiveBorder = 0x00c8d0d4;
static const TColor clInactiveBorder = 0x00c8d0d4;
static const TColor clAppWorkSpace = 0x00808080;
static const TColor clHighlight = 0x000a246a;
static const TColor clHighlightText = 0x00ffffff;
static const TColor clBtnFace = 0x00d4d0c8;
static const TColor clBtnShadow = 0x00808080;
static const TColor clGrayText = 0x00808080;
static const TColor clBtnText = 0x0;
static const TColor clInactiveCaptionText = 0x00c8d0d4;
static const TColor clBtnHighlight = 0x00ffffff;
static const TColor cl3DDkShadow = 0x00404040;
static const TColor cl3DLight = 0x00c8d0d4;
static const TColor clInfoText = 0x0;
static const TColor clInfoBk = 0x00ffffe1;
static const TColor clHotLight = 0x00800000;
static const TColor clGradientActiveCaption = 0x00f0caa6;
static const TColor clGradientInactiveCaption = 0x00c0c0c0;
static const TColor clMenuHighlight = 0x00316ac5;
static const TColor clMenuBar = 0x00ece9d8;
static const TColor clCursorForeground = 0x00ffffff;
static const TColor clCursorBackground = 0x0;

static const TColor clDark = 0x0;
static const TColor clMid = 0xa0a0a4;

static const TColor clBlack = 0x0;
static const TColor clMaroon = 0x800000;
static const TColor clGreen = 0x008000;
static const TColor clOlive = 0x808000;
static const TColor clNavy = 0x000080;
static const TColor clPurple = 0x800080;
static const TColor clTeal = 0x008080;
static const TColor clGray = 0x808080;
static const TColor clSilver = 0xc0c0c0;
static const TColor clRed = 0xff0000;
static const TColor clLime = 0x00ff00;
static const TColor clYellow = 0xffff00;
static const TColor clBlue = 0x0000ff;
static const TColor clFuchsia = 0xff00ff;
static const TColor clAqua = 0x00ffff;
static const TColor clLtGray = 0xc0c0c0;
static const TColor clDkGray = 0x808080;
static const TColor clWhite = 0x00ffffff;
static const TColor clMoneyGreen = 0xc0dcc0;
static const TColor clSkyBlue = 0xa6caf0;
static const TColor clCream = 0xfffbf0;
static const TColor clMedGray = 0xa0a0a4;

static const TColor clNone = 0x1fffffff;
static const TColor clDefault = 0x20000000;

#define NCOLORS(depth)  ipower(2, int(depth))
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
struct TPoint : public XPoint
{
public:
    inline TPoint()
    {
        x = y = 0;
    };
    inline TPoint(int px, int py)
    {
        x = (short)px ;
        y = (short)py;
    };
};

TPoint __fastcall Point(int x, int y);

enum TAlign
{
    alNone, alClient,
    alLeft, alTop, alRight, alBottom,
    alLeftTop, alCenterTop, alRightTop,
    alLeftCenter, alCenter, alRightCenter,
    alLeftBottom, alCenterBottom, alRightBottom,
    alHCenter, alVCenter, alWidth, alHeight
};
//---------------------------------------------------------------------------
//  TRect
//---------------------------------------------------------------------------
struct TRect
{
    int left, top;
    uint_t width, height, rx, ry;

    TRect()
    {
        erase();
    }
    __fastcall TRect(const TPoint& TL, const TPoint& BR)
    {
        left=TL.x;
        top=TL.y;
        width = BR.x - TL.x + 1;
        height = BR.y - TL.y + 1;
    }
    __fastcall TRect(int l, int t, uint_t w, uint_t h)
    {
        left=l;
        top=t;
        width=w;
        height=h;
        rx=ry=0;
    }
    __fastcall TRect(int l, int t, uint_t w, uint_t h, uint_t arx, uint_t ary)
    {
        left=l;
        top=t;
        width=w;
        height=h;
        rx=arx;
        ry=ary;
    }
    __fastcall TRect(int l, int t, uint_t w, uint_t h, uint_t rad)
    {
        left=l;
        top=t;
        width=w;
        height=h;
        rx = rad;
        ry = rad;
    }
    __fastcall TRect(XRectangle &xrect)
    {
        left = xrect.x;
        top = xrect.y;
        width = xrect.width;
        height = xrect.height;
        rx = ry = 0;
    }

    int right () const
    {
        return left + width - 1;
    }
    int bottom() const
    {
        return top + height - 1;
    }
    void __fastcall round(uint_t arx, uint_t ary)
    {
        rx = arx;
        ry = ary;
    }
    void __fastcall round(uint_t rad)
    {
        rx = rad;
        ry = rad;
    }
    TRect & __fastcall inflate(int dx, int dy)
    {
        top -= dy;
        left -= dx;
        width += (dx << 1);
        height += (dy << 1);
        if(rx > 0) rx += dx;
        if(ry > 0) ry += dy;
        return *this;
    }
    void __fastcall offset(int dx, int dy)
    {
        left += dx;
        top += dy;
    }
    TRect __fastcall align(const TRect &rect, TAlign value);
    TPoint center();
    TRect & __fastcall bounds(int l, int t, int r, int b)
    {
        left = l;
        top = t;
        width = (r >= l) ? r - l + 1 : 0;
        height = (b >= t) ? b - t + 1 : 0;
        return *this;
    }
    void erase()
    {
        top = 0;
        height = 0;
        left = 0;
        width = 0;
        rx = 0;
        ry = 0;
    }
    bool isempty()
    {
        return (width == 0 || height == 0);
    }
    bool __fastcall inside(int x, int y)
    {
        return (x >= left) && (x <= right()) && (y >= top) && (y <= bottom());
    }
    bool __fastcall inside(const TPoint &p)
    {
        return inside(p.x, p.y);
    }
    bool __fastcall inside(const TRect &r);
    bool __fastcall intersect(TRect &r2, TRect &rect);
    bool __fastcall unionrect(TRect &r2, TRect &rect);
    XRectangle xrect()
    {
        XRectangle xr;
        xr.x = left;
        xr.y = top;
        xr.width = width;
        xr.height = height;
        return xr;
    }
    bool __fastcall operator ==(const TRect& rc) const
    {
        return left ==  rc.left  && top == rc.top &&
               width == rc.width && height == rc.height &&
               rx == rc.rx && ry == rc.ry;
    }
    bool __fastcall operator !=(const TRect& rc) const
    {
        return !(rc==*this);
    }
};

inline void OffsetRect(TRect &rect, int dx, int dy) { rect.offset(dx, dy); };
//---------------------------------------------------------------------------
//  TEllipse
//---------------------------------------------------------------------------
struct TEllipse
{
    int cx, cy;
    uint_t a, b;

    TEllipse()
    {
        erase();
    }
    __fastcall TEllipse(int _cx, int _cy, uint_t _a, uint_t _b)
    {
        cx = _cx;
        cy = _cy;
        a = _a;
        b = _b;
    }
    __fastcall TEllipse(const TPoint& TL, const TPoint& BR)
    {
        a = (BR.x - TL.x + 1) / 2;
        b = (BR.y - TL.y + 1) / 2;
        cx = TL.x + a;
        cy = TL.y + b;
    }
    __fastcall TEllipse(int _cx, int _cy, uint_t rad)
    {
        cx = _cx;
        cy = _cy;
        a = b = rad;
    }
    int right () const
    {
        return cx + a;
    }
    int bottom() const
    {
        return cy + b;
    }
    TEllipse & __fastcall inflate(int dx, int dy)
    {
        a += dy;
        b += dx;
        return *this;
    }
    void __fastcall offset(int dx, int dy)
    {
        cx += dx;
        cy += dy;
    }
    TEllipse & __fastcall bounds(int l, int t, int r, int b)
    {
        a = (r >= l) ? (r - l + 1) / 2 : 0;
        b = (b >= t) ? (b - t + 1) / 2 : 0;
        cx = l + a;
        cy = t + b;
        return *this;
    }
    void erase()
    {
        cx = 0;
        cy = 0;
        a = 0;
        b = 0;
    }
    bool isempty()
    {
        return (a == 0 || b == 0);
    }
    bool __fastcall inside(int x, int y);
    bool __fastcall inside(const TPoint &p)
    {
        return inside(p.x, p.y);
    }
    TRect rect()
    {
        TRect xr;
        xr.left = cx - a;
        xr.top = cy - b;
        xr.width = a + a;
        xr.height = b + b;
        return xr;
    }
    bool __fastcall operator ==(const TEllipse &rc) const
    {
        return cx ==  rc.cx  && cy == rc.cy &&
               a == rc.a && b == rc.b;
    }
    bool __fastcall operator !=(const TEllipse &rc) const
    {
        return !(rc==*this);
    }
};
//---------------------------------------------------------------------------
//  TRegion
//---------------------------------------------------------------------------
enum TRegionLocate
{
    InRegion = RectangleIn,
    OutRegion = RectangleOut,
    PartRegion = RectanglePart
};

class TRegion
{
    DECLARE_PROPERTY(TRegion);

private:
    HREGION m_Handle;
    TRect get_ClipRect();
    void  set_ClipRect(TRect r);

public:
    TRegion();
    __fastcall TRegion(const TRegion &r);
    __fastcall TRegion(const TPoint *points, int n);
    __fastcall TRegion(const TRect &r);
    ~TRegion();
    void Clear();
    bool IsEmpty();
    TRegion __fastcall Intersect(const TRegion &r);
    void __fastcall Expand(int dx, int dy);
    void __fastcall Offset(int dx, int dy);
    TRegion __fastcall Subtract(const TRegion &r);
    TRegion __fastcall Xor(const TRegion &r);
    TRegion __fastcall Union(const TRegion &r);
    bool __fastcall InRegion(int x, int y);
    inline bool __fastcall InRegion(const TPoint &p)
    {
        return InRegion(p.x, p.y);
    };
    TRegionLocate __fastcall InRegion(const TRect &r);
    TRegion __fastcall const & operator =(const TRegion &r);
    bool __fastcall operator ==(const TRegion &r);
    inline TRegion __fastcall operator+(TRegion &r)
    {
        return Union(r);
    };
    inline TRegion __fastcall operator+(TRegion r)
    {
        return Union(r);
    };
    TRegion __fastcall operator+(const TRect &r);
    void __fastcall operator+=(const TRegion &r);
    void __fastcall operator+=(const TRect &r);
    inline TRegion __fastcall operator-(TRegion &r)
    {
        return Subtract(r);
    };
    inline TRegion __fastcall operator-(TRegion r)
    {
        return Subtract(r);
    };
    void __fastcall operator-=(const TRegion &r);
    void __fastcall operator -=(const TRect &r);

    property (rw, TRect,   ClipRect);
    property (mr, HREGION, Handle);
};

class TBitmap;
class TCanvas;
class TGraphic;
class TWinControl;

void __fastcall SetRegion(const TRegion &reg, const TBitmap &bmp);
void __fastcall SetRegion(const TRegion &reg, const TCanvas &can);
void __fastcall SetRegion(const TRegion &reg, const TWinControl &win, int kind);
//---------------------------------------------------------------------------
// TGraphicsObject
//---------------------------------------------------------------------------
enum TSubwindowMode {
    swmClipByChildren = ClipByChildren,
    swmIncludeInferiors = IncludeInferiors
};

class TGraphicsObject : public TObject
{
    DECLARE_PROPERTY(TGraphicsObject);

private:
//    void Init();
    void __fastcall Assign(const TGraphicsObject &obj);
    void __fastcall CreateHandle(const TCanvas *owner);
    TColor get_Color();
    TSubwindowMode get_SubwindowMode();
    void set_SubwindowMode(TSubwindowMode value);

protected:
    HGC	m_Handle;
    const TCanvas *m_Canvas;
    virtual void set_Color(TColor value);
//    virtual void Changed();

public:
//    TNotifyEvent OnChange;

    TGraphicsObject();
    __fastcall TGraphicsObject(const TCanvas *owner);
    __fastcall TGraphicsObject(const TGraphicsObject &a);
    virtual ~TGraphicsObject() = 0;
    __fastcall const TGraphicsObject & operator=(const TGraphicsObject &a);
    void Apply();

    property (mr, HGC, Handle);
    property (rw, TColor, Color);
    property (rw, TSubwindowMode, SubwindowMode);
};
//---------------------------------------------------------------------------
// TPen
//---------------------------------------------------------------------------
enum TPenMode
{
    pmBlack = GXclear,
    pmMask = GXand,
    pmMaskPenNot = GXandReverse,
    pmCopy = GXcopy,
    pmMaskNotPen = GXandInverted,
    pmNop = GXnoop,
    pmXor = GXxor,
    pmMerge = GXor,
    pmNotMerge = GXnor,
    pmNotXor = GXequiv,
    pmNot = GXinvert,
    pmMergePenNot = GXorReverse,
    pmNotCopy = GXcopyInverted,
    pmMergeNotPen = GXorInverted,
    pmNotMask = GXnand,
    pmWhite = GXset
};

enum TPenStyle { psSolid, psDash, psDot, psDashDot, psDashDotDot, psClear };
enum TCapStyle
{
    csCapNotLast = CapNotLast,
    csCapButt = CapButt,
    csCapRound = CapRound,
    csCapProjecting = CapProjecting
};

enum TJoinStyle
{
    jsJoinMiter = JoinMiter,
    jsJoinRound = JoinRound,
    jsJoinBevel = JoinBevel
};
//---------------------------------------------------------------------------
class TPen : public TGraphicsObject
{
    DECLARE_PROPERTY(TPen);

private:
    TPenStyle m_Style;
    void Init();
    void __fastcall Assign(const TPen &obj);
    void set_Style(TPenStyle value);
    TCapStyle get_CapStyle();
    void set_CapStyle(TCapStyle value);
    TJoinStyle get_JoinStyle();
    void set_JoinStyle(TJoinStyle value);
    TPenMode get_Mode();
    void set_Mode(TPenMode value);
    short get_Width();
    void set_Width(short value);

public:
    TPen();
    __fastcall TPen(const TCanvas *owner);
    __fastcall TPen(const TPen &a);
    virtual ~TPen();
    __fastcall const TPen & operator=(const TPen &a);

    property (rw, TCapStyle, CapStyle);
    property (rw, TJoinStyle, JoinStyle);
    property (rw, TPenMode, Mode);
    property (m, TPenStyle, Style);
    property (rw, short, Width);
};
//---------------------------------------------------------------------------
// TBrush
//---------------------------------------------------------------------------
enum TBrushStyle
{
    bsSolid = FillSolid,
    bsTiled = FillTiled,
    bsStippled = FillStippled,
    bsOpaqueStippled = FillOpaqueStippled,
    bsClear
};
//---------------------------------------------------------------------------
class TBrush : public TGraphicsObject
{
    DECLARE_PROPERTY(TBrush);

private:
    TBitmap *m_Bitmap;
    void Init();
    void __fastcall Assign(const TBrush &obj);
    void set_Bitmap(TBitmap *value);
    TBrushStyle get_Style();
    void set_Style(TBrushStyle value);
    virtual void set_Color(TColor value);

public:
    TBrush();
    __fastcall TBrush(const TCanvas *owner);
    __fastcall TBrush(const TBrush &a);
    virtual ~TBrush();
    __fastcall const TBrush & operator=(const TBrush &a);

    property (m, TBitmap *, Bitmap);
    property (rw, TBrushStyle, Style);
};
//---------------------------------------------------------------------------
// TFont
//---------------------------------------------------------------------------
#define	XLFD_FOUNDRY			0
#define	XLFD_FAMILY_NAME		1
#define	XLFD_WEIGHT_NAME		2
#define	XLFD_SLANT				3
#define	XLFD_SETWIDTH_NAME		4
#define	XLFD_ADD_STYLE_NAME		5
#define	XLFD_PIXEL_SIZE			6
#define	XLFD_POINT_SIZE			7
#define	XLFD_RESOLUTION_X		8
#define	XLFD_RESOLUTION_Y		9
#define	XLFD_SPACING			10
#define	XLFD_AVERAGE_WIDTH		11
#define	XLFD_CHARSET_REGISTRY	12
#define	XLFD_CHARSET_ENCODING	13

#define XLFD_NFIELDS			14
#define	XLFD_CHARSET			15

//    Font String Properties

#define FS_FOUNDRY				"FOUNDRY"
#define FS_FAMILY_NAME			"FAMILY_NAME"
#define FS_WEIGHT_NAME			"WEIGHT_NAME"
#define FS_SLANT				"SLANT"
#define FS_SETWIDTH_NAME		"SETWIDTH_NAME"
#define FS_ADD_STYLE_NAME		"ADD_STYLE_NAME"
#define FS_SPACING				"SPACING"
#define FS_CHARSET_REGISTRY		"CHARSET_REGISTRY"
#define FS_CHARSET_ENCODING		"CHARSET_ENCODING"
#define FS_FONT					"FONT"
#define FS_FACE_NAME			"FACE_NAME"
#define FS_COPYRIGHT			"COPYRIGHT"
#define FS_NOTICE				"NOTICE"
#define FS_FONT_TYPE			"FONT_TYPE"
#define FS_FONT_VERSION			"FONT_VERSION"
#define FS_RASTERIZER_NAME		"RASTERIZER_NAME"
#define FS_RASTERIZER_VERSION	"RASTERIZER_VERSION"

//#define AXIS_NAMES
//#define AXIS_LIMITS
//#define AXIS_TYPES

//    Font Metrics

#define FM_PIXEL_SIZE  			"PIXEL_SIZE"
#define FM_POINT_SIZE			"POINT_SIZE"
#define FM_RESOLUTION_X			"RESOLUTION_X"
#define FM_RESOLUTION_Y			"RESOLUTION_Y"
#define FM_AVERAGE_WIDTH		"AVERAGE_WIDTH"
#define FM_MIN_SPACE			"MIN_SPACE"
#define FM_NORM_SPACE			"NORM_SPACE"
#define FM_MAX_SPACE			"MAX_SPACE"
#define FM_END_SPACE			"END_SPACE"
#define FM_AVG_CAPITAL_WIDTH	"AVG_CAPITAL_WIDTH"
#define FM_AVG_LOWERCASE_WIDTH	"AVG_LOWERCASE_WIDTH"
#define FM_FIGURE_WIDTH			"FIGURE_WIDTH"
#define FM_SUPERSCRIPT_X		"SUPERSCRIPT_X"
#define FM_SUPERSCRIPT_Y		"SUPERSCRIPT_Y"
#define FM_SUBSCRIPT_X			"SUBSCRIPT_X"
#define FM_SUBSCRIPT_Y			"SUBSCRIPT_Y"
#define FM_SUPERSCRIPT_SIZE		"SUPERSCRIPT_SIZE"
#define FM_SUBSCRIPT_SIZE		"SUBSCRIPT_SIZE"
#define FM_SMALL_CAP_SIZE		"SMALL_CAP_SIZE"
#define FM_UNDERLINE_POSITION	"UNDERLINE_POSITION"
#define FM_UNDERLINE_THICKNESS	"UNDERLINE_THICKNESS"
#define FM_STRIKEOUT_ASCENT		"STRIKEOUT_ASCENT"
#define FM_STRIKEOUT_DESCENT	"STRIKEOUT_DESCENT"
#define FM_ITALIC_ANGLE			"ITALIC_ANGLE"
#define FM_CAP_HEIGHT			"CAP_HEIGHT"
#define FM_X_HEIGHT				"X_HEIGHT"
#define FM_RELATIVE_SETWIDTH	"RELATIVE_SETWIDTH"
#define FM_RELATIVE_WEIGHT		"RELATIVE_WEIGHT"
#define FM_WEIGHT				"WEIGHT"
#define FM_DESTINATION			"DESTINATION"
#define FM_RAW_ASCENT			"RAW_ASCENT"
#define FM_RAW_DESCENT			"RAW_DESCENT"
//#define RAW_*
//#define AXIS_NAMES
//#define AXIS_LIMITS
//#define AXIS_TYPES

#define FW_MEDIUM		"medium"
#define FW_BOOK			"book"
#define	FW_SEMILIGHT	"semilight"
#define	FW_LIGHT		"light"
#define FW_BOLD			"bold"
#define	FW_DEMIBOLD		"demibold"
#define	FW_DEMI_BOLD	"demi bold"
#define	FW_DARK			"dark"

#define FSW_NORMAL 			"normal"
#define FSW_CONDENSED		"condensed"
#define FSW_SEMICONDENSED	"semicondensed"
#define FSW_EXPANDED		"expanded"
#define FSW_NARROW			"narrow"

struct TFontInfo
{
    TString Foundry;
    TString FamilyName;
    TString WeightName;
    TString Slant;
    TString SetWidthName;
    TString AddStyleName;
    TString Spacing;
    TString CharsetRegistry;
    TString CharsetEncoding;
    TString Font;
    TString FaceName;
    TString Copyright;
    TString Notice;
    TString FontType;
    TString FontVersion;
    TString RasterizerName;
    TString RasterizerVersion;
};

enum TFontStyle
{
    fsRegular = 0,
    fsBold = 1,
    fsItalic = (1 << 1),
    fsUnderline = (1 << 2),
    fsStrikeOut = (1 << 3)
};

enum TFontSpacing { fsMonospaced, fsProportional, fsCharCell };

enum TFontSlant { fslRoman, fslOblique, fslItalic,
                  fslReverseOblique, fslReverseItalic, fslOther
                };

class TFont : public TGraphicsObject
{
    DECLARE_PROPERTY( TFont );

private:
    HFONT m_system;
    HFONT m_Font;
    int m_Style;
    TPenStyle m_PenStyle;
    void Init();
    void __fastcall Assign(const TFont &obj);
    void set_PenStyle(TPenStyle value);
    inline short get_Ascent() { return m_Font->ascent; };
    inline short get_Descent() { return m_Font->descent; };
    TString get_Charset();
    void set_Charset(TString value);
    inline TString get_FaceName() { return GetProperty(FS_FACE_NAME); };
    inline TString get_FontType() { return GetProperty(FS_FONT_TYPE); };
    TString get_FullName();
    void set_FullName(TString value);
    inline bool get_Default() { return m_Font == m_system; };
    TString get_Foundry();
    void set_Foundry(TString value);
    TString get_Family();
    void set_Family(TString value);
    short get_Resolution();
    short get_Points();
    void set_Points(short value);
    short get_Pixels();
    void set_Pixels(short value);
    TFontSlant get_Slant();
    void set_Slant(TFontSlant value);
    TFontSpacing get_Spacing();
    void set_Spacing(TFontSpacing value);
    TString get_Weight();
    void set_Weight(TString value);
    TString get_Setwidth();
    void set_Setwidth(TString value);
    void set_Style(int value);

    inline short get_Height() { return get_Pixels(); };
    inline void set_Height(short value) { set_Pixels(value); };
    inline short get_Size() { return get_Points(); };
    inline void set_Size(short value) { set_Points(value); };

    void SetName();

protected:
//    virtual void Changed();

public:
    TFont();
    __fastcall TFont(const TCanvas *owner);
    __fastcall TFont(const TFont &a);
    virtual ~TFont();
    __fastcall const TFont & operator=(const TFont &a);

    TString __fastcall ConvertCharset(const TString &text);
    ulong_t __fastcall GetMetric(const char *name);
    TString __fastcall GetProperty(const char *name);
    void __fastcall GetInfo(TFontInfo *info);
    void Reset();

    property (r,  short,   Ascent);
    property (rw, TString, Charset);
    property (r,  short,   Descent);
    property (r,  TString, FaceName);
    property (rw, TString, Family);
    property (r,  TString, FontType);
    property (rw, TString, Foundry);
    property (r,  bool,    Default);
    property (mr, HFONT,   Font);
    property (rw, TString, FullName);
    property (rw, short,   Height);
    property (r,  short,   Resolution);
    property (m,  TPenStyle, PenStyle);
    property (rw, short,   Points);
    property (rw, short,   Pixels);
    property (rw, short,   Size);
    property (rw, TFontSlant, Slant);
    property (rw, TFontSpacing, Spacing);
    property (m,  int,  Style);
    property (rw, TString, Weight);
    property (rw, TString, Setwidth);
};
//---------------------------------------------------------------------------
// TCanvas
//---------------------------------------------------------------------------
enum TCopyMode
{
    cmBlackness = GXclear,
    cmDstInvert = GXinvert,
//cmMergeCopy =
    cmMergePaint = GXorInverted,
    cmNotSrcCopy = GXcopyInverted,
    cmDstCopy = GXnoop,
    cmNotSrcErase = GXnand,
//cmPatCopy = PATCOPY;
//cmPatInvert = PATINVERT;
//cmPatPaint = PATPAINT;
    cmSrcAnd = GXand,
    cmSrcCopy = GXcopy,
    cmSrcErase = GXandReverse,
    cmSrcInvert = GXxor,
    cmSrcPaint = GXor,
    cmWhiteness = GXset
                  /*
                  Logical Function Hex Code Definition
                  GXclear          0x0      0
                  GXand            0x1      src AND dst
                  GXandReverse     0x2      src AND (NOT dst)
                  GXcopy           0x3      src
                  GXandInverted    0x4      (NOT src) AND dst
                  GXnoop           0x5      dst
                  GXxor            0x6      src XOR dst
                  GXor             0x7      src OR dst
                  GXnor            0x8      (NOT src) AND (NOT dst)
                  GXequiv          0x9      (NOT src) XOR dst
                  GXinvert         0xa      (NOT dst)
                  GXorReverse      0xb      src OR (NOT dst)
                  GXcopyInverted   0xc      (NOT src)
                  GXorInverted     0xd      (NOT src) OR dst
                  GXnand           0xe      (NOT src) OR (NOT dst)
                  GXset            0xf      1
                  */

};

enum TPixelSize
{
    pfNone = 0,
    pf1bit = 1,
    pf4bit = 4,
    pf8bit = 8,
    pf16bit = 16,
    pf24bit = 24,
    pf32bit = 32
};

enum TAlignment { taLeftJustify, taCenter, taRightJustify, taFull };

enum TTextLayout { tlTop, tlCenter, tlBottom };

enum TPrefixStyle
{
    pmNone = 0,
    pmBold = 1,
    pmUnderline = 4,
    pmColor = 8
};

enum TFormatOptions
{
    DT_NOPREFIX = (1 << 0),
    DT_NOCLIP = (1 << 1),
    DT_EXPANDTABS = (1 << 2),
    DT_WORDBREAK = (1 << 3),
    DT_END_ELLIPSIS = (1 << 4),
    DT_CALCRECT = (1 << 5)
};

struct TFormatText
{
    TAlignment Justify;
    TTextLayout Layout;
    word_t Tab;
    short Leading;
    TColor PrefixColor;
    short PrefixStyle;
    int Options;
    TFormatText() : Justify(taLeftJustify), Layout(tlTop), Tab(4), Leading(0),
        PrefixColor(clBlack), PrefixStyle(pmUnderline),
        Options(DT_EXPANDTABS | DT_WORDBREAK | DT_END_ELLIPSIS) { };
//										Options(DT_EXPANDTABS) { };
};
//---------------------------------------------------------------------------
class TCanvas : public TObject
{
    DECLARE_PROPERTY( TCanvas );

private:
    HDC m_Handle;
    HPICTURE m_Picture;
//	TObject *m_owner;
    TRegion m_ClipMask;
    TPoint  m_ClipOrigin;

    TPixelSize get_Depth();
    HPICTURE get_Picture();
    void set_ClipOrigin(TPoint value);
    void set_ClipMask(TRegion value);
    TCopyMode get_CopyMode();
    void set_CopyMode(TCopyMode value);
    void set_Handle(HDC value);
    void Init();
    __fastcall void Assign(const TCanvas &obj);

protected:

public:
    TPoint  PenPos;
    TFont   *Font;
    TPen	*Pen;
    TBrush	*Brush;
    TPoint  Origin;

    TCanvas();
    __fastcall TCanvas(const TCanvas &a);
    __fastcall TCanvas(HDC owner);
    __fastcall TCanvas(TWinControl *owner);
    __fastcall TCanvas(TBitmap *owner);
    virtual ~TCanvas();
    __fastcall const TCanvas & operator=(const TCanvas &a);
    void Arc(int x, int y, unsigned int width, unsigned int height,
             int angle1, int angle2);
    void Arc(int X1, int Y1, int X2, int Y2, int X3, int Y3, int X4, int Y4);
    void Chord(int x, int y, unsigned int width, unsigned int height,
               int angle1, int angle2);
    void Chord(int X1, int Y1, int X2, int Y2, int X3, int Y3, int X4, int Y4);
    void CopyRect(const TRect &Dest, const TCanvas* Canvas, const TRect &Source);
    void Draw(int x, int y, TGraphic &graphic);
    void DrawText(TRect &rect, const TString &text, TFormatText *format = NULL);
    void Ellipse(int x, int y, uint_t width, uint_t height);
    void Ellipse(const TRect &rect);
    void FillRect(int x1, int y1, uint_t width, uint_t height);
    void FillRect(const TRect &rect);
    void FillRoundRect(int x, int y, uint_t width, uint_t height,
                       int dx, int dy);
    void FillRegion(const TRegion &rgn);
    inline void FrameRect(const TRect &rect)
    {
        Rectangle(rect);
    };
    void Highlight(TRect &rect, int level);
    TColor LightenColor(TColor color, int level);
    void Line(int x1, int y1, int x2, int y2);
    void LineTo(int x, int y);
    void Lock();
    void MoveTo(int x, int y);
    void Pie(int x, int y, uint_t width, uint_t height, int angle1, int angle2);
    void Pie(int X1, int Y1, int X2, int Y2, int X3, int Y3, int X4, int Y4);
    void Point(int x, int y);
    inline void Point(const TPoint &point)
    {
        Point(point.x, point.y);
    };
    void Points(TPoint *points, int npoints, int mode);
    TColor Pixels(int x, int y);
    inline TColor Pixels(const TPoint &point)
    {
        return Pixels(point.x, point.y);
    };
    void Pixels(int x, int y, TColor color);
    void Polygon(TPoint *points, const int npoints);
    void Polyline(TPoint *points, const int npoints);
    void Rectangle(int x, int y, uint_t width, uint_t height);
    void Rectangle(const TRect &rect);
    void Rectangles(TRect *rects, const int nrects);
    void RemoveClipMask();
    void RoundRect(int x, int y, uint_t width, uint_t height, int rx, int ry);
    void SetClipMask(const HBITMAP abitmap);
    void SetClipMask(const TBitmap &abitmap);
    void StretchDraw(const TRect &rect, TGraphic &g);
    void TextLine(int x, int y, int delta, TStrings &text);
    void TextOut(int x, int y, const TString &text);
    inline void TextOut(const TString &text)
    {
        TextOut(PenPos.x, PenPos.y, text);
    };
    void TextRect(const TRect &rect, int x, int y, const TString &text, TAlign flags = alNone);
    inline void TextRect(const TRect &rect, const TString &text, TAlign flags = alNone)
    {
        TextRect(rect,PenPos.x,PenPos.y,text,flags);
    };
    int  TextHeight(const TString &text);
    int  TextWidth(const TString &text);
    TRect TextExtents(const TString &text);
    void TiledDraw(const TRect &rect, TGraphic &graphic);
    void Unlock();

    void Composite(int op, const TRect &dest, const TCanvas* canvas, const TRect &source);

    property (m,  TPoint,  ClipOrigin);
    property (m,  TRegion, ClipMask);
    property (rw, TCopyMode, CopyMode);
    property (r,  TPixelSize, Depth);
    property (m,  HDC, Handle);
    property (r,  HPICTURE, Picture);
};
//---------------------------------------------------------------------------
// TGraphic
//---------------------------------------------------------------------------

/* TProgressEvent is a generic progress notification event which may be
        used by TGraphic classes with computationally intensive (slow)
        operations, such as loading, storing, or transforming image data.
    Event params:
      Stage - Indicates whether this call to the OnProgress event is to
        prepare for, process, or clean up after a graphic operation.  If
        OnProgress is called at all, the first call for a graphic operation
        will be with Stage = psStarting, to allow the OnProgress event handler
        to allocate whatever resources it needs to process subsequent progress
        notifications.  After Stage = psStarting, you are guaranteed that
        OnProgress will be called again with Stage = psEnding to allow you
        to free those resources, even if the graphic operation is aborted by
        an exception.  Zero or more calls to OnProgress with Stage = psRunning
        may occur between the psStarting and psEnding calls.
      PercentDone - The ratio of work done to work remaining, on a scale of
        0 to 100.  Values may repeat or even regress (get smaller) in
        successive calls.  PercentDone is usually only a guess, and the
        guess may be dramatically altered as new information is discovered
        in decoding the image.
      RedrawNow - Indicates whether the graphic can be/should be redrawn
        immediately.  Useful for showing successive approximations of
        an image as data is available instead of waiting for all the data
        to arrive before drawing anything.  Since there is no message loop
        activity during graphic operations, you should call Update to force
        a control to be redrawn immediately in the OnProgress event handler.
        Redrawing a graphic when RedrawNow = False could corrupt the image
        and/or cause exceptions.
      Rect - Area of image that has changed and needs to be redrawn.
      Msg - Optional text describing in one or two words what the graphic
        class is currently working on.  Ex:  "Loading" "Storing"
        "Reducing colors".  The Msg string can also be empty.
        Msg strings should be resourced for translation,  should not
        contain trailing periods, and should be used only for
        display purposes.  (do not: if Msg = 'Loading' then...)
*/

enum TProgressStage { psStarting, psRunning, psEnding };

typedef delegate< void (TObject *Sender, TProgressStage Stage, byte_t PercentDone,
                        bool RedrawNow, const TRect &R, const TString &Msg) >  TProgressEvent;

/* The TGraphic class is a abstract base class for dealing with graphic images
    such as metafile, bitmaps, icons, and other image formats.
      LoadFromFile - Read the graphic from the file system.  The old contents of
        the graphic are lost.  If the file is not of the right format, an
        exception will be generated.
      SaveToFile - Writes the graphic to disk in the file provided.
      LoadFromStream - Like LoadFromFile except source is a stream (e.g.
        TBlobStream).
      SaveToStream - stream analogue of SaveToFile.
      LoadFromClipboardFormat - Replaces the current image with the data
        provided.  If the TGraphic does not support that format it will generate
        an exception.
      SaveToClipboardFormats - Converts the image to a clipboard format.  If the
        image does not support being translated into a clipboard format it
        will generate an exception.
      Height - The native, unstretched, height of the graphic.
      Palette - Color palette of image.  Zero if graphic doesn't need/use palettes.
      Transparent - Image does not completely cover its rectangular area
      Width - The native, unstretched, width of the graphic.
      OnChange - Called whenever the graphic changes
      PaletteModified - Indicates in OnChange whether color palette has changed.
        Stays true until whoever's responsible for realizing this new palette
        (ex: TImage) sets it to False.
      OnProgress - Generic progress indicator event. Propagates out to TPicture
        and TImage OnProgress events. */

enum TResizeMode { rmCrop, rmStretch };

class TGraphic : public TObject
{
    DECLARE_PROPERTY( TGraphic );

private:
    void Init();
    __fastcall void Assign(const TGraphic &obj);
    void set_Height(uint_t value);
    void set_Width(uint_t value);

protected:
    uint_t m_Height;
    uint_t m_Width;
    virtual bool get_Empty() = 0;
    virtual void Progress(TObject *Sender, TProgressStage Stage, byte_t PercentDone,
                          bool RedrawNow, const TRect &R, const TString &Msg);

public:
    TPoint HotSpot;
    TResizeMode ResizeMode;

    TGraphic();
    __fastcall TGraphic(const TGraphic &a);
    __fastcall const TGraphic & operator=(const TGraphic &a);

    virtual void Draw(TCanvas *canvas, int x, int y, const TRect &rect) = 0;
    virtual void StretchDraw(TCanvas *canvas, const TRect &rect) = 0;
    virtual void Load(const TString &filename) = 0;
    virtual void Save(const TString &filename) = 0;
    inline  void SaveAs(const TString &filename)
    {
        TGraphic :: Save(filename) ;
    };
    virtual void SetSize(uint_t w, uint_t h) = 0;
    virtual void TiledDraw(TCanvas *canvas, const TRect &rect);

    TProgressEvent OnProgress;

    property (r, bool, Empty);
    property (m, uint_t, Width);
    property (m, uint_t, Height);
};
/*
  TGraphic = class(TInterfacedPersistent, IStreamPersist)
  private
    FOnChange: TNotifyEvent;
    FModified: Boolean;
    FTransparent: Boolean;
    procedure SetModified(Value: Boolean);
  protected
    procedure Changed(Sender: TObject); virtual;
    procedure DefineProperties(Filer: TFiler); override;
    function Equals(Graphic: TGraphic): Boolean; virtual;

    function GetTransparent: Boolean; virtual;

    procedure ReadData(Stream: TStream); virtual;
    procedure SetTransparent(Value: Boolean); virtual;
    procedure WriteData(Stream: TStream); virtual;
  public
    procedure LoadFromFile(const Filename: string); virtual;
    procedure SaveToFile(const Filename: string); virtual;
    procedure LoadFromStream(Stream: TStream); virtual; abstract;
    procedure SaveToStream(Stream: TStream); virtual; abstract;
    procedure LoadFromClipboardFormat(AFormat: Word; AData: THandle;
      APalette: HPALETTE); virtual; abstract;
    procedure SaveToClipboardFormat(var AFormat: Word; var AData: THandle;
      var APalette: HPALETTE); virtual; abstract;

    property Modified: Boolean read FModified write SetModified;

    property Transparent: Boolean read GetTransparent write SetTransparent;

    property OnChange: TNotifyEvent read FOnChange write FOnChange;
  end;

*/
//---------------------------------------------------------------------------
// TBitmap
//---------------------------------------------------------------------------
#define RGB(r, g, b)	(r << 16) | (g << 8) | b
#define ARGB(a, r, g, b)	(a << 24) | (r << 16) | (g << 8) | b
#define R(clr)			((clr & 0x00FF0000) >> 16)
#define G(clr)			((clr & 0x0000FF00) >> 8)
#define B(clr)		 	(clr & 0x000000FF)
#define A(clr)		 	((clr & 0xFF000000) >> 24)

struct TRGB
{
    byte_t r, g, b;
};
struct TBGR
{
    byte_t b, g, r;
};

union RGBQUAD
{
    unsigned int p;
    struct
    {
        byte_t	r;
        byte_t	g;
        byte_t	b;
        byte_t	a;
    };
};

union BGRQUAD
{
    unsigned int p;
    struct
    {
        byte_t	b;
        byte_t	g;
        byte_t	r;
        byte_t	a;
    };
};

enum TColorMode
{
    cmNone,
    cmBW,
    cmGrayScale,
    cmIndexed,
    cmColor
};

struct TPixelFormat
{
    TPixelSize Depth;
    byte_t ByteOrder;
    byte_t BitOrder;
    TColorMode ColorMode;
    TPixelFormat();
    TPixelFormat(const TPixelFormat &pf);
    TPixelFormat(TPixelSize depth);
    bool operator == (const TPixelFormat &val) const
    {
        return (Depth == val.Depth) && (ByteOrder == val.ByteOrder) &&
               (BitOrder == val.BitOrder) && (ColorMode == val.ColorMode);
    }
};

enum TBitmapHandleType { bmEmpty, bmDIB, bmDDB };
enum TMirrorType { mtMirrorVert, mtMirrorHorz };

class TBitmap : public TGraphic
{
    DECLARE_PROPERTY( TBitmap );

private:
    void Init();
    __fastcall void Assign(const TBitmap &bmp);
    void convert_to_dib();
    void convert_to_ddb();
    inline TCanvas *get_Canvas()
    {
        HandleType = bmDDB;
        return m_Canvas;
    };
    TBitmapHandleType get_HandleType();
    void set_HandleType(TBitmapHandleType value);
    inline HBITMAP get_Handle()
    {
        HandleType = bmDDB;
        return m_Handle;
    };
    inline HBITMAP get_MaskHandle()
    {
        HandleType = bmDDB;
        return m_MaskHandle;
    };
    inline HDIB get_Data()
    {
        HandleType = bmDIB;
        return m_Data;
    };
    inline HDIB get_MaskData()
    {
        HandleType = bmDIB;
        return m_MaskData;
    };
    void set_PixelFormat(TPixelFormat value);
    void set_TransparentColor(TColor value);
    byte_t *get_ScanLine(int n);
    bool get_Empty();

protected:
    HBITMAP m_Handle;
    HDIB m_Data;
    HBITMAP m_MaskHandle;
    HDIB m_MaskData;
    TPixelFormat m_PixelFormat;
    TCanvas *m_Canvas;
    RGBQUAD *m_Colormap;
    TColor m_TransparentColor;

    void SetColormap(TRGB *cmap, int ncolor);

public:
    TBitmap();
    __fastcall TBitmap(TBitmap &obj);
    __fastcall TBitmap(uint_t w, uint_t h);
    virtual ~TBitmap();
    __fastcall const TBitmap & operator = ( const TBitmap &value );

//	void Assign(TObject &obj);
    void Clear();
    void Create(uint_t w, uint_t h, TBitmapHandleType type = bmDDB, const TPixelFormat *pf = NULL, const TPixelFormat *pfmask = NULL);
    inline RGBQUAD *GetColormap()
    {
        return m_Colormap;
    };
    inline void Dormant()
    {
        convert_to_dib();
    };
    void Draw(TCanvas *canvas, int x, int y, const TRect &rect);
    virtual void Load(const TString &filename);
    virtual void Save(const TString &filename);
    void LoadImage(HDC hdc, HBITMAP hmask = 0);
    void LoadImage(HDIB hbmp, HDIB hmask = NULL);
    void Mask(TColor maskcolor, uint_t threshold = 0);
    void Mirror(TMirrorType value);
    TColor Pixels(int x, int y);
    void Pixels(int x, int y, TColor color);
    void Release();
    void ReleaseMask();
    void Rotate(int angle);
    void SetSize(uint_t w, uint_t h);
    void StretchDraw(TCanvas *canvas, const TRect &rect);

    property (r, TCanvas *, Canvas);
    property (r, HBITMAP, Handle);
    property (r, HDIB, Data);
    property (r, HBITMAP, MaskHandle);
    property (r, HDIB, MaskData);
    property_i (r, byte_t *, int, ScanLine);
    property (rw, TBitmapHandleType, HandleType);
    property (m, TPixelFormat, PixelFormat);
    property (m, TColor, TransparentColor);
};
//---------------------------------------------------------------------------
// TImageList
//---------------------------------------------------------------------------
class TImageList : public TBitmap
{
    DECLARE_PROPERTY( TImageList );

private:
    inline uint_t get_Count()
    {
        return Cols * Rows;
    };
    inline uint_t get_Cols()
    {
        return Width / ImageWidth;
    };
    inline uint_t get_Rows()
    {
        return Height / ImageHeight;
    };

protected:

public:
    uint_t ImageHeight;
    uint_t ImageWidth;

    TImageList();

    void __fastcall Draw(TCanvas* canvas, int x, int y, int index);
    void __fastcall DrawRect(TCanvas *canvas, const TRect &rect, int x, int y, int index, TAlign flags);
    void __fastcall GetBitmap(int index, TBitmap* image);
    int  __fastcall GetIndex(uint_t col, uint_t row);

    property (r, uint_t, Count);
    property (r, uint_t, Cols);
    property (r, uint_t, Rows);
};

/*
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	int __fastcall Add(Graphics::TBitmap* Image, Graphics::TBitmap* Mask);
	void __fastcall AddImages(TCustomImageList* Value);
	int __fastcall AddMasked(Graphics::TBitmap* Image, Graphics::TColor MaskColor);
	void __fastcall Delete(int Index);
	void __fastcall Draw(Graphics::TCanvas* Canvas, int X, int Y, int Index, bool Enabled = true);
	void __fastcall Draw(Graphics::TCanvas* Canvas, int X, int Y, int Index, TDrawingStyle ADrawingStyle, TImageType AImageType, bool Enabled = true);
	void __fastcall DrawOverlay(Graphics::TCanvas* Canvas, int X, int Y, int ImageIndex, TOverlay Overlay, bool Enabled = true);
	void __fastcall DrawOverlay(Graphics::TCanvas* Canvas, int X, int Y, int ImageIndex, TOverlay Overlay, TDrawingStyle ADrawingStyle, TImageType AImageType, bool Enabled = true);
	bool __fastcall FileLoad(TResType ResType, const AnsiString Name, Graphics::TColor MaskColor);
	HIDESBASE void __fastcall Insert(int Index, Graphics::TBitmap* Image, Graphics::TBitmap* Mask);
	void __fastcall InsertMasked(int Index, Graphics::TBitmap* Image, Graphics::TColor MaskColor);
	void __fastcall Move(int CurIndex, int NewIndex);
	bool __fastcall Overlay(int ImageIndex, TOverlay Overlay);
	void __fastcall RegisterChanges(TChangeLink* Value);
	bool __fastcall ResourceLoad(TResType ResType, const AnsiString Name, Graphics::TColor MaskColor);
	void __fastcall Replace(int Index, Graphics::TBitmap* Image, Graphics::TBitmap* Mask);
	void __fastcall ReplaceMasked(int Index, Graphics::TBitmap* NewImage, Graphics::TColor MaskColor);
	__property int AllocBy = {read=FAllocBy, write=FAllocBy, default=4};
	__property Graphics::TColor BlendColor = {read=FBlendColor, write=FBlendColor, default=536870911};
	__property Graphics::TColor BkColor = {read=GetBkColor, write=SetBkColor, default=536870911};
	__property TDrawingStyle DrawingStyle = {read=FDrawingStyle, write=SetDrawingStyle, default=2};
	__property TImageType ImageType = {read=FImageType, write=FImageType, default=0};
	__property bool Masked = {read=FMasked, write=FMasked, default=1};
	__property Classes::TNotifyEvent OnChange = {read=FOnChange, write=FOnChange};
*/
//---------------------------------------------------------------------------
}  // namespace
//---------------------------------------------------------------------------
#endif
