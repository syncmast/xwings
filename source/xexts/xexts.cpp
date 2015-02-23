//---------------------------------------------------------------------------
#include <stdio.h>
#include "xexts.h"
#include "rtti.h"
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
void SO_ENTRYPOINT initialization()
{
	RegisterClass(TRandrExtension);
	RegisterClassIdent(TRandrExtension, "RANDR");
}

void SO_EXITPOINT finalization()
{
}
//---------------------------------------------------------------------------
//  TRandrExtension
//---------------------------------------------------------------------------
TRandrExtension :: TRandrExtension() : TXExtension("RANDR")
{
	m_ScreenInfo = XRRGetScreenInfo(Application.Handle, Screen.Root);
	m_sizes = XRRConfigSizes(m_ScreenInfo, &m_SizeCount);
	m_rotations = XRRConfigRotations(m_ScreenInfo, &m_current_rotation);

	ReadConfig();
}
//---------------------------------------------------------------------------
TRandrExtension :: ~TRandrExtension()
{
	XRRFreeScreenConfigInfo(m_ScreenInfo);
}
//---------------------------------------------------------------------------
word_t TRandrExtension :: get_Rate()
{
	return XRRConfigCurrentRate(m_ScreenInfo);
}
//---------------------------------------------------------------------------
word_t TRandrExtension :: get_Rates(const int index)
{
	return (index < m_RateCount) ? m_rates[index] : 0;
}
//---------------------------------------------------------------------------
TScreenSize TRandrExtension :: get_ScreenSize()
{
	word_t sizeid;
	sizeid = XRRConfigCurrentConfiguration(m_ScreenInfo, &m_current_rotation);
	return m_sizes[sizeid];
}
//---------------------------------------------------------------------------
TScreenSize TRandrExtension :: get_ScreenSizes(const int index)
{
	TScreenSize e = {0,0,0,0};
	return (index < m_SizeCount) ? m_sizes[index] : e;
}
//---------------------------------------------------------------------------
TScreenOrientation TRandrExtension :: get_Orientation()
{
	TScreenOrientation ret;
	if(m_current_rotation & RR_Rotate_0)
		ret << scoNormal;
	if(m_current_rotation & RR_Rotate_90)
		ret << sco90;
	if(m_current_rotation & RR_Rotate_180)
		ret << sco180;
	if(m_current_rotation & RR_Rotate_270)
		ret << sco270;
	if(m_current_rotation & RR_Reflect_X)
		ret << scoReflectX;
	if(m_current_rotation & RR_Reflect_Y)
		ret << scoReflectY;
	return ret;
}
//---------------------------------------------------------------------------
TScreenOrientation TRandrExtension :: get_Orientations()
{
	TScreenOrientation ret;
	if(m_rotations & RR_Rotate_0)
		ret << scoNormal;
	if(m_rotations & RR_Rotate_90)
		ret << sco90;
	if(m_rotations & RR_Rotate_180)
		ret << sco180;
	if(m_rotations & RR_Rotate_270)
		ret << sco270;
	if(m_rotations & RR_Reflect_X)
		ret << scoReflectX;
	if(m_rotations & RR_Reflect_Y)
		ret << scoReflectY;
	return ret;
}
//---------------------------------------------------------------------------
void __fastcall TRandrExtension :: ReadConfig()
{
	m_sizeid = XRRConfigCurrentConfiguration(m_ScreenInfo, &m_current_rotation);
	m_Rate = XRRConfigCurrentRate(m_ScreenInfo);
	m_rates = XRRConfigRates(m_ScreenInfo, m_sizeid, &m_RateCount);
}
//---------------------------------------------------------------------------
void __fastcall TRandrExtension :: Bind(TWinControl *wc)
{
	XRRSelectInput(Application.Handle, wc->Handle,
					RRScreenChangeNotifyMask | RRCrtcChangeNotifyMask |
					RROutputChangeNotifyMask | RROutputPropertyNotifyMask);
}
//---------------------------------------------------------------------------
void __fastcall TRandrExtension :: GetVersion(int *major, int *minor)
{
	XRRQueryVersion(Application.Handle, major, minor);
}
//---------------------------------------------------------------------------
bool __fastcall TRandrExtension :: WndProc(XEvent &event)
{
	bool ret = false;
	XRRCrtcChangeNotifyEvent *ea;
	XRROutputChangeNotifyEvent *eb;
	XRROutputPropertyNotifyEvent *ec;

	switch (event.type)
	{
		case RRScreenChangeNotify:
				ReadConfig();
//				XRRUpdateConfiguration(&event);
					printf("RANDR1$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
				ret = true;
				break;
		case RRNotify:
				XRRNotifyEvent *ne = (XRRNotifyEvent *)&event;
				switch(ne->subtype)
				{
					case RRNotify_CrtcChange:
						ea = (XRRCrtcChangeNotifyEvent *)&event;
						printf("RANDR3$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
						ret = true;
						break;
					case RRNotify_OutputChange:
						eb = (XRROutputChangeNotifyEvent *)&event;
						printf("RANDR4$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
						ret = true;
						break;
					case RRNotify_OutputProperty:
						ec = (XRROutputPropertyNotifyEvent *)&event;
						printf("RANDR5$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
				}
				ret = true;
				break;
   	}
	return ret;
}
//---------------------------------------------------------------------------
void __fastcall TRandrExtension :: Apply(int idxsize, int idxrate, TScreenOrientation so)
{
	int size_index;
	Rotation rot;
	short rate;
	int ratecount;
	short *rates;

	if(idxsize >= 0 && idxsize < m_SizeCount)
		size_index = idxsize;
	else
		size_index = XRRConfigCurrentConfiguration(m_ScreenInfo, &rot);
	rot = 0;

	rates = XRRConfigRates(m_ScreenInfo, size_index, &ratecount);
	if(idxrate >= 0 && idxrate < ratecount)
		rate = rates[idxrate];
	else
		rate = rates[0];

	if(so.Empty())
		so << scoNormal;
	if(so.Contains(scoNormal))
		rot |= RR_Rotate_0;
	if(so.Contains(sco90))
		rot |= RR_Rotate_90;
	if(so.Contains(sco180))
		rot |= RR_Rotate_180;
	if(so.Contains(sco270))
		rot |= RR_Rotate_270;
	if(so.Contains(scoReflectX))
		rot |= RR_Reflect_X;
	if(so.Contains(scoReflectY))
		rot |= RR_Reflect_Y;


	XRRSetScreenConfigAndRate(Application.Handle, m_ScreenInfo,
				Screen.Root, size_index, rot, rate, CurrentTime);
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
/*

Status XRRSetScreenConfigAndRate (Display *dpy,
                  XRRScreenConfiguration *config,
                  Drawable draw,
                  int size_index,
                  Rotation rotation,
                  short rate,
                  Time timestamp);

Rotation XRRConfigRotations(
    XRRScreenConfiguration *config,
    Rotation *current_rotation);

Time XRRConfigTimes (
    XRRScreenConfiguration *config,
    Time *config_timestamp);

XRRScreenSize *XRRConfigSizes(
    XRRScreenConfiguration *config,
    int *nsizes);

SizeID XRRConfigCurrentConfiguration (
    XRRScreenConfiguration *config,
    Rotation *rotation);


int XRRRootToScreen(
    Display *dpy,
    Window root);

 * intended to take RRScreenChangeNotify,  or
 * ConfigureNotify (on the root window)
 * returns 1 if it is an event type it understands, 0 if not

int XRRUpdateConfiguration(XEvent *event^);


 * the following are always safe to call, even if RandR is
 * not implemented on a screen


Rotation XRRRotations(
    Display *dpy, int screen,
    Rotation *current_rotation);

XRRScreenSize *XRRSizes(Display *dpy,
    int screen, int *nsizes);

short *XRRRates (Display *dpy, int screen,
    int size_index, int *nrates);

Time XRRTimes (Display *dpy, int screen, Time *config_timestamp);
*/
