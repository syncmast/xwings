#include "stdctrls.h"
#include "stdgraph.h"
#include "xpainter.h"
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
TXPainter XPainter;
//---------------------------------------------------------------------------
//  TXPainter
//---------------------------------------------------------------------------
TXPainter :: TXPainter()
{
    Init();
}
//---------------------------------------------------------------------------
TXPainter :: ~TXPainter()
{
}
//---------------------------------------------------------------------------
__fastcall TXPainter :: TXPainter(const TXPainter &a) : TPainter(a)
{
	Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TXPainter & TXPainter :: operator=(const TXPainter &a)
{
	if(this == &a)
		return *this;
    TPainter :: operator=(a);
    Assign(a);
	return *this;
}
//---------------------------------------------------------------------------
void TXPainter :: Init()
{
}
//---------------------------------------------------------------------------
void __fastcall TXPainter :: Assign(const TXPainter &obj)
{
}
//---------------------------------------------------------------------------
void __fastcall TXPainter :: PaintControl(TControl &ctrl)
{
    m_Canvas = &ctrl.Canvas;
    if(__classid(ctrl) == __classid(TButton))
    {
        DrawButton(ctrl);
        return;
    }
    if(__classid(ctrl) == __classid(TCheckBox))
    {
        DrawCheckBox(ctrl);
        return;
    }
    if(__classid(ctrl) == __classid(TGroupBox))
    {
        DrawGroupBox(ctrl);
        return;
    }
    if(__classid(ctrl) == __classid(TPanel))
    {
        DrawPanel(ctrl);
        return;
    }
    if(__classid(ctrl) == __classid(TComboBox))
    {
        DrawComboBox(ctrl);
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TXPainter :: DrawButton(TControl &ctrl)
{
	int width = 1;
	if(ctrl.CheckState(csMouseOver))
		m_Canvas->Brush->Color =  m_Canvas->LightenColor(ctrl.Color, 20);
	else
		m_Canvas->Brush->Color = ctrl.Color;
	TRect rect = ctrl.ClientRect, focusrect;
//	rect.round(8);
	m_Canvas->FillRect(rect);
	rect.inflate(-1, -1);

	if(ctrl.CheckState(csLButtonDown))
	{
		Frame3D(m_Canvas, rect, clBtnShadow, clBtnHighlight, width);
		m_Canvas->Pen->Color = clWindowFrame;
	}
   else
   {
   		Frame3D(m_Canvas, rect, clBtnHighlight, clBtnShadow, width);
   	m_Canvas->Pen->Color = ctrl.Color;
   }

	if(ctrl.Active)
    {
		m_Canvas->Pen->Color = clWindowFrame;
/*	else
		if(!ctrl.CheckState(csLButtonDown))
			m_Canvas->Pen->Color = ctrl.Color; */
//	m_Canvas->Rectangle(ClientRect);
	focusrect = rect;
	focusrect.inflate(-(width + 2), -(width + 2));
	DrawFocusRect(focusrect);
    }
	if(ctrl.CheckState(csLButtonDown))
			rect.offset(width, width);
   m_Canvas->TextRect(rect, ctrl.Caption, alCenter);
}
//---------------------------------------------------------------------------
void __fastcall TXPainter :: DrawCheckBox(TControl &ctrl)
{
	int width = 2;
	TRect clirect = ctrl.ClientRect;
	m_Canvas->Brush->Color = ctrl.Color;
	m_Canvas->FillRect(clirect);
	TRect rect = TRect(0, 0, ctrl.Height, ctrl.Height);
   	Frame3D(m_Canvas, rect, clBtnShadow, clBtnHighlight, width);
   	rect.inflate(-width, -width);
	m_Canvas->Pen->Width = 0;
	if(ctrl.Active)
	   DrawFocusRect(clirect);
	if(ctrl.Enabled)
		m_Canvas->TextRect(clirect, ctrl.Height + 5, 0, ctrl.Caption, alVCenter);
	else
	{
		TRect r = clirect;
		r.left =  ctrl.Height + 5;
		r.top = 1;
		DrawDisabledText(r, ctrl.Caption, NULL);
	}
	TCheckBox &chkbox = dynamic_cast<TCheckBox &>(ctrl);
   	switch(chkbox.State)
   	{
      	case cbUnchecked:
         	m_Canvas->Brush->Color = clWindow;
         	m_Canvas->FillRect(rect);
         	break;
      	case cbChecked:
         	m_Canvas->Brush->Color = clWindow;
         	m_Canvas->Pen->Color = clBlack;
         	m_Canvas->FillRect(rect);
         	DrawCheck(*m_Canvas, TPoint(rect.left + 2, rect.top + 7), 2, 3);
         	break;
      	case cbUndefined:
         	m_Canvas->Brush->Color = clBtnFace;
         	m_Canvas->Pen->Color = clBtnShadow;
         	m_Canvas->FillRect(rect);
         	DrawCheck(*m_Canvas, TPoint(rect.left + 2, rect.top + 7), 2, 3);
   	};
}
//---------------------------------------------------------------------------
void __fastcall TXPainter :: DrawGroupBox(TControl &ctrl)
{
	int H;
	TRect R;
//	m_Canvas->Font = Font;
	m_Canvas->Brush->Color = ctrl.Color;
	R = ctrl.ClientRect;
    m_Canvas->FillRect(R);

//    m_Canvas->Pen->Color = clBlue;
//    m_Canvas->Rectangle(R);

	H = m_Canvas->TextHeight("0");
	R.top = H / 2 - 1;
	R.height -= R.top;
	DrawEdge(m_Canvas, R, esRaised, esLowered, ebRect);
   	R = TRect(8, 0, m_Canvas->TextWidth(ctrl.Caption) + 2, H);
    m_Canvas->DrawText(R, ctrl.Caption);
}
//---------------------------------------------------------------------------
#define SM_CXHTHUMB 12

void __fastcall TXPainter :: DrawComboBox(TControl &ctrl)
{
	m_Canvas->Brush->Color = ctrl.Color;
	TRect R = ctrl.ClientRect;
    TComboBox &combo = dynamic_cast<TComboBox &>(ctrl);

    bool MouseInControl, DroppedDown, ForceDrawFocused;
    int x;

//    R := ClientRect;
    m_Canvas->Brush->Style = bsSolid;
    if(combo.BorderColor != clNone)
    {
        m_Canvas->Brush->Color = combo.BorderColor;
        m_Canvas->FrameRect(R);
    }
    else
    {
        m_Canvas->Brush->Color = ctrl.Color;
        m_Canvas->FrameRect(R);
    }

//    R.Left := R.Right - GetSystemMetrics(SM_CXHTHUMB) - 3;
R.left = R.right() - SM_CXHTHUMB - 3;
      R.width--;
      R.inflate(0, -1);
      m_Canvas->Brush->Color = combo.ButtonColor;
      m_Canvas->FillRect(R);

//      R.Right := R.Left + 5;
        R.width = 5;
      m_Canvas->Brush->Color = ctrl.Color;
      m_Canvas->FillRect(R);

      if(combo.BorderColor != clNone)
      {
        R.width--;
        m_Canvas->Pen->Color = combo.BorderColor;
        m_Canvas->MoveTo(R.right(), R.top);
        m_Canvas->LineTo(R.right(), R.bottom());
      }

      m_Canvas->Pen->Color = combo.ArrowColor;
      R = ctrl.ClientRect;
      x = R.right() - 10;
      m_Canvas->MoveTo(x + 0, R.top + 10);
      m_Canvas->LineTo(x + 5, R.top + 10);
      m_Canvas->MoveTo(x + 1, R.top + 11);
      m_Canvas->LineTo(x + 4, R.top + 11);
      m_Canvas->MoveTo(x + 2, R.top + 12);
      m_Canvas->LineTo(x + 3, R.top + 12);
}
//---------------------------------------------------------------------------
void __fastcall TXPainter :: DrawPanel(TControl &ctrl)
{
	m_Canvas->Brush->Color = ctrl.Color;
	TRect R = ctrl.ClientRect;
    TPanel &pan = dynamic_cast<TPanel &>(ctrl);
	DrawEdge(m_Canvas, R, pan.BevelInner, pan.BevelOuter, ebRect, pan.BevelWidth);
    m_Canvas->TextRect(R, 0, 0, ctrl.Caption, alCenter);
}
//---------------------------------------------------------------------------
void __fastcall TXPainter :: DrawDisabledText(TRect &rect, const TString &text, TFormatText *format)
{
    TColor old = m_Canvas->Font->Color;
	m_Canvas->Brush->Color = clNone;
	rect.offset( 1, 1);
  	m_Canvas->Font->Color = clBtnHighlight;
	m_Canvas->DrawText(rect, text, format);
  	m_Canvas->Font->Color = clBtnShadow;
  	rect.offset( -1, -1);
  	m_Canvas->DrawText(rect, text, format);
  	m_Canvas->Font->Color = old;
}
//---------------------------------------------------------------------------
void __fastcall TXPainter :: DrawFocusRect(const TRect &rect)
{
	TPenStyle style = m_Canvas->Pen->Style;
	TPenMode old = m_Canvas->Pen->Mode;
   	m_Canvas->Pen->Style = psDot;
   	m_Canvas->Pen->Mode = pmNotMerge;
   	m_Canvas->Rectangle(rect);
   	m_Canvas->Pen->Style = style;
   	m_Canvas->Pen->Mode = old;
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
