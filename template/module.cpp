//---------------------------------------------------------------------------
#include <stdio.h>
#include "module.h"
//---------------------------------------------------------------------------
T :: T()
{

}
//---------------------------------------------------------------------------
T :: ~T()
{
}
//---------------------------------------------------------------------------
__fastcall T :: T(const T &a)
{
	*this = a;
}
//---------------------------------------------------------------------------
__fastcall T & T :: operator=(const T &a)
{
	if(this == &a)
		return *this;
        // выполняет присваивание, развернутое копирование
        // или еще что-нибудь
	return *this;
}


//---------------------------------------------------------------------------

