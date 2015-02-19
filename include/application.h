//---------------------------------------------------------------------------
#ifndef ApplicationH
#define ApplicationH

#include <locale.h>
#include "forms.h"
//---------------------------------------------------------------------------
#define XPROTO_TCP   1
#define XPROTO_DEC   2

#define ALLFONTS "-*-*-*-*-*-*-*-*-*-*-*-*-*-*"
//---------------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------------
extern "C" {
    int __fastcall GetFontPath(TStrings &out);
    int __fastcall GetListExtensions(TStrings &out);
    int __fastcall ExtractFontName(const char *name, int partno, char* buf);
    int __fastcall SelectFontName(const char *tmpl, int maxnum, TStrings &out);
    void __fastcall SetFontPath(TStrings *dirs);
    void __fastcall SendMessage(TWinControl *dest, TMessage &msg);
    bool __fastcall UseXExtension(const char *xextname);
};
//---------------------------------------------------------------------------
//  TClipboard
//---------------------------------------------------------------------------
class TClipboard : public TObject
{
    DECLARE_PROPERTY(TClipboard);

private:
    void Init();
    __fastcall void Assign(const TClipboard &obj);

protected:

public:
    TClipboard();
    __fastcall TClipboard(const TClipboard &a);
    virtual ~TClipboard();
    __fastcall const TClipboard & operator=(const TClipboard &a);
};
//---------------------------------------------------------------------------
// TKeyboard
//---------------------------------------------------------------------------
class TKeyboard : public TObject
{
    DECLARE_PROPERTY(TKeyboard);
    DECLARE_COPY_DISABLE(TKeyboard);

private:
    int get_LedState();
    bool get_AutoRepeat();
    void set_AutoRepeat(bool value);

protected:

public:
    TKeyboard();
    ~TKeyboard();

    property (rw, bool, AutoRepeat);
    property (r, int, LedState);
};
//---------------------------------------------------------------------------
// TMouse
//---------------------------------------------------------------------------
#define ACCEL_DEFAULT	-1

enum TMouseStateKind {msLButtonDown, msMouseDown, msDrag };
typedef TSet<TMouseStateKind>  TMouseState;

struct TMouseAccelerator
{
    int Numerator;
    int Denominator;
    int Threshold;
};

class TMouse : public TObject
{
    DECLARE_PROPERTY(TMouse);
    DECLARE_COPY_DISABLE(TMouse);

private:
    TPoint get_CursorPos();
    void set_CursorPos(TPoint value);
    TPoint get_CursorSize();
    TMouseAccelerator get_Accelerator();
    void set_Accelerator(TMouseAccelerator value);

protected:

public:
    uint_t DragThreshold;
    uint_t RepeatThreshold;
    uint_t DblClickThreshold;
    TMouseState State;
    TMouse();
    ~TMouse();

    property (rw, TMouseAccelerator, Accelerator);
    property (rw, TPoint, CursorPos);
    property (r, TPoint, CursorSize);
};
//---------------------------------------------------------------------------
//  TCaret
//---------------------------------------------------------------------------
enum TCaretKind { ctUnderline, ctVerticalLine, ctSolidBlock };

class TCaret : public TObject
{
    DECLARE_PROPERTY(TCaret);

private:
    TTimer m_timer;
    TCanvas *m_canvas;
    bool m_show;
    inline uint_t get_BlinkTime()
    {
        return m_timer.Interval;
    };
    void set_BlinkTime(uint_t value);
    void DoBlink(TObject *sender);
    void Init();
    void __fastcall Assign(const TCaret &obj);

protected:

public:
    TPoint Pos;
    TColor Color;
    TCaretKind Kind;
    TCaret();
    __fastcall TCaret(const TCaret &a);
    virtual ~TCaret();
    __fastcall const TCaret & operator=(const TCaret &a);
    void __fastcall Show(TWinControl *win);
    void Hide();

