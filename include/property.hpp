//---------------------------------------------------------------------------
#ifndef __PROPERTY_HPP
#define __PROPERTY_HPP

//  class property_rw;
//  class property_r;
//  class property_irw;
//  class property_ir;
//  class property_mr;
//  class property_m;
//  class property_mm;
//  class property_ijrw;
//  class property_ijr;
#ifdef __LP64__
#define offset_type long
#else
#define offset_type int
#endif
//---------------------------------------------------------------------------
//  property_base
//---------------------------------------------------------------------------
class property_base
{
};
//---------------------------------------------------------------------------
//  property_common
//---------------------------------------------------------------------------
template< typename PropertyOwner, offset_type (*PropOffset)() >
class property_common : public property_base
{
protected:
    inline PropertyOwner* owner() const
    {
        return (PropertyOwner *)((char*)this - (*PropOffset)());
    };
};
//---------------------------------------------------------------------------
//  property_rw
//---------------------------------------------------------------------------
template<
typename PropertyOwner,
         offset_type (*PropOffset)(),
         typename PropertyType,
         PropertyType (PropertyOwner::*getter)(),
         void (PropertyOwner::*setter)(PropertyType) >
class property_rw : public property_common<PropertyOwner,PropOffset>
{
    typedef property_common<PropertyOwner,PropOffset> common;

public:
    operator const PropertyType() const
    {
        return (common::owner()->*getter)();
    }
    PropertyType operator*() const
    {
        return (common::owner()->*getter)();
    }
    PropertyType operator->() const
    {
        return (common::owner()->*getter)();
    }
    const property_rw & operator = ( const PropertyType value )
    {
        (common::owner()->*setter)( value );
        return *this;
    }
    // assign left operator using another property
    const property_rw & operator = ( const property_rw &prop )
    {
        (common::owner()->*setter)( PropertyType(prop) );
        return *this;
    }
    bool operator == (const PropertyType val)
    {
        return (common::owner()->*getter)() == val;
    }
    bool operator != (const PropertyType val)
    {
        return (common::owner()->*getter)() != val;
    }
//   friend bool operator == (const PropertyType val, property_rw &prop)
//   {
//      return ((PropertyOwner *)(prop.owner())->*getter)() == val;
//   }
//   friend bool operator != (const PropertyType val, property_rw &prop)
//   {
//      return ((PropertyOwner *)(prop.owner())->*getter)() != val;
//   }
//   friend bool operator == (const property_rw &prop1, const property_rw &prop2)
//   {
//      return ((PropertyOwner *)(prop1.owner())->*getter)() ==
//             ((PropertyOwner *)(prop2.owner())->*getter)();
//   }
//   friend bool operator != (const property_rw &prop1, const property &prop2)
//   {
//      return ((PropertyOwner *)(prop1.owner())->*getter)() !=
//             ((PropertyOwner *)(prop2.owner())->*getter)();
//   }

    void operator +=( const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)((p->*getter)() + data);
    }
    void operator -=( const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)((p->*getter)() - data);
    }
    void operator *=( const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)((p->*getter)() * data);
    }
    void operator /=( const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)((p->*getter)() / data);
    }
    void operator &=( const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)((p->*getter)() & data);
    }
    void operator |=( const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)((p->*getter)() | data);
    }
    PropertyType operator ++()        //prefix
    {
        PropertyOwner *p = common::owner();
        PropertyType val = (p->*getter)() + PropertyType(1);
        (p->*setter)(val);
        return val;
    }
    PropertyType const operator ++(int)  //having an int parameter are postfix operators
    {
        PropertyOwner *p = common::owner();
        PropertyType val = (p->*getter)();
        (p->*setter)(val + PropertyType(1));
        return val;
    }
    PropertyType operator --()        //prefix
    {
        PropertyOwner *p = common::owner();
        PropertyType val = (p->*getter)() - PropertyType(1);
        (p->*setter)(val);
        return val;
    }
    PropertyType const operator --(int)  //having an int parameter are postfix operators
    {
        PropertyOwner *p = common::owner();
        PropertyType val = (p->*getter)();
        (p->*setter)(val - PropertyType(1));
        return val;
    }
};

