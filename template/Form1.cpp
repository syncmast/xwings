//---------------------------------------------------------------------------
#include "Form1.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
TForm1 :: TForm1()
{

}
//---------------------------------------------------------------------------
TForm1 :: TForm1(TWinControl *AParent) : TForm(AParent)
{

}
//---------------------------------------------------------------------------
TForm1 :: ~TForm1()
{
}
//---------------------------------------------------------------------------
__fastcall TForm1 :: TForm1(const TForm1 &a)
{
	*this = a;
}
//---------------------------------------------------------------------------
__fastcall TForm1 & TForm1 :: operator=(const TForm1 &a)
{
	if(this == &a)
		return *this;
        // выполняет присваивание, развернутое копирование
        // или еще что-нибудь
	return *this;
}
//---------------------------------------------------------------------------

