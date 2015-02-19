//---------------------------------------------------------------------------
#ifndef xgdiH
#define xgdiH

#include "graphics.h"
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
enum TCursor
{
    crDefault,
    crNone,
    crArrow,
    crCross,
    crIBeam,
    crSize,
    crSizeNESW,
    crSizeNS,
    crSizeNWSE,
    crSizeWE,
    crUpArrow,
    crHourGlass,
    crDrag,
    crNoDrop,
    crHSplit,
    crVSplit,
    crMultiDrag,
    crSQLWait,
    crNo,
    crAppStart,
    crHelp,
    crHandPoint,
    crSizeAll,
    crUserCursor
};
/*
static const TCursor  crDefault     = None;
static const TCursor  crNone        = None;
static const TCursor  crArrow       = XC_left_ptr;
static const TCursor  crCross       = XC_crosshair;
static const TCursor  crIBeam       = XC_xterm;
static const TCursor  crSize        = XC_fleur;
static const TCursor  crSizeNESW    = -6;
static const TCursor  crSizeNS      = XC_sb_v_double_arrow;
static const TCursor  crSizeNWSE    = -8;
static const TCursor  crSizeWE      = XC_sb_h_double_arrow;
static const TCursor  crUpArrow     = XC_sb_up_arrow;
static const TCursor  crHourGlass   = XC_watch;
static const TCursor  crDrag        = -12;
static const TCursor  crNoDrop      = -13;
static const TCursor  crHSplit      = XC_sb_h_double_arrow;
static const TCursor  crVSplit      = XC_double_arrow;
static const TCursor  crMultiDrag   = -16;
static const TCursor  crSQLWait     = -17;
static const TCursor  crNo          = -18;
static const TCursor  crAppStart    = XC_rtl_logo;
static const TCursor  crHelp        = XC_question_arrow;
static const TCursor  crHandPoint   = XC_hand2;
static const TCursor  crSizeAll     = XC_fleur;
*/
// #define MAX_X_CURSOR   152
// #define MAX_STD_CURSORS   MAX_X_CURSOR / 2

//typedef word_t TShortCut;
//static const word_t scShift = 0x2000;
//static const word_t scCtrl = 0x4000;
//static const word_t scAlt = 0x8000;
//static const word_t scNone = 0x0;

enum TDragMode { dmDragNone, dmDragDrop, dmDragDock };
enum TDragState { dsDragEnter, dsDragExit, dsDragMove };
enum TDragKind { dkDrag, dkDock };

enum TMouseButton
{
    mbLeft = XButton1,
    mbMiddle = XButton2,
    mbRight = XButton3,
    mbWheelUp = XButton4,
    mbWheelDown = XButton5
};

enum __TShiftStates
{
    ssShift         = ShiftMask,
    ssAlt	        = Mod1Mask,
    ssCtrl          = ControlMask,
    ssLeft	        = Button1Mask,
    ssRight	        = Button3Mask,
    ssMiddle        = Button2Mask,
    ssNumLock       = Mod2Mask,
    ssCapsLock      = LockMask,
    ssWin	        = Mod4Mask,
    ssScrollLock    = 1 << 13,
};
typedef uint_t TShiftState;

typedef delegate< void (TObject* Sender, TMouseButton button, TShiftState state, int y, int x) > TMouseEvent;
typedef delegate< void (TObject* Sender, TShiftState state, int y, int x) > TMouseMoveEvent;
typedef delegate< void (TObject* Sender, uint_t &key, uint_t state) > TKeyEvent;
typedef delegate< void (TObject* Sender, char &key) > TKeyPressEvent;
typedef delegate< void (TObject* Sender, TObject* &DragObject, TShiftState state, int x, int y) > TStartDragEvent;
typedef delegate< void (TObject* Sender, TObject* Source, TShiftState state, int x, int y) > TEndDragEvent;
typedef delegate< bool (TObject* Sender, TObject* Source, TShiftState state, int x, int y, TDragState dstate) > TDragOverEvent;
typedef delegate< bool (TObject* Sender) > TCloseQueryEvent;
typedef delegate< bool (TObject* Sender, int x, int y) > TContextPopupEvent;

enum TLedState
{
    lsCapsLock = 1,
    lsNumLock = 2,
    lsScrollLock = 4
};

class TControl;

