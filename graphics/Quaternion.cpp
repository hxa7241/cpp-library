/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include <math.h>

#include "Polar.hpp"
#include "UnitVector3f.hpp"
#include "Matrix3f.hpp"

#include "Quaternion.hpp"


using namespace hxa7241_graphics;




/// standard object services ---------------------------------------------------
Quaternion::Quaternion()
 : s_m( 1.0f )
 , v_m()
{
}


Quaternion::Quaternion
(
   const float         angle,
   const UnitVector3f& axis
)
{
   Quaternion::setAngleAndAxis( angle, axis );
}


Quaternion::Quaternion
(
   const Matrix3f& m
)
{
   Quaternion::setMatrix( m );
}


Quaternion::Quaternion
(
   const float rotationX,
   const float rotationY,
   const float rotationZ
)
{
   Quaternion::setRotations( rotationX, rotationY, rotationZ );
}


Quaternion::Quaternion
(
   const float     s,
   const Vector3f& v,
   const bool      //dummy
)
{
   Quaternion::setElements( s, v );
}


Quaternion::Quaternion
(
   const float s,
   const float vx,
   const float vy,
   const float vz
)
{
   Quaternion::setElements( s, vx, vy, vz );
}


Quaternion::~Quaternion()
{
}


Quaternion::Quaternion
(
   const Quaternion& other
)
 : s_m( other.s_m )
 , v_m( other.v_m )
{
}


Quaternion& Quaternion::operator=
(
   const Quaternion& other
)
{
   if( &other != this )
   {
      s_m = other.s_m;
      v_m = other.v_m;
   }

   return *this;
}




/// commands -------------------------------------------------------------------
void Quaternion::setAngleAndAxis
(
   const float         angle,
   const UnitVector3f& axis
)
{
   float sine;
   float cosine;
   hxa7241_graphics::sinAndcos( angle * 0.5f, sine, cosine );

   s_m = cosine;
   (v_m = axis.get()) *= sine;
}


void Quaternion::setMatrix
(
   const Matrix3f& m
)
{
   const Vector3f& row0 = m.getRow0();
   const Vector3f& row1 = m.getRow1();
   const Vector3f& row2 = m.getRow2();
   const float me[3][3] =
      { { row0.getX(), row1.getX(), row2.getX() },
        { row0.getY(), row1.getY(), row2.getY() },
        { row0.getZ(), row1.getZ(), row2.getZ() } };
   // ALTERNATE FORM //
   //const float me[3][3] =
   //   { { row0.getX(), row0.getY(), row0.getZ() },
   //     { row1.getX(), row1.getY(), row1.getZ() },
   //     { row2.getX(), row2.getY(), row2.getZ() } };


   const float trace = me[0][0] + me[1][1] + me[2][2] + 1.0f;

   if( trace > 0.00000001f )
   {
      const float a = ::sqrtf(trace) * 0.5f;
      const float b = 0.25f / a;

      setElements( a,
         (me[2][1] - me[1][2]) * b,
         (me[0][2] - me[2][0]) * b,
         (me[1][0] - me[0][1]) * b );
   }
   else
   {
      if( (me[0][0] > me[1][1]) & (me[0][0] > me[2][2]) )
      {
         const float a = ::sqrtf( (me[0][0] - (me[1][1] + me[2][2])) +
            1.0f ) * 0.5f;
         const float b = 0.25f / a;

         setElements(
            (me[2][1] - me[1][2]) * b,
            a,
            (me[0][1] + me[1][0]) * b,
            (me[2][0] + me[0][2]) * b );
      }
      else if( me[1][1] > me[2][2] )
      {
         const float a = ::sqrtf((me[1][1] - (me[2][2] + me[0][0])) +
            1.0f ) * 0.5f;
         const float b = 0.25f / a;

         setElements(
            (me[0][2] - me[2][0]) * b,
            (me[0][1] + me[1][0]) * b,
            a,
            (me[1][2] + me[2][1]) * b );
      }
      else
      {
         const float a = ::sqrtf((me[2][2] - (me[0][0] + me[1][1])) +
            1.0f ) * 0.5f;
         const float b = 0.25f / a;

         setElements(
            (me[1][0] - me[0][1]) * b,
            (me[2][0] + me[0][2]) * b,
            (me[1][2] + me[2][1]) * b,
            a );
      }
   }
}