    property (rw, uint_t, BlinkTime);
};
//---------------------------------------------------------------------------
// TMonitor
//---------------------------------------------------------------------------
class TMonitor : public TObject
{
    DECLARE_PROPERTY(TMonitor);
    DECLARE_COPY_DISABLE(TMonitor);

private:
    HMONITOR m_Handle;
    TCanvas m_Canvas;

    short get_MonitorNum();
    bool get_Primary();
    /*
        function GetLeft: Integer;
        function GetHeight: Integer;
        function GetTop: Integer;
        function GetWidth: Integer;
        function GetBoundsRect: TRect;
        function GetWorkareaRect: TRect;
    */
    inline TBackingStore get_BackingStore()
    {
        return (TBackingStore)DoesBackingStore(m_Handle);
    };
    inline TCanvas *get_Canvas()
    {
        return &m_Canvas;
    };
    inline HCOLORMAP get_Colormap()
    {
        return  DefaultColormapOfScreen(m_Handle);
    };
    TPixelSize get_Depth();
    HGC get_GC();
    short get_Height();
    short get_HeightMM();
    short get_Planes();
    HWND get_Root();
    inline bool get_SaveUnder()
    {
        return DoesSaveUnders(m_Handle);
    };
    inline HVISUAL get_Visual()
    {
        return DefaultVisualOfScreen(m_Handle);
    };
    short get_Xdpi();
    short get_Ydpi();
    short get_Width();
    short get_WidthMM();
    TRect get_WorkArea();

public:
    __fastcall TMonitor(int screen_number);

    property (mr, HMONITOR, Handle);
    property (r, short, MonitorNum);
    property (r, bool, Primary);
    /*
        property Left: Integer read GetLeft;
        property Height: Integer read GetHeight;
        property Top: Integer read GetTop;
        property Width: Integer read GetWidth;
        property BoundsRect: TRect read GetBoundsRect;
        property WorkareaRect: TRect read GetWorkareaRect;
    */
    property (r, TBackingStore, BackingStore);
    property (r, TCanvas *, Canvas);
    property (r, HCOLORMAP, Colormap);
//	property (m, TCursor, Cursor);
    property (r, TPixelSize, Depth);
    property (r, HGC, GC);
    property (r, short, Height);
    property (r, short, HeightMM);
    property (r, short, Planes);
    property (r, HWND, Root);
    property (r, bool, SaveUnder);
    property (r, HVISUAL, Visual);
    property (r, short, Width);
    property (r, short, WidthMM);
    property (r, TRect, WorkArea);
    property (r, short, Xdpi);
    property (r, short, Ydpi);

};
//---------------------------------------------------------------------------
// TScreen
//---------------------------------------------------------------------------
class TScreen : public TObject
{
    DECLARE_PROPERTY(TScreen);
    DECLARE_COPY_DISABLE(TScreen);
    friend void __fastcall RegisterPainter(TScreen *scr, const TPainter *painter);

private:
    HSCREEN m_Handle;
    TCanvas m_Canvas;
    TCursor m_Cursor;
    TCursorList m_Cursors;
    TList m_Forms;
    TList m_Monitors;
    bool m_PopupActive;
    TPainter *m_Painter;
    void Init();
    TWinControl *get_ActiveControl();
    TCustomForm *get_ActiveForm();
    inline TCanvas *get_Canvas()
    {
        return &m_Canvas;
    };
    inline HCOLORMAP get_Colormap()
    {
        return  DefaultColormapOfScreen(m_Handle);
    };
    TPixelSize get_Depth();
    HGC get_GC();
    short get_Height();
    short get_HeightMM();
    inline short get_FormCount()
    {
        return m_Forms.Count;
    };
    inline TCustomForm *get_Forms(int index)
    {
        return (TCustomForm *)m_Forms[(int)index];
    };
    inline short get_CursorCount()
    {
        return m_Cursors.Count;
    };
    void set_Cursor(TCursor value);
    inline HCURSOR get_Cursors(TCursor index)
    {
        return m_Cursors[(int)index];
    };
    inline void set_Cursors(TCursor index, HCURSOR value)
    {
        m_Cursors.Items[(int)index] = value;
    };
    short get_MonitorCount();
    TMonitor *get_Monitors(int index)
    {
        return (TMonitor *)m_Monitors[index];
    };
    short get_Planes();
    HWND get_Root();
    short get_Xdpi();
    short get_Ydpi();
    short get_Width();
    short get_WidthMM();
    TRect get_WorkArea();
    void CreateCursors();

protected:

public:
    TFont Font;
    TPen Pen;
    TBrush Brush;
    TScreen();
    ~TScreen();
    void __fastcall AddForm(TCustomForm *form);
    void ClosePopup();
    inline void __fastcall PaintControl(TControl &ctrl)
    {
        m_Painter->PaintControl(ctrl);
    };
    void __fastcall RemoveForm(TCustomForm *form);