class TSizeConstraints // : public TObject
{
    DECLARE_PROPERTY( TSizeConstraints );

private:
    uint_t m_MaxHeight;
    uint_t m_MaxWidth;
    uint_t m_MinHeight;
    uint_t m_MinWidth;
    TControl *m_owner;
    void set_MaxHeight(uint_t value);
    void set_MaxWidth(uint_t value);
    void set_MinHeight(uint_t value);
    void set_MinWidth(uint_t value);
    void Change();

public:
    __fastcall TSizeConstraints(const TControl *owner);

    property (m, uint_t, MaxHeight);
    property (m, uint_t, MaxWidth);
    property (m, uint_t, MinHeight);
    property (m, uint_t, MinWidth);
};

enum TAnchors
{
    akLeft   = 1 << 0,
    akTop    = 1 << 1,
    akRight  = 1 << 2,
    akBottom = 1 << 3
};

enum TControlStyle
{
    csButton = 1 << 0,
    csPopup = 1 << 1,
    csFollowMouse = 1 << 2,
    csClickEvents = 1 << 3,
    csTransparent = 1 << 4,
    csDoubleClicks = 1 << 5,
    csFixedWidth = 1 << 6,
    csFixedHeight = 1 << 7,
    csAllowRepeat = 1 << 8,
    csNoStdEvents = 1 << 9,
    csDisplayDragImage = 1 << 10,
    csActionClient = 1 << 11,
    csMenuEvents = 1 << 12,
    csShapeBounding = 1 << 13,
    csShapeClip = 1 << 14,
    csShapeInput = 1 << 15,
    csRadioItem = 1 << 16,
    csAutoSize = 1 << 17
};

enum TControlState
{
    csActive = 1 << 0,
    csChecked = 1 << 1,
    csDefault = 1 << 2,
    csDocking = 1 << 3,
    csDrag = 1 << 4,
    csEnabled = 1 << 5,
    csFocused = 1 << 6,
    csGrabKeyboard = 1 << 7,
    csGrabMouse = 1 << 8,
    csLButtonDown = 1 << 9,
    csLoading = 1 << 10,
    csMouseDown = 1 << 11,
    csMouseOver = 1 << 12,
    csMouseRepeat = 1 << 13,
    csSelected = 1 << 14,
    csShowing = 1 << 15,
    csUndefined = 1 << 16,
    csVisible = 1 << 17
};

class TWinControl;
class TCustomForm;
class TCustomPopupMenu;

//---------------------------------------------------------------------------
//  TControl
//---------------------------------------------------------------------------
class TControl : public TObject
{
    DECLARE_PROPERTY( TControl );

private:
    TSizeConstraints m_Constraints;
    TColor 	     m_Color;
    TAlign       m_Align;
    TString      m_Caption;
    TPoint       m_anchor;
    int			 m_Anchors;
    TCursor      m_Cursor;
    TFont 		*m_Font;
    int          m_ControlState;
    int          m_ControlStyle;
    byte_t       m_GroupIndex;

    void Init();
    void __fastcall Assign(const TControl &obj);

    inline bool get_Active()    { return CheckState(csActive); };
    inline bool get_AutoSize()  { return CheckStyle(csAutoSize); };
    inline bool get_Checked()   { return CheckState(csChecked); };
    inline bool get_Default()   { return CheckState(csDefault); };
    inline bool get_Enabled()   { return CheckState(csEnabled); };
    inline bool get_MouseOver() { return CheckState(csMouseOver); };
    inline bool get_RadioItem() { return CheckStyle(csRadioItem); };
    inline bool get_Selected()  { return CheckState(csSelected);  };
    inline bool get_Showing()   { return CheckState(csShowing);   };
    inline bool get_Transparent() { return CheckStyle(csTransparent); };
    inline bool get_Undefined() { return CheckState(csUndefined); };
    inline bool get_Visible()   { return CheckState(csVisible);   };

    uint_t get_ClientHeight();
    uint_t get_ClientWidth();

    void set_Visible(bool value);

    void set_Anchors(int value);
    void set_AutoSize(bool value);
    void set_Left(int value);
    void set_Top(int value);
    void set_Width(uint_t value);
    void set_Height(uint_t value);
    void set_ClientHeight(uint_t value);
    void set_ClientWidth(uint_t value);
    inline int get_Left() { return get_BoundsRect().left; };
    inline int get_Top()  { return get_BoundsRect().top;  };
    inline uint_t get_Width() { return get_BoundsRect().width; };
    inline uint_t get_Height() { return get_BoundsRect().height; };
    inline TRect get_BoundsRect() { return m_Bounds.ClipRect; };

