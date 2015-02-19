//---------------------------------------------------------------------------
#include <stdio.h>
#include <X11/extensions/shape.h>
#include "application.h"
#include "netwm.h"
#include "rtti.h"
#include "menu.h"
#include "forms.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
extern HDISPLAY XDpy;
extern HWND RootWnd;
//---------------------------------------------------------------------------
void SO_ENTRYPOINT initialization()
{
    RegisterClass(TForm);
    RegisterClass(THintWindow);
}
void SO_EXITPOINT finalization()
{
}
//---------------------------------------------------------------------------
//	TCustomForm
//---------------------------------------------------------------------------
TCustomForm :: TCustomForm()
{
    Init();
    Visible = false;
}
//---------------------------------------------------------------------------
TCustomForm :: TCustomForm(TWinControl *AParent) //: TWinControl(AParent)
{
    Init();
    Visible = false;
}
//---------------------------------------------------------------------------
TCustomForm :: TCustomForm(HWND win) : TWinControl(win)
{
    Init();
}
//---------------------------------------------------------------------------
TCustomForm :: ~TCustomForm()
{
    Close();
    Application.Forms.Delete(this);
    if(OnDestroy)
        OnDestroy(this);
}
//---------------------------------------------------------------------------
void TCustomForm :: Init()
{
//	unsigned long valuemask = CWBitGravity;
//    XSetWindowAttributes attrib;
//    attrib.bit_gravity = NorthWestGravity;
//	XChangeWindowAttributes(XDpy, m_Handle, valuemask, &attrib);

//	if(Parent == NULL && m_Handle != CScreen.Root)
//	{
//		Parent = CScreen.ActiveDesktop;
//	}
//   	Visible = false;

    Scaled = true;
    m_terminated = false;
    Application.Forms.Add(this);
    SetWMProtocols(m_Handle);
}
//---------------------------------------------------------------------------
TWinControl *TCustomForm :: get_ActiveControl()
{
    TWinControl *ret;
    int revert;
    HWND h;

    XGetInputFocus(XDpy, &h, &revert);
    ret = FindWinControl(h);
    return ret;
}
//---------------------------------------------------------------------------
void TCustomForm :: set_ActiveControl(TWinControl *control)
{
    control->Active = true;
}
//---------------------------------------------------------------------------
int TCustomForm :: get_Desktop()
{
    return GetDesktop(m_Handle);
}
//---------------------------------------------------------------------------
void TCustomForm :: set_Desktop(int val)
{
    if(get_Desktop() != val)
    {
        SetDesktop(m_Handle, val);
    }
}
//---------------------------------------------------------------------------
static const char *__pnames[] =
{
    "_NET_WM_WINDOW_TYPE_NORMAL",
    "_NET_WM_WINDOW_TYPE_DESKTOP",
    "_NET_WM_WINDOW_TYPE_DOCK",
    "_NET_WM_WINDOW_TYPE_TOOLBAR",
    "_NET_WM_WINDOW_TYPE_MENU",
    "_NET_WM_WINDOW_TYPE_UTILITY",
    "_NET_WM_WINDOW_TYPE_SPLASH",
    "_NET_WM_WINDOW_TYPE_DIALOG",
    "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU",
    "_NET_WM_WINDOW_TYPE_POPUP_MENU",
    "_NET_WM_WINDOW_TYPE_TOOLTIP",
    "_NET_WM_WINDOW_TYPE_NOTIFICATION",
    "_NET_WM_WINDOW_TYPE_COMBO",
    "_NET_WM_WINDOW_TYPE_DND"
};

