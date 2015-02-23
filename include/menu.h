//---------------------------------------------------------------------------
#ifndef menuH
#define menuH

#include "forms.h"

typedef word_t TShortCut;
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
TShortCut ShortCut(word_t key, TShiftState shift);
void ShortCutToKey(TShortCut ShortCut, word_t &key, TShiftState &shift);
TString ShortCutToText(TShortCut shortcut);
TShortCut TextToShortCut(const TString &text);
//---------------------------------------------------------------------------
//  CustomMenuItem
//---------------------------------------------------------------------------
class TMenu;
class TCustomPopupMenu;

typedef delegate< void (TObject* Sender, uint_t &w, uint_t &h) > TMenuMeasureItemEvent;

class TCommonMenuItem : public TControl
{
    DECLARE_PROPERTY(TCommonMenuItem);
    friend class TMenu;

private:
    short m_ImageIndex;
    TShortCut m_ShortCut;
    TCustomPopupMenu *m_Submenu;
    TCommonMenuItem *m_ParentItem;
    TMenu *m_Menu;
    TList m_Items;
    inline TCommonMenuItem *get_Items(const int index)
    {
        return (TCommonMenuItem *)m_Items[index];
    };
    inline short get_Count()
    {
        return m_Items.Count;
    };
    void set_ImageIndex(short value);
    void set_Submenu(TCustomPopupMenu *value);
    void set_ShortCut(TShortCut value);
    short get_MenuIndex();
    void set_MenuIndex(short value);
    void ShowSubMenu();
    void HideSubMenu();

protected:
    virtual void set_Caption(TString value);
    void Init();
    void Clear();
    virtual void Click();
    virtual void __fastcall MouseEnter(TShiftState state, int x, int y);
    virtual void __fastcall MouseLeave(TShiftState state, int x, int y);
    virtual void __fastcall MouseDown(TMouseButton button, uint_t state, int x, int y);

public:
    TCommonMenuItem();

    void __fastcall Add(const TCommonMenuItem &item);
    void __fastcall Delete(int index);
    int  __fastcall IndexOf(const TCommonMenuItem &item);
    void __fastcall Insert(int index, TCommonMenuItem &item);
    inline bool IsLine()
    {
        return Caption == "-";
    };
    void __fastcall Remove(const TCommonMenuItem &item);

    TMenu *GetParentMenu();
    virtual void __fastcall MeasureItem(uint_t &width, uint_t &height);

    TMenuMeasureItemEvent OnMeasureItem;

    bool AutoCheck;

