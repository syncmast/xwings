//---------------------------------------------------------------------------

#ifndef ProcessMsgH
#define ProcessMsgH
//---------------------------------------------------------------------------
#include <xwings.h>
#include <stdctrls.h>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
    DECLARE_DYNAMIC_CLASS( TForm1 );

private:	// User declarations
public:		// User declarations
   TButton AButton1;
   TButton AButton2;
   void __fastcall Button1Click(TObject *Sender);
   void __fastcall Button2Click(TObject *Sender);
   TForm1();
};
//---------------------------------------------------------------------------
extern TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