    TCustomForm *get_ParentForm();
    void set_Align(TAlign value);
    inline TSizeConstraints *get_Constraints() { return &m_Constraints; };
    void set_Font(TFont *value);
    void __fastcall UpdateAnchor(const TRect &prect);
    void TurnSiblingsOff();

protected:
    TRegion  m_Bounds;
    TBitmap *m_Bitmap;
    TWinControl *m_Parent;

    virtual	void set_Active(bool value);
    virtual	void set_Checked(bool value);
    virtual	void set_Default(bool value);
    virtual void set_Enabled(bool value);
    virtual	void set_RadioItem(bool value);
    virtual void set_Transparent(bool value);
    virtual void set_Selected(bool value);
    virtual void set_Undefined(bool value);
    virtual void set_GroupIndex(byte_t value);

    virtual TPoint get_ClientOrigin();
    virtual void  set_BoundsRect(TRect rect);
    virtual void set_Bounds( TRegion rgn );
    virtual TRect get_ClientRect();
    virtual TRegion get_ClientRegion();

    virtual	void set_Bitmap(TBitmap *value);
    virtual void set_Color(TColor AColor);
    virtual void set_Caption(TString value);
    virtual void set_Parent(TWinControl *AParent);
    inline virtual void set_Cursor(TCursor value) { m_Cursor = value; };
    virtual void AdjustSize();
    void __fastcall ApplyConstraints(TRect &oldrect, TRect &newrect);
    bool __fastcall DispatchMessage(XEvent &event);

    void __fastcall SetState(const TControlState value, bool on);
    inline void __fastcall StateOn(int value)  { m_ControlState |= value;  };
    inline void __fastcall StateOff(int value) { m_ControlState &= ~value; };

    void __fastcall SetStyle(const TControlStyle value, bool on);
    inline void __fastcall StyleOn(int value)  { m_ControlStyle |= value;  };
    inline void __fastcall StyleOff(int value) { m_ControlStyle &= ~value; };

    virtual void DoShow();
    virtual void DoHide();
    virtual void __fastcall MouseMove(TShiftState state, int x, int y);
    virtual void __fastcall MouseDown(TMouseButton button, TShiftState state, int x, int y);
    virtual void __fastcall MouseUp(TMouseButton button, TShiftState state, int x, int y);
    virtual void Click();
    virtual void DblClick();
    virtual void TripleClick();
    virtual void __fastcall MouseEnter(TShiftState state, int x, int y);
    virtual void __fastcall MouseExit(TShiftState state, int x, int y);
    virtual void Resize();
    virtual void __fastcall SetZOrder(bool topmost);
    virtual void __fastcall BeginDrag(TShiftState state, int x, int y);
    virtual void __fastcall EndDrag(TShiftState state, int x, int y);
    virtual bool __fastcall DragOver(TObject *source, TShiftState state, int x, int y, TDragState dstate);

public:

    TCanvas             Canvas;

    TCursor 			DragCursor;
    TString				Text;
    TString   			Hint;
    bool     			ShowHint;
    TDragMode 			DragMode;
    TDragKind			DragKind;
    TCustomPopupMenu 	*PopupMenu;
    int 				ImageIndex;

    TNotifyEvent 		OnClick;
    TContextPopupEvent 	OnContextPopup;
    TNotifyEvent 		OnDblClick;
    TNotifyEvent 		OnTripleClick;
    TNotifyEvent 		OnResize;
    TMouseEvent  		OnMouseDown;
    TMouseEvent  		OnMouseUp;
    TMouseMoveEvent 	OnMouseMove;
    TMouseMoveEvent 	OnMouseEnter;
    TMouseMoveEvent 	OnMouseExit;
    TNotifyEvent     	OnPaint;

    TStartDragEvent 	OnStartDrag;
    TEndDragEvent   	OnEndDrag;
    TDragOverEvent  	OnDragOver;

    TControl();
    __fastcall TControl(const TControl &a);
    virtual ~TControl() = 0;
    __fastcall const TControl & operator=(const TControl &a);