#define property_rw(type,name) \
		property_rw<__propowner,__propowner::__prop_offset_##name,type, \
					&__propowner::get_##name,&__propowner::set_##name> name
//---------------------------------------------------------------------------
//  property_r
//---------------------------------------------------------------------------
template<
typename PropertyOwner,
         offset_type (*PropOffset)(),
         typename PropertyType,
         PropertyType (PropertyOwner::*getter)() >
class property_r : public property_common<PropertyOwner,PropOffset>
{
    typedef property_common<PropertyOwner,PropOffset> common;

public:
    operator const PropertyType() const
    {
        return (common::owner()->*getter)();
    }
    PropertyType operator*() const
    {
        return (common::owner()->*getter)();
    }
    PropertyType operator->() const
    {
        return (common::owner()->*getter)();
    }
    bool operator == (const PropertyType val)
    {
        return (common::owner()->*getter)() == val;
    }

//	friend bool operator == (const PropertyType val, const property_r &prop)
//   {
//      return (prop.owner()->*getter)() == val;
//   }
//   friend bool operator == (const property_r &prop1, const property_r &prop2)
//   {
//      return (prop1.owner()->*getter)() ==
//             (prop2.owner()->*getter)();
//   }
};

#define property_r(type,name) \
		property_r<__propowner,__propowner::__prop_offset_##name,type,&__propowner::get_##name> name
//---------------------------------------------------------------------------
//  property_irw
//---------------------------------------------------------------------------
template<
typename PropertyOwner,
         offset_type (*PropOffset)(),
         typename PropertyType,
         typename PropertyIndex,
         PropertyType (PropertyOwner::*getter)(const PropertyIndex),
         void (PropertyOwner::*setter)(const PropertyIndex, PropertyType ) >
class property_irw : public property_common<PropertyOwner,PropOffset>
{
    typedef property_common<PropertyOwner,PropOffset> common;

private:
    PropertyIndex idx;

public:
    property_irw() : idx(PropertyIndex(0))
    {
    }
    property_irw & operator[](const PropertyIndex i)
    {
        idx = i;
        return *this;
    }
    const property_irw & operator[](const PropertyIndex i) const
    {
        idx = i;
        return *this;
    }
    operator const PropertyType() const
    {
        return (common::owner()->*getter)(idx);
    }
    PropertyType operator*() const
    {
        return (common::owner()->*getter)(idx);
    }
    PropertyType operator->() const
    {
        return (common::owner()->*getter)(idx);
    }
    const property_irw & operator = ( const PropertyType value )
    {
        (common::owner()->*setter)(idx, value );
        return *this;
    }
    // assign left operator using another property
//    const property_irw & operator = ( const property_irw &prop )
//    {
//        (common::owner()->*setter)(idx, (PropertyType)prop );
//        return *this;
//    }
    bool operator == (const PropertyType val)
    {
        return (common::owner()->*getter)(idx) == val;
    }
//   friend bool operator == (const PropertyType & val, const property_a & prop)
//   {
//      return ((PropertyOwner *)prop.owner()->*getter)(prop.idx) == val;
//   }
//	friend bool operator == (const property_a & prop1, const property_a & prop2)
//   	{
//    	return ((PropertyOwner *)prop1.owner()->*getter)(prop1.idx) ==
//        		((PropertyOwner *)prop2.owner()->*getter)(prop2.idx);
//   	}
    void operator +=( const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)(idx,(p->*getter)(idx) + data);
    }
    void operator -=( const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)(idx,(p->*getter)(idx) - data);
    }
    void operator *=( const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)(idx,(p->*getter)(idx) * data);
    }
    void operator /=( const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)(idx,(p->*getter)(idx) / data);
    }
    void operator &=( const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)(idx,(p->*getter)(idx) & data);
    }
    void operator |=( const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)(idx,(p->*getter)(idx) | data);
    }
    PropertyType operator ++()        //prefix
    {
        PropertyOwner *p = common::owner();
        PropertyType val = (p->*getter)(idx) + PropertyType(1);
        (p->*setter)(idx,val);
        return val;
    }
    PropertyType const operator ++(int)  //having an int parameter are postfix operators
    {
        PropertyOwner *p = common::owner();
        PropertyType val = (p->*getter)(idx);
        (p->*setter)(idx,val + PropertyType(1));
        return val;
    }
    PropertyType operator --()        //prefix
    {
        PropertyOwner *p = common::owner();
        PropertyType val = (p->*getter)(idx) - PropertyType(1);
        (p->*setter)(idx,val);
        return val;
    }
    PropertyType const operator --(int)  //having an int parameter are postfix operators
    {
        PropertyOwner *p = common::owner();
        PropertyType val = (p->*getter)(idx);
        (p->*setter)(idx,val - PropertyType(1));
        return val;
    }
};

