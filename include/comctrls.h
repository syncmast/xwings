#ifndef COMCTRLS_H
#define COMCTRLS_H

#include "xgdi.h"
#include "application.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
//  TCommonButton
//---------------------------------------------------------------------------
class TCommonButton : public TWinControl
{
    DECLARE_PROPERTY(TCommonButton);

private:
    void Init();
    void __fastcall Assign(const TCommonButton &obj);

protected:

public:
    TCommonButton();
    __fastcall TCommonButton(const TCommonButton &a);
    virtual ~TCommonButton() = 0;
    __fastcall const TCommonButton & operator=(const TCommonButton &a);
};
//---------------------------------------------------------------------------
//  TCommonCheckBox
//---------------------------------------------------------------------------
enum TCheckBoxState {cbUnchecked, cbChecked, cbUndefined};

class TCommonCheckBox : public TCommonButton
{
    DECLARE_PROPERTY(TCommonCheckBox);

private:
    TCheckBoxState m_State;

    virtual void set_Checked(bool value);
    void set_State(TCheckBoxState value);
    void Init();
    void __fastcall Assign(const TCommonCheckBox &obj);

protected:
    void Click();

public:
    bool AllowUndefined;

    TCommonCheckBox();
    __fastcall TCommonCheckBox(const TCommonCheckBox &a);
    virtual ~TCommonCheckBox() = 0;
    __fastcall const TCommonCheckBox & operator=(const TCommonCheckBox &a);

    property(m, TCheckBoxState, State);
};
//---------------------------------------------------------------------------
//  TCommonComboBox
//---------------------------------------------------------------------------
class TCommonComboBox : public TWinControl
{
    DECLARE_PROPERTY(TCommonComboBox);

private:
    void Init();
    void __fastcall Assign(const TCommonComboBox &obj);

protected:

public:
    TCommonComboBox();
    __fastcall TCommonComboBox(const TCommonComboBox &a);
    virtual ~TCommonComboBox() = 0;
    __fastcall const TCommonComboBox & operator=(const TCommonComboBox &a);
};
//---------------------------------------------------------------------------
//  TCommonPanel
//---------------------------------------------------------------------------
class TCommonPanel : public TWinControl
{
    DECLARE_PROPERTY(TCommonPanel);

private:
    void Init();
    void __fastcall Assign(const TCommonPanel &obj);

protected:

public:
    TCommonPanel();
    __fastcall TCommonPanel(const TCommonPanel &a);
    virtual ~TCommonPanel() = 0;
    __fastcall const TCommonPanel & operator=(const TCommonPanel &a);
};
//---------------------------------------------------------------------------
//  TCommonGroupBox
//---------------------------------------------------------------------------
class TCommonGroupBox : public TWinControl
{
    DECLARE_PROPERTY(TCommonGroupBox);

private:
    void Init();
    void __fastcall Assign(const TCommonGroupBox &obj);

protected:

public:
    TCommonGroupBox();
    __fastcall TCommonGroupBox(const TCommonGroupBox &a);
    virtual ~TCommonGroupBox() = 0;
    __fastcall const TCommonGroupBox & operator=(const TCommonGroupBox &a);
};
//---------------------------------------------------------------------------
//  TCommonLabel
//---------------------------------------------------------------------------
class TCommonLabel : public TControl
{
    DECLARE_PROPERTY( TCommonLabel );

private:
    TAlignment get_Alignment();
    void set_Alignment(TAlignment value);
    bool get_ShowAccelChar();
    void set_ShowAccelChar(bool value);
    TTextLayout get_Layout();
    void set_Layout(TTextLayout value);
    bool get_WordWrap();
    void set_WordWrap(bool value);

    void Init();
    __fastcall void Assign(const TCommonLabel &obj);

protected:
    TFormatText m_FormatText;
    virtual void set_Caption(TString value);
//    virtual void AdjustSize();

public:
    TCommonLabel();
    __fastcall TCommonLabel(const TCommonLabel &a);
    virtual ~TCommonLabel() = 0;
    __fastcall const TCommonLabel & operator=(const TCommonLabel &a);

