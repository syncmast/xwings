//---------------------------------------------------------------------

//---------------------------------------------------------------------
#include "first.h"
//----------------------------------------------------------------------------
DYNAMIC_CLASS( TFormMain );

int main()
{
	try
	{
        Application.CreateForm(__classid(TFormMain), &FormMain);
        Application.Run();
   }
   catch (Exception &exception)
   {
      Application.ShowException(&exception);
   }

   return 0;
}
//---------------------------------------------------------------------
