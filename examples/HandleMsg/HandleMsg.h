//---------------------------------------------------------------------------

#ifndef HandleMsgH
#define HandleMsgH
//---------------------------------------------------------------------------
#include "xwings.h"
#include "stdctrls.h"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
    DECLARE_DYNAMIC_CLASS( TForm1 );

private:
public:
   TButton Button1;
   TButton Button2;
   void FormCreate(TObject *Sender);
   void Button1Click(TObject *Sender);
   void Button2Click(TObject *Sender);
   TForm1();
   void MyIdleHandler(TObject *Sender, bool &Done);

};
//---------------------------------------------------------------------------
extern TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
