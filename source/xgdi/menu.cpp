//---------------------------------------------------------------------------
#include <stdio.h>
#include "menu.h"
#include "application.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
TShortCut ShortCut(word_t key, TShiftState shift)
{
    TShortCut ret = 0;

    ret |= BIT_OFF(shift, ssNumLock | ssCapsLock | ssScrollLock);
    ret |= (key << 8);
    return ret;
}
//---------------------------------------------------------------------------
void ShortCutToKey(TShortCut shortcut, word_t &key, TShiftState &shift)
{
    key = shortcut >> 8;
    shift = TShiftState(shortcut & 0x00FF);
}
//---------------------------------------------------------------------------
TString ShortCutToText(TShortCut shortcut)
{
    TString ret;
    word_t key;
    TShiftState shift;
    KeySym keysym, dummy;
    char *s;

    ShortCutToKey(shortcut, key, shift);
    if(shift & ssShift) ret =  "Shift+";
    if(shift & ssCtrl) 	ret += "Ctrl+";
    if(shift & ssAlt) 	ret += "Alt+";
    if(shift & ssWin) 	ret += "Win+";

    int keysyms_per_keycode_return;
    KeySym *pkeysym = XGetKeyboardMapping(Application.Handle, key, 1, &keysyms_per_keycode_return);
    keysym = pkeysym[0];
    XFree(pkeysym);

    XConvertCase(keysym, &dummy, &keysym);
    s = XKeysymToString(keysym);
    if(s)
        ret += s;
    return ret;
}
//---------------------------------------------------------------------------
TShortCut TextToShortCut(const TString &text)
{
    TShortCut ret = 0;
    int shift = 0;
    word_t key = 0;
    KeySym keysym;
    TStringList sl;
    sl.Delimiter = '+';
    sl.DelimitedText = text;
    for(register int i = 0; i < sl.Count; i++)
    {
        if(sl[i] == "Shift")
            shift |= ssShift;
        else if(sl[i] == "Ctrl")
            shift |= ssCtrl;
        else if(sl[i] == "Alt")
            shift |= ssAlt;
        else if(sl[i] == "Win")
            shift |= ssWin;
        else
        {
            keysym = XStringToKeysym(sl[i].c_str());
            key = XKeysymToKeycode(Application.Handle, keysym);
        }
    }
    ret = ShortCut(key, TShiftState(shift));
    return ret;
}
//---------------------------------------------------------------------------
//  TCommonMenuItem
//---------------------------------------------------------------------------
TCommonMenuItem :: TCommonMenuItem()
{
    Init();
}
//---------------------------------------------------------------------------
void TCommonMenuItem :: Init()
{
    StyleOn(csButton);
    StyleOn(csFollowMouse);
    m_ImageIndex = -1;
    ShortCut = 0;
    m_ParentItem = NULL;
    m_Menu = NULL;
    m_Submenu = NULL;
    AutoCheck = false;
}
//---------------------------------------------------------------------------
void TCommonMenuItem :: set_Caption(TString value)
{
    if("-" == value)
    {
        StyleOff(csFollowMouse);
        StyleOff(csButton);
    }
    else
    {
        StyleOn(csFollowMouse);
        StyleOn(csButton);
    }
    TControl :: set_Caption(value);
}
//---------------------------------------------------------------------------
void TCommonMenuItem :: set_ImageIndex(short value)
{
    if(m_ImageIndex != value)
    {
        m_ImageIndex = value;
        Invalidate();
    }
}
//---------------------------------------------------------------------------
void TCommonMenuItem :: set_Submenu(TCustomPopupMenu *value)
{
    if(m_Submenu != value)
    {
        m_Submenu = value;
    }
}
//---------------------------------------------------------------------------
void __fastcall TCommonMenuItem :: Add(const TCommonMenuItem &item)
{
    TCommonMenuItem *mi = &const_cast<TCommonMenuItem &>(item);
    m_Items.Add(mi);
//	mi->m_ParentItem = this;
//	m_Kind = miSubmenu;
}
//---------------------------------------------------------------------------
void __fastcall TCommonMenuItem :: Remove(const TCommonMenuItem &item)
{
    TCommonMenuItem *mi = &const_cast<TCommonMenuItem &>(item);
    m_Items.Delete(mi);
//	if(m_Items.Count == 0)
//            m_Kind = miNormal;
}
//---------------------------------------------------------------------------
void TCommonMenuItem :: MeasureItem(uint_t &width, uint_t &height)
{
    if(OnMeasureItem)
        OnMeasureItem(this, width, height);
}
//---------------------------------------------------------------------------
void TCommonMenuItem :: ShowSubMenu()
{
    TMenu *pm = dynamic_cast<TMenu *>(*Parent);
    if(pm)
    {
        if(m_Submenu == NULL && pm->SubMenuClass && Count)
        {
            m_Submenu = (TCustomPopupMenu *)TClass(pm->SubMenuClass).Create();
            if(m_Submenu == NULL)
                return;
            m_Submenu->Images = pm->Images;
            for(register int i = 0; i < Count; i++)
                m_Submenu->Add(*(TCommonMenuItem *)m_Items[i]);
        }
        TPoint p;
        TRect b = BoundsRect;
        if(pm->Toplevel)
        {
//           if(!pm->Active)
//               return;
            p.x = 0;
            p.y = b.height - 1;
        }
        else
        {
            p.x = b.left + b.width - 1;
            p.y = 0;
        }
        p = ClientToScreen(p);
        m_Submenu->Popup(p.x, p.y);
    }
}
//---------------------------------------------------------------------------
void TCommonMenuItem :: HideSubMenu()
{
    if(Count)
    {
        delete m_Submenu;
        m_Submenu = NULL;
    }
    else
        m_Submenu->Close();
}
//---------------------------------------------------------------------------
void TCommonMenuItem :: MouseExit(TShiftState state, int x, int y)
{
    if(m_Submenu)
    {
        TMenu *pm = dynamic_cast<TMenu *>(*Parent);
        if(pm->Toplevel)
        {
            if(y < (int)Height - 5)
                HideSubMenu();
        }
        else
        {
            if(x < (int)Width - 5)
                HideSubMenu();
        }
    }
    TControl :: MouseExit(state, x, y);
}
//---------------------------------------------------------------------------
void TCommonMenuItem :: MouseEnter(TShiftState state, int x, int y)
{
    if(Count || m_Submenu)
    {
        ShowSubMenu();
    }
    TControl :: MouseEnter(state, x, y);
}
//---------------------------------------------------------------------------
void TCommonMenuItem :: MouseDown(TMouseButton button, TShiftState state, int x, int y)
{
    if(m_Submenu)
    {
        if(m_Submenu->Showing)
            m_Submenu->Visible = false;
//			HideSubMenu();
        else
        {
//            TMenu *pm = dynamic_cast<TMenu *>(*Parent);
//            pm->Active = true;
            m_Submenu->Visible = true;
//			ShowSubMenu();
        }
    }
    TControl :: MouseDown(button, state, x, y);
}
//---------------------------------------------------------------------------
void TCommonMenuItem :: Click()
{
    /*	if(m_Kind == miLine)
    		return;
    	Active = !Active;
    	if(m_Kind == miSubmenu)
    	{
    		printf("SHOWING %d %d\n", m_Submenu->id, *m_Submenu->Showing);
    		if(m_Submenu->Showing)
    			m_Submenu->Close();
    		else
    			ShowSubMenu();
    		TControl :: Click();
    		return;
    	}
    	CScreen.ClosePopup();
    //    TCustomPopupMenu *w = dynamic_cast<TCustomPopupMenu*>(*Parent);
    //    if(w)
    //		w->CloseAll();
    */
    if(Enabled)
    {
        if(IsLine())
            return;
        if(!m_Submenu)
            Screen.ClosePopup();
        if(AutoCheck)
            Checked != Checked;
        TControl :: Click();
    }
}
//---------------------------------------------------------------------------
/*
void TCommonMenuItem :: set_ParentItem(TCommonMenuItem *AParent)
{
    if(m_ParentItem != AParent && AParent != this)
    {
        if (m_ParentItem != NULL)
            m_ParentItem->Remove(*this);
        m_ParentItem = AParent;
        if (m_ParentItem != NULL)
            m_ParentItem->Add(*this);
    }
}
//---------------------------------------------------------------------------
TMenu *TCommonMenuItem :: GetParentMenu()
{
	return dynamic_cast<TMenu *>(*Parent);
}
*/
TMenu *TCommonMenuItem :: GetParentMenu()
{
    TCommonMenuItem *item;

    item = this;
    while(item->m_ParentItem)
        item = item->m_ParentItem;
    return item->m_Menu;
}
//---------------------------------------------------------------------------
void TCommonMenuItem :: Clear()
{
    m_Items.Clear();
}
//---------------------------------------------------------------------------
void TCommonMenuItem :: Delete(int index)
{
    TCommonMenuItem *item;

    item = (TCommonMenuItem *)m_Items[index];
    m_Items.Delete(index);
    item->m_Parent = NULL;
//  Cur.FOnChange := nil;

//  MenuChanged(Count = 0);
}
//---------------------------------------------------------------------------
int TCommonMenuItem :: IndexOf(const TCommonMenuItem &item)
{
    TCommonMenuItem *mi = &const_cast<TCommonMenuItem &>(item);
    return m_Items.IndexOf(mi);
}
//---------------------------------------------------------------------------
void TCommonMenuItem :: Insert(int index, TCommonMenuItem &item)
{
    /*  if Item.FParent <> nil then Error(@SMenuReinserted);

      if (Index - 1 >= 0) and (Index - 1 < FItems.Count) then
        if Item.GroupIndex < TMenuItem(FItems[Index - 1]).GroupIndex then
          Item.GroupIndex := TMenuItem(FItems[Index - 1]).GroupIndex;
      VerifyGroupIndex(Index, Item.GroupIndex);
      FItems.Insert(Index, Item);
      Item.FParent := Self;
      Item.FOnChange := SubItemChanged;

      MenuChanged(Count = 1); */
}
//---------------------------------------------------------------------------
void TCommonMenuItem :: set_ShortCut(TShortCut value)
{
    if(m_ShortCut != value)
    {
        m_ShortCut = value;
        Invalidate();
    }
}
//---------------------------------------------------------------------------
short TCommonMenuItem :: get_MenuIndex()
{
    short ret = -1;
    if(m_ParentItem)
        ret = m_ParentItem->IndexOf(*this);
    return ret;
}
//---------------------------------------------------------------------------
void TCommonMenuItem :: set_MenuIndex(short value)
{
    TCommonMenuItem *parent;
    int count;

    if(m_ParentItem)
    {
        count = m_ParentItem->Count;
        if(value < 0)
            value = 0;
        if(value >= count)
            value = count - 1;
        if(value != MenuIndex)
        {
            parent = m_ParentItem;
            parent->Remove(*this);
            parent->Insert(value, *this);
        }
    }
}
/*

destructor TMenuItem.Destroy;
begin
  ShortCutItems.ClearItem(Self);
  if FParent <> nil then
  begin
    FParent.Remove(Self);
    FParent := nil;
  end;
  while Count > 0 do Items[0].Free;
  if FHandle <> 0 then
  begin
    MergeWith(nil);
    DestroyMenu(FHandle);
  end;
  FItems.Free;
  FreeAndNil(FActionLink);
  FreeAndNil(FImageChangeLink);
  if FCommand <> 0 then CommandPool[FCommand] := False;
  if Assigned(FBitmap) then FBitmap.Free;
  inherited Destroy;
end;

const
  Checks: array[Boolean] of DWORD = (MF_UNCHECKED, MF_CHECKED);
  Enables: array[Boolean] of DWORD = (MF_DISABLED or MF_GRAYED, MF_ENABLED);
  Breaks: array[TMenuBreak] of DWORD = (0, MF_MENUBREAK, MF_MENUBARBREAK);
  Separators: array[Boolean] of DWORD = (MF_STRING, MF_SEPARATOR);

procedure TMenuItem.AppendTo(Menu: HMENU; ARightToLeft: Boolean);
const
  IBreaks: array[TMenuBreak] of DWORD = (MFT_STRING, MFT_MENUBREAK, MFT_MENUBARBREAK);
  IChecks: array[Boolean] of DWORD = (MFS_UNCHECKED, MFS_CHECKED);
  IDefaults: array[Boolean] of DWORD = (0, MFS_DEFAULT);
  IEnables: array[Boolean] of DWORD = (MFS_DISABLED or MFS_GRAYED, MFS_ENABLED);
  IRadios: array[Boolean] of DWORD = (MFT_STRING, MFT_RADIOCHECK);
  ISeparators: array[Boolean] of DWORD = (MFT_STRING, MFT_SEPARATOR);
  IRTL: array[Boolean] of DWORD = (0, RightToLeftMenuFlag);
  IOwnerDraw: array[Boolean] of DWORD = (MFT_STRING, MFT_OWNERDRAW);
var
  MenuItemInfo: TMenuItemInfo;
  Caption: string;
  NewFlags: Integer;
  IsOwnerDraw: Boolean;
  ParentMenu: TMenu;
begin
  if FVisible then
  begin
    Caption := FCaption;
    if GetCount > 0 then
      MenuItemInfo.hSubMenu := GetHandle
    else if (FShortCut <> scNone) and ((Parent = nil) or
            (Parent.Parent <> nil) or not (Parent.Owner is TMainMenu)) then
      Caption := Caption + #9 + ShortCutToText(FShortCut);
    if Lo(GetVersion) >= 4 then
    begin
      MenuItemInfo.cbSize := 44; // Required for Windows 95
      MenuItemInfo.fMask := MIIM_CHECKMARKS or MIIM_DATA or MIIM_ID or
        MIIM_STATE or MIIM_SUBMENU or MIIM_TYPE;
      ParentMenu := GetParentMenu;
//      IsOwnerDraw := Assigned(ParentMenu) and ParentMenu.IsOwnerDraw or
      IsOwnerDraw := Assigned(ParentMenu) and
                     (ParentMenu.OwnerDraw or (GetImageList <> nil)) or
                     Assigned(FBitmap) and not FBitmap.Empty;
      MenuItemInfo.fType := IRadios[FRadioItem] or IBreaks[FBreak] or
        ISeparators[FCaption = cLineCaption] or IRTL[ARightToLeft] or
        IOwnerDraw[IsOwnerDraw];
      MenuItemInfo.fState := IChecks[FChecked] or IEnables[FEnabled]
        or IDefaults[FDefault];
      MenuItemInfo.wID := Command;
      MenuItemInfo.hSubMenu := 0;
      MenuItemInfo.hbmpChecked := 0;
      MenuItemInfo.hbmpUnchecked := 0;
      MenuItemInfo.dwTypeData := PChar(Caption);
      if GetCount > 0 then
        MenuItemInfo.hSubMenu := GetHandle;
      InsertMenuItem(Menu, DWORD(-1), True, MenuItemInfo);
    end
    else
    begin
      NewFlags := Breaks[FBreak] or Checks[FChecked] or Enables[FEnabled] or
        Separators[FCaption = cLineCaption] or MF_BYPOSITION;
      if GetCount > 0 then
        InsertMenu(Menu, DWORD(-1), MF_POPUP or NewFlags, GetHandle,
          PChar(FCaption))
      else
        InsertMenu(Menu, DWORD(-1), NewFlags, Command, PChar(Caption));
    end;
  end;
end;

procedure TMenuItem.PopulateMenu;
var
  MenuRightToLeft: Boolean;

  function AddIn(MenuItem: TMenuItem): Boolean;
  begin
    MenuItem.AppendTo(FHandle, MenuRightToLeft);
    Result := False;
  end;

begin
  if (FMenu <> nil) and
     (FMenu is TMainMenu) then
  begin
    InternalRethinkHotkeys(False);
    InternalRethinkLines(False);
  end;

  // all menu items use BiDiMode of their root menu
  MenuRightToLeft := (FMenu <> nil) and FMenu.IsRightToLeft;
  IterateMenus(@AddIn, FMerged, Self);
end;

procedure TMenuItem.MergeWith(Menu: TMenuItem);
begin
  if Assigned(Menu) and (csDestroying in Menu.ComponentState) then exit;
  if FMerged <> Menu then
  begin
    if FMerged <> nil then
      FMerged.FMergedWith := nil;
    FMerged := Menu;
    if FMerged <> nil then
    begin
      FMerged.FMergedWith := Self;
      FMerged.FreeNotification(Self);
    end;
    RebuildHandle;
  end;
end;

procedure TMenuItem.RebuildHandle;
var
  I: Integer;
  LRepopulate: Boolean;
begin
  if csDestroying in ComponentState then Exit;
  if csReading in ComponentState then
    FStreamedRebuild := True
  else
  begin
    if FMergedWith <> nil then
      FMergedWith.RebuildHandle
    else
    begin
      I := GetMenuItemCount(Handle);
      LRepopulate := I = 0;
      while I > 0 do
      begin
        if GetMenuState(Handle, I - 1, MF_BYPOSITION) and MF_BITMAP = 0 then
        begin
          RemoveMenu(Handle, I - 1, MF_BYPOSITION);
          LRepopulate := True;
        end;
        Dec(I);
      end;
      if LRepopulate then
      begin
        if (FParent = nil) and (FMenu is TMainMenu) and
           (GetMenuItemCount(Handle) = 0) then
        begin
          DestroyMenu(FHandle);
          FHandle := 0;
        end
        else
          PopulateMenu;
        MenuChanged(False);
      end;
    end;
  end;
end;


procedure TMenuItem.DrawItem(ACanvas: TCanvas; ARect: TRect; Selected: Boolean);
begin
  if Assigned(FOnDrawItem) then
    FOnDrawItem(Self, ACanvas, ARect, Selected);
end;

procedure TMenuItem.AdvancedDrawItem(ACanvas: TCanvas; ARect: TRect;
  State: TOwnerDrawState; TopLevel: Boolean);
const
  Alignments: array[TPopupAlignment] of Word = (DT_LEFT, DT_RIGHT, DT_CENTER);
  EdgeStyle: array[Boolean] of Longint = (BDR_RAISEDINNER, BDR_SUNKENOUTER);
var
  ImageList: TCustomImageList;
  ParentMenu: TMenu;
  Alignment: TPopupAlignment;
  DrawImage, DrawGlyph: Boolean;
  GlyphRect, SaveRect: TRect;
  DrawStyle: Longint;
  Glyph: TBitmap;
  OldBrushColor: TColor;
  Selected: Boolean;
  Win98Plus: Boolean;
  Win2K: Boolean;

  procedure NormalDraw;
  begin
    with ACanvas do
    begin
      //ImageList := GetImageList;
      if not Selected then FillRect(ARect);
      if ParentMenu is TMenu then
        Alignment := paLeft
      else if ParentMenu is TPopupMenu then
        Alignment := TPopupMenu(ParentMenu).Alignment
      else
        Alignment := paLeft;
      GlyphRect.Left := ARect.Left + 1;
      GlyphRect.Top := ARect.Top + 1;
      if Caption = cLineCaption then
      begin
        FillRect(ARect);
        GlyphRect.Left := 0;
        GlyphRect.Right := -4;
        DrawGlyph := False;
      end
      else
      begin
        DrawImage := (ImageList <> nil) and ((ImageIndex > -1) and
          (ImageIndex < ImageList.Count) or Checked and ((FBitmap = nil) or
          FBitmap.Empty));
        if DrawImage or Assigned(FBitmap) and not FBitmap.Empty then
        begin
          DrawGlyph := True;

          if DrawImage then
          begin
            GlyphRect.Right := GlyphRect.Left + ImageList.Width;
            GlyphRect.Bottom := GlyphRect.Top + ImageList.Height;
          end
          else
          begin
            { Need to add BitmapWidth/Height properties for TMenuItem if we're to
              support them.  Right now let's hardcode them to 16x16. }
            GlyphRect.Right := GlyphRect.Left + 16;
            GlyphRect.Bottom := GlyphRect.Top + 16;
          end;

          { Draw background pattern brush if selected }
          if Checked then
          begin
            Inc(GlyphRect.Right);
            Inc(GlyphRect.Bottom);
            OldBrushColor := Brush.Color;
            if not (odSelected in State) then
            begin
              OldBrushColor := Brush.Color;
              Brush.Bitmap := AllocPatternBitmap(clBtnFace, clBtnHighlight);
              FillRect(GlyphRect);
            end
            else
            begin
              Brush.Color := clBtnFace;
              FillRect(GlyphRect);
            end;
            Brush.Color := OldBrushColor;
            Inc(GlyphRect.Left);
            Inc(GlyphRect.Top);
          end;

          if DrawImage then
          begin
            if (ImageIndex > -1) and (ImageIndex < ImageList.Count) then
              ImageList.Draw(ACanvas, GlyphRect.Left, GlyphRect.Top, ImageIndex,
                Enabled)
            else
            begin
              { Draw a menu check }
              Glyph := TBitmap.Create;
              try
                Glyph.Transparent := True;
                Glyph.Handle := LoadBitmap(0, PChar(OBM_CHECK));
                OldBrushColor := Font.Color;
                Font.Color := clBtnText;
                Draw(GlyphRect.Left + (GlyphRect.Right - GlyphRect.Left - Glyph.Width) div 2 + 1,
                  GlyphRect.Top + (GlyphRect.Bottom - GlyphRect.Top - Glyph.Height) div 2 + 1, Glyph);
                Font.Color := OldBrushColor;
              finally
                Glyph.Free;
              end;
            end;
          end
          else
          begin
            SaveRect := GlyphRect;
            { Make sure image is within glyph bounds }
            if FBitmap.Width < GlyphRect.Right - GlyphRect.Left then
              with GlyphRect do
              begin
                Left := Left + ((Right - Left) - FBitmap.Width) div 2 + 1;
                Right := Left + FBitmap.Width;
              end;
            if FBitmap.Height < GlyphRect.Bottom - GlyphRect.Top then
              with GlyphRect do
              begin
                Top := Top + ((Bottom - Top) - FBitmap.Height) div 2 + 1;
                Bottom := Top + FBitmap.Height;
              end;
            StretchDraw(GlyphRect, FBitmap);
            GlyphRect := SaveRect;
          end;

          if Checked then
          begin
            Dec(GlyphRect.Right);
            Dec(GlyphRect.Bottom);
          end;
        end
        else
        begin
          if (ImageList <> nil) and not TopLevel then
          begin
            GlyphRect.Right := GlyphRect.Left + ImageList.Width;
            GlyphRect.Bottom := GlyphRect.Top + ImageList.Height;
          end
          else
          begin
            GlyphRect.Right := GlyphRect.Left;
            GlyphRect.Bottom := GlyphRect.Top;
          end;
          DrawGlyph := False;
        end;
      end;
      with GlyphRect do
      begin
        Dec(Left);
        Dec(Top);
        Inc(Right, 2);
        Inc(Bottom, 2);
      end;

      if Checked or Selected and DrawGlyph then
        DrawEdge(Handle, GlyphRect, EdgeStyle[Checked], BF_RECT);

      if Selected then
      begin
        if DrawGlyph then ARect.Left := GlyphRect.Right + 1;
        if not (Win98Plus and TopLevel) then
          Brush.Color := clHighlight;
        FillRect(ARect);
      end;
      if TopLevel and Win98Plus then
      begin
        if Selected then
          DrawEdge(Handle, ARect, BDR_SUNKENOUTER, BF_RECT)
        else if odHotLight in State then
          DrawEdge(Handle, ARect, BDR_RAISEDINNER, BF_RECT);
        if not Selected then
          OffsetRect(ARect, 0, -1);
      end;
      if not (Selected and DrawGlyph) then
        ARect.Left := GlyphRect.Right + 1;
      Inc(ARect.Left, 2);
      Dec(ARect.Right, 1);

      DrawStyle := DT_EXPANDTABS or DT_SINGLELINE or Alignments[Alignment];
      if Win2K and (odNoAccel in State) then
        DrawStyle := DrawStyle or DT_HIDEPREFIX;
      { Calculate vertical layout }
      SaveRect := ARect;
      if odDefault in State then
        Font.Style := [fsBold];
      DoDrawText(ACanvas, Caption, ARect, Selected, DrawStyle or DT_CALCRECT or DT_NOCLIP);
      OffsetRect(ARect, 0, ((SaveRect.Bottom - SaveRect.Top) - (ARect.Bottom - ARect.Top)) div 2);
      if TopLevel and Selected and Win98Plus then
        OffsetRect(ARect, 1, 0);
      DoDrawText(ACanvas, Caption, ARect, Selected, DrawStyle);
      if (ShortCut <> 0) and not TopLevel then
      begin
        ARect.Left := ARect.Right;
        ARect.Right := SaveRect.Right - 10;
        DoDrawText(ACanvas, ShortCutToText(ShortCut), ARect, Selected, DT_RIGHT);
      end;
    end;
  end;

  procedure BiDiDraw;
  var
    S: string;
  begin
    with ACanvas do
    begin
      //ImageList := GetImageList;
      if not Selected then FillRect(ARect);
      if ParentMenu is TMenu then
        Alignment := paLeft
      else if ParentMenu is TPopupMenu then
        Alignment := TPopupMenu(ParentMenu).    Alignment
      else
        Alignment := paLeft;
      GlyphRect.Right := ARect.Right - 1;
      GlyphRect.Top := ARect.Top + 1;
      if Caption = cLineCaption then
      begin
        FillRect(ARect);
        GlyphRect.Left := GlyphRect.Right + 2;
        GlyphRect.Right := 0;
        DrawGlyph := False;
      end
      else
      begin
        DrawImage := (ImageList <> nil) and ((ImageIndex > -1) and
          (ImageIndex < ImageList.    Count) or Checked and ((FBitmap = nil) or
          FBitmap.    Empty));
        if DrawImage or Assigned(FBitmap) and not FBitmap.    Empty then
        begin
          DrawGlyph := True;

          if DrawImage then
          begin
            GlyphRect.Left := GlyphRect.Right - ImageList.Width;
            GlyphRect.Bottom := GlyphRect.Top + ImageList.Height;
          end
          else
          begin
            { Need to add BitmapWidth/Height properties for TMenuItem if we're to
              support them.  Right now let's hardcode them to 16x16. }
            GlyphRect.Left := GlyphRect.Right - 16;
            GlyphRect.Bottom := GlyphRect.Top + 16;
          end;

          { Draw background pattern brush if selected }
          if Checked then
          begin
            Dec(GlyphRect.Left);
            Inc(GlyphRect.Bottom);
            OldBrushColor := Brush.Color;
            if not Selected then
            begin
              OldBrushColor := Brush.Color;
              Brush.Bitmap := AllocPatternBitmap(clBtnFace, clBtnHighlight);
              FillRect(GlyphRect);
            end
            else
            begin
              Brush.Color := clBtnFace;
              FillRect(GlyphRect);
            end;
            Brush.Color := OldBrushColor;
            Dec(GlyphRect.Right);
            Inc(GlyphRect.Top);
          end;

          if DrawImage then
          begin
            if (ImageIndex > -1) and (ImageIndex < ImageList.Count) then
              ImageList.Draw(ACanvas, GlyphRect.Left, GlyphRect.Top, ImageIndex,
                Enabled)
            else
            begin
              { Draw a menu check }
              Glyph := TBitmap.Create;
              try
                Glyph.Transparent := True;
                Glyph.Handle := LoadBitmap(0, PChar(OBM_CHECK));
                OldBrushColor := Font.Color;
                Font.Color := clBtnText;
                Draw(GlyphRect.Left + (GlyphRect.Right - GlyphRect.Left - Glyph.Width) div 2 + 1,
                  GlyphRect.Top + (GlyphRect.Bottom - GlyphRect.Top - Glyph.Height) div 2 + 1, Glyph);
                Font.Color := OldBrushColor;
              finally
                Glyph.Free;
              end;
            end;
          end
          else
          begin
            SaveRect := GlyphRect;
            { Make sure image is within glyph bounds }
            if FBitmap.Width < GlyphRect.Right - GlyphRect.Left then
              with GlyphRect do
              begin
                Right := Right - ((Right - Left) - FBitmap.Width) div 2 + 1;
                Left := Right - FBitmap.Width;
              end;
            if FBitmap.Height < GlyphRect.Bottom - GlyphRect.Top then
              with GlyphRect do
              begin
                Top := Top + ((Bottom - Top) - FBitmap.Height) div 2 + 1;
                Bottom := Top + FBitmap.Height;
              end;
            StretchDraw(GlyphRect, FBitmap);
            GlyphRect := SaveRect;
          end;

          if Checked then
          begin
            Dec(GlyphRect.Right);
            Dec(GlyphRect.Bottom);
          end;
        end
        else
        begin
          if (ImageList <> nil) and not TopLevel then
          begin
            GlyphRect.Left := GlyphRect.Right - ImageList.Width;
            GlyphRect.Bottom := GlyphRect.Top + ImageList.Height;
          end
          else
          begin
            GlyphRect.Left := GlyphRect.Right;
            GlyphRect.Bottom := GlyphRect.Top;
          end;
          DrawGlyph := False;
        end;
      end;
      with GlyphRect do
      begin
        Dec(Left);
        Dec(Top);
        Inc(Right, 2);
        Inc(Bottom, 2);
      end;

      if Checked or Selected and DrawGlyph then
        DrawEdge(Handle, GlyphRect, EdgeStyle[Checked], BF_RECT);

      if Selected then
      begin
        if DrawGlyph then ARect.Right := GlyphRect.Left - 1;
        if not (Win98Plus and TopLevel) then
          Brush.Color := clHighlight;
        FillRect(ARect);
      end;
      if TopLevel and Win98Plus then
      begin
        if Selected then
          DrawEdge(Handle, ARect, BDR_SUNKENOUTER, BF_RECT)
        else if odHotLight in State then
          DrawEdge(Handle, ARect, BDR_RAISEDINNER, BF_RECT);
        if not Selected then
          OffsetRect(ARect, 0, -1);
      end;
      if not (Selected and DrawGlyph) then
        ARect.Right := GlyphRect.Left - 1;
      Inc(ARect.Left, 2);
      Dec(ARect.Right, 1);

      DrawStyle := DT_EXPANDTABS or DT_SINGLELINE or Alignments[Alignment];
      if Win2K and (odNoAccel in State) then
        DrawStyle := DrawStyle or DT_HIDEPREFIX;
      { Calculate vertical layout }
      SaveRect := ARect;
      if odDefault in State then
        Font.Style := [fsBold];
      DoDrawText(ACanvas, Caption, ARect, Selected, DrawStyle or DT_CALCRECT or DT_NOCLIP);
      { the DT_CALCRECT does not take into account alignment }
      ARect.Left := SaveRect.Left;
      ARect.Right := SaveRect.Right;
      OffsetRect(ARect, 0, ((SaveRect.Bottom - SaveRect.Top) - (ARect.Bottom - ARect.Top)) div 2);
      if TopLevel and Selected and Win98Plus then
        OffsetRect(ARect, 1, 0);
      DoDrawText(ACanvas, Caption, ARect, Selected, DrawStyle);
      if (ShortCut <> 0) and not TopLevel then
      begin
        S := ShortCutToText(ShortCut);
        ARect.Left := 10;
        ARect.Right := ARect.Left + ACanvas.TextWidth(S);
        DoDrawText(ACanvas, S, ARect, Selected, DT_RIGHT);
      end;
    end;
  end;

begin
  ParentMenu := GetParentMenu;
  ImageList := GetImageList;
  Selected := odSelected in State;
  Win98Plus := (Win32MajorVersion > 4) or
    ((Win32MajorVersion = 4) and (Win32MinorVersion > 0));
  Win2K := (Win32MajorVersion > 4) and (Win32Platform = VER_PLATFORM_WIN32_NT);
  if (ParentMenu <> nil) and (ParentMenu.OwnerDraw or (ImageList <> nil)) and
    (Assigned(FOnAdvancedDrawItem) or Assigned(FOnDrawItem)) then
  begin
    DrawItem(ACanvas, ARect, Selected);
    if Assigned(FOnAdvancedDrawItem) then
      FOnAdvancedDrawItem(Self, ACanvas, ARect, State);
  end else
    if (ParentMenu <> nil) and (not ParentMenu.IsRightToLeft) then
      NormalDraw
    else
      BiDiDraw;
end;

function TMenuItem.GetImageList: TCustomImageList;
var
  LItem: TMenuItem;
  LMenu: TMenu;
begin
  Result := nil;
  LItem := Parent;
  while (LItem <> nil) and (LItem.SubMenuImages = nil) do
    LItem := LItem.Parent;
  if LItem <> nil then
    Result := LItem.SubMenuImages
  else
  begin
    LMenu := GetParentMenu;
    if LMenu <> nil then
      Result := LMenu.Images;
  end;
end;

procedure TMenuItem.MeasureItem(ACanvas: TCanvas; var Width, Height: Integer);
const
  Alignments: array[TPopupAlignment] of Word = (DT_LEFT, DT_RIGHT, DT_CENTER);
var
  Alignment: TPopupAlignment;
  ImageList: TCustomImageList;
  ParentMenu: TMenu;
  DrawGlyph: Boolean;
  TopLevel: Boolean;
  DrawStyle: Integer;
  Text: string;
  R: TRect;

  procedure GetMenuSize;
  var
    NonClientMetrics: TNonClientMetrics;
  begin
    NonClientMetrics.cbSize := sizeof(NonClientMetrics);
    if SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, @NonClientMetrics, 0) then
    begin
      Width := NonClientMetrics.iMenuWidth;
      Height := NonClientMetrics.iMenuHeight;
    end;
  end;

begin
  if GetParentComponent is TMainMenu then
  begin
    TopLevel := True;
    GetMenuSize;
  end
  else TopLevel := False;
  ParentMenu := GetParentMenu;
  ImageList := GetImageList;
  if Caption = cLineCaption then
  begin
    Height := 5;
    Width := -2;
    DrawGlyph := False;
  end
  else if Assigned(ImageList) and ((ImageIndex > -1) or not TopLevel) then
  begin
    Width := ImageList.Width;
    if not TopLevel then
      Height := ImageList.Height;
    DrawGlyph := True;
  end
  else if Assigned(FBitmap) and not FBitmap.Empty then
  begin
    Width := 16;
    if not TopLevel then
      Height := 16;
    DrawGlyph := True;
  end
  else
  begin
    Width := -7;
    DrawGlyph := False;
  end;
  if DrawGlyph and not TopLevel then
    Inc(Width, 15);
  if not TopLevel then
    Inc(Height, 3);
  FillChar(R, SizeOf(R), 0);
  if ParentMenu is TMenu then
    Alignment := paLeft
  else if ParentMenu is TPopupMenu then
    Alignment := TPopupMenu(ParentMenu).Alignment
  else
    Alignment := paLeft;
  if ShortCut <> 0 then
    Text := Concat(Caption, ShortCutToText(ShortCut)) else
    Text := Caption;
  DrawStyle := Alignments[Alignment] or DT_EXPANDTABS or DT_SINGLELINE or
    DT_NOCLIP or DT_CALCRECT;
  DoDrawText(ACanvas, Text, R, False, DrawStyle);
  Inc(Width, R.Right - R.Left + 7);
  if Assigned(FOnMeasureItem) then FOnMeasureItem(Self, ACanvas, Width, Height);
end;

procedure TMenuItem.SetBreak(Value: TMenuBreak);
begin
  if FBreak <> Value then
  begin
    FBreak := Value;
    MenuChanged(True);
  end;
end;

procedure TMenuItem.GetChildren(Proc: TGetChildProc; Root: TComponent);
var
  I: Integer;
begin
  for I := 0 to Count - 1 do Proc(Items[I]);
end;

procedure TMenuItem.SetChildOrder(Child: TComponent; Order: Integer);
begin
  (Child as TMenuItem).MenuIndex := Order;
end;


procedure TMenuItem.MenuChanged(Rebuild: Boolean);
var
  Source: TMenuItem;
begin
  if (Parent = nil) and (Owner is TMenu) then
    Source := nil else
    Source := Self;
  if Assigned(FOnChange) then FOnChange(Self, Source, Rebuild);
end;

procedure TMenuItem.SubItemChanged(Sender: TObject; Source: TMenuItem; Rebuild: Boolean);
begin
  if Rebuild and ((FHandle <> 0) or Assigned(FMergedWith)) then RebuildHandle;
  if Parent <> nil then Parent.SubItemChanged(Self, Source, False)
  else if Owner is TMainMenu then TMainMenu(Owner).ItemChanged;
end;

function TMenuItem.GetParentComponent: TComponent;
begin
  if (FParent <> nil) and (FParent.FMenu <> nil) then
    Result := FParent.FMenu else
    Result := FParent;
end;

procedure TMenuItem.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited Notification(AComponent, Operation);
  if Operation = opRemove then
    if AComponent = Action then
      Action := nil
    else if AComponent = FSubMenuImages then
      SubMenuImages := nil
    else if AComponent = FMerged then
      MergeWith(nil);
end;

procedure TMenuItem.SetSubMenuImages(Value: TCustomImageList);
begin
  if FSubMenuImages <> nil then FSubMenuImages.UnRegisterChanges(FImageChangeLink);
  FSubMenuImages := Value;
  if FSubMenuImages <> nil then
  begin
    FSubMenuImages.RegisterChanges(FImageChangeLink);
    FSubMenuImages.FreeNotification(Self);
  end;
  UpdateItems;
end;

procedure TMenuItem.UpdateItems;

  function UpdateItem(MenuItem: TMenuItem): Boolean;
  begin
    Result := False;
    IterateMenus(@UpdateItem, MenuItem.FMerged, MenuItem);
    MenuItem.SubItemChanged(MenuItem, MenuItem, True);
  end;

begin
  IterateMenus(@UpdateItem, FMerged, Self);
end;

procedure TMenuItem.Add(const AItems: array of TMenuItem);
var
  I: Integer;
begin
  for I := Low(AItems) to High(AItems) do
    Add(AItems[I]);
end;

function TMenuItem.InternalRethinkHotkeys(ForceRethink: Boolean): Boolean;
var
  LDid, LDoing, LToDo, LBest: TStringList;
  I, LIteration, LColumn, LAt, LBestCount: Integer;
  LChar, LCaption, LOrigAvailable, LAvailable, LBestAvailable: string;
  function IfHotkeyAvailable(const AHotkey: string): Boolean;
  var
    At: Integer;
  begin
    At := AnsiPos(AHotkey, LAvailable);
    Result := At <> 0;
    if Result then
      System.Delete(LAvailable, At, 1);
  end;
  procedure CopyToBest;
  var
    I: Integer;
  begin
    LBest.Assign(LDid);
    LBestCount := LDid.Count;
    for I := 0 to LDoing.Count - 1 do
      LBest.AddObject(TMenuItem(LDoing.Objects[I]).FCaption, LDoing.Objects[I]);
    LBestAvailable := LAvailable;
  end;
  procedure InsertHotkeyFarEastFormat(var ACaption: string; const AHotKey: string; AColumn: Integer);
  var
    I: Integer;
    LMBCSFlag: Boolean;
  begin
    LMBCSFlag := False;
    for I := 1 to Length(ACaption) do
      if ACaption[I] in LeadBytes then
      begin
        LMBCSFlag := True;
        System.Break;
      end;
    if LMBCSFlag then
    begin
      if Copy(ACaption, (Length(ACaption) - Length(cDialogSuffix)) + 1, Length(cDialogSuffix)) = cDialogSuffix then
        ACaption := Copy(ACaption, 1, Length(ACaption) - Length(cDialogSuffix)) +
          '(' + cHotkeyPrefix + AHotKey + ')' + cDialogSuffix
      else
        ACaption := ACaption + '(' + cHotkeyPrefix + AHotKey + ')';
    end
    else if AColumn <> 0 then
      System.Insert(cHotkeyPrefix, ACaption, AColumn);
  end;
begin
  Result := False;
  if ForceRethink or
     (not (csDesigning in ComponentState) and GetAutoHotkeys) then
  begin
    LAvailable := ValidMenuHotkeys;
    LDid := nil;
    LDoing := nil;
    LToDo := nil;
    LBest := nil;
    LBestCount := 0;
    try
      LDid := TStringList.Create;
      LDoing := TStringList.Create;
      LToDo := TStringList.Create;
      LBest := TStringList.Create;
      for I := 0 to Count - 1 do
        if Items[I].Visible and
           (Items[I].FCaption <> cLineCaption) and
           (Items[I].FCaption <> '') then
        begin
          LChar := Uppercase(GetHotkey(Items[I].FCaption));
          if LChar = '' then
            LToDo.InsertObject(0, Items[I].FCaption, Items[I])
          else if (AnsiPos(LChar, ValidMenuHotkeys) <> 0) and
                  not IfHotkeyAvailable(LChar) then
          begin
            Items[I].FCaption := StripHotkey(Items[I].FCaption);
            LToDo.InsertObject(0, Items[I].FCaption, Items[I]);
          end;
        end;
      LOrigAvailable := LAvailable;
      for LIteration := 0 to LToDo.Count - 1 do
      begin
        LAvailable := LOrigAvailable;
        LDoing.Assign(LToDo);
        LDid.Clear;
        for I := LDoing.Count - 1 downto 0 do
        begin
          LCaption := LDoing[I];
          LColumn := 1;
          while LColumn <= Length(LCaption) do
          begin
            if LCaption[LColumn] in LeadBytes then
              Inc(LColumn)
            else
            begin
              LChar := Uppercase(Copy(LCaption, LColumn, 1));
              if IfHotkeyAvailable(LChar) then
              begin
                if SysLocale.FarEast then
                  InsertHotkeyFarEastFormat(LCaption, LChar, LColumn)
                else
                  System.Insert(cHotkeyPrefix, LCaption, LColumn);
                LDid.AddObject(LCaption, LDoing.Objects[I]);
                LDoing.Delete(I);
                System.Break;
              end;
            end;
            Inc(LColumn);
          end;
        end;
        if LDid.Count > LBestCount then
          CopyToBest;
        if LDoing.Count > 0 then
          for I := 0 to LDoing.Count - 1 do
          begin
            LAt := LToDo.IndexOfObject(LDoing.Objects[I]);
            LToDo.Move(LAt, LToDo.Count - 1);
          end
        else
          System.Break;
      end;
      if LBestCount = 0 then
        CopyToBest;
      Result := LBest.Count > 0;
      for I := 0 to LBest.Count - 1 do
      begin
        LCaption := LBest[I];
        if SysLocale.FarEast and (AnsiPos(cHotkeyPrefix, LCaption) = 0)
          and (LBestAvailable <> '') then
        begin
          if AnsiPos(cHotkeyPrefix, LCaption) = 0 then
          begin
            InsertHotkeyFarEastFormat(LCaption, Copy(LBestAvailable, Length(LBestAvailable), 1), 0);
            System.Delete(LBestAvailable, length(LBestAvailable), 1);
          end;
        end;
        TMenuItem(LBest.Objects[I]).FCaption := LCaption;
      end;
    finally
      LBest.Free;
      LToDo.Free;
      LDoing.Free;
      LDid.Free;
    end;
  end;
end;

function TMenuItem.RethinkHotkeys: Boolean;
begin
  Result := InternalRethinkHotkeys(True);
  if Result then
    MenuChanged(True);
end;

procedure TMenuItem.SetAutoHotkeys(const Value: TMenuItemAutoFlag);
begin
  if Value <> FAutoHotkeys then
  begin
    FAutoHotkeys := Value;
    MenuChanged(True);
  end;
end;

function TMenuItem.Find(ACaption: string): TMenuItem;
var
  I: Integer;
begin
  Result := nil;
  ACaption := StripHotkey(ACaption);
  for I := 0 to Count - 1 do
    if AnsiSameText(ACaption, StripHotkey(Items[I].Caption)) then
    begin
      Result := Items[I];
      System.Break;
    end;
end;

function TMenuItem.InsertNewLine(ABefore: Boolean; AItem: TMenuItem): Integer;
begin
  if AItem.Parent <> Self then
     Error(@SMenuNotFound);
  if ABefore then
  begin
    if (AItem.MenuIndex > 0) and
       Items[AItem.MenuIndex - 1].IsLine then
    begin
      Result := AItem.MenuIndex - 1;
      Items[AItem.MenuIndex - 1].Visible := True;
    end
    else
    begin
      Result := AItem.MenuIndex;
      Insert(AItem.MenuIndex, NewLine);
    end;
  end
  else
  begin
    if (AItem.MenuIndex < Count - 1) and
       Items[AItem.MenuIndex + 1].IsLine then
    begin
      Result := AItem.MenuIndex + 2;
      Items[AItem.MenuIndex + 1].Visible := True;
    end
    else
    begin
      Result := AItem.MenuIndex + 2;
      Insert(AItem.MenuIndex + 1, NewLine);
    end;
  end;
end;

function TMenuItem.InsertNewLineAfter(AItem: TMenuItem): Integer;
begin
  Result := InsertNewLine(False, AItem);
end;

function TMenuItem.InsertNewLineBefore(AItem: TMenuItem): Integer;
begin
  Result := InsertNewLine(True, AItem);
end;

function TMenuItem.NewBottomLine: Integer;
begin
  Result := 0;
  if Count = 0 then
    Add(NewLine)
  else
    Result := InsertNewLine(False, Items[Count - 1]);
end;

function TMenuItem.NewTopLine: Integer;
begin
  Result := 0;
  if Count = 0 then
    Add(NewLine)
  else
    Result := InsertNewLine(True, Items[0]);
end;

function TMenuItem.InternalRethinkLines(ForceRethink: Boolean): Boolean;
var
  I, LLastAt: Integer;
  LLastBar: TMenuItem;
begin
  Result := False;
  if ForceRethink or
     (not (csDesigning in ComponentState) and GetAutoLineReduction) then
  begin
    LLastAt := 0;
    LLastBar := nil;
    for I := LLastAt to Count - 1 do
      if Items[I].FVisible then
        if Items[I].IsLine then
        begin
          Items[I].FVisible := False;
          Result := True;
        end
        else
        begin
          LLastAt := I;
          System.Break;
        end;
    for I := LLastAt to Count - 1 do
      if Items[I].IsLine then
      begin
        if (LLastBar <> nil) and (LLastBar.FVisible) then
        begin
          LLastBar.FVisible := False;
          Result := True;
        end;
        LLastBar := Items[I];
      end
      else if Items[I].FVisible then
      begin
        if (LLastBar <> nil) and (not LLastBar.FVisible) then
        begin
          LLastBar.FVisible := True;
          Result := True;
        end;
        LLastBar := nil;
        LLastAt := I;
      end;
    for I := Count - 1 downto LLastAt do
      if Items[I].FVisible then
        if Items[I].IsLine then
        begin
          Items[I].FVisible := False;
          Result := True;
        end
        else
          System.Break;
  end;
end;

procedure TMenuItem.SetAutoLineReduction(const Value: TMenuItemAutoFlag);
begin
  if Value <> FAutoLineReduction then
  begin
    FAutoLineReduction := Value;
    MenuChanged(True);
  end;
end;

function TMenuItem.RethinkLines: Boolean;
begin
  Result := InternalRethinkLines(True);
  if Result then
    MenuChanged(True);
end;

function TMenuItem.GetAutoHotkeys: Boolean;
var
  LAuto: TMenuItemAutoFlag;
begin
  LAuto := FAutoHotkeys;
  if (LAuto = maParent) and
     (Parent <> nil) then
    LAuto := cBooleanToItemAutoFlag[Parent.GetAutoHotkeys];
  Result := cItemAutoFlagToBoolean[LAuto];
end;

function TMenuItem.GetAutoLineReduction: Boolean;
var
  LAuto: TMenuItemAutoFlag;
begin
  LAuto := FAutoLineReduction;
  if (LAuto = maParent) and
     (Parent <> nil) then
    LAuto := cBooleanToItemAutoFlag[Parent.GetAutoLineReduction];
  Result := cItemAutoFlagToBoolean[LAuto];
end;
*/
//---------------------------------------------------------------------------
//  TMenu
//---------------------------------------------------------------------------
TMenu :: TMenu()
{
    Init();
}
//---------------------------------------------------------------------------
TMenu :: TMenu(TWinControl *AParent) : TCustomForm(AParent)
{
    Init();
}
//---------------------------------------------------------------------------
TMenu :: ~TMenu()
{
}
//---------------------------------------------------------------------------
void TMenu :: Init()
{
    m_Images = NULL;
    m_Toplevel = false;
    SubMenuClass = NULL;
    SelectColor = clHighlight;
    SelectTextColor = clHighlightText;
    Color = clMenuBar;
//	OverrideRedirect = true;
}
//---------------------------------------------------------------------------
void TMenu :: set_Images(TImageList *value)
{
    if(m_Images != value)
    {
        m_Images = value;
        Update();
    }
}
//---------------------------------------------------------------------------
//void TMenu :: DoShow()
//{
//	TCustomForm :: DoShow();
//}
//---------------------------------------------------------------------------
//void TMenu :: Add(TCommonMenuItem *item)
void TMenu :: Add(TCommonMenuItem &item)
{
    m_MenuItems.Add(&item);
    item.m_Menu = this;
}
//---------------------------------------------------------------------------
void TMenu :: Paint()
{
    for(register int i = 0; i < ControlCount; i++)
    {
        Controls[i]->Paint();
    }
}
//---------------------------------------------------------------------------
void TMenu :: KeyDown(uint_t key, uint_t state)
{
    switch(key)
    {
    case XK_Left:
    case XK_KP_Left:
        printf("LEFT ARROW\n");
        break;
    case XK_Right:
    case XK_KP_Right:
        printf("RIGHT ARROW\n");
        break;
    case XK_Up:
    case XK_KP_Up:
        printf("UP ARROW\n");
        break;
    case XK_Down:
    case XK_KP_Down:
        printf("DOWN ARROW\n");
        break;
    case XK_Escape:
        Screen.ClosePopup();
        break;
    }
    TCustomForm :: KeyDown(key, state);
}
//---------------------------------------------------------------------------
//  TCustomPopupMenu
//---------------------------------------------------------------------------
TCustomPopupMenu :: TCustomPopupMenu()
{
    Init();
}
//---------------------------------------------------------------------------
void TCustomPopupMenu :: Init()
{
    StyleOn(csPopup);
    FormType = ftPopupMenu;
    AutoPopup = true;
    OverrideRedirect = true;
//	GrabKeyboard = true;
//	GrabMouse = true;
}
//---------------------------------------------------------------------------
void TCustomPopupMenu :: Popup(int x, int y)
{
    if(!Count)
        return;
    if(OnPopup)
        OnPopup(this);
    Left = x;
    Top = y;
    m_PopupPoint = TPoint(x, y);
    Visible = true;
    Focused = true;
}
//---------------------------------------------------------------------------
void TCustomPopupMenu :: DoShow()
{
    TPoint pt;
    pt.x = Left;
    pt.y = Top;
    uint_t w, h;
    Measure(w, h);
    if(pt.x + (int)w > Screen.Width)
        pt.x = Screen.Width - w;
    if(pt.y + (int)h > Screen.Height)
        pt.y = Screen.Height - h;
    BoundsRect = TRect(pt.x, pt.y, w, h);

    TCustomForm :: DoShow();
}
//---------------------------------------------------------------------------
void TCustomPopupMenu :: Measure(uint_t &width, uint_t &height)
{
    TCommonMenuItem *mi;
    TMenu *m;
    uint_t w, h, maxw, maxh;
    maxw = maxh = 0;
    for(register int i = 0; i < Count; i++)
    {
        mi = Items[i];
        m = dynamic_cast<TMenu *>(*mi->Parent);
        if( m && m->Toplevel )
            continue;
        mi->Parent = this;
        mi->MeasureItem(w, h);
        if(w > maxw)
            maxw = w;
        mi->BoundsRect = TRect(0, maxh, maxw, h);
        maxh += h;
    }
    for(register int i = 0; i < Count; i++)
    {
        mi = Items[i];
        m = dynamic_cast<TMenu *>(*mi->Parent);
        if( m && m->Toplevel )
            continue;
        mi->Width = maxw;
    }
    width = maxw;
    height = maxh;
}
//---------------------------------------------------------------------------
void TCustomPopupMenu :: MouseExit(TShiftState state, int x, int y)
{
//    if(m_PopupControl)
//    {
//		m_PopupControl->Active = false;
//	}

//	Active = false;
    /*	TCustomForm *tf;
    	TCustomPopupMenu *pm;
    	TRect rect;
    	TPoint p;
        p.x = x;
        p.y = y;

    	for(TList::iterator from = CScreen.Forms.begin();
           from != CScreen.Forms.end();
                ++from)
    	{
    		tf = (TCustomForm *)(*from);
    		pm = dynamic_cast<TCustomPopupMenu *>(tf);
    		if(pm)
    		{
    			if(pm == this)
    				continue;
    			p = ClientToScreen(p);

    		}
    	}
    */
    /*	TCommonMenuItem *mi = dynamic_cast<TCommonMenuItem *>(m_PopupControl);
    	if(mi)
    	{
    		TCustomPopupMenu *w = dynamic_cast<TCustomPopupMenu *>(mi->GetParentMenu());
    		if(w)
    		{
    //			GrabMouse = false;
    //			w->GrabMouse = true;
    		}
        } */
    TMenu :: MouseExit(state, x, y);
}
//---------------------------------------------------------------------------
void TCustomPopupMenu :: MouseEnter(TShiftState state, int x, int y)
{
//	BringToFront();
//	GrabMouse = true;
//	Active = true;
    TMenu :: MouseEnter(state, x, y);
}
//---------------------------------------------------------------------------
void TCustomPopupMenu :: MouseMove(TShiftState state, int x, int y)
{
//	BringToFront();

//	GrabMouse = true;
//	Active = true;
//	printf("MENU_MOUSE_MOVE %d %d %d\n", id, x, y);
    TMenu :: MouseMove(state, x, y);
}
//---------------------------------------------------------------------------
void TCustomPopupMenu :: DoExit()
{
//	TCommonMenuItem *mi = dynamic_cast<TCommonMenuItem *>(m_PopupControl);
//	if(mi)
//	{
//		Close();
//    }
    printf("DOEXIT %d\n", id);
    TMenu :: DoExit();
}
//---------------------------------------------------------------------------
void TCustomPopupMenu :: DoEnter()
{
    printf("DOENTER %d\n", id);
    TMenu :: DoEnter();
}
//---------------------------------------------------------------------------
//  TCustomMainMenu
//---------------------------------------------------------------------------
#define EVENTS_ENABLED_MASK		ExposureMask | \
								ButtonPressMask | \
								ButtonReleaseMask | \
                   				PropertyChangeMask | \
                   				EnterWindowMask | \
                   				LeaveWindowMask | \
                 				StructureNotifyMask

