//---------------------------------------------------------------------------
#ifndef formsH
#define formsH

#include "cursor.h"
#include "sysobj.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
//	TCustomForm
//---------------------------------------------------------------------------
enum TFormType { ftNormal, ftDesktop, ftDock, ftToolbar, ftMenu, ftUtility,
                 ftSplash, ftDialog, ftDropdownMenu, ftPopupMenu, ftTooltip,
                 ftNotification, ftCombo, ftDragnDrop
               };

enum TWindowStateKind
{
    wsModal, wsSticky, wsMaximizedV, wsMaximizedH, wsShaded, wsSkipTaskbar,
    wsSkipPager, wsHidden, wsFullScreen, wsAbove, wsBelow, wsAttention
};
typedef TSet<TWindowStateKind>  TWindowState;

class TCustomForm : public TWinControl
{
    DECLARE_PROPERTY( TCustomForm );

private:
    bool m_terminated;
    TWinControl *get_ActiveControl();
    void set_ActiveControl(TWinControl *control);
    bool get_OverrideRedirect();
    void set_OverrideRedirect(bool value);
    HWND get_TransientFor();
    void set_TransientFor(HWND value);
    TFormType get_FormType();
    void set_FormType(TFormType value);
    int get_Desktop();
    void set_Desktop(int val);
    TWindowState get_WindowState();
    void set_WindowState(TWindowState value);

protected:
    void Init();
    virtual void Activate();
    virtual void Deactivate();
    virtual void Paint();
    virtual void DoShow();
    virtual void DoHide();

    property (rw, bool, OverrideRedirect);
    property (rw, HWND, TransientFor);
    property (rw, TFormType, FormType);
    property (rw, TWindowState, WindowState);

public:
    bool Scaled;

    TNotifyEvent	OnActivate;
    TNotifyEvent 	OnDeactivate;
    TNotifyEvent 	OnDestroy;
    TNotifyEvent 	OnClose;
    TCloseQueryEvent OnCloseQuery;
    TNotifyEvent 	OnShow;
    TNotifyEvent 	OnHide;

    TCustomForm();
    __fastcall TCustomForm(TWinControl *AParent);
    __fastcall TCustomForm(HWND win);
    ~TCustomForm();
    void Close();
    virtual int ShowModal();
    bool __fastcall WndProc(XEvent &event);

    property (rw, TWinControl *, ActiveControl);
    property (rw, int, Desktop);
};
//---------------------------------------------------------------------------
//  TPopupWindow
//---------------------------------------------------------------------------
class TPopupWindow : public TCustomForm
{
private:
    void Init();
    __fastcall void Assign(const TPopupWindow &obj);

protected:

public:
    TPopupWindow();
    __fastcall TPopupWindow(const TPopupWindow &a);
    virtual ~TPopupWindow();
    __fastcall const TPopupWindow & operator=(const TPopupWindow &a);

    virtual void __fastcall Popup(int x, int y);

    TNotifyEvent OnPopup;
};
//---------------------------------------------------------------------------
//	TCustomFormFrame
//---------------------------------------------------------------------------
class TCustomFormFrame : public TCustomForm
{
    DECLARE_PROPERTY( TCustomFormFrame );

private:

protected:

public:
    TCustomFormFrame();
    __fastcall TCustomFormFrame(TWinControl *AParent);
    __fastcall TCustomFormFrame(HWND win);
    ~TCustomFormFrame();
};
//---------------------------------------------------------------------------
//	TForm
//---------------------------------------------------------------------------
enum TAllowedActionsKind
{
    aaMove, aaResize, aaMinimize, aaShade, aaStick, aaMaximizeH,
    aaMaximizeV, aaFullscreen, aaChangeDesktop, aaClose, aaAbove, aaBelow
};
typedef TSet<TAllowedActionsKind>  TAllowedActions;

enum TTypeXHints { xhWMName, xhWMIconName, xhWMCommand, xhWMProperties,
                   xhWMNormalHints, xhWMHints, xhWMClassHint, xhWMClientMachine,
                   xhWMProtocols
                 };

struct TXHints
{
    TString WMName;
    TString WMIconeName;
    char **argv;
    int argc;
};

class TCustomMainMenu;

