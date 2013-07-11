/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include <math.h>
#include <iostream>

#include "Vector4f.hpp"


using namespace hxa7241_graphics;




/// standard object services ---------------------------------------------------
Vector4f::Vector4f()
{
   xyzw_m[0] = 0.0f;
   xyzw_m[1] = 0.0f;
   xyzw_m[2] = 0.0f;
   xyzw_m[3] = 0.0f;
}


Vector4f::Vector4f
(
   const float x,
   const float y,
   const float z,
   const float w
)
{
   Vector4f::set( x, y, z, w );
}


Vector4f::Vector4f
(
   const Vector3f& v3,
   const float     w
)
{
   Vector4f::set( v3, w );
}


Vector4f::Vector4f
(
   const Vector3f& v3
)
{
   Vector4f::set( v3 );
}


Vector4f& Vector4f::operator=
(
   const Vector4f& other
)
{
   if( &other != this )
   {
      xyzw_m[0] = other.xyzw_m[0];
      xyzw_m[1] = other.xyzw_m[1];
      xyzw_m[2] = other.xyzw_m[2];
      xyzw_m[3] = other.xyzw_m[3];
   }

   return *this;
}




/// commands -------------------------------------------------------------------
Vector4f& Vector4f::set
(
   const float x,
   const float y,
   const float z,
   const float w
)
{
   xyzw_m[0] = x;
   xyzw_m[1] = y;
   xyzw_m[2] = z;
   xyzw_m[3] = w;

   return *this;
}


Vector4f& Vector4f::set
(
   const float xyzw[4]
)
{
   xyzw_m[0] = xyzw[0];
   xyzw_m[1] = xyzw[1];
   xyzw_m[2] = xyzw[2];
   xyzw_m[3] = xyzw[3];

   return *this;
}


Vector4f& Vector4f::set
(
   const Vector3f& v3,
   const float w
)
{
   set( v3 );
   xyzw_m[3] = w;

   return *this;
}


Vector4f& Vector4f::set
(
   const Vector3f& v3
)
{
   xyzw_m[0] = v3.getX();
   xyzw_m[1] = v3.getY();
   xyzw_m[2] = v3.getZ();
   xyzw_m[3] = 1.0f;

   return *this;
}


Vector4f& Vector4f::negateEq()
{
   xyzw_m[0] = -xyzw_m[0];
   xyzw_m[1] = -xyzw_m[1];
   xyzw_m[2] = -xyzw_m[2];
   xyzw_m[3] = -xyzw_m[3];

   return *this;
}


Vector4f& Vector4f::absEq()
{
   int i = 3;
   do
   {
      // doesn't change -0.0  but does that matter ?
      if( xyzw_m[i] < 0.0f )
      {
         xyzw_m[i] = -xyzw_m[i];
      }
   } while( i-- > 0 );

   return *this;
}


Vector4f& Vector4f::unitizeEq()
{
   const float length = ::sqrtf(
      (xyzw_m[0] * xyzw_m[0]) +
      (xyzw_m[1] * xyzw_m[1]) +
      (xyzw_m[2] * xyzw_m[2]) +
      (xyzw_m[3] * xyzw_m[3]) );
   const float oneOverLength = (length > 0.0f) ? 1.0f / length : 0.0f;

   xyzw_m[0] *= oneOverLength;
   xyzw_m[1] *= oneOverLength;
   xyzw_m[2] *= oneOverLength;
   xyzw_m[3] *= oneOverLength;

   return *this;
}


/*Vector4f& Vector4f::crossEq
(
   const Vector4f& v
)
{
   const float x = ;
   const float y = ;
   const float z = ;
   const float w = ;

   xyzw_m[0] = x;
   xyzw_m[1] = y;
   xyzw_m[2] = z;
   xyzw_m[3] = w;

   return *this;
}*/


Vector4f& Vector4f::operator+=
(
   const Vector4f& v
)
{
   xyzw_m[0] += v.xyzw_m[0];
   xyzw_m[1] += v.xyzw_m[1];
   xyzw_m[2] += v.xyzw_m[2];
   xyzw_m[3] += v.xyzw_m[3];

   return *this;
}


Vector4f& Vector4f::operator-=
(
   const Vector4f& v
)
{
   xyzw_m[0] -= v.xyzw_m[0];
   xyzw_m[1] -= v.xyzw_m[1];
   xyzw_m[2] -= v.xyzw_m[2];
   xyzw_m[3] -= v.xyzw_m[3];

   return *this;
}


Vector4f& Vector4f::operator*=
(
   const Vector4f& v
)
{
   xyzw_m[0] *= v.xyzw_m[0];
   xyzw_m[1] *= v.xyzw_m[1];
   xyzw_m[2] *= v.xyzw_m[2];
   xyzw_m[3] *= v.xyzw_m[3];

   return *this;
}


