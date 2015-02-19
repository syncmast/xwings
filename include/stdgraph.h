#ifndef STDGRAPH_H_INCLUDED
#define STDGRAPH_H_INCLUDED
//---------------------------------------------------------------------------
#include "graphics.h"
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
enum TDirection { dLeft, dRight, dUp, dDown };

void DrawCheck(TCanvas &canvas, const TPoint &location, int width, int size);
void DrawArrow(TCanvas &canvas, TDirection direction, const TPoint &location, int size);
void DrawChevron(TCanvas &canvas, TDirection direction, const TPoint &loc, int width, int size, int between);

void ColorRGBToHLS(TColor clr, word_t &Hue, word_t &Luminance, word_t &Saturation);
int  HueToRGB(double Lum, double Sat, double Hue);
TColor ColorHLSToRGB(word_t Hue, word_t Luminance, word_t Saturation);
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
#endif // STDGRAPH_H_INCLUDED
