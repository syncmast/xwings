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
#ifndef TLABEL_H_INCLUDED
#define TLABEL_H_INCLUDED

#include "comctrls.h"
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
//  TLabel
//---------------------------------------------------------------------------

/*-------------------------------------------------------------------------------
3D Label Effects :
 - Normal3d : like VCL TLabel
 - Resit3d : resit TLabel
 - Raised3d : raised TLabel
 - Shadowed3d : shadowed TLabel with selectable direction
-------------------------------------------------------------------------------*/
enum T3DEffect { Normal3d, Resit3d, Raised3d, Shadowed3d };
// Fit Type inside rectagle
enum TFitType { ftNormalFit, ftBestFitVert, ftBestFithorz, ftBestFitBoth };
// Gradient Style: If gsNone is selected no gradient style is applied
enum TGradientStyle { gsNone, gsHorizontal, gsVertical, gsElliptic, gsRectangle,
                    gsVertCenter, gsHorizCenter };
// Label Type: if ltNormal is selected the behaviour is like VCL Label
enum TLabelType { ltNormal, ltSubSuperScript, ltHttp, ltMail, ltFtp, ltNews };
// Line Style: if lsNone is selected no line is drawn
enum TLineStyle { lsNone, lsTop, lsBottom, lsMiddle };

struct TColorQuad {
    byte_t Red, Green, Blue, Alpha;
};

class TmyaSuperLabel : public TCommonLabel
{
    DECLARE_PROPERTY( TmyaSuperLabel );

private:
    TColor fColorNormal, m_ShadeRightBottom;
    TColor m_ShadeLeftTop, fLast, m_BeginColor, m_EndColor;
    TLabelType m_LabelType;
    bool m_Effect98, m_Frame, m_ShadeLT;
    TLineStyle m_LineStyle;
    int fOldWidth, fOldHeight, fOldSize, m_AHShadeOffset, m_AVShadeOffset;
    TFitType m_FitType;
    T3DEffect m_Effect3D;
    TGradientStyle m_GradientStyle;

    void set_Effect3D(T3DEffect value);
    void set_ShadeRightBottom(TColor value);
    void set_ShadeLeftTop(TColor value);
    void set_AHShadeOffset(int value);
    void set_AVShadeOffset(int value);
    void set_ShadeLT(bool value);
    void set_FitType(TFitType value);
    void set_BeginColor(TColor value);
    void set_EndColor(TColor value);
    void set_GradientStyle(TGradientStyle value);
    void set_LabelType(TLabelType value);
    void set_Effect98(bool value);
    void set_Frame(bool value);
    void set_LineStyle(TLineStyle value);

    void MouseEnter(TShiftState state, int x, int y);
    void MouseLeave(TShiftState state, int x, int y);
    void DoDrawText(const TRect &Area, word_t Flags);
    void NewDrawText(char *text, TRect &area, word_t flags);
    TString ExtractCaption(TString value);
    void DoHorizontal(int fr, int fg, int fb, int dr, int dg, int db); //These all draw
    void DoVertical(int fr, int fg, int fb, int dr, int dg, int db);  // the gradients.
    void DoElliptic(int fr, int fg, int fb, int dr, int dg, int db); // The fr fg fb etc
    void DoRectangle(int fr, int fg, int fb, int dr, int dg, int db); // are color values.
    void DoVertCenter(int fr, int fg, int fb, int dr, int dg, int db);
    void DoHorizCenter(int fr, int fg, int fb, int dr, int dg, int db);
    void DrawLeftFadeLine(TPoint APoint, int ALength, int AFadeWidth, TColor AFadeColor);
    void DrawRightFadeLine(TPoint APoint, int ALength, int AFadeWidth, TColor AFadeColor);

    void Init();
    __fastcall void Assign(const TmyaSuperLabel &obj);

protected:
    virtual void Click();
    virtual void Paint();

public:
    TString Url;
    TString MailSubject;
    TColor ColorSelected;

   	TmyaSuperLabel();
    __fastcall TmyaSuperLabel(const TmyaSuperLabel &a);
   	virtual ~TmyaSuperLabel();
   	__fastcall const TmyaSuperLabel & operator=(const TmyaSuperLabel &a);

    void OpenObject(char *sObjectPath); //{Related to hiperlink}

    property (m, int, AHShadeOffset);
    property (m, int, AVShadeOffset);
    property (m, TFitType, FitType);
    property (m, T3DEffect, Effect3D);
    property (m, TColor, ShadeRightBottom);
    property (m, TColor, ShadeLeftTop);
    property (m, bool, ShadeLT);
    property (m, TGradientStyle, GradientStyle);
    property (m, TColor, BeginColor);
    property (m, TColor, EndColor);
    property (m, bool, Frame);
    property (m, TLineStyle, LineStyle);
    property (m, TLabelType, LabelType);
    property (m, bool, Effect98);
};
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
#endif // TLABEL_H_INCLUDED
