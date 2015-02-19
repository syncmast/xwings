//---------------------------------------------------------------------------
#ifndef xextsH
#define xextsH

//#include <X11/extensions/shape.h>
#include <X11/extensions/Xrandr.h>
#include "application.h"
//---------------------------------------------------------------------------
/*
enum TShapeOperation {
	shoSet = ShapeSet,
	shoUnion = ShapeUnion,
	shoIntersect = ShapeIntersect,
	shoSubtract = ShapeSubtract,
	shoInvert = ShapeInvert
};

enum TShapeKind {
	shkBounding = ShapeBounding,
	shkClip = ShapeClip,
	shkInput = ShapeInput
};

class TShapeExtension : public TXExtension
{
	DYNAMIC_CLASS(TShapeExtension);

private:

protected:

public:
   	__fastcall TShapeExtension();

	virtual void __fastcall GetVersion(int *major, int *minor);
	virtual void __fastcall Bind(TWinControl *wc);
	void SetShape(const TWinControl &w, const TRegion &region, TShapeKind kind = shkBounding, TShapeOperation op = shoSet, int xoff = 0, int yoff = 0);
	void SetShape(const TWinControl &w, const TBitmap &bmp, TShapeKind kind = shkBounding, TShapeOperation op = shoSet, int xoff = 0, int yoff = 0);
	void SetShape(const TWinControl &w, const TWinControl &src, TShapeKind kind = shkBounding, TShapeOperation op = shoSet, int xoff = 0, int yoff = 0, TShapeKind src_kind = shkBounding);
	void SetShape(const TWinControl &w, TRect *rects, const int nrects, TShapeKind kind = shkBounding, TShapeOperation op = shoSet, int xoff = 0, int yoff = 0, int ordering = Unsorted);
	void SetShape(const TWinControl &w, TShapeKind kind = shkBounding, int xoff = 0, int yoff = 0);
	virtual bool __fastcall WndProc(XEvent &event);

}; */
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
typedef XRRScreenSize TScreenSize;

enum TScreenOrientationKind { scoNormal, sco90, sco180, sco270, scoReflectX, scoReflectY };
typedef TSet<TScreenOrientationKind>  TScreenOrientation;

class TRandrExtension : public TXExtension
{
	DECLARE_DYNAMIC_CLASS(TRandrExtension);
	DECLARE_PROPERTY(TRandrExtension);

private:
	XRRScreenConfiguration *m_ScreenInfo;
	int m_SizeCount;
	XRRScreenSize *m_sizes;
	Rotation m_rotations;
	Rotation m_current_rotation;
	int m_RateCount;
	short *m_rates;
	word_t m_sizeid;
	word_t m_Rate;
	word_t get_Rate();
	word_t get_Rates(const int index);
	TScreenSize get_ScreenSize();
	TScreenSize get_ScreenSizes(const int index);
	TScreenOrientation get_Orientation();
	TScreenOrientation get_Orientations();
	void __fastcall ReadConfig();

protected:

public:
   	__fastcall TRandrExtension();
	~TRandrExtension();

	void __fastcall Apply(int idxsize, int idxrate, TScreenOrientation rotation = TScreenOrientation());
	virtual void __fastcall GetVersion(int *major, int *minor);
	virtual void __fastcall Bind(TWinControl *wc);
	virtual bool __fastcall WndProc(XEvent &event);

	property (r, word_t, Rate);
	property_i (r, word_t, int, Rates);
   	property (mr, int, RateCount);

	property (r, TScreenSize, ScreenSize);
	property_i (r, TScreenSize, int, ScreenSizes);
	property (mr, int, SizeCount);

	property (r, TScreenOrientation, Orientation);
	property (r, TScreenOrientation, Orientations);
};
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
#endif
