/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include <math.h>

#include "Vector3f.hpp"


using namespace hxa7241_graphics;




/// standard object services ---------------------------------------------------
Vector3f::Vector3f()
{
   xyz_m[0] = 0.0f;
   xyz_m[1] = 0.0f;
   xyz_m[2] = 0.0f;
}


Vector3f::Vector3f
(
   const float x,
   const float y,
   const float z
)
{
   xyz_m[0] = x;
   xyz_m[1] = y;
   xyz_m[2] = z;
}


//Vector3f::Vector3f
//(
//   const float xyz[3]
//)
//{
//   xyz_m[0] = xyz[0];
//   xyz_m[1] = xyz[1];
//   xyz_m[2] = xyz[2];
//}


Vector3f& Vector3f::operator=
(
   const Vector3f& that
)
{
   if( &that != this )
   {
      xyz_m[0] = that.xyz_m[0];
      xyz_m[1] = that.xyz_m[1];
      xyz_m[2] = that.xyz_m[2];
   }

   return *this;
}




/// commands -------------------------------------------------------------------
Vector3f& Vector3f::set
(
   const float x,
   const float y,
   const float z
)
{
   xyz_m[0] = x;
   xyz_m[1] = y;
   xyz_m[2] = z;

   return *this;
}


Vector3f& Vector3f::set
(
   const float xyz[3]
)
{
   xyz_m[0] = xyz[0];
   xyz_m[1] = xyz[1];
   xyz_m[2] = xyz[2];

   return *this;
}


Vector3f& Vector3f::negateEq()
{
   xyz_m[0] = -xyz_m[0];
   xyz_m[1] = -xyz_m[1];
   xyz_m[2] = -xyz_m[2];

   return *this;
}


Vector3f& Vector3f::absEq()
{
   if( xyz_m[0] < 0.0f )
   {
      xyz_m[0] = -xyz_m[0];
   }
   if( xyz_m[1] < 0.0f )
   {
      xyz_m[1] = -xyz_m[1];
   }
   if( xyz_m[2] < 0.0f )
   {
      xyz_m[2] = -xyz_m[2];
   }

   return *this;
}


Vector3f& Vector3f::unitizeEq()
{
   const float length = ::sqrtf(
      (xyz_m[0] * xyz_m[0]) +
      (xyz_m[1] * xyz_m[1]) +
      (xyz_m[2] * xyz_m[2]) );
   const float oneOverLength = length != 0.0f ? 1.0f / length : 0.0f;

   xyz_m[0] *= oneOverLength;
   xyz_m[1] *= oneOverLength;
   xyz_m[2] *= oneOverLength;

   return *this;
}


Vector3f& Vector3f::crossEq
(
   const Vector3f& v
)
{
   const float x = (xyz_m[1] * v.xyz_m[2]) - (xyz_m[2] * v.xyz_m[1]);
   const float y = (xyz_m[2] * v.xyz_m[0]) - (xyz_m[0] * v.xyz_m[2]);
   const float z = (xyz_m[0] * v.xyz_m[1]) - (xyz_m[1] * v.xyz_m[0]);

   xyz_m[0] = x;
   xyz_m[1] = y;
   xyz_m[2] = z;

   return *this;
}


Vector3f& Vector3f::operator+=
(
   const Vector3f& v
)
{
   xyz_m[0] += v.xyz_m[0];
   xyz_m[1] += v.xyz_m[1];
   xyz_m[2] += v.xyz_m[2];

   return *this;
}


Vector3f& Vector3f::operator-=
(
   const Vector3f& v
)
{
   xyz_m[0] -= v.xyz_m[0];
   xyz_m[1] -= v.xyz_m[1];
   xyz_m[2] -= v.xyz_m[2];

   return *this;
}


Vector3f& Vector3f::operator*=
(
   const Vector3f& v
)
{
   xyz_m[0] *= v.xyz_m[0];
   xyz_m[1] *= v.xyz_m[1];
   xyz_m[2] *= v.xyz_m[2];

   return *this;
}


Vector3f& Vector3f::operator/=
(
   const Vector3f& v
)
{
   xyz_m[0] /= v.xyz_m[0];
   xyz_m[1] /= v.xyz_m[1];
   xyz_m[2] /= v.xyz_m[2];

   return *this;
}


Vector3f& Vector3f::operator*=
(
   const float f
)
{
   xyz_m[0] *= f;
   xyz_m[1] *= f;
   xyz_m[2] *= f;

   return *this;
}