Vector4f& Vector4f::operator/=
(
   const Vector4f& v
)
{
   xyzw_m[0] /= v.xyzw_m[0];
   xyzw_m[1] /= v.xyzw_m[1];
   xyzw_m[2] /= v.xyzw_m[2];
   xyzw_m[3] /= v.xyzw_m[3];

   return *this;
}


Vector4f& Vector4f::operator*=
(
   const float f
)
{
   xyzw_m[0] *= f;
   xyzw_m[1] *= f;
   xyzw_m[2] *= f;
   xyzw_m[3] *= f;

   return *this;
}


Vector4f& Vector4f::operator/=
(
   const float f
)
{
   const float oneOverF = 1.0f / f;

   xyzw_m[0] *= oneOverF;
   xyzw_m[1] *= oneOverF;
   xyzw_m[2] *= oneOverF;
   xyzw_m[3] *= oneOverF;

   return *this;
}


Vector4f& Vector4f::clampMin
(
   const Vector4f& min
)
{
   int i = 3;
   do
   {
      if( xyzw_m[i] < min.xyzw_m[i] )
      {
         xyzw_m[i] = min.xyzw_m[i];
      }
   } while( i-- > 0 );

   return *this;
}


Vector4f& Vector4f::clampMax
(
   const Vector4f& max
)
{
   int i = 3;
   do
   {
      if( xyzw_m[i] > max.xyzw_m[i] )
      {
         xyzw_m[i] = max.xyzw_m[i];
      }
   } while( i-- > 0 );

   return *this;
}


Vector4f& Vector4f::clamp
(
   const Vector4f& min,
   const Vector4f& max
)
{
   int i = 3;
   do
   {
      if( xyzw_m[i] > max.xyzw_m[i] )
      {
         xyzw_m[i] = max.xyzw_m[i];
      }
      else
      if( xyzw_m[i] < min.xyzw_m[i] )
      {
         xyzw_m[i] = min.xyzw_m[i];
      }
   } while( i-- > 0 );

   return *this;
}


// [0,1)  0 to almost 1
Vector4f& Vector4f::clamp01()
{
   int i = 3;
   do
   {
      if( xyzw_m[i] >= 1.0f )
      {
         xyzw_m[i]  = FLOAT_ALMOST_ONE;
      }
      else
      if( xyzw_m[i] < 0.0f )
      {
         xyzw_m[i] = 0.0f;
      }
   } while( i-- > 0 );

   return *this;
}




/// queries --------------------------------------------------------------------
void Vector4f::get
(
   float& x,
   float& y,
   float& z,
   float& w
) const
{
   x = xyzw_m[0];
   y = xyzw_m[1];
   z = xyzw_m[2];
   w = xyzw_m[3];
}


void Vector4f::get
(
   float xyzw[4]
) const
{
   xyzw[0] = xyzw_m[0];
   xyzw[1] = xyzw_m[1];
   xyzw[2] = xyzw_m[2];
   xyzw[3] = xyzw_m[3];
}


void Vector4f::getDivW
(
   Vector3f& v3
) const
{
   const float oneOverW = (xyzw_m[3] != 0.0f) ? 1.0f / xyzw_m[3] : 0.0f;

   v3.set( xyzw_m );
   v3 *= oneOverW;
}


Vector3f Vector4f::getDivW() const
{
   const float oneOverW = (xyzw_m[3] != 0.0f) ? 1.0f / xyzw_m[3] : 0.0f;

   return Vector3f(
      xyzw_m[0] * oneOverW,
      xyzw_m[1] * oneOverW,
      xyzw_m[2] * oneOverW );
}


float Vector4f::sum() const
{
   return xyzw_m[0] + xyzw_m[1] + xyzw_m[2] + xyzw_m[3];
}


float Vector4f::average() const
{
   return (xyzw_m[0] + xyzw_m[1] + xyzw_m[2] + xyzw_m[3]) * 0.25f;
}


float Vector4f::smallest() const
{
   float smallest;
   smallest = xyzw_m[0] <= xyzw_m[1] ? xyzw_m[0] : xyzw_m[1];
   smallest = smallest  <= xyzw_m[2] ? smallest  : xyzw_m[2];
   smallest = smallest  <= xyzw_m[3] ? smallest  : xyzw_m[3];

   return smallest;
}


float Vector4f::largest() const
{
   float largest;
   largest = xyzw_m[0] >= xyzw_m[1] ? xyzw_m[0] : xyzw_m[1];
   largest = largest   >= xyzw_m[2] ? largest   : xyzw_m[2];
   largest = largest   >= xyzw_m[3] ? largest   : xyzw_m[3];

   return largest;
}


