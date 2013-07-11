/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef Vector4f_h
#define Vector4f_h


#include <iosfwd>

#include "Vector3f.hpp"




#include "hxa7241_graphics.hpp"
namespace hxa7241_graphics
{


/**
 * A 4D vector of floats.<br/><br/>
 *
 * Provides explicit conversion to and from 3D vector.
 *
 * @see Vector3f
 */
class Vector4f
{
/// standard object services ---------------------------------------------------
public:
            Vector4f();
            Vector4f( float x,
                      float y,
                      float z,
                      float w );
   explicit Vector4f( const float xyzw[4] );

            Vector4f( const Vector3f&,
                      float w );
   explicit Vector4f( const Vector3f& );

           ~Vector4f();
            Vector4f( const Vector4f& );
   Vector4f& operator=( const Vector4f& );


/// commands -------------------------------------------------------------------
           Vector4f& set( float x,
                          float y,
                          float z,
                          float w );
           Vector4f& set( const float xyzw[4] );

           Vector4f& set( const Vector3f&,
                          float w );
           Vector4f& set( const Vector3f& );
           Vector4f& set( int,
                          float );

           Vector4f& negateEq ();
           Vector4f& absEq    ();
           Vector4f& unitizeEq();
           //Vector4f& crossEq  ( const Vector4f& );

           Vector4f& operator+=( const Vector4f& );
           Vector4f& operator-=( const Vector4f& );
           Vector4f& operator*=( const Vector4f& );
           Vector4f& operator/=( const Vector4f& );
           Vector4f& operator*=( float );
           Vector4f& operator/=( float );

           Vector4f& clampMin( const Vector4f& );
           Vector4f& clampMax( const Vector4f& );
           Vector4f& clamp   ( const Vector4f& min,
                               const Vector4f& max );
           Vector4f& clamp01();


/// queries --------------------------------------------------------------------
           void      get( float& x,
                          float& y,
                          float& z,
                          float& w )                                      const;
           void      get( float xyzw[4] )                                 const;

           void      get( Vector3f& )                                     const;
           Vector3f  get()                                                const;
           void      getDivW( Vector3f& )                                 const;
           Vector3f  getDivW()                                            const;

           float     getX()                                               const;
           float     getY()                                               const;
           float     getZ()                                               const;
           float     getW()                                               const;
           float     operator[]( int )                                    const;

           float     sum()                                                const;
           float     average()                                            const;
           float     smallest()                                           const;
           float     largest()                                            const;

           float     length()                                             const;
           float     dot( const Vector4f& )                               const;
           float     distance( const Vector4f& )                          const;

           Vector4f  operator-()                                          const;
           Vector4f  abs()                                                const;
           Vector4f  unitized()                                           const;
           //Vector4f  cross( const Vector4f& )                             const;

           Vector4f  operator+ ( const Vector4f& )                        const;
           Vector4f  operator- ( const Vector4f& )                        const;
           Vector4f  operator* ( const Vector4f& )                        const;
           Vector4f  operator/ ( const Vector4f& )                        const;
           Vector4f  operator* ( float )                                  const;
           Vector4f  operator/ ( float )                                  const;

           bool      operator==( const Vector4f& )                        const;
           bool      operator!=( const Vector4f& )                        const;
           bool      isZero()                                             const;

//           // returning vectors of -1.0f or 0.0f or +1.0f
//           Vector4f  sign   ()                                            const;
//           Vector4f  compare( const Vector4f& )                           const;
//
//           Vector4f  equal     ( const Vector4f& )                        const;
//           Vector4f  operator> ( const Vector4f& )                        const;
//           Vector4f  operator>=( const Vector4f& )                        const;
//           Vector4f  operator< ( const Vector4f& )                        const;
//           Vector4f  operator<=( const Vector4f& )                        const;

           void      getClamped  ( const Vector4f& min,
                                   const Vector4f& max,
                                   Vector4f&            )                 const;
           void      getClamped01( Vector4f& result )                     const;

   friend  Vector4f  operator*( float, const Vector4f& );
   friend  Vector4f  operator/( float, const Vector4f& );


/// constants ------------------------------------------------------------------
   static const Vector4f& ZERO();
   static const Vector4f& HALF();
   static const Vector4f& ONE();
   static const Vector4f& EPSILON();
   static const Vector4f& ALMOST_ONE();
   static const Vector4f& MIN();
   static const Vector4f& MAX();
   static const Vector4f& SMALL();
   static const Vector4f& LARGE();
   static const Vector4f& X();
   static const Vector4f& Y();
   static const Vector4f& Z();
   static const Vector4f& W();


/// fields ---------------------------------------------------------------------
private:
   float xyzw_m[4];
};








/// friends
Vector4f operator*( float, const Vector4f& );
Vector4f operator/( float, const Vector4f& );




/// streaming
std::ostream& operator<<( std::ostream&, const Vector4f& );
std::istream& operator>>( std::istream&,       Vector4f& );




/// INLINES ///


/// standard object services ---------------------------------------------------
inline
Vector4f::Vector4f
(
   const float xyzw[4]
)
{
   Vector4f::set( xyzw );
}


inline
Vector4f::~Vector4f()
{
}


inline
Vector4f::Vector4f
(
   const Vector4f& other
)
{
   Vector4f::operator=( other );
}




/// commands -------------------------------------------------------------------
inline
Vector4f& Vector4f::set
(
   const int   i,
   const float f
)
{
   xyzw_m[ i & 0x03 ] = f;

   return *this;
}




/// queries --------------------------------------------------------------------
inline
void Vector4f::get
(
   Vector3f& v3
) const
{
   v3.set( xyzw_m );
}


inline
Vector3f Vector4f::get() const
{
   return Vector3f( xyzw_m );
}


inline
float Vector4f::getX() const
{
   return xyzw_m[0];
}


inline
float Vector4f::getY() const
{
   return xyzw_m[1];
}


inline
float Vector4f::getZ() const
{
   return xyzw_m[2];
}


inline
float Vector4f::getW() const
{
   return xyzw_m[3];
}


inline
float Vector4f::operator[]
(
   const int i
) const
{
   return xyzw_m[ i ];
}


}//namespace




#endif//Vector4f_h
