#include "canmain.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include "netwm.h"
//---------------------------------------------------------------------------
TFormMain *FormMain;
//---------------------------------------------------------------------------
TFormMain::TFormMain()
{
	id = 777;
	Canvas.Pen->Color = clTeal;
   Caption = "Canvas";
   Color = clBlack;
	Rotation = 0;
	PointCount = MaxPoints;
	RotatePoints();
	Align = alCenter;
   BoundsRect = TRect(200,100,408,450);

   OnPaint = TNotifyEvent(this,&TFormMain::FormPaint);
   OnResize = TNotifyEvent(this,&TFormMain::FormResize);
   Timer1.OnTimer = TNotifyEvent(this,&TFormMain::Timer1Timer);
   Timer1.Interval = 800;
   Timer1.Enabled = true;
}
//---------------------------------------------------------------------------
void TFormMain::FormPaint(TObject *Sender)
{
    TRect r = ClientRect;
	int centerX = r.width / 2;
	int centerY = r.height / 2;

	int radius = std::min(centerY, centerX);

	Canvas.Ellipse(0, 0, radius * 2, radius * 2);
	int i,j;
	for (i = 0; i < PointCount; i++)
    {
		for (j = i + 1; j < PointCount; j++)
		{
			Canvas.MoveTo(radius + floor(Points[i].X * radius),
				radius + floor(Points[i].Y * radius));
			Canvas.LineTo(radius + floor(Points[j].X * radius),
				radius + floor(Points[j].Y * radius));
		}
	}
	// A challenge:  Turn the rotating figure into a ball that bounces off the
	// walls of the window.  Don't forget the english (spin) on the ball should
	// pick up when bouncing off the wall...
}
//---------------------------------------------------------------------
void TFormMain::FormResize(TObject *Sender)
{
//	Height = Width;
//		XSync(Application.Handle, false);
//	Update();
}
//---------------------------------------------------------------------
void TFormMain::Timer1Timer(TObject *Sender)
{
        printf("TIMER\n");
 //   Timer1.Enabled = false;
	RotatePoints();
//	Invalidate();
//    Timer1.Enabled = true;
/*    XEvent event;
    event.type = Expose;
    event.xexpose.display = Application.Handle;
    event.xexpose.window = m_Handle;
    event.xexpose.x = 0;
    event.xexpose.y = 0;
    event.xexpose.width = 500;
    event.xexpose.height = 500;
    event.xexpose.count = 10;
    XPutBackEvent(Application.Handle, &event);
    XSync(Application.Handle,false); */
    Update();
//    Repaint();
}
//---------------------------------------------------------------------
void TFormMain::RotatePoints()
{
	// NOTE: all figures are in radians
	const float M_2PI = 2 * M_PI;           // 2 pi radians in a circle
	float StepAngle = M_2PI / PointCount;   // angular distance between points

	Rotation += M_PI / 32;   // Increment the angle of rotation of figure
	if (Rotation > StepAngle)
		Rotation -= StepAngle;   // Keep rotation less than distance between points

	// The loop below has i walking through the Points array, while j walks
	// simultaneously through the angles to each point on the circle.
	// Incrementing j by StepAngle moves j to the next point on the circle with
	// no complicated arithmetic (everything has been set up in advance of the
	// loop).  Initializing j with Rotation causes the entire figure to shift
	// clockwise a small amount.
	//
	int i;
	float j;
	for (i = 0, j = Rotation; i < PointCount; i++, j += StepAngle) {
		Points[i].X = cos(j);   // These values will be multiplied by the
		Points[i].Y = sin(j);   // current radius at display time.
	}
}
//---------------------------------------------------------------------
