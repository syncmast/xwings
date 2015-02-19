//---------------------------------------------------------------------------
#ifndef netwmH
#define netwmH

#include "types.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
//  Clipboard formats
//---------------------------------------------------------------------------
#define CF_TEXT     XA_STRING
#define CF_BITMAP   XA_PIXMAP
//---------------------------------------------------------------------------
enum TAtomIdent
{
    UTF8_STRING,
    CLIPBOARD,
    TARGETS,
    WM_DELETE_WINDOW,
    WM_TAKE_FOCUS,
    WM_SAVE_YOURSELF,
    WM_PROTOCOLS,
    _NET_SUPPORTED,
    _NET_CLIENT_LIST,
    _NET_CLIENT_LIST_STACKING,
    _NET_NUMBER_OF_DESKTOPS,
    _NET_DESKTOP_GEOMETRY,
    _NET_DESKTOP_VIEWPORT,
    _NET_CURRENT_DESKTOP,
    _NET_DESKTOP_NAMES,
    _NET_ACTIVE_WINDOW,
    _NET_WORKAREA,
    _NET_SUPPORTING_WM_CHECK,
    _NET_VIRTUAL_ROOTS,
    _NET_DESKTOP_LAYOUT,
    _NET_SHOWING_DESKTOP,
    _NET_CLOSE_WINDOW,
    _NET_MOVERESIZE_WINDOW,
    _NET_WM_MOVERESIZE,
    _NET_RESTACK_WINDOW,
    _NET_REQUEST_FRAME_EXTENTS,
    _NET_WM_NAME,
    _NET_WM_VISIBLE_NAME,
    _NET_WM_ICON_NAME,
    _NET_WM_VISIBLE_ICON_NAME,
    _NET_WM_DESKTOP,
    _NET_WM_WINDOW_TYPE,
    _NET_WM_STATE,
    _NET_WM_ALLOWED_ACTIONS,
    _NET_WM_STRUT,
    _NET_WM_STRUT_PARTIAL,
    _NET_WM_ICON_GEOMETRY,
    _NET_WM_ICON,
    _NET_WM_PID,
    _NET_WM_HANDLED_ICONS,
    _NET_WM_USER_TIME,
    _NET_WM_USER_TIME_WINDOW,
    _NET_FRAME_EXTENTS,
    _NET_WM_OPAQUE_REGION,
    _NET_WM_PING,
    _NET_WM_SYNC_REQUEST,
    _NET_WM_FULLSCREEN_MONITORS,
    _NET_WM_FULL_PLACEMENT,
    _NET_WM_CM_Sn
};

typedef TCommonList<Atom> TAtomList;