    void __fastcall ApplyAnchorRules(TRect &newparentrect, TRect &outrect);
    inline void BringToFront() { SetZOrder(true); };
    inline bool Dragging()     { return CheckState(csDrag); };
    inline bool CheckStyle(const TControlStyle value)  { return m_ControlStyle & value; };
    inline bool CheckState(const TControlState value)  { return m_ControlState & value; };
    virtual void Invalidate();
    inline void Refresh() { Repaint(); };
    inline void SendToBack() { SetZOrder(false);};
    inline void Show() { Visible = true; };
    inline void Hide() { Visible = false; };
    virtual void Paint();
    virtual void Repaint();
    inline void SetBounds(int left, int top, uint_t width, uint_t height) { set_BoundsRect(TRect(left, top, width, height)); };
    virtual void Update();
    virtual bool __fastcall WndProc(XEvent &event);
    virtual void Scale(int MV, int DV, int MH = 0, int DH = 0);
    TPoint __fastcall ClientToParent(const TPoint &point, TWinControl *AParent = NULL);
    TPoint __fastcall ClientToScreen(const TPoint &point);
    TPoint __fastcall ParentToClient(const TPoint &point, TWinControl *AParent = NULL);
    TPoint __fastcall ScreenToClient(const TPoint &point);

    property (rw,	bool,		Active);
    property (m,	TAlign, 	Align);
    property (rw, 	bool,		AutoSize);
    property (m,    int,        Anchors);
    property (m, 	TBitmap *,	Bitmap);
    property (rw, 	TRect,		BoundsRect);
    property (m, 	TRegion,	Bounds);
    property (m, 	TString, 	Caption);
    property (rw, 	bool, 		Checked);
    property (r,    TPoint,     ClientOrigin);
    property (r, 	TRect, 		ClientRect);
    property (r, 	TRegion, 	ClientRegion);
    property (rw,   uint_t,     ClientHeight);
    property (rw,   uint_t,     ClientWidth);
    property (m, 	TColor, 	Color);
    property (r, 	TSizeConstraints *, Constraints);
    property (m,    TCursor,    Cursor);
    property (rw, 	bool, 		Default);
    property (m, 	TFont *, 	Font);
    property (m,    byte_t,     GroupIndex);
    property (rw,   bool,       RadioItem);
    property (rw, 	int, 		Left);
    property (rw, 	int, 		Top);
    property (rw, 	uint_t, 	Width);
    property (rw, 	uint_t, 	Height);
    property (rw, 	bool, 		Enabled);
    property (r, 	bool, 		MouseOver);
    property (m, 	TWinControl *, Parent);
    property (r, 	TCustomForm *, ParentForm);
    property (rw, 	bool, 		Selected);
    property (r, 	bool, 		Showing);
    property (rw, 	bool, 		Transparent);
    property (rw, 	bool, 		Undefined);
    property (rw, 	bool, 		Visible);
};
/*
  TControl = class(TComponent)
  private
    FParentFont: Boolean;
    FParentColor: Boolean;
    FAutoSize: Boolean;
    FDragMode: TDragMode;
    FBiDiMode: TBiDiMode;
    FParentBiDiMode: Boolean;
    FDragCursor: TCursor;
    FPopupMenu: TPopupMenu;
    FHint: string;
    FScalingFlags: TScalingFlags;
    FShowHint: Boolean;
    FParentShowHint: Boolean;
    FDragKind: TDragKind;
    FDockOrientation: TDockOrientation;
    FHostDockSite: TWinControl;
    FUndockWidth: Integer;
    FUndockHeight: Integer;
    FLRDockWidth: Integer;
    FTBDockHeight: Integer;
    FFloatingDockSiteClass: TWinControlClass;
    FHelpType: THelpType;
    FHelpKeyword: String;
    FHelpContext: THelpContext;
    procedure CalcDockSizes;
    function CreateFloatingDockSite(Bounds: TRect): TWinControl;
    function DoCanAutoSize(var NewWidth, NewHeight: Integer): Boolean;
    function DoCanResize(var NewWidth, NewHeight: Integer): Boolean;
    procedure DoConstraintsChange(Sender: TObject);
    procedure DoConstrainedResize(var NewWidth, NewHeight: Integer);
    procedure DoDragMsg(var DragMsg: TCMDrag);
    procedure FontChanged(Sender: TObject);
    function GetClientHeight: Integer;
    function GetClientWidth: Integer;
    function GetLRDockWidth: Integer;
    function GetMouseCapture: Boolean;
    function GetTBDockHeight: Integer;
    function GetUndockWidth: Integer;
    function GetUndockHeight: Integer;
    procedure ReadIsControl(Reader: TReader);
    procedure SetClientHeight(Value: Integer);
    procedure SetClientSize(Value: TPoint);
    procedure SetClientWidth(Value: Integer);
    procedure SetHelpContext(const Value: THelpContext);
    procedure SetHelpKeyword(const Value: String);
    procedure SetHostDockSite(Value: TWinControl);
    procedure SetMouseCapture(Value: Boolean);
    procedure SetParentColor(Value: Boolean);
    procedure SetParentFont(Value: Boolean);
    procedure SetShowHint(Value: Boolean);
    procedure SetParentShowHint(Value: Boolean);
    procedure SetPopupMenu(Value: TPopupMenu);
    procedure SetZOrderPosition(Position: Integer);
    procedure WriteIsControl(Writer: TWriter);

  protected
    procedure AssignTo(Dest: TPersistent); override;
    procedure BeginAutoDrag; dynamic;
    function CanResize(var NewWidth, NewHeight: Integer): Boolean; virtual;
    function CanAutoSize(var NewWidth, NewHeight: Integer): Boolean; virtual;
    procedure ChangeScale(M, D: Integer); dynamic;
    procedure ConstrainedResize(var MinWidth, MinHeight, MaxWidth, MaxHeight: Integer); virtual;
    function CalcCursorPos: TPoint;
    procedure DefaultDockImage(DragDockObject: TDragDockObject; Erase: Boolean); dynamic;
    procedure DefineProperties(Filer: TFiler); override;
    procedure DockTrackNoTarget(Source: TDragDockObject; X, Y: Integer); dynamic;
    procedure DoContextPopup(MousePos: TPoint; var Handled: Boolean); dynamic;
    procedure DoEndDock(Target: TObject; X, Y: Integer); dynamic;
    procedure DoDock(NewDockSite: TWinControl; var ARect: TRect); dynamic;
    procedure DoStartDock(var DragObject: TDragObject); dynamic;
    procedure DragCanceled; dynamic;
    procedure DragOver(Source: TObject; X, Y: Integer; State: TDragState;
      var Accept: Boolean); dynamic;
    procedure DoEndDrag(Target: TObject; X, Y: Integer); dynamic;
    procedure DoStartDrag(var DragObject: TDragObject); dynamic;
    procedure DrawDragDockImage(DragDockObject: TDragDockObject); dynamic;
    procedure EraseDragDockImage(DragDockObject: TDragDockObject); dynamic;
    function GetDeviceContext(var WindowHandle: HWnd): HDC; virtual;
    function GetDockEdge(MousePos: TPoint): TAlign; dynamic;
    function GetDragImages: TDragImageList; virtual;
    function GetFloating: Boolean; virtual;
    function GetFloatingDockSiteClass: TWinControlClass; virtual;
    procedure PositionDockRect(DragDockObject: TDragDockObject); dynamic;
    procedure ReadState(Reader: TReader); override;
    procedure RequestAlign; dynamic;
    procedure SendCancelMode(Sender: TControl);
    procedure SendDockNotification(Msg: Cardinal; WParam, LParam: Integer);
    procedure SetAutoSize(Value: Boolean); virtual;
    procedure SetDragMode(Value: TDragMode); virtual;
    procedure SetParentComponent(Value: TComponent); override;
    procedure SetZOrder(TopMost: Boolean); dynamic;
    property AutoSize: Boolean read FAutoSize write SetAutoSize default False;
    property DragKind: TDragKind read FDragKind write FDragKind default dkDrag;
    property DragCursor: TCursor read FDragCursor write FDragCursor default crDrag;
    property DragMode: TDragMode read FDragMode write SetDragMode default dmManual;
    property MouseCapture: Boolean read GetMouseCapture write SetMouseCapture;
    property ParentColor: Boolean read FParentColor write SetParentColor default True;
    property ParentFont: Boolean read FParentFont write SetParentFont default True;
    property ParentShowHint: Boolean read FParentShowHint write SetParentShowHint default True;
    property ScalingFlags: TScalingFlags read FScalingFlags write FScalingFlags;
    property OnCanResize: TCanResizeEvent read FOnCanResize write FOnCanResize;
    property OnConstrainedResize: TConstrainedResizeEvent read FOnConstrainedResize write FOnConstrainedResize;
    property OnDragDrop: TDragDropEvent read FOnDragDrop write FOnDragDrop;
    property OnDragOver: TDragOverEvent read FOnDragOver write FOnDragOver;
    property OnEndDock: TEndDragEvent read FOnEndDock write FOnEndDock;
    property OnEndDrag: TEndDragEvent read FOnEndDrag write FOnEndDrag;
    property OnStartDock: TStartDockEvent read FOnStartDock write FOnStartDock;
    property OnStartDrag: TStartDragEvent read FOnStartDrag write FOnStartDrag;
  public
    procedure BeginDrag(Immediate: Boolean; Threshold: Integer = -1);
    function ClientToParent(const Point: TPoint; AParent: TWinControl = nil): TPoint;
    procedure Dock(NewDockSite: TWinControl; ARect: TRect); dynamic;
    function Dragging: Boolean;
    procedure DragDrop(Source: TObject; X, Y: Integer); dynamic;
    procedure EndDrag(Drop: Boolean);
    function GetControlsAlignment: TAlignment; dynamic;
    function GetTextBuf(Buffer: PChar; BufSize: Integer): Integer;
    function GetTextLen: Integer;
    function IsRightToLeft: Boolean;
    function ManualDock(NewDockSite: TWinControl; DropControl: TControl = nil;
      ControlSide: TAlign = alNone): Boolean;
    function ManualFloat(ScreenPos: TRect): Boolean;
    function ReplaceDockedControl(Control: TControl; NewDockSite: TWinControl;
      DropControl: TControl; ControlSide: TAlign): Boolean;
    function ParentToClient(const Point: TPoint; AParent: TWinControl = nil): TPoint;
    procedure SetTextBuf(Buffer: PChar);
    function UseRightToLeftAlignment: Boolean; dynamic;
    function UseRightToLeftReading: Boolean;
    function UseRightToLeftScrollBar: Boolean;
    property ControlState: TControlState read FControlState write FControlState;
    property DockOrientation: TDockOrientation read FDockOrientation write FDockOrientation;
    property Floating: Boolean read GetFloating;
    property FloatingDockSiteClass: TWinControlClass read GetFloatingDockSiteClass write FFloatingDockSiteClass;
    property HostDockSite: TWinControl read FHostDockSite write SetHostDockSite;
    property LRDockWidth: Integer read GetLRDockWidth write FLRDockWidth;
    property TBDockHeight: Integer read GetTBDockHeight write FTBDockHeight;
    property UndockHeight: Integer read GetUndockHeight write FUndockHeight;
    property UndockWidth: Integer read GetUndockWidth write FUndockWidth;
  published
    property HelpType: THelpType read FHelpType write FHelpType default htContext;
    property HelpKeyword: String read FHelpKeyword write SetHelpKeyword stored IsHelpContextStored;
    property HelpContext: THelpContext read FHelpContext write SetHelpContext stored IsHelpContextStored default 0;
end;
*/
//---------------------------------------------------------------------------
//  TButtonControl
//---------------------------------------------------------------------------
class TButtonControl : public TControl
{
private:
    void Init();
    __fastcall void Assign(const TButtonControl &obj);

protected:

public:
    TButtonControl();
    __fastcall TButtonControl(const TButtonControl &a);
    virtual ~TButtonControl();
    __fastcall const TButtonControl & operator=(const TButtonControl &a);
};
//---------------------------------------------------------------------------
//  TPainter
//---------------------------------------------------------------------------
class TPainter : public TObject
{
private:
    void Init();
    __fastcall void Assign(const TPainter &obj);

protected:
    TCanvas *m_Canvas;

public:
    TPainter();
    __fastcall TPainter(const TPainter &a);
    virtual ~TPainter();
    __fastcall const TPainter & operator=(const TPainter &a);