    property_i (r, TCommonMenuItem *, int, Items);
    property (r, short, Count);
    property (m, short, ImageIndex);
    property (rw, short, MenuIndex);
    property (mr, TCommonMenuItem *, ParentItem);
    property (m, TShortCut, ShortCut);
    property (m, TCustomPopupMenu *, Submenu);
};
/*
class TCommonMenuItem : public TObject
{
	DECLARE_PROPERTY(TCommonMenuItem);

private:

    FAutoHotkeys: TMenuItemAutoFlag;
    FAutoLineReduction: TMenuItemAutoFlag;

    FBreak: TMenuBreak;

    FMerged: TMenuItem;
    FMergedWith: TMenuItem;

    FSubMenuImages: TCustomImageList;
    FOnChange: TMenuChangeEvent;
    FOnDrawItem: TMenuDrawItemEvent;
    FOnAdvancedDrawItem: TAdvancedMenuDrawItemEvent;
    procedure AppendTo(Menu: HMENU; ARightToLeft: Boolean);

    procedure MergeWith(Menu: TMenuItem);
    procedure RebuildHandle;
    procedure PopulateMenu;
    procedure SubItemChanged(Sender: TObject; Source: TMenuItem; Rebuild: Boolean);

    procedure SetSubMenuImages(Value: TCustomImageList);
    function InternalRethinkHotkeys(ForceRethink: Boolean): Boolean;
    procedure SetAutoHotkeys(const Value: TMenuItemAutoFlag);
    function InternalRethinkLines(ForceRethink: Boolean): Boolean;
    procedure SetAutoLineReduction(const Value: TMenuItemAutoFlag);

protected:
    procedure AdvancedDrawItem(ACanvas: TCanvas; ARect: TRect;
      State: TOwnerDrawState; TopLevel: Boolean); virtual;
    procedure DoDrawText(ACanvas: TCanvas; const ACaption: string;
      var Rect: TRect; Selected: Boolean; Flags: Longint);
    procedure DrawItem(ACanvas: TCanvas; ARect: TRect; Selected: Boolean); virtual;

    function GetAutoHotkeys: Boolean;
    function GetAutoLineReduction: Boolean;
    function InsertNewLine(ABefore: Boolean; AItem: TMenuItem): Integer;

    virtual void MenuChanged(bool rebuild);

    procedure Notification(AComponent: TComponent;
      Operation: TOperation); override;
    procedure SetBreak(Value: TMenuBreak);

    procedure SetChildOrder(Child: TComponent; Order: Integer); override;

    procedure UpdateItems;

public:
	TCommonMenuItem();
	~TCommonMenuItem();

    function Find(ACaption: string): TMenuItem;

    function GetImageList: TCustomImageList;
    function GetParentComponent: TComponent; override;

    function NewTopLine: Integer;
    function NewBottomLine: Integer;
    function InsertNewLineBefore(AItem: TMenuItem): Integer;
    function InsertNewLineAfter(AItem: TMenuItem): Integer;

    procedure Add(const AItems: array of TMenuItem); overload;
    function RethinkHotkeys: Boolean;
    function RethinkLines: Boolean;

  published
    property AutoHotkeys: TMenuItemAutoFlag read FAutoHotkeys write SetAutoHotkeys default maParent;
    property AutoLineReduction: TMenuItemAutoFlag read FAutoLineReduction write SetAutoLineReduction default maParent;

    property Break: TMenuBreak read FBreak write SetBreak default mbNone;

    property SubMenuImages: TCustomImageList read FSubMenuImages write SetSubMenuImages;

    property OnDrawItem: TMenuDrawItemEvent read FOnDrawItem write FOnDrawItem;
    property OnAdvancedDrawItem: TAdvancedMenuDrawItemEvent read FOnAdvancedDrawItem write FOnAdvancedDrawItem;

};

 */
//---------------------------------------------------------------------------
//  Menu
//---------------------------------------------------------------------------
class TMenu : public TCustomForm
{
    DECLARE_PROPERTY(TMenu);

private:
    TImageList *m_Images;
    TList m_MenuItems;
    void set_Images(TImageList *value);
    inline TCommonMenuItem *get_Items(const int index)
    {
        return (TCommonMenuItem *)m_MenuItems[index];
    };
    inline short get_Count()
    {
        return m_MenuItems.Count;
    };

protected:
    bool m_Toplevel;
    virtual void Init();
//	virtual void DoShow();
    virtual void __fastcall KeyDown(uint_t key, uint_t state);
    virtual void __fastcall Measure(uint_t &width, uint_t &height) = 0;
    virtual void Paint();

public:
    TMenu();
    __fastcall TMenu(TWinControl *AParent);
    virtual ~TMenu();

//	void __fastcall Add(TCommonMenuItem *item);
    void __fastcall Add(TCommonMenuItem &item);
    void __fastcall Remove(TCommonMenuItem &item);

    TMetaClass *SubMenuClass;
    TColor SelectColor;
    TColor SelectTextColor;

