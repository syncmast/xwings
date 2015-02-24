/*-------------------------------------------------------------------------------
This component is the 4th generation of this component. As far as I know I'm the
forth person to make major changes to this component. My contribution consist of
adding hiperlink capabilities, a frame around it, faded lines on several positions
and gradient properties in several directions.

Major properties :
 - Autosizing : it resizes so as to fit best in rectangle. Autosize must be set
                to False before it works.
 - 3D Effects : usual 3D effects such as normal, resit, raised, shadowed
New properties ( 4th generation ) :
 - Hiperlink : http, mail, ftp, news
 - SuperScript and SubScript capabilities
 - Gradient style : horizontal, vertical, elliptic, rectangle, vertical center,
                    horizontal center
 - Frame : draw a rectangle around the label.
 - Faded line : top, bottom, middle.

The idea of faded lines is based on MenuBar by Bluecave Components. I studied
this property and adapted it to this component so as to draw faded lines at the
top, at the bottom and in the middle taking into account the text alignment.

On the other hand, I added hiperlink capabilities so as to avoid to use another
TLabel Component exclusively for it. So I made a mix of several properties.
Besides, I made a small change in the autosizing procedure because in some cases
the bottom of the component cut the lowest part of letters such as "p" or "g".

Finally, I added many comments to the source code for better study. In addition
I've made on-line help.

History :
  1st Release : Louis Louw : louw@gcs.co.za www.gcs.co.za/mbs/mbs.htm
  2nd Release : Durrin Hynes-Christensen : dxh@gv.dk
  3rd Release : I don't know
  4th Release : Maximo Yarritu Arnaez : myarritu@gmx.net
-------------------------------------------------------------------------------*/
#include "label.h"