TFormType TCustomForm :: get_FormType()
{
    TFormType ret = ftNormal;
    Atom tmp;
    void *props;
    char *pname;
    int nitems;

    tmp = XInternAtom(XDpy, "_NET_WM_WINDOW_TYPE", true);
//    int nitems = GetXProperty(tmp, XA_ATOM, &props);
    props = GetXProperty(m_Handle, tmp, &nitems);
    if(nitems != 0)
    {
        tmp = ((Atom *)props)[0];
        pname = XGetAtomName(XDpy, tmp);
        for(register int i = 0; i < SIZEOFARRAY(__pnames); i++)
        {
            if(!strcmp(pname, __pnames[i]))
            {
                ret = (TFormType)i;
                break;
            }
        }
        XFree(pname);
        XFree(props);
    }
    return ret;
}
//---------------------------------------------------------------------------
void TCustomForm :: set_FormType(TFormType value)
{
    const char *propname = __pnames[value];
    Atom al[1], atom;
    al[0] = XInternAtom(XDpy, propname, true);
    atom = XInternAtom(XDpy, "_NET_WM_WINDOW_TYPE", true);
//	SetXProperty(pmReplace, atom, XA_ATOM, 32, (void *)al, 1);
    SetXProperty(m_Handle, atom, XA_ATOM, 32, (void *)al, 1);
}
//---------------------------------------------------------------------------
bool TCustomForm :: get_OverrideRedirect()
{
    XWindowAttributes winattrib;
    XGetWindowAttributes(XDpy, m_Handle, &winattrib);
    return winattrib.override_redirect;
}
//---------------------------------------------------------------------------
void TCustomForm :: set_OverrideRedirect(bool value)
{
    unsigned long valuemask = CWOverrideRedirect;
    XSetWindowAttributes attrib;
    attrib.override_redirect = value;
    XChangeWindowAttributes(XDpy, m_Handle, valuemask, &attrib);
}
//---------------------------------------------------------------------------
void TCustomForm :: DoShow()
{
    TWinControl :: DoShow();
    Screen.AddForm(this);
}
//---------------------------------------------------------------------------
void TCustomForm :: DoHide()
{
    TWinControl :: DoHide();
    Screen.RemoveForm(this);
}
//---------------------------------------------------------------------------
void TCustomForm :: Paint()
{
    if(!Bitmap)
    {
        Canvas.Brush->Color = Color;
        Canvas.FillRect(ClientRect);
    }

    if(OnPaint)
        OnPaint(this);
}
//---------------------------------------------------------------------------
bool __fastcall TCustomForm :: WndProc(XEvent &event)
{
    Atom m_atom;
    bool ret = false;
    TRect rect;

    switch (event.type)
    {
    case ClientMessage:
        m_atom = XInternAtom(XDpy, "WM_DELETE_WINDOW", true);
        if(m_atom == (Atom)event.xclient.data.l[0])
        {
            Close();
            ret = true;
        }
        else
        {
            m_atom = XInternAtom(XDpy, "WM_TAKE_FOCUS", true);
            if(m_atom == (Atom)event.xclient.data.l[0])
            {
                printf("WM_TAKE_FOCUS %d\n", id);
                ret = true;
            }
            else
            {
                m_atom = XInternAtom(XDpy, "WM_SAVE_YOURSELF", true);
                if(m_atom == (Atom)event.xclient.data.l[0])
                {
                    printf("WM_SAVE_YOURSELF %d\n", id);
                    ret = true;
                }
                else
                {
                    m_atom = XInternAtom(XDpy, "_NET_WM_PING", true);
                    if(m_atom == (Atom)event.xclient.data.l[0])
                    {
                        SendPing(0, &event);
                        ret = true;
                    }
                }
            }
        }
        break;
    case MapNotify:
        if(OnShow)
            OnShow(this);
        printf("MAPNOTIFY ONSHOW %d\n", id);
        break;
    case UnmapNotify:
        if(OnHide)
            OnHide(this);
        break;
    case FocusIn:
        Activate();
        ret = true;
        break;
    case FocusOut:
        Deactivate();
        ret = true;
        break;
    case MapRequest:
        printf("MAPREQUEST FORM %x\n",(uint_t)event.xmaprequest.window);
        break;
    case ConfigureRequest:
        printf("ConfigureRequest\n");
        break;
    case PropertyNotify:
        m_atom = event.xproperty.atom;
        char *s = XGetAtomName(XDpy, m_atom);
        printf("XPROPERTY win=%lu atom=%d name=%s\n", m_Handle, (int)m_atom, s);
        XFree(s);
        break;
    }
    if(!ret)
        ret = TWinControl::WndProc(event);
    return ret;
}
//---------------------------------------------------------------------------
int TCustomForm :: ShowModal()
{
    XEvent event;
    HWND wnd;
    TWinControl *tmpctrl;
    TCustomForm *ftmp;
//    long oldmask;
    int etype;
    int ret = 0;

    wnd = GetActiveWindow();
    TCustomForm *frm = Screen.ActiveForm;
    printf("ACTIVEFORM %s\n", (*frm->Caption).c_str());
    TWinControl *win = Screen.ActiveControl;
    printf("ACTIVECONTROL %s\n", (*win->Caption).c_str());
//    XSetTransientForHint(XDpy, wnd, m_Handle);

    TransientFor = wnd;
    WindowState = TWindowState() << wsSkipTaskbar << wsModal;

    /*                       event.type = LeaveNotify;
                           event.xcrossing.display = XDpy;
                           event.xcrossing.window = wnd;
                           event.xcrossing.send_event = true;
                           event.xcrossing.root = CScreen.Root;
                           event.xcrossing.subwindow = None;
                           event.xcrossing.time = CurrentTime;
                           event.xcrossing.x = 0;
                           event.xcrossing.y = 0;
                           event.xcrossing.x_root = 0;
                           event.xcrossing.y_root = 0;
                           event.xcrossing.mode = NotifyNormal;
                           event.xcrossing.detail = NotifyAncestor;
                           event.xcrossing.same_screen = true;
                           event.xcrossing.focus = false;
                           event.xcrossing.state = 0;
                           XPutBackEvent(XDpy, &event); */



    Show();

    while ( !m_terminated )
    {
        XNextEvent( XDpy, &event );
        wnd = event.xany.window;
        etype = event.type;
        if(etype == Expose || etype == ConfigureNotify || etype == LeaveNotify || etype == FocusOut)
        {
            Application.DispatchEvent( event );
            continue;
        }
        if(wnd == m_Handle)
        {
            Application.DispatchEvent( event );
        }
        else
        {
            tmpctrl = FindWinControl(wnd);
            if(tmpctrl)
            {
                ftmp = tmpctrl->ParentForm;
                if(ftmp && ftmp->Handle == m_Handle)
                    tmpctrl->WndProc(event);
            }
        }
    }
    return ret;
}
//---------------------------------------------------------------------------
void TCustomForm :: Close()
{
    bool canclose = true;

    if(OnCloseQuery)
        canclose = OnCloseQuery(this);
    if(canclose)
    {
        Hide();
//		CloseWindow(m_Handle);
        if(OnClose)
            OnClose(this);
        m_terminated = true;
        if(Application.MainForm == this && !Application.MainForm->Visible)
            Application.Terminate();
    }
}
//---------------------------------------------------------------------------
HWND TCustomForm :: get_TransientFor()
{
    Window prop_window;
    XGetTransientForHint(XDpy, m_Handle, &prop_window);
    return prop_window;
}
//---------------------------------------------------------------------------
void TCustomForm :: set_TransientFor(HWND value)
{
    XSetTransientForHint(XDpy, m_Handle, value);
}
//---------------------------------------------------------------------------
void TCustomForm :: Activate()
{
    if(!Active)
    {
        StateOn(csActive);
        if(OnActivate)
            OnActivate(this);
    }
}
//---------------------------------------------------------------------------
void TCustomForm :: Deactivate()
{
    if(Active)
    {
        StateOff(csActive);
//        if(Screen.PopupActive)
//            Screen.ClosePopup();

        if(OnDeactivate)
            OnDeactivate(this);
    }
}
//---------------------------------------------------------------------------
static const char *__stnames[] =
{
    "_NET_WM_STATE_MODAL",
    "_NET_WM_STATE_STICKY",
    "_NET_WM_STATE_MAXIMIZED_VERT",
    "_NET_WM_STATE_MAXIMIZED_HORZ",
    "_NET_WM_STATE_SHADED",
    "_NET_WM_STATE_SKIP_TASKBAR",
    "_NET_WM_STATE_SKIP_PAGER",
    "_NET_WM_STATE_HIDDEN",
    "_NET_WM_STATE_FULLSCREEN",
    "_NET_WM_STATE_ABOVE",
    "_NET_WM_STATE_BELOW",
    "_NET_WM_STATE_DEMANDS_ATTENTION"
};