Vector3f& Vector3f::operator/=
(
   const float f
)
{
   const float oneOverF = 1.0f / f;

   xyz_m[0] *= oneOverF;
   xyz_m[1] *= oneOverF;
   xyz_m[2] *= oneOverF;

   return *this;
}


Vector3f& Vector3f::clampMin
(
   const Vector3f& min
)
{
   if( xyz_m[0] < min.xyz_m[0] )
   {
      xyz_m[0] = min.xyz_m[0];
   }
   if( xyz_m[1] < min.xyz_m[1] )
   {
      xyz_m[1] = min.xyz_m[1];
   }
   if( xyz_m[2] < min.xyz_m[2] )
   {
      xyz_m[2] = min.xyz_m[2];
   }

   return *this;
}


Vector3f& Vector3f::clampMax
(
   const Vector3f& max
)
{
   if( xyz_m[0] > max.xyz_m[0] )
   {
      xyz_m[0] = max.xyz_m[0];
   }
   if( xyz_m[1] > max.xyz_m[1] )
   {
      xyz_m[1] = max.xyz_m[1];
   }
   if( xyz_m[2] > max.xyz_m[2] )
   {
      xyz_m[2] = max.xyz_m[2];
   }

   return *this;
}


Vector3f& Vector3f::clamp
(
   const Vector3f& min,
   const Vector3f& max
)
{
   if( xyz_m[0] > max.xyz_m[0] )
      xyz_m[0] = max.xyz_m[0];
   else
   if( xyz_m[0] < min.xyz_m[0] )
      xyz_m[0] = min.xyz_m[0];

   if( xyz_m[1] > max.xyz_m[1] )
      xyz_m[1] = max.xyz_m[1];
   else
   if( xyz_m[1] < min.xyz_m[1] )
      xyz_m[1] = min.xyz_m[1];

   if( xyz_m[2] > max.xyz_m[2] )
      xyz_m[2] = max.xyz_m[2];
   else
   if( xyz_m[2] < min.xyz_m[2] )
      xyz_m[2] = min.xyz_m[2];

   return *this;
}


// [0,1)  0 to almost 1
Vector3f& Vector3f::clamp01()
{
   if( xyz_m[0] >= 1.0f )
      xyz_m[0]  = FLOAT_ALMOST_ONE;
   else
   if( xyz_m[0] < 0.0f )
      xyz_m[0] = 0.0f;

   if( xyz_m[1] >= 1.0f )
      xyz_m[1]  = FLOAT_ALMOST_ONE;
   else
   if( xyz_m[1] < 0.0f )
      xyz_m[1] = 0.0f;

   if( xyz_m[2] >= 1.0f )
      xyz_m[2]  = FLOAT_ALMOST_ONE;
   else
   if( xyz_m[2] < 0.0f )
      xyz_m[2] = 0.0f;

   return *this;
}




/// queries --------------------------------------------------------------------
void Vector3f::get
(
   float& x,
   float& y,
   float& z
) const
{
   x = xyz_m[0];
   y = xyz_m[1];
   z = xyz_m[2];
}


void Vector3f::get
(
   float xyz[3]
) const
{
   xyz[0] = xyz_m[0];
   xyz[1] = xyz_m[1];
   xyz[2] = xyz_m[2];
}


float Vector3f::sum() const
{
   return xyz_m[0] + xyz_m[1] + xyz_m[2];
}


float Vector3f::average() const
{
   static const float ONE_OVER_3 = 1.0f / 3.0f;

   return (xyz_m[0] + xyz_m[1] + xyz_m[2]) * ONE_OVER_3;
}


float Vector3f::smallest() const
{
   float smallest = xyz_m[0] <= xyz_m[1] ? xyz_m[0] : xyz_m[1];
   smallest       = smallest <= xyz_m[2] ? smallest : xyz_m[2];

   return smallest;
}


float Vector3f::largest() const
{
   float largest = xyz_m[0] >= xyz_m[1] ? xyz_m[0] : xyz_m[1];
   largest       = largest  >= xyz_m[2] ? largest  : xyz_m[2];

   return largest;
}


float Vector3f::length() const
{
   return ::sqrtf(
      (xyz_m[0] * xyz_m[0]) +
      (xyz_m[1] * xyz_m[1]) +
      (xyz_m[2] * xyz_m[2]) );
}