    property (r, TWinControl *, ActiveControl);
    property (r, TCustomForm *, ActiveForm);
    property (r, TCanvas *, Canvas);
    property (r, HCOLORMAP, Colormap);
    property (m, TCursor, Cursor);
    property (r, short, CursorCount);
    property_i (rw, HCURSOR, TCursor, Cursors);
    property (r, TPixelSize, Depth);
    property (r, short, FormCount);
    property_i (r, TCustomForm *, int, Forms);
    property (r, HGC, GC);
    property (mr, HSCREEN, Handle);
    property (r, short, Height);
    property (r, short, HeightMM);
    property (r, short, MonitorCount);
    property_i (r, TMonitor *, int, Monitors);
    property (r, short, Planes);
    property (mr, bool, PopupActive);
    property (r, HWND, Root);
    property (r, short, Width);
    property (r, short, WidthMM);
    property (r, TRect, WorkArea);
    property (r, short, Xdpi);
    property (r, short, Ydpi);
};

void __fastcall RegisterPainter(TScreen *scr, const TPainter *painter);
/*
  TScreen = class(TComponent)
  private
    FFonts: TStrings;
    FImes: TStrings;
    FDefaultIme: string;
    FDefaultKbLayout: HKL;
    FCursor: TCursor;

    FDefaultCursor: HCURSOR;

    FLastActiveControl: TWinControl;
    FLastActiveCustomForm: TCustomForm;
    FFocusedForm: TCustomForm;
    FSaveFocusedList: TList;
    FHintFont: TFont;
    FIconFont: TFont;
    FMenuFont: TFont;
    FAlignLevel: Word;
    FControlState: TControlState;
    FOnActiveControlChange: TNotifyEvent;
    FOnActiveFormChange: TNotifyEvent;
    procedure AlignForm(AForm: TCustomForm);
    procedure AlignForms(AForm: TCustomForm; var Rect: TRect);
    procedure AddDataModule(DataModule: TDataModule);
    procedure AddForm(AForm: TCustomForm);
    procedure DeleteCursor(Index: Integer);
    procedure DestroyCursors;
    function FindMonitor(Handle: HMONITOR): TMonitor;
    procedure IconFontChanged(Sender: TObject);
    function GetDataModule(Index: Integer): TDataModule;
    function GetDataModuleCount: Integer;
    function GetDefaultIME: String;
    function GetDesktopRect: TRect;
    function GetImes: TStrings;
    function GetFonts: TStrings;
    function GetForm(Index: Integer): TForm;
    procedure GetMetricSettings;
    procedure InsertCursor(Index: Integer; Handle: HCURSOR);
    procedure RemoveDataModule(DataModule: TDataModule);
    procedure RemoveForm(AForm: TCustomForm);
    procedure SetCursor(Value: TCursor);
    procedure SetHintFont(Value: TFont);
    procedure SetIconFont(Value: TFont);
    procedure SetMenuFont(Value: TFont);
  public
    procedure DisableAlign;
    procedure EnableAlign;
    function MonitorFromPoint(const Point: TPoint;
      MonitorDefault: TMonitorDefaultTo = mdNearest): TMonitor;
    function MonitorFromRect(const Rect: TRect;
      MonitorDefault: TMonitorDefaultTo = mdNearest): TMonitor;
    function MonitorFromWindow(const Handle: THandle;
      MonitorDefault: TMonitorDefaultTo = mdNearest): TMonitor;
    procedure Realign;
    procedure ResetFonts;

    property Cursor: TCursor read FCursor write SetCursor;

    property DesktopRect: TRect read GetDesktopRect;
    property HintFont: TFont read FHintFont write SetHintFont;
    property IconFont: TFont read FIconFont write SetIconFont;
    property MenuFont: TFont read FMenuFont write SetMenuFont;
    property Fonts: TStrings read GetFonts;
    property Forms[Index: Integer]: TForm read GetForm;
    property Imes: TStrings read GetImes;
    property DefaultIme: string read GetDefaultIme;
    property DefaultKbLayout: HKL read FDefaultKbLayout;
    property OnActiveControlChange: TNotifyEvent
      read FOnActiveControlChange write FOnActiveControlChange;
    property OnActiveFormChange: TNotifyEvent
      read FOnActiveFormChange write FOnActiveFormChange;
  end;
 */