void Quaternion::setRotations
(
   const float rotationX,
   const float rotationY,
   const float rotationZ
)
{
   const Quaternion qx( rotationX, UnitVector3f::X() );
   const Quaternion qy( rotationY, UnitVector3f::Y() );
   const Quaternion qz( rotationZ, UnitVector3f::Z() );

   ((*this = qz) *= qy) *= qx;
}


Quaternion& Quaternion::invert()
{
   v_m.negateEq();

   return *this;
}


Quaternion& Quaternion::operator*=
(
   const Quaternion& other
)
{
   const float w = (s_m * other.s_m) - v_m.dot( other.v_m );
   v_m = (((other.v_m * s_m) += (v_m * other.s_m)) += v_m.cross( other.v_m ));
   s_m = w;

//   const float w = (s_m * other.s_m) - ((v_m.getX() * other.v_m.getX()) +
//      (v_m.getY() * other.v_m.getY()) + (v_m.getZ() * other.v_m.getZ()));
//   const float x = (s_m * other.v_m.getX()) + (other.s_m * v_m.getX()) +
//      (v_m.getY() * other.v_m.getZ()) - (v_m.getZ() * other.v_m.getY());
//   const float y = (s_m * other.v_m.getY()) + (other.s_m * v_m.getY()) +
//      (v_m.getZ() * other.v_m.getX()) - (v_m.getX() * other.v_m.getZ());
//   const float z = (s_m * other.v_m.getZ()) + (other.s_m * v_m.getZ()) +
//      (v_m.getX() * other.v_m.getY()) - (v_m.getY() * other.v_m.getX());
//
//   s_m = w;
//   v_m.set( x, y, z );

   return *this;
}


Quaternion& Quaternion::operator*=
(
   const float scaling
)
{
   float        angle;
   UnitVector3f axis;

   getAngleAndAxis( angle, axis );

   angle *= scaling;

   setAngleAndAxis( angle, axis );

   return *this;
}




/// queries --------------------------------------------------------------------
void Quaternion::getAngleAndAxis
(
   float&        angle,
   UnitVector3f& axis
) const
{
   const float cosine = s_m;

   float sine = ::sqrtf( 1.0f - (cosine * cosine) );
   if( ::fabsf(sine) < 0.0005f )
   {
      sine = 1.0f;
   }
   const float oneOverSine = 1.0f / sine;


   angle = ::acosf( cosine ) * 2.0f;
   axis.set( Vector3f(
      v_m.getX() * oneOverSine,
      v_m.getY() * oneOverSine,
      v_m.getZ() * oneOverSine ) );
}


void Quaternion::getMatrix
(
   Matrix3f& m
) const
{
   const float xx = v_m.getX() * v_m.getX();
   const float yy = v_m.getY() * v_m.getY();
   const float zz = v_m.getZ() * v_m.getZ();
   const float ss = s_m        * s_m;
   const float xy = v_m.getX() * v_m.getY();
   const float sz = s_m        * v_m.getZ();
   const float xz = v_m.getX() * v_m.getZ();
   const float sy = s_m        * v_m.getY();
   const float yz = v_m.getY() * v_m.getZ();
   const float sx = s_m        * v_m.getX();

   m.setElements(
      ss + xx - yy - zz,  2.0f * (xy + sz),  2.0f * (xz - sy),  0,
      2.0f * (xy - sz),  ss - xx + yy - zz,  2.0f * (yz + sx),  0,
      2.0f * (xz + sy),  2.0f * (yz - sx),  ss - xx - yy + zz,  0 );
   // ALTERNATE FORM //
   //m.setElements(
   //   ss + xx - yy - zz,  2.0f * (xy - sz),  2.0f * (xz + sy),  0,
   //   2.0f * (xy + sz),  ss - xx + yy - zz,  2.0f * (yz - sx),  0,
   //   2.0f * (xz - sy),  2.0f * (yz + sx),  ss - xx - yy + zz,  0 );
}


Quaternion Quaternion::getInverse() const
{
   return Quaternion( *this ).invert();
}


Quaternion Quaternion::operator*
(
   const Quaternion& other
) const
{
   return Quaternion( *this ) *= other;
}


Quaternion Quaternion::operator*
(
   const float scaling
) const
{
   return Quaternion( *this ) *= scaling;
}


void Quaternion::rotate
(
   Vector3f& point
) const
{
   // make non-unitized quaternion
   Quaternion qp;
   qp.s_m = 0.0f;
   qp.v_m = point;

   point = ((getInverse() *= qp) *= *this).v_m;
   // ALTERNATE FORM //
   //point = ((*this * qp) *= getInverse()).v_m;
}


