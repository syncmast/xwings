//---------------------------------------------------------------------------
#ifndef canmainH
#define canmainH
//---------------------------------------------------------------------------
#include "xwings.h"

const int  MaxPoints    = 15;  // Number of points to be drawn

struct TRPoint {
  float X, Y;
};
//---------------------------------------------------------------------------
class TFormMain : public TForm
{
	DECLARE_DYNAMIC_CLASS( TFormMain );

private:        // User declarations
    TRPoint     Points[MaxPoints];
    float       Rotation;     // in radians
    int         PointCount;
    void        RotatePoints();
	clock_t clock1;
public:         // User declarations
    TFormMain();
    TTimer Timer1;
    void  FormPaint(TObject *Sender);
    void  FormResize(TObject *Sender);
    void  Timer1Timer(TObject *Sender);
};
//---------------------------------------------------------------------------
extern TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