//---------------------------------------------------------------------------
// TXExtension
//---------------------------------------------------------------------------
class TXExtension : public TObject
{
    DECLARE_PROPERTY(TXExtension);

private:
    bool m_Supported;
    TString m_Name;
    int m_BaseEvent;
    int m_BaseError;

protected:

public:
    __fastcall TXExtension(const char *ename);
    ~TXExtension();
    virtual void __fastcall GetVersion(int *major, int *minor) = 0;
    virtual void __fastcall Bind(TWinControl *wc) = 0;
    virtual bool __fastcall WndProc(XEvent &event) = 0;

    property (mr, int, BaseEvent);
    property (mr, int, BaseError);
    property (mr, TString, Name);
    property (mr, bool, Supported);

};
//---------------------------------------------------------------------------
// Application
//---------------------------------------------------------------------------
struct TXConnection
{
    char *host;
    short display;
    short screen;
    short protocol;
};

typedef delegate< void (XEvent &event, bool &handled) > TMessageEvent;
typedef delegate< void (TObject* sender, bool &done) > TIdleEvent;
typedef delegate< void (TObject* sender, Exception &e) > TExceptionEvent;

class TApplication : public TObject
{
    DECLARE_PROPERTY(TApplication);
    DECLARE_COPY_DISABLE(TApplication);

private:
    HDISPLAY m_Handle;
    TBitmap *m_Icon;
    TString m_title;
    TString m_Hint;
    THintWindow *m_hintwindow;
    TForm *m_MainForm;
    bool m_Terminated;
    bool m_Active;
    int get_fd();
    TString get_ExeName();
    inline short get_FormCount()
    {
        return Forms.Count;
    };
    inline char *get_Locale(const int cat)
    {
        return setlocale(cat,NULL);
    };
    inline void set_Locale(const int cat,char *locale)
    {
        setlocale(cat, locale);
    };
    inline struct lconv *get_LocaleConv()
    {
        return localeconv();
    };
    TString get_Title();
    void set_Title(TString text);
    void set_Hint(TString value);
    const char *get_Charset();
    void set_Icon(TBitmap *value);
    void DoDeactivate();
    void DoActivate();
    bool __fastcall WndProc(XEvent &event);

protected:

public:
    TXConnection *XConnection;
    TList Screens;
    TList Forms;
    TList Extensions;
    TMetaClass *HintWindowClass;
    TColor HintColor;
    short HintShowTime;
    short HintPauseTime;
    bool ShowHint;
    bool ShowMainForm;
    short IdleTimeout;

    TApplication();
    ~TApplication();
    void __fastcall ActivateHint(const TControl *ctrl);
    void BringToFront();
    void __fastcall CreateForm(TMetaClass* InstanceClass, void *Reference);
    void __fastcall DispatchEvent(XEvent &event);
    void __fastcall HandleException(TObject *Sender);
    void HandleMessage();
    void HideHint();
    void Minimize();
    void Restore();
    void Run();
    void Terminate();
    void ProcessMessages();
    void __fastcall SendMessage(HWND destwnd, TMessage &msg);
    void __fastcall ShowException(Exception *e);