bool Quaternion::operator==
(
   const Quaternion& other
) const
{
   return (s_m == other.s_m) & (v_m == other.v_m);
}


bool Quaternion::operator!=
(
   const Quaternion& other
) const
{
   return (s_m != other.s_m) | (v_m != other.v_m);
}




/// constants ------------------------------------------------------------------
const Quaternion& Quaternion::QUARTER_X()
{
   static const Quaternion k( 0.70710678118654752440084436210485f,
      0.70710678118654752440084436210485f, 0.0f, 0.0f );
   return k;
}


const Quaternion& Quaternion::QUARTER_Y()
{
   static const Quaternion k( 0.70710678118654752440084436210485f,
      0.0f, 0.70710678118654752440084436210485f, 0.0f );
   return k;
}


const Quaternion& Quaternion::QUARTER_Z()
{
   static const Quaternion k( 0.70710678118654752440084436210485f,
      0.0f, 0.0f, 0.70710678118654752440084436210485f );
   return k;
}




/// implementation -------------------------------------------------------------
void Quaternion::setElements
(
   const float     s,
   const Vector3f& v
)
{
   s_m = s;
   v_m = v;

   unitize();
}


void Quaternion::setElements
(
   const float s,
   const float vx,
   const float vy,
   const float vz
)
{
   s_m = s;
   v_m.set( vx, vy, vz );

   unitize();
}


void Quaternion::unitize()
{
   const float dotSelf  = (s_m * s_m) + v_m.dot( v_m );

   if( dotSelf != 1.0f )
   {
      const float length = ::sqrtf( dotSelf );

      if( length == 0.0f )
      {
         s_m = 1.0f;
         v_m = Vector3f::ZERO();
      }
      else
      {
         const float oneOverLength = 1.0f / length;

         s_m *= oneOverLength;
         v_m *= oneOverLength;
      }
   }
}








/// test -----------------------------------------------------------------------
#ifdef TESTING


#include <iostream>


