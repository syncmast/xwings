//---------------------------------------------------------------------------
#include "canmain.h"
//---------------------------------------------------------------------
int main()
{
	try
	{
		RegisterClass(TFormMain);
		Application.CreateForm(__classid(TFormMain), &FormMain);

    	Application.Run();
	}
	catch (Exception &exception)
	{
		Application.ShowException(&exception);
	}
    return 0;
}
//---------------------------------------------------------------------------