#define property_irw(type,idx,name) \
		property_irw<__propowner,__propowner::__prop_offset_##name,type,idx, \
					&__propowner::get_##name,&__propowner::set_##name> name
//---------------------------------------------------------------------------
//  property_ir
//---------------------------------------------------------------------------
template<
typename PropertyOwner,
         offset_type (*PropOffset)(),
         typename PropertyType,
         typename PropertyIndex,
         PropertyType (PropertyOwner::*getter)(const PropertyIndex) >
class property_ir : public property_common<PropertyOwner,PropOffset>
{
    typedef property_common<PropertyOwner,PropOffset> common;

private:
    PropertyIndex idx;

public:
    property_ir() : idx(PropertyIndex(0))
    {
    }
    property_ir & operator[](const PropertyIndex i)
    {
        idx = i;
        return *this;
    }
    const property_ir & operator[](const PropertyIndex i) const
    {
        idx = i;
        return *this;
    }
    operator const PropertyType() const
    {
        return (common::owner()->*getter)(idx);
    }
    PropertyType operator*() const
    {
        return (common::owner()->*getter)(idx);
    }
    PropertyType operator->() const
    {
        return (common::owner()->*getter)(idx);
    }
    bool operator == (const PropertyType val)
    {
        return (common::owner()->*getter)(idx) == val;
    }
//   friend bool operator == (const PropertyType val, const property_ir &prop)
//   {
//      return (prop.owner()->*getter)(prop.idx) == val;
//   }
//   friend bool operator == (const property_ar &prop1, const property_ar &prop2)
//   {
//      return (prop1.owner()->*getter)(prop1.idx) ==
//             (prop2.owner()->*getter)(prop2.idx);
//   }
};

#define property_ir(type,idx,name) \
		property_ir<__propowner,__propowner::__prop_offset_##name,type,idx, \
					&__propowner::get_##name> name
//---------------------------------------------------------------------------
//  property_mr
//---------------------------------------------------------------------------
template<
typename PropertyOwner,
         offset_type (*PropOffset)(),
         typename PropertyType,
         PropertyType PropertyOwner::*mget >
class property_mr : public property_common<PropertyOwner,PropOffset>
{
    typedef property_common<PropertyOwner,PropOffset> common;

public:
    operator const PropertyType() const
    {
        return common::owner()->*mget;
    }
    PropertyType operator*() const
    {
        return common::owner()->*mget;
    }
    PropertyType operator->() const
    {
        return common::owner()->*mget;
    }
    bool operator == (const PropertyType val)
    {
        return common::owner()->*mget == val;
    }
//   	friend bool operator == (const PropertyType val, const property_mr &prop)
//   	{
//    	return prop.owner()->*mget == val;
//   	}
    // comparision property and property
//   	friend bool operator == (const property_mr &prop1, const property_mr &prop2)
//   	{
//    	return prop1.owner()->*mget ==
//             prop2.owner()->*mget;
//   	}
};

#define property_mr(type,name) \
		property_mr<__propowner,__propowner::__prop_offset_##name,type,&__propowner::m_##name> name
//---------------------------------------------------------------------------
//  property_m
//---------------------------------------------------------------------------
template <
typename PropertyOwner,
         offset_type (*PropOffset)(),
         typename PropertyType,
         PropertyType PropertyOwner::*mget,
         void (PropertyOwner::*setter)(PropertyType) >
