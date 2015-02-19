//---------------------------------------------------------------------------
#ifndef rttiH
#define rttiH

#include <typeinfo>
#include "types.h"
//---------------------------------------------------------------------------
#define __classid(cls)  &typeid(cls)

typedef const type_info	TMetaClass;
typedef void *(* TCreateObjectProc)();
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
//  TClass
//---------------------------------------------------------------------------
class TClass
{
private:
    TMetaClass *m_info;

public:
    TClass();
    TClass(const TMetaClass *a, TCreateObjectProc proc = NULL);

    inline bool Before(const TClass &rhs) const
    {
        return m_info->before(*rhs.m_info) != 0;
    }
    void * Create();
    inline TMetaClass & Get() const
    {
        return *m_info;
    }
    const TString Name() const;
    void Register(TCreateObjectProc proc);
    void Register(const TString &ident);

    static void * Create(const TString &ident);
    static void * Create(const TMetaClass *a);
};

inline bool operator==(const TClass& lhs, const TClass& rhs)
{
    return (lhs.Get() == rhs.Get()) != 0;
}

inline bool operator<(const TClass& lhs, const TClass& rhs)
{
    return lhs.Before(rhs);
}

inline bool operator!=(const TClass& lhs, const TClass& rhs)
{
    return !(lhs == rhs);
}

inline bool operator>(const TClass& lhs, const TClass& rhs)
{
    return rhs < lhs;
}

inline bool operator<=(const TClass& lhs, const TClass& rhs)
{
    return !(lhs > rhs);
}

inline bool operator>=(const TClass& lhs, const TClass& rhs)
{
    return !(lhs < rhs);
}
//---------------------------------------------------------------------------
}   //namespace
//---------------------------------------------------------------------------
#define CREATE_OBJECT_PROC(cls)	cls::__create_object__
#define RegisterClass(cls)	TClass(__classid(cls)).Register(CREATE_OBJECT_PROC(cls))
#define RegisterClassIdent(cls, str)	TClass(__classid(cls)).Register(str)

#define DECLARE_COPY_DISABLE( classname )   private: classname(const classname &a); classname & operator=(const classname &a)
#define DECLARE_DYNAMIC_CLASS( classname )  public: static void * __create_object__() { return new classname(); }

#define DYNAMIC_CLASS( classname ) 		static void * classname##Create() { return new classname(); }; \
									TClass classname##Class(__classid(classname), classname##Create)
//---------------------------------------------------------------------------
#endif
