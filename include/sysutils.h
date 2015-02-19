//---------------------------------------------------------------------------
#ifndef sysutilsH
#define sysutilsH

#include "types.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
bool        CheckMainThread();
pthread_t   GetMainThreadID();
//---------------------------------------------------------------------------
class TLog
{
public:
    TString LogName;

    TLog();
    ~TLog();

    int printf(const char* format, ...);

};
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------
#endif