class property_m : public property_common<PropertyOwner,PropOffset>
{
    typedef property_common<PropertyOwner,PropOffset> common;

public:
    operator const PropertyType() const
    {
        return common::owner()->*mget;
    }
    PropertyType operator*() const
    {
        return common::owner()->*mget;
    }
    PropertyType operator->() const
    {
        return common::owner()->*mget;
    }
    const property_m & operator = ( const PropertyType value )
    {
        (common::owner()->*setter)( value );
        return *this;
    }
    // assign left operator using another property
    const property_m & operator = ( const property_m &prop )
    {
        (common::owner()->*setter)( PropertyType( prop ) );
        return *this;
    }
//    const property_m & operator = ( const property_base &prop )
//    {
//        (common::owner()->*setter)( PropertyType(prop) );
//        return *this;
//    }
    bool operator == (const PropertyType val)
    {
        return common::owner()->*mget == val;
    }
    bool operator != (const PropertyType val)
    {
        return common::owner()->*mget != val;
    }
    // comparision value and property
//   friend bool operator == (const PropertyType val, property_m &prop)
//   {
//      return (PropertyOwner *)(prop.owner())->*mget == val;
//   }
    friend bool operator != (const PropertyType val, property_m &prop)
    {
        return (PropertyOwner *)(prop.owner())->*mget != val;
    }
    // comparision property and property
//   friend bool operator == (const property_m &prop1, const property_m &prop2)
//   {
//      return (PropertyOwner *)(prop1.owner())->*mget ==
//             (PropertyOwner *)(prop2.owner())->*mget;
//   }
    /*
    PropertyType operator+(const PropertyType data)   // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
        PropertyType val;
     //   PropertyOwner *p = common::owner();
     //   (p->*setter)(p->*mget + data);
        return val;
    }
    PropertyType operator-(const PropertyType data)   // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
        PropertyType val;
    //    PropertyOwner *p = common::owner();
    //    (p->*setter)(p->*mget - data);
        return val;
    }
    */
//	friend bool operator != (const property_m &prop1, const property_m &prop2)
//   	{
//    	return (PropertyOwner *)(prop1.owner())->*mget !=
//             (PropertyOwner *)(prop2.owner())->*mget;
//   	}
    /* friend PropertyType operator - (const property_m &prop1, const PropertyType data)
       	{
       	    PropertyType val;
    //    	return (PropertyOwner *)(prop1.owner())->*mget !=
    //             (PropertyOwner *)(prop2.owner())->*mget;
            return val;
       	}
    */
    void operator +=(const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)(p->*mget + data);
    }
    void operator -=(const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)(p->*mget - data);
    }
    void operator *=(const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)(p->*mget * data);
    }
    void operator /=(const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)(p->*mget / data);
    }
    void operator &=(const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)(p->*mget & data);
    }
    void operator |=(const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)(p->*mget | data);
    }
    PropertyType operator ++()        //prefix
    {
        PropertyOwner *p = common::owner();
        PropertyType val = p->*mget + PropertyType(1);
        (p->*setter)(val);
        return val;
    }
    PropertyType const operator ++(int)  //having an int parameter are postfix operators
    {
        PropertyOwner *p = common::owner();
        PropertyType val = p->*mget;
        (p->*setter)(val + PropertyType(1));
        return val;
    }
    PropertyType operator --()        //prefix
    {
        PropertyOwner *p = common::owner();
        PropertyType val = p->*mget - PropertyType(1);
        (p->*setter)(val);
        return val;
    }
    PropertyType const operator --(int)  //having an int parameter are postfix operators
    {
        PropertyOwner *p = common::owner();
        PropertyType val = p->*mget;
        (p->*setter)(val - PropertyType(1));
        return val;
    }
};

#define property_m(type,name) \
		property_m<__propowner,__propowner::__prop_offset_##name,type, \
					&__propowner::m_##name,&__propowner::set_##name> name