class TForm : public TCustomForm
{
    DECLARE_DYNAMIC_CLASS( TForm );
    DECLARE_PROPERTY( TForm );

private:
    HWND m_WindowHandle;
    HWND m_FrameHandle;
    TCustomForm m_wnd;
    short m_ExtentLeft;
    short m_ExtentTop;
    short m_ExtentRight;
    short m_ExtentBottom;

    TBitmap *m_Icon;
    TCustomMainMenu *m_Menu;
    TCustomFormFrame *m_FormFrame;
    virtual void set_Caption(TString text);
    TAllowedActions get_AllowedActions();
    void set_AllowedActions(TAllowedActions value);
    void set_Icon(TBitmap *value);
    void set_Menu(TCustomMainMenu *value);
//  	bool __fastcall DoConfigure(XEvent &event);
    virtual TRect get_ClientRect();

protected:

//    virtual TRect get_BoundsRect();
    virtual void  set_BoundsRect(TRect rect);

    void Init();
//	virtual void DoShow();
//  virtual void DoHide();
//	virtual void MouseUp(uint_t button, uint_t state, int x, int y);

    property (rw, TAllowedActions, AllowedActions);

public:
    TForm();
    __fastcall TForm(TWinControl *AParent);
    __fastcall TForm(HWND win);
    ~TForm();
//  	virtual void SetBounds(short left, short top, short width, short height);
    bool __fastcall WndProc(XEvent &event);

