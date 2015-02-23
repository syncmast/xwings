//---------------------------------------------------------------------------
#include "ProcessMsg.h"
//---------------------------------------------------------------------------
TForm1 *Form1;
//---------------------------------------------------------------------------
TForm1::TForm1()
{
    Left = 192;
    Top = 107;
    Width = 696;
    Height = 480;
    Caption = "Form1";
    Color = clBtnFace;
    AButton1.Caption = "Ignore Messages";
    AButton1.Left = 128;
    AButton1.Top = 152;
    AButton1.Caption = "Button1";
    AButton1.OnClick = EVENT(TNotifyEvent, TForm1, Button1Click);
    AButton1.Parent = this;

    AButton2.Caption = "Handle Message";
    AButton2.Left = 448;
    AButton2.Top = 152;
    AButton2.Caption = "Button2";
    AButton2.OnClick = EVENT(TNotifyEvent, TForm1, Button2Click);
    AButton2.Parent = this;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
  int x, y;
  for (int i = 1; i < 10000; i++)
 {
 //   Randomize();
    srand(time(0));
    for (int j = 1; j < 10000; j++)
      y = random()%j;
    x = random()%i;
  }
  Canvas.TextOut(10, 10, "The Button1Click handler is finished");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
  int x, y;
  for (int i = 1; i < 640; i++)
  {
//    Randomize();
    srand(time(0));
    for (int j = 1; j < 640; j++)
    {
      y = random()%j;
      Application.ProcessMessages();
    }
    x = random()%i;
  }
  Canvas.TextOut(10, 30, "The Button2Click handler is finished");

}
//---------------------------------------------------------------------------
