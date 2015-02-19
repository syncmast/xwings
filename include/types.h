//---------------------------------------------------------------------------
#ifndef typesH
#define typesH

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <sstream>
#include <list>
#include <exception>
#include <limits.h>
#include <stdlib.h>
#include "property.hpp"
#include "delegate.hpp"
//---------------------------------------------------------------------------
using namespace std;
using namespace delegates;
//---------------------------------------------------------------------------
typedef unsigned char   	byte_t;
typedef unsigned int    	uint_t;
typedef unsigned long   	ulong_t;
typedef unsigned short 		word_t;
typedef unsigned int 		dword_t;
// typedef long long			int64_t;   redeclaration in amd64
// typedef unsigned long long	uint64_t;

#ifdef __LP64__
#define __fastcall
#else
#define __fastcall __attribute__((fastcall))
#endif

#define SO_ENTRYPOINT __attribute__ ((constructor, visibility ("internal")))
#define SO_EXITPOINT __attribute__ ((destructor, visibility ("internal")))

#define BIT_CHECK(flags, bit)	flags & (bit)
#define BIT_ON(flags, bit)	flags | (bit)
#define BIT_OFF(flags, bit)	flags & (~(bit))
#define SIZEOFARRAY(arr) ((int) (sizeof(arr) / sizeof(arr[0])))
#define TRIMVAL(val,min,max)	(val > max) ? max : (val < min) ? min : val
#define INRANGE(val,min,max)	(min <= val && val <= max) ? true : false

#define CM_USER_MESSAGE		0x1000
#define CM_FONTCHANGED    	0x1001
//---------------------------------------------------------------------------
namespace xwings
{

struct TMessage
{
    long Msg;
    long Params[5];
    TMessage();
};
//---------------------------------------------------------------------------
//  TSet
//---------------------------------------------------------------------------
template <typename _T>
class TSet : public set<_T>
{
public:
    inline void __fastcall Clear()
    {
        TSet :: clear();
    };
    inline bool Contains(const _T el) const
    {
        return (TSet :: count(el))? true : false;
    };
    inline bool Empty() const
    {
        return TSet :: empty();
    };
    inline TSet& operator <<(const _T el)
    {
        TSet :: insert(el);
        return *this;
    };
    inline TSet& operator >>(const _T el)
    {
        TSet :: erase(el);
        return *this;
    };
};
//---------------------------------------------------------------------------
//  TString
//---------------------------------------------------------------------------
class TWString;

class TString : public string
{
private:
    TString __fastcall convert_case(bool ucase) const;

public:
    TString() { };
    __fastcall TString(const TString &src) : string(src) { };
    __fastcall TString(const char *src) : string(src) { };
    __fastcall TString(const char* src, unsigned int len)  : string(src, len) { };
    __fastcall TString(string s) : string(s) { };
    __fastcall TString(string::size_type n, char c) : string(n, c) { };
    __fastcall TString(string s, string::size_type apos, string::size_type an) : string(s, apos, an) { };
    __fastcall TString(const wchar_t* src);
    __fastcall TString(const TWString &src);
    __fastcall TString(char c) : string(1, c) { };
    __fastcall TString(int val)
    {
        printf("%d", val);
    };

    __fastcall TString const & operator=(const wchar_t *rvalue);