    property_i (r, TCommonMenuItem *, int, Items);
    property (r, short, Count);
    property (m, TImageList *, Images);
    property (mr, bool, Toplevel);
};
/*
  TRyMenu = class(TObject)
  private
    FFont: TFont;
    FGutterColor: TColor;
    FMenuColor: TColor;
    FSelectedColor: TColor;
    FSelLightColor: TColor;
    FMinWidth: Integer;
    FMinHeight: Integer;
    procedure SetFont(const Value: TFont);
    procedure SetSelectedColor(const Value: TColor);
    procedure SetMinHeight(const Value: Integer);
  protected
  public
    constructor Create;
    destructor Destroy; override;
    procedure Add(Menu: TMenu; Item: TMenuItem);
    procedure MeasureItem(Sender: TObject; ACanvas: TCanvas;
              var Width, Height: Integer);
    procedure AdvancedDrawItem(Sender: TObject; ACanvas: TCanvas;
              ARect: TRect; State: TOwnerDrawState);
  public
    property  MenuColor: TColor read FMenuColor write FMenuColor;
    property  GutterColor: TColor read FGutterColor write FGutterColor;
    property  SelectedColor: TColor read FSelectedColor write SetSelectedColor;
    property  Font: TFont read FFont write SetFont; {можете поменять фонт у меню}
    property  MinHeight: Integer read FMinHeight write SetMinHeight;
    property  MinWidth: Integer read FMinWidth write FMinWidth;
  end;

enum Windows__1 { odSelected, odGrayed, odDisabled, odChecked, odFocused, odDefault,
 odHotLight, odInactive, odNoAccel, odNoFocusRect, odReserved1, odReserved2, odComboBoxEdit };

typedef Set<Windows__1, odSelected, odComboBoxEdit>  TOwnerDrawState;
*/
/*
TMenu = class(TComponent)
  private
    FBiDiMode: TBiDiMode;
    FItems: TMenuItem;
    FWindowHandle: HWND;
    FMenuImage: string;
    FOwnerDraw: Boolean;
    FParentBiDiMode: Boolean;
    FImageChangeLink: TChangeLink;
    FImages: TCustomImageList;
    FOnChange: TMenuChangeEvent;
    procedure SetBiDiMode(Value: TBiDiMode);
    procedure SetOwnerDraw(Value: Boolean);
    procedure SetImages(Value: TCustomImageList);
    procedure SetParentBiDiMode(Value: Boolean);
    procedure SetWindowHandle(Value: HWND);
    procedure ImageListChange(Sender: TObject);
    function IsBiDiModeStored: Boolean;
    function UpdateImage: Boolean;
    function GetAutoHotkeys: TMenuAutoFlag;
    procedure SetAutoHotkeys(const Value: TMenuAutoFlag);
    function GetAutoLineReduction: TMenuAutoFlag;
    procedure SetAutoLineReduction(const Value: TMenuAutoFlag);
  protected
    procedure AdjustBiDiBehavior;
    procedure DoChange(Source: TMenuItem; Rebuild: Boolean); virtual;
    procedure DoBiDiModeChanged;
    function DoGetMenuString(Menu: HMENU; ItemID: UINT; Str: PChar;
      MaxCount: Integer; Flag: UINT): Integer;
    procedure GetChildren(Proc: TGetChildProc; Root: TComponent); override;
    function GetHandle: HMENU; virtual;
    function IsOwnerDraw: Boolean;
    procedure Loaded; override;
    procedure MenuChanged(Sender: TObject; Source: TMenuItem; Rebuild: Boolean); virtual;
    procedure Notification(AComponent: TComponent; Operation: TOperation); override;
    procedure SetChildOrder(Child: TComponent; Order: Integer); override;
    procedure UpdateItems;
    property OnChange: TMenuChangeEvent read FOnChange write FOnChange;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    function DispatchCommand(ACommand: Word): Boolean;
    function DispatchPopup(AHandle: HMENU): Boolean;
    function FindItem(Value: Integer; Kind: TFindItemKind): TMenuItem;
    function GetHelpContext(Value: Integer; ByCommand: Boolean): THelpContext;
    property Images: TCustomImageList read FImages write SetImages;
    function IsRightToLeft: Boolean;
    function IsShortCut(var Message: TWMKey): Boolean; dynamic;
    procedure ParentBiDiModeChanged; overload;
    procedure ParentBiDiModeChanged(AControl: TObject); overload;
    procedure ProcessMenuChar(var Message: TWMMenuChar);
    property AutoHotkeys: TMenuAutoFlag read GetAutoHotkeys write SetAutoHotkeys default maAutomatic;
    property AutoLineReduction: TMenuAutoFlag read GetAutoLineReduction write SetAutoLineReduction default maAutomatic;
    property BiDiMode: TBiDiMode read FBiDiMode write SetBiDiMode stored IsBiDiModeStored;
    property Handle: HMENU read GetHandle;
    property OwnerDraw: Boolean read FOwnerDraw write SetOwnerDraw default False;
    property ParentBiDiMode: Boolean read FParentBiDiMode write SetParentBiDiMode default True;
    property WindowHandle: HWND read FWindowHandle write SetWindowHandle;
  published
    property Items: TMenuItem read FItems;
  end;

 */
