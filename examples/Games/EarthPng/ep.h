//---------------------------------------------------------------------------
#ifndef EarthPongH
#define EarthPongH
//---------------------------------------------------------------------------
#include <xwings.h>
//---------------------------------------------------------------------------
class TFormMain : public TForm
{
private:	// User declarations
    bool GameOver;

public:		// User declarations
	TFormMain();
    void __fastcall IdleLoop(TObject*, bool&);
    TImage *backgroundImage;
    TImage *spriteImage;
    TImage *paddle;
    TCanvas	*backgroundCanvas;
    TCanvas *workCanvas;
    TRect	backgroundRect, spriteRect, changeRect, paddleRect, changePaddleRect;
    int x, y, xDir, yDir, paddleX, paddleY, paddleCenter, Angle;

	void __fastcall FormPaint(TObject *Sender);

	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);

    void __fastcall virtual SetCursor(TMessage &);
};
//---------------------------------------------------------------------------
extern TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
