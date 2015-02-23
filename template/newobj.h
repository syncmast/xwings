#ifndef T_H_INCLUDED
#define T_H_INCLUDED

#include "xgdi.h"
//---------------------------------------------------------------------------
//  T
//---------------------------------------------------------------------------
class T : public TP
{
    DECLARE_PROPERTY( T );

private:
    void Init();
    __fastcall void Assign(const T &obj);

protected:

public:
   	T();
    __fastcall T(const T &a);
   	virtual ~T();
   	__fastcall const T & operator=(const T &a);
};
//---------------------------------------------------------------------------
#endif // T_H_INCLUDED
