#include "comctrls.h"
#include "netwm.h"
#include <stdio.h>
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
extern HDISPLAY XDpy;
//---------------------------------------------------------------------------
//  TCommonButton
//---------------------------------------------------------------------------
TCommonButton :: TCommonButton()
{
    Init();
}
//---------------------------------------------------------------------------
TCommonButton :: ~TCommonButton()
{
}
//---------------------------------------------------------------------------
__fastcall TCommonButton :: TCommonButton(const TCommonButton &a) : TWinControl(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TCommonButton & TCommonButton :: operator=(const TCommonButton &a)
{
    if(this == &a)
        return *this;
    TWinControl :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TCommonButton :: Init()
{
    StyleOn(csButton);
    StyleOn(csFollowMouse);
}
//---------------------------------------------------------------------------
void __fastcall TCommonButton :: Assign(const TCommonButton &obj)
{
}
//---------------------------------------------------------------------------
//  TCommonCheckBox
//---------------------------------------------------------------------------
TCommonCheckBox :: TCommonCheckBox()
{
    Init();
}
//---------------------------------------------------------------------------
TCommonCheckBox :: ~TCommonCheckBox()
{
}
//---------------------------------------------------------------------------
__fastcall TCommonCheckBox :: TCommonCheckBox(const TCommonCheckBox &a) : TCommonButton(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TCommonCheckBox & TCommonCheckBox :: operator=(const TCommonCheckBox &a)
{
    if(this == &a)
        return *this;
    TCommonButton :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TCommonCheckBox :: Init()
{
    AllowUndefined = false;
    m_State = cbUnchecked;
}
//---------------------------------------------------------------------------
void __fastcall TCommonCheckBox :: Assign(const TCommonCheckBox &obj)
{
}
//---------------------------------------------------------------------------
void TCommonCheckBox :: set_Checked(bool value)
{
    State = value ? cbChecked : cbUnchecked;
}
//---------------------------------------------------------------------------
void TCommonCheckBox :: set_State(TCheckBoxState value)
{
    bool chk;

    if(m_State != value)
    {
        switch(value)
        {
        case cbUnchecked:
            chk = false;
            break;
        case cbChecked:
            chk = true;
            break;
        case cbUndefined:
            if(!AllowUndefined)
                value = cbUnchecked;
            chk = false;
            break;
        };
        m_State = value;
        TCommonButton :: set_Checked(chk);
        TCommonButton :: Click();
    }
}
//---------------------------------------------------------------------------
void TCommonCheckBox :: Click()
{
    switch(m_State)
    {
    case cbUnchecked:
        State = AllowUndefined ? cbUndefined : cbChecked;
        break;
    case cbChecked:
        State = cbUnchecked;
        break;
    case cbUndefined:
        State = cbChecked;
        break;
    };
}
//---------------------------------------------------------------------------
//  TCommonComboBox
//---------------------------------------------------------------------------
TCommonComboBox :: TCommonComboBox()
{
    Init();
}
//---------------------------------------------------------------------------
TCommonComboBox :: ~TCommonComboBox()
{
}
//---------------------------------------------------------------------------
__fastcall TCommonComboBox :: TCommonComboBox(const TCommonComboBox &a) : TWinControl(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TCommonComboBox & TCommonComboBox :: operator=(const TCommonComboBox &a)
{
    if(this == &a)
        return *this;
    TWinControl :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TCommonComboBox :: Init()
{
    StyleOn(csButton);
    StyleOn(csFollowMouse);
}
//---------------------------------------------------------------------------
void __fastcall TCommonComboBox :: Assign(const TCommonComboBox &obj)
{
}
//---------------------------------------------------------------------------
//  TCommonPanel
//---------------------------------------------------------------------------
TCommonPanel :: TCommonPanel()
{
    Init();
}
//---------------------------------------------------------------------------
TCommonPanel :: ~TCommonPanel()
{
}
//---------------------------------------------------------------------------
__fastcall TCommonPanel :: TCommonPanel(const TCommonPanel &a) : TWinControl(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TCommonPanel & TCommonPanel :: operator=(const TCommonPanel &a)
{
    if(this == &a)
        return *this;
    TWinControl :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TCommonPanel :: Init()
{
    Width = 185;
    Height = 41;
}
//---------------------------------------------------------------------------
void __fastcall TCommonPanel :: Assign(const TCommonPanel &obj)
{
}
//---------------------------------------------------------------------------
//  TCommonGroupBox
//---------------------------------------------------------------------------
TCommonGroupBox :: TCommonGroupBox()
{
    Init();
}
//---------------------------------------------------------------------------
TCommonGroupBox :: ~TCommonGroupBox()
{
}
//---------------------------------------------------------------------------
__fastcall TCommonGroupBox :: TCommonGroupBox(const TCommonGroupBox &a) : TWinControl(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TCommonGroupBox & TCommonGroupBox :: operator=(const TCommonGroupBox &a)
{
    if(this == &a)
        return *this;
    TWinControl :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TCommonGroupBox :: Init()
{
}
//---------------------------------------------------------------------------
void __fastcall TCommonGroupBox :: Assign(const TCommonGroupBox &obj)
{
}
//---------------------------------------------------------------------------
//  TCommonLabel
//---------------------------------------------------------------------------
TCommonLabel :: TCommonLabel()
{
    Init();
}
//---------------------------------------------------------------------------
TCommonLabel :: ~TCommonLabel()
{
}
//---------------------------------------------------------------------------
__fastcall TCommonLabel :: TCommonLabel(const TCommonLabel &a) : TControl(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TCommonLabel & TCommonLabel :: operator=(const TCommonLabel &a)
{
    if(this == &a)
        return *this;
    TControl :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TCommonLabel :: Init()
{
    Width = 65;
    Height = 17;
    AutoSize = true;
}
//---------------------------------------------------------------------------
__fastcall void TCommonLabel :: Assign(const TCommonLabel &obj)
{
}
//---------------------------------------------------------------------------
bool TCommonLabel :: get_WordWrap()
{
    return BIT_CHECK(m_FormatText.Options, DT_WORDBREAK);
}
//---------------------------------------------------------------------------
void TCommonLabel :: set_WordWrap(bool value)
{
    if( get_WordWrap() != value )
    {
        m_FormatText.Options = value ? BIT_ON(m_FormatText.Options, DT_WORDBREAK) : BIT_OFF(m_FormatText.Options, DT_WORDBREAK);
        AdjustSize();
//        Invalidate;
    }
}
//---------------------------------------------------------------------------
TAlignment TCommonLabel :: get_Alignment()
{
    return m_FormatText.Justify;
}
//---------------------------------------------------------------------------
void TCommonLabel :: set_Alignment(TAlignment value)
{
    if( get_Alignment() != value )
    {
        m_FormatText.Justify = value;
        //        Invalidate;
    }
}
//---------------------------------------------------------------------------
void TCommonLabel :: set_Caption(TString value)
{
    if(Caption != value)
    {
        TControl :: set_Caption(value);
        AdjustSize();
    };
}
//---------------------------------------------------------------------------
bool TCommonLabel :: get_ShowAccelChar()
{
    return ! BIT_CHECK(m_FormatText.Options, DT_NOPREFIX);
}
//---------------------------------------------------------------------------
void TCommonLabel :: set_ShowAccelChar(bool value)
{
    if( get_ShowAccelChar() != value )
    {
        m_FormatText.Options = value ? BIT_OFF(m_FormatText.Options, DT_NOPREFIX) : BIT_ON(m_FormatText.Options, DT_NOPREFIX);
        //Invalidate;
    }
}
//---------------------------------------------------------------------------
TTextLayout TCommonLabel :: get_Layout()
{
    return m_FormatText.Layout;
}
//---------------------------------------------------------------------------
void TCommonLabel :: set_Layout(TTextLayout value)
{
    if( get_Layout() != value )
    {
        m_FormatText.Layout = value;
        //        Invalidate;
    }
}
//---------------------------------------------------------------------------
/*
constructor TCustomLabel.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  ControlStyle := ControlStyle + [csOpaque, csReplicatable];
  Width := 65;
  Height := 17;
  FAutoSize := True;
  FShowAccelChar := True;
end;

function TCustomLabel.GetLabelText: string;
begin
  Result := Caption;
end;

procedure TCustomLabel.DoDrawText(var Rect: TRect; Flags: Longint);
var
  Text: string;
begin
  Text := GetLabelText;
  if (Flags and DT_CALCRECT <> 0) and ((Text = '') or FShowAccelChar and
    (Text[1] = '&') and (Text[2] = #0)) then Text := Text + ' ';
  if not FShowAccelChar then Flags := Flags or DT_NOPREFIX;
  Flags := DrawTextBiDiModeFlags(Flags);
  Canvas.Font := Font;
  if not Enabled then
  begin
    OffsetRect(Rect, 1, 1);
    Canvas.Font.Color := clBtnHighlight;
    DrawText(Canvas.Handle, PChar(Text), Length(Text), Rect, Flags);
    OffsetRect(Rect, -1, -1);
    Canvas.Font.Color := clBtnShadow;
    DrawText(Canvas.Handle, PChar(Text), Length(Text), Rect, Flags);
  end
  else
    DrawText(Canvas.Handle, PChar(Text), Length(Text), Rect, Flags);
end;

procedure TCustomLabel.Paint;
const
  Alignments: array[TAlignment] of Word = (DT_LEFT, DT_RIGHT, DT_CENTER);
  WordWraps: array[Boolean] of Word = (0, DT_WORDBREAK);
var
  Rect, CalcRect: TRect;
  DrawStyle: Longint;
begin
  with Canvas do
  begin
    if not Transparent then
    begin
      Brush.Color := Self.Color;
      Brush.Style := bsSolid;
      FillRect(ClientRect);
    end;
    Brush.Style := bsClear;
    Rect := ClientRect;
    { DoDrawText takes care of BiDi alignments }
    DrawStyle := DT_EXPANDTABS or WordWraps[FWordWrap] or Alignments[FAlignment];
    { Calculate vertical layout }
    if FLayout <> tlTop then
    begin
      CalcRect := Rect;
      DoDrawText(CalcRect, DrawStyle or DT_CALCRECT);
      if FLayout = tlBottom then OffsetRect(Rect, 0, Height - CalcRect.Bottom)
      else OffsetRect(Rect, 0, (Height - CalcRect.Bottom) div 2);
    end;
    DoDrawText(Rect, DrawStyle);
  end;
end;

procedure TCustomLabel.AdjustBounds;
const
  WordWraps: array[Boolean] of Word = (0, DT_WORDBREAK);
var
  DC: HDC;
  X: Integer;
  Rect: TRect;
  AAlignment: TAlignment;
begin
  if not (csReading in ComponentState) and FAutoSize then
  begin
    Rect := ClientRect;
    DC := GetDC(0);
    Canvas.Handle := DC;
    DoDrawText(Rect, (DT_EXPANDTABS or DT_CALCRECT) or WordWraps[FWordWrap]);
    Canvas.Handle := 0;
    ReleaseDC(0, DC);
    X := Left;
    AAlignment := FAlignment;
    if UseRightToLeftAlignment then ChangeBiDiModeAlignment(AAlignment);
    if AAlignment = taRightJustify then Inc(X, Width - Rect.Right);
    SetBounds(X, Top, Rect.Right, Rect.Bottom);
  end;
end;

procedure TCustomLabel.SetAlignment(Value: TAlignment);
begin
  if FAlignment <> Value then
  begin
    FAlignment := Value;
    Invalidate;
  end;
end;

procedure TCustomLabel.SetAutoSize(Value: Boolean);
begin
  if FAutoSize <> Value then
  begin
    FAutoSize := Value;
    AdjustBounds;
  end;
end;

function TCustomLabel.GetTransparent: Boolean;
begin
  Result := not (csOpaque in ControlStyle);
end;

procedure TCustomLabel.SetFocusControl(Value: TWinControl);
begin
  FFocusControl := Value;
  if Value <> nil then Value.FreeNotification(Self);
end;

procedure TCustomLabel.SetShowAccelChar(Value: Boolean);
begin
  if FShowAccelChar <> Value then
  begin
    FShowAccelChar := Value;
    Invalidate;
  end;
end;

procedure TCustomLabel.SetTransparent(Value: Boolean);
begin
  if Transparent <> Value then
  begin
    if Value then
      ControlStyle := ControlStyle - [csOpaque] else
      ControlStyle := ControlStyle + [csOpaque];
    Invalidate;
  end;
end;

procedure TCustomLabel.SetLayout(Value: TTextLayout);
begin
  if FLayout <> Value then
  begin
    FLayout := Value;
    Invalidate;
  end;
end;

procedure TCustomLabel.SetWordWrap(Value: Boolean);
begin
  if FWordWrap <> Value then
  begin
    FWordWrap := Value;
    AdjustBounds;
    Invalidate;
  end;
end;

procedure TCustomLabel.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited Notification(AComponent, Operation);
  if (Operation = opRemove) and (AComponent = FFocusControl) then
    FFocusControl := nil;
end;

procedure TCustomLabel.CMTextChanged(var Message: TMessage);
begin
  Invalidate;
  AdjustBounds;
end;

procedure TCustomLabel.CMFontChanged(var Message: TMessage);
begin
  inherited;
  AdjustBounds;
end;

procedure TCustomLabel.CMDialogChar(var Message: TCMDialogChar);
begin
  if (FFocusControl <> nil) and Enabled and ShowAccelChar and
    IsAccel(Message.CharCode, Caption) then
    with FFocusControl do
      if CanFocus then
      begin
        SetFocus;
        Message.Result := 1;
      end;
end;

*/
//-------------------------------------------------------------------
//  TCommonScrollbar
//-------------------------------------------------------------------
TCommonScrollbar :: TCommonScrollbar()
{
    Init();
}
//---------------------------------------------------------------------------
TCommonScrollbar :: ~TCommonScrollbar()
{
}
//---------------------------------------------------------------------------
__fastcall TCommonScrollbar :: TCommonScrollbar(const TCommonScrollbar &a) : TControl(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TCommonScrollbar & TCommonScrollbar :: operator=(const TCommonScrollbar &a)
{
    if(this == &a)
        return *this;
    TControl :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TCommonScrollbar :: Init()
{
    StyleOn(csButton);
    StyleOn(csFollowMouse);
    StyleOn(csAllowRepeat);
    m_Kind = sbHorizontal;
    m_Max = 100;
    m_Min = 0;
    LargeChange = 1;
    SmallChange = 1;
    m_PageSize = 0;
    m_Position = 0;

    m_ButtonSize = 0;
    Color = clBtnHighlight;
    Increment = 8;
    Margin = 0;
    m_Range = 0;
    m_Size = 0;
    m_ThumbSize = 0;
    Smooth = false;
    Tracking = false;
    m_PageIncrement = 80;
    m_Delay = 10;
    m_LineDiv = 4;
    m_PageDiv = 12;
    m_UpdateNeeded = true;
}
//---------------------------------------------------------------------------
void __fastcall TCommonScrollbar :: Assign(const TCommonScrollbar &obj)
{
}
//---------------------------------------------------------------------------
void TCommonScrollbar :: set_Kind(TScrollbarKind value)
{
    if(m_Kind != value)
    {
        uint_t w, h;
        w = Width;
        h = Height;
        Width = h;
        Height = w;
        m_Kind = value;
        // if not (csLoading in ComponentState) then
        // SetBounds(Left, Top, Height, Width);
        // RecreateWnd;
    }
}
//---------------------------------------------------------------------------
/*
{ TScrollBar }

constructor TScrollBar.Create(AOwner: TComponent);
begin
  Width := 121;
  Height := GetSystemMetrics(SM_CYHSCROLL);
  TabStop := True;
  ControlStyle := [csFramed, csDoubleClicks, csOpaque];

end;

procedure TScrollBar.CreateParams(var Params: TCreateParams);
const
  Kinds: array[TScrollBarKind] of DWORD = (SBS_HORZ, SBS_VERT);
begin
  inherited CreateParams(Params);
  CreateSubClass(Params, 'SCROLLBAR');
  Params.Style := Params.Style or Kinds[FKind];
  if FKind = sbVertical then
    if not UseRightToLeftAlignment then
      Params.Style := Params.Style or SBS_RIGHTALIGN
    else
      Params.Style := Params.Style or SBS_LEFTALIGN;
  if NotRightToLeft then
    FRTLFactor := 1
  else
    FRTLFactor := -1;
end;

procedure TScrollBar.CreateWnd;
var
  ScrollInfo: TScrollInfo;
  LBounds: TRect;
begin
  // Windows' does not always create the window size we ask for, so we have
  //  insist sometimes.  Setting BoundsRect will only adjust the size if needed.
  LBounds := BoundsRect;
  inherited CreateWnd;
  BoundsRect := LBounds;

  SetScrollRange(Handle, SB_CTL, FMin, FMax, False);
  ScrollInfo.cbSize := SizeOf(ScrollInfo);
  ScrollInfo.nPage := FPageSize;
  ScrollInfo.fMask := SIF_PAGE;
  SetScrollInfo(Handle, SB_CTL, ScrollInfo, False);
  if NotRightToLeft then
    SetScrollPos(Handle, SB_CTL, FPosition, True)
  else
    SetScrollPos(Handle, SB_CTL, FMax - FPosition, True);
end;

procedure TScrollBar.SetParams(APosition, AMin, AMax: Integer);
begin
  if (AMax < AMin) or (AMax < FPageSize) then
    raise EInvalidOperation.Create(SScrollBarRange);
  if APosition < AMin then APosition := AMin;
  if APosition > AMax then APosition := AMax;
  if (FMin <> AMin) or (FMax <> AMax) then
  begin
    FMin := AMin;
    FMax := AMax;
    if HandleAllocated then
      SetScrollRange(Handle, SB_CTL, AMin, AMax, FPosition = APosition);
  end;
  if FPosition <> APosition then
  begin
    FPosition := APosition;
    if HandleAllocated then
      if NotRightToLeft then
        SetScrollPos(Handle, SB_CTL, FPosition, True)
      else
        SetScrollPos(Handle, SB_CTL, FMax - FPosition, True);
    Enabled := True;
    Change;
  end;
end;

procedure TScrollBar.SetPosition(Value: Integer);
begin
  SetParams(Value, FMin, FMax);
end;

procedure TScrollBar.SetPageSize(Value: Integer);
var
  ScrollInfo: TScrollInfo;
begin
  if (FPageSize = Value) or (Value > FMax) then exit;
  FPageSize := Value;
  ScrollInfo.cbSize := SizeOf(ScrollInfo);
  ScrollInfo.nPage := Value;
  ScrollInfo.fMask := SIF_PAGE;
  if HandleAllocated then
    SetScrollInfo(Handle, SB_CTL, ScrollInfo, True);
end;

procedure TScrollBar.SetMin(Value: Integer);
begin
  SetParams(FPosition, Value, FMax);
end;

procedure TScrollBar.SetMax(Value: Integer);
begin
  SetParams(FPosition, FMin, Value);
end;

procedure TScrollBar.Change;
begin
  inherited Changed;
  if Assigned(FOnChange) then FOnChange(Self);
end;

procedure TScrollBar.Scroll(ScrollCode: TScrollCode; var ScrollPos: Integer);
begin
  if Assigned(FOnScroll) then FOnScroll(Self, ScrollCode, ScrollPos);
end;

procedure TScrollBar.DoScroll(var Message: TWMScroll);
var
  ScrollPos: Integer;
  NewPos: Longint;
  ScrollInfo: TScrollInfo;
begin
  with Message do
  begin
    NewPos := FPosition;
    case TScrollCode(ScrollCode) of
      scLineUp:
        Dec(NewPos, FSmallChange * FRTLFactor);
      scLineDown:
        Inc(NewPos, FSmallChange * FRTLFactor);
      scPageUp:
        Dec(NewPos, FLargeChange * FRTLFactor);
      scPageDown:
        Inc(NewPos, FLargeChange * FRTLFactor);
      scPosition, scTrack:
        with ScrollInfo do
        begin
          cbSize := SizeOf(ScrollInfo);
          fMask := SIF_ALL;
          GetScrollInfo(Handle, SB_CTL, ScrollInfo);
          NewPos := nTrackPos;
          { We need to reverse the positioning because SetPosition below
            calls SetParams that reverses the position. This acts as a
            double negative. }
          if not NotRightToLeft then NewPos := FMax - NewPos;
        end;
      scTop:
        NewPos := FMin;
      scBottom:
        NewPos := FMax;
    end;
    if NewPos < FMin then NewPos := FMin;
    if NewPos > FMax then NewPos := FMax;
    ScrollPos := NewPos;
    Scroll(TScrollCode(ScrollCode), ScrollPos);
    SetPosition(ScrollPos);
  end;
end;

procedure TScrollBar.CNHScroll(var Message: TWMHScroll);
begin
  DoScroll(Message);
end;

procedure TScrollBar.CNVScroll(var Message: TWMVScroll);
begin
  DoScroll(Message);
end;

procedure TScrollBar.CNCtlColorScrollBar(var Message: TMessage);
begin
  with Message do
    CallWindowProc(DefWndProc, Handle, Msg, WParam, LParam);
end;
*/
//---------------------------------------------------------------------------
//  TCommonEdit
//---------------------------------------------------------------------------
TCommonEdit :: TCommonEdit()
{
    Init();
}
//---------------------------------------------------------------------------
TCommonEdit :: ~TCommonEdit()
{
}
//---------------------------------------------------------------------------
__fastcall TCommonEdit :: TCommonEdit(const TCommonEdit &a) : TWinControl(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TCommonEdit & TCommonEdit :: operator=(const TCommonEdit &a)
{
    if(this == &a)
        return *this;
    TWinControl :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TCommonEdit :: Init()
{
    AutoSelect = true;
    CaretKind = ctVerticalLine;
    CharCase = ecNormal;
    MaxLength = 0;
    PasswordChar = '\0';
    ReadOnly = false;
    Cursor = crIBeam;
    StyleOn(csButton);
}
//---------------------------------------------------------------------------
void __fastcall TCommonEdit :: Assign(const TCommonEdit &obj)
{
}
//---------------------------------------------------------------------------
void __fastcall TCommonEdit :: DoSelectionRequest(XEvent &event)
{
    printf("SELECTION_REQUEST id=%d\n",id);

    unsigned char *data = NULL;
    int nbytes = 0;
    Atom prop;
    /*   xselectionrequest

    Window event.xselectionrequest.owner;
    Window event.xselectionrequest.requestor;
    Atom event.xselectionrequest.selection;
    Atom event.xselectionrequest.target;
    Atom event.xselectionrequest.property;
    Time event.xselectionrequest.time;

    typedef struct {
    int type;		         SelectionRequest

    Display *display;	 Display the event was read from
    Window owner;
    Window requestor;
    Atom selection;
    Atom target;
    Atom property;
    Time time;
    } XSelectionRequestEvent;
               */


    if(data)
    {
        prop = event.xselectionrequest.property;
        XChangeProperty(event.xselectionrequest.display, event.xselectionrequest.requestor,
                        event.xselectionrequest.property, event.xselectionrequest.target,
                        8, PropModeReplace, data, nbytes);
    }
    else
        prop = None;

    XEvent respond;
    respond.xselection.type = SelectionNotify;
    respond.xselection.display = event.xselectionrequest.display;
    respond.xselection.requestor = event.xselectionrequest.requestor;
    respond.xselection.selection = event.xselectionrequest.selection;
    respond.xselection.target = event.xselectionrequest.target;
    respond.xselection.property = prop;
    respond.xselection.time = event.xselectionrequest.time;
    XSendEvent(XDpy, event.xselectionrequest.requestor, 0, 0, &respond);
}
//---------------------------------------------------------------------------
void __fastcall TCommonEdit :: DoSelectionNotify(XEvent &event)
{
    printf("SELECTION_NOTIFY id=%d\n",id);

    Atom prop = event.xselection.property;
    if(prop == None)
        return;

    Atom type_ret;
    int format_ret;
    unsigned long nitems_ret = 0;
    unsigned long bytes_left;
    unsigned char *prop_data = NULL;


    if(XGetWindowProperty(event.xselection.display, event.xselection.requestor, prop, 0, -1, true,
                          event.xselection.target, &type_ret, &format_ret, &nitems_ret,
                          &bytes_left, &prop_data) != Success)
        return;


    XFree(prop_data);

    /*
      Window event.xselection.requestor;
    Atom event.xselection.selection;
    Atom event.xselection.target;
    Atom event.xselection.property;          //Atom or None
    Time event.xselection.time;
    */
    /*            int WriteClipboard(void *data, int len)
                {
                    XSetSelectionOwner(XDpy, selection, m_Handle, CurrentTime)
          Atom selection;

                }

                int ReadClipboard(int cbformat, void *data)
                {
    Window XGetSelectionOwner(XDpy, selection)
          Atom selection;
                }
                void * __fastcall GetXProperty(m_Handle, event.xselection.property, int *nitems);

                XConvertSelection(display, selection, target, property, requestor, time)
          Display *display;
          Atom selection, target;
          Atom property;
          Window requestor;
          Time time;  */


}
//---------------------------------------------------------------------------
void TCommonEdit :: Clear()
{
}
//---------------------------------------------------------------------------
void TCommonEdit :: DoEnter()
{
    TRect r = ClientRect;
    TRect rect = Canvas.TextExtents(Text);
    int x = rect.height;
    int y = r.height - (r.height - x) / 2 - 1;
    Caret.Pos = TPoint(5 + rect.width + 1, y);
    Caret.Kind = CaretKind;
    Caret.Show(this);
    TWinControl :: DoEnter();
}
//---------------------------------------------------------------------------
void TCommonEdit :: DoExit()
{
    Caret.Hide();
    TWinControl :: DoExit();
}
//---------------------------------------------------------------------------
void TCommonEdit :: PasteFromClipboard()
{
    HWND wnd = GetClipboardOwner();
    if(wnd == None)
    {
        /* We didn't get the selection */
        printf("not SET_SELECTION_OWNER id=%d\n",id);
        return;
    }
    printf("CONVERT_SELECTION id=%d\n",id);

    Atom target = CF_TEXT;
    Atom property = GetAtom(CLIPBOARD);
    XConvertSelection(Application.Handle, XA_PRIMARY, target, property, m_Handle, CurrentTime);
}
//---------------------------------------------------------------------------
void TCommonEdit :: CopyToClipboard()
{
    SetClipboardOwner(m_Handle);
    printf("SET_SELECTION_OWNER id=%d\n",id);
}
//---------------------------------------------------------------------------
bool __fastcall TCommonEdit :: WndProc(XEvent &event)
{
    bool ret = false;

    switch (event.type)
    {
    case SelectionRequest:
//                DoSelectionRequest(event);
        break;
    case SelectionNotify:
        DoSelectionNotify(event);
        break;
    case SelectionClear:
        printf("SELECTION_CLEAR id=%d\n",id);
        /*
        Window xselectionclear.window;
        Atom xselectionclear.selection;
        Time xselectionclear.time;
                   */
        break;
    }
    if(!ret)
        ret = TWinControl::WndProc(event);
    return ret;
}
//---------------------------------------------------------------------------
/*
{ TCustomEdit }

constructor TCustomEdit.Create(AOwner: TComponent);
const
  EditStyle = [csClickEvents, csSetCaption, csDoubleClicks, csFixedHeight];
begin
  if NewStyleControls then
    ControlStyle := EditStyle else
    ControlStyle := EditStyle + [csFramed];
  Width := 121;
  Height := 25;
  TabStop := True;
  FAutoSize := True;
  FAutoSelect := True;
  FHideSelection := True;
  AdjustHeight;
end;

procedure TCustomEdit.SetAutoSize(Value: Boolean);
begin
  if FAutoSize <> Value then
  begin
    FAutoSize := Value;
    UpdateHeight;
  end;
end;

procedure TCustomEdit.SetHideSelection(Value: Boolean);
begin
  if FHideSelection <> Value then
  begin
    FHideSelection := Value;
    RecreateWnd;
  end;
end;

function TCustomEdit.GetModified: Boolean;
begin
  Result := FModified;
  if HandleAllocated then Result := SendMessage(Handle, EM_GETMODIFY, 0, 0) <> 0;
end;

function TCustomEdit.GetCanUndo: Boolean;
begin
  Result := False;
  if HandleAllocated then Result := SendMessage(Handle, EM_CANUNDO, 0, 0) <> 0;
end;

procedure TCustomEdit.SetModified(Value: Boolean);
begin
  if HandleAllocated then
    SendMessage(Handle, EM_SETMODIFY, Byte(Value), 0) else
    FModified := Value;
end;

function TCustomEdit.GetSelStart: Integer;
begin
  SendMessage(Handle, EM_GETSEL, Longint(@Result), 0);
end;

procedure TCustomEdit.SetSelStart(Value: Integer);
begin
  SendMessage(Handle, EM_SETSEL, Value, Value);
end;

function TCustomEdit.GetSelLength: Integer;
var
  Selection: TSelection;
begin
  SendMessage(Handle, EM_GETSEL, Longint(@Selection.StartPos), Longint(@Selection.EndPos));
  Result := Selection.EndPos - Selection.StartPos;
end;

procedure TCustomEdit.SetSelLength(Value: Integer);
var
  Selection: TSelection;
begin
  SendMessage(Handle, EM_GETSEL, Longint(@Selection.StartPos), Longint(@Selection.EndPos));
  Selection.EndPos := Selection.StartPos + Value;
  SendMessage(Handle, EM_SETSEL, Selection.StartPos, Selection.EndPos);
  SendMessage(Handle, EM_SCROLLCARET, 0,0);
end;

procedure TCustomEdit.ClearSelection;
begin
  SendMessage(Handle, WM_CLEAR, 0, 0);
end;

procedure TCustomEdit.CutToClipboard;
begin
  SendMessage(Handle, WM_CUT, 0, 0);
end;

procedure TCustomEdit.Undo;
begin
  SendMessage(Handle, WM_UNDO, 0, 0);
end;

procedure TCustomEdit.ClearUndo;
begin
  SendMessage(Handle, EM_EMPTYUNDOBUFFER, 0, 0);
end;

procedure TCustomEdit.SelectAll;
begin
  SendMessage(Handle, EM_SETSEL, 0, -1);
end;

function TCustomEdit.GetSelTextBuf(Buffer: PChar; BufSize: Integer): Integer;
var
  P: PChar;
  StartPos: Integer;
begin
  StartPos := GetSelStart;
  Result := GetSelLength;
  P := StrAlloc(GetTextLen + 1);
  try
    GetTextBuf(P, StrBufSize(P));
    if Result >= BufSize then Result := BufSize - 1;
    StrLCopy(Buffer, P + StartPos, Result);
  finally
    StrDispose(P);
  end;
end;

procedure TCustomEdit.SetSelTextBuf(Buffer: PChar);
begin
  SendMessage(Handle, EM_REPLACESEL, 0, LongInt(Buffer));
end;

function TCustomEdit.GetSelText: string;
var
  P: PChar;
  SelStart, Len: Integer;
begin
  SelStart := GetSelStart;
  Len := GetSelLength;
  SetString(Result, PChar(nil), Len);
  if Len <> 0 then
  begin
    P := StrAlloc(GetTextLen + 1);
    try
      GetTextBuf(P, StrBufSize(P));
      Move(P[SelStart], Pointer(Result)^, Len);
    finally
      StrDispose(P);
    end;
  end;
end;

procedure TCustomEdit.SetSelText(const Value: String);
begin
  SendMessage(Handle, EM_REPLACESEL, 0, Longint(PChar(Value)));
end;

procedure TCustomEdit.CreateParams(var Params: TCreateParams);
const
  Passwords: array[Boolean] of DWORD = (0, ES_PASSWORD);
  ReadOnlys: array[Boolean] of DWORD = (0, ES_READONLY);
  CharCases: array[TEditCharCase] of DWORD = (0, ES_UPPERCASE, ES_LOWERCASE);
  HideSelections: array[Boolean] of DWORD = (ES_NOHIDESEL, 0);
  OEMConverts: array[Boolean] of DWORD = (0, ES_OEMCONVERT);
begin
  inherited CreateParams(Params);
  CreateSubClass(Params, 'EDIT');
  with Params do
  begin
    Style := Style or (ES_AUTOHSCROLL or ES_AUTOVSCROLL) or
      BorderStyles[FBorderStyle] or Passwords[FPasswordChar <> #0] or
      ReadOnlys[FReadOnly] or CharCases[FCharCase] or
      HideSelections[FHideSelection] or OEMConverts[FOEMConvert];
    if NewStyleControls and Ctl3D and (FBorderStyle = bsSingle) then
    begin
      Style := Style and not WS_BORDER;
      ExStyle := ExStyle or WS_EX_CLIENTEDGE;
    end;
  end;
end;

procedure TCustomEdit.CreateWindowHandle(const Params: TCreateParams);
var
  P: TCreateParams;
begin
  if SysLocale.FarEast and (Win32Platform <> VER_PLATFORM_WIN32_NT) and
    ((Params.Style and ES_READONLY) <> 0) then
  begin
    // Work around Far East Win95 API/IME bug.
    P := Params;
    P.Style := P.Style and (not ES_READONLY);
    inherited CreateWindowHandle(P);
    if WindowHandle <> 0 then
      SendMessage(WindowHandle, EM_SETREADONLY, Ord(True), 0);
  end
  else
    inherited CreateWindowHandle(Params);
end;

procedure TCustomEdit.CreateWnd;
begin
  FCreating := True;
  try
    inherited CreateWnd;
  finally
    FCreating := False;
  end;
  DoSetMaxLength(FMaxLength);
  Modified := FModified;
  if FPasswordChar <> #0 then
    SendMessage(Handle, EM_SETPASSWORDCHAR, Ord(FPasswordChar), 0);
  UpdateHeight;
end;

procedure TCustomEdit.DestroyWnd;
begin
  FModified := Modified;
  inherited DestroyWnd;
end;

procedure TCustomEdit.UpdateHeight;
begin
  if FAutoSize and (BorderStyle = bsSingle) then
  begin
    ControlStyle := ControlStyle + [csFixedHeight];
    AdjustHeight;
  end else
    ControlStyle := ControlStyle - [csFixedHeight];
end;

procedure TCustomEdit.AdjustHeight;
var
  DC: HDC;
  SaveFont: HFont;
  I: Integer;
  SysMetrics, Metrics: TTextMetric;
begin
  DC := GetDC(0);
  GetTextMetrics(DC, SysMetrics);
  SaveFont := SelectObject(DC, Font.Handle);
  GetTextMetrics(DC, Metrics);
  SelectObject(DC, SaveFont);
  ReleaseDC(0, DC);
  if NewStyleControls then
  begin
    if Ctl3D then I := 8 else I := 6;
    I := GetSystemMetrics(SM_CYBORDER) * I;
  end else
  begin
    I := SysMetrics.tmHeight;
    if I > Metrics.tmHeight then I := Metrics.tmHeight;
    I := I div 4 + GetSystemMetrics(SM_CYBORDER) * 4;
  end;
  Height := Metrics.tmHeight + I;
end;

procedure TCustomEdit.Change;
begin
  inherited Changed;
  if Assigned(FOnChange) then FOnChange(Self);
end;

procedure TCustomEdit.DefaultHandler(var Message);
begin
  case TMessage(Message).Msg of
    WM_SETFOCUS:
      if (Win32Platform = VER_PLATFORM_WIN32_WINDOWS) and
        not IsWindow(TWMSetFocus(Message).FocusedWnd) then
        TWMSetFocus(Message).FocusedWnd := 0;
  end;
  inherited;
end;

procedure TCustomEdit.WMSetFont(var Message: TWMSetFont);
begin
  inherited;
  if NewStyleControls and
    (GetWindowLong(Handle, GWL_STYLE) and ES_MULTILINE = 0) then
    SendMessage(Handle, EM_SETMARGINS, EC_LEFTMARGIN or EC_RIGHTMARGIN, 0);
end;

procedure TCustomEdit.CMCtl3DChanged(var Message: TMessage);
begin
  if NewStyleControls and (FBorderStyle = bsSingle) then
  begin
    UpdateHeight;
    RecreateWnd;
  end;
  inherited;
end;

procedure TCustomEdit.CMFontChanged(var Message: TMessage);
begin
  inherited;
  if (csFixedHeight in ControlStyle) and not ((csDesigning in
    ComponentState) and (csLoading in ComponentState)) then AdjustHeight;
end;

procedure TCustomEdit.CNCommand(var Message: TWMCommand);
begin
  if (Message.NotifyCode = EN_CHANGE) and not FCreating then Change;
end;

procedure TCustomEdit.CMEnter(var Message: TCMGotFocus);
begin
  if FAutoSelect and not (csLButtonDown in ControlState) and
    (GetWindowLong(Handle, GWL_STYLE) and ES_MULTILINE = 0) then SelectAll;
  inherited;
end;

procedure TCustomEdit.CMTextChanged(var Message: TMessage);
begin
  inherited;
  if not HandleAllocated or (GetWindowLong(Handle, GWL_STYLE) and
    ES_MULTILINE <> 0) then Change;
end;

procedure TCustomEdit.WMContextMenu(var Message: TWMContextMenu);
begin
  SetFocus;
  inherited;
end;
*/
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------

