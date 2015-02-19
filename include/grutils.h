//---------------------------------------------------------------------------
#ifndef grutilsH
#define grutilsH

#include "graphics.h"
//---------------------------------------------------------------------------
namespace xwings {
//---------------------------------------------------------------------------
void __fastcall AddColorNoise(TBitmap &clip, int amount);
void __fastcall AddMonoNoise(TBitmap &clip, int amount);
void __fastcall Emboss(TBitmap &bmp, int amount);
void __fastcall Saturation(TBitmap &clip, int amount);
void __fastcall Blend(TBitmap &src1, TBitmap &src2, TBitmap &dst, int amount);
void __fastcall Blend(TBitmap &src1, TBitmap &src2, TBitmap &alfa, TBitmap &dst);
void __fastcall Invert(TBitmap &src);
void __fastcall Triangles(TBitmap &src, int amount);
void __fastcall RippleTooth(TBitmap &src, int amount);
void __fastcall RippleTriangle(TBitmap &src, int amount);
void __fastcall RippleRandom(TBitmap &src, int amount);
void __fastcall Solorize(TBitmap &src, TBitmap &dst, int amount);
void __fastcall Posterize(TBitmap &src, TBitmap &dst, int amount);
void __fastcall TurnRight(TBitmap &src, TBitmap &dst);
void __fastcall Turn(TBitmap &src, TBitmap &dst);
void __fastcall HeightMap(TBitmap &src, int amount);
void __fastcall Spray(TBitmap &clip, int amount);
void __fastcall Mosaic(TBitmap &bm, uint_t size);
void __fastcall Lightness(TBitmap &bm, int amount);
void __fastcall Darkness(TBitmap &clip, int amount);
void __fastcall SplitBlur(TBitmap &clip, uint_t amount);
void __fastcall GaussianBlur(TBitmap &clip, uint_t amount);
void __fastcall Contrast(TBitmap &clip, int amount);
void __fastcall AntiAlias(TBitmap &clip, const TRect &rect);
void __fastcall FishEye(TBitmap &bmp, TBitmap &dst, double amount);
void __fastcall Marble(TBitmap &src, TBitmap &dst, double scale, int turbulence, int var);
	void __fastcall ShadowUpLeft(TBitmap &src);
	void __fastcall ShadowUpRight(TBitmap &src);
	void __fastcall ShadowDownLeft(TBitmap &src);
	void __fastcall ShadowDownRight(TBitmap &src);
void __fastcall CutString(TCanvas *Canvas, TString &str, int maxlen);

void Convert2Res(const char *filename, const char *outfn, const char *resname);
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
#endif
