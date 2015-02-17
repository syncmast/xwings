//---------------------------------------------------------------------------
#include "tobj.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
//  TObject
//---------------------------------------------------------------------------
TObject :: TObject()
{
    Init();
}
//---------------------------------------------------------------------------
TObject :: ~TObject()
{
}
//---------------------------------------------------------------------------
__fastcall TObject :: TObject(const TObject &a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const TObject & TObject :: operator=(const TObject &a)
{
    if(this == &a)
        return *this;
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TObject :: Init()
{
    id = 0;
}
//---------------------------------------------------------------------------
__fastcall void TObject :: Assign(const TObject &obj)
{
    id = obj.id;
}
//---------------------------------------------------------------------------
const TString TObject :: ClassName()
{
    /*	 int     status;
      const char   *realname;

    	const char *tmp = typeid(*this).name();
    //	while(isdigit(*tmp))
    //		tmp++;

    //		  const std::type_info  &ti = typeid(u);

      realname = abi::__cxa_demangle(tmp, 0, 0, &status);
      free((void *)realname);
    	return realname;
    */
    return TClass(__classid(*this)).Name();
}
//---------------------------------------------------------------------------
TMetaClass * TObject :: ClassType()
{
    return &typeid(*this);
}
//---------------------------------------------------------------------------
TErrorAction TObject :: DoError(int errcode)
{
    TErrorAction ret = errIgnore;

    if(OnError)
        ret = OnError(this, errcode);

    return ret;
}
//---------------------------------------------------------------------------
//  THandleObject
//---------------------------------------------------------------------------
THandleObject :: THandleObject() : m_Handle(0)
{
    Init();
}
//---------------------------------------------------------------------------
THandleObject :: ~THandleObject()
{
}
//---------------------------------------------------------------------------
__fastcall THandleObject :: THandleObject(const THandleObject &a) : TObject(a)
{
    Assign(a);
}
//---------------------------------------------------------------------------
__fastcall const THandleObject & THandleObject :: operator=(const THandleObject &a)
{
    if(this == &a)
        return *this;
    TObject :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void THandleObject :: Init()
{
}
//---------------------------------------------------------------------------
void __fastcall THandleObject :: Assign(const THandleObject &obj)
{
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------