    TString & __fastcall printf(const char* format, ...);
    inline void Clear()
    {
        string :: erase();
    };
    inline int __fastcall CompareIC(const TString &rhs) const
    {
        return string::compare(rhs.LowerCase());
    }; //ignorecase
    inline TString & __fastcall Delete(int index, int count)
    {
        string::erase(index, count);
        return *this;
    };
    inline TString & __fastcall Insert(const TString &src, int index)
    {
        string :: insert(index, src);
        return *this;
    };
    inline bool IsEmpty() const
    {
        return string :: empty();
    };
    inline bool Empty() const
    {
        return string :: empty();
    };
    inline bool __fastcall IsPathDelimiter(int index) const
    {
        return string :: at(index) == '/';
    };
    inline int __fastcall LastDelimiter(const TString& delimiters, int opos = 0) const
    {
        return string :: find_last_of(delimiters, opos);
    };
    inline int __fastcall LastPos(const TString& subStr, int opos = 0) const
    {
        return string :: rfind(subStr, opos);
    };
    inline int __fastcall LastPos(char c, int opos = 0) const
    {
        return string :: rfind(c, opos);
    };
    inline int Length() const
    {
        return string :: size();
    };
    inline TString LowerCase() const
    {
        return convert_case(false);
    };
    inline TString UpperCase() const
    {
        return convert_case(true);
    };
    inline int __fastcall Pos(const TString &subStr, int opos = 0) const
    {
        return string :: find(subStr, opos);
    };
    inline int __fastcall Pos(char c, int opos = 0) const
    {
        return string :: find(c, opos);
    };
    inline TString & __fastcall SetLength(int newLength)
    {
        string :: resize(newLength);
        return *this;
    };
    inline TString & __fastcall StringOfChar(char ch, int count)
    {
        string :: assign(count, ch);
        return *this;
    };
    inline TString __fastcall SubString(int index, int count = 0) const
    {
        return count ? string :: substr(index, count) : string :: substr(index);
    };
    int __fastcall ToInt(int defaultValue = INT_MAX, int base = 10) const;
    double ToDouble() const;
    inline TString & Trim()
    {
        TrimLeft();
        TrimRight();
        return *this;
    };
    TString & TrimLeft();
    TString & TrimRight();
};

/*
  class RTL_DELPHIRETURN AnsiString
  {
  public:
    // the TStringFloatFormat enum is used by FloatToStrF
    enum TStringFloatFormat
    {sffGeneral, sffExponent, sffFixed, sffNumber, sffCurrency};

    static AnsiString __fastcall FormatFloat(const AnsiString& format,
                         const long double& value);
    static AnsiString __fastcall FloatToStrF(long double value,
                         TStringFloatFormat format, int precision, int digits);
    static AnsiString __fastcall IntToHex(int value, int digits);
    static AnsiString __fastcall CurrToStr(Currency value);
    static AnsiString __fastcall CurrToStrF(Currency value,
                        TStringFloatFormat format, int digits);

    // Convert to Unicode
    int          __fastcall WideCharBufSize() const;
    wchar_t*     __fastcall WideChar(wchar_t* dest, int destSize) const;

    bool         __fastcall IsDelimiter(const AnsiString& delimiters, int index) const;
    char*        __fastcall AnsiLastChar() const;

  };
 */

//---------------------------------------------------------------------------
//  TWString
//---------------------------------------------------------------------------
class TWString : public wstring
{
private:
    TWString __fastcall convert_case(bool ucase) const;

public:
    TWString() { };
    __fastcall TWString(const TWString &src) : wstring(src) { };
    __fastcall TWString(const wchar_t *src) : wstring(src) { };
    __fastcall TWString(const wchar_t *src, unsigned int len)  : wstring(src, len) { };
    __fastcall TWString(wstring s) : wstring(s) { };
    __fastcall TWString(wstring::size_type n, wchar_t c) : wstring(n, c) { };
    __fastcall TWString(wstring s, wstring::size_type apos, wstring::size_type an) : wstring(s, apos, an) { };
    __fastcall TWString(const char* src);
    __fastcall TWString(const TString &src);
    __fastcall TWString(wchar_t c) : wstring(1, c) { };
    __fastcall TWString(int val)
    {
        printf(L"%d", val);
    };

    __fastcall TWString const & operator=(const char *rvalue);

