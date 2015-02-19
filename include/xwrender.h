//---------------------------------------------------------------------------
#ifndef xwrenderH
#define xwrenderH

#include "graphics.h"
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
void DrawLinearGradient(TCanvas *canvas, TRect rect, int *stops, TColor *colors, int ncolors, int angle);
void DrawLinearGradient(TCanvas *canvas, TRect rect, TColor color_from, TColor color_to, int angle);
void DrawRadialGradient(TCanvas *canvas, TRect rect, int *stops, TColor *colors, int ncolors);
void DrawConicalGradient(TCanvas *canvas, TRect rect, int *stops, TColor *colors, int ncolors);
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
#endif
