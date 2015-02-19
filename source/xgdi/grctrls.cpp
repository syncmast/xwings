//---------------------------------------------------------------------------
#include "grctrls.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
//  TPaintBox
//---------------------------------------------------------------------------
TPaintBox :: TPaintBox()
{
    Init();
}
//---------------------------------------------------------------------------
TPaintBox :: ~TPaintBox()
{
}
//---------------------------------------------------------------------------
__fastcall TPaintBox :: TPaintBox(const TPaintBox &a) : TControl(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TPaintBox & TPaintBox :: operator=(const TPaintBox &a)
{
    if(this == &a)
        return *this;
    TControl :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TPaintBox :: Init()
{
}
//---------------------------------------------------------------------------
void __fastcall TPaintBox :: Assign(const TPaintBox &obj)
{
}
//---------------------------------------------------------------------------
// TImage
//---------------------------------------------------------------------------
TImage :: TImage()
{
    Init();
}
//---------------------------------------------------------------------------
TImage :: ~TImage()
{
}
//---------------------------------------------------------------------------
__fastcall TImage :: TImage(const TImage &a) : TControl(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TImage & TImage :: operator=(const TImage &a)
{
    if(this == &a)
        return *this;
    TControl :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TImage :: Init()
{
    m_Stretch = false;
    m_Proportional = false;
    m_Center = false;
    Height = 105;
    Width = 105;
    m_Picture = NULL;
    FDrawing = false;
}
//---------------------------------------------------------------------------
void __fastcall TImage :: Assign(const TImage &obj)
{
}
//---------------------------------------------------------------------------
TRect TImage :: DestRect()
{
    int w, h, cw, ch, w1, h1;
    TRect ret;

    w = m_Picture->Width;
    h = m_Picture->Height;
    w1 = w;
    h1 = h;
    cw = (*ClientRect).width;
    ch = (*ClientRect).height;
    if(m_Stretch || (m_Proportional && ((w > cw) || (h > ch))))
    {
        if(m_Proportional && (w > 0) && (h > 0))
        {
            if(w > h)
            {
                w = cw;
                h = cw * h1 / w1;
                if(h > ch)  // woops, too big
                {
                    h = ch;
                    w = ch * w1 / h1;
                }
            }
            else
            {
                h = ch;
                w = ch * w1 / h1;
                if(w > cw)  // woops, too big
                {
                    w = cw;
                    h = cw * h1 / w1;
                }
            }
        }
        else
        {
            w = cw;
            h = ch;
        }
    }
//	ret = TRect(Left, Top, w, h);
    ret = TRect(0, 0, w, h);

    if(m_Center)
        ret.offset((cw - w) / 2, (ch - h) / 2);

    return ret;
}
//---------------------------------------------------------------------------
void TImage :: Paint()
{
    if(m_Picture == NULL)
        return;
    bool Save;
    TRect rect = DestRect();

    Save = FDrawing;
    FDrawing = true;
    Canvas.StretchDraw(rect, *m_Picture);
    FDrawing = Save;
}
//---------------------------------------------------------------------------
void TImage :: Progress(TObject *Sender, TProgressStage Stage, byte_t PercentDone, bool RedrawNow,
                        const TRect &R, const TString &Msg)
{
    if(IncrementalDisplay && RedrawNow)
    {
        Update();
    }
    if(OnProgress)
        OnProgress(Sender, Stage, PercentDone, RedrawNow, R, Msg);
}
//---------------------------------------------------------------------------
void TImage :: set_Center(bool value)
{
    if(m_Center != value)
    {
        m_Center = value;
        PictureChanged(this);
    }
}
//---------------------------------------------------------------------------
void TImage :: set_Picture(TGraphic *value)
{
    m_Picture = value;
//	m_Picture->OnChange = PictureChanged;
//	m_Picture->OnProgress = Progress;
}
//---------------------------------------------------------------------------
void TImage :: set_Stretch(bool value)
{
    if(value != m_Stretch)
    {
        m_Stretch = value;
        PictureChanged(this);
    }
}
//---------------------------------------------------------------------------
void TImage :: set_Transparent(bool value)
{
    if(value != Transparent)
    {
        Transparent = value;
        PictureChanged(this);
    }
}
//---------------------------------------------------------------------------
void TImage :: set_Proportional(bool value)
{
    if(m_Proportional != value)
    {
        m_Proportional = value;
        PictureChanged(this);
    }
}
//---------------------------------------------------------------------------
void TImage :: PictureChanged(TObject *Sender)
{
    if(AutoSize && (m_Picture->Width > 0) && (m_Picture->Height > 0))
        BoundsRect = TRect(Left, Top, m_Picture->Width, m_Picture->Height);
    if(!FDrawing)
        Update();
}
//---------------------------------------------------------------------------
//  TAnimate
//---------------------------------------------------------------------------
TAnimate :: TAnimate()
{
    Init();
}
//---------------------------------------------------------------------------
TAnimate :: ~TAnimate()
{
}
//---------------------------------------------------------------------------
__fastcall TAnimate :: TAnimate(const TAnimate &a) : TImage(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TAnimate & TAnimate :: operator=(const TAnimate &a)
{
    if(this == &a)
        return *this;
    TImage :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TAnimate :: Init()
{
    Sequence = 25;
    m_CurrentFrame = 0;
    m_StartFrame = 0;
    m_StopFrame = 0;
    m_Repeats = 0;
    m_timer.OnTimer = TNotifyEvent(this, &TAnimate :: DoChange);
}
//---------------------------------------------------------------------------
void __fastcall TAnimate :: Assign(const TAnimate &obj)
{
}
//---------------------------------------------------------------------------
void TAnimate :: set_StartFrame(short value)
{
    if(m_StartFrame != value)
    {
        m_StartFrame = value;
        Stop();
        m_CurrentFrame = value;
    }
}
//---------------------------------------------------------------------------
void TAnimate :: set_StopFrame(short value)
{
    if(m_StopFrame != value)
    {
        m_StopFrame = value;
        Stop();
    }
}
//---------------------------------------------------------------------------
void TAnimate ::  set_Repeats(short value)
{
    if( m_Repeats != value )
    {
        m_Repeats = value;
        Stop();
    }
}
//---------------------------------------------------------------------------
short TAnimate :: get_FrameCount()
{
    short ret = 0;
    return ret;
}
//---------------------------------------------------------------------------
void TAnimate :: set_Active(bool value)
{
//    if FActive <> Value then
//    {
    if(value)
        Play(m_StartFrame, m_StopFrame, m_Repeats);
    else
        Stop();
//    }
}
//---------------------------------------------------------------------------
void TAnimate :: DoChange(TObject *Sender)
{
    TImageList *lst = dynamic_cast<TImageList *>(*Picture);

//    TRect rect = TRect(0, 0, w, h);
//    lst->DrawRect(bmp.Canvas, rect, 0, 0, m_CurrentFrame, alClient);

    m_CurrentFrame++;
    if(m_CurrentFrame == m_StopFrame)
        m_CurrentFrame = m_StartFrame;
}
//---------------------------------------------------------------------------
void __fastcall TAnimate :: Play(int fromframe, int toframe, int repeats)
{
    m_timer.Enabled = false;
//	if(m_List.Count > 1)
//	{
    m_StartFrame = fromframe;
    m_StopFrame = toframe;
    m_Repeats = repeats;
    m_CurrentFrame = m_StartFrame;
    m_timer.Interval = 1000 / Sequence;
    m_timer.Enabled = true;
//	}
    /*
    procedure TAnimate.Play(FromFrame, ToFrame: Word; Count: Integer);
    begin
      HandleNeeded;
      CheckOpen;
      FActive := True;
      { ACM_PLAY excpects -1 for repeated animations }
      if Count = 0 then Count := -1;
      if Perform(ACM_PLAY, Count, MakeLong(FromFrame - 1, ToFrame - 1)) <> 1 then
        FActive := False;
    end;
    */
}
//---------------------------------------------------------------------------
void TAnimate :: Stop()
{
    m_timer.Enabled = false;
    m_CurrentFrame = m_StartFrame;
    Active = false;
    m_StopCount++;
    if(OnStop)
        OnStop(this);
}
//---------------------------------------------------------------------------

//		XFilters *flt = XRenderQueryFilters(Application.Handle, src->Handle);
//		for(int i = 0; i < flt->nfilter; i++)
//			printf("FILTER: %s\n", flt->filter[i]);
//		XFree(flt);
//		hpic = XRenderCreatePicture(Application.Handle, hsrc, pictformat, 0, NULL);
//		XRenderChangePicture(Application.Handle, src->Canvas->Picture, valuemask, &pa);

//		XTransform transform = {{{ 1, 0, 0 },
//								 {0, 1, 0 },
//								 {0, 0, 2}}};
//			printf("CURSOR: %d\n", XDoubleToFixed(1.));
//		XRenderSetPictureTransform (Application.Handle, pic, &transform);

//        HBITMAP htmp = src->MaskHandle;
//		src->Canvas->SetClipMask(hmsk);

//		c = XRenderCreateCursor(Application.Handle, src->Canvas->Picture, hot_x, hot_y);
//---------------------------------------------------------------------------
/*TTimer __cursor_timer;
//---------------------------------------------------------------------------
__fastcall TCustomCursor :: TCustomCursor(TImageList *lst, int hot_x, int hot_y)
{
	HCURSOR c;
	TBitmap bmp;
	uint_t w, h;
	TPixelFormat pf = lst->PixelFormat;
	XQueryBestCursor(Application.Handle, CScreen.Root, lst->ImageWidth, lst->ImageHeight, &w, &h);
	bmp.Create(w, h, &pf, lst->HandleType);
	if(pf.Depth == pf1bit)
	{
		XColor fg, bg;
		fg.pixel = clCursorForeground;
		bg.pixel = clCursorBackground;
		XQueryColor(Application.Handle, CScreen.Colormap, &fg);
		XQueryColor(Application.Handle, CScreen.Colormap, &bg);
		for(register uint_t i = 0; i < lst->Count; i++)
		{
			lst->Draw(bmp.Canvas, 0, 0, i);
			c = XCreatePixmapCursor(Application.Handle, bmp.Handle, bmp.MaskHandle,
									&fg, &bg, hot_x, hot_y);
			m_List.Add(c);
		}
	}
	else
	{
		int	ncursor = lst->Count;
		XAnimCursor	*cur;
		XAnimCursor	*cursors = new XAnimCursor[ncursor];
		TRect rect = TRect(0, 0, w, h);
		for(int i = 0; i < ncursor; i++)
		{
			cur = &cursors[i];
			lst->DrawRect(bmp.Canvas, rect, 0, 0, i, alClient);
			bmp.Canvas->SetClipMask(bmp.MaskHandle);
			cur->cursor = XRenderCreateCursor(Application.Handle, bmp.Canvas->Picture, hot_x, hot_y);
			cur->delay = 500;
		}
		c = XRenderCreateAnimCursor(Application.Handle, ncursor, cursors);
		m_List.Add(c);
		delete [] cursors;
	}
}
//---------------------------------------------------------------------------
{ TAnimate }

constructor TAnimate.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  ControlStyle := ControlStyle + [csReflector];
  Width := 100;
  Height := 80;
  AutoSize := True;
  FCenter := True;
  FStartFrame := 1;
  FTransparent := True;
end;

procedure TAnimate.CreateWnd;
begin
  FRecreateNeeded := False;
  FOpen := False;
  inherited CreateWnd;
  UpdateActiveState;
end;

procedure TAnimate.DestroyWnd;
var
  OldActive, OldOpen: Boolean;
begin
  OldActive := FActive;
  OldOpen := FOpen;
  SetOpen(False);
  inherited DestroyWnd;
  FOpen := OldOpen;
  FActive := OldActive;
end;

procedure TAnimate.WMNCCalcSize(var Message: TWMNCCalcSize);
begin
  if csDesigning in ComponentState then
    with Message.CalcSize_Params^ do
      InflateRect(rgrc[0], -1, -1);
  inherited;
end;

procedure TAnimate.WMNCHitTest(var Message: TWMNCHitTest);
begin
  with Message do
    if not (csDesigning in ComponentState) then
      Result := HTCLIENT
    else
      inherited;
end;

procedure TAnimate.WMWindowPosChanged(var Message: TWMWindowPosChanged);
var
  R: TRect;
begin
  inherited;
  InvalidateRect(Handle, nil, True);
  R := Rect(0, 0, FrameWidth, FrameHeight);
  if Center then
    OffsetRect(R, (ClientWidth - (R.Right - R.Left)) div 2,
      (ClientHeight - (R.Bottom - R.Top)) div 2);
  ValidateRect(Handle, @R);
  UpdateWindow(Handle);
  InvalidateRect(Handle, @R, False);
end;

procedure TAnimate.DoOpen;
begin
  if Assigned(FOnOpen) then FOnOpen(Self);
end;

procedure TAnimate.DoClose;
begin
  if Assigned(FOnClose) then FOnClose(Self);
end;

procedure TAnimate.DoStart;
begin
  if Assigned(FOnStart) then FOnStart(Self);
end;

function TAnimate.GetActualResHandle: THandle;
begin
  if FCommonAVI <> aviNone then Result := GetShellModule
  else if FResHandle <> 0 then Result := FResHandle
  else if MainInstance <> 0 then Result := MainInstance
  else Result := HInstance;
end;

procedure TAnimate.GetFrameInfo;

  function CreateResStream: TStream;
  const
    ResType = 'AVI';
  var
    Instance: THandle;
  begin
    { AVI is from a file }
    if FFileName <> '' then
      Result := TFileStream.Create(FFileName, fmShareDenyNone)
    else
    begin
      { AVI is from a resource }
      Instance := GetActualResHandle;
      if FResName <> '' then
        Result := TResourceStream.Create(Instance, FResName, ResType)
      else Result := TResourceStream.CreateFromID(Instance, GetActualResId, ResType);
    end;
  end;

const
  CountOffset = 48;
  WidthOffset = 64;
  HeightOffset = 68;
begin
  with CreateResStream do
  try
    if Seek(CountOffset, soFromBeginning) = CountOffset then
      ReadBuffer(FFrameCount, SizeOf(FFrameCount));
    if Seek(WidthOffset, soFromBeginning) = WidthOffset then
      ReadBuffer(FFrameWidth, SizeOf(FFrameWidth));
    if Seek(HeightOffset, soFromBeginning) = HeightOffset then
      ReadBuffer(FFrameHeight, SizeOf(FFrameHeight));
  finally
    Free;
  end;
end;

procedure TAnimate.SetCenter(Value: Boolean);
begin
  if FCenter <> Value then
  begin
    FCenter := Value;
    RecreateWnd;
  end;
end;

procedure TAnimate.SetCommonAVI(Value: TCommonAVI);
begin
  if FCommonAVI <> Value then
  begin
    FRecreateNeeded := (FCommonAVI = aviNone) and
      (GetComCtlVersion < ComCtlVersionIE4);
    FCommonAVI := Value;
    FFileName := '';
    FResHandle := 0;
    FResName := '';
    FResId := 0;
    if Value = aviNone then SetOpen(False) else Reset;
  end;
end;

procedure TAnimate.SetFileName(Value: string);
var
  Save: TAnimateParams;
begin
  if AnsiCompareText(FFileName, Value) <> 0 then
  begin
    GetAnimateParams(Save);
    try
      FFileName := Value;
      FCommonAVI := aviNone;
      FResHandle := 0;
      FResName := '';
      FResId := 0;
      if FFileName = '' then SetOpen(False) else Reset;
    except
      SetAnimateParams(Save);
      raise;
    end;
  end;
end;

procedure TAnimate.SetOpen(Value: Boolean);
begin
  if (FOpen <> Value) then
    if Value then
    begin
      FOpen := InternalOpen;
      if AutoSize then AdjustSize;
    end
    else FOpen := InternalClose;
end;

procedure TAnimate.SetTimers(Value: Boolean);
begin
  if FTimers <> Value then
  begin
    FTimers := Value;
    RecreateWnd;
  end;
end;

procedure TAnimate.CheckOpen;
begin
  SetOpen(True);
  if not Open then raise Exception.CreateRes(@SCannotOpenAVI);
end;

function TAnimate.InternalOpen: Boolean;
var
  R: TRect;
begin
  if FRecreateNeeded then RecreateWnd;
  HandleNeeded;
  { Preserve dimensions to prevent auto sizing }
  if not Center then R := BoundsRect;
  Result := Perform(ACM_OPEN, GetActualResHandle, GetActualResId) <> 0;
  { Restore dimensions in case control was resized }
  if not Center then BoundsRect := R;
  if Result then
  begin
    GetFrameInfo;
    FStartFrame := 1;
    FStopFrame := FFrameCount;
    DoOpen;
  end;
end;

function TAnimate.InternalClose: Boolean;
begin
  if FActive then Stop;
  Result := SendMessage(Handle, ACM_OPEN, 0, 0) <> 0;
  DoClose;
  Invalidate;
end;

procedure TAnimate.Reset;
begin
  if not (csLoading in ComponentState) then
  begin
    SetOpen(False);
    Seek(1);
  end;
end;

procedure TAnimate.Seek(Frame: Smallint);
begin
  CheckOpen;
  SendMessage(Handle, ACM_PLAY, 1, MakeLong(Frame - 1, Frame - 1));
end;

*/
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------

