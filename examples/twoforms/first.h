//----------------------------------------------------------------------------
#ifndef FirstH
#define FirstH
//----------------------------------------------------------------------------
#include <xol.h>
#include <stdctrls.h>
//----------------------------------------------------------------------------
class TFormMain : public TForm
{
public:
    TFormMain();
    TButton AButton1;
    TSpeedButton SButton1;

	void Button1Click(TObject *Sender);
};
//----------------------------------------------------------------------------
extern TFormMain *FormMain;
//----------------------------------------------------------------------------
#endif