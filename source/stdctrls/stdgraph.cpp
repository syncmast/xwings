#include <math.h>
#include "stdgraph.h"
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
void DrawCheck(TCanvas &canvas, const TPoint &location, int width, int size)
{
    TPoint pt[7];

    pt[0] = Point(location.x, location.y);
    pt[1] = Point(location.x + size, location.y + size);
    pt[2] = Point(location.x + size * 2 + size, location.y - size);
    pt[3] = Point(location.x + size * 2 + size, location.y - size - 1);
    pt[4] = Point(location.x + size, location.y + size - 1);
    pt[5] = Point(location.x - 1, location.y - 2);

    canvas.Pen->Width = width;
    canvas.Polyline(pt, 6);
}
//---------------------------------------------------------------------------
const TPoint ArrowPts[][3] =
    { { TPoint( 1, 0 ), TPoint( 0, 1 ), TPoint( 1, 2 ) },
      { TPoint( 0, 0 ), TPoint( 1, 1 ), TPoint( 0, 2 ) },
      { TPoint( 0, 1 ), TPoint( 1, 0 ), TPoint( 2, 1 ) },
      { TPoint( 0, 0 ), TPoint( 1, 1 ), TPoint( 2, 0 ) } };

void DrawArrow(TCanvas &canvas, TDirection direction, const TPoint &location, int size)
{
    TPoint pts[3];
    int OldWidth;
    TColor OldColor;

    OldColor = canvas.Brush->Color;
    canvas.Brush->Color = canvas.Pen->Color;
    memcpy( pts, ArrowPts[direction], sizeof(pts));
    for(register int i = 0; i < 3; i++)
        pts[i] = Point(pts[i].x * size + location.x, pts[i].y * size + location.y);

    OldWidth = canvas.Pen->Width;
    canvas.Pen->Width = 1;
    canvas.Polygon(pts, 3);
    canvas.Pen->Width = OldWidth;
    canvas.Brush->Color = OldColor;
}
//---------------------------------------------------------------------------
static void DrawLine(TCanvas &canvas, TDirection direction, const TPoint &location, int width, int size)
{
    TPoint pts[3];

    memcpy( pts, ArrowPts[direction], sizeof(pts));
    // Scale to the correct size
    for(register int i = 0; i < 3; i++)
        pts[i] = Point(pts[i].x * size + location.x, pts[i].y * size + location.y);
    switch(direction)
    {
        case dDown : pts[2] = Point(pts[2].x + 1, pts[2].y - 1); break;
        case dRight: pts[2] = Point(pts[2].x, pts[2].y); break;
        case dUp:
        case dLeft : pts[2] = Point(pts[2].x, pts[2].y);
    };
    canvas.Polyline(pts, 3);
}

void DrawChevron(TCanvas &canvas, TDirection direction, const TPoint &loc, int width, int size, int between)
{
    int OldWidth;
    TPoint location = loc;
    OldWidth = canvas.Pen->Width;
    canvas.Pen->Width = width;
    switch(direction)
    {
        case dLeft:
        case dRight:
            location.x -= size;
            DrawLine(canvas, direction, location, width, size);
            location.x += width;
            DrawLine(canvas, direction, location, width, size);
            location.x += (between + width);
            DrawLine(canvas, direction, location, width, size);
            location.x += width;
            DrawLine(canvas, direction, location, width, size);
            break;
        case dUp:
        case dDown:
            location.y -= size;
            DrawLine(canvas, direction, location, width, size);
            location.y += width;
            DrawLine(canvas, direction, location, width, size);
            location.y += (between + width);
            DrawLine(canvas, direction, location, width, size);
            location.y += width;
            DrawLine(canvas, direction, location, width, size);
            break;
    };
    canvas.Pen->Width = OldWidth;
}
//---------------------------------------------------------------------------
#define HLSMAX  240       // H,L, and S vary over 0-HLSMAX
#define RGBMAX  255       // R,G, and B vary over 0-RGBMAX
                          // HLSMAX BEST IF DIVISIBLE BY 6
                          // RGBMAX, HLSMAX must each fit in a byte.

/* Hue is undefined if Saturation is 0 (grey-scale)
    This value determines where the Hue scrollbar is
    initially set for achromatic colors */

#define HLSUndefined  (HLSMAX*2/3)

