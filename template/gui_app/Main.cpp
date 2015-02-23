//---------------------------------------------------------------------------
#include "Form1.h"
//---------------------------------------------------------------------------
int main(int argc, char* argv[])
{
   	Application.Init();
   
   	TForm1 mf;
   	Application.MainForm = &mf;
	mf.Show();
	Application.Title = "My application";
   	Application.Run();
	return 0;
}
//---------------------------------------------------------------------------