    property (rw, TAlignment, Alignment);
    property (rw, bool, ShowAccelChar);
    property (rw, TTextLayout, Layout);
    property (rw, bool, WordWrap);
};
//---------------------------------------------------------------------------
/*
TTextLayout = (tlTop, tlCenter, tlBottom);

  TCustomLabel = class(TGraphicControl)
  private
    FFocusControl: TWinControl;
    FAlignment: TAlignment;
    FAutoSize: Boolean;
    FLayout: TTextLayout;
    FWordWrap: Boolean;
    FShowAccelChar: Boolean;
    function GetTransparent: Boolean;
    procedure SetAlignment(Value: TAlignment);
    procedure SetFocusControl(Value: TWinControl);
    procedure SetShowAccelChar(Value: Boolean);
    procedure SetTransparent(Value: Boolean);
    procedure SetLayout(Value: TTextLayout);
    procedure SetWordWrap(Value: Boolean);
    procedure CMTextChanged(var Message: TMessage); message CM_TEXTCHANGED;
    procedure CMFontChanged(var Message: TMessage); message CM_FONTCHANGED;
    procedure CMDialogChar(var Message: TCMDialogChar); message CM_DIALOGCHAR;
  protected
    procedure AdjustBounds; dynamic;
    procedure DoDrawText(var Rect: TRect; Flags: Longint); dynamic;
    function GetLabelText: string; virtual;
    procedure Loaded; override;
    procedure Notification(AComponent: TComponent;
      Operation: TOperation); override;
    procedure Paint; override;
    procedure SetAutoSize(Value: Boolean); override;
    property Alignment: TAlignment read FAlignment write SetAlignment
      default taLeftJustify;
    property AutoSize: Boolean read FAutoSize write SetAutoSize default True;
    property FocusControl: TWinControl read FFocusControl write SetFocusControl;
    property ShowAccelChar: Boolean read FShowAccelChar write SetShowAccelChar default True;
    property Transparent: Boolean read GetTransparent write SetTransparent default False;
    property Layout: TTextLayout read FLayout write SetLayout default tlTop;
    property WordWrap: Boolean read FWordWrap write SetWordWrap default False;
  public
    constructor Create(AOwner: TComponent); override;
    property Caption;
    property Canvas;
  end;

  TLabel = class(TCustomLabel)
  published
    property Align;
    property Alignment;
    property Anchors;
    property AutoSize;
    property BiDiMode;
    property Caption;
    property Color;
    property Constraints;
    property DragCursor;
    property DragKind;
    property DragMode;
    property Enabled;
    property FocusControl;
    property Font;
    property ParentBiDiMode;
    property ParentColor;
    property ParentFont;
    property ParentShowHint;
    property ShowAccelChar;
    property Transparent;
    property Layout;
    property Visible;
    property WordWrap;

  end;

*/
//---------------------------------------------------------------------------
//  TCommonScrollbar
//---------------------------------------------------------------------------
enum TScrollbarKind { sbHorizontal, sbVertical };
enum TScrollCode { scLineUp, scLineDown, scPageUp, scPageDown, scPosition, scTrack, scTop, scBottom, scEndScroll };

typedef delegate< void (TObject* Sender, TScrollCode ScrollCode, int &ScrollPos) > TScrollEvent;

class TCommonScrollbar : public TControl
{
    DECLARE_PROPERTY(TCommonScrollbar);

private:
    int m_Max;
    int m_Min;
    int m_PageSize;
    int m_Position;
    TScrollbarKind m_Kind;

    int m_Delay;
    int m_PageDiv;
    int m_LineDiv;
    bool m_UpdateNeeded;
    bool m_Scaled;
    int m_CalcRange;
    short m_PageIncrement;
    short m_ButtonSize;
    int m_Range;
    int m_Size;
    int m_ThumbSize;
    int get_ScrollPos();
    void set_Max(int value);
    void set_Min(int value);
    void set_PageSize(int value);
    void set_ButtonSize(short value);
    void set_Position(int value);
    void set_Range(int value);
    void set_Size(int value);
    void set_ThumbSize(int value);
    void set_Kind(TScrollbarKind value);

    void Init();
    void __fastcall Assign(const TCommonScrollbar &obj);

protected:

public:
    short Increment;
    short Margin;
    bool Tracking;
    bool Smooth;
    word_t LargeChange;
    word_t SmallChange;

    TCommonScrollbar();
    __fastcall TCommonScrollbar(const TCommonScrollbar &a);
    virtual ~TCommonScrollbar();
    __fastcall const TCommonScrollbar & operator=(const TCommonScrollbar &a);

    TScrollEvent OnScroll;

    property (m, short, ButtonSize);
    property (m, TScrollbarKind, Kind);
    property (m, int, Position);
    property (m, int, Range);
    property (r, int, ScrollPos);
    property (m, int, Size);
    property (m, int, ThumbSize);
    property (m, int, Max);
    property (m, int, Min);
    property (m, int, PageSize);
};
//---------------------------------------------------------------------------
//  TCommonEdit
//---------------------------------------------------------------------------
enum TEditCharCase { ecNormal, ecUpperCase, ecLowerCase };

class TCommonEdit : public TWinControl
{
    DECLARE_PROPERTY(TCommonEdit);

private:
    void Init();
    void __fastcall Assign(const TCommonEdit &obj);
    void __fastcall DoSelectionRequest(XEvent &event);
    void __fastcall DoSelectionNotify(XEvent &event);
    bool __fastcall WndProc(XEvent &event);

protected:
    virtual void DoEnter();
    virtual void DoExit();

public:
    bool AutoSelect;
    TCaretKind CaretKind;
    TEditCharCase CharCase;
    int  MaxLength;
    char PasswordChar;
    bool ReadOnly;