float Vector4f::length() const
{
   return ::sqrtf(
      (xyzw_m[0] * xyzw_m[0]) +
      (xyzw_m[1] * xyzw_m[1]) +
      (xyzw_m[2] * xyzw_m[2]) +
      (xyzw_m[3] * xyzw_m[3]) );
}


float Vector4f::dot
(
   const Vector4f& v
) const
{
   return
      (xyzw_m[0] * v.xyzw_m[0]) +
      (xyzw_m[1] * v.xyzw_m[1]) +
      (xyzw_m[2] * v.xyzw_m[2]) +
      (xyzw_m[3] * v.xyzw_m[3]);
}


float Vector4f::distance
(
   const Vector4f& v
) const
{
   const float xDif = xyzw_m[0] - v.xyzw_m[0];
   const float yDif = xyzw_m[1] - v.xyzw_m[1];
   const float zDif = xyzw_m[2] - v.xyzw_m[2];
   const float wDif = xyzw_m[3] - v.xyzw_m[3];

   return ::sqrtf(
      (xDif * xDif) +
      (yDif * yDif) +
      (zDif * zDif) +
      (wDif * wDif) );

// //const Vector4f dif( *this - v );
// Vector4f dif( *this );
// dif -= v;
//
// return dif.length();
}


Vector4f Vector4f::operator-() const
{
   return Vector4f(
      -xyzw_m[0],
      -xyzw_m[1],
      -xyzw_m[2],
      -xyzw_m[3] );
}


Vector4f Vector4f::abs() const
{
   return Vector4f( *this ).absEq();
}


Vector4f Vector4f::unitized() const
{
   const float length = ::sqrtf(
      (xyzw_m[0] * xyzw_m[0]) +
      (xyzw_m[1] * xyzw_m[1]) +
      (xyzw_m[2] * xyzw_m[2]) +
      (xyzw_m[3] * xyzw_m[3]) );
   const float oneOverLength = (length > 0.0f) ? 1.0f / length : 0.0f;

   return Vector4f(
      xyzw_m[0] * oneOverLength,
      xyzw_m[1] * oneOverLength,
      xyzw_m[2] * oneOverLength,
      xyzw_m[3] * oneOverLength );
}


/*Vector4f Vector4f::cross
(
   const Vector4f& v
) const
{
 return Vector4f(
    ,
    ,
    ,
     );
}*/


Vector4f Vector4f::operator+
(
   const Vector4f& v
) const
{
   return Vector4f(
      xyzw_m[0] + v.xyzw_m[0],
      xyzw_m[1] + v.xyzw_m[1],
      xyzw_m[2] + v.xyzw_m[2],
      xyzw_m[3] + v.xyzw_m[3] );
}


Vector4f Vector4f::operator-
(
   const Vector4f& v
) const
{
   return Vector4f(
      xyzw_m[0] - v.xyzw_m[0],
      xyzw_m[1] - v.xyzw_m[1],
      xyzw_m[2] - v.xyzw_m[2],
      xyzw_m[3] - v.xyzw_m[3] );
}


Vector4f Vector4f::operator*
(
   const Vector4f& v
) const
{
   return Vector4f(
      xyzw_m[0] * v.xyzw_m[0],
      xyzw_m[1] * v.xyzw_m[1],
      xyzw_m[2] * v.xyzw_m[2],
      xyzw_m[3] * v.xyzw_m[3] );
}


Vector4f Vector4f::operator/
(
   const Vector4f& v
) const
{
   return Vector4f(
      xyzw_m[0] / v.xyzw_m[0],
      xyzw_m[1] / v.xyzw_m[1],
      xyzw_m[2] / v.xyzw_m[2],
      xyzw_m[3] / v.xyzw_m[3] );
}


Vector4f Vector4f::operator*
(
   const float f
) const
{
   return Vector4f(
      xyzw_m[0] * f,
      xyzw_m[1] * f,
      xyzw_m[2] * f,
      xyzw_m[3] * f );
}


Vector4f Vector4f::operator/
(
   const float f
) const
{
   const float oneOverF = 1.0f / f;

   return Vector4f(
      xyzw_m[0] * oneOverF,
      xyzw_m[1] * oneOverF,
      xyzw_m[2] * oneOverF,
      xyzw_m[3] * oneOverF );
}


bool Vector4f::operator==
(
   const Vector4f& v
) const
{
   return
      (xyzw_m[0] == v.xyzw_m[0]) &
      (xyzw_m[1] == v.xyzw_m[1]) &
      (xyzw_m[2] == v.xyzw_m[2]) &
      (xyzw_m[3] == v.xyzw_m[3]);
}