float Vector3f::dot
(
   const Vector3f& v
) const
{
   return
      (xyz_m[0] * v.xyz_m[0]) +
      (xyz_m[1] * v.xyz_m[1]) +
      (xyz_m[2] * v.xyz_m[2]);
}


float Vector3f::distance
(
   const Vector3f& v
) const
{
   const float xDif = xyz_m[0] - v.xyz_m[0];
   const float yDif = xyz_m[1] - v.xyz_m[1];
   const float zDif = xyz_m[2] - v.xyz_m[2];

   return ::sqrtf(
      (xDif * xDif) +
      (yDif * yDif) +
      (zDif * zDif) );

   //Vector3f dif( *this - v );   // Vector3f dif( *this );   dif -= v;
   //
   //return ::sqrtf( dif.Dot( dif ) );
}


float Vector3f::distance2
(
   const Vector3f& v
) const
{
   const float xDif = xyz_m[0] - v.xyz_m[0];
   const float yDif = xyz_m[1] - v.xyz_m[1];
   const float zDif = xyz_m[2] - v.xyz_m[2];

   return (xDif * xDif) + (yDif * yDif) + (zDif * zDif);
}


Vector3f Vector3f::operator-() const
{
   return Vector3f( -xyz_m[0], -xyz_m[1], -xyz_m[2] );
}


Vector3f Vector3f::abs() const
{
   return Vector3f(
      xyz_m[0] >= 0.0f ? xyz_m[0] : -xyz_m[0],
      xyz_m[1] >= 0.0f ? xyz_m[1] : -xyz_m[1],
      xyz_m[2] >= 0.0f ? xyz_m[2] : -xyz_m[2] );

   //return Vector3f( *this ).absEq();
}


Vector3f Vector3f::unitize() const
{
   const float length = ::sqrtf(
      (xyz_m[0] * xyz_m[0]) +
      (xyz_m[1] * xyz_m[1]) +
      (xyz_m[2] * xyz_m[2]) );
   const float oneOverLength = length != 0.0f ? 1.0f / length : 0.0f;

   return Vector3f(
      xyz_m[0] * oneOverLength,
      xyz_m[1] * oneOverLength,
      xyz_m[2] * oneOverLength );
}


Vector3f Vector3f::cross
(
   const Vector3f& v
) const
{
   return Vector3f(
      (xyz_m[1] * v.xyz_m[2]) - (xyz_m[2] * v.xyz_m[1]),
      (xyz_m[2] * v.xyz_m[0]) - (xyz_m[0] * v.xyz_m[2]),
      (xyz_m[0] * v.xyz_m[1]) - (xyz_m[1] * v.xyz_m[0]) );
}


Vector3f Vector3f::operator+
(
   const Vector3f& v
) const
{
   return Vector3f(
      xyz_m[0] + v.xyz_m[0],
      xyz_m[1] + v.xyz_m[1],
      xyz_m[2] + v.xyz_m[2] );
}


Vector3f Vector3f::operator-
(
   const Vector3f& v
) const
{
   return Vector3f(
      xyz_m[0] - v.xyz_m[0],
      xyz_m[1] - v.xyz_m[1],
      xyz_m[2] - v.xyz_m[2] );
}


Vector3f Vector3f::operator*
(
   const Vector3f& v
) const
{
   return Vector3f(
      xyz_m[0] * v.xyz_m[0],
      xyz_m[1] * v.xyz_m[1],
      xyz_m[2] * v.xyz_m[2] );
}


Vector3f Vector3f::operator/
(
   const Vector3f& v
) const
{
   return Vector3f(
      xyz_m[0] / v.xyz_m[0],
      xyz_m[1] / v.xyz_m[1],
      xyz_m[2] / v.xyz_m[2] );
}


Vector3f Vector3f::operator*
(
   const float f
) const
{
   return Vector3f(
      xyz_m[0] * f,
      xyz_m[1] * f,
      xyz_m[2] * f );
}


Vector3f Vector3f::operator/
(
   const float f
) const
{
   const float oneOverF = 1.0f / f;

   return Vector3f(
      xyz_m[0] * oneOverF,
      xyz_m[1] * oneOverF,
      xyz_m[2] * oneOverF );
}


bool Vector3f::operator==
(
   const Vector3f& v
) const
{
   return
      (xyz_m[0] == v.xyz_m[0]) &
      (xyz_m[1] == v.xyz_m[1]) &
      (xyz_m[2] == v.xyz_m[2]);
}