    TWString & __fastcall printf(const wchar_t* format, ...);
    inline void Clear()
    {
        wstring :: erase();
    };
    inline int __fastcall CompareIC(const TWString &rhs) const
    {
        return wstring::compare(rhs.LowerCase());
    }; //ignorecase
    inline TWString & __fastcall Delete(int index, int count)
    {
        wstring::erase(index, count);
        return *this;
    };
    inline TWString & __fastcall Insert(const TWString &src, int index)
    {
        wstring :: insert(index, src);
        return *this;
    };
    inline bool IsEmpty() const
    {
        return wstring :: empty();
    };
    inline bool Empty() const
    {
        return wstring :: empty();
    };
    inline bool __fastcall IsPathDelimiter(int index) const
    {
        return wstring :: at(index) == '/';
    };
    inline int __fastcall LastDelimiter(const TWString& delimiters, int opos = 0) const
    {
        return wstring :: find_last_of(delimiters, opos);
    };
    inline int __fastcall LastPos(const TWString &subStr, int opos = 0) const
    {
        return wstring :: rfind(subStr, opos);
    };
    inline int __fastcall LastPos(wchar_t c, int opos = 0) const
    {
        return wstring :: rfind(c, opos);
    };
    inline int Length() const
    {
        return wstring :: size();
    };
    inline TWString LowerCase() const
    {
        return convert_case(false);
    };
    inline TWString UpperCase() const
    {
        return convert_case(true);
    };
    inline int __fastcall Pos(const TWString &subStr, int opos = 0) const
    {
        return wstring :: find(subStr, opos);
    };
    inline int __fastcall Pos(wchar_t c, int opos = 0) const
    {
        return wstring :: find(c, opos);
    };
    inline TWString & __fastcall SetLength(int newLength)
    {
        wstring :: resize(newLength);
        return *this;
    };
    inline TWString & __fastcall StringOfChar(wchar_t ch, int count)
    {
        wstring :: assign(count, ch);
        return *this;
    };
    inline TWString __fastcall SubString(int index, int count = 0) const
    {
        return count ? wstring :: substr(index, count) : wstring :: substr(index);
    };
    int __fastcall ToInt(int defaultValue = INT_MAX, int base = 10) const;
    double ToDouble() const;
    inline TWString & Trim()
    {
        TrimLeft();
        TrimRight();
        return *this;
    };
    TWString & TrimLeft();
    TWString & TrimRight();
};

typedef TWString	TWideString;

template <typename C, typename T>
basic_string<C> __to_string(T val)
{
    basic_ostringstream<C> oss;
    oss << val;
    return oss.str();
}

template <typename T>
inline TString toString(T val)
{
    return __to_string<char, T>(val);
}

template <typename T>
inline TWideString toWideString(T val)
{
    return __to_string<wchar_t, T>(val);
}

template<typename C, typename T>
T __from_string(const basic_string<C>& s)
{
    basic_istringstream<C> iss(s);
    T res;
    iss >> res;
    return res;
}

template<typename T>
inline T fromString(const TString &s)
{
    return __from_string<char, T>(s);
}

template<typename T>
inline T fromString(const TWString &s)
{
    return __from_string<wchar_t, T>(s);
}
//---------------------------------------------------------------------------
//  Exception
//---------------------------------------------------------------------------
class Exception : public exception
{
    DECLARE_PROPERTY(Exception);

private:
    const char *m_msg;
    inline TString get_Message()
    {
        return TString( what() );
    };

public:
    Exception() : m_msg(0) { };
    __fastcall Exception(const char *msg) : m_msg(msg) { };
    virtual const char* what() const throw()
    {
        return m_msg? m_msg : exception::what();
    };

    property (r, TString, Message);
};
//---------------------------------------------------------------------------
//  TCommonList
//---------------------------------------------------------------------------
template <typename T>
class TCommonList : public list<T>
{
    DECLARE_PROPERTY(TCommonList);

private:
    inline void set_Count(int n)
    {
        list<T>::resize(n);
    };
    inline int get_Count()
    {
        return list<T>::size();
    };

    T &operator_index(const int index)
    {
        if(index >= Count)
            list<T>::resize(index + 1);

        if(index >= 0 && index < Count)
        {
            int i = 0;
            for(typename list<T>::const_iterator from = list<T>::begin();
                    from != list<T>::end();
                    ++from, i++)
            {
                if(i == index)
                    return const_cast<T &>(*from);
            }
        }
        return const_cast<T &>(*list<T>::end());
    }

    inline T get_Items(const int index)
    {
        return operator_index(index);
    }