extern "C" {
    void InitNETWM();

    Atom __fastcall GetAtom(TAtomIdent idx);
    Atom __fastcall GetAtomXProperty(const char *name, bool create);
    void * __fastcall GetXProperty(HWND wnd, Atom prop, int *nitems = NULL);
    void __fastcall SetXProperty(HWND wnd, Atom prop, Atom type,
                                 int format, void *data, int nelements);
    void __fastcall AddXProperty(HWND wnd, Atom prop, Atom type,
                                 int format, void *data, int nelements);
    void __fastcall DelXProperty(HWND wnd, Atom prop);

    int  __fastcall GetNETWMSupport(TAtomList *al, TStrings *names);
    void __fastcall SetNETWMSupport(TAtomList &al);

//  Root Window
    HWND GetActiveWindow();				            //  _NET_ACTIVE_WINDOW
    void __fastcall SetActiveWindow(HWND wnd);
    int  GetCurrentDesktop();			//  _NET_CURRENT_DESKTOP
    void __fastcall SetCurrentDesktop(int n);
    int  GetDesktopCount();				//  _NET_NUMBER_OF_DESKTOPS
    void __fastcall SetDesktopCount(int n);
    void __fastcall GetDesktopNames(TStrings &out);	//  _NET_DESKTOP_NAMES
    void __fastcall QueryFrameExtents(HWND wnd);    //  _NET_REQUEST_FRAME_EXTENTS
    TString GetWMName();					//  _NET_SUPPORTING_WM_CHECK
    TRect __fastcall GetWorkArea(int desktop);		//  _NET_WORKAREA

//  Window
    void __fastcall CloseWindow(HWND wnd);  		//  _NET_CLOSE_WINDOW
    void __fastcall GetWindowRect(HWND wnd, TRect &rect);
    void __fastcall GetClientRect(HWND wnd, TRect &rect);
    void __fastcall InvalidateRect(HWND wnd, const TRect &rect);
    void __fastcall MoveWindow(HWND wnd, int x, int y, int w, int h);
    void __fastcall UpdateWindow(HWND wnd);
    void __fastcall UpdateWindowRect(HWND wnd, const TRect &rect);

//	Application Window
    void __fastcall GetAppClass(HWND wnd, TString &appname, TString &appclass);		  // WM_CLASS
    void __fastcall SetAppClass(HWND wnd, const TString &appname, const TString &appclass);
    pid_t __fastcall GetWMpid(HWND wnd);				//  _NET_WM_PID
    void __fastcall SetWMpid(HWND wnd);
    void __fastcall SetWMProtocols(HWND wnd);
    TString __fastcall GetClientMachine(HWND wnd);	//  WM_CLIENT_MACHINE
    void __fastcall SetClientMachine(HWND wnd);

    int  __fastcall GetDesktop(HWND wnd);			//  _NET_WM_DESKTOP
    void __fastcall SetDesktop(HWND wnd, int val);
    void __fastcall GetFrameExtents(HWND wnd, long *exts);      //  _NET_FRAME_EXTENTS
    void __fastcall SetFrameExtents(HWND wnd, long left, long top, long right, long bottom);

//	Window Manager Protocols
    void __fastcall SendPing(HWND wnd, XEvent *back);	// _NET_WM_PING

//	Other Properties
    void __fastcall WakeupWindow(HWND wnd);
    HBITMAP __fastcall WindowToBitmap(HWND wnd);
    HBITMAP __fastcall WindowToBitmapRect(HWND wnd, const TRect &rect);
    HMONITOR __fastcall MonitorFromWindow(HWND wnd);

//  Clipboard utils
    HWND GetClipboardOwner();
    void __fastcall SetClipboardOwner(HWND wnd);
};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
/*
_NET_SUPPORTED
_NET_CLIENT_LIST
_NET_DESKTOP_GEOMETRY
_NET_DESKTOP_VIEWPORT
_NET_VIRTUAL_ROOTS
_NET_DESKTOP_LAYOUT
_NET_SHOWING_DESKTOP
*
Other Root Window Messages
*
_NET_MOVERESIZE_WINDOW
_NET_WM_MOVERESIZE
_NET_RESTACK_WINDOW
*
Application Window Properties
*
_NET_WM_NAME
_NET_WM_VISIBLE_NAME
_NET_WM_ICON_NAME
_NET_WM_VISIBLE_ICON_NAME
_NET_WM_WINDOW_TYPE
_NET_WM_STATE
_NET_WM_ALLOWED_ACTIONS
_NET_WM_STRUT
_NET_WM_STRUT_PARTIAL
_NET_WM_ICON_GEOMETRY
_NET_WM_ICON
_NET_WM_HANDLED_ICONS
_NET_WM_USER_TIME
_NET_WM_USER_TIME_WINDOW
*
Window Manager Protocols
*
_NET_WM_SYNC_REQUEST
_NET_WM_FULLSCREEN_MONITORS
*
Other Properties
*
_NET_WM_FULL_PLACEMENT
*
Compositing Managers
*
_NET_WM_CM_Sn Manager Selection
WM_TRANSIENT_FOR for override-redirect windows

*/
