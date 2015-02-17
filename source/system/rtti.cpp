//---------------------------------------------------------------------------
#include <cxxabi.h>
#include "rtti.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
class TClassRegister
{
    typedef map<TString, TCreateObjectProc> TCreateStringMap;
    typedef map<TMetaClass *, TCreateObjectProc> TCreateTypeMap;

private:
    static TCreateTypeMap &get_type_map()
    {
        static TCreateTypeMap __class_type_map;
        return __class_type_map;
    }
    static TCreateStringMap &get_string_map()
    {
        static TCreateStringMap __class_string_map;
        return __class_string_map;
    }
public:

    static void Register(const TMetaClass *a, const TString &ident)
    {
        TCreateTypeMap::iterator it = get_type_map().find(a);
        if(it == get_type_map().end())
            return;
        get_string_map()[ident] = *it->second;
    }

    static void Register(const TMetaClass *a, TCreateObjectProc proc)
    {
        get_type_map()[a] = proc;
    }

    static void *Create(const TString &ident)
    {
        TCreateStringMap::iterator it = get_string_map().find(ident);
        if(it != get_string_map().end())
            return (*it->second)();
        return NULL;
    }

    static void *Create(const TMetaClass *a)
    {
        TCreateTypeMap::iterator it = get_type_map().find(a);
        if(it != get_type_map().end())
            return (*it->second)();
        return NULL;
    }
};
//---------------------------------------------------------------------------
//  TClass
//---------------------------------------------------------------------------
TClass :: TClass()
    : m_info(__classid(void *))
{
}
//---------------------------------------------------------------------------
TClass::TClass(TMetaClass *a, TCreateObjectProc proc)
    : m_info(a)
{
    Register(proc);
}
//---------------------------------------------------------------------------
void * TClass :: Create()
{
    return TClassRegister :: Create(m_info);
}
//---------------------------------------------------------------------------
void * TClass :: Create(const TString &ident)
{
    return TClassRegister :: Create(ident);
}
//---------------------------------------------------------------------------
void * TClass :: Create(const TMetaClass *a)
{
    return TClassRegister :: Create(a);
}
//---------------------------------------------------------------------------
const TString TClass :: Name() const
{
    int     status;
    const char   *realname;

    const char *tmp = m_info->name();
    realname = abi::__cxa_demangle(tmp, 0, 0, &status);
    TString ret = realname;
    free((void *)realname);
    return ret;
}
//---------------------------------------------------------------------------
void TClass :: Register(const TCreateObjectProc proc)
{
    if(proc == NULL)
        return;
    TClassRegister :: Register(m_info, proc);
    TClassRegister :: Register(m_info, Name());
}
//---------------------------------------------------------------------------
void TClass :: Register(const TString &ident)
{
    TClassRegister :: Register(m_info, ident);
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