    void set_Items(const int index, T item)
    {
        if(index >= Count)
            list<T>::resize(index + 1);
        if(index >= 0 && index < Count)
        {
            int i = 0;
            for( typename list<T>::iterator from = list<T>::begin();
                    from != list<T>::end();
                    ++from, i++)
            {
                if(i == index)
                {
                    *from = item;
                    return;
                }
            }
        }
    }

protected:

public:
    inline void __fastcall Add(const T &p)
    {
        list<T>::push_back(p);
    };
    inline void __fastcall Add(const TCommonList &lst)
    {
        list<T>::merge(lst);
    };
    void __fastcall Add(const T *p, int nitems)
    {
        for(register int i = 0; i < nitems; i++)
            list<T>::push_back(p[i]);
    }
    inline void Clear()
    {
        list<T>::clear();
    };
    void __fastcall Delete(int index)
    {
        int i = 0;
        for(typename list<T>::iterator from = list<T>::begin();
                from != list<T>::end();
                ++from, i++)
        {
            if(i == index)
            {
                list<T>::erase(from);
                return;
            }
        }
    }
    inline void __fastcall Delete(const T &item)
    {
        list<T>::remove(item);
    };
    inline bool Empty()
    {
        return list<T>::empty();
    };
    void __fastcall Exchange(int index1, int index2)
    {
        T item1, item2;

        if (index1 < 0 || index1 >= Count ||
                index2 < 0 || index2 >= Count)
            return;
        item1 = Items[index1];
        item2 = Items[index2];
        Items[index1] = item2;
        Items[index2] = item1;
    }
    inline T First()
    {
        return (list<T>::empty())? *list<T>::end() : list<T>::front();
    };
    int  __fastcall IndexOf(const T &item)
    {
        int i = 0;
        for(typename list<T>::iterator from = list<T>::begin();
                from != list<T>::end();
                ++from, i++)
        {
            if(*from == item)
                return i;
        }
        return -1;
    }
    void __fastcall Insert(int index, const T &item)
    {
        int i = 0;
        for(typename list<T>::iterator from = list<T>::begin();
                from != list<T>::end();
                ++from, i++)
        {
            if(i == index)
            {
                list<T>::insert(from, item);
                return;
            }
        }
    }
    inline T Last()
    {
        return (list<T>::empty())? *list<T>::end() : list<T>::back();
    };
    void __fastcall Move(int newindex, int curindex)
    {
        T item;

        if (curindex != newindex)
        {
            if (newindex < 0 || newindex >= Count)
                return;
            item = Items[curindex];
            Items[curindex] = *list<T>::end();
            Delete(curindex);
            Insert(newindex, *list<T>::end());
            Items[newindex] = item;
        }
    }
    inline void PopBack()
    {
        if(!list<T>::empty()) list<T>::pop_back();
    };
    inline void PopFront()
    {
        if(!list<T>::empty()) list<T>::pop_front();
    };
    inline void __fastcall PushBack(const T &p)
    {
        list<T>::push_back(p);
    };
    inline void __fastcall PushFront(const T &p)
    {
        list<T>::push_front(p);
    };
    inline void Reverse()
    {
        list<T>::reverse();
    };
    inline void Sort()
    {
        list<T>::sort();
    };
    inline void __fastcall Swap(const TCommonList &lst)
    {
        list<T>::swap(lst);
    };
    inline void Unique()
    {
        list<T>::unique();
    };

//   	inline T __fastcall operator[](const int index)  { return TCommonList::get_Items(index); };

    inline T & __fastcall operator[](const int index)
    {
        return operator_index(index);
    };
    inline const T & __fastcall operator[](const int index) const
    {
        return operator_index(index);
    };