bool Vector3f::operator!=
(
   const Vector3f& v
) const
{
   return
      (xyz_m[0] != v.xyz_m[0]) |
      (xyz_m[1] != v.xyz_m[1]) |
      (xyz_m[2] != v.xyz_m[2]);
}


bool Vector3f::isZero() const
{
   return bool(
      (xyz_m[0] == 0.0f) &
      (xyz_m[1] == 0.0f) &
      (xyz_m[2] == 0.0f) );
}


Vector3f Vector3f::sign() const
{
   return compare( Vector3f::ZERO() );
}


Vector3f Vector3f::compare
(
   const Vector3f& v
) const
{
   return Vector3f(
      xyz_m[0] > v.xyz_m[0] ? +1.0f : (xyz_m[0] == v.xyz_m[0] ? 0.0f : -1.0f),
      xyz_m[1] > v.xyz_m[1] ? +1.0f : (xyz_m[1] == v.xyz_m[1] ? 0.0f : -1.0f),
      xyz_m[2] > v.xyz_m[2] ? +1.0f : (xyz_m[2] == v.xyz_m[2] ? 0.0f : -1.0f) );
}


Vector3f Vector3f::equal
(
   const Vector3f& v
) const
{
   return Vector3f(
      static_cast<float>( xyz_m[0] == v.xyz_m[0] ),
      static_cast<float>( xyz_m[1] == v.xyz_m[1] ),
      static_cast<float>( xyz_m[2] == v.xyz_m[2] ) );
}


Vector3f Vector3f::operator>
(
   const Vector3f& v
) const
{
   return Vector3f(
      static_cast<float>( xyz_m[0] > v.xyz_m[0] ),
      static_cast<float>( xyz_m[1] > v.xyz_m[1] ),
      static_cast<float>( xyz_m[2] > v.xyz_m[2] ) );
}


Vector3f Vector3f::operator>=
(
   const Vector3f& v
) const
{
   return Vector3f(
      static_cast<float>( xyz_m[0] >= v.xyz_m[0] ),
      static_cast<float>( xyz_m[1] >= v.xyz_m[1] ),
      static_cast<float>( xyz_m[2] >= v.xyz_m[2] ) );
}


Vector3f Vector3f::operator<
(
   const Vector3f& v
) const
{
   return Vector3f(
      static_cast<float>( xyz_m[0] < v.xyz_m[0] ),
      static_cast<float>( xyz_m[1] < v.xyz_m[1] ),
      static_cast<float>( xyz_m[2] < v.xyz_m[2] ) );
}


Vector3f Vector3f::operator<=
(
   const Vector3f& v
) const
{
   return Vector3f(
      static_cast<float>( xyz_m[0] <= v.xyz_m[0] ),
      static_cast<float>( xyz_m[1] <= v.xyz_m[1] ),
      static_cast<float>( xyz_m[2] <= v.xyz_m[2] ) );
}


Vector3f Vector3f::clampedMin
(
   const Vector3f& min
) const
{
   Vector3f r( *this );

   r.xyz_m[0] = r.xyz_m[0] >= min.xyz_m[0] ? r.xyz_m[0] : min.xyz_m[0];
   r.xyz_m[1] = r.xyz_m[1] >= min.xyz_m[1] ? r.xyz_m[1] : min.xyz_m[1];
   r.xyz_m[2] = r.xyz_m[2] >= min.xyz_m[2] ? r.xyz_m[2] : min.xyz_m[2];

   return r;
}


Vector3f Vector3f::clampedMax
(
   const Vector3f& max
) const
{
   Vector3f r( *this );

   r.xyz_m[0] = r.xyz_m[0] <= max.xyz_m[0] ? r.xyz_m[0] : max.xyz_m[0];
   r.xyz_m[1] = r.xyz_m[1] <= max.xyz_m[1] ? r.xyz_m[1] : max.xyz_m[1];
   r.xyz_m[2] = r.xyz_m[2] <= max.xyz_m[2] ? r.xyz_m[2] : max.xyz_m[2];

   return r;
}


Vector3f Vector3f::clamped
(
   const Vector3f& min,
   const Vector3f& max
) const
{
   Vector3f r( *this );

   for( dword i = 3;  i-- > 0; )
   {
      r.xyz_m[i] = r.xyz_m[i] >= min.xyz_m[i] ? r.xyz_m[i] : min.xyz_m[i];
      r.xyz_m[i] = r.xyz_m[i] <= max.xyz_m[i] ? r.xyz_m[i] : max.xyz_m[i];
   }

   return r;
}


