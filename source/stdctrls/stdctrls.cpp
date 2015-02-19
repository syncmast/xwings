//---------------------------------------------------------------------------
#include "application.h"
#include "rtti.h"
#include "xpainter.h"
#include "stdctrls.h"
#include <stdio.h>
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
void SO_ENTRYPOINT initialization()
{
    RegisterPainter(NULL, &XPainter);
//	RegisterClass(TCheckBox);
//	REGISTERCLASSSTRING("xbm", TXBMImage);
}

void SO_EXITPOINT finalization()
{
}
//---------------------------------------------------------------------------
DYNAMIC_CLASS( TCheckBox );
DYNAMIC_CLASS( TPopupMenu );
//---------------------------------------------------------------------------
void FrameRounded(TCanvas &canvas, TRect &rect, TColor color, int r)
{
    // Draw Frame with round corners
    canvas.Pen->Color = color;
    rect.width--;
    rect.height--;
    TPoint point[9];
    point[0] = TPoint(rect.left + r, rect.top);
    point[1] = TPoint(rect.right() - r, rect.top);
    point[2] = TPoint(rect.right()    , rect.top    + r);
    point[3] = TPoint(rect.right()    , rect.bottom() - r);
    point[4] = TPoint(rect.right() - r, rect.bottom() );
    point[5] = TPoint(rect.left  + r, rect.bottom() );
    point[6] = TPoint(rect.left     , rect.bottom() - r);
    point[7] = TPoint(rect.left     , rect.top    + r);
    point[8] = TPoint(rect.left  + r, rect.top       );

    canvas.Polyline(point, 9);

    rect.width++;
    rect.height++;
}

void DrawButtonArrow(TCanvas *ACanvas, const TRect &rect, TArrowOrientation orientation, bool down)
{
    ACanvas->Brush->Color = clBtnFace;
    ACanvas->FillRect(rect);
    if(down)
        Frame3D(ACanvas, rect, clBtnShadow, clBtnHighlight, 1);
    else
        Frame3D(ACanvas, rect, clBtnHighlight, clBtnShadow, 1);

    TRect arrow;
    arrow.width = rect.width / 2;
    arrow.height = rect.height / 2;
    arrow = const_cast<TRect &>(rect).align(arrow, alCenter);
    if(down)
    {
        arrow.left += 1;
        arrow.top += 1;
    }
    DrawArrow(ACanvas, clBtnShadow, arrow, orientation, true);
}

void DrawArrow(TCanvas *ACanvas, TColor color, const TRect &rect, TArrowOrientation orientation, bool fill)
{
    TPoint pt[4];
    TColor old;

    if(fill)
    {
        old = ACanvas->Brush->Color;
        ACanvas->Brush->Color = color;
    }
    else
    {
        old = ACanvas->Pen->Color;
        ACanvas->Pen->Color = color;
    }
    switch(orientation)
    {
        case arrowLeft:
            pt[0] = TPoint(rect.left + rect.width - 1, rect.top);
            pt[1] = TPoint(rect.left + rect.width - 1, rect.top + rect.height - 1);
            pt[2] = TPoint(rect.left, rect.top + rect.height/2);
            break;
        case arrowUp:
            pt[0] = TPoint(rect.left + rect.width/2, rect.top);
            pt[1] = TPoint(rect.left + rect.width - 1, rect.top + rect.height - 1);
            pt[2] = TPoint(rect.left, rect.top + rect.height - 1);
            break;
        case arrowRight:
            pt[0] = TPoint(rect.left, rect.top);
            pt[1] = TPoint(rect.left + rect.width - 1, rect.top + rect.height/2);
            pt[2] = TPoint(rect.left, rect.top + rect.height - 1);
            break;
        case arrowDown:
            pt[0] = TPoint(rect.left, rect.top);
            pt[1] = TPoint(rect.left + rect.width - 1, rect.top);
            pt[2] = TPoint(rect.left + rect.width/2, rect.top + rect.height - 1);
            break;
    }
    if(fill)
    {
        ACanvas->Polygon(pt, 3);
        ACanvas->Brush->Color = old;
    }
    else
    {
        pt[3] = pt[0];
        ACanvas->Polyline(pt, 4);
        ACanvas->Pen->Color = old;
    }
}

TColor NewColor(TColor clr, int value)
{
	int r, g, b;

  	if(value > 200)
  		value = 200;

  	r = R(clr);
  	g = G(clr);
  	b = B(clr);

	r = r + ((255 - r) * value) / 200;
  	g = g + ((0 - g) * value) / 200;
  	b = b + ((0 - b) * value) / 200;

//	r = r + value;
//  	g = g + value;
//  	b = b + value;

	return RGB(r, g, b);
}

TColor GetShadeColor(TColor clr, int value)
{
	int r, g, b;


//  clr := ColorToRGB(clr);
  	r = R(clr);
  	g = G(clr);
  	b = B(clr);

  	r = r - value;
  	if( r < 0 ) r = 0;
  	if( r > 255 ) r = 255;

  	g = (g - value) + 2;
  	if( g < 0 ) g = 0;
  	if( g > 255 ) g = 255;

  	b = (b - value);
  	if( b < 0 ) b = 0;
  	if( b > 255 ) b = 255;

	return RGB(r, g, b);
}