    property (rw, int, Count);
    property_i (rw, T, int, Items);
};
//---------------------------------------------------------------------------
//  TList
//---------------------------------------------------------------------------
typedef TCommonList<void *> TList;

//class TList : public TCommonList<void *>
//{
//};
/*class TList : public list<void *>
{
	DECLARE_PROPERTY(TList);

private:
   inline void set_Count(int n) { resize(n);};
   inline int get_Count() { return size(); };
   void * get_Items(const int index);
   void set_Items(const int index, void *item);

protected:

public:
   __fastcall TList();
   __fastcall ~TList();
   inline void __fastcall Add(void *p)  { push_back(p); };
   inline void __fastcall Add(TList &lst) { merge(lst); };
   inline void __fastcall Clear() { clear(); };
          void __fastcall Delete(int index);
   inline void __fastcall Delete(void *item) { remove(item); };
   inline bool __fastcall Empty() { return empty(); };
          void __fastcall Exchange(int index1, int index2);
   inline void * __fastcall First() { return (empty())? NULL : front(); };
          int  __fastcall IndexOf(void *item);
          void __fastcall Insert(int index, void *item);
   inline void * __fastcall Last() { return (empty())? NULL : back(); };
          void __fastcall Move(int newindex, int curindex);
   inline void __fastcall Pack() { remove(NULL); };
   inline void __fastcall PopBack() { if(!empty()) pop_back(); };
   inline void __fastcall PopFront() { if(!empty()) pop_front(); };
   inline void __fastcall PushBack(void *p) { push_back(p); };
   inline void __fastcall PushFront(void *p) { push_front(p); };
   inline void __fastcall Reverse() { reverse(); };
   inline void __fastcall Sort() { sort(); };
   inline void __fastcall Swap(TList &lst) { swap(lst); };
   inline void __fastcall Unique() { unique(); };

   inline void * __fastcall operator[](const int index)  { return get_Items(index); };

   property (rw, int, Count);
   property_i (rw, void *, int, Items);
}; */
//---------------------------------------------------------------------------
//  TCommonStrings
//---------------------------------------------------------------------------
template <typename Ts, typename Tc>
class TCommonStrings : public TCommonList<Ts>
{
    DECLARE_PROPERTY(TCommonStrings);

private:
    Ts get_Text()
    {
        Ts result;
        Ts carriage;
        if(WIDECHAR)
            carriage = L"\n";
        else
            carriage = "\n";

        for(typename TCommonStrings::iterator from = TCommonStrings::begin();
                from != TCommonStrings::end();
                ++from)
        {
            result = result + *from + carriage;
        }
        return result.erase(result.length()-1);
    }
    void set_Text(Ts value)
    {
        Ts arg;
        if(WIDECHAR)
            arg = L"\n\r";
        else
            arg = "\n\r";
        typename Ts::size_type pos1, pos2;

        TCommonStrings::clear();
        pos1 = pos2 = 0;
        do
        {
            pos2 = value.find_first_of(arg, pos1);
            if(pos2 != Ts::npos)
            {
                this->push_back(value.substr(pos1, pos2 - pos1));
                pos1 = pos2 + 1;
            }
            else
            {
                pos2 = value.size();
                this->push_back(value.substr(pos1, pos2 - pos1));
                break;
            }
        }
        while(1);
    }
    Ts get_DelimitedText()
    {
        Ts lb, result, ld;
        lb += Delimiter;
        ld += QuoteChar;
        for(typename TCommonStrings::iterator from = TCommonStrings::begin();
                from != TCommonStrings::end();
                ++from)
        {
            result = result + ld + *from + ld + lb;
        }
        return result.erase(result.length()-1);
    }
    void set_DelimitedText(Ts value)
    {
        Ts arg;
        typename Ts::size_type pos1, pos2;

        TCommonStrings::clear();
        pos1 = pos2 = 0;
        do
        {
            pos2 = value.find_first_of(Delimiter, pos1);
            if(pos2 != Ts::npos)
            {
                this->push_back(value.substr(pos1, pos2 - pos1));
                pos1 = pos2 + 1;
            }
            else
            {
                this->push_back(value.substr(pos1));
                break;
            }
        }
        while(1);
    }
    Ts get_Names(const int index)
    {
        Ts tmp1, tmp2;
        ExtractNameValue(get_Strings(index), tmp1, tmp2);
        return tmp1;
    }
    const Ts &get_Strings(const int index)
    {
        if(index >= 0 && index < TCommonList<Ts>::Count)
        {
            int i = 0;
            for(typename TCommonStrings::iterator from = TCommonStrings::begin();
                    from != TCommonStrings::end();
                    ++from, i++)
            {
                if(i == index)
                    return *from;
            }
        }
        return *TCommonStrings::end();
    }
    void set_Strings(const int index, const Ts &s)
    {
        if(index >= 0 && index < TCommonList<Ts>::Count)
        {
            int i = 0;
            for(typename TCommonStrings::iterator from = TCommonStrings::begin();
                    from != TCommonStrings::end();
                    ++from, i++)
            {
                if(i == index)
                {
                    *from = s;
                    return;
                }
            }
        }
    }
    Ts get_Values(const Ts name)
    {
        Ts tmp1, tmp2;
        int i = 0;
        bool b;

        for(typename TCommonStrings::iterator from = TCommonStrings::begin();
                from != TCommonStrings::end();
                ++from, ++i)
        {
            b = ExtractNameValue(*from, tmp1, tmp2);
            if(b)
                continue;
            if(tmp1 == name)
                return tmp2;
        }
        return *TCommonStrings::end();
    }
    void set_Values(const Ts name, Ts value)
    {
        Ts emptystr;
        if(WIDECHAR)
            emptystr = L"";
        else
            emptystr = "";
        Ts eqstr;
        if(WIDECHAR)
            eqstr = L"=";
        else
            eqstr = "=";
        int i = IndexOfName(name);
        if(!value.empty())
        {
            if(i < 0)
                TCommonList<Ts>::Add(emptystr);
            Ts s(name + eqstr + value);
            set_Strings(i, s);
        }
        else
        {
            if(i >= 0)
                TCommonStrings::Delete(i);
        }
    }

public:
    enum { WIDECHAR = (sizeof(Tc) == sizeof(wchar_t)) };
    Tc Delimiter;
    Tc QuoteChar;
    Tc CommentChar;

