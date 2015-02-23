//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <X11/extensions/shape.h>
#include "xgdi.h"
#include "netwm.h"
#include "rtti.h"
#include "application.h"
#include "menu.h"

#define EVENTS_ENABLED_MASK		ExposureMask | \
                  				ButtonPressMask | \
                  				ButtonReleaseMask | \
                  				PointerMotionMask | \
                  				ButtonMotionMask | \
                  				KeyPressMask | \
                   				KeyReleaseMask | \
                   				EnterWindowMask | \
                   				LeaveWindowMask | \
                   				FocusChangeMask | \
                   				PropertyChangeMask | \
                 				StructureNotifyMask
// SubstructureNotifyMask
// ResizeRedirectMask |
// SubstructureRedirectMask |
// VisibilityChangeMask |
// ColormapChangeMask
// GCGraphicsExposures in GC	GraphicsExpose	XGraphicsExposeEvent
// NoExpose	XNoExposeEvent
// KeymapStateMask	KeymapNotify	XKeymapEvent
// OwnerGrabButtonMask	N.A.	N.A.
// PointerMotionHintMask	N.A.	N.A.
// ResizeRedirectMask	ResizeRequest	XResizeRequestEvent
// StructureNotifyMask	CirculateNotify	XCirculateEvent
//					ConfigureNotify	XConfigureEvent
//					DestroyNotify	XDestroyWindowEvent
//					GravityNotify	XGravityEvent
//					MapNotify	XMapEvent
//					ReparentNotify	XReparentEvent
//					UnmapNotify	XUnmapEvent
// SubstructureNotifyMask	CirculateNotify	XCirculateEvent
//N.A.	MappingNotify	XMappingEvent
//N.A.	SelectionClear	XSelectionClearEvent
//N.A.	SelectionNotify	XSelectionEvent
//N.A.	SelectionRequest	XSelectionRequestEvent

#define EVENTS_DISABLED_MASK	ExposureMask

//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
extern HDISPLAY XDpy;
extern HWND RootWnd;

static TObject *DragObject;
static TRect DragRect;
static bool DragAccept;

