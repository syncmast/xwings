//---------------------------------------------------------------------------
#include "HandleMsg.h"
//---------------------------------------------------------------------
int main()
{
	try
	{
		Application.Initialize();

		RegisterClass(TForm1);
		Application.CreateForm(__classid(TForm1), &Form1);

    	Application.Run();
	}
	catch (Exception &exception)
	{
		Application.ShowException(&exception);
	}
    return 0;
}
//---------------------------------------------------------------------------
