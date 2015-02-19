//---------------------------------------------------------------------------
#ifndef cursorH
#define cursorH

#include "xgdi.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
typedef TCommonList<HCURSOR> TCursorList;

HCURSOR __fastcall CreateCursor(int shape);
HCURSOR __fastcall CreateCursor(TFont *src, TFont *mask, int src_chr, int mask_chr);
HCURSOR __fastcall CreateCursor(TBitmap *src, int hot_x, int hot_y);
HCURSOR __fastcall CreateCursor(TCursorList &lst, int delay);
HCURSOR __fastcall CreateCursor(TImageList *lst, int hot_x, int hot_y, int delay);
void __fastcall DestroyCursor(HCURSOR hcur);
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
