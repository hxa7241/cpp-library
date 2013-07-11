/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef Coord2_h
#define Coord2_h


#include <limits>
#include <math.h>




#include "hxa7241_graphics.hpp"
namespace hxa7241_graphics
{


/**
 * 2D coordinate class template.
 */
template<class TYPE>
class Coord2
{
/// standard object services ---------------------------------------------------
public:
                    Coord2();
   explicit         Coord2( TYPE both );
                    Coord2( TYPE x,
                            TYPE y );
   explicit         Coord2( const TYPE xy[2] );

                   ~Coord2();
                    Coord2( const Coord2& );
           Coord2&  operator=( const Coord2& );


/// commands -------------------------------------------------------------------
           Coord2&  set( TYPE both );
           Coord2&  set( TYPE x,
                         TYPE y );
           Coord2&  set( const TYPE xy[2] );

           Coord2&  negateEq ();
           Coord2&  absEq    ();

           Coord2&  operator+=( const Coord2& );
           Coord2&  operator-=( const Coord2& );
           Coord2&  operator*=( const Coord2& );
           Coord2&  operator/=( const Coord2& );
           Coord2&  operator*=( TYPE );
           Coord2&  operator/=( TYPE );

           Coord2&  clampMin( const Coord2& );
           Coord2&  clampMax( const Coord2& );
           Coord2&  clamp   ( const Coord2& min,
                              const Coord2& max );


/// queries --------------------------------------------------------------------
           void     get( TYPE& x,
                         TYPE& y )                                        const;
           void     get( TYPE xy[2] )                                     const;

           TYPE     getX()                                                const;
           TYPE     getY()                                                const;
           TYPE     operator[]( int )                                     const;

           Coord2   operator-()                                           const;
           Coord2   abs()                                                 const;

           Coord2<dword> toInt()                                          const;
           Coord2<float> toFp()                                           const;

           Coord2   operator+( const Coord2& )                            const;
           Coord2   operator-( const Coord2& )                            const;
           Coord2   operator*( const Coord2& )                            const;
           Coord2   operator/( const Coord2& )                            const;
           Coord2   operator*( TYPE )                                     const;
           Coord2   operator/( TYPE )                                     const;

           bool     operator==( const Coord2& )                           const;
           bool     operator!=( const Coord2& )                           const;
           bool     isZero()                                              const;

           // returning vectors of TYPE( -1 or 0 or +1 )
           Coord2   sign   ()                                             const;
           Coord2   compare( const Coord2& )                              const;

           Coord2   equal     ( const Coord2& )                           const;
           Coord2   operator> ( const Coord2& )                           const;
           Coord2   operator>=( const Coord2& )                           const;
           Coord2   operator< ( const Coord2& )                           const;
           Coord2   operator<=( const Coord2& )                           const;

           Coord2   getClamped( const Coord2& min,
                                const Coord2& max )                       const;