#define HTTP "http://"
#define MAIL "mailto:"
#define FTP  "ftp://"
#define NEWS "news://"
#define SUB  '_'
#define SUPER '^'
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
//  TmyaSuperLabel
//---------------------------------------------------------------------------
DYNAMIC_CLASS( TmyaSuperLabel );
//---------------------------------------------------------------------------
TmyaSuperLabel :: TmyaSuperLabel()
{
    Init();
}
//---------------------------------------------------------------------------
TmyaSuperLabel :: ~TmyaSuperLabel()
{
}
//---------------------------------------------------------------------------
__fastcall TmyaSuperLabel :: TmyaSuperLabel(const TmyaSuperLabel &a) : TCommonLabel(a)
{
	Init();
	Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TmyaSuperLabel & TmyaSuperLabel :: operator=(const TmyaSuperLabel &a)
{
	if(this == &a)
		return *this;
    TCommonLabel :: operator=(a);
    Assign(a);
	return *this;
}
//---------------------------------------------------------------------------
void TmyaSuperLabel :: Init()
{
    m_Effect3D = Normal3D;
  Transparent = true;
  m_ShadeRightBottom = clGray;
  m_ShadeLeftTop = clWhite;
  m_AHShadeOffset = 5;
  m_AVShadeOffset = -5;
  fLast = clWhite;
  AutoSize = false;
  m_FitType = ftBestFitBoth;
  fOldSize = Canvas.Font->Size;
  fOldWidth = Width;
  fOldHeight = Height;

  m_GradientStyle = gsNone;
  m_BeginColor = clNavy;
  m_EndColor = clAqua;
  m_Frame = false;
  m_LineStyle = lsNone;
  m_LabelType = ltNormal;
  m_Effect98 = true;
  ColorSelected = clGreen;
}
//---------------------------------------------------------------------------
__fastcall void TmyaSuperLabel :: Assign(const TmyaSuperLabel &obj)
{
}
//---------------------------------------------------------------------------
void TmyaSuperLabel :: DoDrawText(const TRect &Area, word_t Flags)
{
    TString text;
    byte_t Size;
    TRect TmpRect;
    TColor UpperColor, LowerColor;

  text = Caption;
  if( (Flags and DT_CALCRECT) and
     ((text[0]=='\0') or (ShowAccelChar and (text[0]=='&') and (text[1]=='\0'))) )
      text =  " ";
  if(!ShowAccelChar)
    Flags = Flags | DT_NOPREFIX;
  Canvas.Font = Font;
    switch(m_Effect3D)
    {
        case Resit3D:
                UpperColor = m_ShadeRightBottom;
                LowerColor = m_ShadeLeftTop;
                TmpRect = Area;
                OffsetRect(TmpRect, 1, 1);
                Canvas.Font->Color = LowerColor;
                NewDrawText(text, TmpRect, Flags);
                TmpRect = Area;
                OffsetRect(TmpRect, -1, -1);
                Canvas.Font->Color = UpperColor;
                NewDrawText(text, TmpRect, Flags);
                break;
        case Raised3D:
                UpperColor = m_ShadeLeftTop;
                LowerColor = m_ShadeRightBottom;
                TmpRect = Area;
                OffsetRect(TmpRect, 1, 1);
                Canvas.Font->Color = LowerColor;
                NewDrawText(text, TmpRect, Flags);
                TmpRect = Area;
                OffsetRect(TmpRect, -1, -1);
                Canvas.Font->Color = UpperColor;
                NewDrawText(text, TmpRect, Flags);
                break;
        case Shadowed3D:
                UpperColor = m_ShadeLeftTop;
                LowerColor = m_ShadeRightBottom;
                TmpRect = Area;
                OffsetRect(TmpRect, m_AHShadeOffset, m_AVShadeOffset);
                Canvas.Font->Color = LowerColor;
                NewDrawText(text, TmpRect, Flags);
                break;
  };
  Canvas.Font->Color = Font->Color;
  if(!AutoSize)
    if((fOldSize != Canvas.Font->Size) or (fOldWidth != Width) or (fOldHeight != Height))
    {
        switch(m_FitType)
        {
            case ftBestFitBoth:
                Canvas.Font->Size = 0;
                while((abs(Canvas.Font->Height) < Area.bottom() - 6) and (Canvas.TextWidth(TString(text)) < Area.right()))
                    Canvas.Font->Size = Canvas.Font->Size + 1;
                Canvas.Font->Size = Canvas.Font->Size - 1;
                Font->Size = Canvas.Font->Size;
                break;
            case ftBestFitVert: Font->Height = Area.bottom() - 2; break;
            case ftBestFitHorz:
                Canvas.Font->Size = 0;
                while(Canvas.TextWidth(TString(Text)) < Area.right())
                    Canvas.Font->Size = Canvas.Font->Size + 1;
                Canvas.Font->Size = Canvas.Font->Size - 1;
                Font->Size = Canvas.Font->Size;
                break;
      };
      fOldSize = Canvas.Font->Size;
      fOldWidth = Width;
      fOldHeight = Height;
    };
  if(!Enabled)
    Canvas.Font->Color = clGrayText;
  NewDrawText(text, Area, Flags);
}

void TmyaSuperLabel :: NewDrawText(const TString &Text, const TRect &Area, word_t Flags)
{
    int nFor, XChar;
    bool SubScript, SuperScript;
    TFont DefaultFont;

    if(LabelType == ltSubSuperScript)
    {
            //{ the font is changed when a SubScript or SuperScript is drawn }
//        DefaultFont := TFont.Create;
        DefaultFont = *Canvas.Font;
            //{ Calculate the X position for each char }
        switch(Alignment)
        {
            case taRightJustify: XChar = Area.right() - Canvas.TextWidth(ExtractCaption(Text)); break;
            case taLeftJustify: XChar = Area.left; break;
            case taCenter: XChar = Area.left + (Area.right() - Canvas.TextWidth(ExtractCaption(Text))) / 2;
        };
            //{ Each char is drawn taking into account the previous char which indicates the way of drawing }
        for(nFor = 0; nFor < Text.Length(); nFor++)
        {
            if((nFor > 0) and (Text[nFor-1] == SUB))
                SubScript = true;
            else
                SubScript = false;
            if((nFor>0) and (Text[nFor-1] == SUPER))
                SuperScript = true;
            else
                SuperScript = false;
            if((Text[nFor] != SUB) and (Text[nFor] != SUPER))
            {
                if(SubScript)
                {
                    //{ Recalculate the height font }
                    Canvas.Font->Height = Canvas.Font->Height * 8 / 10;
                    //{ Calculate de X position to draw the char }
                    Canvas.TextRect(TRect(XChar,Area.top,XChar + Canvas.TextWidth(Text[nFor]),Area.bottom()),
                          XChar,
                          Area.top + abs(8*Canvas.Font->Height-10*DefaultFont.Height) / 10, Text[nFor]);
                    //{ Calculate the next X position }
                    XChar = XChar + Canvas.TextWidth(Text[nFor]);
                };
                if(SuperScript)
                {
                    //{ Recalculate the height font }
                    Canvas.Font->Height = Canvas.Font->Height * 8 / 10;
                    //{ Calculate de X position to draw the char }
                    Canvas.TextRect(TRect(XChar,Area.top,XChar + Canvas.TextWidth(Text[nFor]),Area.bottom()),
                          XChar,
                          Area.top - abs(8*Canvas.Font->Height-10*DefaultFont.Height) / 20, Text[nFor]);
                    //{ Calculate the next X position }
                    XChar = XChar + Canvas.TextWidth(Text[nFor]);
                };
                if((!SubScript) and (!SuperScript))
                {
                    //{ if the actual font is different the default font is loaded }
                    *Canvas.Font = DefaultFont;
                    Canvas.TextRect(TRect(XChar,Area.top, XChar + Canvas.TextWidth(Text[nFor]), Area.bottom()),
                          XChar,
                          Area.top,
                          Text[nFor]);
                    XChar = XChar + Canvas.TextWidth(Text[nFor]);
                };
                *Canvas.Font = DefaultFont;
            };
        };
    }
    else
//        DrawText(Canvas.Handle, Text, StrLen(Text), Area, Flags);
        DrawText(Area, Text, TFormatText *format = NULL);
}

void TmyaSuperLabel :: Paint()
{
    const word_t Alignments[TAlignment] = {DT_LEFT, DT_RIGHT, DT_CENTER};

    TRect Area;
    //{ These are the separate color values for RGB of color values }
    int FromR, FromG, FromB;
    int DiffR, DiffG, DiffB;
    int ALength;
    TPoint AuxPoint;
    TString AuxCaption;
    //{ Useful to draw the bevel around the TEdit }
  procedure BevelRect(Color1, Color2: TColor; const R: TRect);
  begin
    with Canvas do
    begin
      Pen.Color := Color1;
      PolyLine([Point(R.Left, R.Bottom), Point(R.Left, R.Top), Point(R.Right, R.Top)]);
      Pen.Color := Color2;
      PolyLine([Point(R.Right, R.Top), Point(R.Right, R.Bottom), Point(R.Left, R.Bottom)]);
    end;
  end;
{
    FromR = TColorQuad(fBeginColor).Red;
    FromG = TColorQuad(fBeginColor).Green;
    FromB = TColorQuad(fBeginColor).Blue;
    DiffR = TColorQuad(fEndColor).Red-FromR;
    DiffG = TColorQuad(fEndColor).Green-FromG;
    DiffB = TColorQuad(fEndColor).Blue-FromB;
/*    { FromR := FBeginColor and $000000ff;
    FromG := (FBeginColor shr 8) and $000000ff;
    FromB := (FBeginColor shr 16) and $000000ff;
    DiffR := (FEndColor and $000000ff) - FromR;
    DiffG := ((FEndColor shr 8) and $000000ff) - FromG;
    DiffB := ((FEndColor shr 16) and $000000ff) - FromB; } */
 //   { Depending on gradient style selected, go draw it on the Bitmap canvas }
    switch(FGradientStyle)
    {
        case gsHorizontal: DoHorizontal(FromR, FromG, FromB, DiffR, DiffG, DiffB); break;
        case gsVertical: DoVertical(FromR, FromG, FromB, DiffR, DiffG, DiffB); break;
        case gsElliptic: DoElliptic(FromR, FromG, FromB, DiffR, DiffG, DiffB); break;
        case gsRectangle: DoRectangle(FromR, FromG, FromB, DiffR, DiffG, DiffB); break;
        case gsVertCenter: DoVertCenter(FromR, FromG, FromB, DiffR, DiffG, DiffB); break;
        case gsHorizCenter: DoHorizCenter(FromR, FromG, FromB, DiffR, DiffG, DiffB); break;
        case gsNone:
            if(not Transparent)
            {
                Canvas.Brush->Style = bsSolid;
                Canvas.Brush->Color = Color;
                Canvas.FillRect(ClientRect);
            }
    }
    Canvas.Brush->Style = bsClear;
    Area = ClientRect;
    if(Frame)
    {
        BevelRect(clBtnShadow, clBtnShadow, Rect(Area.Left+1, Area.Top+1, Area.Right-1, Area.Bottom-1));
        BevelRect(clBtnHighlight, clBtnHighlight, Rect(Area.Left, Area.Top, Area.Right-2, Area.Bottom-2));
    }
    Area = Rect(Area.Left,Area.Top,Area.Right,Area.Bottom-2);
    DoDrawText(Area, (DT_EXPANDTABS or DT_WORDBREAK) or Alignments[Alignment]);

//  { Depending on Line Style line is drawn }
  ALength = (Area.Right-Area.left-4) / 2;
    switch(LineStyle)
    {
        case lsTop:
            AuxPoint.x = Area.left + 2;
            AuxPoint.y = Area.top + 2;
            DrawLeftFadeLine(AuxPoint, ALength, ALength / 3, clBtnShadow);
            AuxPoint.y = AuxPoint.y + 1;
            DrawLeftFadeLine(AuxPoint, ALength, ALength /3 3, clBtnHighLight);
            AuxPoint.y = AuxPoint.y - 1;
            AuxPoint.x = AuxPoint.x + ALength;
            DrawRightFadeLine(AuxPoint, ALength, ALength div 3, clBtnShadow);
            AuxPoint.y = AuxPoint.y + 1;
            DrawRightFadeLine(AuxPoint, ALength, ALength div 3, clBtnHighLight);
            break;
        case lsBottom:
            AuxPoint.x = Area.left + 2;
            AuxPoint.y = Area.bottom() - 3;
            DrawLeftFadeLine(AuxPoint, ALength, ALength / 3, clBtnShadow);
            AuxPoint.y += 1;
            DrawLeftFadeLine(AuxPoint, ALength, ALength / 3, clBtnHighLight);
            AuxPoint.y -= 1;
            AuxPoint.x += ALength;
            DrawRightFadeLine(AuxPoint, ALength, ALength / 3, clBtnShadow);
            AuxPoint.y += 1;
            DrawRightFadeLine(AuxPoint, ALength, ALength div 3, clBtnHighLight);
            break;
        case lsMiddle:
            AuxPoint.y = Area.Top + (Area.Bottom - Area.Top) / 2;
            AuxCaption = Caption;
            if(LabelType == ltSubSuperScript)
                AuxCaption = ExtractCaption(Caption);
            switch(Alignment)
            {
                case taCenter:
                    AuxPoint.x = Area.left + 2;
                    ALength = ((Area.Right - 2) - (Area.Left + 2) - Canvas.TextWidth(AuxCaption) - 12) / 2;
                    DrawLeftFadeLine(AuxPoint, ALength, ALength / 3, clBtnShadow);
                    AuxPoint.y += 1;
                    DrawLeftFadeLine(AuxPoint, ALength, ALength / 3, clBtnHighLight);
                    AuxPoint.x = Area.Right - 2 - ALength;
                    AuxPoint.y -= 1;
                    DrawRightFadeLine(AuxPoint, ALength, ALength / 3, clBtnShadow);
                    AuxPoint.y += 1;
                    DrawRightFadeLine(AuxPoint, ALength, ALength / 3, clBtnHighLight);
                    break;
                case taLeftJustify:
                    AuxPoint.x = Area.left + Canvas.TextWidth(AuxCaption) + 6;
                    ALength = Area.Right - AuxPoint.x - 2;
                    DrawRightFadeLine(AuxPoint, ALength, ALength / 3, clBtnShadow);
                    AuxPoint.y += 1;
                    DrawRightFadeLine(AuxPoint, ALength, ALength / 3, clBtnHighLight);
                    break;
                case taRightJustify:
                    AuxPoint.x = Area.left + 2;
                    ALength = Area.Right - Canvas.TextWidth(AuxCaption) - 6 - AuxPoint.x;
                    DrawLeftFadeLine(AuxPoint, ALength, ALength / 3, clBtnShadow);
                    AuxPoint.y += 1;
                    DrawLeftFadeLine(AuxPoint, ALength, ALength / 3, clBtnHighLight);
                    break;
            }
    }
}

void TmyaSuperLabel :: set_ShadeRightBottom(TColor value)
{
    if(m_ShadeRightBottom == value)
        return;
    m_ShadeRightBottom = value;
    Invalidate();
}

void TmyaSuperLabel :: set_FitType(TFitType value)
{
    if(fFitType == value)
        return;
    fFitType = value;
    fOldSize = fOldSize + 1;
    Invalidate();
}

void TmyaSuperLabel :: set_AHShadeOffset(int value)
{
    if(value == m_AHShadeOffset)
        return;
    m_AHShadeOffset = value;
    Invalidate();
}

void TmyaSuperLabel :: set_AVShadeOffset(int value)
{
    if(value == m_AVShadeOffset)
        return;
    m_AVShadeOffset = value;
    Invalidate();
}

void TmyaSuperLabel :: set_ShadeLeftTop(TColor value)
{
    if(not (m_ShadeLeftTop == value) and (not m_ShadeLT))
    {
        m_ShadeLeftTop = value;
        Invalidate();
    }
}

void TmyaSuperLabel :: set_Effect3D(T3DEffect value)
{
    if(m_Effect3D == value)
        return;
    m_Effect3D = value;
    Invalidate();
}

void TmyaSuperLabel :: set_ShadeLT(bool value)
{
    if(m_ShadeLT == value)
        return;
    m_ShadeLT = value;
    if(m_ShadeLT)
    {
        fLast = m_ShadeLeftTop;
        m_ShadeLeftTop = clWhite;
    }
    else
        m_ShadeLeftTop = Flast;
    Invalidate();
}

void TmyaSuperLabel :: set_BeginColor(TColor value)
{
    if(m_BeginColor == value)
        return;
    m_BeginColor = value;
    Invalidate();
}

void TmyaSuperLabel :: set_EndColor(TColor value)
{
    if(m_EndColor == value)
        return;
    m_EndColor = value;
    Invalidate();
}

void TmyaSuperLabel :: set_GradientStyle(TGradientStyle value)
{
    if(m_GradientStyle == value)
        return;
    m_GradientStyle = value;
    Invalidate();
}

void TmyaSuperLabel :: MouseEnter(TShiftState state, int x, int y)
{
    if(LabelType > ltSubSuperScript)
    {
        if(Effect98)
        {
            fColorNormal = Font->Color;
            Font->Color = ColorSelected;
            Font->Style = Font->Style << fsUnderline;
            Invalidate();
        }
    }
    TCommonLabel :: MouseEnter(state, x, y);
}

void TmyaSuperLabel :: MouseLeave(TShiftState state, int x, int y)
{
    if(LabelType > ltSubSuperScript)
    {
        if(Effect98)
        {
            Font->Color = fColorNormal;
            Font->Style = Font->Style >> fsUnderline;
            Invalidate();
        }
    }
    TCommonLabel :: MouseLeave(state, x, y);
}

void TmyaSuperLabel :: set_LabelType(TLabelType value)
{
    if(m_LabelType == value)
        return;
    m_LabelType = value;
    if(m_LabelType in [ltHttp, ltFtp, ltNews, ltMail])
        Cursor = crHandPoint;
    else
        Cursor = crDefault;
    Invalidate();
}

void TmyaSuperLabel :: set_Effect98(bool value)
{
    if(m_Effect98 == value)
        return;
    m_Effect98 = value;
}

void TmyaSuperLabel :: set_Frame(bool value)
{
    if(m_Frame == value)
        return;
    m_Frame = value;
    Invalidate();
}

void TmyaSuperLabel :: set_LineStyle(TLineStyle value)
{
    if(m_LineStyle == value)
        return;
    m_LineStyle = value;
    Invalidate();
}

void TmyaSuperLabel :: Click()
{
    TString Choice, App;
    char TempPChar[80];

    TCommonLabel :: Click();
    if(Url == "")
        Choice = Caption;
    else
        Choice = Url;
    switch(LabelType)
    {
        case ltHttp: App = HTTP + Choice; break;
        case ltFtp: App = FTP + Choice; break;
        case ltNews: App = NEWS + Choice; break;
        case ltMail: App = MAIL + Choice + "?subject=" + MailSubject;
    }
    if(LabelType in [ltHttp, ltFtp, ltNews, ltMail])
    {
        StrPCopy(TempPChar,App);
        OpenObject(TempPChar);
    }
}

void TmyaSuperLabel :: OpenObject(char *sObjectPath)
{
/*    if(LabelType in [ltNormal, ltSubSuperScript])
        return;
    if(LabelType != ltHttp)
        ShellExecute(0, Nil, sObjectPath, Nil, Nil, SW_NORMAL);
    else
    {
        if ShellExecute(Application.Handle,PChar('open'),sObjectPath,PChar(''),nil, SW_NORMAL)<33 then
            if ShellExecute(Application.Handle,PChar('open'),PChar('netscape.exe'),sObjectPath, nil,SW_NORMAL)<32 then
                if ShellExecute(Application.Handle,PChar('open'),PChar('iexplore.exe'),sObjectPath,nil,SW_NORMAL)<32 then
                    ShowMessage ('Sorry your browser could not be found');
    } */
}

void TmyaSuperLabel :: DrawLeftFadeLine(TPoint &APoint, int ALength, int AFadeWidth, TColor AFadeColor)
{
    int R1, G1, B1, R2, G2, B2;
    TColor C;

    C = ColorToRGB(AFadeColor);
    R1 = TColorQuad(C).Red;
    G1 = TColorQuad(C).Green;
    B1 = TColorQuad(C).Blue;
  for(register int I = APoint.X; I <= APoint.X + ALength; I++)
  {
    if(I < (APoint.X + AFadeWidth))
    {
      C = Canvas.Pixels[I, APoint.Y];
      R2 = TColorQuad(C).Red;
      G2 = TColorQuad(C).Green;
      B2 = TColorQuad(C).Blue;
      R2 = R2+(((R1-R2)*(I-APoint.X)) div AFadeWidth);
      G2 = G2+(((G1-G2)*(I-APoint.X)) div AFadeWidth);
      B2 = B2+(((B1-B2)*(I-APoint.X)) div AFadeWidth);
      C = RGB(R2, G2, B2);
      Canvas.Pixels[I,APoint.Y] = C;
    }
    else
        Canvas.Pixels[I,APoint.Y] = AFadeColor;
  }
}

void TmyaSuperLabel :: DrawRightFadeLine(TPoint &APoint, int ALength, int AFadeWidth, TColor AFadeColor)
{
    int R1, G1, B1, R2, G2, B2;
    TColor C;

    C = ColorToRGB(AFadeColor);
    R1 = TColorQuad(C).Red;
    G1 = TColorQuad(C).Green;
    B1 = TColorQuad(C).Blue;
    for(register int I = APoint.X; I <= APoint.X + ALength; I++)
    {
        if(I > (APoint.X + Alength-AFadeWidth))
        {
            C = Canvas.Pixels[I, APoint.Y];
            R2 = TColorQuad(C).Red;
            G2 = TColorQuad(C).Green;
            B2 = TColorQuad(C).Blue;
            R2 = R2+(((R1-R2)*(APoint.X+ALength-I)) div AFadeWidth);
            G2 = G2+(((G1-G2)*(APoint.X+ALength-I)) div AFadeWidth);
            B2 = B2+(((B1-B2)*(APoint.X+ALength-I)) div AFadeWidth);
            C = RGB(R2, G2, B2);
            Canvas.Pixels[I,APoint.Y] := C;
        }
        else
            Canvas.Pixels[I,APoint.Y] = AFadeColor;
    }
}

/* I'll explain a little about the Horizontal gradient, the other styles are all
 consistent with their logic.  The six R, G, and B values are passed to us.
 We define some local variables we'll need: a rectangle, a FOR loop counter,
 and our own RGB numbers.  For a horizontal gradient, we'll draw a series of
 rectangles, each one a little closer in color to the EndClr value.  A horizontal
 gradient rectangle will always be from the top to the bottom of the canvas,
 so we set top to 0 and bottom to however tall our control is.  Then, we draw
 a series of 255 rectangles.  The starting point and width of each will depend
 on the actual width of our control.  It starts out on the left, draws the
 first rectangle in a color that's a percentage of the difference plus the
 starting color.  As I increments through the loop, the rectangles move to the
 right and the color gets closer and closer to the EndClr. */

void TmyaSuperLabel :: DoHorizontal(int fr, int fg, int fb, int dr, int dg, int db)
{
    TRect ColorRect;
    int I;
    byte_t R, G, B;

    ColorRect.top = 0;                    //{ Set rectangle top }
    ColorRect.Bottom = Height;
    for(I=0 ; I <= 255; I++)
    {                                   //{ Make lines (rectangles) of color }
        ColorRect.left = MulDiv(I, Width, 256);      //{ Find left for this color }
        ColorRect.Right = MulDiv(I+1, Width, 256);   //{ Find Right }
        R = fr + MulDiv(I, dr, 255);           //{ Find the RGB values }
        G = fg + MulDiv(I, dg, 255);
        B = fb + MulDiv(I, db, 255);
        Canvas.Brush->Color = RGB(R, G, B);   //{ Plug colors into brush }
        Canvas.FillRect(ColorRect);           //{ Draw on Bitmap }
    }
}

void TmyaSuperLabel :: DoVertical(int fr, int fg, int fb, int dr, int dg, int db)
{
    TRect ColorRect;
    int I;
    byte_t R, G, B;

    ColorRect.left = 0;                    //{ Set rectangle left & right }
    ColorRect.Right = Width;
    for(I = 0; I <= 255; I++)
    {                                   //{ Make lines (rectangles) of color }
        ColorRect.top = MulDiv(I, Height, 256);        //{ Find top for this color }
        ColorRect.Bottom = MulDiv(I+1, Height, 256);   // { Find Bottom }
        R = fr + MulDiv(I, dr, 255);                   // { Find the RGB values }
        G = fg + MulDiv(I, dg, 255);
        B = fb + MulDiv(I, db, 255);
        Canvas.Brush->Color = RGB(R, G, B);             //{ Plug colors into brush }
        Canvas.FillRect(ColorRect);                     //{ Draw on Bitmap }
    }
}

void TmyaSuperLabel :: DoElliptic(int fr, int fg, int fb, int dr, int dg, int db)
{
    byte_t R, G, B;
    Pw, Ph, x1,y1,x2,y2: Real;
    int I, cWidth, cHeight;
/*The elliptic is a bit different, since I had to use real numbers. I cut down
 on the number (to 155 instead of 255) of iterations in an attempt to speed
 things up, to no avail.  I think it just takes longer for windows to draw an
 ellipse as opposed to a rectangle.*/

    cWidth = Width;
    cHeight = Height;
//  cWidth := Canvas.ClipRect.Right-Canvas.ClipRect.Left;
//  cHeight := Canvas.ClipRect.Bottom-Canvas.ClipRect.Top;
    Canvas.Pen->Style = psClear;
    Canvas.Pen->Mode = pmCopy;
    x1 = 0 - (cWidth / 4);
    x2 = cWidth + (cWidth / 4)+4;
    y1 = 0 - (cHeight / 4);
    y2 = cHeight + (cHeight / 4)+4;
    Pw = ((cWidth / 4) + (cWidth / 2)) / 155;
    Ph = ((cHeight / 4) + (cHeight / 2)) / 155;
  for(I = 0; I <= 155; I++)
  {         //{ Make ellipses of color }
    x1 = x1 + Pw;
    x2 = X2 - Pw;
    y1 = y1 + Ph;
    y2 = y2 - Ph;
    R = fr + MulDiv(I, dr, 155);                       //{ Find the RGB values }
    G = fg + MulDiv(I, dg, 155);
    B = fb + MulDiv(I, db, 155);
    Canvas.Brush->Color = R or (G shl 8) or (b shl 16); //{ Plug colors into brush }
    Canvas.Ellipse(Trunc(x1),Trunc(y1),Trunc(x2),Trunc(y2));
  }
  Canvas.Pen->Style = psSolid;
}

void TmyaSuperLabel :: DoRectangle(int fr, int fg, int fb, int dr, int dg, int db)
{
    byte_t R, G, B;
    float Pw, Ph, x1,y1,x2,y2;
    int I, cWidth, cHeight;

//  cWidth := Canvas.ClipRect.Right-Canvas.ClipRect.Left;
//  cHeight := Canvas.ClipRect.Bottom-Canvas.ClipRect.Top;
    cWidth = Width;
    cHeight = Height;
    Canvas.Pen->Style = psClear;
    Canvas.Pen->Mode = pmCopy;
    x1 = 0;
    x2 = cWidth + 2;
    y1 = 0;
    y2 = cHeight + 2;
    Pw = (cWidth / 2) / 255;
    Ph = (cHeight / 2) / 255;
  for(I = 0; I <= 255; I++)
  {         //{ Make rectangles of color }
    x1 = x1 + Pw;
    x2 = X2 - Pw;
    y1 = y1 + Ph;
    y2 = y2 - Ph;
    R = fr + MulDiv(I, dr, 255);    //{ Find the RGB values }
    G = fg + MulDiv(I, dg, 255);
    B = fb + MulDiv(I, db, 255);
    Canvas.Brush->Color = RGB(R, G, B);   //{ Plug colors into brush }
    Canvas.FillRect(Rect(Trunc(x1),Trunc(y1),Trunc(x2),Trunc(y2)));
  }
  Canvas.Pen->Style = psSolid;
}

void TmyaSuperLabel :: DoVertCenter(int fr, int fg, int fb, int dr, int dg, int db)
{
    TRect ColorRect;
    byte_t R, G, B;
    int I, Haf, cWidth, cHeight;

//  cWidth := Canvas.ClipRect.Right-Canvas.ClipRect.Left;
//  cHeight := Canvas.ClipRect.Bottom-Canvas.ClipRect.Top;
    cWidth = Width;
    cHeight = Height;
    Haf = cHeight / 2;
    ColorRect.left = 0;
    ColorRect.Right = cWidth;
    for(I = 0; I <= Haf; I++)
    {
        ColorRect.top = MulDiv (I, Haf, Haf);
        ColorRect.Bottom = MulDiv (I + 1, Haf, Haf);
        R = fr + MulDiv(I, dr, Haf);
        G = fg + MulDiv(I, dg, Haf);
        B = fb + MulDiv(I, db, Haf);
        Canvas.Brush->Color = RGB(R, G, B);
        Canvas.FillRect(ColorRect);
        ColorRect.top = Height - (MulDiv (I, Haf, Haf));
        ColorRect.Bottom = Height - (MulDiv (I + 1, Haf, Haf));
        Canvas.FillRect(ColorRect);
    };
}

void TmyaSuperLabel :: DoHorizCenter(int fr, int fg, int fb, int dr, int dg, int db)
{
    TRect ColorRect;
    byte_t R, G, B;
    int I, Haf, cWidth, cHeight;

//  cWidth := Canvas.ClipRect.Right-Canvas.ClipRect.Left;
//  cHeight := Canvas.ClipRect.Bottom-Canvas.ClipRect.Top;
    cWidth = Width;
    cHeight = Height;
    Haf = cWidth / 2;
    ColorRect.top = 0;
    ColorRect.Bottom = cHeight;
  for(I = 0; I <= Haf; I++)
  {
    ColorRect.left = MulDiv (I, Haf, Haf);
    ColorRect.Right = MulDiv (I + 1, Haf, Haf);
    R = fr + MulDiv(I, dr, Haf);
    G = fg + MulDiv(I, dg, Haf);
    B = fb + MulDiv(I, db, Haf);
    Canvas.Brush->Color = RGB(R, G, B);
    Canvas.FillRect(ColorRect);
    ColorRect.left = Width - (MulDiv (I, Haf, Haf));
    ColorRect.Right = Width - (MulDiv (I + 1, Haf, Haf));
    Canvas.FillRect(ColorRect);
  }
}

TString TmyaSuperLabel :: ExtractCaption(TString value)
{
    byte_t nFor;
    TString AuxString;

    AuxString = "";
    for(nFor = 1; nFor <= Length(value); nFor++)
        if((value[nFor] != SUB) and (value[nFor] != SUPER))
            AuxString = AuxString + value[nFor];
    ExtractCaption = AuxString;
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
