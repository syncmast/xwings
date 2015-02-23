//---------------------------------------------------------------------------
#include "HandleMsg.h"
//---------------------------------------------------------------------------
TForm1 *Form1;

// global variables to show the order of events

int  XPos, YPos, Delta;

// This is a utility procedure to display messages
// add this at the beginning of the unit
void StatusMsg(TForm *Form, TCanvas *Canvas, char *Message, bool Bkg)
{
  if (!Bkg)
    Canvas->Font->Style = fsBold; // foreground messages are bold
  Canvas->TextOut(XPos, YPos, Message);
  if (!Bkg)
    Canvas->Font->Style = fsRegular;
  // change Xpos and YPos to prepare for the next message
  YPos += Delta;
  if(YPos >= Form->ClientHeight)
  {
    YPos = 10;
    XPos += 100;
  }
  if (XPos >= Form->ClientWidth - 100)
  {
    if (Canvas->Font->Color == clRed)
      Canvas->Font->Color = clBlack;
    else
      Canvas->Font->Color = clRed;
    XPos = 10;
  }
}
//---------------------------------------------------------------------------
TForm1::TForm1()
{
/*  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  PixelsPerInch = 96
  TextHeight = 13
*/
    Left = 192;
    Top = 107;
    Width = 696;
    Height = 480;
    Caption = "Form1";
    Color = clBtnFace;
    Button1.Caption = "Do not yield";
    Button1.Left = 128;
    Button1.Top = 152;
    Button1.Width = 100;
    Button1.Parent = this;
    Button1.OnClick = EVENT(TNotifyEvent, TForm1, Button1Click);

    Button2.Caption = "Handle Message";
    Button2.Left = 448;
    Button2.Top = 152;
    Button2.Width = 100;
    Button2.Parent = this;
    Button2.OnClick = EVENT(TNotifyEvent, TForm1, Button2Click);

    Application.OnIdle = EVENT(TIdleEvent, TForm1, MyIdleHandler);

  XPos = 10;
  YPos = 10;
  if (Canvas.Font->Height > 0)
    Delta = Canvas.Font->Height + 1;
  else
    Delta = 1 - Canvas.Font->Height;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
  int X, Y;
  StatusMsg(Form1, &Canvas, "The Button1Click handler is starting", false);
  for (int I = 1; I < 20000; I++)
  {

//    randomize();
    srand(time(0));
    for (int J = 1; J < 20000; J++)
      Y = random()%J;
    X = random()%I;
  }
  StatusMsg(Form1, &Canvas, "The Button1Click handler is done", false);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
  int X, Y;
  StatusMsg(Form1, &Canvas, "The Button2Click handler is starting", false);

  for (int I = 1; I < 500; I++)
  {
//    randomize();
    srand(time(0));
    for (int J = 1; J < 1000; J++)
    {
      Y = random()%J;
      // Yield to OnIdle or other messages
//      Application.HandleMessage();
      Application.ProcessMessages();
    }
    X = random()%I;
  }
  StatusMsg(Form1, &Canvas, "The Button2Click handler is done", false);
}
//---------------------------------------------------------------------------
/* This is the OnIdle event handler. It is set in the Form’s OnCreate event handler, so you need only add it as a private method of the form.  Usually it would perform some background processing for the application.  This one simply writes a message to let you know when it’s there. */

void __fastcall TForm1::MyIdleHandler(TObject *Sender, bool &Done)
{
  StatusMsg(Form1, &Canvas, "This represents a background process.", true);
  Done = false;
}
//---------------------------------------------------------------------------
