//---------------------------------------------------------------------------
#include <math.h>
#include "zoomler.h"
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
void UpRect(TCanvas *C, int x1, int y1, int x2, int y2)
{
	TRect rect;
     C->Brush->Color = clSilver;
     C->FillRect(rect.bounds(x1,y1,x2,y2));
     C->Pen->Color = clWhite;
     C->MoveTo(x1,y2);
     C->LineTo(x1,y1);
     C->LineTo(x2,y1);
     C->Pen->Color = cl3DDkShadow;
     C->MoveTo(x2,y1+1);
     C->LineTo(x2,y2);
     C->LineTo(x1-1,y2);
}

void DownRect(TCanvas *C, int x1, int y1, int x2, int y2)
{
	TRect rect;
     C->Brush->Color = clSilver;
     C->FillRect(rect.bounds(x1,y1,x2,y2));
     C->Pen->Color = clWhite;
     C->MoveTo(x2,y1);
     C->LineTo(x2,y2);
     C->LineTo(x1,y2);
     C->Pen->Color = cl3DDkShadow;
     C->MoveTo(x1,y2);
     C->LineTo(x1,y1);
     C->LineTo(x2,y1);
}

double DMin(double a, double b)
{
	return a < b ? a : b;
}

double DMax(double a, double b)
{
    return a > b ? a : b;
}

TZoomler :: TZoomler()
{
//   ControlStyle := ControlStyle + [csOpaque];
   m_Min = 1.0;
   m_Max = 100.0;
   m_Start = 30.0;
   m_Finish = 70.0;
   FStep = 1.0;
   Width = 200;
   Height = 14;
   CalcScreen();
   DblClkEnable = true;
   m_status = st_NONE;
   Orientation = Horizontal;
   Timer.OnTimer = EVENT(TNotifyEvent, TZoomler, OnTimer);
//   Timer.Enabled = false;
}

TZoomler :: ~TZoomler()
{
}

void TZoomler :: set_Min(double m)
{
	if(m >= m_Max)
		m_Min = m_Max-FStep;
	else
		m_Min = m;
	CalcScreen();
	Repaint();
}

void TZoomler :: set_Max(double m)
{
	if(m <= m_Min)
		m_Max = m_Min + FStep;
	else
		m_Max = m;
	CalcScreen();
	Repaint();
}

void TZoomler :: set_Start(double m)
{
	if(m >= m_Finish)
		m_Start = m_Finish - FStep;
	if(m < m_Min)
		m_Start = m_Min;
	else
		m_Start = m;
	CalcScreen();
	Repaint();
}

void TZoomler :: set_Finish(double m)
{
	if(m <= m_Start)
		m_Finish = m_Start + FStep;
	if(m > m_Max)
		m_Finish = m_Max;
	else
		m_Finish = m;
	CalcScreen();
	Repaint();
}

void TZoomler :: set_Orientation(TOrientation aOrientation)
{
	m_Orientation = aOrientation;
	Repaint();
}

void TZoomler :: OnTimer(TObject *Sender)
{
   Timer.Interval = TimerRate;
   MouseDown(hButton, hShift, hx, hy);
}

void TZoomler :: DblClick()
{
	TControl :: DblClick();
	if(!DblClkEnable)
		return;
	m_Start = m_Min;
	m_Finish = m_Max;
	CalcScreen();
	Repaint();
	if(!OnChanged.empty())
		OnChanged(this);
}

