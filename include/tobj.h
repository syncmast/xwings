//---------------------------------------------------------------------------
#ifndef tobjH
#define tobjH

#include "rtti.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
//  TObject
//---------------------------------------------------------------------------
enum TErrorAction { errIgnore, errAbort, errRetry };

class TObject;

#define delegate FastDelegate

typedef delegate< TErrorAction  (TObject* sender, int errcode) > TErrorEvent;

class TObject
{
private:
    bool m_dynamic;
    void Init();
    void __fastcall Assign(const TObject &obj);

protected:
    virtual TErrorAction DoError(int errcode);

public:
    int id;

    TErrorEvent OnError;

    TObject();
    __fastcall TObject(const TObject &a);
    virtual ~TObject();
    __fastcall const TObject & operator=(const TObject &a);
    /*	__fastcall void *operator new(size_t size)
    	{
    		void *ret = :: operator new(size);
    		((TObject *)ret)->m_dynamic = true;
    		return ret;
    	}
    	__fastcall void *operator new[](size_t size)
    	{
    		void *ret = :: operator new[](size);
    		((TObject *)ret)->m_dynamic = true;
    		return ret;
    	}
    */
    const TString ClassName();
    TMetaClass *ClassType();
};

typedef delegate< void (TObject *Sender) > TNotifyEvent;

#define HANDLEERROR(errcode, retval, op_retry) \
    switch(DoError(errcode)) {  \
        case errIgnore: break;      \
        case errAbort:  return retval; \
        case errRetry:  op_retry; }
//---------------------------------------------------------------------------
#define EVENT(te, cls, mt)	te(this,&cls::mt)
//---------------------------------------------------------------------------
//  THandleObject
//---------------------------------------------------------------------------
class THandleObject : public TObject
{
    DECLARE_PROPERTY(THandleObject);

private:
    void Init();
    void __fastcall Assign(const THandleObject &obj);

protected:
    int m_Handle;

public:
    THandleObject();
    __fastcall THandleObject(const THandleObject &a);
    virtual ~THandleObject();
    __fastcall const THandleObject & operator=(const THandleObject &a);

    property (mr, int, Handle);
};
//---------------------------------------------------------------------------
}   //namespace
//---------------------------------------------------------------------------
#endif