void DrawGradient(TCanvas *ACanvas, TRect ARect, bool IsRightToLeft)
{
 	int i, v;
	TRect fRect;

  	fRect = ARect;
  	v = 0;
  	if(IsRightToLeft)
  	{
    	fRect.left = fRect.right() - 1;
    	for(i = ARect.right(); i >= ARect.left; i--)
    	{
      		if (fRect.left < ARect.right() &&
        		fRect.left > ARect.right() - 24 + 5)
        		v += 3;
      		else
        		v++;

      		if(v > 96)
      			v = 96;
      		ACanvas->Brush->Color = NewColor(clBlue, v);
      		ACanvas->FillRect(fRect);

      		fRect.left = fRect.left - 1;
      		fRect.width = 1;
    	}
	}
  	else
  	{
    	fRect.width = 1;
    	for(i = ARect.left; i <= ARect.right(); i++)
    	{
//      		if(fRect.left > ARect.left &&
//        		fRect.left < ARect.left + 24 + 5)
//        		v += 1;
//      		else
        		v++;

//      		if(v > 96)
//      			v = 96;
//      		ACanvas->Brush->Color = NewColor(clBlue, v);
      		ACanvas->Pen->Color = NewColor(clBlue, v);
      		ACanvas->Line(fRect.left, fRect.top, fRect.left, fRect.bottom());
//      		ACanvas->FillRect(fRect);

      		fRect.left = fRect.left + 1;
      		fRect.width = 1;
    	};
  	};
}
//---------------------------------------------------------------------------
void DrawEdge(TCanvas *canvas, const TRect &rect, TEdgeStyle EdgeInner, TEdgeStyle EdgeOuter, int EdgeBorders, int EdgeWidth)
{
	TRect RInner, R;
	R = rect;
  	RInner = R;
  	int oldwidth = canvas->Pen->Width;

  	canvas->Pen->Width = EdgeWidth;
  	RInner.inflate(-EdgeWidth-2, -EdgeWidth-2);

  	if(EdgeBorders & ebTop)
  	{
    	switch(EdgeOuter)
    	{
      		case esRaised: canvas->Pen->Color = clBtnHighlight; break;
      		case esLowered: canvas->Pen->Color = clBtnShadow; break;
      		case esNone: break;
    	};
    	if(EdgeOuter != esNone)
    	{
    		if(rect.rx > 0 && rect.ry > 0)
      		{
      			canvas->Arc(R.left, R.top, R.rx * 2, R.ry * 2, 90, 45);
				canvas->Line(R.left + R.rx, R.top, R.left + R.width - R.rx, R.top);
				canvas->Arc(R.left + R.width - R.rx * 2, R.top, R.rx * 2, R.ry * 2, 90, -45);
      		}
      		else
       			canvas->Line(R.left, R.top, R.right(), R.top);
		}

    	switch(EdgeInner)
    	{
      		case esRaised: canvas->Pen->Color = clBtnHighlight; break;
      		case esLowered: canvas->Pen->Color = clBtnShadow; break;
      		case esNone: break;
    	};
    	if(EdgeInner != esNone)
    	{
    		if(rect.rx > 0 && rect.ry > 0)
      		{
      			canvas->Arc(RInner.left, RInner.top, RInner.rx * 2, RInner.ry * 2, 90, 45);
				canvas->Line(RInner.left + RInner.rx, RInner.top, RInner.left + RInner.width - RInner.rx, RInner.top);
				canvas->Arc(RInner.left + RInner.width - RInner.rx * 2, RInner.top, RInner.rx * 2, RInner.ry * 2, 90, -45);
      		}
      		else
      			canvas->Line(RInner.left, RInner.top, RInner.right(), RInner.top);
    	};
  	};

	if(EdgeBorders & ebLeft)
  	{
    	switch(EdgeOuter)
      	{
        	case esRaised: canvas->Pen->Color = clBtnHighlight; break;
        	case esLowered: canvas->Pen->Color = clBtnShadow; break;
        	case esNone: break;
      	};
      	if(EdgeOuter != esNone)
      	{
			if(rect.rx > 0 && rect.ry > 0)
      		{
      			canvas->Arc(R.left, R.top, R.rx * 2, R.ry * 2, 135, 45);
				canvas->Line(R.left, R.top + R.height - R.ry, R.left, R.top + R.ry);
				canvas->Arc(R.left, R.top + R.height - R.ry * 2, R.rx * 2, R.ry * 2, -135, -45);
      		}
      		else
        		canvas->Line(R.left, R.top, R.left, R.bottom());
		}
      	switch(EdgeInner)
      	{
        	case esRaised: canvas->Pen->Color = clBtnHighlight; break;
        	case esLowered: canvas->Pen->Color = clBtnShadow; break;
        	case esNone: break;
      	};
      	if(EdgeInner != esNone)
      	{
			if(rect.rx > 0 && rect.ry > 0)
      		{
      			canvas->Arc(RInner.left, RInner.top, RInner.rx * 2, RInner.ry * 2, 135, 45);
				canvas->Line(RInner.left, RInner.top + RInner.height - RInner.ry, RInner.left, RInner.top + RInner.ry);
				canvas->Arc(RInner.left, RInner.top + RInner.height - RInner.ry * 2, RInner.rx * 2, RInner.ry * 2, -135, -45);
      		}
      		else
       			canvas->Line(RInner.left, RInner.top, RInner.left, RInner.bottom());
      	}
	};

	if(EdgeBorders & ebRight)
  	{
    	switch(EdgeOuter)
      	{
        	case esRaised: canvas->Pen->Color = clBlack; break;
        	case esLowered: canvas->Pen->Color = clBtnHighlight; break;
        	case esNone: break;
      	};
      	if(EdgeOuter != esNone)
      	{
      		if(rect.rx > 0 && rect.ry > 0)
      		{
				canvas->Arc(R.left + R.width - R.rx * 2, R.top, R.rx * 2, R.ry * 2, 0, 45);
      			canvas->Line(R.left + R.width, R.top + R.ry, R.left + R.width, R.top + R.height - R.ry);
      			canvas->Arc(R.left + R.width - R.rx * 2, R.top + R.height - R.ry * 2, R.rx * 2, R.ry * 2, 0, -45);
      		}
      		else
        		canvas->Line(R.right(), R.top, R.right(), R.bottom());
		}
      	switch(EdgeInner)
      	{
        	case esRaised: canvas->Pen->Color = clBtnShadow; break;
        	case esLowered: canvas->Pen->Color = clBtnHighlight; break;
        	case esNone: break;
      	};
      	if(EdgeInner != esNone)
      	{
      		if(rect.rx > 0 && rect.ry > 0)
      		{
				canvas->Arc(RInner.left + RInner.width - RInner.rx * 2, RInner.top, RInner.rx * 2, RInner.ry * 2, 0, 45);
      			canvas->Line(RInner.left + RInner.width, RInner.top + RInner.ry, RInner.left + RInner.width, RInner.top + RInner.height - RInner.ry);
      			canvas->Arc(RInner.left + RInner.width - RInner.rx * 2, RInner.top + RInner.height - RInner.ry * 2, RInner.rx * 2, RInner.ry * 2, 0, -45);
      		}
      		else
        		canvas->Line(RInner.right(), RInner.top, RInner.right(), RInner.bottom());
      	};
	};

  	if(EdgeBorders & ebBottom)
  	{
    	if(EdgeOuter == esRaised)
        	canvas->Pen->Color = clBlack;
      	else if(EdgeOuter == esLowered)
        	canvas->Pen->Color = clBtnHighlight;
      	if(EdgeOuter != esNone)
      	{
      		if(rect.rx > 0 && rect.ry > 0)
      		{
      			canvas->Arc(R.left + R.width - R.rx * 2, R.top + R.height - R.ry * 2, R.rx * 2, R.ry * 2, -90, 45);
				canvas->Line(R.left + R.rx, R.top + R.height, R.left + R.width - R.rx, R.top + R.height);
				canvas->Arc(R.left, R.top + R.height - R.ry * 2, R.rx * 2, R.ry * 2, -90, -45);
      		}
      		else
        		canvas->Line(R.left, R.bottom(), R.right(), R.bottom());
		}
      	switch(EdgeInner)
      	{
        	case esRaised: canvas->Pen->Color = clBtnShadow; break;
        	case esLowered: canvas->Pen->Color = clBtnHighlight; break;
        	case esNone: break;
      	};
      	if(EdgeInner != esNone)
      	{
      		if(rect.rx > 0 && rect.ry > 0)
      		{
      			canvas->Arc(RInner.left + RInner.width - RInner.rx * 2, RInner.top + RInner.height - RInner.ry * 2, RInner.rx * 2, RInner.ry * 2, -90, 45);
				canvas->Line(RInner.left + RInner.rx, RInner.top + RInner.height, RInner.left + RInner.width - RInner.rx, RInner.top + RInner.height);
				canvas->Arc(RInner.left, RInner.top + RInner.height - RInner.ry * 2, RInner.rx * 2, RInner.ry * 2, -90, -45);
      		}
      		else
        		canvas->Line(RInner.left, RInner.bottom(), RInner.right(), RInner.bottom());
      	};
	};
	canvas->Pen->Width = oldwidth;
}
//---------------------------------------------------------------------------
void Frame3D(TCanvas *canvas, const TRect &arect, TColor topcolor, TColor bottomcolor, int dwidth)
{
	int rx, ry, dx, dy, x, y;
	uint_t width, height;
	TPoint topright, bottomleft, tmppoint;
   	TPoint points[3];
	TRect rect = arect;

   	canvas->Pen->Width = 1;
   	while(dwidth > 0)
   	{
    	if(dwidth == 1 && bottomcolor > topcolor)
      	{
//      	topcolor -= 0x00303030;
      		bottomcolor -= 0x00404040;
      	}
      	x = rect.left;
		y = rect.top;
		width = rect.width;
		height = rect.height;
		dx = rect.rx;
		dy = rect.ry;
		rx = dx * 2;
		ry = dy * 2;
      	dwidth--;
      	canvas->Pen->Color = topcolor;
      	if(rect.rx > 0 && rect.ry > 0)
      	{
      		canvas->Arc(x, y, rx, ry, 90, 90);
			canvas->Line(x + dx, y, x + width - dx, y);
			canvas->Arc(x + width - rx, y, rx, ry, 45, 45);
			canvas->Line(x, y + height - dy, x, y + dy);
			canvas->Arc(x, y + height - ry, rx, ry, -135, -45);
      	}
      	else
      	{
      		topright.x = rect.right();
      		topright.y = rect.top;
      		bottomleft.x = rect.left;
      		bottomleft.y = rect.bottom();
      		points[0] = bottomleft;
      		tmppoint.x = rect.left;
      		tmppoint.y = rect.top;
      		points[1] = tmppoint;
      		points[2] = topright;
      		canvas->Polyline(points, 3);
      	}
      	canvas->Pen->Color = bottomcolor;
      	if(rect.rx > 0 && rect.ry > 0)
      	{
      		canvas->Arc(x + width - rx, y, rx, ry, 45, -45);
      		canvas->Line(x + width, y + dy, x + width, y + height - dy);
      		canvas->Arc(x + width - rx, y + height - ry, rx, ry, 0, -90);
			canvas->Line(x + dx, y + height, x + width - dx, y + height);
			canvas->Arc(x, y + height - ry, rx, ry, -90, -45);
      	}
      	else
      	{
      		points[0] = topright;
      		tmppoint.x = rect.right();
      		tmppoint.y = rect.bottom();
      		points[1] = tmppoint;
      		points[2] = bottomleft;
      		canvas->Polyline(points, 3);
      	}
      	rect.inflate(-1,-1);
   	};
}
//---------------------------------------------------------------------------
// DrawButtonFace - returns the remaining usable area inside the Client rect.}
TRect DrawButtonFace(TCanvas *canvas, const TRect &Client, bool IsDown,
                 bool IsFocused, bool Flat, TColor FillColor)
{
	TRect R, Result;

  	R = Client;
 // 	canvas->Pen->Width = 0;
  	canvas->Brush->Color = FillColor;
  	canvas->FillRect(R);
//  	canvas->Rectangle(R);
 // 	return Result;
  	if(IsDown)
  	{
    	if(!Flat)
        	DrawEdge(canvas, R, esLowered, esNone, ebTop | ebLeft);
      	DrawEdge(canvas, R, esNone, esLowered, ebBottom | ebRight);
      	DrawEdge(canvas, R, esNone, esLowered, ebTop | ebLeft);
  	}
  	else
  	{
    	DrawEdge(canvas, R, esNone, esRaised, ebBottom | ebRight);
      	if(!Flat)
        	DrawEdge(canvas, R, esRaised, esNone, ebBottom | ebRight);
      	R.height--; R.width--;
     	DrawEdge(canvas, R, esNone, esRaised, ebLeft | ebTop);
  	};
  	if(IsFocused)
  	{
    	R.inflate(-1, -1);
      	canvas->Rectangle(R.left, R.top, R.width, R.height);
  	};

  	Result = TRect(Client.left + 1, Client.top + 1, Client.width - 2,
    				Client.height - 2);
  	if(IsDown)
    	Result.offset(1, 1);
  	return Result;
}
//---------------------------------------------------------------------------
void DrawDisabledText (TCanvas *Canvas, const TString &text, TRect &rect, TFormatText *format)
{
    TColor old = Canvas->Font->Color;
	Canvas->Brush->Color = clNone;
	rect.offset( 1, 1);
  	Canvas->Font->Color = clBtnHighlight;
	Canvas->DrawText(rect, text, format);
  	Canvas->Font->Color = clBtnShadow;
  	rect.offset( -1, -1);
  	Canvas->DrawText(rect, text, format);
  	Canvas->Font->Color = old;
}
//---------------------------------------------------------------------------
const char cHotkeyPrefix = '&';
const char cLineCaption = '-';
const char *cDialogSuffix = "...";
//---------------------------------------------------------------------------
//  TMenuItem
//---------------------------------------------------------------------------
TMenuItem :: TMenuItem()
{
	Color = clMenuBar;
	FMinWidth = 0;
	FMinHeight= 18;
	FGutterColor = clBtnFace; //{серая полоска}
}
//---------------------------------------------------------------------------
/*
TMenuItem :: TMenuItem(TWinControl *AParent) : TCommonMenuItem(AParent)
{
	Color = clMenuBar;
	FGutterColor = clBtnFace;
}
*/
//---------------------------------------------------------------------------
void TMenuItem :: MeasureItem(uint_t &width, uint_t &height)
{
	TRect rect;
	TMenu *tm = GetParentMenu();
	int gutter;

	if(tm->Toplevel)
		gutter = 0;
	else
	{
		if(tm->Images)
			gutter = tm->Images->ImageWidth + 9; // {четыре точки до картинки + картинка + пять после}
		else
			if(IsLine())
				gutter = max(Canvas.TextHeight("W"), FMinHeight) + 4;
			else
				gutter = FMinHeight + 4; // {ширина = высоте + 2 + 2 точки}
	}
	if(IsLine())
    {
        width = 16;
		height = 8;
    }
    else
	{
			rect = Canvas.TextExtents(Caption);
			width = gutter + (rect.width > 16 ? rect.width : 16);// + 16;
			height = (rect.height > 16 ? rect.height : 16) + 6;
			if((short)ImageIndex != -1)
			{
				if(tm && tm->Images)
				{
					width += tm->Images->ImageWidth;
					if(tm->Images->ImageHeight + 5 > height)
						height = tm->Images->ImageHeight + 6;
				}
			}
			if(ShortCut)
				width += (Canvas.TextWidth(ShortCutToText(ShortCut)) + 20);
	}
	TCommonMenuItem :: MeasureItem(width, height);
}
//---------------------------------------------------------------------------
void TMenuItem :: Paint()
{
	int x = 9;
	int gutter;
	TColor guttercolor, fontcolor, fillcolor, bordercolor;
	TRect rect = ClientRect; // BoundsRect;

//	TMenu *tm = GetParentMenu();
TMenu *tm = dynamic_cast<TMenu *>(*Parent);

	if(tm->Images)
		gutter = tm->Images->ImageWidth + 9; // {четыре точки до картинки + картинка + пять после}
    else
		if(IsLine())
			gutter = max(Canvas.TextHeight("W"), FMinHeight) + 4;
		else
			gutter = FMinHeight + 4; // {ширина = высоте + 2 + 2 точки}

    Canvas.Pen->Width = 1;

	if(CheckState(csLButtonDown))
	{
		fillcolor = tm->SelectColor;
		fontcolor = tm->SelectTextColor;
		guttercolor = tm->SelectColor;
		bordercolor = fillcolor;
	}
	else if(CheckState(csMouseOver)) // || Active)
	{
		fillcolor = clSkyBlue;
		fontcolor = clBlack;
		guttercolor = Canvas.LightenColor(clSkyBlue, -20);
		bordercolor = Canvas.LightenColor(clSkyBlue, -60);
	}
	else
	{
		fillcolor = Color;
		fontcolor = clBlack;
		guttercolor = Canvas.LightenColor(Color, -20);
		bordercolor = fillcolor;
	}

	if(tm->Toplevel) // {если это полоска основного меню}
	{
	    Canvas.Pen->Color = bordercolor;
		Canvas.Brush->Color = fillcolor;
		Canvas.Font->Color = fontcolor;
		Canvas.FillRect(rect);
		Canvas.Rectangle(rect.left + 1, rect.top + 1, rect.width - 1, rect.height - 3);
	}
	else
	{
		Canvas.Brush->Color = guttercolor;
		Canvas.FillRect(0, 0, gutter, Height);
		Canvas.Brush->Color = fillcolor;
		Canvas.FillRect(gutter, 0, Width - gutter, Height);
		Canvas.Font->Color = fontcolor;
		if(CheckState(csMouseOver))
		{
            Canvas.Pen->Color = bordercolor;
		    Canvas.Rectangle(rect.left + 1, rect.top + 1, rect.width - 2, rect.height - 1);
		}
		x += gutter;
	}
    if(IsLine())
    {
        rect = TRect(gutter + 5, Height / 2, Width - 12 - gutter, 5);
        DrawEdge(&Canvas, rect, esRaised, esLowered, ebTop);
    }
	else
	{
			if(tm->Images && (short)ImageIndex > -1)
				tm->Images->DrawRect(&Canvas, BoundsRect, 5, 0, ImageIndex, alVCenter);
			Canvas.TextRect(rect, x, 0, Caption, alVCenter);
			if(ShortCut)
				Canvas.TextRect(rect, -10, 0, ShortCutToText(ShortCut), alRightCenter);
	}
//    Canvas.Pen->Width = 0;

/*
	FMenuColor = m_Canvas->LightenColor(clBtnFace, 85);
	FSelectedColor = m_Canvas->LightenColor(clHighlight, 65);//{выделенный пункт меню}
	FSelLightColor = m_Canvas->LightenColor(clHighlight, 75);

	if(tm->Images)
		gutter = tm->Images->ImageWidth + 9; // {четыре точки до картинки + картинка + пять после}
    else
		if(Kind == miLine)
			gutter = max(m_Canvas->TextHeight("W"), FMinHeight) + 4;
		else
			gutter = m_Height + 4; // {ширина = высоте + 2 + 2 точки}

    m_Canvas->Pen->Color = clBlack;

    if(CheckState(csLButtonDown)) // {если пункт меню выделен}
    {
		m_Canvas->Brush->Color = FSelectedColor;
		m_Canvas->FillRect(BoundsRect);
		m_Canvas->Rectangle(BoundsRect);
	}
    else
    {
		if(tm->TopLevel) // {если это полоска основного меню}
		{
			if(CheckState(csMouseOver)) // {если мышь над пунктом меню}
			{
				m_Canvas->Pen->Color = clBtnShadow;
				m_Canvas->Brush->Color = FSelectedColor;
				m_Canvas->FillRect(BoundsRect);
				m_Canvas->Rectangle(BoundsRect);
			}
			else
			{
				m_Canvas->Brush->Color = clBtnFace;
				m_Canvas->FillRect(BoundsRect);
			}
		}
		else
		{   // {ничем не примечательный пункт меню}
			m_Canvas->Brush->Color = FGutterColor; // {полоска}
			m_Canvas->FillRect(m_Left, m_Top, gutter, m_Height);
			m_Canvas->Brush->Color = Color;
			m_Canvas->FillRect(gutter, m_Top, m_Width - gutter, m_Height);
		};

		if(Checked)
		{ // {подсвечиваем чекнутый пункт меню}
			m_Canvas->Brush->Color = FSelLightColor;
			if( tm->Images && ImageIndex > -1 )
			{
								//{если имеется картинка то рисуем квадратик вокруг нее}
//				RoundRect(ARect.Left + 2, ARect.Top, Gutter - 2 - 1,
//				ARect.Bottom, _RectEl[RadioItem], _RectEl[RadioItem]);
			}
			else // {рисуем просто галочку}
			{
//				Rectangle((ARect.Left + 2 + Gutter - 1 - 2) div 2 - 8,
//				(ARect.Top + ARect.Bottom) div 2 - 8,
//				(ARect.Left + 2 + Gutter - 1 - 2) div 2 + 8,
//				(ARect.Top + ARect.Bottom) div 2 + 8);
//				Draw((ARect.Left + 2 + Gutter - 1 - 2 - BmpCheck[RadioItem].Width) div 2,
//				(ARect.Top + ARect.Bottom - BmpCheck[RadioItem].Height) div 2,
//				BmpCheck[RadioItem]);
			}
		};

		if( tm->Images && ImageIndex > -1 && !TopLevel)
		{
			if(Enabled)
			{
				ImageList.Draw(ACanvas, ARect.Left + 4,
				(ARect.Top + ARect.Bottom - ImageList.Height) div 2,
				ImageIndex, True); // {рисуем цветную картинку}
			}
			else
			{  // {рисуем погасшую картинку}
				GetBmpFromImgList(FMonoBitmap, ImageList, ImageIndex);
				DoDrawMonoBmp(ACanvas, clBtnShadow, ARect.Left + 4,
				(ARect.Top + ARect.Bottom - ImageList.Height) div 2);
			};
		}
		with Font do
		begin
			if (odDefault in State) then Style := [fsBold];
			if (odDisabled in State) then Color := clGray
			else Color := clBlack;
		end;

    Brush.Style := bsClear;
    if TopLevel then {пусто}
    else Inc(ARect.Left, Gutter + 5); {отступ для текста}

    if IsLine then {если разделитель}
    begin
      Pen.Color := clBtnShadow;
      MoveTo(ARect.Left, ARect.Top + (ARect.Bottom - ARect.Top) div 2);
      LineTo(ARect.Right, ARect.Top + (ARect.Bottom - ARect.Top) div 2);
    end else
    begin {текст меню}
      Windows.DrawText(Handle, PChar(Caption), Length(Caption), ARect,
        _Flags or _FlagsTopLevel[TopLevel]);
      if ShortCut <> 0 then {разпальцовка}
      begin
        Dec(ARect.Right, 5);
        Windows.DrawText(Handle, PChar(ShortCutToText(ShortCut)),
          Length(ShortCutToText(ShortCut)), ARect,
          _Flags or _FlagsShortCut);
      end
    end
  end */
}
void TMenuItem :: DoDrawText(const TString &ACaption,
  TRect &Rect, bool Selected, int Flags)
{
    TString text;
    TRect R;
    TMenu *ParentMenu;

    ParentMenu = GetParentMenu();
/*    if(ParentMenu && ParentMenu->IsRightToLeft)
    {
        if Flags and DT_LEFT = DT_LEFT then
            Flags := Flags and (not DT_LEFT) or DT_RIGHT
        else if Flags and DT_RIGHT = DT_RIGHT then
            Flags := Flags and (not DT_RIGHT) or DT_LEFT;
        Flags := Flags or DT_RTLREADING;
    } */
    text = ACaption;
    if((Flags && DT_CALCRECT) && (text.IsEmpty() || (text[0] == cHotkeyPrefix) && (text[1] == '\0')))
       text = text + ' ';

    if(text == TString(cLineCaption))
    {
        if((Flags && DT_CALCRECT) == 0)
        {
            R = Rect;
            R.top += 4;
//            DrawEdge(Handle, R, EDGE_ETCHED, BF_TOP);
            DrawEdge(&Canvas, R, esRaised, esLowered, ebTop);
        }
    }
    else
    {
      Canvas.Brush->Style = bsClear;
      if(Default)
        Canvas.Font->Style = Canvas.Font->Style | fsBold;
      if(!Enabled)
      {
        if(!Selected)
        {
          Rect.offset(1, 1);
          Canvas.Font->Color = clBtnHighlight;
 //         DrawText(Handle, PChar(Text), Length(Text), Rect, Flags);
          Canvas.TextRect(Rect, 0, 0, text, alVCenter);
          Rect.offset(-1, -1);
        }
        if(Selected && (clHighlight == clBtnShadow))
            Canvas.Font->Color = clBtnHighlight;
        else
            Canvas.Font->Color = clBtnShadow;
      }
 //     DrawText(Handle, PChar(Text), Length(Text), Rect, Flags);
      Canvas.TextRect(Rect, 0, 0, text, alVCenter);
    }
}