void TZoomler :: Paint()
{
	TPoint point[3];
	TBitmap Buffer;
	Buffer.Width = *Width;
	Buffer.Height = *Height;
	CalcScreen();
      DownRect(Buffer.Canvas,0,0,Width-1,Height-1);
      Buffer.Canvas->Brush->Color = 0xE0E0E0;
      Buffer.Canvas->Pen->Color = 0xE0E0E0;
      TRect rect;
      rect.bounds(2, 2, Width-2, Height-2);
      Buffer.Canvas->Rectangle(rect);
//      Buffer.Canvas->Rectangle(2, 2, Width-2, Height-2);
      switch(Orientation)
      {
			case Horizontal:
                 UpRect(Buffer.Canvas,FSL,2,FSR,Height-3);
                 DownRect(Buffer.Canvas,FSL+10,3,FSR-10,Height-4);
                 if(LeftMoving)
					DownRect(Buffer.Canvas,1,2,11,Height-3);
                 else
                    UpRect(Buffer.Canvas,1,2,11,Height-3);
                 if(RightMoving)
                    DownRect(Buffer.Canvas,Width-11,2,Width-2,Height-3);
                 else
                    UpRect(Buffer.Canvas,Width-11,2,Width-2,Height-3);
                 Buffer.Canvas->Pen->Color = clGray;
                 Buffer.Canvas->Brush->Color = clGray;
                 Buffer.Canvas->MoveTo(FSL+5,3);
                 Buffer.Canvas->LineTo(FSL+5,Height-4);
                 Buffer.Canvas->MoveTo(FSR-5,3);
                 Buffer.Canvas->LineTo(FSR-5,Height-4);
                 point[0] = TPoint(9,3);
                 point[1] = TPoint(9,Height-4);
                 point[2] = TPoint(2,(Height-4) / 2 + 2);
                 Buffer.Canvas->Polygon(point,3);
                 point[0] = TPoint(Width-9,3);
                 point[1] = TPoint(Width-9,Height-4);
                 point[2] = TPoint(Width-3,(Height-4)/2+2);
                 Buffer.Canvas->Polygon(point,3);
                 break;
			case Vertical:
                         UpRect(Buffer.Canvas,2,FSL,Width - 3,FSR);
                         DownRect(Buffer.Canvas,3,FSL+10,Width - 4,FSR-10);
                         if(LeftMoving)
                            DownRect(Buffer.Canvas,2,1,Width - 3,11);
                         else
                            UpRect(Buffer.Canvas,2,1,Width - 3,11);
                         if(RightMoving)
                            DownRect(Buffer.Canvas,2,Height-11,Width-3,Height-2);
                         else
                            UpRect(Buffer.Canvas,2,Height-11,Width-3,Height-2);
                         Buffer.Canvas->Pen->Color = clGray;
                         Buffer.Canvas->Brush->Color = clGray;
                         Buffer.Canvas->MoveTo(3,FSL+5);
                         Buffer.Canvas->LineTo(Width-4,FSL+5);
                         Buffer.Canvas->MoveTo(3,FSR-5);
                         Buffer.Canvas->LineTo(Width-4,FSR-5);
                         point[0] = TPoint(3,9);
                         point[1] = TPoint(Width-4,9);
                         point[2] = TPoint((Width-4)/2+2,2);
                         Buffer.Canvas->Polygon(point,3);
                         point[0] = TPoint(3,Height-9);
                         point[1] = TPoint(Width-4,Height-9);
                         point[2] = TPoint((Width-4)/2+2,Height-3);
                         Buffer.Canvas->Polygon(point,3);
                      break;
      };
//   Canvas->CopyMode = cmSrcCopy;
   Canvas.Draw(0, 0, Buffer);
}

void TZoomler :: CalcScreen()
{
	double coef;
    int osl,osr;

	switch(Orientation)
	{
		case Horizontal: coef = (Width-24)/(m_Max - m_Min); break;
		case Vertical  : coef = (Height-24)/(m_Max - m_Min); break;
	};
	osl = FSL;
	osr = FSR;
    FSL = 12 + round((m_Start - m_Min) * coef);
	switch(Orientation)
	{
		case Horizontal: FSR = Width-12-round((m_Max-m_Finish)*coef);
		case Vertical  : FSR = Height-12-round((m_Max-m_Finish)*coef);
	};
	if((FSR-FSL)<30)
	{
		if(abs(FSR-osr) > abs(FSL-osl))
			FSR = FSL+30;
		else
			if(abs(FSL-osl)>abs(FSR-osr))
				FSL = FSR-30;
			else
			{
				FSL = osl;
				FSR = osr;
			};
	};
}