    __fastcall TCommonStrings() : Delimiter(WIDECHAR ? L',' : ','),
        QuoteChar(WIDECHAR ? L'"' : '"'),
        CommentChar(WIDECHAR ? L'#' : '#')
    {
    }
    __fastcall TCommonStrings(Tc **strings, int nstrings) : Delimiter(WIDECHAR ? L',' : ','),
        QuoteChar(WIDECHAR ? L'"' : '"'),
        CommentChar(WIDECHAR ? L'#' : '#')
    {
        TCommonStrings::Add(strings, nstrings);
    }
    inline void Add(Tc *str)
    {
        TCommonStrings::push_back(str);
    };
    void Add(Tc **strings, int nstrings)
    {
        for(register int i = 0; i < nstrings; i++)
            TCommonStrings::push_back(strings[i]);
    }
    inline void Add(TCommonStrings &lst)
    {
        TCommonStrings::merge(lst);
    };
    void Assign(TCommonStrings &strings)
    {
        TCommonStrings::clear();
        QuoteChar = strings.QuoteChar;
        Delimiter = strings.Delimiter;
        CommentChar = strings.CommentChar;
        Add(strings);
    }
    bool ExtractNameValue(const Ts &str, Ts &name, Ts &value)
    {
        Ts tmp;
        typename Ts::size_type poscmnt, poseq;

        poseq = str.find(WIDECHAR ? L'=' : '=');
        if(poseq == Ts::npos)
            return true;
        poscmnt = str.find(CommentChar);
        if(poscmnt != Ts::npos && poscmnt < poseq)
            return true;
        name = str.substr(0, poseq);
        name.Trim();
        poseq += 1;
        if(poscmnt == Ts::npos)
            value = str.substr(poseq);
        else
            value = str.substr(poseq, poscmnt - poseq);
        value.Trim();
        return false;
    }
    inline int  Find(const Ts &str)
    {
        return TCommonStrings::IndexOf(str);
    };
    int  IndexOfName(const Ts &name)
    {
        Ts tmp1, tmp2;
        int i = 0;
        bool b;

        for(typename TCommonStrings::iterator from = TCommonStrings::begin();
                from != TCommonStrings::end();
                ++from, ++i)
        {
            b = ExtractNameValue(*from, tmp1, tmp2);
            if(b)
                continue;
            if(tmp1 == name)
                return i;
        }
        return -1;
    }
    inline void Pack()
    {
        TCommonStrings::remove(Ts());
    };