    virtual void __fastcall PaintControl(TControl &ctrl) = 0;
};
//---------------------------------------------------------------------------
//  TWinControl
//---------------------------------------------------------------------------
enum TBackingStore
{
    bsNotUseful = NotUseful,
    bsWhenMapped = WhenMapped,
    bsAlways = Always
};

enum TShapeOperation
{
    shSet,
    shUnion,
    shIntersect,
    shSubtract,
    shInvert
};

enum TShapeKind
{
    shBounding,
    shClip,
    shInput
};

struct TShapeParam
{
    TShapeOperation op;
    TShapeKind kind;
    int xoffset;
    int yoffset;
    TShapeKind src_kind;
    int ordering;
    TShapeParam() : op(shSet), kind(shBounding),
        xoffset(0), yoffset(0), src_kind(shBounding), ordering(0)
    {
    }
};

typedef delegate< void (TObject* Sender, TShapeKind kind, const TRect &rect, bool shaped) > TChangeShapeEvent;

class TWinControl : public TControl
{
    DECLARE_PROPERTY(TWinControl);

private:
    bool m_statichandle;
    uint_t m_width;
    uint_t m_height;
    TList m_Controls;
    TRegion m_updatergn;
    TColor m_BorderColor;
    TBitmap *m_BorderBitmap;