//---------------------------------------------------------------------------
//  TSizeConstraints
//---------------------------------------------------------------------------
TSizeConstraints :: TSizeConstraints(const TControl *owner) :
    m_MaxHeight(0), m_MaxWidth(0), m_MinHeight(0), m_MinWidth(0),
    m_owner(const_cast<TControl *>(owner))
{

}
//---------------------------------------------------------------------------
void TSizeConstraints :: set_MaxHeight(uint_t value)
{
    if(value != m_MaxHeight)
    {
        m_MaxHeight = value;
        if(value > 0 && value < m_MinHeight)
            m_MinHeight = value;
        Change();
    }
}
//---------------------------------------------------------------------------
void TSizeConstraints :: set_MaxWidth(uint_t value)
{
    if(value != m_MaxWidth)
    {
        m_MaxWidth = value;
        if(value > 0 && value < m_MinWidth)
            m_MinWidth = value;
        Change();
    }
}
//---------------------------------------------------------------------------
void TSizeConstraints :: set_MinHeight(uint_t value)
{
    if(value != m_MinHeight)
    {
        m_MinHeight = value;
        if(m_MaxHeight > 0 && value > m_MaxHeight)
            m_MaxHeight = value;
        Change();
    }
}
//---------------------------------------------------------------------------
void TSizeConstraints :: set_MinWidth(uint_t value)
{
    if(value != m_MinWidth)
    {
        m_MinWidth = value;
        if(m_MaxWidth > 0 && value > m_MaxWidth)
            m_MaxWidth = value;
        Change();
    }
}
//---------------------------------------------------------------------------
void TSizeConstraints :: Change()
{
    TWinControl *wc = dynamic_cast<TWinControl*>(m_owner);
    if(wc)
    {
        XSizeHints *hints = XAllocSizeHints();
        hints->flags = PMinSize | PMaxSize;
        hints->min_width = m_MinWidth;
        hints->min_height = m_MinHeight;
        hints->max_width = m_MaxWidth ? m_MaxWidth : INT_MAX;
        hints->max_height = m_MaxHeight ? m_MaxHeight : INT_MAX;
        XSetWMNormalHints(XDpy, wc->Handle, hints);
        XFree(hints);
    }
}
//---------------------------------------------------------------------------
//  TControl
//---------------------------------------------------------------------------
TControl :: TControl()
    : m_Constraints(this), m_Bounds(TRect(0,0,32,32))
{
    Init();
}
//---------------------------------------------------------------------------
TControl :: TControl(const TControl &a)
    : TObject(a), m_Constraints(this), m_Bounds(TRect(0,0,32,32))
{
    Assign(a);
}
//---------------------------------------------------------------------------
/*TControl :: TControl(TWinControl *AParent) :
	m_Constraints(this)
{
	Init();
	set_Parent(AParent);
} */
//---------------------------------------------------------------------------
TControl :: ~TControl()
{
    if(m_Parent)
        m_Parent->RemoveControl(this);
}
//---------------------------------------------------------------------------
__fastcall const TControl & TControl :: operator=(const TControl &a)
{
    if(this == &a)
        return *this;
    TObject :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void __fastcall TControl :: Assign(const TControl &a)
{
    Parent = a.m_Parent;
//private:
    m_Color = a.m_Color;
    m_Align = a.m_Align;
    m_Caption = a.m_Caption;
//   	TPoint       m_anchor;
//   	bool		m_AutoSize;
    m_Font = a.m_Font;
//	m_Constraints = a.m_Constraints;
    m_ControlState = a.m_ControlState;
    m_ControlStyle = a.m_ControlStyle;


//protected:
//	SetBounds(a.m_Left, a.m_Top, a.m_Width, a.m_Height);
    BoundsRect = a.BoundsRect;
    m_Bitmap = a.m_Bitmap;

    m_Anchors = a.m_Anchors;
    Cursor = a.Cursor;
    DragCursor = a.DragCursor;
    Text = a.Text;
    Hint = a.Hint;
    ShowHint = a.ShowHint;
    DragMode = a.DragMode;
    DragKind = a.DragKind;
    PopupMenu = a.PopupMenu;
    ImageIndex = a.ImageIndex;
    GroupIndex = a.GroupIndex;

    OnClick = a.OnClick;;
    OnContextPopup = a.OnContextPopup;
    OnDblClick = a.OnDblClick;
    OnTripleClick = a.OnTripleClick;
    OnResize = a.OnResize;
    OnMouseDown = a.OnMouseDown;
    OnMouseUp = a.OnMouseUp;
    OnMouseMove = a.OnMouseMove;
    OnMouseEnter = a.OnMouseEnter;
    OnMouseLeave = a.OnMouseLeave;
    OnStartDrag = a.OnStartDrag;
    OnEndDrag = a.OnEndDrag;
    OnDragOver = a.OnDragOver;

    /*
    	Active = a.Active;
       	property (m, TAlign, Align);
    	property (m, bool, AutoSize);
       	property (m, TBitmap *, Bitmap);
       	property (rw, TRect, BoundsRect);
       	property (mr, TCanvas *, Canvas);
       	property (m, TString, Caption);
       	property (rw, bool, Checked);
       	property (r, TRect, ClientRect);
       	property (m, TColor, Color);
       	property (r, TSizeConstraints *, Constraints);
       	property (m, TFont *, Font);
       	property (m, short, Left);
       	property (m, short, Top);
       	property (m, short, Width);
       	property (m, short, Height);
       	property (rw, bool, Enabled);
       	property (r, bool, Showing);
       	Visible = a.Visible;
    */
}
//---------------------------------------------------------------------------
void TControl :: Init()
{
    m_Parent = NULL;
    m_Color = clNone;
    m_Font = &Screen.Font;
    m_ControlState = 0;
    m_ControlStyle = 0;
    StateOn(csEnabled | csVisible | csLoading);
    m_Align = alNone;
    m_Cursor = crDefault;
    DragCursor = crDrag;
    m_Anchors = akLeft | akTop;
    DragMode = dmDragNone;
    DragKind = dkDrag;
    PopupMenu = NULL;
    m_GroupIndex = 0;
    ImageIndex = -1;
    m_Bitmap = NULL;
}
//---------------------------------------------------------------------------
#define UB_NOCHANGE			0
#define UB_CHANGE_POSITION	1
#define UB_CHANGE_SIZE		2

void  TControl :: set_BoundsRect(TRect rect)
{
    TRect old = m_Bounds.ClipRect;
    TRect newrect;
    TRect oldrect = rect;

    ApplyConstraints(oldrect, newrect);
    m_Bounds.ClipRect = newrect;

    if (m_Parent != NULL)
    {
        TPoint p = m_Parent->Canvas.Origin;
        p.x += newrect.left;
        p.y += newrect.top;
        Canvas.Origin = p;
    }
    else
    {
        Canvas.Origin = TPoint(newrect.left, newrect.top);
    };

    int tmp = UB_NOCHANGE;
    if(newrect.left != old.left || newrect.top != old.top)
        tmp = UB_CHANGE_POSITION;
    if(newrect.width != old.width || newrect.height != old.height)
        tmp |= UB_CHANGE_SIZE;

    if(tmp != UB_NOCHANGE)
        Repaint(); //Update();
    if(tmp & UB_CHANGE_SIZE)
        Resize();
}
//---------------------------------------------------------------------------
void TControl :: set_Left(int value)
{
    TRect b = get_BoundsRect();
    if(b.left != value)
    {
        b.left = value;
        set_BoundsRect(b);
    }
}
//---------------------------------------------------------------------------
void TControl :: set_Top(int value)
{
    TRect b = get_BoundsRect();
    if(b.top != value)
    {
        b.top = value;
        set_BoundsRect(b);
    }
}
//---------------------------------------------------------------------------
void TControl :: set_Width(uint_t value)
{
    TRect b = get_BoundsRect();
    if(b.width != value)
    {
        b.width = value;
        set_BoundsRect(b);
    }
}
//---------------------------------------------------------------------------
void TControl :: set_Height(uint_t value)
{
    TRect b = get_BoundsRect();
    if(b.height != value)
    {
        b.height = value;
        set_BoundsRect(b);
    }
}
//---------------------------------------------------------------------------
void TControl :: set_Bounds(TRegion rgn)
{
    m_Bounds = rgn;
}
//---------------------------------------------------------------------------
uint_t TControl :: get_ClientWidth()
{
    return (*ClientRect).width;
}
//---------------------------------------------------------------------------
void TControl :: set_ClientWidth(uint_t value)
{
    TRect crect = ClientRect;
    TRect brect = BoundsRect;
    BoundsRect = TRect(brect.left, brect.top, brect.width + value - crect.width, brect.height);
}
//---------------------------------------------------------------------------
uint_t TControl :: get_ClientHeight()
{
    return (*ClientRect).height;
}
//---------------------------------------------------------------------------
void TControl :: set_ClientHeight(uint_t value)
{
    TRect crect = ClientRect;
    TRect brect = BoundsRect;
    BoundsRect = TRect(brect.left, brect.top, brect.width, brect.height + value - crect.height);
}
//---------------------------------------------------------------------------
TRect TControl :: get_ClientRect()
{
    TRect ret = m_Bounds.ClipRect;
    ret.left = ret.top = 0;
    return ret;
}
//---------------------------------------------------------------------------
TRegion TControl :: get_ClientRegion()
{
    TRect rect = m_Bounds.ClipRect;
    TRegion ret = m_Bounds;
    ret.Offset(-rect.left, -rect.top);
    return ret;
}
//---------------------------------------------------------------------------
TCustomForm * TControl :: get_ParentForm()
{
    TWinControl *pw = m_Parent;
    TWinControl *rw = NULL;
    while(pw)
    {
        rw = pw;
        pw = pw->Parent;
    }
    return dynamic_cast<TCustomForm *>(rw);
}
//---------------------------------------------------------------------------
TPoint TControl :: get_ClientOrigin()
{
    TPoint ret;
    TRect b = get_BoundsRect();
    if( m_Parent )
        ret = m_Parent->ClientOrigin;
    ret.x += b.left;
    ret.y += b.top;
    return ret;
}
//---------------------------------------------------------------------------
void TControl :: set_Caption(TString value)
{
    if(m_Caption != value)
    {
        m_Caption = value;
        Invalidate();
    };
}
//---------------------------------------------------------------------------
void TControl :: set_Align(TAlign value)
{
    if(m_Align != value)
    {
        m_Align = value;
        Update();
    };
}
//---------------------------------------------------------------------------
void TControl :: set_AutoSize(bool value)
{
    if(AutoSize != value)
    {
        SetStyle(csAutoSize, value);
        AdjustSize();
    };
}
//---------------------------------------------------------------------------
void TControl :: set_Anchors(int value)
{
    if( m_Anchors != value)
    {
        m_Anchors = value;
//        TRect prect;
//        if(m_Parent == NULL)
//            prect = TRect(0, 0, CScreen.Width, CScreen.Height);
//        else
//            prect = m_Parent->ClientRect;
//		UpdateAnchor(prect);
    };
}
//---------------------------------------------------------------------------
void TControl :: set_Font(TFont *value)
{
    if(m_Font != value)
    {
        m_Font = value;
        Update();
    };
}
//---------------------------------------------------------------------------
void __fastcall TControl :: SetState(const TControlState value, bool on)
{
    if( on )
        m_ControlState |= value;
    else
        m_ControlState &= ~value;
}
//---------------------------------------------------------------------------
void TControl :: set_Enabled(bool value)
{
    if(Enabled != value)
    {
        SetState(csEnabled, value);
        Invalidate();
    };
}
/*
procedure TMenuItem.VerifyGroupIndex(Position: Integer; Value: Byte);
var
  I: Integer;
begin
  for I := 0 to GetCount - 1 do
    if I < Position then
    begin
      if Items[I].GroupIndex > Value then Error(@SGroupIndexTooLow)
    end
    else
      { Ripple change to menu items at Position and after }
      if Items[I].GroupIndex < Value then Items[I].FGroupIndex := Value;
end;
*/
//---------------------------------------------------------------------------
void TControl :: TurnSiblingsOff()
{
    TControl *item;

    if(m_Parent)
    {
        for(register int i = 0; i < m_Parent->ControlCount; i++)
        {
            item = m_Parent->Controls[i];
            if(item != this && item->RadioItem && item->GroupIndex == m_GroupIndex)
                item->Checked = false;
        }
    }
}
//---------------------------------------------------------------------------
void TControl :: set_Checked(bool value)
{
    if(Checked != value)
    {
        SetState(csChecked, value);
        if(value && RadioItem)
            TurnSiblingsOff();
        Invalidate();
    };
}
//---------------------------------------------------------------------------
void TControl :: set_RadioItem(bool value)
{
    if(RadioItem != value)
    {
        SetStyle(csRadioItem, value);
        if(Checked && RadioItem)
            TurnSiblingsOff();
    }
}
//---------------------------------------------------------------------------
void TControl :: set_GroupIndex(byte_t value)
{
    if(m_GroupIndex != value)
    {
//        if Parent <> nil then Parent.VerifyGroupIndex(Parent.IndexOf(Self), Value);
        m_GroupIndex = value;
        if(Checked && RadioItem)
            TurnSiblingsOff();
    }
}
//---------------------------------------------------------------------------
void TControl :: set_Active(bool value)
{
    if(Active != value)
    {
        SetState(csActive, value);
        Invalidate();
    };
}
//---------------------------------------------------------------------------
void TControl :: set_Default(bool value)
{
    if(Default != value)
    {
        if(value && m_Parent)
            for(register int i = 0; i < m_Parent->ControlCount; i++)
                if(m_Parent->Controls[i]->Default)
                    m_Parent->Controls[i]->Default = false;
        SetState(csDefault, value);
        Invalidate();
    };
}
//---------------------------------------------------------------------------
void TControl :: set_Selected(bool value)
{
    if(Selected != value)
    {
        SetState(csSelected, value);
        Invalidate();
    };
}
//---------------------------------------------------------------------------
void TControl :: set_Undefined(bool value)
{
    if(Undefined != value)
    {
        SetState(csUndefined, value);
        Invalidate();
    };
}
//---------------------------------------------------------------------------
void __fastcall TControl :: SetStyle(const TControlStyle value, bool on)
{
    if( on )
        m_ControlStyle |= value;
    else
        m_ControlStyle &= ~value;
}
//---------------------------------------------------------------------------
void TControl :: set_Transparent(bool value)
{
    if(Transparent != value)
    {
        SetStyle(csTransparent, value);
        Invalidate();
    };
}
//---------------------------------------------------------------------------
void TControl :: set_Bitmap(TBitmap *value)
{
    if(m_Bitmap != value)
    {
        m_Bitmap = value;
        Update();
    };
}
//---------------------------------------------------------------------------
void TControl :: set_Color(TColor value)
{
    if(m_Color != value)
    {
        m_Color = value;
        Update();
    };
}
//---------------------------------------------------------------------------
void TControl :: set_Parent(TWinControl *AParent)
{
    if (m_Parent != AParent && AParent != this)
    {
        if (m_Parent != NULL)
            m_Parent->RemoveControl(this);
        m_Parent = AParent;
        if (AParent != NULL)
        {
            AParent->InsertControl(this);

            Canvas.Handle = AParent->Handle;
            TPoint p = AParent->Canvas.Origin;
            TRect brect = BoundsRect;
            p.x += brect.left;
            p.y += brect.top;
            Canvas.Origin = p;
        }
        else
        {
            Canvas.Handle = Screen.Canvas->Handle;
            Canvas.Origin = TPoint(Left, Top);
        }
    }
}
//---------------------------------------------------------------------------
void TControl :: set_Visible(bool value)
{
    if(Visible != value)
    {
        if(value)
            DoShow();
        else
            DoHide();
        XSync(XDpy, false);
    };
}
//---------------------------------------------------------------------------
void TControl :: AdjustSize()
{
    if(!CheckState(csLoading))
    {
        TRect r = BoundsRect;
        BoundsRect = r;
    }
}
//---------------------------------------------------------------------------
void __fastcall TControl :: UpdateAnchor(const TRect &prect)
{
    TRect b = BoundsRect;
    m_anchor.x = prect.width - b.left - b.width + 1;
    m_anchor.y = prect.height - b.top - b.height + 1;
}
//---------------------------------------------------------------------------
void __fastcall TControl :: ApplyAnchorRules(TRect &newparentrect, TRect &outrect)
{
    TRect tmpr = BoundsRect;

    if(m_Parent == NULL)
    {
        outrect = tmpr;
        return;
    }

    if(m_anchor.x == 0 && m_anchor.y == 0)
    {
        outrect = tmpr;
        return;
    }

    int l, t, r, b;

    if(BIT_CHECK(m_Anchors,akLeft))
        l = tmpr.left;
    else
        l = newparentrect.width - m_anchor.x - tmpr.width + 1;

    if(BIT_CHECK(m_Anchors,akTop))
        t = tmpr.top;
    else
        t = newparentrect.height - m_anchor.y - tmpr.height + 1;

    if(BIT_CHECK(m_Anchors,akRight))
    {
        r = newparentrect.width - m_anchor.x;
        if(r < l)
            r = l;
    }
    else
        r = tmpr.right();

    if(BIT_CHECK(m_Anchors,akBottom))
    {
        b = newparentrect.height - m_anchor.y;
        if(b < t)
            b = t;
    }
    else
        b = tmpr.bottom();

    outrect.bounds(l,t,r,b);
}
//---------------------------------------------------------------------------
void __fastcall TControl :: ApplyConstraints(TRect &oldrect, TRect &newrect)
{
// Apply align

    TRect parentrect;
    if(m_Parent == NULL)
        parentrect = TRect(0, 0, Screen.Width, Screen.Height);
    else
        parentrect = m_Parent->ClientRect;

    newrect = parentrect.align(oldrect, m_Align);

// Apply constraints

    uint_t t = m_Constraints.MinWidth;
    if(t > 0 && newrect.width < t)
        newrect.width = t;

    t = m_Constraints.MaxWidth;
    if(t > 0 && newrect.width > t)
        newrect.width = t;

    t = m_Constraints.MinHeight;
    if(t > 0 && newrect.height < t)
        newrect.height = t;

    t = m_Constraints.MaxHeight;
    if(t > 0 && newrect.height > t)
        newrect.height = t;
}
//---------------------------------------------------------------------------
void TControl :: Paint()
{
    if(OnPaint)
        OnPaint(this);
    else
        Screen.PaintControl(*this);
}
//---------------------------------------------------------------------------
void TControl :: Update()
{
    if(m_Parent)
        m_Parent->Update();
}
//---------------------------------------------------------------------------
void TControl :: Repaint()
{
    if(m_Parent)
        m_Parent->PaintControl(this);
}
//---------------------------------------------------------------------------
void TControl :: Invalidate()
{
    if(m_Parent)
//        m_Parent->InvalidateRgn(m_Bounds);
        InvalidateRect(m_Parent->Handle, BoundsRect);
}
//---------------------------------------------------------------------------
void TControl :: DoShow()
{
    StateOn(csVisible);
    if(Parent != NULL && Parent->Showing)
    {
        StateOn(csShowing);
        Paint();
    }
}
//---------------------------------------------------------------------------
void TControl :: DoHide()
{
    StateOff(csVisible);
    StateOff(csShowing);
//	Parent->UpdateRect(BoundsRect);
    Parent->Update();
}
//---------------------------------------------------------------------------
void __fastcall TControl :: SetZOrder(bool topmost)
{
    if(m_Parent)
        m_Parent->MoveControl(this, topmost);
}
//---------------------------------------------------------------------------
void TControl :: Resize()
{
    if(OnResize)
        OnResize(this);
}
//---------------------------------------------------------------------------
void TControl :: Click()
{
    if(OnClick)
        OnClick(this);
}
//---------------------------------------------------------------------------
void TControl :: DblClick()
{
    if(OnDblClick)
        OnDblClick(this);
}
//---------------------------------------------------------------------------
void TControl :: TripleClick()
{
    if(OnTripleClick)
        OnTripleClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TControl :: MouseMove(TShiftState state, int x, int y)
{
    if(DragObject != NULL)
    {
        DragAccept = DragOver(DragObject, state, x, y, dsDragMove);
        return;
    }
    else
    {
        if(CheckState(csMouseDown) && DragMode != dmDragNone)
        {
            BeginDrag(state, x, y);
            return;
        }
    }
    if(!OnMouseMove.empty())
        OnMouseMove(this, state, x, y);
}
//---------------------------------------------------------------------------
void __fastcall TControl :: MouseDown(TMouseButton button, TShiftState state, int x, int y)
{
    bool handled = false;
    /*
    	if(CScreen.PopupActive)
    	{
    		TCustomForm *form = get_ParentForm();
    		if(!dynamic_cast<TCustomPopupMenu *>(form ? form : this))
    			CScreen.ClosePopup();
    	}
    */
//	if(Enabled)
//	{
//		if(!Hint.empty())
//			Application.HideHint();
//		if(button == Button1 && (state & ~(Mod2Mask | LockMask)) == 0)
//			StateOn(csLButtonDown);
//		StateOn(csMouseDown);
    DragRect.left = x;
    DragRect.top = y;
//		if(CheckStyle(csButton))
//   		{
//    		if(button == Button1)
//  			{
//  				Active = true;
//  				Paint();
//  			}
//  		}
    if(OnMouseDown)
    {
        OnMouseDown(this, button, state, x, y);
        handled = true;
    }
//    }
//	if(m_Parent && !handled)
//		m_Parent->MouseDown(button, state, m_Left + x, m_Top + y);

    if(m_Parent && !handled)
    {
        TRect r = m_Parent->ClientRect;
        TRect b = BoundsRect;
        int x1 = b.left + x;// - r.left;
        int y1 = b.top + y;// - r.top;
        if(r.inside(b.left + x, b.top + y))
            m_Parent->MouseDown(button, state, x1, y1);
    }
}
//---------------------------------------------------------------------------
void __fastcall TControl :: MouseUp(TMouseButton button, TShiftState state, int x, int y)
{
    bool handled = false;

//	if((state & ~(Mod2Mask | LockMask)) == Button1Mask)
//	{
//		StateOff(csLButtonDown);
//	}
//	StateOff(csMouseDown);

//	if(Enabled)
//	{
    if(DragObject != NULL)
        EndDrag(state, x, y);

//   		if(CheckStyle(csButton))
//   		{
//   			if(button == Button1)
//   			{
//   			    Active = false;
//   			    Paint();
//   			}
//   		}
    if(button == mbRight)
    {
        if(!OnContextPopup.empty())
            handled = OnContextPopup(this, x, y);
        if(!handled)
        {
            if(PopupMenu && PopupMenu->AutoPopup)
            {
                TPoint p;
                p.x = x;
                p.y = y;
                p = ClientToScreen(p);
                PopupMenu->Popup(p.x, p.y);
                handled = true;
            }
        }
//   			handled = true;
    }
    /*   	if(button == mbRight)
       	{
       		bool handled = false;

       		if(!OnContextPopup.empty())
        		handled = OnContextPopup(this, x, y);

        	if(!handled)
        	{
       			TControl *wc = this;
       			do{
       				if(wc->PopupMenu)
       				{
       					if(wc->PopupMenu->AutoPopup)
       					{
       						wc->PopupMenu->Popup(wc, x, y);
       						break;
       					}
       				}
       				wc = wc->Parent;
    			}while(wc);
    		}
     	}
    */
    if(!OnMouseUp.empty())
    {
        OnMouseUp(this, button, state, x, y);
        handled = true;
    }
//	}
//	if(m_Parent && !handled)
//		m_Parent->MouseUp(button, state, m_Left + x, m_Top + y);

    if(m_Parent && !handled)
    {
        TRect r = m_Parent->ClientRect;
        TRect b = BoundsRect;
        int x1 = b.left + x;// - r.left;
        int y1 = b.top + y;// - r.top;
        if(r.inside(b.left + x, b.top + y))
            m_Parent->MouseUp(button, state, x1, y1);
    }
}
//---------------------------------------------------------------------------
void __fastcall TControl :: MouseEnter(TShiftState state, int x, int y)
{
    StateOn(csMouseOver);
    if(CheckStyle(csFollowMouse))
    {
        Paint();
    }
    if(!dynamic_cast<TWinControl*>(this) && m_Parent != NULL && m_Cursor != crDefault)
    {
        XDefineCursor(XDpy, m_Parent->Handle, Screen.Cursors[m_Cursor]);
    }
    if(state & (Button1Mask | Button2Mask | Button3Mask))
    {
        DragAccept = DragOver(DragObject, state, x, y, dsDragEnter);
    }
    if(!Hint.empty())
        Application.ActivateHint(this);
    if(OnMouseEnter)
        OnMouseEnter(this, state, x, y);
}
//---------------------------------------------------------------------------
void __fastcall TControl :: MouseLeave(TShiftState state, int x, int y)
{
    StateOff(csMouseOver);
    StateOff(csMouseDown);
    StateOff(csLButtonDown);
    if(!Hint.empty())
        Application.HideHint();
    if(CheckStyle(csFollowMouse))
    {
        Paint();
    }
    else if(CheckStyle(csButton))
    {
        if((state & ~(Mod2Mask | LockMask)) == Button1Mask)
            Paint();
    }
    if(!dynamic_cast<TWinControl*>(this) && m_Parent != NULL && Cursor != crDefault)
    {
        XDefineCursor(XDpy, m_Parent->Handle, Screen.Cursors[m_Parent->Cursor]);
    }
    if(state & (Button1Mask | Button2Mask | Button3Mask))
    {
        DragAccept = DragOver(DragObject, state, x, y, dsDragExit);
    }
    if(OnMouseLeave)
        OnMouseLeave(this, state, x, y);
}
//---------------------------------------------------------------------------
bool __fastcall TControl :: DispatchMessage(XEvent &event)
{
    bool ret = false;
    TMessage msg;
//   TObject *obj;

    msg.Msg = event.xclient.message_type;
    for(register int i = 0; i < 5; i++)
        msg.Params[i] = event.xclient.data.l[i];
    switch (msg.Msg)
    {
    case CM_FONTCHANGED:
//         obj = (TObject *)(msg.Params[0]);
//         if(obj == m_Font)
        Update();
        break;
    }
    return ret;
}
//---------------------------------------------------------------------------
bool __fastcall TControl :: WndProc(XEvent &event)
{
    static Time xtime, xtime1;
    int x, y;
    TRegion clireg = ClientRegion;
    TRect prect;
    if(m_Parent == NULL)
        prect = TRect(0, 0, Screen.Width, Screen.Height);
    else
        prect = m_Parent->ClientRect;
//    TRect clirect = clireg.ClipRect;
    bool ret = false;

    if(event.type == UnmapNotify)
    {
        StateOff(csShowing);
        return false;
    }
    if(!Visible)
//	if(!Showing)
        return false;

    switch (event.type)
    {
    case MapNotify:
        StateOn(csShowing);
        StateOff(csLoading);
        UpdateAnchor(prect);
        break;
//      case UnmapNotify:
//			StateOff(csShowing);
//			ret = true;
//            break;
    case ButtonPress:
        x = event.xbutton.x - Canvas.Origin.x;
        y = event.xbutton.y - Canvas.Origin.y;
        if(clireg.InRegion(x, y) || CheckState(csGrabMouse))
        {
            if(Screen.PopupActive)
            {
                TCustomForm *form = get_ParentForm();
                if(!dynamic_cast<TCustomPopupMenu *>(form ? form : this))
                    Screen.ClosePopup();
            }
            if(Enabled)
            {
                if(CheckStyle(csAllowRepeat))
                {
                    if(event.xany.send_event == false)
                    {
                        StateOn(csMouseRepeat);
                        XSendEvent(XDpy, PointerWindow, false, ButtonPressMask, &event);
                        xtime = event.xbutton.time;
                    }
                    else
                    {
                        if(CheckState(csMouseRepeat))
                        {
                            event.xbutton.time += 100;
                            XSendEvent(XDpy, PointerWindow, false, ButtonPressMask, &event);
                            if(event.xbutton.time - xtime < 1000)
                            {
                                return true;
                            }
                            else
                                xtime = event.xbutton.time;
                        }
                        else
                            return true;
                    }
                }
                if(!Hint.empty())
                    Application.HideHint();
                StateOn(csMouseDown);
                if(event.xbutton.button == XButton1 && (event.xbutton.state & Button1Mask) == 0)
                    StateOn(csLButtonDown);
                if(CheckStyle(csButton) && event.xany.send_event == false)
                {
                    if(event.xbutton.button == XButton1)
                    {
                        Active = true;
                        Paint();
                    }
                }
                MouseDown(TMouseButton(event.xbutton.button), event.xbutton.state, x, y);
                ret = true;
            }
        }
        break;
    case ButtonRelease:
        x = event.xbutton.x - Canvas.Origin.x;
        y = event.xbutton.y - Canvas.Origin.y;
        if(clireg.InRegion(x, y) || CheckState(csGrabMouse))
        {
            if(Enabled)
            {
                bool flag = CheckState(csLButtonDown);
                if(CheckState(csMouseRepeat))
                {
                    StateOff(csMouseRepeat);
                    XEvent xe1;
                    XNextEvent(XDpy, &xe1);
                }
                StateOff(csMouseDown);

                MouseUp(TMouseButton(event.xbutton.button), event.xbutton.state, x, y);

                if((event.xbutton.state & Button1Mask) == Button1Mask)
                {
                    StateOff(csLButtonDown);
                }

                if(CheckStyle(csButton))
                {
                    if(event.xbutton.button == XButton1)
                    {
//                           Active = false;
                        Paint();
                    }
                }

                if(flag)
                {
                    if(event.xbutton.time - xtime1 < 2 * Mouse.DblClickThreshold)
                        TripleClick();
                    else if(event.xbutton.time - xtime < Mouse.DblClickThreshold)
                        DblClick();
                    else
                        Click();
                    xtime1 = xtime;
                    xtime = event.xbutton.time;
                }

                if(CheckStyle(csButton))
                {
                    if(event.xbutton.button == XButton1)
                    {
                        Paint();
                    }
                }
                ret = true;
            }
//				MouseUp(TMouseButton(event.xbutton.button), event.xbutton.state,
//                   		event.xbutton.x-clirect.left,
//                   		event.xbutton.y-clirect.top);
//             	ret = true;
        }
        break;
    case MotionNotify:
        if(!Enabled)
            break;
        x = event.xbutton.x - Canvas.Origin.x;
        y = event.xbutton.y - Canvas.Origin.y;
        if(clireg.InRegion(x, y) || CheckState(csGrabMouse))
        {
            if(CheckState(csMouseOver))
            {
                MouseMove(event.xmotion.state, x, y);
            }
            else
            {
                TControl *tmpc = NULL;
                TWinControl *tmpw = get_ParentForm();
                if(tmpw == NULL)
                    tmpw = dynamic_cast<TWinControl*>(this);
                if(tmpw)
                    tmpc = tmpw->ControlOnState(csMouseOver);
                if(tmpc)
                    tmpc->MouseLeave(event.xmotion.state, x, y);
                MouseEnter(event.xmotion.state, x, y);
            }
            ret = true;
        }
        break;
    case EnterNotify:
        x = event.xbutton.x - Canvas.Origin.x;
        y = event.xbutton.y - Canvas.Origin.y;
        if(clireg.InRegion(x, y))
        {
            MouseEnter(event.xcrossing.state, x, y);
            ret = true;
        }
        break;
    case LeaveNotify:
        x = event.xbutton.x - Canvas.Origin.x;
        y = event.xbutton.y - Canvas.Origin.y;
        if(CheckState(csMouseOver))
        {
            MouseLeave(event.xcrossing.state, x, y);
            ret = true;
        }
        break;
    case ConfigureNotify:
        break;
    case ClientMessage:
        printf("CLIENTMESSAGE id=%d\n",id);
        ret = DispatchMessage(event);
        break;
    }
    return ret;
}
//---------------------------------------------------------------------------
void TControl :: BeginDrag(unsigned int state, int x, int y)
{
    uint_t threshold = Mouse.DragThreshold;

    if(dynamic_cast<TCustomForm*>(this) && DragKind != dkDock)
        return;

    if(DragObject == NULL)
    {
        if(DragRect.left > x)
            DragRect.width = DragRect.left - x;
        else
            DragRect.width = x - DragRect.left;

        if(DragRect.top > y)
            DragRect.height = DragRect.top - y;
        else
            DragRect.height = y - DragRect.top;

        if(DragRect.width > threshold || DragRect.height > threshold)
        {
            DragObject = this;
            if(!OnStartDrag.empty())
                OnStartDrag(this, DragObject, state, x, y);
//         ControlState.insert(csDrag);
        }
    };
}
//---------------------------------------------------------------------------
void TControl :: EndDrag(unsigned int state, int x, int y)
{
    if(!DragAccept)
        DragObject = NULL;
    if(!OnEndDrag.empty())
        OnEndDrag(this, DragObject, state, x, y);
//   ControlState.erase(csDrag);
    DragObject = NULL;
}
//---------------------------------------------------------------------------
bool TControl :: DragOver(TObject *source, unsigned int state, int x, int y,
                          TDragState dstate)
{
    bool accept = (!OnDragOver.empty()) || (!OnEndDrag.empty());
    if(!OnDragOver.empty())
        accept = OnDragOver(this, source, state, x, y, dstate);
    return accept;
}
//---------------------------------------------------------------------------
void TControl :: Scale(int MV, int DV, int MH, int DH)
{
    int X, Y, W, H;

    if(MV != DV || MH != DH)
    {
        X = Left * MH / DH;
        Y = Top * MV / DV;
        if(CheckStyle(csFixedWidth))
            W = Width;
        else
            W = (Left + Width) * MH / DH - X;

        if(CheckStyle(csFixedHeight))
            H = Height;
        else
            H = (Top + Height) * MV / DV - Y;

        BoundsRect = TRect(X, Y, W, H);

//    if not ParentFont and (sfFont in Flags) then
//      Font.Size := MulDiv(Font.Size, MV, DV);
    };
}
//---------------------------------------------------------------------------
TPoint __fastcall TControl :: ClientToScreen(const TPoint &point)
{
    TPoint ret = get_ClientOrigin();
    ret.x += point.x;
    ret.y += point.y;
    return ret;
}
//---------------------------------------------------------------------------
TPoint __fastcall TControl :: ScreenToClient(const TPoint &point)
{
    TPoint ret = get_ClientOrigin();
    ret.x = point.x - ret.x;
    ret.y = point.y - ret.y;
    return ret;
}
//---------------------------------------------------------------------------
TPoint __fastcall TControl :: ClientToParent(const TPoint &point, TWinControl *AParent)
{
    TWinControl *LParent;
    TPoint ret;

    if(AParent == NULL)
        AParent = m_Parent;
//    if(AParent == NULL)
//        raise EInvalidOperation.CreateFmt(SParentRequired, [Name]);

    ret = point;
    TRect b = get_BoundsRect();
    ret.x += b.left;
    ret.y += b.top;
    LParent = m_Parent;
    while( LParent )
    {
        if( LParent->Parent )
        {
            b = LParent->BoundsRect;
            ret.x += b.left;
            ret.y += b.top;
        };
        if( LParent == AParent )
            break;
        else
            LParent = LParent->Parent;
    };
//  if LParent = nil then
//    raise EInvalidOperation.CreateFmt(SParentGivenNotAParent, [Name]);

    return ret;
}
//---------------------------------------------------------------------------
TPoint __fastcall TControl :: ParentToClient(const TPoint &point, TWinControl *AParent)
{
    TWinControl *LParent;

    if( AParent == NULL)
        AParent = m_Parent;
//    if( AParent == NULL)
//        raise EInvalidOperation.CreateFmt(SParentRequired, [Name]);

    TPoint ret;
    ret = point;
    TRect b = get_BoundsRect();
    ret.x -= b.left;
    ret.y -= b.top;
    LParent = m_Parent;
    while( LParent )
    {
        if( LParent->Parent )
        {
            b = LParent->BoundsRect;
            ret.x -= b.left;
            ret.y -= b.top;
        };
        if( LParent == AParent )
            break;
        else
            LParent = LParent->Parent;
    }
//    if( LParent == NULLL )
//        raise EInvalidOperation.CreateFmt(SParentGivenNotAParent, [Name]);

    return ret;
};
//---------------------------------------------------------------------------
//  TButtonControl
//---------------------------------------------------------------------------
TButtonControl :: TButtonControl()
{
    Init();
};
//---------------------------------------------------------------------------
TButtonControl :: ~TButtonControl()
{
};
//---------------------------------------------------------------------------
__fastcall TButtonControl :: TButtonControl(const TButtonControl &a) : TControl(a)
{
    Assign(a);
};
//---------------------------------------------------------------------------
__fastcall const TButtonControl & TButtonControl :: operator=(const TButtonControl &a)
{
    if(this == &a)
        return *this;
    TControl :: operator=(a);
    Assign(a);
    return *this;
};
//---------------------------------------------------------------------------
void TButtonControl :: Init()
{
    StyleOn(csButton);
    StyleOn(csFollowMouse);
};
//---------------------------------------------------------------------------
__fastcall void TButtonControl :: Assign(const TButtonControl &obj)
{
};
//---------------------------------------------------------------------------
//  TPainter
//---------------------------------------------------------------------------
TPainter :: TPainter()
{
    Init();
};
//---------------------------------------------------------------------------
TPainter :: ~TPainter()
{
};
//---------------------------------------------------------------------------
__fastcall TPainter :: TPainter(const TPainter &a) : TObject(a)
{
    Assign(a);
};
//---------------------------------------------------------------------------
__fastcall const TPainter & TPainter :: operator=(const TPainter &a)
{
    if(this == &a)
        return *this;
    TObject :: operator=(a);
    Assign(a);
    return *this;
};
//---------------------------------------------------------------------------
void TPainter :: Init()
{
    m_Canvas = NULL;
};
//---------------------------------------------------------------------------
__fastcall void TPainter :: Assign(const TPainter &obj)
{
};
//---------------------------------------------------------------------------
//  TWinControl
//---------------------------------------------------------------------------
TWinControl :: TWinControl()
{
    m_Handle = 0;
    Init();
};
//---------------------------------------------------------------------------
TWinControl :: ~TWinControl()
{
    if(Parent)
        Parent->RemoveControl(this);

    TControl *tmp;
    while(m_Controls.size())
    {
        tmp = reinterpret_cast<TControl *>(m_Controls.front());
        tmp->Parent = NULL;
    }

    if(!m_statichandle)
        DestroyHandle();
}
//---------------------------------------------------------------------------
__fastcall TWinControl :: TWinControl(const TWinControl &a) : TControl(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TWinControl & TWinControl :: operator=(const TWinControl &a)
{
    if(this == &a)
        return *this;
    TControl :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TWinControl :: Init()
{
    m_width = 0;
    m_height = 0;
    m_statichandle = false;
    m_Bitmap = NULL;
    TabOrder = -1;
    TabStop = false;
    if(m_Handle == 0)
        CreateHandle();
    Canvas.Handle = m_Handle;
    Color = clBtnFace;
    m_BorderColor = clNone;

    XClientMessageEvent e;
    e.type = ClientMessage;
    e.window = m_Handle;
    e.message_type = CreateNotify;
    e.format = 32;
    XSendEvent(XDpy, m_Handle, false, StructureNotifyMask, (XEvent *)&e);

    XShapeSelectInput(XDpy, m_Handle, ShapeNotifyMask);
}
//---------------------------------------------------------------------------
void __fastcall TWinControl :: Assign(const TWinControl &obj)
{
//	m_Controls = a.m_Controls;

    TControl *tmpc, *tmpd;
    TWinControl *tmpw, *tmpv;

//   	for(TList::const_iterator from = a.m_Controls.begin();
//       from != a.m_Controls.end();
//       ++from)
    printf("COUNT %d\n", *obj.ControlCount);
    int count = obj.ControlCount;
    for(register int i = 0; i < count; i++)
    {
        tmpc = const_cast<TWinControl &>(obj).Controls[i]; //(TControl *)(*from);
//		tmpc = (TControl *)a.m_Controls[i];
//		tmpc = a.Controls[i];
        tmpd = (TControl *)TClass(tmpc->ClassType()).Create();
        tmpw = dynamic_cast<TWinControl *>(tmpd);
        if(tmpw)
        {
            tmpv = dynamic_cast<TWinControl *>(tmpc);
            *tmpw = *tmpv;
        }
        else
            *tmpd = *tmpc;
        tmpd->Parent = this;
    }

//private:
//	bool m_statichandle;
    /*   	TRegion m_updatergn;
       	TColor m_BorderColor;
       	TBitmap *m_BorderBitmap;

    protected:
    	HWND m_Handle;

       TNotifyEvent OnEnter;
       TNotifyEvent OnExit;
       TKeyEvent OnKeyUp;
       TKeyEvent OnKeyDown;
       TKeyPressEvent OnKeyPress;

    	property (rw, bool, GrabKeyboard);
    	property (rw, bool, GrabMouse);
    	property (rw, bool, SaveUnder);
    	property (rw, TBackingStore, BackingStore);

    public:
       	short TabOrder;
       	bool TabStop;

       	property (m, TBitmap *, BorderBitmap);
       	property (rw, uint_t, BorderWidth);
       	property (m, TColor, BorderColor);
       	property_i (r, TControl *, int, Controls);
       	property (r, short, ControlCount);
       	property (mr, HWND, Handle);
       	property (rw, HWND, ParentWindow);
     */
}
//---------------------------------------------------------------------------
__fastcall TWinControl :: TWinControl(HWND win)
{
    if(win == 0)
        win = Screen.Root;
    m_Handle = win;
    Init();
    m_statichandle = true;
    XWindowAttributes winattrib;

    XGetWindowAttributes(XDpy, m_Handle, &winattrib);
    Visible = (winattrib.map_state == IsUnmapped) ? false : true;

    printf("ROOTWINDOW %lu, %d %d %d %d %d %ld\n", m_Handle, winattrib.x, winattrib.y, winattrib.width, winattrib.height,
           winattrib.map_state, winattrib.do_not_propagate_mask);
}
//---------------------------------------------------------------------------
void TWinControl :: CreateHandle()
{
//	int propagate = KeyPressMask | KeyReleaseMask;// | ButtonPressMask | ButtonReleaseMask |
//						 PointerMotionMask | Button1MotionMask | Button2MotionMask |
//						 Button3MotionMask | Button4MotionMask | Button5MotionMask |
//						 ButtonMotionMask;

    unsigned long valuemask = CWEventMask | CWBitGravity | // CWDontPropagate |
                              CWWinGravity;
    XSetWindowAttributes attributes;
    attributes.event_mask = EVENTS_ENABLED_MASK;
//	attributes.do_not_propagate_mask = propagate;
    attributes.bit_gravity = NorthWestGravity; // StaticGravity; ForgetGravity; NorthWestGravity;
    attributes.win_gravity = NorthWestGravity;
    HWND parentwin = (Parent) ? Parent->Handle : *Screen.Root;
    m_Handle = XCreateWindow(XDpy, parentwin, 0, 0, 32, 32, 0, CopyFromParent,
                             InputOutput, CopyFromParent, valuemask, &attributes);

    TXExtension *ext;
    for(TList::iterator from = Application.Extensions.begin();
            from != Application.Extensions.end();
            ++from)
    {
        ext = reinterpret_cast<TXExtension *>(*from);
        ext->Bind(this);
    }
}
//---------------------------------------------------------------------------
long TWinControl :: get_EventMask()
{
    XWindowAttributes window_attributes;
    XGetWindowAttributes(XDpy, m_Handle, &window_attributes);
    return window_attributes.your_event_mask;
}
//---------------------------------------------------------------------------
void TWinControl :: set_EventMask(long value)
{
    XSelectInput(XDpy, m_Handle, value);
}
//---------------------------------------------------------------------------
TRect TWinControl :: get_ClientRect()
{
    TRect ret;
    GetClientRect(m_Handle, ret);
    ret.width -= Canvas.Origin.x;
    ret.height -= Canvas.Origin.y;
    return ret;
}
//---------------------------------------------------------------------------
TPoint TWinControl :: get_ClientOrigin()
{
    TPoint ret;
    Window dummy;
    int dest_x_return, dest_y_return;

    XTranslateCoordinates(XDpy, m_Handle, Screen.Root, 0, 0, &dest_x_return,
                          &dest_y_return, &dummy);
    ret.x = dest_x_return + Canvas.Origin.x;
    ret.y = dest_y_return + Canvas.Origin.y;
    return ret;
}
//---------------------------------------------------------------------------
void __fastcall TWinControl :: DoConfigure(XEvent &event)
{
    uint_t neww, newh;
    TRect rect, rect1;
    TControl *tmpc;
    TWinControl *tmpw;

    neww = event.xconfigure.width - Canvas.Origin.x;
    newh = event.xconfigure.height - Canvas.Origin.y;

    rect = TRect(0, 0, neww, newh);

    if(m_width != neww || m_height != newh)
    {
        TRegion updatergn;
        updatergn += rect;
        for(TList::iterator from = m_Controls.begin();
                from != m_Controls.end();
                ++from)
        {
            tmpc = static_cast<TControl *>(*from);
            tmpw = dynamic_cast<TWinControl *>(tmpc);
            tmpc->ApplyAnchorRules(rect, rect1);
            tmpc->BoundsRect = rect1;
            if(!tmpw)
                if(tmpc->Showing)
                {
                    //                    rect1 = tmpc->BoundsRect;
                    //                    rect1.left -= Canvas.Origin.x;
                    //                    rect1.top -= Canvas.Origin.y;
                    updatergn -= tmpc->BoundsRect;
                }
        }
//                   if(m_width > neww || m_height > newh)
//                   {
        if(CheckState(csShowing))
        {
//                            m_updatergn.Offset(-Canvas.Origin.x, -Canvas.Origin.y);
            Canvas.ClipMask = updatergn;
            Paint();
            Canvas.RemoveClipMask();
        }
//                   }
        Resize();
        /*
        if(m_width > neww || m_height > newh)
                            {
                    		   	XEvent event;
                                event.type = Expose;
                                event.xexpose.display = XDpy;
                                event.xexpose.window = m_Handle;
                                event.xexpose.x = neww - 5;
                                event.xexpose.y = 0;
                                event.xexpose.width = 5;
                                event.xexpose.height = newh;
                                event.xexpose.count = 0;
                                XPutBackEvent(XDpy, &event);
                                event.xexpose.x = 0;
                                event.xexpose.y = newh - 5;
                                event.xexpose.width = neww;
                                event.xexpose.height = 5;
                                event.xexpose.count = 1;
                                XPutBackEvent(XDpy, &event);
                            }
        */
        m_width = neww;
        m_height = newh;
    }
}
//---------------------------------------------------------------------------
void TWinControl :: set_BoundsRect(TRect rect)
{
//    TRect old = BoundsRect;
    TRect newrect;
    ApplyConstraints(rect, newrect);
//    if(old == newrect)
//        return;
    m_Bounds.ClipRect = newrect;
    if(m_Parent)
    {
        newrect.left += m_Parent->Canvas.Origin.x;
        newrect.top += m_Parent->Canvas.Origin.y;
    }
    MoveWindow(m_Handle, newrect.left, newrect.top, newrect.width, newrect.height);
}
//---------------------------------------------------------------------------
HWND TWinControl :: get_ParentWindow()
{
    Window root;
    Window parent;
    Window *children;
    unsigned int nchildren;
    XQueryTree(XDpy, m_Handle, &root, &parent, &children, &nchildren);
    XFree(children);
    return parent;
}
//---------------------------------------------------------------------------
void TWinControl :: set_ParentWindow(HWND value)
{
    if(get_ParentWindow() != value)
    {
        TRect b = BoundsRect;
        XReparentWindow(XDpy, m_Handle, value ? value : Screen.Root, b.left, b.top);
    }
}
//---------------------------------------------------------------------------
void TWinControl :: set_Parent(TWinControl *AParent)
{
    if (m_Parent != AParent && AParent != this)
    {
        TRect brect = BoundsRect;
        if (m_Parent != NULL)
            m_Parent->RemoveControl(this);
        m_Parent = AParent;
        if (AParent != NULL)
        {
            if(get_ParentWindow() != AParent->Handle)
            {
                TPoint p = AParent->Canvas.Origin;
                p.x += brect.left;
                p.y += brect.top;

                XReparentWindow(XDpy, m_Handle, AParent->Handle, p.x, p.y);
                AParent->InsertControl(this);
            }
        }
        else
        {
            XReparentWindow(XDpy, m_Handle, Screen.Root, brect.left, brect.top);
        }
    }
}
//---------------------------------------------------------------------------
void TWinControl :: set_Enabled(bool value)
{
    if(Enabled != value)
    {
        long mask = value ? EVENTS_ENABLED_MASK : EVENTS_DISABLED_MASK;
        set_EventMask(mask);
        TControl :: set_Enabled(value);
    };
}
//---------------------------------------------------------------------------
void TWinControl :: set_Color(TColor value)
{
    if(Color != value)
    {
        if(value == clNone)
            XSetWindowBackgroundPixmap(XDpy, m_Handle, None);
        else
            XSetWindowBackground(XDpy, m_Handle, value);
        if(Showing)
            XClearArea(XDpy, m_Handle, 0, 0, 0, 0, true);
        TControl :: set_Color(value);
    }
}
//---------------------------------------------------------------------------
void TWinControl :: set_BorderColor(TColor value)
{
    if(m_BorderColor != value)
    {
        if(value == clNone)
            XSetWindowBorderWidth(XDpy, m_Handle, 0);
        else
            XSetWindowBorder(XDpy, m_Handle, value);
        m_BorderColor = value;
        Update();
    }
}
//---------------------------------------------------------------------------
uint_t TWinControl :: get_BorderWidth()
{
    XWindowAttributes window_attributes_return;
    XGetWindowAttributes(XDpy, m_Handle, &window_attributes_return);
    return window_attributes_return.border_width;
}
//---------------------------------------------------------------------------
void TWinControl :: set_BorderWidth(uint_t value)
{
    if(get_BorderWidth() != value)
    {
        XSetWindowBorderWidth(XDpy, m_Handle, value);
        Update();
    }
}
//---------------------------------------------------------------------------
void TWinControl :: set_Bitmap(TBitmap *value)
{
    if(m_Bitmap != value)
    {
        if(value)
            XSetWindowBackgroundPixmap(XDpy, m_Handle, value->Handle);
        else
        {
            if(Color == clNone)
                XSetWindowBackgroundPixmap(XDpy, m_Handle, None);
            else
                XSetWindowBackground(XDpy, m_Handle, Color);
        }
        if(Showing)
            XClearArea(XDpy, m_Handle, 0, 0, 0, 0, true);
        m_Bitmap = value;
    }
}
//---------------------------------------------------------------------------
void TWinControl :: set_BorderBitmap(TBitmap *value)
{
    if(m_BorderBitmap != value)
    {
        if(value)
            XSetWindowBorderPixmap(XDpy, m_Handle, value->Handle);
        else
        {
            if(m_BorderColor == clNone)
                XSetWindowBorderPixmap(XDpy, m_Handle, None);
            else
                XSetWindowBorder(XDpy, m_Handle, m_BorderColor);
        }
        m_BorderBitmap = value;
    }
}
//---------------------------------------------------------------------------
void TWinControl :: InsertControl(TControl *control)
{
    m_Controls.Add(control);
}
//---------------------------------------------------------------------------
void TWinControl :: RemoveControl(TControl *control)
{
    m_Controls.Delete(control);
}
//---------------------------------------------------------------------------
void TWinControl :: Broadcast(XEvent &event)
{
    TWinControl *tmpctrl;
    TControl *tmpc;

    for(TList::iterator from = m_Controls.begin();
            from != m_Controls.end();
            ++from)
    {
        tmpc = (TControl *)(*from);
        tmpctrl = dynamic_cast<TWinControl *>(tmpc);
        if(tmpctrl != NULL)
            tmpctrl->Broadcast(event);
//		else
//         tmpc->WndProc(event);
    }
    WndProc(event);
}
//---------------------------------------------------------------------------
void TWinControl :: Create()
{
    printf("CREATEWIN id=%d\n", id);
    if(OnCreate)
        OnCreate(this);
}
//---------------------------------------------------------------------------
void TWinControl :: DoShow()
{
    TWinControl *tmpw;
    int count;

    StateOn(csVisible);
    XMapWindow(XDpy, m_Handle);
    if(CheckState(csGrabKeyboard))
    {
        count = XGrabKeyboard(XDpy, m_Handle, false,
                              GrabModeAsync, GrabModeAsync,
                              CurrentTime);
        if(count)
            StateOff(csGrabKeyboard);
    };
    if(CheckState(csGrabMouse))
    {
        count = XGrabPointer(XDpy, m_Handle, false,
                             ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
                             LeaveWindowMask | EnterWindowMask,
                             GrabModeAsync, GrabModeAsync,
                             None, None, CurrentTime);

//		count = XGrabButton(XDpy, AnyButton, AnyModifier, m_Handle, false, ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
//                     LeaveWindowMask | EnterWindowMask,
//                GrabModeAsync, GrabModeAsync, None, None);

        if(count)
            StateOff(csGrabMouse);
    };
    for(TList::iterator from = m_Controls.begin();
            from != m_Controls.end();
            ++from)
    {
        tmpw = dynamic_cast<TWinControl *>((TControl *)(*from));
        if(tmpw != NULL && tmpw->Visible)
            tmpw->DoShow();
    }
}
//---------------------------------------------------------------------------
void TWinControl :: DoHide()
{
    StateOff(csVisible);
    XUnmapWindow(XDpy, m_Handle);
}
//---------------------------------------------------------------------------
void TWinControl :: Invalidate()
{
//    InvalidateRgn(ClientRegion);
    InvalidateRect(m_Handle, ClientRect);
}
//---------------------------------------------------------------------------
void TWinControl :: PaintControl(TControl *ctrl)
{
//	TWinControl *tmpctrl;
    TControl *tmpc;
    TRegion updatergn;

    if(!ctrl->Showing)
        return;
//	tmpctrl = dynamic_cast<TWinControl *>(ctrl);
//	if(!tmpctrl)
//	{
    updatergn += ctrl->BoundsRect;

    for(TList::reverse_iterator from = m_Controls.rbegin();
            *from != ctrl;
            ++from)
    {
        tmpc = (TControl *)(*from);
        if(!tmpc->Showing)
            continue;
//			tmpctrl = dynamic_cast<TWinControl *>(tmpc);
//			if(tmpctrl)
//				continue;
        updatergn -= tmpc->BoundsRect;
    }

    Canvas.ClipMask = updatergn;

    ctrl->Paint();

    Canvas.RemoveClipMask();
//  }
}
//---------------------------------------------------------------------------
void TWinControl :: Repaint()
{
    TWinControl *tmpctrl;
    TControl *tmpc;

    if(!CheckState(csShowing))
        return;

    TRect r = ClientRect;
    TRegion updatergn; //(ClientRect);
    updatergn += r;
    for(TList::iterator from = m_Controls.begin();
            from != m_Controls.end();
            ++from)
    {
        tmpc = (TControl *)(*from);
        if(!tmpc->Showing)
            continue;
        tmpctrl = dynamic_cast<TWinControl *>(tmpc);
        if(!tmpctrl)
        {
            tmpc->Paint();
            updatergn -= tmpc->BoundsRect;
        }
    }
    Canvas.ClipMask = updatergn;

//	TControl :: Update();
    Paint();

    Canvas.RemoveClipMask();
//	XFlush(XDpy);
    XSync(XDpy, false);
}
//---------------------------------------------------------------------------
void TWinControl :: Update()
{
//    if(!m_updatergn.IsEmpty())
    UpdateWindow(m_Handle);
}
//---------------------------------------------------------------------------
/*
void TWinControl :: UpdateRect(const TRect &rect)
{
	m_updatergn += rect;
   m_Canvas->ClipMask = m_updatergn;
   m_updatergn.Clear();
   Update();
	m_Canvas->RemoveClipMask();
}
* */
//---------------------------------------------------------------------------
void TWinControl :: SetZOrder(bool topmost)
{
    if(topmost)
        XRaiseWindow(XDpy, m_Handle);
    else
        XLowerWindow(XDpy, m_Handle);

    TControl :: SetZOrder(topmost);
}
//---------------------------------------------------------------------------
void TWinControl :: MoveControl(TControl *control, bool topmost)
{
    m_Controls.remove(control);
    if(topmost)
        m_Controls.push_back(control);
    else
        m_Controls.push_front(control);
}
//---------------------------------------------------------------------------
bool __fastcall TWinControl :: WndProc(XEvent &event)
{
    char buf[31];
    KeySym keysym;
    int count;
    bool ret = false;
    TRect rect;
    TWinControl *tmpctrl;
    TControl *tmpc;

    switch (event.type)
    {
    case Expose:
        printf("EXPOSE %d %d %d %d %d\n", event.xexpose.x, event.xexpose.y, event.xexpose.width, event.xexpose.height, event.xexpose.count);
        rect = TRect(event.xexpose.x - Canvas.Origin.x, event.xexpose.y - Canvas.Origin.y, event.xexpose.width, event.xexpose.height);
//  			rect = TRect(event.xexpose.x, event.xexpose.y, event.xexpose.width, event.xexpose.height);
        m_updatergn += rect;
        if (event.xexpose.count == 0)
        {
            for(TList::iterator from = m_Controls.begin();
                    from != m_Controls.end();
                    ++from)
            {
                tmpc = (TControl *)(*from);
                if(!tmpc->Showing)
                    continue;
                tmpctrl = dynamic_cast<TWinControl *>(tmpc);
                if(!tmpctrl)
                {
                    tmpc->Repaint();
                    m_updatergn -= tmpc->BoundsRect;
                }
            }
            if(!m_updatergn.IsEmpty())
            {
                Canvas.ClipMask = m_updatergn;
                Paint();
                Canvas.RemoveClipMask();
                m_updatergn.Clear();
            }
        }
        ret = true;
        break;
    case XKeyPress:
        KeyDown(event.xkey.keycode, event.xkey.state);
        ret = true;
        break;
    case KeyRelease:
        KeyUp(event.xkey.keycode, event.xkey.state);
        count = XLookupString( &event.xkey, buf, 30, &keysym, NULL );
        buf[count] = '\0';
        if(buf[0])
            KeyPress(buf[0]);
        ret = true;
        break;
    case MappingNotify:
        XRefreshKeyboardMapping(&event.xmapping);
        printf("MAPPINGNOTIFY id=%d\n",id);
        ret = true;
        break;
    case FocusIn:
        StateOn(csFocused);
        DoEnter();
        printf("FOCUSIN id=%d mode=%d detail=%d\n",id, event.xfocus.mode, event.xfocus.detail);
        ret = true;
        break;
    case FocusOut:
        StateOff(csFocused);
        DoExit();
        printf("FOCUSOUT id=%d mode=%d detail=%d\n",id, event.xfocus.mode, event.xfocus.detail);
        ret = true;
        break;
    case ConfigureNotify:
        DoConfigure(event);
        ret = true;
        break;
    case ClientMessage:
        if(event.xclient.message_type == CreateNotify)
        {
            Create();
            printf("CREATE %d %s\n",id, ClassName().c_str());
            ret = true;
        }
        break;
    case MapRequest:
        printf("MAPREQUEST WIN%x\n",(uint_t)event.xmaprequest.window);
        break;
    case ConfigureRequest:
        printf("ConfigureRequest WIN\n");
        break;
    case ResizeRequest:
        XConfigureEvent e;
        e.type = ConfigureNotify;
        e.window = m_Handle;
        e.x = 10;
        e.y = 10;
        e.width = 100;
        e.height = 50;
        printf("ResizeRequest WIN\n");
        XSendEvent(XDpy, m_Handle, true, StructureNotifyMask, (XEvent *)&e);
        break;
    case CirculateNotify:
        printf("CIRCULATE WIN%x %d\n",(uint_t)event.xcirculate.window, event.xcirculate.place);
        break;
    default:
        int baseEvent;
        int baseError;
        bool supported;
        supported = XShapeQueryExtension(XDpy, &baseEvent, &baseError);
        if(supported && (event.type - baseEvent == ShapeNotify))
        {
            XShapeEvent &she = reinterpret_cast<XShapeEvent &>(event);
            rect = TRect(she.x, she.y, she.width, she.height);
            ChangeShape(TShapeKind(she.kind), rect, she.shaped);
            printf("CHANGESHAPE %d %d %d %d, %d %d\n", she.x, she.y, she.width, she.height, she.kind, she.shaped);
            ret = true;
//typedef struct {
//    int	type;		    /* of event */
//    unsigned long serial;   /* # of last request processed by server */
//    Bool send_event;	    /* true if this came frome a SendEvent request */
//    Display *display;	    /* Display the event was read from */
//    Window window;	    /* window of event */
//    int kind;		    /* ShapeBounding or ShapeClip */
//    int x, y;		    /* extents of new region */
//    unsigned width, height;
//    Time time;		    /* server timestamp when region changed */
//    Bool shaped;	    /* true if the region exists */
//} XShapeEvent;
        }
        else
        {
            TXExtension *ext;
            int old_type = event.type;
            for(TList::iterator from = Application.Extensions.begin();
                    from != Application.Extensions.end();
                    ++from)
            {
                ext = reinterpret_cast<TXExtension *>(*from);
                event.type -= ext->BaseEvent;
                ret = ext->WndProc(event);
                if(ret)
                    break;
            }
            event.type = old_type;
        }
    }
    if(!ret)
    {
//            for(TList::iterator from = m_Controls.begin();
//       	 			 from != m_Controls.end();
//          			 ++from)
        for(TList::reverse_iterator from = m_Controls.rbegin();
                from != m_Controls.rend();
                ++from)
        {
            tmpc = (TControl *)(*from);
            tmpctrl = dynamic_cast<TWinControl *>(tmpc);
            if(!tmpctrl)
            {
                if(tmpc->WndProc(event))
                    return true;
            }
        }
        ret = TControl::WndProc(event);
    }
    return ret;
}
//---------------------------------------------------------------------------
TWinControl *TWinControl :: FindWinControl(HWND wnd)
{
    TWinControl *tmpctrl;
    TControl *tmpc;

    for(TList::iterator from = m_Controls.begin();
            from != m_Controls.end();
            ++from)
    {
        tmpc = (TControl *)(*from);
        tmpctrl = dynamic_cast<TWinControl *>(tmpc);
        if(tmpctrl == NULL)
            continue;
        if(tmpctrl->Handle == wnd)
            return tmpctrl;
        else
        {
            tmpctrl = tmpctrl->FindWinControl(wnd);
            if(tmpctrl)
                return tmpctrl;
        }
    }
    return NULL;
}
//---------------------------------------------------------------------------
/*
bool TWinControl :: ContainsControl(TControl* AControl)
{
	TWinControl *tmpctrl;
	TControl *tmpc;

   for(TList::iterator from = m_Controls.begin();
       from != m_Controls.end();
            ++from)
	{
		tmpc = (TControl *)(*from);
      if(tmpc == AControl)
         return true;
		tmpctrl = dynamic_cast<TWinControl *>(tmpc);
		if(tmpctrl == NULL)
			continue;
		if(tmpctrl->ContainsControl(AControl))
				return true;
	}
	return false;
}
* */
//---------------------------------------------------------------------------
TControl * TWinControl :: ControlAtPos(const TPoint &p)
{
    TControl *tmpc;
    TWinControl *tmpctrl;
    TRegion rgn;

    for(TList::reverse_iterator from = m_Controls.rbegin();
            from != m_Controls.rend();
            ++from)
    {
        tmpc = (TControl *)(*from);
        rgn = tmpc->Bounds;
        if(rgn.InRegion(p))
        {
            tmpctrl = dynamic_cast<TWinControl *>(tmpc);
            if(tmpctrl)
            {
                tmpc = tmpctrl->ControlAtPos(p);
                if(tmpc == NULL)
                    return tmpctrl;
                else
                    return tmpc;
            }
            else
                return tmpc;
        }
    }
    return NULL;
}
//---------------------------------------------------------------------------
TControl * TWinControl :: ControlOnState(const TControlState &state)
{
    TControl *tmpc;
    TWinControl *tmpctrl;

//    for(TList::reverse_iterator from = m_Controls.rbegin();
//        from != m_Controls.rend();
//        ++from)
    for(TList::iterator from = m_Controls.begin();
            from != m_Controls.end();
            ++from)
    {
        tmpc = (TControl *)(*from);
        if(tmpc->CheckState(state))
            return tmpc;
        tmpctrl = dynamic_cast<TWinControl *>(tmpc);
        if(tmpctrl)
        {
            tmpc = tmpctrl->ControlOnState(state);
            if(tmpc)
                return tmpc;
        }
    }
    if(CheckState(state))
        return this;
    return NULL;
}
//---------------------------------------------------------------------------
void TWinControl :: DestroyHandle()
{
    if(m_Handle)
    {
        XDestroyWindow(XDpy, m_Handle);
        m_Handle = 0;
    }
}
//---------------------------------------------------------------------------
void TWinControl :: DoEnter()
{
//	StateOn(csActive);
//	Update();
    Repaint();
    if(OnEnter)
        OnEnter(this);
}
//---------------------------------------------------------------------------
void TWinControl :: DoExit()
{
//	StateOff(csActive);
//	Update();
    Repaint();
    if(OnExit)
        OnExit(this);
}
//---------------------------------------------------------------------------
void TWinControl :: KeyPress(char &key)
{
    if(OnKeyPress)
        OnKeyPress(this, key);
}
//---------------------------------------------------------------------------
void TWinControl :: KeyDown(uint_t &key, uint_t state)
{
    if(OnKeyDown)
        OnKeyDown(this, key, state);
}
//---------------------------------------------------------------------------
void TWinControl :: KeyUp(uint_t &key, uint_t state)
{
    if(OnKeyUp)
        OnKeyUp(this, key, state);
}
//---------------------------------------------------------------------------
bool TWinControl :: CanFocus()
{
    TWinControl *wctrl;

    wctrl = this;
    while(wctrl != NULL)
    {
        if(!(wctrl->Visible && wctrl->Enabled))
            return false;
        wctrl = wctrl->Parent;
    };
    return true;
}
//---------------------------------------------------------------------------
bool TWinControl :: get_Focused()
{
    int revert;
    HWND h;
    bool ret;
    XGetInputFocus(XDpy, &h, &revert);
    if(h == m_Handle)
    {
        StateOn(csFocused);
        ret = true;
    }
    else
    {
        StateOff(csFocused);
        ret = false;
    }
    return ret;
}
//---------------------------------------------------------------------------
void TWinControl :: set_Focused(bool value)
{
    if(get_Focused() != value)
    {
        if(value)
        {
            if(CanFocus())
            {
                XSetInputFocus(XDpy, m_Handle, RevertToParent, CurrentTime);
                StateOn(csFocused);
            }
        }
        else
        {
            XSetInputFocus(XDpy, RootWnd, RevertToParent, CurrentTime);
            StateOff(csFocused);
        }
    }
}
//---------------------------------------------------------------------------
void TWinControl :: set_Cursor(TCursor value)
{
    HCURSOR hcur = Screen.Cursors[value];
    XDefineCursor(XDpy, m_Handle, hcur);
    TControl :: set_Cursor(value);
}
//---------------------------------------------------------------------------
void TWinControl :: ScrollBy(int delta_x, int delta_y)
{
    TControl *tmpc;

    for(TList::iterator from = m_Controls.begin();
            from != m_Controls.end();
            ++from)
    {
        tmpc = (TControl *)(*from);
        tmpc->Left += delta_x;
        tmpc->Top += delta_y;
    }
}
//---------------------------------------------------------------------------
void TWinControl :: Scale(int MV, int DV, int MH, int DH)
{
    TControl *tmpc;

    if(MH == 0)
        MH = MV;
    if(DH == 0)
        DH = DV;
    for(TList::iterator from = m_Controls.begin();
            from != m_Controls.end();
            ++from)
    {
        tmpc = (TControl *)(*from);
        if(!tmpc)
            tmpc->Scale(MV, DV, MH, DH);
    }
    TControl :: Scale(MV, DV, MH, DH);
}
//---------------------------------------------------------------------------
bool TWinControl :: get_SaveUnder()
{
    XWindowAttributes winattrib;
    XGetWindowAttributes(XDpy, m_Handle, &winattrib);
    return winattrib.save_under;
}
//---------------------------------------------------------------------------
void TWinControl :: set_SaveUnder(bool value)
{
    unsigned long valuemask = CWSaveUnder;
    XSetWindowAttributes attrib;
    attrib.save_under = value;
    XChangeWindowAttributes(XDpy, m_Handle, valuemask, &attrib);
}
//---------------------------------------------------------------------------
TBackingStore TWinControl :: get_BackingStore()
{
    XWindowAttributes winattrib;
    XGetWindowAttributes(XDpy, m_Handle, &winattrib);
    return (TBackingStore)winattrib.backing_store;
}
//---------------------------------------------------------------------------
void TWinControl :: set_BackingStore(TBackingStore value)
{
    unsigned long valuemask = CWBackingStore;
    XSetWindowAttributes attrib;
    attrib.backing_store = value;
    XChangeWindowAttributes(XDpy, m_Handle, valuemask, &attrib);
}
//---------------------------------------------------------------------------
void TWinControl :: set_GrabKeyboard(bool value)
{
    if(GrabKeyboard != value)
    {
        int ret = 0;
        if(value)
        {
            if(Showing)
            {
                ret = XGrabKeyboard(XDpy, m_Handle, false,
                                    GrabModeAsync, GrabModeAsync,
                                    CurrentTime);
            }
            if(ret == 0)
                StateOn(csGrabKeyboard);
        }
        else
        {
            if(Showing)
            {
                XUngrabKeyboard(XDpy, CurrentTime);
            }
            StateOff(csGrabKeyboard);
        }
    };
}
//---------------------------------------------------------------------------
void TWinControl :: set_GrabMouse(bool value)
{
    if(GrabMouse != value)
    {
        int ret = 0;
        if(value)
        {
            if(Showing)
            {
                XUngrabPointer(XDpy, CurrentTime);
                ret = XGrabPointer(XDpy, m_Handle, false,
                                   ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
                                   LeaveWindowMask | EnterWindowMask,
                                   GrabModeAsync, GrabModeAsync,
                                   None, None, CurrentTime);

//                ret = XGrabButton(XDpy, AnyButton, AnyModifier, m_Handle, false, ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
//                     LeaveWindowMask | EnterWindowMask,
//                GrabModeAsync, GrabModeAsync, None, None);
            }
            if(ret == 0)
                StateOn(csGrabMouse);
        }
        else
        {
            if(Showing)
            {
                XUngrabPointer(XDpy, CurrentTime);
            }
            StateOff(csGrabMouse);
        }
    };
}
//---------------------------------------------------------------------------
void TWinControl :: ChangeShape(TShapeKind kind, const TRect &rect, bool shaped)
{
    switch(kind)
    {
    case shBounding:
        if(shaped)
            StyleOn(csShapeBounding);
        else
            StyleOff(csShapeBounding);
        break;
    case shClip:
        if(shaped)
            StyleOn(csShapeClip);
        else
            StyleOff(csShapeClip);
        break;
    case shInput:
        if(shaped)
            StyleOn(csShapeInput);
        else
            StyleOff(csShapeInput);
        break;
    }
    if(OnChangeShape)
        OnChangeShape(this, kind, rect, shaped);
}
//---------------------------------------------------------------------------
void TWinControl :: SetShape(const TRegion &region, const TShapeParam *param)
{
    TShapeKind kind = shBounding;
    TShapeOperation op = shSet;
    int xoff = 0, yoff = 0;
    if(param)
    {
        kind = param->kind;
        op = param->op;
        xoff = param->xoffset;
        yoff = param->yoffset;
    }
    XShapeCombineRegion(XDpy, m_Handle, kind, xoff, yoff, region.Handle, op);
}
//---------------------------------------------------------------------------
void TWinControl :: SetShape(const TBitmap &bmp, const TShapeParam *param)
{
    TShapeKind kind = shBounding;
    TShapeOperation op = shSet;
    int xoff = 0, yoff = 0;
    if(param)
    {
        kind = param->kind;
        op = param->op;
        xoff = param->xoffset;
        yoff = param->yoffset;
    }
    XShapeCombineMask(XDpy, m_Handle, kind, xoff, yoff, bmp.Handle, op);
}
//---------------------------------------------------------------------------
void TWinControl :: SetShape(const TWinControl &src, const TShapeParam *param)
{
    TShapeKind kind = shBounding;
    TShapeOperation op = shSet;
    int xoff = 0, yoff = 0;
    TShapeKind src_kind = shBounding;
    if(param)
    {
        kind = param->kind;
        op = param->op;
        xoff = param->xoffset;
        yoff = param->yoffset;
        src_kind = param->src_kind;
    }
    XShapeCombineShape(XDpy, m_Handle, kind, xoff, yoff, src.Handle, src_kind, op);
}
//---------------------------------------------------------------------------
void TWinControl :: SetShape(TShapeKind kind, int xoff, int yoff)
{
    XShapeOffsetShape(XDpy, m_Handle, kind, xoff, yoff);
}
//---------------------------------------------------------------------------
void TWinControl :: SetShape(const TRect *rects, int nrects, const TShapeParam *param)
{
    TShapeKind kind = shBounding;
    TShapeOperation op = shSet;
    int xoff = 0, yoff = 0;
//	TShapeKind src_kind = shBounding;
    int ordering = Unsorted;
    if(param)
    {
        kind = param->kind;
        op = param->op;
        xoff = param->xoffset;
        yoff = param->yoffset;
//		src_kind = param->src_kind;
        ordering = param->ordering;
    }
    TRect *arects = const_cast<TRect *>(rects);
    XRectangle *rectangles;
    rectangles = new XRectangle[nrects];
    for(register int i = 0; i < nrects; i++)
    {
        rectangles[i] = arects[i].xrect();
    }
    XShapeCombineRectangles(XDpy, m_Handle, kind, xoff, yoff, rectangles, nrects, op, ordering);
    delete[] rectangles;
}
//---------------------------------------------------------------------------
void TWinControl :: RemoveShape(TShapeKind kind)
{
    if(kind == -1)
    {
        XShapeCombineMask(XDpy, m_Handle, ShapeBounding, 0, 0, None, ShapeSet);
        XShapeCombineMask(XDpy, m_Handle, ShapeClip, 0, 0, None, ShapeSet);
        XShapeCombineMask(XDpy, m_Handle, ShapeInput, 0, 0, None, ShapeSet);
    }
    else
        XShapeCombineMask(XDpy, m_Handle, kind, 0, 0, None, ShapeSet);
}
//---------------------------------------------------------------------------
/*

    FParentColor: Boolean;
    FOriginalParentSize: TPoint;
     procedure SetParentColor(const Value: Boolean);
    procedure SetParentFont(const Value: Boolean);
    FParentFont: Boolean;
    procedure ParentColorChanged; dynamic;
    procedure ParentFontChanged; dynamic;
    property ParentColor: Boolean read FParentColor write SetParentColor default True;
    property ParentFont: Boolean read FParentFont write SetParentFont default True;
    function ClientToParent(const Point: TPoint; AParent: TWidgetControl = nil): TPoint;
    function ParentToClient(const Point: TPoint; AParent: TWidgetControl = nil): TPoint;


  TControl = class(TComponent)
  private
    FFont: TFont;
    FFontHeight: Integer;
    FSizedFlags: TScalingFlags;
    FHelpContext: THelpContext;
    FHelpFile: string;
    FHelpKeyword: string;
    FHelpType: THelpType;
    function GetClientHeight: Integer;
    function GetClientWidth: Integer;
    procedure SetClientHeight(const Value: Integer);
    procedure SetClientSize(Value: TPoint);
    procedure SetClientWidth(const Value: Integer);
    procedure SetFont(const Value: TFont);
    procedure SetPopupMenu(const Value: TPopupMenu);
    procedure SetMouseCapture(const Value: Boolean);
    function GetMouseCapture: Boolean;
  protected
    procedure AdjustSize; dynamic;
    procedure SetHelpContext(const Value: THelpContext);
    procedure SetHelpKeyword(const Value: String);
    function WantKey(Key: Integer; Shift: TShiftState; const KeyText: WideString): Boolean; dynamic;
    property MouseCapture: Boolean read GetMouseCapture write SetMouseCapture;
  public

  published

  end;

    procedure KeyString(var S: WideString; var Handled: Boolean); dynamic;
    property OnKeyString: TKeyStringEvent read FOnKeyString write FOnKeyString;

        procedure FixupTabList;
           FTabList: TList;
     procedure UpdateTabOrder(Value: TTabOrder);
     function FindNextControl(CurControl: TWidgetControl; GoForward, CheckTabStop, CheckParent: Boolean): TWidgetControl;
    procedure SelectFirst;
    function SelectNext(CurControl: TWidgetControl; GoForward, CheckTabStop: Boolean): Boolean;
    procedure GetTabOrderList(List: TList); dynamic;

    FBrush: TBrush;
    procedure SetBrush(const Value: TBrush);
    property Brush: TBrush read FBrush write SetBrush;


    TWidgetControl = class(TControl)
  private
    FStyle: TWidgetStyle;
    FPalette: TWidgetPalette;
    FInputKeys: TInputKeys;
    procedure DoFlipChildren;
    function CheckInputKeys(Key: Integer; Shift: TShiftState; const KeyText: WideString): Boolean;
    function IsMenuKey(Key: Integer; Shift: TShiftState; const KeyText: WideString): Boolean;
    procedure UpdateShowing;
    function GetStyle: TWidgetStyle;
    procedure SetStyle(const Value: TWidgetStyle);
  protected
    procedure AdjustClientRect(var Rect: TRect); virtual;
    function CustomAlignInsertBefore(C1, C2: TControl): Boolean; virtual;
    procedure CustomAlignPosition(Control: TControl; var NewLeft, NewTop, NewWidth,
      NewHeight: Integer; var AlignRect: TRect); virtual;
    procedure AlignControls(AControl: TControl; var Rect: TRect); virtual;
    procedure ChangeBounds(ALeft, ATop, AWidth, AHeight: Integer); override;
    procedure FontChanged; override;
    procedure MaskChanged; dynamic;
    function NeedKey(Key: Integer; Shift: TShiftState; const KeyText: WideString): Boolean; dynamic;
    procedure RestoreWidgetState; virtual;
    procedure SaveWidgetState; virtual;
    procedure SetChildOrder(Child: TComponent; Order: Integer); override;
    procedure ShowControl(AControl: TControl); virtual;
    procedure StyleChanged(Sender: TObject); dynamic;
    procedure UpdateWidgetShowing; dynamic;
    function WantKey(Key: Integer; Shift: TShiftState; const KeyText: WideString): Boolean; override;
    property InputKeys: TInputKeys read FInputKeys write FInputKeys;
    property Style: TWidgetStyle read GetStyle write SetStyle;
  public
    function ControlAtPos(const Pos: TPoint; AllowDisabled: Boolean;
      AllowWidgets: Boolean = False): TControl;
    function FindChildControl(const ControlName: string): TControl;
    procedure FlipChildren(AllLevels: Boolean); dynamic;
  end;
*/
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