//---------------------------------------------------------------------------
//  property_mm
//---------------------------------------------------------------------------
template <
typename PropertyOwner,
         offset_type (*PropOffset)(),
         typename PropertyType,
         PropertyType PropertyOwner::*mget,
         PropertyType PropertyOwner::*mset >
class property_mm : public property_common<PropertyOwner,PropOffset>
{
    typedef property_common<PropertyOwner,PropOffset> common;

public:
    operator const PropertyType() const
    {
        return common::owner()->*mget;
    }
    PropertyType operator*() const
    {
        return common::owner()->*mget;
    }
    PropertyType operator->() const
    {
        return common::owner()->*mget;
    }
    const property_mm & operator = ( const PropertyType value )
    {
        common::owner()->*mset = value;
        return *this;
    }
    // assign left operator using another property
    const property_mm & operator = ( const property_mm &prop )
    {
        common::owner()->*mset = PropertyType(prop);
        return *this;
    }

    bool operator == (const PropertyType val)
    {
        return common::owner()->*mget == val;
    }
    bool operator != (const PropertyType val)
    {
        return common::owner()->*mget != val;
    }
    // comparision value and property
//   friend bool operator == (const PropertyType val, property_mm &prop)
//   {
//      return (PropertyOwner *)(prop.owner())->*mget == val;
//   }
//   friend bool operator != (const PropertyType val, property_mm &prop)
//   {
//      return (PropertyOwner *)(prop.owner())->*mget != val;
//   }
    // comparision property and property
//   friend bool operator == (const property_m &prop1, const property_m &prop2)
//   {
//      return (PropertyOwner *)(prop1.owner())->*mget ==
//             (PropertyOwner *)(prop2.owner())->*mget;
//   }
//   friend bool operator != (const property_m &prop1, const property_m &prop2)
//   {
//      return (PropertyOwner *)(prop1.owner())->*mget !=
//             (PropertyOwner *)(prop2.owner())->*mget;
//   }
    void operator +=(const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        p->*mset = p->*mget + data;
    }
    void operator -=(const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        p->*mset = p->*mget - data;
    }
    void operator *=(const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        p->*mset = p->*mget * data;
    }
    void operator /=(const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        p->*mset = p->*mget / data;
    }
    void operator &=(const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        p->*mset = p->*mget & data;
    }
    void operator |=(const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        p->*mset = p->*mget | data;
    }
    PropertyType operator ++()        //prefix
    {
        PropertyOwner *p = common::owner();
        PropertyType val = p->*mget + PropertyType(1);
        p->*mset = val;
        return val;
    }
    PropertyType const operator ++(int)  //having an int parameter are postfix operators
    {
        PropertyOwner *p = common::owner();
        PropertyType val = p->*mget;
        p->*mset = val + PropertyType(1);
        return val;
    }
    PropertyType operator --()        //prefix
    {
        PropertyOwner *p = common::owner();
        PropertyType val = p->*mget - PropertyType(1);
        p->*mset = val;
        return val;
    }
    PropertyType const operator --(int)  //having an int parameter are postfix operators
    {
        PropertyOwner *p = common::owner();
        PropertyType val = p->*mget;
        p->*mset = val - PropertyType(1);
        return val;
    }
};

#define property_mm(type,name) \
		property_mm<__propowner,__propowner::__prop_offset_##name,type, \
					&__propowner::m_##name,&__propowner::m_##name> name

//---------------------------------------------------------------------------
//  property_ijrw
//---------------------------------------------------------------------------
template<
typename PropertyOwner,
         offset_type (*PropOffset)(),
         typename PropertyType,
         typename PropertyIndex1,
         typename PropertyIndex2,
         PropertyType (PropertyOwner::*getter)(const PropertyIndex1, const PropertyIndex2),
         void (PropertyOwner::*setter)(const PropertyIndex1, const PropertyIndex2, PropertyType ) >
class property_ijrw : public property_common<PropertyOwner,PropOffset>
{
    typedef property_common<PropertyOwner,PropOffset> common;

private:
    class array1d
    {
    private:
        property_ijrw *m_;

