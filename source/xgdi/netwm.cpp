//---------------------------------------------------------------------------
#include <stdio.h>
#include <unistd.h>
#include "sysutils.h"
#include "application.h"
#include "netwm.h"
//---------------------------------------------------------------------------
#define ATOMNAME(id)   __atom_names[id]
#define ATOMVALUE(id)  __atoms[id]
#define CHECK_SUPPORTED(id)  ATOMVALUE(id) ? true : false
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
static const char *__atom_names[] =
{
    "UTF8_STRING",
    "CLIPBOARD",
    "TARGETS",
    "WM_DELETE_WINDOW",
    "WM_TAKE_FOCUS",
    "WM_SAVE_YOURSELF",
    "WM_PROTOCOLS",
    "_NET_SUPPORTED",
    "_NET_CLIENT_LIST",
    "_NET_CLIENT_LIST_STACKING",
    "_NET_NUMBER_OF_DESKTOPS",
    "_NET_DESKTOP_GEOMETRY",
    "_NET_DESKTOP_VIEWPORT",
    "_NET_CURRENT_DESKTOP",
    "_NET_DESKTOP_NAMES",
    "_NET_ACTIVE_WINDOW",
    "_NET_WORKAREA",
    "_NET_SUPPORTING_WM_CHECK",
    "_NET_VIRTUAL_ROOTS",
    "_NET_DESKTOP_LAYOUT",
    "_NET_SHOWING_DESKTOP",
    "_NET_CLOSE_WINDOW",
    "_NET_MOVERESIZE_WINDOW",
    "_NET_WM_MOVERESIZE",
    "_NET_RESTACK_WINDOW",
    "_NET_REQUEST_FRAME_EXTENTS",
    "_NET_WM_NAME",
    "_NET_WM_VISIBLE_NAME",
    "_NET_WM_ICON_NAME",
    "_NET_WM_VISIBLE_ICON_NAME",
    "_NET_WM_DESKTOP",
    "_NET_WM_WINDOW_TYPE",
    "_NET_WM_STATE",
    "_NET_WM_ALLOWED_ACTIONS",
    "_NET_WM_STRUT",
    "_NET_WM_STRUT_PARTIAL",
    "_NET_WM_ICON_GEOMETRY",
    "_NET_WM_ICON",
    "_NET_WM_PID",
    "_NET_WM_HANDLED_ICONS",
    "_NET_WM_USER_TIME",
    "_NET_WM_USER_TIME_WINDOW",
    "_NET_FRAME_EXTENTS",
    "_NET_WM_OPAQUE_REGION",
    "_NET_WM_PING",
    "_NET_WM_SYNC_REQUEST",
    "_NET_WM_FULLSCREEN_MONITORS",
    "_NET_WM_FULL_PLACEMENT",
    "_NET_WM_CM_Sn"
};

static Atom __atoms[SIZEOFARRAY(__atom_names)];