TWindowState TCustomForm :: get_WindowState()
{
    TWindowState ret;
    const int n = SIZEOFARRAY(__stnames);
    void *props;
    Atom tmp;
    Atom atoms[n];
    int nitems;

    tmp = XInternAtom(XDpy, "_NET_WM_STATE", true);
//    int nitems = GetXProperty(tmp, XA_ATOM, &props);
    props = GetXProperty(m_Handle, tmp, &nitems);
    XInternAtoms(XDpy, (char **)__stnames, n, true, atoms);
    for(register int i = 0; i < nitems; i++)
    {
        tmp = ((Atom *)props)[i];
        for(register int j = 0; j < n; j++)
            if(tmp == atoms[j])
            {
                ret << (TWindowStateKind)j;
            }
    }
    XFree(props);
    return ret;
}
//---------------------------------------------------------------------------
void TCustomForm :: set_WindowState(TWindowState value)
{
    const int n = SIZEOFARRAY(__stnames);
    Atom al[n], atoms[n];
    int nitems = value.size();
    XInternAtoms(XDpy, (char **)__stnames, n, true, atoms);
    int i = 0;
    for(register int j = 0; j < n; j++)
    {
        if(value.Contains((TWindowStateKind)j))
            al[i++] = atoms[j];
    }
    Atom atom = XInternAtom(XDpy, "_NET_WM_STATE", true);
//	SetXProperty(pmReplace, atom, XA_ATOM, 32, (void *)al, nitems);
    SetXProperty(m_Handle, atom, XA_ATOM, 32, (void *)al, nitems);
}
//---------------------------------------------------------------------------
//  TPopupWindow
//---------------------------------------------------------------------------
TPopupWindow :: TPopupWindow()
{
    Init();
};
//---------------------------------------------------------------------------
TPopupWindow :: ~TPopupWindow()
{
};
//---------------------------------------------------------------------------
__fastcall TPopupWindow :: TPopupWindow(const TPopupWindow &a) : TCustomForm(a)
{
    Assign(a);
};
//---------------------------------------------------------------------------
__fastcall const TPopupWindow & TPopupWindow :: operator=(const TPopupWindow &a)
{
    if(this == &a)
        return *this;
    TCustomForm :: operator=(a);
    Assign(a);
    return *this;
};
//---------------------------------------------------------------------------
void TPopupWindow :: Init()
{
    StyleOn(csPopup);
    OverrideRedirect = true;
};
//---------------------------------------------------------------------------
__fastcall void TPopupWindow :: Assign(const TPopupWindow &obj)
{
};
//---------------------------------------------------------------------------
void __fastcall TPopupWindow :: Popup(int x, int y)
{
    if(OnPopup)
        OnPopup(this);
    Left = x;
    Top = y;
    Visible = true;
}
//---------------------------------------------------------------------------
//	TCustomFormFrame
//---------------------------------------------------------------------------
TCustomFormFrame :: TCustomFormFrame()
{
}
//---------------------------------------------------------------------------
TCustomFormFrame :: TCustomFormFrame(TWinControl *AParent) : TCustomForm(AParent)
{
}
//---------------------------------------------------------------------------
TCustomFormFrame :: TCustomFormFrame(HWND win) : TCustomForm(win)
{
}
//---------------------------------------------------------------------------
TCustomFormFrame :: ~TCustomFormFrame()
{
}
//---------------------------------------------------------------------------
//	TForm
//---------------------------------------------------------------------------
TForm :: TForm()
{
    FormType = ftNormal;
    Init();
}
//---------------------------------------------------------------------------
TForm :: TForm(TWinControl *AParent) : TCustomForm(AParent)
{
    FormType = ftNormal;
    Init();
}
//---------------------------------------------------------------------------
TForm :: TForm(HWND win) : TCustomForm(win)
{
    Init();
}
//---------------------------------------------------------------------------
TForm :: ~TForm()
{
}
//---------------------------------------------------------------------------
void TForm :: Init()
{
    /*	unsigned long valuemask = CWEventMask | CWBitGravity | //CWDontPropagate |
    									  CWWinGravity;
    	XSetWindowAttributes attributes;
    	attributes.event_mask = ExposureMask | \
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
                     				StructureNotifyMask;
    //	attributes.do_not_propagate_mask = propagate;
       attributes.bit_gravity = NorthWestGravity; // StaticGravity; ForgetGravity; NorthWestGravity;
       attributes.win_gravity = NorthWestGravity;
    	m_WindowHandle = XCreateWindow(XDpy, m_Handle, 0, 0, 320, 320, 0, CopyFromParent,
                           InputOutput, CopyFromParent, valuemask, &attributes);
        XMapWindow(XDpy,m_WindowHandle);
    Canvas.Handle = m_WindowHandle;
    */
//Parent = &m_wnd;
//m_wnd.Align = alClient;
//m_wnd.Visible = true;
//Canvas.Handle = m_wnd.Handle;


    m_FrameHandle = 0;
    m_ExtentLeft = 0;
    m_ExtentTop = 0;
    m_ExtentRight = 0;
    m_ExtentBottom = 0;

    m_FormFrame = NULL;
    m_Icon = NULL;
    m_Menu = NULL;

    QueryFrameExtents(m_Handle);
//XSync(XDpy, false);
//XFlush(XDpy);

    /*
    XEvent event;
    Atom m_atom = XInternAtom(XDpy, "_NET_FRAME_EXTENTS", true);

    while( XCheckTypedWindowEvent(XDpy, m_Handle, PropertyNotify, &event) )
    {
        printf("==== FRAME EXTENTS ======\n");

        			printf("XPROPERTY id=%d name=%s\n", id, XGetAtomName(XDpy, event.xproperty.atom));
    				if(m_atom == event.xproperty.atom)
    				{
    					if(event.xproperty.state == PropertyNewValue)
    					{
    						int ext[4];
    						GetFrameExtents(m_Handle, ext);
    						printf("==== FRAME EXTENTS  %d %d %d %d %d\n",id, ext[0],ext[1],ext[2],ext[3]);
    						m_ExtentLeft = ext[0];
    						m_ExtentRight = ext[1];
    						m_ExtentTop = ext[2];
    						m_ExtentBottom = ext[3];
    					}
    					else
    					{
    						m_ExtentLeft = 0;
    						m_ExtentRight = 0;
    						m_ExtentTop = 0;
    						m_ExtentBottom = 0;
    					}
    				}
    }
    */
    /*

    						int ext[4];
    						GetFrameExtents(m_Handle, ext);
    						printf("==== FRAME EXTENTS  %d %d %d %d %d\n", id, ext[0],ext[1],ext[2],ext[3]);
    						m_ExtentLeft = ext[0];
    						m_ExtentRight = ext[1];
    						m_ExtentTop = ext[2];
    						m_ExtentBottom = ext[3];
    */
    TStrings s;
    TRect r;
    GetDesktopNames(s);
    for(int i = 0; i < s.Count; i++)
    {
        r = GetWorkArea(i);
        printf("DESKTOPNAME %s %d %d %d %d\n", s[i].c_str(), r.left, r.top, r.width, r.height);
    }
    /*
    XSizeHints *wmhint = XAllocSizeHints();
    		wmhint->flags = PPosition;
    		wmhint->x = 100;
    		wmhint->y = 100;
    		XSetWMNormalHints(XDpy, m_Handle, wmhint);
    		XFree(wmhint);
    */
    /*	if(Application.MainForm)
    	{
    		XWMHints *wmhint = XAllocWMHints();
    		wmhint->flags = WindowGroupHint | InputHint | StateHint;
    		wmhint->window_group = Application.MainForm->Handle;
    		wmhint->input = false;
        	wmhint->initial_state = NormalState;
    		XSetWMHints(XDpy, m_Handle, wmhint);
    		XFree(wmhint);
    	}
    */

//	OverrideRedirect = true;
//	BorderColor = clBlue;
//	BorderWidth = 5;

//	WindowState = TWindowState() << wsSticky;
}
//---------------------------------------------------------------------------
void TForm :: set_BoundsRect(TRect rect)
{

//	TWinControl :: set_BoundsRect(rect);

//    TRect old = BoundsRect;
    TRect newrect;
    ApplyConstraints(rect, newrect);
//    if(old == newrect)
//        return;
    m_Bounds.ClipRect = newrect;
//    if(Visible)
//    {
    newrect.left += m_ExtentLeft;
    newrect.top += m_ExtentTop;
    newrect.width -= (m_ExtentLeft + m_ExtentRight);
    newrect.height -= (m_ExtentTop + m_ExtentBottom);
    MoveWindow(m_Handle, newrect.left, newrect.top, newrect.width, newrect.height);
//        MoveWindow(m_WindowHandle, 0, 0, newrect.width, newrect.height);
//   }
    printf("FORMSIZE %d %d %d %d\n", newrect.left, newrect.top, newrect.width, newrect.height);
}
//---------------------------------------------------------------------------
TRect TForm :: get_ClientRect()
{
    TRect rect = TWinControl::get_ClientRect();
//	rect.width -= (m_ExtentLeft + m_ExtentRight);
//	rect.height -= (m_ExtentTop + m_ExtentBottom);
//	if(m_Menu)
//	{
//		rect.top += m_Menu->Height;
//		rect.height -= m_Menu->Height;
//	}
    return rect;
}
//---------------------------------------------------------------------------
void TForm :: set_Caption(TString text)
{
    const char *__aanames[] =
    {
        "_NET_WM_NAME",
        "UTF8_STRING"
    };
    const int n = SIZEOFARRAY(__aanames);
    Atom atoms[n];
    char *list;

    list = (char *)text.c_str();

    XInternAtoms(XDpy, (char **)__aanames, n, true, atoms);

    if(atoms[0] != None && atoms[1] != None)
    {
//		SetXProperty(pmReplace, atoms[0], atoms[1], 8, (void*)list, (int)text.size());
        SetXProperty(m_Handle, atoms[0], atoms[1], 8, (void *)list, (int)text.size());
    }
    else
    {
        XTextProperty txtprop;

        Xutf8TextListToTextProperty(XDpy, &list, 1, XCompoundTextStyle, &txtprop);
        XSetWMName(XDpy, m_Handle, &txtprop);
    }
    TControl :: set_Caption(text);
}
//---------------------------------------------------------------------------
void TForm :: set_Icon(TBitmap *value)
{
    if(value == m_Icon)
        return;
    XWMHints hint;
    hint.flags = IconPixmapHint | IconMaskHint;
    hint.icon_pixmap = value->Handle;
    hint.icon_mask = value->MaskHandle;
    XSetWMHints(XDpy, m_Handle, &hint);
    m_Icon = value;
}
//---------------------------------------------------------------------------
void TForm :: set_Menu(TCustomMainMenu *value)
{
    if(value == m_Menu)
        return;
    if(value)
    {
        value->Parent = this;
        value->Visible = true;
    }
    else
    {
        if(m_Menu)
        {
            m_Menu->Visible = false;
            m_Menu->Parent = NULL;
        }
    }
    m_Menu = value;
//	Canvas.Origin = TPoint(0, m_Menu->Height);
//	m_Menu->Parent = this;
    /*	TRect rect;
    	TControl *tc;
    	TWinControl *tw;
        for(int i = 0; i < ControlCount; i++)
        {
            tc = Controls[i];
            if(tc == m_Menu)
                continue;
            rect = tc->BoundsRect;
            tc->BoundsRect = rect;
        } */
    /*    if(m_Menu)
        {
            XReparentWindow(XDpy, m_Menu->Handle, m_Handle, 0, 0);
            XMapWindow(XDpy, m_Menu->Handle);
        } */
//	if(m_FormFrame)
//		m_Menu->Parent = m_FormFrame;
//	else
//		m_Menu->Parent = this;
//	value->TransientFor = m_Handle;
//	m_Menu->Parent = this;
//    m_Menu->Top = -m_Menu->Height;
//	m_Menu->Visible = true;
}
//---------------------------------------------------------------------------
static const char *__aanames[] =
{
    "_NET_WM_ACTION_MOVE",
    "_NET_WM_ACTION_RESIZE",
    "_NET_WM_ACTION_MINIMIZE",
    "_NET_WM_ACTION_SHADE",
    "_NET_WM_ACTION_STICK",
    "_NET_WM_ACTION_MAXIMIZE_HORZ",
    "_NET_WM_ACTION_MAXIMIZE_VERT",
    "_NET_WM_ACTION_FULLSCREEN",
    "_NET_WM_ACTION_CHANGE_DESKTOP",
    "_NET_WM_ACTION_CLOSE",
    "_NET_WM_ACTION_ABOVE",
    "_NET_WM_ACTION_BELOW"
};