void TZoomler :: DoLeftMove() // {one step to left}
{
	double d;

    if(m_Start >= m_Min + FStep)
		d = FStep;
    else
		d = m_Start-m_Min;
    m_Start = m_Start - d;
    m_Finish = m_Finish - d;
    CalcScreen();
    Repaint();
	if(!OnChanged.empty())
		OnChanged(this);
}

void TZoomler :: DoRightMove() // {one step to right}
{
	double d;

    if(m_Finish <= m_Max-FStep)
		d = FStep;
    else
		d = m_Max - m_Finish;
    m_Start = m_Start+d;
    m_Finish = m_Finish+d;
    CalcScreen();
    Repaint();
	if(!OnChanged.empty())
		OnChanged(this);
}

void TZoomler :: DoLeftPage(int x) // {proportionally}
{
	double d;

	d = m_Start;
    m_Start = m_Start-(m_Start-m_Min)/(FSL-11)*(FSL-x);
    m_Finish = m_Finish-(d-m_Start);
    CalcScreen();
    Repaint();
	if(!OnChanged.empty())
		OnChanged(this);
}

void TZoomler :: DoRightPage(int x) // {proportionally}
{
	double d;

    d = m_Finish;
    switch(Orientation)
    {
		case Horizontal: m_Finish = m_Finish + (m_Max - m_Finish)/(Width-11-FSR)*(x-FSR); break;
		case Vertical  : m_Finish = m_Finish + (m_Max - m_Finish)/(Height-11-FSR)*(x-FSR); break;
    };
    m_Start = m_Start + (Finish - d);
    CalcScreen();
    Repaint();
	if(!OnChanged.empty())
		OnChanged(this);
}

void TZoomler :: MouseDown(TMouseButton Button, TShiftState Shift, int x, int y)
{
	byte_t p;
    int m;

	TControl :: MouseDown(Button, Shift, x, y);
	switch(Orientation)
	{
		case Horizontal: m = x; break;
		case Vertical  : m = y; break;
	};
	XS = m;
	p = WherePoint(x, y);
	switch(p)
	{
		case sc_LEFTSIZE: m_status = st_LEFT; break;
		case sc_RIGHTSIZE: m_status = st_RIGHT; break;
		case sc_LEFTMOVE :
			DoLeftMove();
			LeftMoving = true;
			break;
		case sc_RIGHTMOVE:
			DoRightMove();
			RightMoving = true;
			break;
		case sc_MOVE: m_status = st_BOTH; break;
		case sc_LEFTPAGE: DoLeftPage(m); break;
		case sc_RIGHTPAGE: DoRightPage(m); break;
		default: m_status = st_NONE;
	};
   if(p == sc_LEFTMOVE || p == sc_RIGHTMOVE)
   {
		if( !Timer.Enabled )
			Timer.Interval = TimerDelay;
		hx = x;
		hy = y;
		hButton = Button;
		hShift = Shift;
		Timer.Enabled = true;
   };
}

void TZoomler :: MouseUp(TMouseButton Button, TShiftState Shift, int x, int y)
{
   TControl :: MouseUp(Button, Shift, x, y);
   Timer.Enabled = false;
   LeftMoving = false;
   RightMoving = false;
   if(m_status != st_NONE)
   {
		if(!OnChanged.empty())
			OnChanged(this);
   }
   Repaint();
   m_status = st_NONE;
}

void TZoomler :: MouseMove(TShiftState Shift, int x, int y)
{
   TControl :: MouseMove(Shift, x, y);
   switch(WherePoint(x,y))
   {
		case sc_LEFTSIZE:
		case sc_RIGHTSIZE:
			switch(Orientation)
			{
				case Horizontal: Cursor = crSizeWE; break;
                case Vertical  : Cursor = crSizeNS; break;
            };
            break;
		case sc_MOVE: Cursor = crHandPoint; break;
		default: Cursor = crDefault;
   };
   if(m_status != st_NONE)
   {
      switch(Orientation)
      {
			case Horizontal: ProcessMove(x); break;
			case Vertical  : ProcessMove(y); break;
      };
   };
}

