//---------------------------------------------------------------------------
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <locale.h>
#include <stddef.h>
#include <sys/types.h>
#include <ctype.h>
#include <time.h>
#include "rtti.h"
#include "application.h"
#include "netwm.h"
#include "TestForm.h"
#include "vxutil.h"
#include "grutils.h"
#include "xpainter.h"
#include "xwrender.h"
#include "stdgraph.h"
//---------------------------------------------------------------------------
TMainForm *MainForm1;
extern TForm *SecondForm;
//---------------------------------------------------------------------------
TMainForm :: TMainForm()
{
	id = 50;
    BoundsRect = TRect(0,0,800,500);

//    XSync(Application.Handle, false);
//OverrideRedirect = true;
//	Canvas->Font->Style = BIT_ON(Canvas->Font->Style, fsUnderline);// | fsItalic);

//	Align = alLeftCenter;
	Align = alCenter;

	TRegion rgn;
	rgn.ClipRect = TRect(10, 10, 32, 32);
	rgn.Expand(-29, -29);
	TRect rect1 = rgn.ClipRect;
printf("ALIGNCENTER id=%d %d %d %d %d\n",id, rect1.left, rect1.top, rect1.width, rect1.height);

    TRect rect = BoundsRect;
printf("ALIGNCENTER id=%d %d %d %d %d\n",id, rect.left, rect.top, rect.width, rect.height);

	TString ws = "Main Window:Главное окно";
   Caption = ws;

 m=j=k=l=0;
//for (int i=0; i<THREADS; ++i)
 // 	Threads[i].OnExecute = TNotifyEvent(this,&TMainForm :: SetTest);
//	thread.Detached = true;

	Timer.id = 333;
	Timer.OnTimer = TNotifyEvent(this,&TMainForm :: TestShow);
	Timer.Enabled = false;
	Timer.Interval = 5000;

	xbmimg.Load("../../images/access8.bmp");
//	jpgimg.Load("../../images/gradgray.jpg");
    Image1.id = 60;
	Image1.Parent = this;
	Image1.BoundsRect = TRect(350,50,300,200);
	Image1.Picture = &xbmimg;
	Image1.Stretch = true;
	Image1.Cursor = crHourGlass;
		Image1.OnMouseMove = EVENT(TMouseMoveEvent,TMainForm,TestMouseMove);
	Image1.OnMouseEnter = EVENT(TMouseMoveEvent,TMainForm,TestMouseEnter);
	Image1.OnMouseLeave = EVENT(TMouseMoveEvent,TMainForm,TestMouseExit);

//	gifimg.Load("../../cursors/object.gif");
//	gifimg.Load("../../cursors/cr02.gif");
//	Image2.Parent = this;
//	Image2.BoundsRect = TRect(450,210,50,50);
//	Image2.Picture = gifimg;
//	Image2.Stretch = true;

//	Image1.Proportional = true;

	Label1.id = 100;
	Label1.Caption = "First label: Second label";
	Label1.Parent = this;
	Label1.Left = 100;
	Label1.Top = 170;

//	Label1.BoundsRect = TRect(100,270,200,50);
//   Canvas->Font->Color = clFuchsia;
//	Label1.Color = clRed;
//	Label1.OnDblClick = EVENT(TNotifyEvent, TMainForm, SetDblTest);
//	Label1.OnStartDrag = EVENT(TStartDragEvent, TMainForm, TestStartDrag);
//	Label1.OnMouseEnter = &TestMouseEnter;
//   Label1.OnMouseLeave = &TestMouseExit;
//	Label1.DragMode = dmDragDrop;
//	Label1.Visible = false;

    Panel1.id = 222;
    Panel1.Parent = this;
	Panel1.BoundsRect = TRect(350,250,300,200);
    Panel1.Caption = "Panel1";
		Panel1.OnMouseMove = EVENT(TMouseMoveEvent,TMainForm,TestMouseMove);
	Panel1.OnMouseEnter = EVENT(TMouseMoveEvent,TMainForm,TestMouseEnter);
	Panel1.OnMouseLeave = EVENT(TMouseMoveEvent,TMainForm,TestMouseExit);

    Scrollbar1.Parent = &Panel1;
    Scrollbar1.Left = 5;
    Scrollbar1.Top = 150;
    Scrollbar1.Align = alBottom;
    Scrollbar1.Hint = "This horiz scrollbar";

    Scrollbar2.Parent = &Panel1;
    Scrollbar2.Left = 200;
    Scrollbar2.Top = 5;
    Scrollbar2.Kind = sbVertical;
    Scrollbar2.Align = alRight;
    Scrollbar2.Hint = "This vert scrollbar";

	Combobox1.id = 210;
	Combobox1.Parent = &Panel1;
//    Combobox1.Left = 10 + 140;
//    Combobox1.Top = 150;
//	Combobox1.Color = clBtnFace;
//	Combobox1.BoundsRect = TRect(10,10,180,15);
	Combobox1.Align = alCenter;
//	Combobox1.Hint = "This combobox";
		Combobox1.OnMouseMove = EVENT(TMouseMoveEvent,TMainForm,TestMouseMove);
	Combobox1.OnMouseEnter = EVENT(TMouseMoveEvent,TMainForm,TestMouseEnter);
	Combobox1.OnMouseLeave = EVENT(TMouseMoveEvent,TMainForm,TestMouseExit);

    Edit1.Parent = this;
    Edit1.id = 999;
    Edit1.Left = 200;
    Edit1.Top = 100;
    Edit1.Text = "Edit1";
    Edit1.OnClick = EVENT(TNotifyEvent, TMainForm, SetTest);
//    Edit1.Hint = "This TEdit";

	SpeedButton1.id = 200;
	SpeedButton1.Parent = this;
	SpeedButton1.Color = clBtnFace;
	SpeedButton1.BoundsRect = TRect(10,10,160,130);
	SpeedButton1.Flat = false;
	SpeedButton1.Transparent = false;
	SpeedButton1.Align = alCenter;
	SpeedButton1.Hint = "This speedbutton";
	SpeedButton1.OnClick = EVENT(TNotifyEvent, TMainForm, SetImageTest);
//	SpeedButton1.Cursor = crHourGlass;
	SpeedButton1.OnMouseMove = EVENT(TMouseMoveEvent,TMainForm,TestMouseMove);
	SpeedButton1.OnMouseEnter = EVENT(TMouseMoveEvent,TMainForm,TestMouseEnter);
	SpeedButton1.OnMouseLeave = EVENT(TMouseMoveEvent,TMainForm,TestMouseExit);
//	SpeedButton1.Visible = false;
//			SpeedButton1.Anchors << akTop << akLeft;
//	SpeedButton1.Anchors << akRight << akBottom;
/*
	SpeedButton2.id = 201;
	SpeedButton2.Parent = this;
	SpeedButton2.Color = clBtnFace;
	SpeedButton2.BoundsRect = TRect(10,10,160,130);
	SpeedButton2.Flat = false;
	SpeedButton2.Transparent = false;
//	SpeedButton2.Align = alCenter;
	SpeedButton2.Hint = "This speedbutton2";
	SpeedButton2.Visible = false;

	SpeedButton3 = SpeedButton1;
	SpeedButton3.BoundsRect = TRect(200,300,60,30);
	SpeedButton3.Visible = true; */

//	AButton1 = new TButton(this);
	AButton1.id = 300;
	AButton1.Caption = "OK";
	AButton1.Hint = "This Button";
	AButton1.Parent = this;
//	AButton1.Color = clBtnFace;
//	AButton1.BoundsRect = TRect(200,100,95,45);
//	AButton1.Left = 370;
//	AButton1.Top = 300;
	AButton1.Align = alCenter;
//	AButton1.PopupMenu = &PMenu;
	AButton1.OnClick = EVENT(TNotifyEvent, TMainForm, SetNETWMTest);
	AButton1.OnDblClick = EVENT(TNotifyEvent, TMainForm, SetDblTest);
	AButton1.OnMouseEnter = EVENT(TMouseMoveEvent,TMainForm,TestMouseEnter);
	AButton1.OnMouseLeave = EVENT(TMouseMoveEvent,TMainForm,TestMouseExit);
//	AButton1.OnMouseMove = EVENT(TMouseMoveEvent,TMainForm,TestMouseMove);
//	AButton1.Visible = false;
//    unsigned long valuemask = CWWinGravity;
//    XSetWindowAttributes attributes;
//    attributes.win_gravity = CenterGravity;
//    XChangeWindowAttributes(Application.Handle, AButton1.Handle, valuemask, &attributes);

	AButton2 = new TButton();
	AButton2->Parent = this;
	AButton2->id = 400;
	AButton2->Caption = "Cancel";//Отмена";
	AButton2->Hint = "This Cancel Button";
	AButton2->Color = clBtnFace;
//	AButton2->BoundsRect = TRect(400,300,80,30);
	AButton2->Left = 450;
	AButton2->Top = 300;
	AButton2->OnClick = EVENT(TNotifyEvent, TMainForm, SetTwoFormTest);
	AButton2->OnDblClick = EVENT(TNotifyEvent, TMainForm, SetDblTest);
//	AButton2->Cursor = crAppStart;
//	AButton2->OnMouseMove = SpeedButton1.OnMouseMove;
//	AButton2->Align = alCenter;
	AButton2->OnMouseLeave = EVENT(TMouseMoveEvent,TMainForm,TestMouseExit);
	AButton2->OnMouseEnter = EVENT(TMouseMoveEvent,TMainForm,TestMouseEnter);
//	AButton2->Anchors << akTop << akLeft;
//	AButton2->Anchors << akTop << akBottom;
	AButton2->Anchors = akRight | akBottom;
//	AButton2->Visible = false;

//    valuemask = CWWinGravity;
//    attributes.win_gravity = SouthEastGravity;
//    XChangeWindowAttributes(Application.Handle, AButton2->Handle, valuemask, &attributes);

//	AButton3 = *AButton2;
//	AButton3.BoundsRect = TRect(200,300,60,30);

	GroupBox1.id = 600;
	GroupBox1.Parent = this;
	GroupBox1.BoundsRect = TRect(10,250,150,150);
	GroupBox1.Caption = "TESTtest";
	GroupBox1.Anchors = akLeft | akTop | akRight | akBottom;
//	GroupBox1.BorderWidth = 3;
//	GroupBox1.BorderColor = clAqua;
	GroupBox1.Color = clBtnFace;

	CheckBox1.id = 500;
	CheckBox1.Caption = "Checkbox";
	CheckBox1.Parent = &GroupBox1;
   	CheckBox1.Color = clBtnFace;
	CheckBox1.Left = 20;
	CheckBox1.Top = 20;
   	CheckBox1.OnClick = EVENT(TNotifyEvent, TMainForm, SetTestCheck);
//   	CheckBox1.BorderWidth = 1;
//	CheckBox1.BorderColor = clRed;
	CheckBox1.Checked = true;
	CheckBox1.AllowUndefined = true;
	CheckBox1.Enabled = true;
	CheckBox1.Anchors = akRight | akBottom;
/*
	GroupBox2 = GroupBox1;
	GroupBox2.BoundsRect = TRect(180,300,150,150,30,30);
*/
	TBitmap bmpimg;
//	TPixelFormat a;
//	a.Depth = pf24bit;
	bmpimg.Load("../../images/a.bmp");
//	bmpimg.PixelFormat = a;
	bmpimg.TransparentColor = clFuchsia;
//	bmpimg.Mask(clWhite);
	HCURSOR hcur = CreateCursor(&bmpimg,0,0);
	xwings::Screen.Cursors[crUserCursor] = hcur;
	Cursor = crUserCursor;
//	Cursor1->Activate(this);
/*
    ScrollBarH.Parent = this;
	ScrollBarH.Kind = sbHorizontal;
    ScrollBarH.BoundsRect = TRect(40,20,230,20);

    ScrollBarV.Parent = this;
	ScrollBarV.Kind = sbVertical;
    ScrollBarV.BoundsRect = TRect(20,40,20,230);
*/

	OnPaint = EVENT(TNotifyEvent, TMainForm, TestPaint);
   OnMouseEnter = EVENT(TMouseMoveEvent,TMainForm,TestMouseEnter);
   OnMouseLeave = EVENT(TMouseMoveEvent,TMainForm,TestMouseExit);
   OnShow = EVENT(TNotifyEvent, TMainForm, TestShow);
   OnMouseMove = EVENT(TMouseMoveEvent, TMainForm, TestMouseMove);

//   OnDragOver = &TestDragOver;
   OnEndDrag = EVENT(TEndDragEvent, TMainForm, TestEndDrag);
   OnResize = EVENT(TNotifyEvent, TMainForm, SetTestResize);
    OnHide = EVENT(TNotifyEvent, TMainForm, SetTestResize);
    OnKeyUp = EVENT(TKeyEvent, TMainForm, DoKeyUp);
    OnKeyDown = EVENT(TKeyEvent, TMainForm, DoKeyDown);
    OnKeyPress = EVENT(TKeyPressEvent, TMainForm, DoKeyPress);

   Constraints->MinWidth = 200;
   Constraints->MinHeight = 200;
//   Constraints->MaxWidth = 600;
//   Constraints->MaxHeight = 600;

//	FormType = ftSplash;

	for(int i = 0; i < 9; i++)
	{
		ItemsMenu[i].Caption = "Menu Item" + TString(i);
//		ItemsMenu[i].OnClick = EVENT(TNotifyEvent, TMainForm, SetDblTest);
		ItemsMenu[i].id = i;
		ItemsMenu[i].ImageIndex = i;
//		PMenu.Add(&ItemsMenu[i]);
		PMenu.Add(ItemsMenu[i]);
	}
	MMenu.id = 888;
	MMenu.Images = &Images1;
	for(int i = 0; i < 4; i++)
	{
//		MMenu.Add(&ItemsMenu[i]);
		MMenu.Add(ItemsMenu[i]);
		ItemsMenu[i].ImageIndex = -1;
	}
	Menu = &MMenu;

//		ItemsMenu[1].Submenu = &PMenu;
        ItemsMenu[1].Add(ItemsMenu[4]);
        ItemsMenu[1].Add(ItemsMenu[5]);

		ItemsMenu[4].Caption = "Create";
		ItemsMenu[4].ShortCut = TextToShortCut("Ctrl+N");

		ItemsMenu[5].Caption = "Open";
		ItemsMenu[5].ShortCut = TextToShortCut("Ctrl+O");

		ItemsMenu[6].Caption = "-";
		ItemsMenu[8].Caption = "-";

//		ItemsMenu[6].Kind = miLine;
//		ItemsMenu[8].Kind = miLine;

		ItemsMenu[7].Submenu = &PMenu3;
	for(int i = 10; i < 20; i++)
	{
		ItemsMenu[i].Caption = "Menu Item" + TString(i);
		ItemsMenu[i].OnClick = EVENT(TNotifyEvent, TMainForm, SetDblTest);
		ItemsMenu[i].id = i;
		ItemsMenu[i].ImageIndex = i;
//		PMenu3.Add(&ItemsMenu[i]);
		PMenu3.Add(ItemsMenu[i]);
	}
	PMenu3.Images = &Images1;

		ItemsMenu[9].Caption = "Exit";
		ItemsMenu[9].OnClick = EVENT(TNotifyEvent, TMainForm, SetTwoFormTest);
		ItemsMenu[9].id = 10;
		ItemsMenu[9].ImageIndex = 11;
		ItemsMenu[9].ShortCut = TextToShortCut("Ctrl+X");
//		PMenu.Add(&ItemsMenu[9]);
		PMenu.Add(ItemsMenu[9]);

	PMenu.id = 700;
	PMenu.Images = &Images1;
	PopupMenu = &PMenu;

	PMenu2.id = 900;
	for(int i = 0; i < 7; i++)
	{
//		PMenu2.Add(&ItemsMenu[i]);
		PMenu2.Add(ItemsMenu[i]);
	}
	PMenu2.Images = &Images1;
	CheckBox1.PopupMenu = &PMenu2;

	Application.OnMessage = EVENT(TMessageEvent, TMainForm, DispatchEvent);
//	Application.OnIdle = EVENT(TIdleEvent, TMainForm, MyIdleHandler);
	Application.OnActivate = EVENT(TNotifyEvent, TMainForm, ActivateApp);
	Application.OnDeactivate = EVENT(TNotifyEvent, TMainForm, DeActivateApp);
	Application.OnSignal = EVENT(TSignalEvent, TMainForm, TestSignal);

	Signal.Signal = SIGINT;
	Signal.ResetHandler = true;
	Signal.Handler = shCustom;
	Signal.OnSignal = EVENT(TSignalEvent, TMainForm, TestSignal);

	Images1.Load("../../images/access4.bmp");
	Images1.ImageWidth = 26;
	Images1.ImageHeight = 24;
	Images1.PixelFormat = TPixelFormat();
	Images1.TransparentColor = 0xc0c0c0;

//	Cursor1 = new TCustomCursor(&Images1,0,0);
//	Cursor1->Activate(this);

	SockThread.OnExecute = TNotifyEvent(this,&TMainForm :: SetSockTest);
	SockThread.Start();

//	Desktop = -1;

//	for(int i = 0; i < 5; i++)
//	{
//		Process1[i].id = i;
//		Process1[i].OnExecute = TNotifyEvent(this,&TMainForm :: SetProcTest);
//		Process1[i].Start();
//	}

	ws = GetWMName();
	printf("WINDOWMANAGER %s\n", ws.c_str());
    Mouse.CursorPos = TRect(0, 0, xwings::Screen.Width, xwings::Screen.Height).center();
/*
  char *names_ret;
  int nitems;
  void *p1;
  Atom targets;
  targets = XInternAtom(Application.Handle, "TARGETS", true);
  names_ret = XGetAtomName(Application.Handle, XA_PRIMARY);
			printf("XA_PRIMARY is %s\n",names_ret);
			XFree(names_ret);
    XSetSelectionOwner(Application.Handle, XA_PRIMARY, m_Handle, CurrentTime);
//    XFlush(Application.Handle);
   p1 = GetXProperty(m_Handle, targets, &nitems);
   XConvertSelection(Application.Handle, XA_PRIMARY, XA_STRING, None, m_Handle, CurrentTime);
*/
}
//---------------------------------------------------------------------------
static int var;