    public:
        array1d(property_ijrw *val) : m_(val)
        {
        }
        property_ijrw & operator[](const PropertyIndex2 i)
        {
            m_->idx2 = i;
            return *m_;
        }
        const property_ijrw & operator[](const PropertyIndex2 i) const
        {
            m_->idx2 = i;
            return *m_;
        }
    };
    PropertyIndex1 idx1;
    PropertyIndex2 idx2;
    array1d a_;

public:
    property_ijrw() : idx1(PropertyIndex1(0)), idx2(PropertyIndex2(0)), a_(this)
    {
    }
    array1d &operator[](const PropertyIndex1 i)
    {
        idx1 = i;
        return a_;
    }
    const array1d &operator[](const PropertyIndex1 i) const
    {
        idx1 = i;
        return a_;
    }
    operator const PropertyType() const
    {
        return (common::owner()->*getter)(idx1, idx2);
    }
    PropertyType operator*() const
    {
        return (common::owner()->*getter)(idx1, idx2);
    }
    PropertyType operator->() const
    {
        return (common::owner()->*getter)(idx1, idx2);
    }
    const property_ijrw & operator = ( const PropertyType value )
    {
        (common::owner()->*setter)(idx1, idx2, value );
        return *this;
    }
    // assign left operator using another property
    const property_ijrw & operator = ( const property_ijrw &prop )
    {
        (common::owner()->*setter)(idx1, idx2, PropertyType(prop) );
        return *this;
    }
    bool operator == (const PropertyType val)
    {
        return (common::owner()->*getter)(idx1, idx2) == val;
    }
//   friend bool operator == (const PropertyType & val, const property_a & prop)
//   {
//      return ((PropertyOwner *)prop.owner()->*getter)(prop.idx) == val;
//   }
//	friend bool operator == (const property_a & prop1, const property_a & prop2)
//   	{
//    	return ((PropertyOwner *)prop1.owner()->*getter)(prop1.idx) ==
//        		((PropertyOwner *)prop2.owner()->*getter)(prop2.idx);
//   	}
    void operator +=( const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)(idx1, idx2,(p->*getter)(idx1, idx2) + data);
    }
    void operator -=( const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)(idx1, idx2,(p->*getter)(idx1, idx2) - data);
    }
    void operator *=( const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)(idx1, idx2,(p->*getter)(idx1, idx2) * data);
    }
    void operator /=( const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)(idx1, idx2,(p->*getter)(idx1, idx2) / data);
    }
    void operator &=( const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)(idx1, idx2,(p->*getter)(idx1, idx2) & data);
    }
    void operator |=( const PropertyType data)
    {
        PropertyOwner *p = common::owner();
        (p->*setter)(idx1, idx2,(p->*getter)(idx1, idx2) | data);
    }
    PropertyType operator ++()        //prefix
    {
        PropertyOwner *p = common::owner();
        PropertyType val = (p->*getter)(idx1, idx2) + PropertyType(1);
        (p->*setter)(idx1, idx2,val);
        return val;
    }
    PropertyType const operator ++(int)  //having an int parameter are postfix operators
    {
        PropertyOwner *p = common::owner();
        PropertyType val = (p->*getter)(idx1, idx2);
        (p->*setter)(idx1, idx2,val + PropertyType(1));
        return val;
    }
    PropertyType operator --()        //prefix
    {
        PropertyOwner *p = common::owner();
        PropertyType val = (p->*getter)(idx1, idx2) - PropertyType(1);
        (p->*setter)(idx1, idx2,val);
        return val;
    }
    PropertyType const operator --(int)  //having an int parameter are postfix operators
    {
        PropertyOwner *p = common::owner();
        PropertyType val = (p->*getter)(idx1, idx2);
        (p->*setter)(idx1, idx2,val - PropertyType(1));
        return val;
    }
};

#define property_ijrw(type,idx1,idx2,name) \
		property_ijrw<__propowner,__propowner::__prop_offset_##name,type,idx1,idx2, \
					&__propowner::get_##name,&__propowner::set_##name> name

