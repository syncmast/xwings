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
enum TLabelType { ltNormal, ltSubSuperScript, ltHttp, ltMail, ltFtp, ltNews );
// Line Style: if lsNone is selected no line is drawn
enum TLineStyle { lsNone, lsTop, lsBottom, lsMiddle };

struct TColorQuad {
    byte_t Red, Green, Blue, Alpha;
};

class TmyaSuperLabel : public TCommonLabel
{
    DECLARE_PROPERTY( TmyaSuperLabel );

private:
    TColor fColorNormal, fColorSelected, fShadowColor;
    TColor fWhiteColor, fLast, fBeginColor, fEndColor;
    TLabelType fLabelType;
    bool fEffect98, fFrame, fShadeLTSet;
    TString fUrl, fMailSubject;
    TLineStyle fLineStyle;
    int fOldWidth, fOldHeight, fOldSize, fhOffSet, fvOffSet;
    TFitType fFitType;
    T3DEffect f3DEffect;
    TGradientStyle fGradientStyle;

    void set_StyleEffect(T3DEffect value);
    void set_ShadowColor(TColor value);
    void set_WhiteColor(TColor value);
    void SetFhOffSet(value: integer);
    void SetFvOffSet(value: integer);
    void SetShadeLT(value: boolean);
    void SetFitType(Value: TFitType);
    void SetBeginColor(Value: TColor);
    void SetEndColor(Value: TColor);
    void SetGradient(Value: TGradientStyle);
    void SetLabelType(const Value: TLabelType);
    void SetEffect98(const Value: Boolean);
    void SetFrame(const Value: Boolean);
    void SetLineStyle(const Value: TLineStyle);
    void SuperLabelMouseEnter(var Message: TMessage); message CM_MOUSEENTER;
    void SuperLabelMouseLeave(var Message: TMessage); message CM_MOUSELEAVE;
    void DoDrawText(const TRect &Area, word_t Flags);
    void NewDrawText(Text: PChar; var Area: TRect; Flags: Word);
    TString ExtractCaption(TString value);
    void DoHorizontal(int fr, int fg, int fb, int dr, int dg, int db); //These all draw
    void DoVertical(int fr, int fg, int fb, int dr, int dg, int db);  // the gradients.
    void DoElliptic(int fr, int fg, int fb, int dr, int dg, int db); // The fr fg fb etc
    void DoRectangle(int fr, int fg, int fb, int dr, int dg, int db); // are color values.
    void DoVertCenter(int fr, int fg, int fb, int dr, int dg, int db);
    void DoHorizCenter(fr, fg, fb, dr, dg, db: Integer);
    void DrawLeftFadeLine(APoint: TPoint; ALength, AFadeWidth: Integer;
                               AFadeColor: TColor);
    void DrawRightFadeLine(APoint: TPoint; ALength, AFadeWidth: Integer;
                                AFadeColor: TColor);

    void Init();
    __fastcall void Assign(const TmyaSuperLabel &obj);

protected:
    virtual void Click();
    virtual void Paint();

public:
   	TmyaSuperLabel();
    __fastcall TmyaSuperLabel(const TmyaSuperLabel &a);
   	virtual ~TmyaSuperLabel();
   	__fastcall const TmyaSuperLabel & operator=(const TmyaSuperLabel &a);

    procedure OpenObject(sObjectPath: PChar); {Related to hiperlink}

    property Align;
    property Caption;
    Property AFitType: TFitType read FFitType write SetFitType;

    property AStyle3D: T3DEffect read F3DEffect write setStyleEffect default Normal3d;
    property AShadeRightBottom: TColor read FShadowColor write SetShadowColor default clGray;
    property AShadeLeftTop: TColor read FWhiteColor write SetWhiteColor default clWhite;
    property AHShadeOffSet: integer read FhOffSet write SetFhOffSet default 5;
    property AVShadeOffSet: integer read FvOffSet write SetFvOffSet default -5;
    property AShadeLTSet: boolean read FShadeLTSet write setShadeLT default true;

    property GradientStyle: TGradientStyle read FGradientStyle write SetGradient default gsNone;
    property BeginColor: TColor read FBeginColor write SetBeginColor default clNavy;
    property EndColor: TColor read FEndColor write SetEndColor default clAqua;

    property Frame: Boolean read fFrame write SetFrame default False;

    property LineStyle: TLineStyle read fLineStyle write SetLineStyle default lsNone;

    property LabelType: TLabelType read fLabelType write SetLabelType default ltNormal;
    property Effect98: Boolean read fEffect98 write SetEffect98 default True;
    property ColorSelected: TColor read fColorSelected write fColorSelected default clGreen;
    property MailSubject: string read fMailSubject write fMailSubject;
    property Url: string read fUrl write fUrl;
    property OnMouseEnter: TNotifyEvent read fOnMouseEnter write fOnMouseEnter;
    property OnMouseLeave: TNotifyEvent read fOnMouseLeave write fOnMouseLeave;

    property DragCursor;
    property DragMode;
    property Enabled;
    property Font;
    property ParentColor;
    property ParentFont;
    property ParentShowHint;
    property PopupMenu;
    property ShowHint;
    property Visible;
    property Transparent;
    property Width;
    property Top;
    property Left;
    property Height;
    property OnClick;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnEndDrag;
    property OnMouseDown;
    property OnMouseUp;
};
//---------------------------------------------------------------------------
#endif // TLABEL_H_INCLUDED