    TNotifyEvent	OnActivate;
    TNotifyEvent 	OnDeactivate;
    TExceptionEvent OnException;
    TNotifyEvent 	OnHint;
    TIdleEvent 		OnIdle;
    TMessageEvent 	OnMessage;
    TNotifyEvent	OnMinimize;
    TNotifyEvent	OnRestore;
    TSignalEvent	OnSignal;

    property (mr, bool, Active);
    property (r, int, fd);
    property (r, TString, ExeName);
    property (r, short, FormCount);
    property (mr, HDISPLAY, Handle);
    property (m, TString, Hint);
    property (m, TBitmap *, Icon);
    property (rw, TString, Title);
    property_i (rw, char *, int, Locale);
    property (r, struct lconv *, LocaleConv);
    property (r, const char *, Charset);
    property (mr, bool, Terminated);
    property (mr, TForm *, MainForm);
};

extern TApplication Application;
extern TScreen Screen;
extern TKeyboard Keyboard;
extern TMouse Mouse;
extern TCaret Caret;


/*
  TApplication = class(TComponent)
  private
    FHelpFile: string;
    FHintActive: Boolean;
    FUpdateFormatSettings: Boolean;
    FUpdateMetricSettings: Boolean;
    FTopMostList: TList;
    FTopMostLevel: Integer;
    FHandleCreated: Boolean;
    FWindowList: Pointer;
    FAutoDragDocking: Boolean;
    function CheckIniChange(var Message: TMessage): Boolean;
    procedure DoNormalizeTopMosts(IncludeMain: Boolean);
    function GetCurrentHelpFile: string;
    procedure IconChanged(Sender: TObject);
    function InvokeHelp(Command: Word; Data: Longint): Boolean;
    procedure SetBiDiMode(Value: TBiDiMode);
    procedure SetHandle(Value: HWnd);
    procedure SettingChange(var Message: TWMSettingChange);
    procedure WndProc(var Message: TMessage);
  protected
    procedure Idle(const Msg: TMsg);
    function IsDlgMsg(var Msg: TMsg): Boolean;
    function IsHintMsg(var Msg: TMsg): Boolean;
    function IsKeyMsg(var Msg: TMsg): Boolean;
    function IsMDIMsg(var Msg: TMsg): Boolean;
    function IsShortCut(var Message: TWMKey): Boolean;
  public
    procedure CreateHandle;
    procedure HandleException(Sender: TObject);
    function HelpCommand(Command: Integer; Data: Longint): Boolean;
    function HelpContext(Context: THelpContext): Boolean;
    function HelpJump(const JumpID: string): Boolean;
    function HelpKeyword(const Keyword: String): Boolean;
    function IsRightToLeft: Boolean;
    function MessageBox(const Text, Caption: PChar; Flags: Longint = MB_OK): Integer;
    procedure NormalizeAllTopMosts;
    procedure NormalizeTopMosts;
    procedure RestoreTopMosts;
    property AutoDragDocking: Boolean read FAutoDragDocking write FAutoDragDocking default True;
    property CurrentHelpFile: string read GetCurrentHelpFile;
    property HelpFile: string read FHelpFile write FHelpFile;

    property HintShortCuts: Boolean read FHintShortCuts write FHintShortCuts;
    property HintShortPause: Integer read FHintShortPause write FHintShortPause;
    property UpdateFormatSettings: Boolean read FUpdateFormatSettings
      write FUpdateFormatSettings;
    property UpdateMetricSettings: Boolean read FUpdateMetricSettings
      write FUpdateMetricSettings;

    property OnException: TExceptionEvent read FOnException write FOnException;
    property OnHelp: THelpEvent read FOnHelp write FOnHelp;
    property OnShowHint: TShowHintEvent read FOnShowHint write FOnShowHint;
    property OnShortCut: TShortCutEvent read FOnShortCut write FOnShortCut;
    property OnSettingChange: TSettingChangeEvent read FOnSettingChange write FOnSettingChange;
  end;
*/
//---------------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
