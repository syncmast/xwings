//---------------------------------------------------------------------------
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include <stdio.h>
#include <errno.h>
#include <locale.h>
#include <stddef.h>
#include <sys/types.h>
#include <ctype.h>
#include "TestForm.h"
#include "extctrls.h"
#include "lnxfile.h"
#include "imgfmt.h"
#include "xexts.h"
#include "lnxfile.h"
#include "sysutils.h"
#include "rtti.h"
#include "grutils.h"
#include "xstream.h"
#include "netwm.h"
#include "xwings.h"
#include <cups/cups.h>
//#include <X11/extensions/Print.h>
#pragma pack(push,1)
typedef struct tagBITMAPFILEHEADER {
	word_t	bfType;		//тип файла (для битового образа - BM)
	dword_t	bfSize;		//размер файла в dword
	word_t	bfReserved1;
	word_t	bfReserved2;
 	dword_t	bfOffbits;	//смещение данных битового образа от начала файла в байтах
} BITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER {
	dword_t	biSize;		//число байт, занимаемых структурой BITMAPINFOHEADER
 	dword_t	biWidth;	//ширина битового образа в пикселах
 	dword_t	biHeight;	//высота битового образа в пикселах
	word_t	biPlanes;	//число битовых плоскостей устройства
	word_t	biBitCount;	//число битов на пиксель
	dword_t	biCompression;	//тип сжатия
	dword_t	biSizeImage;	//размер картинки в байтах
	dword_t	biXPelsPerMeter;//горизонтальное разрешение устройства, пиксел/м
	dword_t	biYPelPerMeter;	//вертикальное разрешение устройства, пиксел/м
	dword_t	biClrUsed;	//число используемых цветов
	dword_t	biClrImportant; //число "важных" цветов
} BITMAPINFOHEADER;
#pragma pack(pop)
#include "../../images/about.res"
//---------------------------------------------------------------------------
TForm *SecondForm;
TLog AppLog;