bool Vector4f::operator!=
(
   const Vector4f& v
) const
{
   return
      (xyzw_m[0] != v.xyzw_m[0]) |
      (xyzw_m[1] != v.xyzw_m[1]) |
      (xyzw_m[2] != v.xyzw_m[2]) |
      (xyzw_m[3] != v.xyzw_m[3]);
}


bool Vector4f::isZero() const
{
   return bool(
      (xyzw_m[0] == 0.0f) &
      (xyzw_m[1] == 0.0f) &
      (xyzw_m[2] == 0.0f) &
      (xyzw_m[3] == 0.0f) );
}


void Vector4f::getClamped
(
   const Vector4f& min,
   const Vector4f& max,
   Vector4f&       result
) const
{
   result = *this;
   result.clamp( min, max );
}


// [0,1)  0 to almost 1
void Vector4f::getClamped01
(
   Vector4f& result
) const
{
   result = *this;
   result.clamp01();
}




/// friends --------------------------------------------------------------------
Vector4f hxa7241_graphics::operator*
(
   const float     f,
   const Vector4f& v
)
{
   return Vector4f(
      f * v.xyzw_m[0],
      f * v.xyzw_m[1],
      f * v.xyzw_m[2],
      f * v.xyzw_m[3] );
}




Vector4f hxa7241_graphics::operator/
(
   const float     f,
   const Vector4f& v
)
{
   return Vector4f(
      f / v.xyzw_m[0],
      f / v.xyzw_m[1],
      f / v.xyzw_m[2],
      f / v.xyzw_m[3] );
}




/// constants ------------------------------------------------------------------
const Vector4f& Vector4f::ZERO()
{
   static const Vector4f k( 0.0f, 0.0f, 0.0f, 0.0f );
   return k;
}


const Vector4f& Vector4f::HALF()
{
   static const Vector4f k( 0.5f, 0.5f, 0.5f, 0.5f );
   return k;
}


const Vector4f& Vector4f::ONE()
{
   static const Vector4f k( 1.0f, 1.0f, 1.0f, 1.0f );
   return k;
}


const Vector4f& Vector4f::EPSILON()
{
   static const Vector4f k( FLOAT_EPSILON, FLOAT_EPSILON, FLOAT_EPSILON,
      FLOAT_EPSILON );
   return k;
}


const Vector4f& Vector4f::ALMOST_ONE()
{
   static const Vector4f k( FLOAT_ALMOST_ONE, FLOAT_ALMOST_ONE, FLOAT_ALMOST_ONE,
      FLOAT_ALMOST_ONE );
   return k;
}


const Vector4f& Vector4f::MIN()
{
   static const Vector4f k( FLOAT_MIN_NEG, FLOAT_MIN_NEG, FLOAT_MIN_NEG,
      FLOAT_MIN_NEG );
   return k;
}


const Vector4f& Vector4f::MAX()
{
   static const Vector4f k( FLOAT_MAX, FLOAT_MAX, FLOAT_MAX, FLOAT_MAX );
   return k;
}


const Vector4f& Vector4f::SMALL()
{
   static const Vector4f k( FLOAT_SMALL, FLOAT_SMALL, FLOAT_SMALL,
      FLOAT_SMALL );
   return k;
}


const Vector4f& Vector4f::LARGE()
{
   static const Vector4f k( FLOAT_LARGE, FLOAT_LARGE, FLOAT_LARGE,
      FLOAT_LARGE );
   return k;
}


const Vector4f& Vector4f::X()
{
   static const Vector4f k( 1.0f, 0.0f, 0.0f, 0.0f );
   return k;
}


const Vector4f& Vector4f::Y()
{
   static const Vector4f k( 0.0f, 1.0f, 0.0f, 0.0f );
   return k;
}


const Vector4f& Vector4f::Z()
{
   static const Vector4f k( 0.0f, 0.0f, 1.0f, 0.0f );
   return k;
}


const Vector4f& Vector4f::W()
{
   static const Vector4f k( 0.0f, 0.0f, 0.0f, 1.0f );
   return k;
}




/// streaming ------------------------------------------------------------------
std::ostream& hxa7241_graphics::operator<<
(
   std::ostream&   out,
   const Vector4f& obj
)
{
   return out << '(' << obj[0] << ' ' << obj[1] << ' ' << obj[2] << ' ' <<
      obj[3] << ')';
}


std::istream& hxa7241_graphics::operator>>
(
   std::istream& in,
   Vector4f&     obj
)
{
   char  c;
   float xyzw[4];

   in >> c >> xyzw[0] >> xyzw[1] >> xyzw[2] >> xyzw[3] >> c;
   obj.set( xyzw );

   return in;
}
