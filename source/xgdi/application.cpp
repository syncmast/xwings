//---------------------------------------------------------------------------
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/poll.h>
#include <X11/cursorfont.h>
#include "application.h"
#include "menu.h"
#include "netwm.h"
#include "rtti.h"
#include "vxutil.h"
//---------------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
extern HDISPLAY XDpy;
extern HWND RootWnd;
//---------------------------------------------------------------------------
TApplication Application;
TScreen Screen;
TKeyboard Keyboard;
TMouse Mouse;
TCaret Caret;
//---------------------------------------------------------------------------
void __fastcall SendMessage(TWinControl *dest, TMessage &msg)
{
    Application.SendMessage(dest ? dest->Handle : 0, msg);
}
//---------------------------------------------------------------------------
int __fastcall GetListExtensions(TStrings &out)
{
    char **exts;
    int num_exts;
    exts = XListExtensions(Application.Handle, &num_exts);
    out.Add(exts, num_exts);
    XFreeExtensionList(exts);
    return num_exts;
}
//---------------------------------------------------------------------------
int __fastcall GetFontPath(TStrings &out)
{
    char **paths;
    int num_paths;
    paths = XGetFontPath(Application.Handle, &num_paths);
    out.Add(paths, num_paths);
    XFreeFontPath(paths);
    return num_paths;
}
//---------------------------------------------------------------------------
void __fastcall SetFontPath(TStrings *dirs)
{
    char **paths = NULL;
    int ndirs = 0;
    if(dirs)
    {
        ndirs = dirs->Count;
        paths = new char*[ndirs];
        for(register int i = 0; i < ndirs; i++)
            paths[i] = (char*)(*dirs->Strings[i]).c_str();
    }
    XSetFontPath(Application.Handle, paths, ndirs);
    delete [] paths;
}
//---------------------------------------------------------------------------
int __fastcall SelectFontName(const char *tmpl, int maxnum, TStrings &out)
{
    char **fonts;
    int num_fonts;
    fonts = XListFonts(Application.Handle, tmpl, maxnum, &num_fonts);
    out.Add(fonts, num_fonts);
    XFreeFontNames(fonts);
    return num_fonts;
}
//---------------------------------------------------------------------------
int __fastcall ExtractFontName(const char *name, int partno, char* buf)
{
    if(name[0] != '-')
        return 0;

    char *str = strdup(name);
    char *cp;
    char *cd[XLFD_NFIELDS];
    int j = 0, len;
    for(cp = strtok(str, "-");
            cp != NULL;
            cp = strtok(NULL, "-"))
    {
        cd[j++] = cp;
    }
    len = strlen(cd[partno]);
    if(buf)
        strcpy(buf, cd[partno]);

    free(str);
    return len;
}
//---------------------------------------------------------------------------
bool __fastcall UseXExtension(const char *xextname)
{
    bool ret = false;
    TXExtension *ext;

    ext = dynamic_cast<TXExtension *>((TObject *)TClass::Create(xextname));
    if(ext)
        ret = true;

    return ret;
}
//---------------------------------------------------------------------------
void __fastcall RegisterPainter(TScreen *scr, const TPainter *painter)
{
    if(scr == NULL)
        Screen.m_Painter = const_cast<TPainter *>(painter);
    else
        scr->m_Painter = const_cast<TPainter *>(painter);
}
//---------------------------------------------------------------------------
//  TClipboard
//---------------------------------------------------------------------------
DYNAMIC_CLASS( TClipboard );
//---------------------------------------------------------------------------
TClipboard :: TClipboard()
{
    Init();
}
//---------------------------------------------------------------------------
TClipboard :: ~TClipboard()
{
}
//---------------------------------------------------------------------------
__fastcall TClipboard :: TClipboard(const TClipboard &a) : TObject(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TClipboard & TClipboard :: operator=(const TClipboard &a)
{
    if(this == &a)
        return *this;
    TObject :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TClipboard :: Init()
{
}
//---------------------------------------------------------------------------
__fastcall void TClipboard :: Assign(const TClipboard &obj)
{
}

//---------------------------------------------------------------------------
// TXExtension
//---------------------------------------------------------------------------
__fastcall TXExtension :: TXExtension(const char *ename)
{
    int major_opcode_return;
    m_Supported = XQueryExtension(XDpy, ename, &major_opcode_return, &m_BaseEvent, &m_BaseError);
    if(m_Supported)
    {
        Application.Extensions.Add(this);
    }
    m_Name = ename;
}
//---------------------------------------------------------------------------
TXExtension :: ~TXExtension()
{
    Application.Extensions.Delete(this);
}
//---------------------------------------------------------------------------
// TKeyboard
//---------------------------------------------------------------------------
TKeyboard :: TKeyboard()
{

}
//---------------------------------------------------------------------------
TKeyboard :: ~TKeyboard()
{
}
//---------------------------------------------------------------------------
int TKeyboard :: get_LedState()
{
    XKeyboardState values;
    XGetKeyboardControl(Application.Handle, &values);
    return values.led_mask;
}
//---------------------------------------------------------------------------
bool TKeyboard :: get_AutoRepeat()
{
    XKeyboardState values;
    XGetKeyboardControl(Application.Handle, &values);
    return (bool)values.global_auto_repeat;
}
//---------------------------------------------------------------------------
void TKeyboard :: set_AutoRepeat(bool value)
{
    if(get_AutoRepeat() != value)
    {
        if(value)
            XAutoRepeatOn(Application.Handle);
        else
            XAutoRepeatOff(Application.Handle);
    }
}
//---------------------------------------------------------------------------
// TMouse
//---------------------------------------------------------------------------
TMouse :: TMouse()
{
    DragThreshold = 5;
    RepeatThreshold = 1000;
    DblClickThreshold = 500;
}
//---------------------------------------------------------------------------
TMouse :: ~TMouse()
{
}
//---------------------------------------------------------------------------
TMouseAccelerator TMouse :: get_Accelerator()
{
    TMouseAccelerator ret;

    XGetPointerControl(Application.Handle, &ret.Numerator, &ret.Denominator,
                       &ret.Threshold);
    return ret;
}
//---------------------------------------------------------------------------
void TMouse :: set_Accelerator(TMouseAccelerator value)
{
    XChangePointerControl(Application.Handle, true, true, value.Numerator,
                          value.Denominator, value.Threshold);
}
//---------------------------------------------------------------------------
TPoint TMouse :: get_CursorPos()
{
    TPoint tmp;
    Window wdummy1, wdummy2;
    int win_x, win_y, x, y;
    unsigned int mask;
    /*
    // Размораживаем очередь ??????
    XEvent event;
    event.xmotion.type = MotionNotify;
    for( register int i = 0; i < 100; i++)
        XSendEvent(Application.Handle, PointerWindow, false, PointerMotionMask, &event);
    */

    XQueryPointer(Application.Handle, Screen.Root, &wdummy1, &wdummy2, &x, &y,
                  &win_x, &win_y, &mask);
    tmp.x = x;
    tmp.y = y;
    return tmp;
}
//---------------------------------------------------------------------------
void TMouse :: set_CursorPos(TPoint value)
{
    XWarpPointer(Application.Handle, None, Screen.Root, 0, 0, 0, 0, value.x, value.y);
}
//---------------------------------------------------------------------------
TPoint TMouse :: get_CursorSize()
{
    TPoint tmp;
    uint_t w, h;

    XQueryBestCursor(Application.Handle, Screen.Root, 100, 100, &w, &h);
    tmp.x = w;
    tmp.y = h;
    return tmp;
}
//---------------------------------------------------------------------------
//  TCaret
//---------------------------------------------------------------------------
TCaret :: TCaret()
{
    Init();
}
//---------------------------------------------------------------------------
TCaret :: ~TCaret()
{
}
//---------------------------------------------------------------------------
__fastcall TCaret :: TCaret(const TCaret &a) : TObject(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TCaret & TCaret :: operator=(const TCaret &a)
{
    if(this == &a)
        return *this;
    TObject :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TCaret :: Init()
{
    m_timer.OnTimer = TNotifyEvent(this, &TCaret :: DoBlink);
    m_timer.Enabled = false;
    m_timer.Interval = 500;
    m_show = false;
    Pos.x = 0;
    Pos.y = 0;
    Color = clBlack;
    Kind = ctUnderline;
    m_canvas = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TCaret :: Assign(const TCaret &obj)
{
}
//---------------------------------------------------------------------------
void TCaret :: DoBlink(TObject *sender)
{
    TColor oldcolor = m_canvas->Pen->Color;
    TPenMode oldmode = m_canvas->Pen->Mode;
    int oldwidth = m_canvas->Pen->Width;
    TRect rect = m_canvas->TextExtents("H");
    int w, h;
    w = rect.width;
    h = rect.height;
    switch(Kind)
    {
    case ctUnderline:
        h = 1;
        break;
    case ctVerticalLine:
        w = 1;
        break;
    case ctSolidBlock:
        break;
    }
    m_canvas->Pen->Color = Color;
    m_canvas->Pen->Mode = pmNotMerge;
    m_canvas->Pen->Width = w;
    m_canvas->Line(Pos.x + w / 2, Pos.y, Pos.x + w / 2, Pos.y - h);
    XFlush(XDpy);
    m_canvas->Pen->Color = oldcolor;
    m_canvas->Pen->Mode = oldmode;
    m_canvas->Pen->Width = oldwidth;
    m_show = !m_show;
    /*
       pmNotXor = GXequiv,
       pmNotDst = GXinvert,
       pmOrNot = GXorReverse,

    */
}
//---------------------------------------------------------------------------
void TCaret :: set_BlinkTime(uint_t value)
{
    if(m_timer.Interval != value)
        m_timer.Interval = value;
}
//---------------------------------------------------------------------------
void __fastcall TCaret :: Show(TWinControl *win)
{
//    if(m_canvas && m_show)
//        Hide();
    m_canvas = &win->Canvas;
    m_timer.Enabled = true;
    DoBlink(NULL);
}
//---------------------------------------------------------------------------
void TCaret :: Hide()
{
    m_timer.Enabled = false;
    if(m_show)
        DoBlink(NULL);
}
//---------------------------------------------------------------------------
// TMonitor
//---------------------------------------------------------------------------
__fastcall TMonitor :: TMonitor(int screen_number)
    : m_Canvas(RootWindow(XDpy, screen_number))
{
    m_Handle = ScreenOfDisplay(XDpy, screen_number);
}
//---------------------------------------------------------------------------
short TMonitor :: get_MonitorNum()
{
    return  XScreenNumberOfScreen(m_Handle);
}
//---------------------------------------------------------------------------
bool TMonitor :: get_Primary()
{
    return  XScreenNumberOfScreen(m_Handle) == DefaultScreen(XDpy);
}
//---------------------------------------------------------------------------
TPixelSize TMonitor :: get_Depth()
{
    return  (TPixelSize)DefaultDepthOfScreen(m_Handle);
}
//---------------------------------------------------------------------------
HGC TMonitor :: get_GC()
{
    return  DefaultGCOfScreen(m_Handle);
}
//---------------------------------------------------------------------------
short TMonitor :: get_Height()
{
    return  HeightOfScreen(m_Handle);
}
//---------------------------------------------------------------------------
short TMonitor :: get_HeightMM()
{
    return  HeightMMOfScreen(m_Handle);
}
//---------------------------------------------------------------------------
short TMonitor :: get_Planes()
{
    return  PlanesOfScreen(m_Handle);
}
//---------------------------------------------------------------------------
HWND TMonitor :: get_Root()
{
    return  RootWindowOfScreen(m_Handle);
}
//---------------------------------------------------------------------------
short TMonitor :: get_Xdpi()
{
    return  (254 * Width)/(10 * WidthMM);
}
//---------------------------------------------------------------------------
short TMonitor :: get_Ydpi()
{
    return  (254 * Height)/(10 * HeightMM);
}
//---------------------------------------------------------------------------
short TMonitor :: get_Width()
{
    return  WidthOfScreen(m_Handle);
}
//---------------------------------------------------------------------------
short TMonitor :: get_WidthMM()
{
    return  WidthMMOfScreen(m_Handle);
}
//---------------------------------------------------------------------------
TRect TMonitor :: get_WorkArea()
{
    int desktop = GetCurrentDesktop();
    return  GetWorkArea(desktop);
}
//---------------------------------------------------------------------------
// TScreen
//---------------------------------------------------------------------------
TScreen :: TScreen()
    : m_Canvas(DefaultRootWindow(XDpy))
{
    Init();
}
//---------------------------------------------------------------------------
TScreen :: ~TScreen()
{
    for(TCursorList::iterator from = m_Cursors.begin();
            from != m_Cursors.end();
            ++from)
    {
        if(*from != None)
            XFreeCursor(Application.Handle, *from);
    }
    m_Cursors.Clear();

    TList of;

    for(TList::iterator from = m_Forms.begin();
            from != m_Forms.end();
            ++from)
    {
        printf("@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
        printf("ScreenFormsCount %d\n", *m_Forms.Count);
//		((TCustomForm *)(*from))->Close();
        of.Add(*from);
//		delete (TCustomForm *)(*from);
        printf("after ScreenFormsCount %d\n", *m_Forms.Count);
        printf("@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    }

    for(TList::iterator from = of.begin();
            from != of.end();
            ++from)
    {
        ((TCustomForm *)(*from))->Close();
//		pm = (TCustomForm *)(*from);
//		pm->Close();
    }
//   	Forms.Clear();

    TMonitor *mon;
    for(TList::iterator from = m_Monitors.begin();
            from != m_Monitors.end();
            ++from)
    {
        mon = (TMonitor *)(*from);
        delete mon;
    }
    m_Monitors.Clear();

}
//---------------------------------------------------------------------------
void TScreen :: Init()
{
    m_Handle = DefaultScreenOfDisplay(XDpy);
    m_PopupActive = false;
    m_Painter = NULL;
    CreateCursors();
    for(register short i = 0; i < ScreenCount(XDpy); i++)
    {
        m_Monitors.Add(new TMonitor(i));
    }
}
//---------------------------------------------------------------------------
TWinControl * TScreen ::  get_ActiveControl()
{
    TCustomForm *frm;
    TWinControl *win;
    int revert;
    HWND h;

    XGetInputFocus(XDpy, &h, &revert);
    for(TList::iterator from = m_Forms.begin();
            from != m_Forms.end();
            ++from)
    {
        frm = (TCustomForm *)(*from);
        if(frm->Handle == h)
            return frm;
        win = frm->FindWinControl(h);
        if(win)
            return win;
    }
    return NULL;
}
//---------------------------------------------------------------------------
TCustomForm * TScreen ::  get_ActiveForm()
{
    TCustomForm *frm;
    int revert;
    HWND h;

    XGetInputFocus(XDpy, &h, &revert);
    for(TList::iterator from = m_Forms.begin();
            from != m_Forms.end();
            ++from)
    {
        frm = (TCustomForm *)(*from);
        if(frm->Handle == h)
            return frm;
        if(frm->FindWinControl(h))
            return frm;
    }
    return NULL;
}
//---------------------------------------------------------------------------
void TScreen :: set_Cursor(TCursor value)
{
    if(m_Cursor != value)
    {
        m_Cursor = value;
        HCURSOR c = m_Cursors[(int)value];
        XDefineCursor(Application.Handle, RootWindowOfScreen(m_Handle), c);
    }
}
//---------------------------------------------------------------------------
TPixelSize TScreen :: get_Depth()
{
    return  (TPixelSize)DefaultDepthOfScreen(m_Handle);
}
//---------------------------------------------------------------------------
HGC TScreen :: get_GC()
{
    return  DefaultGCOfScreen(m_Handle);
}
//---------------------------------------------------------------------------
short TScreen :: get_Height()
{
    return  HeightOfScreen(m_Handle);
}
//---------------------------------------------------------------------------
short TScreen :: get_HeightMM()
{
    return  HeightMMOfScreen(m_Handle);
}
//---------------------------------------------------------------------------
short TScreen :: get_MonitorCount()
{
    return  ScreenCount(XDpy);
}
//---------------------------------------------------------------------------
short TScreen :: get_Planes()
{
    return  PlanesOfScreen(m_Handle);
}
//---------------------------------------------------------------------------
HWND TScreen :: get_Root()
{
    return  RootWindowOfScreen(m_Handle);
}
//---------------------------------------------------------------------------
short TScreen :: get_Xdpi()
{
    return  (254 * Width)/(10 * WidthMM);
}
//---------------------------------------------------------------------------
short TScreen :: get_Ydpi()
{
    return  (254 * Height)/(10 * HeightMM);
}
//---------------------------------------------------------------------------
short TScreen :: get_Width()
{
    return  WidthOfScreen(m_Handle);
}
//---------------------------------------------------------------------------
short TScreen :: get_WidthMM()
{
    return  WidthMMOfScreen(m_Handle);
}
//---------------------------------------------------------------------------
TRect TScreen :: get_WorkArea()
{
    int desktop = GetCurrentDesktop();
    return  GetWorkArea(desktop);
}
//---------------------------------------------------------------------------
void TScreen :: CreateCursors()
{
    /*	short xc[23];
    	xc[crDefault] = 0;
    	xc[crNone] = 1;
    	xc[crArrow] = XC_left_ptr;
    	xc[crCross] = XC_crosshair;
    	xc[crIBeam] = XC_xterm;
    	xc[crSize]  = XC_fleur;
    	xc[crSizeNESW] = 4;
    	xc[crSizeNS]   = XC_sb_v_double_arrow;
    	xc[crSizeNWSE] = 6;
    	xc[crSizeWE]   = XC_sb_h_double_arrow;
    	xc[crUpArrow]  = XC_sb_up_arrow;
    	xc[crHourGlass]= XC_watch;
    	xc[crDrag]     = 10;
    	xc[crNoDrop]   = 12;
    	xc[crHSplit]   = XC_sb_h_double_arrow;
    	xc[crVSplit]   = XC_double_arrow;
    	xc[crMultiDrag]= 16;
    	xc[crSQLWait]  = 18;
    	xc[crNo]       = 20;
    	xc[crAppStart] = XC_rtl_logo;
    	xc[crHelp]     = XC_question_arrow;
    	xc[crHandPoint]= XC_hand2;
    	xc[crSizeAll]  = XC_fleur; */

    m_Cursors[crHourGlass] = CreateCursor(XC_watch);
    m_Cursors[crIBeam] = CreateCursor(XC_xterm);
}
//---------------------------------------------------------------------------
void __fastcall TScreen :: AddForm(TCustomForm *form)
{
    m_Forms.Add(form);
    if(dynamic_cast<TCustomPopupMenu *>(form))
        m_PopupActive = true;
}
//---------------------------------------------------------------------------
void __fastcall TScreen :: RemoveForm(TCustomForm *form)
{
    m_Forms.Delete(form);
}
//---------------------------------------------------------------------------
void TScreen :: ClosePopup()
{
    TList menus;
    TCustomForm *tf;
    TCustomPopupMenu *pm;

    for(TList::iterator from = m_Forms.begin();
            from != m_Forms.end();
            ++from)
    {
        tf = (TCustomForm *)(*from);
        pm = dynamic_cast<TCustomPopupMenu *>(tf);
        if(pm)
            menus.Add(pm);
    }

    for(TList::iterator from = menus.begin();
            from != menus.end();
            ++from)
    {
        pm = (TCustomPopupMenu *)(*from);
        pm->Close();
    }
    m_PopupActive = false;
}
//---------------------------------------------------------------------------
// TApplication
//---------------------------------------------------------------------------
static int __errorHandler(Display *dpy, XErrorEvent *rep)
{
    char buffer[BUFSIZ];
    char mesg[BUFSIZ];
    char number[32];
    const char *mtype = "XlibMessage";

    XGetErrorText(dpy, rep->error_code, buffer, BUFSIZ);
    XGetErrorDatabaseText(dpy, mtype, "XError", "X Error", mesg, BUFSIZ);
    fprintf(stderr, "%s: %s\n", mesg, buffer);

    XGetErrorDatabaseText(dpy, mtype, "MajorCode", "Request Major code %d",
                          mesg, BUFSIZ);
    fprintf(stderr, mesg, rep->request_code);
    sprintf(number, "%d", rep->request_code);
    XGetErrorDatabaseText(dpy, "XRequest", number, "",  buffer, BUFSIZ);
    fprintf(stderr, " %s\n", buffer);

    XGetErrorDatabaseText(dpy, mtype, "MinorCode", "Request Minor code",
                          mesg, BUFSIZ);
    fprintf(stderr, mesg, rep->minor_code);
    fputs("\n", stderr);

    XGetErrorDatabaseText(dpy, mtype, "ResourceID", "ResourceID 0x%x",
                          mesg, BUFSIZ);
    fprintf(stderr, mesg, rep->resourceid);
    fputs("\n", stderr);

    XGetErrorDatabaseText(dpy, mtype, "ErrorSerial", "Error Serial #%d",
                          mesg, BUFSIZ);
    fprintf(stderr, mesg, rep->serial);
    fputs("\n", stderr);

    XGetErrorDatabaseText(dpy, mtype, "CurrentSerial", "Current Serial #%d",
                          mesg, BUFSIZ);
    fprintf(stderr, mesg, NextRequest(dpy)-1);
    fputs("\n", stderr);

    exit(1);
//    return 1;
}

static int __errorIOHandler(Display *dpy)
{
    fputs("Fatal error occured\n", stderr);
    return 0;
}

static int __signals[] = { SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGABRT, SIGFPE, SIGSEGV, SIGPIPE,
                           SIGALRM, SIGTERM, SIGUSR1, SIGUSR2, SIGCHLD, SIGCONT, SIGTSTP,
                           SIGTTOU, SIGTTIN, SIGPOLL, SIGPROF, SIGSYS, SIGTRAP, SIGURG,
                           SIGVTALRM, SIGXCPU, SIGXFSZ, SIGSTKFLT, SIGIO,
                           SIGPWR, SIGWINCH, SIGUNUSED, SIGBUS
                         };

static void __sig_handler(int signum, siginfo_t *siginfo, void *context)
{
    if(Application.OnSignal)
        Application.OnSignal(NULL, siginfo, context);
    return;
}

static void __set_global_sig_handler()
{
    struct sigaction act, old;
    sigset_t mask;
    sigfillset(&mask);
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = __sig_handler;
    act.sa_mask = mask;
    act.sa_flags = SA_SIGINFO | SA_RESTART;
    for(register uint_t i = 0; i < sizeof(__signals); i++)
    {
        sigaction(__signals[i], NULL, &old);
        if(old.sa_handler == SIG_DFL || old.sa_handler == SIG_IGN)
            sigaction(__signals[i], &act, NULL);
    }
//	for(register int i = SIGRTMIN; i <= SIGRTMAX; i++)
//	{
//		sigaction(i, &act, NULL);
//	}
}
//---------------------------------------------------------------------------
TApplication :: TApplication()
{
    m_hintwindow = NULL;
    IdleTimeout = 1000;
    XInitThreads();
    XSetErrorHandler(__errorHandler);
    XSetIOErrorHandler(__errorIOHandler);
    setlocale(LC_ALL,"");
    if (!XSupportsLocale())
    {
        setlocale(LC_ALL, "C");
    }
    XSetLocaleModifiers("@im=local");

    m_Terminated = false;
    ShowHint = true;
    ShowMainForm = true;
    m_Active = false;

    char *display_name;

    if(XConnection == NULL)
        display_name = NULL;

    m_Handle = XOpenDisplay(display_name);
    if(m_Handle == NULL)
    {
        pid_t pid;

        pid = fork();
        switch(pid)
        {
        case 0: /* мы в дочернем процессе */
            break;
        case -1: /* ошибка fork() - случилось что-то страшное */
//					fprintf(stderr,"Error: initial fork failed: %s\n",
//							strerror(errno));
            exit(1);
            break;
        default: /* мы в родительском процессе */
            execlp("X", "", NULL);
            exit(0);
            break;
        }
        for( register int i = 0; i < 10; i++)
        {
            sleep(1);
            m_Handle = XOpenDisplay(":0");
            if(m_Handle != NULL)
                break;
        }
        if(m_Handle == NULL)
        {
            fputs("X server not connected\n", stderr);
            exit(1);
        }
        HCURSOR c;
        c = XCreateFontCursor(m_Handle, XC_left_ptr);
        XDefineCursor(m_Handle, DefaultRootWindow(m_Handle), c);
    }
    XDpy = m_Handle;

//   	XSynchronize(XDpy, true);   // debug enable

    HintWindowClass = __classid(THintWindow);
    HintColor = clInfoBk;
    HintShowTime = 3000;
    HintPauseTime = 800;

    SetSigChildHandler();

    InitNETWM();
}
//---------------------------------------------------------------------------
TApplication :: ~TApplication()
{
    DoDeactivate();
    /*
       	for(TList::iterator from = Forms.begin();
           Forms.size();
           ++from)
    	{
    		printf("@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    		printf("FormsCount %d\n", *Forms.Count);
    		delete (TCustomForm *)(*from);
    		printf("after FormsCount %d\n", *Forms.Count);
    		printf("@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
       	}
    */
    TList of;

    for(TList::iterator from = Forms.begin();
            from != Forms.end();
            ++from)
    {
        of.Add(*from);
    }

    for(TList::iterator from = of.begin();
            from != of.end();
            ++from)
    {
        ((TCustomForm *)(*from))->Close();
    }

    /*
       TScreen *scr;

       for(TList::iterator from = Screens.begin();
           from != Screens.end();
           ++from)
    	{
    		scr = (TScreen *)(*from);
    		if(scr != &CScreen)
    			delete scr;
       }
       Screens.Clear();
    */
    XCloseDisplay(m_Handle);
}
//---------------------------------------------------------------------------
void TApplication :: Run()
{
    if(OnSignal)
        __set_global_sig_handler();

    if(ShowMainForm && m_MainForm)
    {
        m_MainForm->Visible = true;
    }

    while ( !m_Terminated )
    {
        HandleMessage();
    }
}
//---------------------------------------------------------------------------
void TApplication :: Terminate()
{
    m_Terminated = true;
}
//---------------------------------------------------------------------------
void TApplication :: DoDeactivate()
{
    if(m_Active)
    {
        Window focus;
        int revert_to;
        TCustomForm *tmpform;
        TWinControl *tmpctrl;
        XGetInputFocus(m_Handle, &focus, &revert_to);
        for(TList::iterator from = Forms.begin();
                from != Forms.end();
                ++from)
        {
            tmpform = (TCustomForm *)(*from);
            if(tmpform->Handle == focus)
                return;
            tmpctrl = tmpform->FindWinControl(focus);
            if(tmpctrl)
                return;
        }
        m_Active = false;

        if(Screen.PopupActive)
        {
            Screen.ClosePopup();
        }

        if(OnDeactivate)
            OnDeactivate(this);
    }
}
//---------------------------------------------------------------------------
void TApplication :: DoActivate()
{
    if(!m_Active)
    {
        m_Active = true;
        if(OnActivate)
            OnActivate(this);
    }
}
//---------------------------------------------------------------------------
bool __fastcall TApplication :: WndProc(XEvent &event)
{
    Atom m_atom;
    bool ret = false;

    switch (event.type)
    {
    case ClientMessage:
        m_atom = XInternAtom(XDpy, "WM_TAKE_FOCUS", true);
        if(m_atom == (Atom)event.xclient.data.l[0])
        {
            printf("APP WM_TAKE_FOCUS %d\n", id);
            DoActivate();
        }
        else
        {
            m_atom = XInternAtom(XDpy, "_NET_WM_PING", true);
            if(m_atom == (Atom)event.xclient.data.l[0])
            {
                printf("APP _NET_WM_PING %d\n", id);
            }
        }
        printf("****CLIENTMESSAGE %d %d\n", id, (int)event.xclient.data.l[0]);
        break;
    case FocusIn:
        DoActivate();
        break;
    case FocusOut:
        DoDeactivate();
        break;
    }
    return ret;
}
//---------------------------------------------------------------------------
void __fastcall TApplication :: DispatchEvent(XEvent &event)
{
    TCustomForm *tmpform;
    TWinControl *tmpctrl;
    HWND wnd = event.xany.window;

    if(OnMessage)
    {
        bool handled = false;
        OnMessage(event, handled);
        if(handled)
            return;
    }
    WndProc(event);
    for(TList::reverse_iterator from = Forms.rbegin();
            from != Forms.rend();
            ++from)
    {
        tmpform = (TCustomForm *)(*from);
        if(wnd == 0)
            tmpform->Broadcast(event);
        else
        {
            if(tmpform->Handle == wnd)
            {
                tmpform->WndProc(event);
                break;
            }
            tmpctrl = tmpform->FindWinControl(wnd);
            if(tmpctrl)
            {
                tmpctrl->WndProc(event);
                break;
            }
        }
    }

//    printf("DISPATCH %d %lu\n", event.type, event.xany.serial);
}
//---------------------------------------------------------------------------
void TApplication :: HandleMessage()
{
    XEvent event;
    struct pollfd pfd;

    pfd.events = POLLIN;
    pfd.fd = ConnectionNumber(m_Handle);

    if(!XPending(m_Handle))
    {
        if(!poll(&pfd, 1, IdleTimeout))
        {
            if(OnIdle)
            {
                bool done = true;
                OnIdle(this, done);
                if(!done)
                    return;
            }
        }
    }
    XNextEvent ( m_Handle, &event );
    DispatchEvent ( event );
}
//---------------------------------------------------------------------------
void TApplication :: ProcessMessages()
{
    XEvent event;
    XSync(m_Handle, false);
    int num = QLength(m_Handle);
    for(register int i = 0; i < num; i++)
    {
        XNextEvent ( m_Handle, &event );
        DispatchEvent ( event );
    }
}
//---------------------------------------------------------------------------
int TApplication :: get_fd()
{
    return ConnectionNumber(m_Handle);
}
//---------------------------------------------------------------------------
void TApplication :: set_Title(TString text)
{
    XTextProperty txtprop;
    char *list;
    m_title = text;
    if(m_MainForm)
    {
        list = (char *)m_title.c_str();
        Xutf8TextListToTextProperty(m_Handle, &list, 1, XCompoundTextStyle, &txtprop);
        XSetWMIconName(m_Handle, m_MainForm->Handle, &txtprop);
        XFree(txtprop.value);
    }
}
//---------------------------------------------------------------------------
TString TApplication :: get_Title()
{
    XTextProperty txtprop;
    char **list;
    int num;
    if(m_MainForm)
    {
        XGetWMIconName(m_Handle, m_MainForm->Handle, &txtprop);
        Xutf8TextPropertyToTextList(m_Handle, &txtprop, &list, &num);
        XFree(txtprop.value);
        m_title = *list;
        XFreeStringList(list);
    }
    return m_title;
}
//---------------------------------------------------------------------------
void TApplication :: set_Hint(TString value)
{
    if(m_Hint != value)
    {
        m_Hint = value;
        if(OnHint)
            OnHint(this);
    }
}
//---------------------------------------------------------------------------
void TApplication :: set_Icon(TBitmap *value)
{
    if(value != m_Icon)
    {
        if(m_MainForm)
            m_MainForm->Icon = value;
        m_Icon = value;
    }
}
//---------------------------------------------------------------------------
TString TApplication :: get_ExeName()
{
    return TString(program_invocation_name);
}
//---------------------------------------------------------------------------
const char * TApplication :: get_Charset()
{
    const char *str = get_Locale(LC_CTYPE);
    const char *tmp;
    tmp = strchr(str, '.');
    if(tmp == NULL)
        tmp = str;
    else
        tmp++;
    return tmp;
}
//---------------------------------------------------------------------------
void __fastcall TApplication :: SendMessage(HWND wnd, TMessage &msg)
{
    XEvent event;
    long event_mask = SubstructureNotifyMask | SubstructureRedirectMask;

    event.xclient.type = ClientMessage;
    event.xclient.window = wnd;
    event.xclient.message_type = msg.Msg;
    event.xclient.format = 32;
    for(register int i = 0; i < 5; i++)
        event.xclient.data.l[i] = msg.Params[i];
    if(msg.Msg > CM_USER_MESSAGE)
        DispatchEvent(event);
    else
    {
        if(wnd == 0)
            wnd = Screen.Root;
        XSendEvent(m_Handle, wnd, false, event_mask, &event);
    }
}
//---------------------------------------------------------------------------
void __fastcall TApplication :: ActivateHint(const TControl *ctrl)
{
    if(!ShowHint)
        return;
    delete m_hintwindow;
    m_hintwindow = (THintWindow *)TClass(HintWindowClass).Create();
    m_hintwindow->Color = HintColor;
    m_hintwindow->ShowTime = HintShowTime;
    m_hintwindow->PauseTime = HintPauseTime;
    m_hintwindow->Activate(ctrl);
    Hint = ctrl->Hint;
}
//---------------------------------------------------------------------------
void TApplication :: HideHint()
{
    delete m_hintwindow;
    m_hintwindow = NULL;
}
//---------------------------------------------------------------------------
void TApplication :: BringToFront()
{
    if(m_MainForm)
        m_MainForm->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TApplication :: CreateForm(TMetaClass* InstanceClass, void *Reference)
{
    void *instance, **ftmp;
    instance = TClass(InstanceClass).Create();
    ftmp = (void **)Reference;

    *ftmp = instance;
    if(m_MainForm == NULL && instance)
    {
        TForm *tmpform = dynamic_cast<TForm *>((TObject *)instance);
        if(tmpform)
        {
            m_MainForm = tmpform;
            m_MainForm->Icon = m_Icon;
            if(!m_title.IsEmpty())
                Title = m_title;
            SetWMpid(m_MainForm->Handle);
            SetAppClass(m_MainForm->Handle, ExtractFileName(ExeName), m_MainForm->ClassName());
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TApplication :: HandleException(TObject *Sender)
{
    /*
      if GetCapture <> 0 then SendMessage(GetCapture, WM_CANCELMODE, 0, 0);
      if ExceptObject is Exception then
      begin
        if not (ExceptObject is EAbort) then
        	if(!OnException.empty())
    			OnException(Sender, Exception(ExceptObject));
    		else
    			ShowException(Exception(ExceptObject));
      end else
        SysUtils.ShowException(ExceptObject, ExceptAddr);
    */
}
//---------------------------------------------------------------------------
void __fastcall TApplication :: ShowException(Exception *e)
{
    TString msg = e->Message;
    printf("EXCEPTION %s\n", msg.c_str());
}
//---------------------------------------------------------------------------
void TApplication :: Minimize()  // частично работает
{
    TCustomForm *tmpform;

    if(m_MainForm)
    {
        for(TList::iterator from = Forms.begin();
                from != Forms.end();
                ++from)
        {
            tmpform = (TCustomForm *)(*from);
            XIconifyWindow(m_Handle, tmpform->Handle, Screen.Monitors[0]->MonitorNum);
//			tmpform->Visible = false;
        }
    }
    if(OnMinimize)
        OnMinimize(this);
}
//---------------------------------------------------------------------------
void TApplication :: Restore()  // not working
{
    TCustomForm *tmpform;

    if(m_MainForm)
    {
        for(TList::iterator from = Forms.begin();
                from != Forms.end();
                ++from)
        {
            tmpform = (TCustomForm *)(*from);
            tmpform->Visible = true;
        }
    }
    if(OnRestore)
        OnRestore(this);
}
//---------------------------------------------------------------------------
}  // namespace
//---------------------------------------------------------------------------------