int main(int argc, char* argv[])
{
    int i;
  cups_dest_t *dests, *dest;
  int num_dests = cupsGetDests(&dests);

  for (i = num_dests, dest = dests; i > 0; i --, dest ++)
  {
    if (dest->instance)
      printf("%s/%s\n", dest->name, dest->instance);
    else
      puts(dest->name);
  }

  const char *value;

    for (i = num_dests, dest = dests; i > 0; i --, dest ++)
    if (dest->instance == NULL)
    {
        value = cupsGetOption("printer-info", dest->num_options, dest->options);
        printf("%s (%s)\n", dest->name, value ? value : "no description");

        value = cupsGetOption("auth-info-required", dest->num_options, dest->options);
        printf("%s (%s)\n", dest->name, value ? value : "no description");

        value = cupsGetOption("printer-is-accepting-jobs", dest->num_options, dest->options);
        printf("%s (%s)\n", dest->name, value ? value : "no description");
    }
/*
"printer-is-shared"	"true" if the destination is being shared with other computers, "false" if not.
"printer-location"	The human-readable location of the destination such as "Lab 4".
"printer-make-and-model"	The human-readable make and model of the destination such as "HP LaserJet 4000 Series".
"printer-state"	"3" if the destination is idle, "4" if the destination is printing a job, and "5" if the destination is stopped.
"printer-state-change-time"	The UNIX time when the destination entered the current state.
"printer-state-reasons"	Additional comma-delimited state keywords for the destination such as "media-tray-empty-error" and "toner-low-warning".
"printer-type"
 */
    cupsFreeDests(num_dests, dests);

printf("%s : %s\n", program_invocation_name, program_invocation_short_name);

	AppLog.LogName = ChangeFileExt(Application.ExeName, ".log");
	AppLog.printf("%s : PAGESIZE %d\n", Now().c_str(), getpagesize());
	TDirectory dir;
	AppLog.printf("CURRENT DIR %s\n", (*dir.CurrentDir).c_str());
	TString ds = dir.CurrentDir;
	dir.Open(ds);
	AppLog.printf("CURRENT DIR NEXT %s\n", dir.Next().c_str());
	dir.Close();

printf("ULONG %d, UINT %d TCOLOR %d\n", sizeof(unsigned long), sizeof(unsigned int), sizeof(TColor));

	TFile file("../../bin/098765.txt");
//	file.Name = "../../images/access4.bmp";
	AppLog.printf("%s %d %d %d\n", (*file.Name).c_str(), *file.Size, *file.SizeOnDisk, *file.Type);
	AppLog.printf("ACCESSTIME %s\n", TimeToString(file.AccessTime).c_str());
	AppLog.printf("CHANGETIME %s\n", TimeToString(file.ChangeTime).c_str());
	AppLog.printf("MODIFYTIME %s\n", TimeToString(file.ModifyTime).c_str());
//	TLabel *ltmp = new TLabel[10];
//	ltmp[0].Caption = "TEST";
//	AppLog.printf("TESTLABEL0 %s\n", (*ltmp[0].Caption).c_str());
//	delete [] ltmp;


/*
   char *printServerName = ":64";//":1";
    XPPrinterList plist;
//    XPContext pcontext;
    int plistCnt = 0;
	HDISPLAY PrintHandle = XOpenDisplay(printServerName);

   plist =  XpGetPrinterList(PrintHandle, NULL, &plistCnt );
   printf("COUNT PRINTERS: %d\n",plistCnt);
   for(int i = 0 ; i < plistCnt; i++)
   {
      printf("PRINTER NAME: %s\n",plist[i].name);
      printf("PRINTER DESC: %s\n",plist[i].desc);
   }
   XpFreePrinterList( plist );
	XCloseDisplay(PrintHandle);
*/

XFontSet fontset;
char **missing_charsets;
int num_missing_charsets = 0;
char *default_string;
char *program_name = "TEST FONTSET";

fontset = XCreateFontSet(Application.Handle,
                         "-misc-fixed-*-*-*-*-*-130-75-75-*-*-*-*",
                         &missing_charsets, &num_missing_charsets,
                         &default_string);
/*
 * if there are charsets for which no fonts can
 * be found, print a warning message.
 */
if (num_missing_charsets > 0) {
    fprintf(stderr, "%s: The following charsets are missing:\n", program_name);
    for(int i=0; i < num_missing_charsets; i++)
        fprintf(stderr, "%s: %s\n", program_name, missing_charsets[i]);
    fprintf(stderr, "%s: The string %s will be used in place\n", program_name, default_string);
    fprintf(stderr, "%s: of any characters from those sets.\n", program_name);
    XFreeStringList(missing_charsets);
}

program_name = XBaseFontNameListOfFontSet(fontset);
printf("BASEFONTNAME %s\n",program_name);
program_name = XLocaleOfFontSet(fontset);
printf("LOCALE %s\n",program_name);

XFreeFontSet(Application.Handle, fontset);


    TFileStream filestr("../../images/two.xbm", fstream::in | fstream::out);

    TMemoryStream ss( about, sizeof(about) );
    cout << "sizeof(about) = " << sizeof(about) << endl;

    TStringStream strstr;
    TString str10 = "Hello, this i";
    TString sss;
    strstr.WriteString(str10);
    cout << str10 << endl;
    sss = strstr.ReadString();
    cout << sss << endl;

  string sval;
  int val, len1, len2;
  len1 = strstr.Size;
  len2 = ss.Size;
  strstr << "120 42 377 6 5 2000";


  for (int n=0; n<6; n++)
  {
//    filestr >> sval;
    strstr >> sval;
//    cout << val << endl;
    cout << sval << endl;
  }

	BITMAPFILEHEADER finfo;
	BITMAPINFOHEADER hinfo;
	ss.Read(&finfo, sizeof(BITMAPFILEHEADER));
//	ss.read((char *)&finfo.bfType, sizeof(finfo.bfType));
//	ss.read((char *)&finfo.bfSize, sizeof(finfo.bfSize));
//	ss.read((char *)&finfo.bfReserved1, sizeof(finfo.bfReserved1));
//	ss.read((char *)&finfo.bfReserved2, sizeof(finfo.bfReserved2));
//	ss.read((char *)&finfo.bfOffbits, sizeof(finfo.bfOffbits));

	ss.Read(&hinfo, sizeof(BITMAPINFOHEADER));
	if(finfo.bfType == 0x4D42)
	{
		fprintf(stderr, "This BMP %u %u %u %u %d %d\n", finfo.bfSize, finfo.bfOffbits, hinfo.biWidth, hinfo.biHeight, len1, len2 );
//		exit(0);
	}

//int i;
  filestr.close();

clock_t cl1 = clock();
TStrings strlst;
GetListExtensions(strlst);

	AppLog.printf("+++++++++++++++++++++++++\n");
    for(int i=0; i < strlst.Count; i++)
        AppLog.printf("%s\n", strlst[i].c_str());
	AppLog.printf("+++++++++++++++++++++++++\n");
	strlst.Clear();


	TRandrExtension randr;
	int major, minor;
	randr.GetVersion(&major, &minor);
	AppLog.printf("Version RANDR %d.%d\n", major, minor);

	for(int i=0; i < randr.SizeCount; i++)
		AppLog.printf("SCREEN: %d %d %d %d\n", (*randr.ScreenSizes[i]).width, (*randr.ScreenSizes[i]).height,
										(*randr.ScreenSizes[i]).mwidth, (*randr.ScreenSizes[i]).mheight);

//	randr.Apply(3, 0);

	AppLog.printf("SERVER VENDOR %s\n",ServerVendor(*Application.Handle));
	AppLog.printf("VENDOR RELEASE %d\n",VendorRelease(*Application.Handle));
	AppLog.printf("PROTOCOL VERSION %d\n",ProtocolVersion(*Application.Handle));
	AppLog.printf("PROTOCOL REVISION %d\n",ProtocolRevision(*Application.Handle));
	AppLog.printf("SCREEN: Xdpi=%d Ydpi=%d\n",*xwings::Screen.Xdpi,*xwings::Screen.Ydpi);
	AppLog.printf("SCREEN: wMM=%d hMM=%d\n",*xwings::Screen.WidthMM,*xwings::Screen.HeightMM);
	AppLog.printf("SCREEN: w=%d h=%d\n",*xwings::Screen.Width,*xwings::Screen.Height);
	AppLog.printf("LOCALE: %s\n",(char*)Application.Locale[LC_ALL]);
	long l = XExtendedMaxRequestSize(Application.Handle);
	AppLog.printf("++++XExtendedMaxRequestSize %ld\n", l);
	AppLog.printf("DISPLAY %s\n", DisplayString(*Application.Handle));
	AppLog.printf("CONNECTION NUMBER %d\n", *Application.fd);
	AppLog.printf("DISPLAY PLANES %d\n", DisplayPlanes(*Application.Handle, 0));
	AppLog.printf("BLACK PIXEL %ld\n", BlackPixel(*Application.Handle, 0));
	AppLog.printf("WHITE PIXEL %lX\n", WhitePixel(*Application.Handle, 0));
	AppLog.printf("CELLS %d\n", CellsOfScreen(xwings::Screen.Handle));

int *depths;
int count_return;
depths = XListDepths(Application.Handle, 0, &count_return);
for(register int i = 0; i < count_return; i++)
	AppLog.printf("depth %d  %d\n", i, depths[i]);
XFree(depths);

	Keyboard.AutoRepeat = true;

int min_keycodes_return, max_keycodes_return;
XDisplayKeycodes(Application.Handle, &min_keycodes_return, &max_keycodes_return);
AppLog.printf("KEYCODES MIN %d  MAX %d\n",min_keycodes_return, max_keycodes_return);

KeySym *ks;
int keysyms_per_keycode_return;
ks = XGetKeyboardMapping(Application.Handle, min_keycodes_return, max_keycodes_return - min_keycodes_return + 1,
                            &keysyms_per_keycode_return);
AppLog.printf("keysyms_per_keycode %d\n",keysyms_per_keycode_return);

XFree(ks);

AppLog.printf("SCREENRESOURSE %s\n",XScreenResourceString(xwings::Screen.Handle));
AppLog.printf("XDEFAULTSTRING %s\n",XDefaultString());
AppLog.printf("XDISPLAYNAME %s\n",XDisplayName(NULL));

AppLog.printf("MOUSE ACCEL %d %d %d\n", (*Mouse.Accelerator).Numerator,
								 (*Mouse.Accelerator).Denominator,
								 (*Mouse.Accelerator).Threshold);
/*
FILE *f;
f = fopen("./ListFonts.txt", "w");
TStrings strlst;
GetFontPath(strlst);

	fprintf(f, "FONT PATH:\n");
    for(i=0; i < strlst.Count; i++)
        fprintf(f, "%s\n", strlst[i].c_str());

	strlst.Clear();

	SelectFontName("*", 15000, strlst);
 fprintf(f, "FONT COUNT: %d\n", (int)strlst.Count);

	TStrings weight, slant, setweight, tmps, facename, family, fonttype;
	tmps.Delimiter = '-';

 strlst.Sort();
 strlst.Unique();
 TFont font;
    for(i=0; i < strlst.Count; i++)
    {
        tmps.DelimitedText = strlst[i];
        fprintf(f, "%s\n", strlst[i].c_str());
        font.FullName = strlst[i];
        facename.Add(font.FaceName);
        fonttype.Add(font.FontType);
       	if(tmps.Count > 3)
       	{
       		family.Add(tmps[2]);
        	weight.Add(tmps[3]);
        	slant.Add(tmps[4]);
        	setweight.Add(tmps[5]);
        }
    }
    	fprintf(f, "FONTS: %d\n", *strlst.Count);
 weight.Sort();
 weight.Unique();
     for(i=0; i < weight.Count; i++)
        fprintf(f, "%s\n", weight[i].c_str());

 slant.Sort();
 slant.Unique();
      for(i=0; i < slant.Count; i++)
        fprintf(f, "%s\n", slant[i].c_str());

 setweight.Sort();
 setweight.Unique();
      for(i=0; i < setweight.Count; i++)
        fprintf(f, "%s\n", setweight[i].c_str());

 fprintf(f, "FAMILY===================\n");
 family.Sort();
 family.Unique();
      for(i=0; i < family.Count; i++)
        fprintf(f, "%s\n", family[i].c_str());

 fprintf(f, "FACENAME===================\n");
  facename.Sort();
 facename.Unique();
      for(i=0; i < facename.Count; i++)
        fprintf(f, "%s\n", facename[i].c_str());

 fprintf(f, "FONTTYPE===================\n");
  fonttype.Sort();
 fonttype.Unique();
 fonttype.Pack();
      for(i=0; i < fonttype.Count; i++)
        fprintf(f, "%s\n", fonttype[i].c_str());

 fprintf(f, "ENDFONTLIST %ld  %ld\n", clock() - cl1, CLOCKS_PER_SEC);
	fclose(f);
*/

   TList lst, lst1;
   lst.Count = 5;
   printf("Count = %d %d\n", (int)lst.Count, (int)lst1.Count);
   for(int i = 0 ; i < lst.Count; i++)
      lst.Items[i] = (void *)i;
   lst.Exchange(0,4);
   lst.Move(2,3);
   void *p;
   lst[1] = (void *)99;
   lst.Items[2] = (void *)98;
   lst1 = lst;

   printf("Count = %d %d\n", (int)lst.Count, (int)lst1.Count);

   for(int j=0; j < lst1.Count; j++)
   {
      p = lst1.Items[j];
//      p = lst1[j];
      printf("Z = %ld\n", long(p));
   }
   AppLog.printf("n = %d\n", (int)'\n');
   AppLog.printf("r = %d\n", (int)'\r');
   AppLog.printf("t = %d\n", (int)'\t');
   AppLog.printf("b = %d\n", (int)'\b');

   TStrings slst;
   TString ws;

   slst.DelimitedText = "str1,str2,  val  = str3";
   slst.Exchange(0,2);
   AppLog.printf("count = %d %d\n", *slst.Count, slst.IndexOfName("val"));
   ws = slst.Text;
   AppLog.printf("txt = %s\n", ws.c_str());
   ws = slst.DelimitedText;
   AppLog.printf("dxt = %s\n", ws.c_str());
   for(int j=0; j < slst.Count; j++)
   {
 //     p = lst.Items[j];
      ws = slst[j];
      AppLog.printf("S = %s\n", ws.c_str());
   }

	RegisterClass(TMainForm);
	Application.CreateForm(__classid(TMainForm), &MainForm1);

//	MainForm1 = new TMainForm();

	const char *s = "My application Мое приложение";
	AppLog.printf("%s\n",s);
	Application.Title = s;
   ws = Application.Title;
	AppLog.printf("Title = %s\n", ws.c_str());

	ws = Application.ExeName;

	MainForm1->Test[10][20] = 99;
	AppLog.printf("SET TEST = %d\n", *MainForm1->Test[10][20]);

	AppLog.printf("EXE = %s\n", ws.c_str());
	TForm *s3 = (TForm *)TClass(__classid(TForm)).Create();
	s3->BoundsRect = TRect(0,0, 600, 400);

//printf("======DESKTOP COUNTS %d\n", *CScreen.ActiveDesktop->ControlCount);

	s3->id = 888;
/*	XWMHints *wmh = XAllocWMHints();
wmh->flags = WindowGroupHint;
wmh->window_group = mf.Handle;
XSetWMHints(Application.Handle, s3->Handle, wmh);
XFree(wmh);

unsigned int value_mask = CWSibling | CWStackMode;
      XWindowChanges values;
      values.sibling = mf.Handle;
      values.stack_mode = Below;

XConfigureWindow(Application.Handle, s3->Handle, value_mask, &values);
printf("BAD = %lx %lx\n", *s3->Handle, *mf.Handle);
*/

//XBell(Application.Handle, 90);

	TBMPImage xbmimg;
	xbmimg.Load("../../images/about.bmp");
	xbmimg.PixelFormat = TPixelFormat();

//	Convert2Res("../../images/about.bmp", "../../images/about.res", "about");

	s3->Bitmap = &xbmimg;
	xbmimg.Load("../../images/a.bmp");
	xbmimg.Mask(clFuchsia);
	s3->Caption = "Third FORM : Третья ФОРМА\n";
//	s3->WindowState = TWindowState() << wsSkipTaskbar  << wsShaded;
	TBitmap xpmimg;
	xpmimg.Load("../../images/two.xbm");
	s3->SetShape(xpmimg);

	s3->Visible = true;
//	s3->WindowState = TWindowState() << wsSkipTaskbar  << wsShaded;
	s3->Align = alCenter;
	s3->Icon = &xbmimg;
	s3->BringToFront();
	SecondForm = s3;
	Application.Icon = &xbmimg;

//	TLabel lbl(s3);
//	lbl.id = 101;
//	lbl.Left = 100;
//	lbl.Top = 50;
//	lbl.Font->Color = clWhite;
	TWString wp = "   -92.56   ";
	TString tmpstr(wp);
//	lbl.Caption = *(s3->Caption) + ws.printf("TEST PRINTF %d", 777).LowerCase();
	TWideString wstr;
	wstr = wstr.printf(L"DDDD %d\n", 546).LowerCase();
//	AppLog.printf("SS %ls %s %s\n", wstr.c_str(), (*lbl.Caption).c_str(), tmpstr.c_str());

	AppLog.printf("SIZECLASS %d %d %d %d\n", sizeof(TControl), sizeof(TWinControl), sizeof(TForm), sizeof(TBitmap));
	AppLog.printf("SIZECLASS %d %d %d %d\n", sizeof(TApplication), sizeof(TSocket), sizeof(TLocalSocket), sizeof(TLabel));

   Application.Run();

	delete s3;

	cl1 = clock();

	for(int i = 0; i < 1000; i++)
	{
		tmpstr = "   -92.56   ";
		tmpstr.Trim();
	}

clock_t cl2 = clock();
AppLog.printf("TIMEOUTPUT %ld  %ld\n", cl2 - cl1, CLOCKS_PER_SEC);
	AppLog.printf("INTVAL #%s#\n", tmpstr.c_str());

	tmpstr = toString<int>(999);
	AppLog.printf("INTVAL $%s$\n", tmpstr.c_str());

	return 0;
}
//---------------------------------------------------------------------------
/*
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <assert.h>
#include <iostream.h>
#include <stdio.h>
#include <unistd.h>

#define SIZEOF(x) (sizeof(x) / (sizeof((x)[0])))

main()
{
      Window w2;

      Display *dpy = XOpenDisplay(NULL);
      assert(dpy);
      Screen *scr = DefaultScreenOfDisplay(dpy);

      // ChangeSaveSet

//        Display *dpy2 = XOpenDisplay(NULL);
//        assert(dpy2);
//        XAddToSaveSet(dpy2, w);
//        XCloseDisplay(dpy2);

      // ConfigureWindow

      Window w3 = XCreateWindow(dpy, w, 10, 50, 100, 10, 2,
			 CopyFromParent, CopyFromParent, CopyFromParent, 0, NULL);

      XMapWindow(dpy, w3);

      XWindowChanges wc;
      wc.x = -5;
      wc.y = -10;
      wc.width = 50;
      wc.height = 40;
      wc.border_width = 7;
      wc.sibling = w2;
      wc.stack_mode = Opposite;
      XConfigureWindow(dpy, w3,
		       CWX | CWY | CWWidth | CWHeight | CWBorderWidth | CWSibling | CWStackMode,
		       &wc);

      // CirculateWindow

      XCirculateSubwindows(dpy, w, RaiseLowest);

      // QueryTree

      Window parent;
      Window *children;
      unsigned nchildren;
      success = XQueryTree(dpy, w, &root, &parent, &children, &nchildren);
      XFree(children);

      // ListProperties

      int num_prop;
      Atom *list = XListProperties(dpy, w, &num_prop);
      XFree(list);

      // SetSelectionOwner

      XSetSelectionOwner(dpy, XA_PRIMARY, w, 12000);
      XSetSelectionOwner(dpy, XA_SECONDARY, w, CurrentTime);

      // GetSelectionOwner

      Window wx = XGetSelectionOwner(dpy, XA_PRIMARY);

      // ConvertSelection

      XConvertSelection(dpy, XA_SECONDARY, XA_CURSOR, XA_POINT, w, CurrentTime);

      // SendEvent

      // GrabPointer

      cerr << "Grabbing" << endl;
      int res = XGrabPointer(dpy, w, False, Button5MotionMask | PointerMotionHintMask,
			     GrabModeSync, GrabModeAsync, w, None, CurrentTime);
      XSync(dpy, False);
//      sleep(5);

      // UngrabPointer

      cerr << "Ungrabbing" << endl;
      XUngrabPointer(dpy, CurrentTime);

      // GrabButton

      XGrabButton(dpy, 3, ShiftMask | ControlMask, w, False, PointerMotionHintMask | Button2MotionMask,
		  GrabModeAsync, GrabModeSync, None, None);

      XGrabButton(dpy, 2, AnyModifier, w, False, PointerMotionHintMask | Button2MotionMask,
		  GrabModeAsync, GrabModeSync, None, None);

      // UngrabButton

      XUngrabButton(dpy, 2, LockMask, w);

      // ChangeActivePointerGrab

      XChangeActivePointerGrab(dpy, ButtonPressMask, None, CurrentTime);

      // GrabKeyboard

      XGrabKeyboard(dpy, w, True, GrabModeSync, GrabModeSync, 12000);

      // UngrabKeyboard

      XUngrabKeyboard(dpy, 13000);

      // GrabKey

      XGrabKey(dpy, XKeysymToKeycode(dpy, XK_Tab), ShiftMask | Mod3Mask, w, True, GrabModeSync,
	       GrabModeSync);

      // UngrabKey

      XUngrabKey(dpy, AnyKey, AnyModifier, w);

      // AllowEvents

      XAllowEvents(dpy, AsyncPointer, 14000);

      // GetMotionEvents

      int nevents;
      XGetMotionEvents(dpy, w, 15000, 16000, &nevents);

      // WarpPointer

      XWarpPointer(dpy, w, w2, 0, 0, 100, 100, 20, 30);

      // SetInputFocus

      XSetInputFocus(dpy,w, RevertToPointerRoot, 17000);
      XSetInputFocus(dpy, PointerRoot, RevertToPointerRoot, 17000);

      // GetInputFocus

      Window focus;
      int revert_to;
      XGetInputFocus(dpy, &focus, &revert_to);

      // QueryKeymap

      char keys_return[32];
      XQueryKeymap(dpy, keys_return);

      // ListFonts

      int actual_count;
      char **fontList = XListFonts(dpy, "*", 100, &actual_count);
      XFree((char *)fontList);

      // ListFontsWithInfo

      int count;
      XFontStruct *info;
      char **names = XListFontsWithInfo(dpy, "*", 100, &count, &info);
      XFreeFontInfo(names, info, count);

      // SetFontPath
      // GetFontPath

      int npaths;
      char **charList = XGetFontPath(dpy, &npaths);

      char **charList2 = new char *[npaths + 1];
      memcpy(charList2, charList, npaths * sizeof(char *));
      charList2[npaths] = charList2[0];

      XSetFontPath(dpy, charList2, npaths + 1);
      XSetFontPath(dpy, charList, npaths); // Reset to some reasonnable value

      XFreeFontPath(charList);
      delete [] charList2;

      // SetClipRectangles

      XRectangle rectangles[] = { { 10, 20, 30, 40 }, { 100, 200, 5, 3 }, { -5, 1, 12, 24 } };
      XSetClipRectangles(dpy, gc, 12, 9, rectangles, SIZEOF(rectangles), Unsorted);

      // PolyLine

      XDrawLines(dpy, w, gc, points, SIZEOF(points), CoordModePrevious);

      // PolySegment

      XSegment segments[] = { { 7, 8, 9, 10 }, { 11, 12, 13, 14 }, { 15, 16, 17, 18 } };
      XDrawSegments(dpy, w, gc, segments, SIZEOF(segments));

      // PolyRectangle

      XDrawRectangles(dpy, w, gc, rectangles, SIZEOF(rectangles));

      // PolyArc

      XArc arcs[] = { { 10, 20, 30, 40, 50, 60 }, { -70, 80, 90, 100, 110, 120 },
		      { 10, 20, 30, 40, 50, -30 } };

      XDrawArcs(dpy, w, gc, arcs, SIZEOF(arcs));

      // FillPoly

      XFillPolygon(dpy, w, gc, points, SIZEOF(points), Convex, CoordModePrevious);

      // PolyFillRectangle

      XFillRectangles(dpy, w, gc, rectangles, SIZEOF(rectangles));

      // PolyFillArc

      XFillArcs(dpy, w, gc, arcs, SIZEOF(arcs));

      // PolyText8

#define STRINGL(x) x, strlen(x)
      XTextItem items[] = { STRINGL("toto"), -3, fs -> fid,
			    STRINGL("titi"), 3, None,
			    STRINGL("tutu"), 0, fs -> fid };
      XDrawText(dpy, w, gc, 10, 10, items, SIZEOF(items));

      XTextItem items2[] = { STRINGL("totox"), -3, fs -> fid,
			    STRINGL("titi"), 3, None,
			    STRINGL("tutu"), 0, fs -> fid };
      XDrawText(dpy, w, gc, 10, 10, items2, SIZEOF(items2));

      // CreateColormap
      // Don't forget to tell the kids how it was when we had only 8 bits per pixel.

      Colormap colormap = XCreateColormap(dpy, w, DefaultVisualOfScreen(scr), None);

      // FreeColormap

      XFreeColormap(dpy, colormap);
      colormap = XCreateColormap(dpy, w, DefaultVisualOfScreen(scr), None);

      // CopyColormapAndFree

      Colormap colormap2 = XCopyColormapAndFree(dpy, colormap);

      // InstallColormap

      XInstallColormap(dpy, colormap2);

      // UninstallColormap

      XUninstallColormap(dpy, colormap2);

      // ListInstalledColormaps

      int num;
      Colormap *colormapList = XListInstalledColormaps(dpy, w, &num);

      // AllocColor

      XColor screen;
      screen.red = 0;
      screen.green = 32767;
      screen.blue = 65535;
      screen.flags = DoRed | DoGreen | DoBlue;
      success = XAllocColor(dpy, colormap, &screen);

      // AllocNamedColor

      XColor screen2, exact;
      success = XAllocNamedColor(dpy, colormap, "Wheat", &screen2, &exact);

      // AllocColorCells

      unsigned long plane_masks, pixels;
      success = XAllocColorCells(dpy, colormap, False, &plane_masks, 1, &pixels, 1);

      // AllocColorPlanes

      unsigned long rmask, gmask, bmask;
      success = XAllocColorPlanes(dpy, colormap, False, &pixels, 1, 0, 0, 0, &rmask, &gmask, &bmask);

      // FreeColors

      unsigned long pixels2[2] = { screen.pixel, screen2.pixel };
      XFreeColors(dpy, colormap, pixels2, 2, 0);

      // StoreColors

      success = XAllocColorCells(dpy, colormap, False, NULL, 0, pixels2, 2);

      // On many contemporary (that is, year 2000) video cards, you can't allocate read / write cells
      // I want my requests to be sent, however.
      if (!success) {
	    XSetErrorHandler(errorHandler);
      }

      XColor colors[2];
      colors[0] = screen;  colors[0].pixel = pixels2[0];
      colors[1] = screen2; colors[1].pixel = pixels2[1];
      XStoreColors(dpy, colormap, colors, 2);

      // StoreNamedColor

      XStoreNamedColor(dpy, colormap, "Wheat", colors[0].pixel, DoBlue);

      XSync(dpy, False);
      XSetErrorHandler(NULL); // Restore the default handler

      // QueryColors

      screen2.pixel = WhitePixelOfScreen(scr);
      XQueryColor(dpy, colormap, &screen2);

      // LookupColor

      success = XLookupColor(dpy, colormap, "DarkCyan", &exact, &screen);

      // ChangeKeyboardMapping
      // GetKeyboardMapping

      int min_keycodes, max_keycodes;
      XDisplayKeycodes(dpy, &min_keycodes, &max_keycodes);

      int keysyms_per_keycode;
      KeySym *keysyms = XGetKeyboardMapping(dpy, min_keycodes, max_keycodes - min_keycodes + 1,
					    &keysyms_per_keycode);
      XChangeKeyboardMapping(dpy, min_keycodes, keysyms_per_keycode, keysyms,
			     max_keycodes - min_keycodes + 1);

      // ChangeKeyboardControl
      // GetKeyboardControl

      XKeyboardState keyboardState;
      XGetKeyboardControl(dpy, &keyboardState);

      XKeyboardControl keyboardValues;
      keyboardValues.key_click_percent = keyboardState.key_click_percent;
      keyboardValues.bell_percent = keyboardState.bell_percent;
      keyboardValues.bell_pitch = keyboardState.bell_pitch;
      keyboardValues.bell_duration = keyboardState.bell_duration;
      keyboardValues.led = 1;
      keyboardValues.led_mode = LedModeOn;
      keyboardValues.key = min_keycodes;
      keyboardValues.auto_repeat_mode = AutoRepeatModeDefault;
      XChangeKeyboardControl(dpy,
			       KBKeyClickPercent | KBBellPercent | KBBellPitch | KBBellDuration
			     | KBLed | KBLedMode | KBKey | KBAutoRepeatMode,
			     &keyboardValues);

      // ChangePointerControl
      // GetPointerControl

      int accel_numerator, accel_denominator, threshold;
      XGetPointerControl(dpy, &accel_numerator, &accel_denominator, &threshold);

      XChangePointerControl(dpy, True, True, accel_numerator, accel_denominator, threshold);

      // SetScreenSaver
      // GetScreenSaver

      int timeout, interval, prefer_blanking, allow_exposures;
      XGetScreenSaver(dpy, &timeout, &interval, &prefer_blanking, &allow_exposures);
      XSetScreenSaver(dpy, timeout, interval, prefer_blanking, allow_exposures);

      // ChangeHosts
      // ListHosts

      int nhosts;
      Bool state;
      XHostAddress *hostList = XListHosts(dpy, &nhosts, &state);

      XHostAddress host;
      host.family = FamilyInternet;
      host.length = 4;
      host.address = "\001\002\003\004";
      XAddHost(dpy, &host);

      // SetAccessControl

      XSetAccessControl(dpy, EnableAccess);

      // SetCloseDownMode

      XSetCloseDownMode(dpy, RetainTemporary);

      // KillClient

      XKillClient(dpy, AllTemporary);

      // RotateProperties

      Atom properties[] = { XInternAtom(dpy, "CUT_BUFFER0", False),
			    XInternAtom(dpy, "CUT_BUFFER1", False),
			    XInternAtom(dpy, "CUT_BUFFER2", False) };
      XRotateWindowProperties(dpy, RootWindowOfScreen(scr), properties, SIZEOF(properties), -1);

      // ForceScreenSaver

      XForceScreenSaver(dpy, ScreenSaverReset);

      // SetPointerMapping
      // GetPointerMapping

      unsigned char map[64];
      int map_length = XGetPointerMapping(dpy, map, 64);
      XSetPointerMapping(dpy, map, map_length);

      // SetModifierMapping
      // GetModifierMapping

      XModifierKeymap *modmap = XGetModifierMapping(dpy);
      XSetModifierMapping(dpy, modmap);

      // NoOperation

      XNoOp(dpy);

      for(;;) {
	    XEvent e;
	    XNextEvent(dpy, &e);
	    cout << "Got an event of type " << e.type << endl;
      }
}
*/
