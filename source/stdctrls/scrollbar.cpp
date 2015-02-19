#include "stdctrls.h"
#include "scrollbar.h"
//---------------------------------------------------------------------------
namespace xwings {
//-------------------------------------------------------------------
//  TScrollbar
//-------------------------------------------------------------------
TScrollbar :: TScrollbar()
{
    Init();
}
//---------------------------------------------------------------------------
TScrollbar :: ~TScrollbar()
{
}
//---------------------------------------------------------------------------
__fastcall TScrollbar :: TScrollbar(const TScrollbar &a) : TCommonScrollbar(a)
{
	Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TScrollbar & TScrollbar :: operator=(const TScrollbar &a)
{
	if(this == &a)
		return *this;
    TCommonScrollbar :: operator=(a);
    Assign(a);
	return *this;
}
//---------------------------------------------------------------------------
void TScrollbar :: Init()
{
    m_Style = ssRegular;
    Width = 140;
    Height = 20;
}
//---------------------------------------------------------------------------
void __fastcall TScrollbar :: Assign(const TScrollbar &obj)
{
}
//---------------------------------------------------------------------------
void TScrollbar :: Paint()
{
    TRect r = ClientRect;
    TRect rect;

    if(Kind == sbHorizontal)
    {
        rect = TRect(r.left + r.width - r.height, r.top, r.height, r.height);
        if(CheckState(csLButtonDown))
            DrawButtonArrow(&Canvas, rect, arrowRight, true);
        else
            DrawButtonArrow(&Canvas, rect, arrowRight, false);

        rect = TRect(r.left, r.top, r.height, r.height);
        if(CheckState(csLButtonDown))
            DrawButtonArrow(&Canvas, rect, arrowLeft, true);
        else
            DrawButtonArrow(&Canvas, rect, arrowLeft, false);

        Canvas.Brush->Color = clScrollBar;
        rect = TRect(r.left + r.height + 1, r.top, r.width - 2 * r.height - 2, r.height);
        Canvas.FillRect(rect);

        Canvas.Brush->Color = clBtnFace;
        rect = TRect(r.left + r.width/2, r.top, r.height, r.height);
        Canvas.FillRect(rect);
        if(CheckState(csLButtonDown))
            Frame3D(&Canvas, rect, clBtnShadow, clBtnHighlight, 1);
        else
            Frame3D(&Canvas, rect, clBtnHighlight, clBtnShadow, 1);
    }
    else
    {
        rect = TRect(r.left, r.top + r.height - r.width, r.width, r.width);
        if(CheckState(csLButtonDown))
            DrawButtonArrow(&Canvas, rect, arrowDown, true);
        else
            DrawButtonArrow(&Canvas, rect, arrowDown, false);

        rect = TRect(r.left, r.top, r.width, r.width);
        if(CheckState(csLButtonDown))
            DrawButtonArrow(&Canvas, rect, arrowUp, true);
        else
            DrawButtonArrow(&Canvas, rect, arrowUp, false);

        Canvas.Brush->Color = clScrollBar;
        rect = TRect(r.left, r.top + r.width + 1, r.width, r.height - 2 * r.width - 2);
        Canvas.FillRect(rect);

        Canvas.Brush->Color = clBtnFace;
        rect = TRect(r.left, r.top + r.height/2, r.width, r.width);
        Canvas.FillRect(rect);
        if(CheckState(csLButtonDown))
            Frame3D(&Canvas, rect, clBtnShadow, clBtnHighlight, 1);
        else
            Frame3D(&Canvas, rect, clBtnHighlight, clBtnShadow, 1);
    }
}
//---------------------------------------------------------------------------
/*
int TCommonScrollbar :: get_ScrollPos()
{
    int Result = 0;
    if(Visible)
        Result = m_Position;
    return Result;
}
//---------------------------------------------------------------------------
void TCommonScrollbar :: set_ButtonSize(int value)
{
  SysConsts: array[TScrollBarKind] of Integer = (SM_CXHSCROLL, SM_CXVSCROLL);
var
  NewValue: Integer;
begin
    if(value != m_ButtonSize)
    {
    NewValue := Value;
    if NewValue = 0 then
      Value := GetSystemMetrics(SysConsts[Kind]);
        m_ButtonSize = value;
        m_UpdateNeeded = true;
        m_Control.UpdateScrollBars;
        if NewValue = 0 then
            m_ButtonSize = 0;
    }
}
//---------------------------------------------------------------------------

function TControlScrollBar.IsIncrementStored: Boolean;
begin
  Result := not Smooth;
end;

procedure TControlScrollBar.Assign(Source: TPersistent);
begin
  if Source is TControlScrollBar then
  begin
    Visible := TControlScrollBar(Source).Visible;
    Range := TControlScrollBar(Source).Range;
    Position := TControlScrollBar(Source).Position;
    Increment := TControlScrollBar(Source).Increment;
    Exit;
  end;
  inherited Assign(Source);
end;

procedure TControlScrollBar.CalcAutoRange;
var
  I: Integer;
  NewRange, AlignMargin: Integer;

  procedure ProcessHorz(Control: TControl);
  begin
    if Control.Visible then
      case Control.Align of
        alLeft, alNone:
          if (Control.Align = alLeft) or (Control.Anchors * [akLeft, akRight] = [akLeft]) then
            NewRange := Max(NewRange, Position + Control.Left + Control.Width);
        alRight: Inc(AlignMargin, Control.Width);
      end;
  end;

  procedure ProcessVert(Control: TControl);
  begin
    if Control.Visible then
      case Control.Align of
        alTop, alNone:
          if (Control.Align = alTop) or (Control.Anchors * [akTop, akBottom] = [akTop]) then
            NewRange := Max(NewRange, Position + Control.Top + Control.Height);
        alBottom: Inc(AlignMargin, Control.Height);
      end;
  end;

begin
  if FControl.FAutoScroll then
  begin
    if FControl.AutoScrollEnabled then
    begin
      NewRange := 0;
      AlignMargin := 0;
      for I := 0 to FControl.ControlCount - 1 do
        if Kind = sbHorizontal then
          ProcessHorz(FControl.Controls[I]) else
          ProcessVert(FControl.Controls[I]);
      DoSetRange(NewRange + AlignMargin + Margin);
    end
    else DoSetRange(0);
  end;
end;

function TControlScrollBar.IsScrollBarVisible: Boolean;
var
  Style: Longint;
begin
  Style := WS_HSCROLL;
  if Kind = sbVertical then Style := WS_VSCROLL;
  Result := (Visible) and
            (GetWindowLong(FControl.Handle, GWL_STYLE) and Style <> 0);
end;

function TControlScrollBar.ControlSize(ControlSB, AssumeSB: Boolean): Integer;
var
  BorderAdjust: Integer;

  function ScrollBarVisible(Code: Word): Boolean;
  var
    Style: Longint;
  begin
    Style := WS_HSCROLL;
    if Code = SB_VERT then Style := WS_VSCROLL;
    Result := GetWindowLong(FControl.Handle, GWL_STYLE) and Style <> 0;
  end;

  function Adjustment(Code, Metric: Word): Integer;
  begin
    Result := 0;
    if not ControlSB then
      if AssumeSB and not ScrollBarVisible(Code) then
        Result := -(GetSystemMetrics(Metric) - BorderAdjust)
      else if not AssumeSB and ScrollBarVisible(Code) then
        Result := GetSystemMetrics(Metric) - BorderAdjust;
  end;

begin
  BorderAdjust := Integer(GetWindowLong(FControl.Handle, GWL_STYLE) and
    (WS_BORDER or WS_THICKFRAME) <> 0);
  if Kind = sbVertical then
    Result := FControl.ClientHeight + Adjustment(SB_HORZ, SM_CXHSCROLL) else
    Result := FControl.ClientWidth + Adjustment(SB_VERT, SM_CYVSCROLL);
end;

function TControlScrollBar.NeedsScrollBarVisible: Boolean;
begin
  Result := FRange > ControlSize(False, False);
end;

procedure TControlScrollBar.ScrollMessage(var Msg: TWMScroll);
var
  Incr, FinalIncr, Count: Integer;
  CurrentTime, StartTime, ElapsedTime: Longint;

  function GetRealScrollPosition: Integer;
  var
    SI: TScrollInfo;
    Code: Integer;
  begin
    SI.cbSize := SizeOf(TScrollInfo);
    SI.fMask := SIF_TRACKPOS;
    Code := SB_HORZ;
    if FKind = sbVertical then Code := SB_VERT;
    Result := Msg.Pos;
    if FlatSB_GetScrollInfo(FControl.Handle, Code, SI) then
      Result := SI.nTrackPos;
  end;

begin
  with Msg do
  begin
    if FSmooth and (ScrollCode in [SB_LINEUP, SB_LINEDOWN, SB_PAGEUP, SB_PAGEDOWN]) then
    begin
      case ScrollCode of
        SB_LINEUP, SB_LINEDOWN:
          begin
            Incr := FIncrement div FLineDiv;
            FinalIncr := FIncrement mod FLineDiv;
            Count := FLineDiv;
          end;
        SB_PAGEUP, SB_PAGEDOWN:
          begin
            Incr := FPageIncrement;
            FinalIncr := Incr mod FPageDiv;
            Incr := Incr div FPageDiv;
            Count := FPageDiv;
          end;
      else
        Count := 0;
        Incr := 0;
        FinalIncr := 0;
      end;
      CurrentTime := 0;
      while Count > 0 do
      begin
        StartTime := GetCurrentTime;
        ElapsedTime := StartTime - CurrentTime;
        if ElapsedTime < FDelay then Sleep(FDelay - ElapsedTime);
        CurrentTime := StartTime;
        case ScrollCode of
          SB_LINEUP: SetPosition(FPosition - Incr);
          SB_LINEDOWN: SetPosition(FPosition + Incr);
          SB_PAGEUP: SetPosition(FPosition - Incr);
          SB_PAGEDOWN: SetPosition(FPosition + Incr);
        end;
        FControl.Update;
        Dec(Count);
      end;
      if FinalIncr > 0 then
      begin
        case ScrollCode of
          SB_LINEUP: SetPosition(FPosition - FinalIncr);
          SB_LINEDOWN: SetPosition(FPosition + FinalIncr);
          SB_PAGEUP: SetPosition(FPosition - FinalIncr);
          SB_PAGEDOWN: SetPosition(FPosition + FinalIncr);
        end;
      end;
    end
    else
      case ScrollCode of
        SB_LINEUP: SetPosition(FPosition - FIncrement);
        SB_LINEDOWN: SetPosition(FPosition + FIncrement);
        SB_PAGEUP: SetPosition(FPosition - ControlSize(True, False));
        SB_PAGEDOWN: SetPosition(FPosition + ControlSize(True, False));
        SB_THUMBPOSITION:
            if FCalcRange > 32767 then
              SetPosition(GetRealScrollPosition) else
              SetPosition(Pos);
        SB_THUMBTRACK:
          if Tracking then
            if FCalcRange > 32767 then
              SetPosition(GetRealScrollPosition) else
              SetPosition(Pos);
        SB_TOP: SetPosition(0);
        SB_BOTTOM: SetPosition(FCalcRange);
        SB_ENDSCROLL: begin end;
      end;
  end;
end;

procedure TControlScrollBar.SetPosition(Value: Integer);
var
  Code: Word;
  Form: TCustomForm;
  OldPos: Integer;
begin
  if csReading in FControl.ComponentState then
    FPosition := Value
  else
  begin
    if Value > FCalcRange then Value := FCalcRange
    else if Value < 0 then Value := 0;
    if Kind = sbHorizontal then
      Code := SB_HORZ else
      Code := SB_VERT;
    if Value <> FPosition then
    begin
      OldPos := FPosition;
      FPosition := Value;
      if Kind = sbHorizontal then
        FControl.ScrollBy(OldPos - Value, 0) else
        FControl.ScrollBy(0, OldPos - Value);
      if csDesigning in FControl.ComponentState then
      begin
        Form := GetParentForm(FControl);
        if (Form <> nil) and (Form.Designer <> nil) then Form.Designer.Modified;
      end;
    end;
    if FlatSB_GetScrollPos(FControl.Handle, Code) <> FPosition then
      FlatSB_SetScrollPos(FControl.Handle, Code, FPosition, True);
  end;
end;

procedure TControlScrollBar.SetSize(Value: Integer);
const
  SysConsts: array[TScrollBarKind] of Integer = (SM_CYHSCROLL, SM_CYVSCROLL);
var
  NewValue: Integer;
begin
  if Value <> Size then
  begin
    NewValue := Value;
    if NewValue = 0 then
      Value := GetSystemMetrics(SysConsts[Kind]);
    FSize := Value;
    FUpdateNeeded := True;
    FControl.UpdateScrollBars;
    if NewValue = 0 then
      FSize := 0;
  end;
end;

procedure TControlScrollBar.SetStyle(Value: TScrollBarStyle);
begin
  if Style <> Value then
  begin
    FStyle := Value;
    FUpdateNeeded := True;
    FControl.UpdateScrollBars;
  end;
end;

procedure TControlScrollBar.SetThumbSize(Value: Integer);
begin
  if Value <> ThumbSize then
  begin
    FThumbSize := Value;
    FUpdateNeeded := True;
    FControl.UpdateScrollBars;
  end;
end;

procedure TControlScrollBar.DoSetRange(Value: Integer);
begin
  FRange := Value;
  if FRange < 0 then FRange := 0;
  FControl.UpdateScrollBars;
end;

procedure TControlScrollBar.SetRange(Value: Integer);
begin
  FControl.FAutoScroll := False;
  FScaled := True;
  DoSetRange(Value);
end;

function TControlScrollBar.IsRangeStored: Boolean;
begin
  Result := not FControl.AutoScroll;
end;


procedure TControlScrollBar.Update(ControlSB, AssumeSB: Boolean);
type
  TPropKind = (pkStyle, pkButtonSize, pkThumbSize, pkSize, pkBkColor);
const
  Props: array[TScrollBarKind, TPropKind] of Integer = (
    { Horizontal }
    (WSB_PROP_HSTYLE, WSB_PROP_CXHSCROLL, WSB_PROP_CXHTHUMB, WSB_PROP_CYHSCROLL,
     WSB_PROP_HBKGCOLOR),
    { Vertical }
    (WSB_PROP_VSTYLE, WSB_PROP_CYVSCROLL, WSB_PROP_CYVTHUMB, WSB_PROP_CXVSCROLL,
     WSB_PROP_VBKGCOLOR));
  Kinds: array[TScrollBarKind] of Integer = (WSB_PROP_HSTYLE, WSB_PROP_VSTYLE);
  Styles: array[TScrollBarStyle] of Integer = (FSB_REGULAR_MODE,
    FSB_ENCARTA_MODE, FSB_FLAT_MODE);
var
  Code: Word;
  ScrollInfo: TScrollInfo;

  procedure UpdateScrollProperties(Redraw: Boolean);
  begin
    FlatSB_SetScrollProp(FControl.Handle, Props[Kind, pkStyle], Styles[Style], Redraw);
    if ButtonSize > 0 then
      FlatSB_SetScrollProp(FControl.Handle, Props[Kind, pkButtonSize], ButtonSize, False);
    if ThumbSize > 0 then
      FlatSB_SetScrollProp(FControl.Handle, Props[Kind, pkThumbSize], ThumbSize, False);
    if Size > 0 then
      FlatSB_SetScrollProp(FControl.Handle, Props[Kind, pkSize], Size, False);
    FlatSB_SetScrollProp(FControl.Handle, Props[Kind, pkBkColor],
      ColorToRGB(Color), False);
  end;

begin
  FCalcRange := 0;
  Code := SB_HORZ;
  if Kind = sbVertical then Code := SB_VERT;
  if Visible then
  begin
    FCalcRange := Range - ControlSize(ControlSB, AssumeSB);
    if FCalcRange < 0 then FCalcRange := 0;
  end;
  ScrollInfo.cbSize := SizeOf(ScrollInfo);
  ScrollInfo.fMask := SIF_ALL;
  ScrollInfo.nMin := 0;
  if FCalcRange > 0 then
    ScrollInfo.nMax := Range else
    ScrollInfo.nMax := 0;
  ScrollInfo.nPage := ControlSize(ControlSB, AssumeSB) + 1;
  ScrollInfo.nPos := FPosition;
  ScrollInfo.nTrackPos := FPosition;
  UpdateScrollProperties(FUpdateNeeded);
  FUpdateNeeded := False;
  FlatSB_SetScrollInfo(FControl.Handle, Code, ScrollInfo, True);
  SetPosition(FPosition);
  FPageIncrement := (ControlSize(True, False) * 9) div 10;
  if Smooth then FIncrement := FPageIncrement div 10;
end;
*/
/*
TScrollBar :: TScrollBar()
{
    Kind = sbVertical;
    m_CurrentPosition = spNone;
    m_BackGroundColor = clAqua; // clBtnFace;
    m_ArrowBackGroundColor = clRed; // clBtnFace;
    m_ArrowColor = clGreen; // clBtnFace;
    m_ClickedColor = clYellow;
    m_ScrollButtonColor = clFuchsia;
    m_Size = 100;
    m_Clicked = false;
    m_Position = 1;
    FTimer.Enabled = false;
    FTimer.OnTimer = TNotifyEvent(this,&TScrollBar :: OnTimer);
    FTimer.Interval = 100;
}

void TScrollBar :: MouseExit(TShiftState state, int x, int y)
{
    TControl :: MouseExit(state, x, y);
    CurrentPosition = spNone;
}

uint_t TScrollBar :: get_Delay()
{
    return FTimer.Interval;
}

word_t TScrollBar :: get_ScrollButtonSize()
{
    word_t V, ret = 3;

    switch(m_Kind)
    {
        case sbHorizontal:
            V = Height;
            if( Height >= Width / 5)
            {
                V = Width / 7;
                if( V <= 10 )
                    V = 10;
            };
            ret = (Width - 2 * V - 6) / m_Size;
            break;
        case sbVertical:
            V = Width;
            if( Width >= Height / 5 )
            {
                V = Height / 7;
                if( V <= 10 )
                    V = 10;
            };
            ret = ( Height - 2 * V - 6 ) / m_Size;
            break;
    }
    if(ret < 3)
        ret = 3;

    return ret;
}

void TScrollBar :: set_CurrentPosition(TPosition value)
{
    word_t V;
    TRect rect;

    if(value == m_CurrentPosition)
        return;
  //Зарисовать
    switch(m_Kind)
    {
        case sbHorizontal:
            V = Height;
            if( Height >= Width / 5 )
            {
                V = Width / 7;
                if( V <= 10 )
                    V = 10;
            };
            switch(m_CurrentPosition)
            {
                case spLeftUp:
                    m_Canvas->Brush->Color = m_ArrowBackGroundColor;
                    m_Canvas->Brush->Style = bsSolid;
//                    m_Canvas->Pen->Style = psClear;
                    rect.bounds( 1, 1, V + 1, Height );
                    m_Canvas->Rectangle( rect );
                    m_Canvas->FillRect( rect );
                    break;//spLeftUp
                case spRightDown:
                    m_Canvas->Brush->Color = m_ArrowBackGroundColor;
                    m_Canvas->Brush->Style = bsSolid;
//                    m_Canvas->Pen->Style = psClear;
                    rect.bounds( Width - V + 1, 1, Width, Height );
                    m_Canvas->Rectangle( rect );
                    m_Canvas->FillRect( rect );
                    break;
                case spCenter:
                case spNone: break;
            };//case
            break;//sbHorizontal
        case sbVertical:
            V = Width;
            if( Width >= Height / 5 )
            {
                V = Height / 7;
                if( V <= 10 )
                    V = 10;
            };
            switch( m_CurrentPosition )
            {
                case spLeftUp:
                    m_Canvas->Brush->Color = m_ArrowBackGroundColor;
//                    m_Canvas->Pen->Style = psClear;
                    m_Canvas->Brush->Style = bsSolid;
                    rect.bounds( 1, 1, Width, V + 1 );
                    m_Canvas->Rectangle( rect );
                    m_Canvas->FillRect( rect );
                    break;
                case spRightDown:
                    m_Canvas->Brush->Color = m_ArrowBackGroundColor;
//                    m_Canvas->Pen->Style = psClear;
                    m_Canvas->Brush->Style = bsSolid;
                    rect.bounds( 1, Height - V + 1, Width, Height );
                    m_Canvas->Rectangle( rect );
                    m_Canvas->FillRect( rect );
                    break;
                case spCenter:
                case spNone: break;
            };
            break;//sbVertical
    };
  //Нарисовать
    switch( m_Kind )
    {
        case sbHorizontal:
        //------------------------------Рассчет позиции
            V = Height;
            if( Height >= Width / 5 )
            {
                V = Width / 7;
                if( V <= 10 )
                    V = 10;
            };
        //------------------------------
            switch( value )
            {
                case spRightDown:
                    if( m_Clicked )
                        m_Canvas->Brush->Color = m_ClickedColor;
                    else
                        m_Canvas->Brush->Color = m_ArrowColor;
                    m_Canvas->Brush->Style = bsSolid;
//                    m_Canvas->Pen->Style = psClear;
                    rect.bounds( Width - V + 1, 1, Width, Height );
                    m_Canvas->Rectangle( rect );
                    m_Canvas->FillRect( rect );
                    break;
                case spLeftUp:
                    if( m_Clicked )
                        m_Canvas->Brush->Color = m_ClickedColor;
                    else
                        m_Canvas->Brush->Color = m_ArrowColor;
                    m_Canvas->Brush->Style = bsSolid;
//                    m_Canvas->Pen->Style = psClear;
                    rect.bounds( 1, 1, V + 1, Height );
                    m_Canvas->Rectangle( rect );
                    m_Canvas->FillRect( rect );
                    break;
                case spCenter:
                case spNone: break;
            }
            break;//sbHorizontal
        case sbVertical:
            V = Width;
            if( Width >= Height / 5 )
            {
                V = Height / 7;
                if( V <= 10 )
                    V = 10;
            };
            switch( value )
            {
                case spLeftUp:
                    if( m_Clicked )
                        m_Canvas->Brush->Color = m_ClickedColor;
                    else
                        m_Canvas->Brush->Color = m_ArrowColor;
//                    m_Canvas->Pen->Style = psClear;
                    m_Canvas->Brush->Style = bsSolid;
                    rect.bounds( 1, 1, Width, V + 1 );
                    m_Canvas->Rectangle( rect );
                    m_Canvas->FillRect( rect );
                    break;
                case spRightDown:
                    if( m_Clicked )
                        m_Canvas->Brush->Color = m_ClickedColor;
                    else
                        m_Canvas->Brush->Color = m_ArrowColor;
//                    m_Canvas->Pen->Style = psClear;
                    m_Canvas->Brush->Style = bsSolid;
                    rect.bounds( 1, Height - V + 1, Width, Height );
                    m_Canvas->Rectangle( rect );
                    m_Canvas->FillRect( rect );
                    break;
                case spCenter:
                case spNone: break;
            };
            break;//sbVertical
    }
    m_CurrentPosition = value;
  //Invalidate;
    Update();
};

void TScrollBar :: OnTimer( TObject *Sender )
{
    switch( m_CurrentPosition )
    {
        case spLeftUp:
            if( m_Position - 1 >= 0 )
                Position = m_Position - 1;
            break;
        case spRightDown:
            if( m_Position + 1 <= m_Size )
                Position = m_Position + 1;
        case spCenter:
        case spNone: break;
    }
}

void TScrollBar :: Paint()
{
    word_t V;
    TRect rect;
    TRect SB;

    m_Canvas->Brush->Color = m_BackGroundColor;
    m_Canvas->Pen->Style = psSolid;
//    m_Canvas->Rectangle( 0, 0, m_Width, m_Height);
    m_Canvas->FillRect( 0, 0, Width, Height);
    switch( m_Kind )
    {
        case sbHorizontal:
            V = Height;
            if( Height >= Width / 5 )
            {
                V = Width / 7;
                if( V <= 10 )
                    V = 10;
            };
        //Левая кнопка
            m_Canvas->Brush->Color = m_ArrowBackGroundColor;
            if( m_CurrentPosition == spLeftUp )
                m_Canvas->Brush->Color = m_ArrowColor;
            if( m_CurrentPosition == spLeftUp && m_Clicked )
                m_Canvas->Brush->Color = m_ClickedColor;
            m_Canvas->MoveTo( V, 0);
            m_Canvas->LineTo( V, Height);
//            m_Canvas->Pen->Style = psClear;
            rect.bounds( 1, 1, V + 1, Height );
            m_Canvas->Rectangle( rect );
            m_Canvas->FillRect( rect );
//            m_Canvas->Pen->Style = psSolid;
        //Правая кнопка
            m_Canvas->Brush->Color = m_ArrowBackGroundColor;
            if( m_CurrentPosition == spRightDown )
                m_Canvas->Brush->Color = m_ArrowColor;
            if( m_CurrentPosition == spRightDown && m_Clicked )
                m_Canvas->Brush->Color = m_ClickedColor;
            m_Canvas->MoveTo( Width - V, 0);
            m_Canvas->LineTo( Width - V, Height);
//            m_Canvas->Pen->Style = psClear;
            rect.bounds( Width - V + 1, 1, Width, Height );
            m_Canvas->Rectangle( rect );
            m_Canvas->FillRect( rect );
//            m_Canvas->Pen->Style = psSolid;
        //Прорисовка указателя
            m_Canvas->Brush->Color = m_ScrollButtonColor;
            m_Canvas->Brush->Style = bsSolid;
            if( m_Position < m_Size )
            {
                SB.bounds(
                    ( V + 3 ) + (( Width - 2 * V ) / m_Size ) * ( m_Position - 1 ),
                    2,
                    ( V + 3 ) + (( Width - 2 * V ) / m_Size ) * ( m_Position - 1 ) + ScrollButtonSize,
                    Height - 2 );
//                m_Canvas->Rectangle(SB);
                m_Canvas->FillRect(SB);
            }
            else
            {
                rect.bounds( Width - V - ScrollButtonSize - 3, 2, Width - V - 3, Height - 2 );
//                m_Canvas->Rectangle( rect );
                m_Canvas->FillRect( rect );
            }
            break;//scHorizontal */
/*        case sbVertical:
            V = m_Width;
            if( m_Width >= m_Height / 5 )
            {
                V = m_Height / 7;
                if( V <= 10 )
                    V = 10;
            };
        //Левая кнопка
            m_Canvas->Brush->Color = m_ArrowBackGroundColor;
            if( m_CurrentPosition == spLeftUp )
                m_Canvas->Brush->Color = m_ArrowColor;
            if( m_CurrentPosition == spLeftUp && m_Clicked )
                m_Canvas->Brush->Color = m_ClickedColor;
            m_Canvas->MoveTo( 0, V );
            m_Canvas->LineTo( m_Width, V );
//            m_Canvas->Pen->Style = psClear;
            rect.bounds( 1, 1, m_Width, V + 1 );
            m_Canvas->Rectangle( rect );
            m_Canvas->FillRect( rect );
            m_Canvas->Pen->Style = psSolid;
        //Правая кнопка
            m_Canvas->Brush->Color = m_ArrowBackGroundColor;
            if( m_CurrentPosition == spRightDown )
                m_Canvas->Brush->Color = m_ArrowColor;
            if( m_CurrentPosition == spRightDown && m_Clicked )
                m_Canvas->Brush->Color = m_ClickedColor;
            m_Canvas->MoveTo( 0, m_Height - V );
            m_Canvas->LineTo( m_Width, m_Height - V );
//            m_Canvas->Pen->Style = psClear;
            rect.bounds( 1, m_Height - V + 1, m_Width, m_Height );
            m_Canvas->Rectangle( rect );
            m_Canvas->FillRect( rect );
            m_Canvas->Pen->Style = psSolid;
        //Прорисовка указателя
            m_Canvas->Brush->Color = m_ScrollButtonColor;
            m_Canvas->Brush->Style = bsSolid;
            if( m_Position < m_Size )
            {
                SB.bounds(
                    2,
                    ( V + 3 ) + (( m_Height - 2 * V ) / m_Size ) * ( m_Position - 1 ),
                    m_Width - 2,
                    SB.top + ScrollButtonSize );
                m_Canvas->Rectangle(SB);
                m_Canvas->FillRect(SB);
            }
            else
            {
                rect.bounds( 2, m_Height - V - ScrollButtonSize - 3, m_Width - 2, m_Height - V - 3 );
                m_Canvas->Rectangle( rect );
                m_Canvas->FillRect( rect );
            }
            break; */ /*
    }
}

void TScrollBar :: set_ArrowBackGroundColor(TColor value)
{
    if(m_ArrowBackGroundColor == value)
        return;
    m_ArrowBackGroundColor = value;
//  Invalidate;
    Update();
};

void TScrollBar :: set_ArrowColor(TColor Value)
{
    if(Value == m_ArrowColor)
        return;
    m_ArrowColor = Value;
//  Invalidate;
    Update();
};

void TScrollBar :: set_BackGroundColor(TColor Value)
{
    if(m_BackGroundColor == Value)
        return;
    m_BackGroundColor = Value;
//  Invalidate;
    Update();
};

void TScrollBar :: set_MyClicked(bool Value)
{
    TPosition temp;

    if(m_Clicked == Value)
        return;
    m_Clicked = Value;
    temp = CurrentPosition;
    CurrentPosition = spNone;
    CurrentPosition = temp;
};

void TScrollBar :: set_ClickedColor(TColor Value)
{
    if(m_ClickedColor == Value)
        return;
    m_ClickedColor = Value;
//  Invalidate;
    Update();
};

void TScrollBar :: set_Delay(uint_t Value)
{
    if(Value == 0)
        Value = 75;
    FTimer.Interval = Value;
};

void TScrollBar :: set_Position(word_t Value)
{
    if(m_Position == Value)
        return;
    if(Value == 0)
        Value = 1;
    if(Value > m_Size)
        Value = m_Size;
    m_Position = Value;
//  Invalidate;
    Update();
};

void TScrollBar :: set_Kind(TScrollBarKind Value)
{
    if(m_Kind == Value)
        return;
    m_Kind = Value;
    CurrentPosition = CurrentPosition;
};

void TScrollBar :: set_ScrollButtonColor(TColor Value)
{
    if(m_ScrollButtonColor == Value)
        return;
    m_ScrollButtonColor = Value;
//    Invalidate;
    Update();
};

void TScrollBar :: set_Size(word_t Value)
{
    if(Value == m_Size)
        return;
    if(Value == 0)
        Value = 1;
    m_Size = Value;
//    Invalidate;
    Update();
};

void TScrollBar :: MouseDown(TMouseButton button, TShiftState state, int x, int y)
{
  TControl :: MouseDown(button, state, x, y);
  MyClicked = true;//рисует нажатую кнопку
  switch( m_CurrentPosition )
  {
        case spLeftUp:
            if( m_Position != 1 )
                FTimer.Enabled = true;
            break;
        case spRightDown:
            if( m_Position != m_Size )
                FTimer.Enabled = true;
            break;
        case spCenter:
        case spNone: break;
  };
}

void TScrollBar :: MouseUp(TMouseButton button, TShiftState state, int x, int y)
{
    TControl :: MouseUp( button, state, x, y );
    MyClicked = false;
    FTimer.Enabled = false;
} */
/*
procedure TMyScrollBar.WMNCHitTest(var Message: TWMNCHitTest);
var pos:TPoint;
    V:word;
begin
  inherited;
  pos:=ScreenToClient(Point(Message.XPos,Message.YPos));
  case m_Kind of
    sbHorizontal:
      begin
        V:=Height;
        if Height>=Width div 5 then
          begin
            V:=Width div 7;
            if V<=10 then V:=10;
          end;
        if(pos.X>0)and(pos.X<V) then//spLeftUp
            CurrentPosition:=spLeftUp;
        if (pos.X>Width-V)and(pos.X<Width) then//spRightDown
            CurrentPosition:=spRightDown;
        if (Pos.X>V)and(pos.X<Width-V) then//spCenter
          CurrentPosition:=spCenter;
      end;
    sbVertical:
      begin
        V:=Width;
        if Width>=Height div 5 then
          begin
            V:=Height div 7;
            if V<=10 then V:=10;
          end;
        if(pos.Y>0)and(pos.Y<V) then//spLeftUp
          CurrentPosition:=spLeftUp;
        if(pos.Y>V)and(pos.Y<Height-V) then//spCenter
          CurrentPosition:=spCenter;
        if(pos.Y>Height-V+1)and(pos.Y<Height) then//spRightDown
          CurrentPosition:=spRightDown;
      end;
  end;//case
end;
*/
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
