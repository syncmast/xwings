//---------------------------------------------------------------------
#include "second.h"
//---------------------------------------------------------------------
TForm2 *Form2;
//---------------------------------------------------------------------
TForm2::TForm2()
  : TForm()
{
    id = 60;
    BoundsRect = TRect(0,0,800,500);
	Caption = "Second Form";

    AButton1.Parent = this;
    AButton1.Caption = "Close";
    AButton1.Align = alCenter;
    AButton1.OnClick = TNotifyEvent(this, &TForm2::Button1Click);
}
//---------------------------------------------------------------------
void TForm2::Button1Click(TObject* Sender)
{
    Close();
}
//----------------------------------------------------------------------------
