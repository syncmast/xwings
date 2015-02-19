#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include "sysobj.h"
#include <comctrls.h>
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
//  TScrollbar
//---------------------------------------------------------------------------
enum TScrollbarStyle { ssRegular, ssFlat, ssHotTrack };

class TScrollbar : public TCommonScrollbar
{
	DECLARE_PROPERTY(TScrollbar);

private:
    TScrollbarStyle m_Style;
    void set_Style(TScrollbarStyle value);

    void Init();
    void __fastcall Assign(const TScrollbar &obj);

protected:
    void Paint();

public:
   	TScrollbar();
    __fastcall TScrollbar(const TScrollbar &a);
   	virtual ~TScrollbar();
   	__fastcall const TScrollbar & operator=(const TScrollbar &a);

    property (m, TScrollbarStyle, Style);
};
/*
  TControlScrollBar = class(TPersistent)
  private
    FControl: TScrollingWinControl;

    constructor Create(AControl: TScrollingWinControl; AKind: TScrollBarKind);
    procedure CalcAutoRange;
    function ControlSize(ControlSB, AssumeSB: Boolean): Integer;
    procedure DoSetRange(Value: Integer);
    function GetScrollPos: Integer;
    function NeedsScrollBarVisible: Boolean;
    function IsIncrementStored: Boolean;
    procedure ScrollMessage(var Msg: TWMScroll);
    procedure SetVisible(Value: Boolean);
    function IsRangeStored: Boolean;
    procedure Update(ControlSB, AssumeSB: Boolean);
  public

*/
/*
enum TPosition { spLeftUp, spCenter, spRightDown, spNone };

class TScrollBar : public TWinControl
{
    DECLARE_PROPERTY(TScrollBar);

private:

protected:
    word_t m_Position;
    TScrollBarKind m_Kind;
    TPosition m_CurrentPosition;
    TColor m_BackGroundColor;
    TColor m_ArrowColor;
    TColor m_ArrowBackGroundColor;
    TColor m_ClickedColor;
    bool m_Clicked;
//    word_t FScrollButtonSize;
    word_t m_Size;
    TColor m_ScrollButtonColor;
    TTimer FTimer;
    void set_Kind(TScrollBarKind value);
    void set_CurrentPosition(TPosition value);
    void set_BackGroundColor(TColor value);
    void set_ArrowColor(TColor value);
    void set_ArrowBackGroundColor(TColor value);
    void set_ClickedColor(TColor value);
    word_t get_ScrollButtonSize();
    void set_Size(word_t Value);
    void set_ScrollButtonColor(TColor value);
    void set_Position(word_t value);
    inline bool get_MyClicked() { return m_Clicked; };
    void set_MyClicked(bool value);

    void OnTimer(TObject *Sender);

    void set_Delay(uint_t Value);
    uint_t get_Delay();

    virtual void Paint();

//    procedure WMNCHitTest(var Message:TWMNCHitTest);message WM_NCHitTest;

    virtual void MouseExit(TShiftState state, int x, int y);
    virtual void MouseDown(TMouseButton button, TShiftState state, int x, int y);
    virtual void MouseUp(TMouseButton button, TShiftState state, int x, int y);

//    procedure WMLButtonDown(var Message:TWMLButtonDown);message WM_LBUTTONDOWN;
//    procedure WMLButtonUp(var Message:TWMLButtonUp);message WM_LBUTTONUP;

    property (m , TPosition, CurrentPosition);  // : read FCurrentPosition write MySetCurrentPosition;
    property (rw , bool,      MyClicked);  // read FClicked write SetClicked;
    property (r , word_t, ScrollButtonSize);  // read GetScrollButtonSize;

public:
    TScrollBar();

    property (m , word_t, Position);  // read FPosition write SetMyPosition;
    property (m , TScrollBarKind, Kind);  // read FScrollBarKind write SetScrollBarKind;
    property (m , TColor, BackGroundColor); // read FBackGroundColor write SetBackGroundColor;
    property (m , TColor, ArrowColor); // read FArrowColor write SetArrowColor;
    property (m , TColor, ArrowBackGroundColor);  //read FArrowBackGroundColor write SetArrowBackGroundColor;
    property (m , TColor, ClickedColor);   // read FClickedColor write SetClickedColor;
    property (mr, bool,   Clicked);  // read FClicked;
    property (m , word_t, Size); // read FSize write SetSize;
    property (m , TColor, ScrollButtonColor); // read FScrollButtonColor write SetScrollButtonColor;
    property (rw, uint_t, Delay); // read GetInterval write SetInterval;
};

*/
/*

#pragma option push -b-
enum TScrollCode { scLineUp, scLineDown, scPageUp, scPageDown, scPosition, scTrack, scTop, scBottom, scEndScroll };
#pragma option pop

typedef void __fastcall (__closure *TScrollEvent)(System::TObject* Sender, TScrollCode ScrollCode, int &ScrollPos);

class DELPHICLASS TScrollBar;
class PASCALIMPLEMENTATION TScrollBar : public Controls::TWinControl
{
	typedef Controls::TWinControl inherited;

private:
	Forms::TScrollBarKind FKind;
	int FPosition;
	int FMin;
	int FMax;
	int FPageSize;
	int FRTLFactor;
	Forms::TScrollBarInc FSmallChange;
	Forms::TScrollBarInc FLargeChange;
	Classes::TNotifyEvent FOnChange;
	TScrollEvent FOnScroll;
	void __fastcall DoScroll(Messages::TWMScroll &Message);
	void __fastcall SetKind(Forms::TScrollBarKind Value);
	void __fastcall SetMax(int Value);
	void __fastcall SetMin(int Value);
	void __fastcall SetPosition(int Value);
	void __fastcall SetPageSize(int Value);
	MESSAGE void __fastcall CNHScroll(Messages::TWMScroll &Message);
	MESSAGE void __fastcall CNVScroll(Messages::TWMScroll &Message);
	MESSAGE void __fastcall CNCtlColorScrollBar(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall WMEraseBkgnd(Messages::TWMEraseBkgnd &Message);

protected:
	virtual void __fastcall CreateParams(Controls::TCreateParams &Params);
	virtual void __fastcall CreateWnd(void);
	DYNAMIC void __fastcall Change(void);
	DYNAMIC void __fastcall Scroll(TScrollCode ScrollCode, int &ScrollPos);

public:
	__fastcall virtual TScrollBar(Classes::TComponent* AOwner);
	void __fastcall SetParams(int APosition, int AMin, int AMax);

__published:
	__property Forms::TScrollBarKind Kind = {read=FKind, write=SetKind, default=0};
	__property Forms::TScrollBarInc LargeChange = {read=FLargeChange, write=FLargeChange, default=1};
	__property int Max = {read=FMax, write=SetMax, default=100};
	__property int Min = {read=FMin, write=SetMin, default=0};
	__property int PageSize = {read=FPageSize, write=SetPageSize, nodefault};
	__property int Position = {read=FPosition, write=SetPosition, default=0};
	__property Forms::TScrollBarInc SmallChange = {read=FSmallChange, write=FSmallChange, default=1};
	__property Classes::TNotifyEvent OnChange = {read=FOnChange, write=FOnChange};

public:
	#pragma option push -w-inl
	// TWinControl.CreateParented // inline __fastcall TScrollBar(HWND ParentWindow) : Controls::TWinControl(ParentWindow) { }
	#pragma option pop
	#pragma option push -w-inl
	// TWinControl.Destroy // inline __fastcall virtual ~TScrollBar(void) { }
	#pragma option pop

};

*/
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
#endif // SCROLLBAR_H