    void Init();
    void __fastcall Assign(const TWinControl &obj);
    void __fastcall DoConfigure(XEvent &event);
    void CreateHandle();
    void set_BorderColor(TColor AColor);
    HWND get_ParentWindow();
    void set_ParentWindow(HWND value);
    uint_t get_BorderWidth();
    void set_BorderWidth(uint_t value);
    virtual	void set_Bitmap(TBitmap *value);
    virtual void set_Color(TColor AColor);
    virtual void set_Enabled(bool value);
    inline TControl *get_Controls(const int index)
    {
        return reinterpret_cast<TControl *>(m_Controls[index]);
    };
    inline short get_ControlCount()
    {
        return m_Controls.Count;
    };
    inline bool get_GrabKeyboard()
    {
        return CheckState(csGrabKeyboard);
    };
    void set_GrabKeyboard(bool value);
    inline bool get_GrabMouse()
    {
        return CheckState(csGrabMouse);
    };
    void set_GrabMouse(bool value);
    void set_BorderBitmap(TBitmap *value);
    bool get_Focused();
    void set_Focused(bool value);
    bool get_SaveUnder();
    void set_SaveUnder(bool value);
    TBackingStore get_BackingStore();
    void set_BackingStore(TBackingStore value);
    long get_EventMask();
    void set_EventMask(long value);

protected:
    HWND m_Handle;

