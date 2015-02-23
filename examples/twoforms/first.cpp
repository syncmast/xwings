//---------------------------------------------------------------------
#include "first.h"
#include "second.h"
//---------------------------------------------------------------------
TFormMain *FormMain;
//---------------------------------------------------------------------
TFormMain::TFormMain()
  : TForm()
{
    id = 50;
    BoundsRect = TRect(0,0,800,500);
	Align = alCenter;
	Caption = "First Form";

    AButton1.Parent = this;
	AButton1.Caption = "Second";
	AButton1.Align = alCenter;
	AButton1.OnClick = TNotifyEvent(this, &TFormMain::Button1Click);

    SButton1.Parent = this;
	SButton1.Caption = "Second2";
	SButton1.Align = alLeftCenter;
	SButton1.OnClick = TNotifyEvent(this, &TFormMain::Button1Click);
}
//---------------------------------------------------------------------
void TFormMain::Button1Click(TObject* Sender)
{
    Form2 = new TForm2();
    Form2->ShowModal();
    delete Form2;
}
//----------------------------------------------------------------------------