//---------------------------------------------------------------------------
//  property_ijr
//---------------------------------------------------------------------------
template<
typename PropertyOwner,
         offset_type (*PropOffset)(),
         typename PropertyType,
         typename PropertyIndex1,
         typename PropertyIndex2,
         PropertyType (PropertyOwner::*getter)(const PropertyIndex1, const PropertyIndex2) >
class property_ijr : public property_common<PropertyOwner,PropOffset>
{
    typedef property_common<PropertyOwner,PropOffset> common;

private:
    class array1d
    {
    private:
        property_ijr *m_;

    public:
        array1d(property_ijr *val) : m_(val)
        {
        }
        property_ijr & operator[](const PropertyIndex2 i)
        {
            m_->idx2 = i;
            return *m_;
        }
        const property_ijr & operator[](const PropertyIndex2 i) const
        {
            m_->idx2 = i;
            return *m_;
        }
    };
    PropertyIndex1 idx1;
    PropertyIndex2 idx2;
    array1d a_;

public:
    property_ijr() : a_(this), idx1(PropertyIndex1(0)), idx2(PropertyIndex2(0))
    {
    }
    array1d &operator[](const PropertyIndex1 i)
    {
        idx1 = i;
        return a_;
    }
    const array1d &operator[](const PropertyIndex1 i) const
    {
        idx1 = i;
        return a_;
    }
    operator const PropertyType() const
    {
        return (common::owner()->*getter)(idx1, idx2);
    }
    PropertyType operator*() const
    {
        return (common::owner()->*getter)(idx1, idx2);
    }
    PropertyType operator->() const
    {
        return (common::owner()->*getter)(idx1, idx2);
    }
    bool operator == (const PropertyType val)
    {
        return (common::owner()->*getter)(idx1, idx2) == val;
    }
//   friend bool operator == (const PropertyType val, const property_ir &prop)
//   {
//      return (prop.owner()->*getter)(prop.idx) == val;
//   }
//   friend bool operator == (const property_ar &prop1, const property_ar &prop2)
//   {
//      return (prop1.owner()->*getter)(prop1.idx) ==
//             (prop2.owner()->*getter)(prop2.idx);
//   }
};

#define property_ijr(type,idx1,idx2,name) \
		property_ijr<__propowner,__propowner::__prop_offset_##name,type,idx1,idx2, \
					&__propowner::get_##name> name

//---------------------------------------------------------------------------
//  main macros
//---------------------------------------------------------------------------
#define property(access,type,name) \
		inline static offset_type __prop_offset_##name() { return (offset_type)&(((__propowner*)4)->name) - 4; }; \
		friend class property_common<__propowner,__propowner::__prop_offset_##name>; \
		property_##access(type,name)

#define property_i(access,type,idx,name) \
		inline static offset_type __prop_offset_##name() { return (offset_type)&(((__propowner*)4)->name) - 4; }; \
		friend class property_common<__propowner,__propowner::__prop_offset_##name>; \
		property_i##access(type,idx,name)

#define property_ij(access,type,idx1,idx2,name) \
		inline static offset_type __prop_offset_##name() { return (offset_type)&(((__propowner*)4)->name) - 4; }; \
		friend class property_common<__propowner,__propowner::__prop_offset_##name>; \
		property_ij##access(type,idx1,idx2,name)

/*
template <
	typename PropertyOwner,
	typename PropertyType,
	PropertyType (PropertyOwner::*getter)(),
	void (PropertyOwner::*setter)(PropertyType) >
std::ostream & operator << ( std::ostream & os, property<PropertyOwner,PropertyType,getter,setter> prop )
{
	return os << (PropertyType)prop;
}

template <
	typename PropertyOwner,
	typename PropertyType,
	PropertyType (PropertyOwner::*getter)(),
	void (PropertyOwner::*setter)(PropertyType) >
std::istream & operator >> ( std::istream & is, property<PropertyOwner,PropertyType,getter,setter> prop )
{
	PropertyType value;
	is >> value;
	prop = value;
	return is;
}
*/
#define DECLARE_PROPERTY( cls )	typedef cls __propowner

#endif
//---------------------------------------------------------------------------

