//---------------------------------------------------------------------------
#include <stdio.h>
#include <cmath>
#include "application.h"
#include "xwrender.h"

#define DegToRad(x) x * M_PI / 180
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
static void __convertcolors(TColor clr, XRenderColor &xrclr)
{
    xrclr.red   = R(clr) * 0xffff / 0xff;
    xrclr.green = G(clr) * 0xffff / 0xff;
    xrclr.blue  = B(clr) * 0xffff / 0xff;
    xrclr.alpha = (0xff - A(clr)) * 0xffff / 0xff;
}
//---------------------------------------------------------------------------
void DrawLinearGradient(TCanvas *canvas, TRect rect, int *stops, TColor *colors, int ncolors, int angle)
{
    Picture pic1;
    int *xrstops;
    XRenderColor *xrcolors;

    XLinearGradient gradient;
    double sina = sin(DegToRad(angle));
    double cosa = cos(DegToRad(angle));
    double k = (abs(rect.height * sina) + abs(rect.width * cosa)) / 2;
    double y = k * sina;
    double x = k * cosa;

    gradient.p1.x = XDoubleToFixed (x + rect.width / 2);
    gradient.p1.y = XDoubleToFixed (rect.height / 2 - y);
    gradient.p2.x = XDoubleToFixed (rect.width / 2 - x);
    gradient.p2.y = XDoubleToFixed (y + rect.height / 2);

    xrcolors = new XRenderColor[ncolors];
    xrstops = new int[ncolors];
    for(register int i = 0; i < ncolors; i++)
    {
        xrstops[i] = XDoubleToFixed ((double)stops[i] / 100);
        __convertcolors(colors[i], xrcolors[i]);
    }

    pic1 = XRenderCreateLinearGradient(Application.Handle, &gradient, xrstops, xrcolors, ncolors);
    XRenderComposite(Application.Handle, PictOpSrc, pic1, None, canvas->Picture,
                     0, 0, 0, 0, rect.left, rect.top, rect.width, rect.height);
    XRenderFreePicture(Application.Handle, pic1);
    delete[] xrcolors;
    delete[] xrstops;
}
//---------------------------------------------------------------------------
void DrawLinearGradient(TCanvas *canvas, TRect rect, TColor color_from, TColor color_to, int angle)
{
    Picture pic1;
    int nstops = 2;
    int stops[2];
    XRenderColor colors[2];

    XLinearGradient gradient;
    double sina = sin(DegToRad(angle));
    double cosa = cos(DegToRad(angle));
    double k = (abs(rect.height * sina) + abs(rect.width * cosa)) / 2;
    double y = k * sina;
    double x = k * cosa;

    gradient.p1.x = XDoubleToFixed (x + rect.width / 2);
    gradient.p1.y = XDoubleToFixed (rect.height / 2 - y);
    gradient.p2.x = XDoubleToFixed (rect.width / 2 - x);
    gradient.p2.y = XDoubleToFixed (y + rect.height / 2);

    stops[0] = XDoubleToFixed (0);
    stops[1] = XDoubleToFixed (1);

    __convertcolors(color_from, colors[0]);
    __convertcolors(color_to, colors[1]);

    pic1 = XRenderCreateLinearGradient(Application.Handle, &gradient, stops, colors, nstops);
    XRenderComposite(Application.Handle, PictOpSrc, pic1, None, canvas->Picture,
                     0, 0, 0, 0, rect.left, rect.top, rect.width, rect.height);
    XRenderFreePicture(Application.Handle, pic1);
}
//---------------------------------------------------------------------------
void DrawRadialGradient(TCanvas *canvas, TRect rect, int *stops, TColor *colors, int ncolors)
{
    Picture pic1;
    int *xrstops;
    XRenderColor *xrcolors;

    XRadialGradient gradient;
    gradient.inner.radius = XDoubleToFixed(10);
    gradient.inner.x = XDoubleToFixed(rect.center().x - rect.left);
    gradient.inner.y = XDoubleToFixed(rect.center().y - rect.top);

    gradient.outer.radius = XDoubleToFixed(rect.width / 2);
    gradient.outer.x = XDoubleToFixed(rect.center().x - rect.left);
    gradient.outer.y = XDoubleToFixed(rect.center().y - rect.top);

    xrcolors = new XRenderColor[ncolors];
    xrstops = new int[ncolors];
    for(register int i = 0; i < ncolors; i++)
    {
        xrstops[i] = XDoubleToFixed ((double)stops[i] / 100);
        __convertcolors(colors[i], xrcolors[i]);
    }

    pic1 = XRenderCreateRadialGradient(Application.Handle, &gradient, xrstops, xrcolors, ncolors);
    XRenderComposite(Application.Handle, PictOpSrc, pic1, None, canvas->Picture,
                     0, 0, 0, 0, rect.left, rect.top, rect.width, rect.height);
    XRenderFreePicture(Application.Handle, pic1);
    delete[] xrcolors;
    delete[] xrstops;
}
//---------------------------------------------------------------------------
void DrawConicalGradient(TCanvas *canvas, TRect rect, int *stops, TColor *colors, int ncolors)
{
    Picture pic1;
    int *xrstops;
    XRenderColor *xrcolors;

    XConicalGradient gradient;
    gradient.angle = XDoubleToFixed(45);
    gradient.center.x = XDoubleToFixed(rect.center().x - rect.left);
    gradient.center.y = XDoubleToFixed(rect.center().y - rect.top);

    xrcolors = new XRenderColor[ncolors];
    xrstops = new int[ncolors];
    for(register int i = 0; i < ncolors; i++)
    {
        xrstops[i] = XDoubleToFixed ((double)stops[i] / 100);
        __convertcolors(colors[i], xrcolors[i]);
    }

    pic1 = XRenderCreateConicalGradient(Application.Handle, &gradient, xrstops, xrcolors, ncolors);
    XRenderComposite(Application.Handle, PictOpSrc, pic1, None, canvas->Picture,
                     0, 0, 0, 0, rect.left, rect.top, rect.width, rect.height);
    XRenderFreePicture(Application.Handle, pic1);
    delete[] xrcolors;
    delete[] xrstops;
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