TAllowedActions TForm :: get_AllowedActions()
{
    TAllowedActions ret;
    const int n = SIZEOFARRAY(__aanames);
    void *props;
    Atom tmp;
    Atom atoms[n];
    int nitems;

    tmp = XInternAtom(XDpy, "_NET_WM_ALLOWED_ACTIONS", true);
//    int nitems = GetXProperty(tmp, XA_ATOM, &props);
    props = GetXProperty(m_Handle, tmp, &nitems);
    XInternAtoms(XDpy, (char **)__aanames, n, true, atoms);
    for(register int i = 0; i < nitems; i++)
    {
        tmp = ((Atom *)props)[i];
        for(register int j = 0; j < n; j++)
            if(tmp == atoms[j])
            {
                ret << (TAllowedActionsKind)j;
            }
    }
    XFree(props);
    return ret;
}
//---------------------------------------------------------------------------
void TForm :: set_AllowedActions(TAllowedActions value)
{
    const int n = SIZEOFARRAY(__aanames);
    Atom al[n], atoms[n], atom;
    int nitems = value.size();
    XInternAtoms(XDpy, (char **)__aanames, n, true, atoms);
    int i = 0;
    for(register int j = 0; j < n; j++)
    {
        if(value.Contains((TAllowedActionsKind)j))
            al[i++] = atoms[j];
    }
    atom = XInternAtom(XDpy, "_NET_WM_ALLOWED_ACTIONS", true);
//	SetXProperty(pmReplace, atom, XA_ATOM, 32, (void *)al, nitems);
    SetXProperty(m_Handle, atom, XA_ATOM, 32, (void *)al, nitems);
}
//---------------------------------------------------------------------------
//void TForm :: MouseUp(uint_t button, uint_t state, int x, int y)
//{
//	BringToFront();
//	TControl :: MouseUp(button, state, x, y);
//}
//---------------------------------------------------------------------------
//void TForm :: DoShow()
//{
//	TCustomForm :: DoShow();
//}
//---------------------------------------------------------------------------
//void TForm :: DoHide()
//{
//	TCustomForm :: DoHide();
//}
//---------------------------------------------------------------------------
/* void TForm :: SetBounds(short left, short top, short width, short height)
{
	left = m_Left + m_ExtentLeft;
	top = m_Top + m_ExtentTop;
	width -= (m_ExtentLeft + m_ExtentRight);
	height -= (m_ExtentTop + m_ExtentBottom);

	TWinControl :: SetBounds(left, top, width, height);
} */
//---------------------------------------------------------------------------
/*
bool __fastcall TForm :: DoConfigure(XEvent &event)
{
    int w, h, neww, newh;
    TRect rect, rect1;
	TControl *tmpc;
	TWinControl *tmpw;

	printf("CONFIGURENOTIFY id=%d %d %d %d %d\n",id, event.xconfigure.x, event.xconfigure.y, event.xconfigure.width, event.xconfigure.height);
                neww = event.xconfigure.width;
				newh = event.xconfigure.height;
                rect = TRect(0, 0, neww, newh);
           		if(m_width != neww || m_height != newh)
           		{
					TRegion m_updatergn;
					m_updatergn += rect;
					for(int i = 0; i < ControlCount; i++)
					{
						tmpc = Controls[i];
						tmpw = dynamic_cast<TWinControl *>(tmpc);
						tmpc->ApplyAnchorRules(rect, rect1);
						tmpc->BoundsRect = rect1;
						if(!tmpw)
                          if(tmpc->Showing)
                               m_updatergn -= rect1;
					}

					if(CheckState(csShowing))
					{
						m_Canvas->ClipMask = m_updatergn;
						Paint();
						m_Canvas->RemoveClipMask();
					}
            		Resize();
            		printf("RESIZING...\n");

                    m_width = neww;
                    m_height = newh;
           		}
    m_Bounds.ClipRect = TRect(event.xconfigure.x, event.xconfigure.y, event.xconfigure.width, event.xconfigure.height);
//    m_Bounds.ClipRect = TRect(event.xconfigure.x, event.xconfigure.y, event.xconfigure.width + m_ExtentLeft + m_ExtentRight, event.xconfigure.height + m_ExtentTop + m_ExtentBottom);
//    m_Bounds.Offset(newrect.left - old.left, newrect.top - old.top);
//	m_Bounds.Expand(newrect.width - old.width, newrect.height - old.height);
	return true;
} */
//---------------------------------------------------------------------------
bool __fastcall TForm :: WndProc(XEvent &event)
{
    bool ret = false;
    Atom m_atom;
//	HWND tmp;
//	TRect rect, rect1, cli;
//	TControl *tmpc;
//	int w, h, neww, newh;
//	TRegion updatergn;

//	XWindowAttributes winattrib;
    char *s;

    switch (event.type)
    {
    case MapNotify:
        printf("TFORM MAPNOTIFY id=%d\n", id);
        m_FrameHandle = ParentWindow;
        if(m_FrameHandle != Screen.Root)  // работаем с WindowManager
        {
            if(CheckStyle(csShapeBounding))
                XShapeCombineShape(XDpy, m_FrameHandle, ShapeBounding, m_ExtentLeft, m_ExtentTop, m_Handle, ShapeBounding, ShapeSet);
            if(CheckStyle(csShapeClip))
                XShapeCombineShape(XDpy, m_FrameHandle, ShapeClip, m_ExtentLeft, m_ExtentTop, m_Handle, ShapeClip, ShapeSet);
            if(CheckStyle(csShapeInput))
                XShapeCombineShape(XDpy, m_FrameHandle, ShapeInput, m_ExtentLeft, m_ExtentTop, m_Handle, ShapeInput, ShapeSet);
        }
        else // работаем без WM
        {
        }
//				tmp = m_Handle;
//				m_Handle = m_WindowHandle;
//				m_WindowHandle = tmp;
        break;
    case UnmapNotify:
//				if(m_FormFrame)
//					delete m_FormFrame;
        m_FrameHandle = 0;
        break;
    case PropertyNotify:
        s = XGetAtomName(XDpy, event.xproperty.atom);
        printf("XPROPERTY id=%d name=%s\n", id, s);
        XFree(s);
        m_atom = XInternAtom(XDpy, "_NET_FRAME_EXTENTS", true);
        if(m_atom == event.xproperty.atom)
        {
            TRect br = BoundsRect;
            if(event.xproperty.state == PropertyNewValue)
            {
                long ext[4];
                GetFrameExtents(m_Handle, ext);
                printf("==== FRAME EXTENTS  %d %ld %ld %ld %ld\n",id, ext[0],ext[1],ext[2],ext[3]);
                m_ExtentLeft = ext[0];
                m_ExtentRight = ext[1];
                m_ExtentTop = ext[2];
                m_ExtentBottom = ext[3];
            }
            /*					else
            					{
            						m_ExtentLeft = 0;
            						m_ExtentRight = 0;
            						m_ExtentTop = 0;
            						m_ExtentBottom = 0;
            					} */
            BoundsRect = br;
//					BoundsRect = TRect(br.left + m_ExtentLeft, br.top + m_ExtentTop,
//                                          br.width - m_ExtentLeft - m_ExtentRight,
//                                          br.height - m_ExtentTop - m_ExtentBottom);
        }
        break;
    case ConfigureNotify:
        if(!CheckState(csLoading))
        {
            m_Bounds.ClipRect = TRect(event.xconfigure.x - m_ExtentLeft, event.xconfigure.y - m_ExtentTop,
                                      event.xconfigure.width + m_ExtentLeft + m_ExtentRight,
                                      event.xconfigure.height + m_ExtentTop + m_ExtentBottom);
        }
        break;
    case ButtonRelease:
        BringToFront();
        break;
    }
    if(!ret)
        ret = TCustomForm::WndProc(event);
    return ret;
}
//---------------------------------------------------------------------------
/*
  TCustomForm = class(TScrollingWidget)
  private
    FBorderIcons: TBorderIcons;
    FKeyPreview: Boolean;
    FDropTarget: Boolean;
    FMenu: TMainMenu;
    FModalResult: TModalResult;
    FBorderStyle: TFormBorderStyle;
    FClientHeight: Integer;
    FClientWidth: Integer;
    FTextHeight: Integer;
    FTextWidth: Integer;
    FOnHelp: THelpEvent;
    FOnShortCut: TShortCutEvent;
    FOnDeactivate: TNotifyEvent;
    FFormStyle: TFormStyle;
    FPosition: TPosition;
    FWindowState: TWindowState;
    FSizeGrip: Boolean;
    FGripper: QSizeGripH;
    FMenuHeight: Integer;
    FMDIParent: TForm;
    FMDIChildList: TList;
    procedure CheckGripper;
    procedure SetClientHeight(Value: Integer);
    procedure SetClientWidth(Value: Integer);
    procedure SetMenu(const Value: TMainMenu);
    procedure SetBorderStyle(const Value: TFormBorderStyle);
    procedure SetBorderIcons(const Value: TBorderIcons);
    procedure SetFormStyle(const Value: TFormStyle);
    procedure SetPosition(const Value: TPosition);
    procedure SetModalResult(const Value: TModalResult);
    procedure SetWindowState(const Value: TWindowState);
    procedure ShowWindowState;
    procedure SetMDIParent(const Value: TForm);
    function MDIChildList: TList;
    function GetMDIChildCount: Integer;
    function GetMDIChildren(I: Integer): TForm;
    procedure RemoveMDIChild(AForm: TCustomForm);
    procedure AppendMDIChild(AForm: TCustomForm);
    function GetClientHandle: QWorkspaceH;
    function GetActiveMDIChild: TForm;
    function GetTextHeight: Integer;
    function GetTextWidth: Integer;
    procedure MDIChildActivated(w: QWidgetH); cdecl;
  protected
    FFormState: TFormState;
    procedure BeginAutoDrag; override;
    procedure ChangeScale(MV, DV, MH, DH: Integer); override;
    procedure Deactivate; dynamic;
    procedure FontChanged; override;
    procedure GetChildren(Proc: TGetChildProc; Root: TComponent); override;
    function GetClientRect: TRect; override;
    function GetParentWidget: QWidgetH; override;
    function HandleCreateException: Boolean; dynamic;
    procedure IconChanged(Sender: TObject); virtual;
    function MDIChildTop: Integer;
    function MDIChildLeft: Integer;
    procedure ParentFontChanged; override;
    procedure SetChildOrder(Child: TComponent; Order: Integer); override;
    function WantKey(Key: Integer; Shift: TShiftState; const KeyText: WideString): Boolean; override;
    function WidgetFlags: Integer; override;
    property ActiveMDIChild: TForm read GetActiveMDIChild;
    property BorderIcons: TBorderIcons read FBorderIcons write SetBorderIcons stored IsForm
      default [biSystemMenu, biMinimize, biMaximize];
    property BorderStyle: TFormBorderStyle read FBorderStyle write SetBorderStyle
      stored IsForm default fbsSizeable;
    property ClientHeight write SetClientHeight stored IsClientSizeStored;
    property ClientWidth write SetClientWidth stored IsClientSizeStored;
    property FormStyle: TFormStyle read FFormStyle write SetFormStyle
      stored IsForm default fsNormal;
    property MDIChildCount: Integer read GetMDIChildCount;
    property MDIChildren[I: Integer]: TForm read GetMDIChildren;
    property MDIParent: TForm read FMDIParent write SetMDIParent;
    property Position: TPosition read FPosition write SetPosition default poDesigned;
    property SizeGrip: Boolean read FSizeGrip write FSizeGrip default True;
    property OnHelp: THelpEvent read FOnHelp write FOnHelp;
    property OnShortCut: TShortCutEvent read FOnShortCut write FOnShortCut;
  public
    procedure AfterConstruction; override;
    procedure BeforeDestruction; override;
    procedure Close;
    function CloseQuery: Boolean; virtual;
    procedure DefocusControl(Control: TWidgetControl; Removing: Boolean);
    procedure FocusControl(Control: TWidgetControl);
    procedure InvokeHelp; override;
    function IsShortCut(Key: Integer; Shift: TShiftState; const KeyText: WideString): Boolean; dynamic;
    property DropTarget: Boolean read FDropTarget write FDropTarget;
    property FormState: TFormState read FFormState;
    property HelpFile;
    property KeyPreview: Boolean read FKeyPreview write FKeyPreview default False;
    property Menu: TMainMenu read FMenu write SetMenu;
    property ModalResult: TModalResult read FModalResult write SetModalResult;
    property WindowState: TWindowState read FWindowState write SetWindowState
      stored IsForm default wsNormal;
  end;


    TCustomForm = class(TScrollingWinControl)
  private
    FBorderIcons: TBorderIcons;
    FBorderStyle: TFormBorderStyle;
    FSizeChanging: Boolean;
    FWindowState: TWindowState;
    FShowAction: TShowAction;
    FKeyPreview: Boolean;
    FFormStyle: TFormStyle;
    FPosition: TPosition;
    FDefaultMonitor: TDefaultMonitor;
    FTileMode: TTileMode;
    FDropTarget: Boolean;
    FHelpFile: string;
    FMenu: TMainMenu;
    FModalResult: TModalResult;
    FWindowMenu: TMenuItem;
    FClientWidth: Integer;
    FClientHeight: Integer;
    FTextHeight: Integer;
    FSavedBorderStyle: TFormBorderStyle;
    FOnActivate: TNotifyEvent;
    FOnDeactivate: TNotifyEvent;
    FOnHelp: THelpEvent;
    FOnShortCut: TShortCutEvent;
    FAlphaBlend: Boolean;
    FAlphaBlendValue: Byte;
    FTransparentColor: Boolean;
    FTransparentColorValue: TColor;
    procedure RefreshMDIMenu;
    procedure CloseModal;
    function GetActiveMDIChild: TForm;
    function GetMDIChildCount: Integer;
    function GetMDIChildren(I: Integer): TForm;
    function GetMonitor: TMonitor;
    function GetPixelsPerInch: Integer;
    function GetTextHeight: Integer;
    procedure IconChanged(Sender: TObject);
    procedure MergeMenu(MergeState: Boolean);
    procedure ReadIgnoreFontProperty(Reader: TReader);
    procedure ReadTextHeight(Reader: TReader);
    procedure SetBorderIcons(Value: TBorderIcons);
    procedure SetBorderStyle(Value: TFormBorderStyle);
    procedure SetClientHeight(Value: Integer);
    procedure SetClientWidth(Value: Integer);
    procedure SetFormStyle(Value: TFormStyle);
    procedure SetMenu(Value: TMainMenu);
    procedure SetPosition(Value: TPosition);
    procedure SetWindowMenu(Value: TMenuItem);
    procedure SetWindowState(Value: TWindowState);
    procedure SetWindowToMonitor;
    procedure WMQueryDragIcon(var Message: TWMQueryDragIcon); message WM_QUERYDRAGICON;
    procedure WMNCLButtonDown(var Message: TWMNCLButtonDown); message WM_NCLBUTTONDOWN;
    procedure WMCommand(var Message: TWMCommand); message WM_COMMAND;
    procedure WMInitMenuPopup(var Message: TWMInitMenuPopup); message WM_INITMENUPOPUP;
    procedure WMMenuChar(var Message: TWMMenuChar); message WM_MENUCHAR;
    procedure WMMenuSelect(var Message: TWMMenuSelect); message WM_MENUSELECT;
    procedure WMActivate(var Message: TWMActivate); message WM_ACTIVATE;
    procedure WMSysCommand(var Message: TWMSysCommand); message WM_SYSCOMMAND;
    procedure WMMDIActivate(var Message: TWMMDIActivate); message WM_MDIACTIVATE;
    procedure WMNextDlgCtl(var Message: TWMNextDlgCtl); message WM_NEXTDLGCTL;
    procedure WMEnterMenuLoop(var Message: TMessage); message WM_ENTERMENULOOP;
    procedure WMHelp(var Message: TWMHelp); message WM_HELP;
    procedure WMGetMinMaxInfo(var Message: TWMGetMinMaxInfo); message WM_GETMINMAXINFO;
    procedure WMSettingChange(var Message: TMessage); message WM_SETTINGCHANGE;
    procedure WMWindowPosChanging(var Message: TWMWindowPosChanging); message WM_WINDOWPOSCHANGING;
    procedure CMAppSysCommand(var Message: TMessage); message CM_APPSYSCOMMAND;
    procedure CMBiDiModeChanged(var Message: TMessage); message CM_BIDIMODECHANGED;
    procedure CMDeactivate(var Message: TCMDeactivate); message CM_DEACTIVATE;
    procedure CMDialogKey(var Message: TCMDialogKey); message CM_DIALOGKEY;
    procedure CMColorChanged(var Message: TMessage); message CM_COLORCHANGED;
    procedure CMFontChanged(var Message: TMessage); message CM_FONTCHANGED;
    procedure CMMenuChanged(var Message: TMessage); message CM_MENUCHANGED;
    procedure CMIconChanged(var Message: TMessage); message CM_ICONCHANGED;
    procedure CMRelease(var Message: TMessage); message CM_RELEASE;
    procedure CMTextChanged(var Message: TMessage); message CM_TEXTCHANGED;
    procedure CMUIActivate(var Message); message CM_UIACTIVATE;
    procedure CMParentBiDiModeChanged(var Message: TMessage); message CM_PARENTBIDIMODECHANGED;
    procedure CMParentFontChanged(var Message: TMessage); message CM_PARENTFONTCHANGED;
    procedure CMIsShortCut(var Message: TWMKey); message CM_ISSHORTCUT;
    procedure SetAlphaBlend(const Value: Boolean);
    procedure SetAlphaBlendValue(const Value: Byte);
    procedure SetTransparentColor(const Value: Boolean);
    procedure SetTransparentColorValue(const Value: TColor);
    procedure InitAlphaBlending(var Params: TCreateParams);
  protected
    FFormState: TFormState;
    procedure AlignControls(AControl: TControl; var Rect: TRect); override;
    procedure BeginAutoDrag; override;
    procedure ChangeScale(M, D: Integer); override;
    procedure DefineProperties(Filer: TFiler); override;
    function GetClientRect: TRect; override;
    procedure GetChildren(Proc: TGetChildProc; Root: TComponent); override;
    function GetFloating: Boolean; override;
    function HandleCreateException: Boolean; dynamic;
    function PaletteChanged(Foreground: Boolean): Boolean; override;
    procedure RequestAlign; override;
    procedure SetChildOrder(Child: TComponent; Order: Integer); override;
    procedure DoDock(NewDockSite: TWinControl; var ARect: TRect); override;
    procedure UpdateWindowState;
    procedure Resizing(State: TWindowState); override;
    property ActiveMDIChild: TForm read GetActiveMDIChild;
    property AlphaBlend: Boolean read FAlphaBlend write SetAlphaBlend;
    property AlphaBlendValue: Byte read FAlphaBlendValue write SetAlphaBlendValue;
    property BorderIcons: TBorderIcons read FBorderIcons write SetBorderIcons stored IsForm
      default [biSystemMenu, biMinimize, biMaximize];
    property AutoScroll stored IsAutoScrollStored;
    property TransparentColor: Boolean read FTransparentColor write SetTransparentColor;
    property TransparentColorValue: TColor read FTransparentColorValue write SetTransparentColorValue;
    property DefaultMonitor: TDefaultMonitor read FDefaultMonitor write FDefaultMonitor
      stored IsForm default dmActiveForm;
    property FormStyle: TFormStyle read FFormStyle write SetFormStyle
      stored IsForm default fsNormal;
    property HorzScrollBar stored IsForm;
    property MDIChildCount: Integer read GetMDIChildCount;
    property MDIChildren[I: Integer]: TForm read GetMDIChildren;
    property ParentFont default False;
    property Position: TPosition read FPosition write SetPosition stored IsForm
      default poDesigned;
    property TileMode: TTileMode read FTileMode write FTileMode default tbHorizontal;
    property VertScrollBar stored IsForm;
    property WindowMenu: TMenuItem read FWindowMenu write SetWindowMenu stored IsForm;
    property OnCanResize stored IsForm;
    property OnHelp: THelpEvent read FOnHelp write FOnHelp;
    property OnShortCut: TShortCutEvent read FOnShortCut write FOnShortCut;
  public
    procedure AfterConstruction; override;
    procedure BeforeDestruction; override;
    function CloseQuery: Boolean; virtual;
    procedure DefocusControl(Control: TWinControl; Removing: Boolean);
    procedure Dock(NewDockSite: TWinControl; ARect: TRect); override;
    procedure FocusControl(Control: TWinControl);
    function IsShortCut(var Message: TWMKey): Boolean; dynamic;
    procedure MakeFullyVisible(AMonitor: TMonitor = nil);
    procedure SendCancelMode(Sender: TControl);
    property BorderStyle: TFormBorderStyle read FBorderStyle write SetBorderStyle
      stored IsForm default bsSizeable;
    property DropTarget: Boolean read FDropTarget write FDropTarget;
    property FormState: TFormState read FFormState;
    property HelpFile: string read FHelpFile write FHelpFile;
    property KeyPreview: Boolean read FKeyPreview write FKeyPreview
      stored IsForm default False;
    property Menu: TMainMenu read FMenu write SetMenu stored IsForm;
    property ModalResult: TModalResult read FModalResult write FModalResult;
    property Monitor: TMonitor read GetMonitor;
    property WindowState: TWindowState read FWindowState write SetWindowState
      stored IsForm default wsNormal;
  end;


*/
//---------------------------------------------------------------------------
//	THintWindow
//---------------------------------------------------------------------------
THintWindow :: THintWindow()
{
    Init();
}
//---------------------------------------------------------------------------
void THintWindow :: Init()
{
    FormType = ftTooltip;
    PauseTime = 800;
    ShowTime = 3000;
    MaxWidth = 0;
    m_Control = NULL;
    Color = clInfoBk;
    OverrideRedirect = true;
    BorderWidth = 1;
    BorderColor = clBlack;
    m_timer.OnTimer = TNotifyEvent(this, &THintWindow :: DoHint);
}
//---------------------------------------------------------------------------
void THintWindow :: DoHint(TObject *Sender)
{
    Canvas.Lock();
    if(CheckState(csVisible))
    {
        Visible = false;
    }
    else
    {
        BoundsRect = CalcHintRect();
        Visible = true;
        m_timer.Interval = ShowTime;
    }
    Canvas.Unlock();
}
//---------------------------------------------------------------------------
void THintWindow :: Activate(const TControl *ctrl)
{
    m_Control = ctrl;
    m_timer.Interval = PauseTime;
    m_timer.Enabled = true;
}
//---------------------------------------------------------------------------
void THintWindow :: Paint()
{
    Canvas.Brush->Color = clNone;
    Canvas.TextRect(ClientRect, 0, 0, m_Control->Hint, alCenter);
}
//---------------------------------------------------------------------------
TRect THintWindow :: CalcHintRect()
{
    TRect ret = Canvas.TextExtents(m_Control->Hint);
    ret.width += 8;
    ret.height += 6;
    TPoint pt = Mouse.CursorPos;
    ret.left = pt.x;
    ret.top = pt.y + ret.height - 2;    // Cursor height
    return ret;
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