Vector3f Vector3f::clamped01() const   // [0,1)  0 to almost 1
{
   Vector3f result;

   if( xyz_m[0] >= 1.0f )
      result.xyz_m[0] = FLOAT_ALMOST_ONE;
   else
   if( xyz_m[0] < 0.0f )
      result.xyz_m[0] = 0.0f;
   else
      result.xyz_m[0] = xyz_m[0];

   if( xyz_m[1] >= 1.0f )
      result.xyz_m[1] = FLOAT_ALMOST_ONE;
   else
   if( xyz_m[1] < 0.0f )
      result.xyz_m[1] = 0.0f;
   else
      result.xyz_m[1] = xyz_m[1];

   if( xyz_m[2] >= 1.0f )
      result.xyz_m[2] = FLOAT_ALMOST_ONE;
   else
   if( xyz_m[2] < 0.0f )
      result.xyz_m[2] = 0.0f;
   else
      result.xyz_m[2] = xyz_m[2];

   return result;

   //result = *this;
   //result.clamp01();
}




/// friends --------------------------------------------------------------------
Vector3f hxa7241_graphics::operator*
(
   const float     f,
   const Vector3f& v
)
{
   return Vector3f(
      f * v.xyz_m[0],
      f * v.xyz_m[1],
      f * v.xyz_m[2] );
}




Vector3f hxa7241_graphics::operator/
(
   const float     f,
   const Vector3f& v
)
{
   return Vector3f(
      f / v.xyz_m[0],
      f / v.xyz_m[1],
      f / v.xyz_m[2] );
}




/// constants ------------------------------------------------------------------
const Vector3f& Vector3f::ZERO()
{
   static const Vector3f k( 0.0f, 0.0f, 0.0f );
   return k;
}


const Vector3f& Vector3f::HALF()
{
   static const Vector3f k( 0.5f, 0.5f, 0.5f );
   return k;
}


const Vector3f& Vector3f::ONE()
{
   static const Vector3f k( 1.0f, 1.0f, 1.0f );
   return k;
}


const Vector3f& Vector3f::EPSILON()
{
   static const Vector3f k( FLOAT_EPSILON, FLOAT_EPSILON, FLOAT_EPSILON );
   return k;
}


const Vector3f& Vector3f::ALMOST_ONE()
{
   static const Vector3f k(
      FLOAT_ALMOST_ONE,
      FLOAT_ALMOST_ONE,
      FLOAT_ALMOST_ONE );
   return k;
}


const Vector3f& Vector3f::MIN()
{
   static const Vector3f k( FLOAT_MIN_NEG, FLOAT_MIN_NEG, FLOAT_MIN_NEG );
   return k;
}


const Vector3f& Vector3f::MAX()
{
   static const Vector3f k( FLOAT_MAX, FLOAT_MAX, FLOAT_MAX );
   return k;
}


const Vector3f& Vector3f::SMALL()
{
   static const Vector3f k( FLOAT_SMALL, FLOAT_SMALL, FLOAT_SMALL );
   return k;
}


const Vector3f& Vector3f::LARGE()
{
   static const Vector3f k( FLOAT_LARGE, FLOAT_LARGE, FLOAT_LARGE );
   return k;
}


const Vector3f& Vector3f::X()
{
   static const Vector3f k( 1.0f, 0.0f, 0.0f );
   return k;
}


const Vector3f& Vector3f::Y()
{
   static const Vector3f k( 0.0f, 1.0f, 0.0f );
   return k;
}


const Vector3f& Vector3f::Z()
{
   static const Vector3f k( 0.0f, 0.0f, 1.0f );
   return k;
}




/// streaming ------------------------------------------------------------------
#ifdef TESTING


#include <istream>
#include <ostream>


namespace hxa7241_graphics
{

std::ostream& operator<<
(
   std::ostream&   out,
   const Vector3f& obj
)
{
   return out << '(' << obj[0] << ' ' << obj[1] << ' ' << obj[2] << ')';
}


std::istream& operator>>
(
   std::istream& in,
   Vector3f&     obj
)
{
   char  c;
   float xyz[3];

   in >> c >> xyz[0] >> xyz[1] >> xyz[2] >> c;
   obj.set( xyz );

   return in;
}

}


#endif//TESTING
