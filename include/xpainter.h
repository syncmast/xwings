#ifndef PAINTER_H_INCLUDED
#define PAINTER_H_INCLUDED

#include "xgdi.h"
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
//  TXPainter
//---------------------------------------------------------------------------
class TXPainter : public TPainter
{
private:
    void Init();
    __fastcall void Assign(const TXPainter &obj);

protected:

public:
   	TXPainter();
    __fastcall TXPainter(const TXPainter &a);
   	virtual ~TXPainter();
   	__fastcall const TXPainter & operator=(const TXPainter &a);

   	virtual void __fastcall PaintControl(TControl &ctrl);
   	void __fastcall DrawButton(TControl &ctrl);
   	void __fastcall DrawCheckBox(TControl &ctrl);
   	void __fastcall DrawComboBox(TControl &ctrl);
   	void __fastcall DrawGroupBox(TControl &ctrl);
   	void __fastcall DrawPanel(TControl &ctrl);
    void __fastcall DrawDisabledText(TRect &rect, const TString &text, TFormatText *format = NULL);
    void __fastcall DrawFocusRect(const TRect &rect);
};
//---------------------------------------------------------------------------
extern TXPainter XPainter;
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
#endif // PAINTER_H_INCLUDED
