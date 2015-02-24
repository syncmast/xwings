//---------------------------------------------------------------------------

#ifndef stdctrlsH
#define stdctrlsH

#include "xgdi.h"
#include "comctrls.h"
#include "menu.h"
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
enum TArrowOrientation { arrowLeft, arrowUp, arrowRight, arrowDown };

void DrawButtonArrow(TCanvas *ACanvas, const TRect &rect, TArrowOrientation orientation, bool down = false);
void DrawArrow(TCanvas *ACanvas, TColor color, const TRect &rect, TArrowOrientation orientation, bool fill = false);
void DrawGradient(TCanvas *ACanvas, TRect ARect, bool IsRightToLeft);
void FrameRounded(TCanvas &canvas, TRect &rect, TColor color, int r);

enum TEdgeStyle {esNone, esRaised, esLowered};
enum TEdgeBorder {ebLeft = (1 << 0),
                  ebTop = (1 << 1),
                  ebRight = (1 << 2),
                  ebBottom = (1 << 3),
                  ebRect = (ebLeft | ebTop | ebRight | ebBottom)
                  };

TRect DrawButtonFace(TCanvas *canvas, const TRect &Client, bool IsDown, bool IsFocused, bool Flat, TColor FillColor);
void DrawDisabledText (TCanvas *Canvas, const TString &text, TRect &rect, TFormatText *format = NULL);
void DrawEdge(TCanvas *canvas, const TRect &rect, TEdgeStyle EdgeInner, TEdgeStyle EdgeOuter, int EdgeBorders, int EdgeWidth = 0);
void Frame3D(TCanvas *canvas, const TRect &arect, TColor topcolor, TColor bottomcolor, int dwidth = 1);
//---------------------------------------------------------------------------
//  MenuItem
//---------------------------------------------------------------------------
class TMenuItem : public TCommonMenuItem
{
	DECLARE_PROPERTY(TMenuItem);

private:
	int FMinWidth;
    int FMinHeight;
	TColor FGutterColor;
	TColor FMenuColor;
	TColor FSelectedColor;//{выделенный пункт меню}
	TColor FSelLightColor;

protected:
   	void Paint();
   	virtual void __fastcall MeasureItem(uint_t &width, uint_t &height);
   	void __fastcall DoDrawText(const TString &ACaption,
  TRect &Rect, bool Selected, int Flags);

public:
	TMenuItem();
//	__fastcall TMenuItem(TWinControl *AParent);
};
//---------------------------------------------------------------------------
//  PopupMenu
//---------------------------------------------------------------------------
class TPopupMenu : public TCustomPopupMenu
{
	DECLARE_PROPERTY(TPopupMenu);

private:
	void Init();

protected:

public:
	TPopupMenu();
};
//---------------------------------------------------------------------------
//  MainMenu
//---------------------------------------------------------------------------
class TMainMenu : public TCustomMainMenu
{
private:

protected:
	virtual void Init();

public:
	__fastcall TMainMenu();
	__fastcall TMainMenu(TWinControl *AParent);

};
//---------------------------------------------------------------------------
//  TLabel
//---------------------------------------------------------------------------
class TLabel : public TCommonLabel
{
    DECLARE_PROPERTY( TLabel );

private:
    void Init();
    void __fastcall Assign(const TLabel &obj);

protected:
    virtual void AdjustSize();

public:
   	TLabel();
    __fastcall TLabel(const TLabel &a);
   	virtual ~TLabel();
   	__fastcall const TLabel & operator=(const TLabel &a);
   	virtual void Paint();

};


class TCustomLabel : public TControl
{
    DECLARE_PROPERTY( TCustomLabel );

private:
    TWinControl *m_FocusControl;
    TAlignment m_Alignment;
    TTextLayout m_Layout;
    bool m_WordWrap;
    bool m_ShowAccelChar;

    void set_Alignment(TAlignment Value);
    void set_WordWrap(bool Value);
    void set_Layout(TTextLayout Value);
    void set_ShowAccelChar(bool Value);
    void set_FocusControl(TWinControl *Value);

    void Init();
    void __fastcall Assign(const TCustomLabel &obj);

protected:
    virtual void AdjustSize();
    virtual void DoDrawText(TRect &Rect, int Flags);

public:
   	TCustomLabel();
    __fastcall TCustomLabel(const TCustomLabel &a);
   	virtual ~TCustomLabel();
   	__fastcall const TCustomLabel & operator=(const TCustomLabel &a);
   	virtual void Paint();

    property (m, TAlignment, Alignment);
    property (m, TWinControl *, FocusControl);
    property (m, TTextLayout, Layout);
    property (m, bool, ShowAccelChar);
    property (m, bool, WordWrap);

};
//---------------------------------------------------------------------------
//  SpeedButton
//---------------------------------------------------------------------------
enum TButtonState {bsUp, bsDisabled, bsDown, bsExclusive};