void ColorRGBToHLS(TColor clr, word_t &Hue, word_t &Luminance, word_t &Saturation)
{
    double H, L, S;
    word_t R, G, B;
    double cMax, cMin;
    double Rdelta, Gdelta, Bdelta; // intermediate value: % of spread from max

    R = R(clr);
    G = G(clr);
    B = B(clr);

// calculate lightness
    cMax = max(max(R, G), B);
    cMin = min(min(R, G), B);
    L = ( ((cMax + cMin) * HLSMAX) + RGBMAX ) / ( 2 * RGBMAX);
    if(cMax == cMin)  // r=g=b --> achromatic case
    {                // saturation
        Hue = round(HLSUndefined);
//    pwHue := 160;      { MS's ColorRGBToHLS always defaults to 160 in this case }
        Luminance = round(L);
        Saturation = 0;
    }
    else                 // chromatic case
    {
        // saturation
        if(L <= HLSMAX/2)
            S = ( ((cMax-cMin)*HLSMAX) + ((cMax+cMin)/2) ) / (cMax+cMin);
        else
            S = ( ((cMax-cMin)*HLSMAX) + ((2*RGBMAX-cMax-cMin)/2) ) / (2*RGBMAX-cMax-cMin);

        // hue
        Rdelta = ( ((cMax-R)*(HLSMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);
        Gdelta = ( ((cMax-G)*(HLSMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);
        Bdelta = ( ((cMax-B)*(HLSMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);

        if(R == cMax)
            H = Bdelta - Gdelta;
        else if(G == cMax)
            H = (HLSMAX/3) + Rdelta - Bdelta;
        else // B == cMax
            H = ((2 * HLSMAX) / 3) + Gdelta - Rdelta;

        if(H < 0)
            H = H + HLSMAX;
        if(H > HLSMAX)
            H = H - HLSMAX;
        Hue = round(H);
        Luminance = round(L);
        Saturation = round(S);
    };
}
//---------------------------------------------------------------------------
int HueToRGB(double Lum, double Sat, double Hue)
{
    double ResultEx;

//   range check: note values passed add/subtract thirds of range
    if(Hue < 0)
        Hue = Hue + HLSMAX;

    if(Hue > HLSMAX)
        Hue = Hue - HLSMAX;

//   return r,g, or b value from this tridrant
    if(Hue < (HLSMAX/6))
        ResultEx = Lum + (((Sat-Lum)*Hue+(HLSMAX/12))/(HLSMAX/6));
    else if(Hue < (HLSMAX/2))
        ResultEx = Sat;
    else if(Hue < ((HLSMAX*2)/3))
        ResultEx = Lum + (((Sat-Lum)*(((HLSMAX*2)/3)-Hue)+(HLSMAX/12))/(HLSMAX/6));
    else
        ResultEx = Lum;
    return round(ResultEx);
}
//---------------------------------------------------------------------------
static int RoundColor(double Value)
{
    int ret;
    if(Value > 255)
      ret = 255;
    else
      ret = round(Value);
    return ret;
}
//---------------------------------------------------------------------------
TColor ColorHLSToRGB(word_t Hue, word_t Luminance, word_t Saturation)
{
    double R,G,B;              // RGB component values
    double Magic1,Magic2;      // calculated magic numbers (really!)

    if(Saturation == 0)
    {            // achromatic case
        R = (Luminance * RGBMAX)/HLSMAX;
        G = R;
        B = R;
        if(Hue != HLSUndefined)
            ;//{ ERROR }
    }
    else
    {            // chromatic case
                 // set up magic numbers
        if(Luminance <= (HLSMAX/2))
            Magic2 = (Luminance * (HLSMAX + Saturation) + (HLSMAX/2)) / HLSMAX;
        else
            Magic2 = Luminance + Saturation - ((Luminance * Saturation) + (HLSMAX/2)) / HLSMAX;
        Magic1 = 2 * Luminance - Magic2;

        // get RGB, change units from HLSMAX to RGBMAX
        R = (HueToRGB(Magic1,Magic2,Hue+(HLSMAX/3))*RGBMAX + (HLSMAX/2))/HLSMAX;
        G = (HueToRGB(Magic1,Magic2,Hue)*RGBMAX + (HLSMAX/2)) / HLSMAX;
        B = (HueToRGB(Magic1,Magic2,Hue-HLSMAX/3)*RGBMAX + (HLSMAX/2))/HLSMAX;
    };
    return RGB(RoundColor(R), RoundColor(G), RoundColor(B));
}
//---------------------------------------------------------------------------
TColor ColorAdjustLuma(TColor clr, int n, bool fScale)
{
    word_t H, L, S;

    ColorRGBToHLS(clr, H, L, S);
    return ColorHLSToRGB(H, L + n, S);
}

//threadvar
TColor  CurrentHighlightColor, CurrentHighlight;
TColor  CurrentShadowColor, CurrentShadow;

TColor GetHighLightColor(const TColor Color)
{
    word_t H, L, S;
    uint_t Clr;
    TColor Result;

    if(Color == CurrentHighlightColor)
        Result = CurrentHighlight;
    else
    {
        Clr = Color;  // Clr = ColorToRGB(Color);
        ColorRGBToHLS(Clr, H, L, S);
        if(S > 220)
            Result = ColorHLSToRGB(H, L - 19, S);
        else
            Result = ColorAdjustLuma(Clr, 19, false);
        CurrentHighlightColor = Color;
        CurrentHighlight = Result;
    }
    return Result;
}
//---------------------------------------------------------------------------
TColor GetShadowColor(const TColor Color)
{
    word_t H, L, S;
    TColor Result;

    if(Color == CurrentShadowColor)
        Result = CurrentShadow;
    else
    {
        if(Color == clBtnFace)
            Result = clBtnShadow;
        else
        {
            ColorRGBToHLS(Color, H, L, S);
            if(S >= 160)
                Result = ColorHLSToRGB(H, L - 50, S);
            else
                Result = TColor(ColorAdjustLuma(Color, -52, False));
        }
        CurrentShadowColor = Color;
        CurrentShadow = Result;
    }
    return Result;
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