    TRect get_ClientRect();
    virtual void  set_BoundsRect(TRect rect);
    virtual TPoint get_ClientOrigin();
    virtual void set_Cursor(TCursor value);
    virtual void set_Parent(TWinControl *AParent);

    virtual void Create();
    virtual void __fastcall ChangeShape(TShapeKind kind, const TRect &rect, bool shaped);
    virtual void DoHide();
    virtual void DoShow();
    virtual void __fastcall SetZOrder(bool topmost);
    virtual void DoEnter();
    virtual void DoExit();

    TNotifyEvent OnEnter;
    TNotifyEvent OnExit;
    TKeyEvent OnKeyUp;
    TKeyEvent OnKeyDown;
    TKeyPressEvent OnKeyPress;

    property (rw, long, EventMask);
    property (rw, bool, GrabKeyboard);
    property (rw, bool, GrabMouse);
    property (rw, bool, SaveUnder);
    property (rw, TBackingStore, BackingStore);

public:
    short TabOrder;
    bool TabStop;

    TNotifyEvent OnCreate;
    TChangeShapeEvent OnChangeShape;

    TWinControl();
    __fastcall TWinControl(HWND win);
    __fastcall TWinControl(const TWinControl &a);
    virtual ~TWinControl() = 0;
    __fastcall const TWinControl & operator=(const TWinControl &a);

    void Broadcast(XEvent &event);
    bool CanFocus();
    TControl *ControlAtPos(const TPoint &p);
    TControl *ControlOnState(const TControlState &state);
//   bool ContainsControl(TControl* AControl);
    void DestroyHandle();
    TWinControl *FindWinControl(HWND wnd);
    void InsertControl(TControl *control);
    virtual void Invalidate();
//    inline void InvalidateRect(const TRect &rect) { m_updatergn += rect; };
    inline void __fastcall InvalidateRgn(const TRegion &rgn)
    {
        m_updatergn += rgn;
    };
    virtual void __fastcall KeyDown(uint_t &key, uint_t state);
    virtual void __fastcall KeyPress(char &key);
//   virtual void KeyString(WideString &S, bool &Handled);
    virtual void __fastcall KeyUp(uint_t &key, uint_t state);
    void MoveControl(TControl *control, bool topmost);
    void RemoveControl(TControl *control);
    virtual void Repaint();
    void SetShape(const TRegion &region, const TShapeParam *param = NULL);
    void SetShape(const TBitmap &bmp, const TShapeParam *param = NULL);
    void SetShape(const TWinControl &src, const TShapeParam *param = NULL);
    void SetShape(TShapeKind kind, int xoff, int yoff);
    void SetShape(const TRect *rects, int nrects, const TShapeParam *param = NULL);
    void RemoveShape(TShapeKind kind = TShapeKind(-1));

    virtual void ScrollBy(int delta_x, int delta_y);
    virtual void Scale(int MV, int DV, int MH = 0, int DH = 0);
    virtual bool __fastcall WndProc(XEvent &event);
    virtual void Update();
    void PaintControl(TControl *ctrl);
//   void UpdateRect(const TRect &rect);