    property (m, TBitmap *, Icon);
    property (m, TCustomMainMenu *, Menu);
};
/*
 * enum TFormBorderStyle { bsNone, bsSingle, bsSizeable, bsDialog, bsToolWindow, bsSizeToolWin };
 *
 class PASCALIMPLEMENTATION TCustomForm : public TScrollingWinControl
{
	typedef TScrollingWinControl inherited;

private:
	Controls::TWinControl* FActiveControl;
	Controls::TWinControl* FFocusedControl;
	TBorderIcons FBorderIcons;
	TFormBorderStyle FBorderStyle;
	bool FSizeChanging;
	TWindowState FWindowState;
	bool FKeyPreview;
	bool FActive;
	TFormStyle FFormStyle;
	TPosition FPosition;
	TDefaultMonitor FDefaultMonitor;
	TTileMode FTileMode;
	bool FDropTarget;
	TPrintScale FPrintScale;
	AnsiString FHelpFile;
	Graphics::TIcon* FIcon;
	bool FInCMParentBiDiModeChanged;
	Menus::TMainMenu* FMenu;
	Controls::TModalResult FModalResult;
	HWND FClientHandle;
	Menus::TMenuItem* FWindowMenu;
	int FPixelsPerInch;
	Menus::TMenuItem* FObjectMenuItem;
	int FClientWidth;
	int FClientHeight;
	int FTextHeight;
	void *FDefClientProc;
	void *FClientInstance;
	TFormBorderStyle FSavedBorderStyle;
	Classes::TNotifyEvent FOnActivate;
	TCloseQueryEvent FOnCloseQuery;
	Classes::TNotifyEvent FOnDeactivate;
	THelpEvent FOnHelp;
	TShortCutEvent FOnShortCut;
	Classes::TNotifyEvent FOnCreate;
	Classes::TNotifyEvent FOnDestroy;
	bool FAlphaBlend;
	Byte FAlphaBlendValue;
	bool FTransparentColor;
	Graphics::TColor FTransparentColorValue;
	void __fastcall RefreshMDIMenu(void);
	void __fastcall ClientWndProc(Messages::TMessage &Message);
	void __fastcall CloseModal(void);
	TForm* __fastcall GetActiveMDIChild(void);
	Graphics::TCanvas* __fastcall GetCanvas(void);
	HICON __fastcall GetIconHandle(void);
	int __fastcall GetMDIChildCount(void);
	TForm* __fastcall GetMDIChildren(int I);
	TMonitor* __fastcall GetMonitor(void);
	int __fastcall GetPixelsPerInch(void);
	bool __fastcall GetScaled(void);
	int __fastcall GetTextHeight(void);
	void __fastcall IconChanged(System::TObject* Sender);
	bool __fastcall IsAutoScrollStored(void);
	bool __fastcall IsClientSizeStored(void);
	void __fastcall MergeMenu(bool MergeState);
	void __fastcall ReadIgnoreFontProperty(Classes::TReader* Reader);
	void __fastcall ReadTextHeight(Classes::TReader* Reader);
	void __fastcall SetActive(bool Value);
	void __fastcall SetActiveControl(Controls::TWinControl* Control);
	void __fastcall SetBorderIcons(TBorderIcons Value);
	void __fastcall SetBorderStyle(TFormBorderStyle Value);
	HIDESBASE void __fastcall SetClientHeight(int Value);
	HIDESBASE void __fastcall SetClientWidth(int Value);
	void __fastcall SetFormStyle(TFormStyle Value);
	void __fastcall SetIcon(Graphics::TIcon* Value);
	void __fastcall SetMenu(Menus::TMainMenu* Value);
	void __fastcall SetPixelsPerInch(int Value);
	void __fastcall SetPosition(TPosition Value);
	void __fastcall SetScaled(bool Value);
	HIDESBASE void __fastcall SetVisible(bool Value);
	void __fastcall SetWindowFocus(void);
	void __fastcall SetWindowMenu(Menus::TMenuItem* Value);
	void __fastcall SetObjectMenuItem(Menus::TMenuItem* Value);
	void __fastcall SetWindowState(TWindowState Value);
	void __fastcall SetWindowToMonitor(void);
	void __fastcall WritePixelsPerInch(Classes::TWriter* Writer);
	void __fastcall WriteTextHeight(Classes::TWriter* Writer);
	Graphics::TColor __fastcall NormalColor(void);
	MESSAGE void __fastcall CMIsShortCut(Messages::TWMKey &Message);
	void __fastcall SetLayeredAttribs(void);
	void __fastcall SetAlphaBlend(const bool Value);
	void __fastcall SetAlphaBlendValue(const Byte Value);
	void __fastcall SetTransparentColor(const bool Value);
	void __fastcall SetTransparentColorValue(const Graphics::TColor Value);
	void __fastcall InitAlphaBlending(Controls::TCreateParams &Params);

protected:
	DYNAMIC void __fastcall Activate(void);
	virtual void __fastcall AlignControls(Controls::TControl* AControl, Types::TRect &Rect);
	DYNAMIC void __fastcall BeginAutoDrag(void);
	DYNAMIC void __fastcall ChangeScale(int M, int D);
	virtual void __fastcall CreateParams(Controls::TCreateParams &Params);
	virtual void __fastcall CreateWindowHandle(const Controls::TCreateParams &Params);
	virtual void __fastcall CreateWnd(void);
	DYNAMIC void __fastcall Deactivate(void);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	virtual void __fastcall DestroyWindowHandle(void);
	DYNAMIC void __fastcall DoClose(TCloseAction &Action);
	virtual void __fastcall DoCreate(void);
	virtual void __fastcall DoDestroy(void);
	DYNAMIC void __fastcall GetChildren(Classes::TGetChildProc Proc, Classes::TComponent* Root);
	virtual bool __fastcall GetFloating(void);
	DYNAMIC bool __fastcall HandleCreateException(void);
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	DYNAMIC void __fastcall Paint(void);
	virtual void __fastcall ReadState(Classes::TReader* Reader);
	DYNAMIC void __fastcall RequestAlign(void);
	DYNAMIC void __fastcall SetChildOrder(Classes::TComponent* Child, int Order);
	DYNAMIC void __fastcall DoDock(Controls::TWinControl* NewDockSite, Types::TRect &ARect);
	void __fastcall UpdateWindowState(void);
	DYNAMIC void __fastcall VisibleChanging(void);
	virtual void __fastcall WndProc(Messages::TMessage &Message);
	virtual void __fastcall Resizing(TWindowState State);
	__property TForm* ActiveMDIChild = {read=GetActiveMDIChild};
	__property bool AlphaBlend = {read=FAlphaBlend, write=SetAlphaBlend, nodefault};
	__property Byte AlphaBlendValue = {read=FAlphaBlendValue, write=SetAlphaBlendValue, nodefault};
	__property TBorderIcons BorderIcons = {read=FBorderIcons, write=SetBorderIcons, stored=IsForm, default=7};
	__property AutoScroll  = {stored=IsAutoScrollStored, default=1};
	__property HWND ClientHandle = {read=FClientHandle, nodefault};
	__property bool TransparentColor = {read=FTransparentColor, write=SetTransparentColor, nodefault};
	__property Graphics::TColor TransparentColorValue = {read=FTransparentColorValue, write=SetTransparentColorValue, nodefault};
	__property TDefaultMonitor DefaultMonitor = {read=FDefaultMonitor, write=FDefaultMonitor, stored=IsForm, default=3};
	__property HorzScrollBar  = {stored=IsForm};
	__property int MDIChildCount = {read=GetMDIChildCount, nodefault};
	__property TForm* MDIChildren[int I] = {read=GetMDIChildren};
	__property Menus::TMenuItem* ObjectMenuItem = {read=FObjectMenuItem, write=SetObjectMenuItem, stored=IsForm};
	__property int PixelsPerInch = {read=GetPixelsPerInch, write=SetPixelsPerInch, stored=false, nodefault};
	__property ParentFont  = {default=0};
	__property TPosition Position = {read=FPosition, write=SetPosition, stored=IsForm, default=0};
	__property TPrintScale PrintScale = {read=FPrintScale, write=FPrintScale, stored=IsForm, default=1};
	__property bool Scaled = {read=GetScaled, write=SetScaled, stored=IsForm, default=1};
	__property TTileMode TileMode = {read=FTileMode, write=FTileMode, default=0};
	__property VertScrollBar  = {stored=IsForm};
	__property Menus::TMenuItem* WindowMenu = {read=FWindowMenu, write=SetWindowMenu, stored=IsForm};
	__property Classes::TNotifyEvent OnActivate = {read=FOnActivate, write=FOnActivate, stored=IsForm};
	__property OnCanResize  = {stored=IsForm};
	__property Classes::TNotifyEvent OnDeactivate = {read=FOnDeactivate, write=FOnDeactivate, stored=IsForm};
	__property THelpEvent OnHelp = {read=FOnHelp, write=FOnHelp};
	__property TShortCutEvent OnShortCut = {read=FOnShortCut, write=FOnShortCut};

public:
	__fastcall virtual TCustomForm(Classes::TComponent* AOwner);
	__fastcall virtual ~TCustomForm(void);
	virtual void __fastcall AfterConstruction(void);
	virtual void __fastcall BeforeDestruction(void);
	virtual bool __fastcall CloseQuery(void);
	virtual void __fastcall DefaultHandler(void *Message);
	void __fastcall DefocusControl(Controls::TWinControl* Control, bool Removing);
	DYNAMIC void __fastcall Dock(Controls::TWinControl* NewDockSite, const Types::TRect &ARect);
	void __fastcall FocusControl(Controls::TWinControl* Control);
	Graphics::TBitmap* __fastcall GetFormImage(void);
	DYNAMIC bool __fastcall IsShortCut(Messages::TWMKey &Message);
	void __fastcall MakeFullyVisible(TMonitor* AMonitor = (TMonitor*)(0x0));
	void __fastcall Print(void);
	void __fastcall Release(void);
	HIDESBASE void __fastcall SendCancelMode(Controls::TControl* Sender);
	virtual int __fastcall ShowModal(void);
	virtual bool __fastcall WantChildKey(Controls::TControl* Child, Messages::TMessage &Message);
	__property TFormBorderStyle BorderStyle = {read=FBorderStyle, write=SetBorderStyle, stored=IsForm, default=2};
	__property bool DropTarget = {read=FDropTarget, write=FDropTarget, nodefault};
	__property AnsiString HelpFile = {read=FHelpFile, write=FHelpFile};
	__property bool KeyPreview = {read=FKeyPreview, write=FKeyPreview, stored=IsForm, default=0};
	__property Controls::TModalResult ModalResult = {read=FModalResult, write=FModalResult, nodefault};
	__property TMonitor* Monitor = {read=GetMonitor};
public:
	#pragma option push -w-inl
	inline __fastcall TCustomForm(HWND ParentWindow) : TScrollingWinControl(ParentWindow) { }
	#pragma option pop

};

*/

class THintWindow : public TCustomForm
{
    DECLARE_DYNAMIC_CLASS( THintWindow );

private:
    TTimer m_timer;
    const TControl *m_Control;
    void DoHint(TObject *Sender);
    void Init();

public:
    short PauseTime;
    short ShowTime;
    short MaxWidth;

    THintWindow();

    void __fastcall Activate(const TControl *ctrl);
    virtual void Paint();
    virtual __fastcall TRect CalcHintRect();
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