namespace hxa7241_graphics
{

std::ostream& operator<<( std::ostream&, const Vector3f& );
std::istream& operator>>( std::istream&,       Vector3f& );


// for casting to and then accessing private fields
struct QImposter
{
   float    s_m;
   Vector3f v_m;
};


bool test_Quaternion
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   //seed
)
{
   bool isSucceeded = true;

   if( pOut ) *pOut << "[ test_Quaternion ]\n\n";


   static const float TOLERANCE = 1e-4f;


   // to and from angle,axis are opposites
   {
      Quaternion q;

      // to
      const float        angle0 = 3.590392f;
      const UnitVector3f axis0( Vector3f( 1.0f, -0.5f, 0.3f ) );
      q.setAngleAndAxis( angle0, axis0 );

      // from
      float        angle1;
      UnitVector3f axis1;
      q.getAngleAndAxis( angle1, axis1 );

      const bool isOk = (::fabsf(angle0 - angle1) < TOLERANCE) &
         (axis0.get().dot(axis1.get()) > 0.999f);
      isSucceeded &= isOk;

      if( pOut && isVerbose ) *pOut << "angles dif: " << (angle1 - angle0) <<
         "  axiss dot: " << axis0.get().dot(axis1.get()) << "\n";
      if( pOut && isVerbose ) *pOut << "\n";
      if( pOut ) *pOut << "to/from angle,axis : " <<
         (isOk ? "--- succeeded" : "*** failed") << "\n\n";
   }


   // to and from matrix are opposites
   {
      Quaternion q;

      // to
      Matrix3f m0;
      {
         Matrix3f mx;
         Matrix3f my;
         Matrix3f mz;
         mx.setToRotateX( 1.0f );
         my.setToRotateY( 2.0f );
         mz.setToRotateZ( 4.0f );

         ((m0 = mz) *= my) *= (mx);
      }
      q.setMatrix( m0 );

      // from
      Matrix3f m1;
      q.getMatrix( m1 );

      bool isOk = true;
      {
         Vector3f d[3] = { Vector3f(m1.getRow0() - m0.getRow0()),
            Vector3f(m1.getRow1() - m0.getRow1()),
            Vector3f(m1.getRow2() - m0.getRow2()) };
         for( dword i = 3;  i-- > 0; )
         {
            isOk &= ::fabsf(d[i].getX()) < TOLERANCE;
            isOk &= ::fabsf(d[i].getY()) < TOLERANCE;
            isOk &= ::fabsf(d[i].getZ()) < TOLERANCE;
         }
         isOk &= m1.getCol3().isZero();
      }

      isSucceeded &= isOk;

      if( pOut && isVerbose ) *pOut << "matr row0 dif: " <<
         (m1.getRow0() - m0.getRow0()) << "\n";
      if( pOut && isVerbose ) *pOut << "matr row1 dif: " <<
         (m1.getRow1() - m0.getRow1()) << "\n";
      if( pOut && isVerbose ) *pOut << "matr row2 dif: " <<
         (m1.getRow2() - m0.getRow2()) << "\n";
      if( pOut && isVerbose ) *pOut << "matr col3 dif: " <<
         (m1.getCol3() - m0.getCol3()) << "\n";
      if( pOut && isVerbose ) *pOut << "\n";
      if( pOut ) *pOut << "to/from matrix : " <<
         (isOk ? "--- succeeded" : "*** failed") << "\n\n";
   }


   // invert is self opposite
   {
      const Quaternion q0( 1.0f, 2.0f, 4.0f );

      Quaternion q1( q0 );
      q1.invert();
      q1.invert();

      const QImposter& q0_ = *reinterpret_cast<const QImposter*>(&q0);
      const QImposter& q1_ = *reinterpret_cast<const QImposter*>(&q1);

      const bool isOk = (::fabsf(q1_.s_m - q0_.s_m) < TOLERANCE) &
         (::fabsf(q1_.v_m.getX() - q0_.v_m.getX()) < TOLERANCE) &
         (::fabsf(q1_.v_m.getY() - q0_.v_m.getY()) < TOLERANCE) &
         (::fabsf(q1_.v_m.getZ() - q0_.v_m.getZ()) < TOLERANCE);
      isSucceeded &= isOk;

      if( pOut && isVerbose ) *pOut << "quat dif: " << (q1_.s_m - q0_.s_m) <<
         ", " << (q1_.v_m - q0_.v_m) << "\n";
      if( pOut && isVerbose ) *pOut << "\n";
      if( pOut ) *pOut << "invert self-opposite : " <<
         (isOk ? "--- succeeded" : "*** failed") << "\n\n";
   }


   // all commands maintain invariants - unitization
   {
      bool isOk = true;

      Quaternion q;
      const QImposter& q_ = *reinterpret_cast<const QImposter*>(&q);

      q.setAngleAndAxis( 3.590392f, UnitVector3f( Vector3f( 1.0f, -0.5f, 0.3f )
         ) );
      isOk &= ::fabsf( ::sqrtf( (q_.s_m * q_.s_m) + q_.v_m.dot( q_.v_m )) - 1.0f
         ) < TOLERANCE;
      if( pOut && isVerbose ) *pOut << ::sqrtf( (q_.s_m * q_.s_m) +
         q_.v_m.dot( q_.v_m ) ) << " ";

      {
         Matrix3f m0;
         {
            Matrix3f mx;
            Matrix3f my;
            Matrix3f mz;
            mx.setToRotateX( 1.0f );
            mx.setToRotateY( 2.0f );
            mx.setToRotateZ( 4.0f );

            ((m0 = mx) *= my) *= mz;
         }
         q.setMatrix( m0 );
         isOk &= ::fabsf( ::sqrtf( (q_.s_m * q_.s_m) + q_.v_m.dot( q_.v_m ) ) -
            1.0f ) < TOLERANCE;
         if( pOut && isVerbose ) *pOut << ::sqrtf( (q_.s_m * q_.s_m) +
            q_.v_m.dot( q_.v_m ) ) << " ";
      }

      q.setRotations( 1.0f, 2.0f, 4.0f );
      isOk &= ::fabsf( ::sqrtf( (q_.s_m * q_.s_m) + q_.v_m.dot( q_.v_m )) - 1.0f
         ) < TOLERANCE;
      if( pOut && isVerbose ) *pOut << ::sqrtf( (q_.s_m * q_.s_m) +
         q_.v_m.dot( q_.v_m ) ) << " ";

      q.invert();
      isOk &= ::fabsf( ::sqrtf( (q_.s_m * q_.s_m) + q_.v_m.dot( q_.v_m )) - 1.0f
         ) < TOLERANCE;
      if( pOut && isVerbose ) *pOut << ::sqrtf( (q_.s_m * q_.s_m) +
         q_.v_m.dot( q_.v_m ) ) << " ";

      const Quaternion q1( 1.0f, UnitVector3f( Vector3f( -0.25f, 10.0f, 0.3f ) )
         );
      q.operator*=( q1 );
      isOk &= ::fabsf( ::sqrtf( (q_.s_m * q_.s_m) + q_.v_m.dot( q_.v_m )) - 1.0f
         ) < TOLERANCE;
      if( pOut && isVerbose ) *pOut << ::sqrtf( (q_.s_m * q_.s_m) +
         q_.v_m.dot( q_.v_m ) ) << " ";

      q.setRotations( 1.0f, 2.0f, 4.0f );
      q.operator*=( 0.837405f );
      isOk &= ::fabsf( ::sqrtf( (q_.s_m * q_.s_m) + q_.v_m.dot( q_.v_m )) - 1.0f
         ) < TOLERANCE;
      if( pOut && isVerbose ) *pOut << ::sqrtf( (q_.s_m * q_.s_m) +
         q_.v_m.dot( q_.v_m ) ) << " ";

//      q.setElements( 1.0f, -2.2f, 3.3f, 11.0f );
//      isOk &= ::fabsf( ::sqrtf( (q_.s_m * q_.s_m) + q_.v_m.dot( q_.v_m )) - 1.0f
//         ) < TOLERANCE;
//      if( pOut && isVerbose ) *pOut << ::sqrtf( (q_.s_m * q_.s_m) +
//         q_.v_m.dot( q_.v_m ) ) << " ";
//
//      q.unitize();
//      isOk &= ::fabsf( ::sqrtf( (q_.s_m * q_.s_m) + q_.v_m.dot( q_.v_m )) - 1.0f
//         ) < TOLERANCE;
//      if( pOut && isVerbose ) *pOut << ::sqrtf( (q_.s_m * q_.s_m) +
//         q_.v_m.dot( q_.v_m ) ) << " ";

      isSucceeded &= isOk;

      if( pOut && isVerbose ) *pOut << "\n\n";
      if( pOut ) *pOut << "invariant : " <<
         (isOk ? "--- succeeded" : "*** failed") << "\n\n";
   }


   // invert is invert
   {
      const Vector3f p0( 10.0f, 22.0f, -33.0f );

      // set quaternion and point
      Quaternion q( 3.590392f, UnitVector3f( Vector3f( 1.0f, -0.5f, 0.3f ) ) );
      Vector3f   p1( p0 );

      // rotate point
      q.rotate( p1 );

      // invert quaternion
      q.invert();

      // rotate point
      q.rotate( p1 );

      // test equality with original point
      const bool isOk = (::fabsf(p1.getX() - p0.getX()) < TOLERANCE) &
         (::fabsf(p1.getY() - p0.getY()) < TOLERANCE) &
         (::fabsf(p1.getZ() - p0.getZ()) < TOLERANCE);
      isSucceeded &= isOk;

      if( pOut && isVerbose ) *pOut << "point dif: " << (p1 - p0) << "\n";
      if( pOut && isVerbose ) *pOut << "\n";
      if( pOut ) *pOut << "invert : " <<
         (isOk ? "--- succeeded" : "*** failed") << "\n\n";
   }


   // rotations are same
   {
      const Vector3f p0( 10.0f, 22.0f, -33.0f );

      // set matrix from x,y,z
      Matrix3f m0;
      {
         Matrix3f mx;
         Matrix3f my;
         Matrix3f mz;
         mx.setToRotateX( 1.0f );
         my.setToRotateY( 2.0f );
         mz.setToRotateZ( 4.0f );

         ((m0 = mx) *= my) *= mz;
      }

      // set quaternion a from matrix
      Quaternion qm( m0 );

      // set quaternion b from x,y,z
      Quaternion qr( 1.0f, 2.0f, 4.0f );

      Vector3f pm0( p0 );
      Vector3f pqm( p0 );
      Vector3f pqr( p0 );

      // rotate point
      pm0 = m0 * pm0;
      qm.rotate( pqm );
      qr.rotate( pqr );

      // test equality
      bool isOk = (::fabsf(pqm.getX() - pm0.getX()) < TOLERANCE) &
         (::fabsf(pqm.getY() - pm0.getY()) < TOLERANCE) &
         (::fabsf(pqm.getZ() - pm0.getZ()) < TOLERANCE);
      isOk &= (::fabsf(pqr.getX() - pm0.getX()) < TOLERANCE) &
         (::fabsf(pqr.getY() - pm0.getY()) < TOLERANCE) &
         (::fabsf(pqr.getZ() - pm0.getZ()) < TOLERANCE);
      isSucceeded &= isOk;

      if( pOut && isVerbose ) *pOut << "quat from matr dif: " <<
         (pqm - pm0) << "\n";
      if( pOut && isVerbose ) *pOut << "quat from rots dif: " <<
         (pqr - pm0) << "\n";
      if( pOut && isVerbose ) *pOut << "\n";
      if( pOut ) *pOut << "rotations : " <<
         (isOk ? "--- succeeded" : "*** failed") << "\n\n";
   }


   // rotation scaling correct
   {
      const Vector3f p0( 10.0f, 22.0f, -33.0f );

      Quaternion qr( 0.0f, 1.0f, 0.0f );
      Quaternion qs( 0.0f, 0.5f, 0.0f );
      qs *= 2.0f;

      Vector3f pqr( p0 );
      Vector3f pqs( p0 );

      qr.rotate( pqr );
      qs.rotate( pqs );

      // test equality
      bool isOk = (::fabsf(pqs.getX() - pqr.getX()) < TOLERANCE) &
         (::fabsf(pqs.getY() - pqr.getY()) < TOLERANCE) &
         (::fabsf(pqs.getZ() - pqr.getZ()) < TOLERANCE);
      isSucceeded &= isOk;

      if( pOut && isVerbose ) *pOut << "quat from scal dif: " <<
         (pqs - pqr) << "\n";
      if( pOut && isVerbose ) *pOut << "\n";
      if( pOut ) *pOut << "rotations scaling : " <<
         (isOk ? "--- succeeded" : "*** failed") << "\n\n";
   }


   // constants correct
   {
      const Vector3f p0( 10.0f, 22.0f, -33.0f );

      // set matrixs for quarter turns
      Matrix3f mx;
      Matrix3f my;
      Matrix3f mz;
      mx.setToRotateX( 1.5707963267948966192313216916398f );
      my.setToRotateY( 1.5707963267948966192313216916398f );
      mz.setToRotateZ( 1.5707963267948966192313216916398f );

      Vector3f pxm( p0 );
      Vector3f pym( p0 );
      Vector3f pzm( p0 );
      Vector3f pxq( p0 );
      Vector3f pyq( p0 );
      Vector3f pzq( p0 );

      // rotate point with matrixs
      pxm = mx * pxm;
      pym = my * pym;
      pzm = mz * pzm;

      // rotate point with quaternion constants
      Quaternion::QUARTER_X().rotate( pxq );
      Quaternion::QUARTER_Y().rotate( pyq );
      Quaternion::QUARTER_Z().rotate( pzq );

      // test equality
      bool isOk = (::fabsf(pxq.getX() - pxm.getX()) < TOLERANCE) &
         (::fabsf(pxq.getY() - pxm.getY()) < TOLERANCE) &
         (::fabsf(pxq.getZ() - pxm.getZ()) < TOLERANCE);
      isOk &= (::fabsf(pyq.getX() - pym.getX()) < TOLERANCE) &
         (::fabsf(pyq.getY() - pym.getY()) < TOLERANCE) &
         (::fabsf(pyq.getZ() - pym.getZ()) < TOLERANCE);
      isOk &= (::fabsf(pzq.getX() - pzm.getX()) < TOLERANCE) &
         (::fabsf(pzq.getY() - pzm.getY()) < TOLERANCE) &
         (::fabsf(pzq.getZ() - pzm.getZ()) < TOLERANCE);

      isSucceeded &= isOk;

      if( pOut && isVerbose ) *pOut << "x point dif: " << (pxq - pxm) <<
         "\n";
      if( pOut && isVerbose ) *pOut << "y point dif: " << (pyq - pym) <<
         "\n";
      if( pOut && isVerbose ) *pOut << "z point dif: " << (pzq - pzm) <<
         "\n";
      if( pOut && isVerbose ) *pOut << "\n";
      if( pOut ) *pOut << "constants : " <<
         (isOk ? "--- succeeded" : "*** failed") << "\n\n";
   }


   if( pOut ) *pOut << (isSucceeded ? "--- successfully" : "*** failurefully")<<
      " completed " << "\n\n\n";

   if( pOut ) pOut->flush();


   return isSucceeded;
}


}//namespace


#endif//TESTING
