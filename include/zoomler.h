//---------------------------------------------------------------------------
#ifndef zoomlerH
#define zoomlerH

#include "sysobj.h"
#include "xgdi.h"
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
const int  sc_LEFTPAGE = 0;
const int  sc_RIGHTPAGE = 1;
const int  sc_LEFTSIZE = 2;
const int  sc_RIGHTSIZE = 3;
const int  sc_MOVE = 4;
const int  sc_LEFTMOVE = 5;
const int  sc_RIGHTMOVE = 6;
const int  sc_NONE = 7;

const int  TimerDelay = 500;
const int  TimerRate  = 50;

enum TOrientation { Horizontal, Vertical };
enum TZoomStatus { st_NONE, st_LEFT, st_RIGHT, st_BOTH };

class TZoomler : public TControl
{
	DECLARE_PROPERTY( TZoomler );

private:
	int FSL,FSR;
    double m_Min, m_Max, m_Start, m_Finish, FStep;
    TZoomStatus m_status;
    int XS;
    TOrientation m_Orientation;
    TTimer Timer;
    TMouseButton hButton;
    TShiftState hShift;
    int hx, hy;
    bool LeftMoving;
    bool RightMoving;
    void ProcessMove(int X);
    byte_t WherePoint(int X, int Y);
    void CalcScreen();
    void DoLeftMove();
    void DoRightMove();
    void DoLeftPage(int X);
    void DoRightPage(int X);
    void set_Min(double m);
    void set_Max(double m);
    void set_Start(double m);
    void set_Finish(double m);
    void set_Orientation(TOrientation aOrientation);
    void OnTimer(TObject *Sender);

protected:
    void MouseDown(TMouseButton Button, TShiftState Shift, int X, int Y);
    void MouseMove(TShiftState Shift, int X, int Y);
    void MouseUp(TMouseButton Button, TShiftState Shift, int X, int Y);
    void DblClick();
    void Paint();

public:
    TZoomler();
    ~TZoomler();

    bool DblClkEnable;

//    property Align;
//    property Visible;
    TNotifyEvent OnScrolling;
    TNotifyEvent OnZooming;
    TNotifyEvent OnChanged;

    property (m, double, Min);
    property (m, double, Max);
    property (m, double, Start);
    property (m, double, Finish);
    property (m, TOrientation, Orientation);

};

void UpRect(TCanvas *C, int x1, int y1, int x2, int y2);
void DownRect(TCanvas *C, int x1, int y1, int x2, int y2);

double DMin(double a, double b);
double DMax(double a, double b);
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
#endif
