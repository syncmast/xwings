#include "newobj.h"
//---------------------------------------------------------------------------
//  T
//---------------------------------------------------------------------------
DYNAMIC_CLASS( T );
//---------------------------------------------------------------------------
T :: T()
{
    Init();
}
//---------------------------------------------------------------------------
T :: ~T()
{
}
//---------------------------------------------------------------------------
__fastcall T :: T(const T &a) : TP(a)
{
	Init();
	Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const T & T :: operator=(const T &a)
{
	if(this == &a)
		return *this;
    TP :: operator=(a);
    Assign(a);
	return *this;
}
//---------------------------------------------------------------------------
void T :: Init()
{
}
//---------------------------------------------------------------------------
__fastcall void T :: Assign(const T &obj)
{
}
//---------------------------------------------------------------------------