//---------------------------------------------------------------------------
//  TCustomPopupMenu
//---------------------------------------------------------------------------
class TCustomPopupMenu : public TMenu
{
    DECLARE_PROPERTY(TCustomPopupMenu);

private:
    TPoint m_PopupPoint;

protected:
    virtual void Init();
    virtual void DoShow();
    virtual void Measure(uint_t &width, uint_t &height);

    virtual void MouseEnter(TShiftState state, int x, int y);
    virtual void MouseLeave(TShiftState state, int x, int y);
    virtual void MouseMove(TShiftState state, int x, int y);
    virtual void DoEnter();
    virtual void DoExit();

public:
    bool AutoPopup;

    TCustomPopupMenu();
    virtual void __fastcall Popup(int x, int y);

    TNotifyEvent OnPopup;

    property (mr, TPoint, PopupPoint);
};
/*
  TPopupAlignment = (paLeft, paRight, paCenter);
  TTrackButton = (tbRightButton, tbLeftButton);
  TMenuAnimations = (maLeftToRight, maRightToLeft, maTopToBottom, maBottomToTop, maNone);
  TMenuAnimation = set of TMenuAnimations;

  TPopupMenu = class(TMenu)
  private
    FPopupPoint: TPoint;
    FAlignment: TPopupAlignment;
    FAutoPopup: Boolean;
    FPopupComponent: TComponent;
    FTrackButton: TTrackButton;
    FMenuAnimation: TMenuAnimation;
    FOnPopup: TNotifyEvent;
    function GetHelpContext: THelpContext;
    procedure SetHelpContext(Value: THelpContext);
    procedure SetBiDiModeFromPopupControl;
  protected
    function UseRightToLeftAlignment: Boolean;
    procedure DoPopup(Sender: TObject); virtual;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure Popup(X, Y: Integer); virtual;
    property PopupComponent: TComponent read FPopupComponent write FPopupComponent;
    property PopupPoint: TPoint read FPopupPoint;
  published
    property Alignment: TPopupAlignment read FAlignment write FAlignment default paLeft;
    property AutoHotkeys;
    property AutoLineReduction;
    property AutoPopup: Boolean read FAutoPopup write FAutoPopup default True;
    property HelpContext: THelpContext read GetHelpContext write SetHelpContext default 0;
    property Images;
    property MenuAnimation: TMenuAnimation read FMenuAnimation write FMenuAnimation default [];
    property OwnerDraw;
    property TrackButton: TTrackButton read FTrackButton write FTrackButton default tbRightButton;
    property OnChange;
    property OnPopup: TNotifyEvent read FOnPopup write FOnPopup;
  end;

 */
//---------------------------------------------------------------------------
//  TCustomMainMenu
//---------------------------------------------------------------------------
class TCustomMainMenu : public TMenu
{
private:
    void set_Parent(TWinControl *AParent);

protected:
    virtual void Init();
    virtual void DoShow();
    virtual void MouseDown(TMouseButton button, TShiftState state, int x, int y);
    void Measure(uint_t &width, uint_t &height);

public:
    __fastcall TCustomMainMenu();
    __fastcall TCustomMainMenu(TWinControl *AParent);
};

/*
  TMainMenu = class(TMenu)
  private
    FAutoMerge: Boolean;
    procedure ItemChanged;
    procedure SetAutoMerge(Value: Boolean);
  protected
    procedure MenuChanged(Sender: TObject; Source: TMenuItem; Rebuild: Boolean); override;
  public
    procedure Merge(Menu: TMainMenu);
    procedure Unmerge(Menu: TMainMenu);
  published
    property AutoHotkeys;
    property AutoLineReduction;
    property AutoMerge: Boolean read FAutoMerge write SetAutoMerge default False;
    property OwnerDraw;
    property OnChange;
  end;

 */
//---------------------------------------------------------------------------
}  // namespace
//---------------------------------------------------------------------------
#endif
