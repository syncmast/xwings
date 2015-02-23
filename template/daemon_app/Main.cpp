//---------------------------------------------------------------------------
#include "Service1.h"
//---------------------------------------------------------------------------
int main(int argc, char* argv[])
{
   	Application.Initialize();
   
   	TForm1 mf;
   	Application.MainForm = &mf;
	mf.Show();
	Application.Title = "My service application";
   	Application.Run();
	return 0;
}
//---------------------------------------------------------------------------