//---------------------------------------------------------------------------
//  PopupMenu
//---------------------------------------------------------------------------
TPopupMenu :: TPopupMenu()
{
	Init();
}
//---------------------------------------------------------------------------
void TPopupMenu :: Init()
{
    SubMenuClass = __classid(TPopupMenu);
	BorderWidth = 1;
	BorderColor = clGray;
}
//---------------------------------------------------------------------------
//  MainMenu
//---------------------------------------------------------------------------
TMainMenu :: TMainMenu()
{
	Init();
}
//---------------------------------------------------------------------------
TMainMenu :: TMainMenu(TWinControl *AParent) : TCustomMainMenu(AParent)
{
	Init();
}
//---------------------------------------------------------------------------
void TMainMenu :: Init()
{
    SubMenuClass = __classid(TPopupMenu);
	BorderWidth = 1;
	BorderColor = clGray;
}
//---------------------------------------------------------------------------
//  TLabel
//---------------------------------------------------------------------------
TLabel :: TLabel()
{
    Init();
}
//---------------------------------------------------------------------------
TLabel :: ~TLabel()
{
}
//---------------------------------------------------------------------------
__fastcall TLabel :: TLabel(const TLabel &a) : TCommonLabel(a)
{
	Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TLabel & TLabel :: operator=(const TLabel &a)
{
	if(this == &a)
		return *this;
    TCommonLabel :: operator=(a);
    Assign(a);
	return *this;
}
//---------------------------------------------------------------------------
void TLabel :: Init()
{
    Width = 100;
}
//---------------------------------------------------------------------------
void __fastcall TLabel :: Assign(const TLabel &obj)
{
}
//---------------------------------------------------------------------------
void TLabel :: AdjustSize()
{
//    if( AutoSize )
//    {
        TRect rect = BoundsRect;
 //       m_Canvas->Font = Font;
        m_FormatText.Options = BIT_ON(m_FormatText.Options, DT_CALCRECT);
        Canvas.DrawText(rect, Caption, &m_FormatText);
        m_FormatText.Options = BIT_OFF(m_FormatText.Options, DT_CALCRECT);
        BoundsRect = rect;
//    }
}
//---------------------------------------------------------------------------
void TLabel :: Paint()
{
    TRect rect = ClientRect;
//    m_Canvas->Font = Font;
//    Canvas.Brush->Style = bsClear;
	Canvas.Brush->Color = Color;
	Canvas.Pen->Color = clRed;
//	Canvas.FillRect(rect);
	Canvas.Rectangle(rect);
//	Canvas.TextOut(0,0, Caption);
	if(Enabled)
		Canvas.DrawText(rect, Caption, &m_FormatText);
	else
		DrawDisabledText(&Canvas, Caption, rect, &m_FormatText);
}


// TCustomLabel

TCustomLabel :: TCustomLabel()
{
    Init();
}
//---------------------------------------------------------------------------
TCustomLabel :: ~TCustomLabel()
{
}
//---------------------------------------------------------------------------
__fastcall TCustomLabel :: TCustomLabel(const TCustomLabel &a) : TControl(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TCustomLabel & TCustomLabel :: operator=(const TCustomLabel &a)
{
    if(this == &a)
        return *this;
    TControl :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TCustomLabel :: Init()
{
    Width = 65;
    Height = 17;
    m_ShowAccelChar = true;
    m_WordWrap = false;
    m_Layout = tlTop;
    m_FocusControl = NULL;
    m_Alignment = taLeftJustify;
}
//---------------------------------------------------------------------------
__fastcall void TCustomLabel :: Assign(const TCustomLabel &obj)
{
}
//---------------------------------------------------------------------------
void TCustomLabel :: DoDrawText(TRect &Rect, int Flags)
{
    TString Text;

    Text = Caption;
    if( (Flags and DT_CALCRECT) and ((Text.IsEmpty() or m_ShowAccelChar and
    (Text[0] == '&') and (Text[2] == '\0')) ) )
            Text = Text + " ";
    if(!m_ShowAccelChar)
        Flags = Flags or DT_NOPREFIX;
    Canvas.Font = Font;
    if(!Enabled)
    {
        OffsetRect(Rect, 1, 1);
        Canvas.Font->Color = clBtnHighlight;
        DrawText(Canvas.Handle, PChar(Text), Length(Text), Rect, Flags);
        OffsetRect(Rect, -1, -1);
        Canvas.Font.Color := clBtnShadow;
        DrawText(Canvas.Handle, PChar(Text), Length(Text), Rect, Flags);
    }
    else
        DrawText(Canvas.Handle, PChar(Text), Length(Text), Rect, Flags);
}

void TCustomLabel :: Paint()
{
const word_t Alignments[3] = { DT_LEFT, DT_RIGHT, DT_CENTER};
const word_t WordWraps[2] = { 0, DT_WORDBREAK};

    TRect Rect, CalcRect;
    int DrawStyle;

    if(!Transparent)
    {
        Canvas.Brush->Color = Color;
        Canvas.Brush->Style = bsSolid;
        FillRect(ClientRect);
    };
    Canvas.Brush->Style = bsClear;
    Rect = ClientRect;
    // DoDrawText takes care of BiDi alignments
    DrawStyle = DT_EXPANDTABS or WordWraps[m_WordWrap] or Alignments[FAlignment];
    // Calculate vertical layout
    if(m_Layout != tlTop)
    {
      CalcRect = Rect;
      DoDrawText(CalcRect, DrawStyle or DT_CALCRECT);
      if(m_Layout == tlBottom)
        OffsetRect(Rect, 0, Height - CalcRect.Bottom);
      else
        OffsetRect(Rect, 0, (Height - CalcRect.Bottom) / 2);
    }
    DoDrawText(Rect, DrawStyle);
}

void TCustomLabel :: AdjustSize()
{
    const word_t WordWraps[2] = {0, DT_WORDBREAK};

    HDC DC;
    int X;
    TRect Rect;
    TAlignment AAlignment;

    if(AutoSize)
    {
        Rect = ClientRect;
        DC = GetDC(0);
        Canvas.Handle = DC;
        DoDrawText(Rect, (DT_EXPANDTABS or DT_CALCRECT) or WordWraps[FWordWrap]);
        Canvas.Handle = 0;
        ReleaseDC(0, DC);
        X = Left;
        AAlignment = FAlignment;
        if(AAlignment == taRightJustify)
            X += (Width - Rect.right());
        SetBounds(X, Top, Rect.Right, Rect.Bottom);
    }
}

void TCustomLabel :: set_Alignment(TAlignment Value)
{
  if(m_Alignment != Value)
  {
    m_Alignment = Value;
    Invalidate();
  };
}

void TCustomLabel :: set_FocusControl(TWinControl *Value)
{
    FFocusControl = Value;
//    if Value <> nil then Value.FreeNotification(Self);
}

void TCustomLabel :: set_ShowAccelChar(bool Value)
{
    if(m_ShowAccelChar != Value)
    {
        m_ShowAccelChar = Value;
        Invalidate();
    };
}

void TCustomLabel :: set_Layout(TTextLayout Value)
{
    if( m_Layout != Value)
    {
        m_Layout = Value;
        Invalidate();
    };
}

void TCustomLabel :: set_WordWrap(bool Value)
{
  if(m_WordWrap != Value)
  {
    m_WordWrap = Value;
    AdjustSize();
    Invalidate();
  };
}
/*
procedure TCustomLabel.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited Notification(AComponent, Operation);
  if (Operation = opRemove) and (AComponent = FFocusControl) then
    FFocusControl := nil;
end;

procedure TCustomLabel.CMTextChanged(var Message: TMessage);
begin
  Invalidate;
  AdjustBounds;
end;

procedure TCustomLabel.CMFontChanged(var Message: TMessage);
begin
  inherited;
  AdjustBounds;
end;

procedure TCustomLabel.CMDialogChar(var Message: TCMDialogChar);
begin
  if (FFocusControl <> nil) and Enabled and ShowAccelChar and
    IsAccel(Message.CharCode, Caption) then
    with FFocusControl do
      if CanFocus then
      begin
        SetFocus;
        Message.Result := 1;
      end;
end;
*/
//---------------------------------------------------------------------------
//  SpeedButton
//---------------------------------------------------------------------------
TSpeedButton :: TSpeedButton()
{
	Down = false;
	FState = bsUp;
	FGroupIndex = 0;
//	StyleOn(csButton);
//	StyleOn(csFollowMouse);
}
//---------------------------------------------------------------------------
TSpeedButton :: TSpeedButton(TWinControl *AParent) // : TControl(AParent)
{
	Down = false;
	FState = bsUp;
	FGroupIndex = 0;
//	StyleOn(csButton);
//	StyleOn(csFollowMouse);
}
//---------------------------------------------------------------------------
void TSpeedButton :: Paint()
{
//  TBrushStyle BrushStyle;
  TRect PaintRect;

  if(!Enabled)
  {
      FState = bsDisabled;
  }
  else
  {
  	if(FState == bsDisabled)
  	{
    	if(Down  && (FGroupIndex != 0))
      		FState = bsExclusive;
    	else
      		FState = bsUp;
    }
  }
//  Canvas->Font = Self.Font;
//  PaintRect = BoundsRect;
//  if(Transparent)
//    BrushStyle = bsClear;
////  else
//    BrushStyle = bsSolid;

	int width = 2;
//	Canvas.Brush->Color = Color;
	if(CheckState(csMouseOver))
		Canvas.Brush->Color = Canvas.LightenColor(Color, 20);
	else
		Canvas.Brush->Color = Color;

	TRect rect = ClientRect;
	Canvas.FillRect(rect);
	TColor shadow, high;
	high = Canvas.LightenColor(Color, 30);
	shadow = Canvas.LightenColor(Color, -90);

	if(CheckState(csLButtonDown))
	{
		Frame3D(&Canvas, rect, shadow, high, width);
		Canvas.Pen->Color = clWindowFrame;
	}
   else
   {
   		Frame3D(&Canvas, rect, high, shadow, width);
   		Canvas.Pen->Color = Color;
   }
   		Canvas.TextRect(rect, Caption, alCenter);

/*
  if(!Flat)
    w->Canvas->DrawButtonFace(PaintRect, 1, (FState == bsDown) || (FState == bsExclusive), false,
      false, Color);
  else
  {
    if(!Transparent)
    {
      w->Canvas->Brush->Color = Color;
      w->Canvas->FillRect(PaintRect);
    };
    if(FState == bsDown || FState == bsExclusive)
      w->Canvas->DrawEdge(PaintRect, esNone, esLowered, ebRect);
    else if(ControlState.count(csMouseOver) && (FState == bsUp))
           w->Canvas->DrawEdge(PaintRect, esNone, esRaised, ebRect);
  };
  if(FState == bsDown || FState == bsExclusive)
  {
    if((FState == bsExclusive) && (!Flat || !ControlState.count(csMouseOver)))
    {
      PaintRect.top++;
      PaintRect.left++;
//      Canvas.Brush.Bitmap := AllocPatternBitmap(clBtnFace, clBtnHighlight);
      w->Canvas->FillRect(PaintRect);
    };
    offset.x = 1;
    offset.y = 1;
  }
  else
  {
    offset.x = 0;
    offset.y = 0;
  };
  */
//  TButtonGlyph(FGlyph).Draw(Canvas, PaintRect, Offset, Caption, FLayout, FMargin,
//    FSpacing, FState, Transparent);
}
//---------------------------------------------------------------------------
//  TButton
//---------------------------------------------------------------------------
TButton :: TButton()
{
    Init();
}
//---------------------------------------------------------------------------
TButton :: ~TButton()
{
}
//---------------------------------------------------------------------------
__fastcall TButton :: TButton(const TButton &a) : TCommonButton(a)
{
	Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TButton & TButton :: operator=(const TButton &a)
{
	if(this == &a)
		return *this;
    TCommonButton :: operator=(a);
    Assign(a);
	return *this;
}
//---------------------------------------------------------------------------
void TButton :: Init()
{
	Width = 75;
	Height = 25;
}
//---------------------------------------------------------------------------
void __fastcall TButton :: Assign(const TButton &obj)
{
}
//---------------------------------------------------------------------------
//  TCheckBox
//---------------------------------------------------------------------------
TCheckBox :: TCheckBox()
{
    Init();
}
//---------------------------------------------------------------------------
TCheckBox :: ~TCheckBox()
{
}
//---------------------------------------------------------------------------
__fastcall TCheckBox :: TCheckBox(const TCheckBox &a) : TCommonCheckBox(a)
{
	Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TCheckBox & TCheckBox :: operator=(const TCheckBox &a)
{
	if(this == &a)
		return *this;
    TCommonCheckBox :: operator=(a);
    Assign(a);
	return *this;
}
//---------------------------------------------------------------------------
void TCheckBox :: Init()
{
	Width = 99;
  	Height = 19;
}
//---------------------------------------------------------------------------
void __fastcall TCheckBox :: Assign(const TCheckBox &obj)
{
}
//---------------------------------------------------------------------------
//	TComboBox
//---------------------------------------------------------------------------
TComboBox :: TComboBox()
{
    Init();
}
//---------------------------------------------------------------------------
TComboBox :: ~TComboBox()
{
}
//---------------------------------------------------------------------------
__fastcall TComboBox :: TComboBox(const TComboBox &a) : TCommonComboBox(a)
{
	Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TComboBox & TComboBox :: operator=(const TComboBox &a)
{
	if(this == &a)
		return *this;
    TCommonComboBox :: operator=(a);
    Assign(a);
	return *this;
}
//---------------------------------------------------------------------------
void TComboBox :: Init()
{
    Width = 145;
    Height = 20;
    BorderColor = clBlack;
    ArrowColor = clBlack;
    ButtonColor = clBtnFace;
}
//---------------------------------------------------------------------------
void __fastcall TComboBox :: Assign(const TComboBox &obj)
{
}
//---------------------------------------------------------------------------
void TComboBox :: Paint()
{
    TRect r = ClientRect;
    Frame3D(&Canvas, r, clBtnShadow, clBtnHighlight, 1);

    TRect rect = TRect(r.width - r.height, r.top + 1, r.height - 2, r.height - 2);
    if(CheckState(csLButtonDown))
        DrawButtonArrow(&Canvas, rect, arrowDown, true);
    else
        DrawButtonArrow(&Canvas, rect, arrowDown, false);
    if(Enabled)
    {
        r.left += 1;
        r.top += 1;
        r.width = rect.left - 2;
        r.height -= 2;
        Canvas.Brush->Color = clWhite;
        Canvas.FillRect(r);
    }
    int x = Canvas.TextHeight("O");
    int y = (r.height - x) / 2 + 1;
    Canvas.TextOut(7, y, "ComboBox");
}
//---------------------------------------------------------------------------
//	TPanel
//---------------------------------------------------------------------------
TPanel :: TPanel()
{
    Init();
}
//---------------------------------------------------------------------------
TPanel :: ~TPanel()
{
}
//---------------------------------------------------------------------------
__fastcall TPanel :: TPanel(const TPanel &a) : TCommonPanel(a)
{
	Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TPanel & TPanel :: operator=(const TPanel &a)
{
	if(this == &a)
		return *this;
    TCommonPanel :: operator=(a);
    Assign(a);
	return *this;
}
//---------------------------------------------------------------------------
void TPanel :: Init()
{
    BevelInner = esNone;
    BevelOuter = esRaised;
    BevelWidth = 0;
}
//---------------------------------------------------------------------------
void __fastcall TPanel :: Assign(const TPanel &obj)
{
}
//---------------------------------------------------------------------------
//	TGroupBox
//---------------------------------------------------------------------------
TGroupBox :: TGroupBox()
{
    Init();
}
//---------------------------------------------------------------------------
TGroupBox :: ~TGroupBox()
{
}
//---------------------------------------------------------------------------
__fastcall TGroupBox :: TGroupBox(const TGroupBox &a) : TCommonGroupBox(a)
{
	Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TGroupBox & TGroupBox :: operator=(const TGroupBox &a)
{
	if(this == &a)
		return *this;
    TCommonGroupBox :: operator=(a);
    Assign(a);
	return *this;
}
//---------------------------------------------------------------------------
void TGroupBox :: Init()
{
}
//---------------------------------------------------------------------------
void __fastcall TGroupBox :: Assign(const TGroupBox &obj)
{
}
//---------------------------------------------------------------------------
//  Bevel
//---------------------------------------------------------------------------
TBevel :: TBevel()
{
   m_Style = bsLowered;
   m_Shape = bsBox;
}
//---------------------------------------------------------------------------
TBevel :: TBevel(TWinControl *AParent) // : TControl(AParent)
{
   m_Style = bsLowered;
   m_Shape = bsBox;
}
//---------------------------------------------------------------------------
void TBevel :: Paint()
{
//   VColor XorColor = 0x00FFD8CE;
//   VColor Color1, Color2;

	Canvas.Pen->Color = Canvas.Font->Color;
	TRect rect;

/*
procedure TBevel.Paint;

  procedure BevelRect(const R: TRect);
  begin
    with Canvas do
    begin
      Pen.Color := Color1;
      PolyLine([Point(R.Left, R.Bottom), Point(R.Left, R.Top),
        Point(R.Right, R.Top)]);
      Pen.Color := Color2;
      PolyLine([Point(R.Right, R.Top), Point(R.Right, R.Bottom),
        Point(R.Left, R.Bottom)]);
    end;
  end;

 Line(x1, y1, x2, y2)
  procedure BevelLine(C: TColor; X1, Y1, X2, Y2: Integer);
  begin
    with Canvas do
    begin
      Pen.Color := C;
      Line(x1, y1, x2, y2)
      MoveTo(X1, Y1);
      LineTo(X2, Y2);
    end;
  end;

begin
  with Canvas do
  begin
    Pen.Width := 1;

    if(m_Style == bsLowered)
    {
      Color1 = clBtnShadow;
      Color2 = clBtnHighlight;
    }
    else
    {
      Color1 = clBtnHighlight;
      Color2 = clBtnShadow;
    };

    switch (m_Shape)
    {
      case bsBox:
         BevelRect(Rect(0, 0, Width - 1, Height - 1));
         break;
      case bsFrame:
         Temp = Color1;
         Color1 = Color2;
         BevelRect(Rect(1, 1, Width - 1, Height - 1));
         Color2 = Temp;
         Color1 = Temp;
         BevelRect(Rect(0, 0, Width - 2, Height - 2));
         break;
      case bsTopLine:
         BevelLine(Color1, 0, 0, Width, 0);
         Pen.Color := C;
      Line(x1, y1, x2, y2)
         BevelLine(Color2, 0, 1, Width, 1);
         Pen.Color := C;
      Line(x1, y1, x2, y2)
         break;
      case bsBottomLine:
         BevelLine(Color1, 0, Height - 2, Width, Height - 2);
         Pen.Color := Color1;
      Line(x1, y1, x2, y2)
         BevelLine(Color2, 0, Height - 1, Width, Height - 1);
         Pen.Color := C;
      Line(x1, y1, x2, y2)
         break;
      case bsLeftLine:
         BevelLine(Color1, 0, 0, 0, Height);
         Pen.Color := C;
      Line(x1, y1, x2, y2)
         BevelLine(Color2, 1, 0, 1, Height);
         Pen.Color := C;
      Line(x1, y1, x2, y2)
         break;
      case bsRightLine:
         BevelLine(Color1, Width - 2, 0, Width - 2, Height);
         Pen.Color := C;
      Line(x1, y1, x2, y2)
         BevelLine(Color2, Width - 1, 0, Width - 1, Height);
         Pen.Color := C;
      Line(x1, y1, x2, y2)
         break;
    }
  end; //canvas
end;
*/
}
//---------------------------------------------------------------------------
void TBevel :: set_Style(TBevelStyle value)
{
  if(value != m_Style)
  {
      m_Style = value;
      Update();
  };
}
//---------------------------------------------------------------------------
void TBevel :: set_Shape(TBevelShape value)
{
  if(value != m_Shape)
  {
      m_Shape = value;
      Update();
  };
}
//---------------------------------------------------------------------------
//  TEdit
//---------------------------------------------------------------------------
TEdit :: TEdit()
{
    Init();
}
//---------------------------------------------------------------------------
TEdit :: ~TEdit()
{
}
//---------------------------------------------------------------------------
__fastcall TEdit :: TEdit(const TEdit &a) : TCommonEdit(a)
{
	Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TEdit & TEdit :: operator=(const TEdit &a)
{
	if(this == &a)
		return *this;
    TCommonEdit :: operator=(a);
    Assign(a);
	return *this;
}
//---------------------------------------------------------------------------
void TEdit :: Init()
{
    Width = 145;
    Height = 20;
}
//---------------------------------------------------------------------------
void __fastcall TEdit :: Assign(const TEdit &obj)
{
}
//---------------------------------------------------------------------------
void TEdit :: Paint()
{
    TRect r = ClientRect;
    Frame3D(&Canvas, r, clBtnShadow, clBtnHighlight, 1);
    if(Enabled)
    {
        r.left += 1;
        r.top += 1;
        r.width -= 2;
        r.height -= 2;
        Canvas.Brush->Color = clWindow;
        Canvas.FillRect(r);
    }
    int x = Canvas.TextHeight("O");
    int y = (r.height - x) / 2 + 1;
    Canvas.TextOut(7, y, Text);
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