void InitNETWM()
{
    int n = SIZEOFARRAY(__atom_names);
    Atom m_atom = XInternAtom(Application.Handle, ATOMNAME(_NET_SUPPORTED), true);
    if(m_atom)
    {
        XInternAtoms(Application.Handle, (char **)__atom_names, n, true, __atoms);
    }
    else
    {
        TAtomList al;

        m_atom = XInternAtom(Application.Handle, ATOMNAME(_NET_SUPPORTED), false);
        __atoms[_NET_SUPPORTED] = m_atom;

        m_atom = XInternAtom(Application.Handle, ATOMNAME(_NET_WM_NAME), false);
        __atoms[_NET_WM_NAME] = m_atom;

        al.Add(m_atom);
        SetNETWMSupport(al);

    }

    TAtomList al;


    XInternAtoms(Application.Handle, (char **)__atom_names, n, true, __atoms);
    for(register int i = 0; i < SIZEOFARRAY(__atom_names); i++)
    {
        if(__atoms[i] != None)
            continue;
        switch(i)
        {
        case _NET_SUPPORTED:
            m_atom = XInternAtom(Application.Handle, __atom_names[_NET_SUPPORTED], false);
            __atoms[_NET_SUPPORTED] = m_atom;
            al.Add(m_atom);
            SetNETWMSupport(al);
            break;
        case _NET_CLIENT_LIST:
            break;
        }
    }

    /*
     * supported atoms
    _NET_WM_NAME
    _NET_CLOSE_WINDOW
    _NET_WM_STATE
    _NET_WM_STATE_SHADED
    _NET_WM_STATE_MAXIMIZED_HORZ
    _NET_WM_STATE_MAXIMIZED_VERT
    _NET_WM_DESKTOP
    _NET_NUMBER_OF_DESKTOPS
    _NET_CURRENT_DESKTOP
    _NET_WM_WINDOW_TYPE
    _NET_WM_WINDOW_TYPE_DESKTOP
    _NET_WM_WINDOW_TYPE_DOCK
    _NET_WM_WINDOW_TYPE_TOOLBAR
    _NET_WM_WINDOW_TYPE_MENU
    _NET_WM_WINDOW_TYPE_DIALOG
    _NET_WM_WINDOW_TYPE_NORMAL
    _NET_WM_STATE_MODAL
    _NET_CLIENT_LIST
    _NET_CLIENT_LIST_STACKING
    _NET_WM_STATE_SKIP_TASKBAR
    _NET_WM_STATE_SKIP_PAGER
    _NET_WM_ICON_NAME
    _NET_WM_ICON
    _NET_WM_ICON_GEOMETRY
    _NET_WM_MOVERESIZE
    _NET_ACTIVE_WINDOW
    _NET_WM_STRUT
    _NET_WM_STATE_HIDDEN
    _NET_WM_WINDOW_TYPE_UTILITY
    _NET_WM_WINDOW_TYPE_SPLASH
    _NET_WM_STATE_FULLSCREEN
    _NET_WM_PING
    _NET_WM_PID
    _NET_WORKAREA
    _NET_SHOWING_DESKTOP
    _NET_DESKTOP_LAYOUT
    _NET_DESKTOP_NAMES
    _NET_WM_ALLOWED_ACTIONS
    _NET_WM_ACTION_MOVE
    _NET_WM_ACTION_RESIZE
    _NET_WM_ACTION_SHADE
    _NET_WM_ACTION_STICK
    _NET_WM_ACTION_MAXIMIZE_HORZ
    _NET_WM_ACTION_MAXIMIZE_VERT
    _NET_WM_ACTION_CHANGE_DESKTOP
    _NET_WM_ACTION_CLOSE
    _NET_WM_STATE_ABOVE
    _NET_WM_STATE_BELOW
    _NET_STARTUP_ID
    _NET_WM_STRUT_PARTIAL
    _NET_WM_ACTION_FULLSCREEN
    _NET_WM_ACTION_MINIMIZE
    _NET_FRAME_EXTENTS
    _NET_REQUEST_FRAME_EXTENTS
    _NET_WM_USER_TIME
    _NET_WM_STATE_DEMANDS_ATTENTION
    _NET_MOVERESIZE_WINDOW
    _NET_DESKTOP_GEOMETRY
    _NET_DESKTOP_VIEWPORT
    _NET_WM_USER_TIME_WINDOW
    _NET_WM_ACTION_ABOVE
    _NET_WM_ACTION_BELOW
    _NET_WM_STATE_STICKY
    _NET_WM_FULLSCREEN_MONITORS

     */
    /*
    _NET_SUPPORTED
    _NET_CLIENT_LIST
    _NET_CLIENT_LIST_STACKING
    _NET_NUMBER_OF_DESKTOPS
    _NET_DESKTOP_GEOMETRY
    _NET_DESKTOP_VIEWPORT
    _NET_CURRENT_DESKTOP
    _NET_DESKTOP_NAMES
    _NET_ACTIVE_WINDOW
    _NET_WORKAREA
    _NET_SUPPORTING_WM_CHECK
    _NET_VIRTUAL_ROOTS
    _NET_DESKTOP_LAYOUT
    _NET_SHOWING_DESKTOP

    _NET_CLOSE_WINDOW
    _NET_MOVERESIZE_WINDOW
    _NET_WM_MOVERESIZE
    _NET_RESTACK_WINDOW
    _NET_REQUEST_FRAME_EXTENTS

    _NET_WM_NAME
    _NET_WM_VISIBLE_NAME
    _NET_WM_ICON_NAME
    _NET_WM_VISIBLE_ICON_NAME
    _NET_WM_DESKTOP
    _NET_WM_WINDOW_TYPE
    _NET_WM_STATE
    _NET_WM_ALLOWED_ACTIONS
    _NET_WM_STRUT
    _NET_WM_STRUT_PARTIAL
    _NET_WM_ICON_GEOMETRY
    _NET_WM_ICON
    _NET_WM_PID
    _NET_WM_HANDLED_ICONS
    _NET_WM_USER_TIME
    _NET_WM_USER_TIME_WINDOW
    _NET_FRAME_EXTENTS
    _NET_WM_OPAQUE_REGION

    _NET_WM_PING
    _NET_WM_SYNC_REQUEST
    _NET_WM_FULLSCREEN_MONITORS

    _NET_WM_FULL_PLACEMENT

    _NET_WM_CM_Sn Manager Selection
    */

}
//---------------------------------------------------------------------------
//  X window utils
//---------------------------------------------------------------------------
void __fastcall GetWindowRect(HWND wnd, TRect &rect)
{
    Window root;
    int x, y;
    unsigned int width, height;
    unsigned int bw;
    unsigned int depth;

    XGetGeometry(Application.Handle, wnd, &root, &x, &y, &width,
                 &height, &bw, &depth);

    rect.left = x - bw;
    rect.top  = y - bw;
    rect.width = width + 2 * bw;
    rect.height = height + 2 * bw;
}
//---------------------------------------------------------------------------
void __fastcall MoveWindow(HWND wnd, int l, int t, int w, int h)
{
    Window root;
    int x, y;
    unsigned int width, height;
    unsigned int bw;
    unsigned int depth;

    XGetGeometry(Application.Handle, wnd, &root, &x, &y, &width,
                 &height, &bw, &depth);

    int bw2 = bw << 1;
    if( w < bw2 + 1 )
        w = bw2 + 1;
    if( h < bw2 + 1)
        h = bw2 + 1;
    XMoveResizeWindow(Application.Handle, wnd, l, t, w - bw2, h - bw2);
}
//---------------------------------------------------------------------------
void __fastcall GetClientRect(HWND wnd, TRect &rect)
{
    Window root;
    int x, y;
    unsigned int width, height;
    unsigned int bw;
    unsigned int depth;

//    if(!CheckMainThread())
//        WakeupWindow(wnd);
    XGetGeometry(Application.Handle, wnd, &root, &x, &y, &width,
                 &height, &bw, &depth);

    rect.left = 0;
    rect.top  = 0;
    rect.width = width;
    rect.height = height;
}
//---------------------------------------------------------------------------
void __fastcall InvalidateRect(HWND wnd, const TRect &rect)
{
    XEvent event;
    event.type = Expose;
    event.xexpose.display = Application.Handle;
    event.xexpose.window = wnd;
    event.xexpose.x = rect.left;
    event.xexpose.y = rect.top;
    event.xexpose.width = rect.width;
    event.xexpose.height = rect.height;
    event.xexpose.count = 777;
    XPutBackEvent(Application.Handle, &event);
//    XSync(Application.Handle, false);
}
//---------------------------------------------------------------------------
void __fastcall UpdateWindowRect(HWND wnd, const TRect &rect)
{
    XEvent event;
    event.type = Expose;
    event.xexpose.display = Application.Handle;
    event.xexpose.window = wnd;
    event.xexpose.x = rect.left;
    event.xexpose.y = rect.top;
    event.xexpose.width = rect.width;
    event.xexpose.height = rect.height;
    event.xexpose.count = 0;
//    XPutBackEvent(Application.Handle, &event);
    XSendEvent(Application.Handle, wnd, false, ExposureMask, &event);
//    XSync(Application.Handle, false);
    XFlush(Application.Handle);
}
//---------------------------------------------------------------------------
void __fastcall UpdateWindow(HWND wnd)
{
//    TRect rect;
//    GetClientRect(wnd, rect);
    UpdateWindowRect(wnd, TRect(0,0,35000,35000));
}
//---------------------------------------------------------------------------
//  X property utils
//---------------------------------------------------------------------------
Atom __fastcall GetAtom(TAtomIdent idx)
{
    return __atoms[idx];
}
//---------------------------------------------------------------------------
Atom __fastcall GetAtomXProperty(const char *name, bool create)
{
    return XInternAtom(Application.Handle, name, !create);
}
//---------------------------------------------------------------------------
void * __fastcall GetXProperty(HWND wnd, Atom prop, int *nitems)
{
    Atom type_ret;
    int format_ret;
    unsigned long items_ret = 0;
    unsigned long bytes_left;
    unsigned char *prop_data = NULL;

//   int read_bytes = 1024;

//   do
//	{
//		if(prop_data != 0)
//			XFree(prop_data);
    if(XGetWindowProperty(Application.Handle, wnd, prop, 0, -1, false,
                          AnyPropertyType, &type_ret, &format_ret, &items_ret,
                          &bytes_left, &prop_data) != Success)
        return NULL;
//       read_bytes *= 2;
//	}while(bytes_left != 0);

    if(nitems)
        *nitems = items_ret;
    return prop_data;
}
//---------------------------------------------------------------------------
void __fastcall SetXProperty(HWND wnd, Atom prop, Atom type,
                             int format, void *data, int nelements)
{
    unsigned char *prop_data = (unsigned char *)data;

    XChangeProperty(Application.Handle, wnd, prop, type, format, PropModeReplace, prop_data, nelements);
}
//---------------------------------------------------------------------------
void __fastcall AddXProperty(HWND wnd, Atom prop, Atom type,
                             int format, void *data, int nelements)
{
    unsigned char *prop_data = (unsigned char *)data;

    XChangeProperty(Application.Handle, wnd, prop, type, format, PropModeAppend, prop_data, nelements);
}
//---------------------------------------------------------------------------
void __fastcall DelXProperty(HWND wnd, Atom prop)
{
    XDeleteProperty(Application.Handle, wnd, prop);
}
//---------------------------------------------------------------------------
//	_NET_SUPPORTED
//---------------------------------------------------------------------------
int __fastcall GetNETWMSupport(TAtomList *al, TStrings *names)
{
    Atom m_atom;
    Atom type_ret;
    int format_ret;
    unsigned long items_ret;
    unsigned long after_ret;
    unsigned char *prop_data = 0;
    Atom *atoms;
    char *names_ret;

    m_atom = ATOMVALUE(_NET_SUPPORTED);
    if(XGetWindowProperty(Application.Handle, Screen.Root, m_atom, 0, -1, false,
                          XA_ATOM, &type_ret, &format_ret, &items_ret,
                          &after_ret, &prop_data) != Success)
        return 0;
    atoms = (Atom *)prop_data;
    if(al)
    {
        al->Clear();
        al->Add(atoms, items_ret);
    }
    if(names)
    {
        for(register unsigned int i = 0; i < items_ret; i++)
        {
            names_ret = XGetAtomName(Application.Handle, atoms[i]);
            names->Add(names_ret);
            XFree(names_ret);
        }
    }
    XFree(prop_data);
    return (int)items_ret;
}
//---------------------------------------------------------------------------
void __fastcall SetNETWMSupport(TAtomList &al)
{
    int nelements = al.Count;
    Atom *atoms = new Atom[nelements];
    for(register int i = 0; i < nelements; i++)
        atoms[i] = al[i];
    unsigned char *prop_data = (unsigned char *)atoms;
    Atom m_atom = ATOMVALUE(_NET_SUPPORTED);;
    XChangeProperty(Application.Handle, Screen.Root, m_atom, XA_ATOM, 32, PropModeReplace, prop_data, nelements);
    delete [] atoms;
}
//---------------------------------------------------------------------------
void __fastcall CloseWindow(HWND wnd)
{
    XEvent event;
    long event_mask = SubstructureNotifyMask | SubstructureRedirectMask;

    event.xclient.type = ClientMessage;
    event.xclient.window = wnd;
    event.xclient.message_type = XInternAtom(Application.Handle, "_NET_CLOSE_WINDOW", true);
    event.xclient.format = 32;
    event.xclient.data.l[0] = CurrentTime;
    event.xclient.data.l[1] = 1;
    for(register int i = 2; i < 5; i++)
        event.xclient.data.l[i] = 0;
    XSendEvent(Application.Handle, Screen.Root, false, event_mask, &event);
}
//---------------------------------------------------------------------------
void __fastcall SetWMProtocols(HWND wnd)
{
    Atom m_atom[4];
    m_atom[0] = XInternAtom(Application.Handle, "WM_DELETE_WINDOW", true);
    m_atom[1] = XInternAtom(Application.Handle, "WM_TAKE_FOCUS", true);
    m_atom[2] = XInternAtom(Application.Handle, "WM_SAVE_YOURSELF", true);
    m_atom[3] = XInternAtom(Application.Handle, "_NET_WM_PING", true);
    XSetWMProtocols(Application.Handle, wnd, m_atom, 4);
}
//---------------------------------------------------------------------------
void __fastcall QueryFrameExtents(HWND wnd)
{
    XEvent event;
    long event_mask = SubstructureNotifyMask | SubstructureRedirectMask;

    event.xclient.type = ClientMessage;
    event.xclient.window = wnd;
    event.xclient.message_type = XInternAtom(Application.Handle, "_NET_REQUEST_FRAME_EXTENTS", true);
    event.xclient.format = 32;
    for(register int i = 0; i < 5; i++)
        event.xclient.data.l[i] = 0;
    XSendEvent(Application.Handle, Screen.Root, false, event_mask, &event);
}
//---------------------------------------------------------------------------
void __fastcall GetFrameExtents(HWND wnd, long *exts)
{
    Atom type_ret;
    int format_ret;
    unsigned long items_ret;
    unsigned long after_ret;
    unsigned char *prop_data = 0;

    memset(exts, 0, 4 * sizeof(long));
    Atom m_atom = XInternAtom(Application.Handle, "_NET_FRAME_EXTENTS", true);

    if(XGetWindowProperty(Application.Handle, wnd, m_atom, 0, -1, false,
                          XA_CARDINAL, &type_ret, &format_ret, &items_ret,
                          &after_ret, &prop_data) == Success)
    {
        memcpy(exts, prop_data, items_ret * sizeof(long));
        XFree(prop_data);
    }
}
//---------------------------------------------------------------------------
void __fastcall SetFrameExtents(HWND wnd, long left, long top, long right, long bottom)
{
    unsigned long ul[4];
    ul[0] = left;
    ul[1] = right;
    ul[2] = top;
    ul[3] = bottom;
    Atom m_atom = XInternAtom(Application.Handle, "_NET_FRAME_EXTENTS", true);
    XChangeProperty(Application.Handle, wnd, m_atom, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)ul, 4);
    /*
    	XChangeProperty(display, w, property, type, format, mode, data, nelements)
          Display *display;
          Window w;
          Atom property, type;
          int format;
          int mode;
          unsigned char *data;
          int nelements; */
}
//---------------------------------------------------------------------------
int __fastcall GetDesktop(HWND wnd)
{
    Atom m_atom;
    Atom type_ret;
    int format_ret;
    unsigned long items_ret;
    unsigned long after_ret;
    unsigned char *prop_data = 0;
    int ret;

    m_atom = XInternAtom(Application.Handle, "_NET_WM_DESKTOP", true);
    if(XGetWindowProperty(Application.Handle, wnd, m_atom, 0, -1, false,
                          XA_CARDINAL, &type_ret, &format_ret, &items_ret,
                          &after_ret, &prop_data) != Success)
        return 0;
    ret = prop_data ? *(int *)prop_data : 0;
    XFree(prop_data);
    return ret;
}
//---------------------------------------------------------------------------
void __fastcall SetDesktop(HWND wnd, int val)
{
    Atom m_atom = XInternAtom(Application.Handle, "_NET_WM_DESKTOP", true);
    XChangeProperty(Application.Handle, wnd, m_atom, XA_CARDINAL, 32,
                    PropModeReplace, (const unsigned char *)&val, 1);

    XWindowAttributes winattrib;
    XGetWindowAttributes(Application.Handle, wnd, &winattrib);
    if(winattrib.map_state != IsUnmapped)
    {
        XEvent event;
        long event_mask = SubstructureNotifyMask | SubstructureRedirectMask;
        event.xclient.type = ClientMessage;
        event.xclient.window = wnd;
        event.xclient.message_type = m_atom;
        event.xclient.format = 32;
        event.xclient.data.l[0] = val;
        event.xclient.data.l[1] = 1;
        for(register int i = 2; i < 5; i++)
            event.xclient.data.l[i] = 0;
        XSendEvent(Application.Handle, Screen.Root, false, event_mask, &event);
    }
}
//---------------------------------------------------------------------------
int GetDesktopCount()
{
    Atom m_atom;
    Atom type_ret;
    int format_ret;
    unsigned long items_ret;
    unsigned long after_ret;
    unsigned char *prop_data = 0;
    int ret;

    m_atom = XInternAtom(Application.Handle, "_NET_NUMBER_OF_DESKTOPS", true);
    if(XGetWindowProperty(Application.Handle, Screen.Root, m_atom, 0, -1, false,
                          XA_CARDINAL, &type_ret, &format_ret, &items_ret,
                          &after_ret, &prop_data) != Success)
        return 0;
    ret = prop_data ? *(int *)prop_data : 0;
    XFree(prop_data);
    return ret;
}
//---------------------------------------------------------------------------
void __fastcall SetDesktopCount(int n)
{
    Atom m_atom = XInternAtom(Application.Handle, "_NET_NUMBER_OF_DESKTOPS", true);
    XEvent event;
    long event_mask = SubstructureNotifyMask | SubstructureRedirectMask;
    event.xclient.type = ClientMessage;
    event.xclient.window = Screen.Root;
    event.xclient.message_type = m_atom;
    event.xclient.format = 32;
    event.xclient.data.l[0] = n;
    for(register int i = 1; i < 5; i++)
        event.xclient.data.l[i] = 0;
    XSendEvent(Application.Handle, Screen.Root, false, event_mask, &event);
}
//---------------------------------------------------------------------------
int GetCurrentDesktop()
{
    Atom m_atom;
    Atom type_ret;
    int format_ret;
    unsigned long items_ret;
    unsigned long after_ret;
    unsigned char *prop_data = 0;
    int ret;

    m_atom = XInternAtom(Application.Handle, "_NET_CURRENT_DESKTOP", true);
    if(XGetWindowProperty(Application.Handle, Screen.Root, m_atom, 0, -1, false,
                          XA_CARDINAL, &type_ret, &format_ret, &items_ret,
                          &after_ret, &prop_data) != Success)
        return 0;
    ret = prop_data ? *(int *)prop_data : 0;
    XFree(prop_data);
    return ret;
}
//---------------------------------------------------------------------------
void __fastcall SetCurrentDesktop(int n)
{
    Atom m_atom = XInternAtom(Application.Handle, "_NET_CURRENT_DESKTOP", true);
    XEvent event;
    long event_mask = SubstructureNotifyMask | SubstructureRedirectMask;
    event.xclient.type = ClientMessage;
    event.xclient.window = Screen.Root;
    event.xclient.message_type = m_atom;
    event.xclient.format = 32;
    event.xclient.data.l[0] = n;
    event.xclient.data.l[1] = CurrentTime;
    for(register int i = 2; i < 5; i++)
        event.xclient.data.l[i] = 0;
    XSendEvent(Application.Handle, Screen.Root, false, event_mask, &event);
}
//---------------------------------------------------------------------------
void __fastcall SendPing(HWND wnd, XEvent *back)
{
    long event_mask = SubstructureNotifyMask | SubstructureRedirectMask;

    if(back)
    {
        back->xclient.window = Screen.Root;
        XSendEvent(Application.Handle, Screen.Root, false, event_mask, back);
    }
    else
    {
        Atom m_atom = XInternAtom(Application.Handle, "_NET_WM_PING", true);
        Atom m_atom1 = XInternAtom(Application.Handle, "WM_PROTOCOLS", true);
        XEvent event;
        event.xclient.type = ClientMessage;
        event.xclient.window = wnd;
        event.xclient.message_type = m_atom1;
        event.xclient.format = 32;
        event.xclient.data.l[0] = m_atom;
        event.xclient.data.l[1] = CurrentTime;
        event.xclient.data.l[2] = wnd;
        for(register int i = 3; i < 5; i++)
            event.xclient.data.l[i] = 0;
        XSendEvent(Application.Handle, Screen.Root, false, event_mask, &event);
    }
}
//---------------------------------------------------------------------------
HWND GetActiveWindow()
{
    Atom m_atom;
    Atom type_ret;
    int format_ret;
    unsigned long items_ret;
    unsigned long after_ret;
    unsigned char *prop_data = 0;
    HWND ret;

    m_atom = XInternAtom(Application.Handle, "_NET_ACTIVE_WINDOW", true);
    if(m_atom == None)
    {
        XGetInputFocus(Application.Handle, &ret, &format_ret);
//        if(ret == PointerRoot)
//            ret =
        return ret;
    }
    if(XGetWindowProperty(Application.Handle, Screen.Root, m_atom, 0, -1, false,
                          XA_WINDOW, &type_ret, &format_ret, &items_ret,
                          &after_ret, &prop_data) != Success)
        return None;
    ret = prop_data ? *(HWND *)prop_data : None;
    XFree(prop_data);
    return ret;
}
//---------------------------------------------------------------------------
void __fastcall SetActiveWindow(HWND wnd)
{
    XEvent event;
    long event_mask = SubstructureNotifyMask | SubstructureRedirectMask;

    event.xclient.type = ClientMessage;
    event.xclient.window = wnd;
    event.xclient.message_type = XInternAtom(Application.Handle, "_NET_ACTIVE_WINDOW", true);
    event.xclient.format = 32;
    event.xclient.data.l[0] = 1;
    event.xclient.data.l[1] = CurrentTime;
    for(register int i = 2; i < 5; i++)
        event.xclient.data.l[i] = 0;
    XSendEvent(Application.Handle, Screen.Root, false, event_mask, &event);
}
//---------------------------------------------------------------------------
void __fastcall GetAppClass(HWND wnd, TString &appname, TString &appclass)
{
    XClassHint chint;
    XGetClassHint(Application.Handle, wnd, &chint);
    appname = chint.res_name;
    appclass = chint.res_class;
}
//---------------------------------------------------------------------------
void __fastcall SetAppClass(HWND wnd, const TString &appname, const TString &appclass)
{
    XClassHint chint;
    chint.res_name = (char *)appname.c_str();
    chint.res_class = (char *)appclass.c_str();
    XSetClassHint(Application.Handle, wnd, &chint);
}
//---------------------------------------------------------------------------
TString GetWMName()
{
    TString ret;
    HWND *pwnd, wnd1, wnd2;
    char *stmp;
    Atom prop = GetAtomXProperty("_NET_SUPPORTING_WM_CHECK", false);
    pwnd = (HWND *)GetXProperty(Screen.Root, prop);
    wnd1 = *pwnd;
    XFree(pwnd);
    pwnd = (HWND *)GetXProperty(wnd1, prop);
    wnd2 = *pwnd;
    XFree(pwnd);
    if(wnd1 == wnd2)
    {
        prop = GetAtomXProperty("_NET_WM_NAME", false);
        stmp = (char *)GetXProperty(wnd1, prop);
        ret = stmp;
        XFree(stmp);
    }
    return ret;
}
//---------------------------------------------------------------------------
void __fastcall GetDesktopNames(TStrings &out)
{
    int num;
    Atom utf8;
    Atom prop;
    unsigned char *list;
    char **ls;
    XTextProperty txtprop;

    prop = GetAtomXProperty("_NET_DESKTOP_NAMES", false);
    utf8 = GetAtomXProperty("UTF8_STRING", false);
    list = (unsigned char *)GetXProperty(Screen.Root, prop, &num);
    txtprop.value = list;
    txtprop.encoding = utf8;
    txtprop.format = 8;
    txtprop.nitems = num;
    Xutf8TextPropertyToTextList(Application.Handle, &txtprop, &ls, &num);
    out.Add(ls, num - 1);
    XFree(list);
    XFreeStringList(ls);
}
//---------------------------------------------------------------------------
TRect __fastcall GetWorkArea(int desktop)
{
    int num;
    Atom prop;
    int *list, *out;
    TRect ret;

    prop = GetAtomXProperty("_NET_WORKAREA", false);
    list = (int *)GetXProperty(Screen.Root, prop, &num);
    if(desktop * 4 < num)
    {
        out = list + (desktop * 4);
        ret = TRect(out[0], out[1], out[2], out[3]);
    }
    XFree(list);
    return ret;
}
//---------------------------------------------------------------------------
TString __fastcall GetClientMachine(HWND wnd)
{
    TString ret;
    XTextProperty tp;

    XGetWMClientMachine(Application.Handle, wnd, &tp);
    if(tp.value)
        ret = (char *)tp.value;
    return ret;
}
//---------------------------------------------------------------------------
void __fastcall SetClientMachine(HWND wnd)
{
    XTextProperty tp;
    char buf[2 * HOST_NAME_MAX + 1];
    char *tmp;
    int len;

    gethostname(buf, HOST_NAME_MAX);
    len = strlen(buf);
    tmp = buf + len;
    *tmp = '.';
    getdomainname(tmp + 1, HOST_NAME_MAX);
    tp.value = (unsigned char *)buf;
    tp.encoding = XA_STRING;
    tp.format = 8;
    tp.nitems = strlen(buf);
    XSetWMClientMachine(Application.Handle, wnd, &tp);
}
//---------------------------------------------------------------------------
pid_t __fastcall GetWMpid(HWND wnd)
{
    Atom prop = GetAtomXProperty("_NET_WM_PID", false);
    pid_t *pid = NULL, ret;
    pid = (pid_t *)GetXProperty(wnd, prop);
    if(pid != NULL)
    {
        ret = *pid;
        XFree(pid);
    }
    else
        ret = 0;
    return ret;
}
//---------------------------------------------------------------------------
void __fastcall SetWMpid(HWND wnd)
{
    Atom prop = GetAtomXProperty("_NET_WM_PID", false);
    pid_t pid = getpid();
    SetXProperty(wnd, prop, XA_CARDINAL, 32, (void *)&pid, 1);
    SetClientMachine(wnd);
}
//---------------------------------------------------------------------------
static void __fastcall __copytodc(HWND wnd, HDC hbmp, XWindowAttributes *parent_winattrib)
{
//    Window root, wtmp;
//    Window parent;
//    Window *children;
//    unsigned int nchildren;
//    XWindowAttributes child_winattrib;

    unsigned long valuemask = GCSubwindowMode;
    XGCValues values;

    XGetGCValues(Application.Handle, Screen.GC, valuemask, &values);
    int oldmode = values.subwindow_mode;
    XSetSubwindowMode(Application.Handle, Screen.GC, IncludeInferiors);

    XCopyArea(Application.Handle, wnd, hbmp, Screen.GC, 0, 0,
              parent_winattrib->width + 2 * parent_winattrib->border_width, parent_winattrib->height  + 2 * parent_winattrib->border_width,
              parent_winattrib->x + parent_winattrib->border_width, parent_winattrib->y + parent_winattrib->border_width);

    XSetSubwindowMode(Application.Handle, Screen.GC, oldmode);

    /*    XQueryTree(Application.Handle, wnd, &root, &parent, &children, &nchildren);
        if(children != NULL)
        {
            for(register unsigned int i = 0; i < nchildren; i++)
            {
                wtmp = children[i];
                XGetWindowAttributes(Application.Handle, wtmp, &child_winattrib);
                if(child_winattrib.depth != CScreen.Depth || child_winattrib.map_state != IsViewable)
                    continue;
                child_winattrib.x += parent_winattrib->x;
                child_winattrib.y += parent_winattrib->y;
                __copytobmp(wtmp, hbmp, &child_winattrib);
            }
            XFree(children);
        } */
}
//---------------------------------------------------------------------------
HBITMAP __fastcall WindowToBitmap(HWND wnd)
{
    XWindowAttributes winattrib;
    HBITMAP ret;

    XGetWindowAttributes(Application.Handle, wnd, &winattrib);
    if(winattrib.depth != Screen.Depth || winattrib.map_state != IsViewable)
        return 0;
    winattrib.x = 0;
    winattrib.y = 0;
    ret = XCreatePixmap(Application.Handle, wnd, winattrib.width, winattrib.height, winattrib.depth);
    __copytodc(wnd, ret, &winattrib);

    return ret;
}
//---------------------------------------------------------------------------
/*
static void __fastcall __copytodcrect(HWND wnd, HDC hbmp, HWND wmain, const TRect &rect)
{
    Window root, wtmp;
    Window parent;
    Window *children;
    unsigned int nchildren;
    XWindowAttributes child_winattrib;
    int dest_x, dest_y;
    int src_x, src_y;
    TRect tmprect1, tmprect2;

    XGetWindowAttributes(Application.Handle, wnd, &child_winattrib);
    if(child_winattrib.depth != CScreen.Depth || child_winattrib.map_state != IsViewable)
                return;
    if(!XTranslateCoordinates(Application.Handle, wnd, wmain, child_winattrib.x, child_winattrib.y, &dest_x, &dest_y, &root))
                return;
    tmprect1 = TRect(dest_x - child_winattrib.x, dest_y - child_winattrib.y, child_winattrib.width, child_winattrib.height);
    if(!const_cast<TRect &>(rect).intersect(tmprect1, tmprect2))
                return;
    XTranslateCoordinates(Application.Handle, wmain, wnd, tmprect2.left, tmprect2.top, &src_x, &src_y, &root);

    XCopyArea(Application.Handle, wnd, hbmp, CScreen.GC, src_x, src_y,
              tmprect2.width, tmprect2.height,
              tmprect2.left - rect.left, tmprect2.top - rect.top);

    XQueryTree(Application.Handle, wnd, &root, &parent, &children, &nchildren);
    if(children != NULL)
    {
        for(register unsigned int i = 0; i < nchildren; i++)
        {
            wtmp = children[i];
            __copytobmprect(wtmp, hbmp, wmain, rect);
        }
        XFree(children);
    }
}
*/
//---------------------------------------------------------------------------
static void __fastcall __copytodcrect(HWND wnd, HDC hbmp, const TRect &rect)
{
//    Window root, wtmp;
//    Window parent;
//    Window *children;
//    unsigned int nchildren;
//    XWindowAttributes child_winattrib;
//    int dest_x, dest_y;
//    int src_x, src_y;
//    TRect tmprect1, tmprect2;
    /*
        XGetWindowAttributes(Application.Handle, wnd, &child_winattrib);
        if(child_winattrib.depth != CScreen.Depth || child_winattrib.map_state != IsViewable)
                    return;
        if(!XTranslateCoordinates(Application.Handle, wnd, wmain, child_winattrib.x, child_winattrib.y, &dest_x, &dest_y, &root))
                    return;
        tmprect1 = TRect(dest_x - child_winattrib.x, dest_y - child_winattrib.y, child_winattrib.width, child_winattrib.height);
        if(!const_cast<TRect &>(rect).intersect(tmprect1, tmprect2))
                    return;
        XTranslateCoordinates(Application.Handle, wmain, wnd, tmprect2.left, tmprect2.top, &src_x, &src_y, &root);
    */
    unsigned long valuemask = GCSubwindowMode;
    XGCValues values;

    XGetGCValues(Application.Handle, Screen.GC, valuemask, &values);
    int oldmode = values.subwindow_mode;
    XSetSubwindowMode(Application.Handle, Screen.GC, IncludeInferiors);

    XCopyArea(Application.Handle, wnd, hbmp, Screen.GC, rect.left, rect.top,
              rect.width, rect.height,
              0, 0);

    XSetSubwindowMode(Application.Handle, Screen.GC, oldmode);
    /*
        XQueryTree(Application.Handle, wnd, &root, &parent, &children, &nchildren);
        if(children != NULL)
        {
            for(register unsigned int i = 0; i < nchildren; i++)
            {
                wtmp = children[i];
                __copytobmprect(wtmp, hbmp, wmain, rect);
            }
            XFree(children);
        } */
}
//---------------------------------------------------------------------------
HBITMAP __fastcall WindowToBitmapRect(HWND wnd, const TRect &rect)
{
    HBITMAP ret;

    ret = XCreatePixmap(Application.Handle, wnd, rect.width, rect.height, Screen.Depth);
    __copytodcrect(wnd, ret, rect);
//    __copytodcrect(wnd, ret, wnd, rect);

    return ret;
}
//---------------------------------------------------------------------------
HMONITOR __fastcall MonitorFromWindow(HWND wnd)
{
    XWindowAttributes window_attributes;
    XGetWindowAttributes(Application.Handle, wnd, &window_attributes);
    return window_attributes.screen;
}
//---------------------------------------------------------------------------
void __fastcall WakeupWindow(HWND wnd)
{
    XClientMessageEvent cliEvent;
    memset(&cliEvent, 0, sizeof(XClientMessageEvent));
    cliEvent.type = ClientMessage;
    cliEvent.window = wnd;
    cliEvent.format = 32;
    XSendEvent(Application.Handle, wnd, 0, 0, (XEvent*)&cliEvent);
//   XFlush(Application.Handle);
}
//---------------------------------------------------------------------------
//  Clipboard utils
//---------------------------------------------------------------------------
HWND GetClipboardOwner()
{
    return XGetSelectionOwner(Application.Handle, XA_PRIMARY);
}
//---------------------------------------------------------------------------
void __fastcall SetClipboardOwner(HWND wnd)
{
    XSetSelectionOwner(Application.Handle, XA_PRIMARY, wnd, CurrentTime);
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
