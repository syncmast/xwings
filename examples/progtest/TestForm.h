//---------------------------------------------------------------------------

#ifndef TestFormH
#define TestFormH
//---------------------------------------------------------------------------
#include "forms.h"
#include "stdctrls.h"
#include "extctrls.h"
#include "socket.h"
#include "menu.h"
#include "grctrls.h"
#include "zoomler.h"
#include "imgfmt.h"
#include "cursor.h"
#include "scrollbar.h"

#define            LOOPCONSTANT     100000
#define            THREADS          10
using namespace xwings;
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
	DECLARE_DYNAMIC_CLASS( TMainForm );
	DECLARE_PROPERTY( TMainForm );

private:	// User declarations
	int get_Test(int x, int y);
	void set_Test(int x, int y, int v);

public:		// User declarations

int                m,j,k,l;

    TLabel Label1;
    TPanel Panel1;
    TScrollbar Scrollbar1;
    TScrollbar Scrollbar2;
    TComboBox Combobox1;
    TEdit Edit1;
	TSpeedButton SpeedButton1;
//	TSpeedButton SpeedButton2;
//	TSpeedButton SpeedButton3;
	TButton AButton1;
	TButton *AButton2;
//	TButton AButton3;
	TTimer Timer;
	TCheckBox CheckBox1;
	TPopupMenu PMenu;
	TPopupMenu PMenu2;
	TPopupMenu PMenu3;
	TMainMenu MMenu;
	TMenuItem ItemsMenu[20];
	TGroupBox GroupBox1;
//	TGroupBox GroupBox2;
	TImageList Images1;
	TImage Image1;
//	TImage Image2;
	TBitmap xbmimg;
//	TGIFImage gifimg;
//	TCustomCursor *Cursor1;
//	TScrollBar ScrollBarV;
//	TScrollBar ScrollBarH;

	TMutex Mutex1;
//	TThread Threads[THREADS];
	TThread SockThread;
	TSignal Signal;

//	TProcess Process1[5];
//	TInetSocket Server1;
//   TMainForm(VWinControl *AParent);
   __fastcall TMainForm();
	~TMainForm();

	void TestShow(TObject *object);
	void TestPaint(TObject *object);
	void SetTest(TObject *object);
	void SetTwoFormTest(TObject *object);
	void SetNETWMTest(TObject *object);
   void SetTestCheck(TObject *object);
	void SetTestResize(TObject *object);
	void SetDblTest(TObject *object);
	void SetTripleTest(TObject *object);
	void SetTestMouse(TObject *Sender, unsigned int button,
    									 unsigned int state, int x, int y);
	void TestMouseMove(TObject *Sender, uint_t state, int x, int y);
   void TestMouseEnter(TObject *Sender, unsigned int state, int x, int y);
   void TestMouseExit(TObject *Sender, unsigned int state, int x, int y);
   void TestStartDrag(TObject *Sender, TObject *&drag, unsigned int state, int x, int y);
	void TestDragOver(TObject* Sender, TObject *Source,
               unsigned int state, int x, int y, TDragState dstate, bool &Accept);
	void TestEndDrag(TObject* Sender, TObject* Source, unsigned int state,int x, int y);
   void SetImageTest(TObject *object);
   void DispatchEvent(XEvent &event, bool &handled);
   void MyIdleHandler(TObject *Sender, bool &Done);
   void ActivateApp(TObject *object);
   void DeActivateApp(TObject *object);
   void TestSignal(TSignal *obj, siginfo_t *info, void *context);
   void SetSockTest(TObject *object);
   void SetProcTest(TObject *object);
   void AcceptSockTest(TSocket *object, TSocket *sock);
   void AcceptSockTest2(TSocket *object, TSocket *sock);
   void DoKeyUp(TObject *object, uint_t &key, TShiftState ss);
   void DoKeyDown(TObject *object, uint_t &key, TShiftState ss);
   void DoKeyPress(TObject *object, char &key);

   property_ij(rw,int,int,int,Test);
};
//---------------------------------------------------------------------------
extern TMainForm *MainForm1;
//---------------------------------------------------------------------------
#endif
