/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef UnitVector3f_h
#define UnitVector3f_h


#include "Vector3f.hpp"




#include "hxa7241_graphics.hpp"
namespace hxa7241_graphics
{


/**
 * A unitized 3D vector.<br/><br/>
 *
 * @invariant
 * v_m is unitized or zero
 */
class UnitVector3f
{
/// standard object services ---------------------------------------------------
public:
                     UnitVector3f();
   explicit          UnitVector3f( const Vector3f& );

                    ~UnitVector3f();
                     UnitVector3f( const UnitVector3f& );
           UnitVector3f& operator=( const UnitVector3f& );
private:
                     UnitVector3f( bool dummy,
                                   const Vector3f& );
                     UnitVector3f( float x,
                                   float y,
                                   float z );
public:

/// commands -------------------------------------------------------------------
           UnitVector3f&   set( const Vector3f& );

           UnitVector3f&   negateEq();
           UnitVector3f&   crossEq ( const UnitVector3f& );


/// queries --------------------------------------------------------------------
           const Vector3f& get()                                          const;

           UnitVector3f    operator-()                                    const;
           UnitVector3f    cross    ( const UnitVector3f& )               const;

           bool            operator==( const UnitVector3f& )              const;
           bool            operator!=( const UnitVector3f& )              const;
           bool            isZero()                                       const;


/// constants ------------------------------------------------------------------
   static const UnitVector3f& ZERO();
   static const UnitVector3f& X();
   static const UnitVector3f& Y();
   static const UnitVector3f& Z();


/// fields ---------------------------------------------------------------------
private:
   Vector3f v_m;
};








/// INLINES ///


/// commands -------------------------------------------------------------------
inline
UnitVector3f& UnitVector3f::set
(
   const Vector3f& v
)
{
   v_m = v;
   v_m.unitizeEq();

   return *this;
}


inline
UnitVector3f& UnitVector3f::negateEq()
{
   v_m.negateEq();

   return *this;
}


inline
UnitVector3f& UnitVector3f::crossEq
(
   const UnitVector3f& other
)
{
   v_m.crossEq( other.v_m );

   return *this;
}




/// standard object services ---------------------------------------------------
inline
UnitVector3f::UnitVector3f()
 : v_m()
{
}


inline
UnitVector3f::UnitVector3f
(
   const Vector3f& v
)
 : v_m()
{
   UnitVector3f::set( v );
}


inline
UnitVector3f::~UnitVector3f()
{
}


inline
UnitVector3f::UnitVector3f
(
   const UnitVector3f& other
)
 : v_m( other.v_m )
{
}


inline
UnitVector3f& UnitVector3f::operator=
(
   const UnitVector3f& other
)
{
   v_m = other.v_m;

   return *this;
}


inline
UnitVector3f::UnitVector3f
(
   const bool      ,//dummy,
   const Vector3f& v
)
 : v_m( v )
{
}


inline
UnitVector3f::UnitVector3f
(
   const float x,
   const float y,
   const float z
)
 : v_m( x, y, z )
{
}




/// queries --------------------------------------------------------------------
inline
const Vector3f& UnitVector3f::get() const
{
   return v_m;
}


inline
UnitVector3f UnitVector3f::operator-() const
{
   return UnitVector3f( false, -v_m );
}


inline
UnitVector3f UnitVector3f::cross
(
   const UnitVector3f& other
) const
{
   return UnitVector3f( false, v_m.cross( other.v_m ) );
}


inline
bool UnitVector3f::operator==
(
   const UnitVector3f& other
) const
{
   return v_m == other.v_m;
}


inline
bool UnitVector3f::operator!=
(
   const UnitVector3f& other
) const
{
   return v_m != other.v_m;
}


inline
bool UnitVector3f::isZero() const
{
   return v_m.isZero();
}


}//namespace




#endif//UnitVector3f_h