    property (rw, Ts, DelimitedText);
    property_i (r, Ts, int, Names);
    property_i (rw, const Ts &, int, Strings);
    property (rw, Ts, Text);
    property_i (rw, Ts, Ts, Values);
};
//---------------------------------------------------------------------------
//  TStrings
//---------------------------------------------------------------------------
typedef TCommonStrings<TString, char> TStrings;
typedef TCommonStrings<TString, char> TStringList;
/*
class TStrings : public TCommonList<TString>
{
	DECLARE_PROPERTY(TStrings);

private:
   TString get_DelimitedText();
   void set_DelimitedText(TString value);
   TString get_Text();
   void set_Text(TString value);
   const TString &get_Strings(const int index);
   void set_Strings(const int index, const TString &s);
   TString get_Names(const int index);
   TString get_Values(const TString name);
   void set_Values(const TString name, TString value);

protected:

public:
   	char Delimiter;
   	char QuoteChar;
   	char CommentChar;

   	__fastcall TStrings();
   	__fastcall TStrings(char **strings, int nstrings);
   	void Add(char **strings, int nstrings);
   	inline void Add(TStrings &lst) { merge(lst); };
	void Assign(TStrings &strings);

//          void Exchange(int index1, int index2);
          bool ExtractNameValue(const TString &str, TString &name, TString &value);
   inline int  Find(const TString &str) { return IndexOf(str);};
//   inline TString &First() { return front(); };
//          int  IndexOf(const TString &str);
          int  IndexOfName(const TString &name);
//          void Insert(int index, const TString &str);
//   inline TString &Last() { return back(); };
//          void Move(int newindex, int curindex);
   inline void Pack() { remove(TString()); };

//   inline TString &operator[](const int index) { return get_Strings(index); };

   property (rw, TString, DelimitedText);
   property (rw, TString, Text);
   property_i (rw, const TString &, int, Strings);
   property_i (r, TString, int, Names);
   property_i (rw, TString, TString, Values);
};
*/
/*
class TStrings : public list<TString>
{
	DECLARE_PROPERTY(TStrings);

private:
   inline int get_Count() { return size(); };
   inline void set_Count(int n) { resize(n);};
   TString get_DelimitedText();
   void set_DelimitedText(TString value);
   TString get_Text();
   void set_Text(TString value);
   TString &get_Strings(const int index);
   void set_Strings(const int index, TString &s);
   TString get_Names(const int index);
   TString get_Values(const TString name);
   void set_Values(const TString name, TString value);

protected:

public:
   char Delimiter;
   char QuoteChar;
   char CommentChar;

   __fastcall TStrings();
   __fastcall TStrings(char **strings, int nstrings);
   inline void Add(const TString &str)  { push_back(str); };
   inline void Add(TStrings &lst) { merge(lst); };
   		  void Add(char **strings, int nstrings);
          void Assign(TStrings &strings);
   inline void Clear() { clear(); };
          void Delete(int index);
   inline void Delete(const TString &str) { remove(str); };
   inline bool Empty() { return empty(); };
          void Exchange(int index1, int index2);
          bool ExtractNameValue(const TString &str, TString &name, TString &value);
   inline int  Find(const TString &str) { return IndexOf(str);};
   inline TString &First() { return front(); };
          int  IndexOf(const TString &str);
          int  IndexOfName(const TString &name);
          void Insert(int index, const TString &str);
   inline TString &Last() { return back(); };
          void Move(int newindex, int curindex);
   inline void Pack() { remove(TString()); };
   inline void PopBack() { if(!empty()) pop_back(); };
   inline void PopFront() { if(!empty()) pop_front(); };
   inline void PushBack(const TString &str) { push_back(str); };
   inline void PushFront(const TString &str) { push_front(str); };
   inline void Reverse() { reverse(); };
   inline void Sort() { sort(); };
   inline void Swap(TStrings &lst) { swap(lst); };
   inline void Unique() { unique(); };

   inline TString &operator[](const int index) { return get_Strings(index); };

   property (rw, int, Count);
   property (rw, TString, DelimitedText);
   property (rw, TString, Text);
   property_i (rw, TString &, int, Strings);
   property_i (r, TString, int, Names);
   property_i (rw, TString, TString, Values);
};
 */
/*
{ TStrings class }

  TStrings = class(TPersistent)
  private
  protected
  public
    function Equals(Strings: TStrings): Boolean;
    procedure LoadFromFile(const FileName: string); virtual;
    procedure LoadFromStream(Stream: TStream); virtual;
    procedure SaveToFile(const FileName: string); virtual;
    procedure SaveToStream(Stream: TStream); virtual;
  end;

*/
//---------------------------------------------------------------------------
//  TWStrings
//---------------------------------------------------------------------------
typedef TCommonStrings<TWString, wchar_t> TWStrings;
typedef TWStrings	TWideStrings;
//---------------------------------------------------------------------------
//  Utils
//---------------------------------------------------------------------------
TString __fastcall ChangeFileExt(const TString &filename, const TString &ext);
TString __fastcall ExtractFileExt(const TString &filename);
TString __fastcall ExtractFilePath(const TString &filename);
TString __fastcall ExtractFileName(const TString &filename);
void    SetSigChildHandler();
TString __fastcall TimeToString(time_t time);
inline TString Now()
{
    return TimeToString(time(NULL));
};
//---------------------------------------------------------------------------
}  //namespace
//---------------------------------------------------------------------------
#endif