void TZoomler :: ProcessMove(int x)
{
	double d;
    double coef;

	if(x==XS)
		return;
	switch(Orientation)
	{
		case Horizontal: coef = (Width-24)/(m_Max-m_Min); break;
		case Vertical  : coef = (Height-24)/(m_Max-m_Min); break;
	};
	d = (x - XS)/coef;
	switch(m_status)
	{
		case st_BOTH:
                   if(d<0)
						d = DMax(d,m_Min-m_Start);
                   else
						d = DMin(d,m_Max-m_Finish);
                   m_Start = m_Start+d;
                   m_Finish = m_Finish+d;
                   if(!OnScrolling.empty())
						OnScrolling(this);
					break;
		case st_LEFT:
                   if(d<0)
						m_Start = m_Start + DMax(d,m_Min-m_Start);
                   else
						m_Start = m_Start+ DMin(d, m_Finish- m_Start);
                   if(!OnZooming.empty())
						OnZooming(this);
					break;
		case st_RIGHT:
                   if(d>0)
						m_Finish = m_Finish + DMin(d, m_Max-m_Finish);
                   else
						m_Finish = m_Finish + DMax(d, m_Start - m_Finish);
                   if(!OnZooming.empty())
						OnZooming(this);
					break;
		case st_NONE: break;
   };
   XS = x;
   Repaint();
}

byte_t TZoomler :: WherePoint(int X, int Y)
{
	byte_t ret;
	TRect rect;

	switch(Orientation)
	{
		case Horizontal:
                      if(rect.bounds(1,3,11,Height-3).inside(X,Y))
                         ret = sc_LEFTMOVE;
                      else
                      if(rect.bounds(12,3,FSL-1,Height-3).inside(X,Y))
                         ret = sc_LEFTPAGE;
                      else
                      if(rect.bounds(FSL+1,3,FSL+11,Height-3).inside(X,Y))
                         ret = sc_LEFTSIZE;
                      else
                      if(rect.bounds(FSL+12,3,FSR-12,Height-3).inside(X,Y))
                         ret = sc_MOVE;
                      else
                      if(rect.bounds(FSR-11,3,FSR-1,Height-3).inside(X,Y))
						ret = sc_RIGHTSIZE;
                      else
                      if(rect.bounds(FSR+1,3,Width-12,Height-3).inside(X,Y))
                         ret = sc_RIGHTPAGE;
                      else
                      if(rect.bounds(Width-11,3,Width-1,Height-3).inside(X,Y))
                         ret = sc_RIGHTMOVE;
                      else
                         ret = sc_NONE;
						break;
		case Vertical:
                      if(rect.bounds(3,1,Width-3,11).inside(X,Y))
                         ret = sc_LEFTMOVE;
                      else
                      if(rect.bounds(3,12,Width-3,FSL-1).inside(X,Y))
                         ret = sc_LEFTPAGE;
                      else
                      if(rect.bounds(3,FSL+1,Width-3,FSL+11).inside(X,Y))
                         ret = sc_LEFTSIZE;
                      else
                      if(rect.bounds(3,FSL+12,Width-3,FSR-12).inside(X,Y))
                         ret = sc_MOVE;
                      else
                      if(rect.bounds(3,FSR-11,Width-3,FSR-1).inside(X,Y))
                         ret = sc_RIGHTSIZE;
                      else
                      if(rect.bounds(3,FSR+1,Width-3,Height-12).inside(X,Y))
                         ret = sc_RIGHTPAGE;
                      else
                      if(rect.bounds(3,Height-11,Width-3,Height-1).inside(X,Y))
                         ret = sc_RIGHTMOVE;
                      else
                         ret = sc_NONE;
                    break;
	};
	return ret;
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------