    property (m, TBitmap *, BorderBitmap);
    property (rw, uint_t, BorderWidth);
    property (m, TColor, BorderColor);
    property_i (r, TControl *, int, Controls);
    property (r, short, ControlCount);
    property (rw, bool, Focused);
    property (mr, HWND, Handle);
    property (rw, HWND, ParentWindow);
};
/*
  TWidgetControl = class(TControl)
  private
    FAlignLevel: Word;
    FTabStop: Boolean;
    FStyle: TWidgetStyle;
    FTabList: TList;
    FTabOrder: Integer;
    FMasked: Boolean;
    FInputKeys: TInputKeys;
    procedure DoFlipChildren;
    function CheckInputKeys(Key: Integer; Shift: TShiftState; const KeyText: WideString): Boolean;
    function GetTabOrder: TTabOrder;
    function IsMenuKey(Key: Integer; Shift: TShiftState; const KeyText: WideString): Boolean;
    procedure RemoveFocus(Removing: Boolean);
    procedure SetParentWidget(const Value: QWidgetH);
    procedure SetTabOrder(const Value: TTabOrder);
    procedure SetTabStop(const Value: Boolean);
    procedure UpdateBounds;
    procedure UpdateShowing;
    procedure UpdateTabOrder(Value: TTabOrder);
    function GetStyle: TWidgetStyle;
    procedure SetStyle(const Value: TWidgetStyle);
    procedure SetMasked(const Value: Boolean);
  protected
    procedure AdjustClientRect(var Rect: TRect); virtual;
    function GetChildHandle: QWidgetH; virtual;
    function CustomAlignInsertBefore(C1, C2: TControl): Boolean; virtual;
    procedure CustomAlignPosition(Control: TControl; var NewLeft, NewTop, NewWidth,
      NewHeight: Integer; var AlignRect: TRect); virtual;
    procedure AlignControls(AControl: TControl; var Rect: TRect); virtual;
    procedure ChangeScale(MV, DV, MH, DH: Integer); override;
    procedure GetChildren(Proc: TGetChildProc; Root: TComponent); override;
    function FindNextControl(CurControl: TWidgetControl; GoForward, CheckTabStop, CheckParent: Boolean): TWidgetControl;
    procedure KeyString(var S: WideString; var Handled: Boolean); dynamic;
    procedure MaskChanged; dynamic;
    function NeedKey(Key: Integer; Shift: TShiftState; const KeyText: WideString): Boolean; dynamic;
    procedure ReadState(Reader: TReader); override;
    procedure RestoreWidgetState; virtual;
    procedure SaveWidgetState; virtual;
    procedure ScaleControls(MV, DV: Integer; MH: Integer = 0; DH: Integer = 0);
    procedure ScrollControls(DeltaX, DeltaY: Integer; IsVisible: Boolean);
    procedure SetChildOrder(Child: TComponent; Order: Integer); override;
    procedure SelectFirst;
    function SelectNext(CurControl: TWidgetControl; GoForward, CheckTabStop: Boolean): Boolean;
    procedure SetZOrderPosition(Position: Integer);
    procedure ShowControl(AControl: TControl); virtual;
    procedure ShowHintChanged; override;
    procedure StyleChanged(Sender: TObject); dynamic;
    procedure TabStopChanged; dynamic;
    function WantKey(Key: Integer; Shift: TShiftState; const KeyText: WideString): Boolean; override;
    property InputKeys: TInputKeys read FInputKeys write FInputKeys;
    property Masked: Boolean read FMasked write SetMasked default False;
    property Style: TWidgetStyle read GetStyle write SetStyle;
  public
    constructor CreateParented(ParentWidget: QWidgetH);
    class function CreateParentedControl(ParentWidget: QWidgetH): TWidgetControl;
    function ControlAtPos(const Pos: TPoint; AllowDisabled: Boolean;
      AllowWidgets: Boolean = False): TControl;
    procedure DisableAlign;
    procedure EnableAlign;
    function FindChildControl(const ControlName: string): TControl;
    procedure FlipChildren(AllLevels: Boolean); dynamic;
    procedure GetTabOrderList(List: TList); dynamic;
    procedure InvokeHelp; virtual;
    procedure Realign;
    procedure ScaleBy(MV, DV: Integer; MH: Integer = 0; DH: Integer = 0);
    procedure ScrollBy(DeltaX, DeltaY: Integer); virtual;
    procedure UpdateControlState;
    property TabOrder: TTabOrder read GetTabOrder write SetTabOrder default -1;
    property TabStop: Boolean read FTabStop write SetTabStop default False;
  end;
*/
//---------------------------------------------------------------------------
}  // namespace
//---------------------------------------------------------------------------
#endif