           bool     isPositive()                                          const;
           TYPE     product()                                             const;


/// constants ------------------------------------------------------------------
   static const Coord2& ZERO();
   static const Coord2& ONE();
   static const Coord2& MIN();
   static const Coord2& MAX();
   static const Coord2& X();
   static const Coord2& Y();


/// fields ---------------------------------------------------------------------
private:
   TYPE xy_m[2];
};








/// aliases
typedef Coord2<dword> Coord2i;
typedef Coord2<float> Coord2f;




/// friends
template<class TYPE>
Coord2<TYPE> operator*( TYPE, const Coord2<TYPE>& );
template<class TYPE>
Coord2<TYPE> operator/( TYPE, const Coord2<TYPE>& );




/// TEMPLATES ///


/// standard object services ---------------------------------------------------
template<class TYPE>
Coord2<TYPE>::Coord2()
{
   xy_m[0] = TYPE();
   xy_m[1] = TYPE();
}


template<class TYPE>
inline
Coord2<TYPE>::Coord2
(
   const TYPE both
)
{
   Coord2::set( both );
}


template<class TYPE>
inline
Coord2<TYPE>::Coord2
(
   const TYPE x,
   const TYPE y
)
{
   Coord2::set( x, y );
}


template<class TYPE>
inline
Coord2<TYPE>::Coord2
(
   const TYPE xy[2]
)
{
   Coord2::set( xy );
}


template<class TYPE>
inline
Coord2<TYPE>::~Coord2()
{
}


template<class TYPE>
inline
Coord2<TYPE>::Coord2
(
   const Coord2& other
)
{
   Coord2::operator=( other );
}


template<class TYPE>
Coord2<TYPE>& Coord2<TYPE>::operator=
(
   const Coord2& other
)
{
   xy_m[0] = other.xy_m[0];
   xy_m[1] = other.xy_m[1];

   return *this;
}




/// commands -------------------------------------------------------------------
template<class TYPE>
Coord2<TYPE>& Coord2<TYPE>::set
(
   const TYPE both
)
{
   xy_m[0] = both;
   xy_m[1] = both;

   return *this;
}


template<class TYPE>
Coord2<TYPE>& Coord2<TYPE>::set
(
   const TYPE x,
   const TYPE y
)
{
   xy_m[0] = x;
   xy_m[1] = y;

   return *this;
}


template<class TYPE>
Coord2<TYPE>& Coord2<TYPE>::set
(
   const TYPE xy[2]
)
{
   xy_m[0] = xy[0];
   xy_m[1] = xy[1];

   return *this;
}


template<class TYPE>
Coord2<TYPE>& Coord2<TYPE>::negateEq()
{
   xy_m[0] = -xy_m[0];
   xy_m[1] = -xy_m[1];

   return *this;
}


template<class TYPE>
Coord2<TYPE>& Coord2<TYPE>::absEq()
{
   if( xy_m[0] < TYPE() )
   {
      xy_m[0] = -xy_m[0];
   }
   if( xy_m[1] < TYPE() )
   {
      xy_m[1] = -xy_m[1];
   }

   return *this;
}


template<class TYPE>
Coord2<TYPE>& Coord2<TYPE>::operator+=
(
   const Coord2& c
)
{
   xy_m[0] += c.xy_m[0];
   xy_m[1] += c.xy_m[1];

   return *this;
}


template<class TYPE>
Coord2<TYPE>& Coord2<TYPE>::operator-=
(
   const Coord2& c
)
{
   xy_m[0] -= c.xy_m[0];
   xy_m[1] -= c.xy_m[1];

   return *this;
}


template<class TYPE>
Coord2<TYPE>& Coord2<TYPE>::operator*=
(
   const Coord2& c
)
{
   xy_m[0] *= c.xy_m[0];
   xy_m[1] *= c.xy_m[1];

   return *this;
}


template<class TYPE>
Coord2<TYPE>& Coord2<TYPE>::operator/=
(
   const Coord2& c
)
{
   xy_m[0] /= c.xy_m[0];
   xy_m[1] /= c.xy_m[1];

   return *this;
}


template<class TYPE>
Coord2<TYPE>& Coord2<TYPE>::operator*=
(
   const TYPE n
)
{
   xy_m[0] *= n;
   xy_m[1] *= n;

   return *this;
}


template<class TYPE>
Coord2<TYPE>& Coord2<TYPE>::operator/=
(
   const TYPE n
)
{
   xy_m[0] /= n;
   xy_m[1] /= n;

   return *this;
}


template<class TYPE>
Coord2<TYPE>& Coord2<TYPE>::clampMin
(
   const Coord2& min
)
{
   if( xy_m[0] < min.xy_m[0] )
   {
      xy_m[0] = min.xy_m[0];
   }
   if( xy_m[1] < min.xy_m[1] )
   {
      xy_m[1] = min.xy_m[1];
   }

   return *this;
}


template<class TYPE>
Coord2<TYPE>& Coord2<TYPE>::clampMax
(
   const Coord2& max
)
{
   if( xy_m[0] > max.xy_m[0] )
   {
      xy_m[0] = max.xy_m[0];
   }
   if( xy_m[1] > max.xy_m[1] )
   {
      xy_m[1] = max.xy_m[1];
   }

   return *this;
}


template<class TYPE>
Coord2<TYPE>& Coord2<TYPE>::clamp
(
   const Coord2& min,
   const Coord2& max
)
{
   if( xy_m[0] > max.xy_m[0] )
      xy_m[0] = max.xy_m[0];
   else
   if( xy_m[0] < min.xy_m[0] )
      xy_m[0] = min.xy_m[0];

   if( xy_m[1] > max.xy_m[1] )
      xy_m[1] = max.xy_m[1];
   else
   if( xy_m[1] < min.xy_m[1] )
      xy_m[1] = min.xy_m[1];

   return *this;
}




/// queries --------------------------------------------------------------------
template<class TYPE>
void Coord2<TYPE>::get
(
   TYPE& x,
   TYPE& y
) const
{
   x = xy_m[0];
   y = xy_m[1];
}


template<class TYPE>
void Coord2<TYPE>::get
(
   TYPE xy[2]
) const
{
   xy[0] = xy_m[0];
   xy[1] = xy_m[1];
}


template<class TYPE>
inline
TYPE Coord2<TYPE>::getX() const
{
   return xy_m[0];
}


template<class TYPE>
inline
TYPE Coord2<TYPE>::getY() const
{
   return xy_m[1];
}


template<class TYPE>
inline
TYPE Coord2<TYPE>::operator[]
(
   const int i
) const
{
   return xy_m[i];
}


template<class TYPE>
Coord2<TYPE> Coord2<TYPE>::operator-() const
{
   return Coord2( -xy_m[0], -xy_m[1] );
}


template<class TYPE>
Coord2<TYPE> Coord2<TYPE>::abs() const
{
   return Coord2(
      xy_m[0] >= TYPE() ? xy_m[0] : -xy_m[0],
      xy_m[1] >= TYPE() ? xy_m[1] : -xy_m[1] );
}


template<class TYPE>
Coord2<dword> Coord2<TYPE>::toInt() const
{
   return Coord2<dword>(
      static_cast<dword>( ::floorf( xy_m[0] ) ),
      static_cast<dword>( ::floorf( xy_m[1] ) ) );
}


template<class TYPE>
Coord2<float> Coord2<TYPE>::toFp() const
{
   return Coord2<float>(
      static_cast<float>( xy_m[0] ),
      static_cast<float>( xy_m[1] ) );
}


template<class TYPE>
Coord2<TYPE> Coord2<TYPE>::operator+
(
   const Coord2& c
) const
{
   return Coord2(
      xy_m[0] + c.xy_m[0],
      xy_m[1] + c.xy_m[1] );
}


template<class TYPE>
Coord2<TYPE> Coord2<TYPE>::operator-
(
   const Coord2& c
) const
{
   return Coord2(
      xy_m[0] - c.xy_m[0],
      xy_m[1] - c.xy_m[1] );
}


template<class TYPE>
Coord2<TYPE> Coord2<TYPE>::operator*
(
   const Coord2& c
) const
{
   return Coord2(
      xy_m[0] * c.xy_m[0],
      xy_m[1] * c.xy_m[1] );
}


template<class TYPE>
Coord2<TYPE> Coord2<TYPE>::operator/
(
   const Coord2& c
) const
{
   return Coord2(
      xy_m[0] / c.xy_m[0],
      xy_m[1] / c.xy_m[1] );
}


template<class TYPE>
Coord2<TYPE> Coord2<TYPE>::operator*
(
   const TYPE n
) const
{
   return Coord2(
      xy_m[0] * n,
      xy_m[1] * n );
}


template<class TYPE>
Coord2<TYPE> Coord2<TYPE>::operator/
(
   const TYPE n
) const
{
   return Coord2(
      xy_m[0] /= n,
      xy_m[1] /= n );
}


template<class TYPE>
bool Coord2<TYPE>::operator==
(
   const Coord2& c
) const
{
   return
      (xy_m[0] == c.xy_m[0]) &
      (xy_m[1] == c.xy_m[1]);
}


template<class TYPE>
bool Coord2<TYPE>::operator!=
(
   const Coord2& c
) const
{
   return
      (xy_m[0] != c.xy_m[0]) |
      (xy_m[1] != c.xy_m[1]);
}


template<class TYPE>
bool Coord2<TYPE>::isZero() const
{
   return bool(
      (xy_m[0] == TYPE()) &
      (xy_m[1] == TYPE()) );
}


template<class TYPE>
Coord2<TYPE> Coord2<TYPE>::sign() const
{
   return compare( Coord2::ZERO() );
}


template<class TYPE>
Coord2<TYPE> Coord2<TYPE>::compare
(
   const Coord2& c
) const
{
   return Coord2(
      static_cast<TYPE>(
         xy_m[0] > c.xy_m[0] ? +1 : (xy_m[0] == c.xy_m[0] ? 0 : -1)),
      static_cast<TYPE>(
         xy_m[1] > c.xy_m[1] ? +1 : (xy_m[1] == c.xy_m[1] ? 0 : -1)) );
}


template<class TYPE>
Coord2<TYPE> Coord2<TYPE>::equal
(
   const Coord2& c
) const
{
   return Coord2(
      static_cast<TYPE>(xy_m[0] == c.xy_m[0]),
      static_cast<TYPE>(xy_m[1] == c.xy_m[1]) );
}


template<class TYPE>
Coord2<TYPE> Coord2<TYPE>::operator>
(
   const Coord2& c
) const
{
   return Coord2(
      static_cast<TYPE>(xy_m[0] > c.xy_m[0]),
      static_cast<TYPE>(xy_m[1] > c.xy_m[1]) );
}


template<class TYPE>
Coord2<TYPE> Coord2<TYPE>::operator>=
(
   const Coord2& c
) const
{
   return Coord2(
      static_cast<TYPE>(xy_m[0] >= c.xy_m[0]),
      static_cast<TYPE>(xy_m[1] >= c.xy_m[1]) );
}


template<class TYPE>
Coord2<TYPE> Coord2<TYPE>::operator<
(
   const Coord2& c
) const
{
   return Coord2(
      static_cast<TYPE>(xy_m[0] < c.xy_m[0]),
      static_cast<TYPE>(xy_m[1] < c.xy_m[1]) );
}


template<class TYPE>
Coord2<TYPE> Coord2<TYPE>::operator<=
(
   const Coord2& c
) const
{
   return Coord2(
      static_cast<TYPE>(xy_m[0] <= c.xy_m[0]),
      static_cast<TYPE>(xy_m[1] <= c.xy_m[1]) );
}


template<class TYPE>
Coord2<TYPE> Coord2<TYPE>::getClamped
(
   const Coord2& min,
   const Coord2& max
) const
{
   Coord2 r( *this );

   for( int i = 3;  i-- > 0; )
   {
      r.xy_m[i] = r.xy_m[i] >= min.xy_m[i] ? r.xy_m[i] : min.xy_m[i];
      r.xy_m[i] = r.xy_m[i] <= max.xy_m[i] ? r.xy_m[i] : max.xy_m[i];
   }

   return r;
}


template<class TYPE>
bool Coord2<TYPE>::isPositive() const
{
   return (xy_m[0] > TYPE()) && (xy_m[1] > TYPE());
}


template<class TYPE>
TYPE Coord2<TYPE>::product() const
{
   return xy_m[0] * xy_m[1];
}




/// friends --------------------------------------------------------------------
template<class TYPE>
Coord2<TYPE> hxa7241_graphics::operator*
(
   const TYPE          n,
   const Coord2<TYPE>& c
)
{
   return Coord2<TYPE>(
      n * c.getX(),
      n * c.getY() );
}




template<class TYPE>
Coord2<TYPE> hxa7241_graphics::operator/
(
   const TYPE          n,
   const Coord2<TYPE>& c
)
{
   return Coord2<TYPE>(
      n / c.getX(),
      n / c.getY() );
}




/// constants ------------------------------------------------------------------
template<class TYPE>
const Coord2<TYPE>& Coord2<TYPE>::ZERO()
{
   static const Coord2 k = Coord2( TYPE(), TYPE() );
   return k;
}


template<class TYPE>
const Coord2<TYPE>& Coord2<TYPE>::ONE()
{
   static const Coord2 k = Coord2( static_cast<TYPE>(1), static_cast<TYPE>(1) );
   return k;
}


template<class TYPE>
const Coord2<TYPE>& Coord2<TYPE>::MIN()
{
   static const Coord2 k = Coord2(
      static_cast<TYPE>(-std::numeric_limits<TYPE>::max()),
      static_cast<TYPE>(-std::numeric_limits<TYPE>::max()) );
   return k;
}


template<class TYPE>
const Coord2<TYPE>& Coord2<TYPE>::MAX()
{
   static const Coord2 k = Coord2(
      std::numeric_limits<TYPE>::max(),
      std::numeric_limits<TYPE>::max() );
   return k;
}


template<class TYPE>
const Coord2<TYPE>& Coord2<TYPE>::X()
{
   static const Coord2 k = Coord2( static_cast<TYPE>(1), TYPE() );
   return k;
}


template<class TYPE>
const Coord2<TYPE>& Coord2<TYPE>::Y()
{
   static const Coord2 k = Coord2( TYPE(), static_cast<TYPE>(1) );
   return k;
}


}//namespace




#endif//Coord2_h