    TCommonEdit();
    __fastcall TCommonEdit(const TCommonEdit &a);
    virtual ~TCommonEdit();
    __fastcall const TCommonEdit & operator=(const TCommonEdit &a);

    void Clear();
    void CopyToClipboard();
    void PasteFromClipboard();
};
/*
  TCustomEdit = class(TWinControl)
  private
    FAutoSize: Boolean;
    FAutoSelect: Boolean;
    FHideSelection: Boolean;
    FCreating: Boolean;
    FModified: Boolean;
    FOnChange: TNotifyEvent;
    procedure AdjustHeight;
    function GetModified: Boolean;
    function GetCanUndo: Boolean;
    procedure SetBorderStyle(Value: TBorderStyle);
    procedure SetHideSelection(Value: Boolean);
    procedure SetModified(Value: Boolean);
    procedure SetSelText(const Value: string);
    procedure UpdateHeight;
    procedure WMSetFont(var Message: TWMSetFont); message WM_SETFONT;
    procedure CMCtl3DChanged(var Message: TMessage); message CM_CTL3DCHANGED;
    procedure CMEnter(var Message: TCMGotFocus); message CM_ENTER;
    procedure CMFontChanged(var Message: TMessage); message CM_FONTCHANGED;
    procedure CNCommand(var Message: TWMCommand); message CN_COMMAND;
    procedure CMTextChanged(var Message: TMessage); message CM_TEXTCHANGED;
    procedure WMContextMenu(var Message: TWMContextMenu);
      message WM_CONTEXTMENU;
  protected
    procedure Change; dynamic;
    procedure CreateParams(var Params: TCreateParams); override;
    procedure CreateWindowHandle(const Params: TCreateParams); override;
    procedure CreateWnd; override;
    procedure DestroyWnd; override;
    function GetSelLength: Integer; virtual;
    function GetSelStart: Integer; virtual;
    function GetSelText: string; virtual;
    procedure SetAutoSize(Value: Boolean); override;
    procedure SetSelLength(Value: Integer); virtual;
    procedure SetSelStart(Value: Integer); virtual;
    property AutoSelect: Boolean read FAutoSelect write FAutoSelect default True;
    property AutoSize: Boolean read FAutoSize write SetAutoSize default True;
    property HideSelection: Boolean read FHideSelection write SetHideSelection default True;
    property OnChange: TNotifyEvent read FOnChange write FOnChange;
  public
    procedure ClearSelection;
    procedure CutToClipboard;
    procedure DefaultHandler(var Message); override;
    procedure Undo;
    procedure ClearUndo;
    function GetSelTextBuf(Buffer: PChar; BufSize: Integer): Integer; virtual;
    procedure SelectAll;
    procedure SetSelTextBuf(Buffer: PChar);
    property CanUndo: Boolean read GetCanUndo;
    property Modified: Boolean read GetModified write SetModified;
    property SelLength: Integer read GetSelLength write SetSelLength;
    property SelStart: Integer read GetSelStart write SetSelStart;
    property SelText: string read GetSelText write SetSelText;
    property Text;
  published
    property TabStop default True;
  end;

    TCaretEdit = class(TCustomControl)
  private
    FActive: Boolean;
    FCaretPosY: Integer;
    FCaretPosX: Integer;
    FCharMatrix: array of Char;
    FCharX: Integer;
    FCharY: Integer;
    FPicture: TPicture;
    FWantReturns: Boolean;
    FWantTabs: Boolean;
    FWindowX: Integer;
    FWindowY: Integer;
    function GetCharMatrix(X, Y: Integer): Char;
    procedure SetCharMatrix(X, Y: Integer; const Value: Char);
    procedure WMChar(var Message: TWMChar); message WM_CHAR;
    procedure WMGetDlgCode(var Message: TWMGetDlgCode); message WM_GETDLGCODE;
    procedure WMKeyDown(var Message: TWMKeyDown); message WM_KEYDOWN;
    procedure WMKillFocus(var Message: TWMSetFocus); message WM_KILLFOCUS;
    procedure WMLButtonDown(var Message: TWMLButtonDown); message WM_LBUTTONDOWN;
    procedure WMSetFocus(var Message: TWMSetFocus); message WM_SETFOCUS;
    property CaretPosX: Integer read FCaretPosX write FCaretPosX;
    property CaretPosY: Integer read FCaretPosY write FCaretPosY;
    property CharMatrix[X, Y: Integer]: Char read GetCharMatrix write SetCharMatrix;
    property CharX: Integer read FCharX write FCharX;
    property CharY: Integer read FCharY write FCharY;
    property WindowX: Integer read FWindowX write FWindowX;
    property WindowY: Integer read FWindowY write FWindowY;
  protected
    procedure Paint; override;
    procedure Resize; override;
  public

  published
    property WantReturns: Boolean read FWantReturns write FWantReturns default True;
    property WantTabs: Boolean read FWantTabs write FWantTabs default True;

  end;

*/
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
#endif // COMCTRLS_H
