//---------------------------------------------------------------------------
#ifndef Form1H
#define Form1H
//---------------------------------------------------------------------------
#include "forms.h"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
private:
public:	
	__fastcall TForm1();
	__fastcall TForm1(const TForm1 &a);
	~TForm1();
	__fastcall TForm1 & operator=(const TForm1 &a);
};
//---------------------------------------------------------------------------
extern TForm1 *Form1;
//---------------------------------------------------------------------------
#endif