TMainForm :: ~TMainForm()
{
//	delete Label1;
//   delete SpeedButton1;
//	delete Cursor1;
//   delete AButton2;
}
//---------------------------------------------------------------------------
int TMainForm :: get_Test(int x, int y)
{
	printf("GET TEST %d %d %d\n", x, y, var);
	return var;
}
//---------------------------------------------------------------------------
void TMainForm :: set_Test(int x, int y, int v)
{
	printf("SET TEST %d %d %d\n", x, y, v);
	var = v;
}
//---------------------------------------------------------------------------
void TMainForm :: TestShow(TObject *object)
{
	TTimer *t = dynamic_cast<TTimer *>(object);
	if(t)
	{
	    t->Enabled = false;

//		printf("OnShowTimer: %d int=%d\n",object->id,*t->Interval);
//		t->Interval += 50;
        printf("OnShowTimer BEGIN\n");
        TRect rect = ClientRect;
		printf("OnShowTimer: %d int=%d active %d\n",object->id,*t->Interval, *Application.Active);
//		if(!Application.Active)
//			Application.Restore();
        t->Enabled = true;
	}
}
//---------------------------------------------------------------------------
void TMainForm :: AcceptSockTest(TSocket *object, TSocket *sock)
{
	TProcess Process1;
	Process1.OnExecute = TNotifyEvent(this, &TMainForm :: SetProcTest);
	Process1.Data = sock;
	Process1.Start();
	Process1.WaitFor();
	sock->Close();
}
//---------------------------------------------------------------------------
void TMainForm :: AcceptSockTest2(TSocket *object, TSocket *sock)
{
//	printf("WORKING %d %p\n", object->id, sock);
//	perror("ERR Server");
	char buff[128];
	time_t ticks = time(NULL);
	snprintf(buff,128,"%24s", ctime(&ticks));
//	printf("server %s",buff);
	sock->Write((const void *)buff,strlen(buff) + 1);
	sock->Close();
//	printf("end server\n");
}
//---------------------------------------------------------------------------
void TMainForm :: SetSockTest(TObject *object)
{
//	TSocket *s;
	TLocalSocket Server1;
//	TInetSocket Server1;
	Server1.Type = stDgram;
//	Server1.Kind = skServerProcess;
	Server1.Kind = skServer;
	Server1.Address = "../../bin/098765.txt";
//	Server1.LocalAddress = "localhost";
//	Server1.LocalPort = 50000;
//	Server1.OnAccept = TAcceptEvent(this, &TMainForm :: AcceptSockTest);
	Server1.OnAccept = TAcceptEvent(this, &TMainForm :: AcceptSockTest2);
	Server1.Open();
	Server1.Connect();
	while(1)
	{
//		Server1.Accept();
			char buff[128];
			memset(buff, 0, sizeof(buff));
			Server1.Read(buff, sizeof(buff));
			printf("server1 %s\n",buff);
			time_t ticks = time(NULL);
			snprintf(buff,128,"%24s", ctime(&ticks));
	printf("server2 %s",buff);
			Server1.Write((const void *)buff,strlen(buff) + 1);
//			sleep(5);
	}
/*
	while(1)
	{
		Server1.Accept();
		s = Server1.Connections[0];
		perror("ERR Server");
		char buff[128];
		time_t ticks = time(NULL);
		snprintf(buff,128,"%24s", ctime(&ticks));
		printf("server %s\n",buff);
		s->Write(buff,strlen(buff) + 1);
		Server1.Close(s);
	}
*/
}
//---------------------------------------------------------------------------
void TMainForm :: SetProcTest(TObject *object)
{
	TProcess *proc = dynamic_cast<TProcess *>(object);
	TSocket *s = (TSocket *)proc->Data;

	printf("WORKING %d %p\n", object->id, s);
	perror("ERR Server");
	char buff[128];
	time_t ticks = time(NULL);
	snprintf(buff,128,"%24s", ctime(&ticks));
	printf("server %s",buff);
	s->Write((const void *)buff,strlen(buff) + 1);
//	s->Close();
	printf("end server\n");
}
//---------------------------------------------------------------------------
void TMainForm :: SetTest(TObject *object)
{
//    Edit1.PasteFromClipboard();

//	TThread *th = dynamic_cast<TThread *>(object);
//	do {
//		th->Lock();
//		printf("OnClick: %d\n",object->id++);
//		th->Unlock();
//		sleep(1);
//	}while(1);
/*
		Mutex1.Lock();

  int   loop = 0;

  for (loop=0; loop<LOOPCONSTANT; ++loop)
  {
    ++m; ++j; ++k; ++l;
  }

      Mutex1.Unlock(); */
}
//---------------------------------------------------------------------------
void TMainForm :: SetTwoFormTest(TObject *object)
{
	printf("TwoForm: %d\n",object->id);
	Close();
//	SecondForm->Show();
}
//---------------------------------------------------------------------------
void TMainForm :: SetNETWMTest(TObject *object)
{

	printf("NETWM OnClick: %d\n",object->id);
TStringList strs;
GetNETWMSupport(NULL, &strs);
for(int i = 0; i < strs.Count; i++)
{
	printf("%s\n", strs[i].c_str());
}

//	Desktop = 0;
/*
	AllowedActions = *AllowedActions >> aaMove;
	TRect r = SpeedButton1.BoundsRect;
//	Canvas->Highlight(r, 20);
//	VColor clr = Canvas->Pixels(r.left + 10, r.top + 10);
//	printf("COLOR %x\n", clr);
//	Canvas->Pixels(r.left + 11, r.top + 11, clRed);
	DrawArrow(Canvas, 100, 220);
	DrawArrow(Canvas, 100, 230, 0);
	DrawArrow(Canvas, 100, 240, 1);
	DrawGradient(Canvas, r, false);

	Images1.Draw(Canvas, 500, 100, 1);
*/
	TLocalSocket Client1;
//	TInetSocket Client1;
	Client1.Kind = skClient;
	Client1.Type = stDgram;
	Client1.Address = "../../bin/098765.txt";
//	Client1.RemotePort = 50000;
//	Client1.RemoteAddress = "localhost";
//	Client1.RemoteHost = "www.rambler.ru";
//	Client1.RemotePort = 80;
//	Client1.RemoteService = "www";
	Client1.Open();
	Client1.Connect();
	perror("ERR2");
//	printf("CLIENT REMOTEADDRESS %s: %d\n", (*Client1.RemoteAddress).c_str(), *Client1.RemotePort);
//	printf("CLIENT LOCALADDRESS %s: %d\n", (*Client1.LocalAddress).c_str(), *Client1.LocalPort);

	char buff[128];
	buff[0] = '1'; buff[1] = '2'; buff[2] = '3'; buff[3] = '\0';
	Client1.Write(buff,strlen(buff));
	Client1.Read(buff,sizeof(buff));
	printf("CLIENT %s\n", buff);

TRect rrr = TRect(350,50,300,200);
    TBitmap bmp;
    HBITMAP hbmp = WindowToBitmapRect(m_Handle, rrr);
    bmp.LoadImage(hbmp);
    bmp.Save("/home/user/09876.bmp");
    XFreePixmap(Application.Handle, hbmp);

    hbmp = WindowToBitmap(xwings::Screen.Root);
    bmp.LoadImage(hbmp);
    bmp.Save("/home/user/12345.bmp");
    XFreePixmap(Application.Handle, hbmp);

    	Application.Minimize();
}
//---------------------------------------------------------------------------
void TMainForm :: SetTestCheck(TObject *object)
{
    if(!CheckBox1.Checked)
        Caret.Hide();
    else
        Caret.Show(&Edit1);

    TPoint p = ClientOrigin;
    TPoint p1 = ClientToScreen(TPoint(10,10));
    printf("FORMCLIENTORIGIN %d %d %d %d\n", p.x, p.y, p1.x, p1.y);

    Edit1.CopyToClipboard();

    printf("COPY_TO_CLIPBOARD %d\n", id);
/*   if(CheckBox1.Checked)
   {
//   		SecondForm->Active = false;
   		Canvas->Font->Color = clFuchsia;
  printf("Creating %d threads\n", THREADS);
  for (int i=0; i<THREADS; ++i)
  	Threads[i].Start();

	   	CheckBox1.Caption = "Start thread";

	  printf("Wait for results\n");
  for (int i=0; i<THREADS; ++i)
  	Threads[i].WaitFor();

  printf("\nUsing %d threads and LOOPCONSTANT = %d\n",
         THREADS, LOOPCONSTANT);
  printf("Values are: (should be %d)\n", THREADS * LOOPCONSTANT);
  printf("  ==>%d, %d, %d, %d\n", m, j, k, l);

  printf("Main completed\n");

   }
   else
   {
//   	SecondForm->Active = true;
   	Canvas->Font->Color = clAqua;
	   CheckBox1.Caption = "Stop thread";

   }
*/
}
//---------------------------------------------------------------------------
void TMainForm :: SetTestResize(TObject *object)
{
//	static int i;
	printf("OnResize: %d\n",object->id);
}
//---------------------------------------------------------------------------
void TMainForm :: SetDblTest(TObject *object)
{
	printf("OnDblClick: %d\n",object->id);
//	printf("Handle = %x\n", Handle);
}
//---------------------------------------------------------------------------
void TMainForm :: SetTripleTest(TObject *object)
{
	printf("OnTripleClick: %d\n",object->id);
//	printf("Handle = %x\n", Handle);
}
//---------------------------------------------------------------------------
void TMainForm :: SetTestMouse(TObject *Sender, uint_t button,
    									 uint_t state, int x, int y)
{
	printf("Mouse %d : %d\n",button, state);
//	printf("Handle = %x\n", Handle);
}
//---------------------------------------------------------------------------
void TMainForm :: TestMouseMove(TObject *Sender, uint_t state, int x, int y)
{
	printf("MouseMove id=%d st=%d, x=%d, y=%d\n", Sender->id, state, x, y);
//	printf("Handle = %x\n", Handle);
}
//---------------------------------------------------------------------------
void TMainForm :: TestMouseEnter(TObject *Sender, unsigned int state, int x, int y)
{
	printf("MouseEnter id=%d st=%d, x=%d, y=%d\n", Sender->id, state, x, y);
//	printf("Handle = %x\n", Handle);
}
//---------------------------------------------------------------------------
void TMainForm :: TestMouseExit(TObject *Sender, unsigned int state, int x, int y)
{
	printf("MouseLeave id=%d st=%d, x=%d, y=%d\n", Sender->id, state, x, y);
//	printf("Handle = %x\n", Handle);
}
//---------------------------------------------------------------------------
void TMainForm :: TestStartDrag(TObject *Sender, TObject *&drag, unsigned int state, int x, int y)
{
//	drag = &SpeedButton1;
//   VLabel lbl(this);
//   drag = &lbl;
//	drag = new VLabel(this);
//	drag->id = 222;
	printf("StartDrag id=%d dragid=%d st=%d, x=%d, y=%d\n", Sender->id,drag->id, state, x, y);
}
//---------------------------------------------------------------------------
void TMainForm :: TestDragOver(TObject* Sender, TObject *Source,
               uint_t state, int x, int y, TDragState dstate, bool &Accept)
{
	printf("DragOver id=%d dragid=%d st=%d, x=%d, y=%d\n", Sender->id,Source->id, state, x, y);
//printf("Handle = %x\n", Handle);
}
//---------------------------------------------------------------------------
void TMainForm :: TestEndDrag(TObject* Sender, TObject* Source, unsigned int state,int x, int y)
{
	printf("EndDrag id=%d dragid=%d st=%d, x=%d, y=%d\n", Sender->id,Source->id, state, x, y);
//	delete Source;
//printf("Handle = %x\n", Handle);
}
//---------------------------------------------------------------------------
void TMainForm :: TestPaint(TObject *object)
{
    Canvas.Brush->Style = bsSolid;
    Canvas.Pen->Style = psSolid;
    Canvas.Brush->Color = clBlue;
    Canvas.FillRect(180, 250, 50, 50);
    Canvas.Brush->Color = clRed;
    Canvas.FillRect(230, 250, 50, 50);
    Canvas.Brush->Color = clGreen;
    Canvas.FillRect(180, 300, 50, 50);
    Canvas.Brush->Color = clBlack;
    Canvas.FillRect(230, 300, 50, 50);

	TRect maskrect = TRect(10, 10, 100, 100);
	TPoint masko = TPoint(5, 5);
	TPoint ro = TPoint(50, 50);

    Canvas.Pen->Color = clBlue;
    Canvas.Rectangle(0, 0, 200, 200);
//    Canvas->ClipOrigin = &masko;
 //   Canvas->ClipMask = TRegion(maskrect);
//    Canvas->ClipOrigin = &masko;

    Canvas.Brush->Color = clMaroon;

    TRegion reg;
    reg += TRect(100, 100, 50, 50);
    reg += TRect(120, 120, 100, 100);
    Canvas.FillRegion(reg);
//    Canvas.FillRect(0, 0, 120, 120);

    Canvas.Pen->Color = clGreen;
    Canvas.Rectangle(maskrect);

	TRect rect = ClientRect;
	Canvas.Pen->Color = clRed;
    Canvas.Rectangle(rect);

       TRect rect1 = TRect(rect.left + 10,rect.top + 10,150,150);
   Canvas.Rectangle(rect1);
   rect.inflate(-2,-2);
//   Canvas.Brush->Color = clNone;
   Canvas.Font->Color = clBlue;

    FrameRounded(Canvas, rect, clGreen, 20);
        rect.inflate(-10,-10);
        rect.rx = rect.ry = 20;
    Canvas.Rectangle(rect);
//   Canvas->Font->PenStyle = psDot;
//   Canvas->Font->Style = fsUnderline | fsItalic;
//   int old = Canvas->Font->Pixels;
//   Canvas->Font->Pixels = 24;
TFormatText ft;
//ft.Justify = taRightJustify;
ft.Layout = tlCenter;
ft.Justify = taCenter;
ft.Leading = 0;
//ft.Justify = taLeftJustify;
ft.PrefixColor = clRed;
ft.PrefixStyle = pmColor;
ft.Options = DT_EXPANDTABS | DT_END_ELLIPSIS | DT_WORDBREAK;// | DT_CALCRECT;
TString str = "VERY very\tvery ph&one&&dou&ble\nlong long short LONG STRING&";
    Canvas.Brush->Style = bsClear;
    Canvas.DrawText(rect1, str, &ft);
    Canvas.Brush->Style = bsSolid;
/*
	Canvas->Pen->Width = 1;
   Canvas->Pen->Color = clBlue;
   Canvas->Pen->Style = psDash;
   Canvas->Line(250,rect.top + 10,400,rect.top + 10);
   Canvas->Pen->Style = psDot;
   Canvas->Line(250,rect.top + 20,400,rect.top + 20);
   Canvas->Pen->Style = psDashDot;
   Canvas->Line(250,rect.top + 30,400,rect.top + 30);
   Canvas->Pen->Style = psDashDotDot;
   Canvas->Line(250,rect.top + 40,400,rect.top + 40);
   Canvas->Pen->Style = psSolid;
   Canvas->Line(250,rect.top + 50,400,rect.top + 50);
    Canvas->Pen->Style = psClear;
   Canvas->Line(250,rect.top + 60,400,rect.top + 60);

   Canvas->Brush->Color = clAqua;
//   Canvas->Brush->Style = bsClear;
   Canvas->FillRect(250,rect.top + 70,150,20);
   Canvas->Brush->Style = bsSolid;
   Canvas->Pen->Style = psSolid;
*/
//   TRect tr[3];
//   tr[0] = TRect(15,223,145,135,30,30);
//   tr[1] = TRect(10,220,150,75);
//   tr[2] = TRect(25,245,45,35,15,15);
//   Canvas->Rectangles(tr, 3);

//Canvas->Brush->Color = clBlue;
//Canvas->FillRect(TRect(15,223,145,135,30,30));

//Frame3D(Canvas, TRect(10,220,150,75,30,30), clBtnShadow, clBtnHighlight, 3);
//Frame3D(Canvas, TRect(10,220,150,150,30,30), clBtnHighlight, clBtnShadow, 3);
//DrawEdge(Canvas, TRect(10,220,150,150,30,30), esLowered, esRaised, ebRect);
//DrawEdge(Canvas, TRect(10,220,150,150,30,30), esRaised, esLowered, ebRect);
//DrawEdge(Canvas, TRect(10,220,150,150,0,0), esRaised, esLowered, ebRect);
//DrawButtonFace(Canvas, TRect(10,220,150,150,0,0), false, false, false, clBtnFace);

/*

Canvas->Pen->Color = clBlack;
CScreen.PaintControl(*this);
   XPainter.DrawFocusRect(TRect(250,rect.top + 70,150,20));

//     TString str1 = "VERY very very phone double long long short LONG STRING";
     Canvas->TextOut(10, 120, str);
     DrawArrow(Canvas, clRed, TRect(rect.left + 15, rect.top + 105, 30, 30), arrowLeft, true);
     DrawArrow(Canvas, clRed, TRect(rect.left + 15 + 35, rect.top + 105, 30, 30), arrowUp);
     DrawArrow(Canvas, clRed, TRect(rect.left + 15, rect.top + 105 + 35, 30, 30), arrowRight, true);
     DrawArrow(Canvas, clRed, TRect(rect.left + 15 + 35, rect.top + 105 + 35, 30, 30), arrowDown);

     CScreen.PaintControl(*this);
     XPainter.DrawDisabledText(rect1, "very very\tvery phone double\nlong long long short string");
   Canvas->Font->Color = clBlack;
//     DrawButtonFaceEx(Canvas, &Label1, TImageList *Glyph, int GlyphIndex, int Spacing,
//                TButtonLayout Layout, TString &Caption, TALFocusStyle FocusStyle,
//                    bool Focused, bool Autosize, bool Transparent, TFont *font,
//                    TColor Color, TColor BorderColor, TColor MarkColor, TColor BlankColor,
//                    TCheckBoxState State, TALButtonCategory ButtonCategory);
/*
     DrawButtonFaceEx(Canvas, &Label1, NULL, 1, 0,
                    blGlyphTop, "Test", ALFSBorder,
                    false, false, true, Canvas->Font,
                    clAqua, clBlue, clRed, clWhite,
                    cbChecked, ChkBox);
*/
//      Canvas->Font->Pixels = old;
//	char buf[256];
//	sprintf(buf,"l=%d t=%d w=%d h=%d",(short)MainForm1->Left,(short)MainForm1->Top,(short)MainForm1->Width,(short)MainForm1->Height);
//	MainForm1->Label1.Caption = buf;

//	Canvas->CopyRect(rect, xpmimg.Canvas, TRect(0,0,xpmimg.Width,xpmimg.Height));

    TPNGImage pngimg;
	pngimg.Load("../../images/pngtest.png");
	Canvas.Draw(50,150,pngimg);
	Canvas.StretchDraw(TRect(50, 150, 50, 50), pngimg);
//	pngimg.SaveAs("../../images/pngtest1.bmp");

    TICOImage icoimg;
	icoimg.Load("../../images/earth.ico");
//	icoimg.HandleType = bmDDB;
	Canvas.Draw(130,150,icoimg);
	Canvas.StretchDraw(TRect(130, 150, 100, 100), icoimg);
//	pngimg.SaveAs("../../images/pngtest1.bmp");

//Canvas.RemoveClipMask();

    Canvas.Pen->Style = psSolid;
    DrawCheck(Canvas, TPoint(250, 50), 2, 4);

//    DrawArrow(Canvas, dUp, TPoint(260, 50), 10);
//    DrawArrow(Canvas, dLeft, TPoint(280, 50), 10);
//    DrawArrow(Canvas, dRight, TPoint(300, 50), 10);
//    DrawArrow(Canvas, dDown, TPoint(320, 50), 10);

//    DrawChevron(Canvas, dUp, TPoint(260, 50), 2, 10, 5);
//    DrawChevron(Canvas, dLeft, TPoint(280, 50), 2, 10, 5);
//    DrawChevron(Canvas, dRight, TPoint(300, 50), 2, 10, 5);
    DrawChevron(Canvas, dDown, TPoint(320, 50), 2, 10, 5);

    Canvas.Pen->Color = clRed;
    Canvas.Rectangle(320, 50, 4, 4);

    int stops[3];
    stops[0] = 0;
    stops[1] = 50;
    stops[2] = 100;
    TColor colors[3];
    colors[0] = clAqua;
    colors[1] = clFuchsia;
    colors[2] = clGreen;
//    DrawLinearGradient(Canvas, TRect(150, 150, 200, 100), stops, colors, 3, 45);
//    DrawConicalGradient(Canvas, TRect(10, 50, 300, 300), stops, colors, 3);
//    DrawRadialGradient(Canvas, TRect(10, 50, 300, 300), stops, colors, 3);
//    DrawLinearGradient(Canvas, TRect(150, 150, 200, 100), clRed, clGreen, 45);
}
//---------------------------------------------------------------------------
void TMainForm :: SetImageTest(TObject *object)
{
/*	TBMPImage out;
//	TXPMImage out1;
	TBitmap xbmimg, xpmimg;
	xbmimg.Load("../../images/321.bmp");
	xpmimg.Load("../../images/about.xpm");

TString s = "../../images/about.xpm";
TString s1;
s1 = ExtractFileExt(s);
printf("EXT %s\n", s1.c_str());
s1 = ExtractFilePath(s);
printf("PATH %s\n", s1.c_str());
s1 = ExtractFileName(s);
printf("NAME %s\n", s1.c_str()); */
//	TXPMImage xpmimg;
//	xpmimg.Load("../../images/about.xpm");

//	xbmimg.HandleType = bmDDB;
//	xbmimg.Mask(0xc0c0c0);
//	xbmimg.Mask(0xffffff);
//	imglist = xbmimg;

//	TColor c1, c2;
//	c1 = Canvas->Pen->Color;
//	c2 = Canvas->Brush->Color;
//	printf("COLORS PEN %lx, BRUSH %lx, DEPTH %d\n", c1, c2, (int)(*xbmimg.PixelFormat).Depth);
//	Canvas->Pen->Color = clWhite;
//	Canvas->Brush->Color = clBlack;

//	TBitmap tmp = tmpimg;
//	TJPGImage xbmimg;
//	xbmimg = tmpimg;

//clock_t cl1 = clock();
//xbmimg.Mirror(mtMirrorHorz);
//TRect rect = TRect(350,50,200,150);

    TJPGImage jpgimg;
	jpgimg.Load("../../images/gradgray.jpg");
//	Blend(xbmimg, xpmimg, jpgimg, out);
//    AddMonoNoise(dynamic_cast<TBitmap &>(*Image1.Picture), 64);
//    AddColorNoise( dynamic_cast<TBitmap &>(*Image1.Picture), 128);
//    ShadowDownLeft(dynamic_cast<TBitmap &>(*Image1.Picture));
//    Emboss( dynamic_cast<TBitmap &>(*Image1.Picture), 5);
//    Saturation( dynamic_cast<TBitmap &>(*Image1.Picture), 0);
//    Invert( dynamic_cast<TBitmap &>(*Image1.Picture));
//    Triangles(dynamic_cast<TBitmap &>(*Image1.Picture), 50);
//    RippleTooth(dynamic_cast<TBitmap &>(*Image1.Picture), 50);
//    RippleTriangle(dynamic_cast<TBitmap &>(*Image1.Picture), 50);
//    RippleRandom(dynamic_cast<TBitmap &>(*Image1.Picture), 50);
//    Solorize(dynamic_cast<TBitmap &>(*Image1.Picture), dynamic_cast<TBitmap &>(*Image1.Picture), 5);
//    Posterize(dynamic_cast<TBitmap &>(*Image1.Picture), dynamic_cast<TBitmap &>(*Image1.Picture), 50);
//    TurnRight(dynamic_cast<TBitmap &>(*Image1.Picture), jpgimg);
//    Turn(dynamic_cast<TBitmap &>(*Image1.Picture), jpgimg);
//    Image1.Picture = jpgimg;
//    HeightMap(dynamic_cast<TBitmap &>(*Image1.Picture), 5);
//    Spray(dynamic_cast<TBitmap &>(*Image1.Picture), 50);
//    Mosaic(dynamic_cast<TBitmap &>(*Image1.Picture), 10);
//    Lightness(dynamic_cast<TBitmap &>(*Image1.Picture), 50);
//    Darkness(dynamic_cast<TBitmap &>(*Image1.Picture), 50);
//    SplitBlur(dynamic_cast<TBitmap &>(*Image1.Picture), 5);
//    GaussianBlur(dynamic_cast<TBitmap &>(*Image1.Picture), 5);
//    Contrast(dynamic_cast<TBitmap &>(*Image1.Picture), 50);
//	Blend(*(TBitmap *)(*Image1.Picture), xpmimg, jpgimg, *(TBitmap *)(*Image1.Picture) );

    TBitmap *src = dynamic_cast<TBitmap *>(*Image1.Picture);
    src->PixelFormat = TPixelFormat(pf24bit);
    src->HandleType = bmDIB;
   		printf("\nwidth            = %d\n", src->Data->width);
  printf("height           = %d\n", src->Data->height);
  printf("xoffset          = %d\n", src->Data->xoffset);
  printf("format           = %d\n", src->Data->format);
  printf("data             = %p\n", src->Data->data);
  printf("byte_order       = %d\n", src->Data->byte_order);
  printf("bitmap_unit      = %d\n", src->Data->bitmap_unit);
  printf("bitmap_bit_order = %d\n", src->Data->bitmap_bit_order);
  printf("bitmap_pad       = %d\n", src->Data->bitmap_pad);
  printf("depth            = %d\n", src->Data->depth);
  printf("bytes_per_line   = %d\n", src->Data->bytes_per_line);
  printf("bits_per_pixel   = %d\n", src->Data->bits_per_pixel);
  printf("red_mask         = %lx\n", src->Data->red_mask);
  printf("green_mask       = %lx\n", src->Data->green_mask);
  printf("blue_mask        = %lx\n", src->Data->blue_mask);

//    src.PixelFormat = TPixelFormat(pf4bit);

		unsigned long	valuemask = 0;
		XRenderPictureAttributes pa;
//		int	format;
		XRenderPictFormat *pictformat, *pfalfa, *pfA8;
		Picture pic1, pic2, pic3;

		XFilters *flt = XRenderQueryFilters(Application.Handle, m_Handle);
		for(int i = 0; i < flt->nfilter; i++)
			printf("FILTER: %s\n", flt->filter[i]);
		XFree(flt);

/*		switch(pf.Depth)
		{
			case pf1bit: format = PictStandardA1; break;
			case pf4bit: format = PictStandardA4; break;
			case pf8bit: format = PictStandardA8; break;
			case pf16bit: format = PictStandardA8; break;
			case pf24bit: format = PictStandardRGB24; break;
			case pf32bit: format = PictStandardARGB32; break;
			default: format = PictStandardARGB32;
		} */
		pictformat = XRenderFindStandardFormat(Application.Handle, PictStandardRGB24);
        pfalfa = XRenderFindStandardFormat(Application.Handle, PictStandardA8);


    XRenderPictFormat templ = {
        0,                          /* id */
        PictTypeDirect,             /* type */
        24,                         /* depth */
        {                           /* direct */
            8,                     /* direct.red */
            0xff,                   /* direct.redMask */
            8,                      /* direct.green */
            0xff,                   /* direct.greenMask */
            8,                      /* direct.blue */
            0xff,                   /* direct.blueMask */
            0,                     /* direct.alpha */
            0,                   /* direct.alphaMask */
        },
        0,                          /* colormap */
    };
    unsigned long mask =
        PictFormatType |
        PictFormatDepth; // |
/*        PictFormatRed |
        PictFormatRedMask |
        PictFormatGreen |
        PictFormatGreenMask |
        PictFormatBlue |
        PictFormatBlueMask |
        PictFormatAlpha | */
//        PictFormatAlphaMask;
        templ.type = PictTypeDirect; //PictTypeIndexed;
//        templ.depth = (*src.PixelFormat).Depth;
        int count = 0;
        pfA8 = XRenderFindFormat(Application.Handle, mask, &templ, count);
        if(pfA8 == NULL)
            exit(1);
        pfA8 = XRenderFindStandardFormat(Application.Handle, PictStandardRGB24);

		valuemask = CPRepeat | CPAlphaMap | CPAlphaXOrigin | CPAlphaYOrigin |
                    CPClipXOrigin | CPClipYOrigin | CPSubwindowMode | CPClipMask | CPPolyEdge |
                    CPPolyMode | CPComponentAlpha;

        pa.repeat = 0;
        pa.alpha_map = None;
        pa.alpha_x_origin = 0;
        pa.alpha_y_origin = 0;
        pa.clip_x_origin = 0;
        pa.clip_y_origin = 0;
        pa.clip_mask = None;
        pa.subwindow_mode = ClipByChildren;    // ClipByChildren, IncludeInferiors
        pa.poly_edge = PolyEdgeSmooth;
        pa.poly_mode = PolyModePrecise;
        pa.component_alpha = true;
 		pic1 = XRenderCreatePicture(Application.Handle, jpgimg.Handle, pfalfa, valuemask, &pa);

        pa.repeat = 0;
        pa.alpha_map = None;
        pa.alpha_x_origin = 0;
        pa.alpha_y_origin = 0;
        pa.clip_x_origin = 0;
        pa.clip_y_origin = 0;
        pa.clip_mask = None;
        pa.subwindow_mode = ClipByChildren;    // ClipByChildren, IncludeInferiors
        pa.poly_edge = PolyEdgeSmooth;
        pa.poly_mode = PolyModePrecise;
        pa.component_alpha = false;
        pic2 = XRenderCreatePicture(Application.Handle, src->Handle, pfA8, valuemask, &pa);

        pa.repeat = 0;
        pa.alpha_map = None;
        pa.alpha_x_origin = 0;
        pa.alpha_y_origin = 0;
        pa.clip_x_origin = 0;
        pa.clip_y_origin = 0;
        pa.clip_mask = None;
        pa.subwindow_mode = ClipByChildren;    // ClipByChildren, IncludeInferiors
        pa.poly_edge = PolyEdgeSmooth;
        pa.poly_mode = PolyModePrecise;
        pa.component_alpha = false;
        pic3 = XRenderCreatePicture(Application.Handle, m_Handle, pictformat, valuemask, &pa);


//	XRenderColor colorTrans = {0x0000, 0x0000, 0x0000, 0x0000};
//	XRenderColor colorWhite = {0xffff, 0xffff, 0xffff, 0xffff};

//Picture XRenderCreateSolidFill(Application.Handle, const XRenderColor *color);

        int	op = PictOpSrc;
//        for(int i = PictOpMinimum; i < PictOpMaximum; i++)
//        {
            XRenderComposite(Application.Handle, op, pic2, pic1, pic3,
                0, 0, 0, 0, 0, 0, 600, 500);
//            Application.ProcessMessages();
//            XRenderFreePicture(Application.Handle, pic3);
//            pic3 = XRenderCreatePicture(Application.Handle, m_Handle, pictformat, valuemask, &pa);
//            getchar();
//        }
//    XRenderFillRectangle(Application.Handle, op, pic3, &colors[1], 0, 0, 600, 500);
//		XRenderChangePicture(Application.Handle, src->Canvas->Picture, valuemask, &pa);

//		XTransform transform = {{{ 1, 0, 0 },
//								 {0, 1, 0 },
//								 {0, 0, 2}}};
//			printf("CURSOR: %d\n", XDoubleToFixed(1.));
//		XRenderSetPictureTransform (Application.Handle, pic, &transform);

		XRenderFreePicture(Application.Handle, pic1);
		XRenderFreePicture(Application.Handle, pic2);
		XRenderFreePicture(Application.Handle, pic3);


/*

#define CPRepeat			    (1 << 0)
#define CPAlphaMap			    (1 << 1)
#define CPAlphaXOrigin			    (1 << 2)
#define CPAlphaYOrigin			    (1 << 3)
#define CPClipXOrigin			    (1 << 4)
#define CPClipYOrigin			    (1 << 5)
#define CPClipMask			    (1 << 6)
#define CPGraphicsExposure		    (1 << 7)
#define CPSubwindowMode			    (1 << 8)
#define CPPolyEdge			    (1 << 9)
#define CPPolyMode			    (1 << 10)
#define CPDither			    (1 << 11)
#define CPComponentAlpha		    (1 << 12)
#define CPLastBit			    12

typedef struct _XRenderPictureAttributes {
    int 		repeat;
    Picture		alpha_map;
    int			alpha_x_origin;
    int			alpha_y_origin;
    int			clip_x_origin;
    int			clip_y_origin;
    Pixmap		clip_mask;
    Bool		graphics_exposures;    ignored
    int			subwindow_mode;
    int			poly_edge;
    int			poly_mode;
    Atom		dither;					ignored
    Bool		component_alpha;
} XRenderPictureAttributes;

#define PictTypeIndexed			    0
#define PictTypeDirect			    1

#define PictOpMinimum			    0
#define PictOpClear			    0
#define PictOpSrc			    1
#define PictOpDst			    2
#define PictOpOver			    3
#define PictOpOverReverse		    4
#define PictOpIn			    5
#define PictOpInReverse			    6
#define PictOpOut			    7
#define PictOpOutReverse		    8
#define PictOpAtop			    9
#define PictOpAtopReverse		    10
#define PictOpXor			    11
#define PictOpAdd			    12
#define PictOpSaturate			    13
#define PictOpMaximum			    13

#define PictOpDisjointMinimum			    0x10
#define PictOpDisjointClear			    0x10
#define PictOpDisjointSrc			    0x11
#define PictOpDisjointDst			    0x12
#define PictOpDisjointOver			    0x13
#define PictOpDisjointOverReverse		    0x14
#define PictOpDisjointIn			    0x15
#define PictOpDisjointInReverse			    0x16
#define PictOpDisjointOut			    0x17
#define PictOpDisjointOutReverse		    0x18
#define PictOpDisjointAtop			    0x19
#define PictOpDisjointAtopReverse		    0x1a
#define PictOpDisjointXor			    0x1b
#define PictOpDisjointMaximum			    0x1b

#define PictOpConjointMinimum			    0x20
#define PictOpConjointClear			    0x20
#define PictOpConjointSrc			    0x21
#define PictOpConjointDst			    0x22
#define PictOpConjointOver			    0x23
#define PictOpConjointOverReverse		    0x24
#define PictOpConjointIn			    0x25
#define PictOpConjointInReverse			    0x26
#define PictOpConjointOut			    0x27
#define PictOpConjointOutReverse		    0x28
#define PictOpConjointAtop			    0x29
#define PictOpConjointAtopReverse		    0x2a
#define PictOpConjointXor			    0x2b
#define PictOpConjointMaximum			    0x2b

#define PictOpBlendMinimum			    0x30
#define PictOpMultiply				    0x30
#define PictOpScreen				    0x31
#define PictOpOverlay				    0x32
#define PictOpDarken				    0x33
#define PictOpLighten				    0x34
#define PictOpColorDodge			    0x35
#define PictOpColorBurn				    0x36
#define PictOpHardLight				    0x37
#define PictOpSoftLight				    0x38
#define PictOpDifference			    0x39
#define PictOpExclusion				    0x3a
#define PictOpHSLHue				    0x3b
#define PictOpHSLSaturation			    0x3c
#define PictOpHSLColor				    0x3d
#define PictOpHSLLuminosity			    0x3e
#define PictOpBlendMaximum			    0x3e

#define PolyEdgeSharp			    0
#define PolyEdgeSmooth			    1

#define PolyModePrecise			    0
#define PolyModeImprecise		    1

#define FilterNearest			    "nearest"
#define FilterBilinear			    "bilinear"
#define FilterConvolution		    "convolution"

#define FilterFast			    "fast"
#define FilterGood			    "good"
#define FilterBest			    "best"

#define FilterAliasNone			    -1

#define SubPixelUnknown			    0
#define SubPixelHorizontalRGB		    1
#define SubPixelHorizontalBGR		    2
#define SubPixelVerticalRGB		    3
#define SubPixelVerticalBGR		    4
#define SubPixelNone			    5

#define RepeatNone                          0
#define RepeatNormal                        1
#define RepeatPad                           2
#define RepeatReflect                       3

*/

    Image1.Update();
/*
for(int d = 0; d <= 255; d ++)
{
	Blend(*(TBitmap *)(*Image1.Picture), xpmimg, *(TBitmap *)(*Image1.Picture), d);
//	Blend(xbmimg, xpmimg, out, d);
//	Canvas->Draw(20,20,out);
//	Canvas->StretchDraw(rect, out);
	Image1.Update();
	Application.ProcessMessages();
}
for(int d = 255; d >= 0; d--)
{
	Blend(xbmimg, *(TBitmap *)(*Image1.Picture), *(TBitmap *)(*Image1.Picture), d);
//	Blend(xbmimg, xpmimg, out, d);
//	Canvas->Draw(20,20,out);
//	Canvas->StretchDraw(rect, out);
	Image1.Update();
	Application.ProcessMessages();
}
*/
//	out.Assign(xbmimg);
//Turn(xbmimg, out);
//AntiAlias(xbmimg, TRect(0,0,xbmimg.Width,xbmimg.Height));
//FishEye(xbmimg, xpmimg, 1);
//Marble(xbmimg, xpmimg, 10, 200, 150);
//ShadowDownRight(xbmimg);
//Canvas->Draw(10,20,&xpmimg);
//	xbmimg.ResizeMode = rmStretch;
//	xbmimg.ResizeMode = rmCrop; , clFuchsia
//	xbmimg.TransparentColor = clFuchsia;
//	printf("ENDTRANSPARENT %ld  %ld\n", clock() - cl1, CLOCKS_PER_SEC);
//	xbmimg.SetSize(696, 696);
//	printf("ENDSIZE %ld  %ld\n", clock() - cl1, CLOCKS_PER_SEC);
//	xbmimg.Rotate(45);
//	xbmimg.TransparentColor = clBlack;
//	printf("ENDROTATE %ld  %ld\n", clock() - cl1, CLOCKS_PER_SEC);

//tmpimg.Width = 1000;
//tmpimg.Height = 800;
//xbmimg.HandleType = bmDIB;
/*   		printf("\nwidth            = %d\n", xbmimg.Data->width);
  printf("height           = %d\n", xbmimg.Data->height);
  printf("xoffset          = %d\n", xbmimg.Data->xoffset);
  printf("format           = %d\n", xbmimg.Data->format);
  printf("data             = %p\n", xbmimg.Data->data);
  printf("byte_order       = %d\n", xbmimg.Data->byte_order);
  printf("bitmap_unit      = %d\n", xbmimg.Data->bitmap_unit);
  printf("bitmap_bit_order = %d\n", xbmimg.Data->bitmap_bit_order);
  printf("bitmap_pad       = %d\n", xbmimg.Data->bitmap_pad);
  printf("depth            = %d\n", xbmimg.Data->depth);
  printf("bytes_per_line   = %d\n", xbmimg.Data->bytes_per_line);
  printf("bits_per_pixel   = %d\n", xbmimg.Data->bits_per_pixel);

   		printf("\nwidth            = %d\n", xpmimg.Data->width);
  printf("height           = %d\n", xpmimg.Data->height); */
/*
for(int i = 0; i <= 0x99; i++)
{
	Marble(xbmimg, xpmimg, 10, 200, i);
	Canvas->Draw(10,20,&xpmimg);
	sleep(1);
} */
//	Canvas->Draw(TRect(20,20, 300, 300),&xbmimg);
/*
clock_t cl2 = clock();
printf("TIMEOUTPUT %ld  %ld\n", cl2 - cl1, CLOCKS_PER_SEC);

	xpmimg.Save("../../images/about.bmp");

	TStrings sl;
	sl.Count = 3;
	sl[0] = "проверка";
	sl[1] = TString("bdhggy_") + TString(77) + "sdnas";
	sl[2] = "bncxsj";
	Canvas->TextLine(100, 300, 30, sl);

		int d = GetDesktop(m_Handle);
		int d1 = GetDesktopCount();
	HWND t = GetActiveWindow();
	printf("GETACTIVEWINDOW %x %x %x %d %d\n", t, m_Handle, m_FrameHandle, d, d1);
//	SetActiveWindow(SecondForm->Handle);
*/
//	SetDesktopCount(4);
/*
int op[] = { GXclear,
GXand,
GXandReverse,
GXcopy,
GXandInverted,
GXnoop,
GXxor,
GXor,
GXnor,
GXequiv,
GXinvert,
GXorReverse,
GXcopyInverted,
GXorInverted,
GXnand,
GXset

}; */
//static int i;
//	TBitmap xpmimg;
//	xpmimg.Load("../../images/splash.bmp");
//	xpmimg.Mask(clWhite, 20);
//	xpmimg.TransparentColor = clWhite;
//	rect = TRect(50,50,xpmimg.Width,xpmimg.Height);

//	Canvas->CopyMode = TCopyMode(op[i++]);
//	Canvas->CopyRect(rect, xpmimg.Canvas, TRect(0,0,xpmimg.Width,xpmimg.Height));
//	Canvas->CopyMode = TCopyMode(GXcopy);
//    ShadowDownRight(xpmimg);
//	Canvas->Draw(50,50,xpmimg);

//	SecondForm->RemoveShape();

}
//---------------------------------------------------------------------------
void TMainForm :: DispatchEvent(XEvent &event, bool &handled)
{
//	HWND wnd = event.xany.window;
		static const char *events[] = {
		"",
		"",
		"KeyPress",
		"KeyRelease",
		"ButtonPress",
		"ButtonRelease",
		"MotionNotify",
		"EnterNotify",
		"LeaveNotify",
		"FocusIn",
		"FocusOut",
		"KeymapNotify",
		"Expose",
		"GraphicsExpose",
		"NoExpose",
		"VisibilityNotify",
		"CreateNotify",
		"DestroyNotify",
		"UnmapNotify",
		"MapNotify",
		"MapRequest",
		"ReparentNotify",
		"ConfigureNotify",
		"ConfigureRequest",
		"GravityNotify",
		"ResizeRequest",
		"CirculateNotify",
		"CirculateRequest",
		"PropertyNotify",
		"SelectionClear",
		"SelectionRequest",
		"SelectionNotify",
		"ColormapNotify",
		"ClientMessage",
		"MappingNotify",
		"Unknown"
	};
//	if(event.type > 34)
//        event.type = 35;
    printf("DISPATCH %s %lu %lx\n", events[event.type], event.xany.serial, event.xany.window);
}
//---------------------------------------------------------------------------
void TMainForm :: MyIdleHandler(TObject *Sender, bool &Done)
{
	printf("APPIDLE\n");
  	Done = false;
}
//---------------------------------------------------------------------------
void TMainForm :: ActivateApp(TObject *object)
{
	printf("---------------ActivateApp: %d\n",object->id);
}
//---------------------------------------------------------------------------
void TMainForm :: DeActivateApp(TObject *object)
{
	printf("--------------DeActivateApp: %d\n",object->id);
//	printf("SIGNAL NAME %s %d\n", (*Signal.Name).c_str(), (int)Signal.Handler);
}
//---------------------------------------------------------------------------
void TMainForm :: TestSignal(TSignal *obj, siginfo_t *info, void *context)
{
	if(obj)
		printf("ОБРАБОТКА СИГНАЛА %d %s\n", *obj->Signal, (*obj->Name).c_str());
	else
		printf("APP ОБРАБОТКА СИГНАЛА %d %s\n", info->si_signo, sys_siglist[info->si_signo]);
//	exit(1);
}
//---------------------------------------------------------------------------
void TMainForm :: DoKeyUp(TObject *object, uint_t &key, TShiftState ss)
{
	printf("-------DoKeyUp: id=%d key=%u, state=%u\n",object->id, key, ss);
}
//---------------------------------------------------------------------------
void TMainForm :: DoKeyDown(TObject *object, uint_t &key, TShiftState ss)
{
    TShortCut sc;
    TString s;

	printf("-------DoKeyDown: id=%d key=%u, state=%u\n",object->id, key, ss);

        sc = ShortCut(key, ss);
        printf("ShortCut = %X\n", sc);
        if(ss & ssLeft) 	printf("MouseLeft\n");
        if(ss & ssRight) 	printf("MouseRight\n");
        if(ss & ssMiddle) 	printf("MouseMiddle\n");
        if(ss & Button4Mask) 	printf("Button4\n");
        if(ss & Button5Mask) 	printf("Button5\n");
        if(ss & ssShift) 	printf("Shift\n");
        if(ss & ssCapsLock) 	printf("CapsLock\n");
        if(ss & ControlMask) 	printf("Control\n");
        if(ss & ssAlt) 	printf("Alt\n");
        if(ss & ssNumLock) 	printf("NumLock\n");
        if(ss & Mod3Mask) 	printf("Mod3\n");
        if(ss & ssWin) 	printf("Win\n");
        if(ss & Mod5Mask) 	printf("Mod5\n");
        if(ss & ssScrollLock) 	printf("ScrollLock\n");
        if(ss &  	(1<<14)) 	printf("u2\n");
        if(ss &  	(1<<15)) 	printf("u3\n");

        s = ShortCutToText(sc);
        printf("SHORTCUT1 %s\n", s.c_str());
        sc = TextToShortCut(s);
        printf("SHORTCUT %X\n", sc);
        s = ShortCutToText(sc);
        printf("SHORTCUT2 %s\n", s.c_str());
}
//---------------------------------------------------------------------------
void TMainForm :: DoKeyPress(TObject *object, char &key)
{
	printf("-------DoKeyPress: id=%d key=%X\n",object->id, key);
}
//---------------------------------------------------------------------------