/*                  				KeyPressMask | \
                   				KeyReleaseMask | \
                  				PointerMotionMask | \   */
/*                 				ButtonMotionMask | \  */
/*                 				ButtonPressMask | \
                  				ButtonReleaseMask | \ */
/*                   				FocusChangeMask | \  */

TCustomMainMenu :: TCustomMainMenu()
{
    Init();
}
//---------------------------------------------------------------------------
TCustomMainMenu :: TCustomMainMenu(TWinControl *AParent) : TMenu(AParent)
{
    Init();
}
//---------------------------------------------------------------------------
void TCustomMainMenu :: Init()
{
//	XSelectInput(Application.Handle, m_Handle, EVENTS_ENABLED_MASK);
    m_Toplevel = true;
//	FormType = ftMenu;
    Align = alWidth;
    Height = 22;
}
//---------------------------------------------------------------------------
void TCustomMainMenu :: Measure(uint_t &width, uint_t &height)
{
    TCommonMenuItem *mi;
    uint_t w, h, maxw, maxh;
    maxw = maxh = 0;
    for(register int i = 0; i < Count; i++)
    {
        mi = Items[i];
        mi->Parent = this;
        mi->MeasureItem(w, h);
        if(h > maxh)
            maxh = h;
        mi->BoundsRect = TRect(maxw, 0, w, h);
        maxw += w;
    }
    w = (*Parent->ClientRect).width;
    width = (maxw < w) ? w : maxw;
    height = maxh ? maxh : 22 ;
}
//---------------------------------------------------------------------------
void TCustomMainMenu :: DoShow()
{
    if(!Count)
        return;
    uint_t w, h;
    Measure(w, h);
    BoundsRect = TRect(Left, Top, w, h);
    TCustomForm :: DoShow();
}
//---------------------------------------------------------------------------
void TCustomMainMenu :: set_Parent(TWinControl *AParent)
{
    if(AParent)
    {
        AParent->Canvas.Origin = TPoint(0, Height);
        Top = -Height;
    }
    else
    {
        AParent->Canvas.Origin = TPoint(0, 0);
        Top = 0;
    }
    TMenu :: set_Parent(AParent);
}
//---------------------------------------------------------------------------
void TCustomMainMenu :: MouseDown(TMouseButton button, TShiftState state, int x, int y)
{
//	TRect rect = ClientRect;

//	m_PopupControl->Active = false;
//	if(!rect.inside(x, y))
//	{
//        Active = true;
    TMenu :: MouseDown(button, state, x, y);
//		CScreen.ClosePopup();
//	}
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------