class TSpeedButton : public TButtonControl
{
private:
   TButtonState FState;

protected:
   	void Paint();

public:
   bool  Down;
   bool  Flat;
   int   FGroupIndex;
   bool  Transparent;
   TSpeedButton();
   __fastcall TSpeedButton(TWinControl *AParent);
};
//---------------------------------------------------------------------------
//  TButton
//---------------------------------------------------------------------------
class TButton : public TCommonButton
{
	DECLARE_PROPERTY(TButton);

private:
    void Init();
    void __fastcall Assign(const TButton &obj);

protected:

public:
   	TButton();
    __fastcall TButton(const TButton &a);
   	virtual ~TButton();
   	__fastcall const TButton & operator=(const TButton &a);
};
//---------------------------------------------------------------------------
//  TCheckBox
//---------------------------------------------------------------------------
class TCheckBox : public TCommonCheckBox
{
	DECLARE_PROPERTY(TCheckBox);

private:
    void Init();
    void __fastcall Assign(const TCheckBox &obj);

protected:

public:
   	TCheckBox();
    __fastcall TCheckBox(const TCheckBox &a);
   	virtual ~TCheckBox();
   	__fastcall const TCheckBox & operator=(const TCheckBox &a);
};
//---------------------------------------------------------------------------
//  TComboBox
//---------------------------------------------------------------------------
class TComboBox : public TCommonComboBox
{
	DECLARE_PROPERTY(TComboBox);

private:
    void Init();
    void __fastcall Assign(const TComboBox &obj);

protected:
    void Paint();

public:
    TColor ButtonColor;
    TColor BorderColor;
    TColor ArrowColor;

   	TComboBox();
    __fastcall TComboBox(const TComboBox &a);
   	virtual ~TComboBox();
   	__fastcall const TComboBox & operator=(const TComboBox &a);

};
/*
  TALCustomComboBox = class(TCustomComboBox)
  private

    FForceDrawFocused : Boolean;
    FMouseInControl: boolean;

    FCustomProperty: String;
    procedure CNCommand(var Message: TWMCommand); message CN_COMMAND;
    Procedure PaintCombo;
    Procedure ReplaceInvalidateInQueueByRefresh;
  protected

    Property BorderColor: Tcolor read FBorderColor Write FBorderColor Default Clblack;
    Property ArrowColor: Tcolor read FArrowColor Write FArrowColor Default Clblack;
    Property ButtonColor: Tcolor read FButtonColor Write FButtonColor Default clBtnFace;

    Property CustomProperty: String read FcustomProperty write FcustomProperty;
  public
    property ForceDrawFocused: Boolean read FForceDrawFocused;
    Property MouseInControl : boolean read FMouseInControl;

  published
  end;
*/
//---------------------------------------------------------------------------
//  TPanel
//---------------------------------------------------------------------------
class TPanel : public TCommonPanel
{
	DECLARE_PROPERTY(TPanel);

private:
    void Init();
    void __fastcall Assign(const TPanel &obj);

protected:

public:
    TEdgeStyle  BevelInner;
    TEdgeStyle  BevelOuter;
    short       BevelWidth;

   	TPanel();
    __fastcall TPanel(const TPanel &a);
   	virtual ~TPanel();
   	__fastcall const TPanel & operator=(const TPanel &a);
};
//---------------------------------------------------------------------------
//  TGroupBox
//---------------------------------------------------------------------------
class TGroupBox : public TCommonGroupBox
{
	DECLARE_PROPERTY(TGroupBox);

private:
    void Init();
    void __fastcall Assign(const TGroupBox &obj);

protected:

public:
   	TGroupBox();
    __fastcall TGroupBox(const TGroupBox &a);
   	virtual ~TGroupBox();
   	__fastcall const TGroupBox & operator=(const TGroupBox &a);

};
//---------------------------------------------------------------------------
//  Bevel
//---------------------------------------------------------------------------
enum TBevelStyle { bsLowered, bsRaised };
enum TBevelShape { bsBox, bsFrame, bsTopLine, bsBottomLine, bsLeftLine,
                   bsRightLine, bsSpacer};

class TBevel : public TControl
{
	DECLARE_PROPERTY(TBevel);

private:
   TBevelStyle m_Style;
   TBevelShape m_Shape;
   inline TBevelStyle get_Style() { return m_Style;};
   void set_Style(TBevelStyle AStyle);
   inline TBevelShape get_Shape() { return m_Shape;};
   void set_Shape(TBevelShape AShape);

protected:
   void Paint();

public:
	__fastcall TBevel();
	__fastcall TBevel(TWinControl *AParent);

   	property (rw, TBevelShape, Shape);
   	property (rw, TBevelStyle, Style);
};
//---------------------------------------------------------------------------
//  TEdit
//---------------------------------------------------------------------------
class TEdit : public TCommonEdit
{
	DECLARE_PROPERTY(TEdit);

private:
    void Init();
    void __fastcall Assign(const TEdit &obj);

protected:
    void Paint();

public:
   	TEdit();
    __fastcall TEdit(const TEdit &a);
   	virtual ~TEdit();
   	__fastcall const TEdit & operator=(const TEdit &a);
};
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
#endif
