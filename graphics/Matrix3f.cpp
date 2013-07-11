/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include <math.h>

#include "Matrix3f.hpp"


using namespace hxa7241_graphics;




/// standard object services ---------------------------------------------------
Matrix3f::Matrix3f()
 : row0_m()
 , row1_m()
 , row2_m()
 , col3_m()
{
}


Matrix3f::Matrix3f
(
   const float* a9
)
 : row0_m( a9 + 0 )
 , row1_m( a9 + 3 )
 , row2_m( a9 + 6 )
 , col3_m()
{
}


Matrix3f::Matrix3f
(
   const float m00,  const float m10,  const float m20,
   const float m01,  const float m11,  const float m21,
   const float m02,  const float m12,  const float m22
)
 : row0_m( m00, m10, m20 )
 , row1_m( m01, m11, m21 )
 , row2_m( m02, m12, m22 )
 , col3_m()
{
}


Matrix3f::Matrix3f
(
   const float m00,  const float m10,  const float m20,  const float m30,
   const float m01,  const float m11,  const float m21,  const float m31,
   const float m02,  const float m12,  const float m22,  const float m32
)
 : row0_m( m00, m10, m20 )
 , row1_m( m01, m11, m21 )
 , row2_m( m02, m12, m22 )
 , col3_m( m30, m31, m32 )
{
}


Matrix3f::Matrix3f
(
   const Vector3f& row0,
   const Vector3f& row1,
   const Vector3f& row2,
   const Vector3f& col3
)
 : row0_m( row0 )
 , row1_m( row1 )
 , row2_m( row2 )
 , col3_m( col3 )
{
}


Matrix3f::Matrix3f
(
   const Vector3f& part0,
   const Vector3f& part1,
   const Vector3f& part2,
   const Vector3f& col3,
   const bool      isRows
)
{
   if( isRows )
   {
      Matrix3f::setRowsCol( part0, part1, part2, col3 );
   }
   else
   {
      Matrix3f::setColumns( part0, part1, part2, col3 );
   }
}


Matrix3f::Matrix3f
(
   const ESet      set,
   const Vector3f& v
)
{
   switch( set )
   {
      case ROTATEXYZ : setToRotateXYZ( v[0], v[1], v[2] ); break;
      case TRANSLATE : setToTranslate( v );                break;
      case SCALE     : setToScale( v );                    break;
   }
}


Matrix3f::Matrix3f
(
   const ERotate set,
   const float   radians
)
{
   switch( set )
   {
      case ROTATEZ : setToRotateZ( radians ); break;
      case ROTATEY : setToRotateY( radians ); break;
      case ROTATEX : setToRotateX( radians ); break;
   }
}


Matrix3f::Matrix3f
(
   const Vector3f& v31,
   const Vector3f& m13
)
 : row0_m( m13 * v31.getX() )
 , row1_m( m13 * v31.getY() )
 , row2_m( m13 * v31.getZ() )
 , col3_m()
{
   // 3x1 * 1x3 makes 3x3:
   //
   // a           a1 a2 a3
   // d * 1 2 3 = d1 d2 d3
   // g           g1 g2 g3
}


Matrix3f::~Matrix3f()
{
}


Matrix3f::Matrix3f
(
   const Matrix3f& that
)
 : row0_m( that.row0_m )
 , row1_m( that.row1_m )
 , row2_m( that.row2_m )
 , col3_m( that.col3_m )
{
}


Matrix3f& Matrix3f::operator=
(
   const Matrix3f& that
)
{
   if( &that != this )
   {
      row0_m = that.row0_m;
      row1_m = that.row1_m;
      row2_m = that.row2_m;
      col3_m = that.col3_m;
   }

   return  *this;
}




/// commands -------------------------------------------------------------------
Matrix3f& Matrix3f::setElements
(
   const float* a9
)
{
   row0_m.set( a9 + 0 );
   row1_m.set( a9 + 3 );
   row2_m.set( a9 + 6 );
   col3_m = Vector3f::ZERO();

   return *this;
}


Matrix3f& Matrix3f::setElements
(
   const float m00,  const float m10,  const float m20,
   const float m01,  const float m11,  const float m21,
   const float m02,  const float m12,  const float m22
)
{
   return setElements( m00, m10, m20, 0.0f,
                       m01, m11, m21, 0.0f,
                       m02, m12, m22, 0.0f );
}


Matrix3f& Matrix3f::setElements
(
   const float m00,  const float m10,  const float m20,  const float m30,
   const float m01,  const float m11,  const float m21,  const float m31,
   const float m02,  const float m12,  const float m22,  const float m32
)
{
   row0_m.set( m00, m10, m20 );
   row1_m.set( m01, m11, m21 );
   row2_m.set( m02, m12, m22 );
   col3_m.set( m30, m31, m32 );

   return *this;
}


Matrix3f& Matrix3f::setRowsCol
(
   const Vector3f& row0,
   const Vector3f& row1,
   const Vector3f& row2,
   const Vector3f& col3
)
{
   row0_m = row0;
   row1_m = row1;
   row2_m = row2;
   col3_m = col3;

   return *this;
}


Matrix3f& Matrix3f::setColumns
(
   const Vector3f& col0,
   const Vector3f& col1,
   const Vector3f& col2,
   const Vector3f& col3
)
{
   row0_m.set( col0.getX(), col1.getX(), col2.getX() );
   row1_m.set( col0.getY(), col1.getY(), col2.getY() );
   row2_m.set( col0.getZ(), col1.getZ(), col2.getZ() );
   col3_m = col3;

   return *this;
}


Matrix3f& Matrix3f::setToScale
(
   const Vector3f& xyz
)
{
   row0_m.set( xyz.getX(), 0.0f, 0.0f );
   row1_m.set( 0.0f, xyz.getY(), 0.0f );
   row2_m.set( 0.0f, 0.0f, xyz.getZ() );
   col3_m = Vector3f::ZERO();

   return *this;
}


Matrix3f& Matrix3f::setToTranslate
(
   const Vector3f& xyz
)
{
   *this  = IDENTITY();
   col3_m = xyz;

   return *this;
}


Matrix3f& Matrix3f::setToRotateX
(
   const float radians
)
{
   const float sin = ::sinf( radians );
   const float cos = ::cosf( radians );

   row0_m.set( 1.0f, 0.0f, 0.0f );
   row1_m.set( 0.0f,  cos,  sin );
   row2_m.set( 0.0f, -sin,  cos );
   col3_m = Vector3f::ZERO();

   return *this;
}


Matrix3f& Matrix3f::setToRotateY
(
   const float radians
)
{
   const float sin = ::sinf( radians );
   const float cos = ::cosf( radians );

   row0_m.set(  cos, 0.0f, -sin );
   row1_m.set( 0.0f, 1.0f, 0.0f );
   row2_m.set(  sin, 0.0f,  cos );
   col3_m = Vector3f::ZERO();

   return *this;
}


Matrix3f& Matrix3f::setToRotateZ
(
   const float radians
)
{
   const float sin = ::sinf( radians );
   const float cos = ::cosf( radians );

   row0_m.set(  cos,  sin, 0.0f );
   row1_m.set( -sin,  cos, 0.0f );
   row2_m.set( 0.0f, 0.0f, 1.0f );
   col3_m = Vector3f::ZERO();

   return *this;
}


Matrix3f& Matrix3f::setToRotateXYZ
(
   const float radiansX,
   const float radiansY,
   const float radiansZ
)
{
   const float sinX = ::sinf( radiansX );
   const float cosX = ::cosf( radiansX );
   const float sinY = ::sinf( radiansY );
   const float cosY = ::cosf( radiansY );
   const float sinZ = ::sinf( radiansZ );
   const float cosZ = ::cosf( radiansZ );

   const float cosXsinY = cosX * sinY;
   const float sinXsinY = sinX * sinY;

   row0_m.set(
      cosY * cosZ,
      ( sinXsinY * cosZ) + (cosX * sinZ),
      (-cosXsinY * cosZ) + (sinX * sinZ) );
   row1_m.set(
      -cosY * sinZ,
      (-sinXsinY * sinZ) + (cosX * cosZ),
      ( cosXsinY * sinZ) + (sinX * cosZ) );
   row2_m.set(
      sinY,
      -sinX * cosY,
      cosX * cosY );
   col3_m = Vector3f::ZERO();

   return *this;
}


/*Matrix3f& Matrix3f::multiplyEq
(
   const Matrix3f& that
)
{
   // this  =  that . this

   Vector3f col0, col1, col2;
   getCols( col0, col1, col2 );

   row0_m.set(
      that.row0_m.dot( col0 ),
      that.row0_m.dot( col1 ),
      that.row0_m.dot( col2 ) ),
   row1_m.set(
      that.row1_m.dot( col0 ),
      that.row1_m.dot( col1 ),
      that.row1_m.dot( col2 ) ),
   row2_m.set(
      that.row2_m.dot( col0 ),
      that.row2_m.dot( col1 ),
      that.row2_m.dot( col2 ) );

   col3_m.set(
      col3_m.dot( that.row0_m ),
      col3_m.dot( that.row1_m ),
      col3_m.dot( that.row2_m ) );
   col3_m += that.col3_m;

   return *this;
}*/


Matrix3f& Matrix3f::operator*=
(
   const Matrix3f& that
)
{
   const Vector3f newCol3(
      that.col3_m.dot( row0_m ),
      that.col3_m.dot( row1_m ),
      that.col3_m.dot( row2_m ) );
   col3_m += newCol3;

   Vector3f thatCol0, thatCol1, thatCol2;
   that.getCols( thatCol0, thatCol1, thatCol2 );

   row0_m.set(
      row0_m.dot( thatCol0 ),
      row0_m.dot( thatCol1 ),
      row0_m.dot( thatCol2 ) ),
   row1_m.set(
      row1_m.dot( thatCol0 ),
      row1_m.dot( thatCol1 ),
      row1_m.dot( thatCol2 ) ),
   row2_m.set(
      row2_m.dot( thatCol0 ),
      row2_m.dot( thatCol1 ),
      row2_m.dot( thatCol2 ) );

   return *this;
}


Matrix3f& Matrix3f::operator*=
(
   const Vector3f& v
)
{
   row0_m *= v;
   row1_m *= v;
   row2_m *= v;

   return *this;
}


Matrix3f& Matrix3f::operator*=
(
   const float f
)
{
   row0_m *= f;
   row1_m *= f;
   row2_m *= f;

   return *this;
}


Matrix3f& Matrix3f::operator-=
(
   const Matrix3f& that
)
{
   row0_m -= that.row0_m;
   row1_m -= that.row1_m;
   row2_m -= that.row2_m;
   col3_m -= that.col3_m;

   return *this;
}


Matrix3f& Matrix3f::transpose()
{
   Vector3f col0, col1, col2;
   getCols( col0, col1, col2 );

   setRowsCol( col0, col1, col2, col3_m );

   return *this;
}


bool Matrix3f::invert()
{

   float m[3][3];
   row0_m.get( m[0][0], m[1][0], m[2][0] );
   row1_m.get( m[0][1], m[1][1], m[2][1] );
   row2_m.get( m[0][2], m[1][2], m[2][2] );


   const float determinant =
      ( m[0][0] * m[1][1] * m[2][2] ) - ( m[0][0] * m[2][1] * m[1][2] )  +
      ( m[1][0] * m[2][1] * m[0][2] ) - ( m[1][0] * m[0][1] * m[2][2] )  +
      ( m[2][0] * m[0][1] * m[1][2] ) - ( m[2][0] * m[1][1] * m[0][2] );
   const float oneOverDet =
      (determinant == 0.0f) ? FLOAT_LARGE : (1.0f / determinant);


   row0_m.set(
       ( m[1][1] * m[2][2] - m[1][2] * m[2][1] ) * oneOverDet,
      -( m[1][0] * m[2][2] - m[1][2] * m[2][0] ) * oneOverDet,
       ( m[1][0] * m[2][1] - m[1][1] * m[2][0] ) * oneOverDet );

   row1_m.set(
      -( m[0][1] * m[2][2] - m[0][2] * m[2][1] ) * oneOverDet,
       ( m[0][0] * m[2][2] - m[0][2] * m[2][0] ) * oneOverDet,
      -( m[0][0] * m[2][1] - m[0][1] * m[2][0] ) * oneOverDet );

   row2_m.set(
       ( m[0][1] * m[1][2] - m[0][2] * m[1][1] ) * oneOverDet,
      -( m[0][0] * m[1][2] - m[0][2] * m[1][0] ) * oneOverDet,
       ( m[0][0] * m[1][1] - m[0][1] * m[1][0] ) * oneOverDet );


   const Vector3f negCol3( -col3_m );

   col3_m.set(
      negCol3.dot( row0_m ),
      negCol3.dot( row1_m ),
      negCol3.dot( row2_m ) );


   return (determinant > FLOAT_SMALL) | (determinant < -FLOAT_SMALL);


// // array[][] versn
// const float det =
//    ( m[0][0] * m[1][1] * m[2][2] ) - ( m[0][0] * m[2][1] * m[1][2] ) +
//    ( m[1][0] * m[2][1] * m[0][2] ) - ( m[1][0] * m[0][1] * m[2][2] ) +
//    ( m[2][0] * m[0][1] * m[1][2] ) - ( m[2][0] * m[1][1] * m[0][2] );
// const float oneOverDet = (det == 0.0f) ? FLOAT_LARGE : (1.0f / det);
//
// Matrix3f result;
//
// result.m[0][0] =  ( m[1][1] * m[2][2] - m[1][2] * m[2][1] ) * oneOverDet;
// result.m[1][0] = -( m[1][0] * m[2][2] - m[1][2] * m[2][0] ) * oneOverDet;
// result.m[2][0] =  ( m[1][0] * m[2][1] - m[1][1] * m[2][0] ) * oneOverDet;
//
// result.m[0][1] = -( m[0][1] * m[2][2] - m[0][2] * m[2][1] ) * oneOverDet;
// result.m[1][1] =  ( m[0][0] * m[2][2] - m[0][2] * m[2][0] ) * oneOverDet;
// result.m[2][1] = -( m[0][0] * m[2][1] - m[0][1] * m[2][0] ) * oneOverDet;
//
// result.m[0][2] =  ( m[0][1] * m[1][2] - m[0][2] * m[1][1] ) * oneOverDet;
// result.m[1][2] = -( m[0][0] * m[1][2] - m[0][2] * m[1][0] ) * oneOverDet;
// result.m[2][2] =  ( m[0][0] * m[1][1] - m[0][1] * m[1][0] ) * oneOverDet;
//
// *this = result;
//
// return det != 0.0f;
}




/// queries --------------------------------------------------------------------
void Matrix3f::getElements
(
   float* pFloats9
) const
{
   row0_m.get( pFloats9 + 0 );
   row1_m.get( pFloats9 + 3 );
   row2_m.get( pFloats9 + 6 );
}


void Matrix3f::getRows
(
   Vector3f& row0,
   Vector3f& row1,
   Vector3f& row2
) const
{
   row0 = row0_m;
   row1 = row1_m;
   row2 = row2_m;
}


void Matrix3f::getCols
(
   Vector3f& col0,
   Vector3f& col1,
   Vector3f& col2
) const
{
   col0.set( row0_m.getX(), row1_m.getX(), row2_m.getX() );
   col1.set( row0_m.getY(), row1_m.getY(), row2_m.getY() );
   col2.set( row0_m.getZ(), row1_m.getZ(), row2_m.getZ() );
}


/*void Matrix3f::getRotations
(
   Vector3f& rotations
) const
{
   // algorithm from 'the matrix and quaternion faq'
   // doesnt work properly

   float angleX;// = 0.0f;
   float angleY;// = 0.0f;
   float angleZ;// = 0.0f;

   const float d = ::asinf( row2_m.getX() );
   const float c = ::cosf( d );

   angleY = d;

   if( ::fabsf(c) > 0.005f )
   {
      const float oneOverC = 1.0f / c;

      const float trX1 =  row2_m.getZ() * oneOverC;
      const float trY1 = -row2_m.getY() * oneOverC;

      angleX = ::atan2f( trY1, trX1 );

      const float trX2 =  row0_m.getX() * oneOverC;
      const float trY2 = -row1_m.getX() * oneOverC;

      angleZ = ::atan2f( trY2, trX2 );
   }
   else
   {
      angleX = 0.0f;

      const float trX =  row1_m.getY();
      const float trY = -row0_m.getY();

      angleZ = ::atan2f( trY, trX );
   }

   rotations.set( angleX, angleY, angleZ );
}*/


Matrix3f Matrix3f::operator*
(
   const Matrix3f& that
) const
{
   return Matrix3f( *this ) *= that;
}


/*void Matrix3f::multiply
(
   const Vector3f& v,
   Vector3f&       result
) const
{
   result.set(
      v.dot( row0_m ),
      v.dot( row1_m ),
      v.dot( row2_m ) );

   result += col3_m;
}


Vector3f& Matrix3f::multiply
(
   Vector3f& v
) const
{
   v.set(
      v.dot( row0_m ),
      v.dot( row1_m ),
      v.dot( row2_m ) );
   v += col3_m;

   return v;
}*/


Matrix3f Matrix3f::operator-
(
   const Matrix3f& that
) const
{
   return Matrix3f( *this ) -= that;
}


Matrix3f Matrix3f::transposed() const
{
   return Matrix3f( *this ).transpose();
}


Matrix3f Matrix3f::inverted
(
   const char* exceptionMessage
) const
{
   Matrix3f i( *this );
   if( !i.invert() )
   {
      throw exceptionMessage;
   }

   return i;
}


bool Matrix3f::operator==
(
   const Matrix3f& that
) const
{
   return
      ( row0_m == that.row0_m ) &&
      ( row1_m == that.row1_m ) &&
      ( row2_m == that.row2_m ) &&
      ( col3_m == that.col3_m );
}


bool Matrix3f::operator!=
(
   const Matrix3f& that
) const
{
   return !operator==( that );
}




/// constants ------------------------------------------------------------------
const Matrix3f& Matrix3f::ZERO()
{
   static const Matrix3f k(
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f );
   return k;
}


const Matrix3f& Matrix3f::IDENTITY()
{
   static const Matrix3f k(
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f );
   return k;
}




/// friends --------------------------------------------------------------------
Matrix3f hxa7241_graphics::operator*
(
   const float     f,
   const Matrix3f& m
)
{
   return Matrix3f( m ) *= f;
}








/// test -----------------------------------------------------------------------
#ifdef TESTING


#include <istream>
#include <ostream>


namespace
{

bool isAlmostEqual
(
   const Matrix3f& m0,
   const Matrix3f& m1
)
{
   static const float TOLERANCE = 1e-3f;

   float a9[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
   m0.getElements( a9 );
   float b9[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
   m1.getElements( b9 );

   bool is = true;
   for( int i = 9;  i-- > 0; )
   {
      is &= ::fabsf(a9[i] - b9[i]) < TOLERANCE;
   }

   return is;
}

}


namespace hxa7241_graphics
{

std::ostream& operator<<( std::ostream&, const Vector3f& );
std::istream& operator>>( std::istream&,       Vector3f& );


std::ostream& operator<<
(
   std::ostream&   out,
   const Matrix3f& obj
)
{
   const Vector3f col3( obj.getCol3() );
   out << "  " << obj.getRow0() << "  " << col3[0] << "\n";
   out << "  " << obj.getRow1() << "  " << col3[1] << "\n";
   out << "  " << obj.getRow2() << "  " << col3[2] << "\n";

   return out;
}


bool test_Matrix3f
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   //seed
)
{
   bool isOk = true;

   if( pOut ) *pOut << "[ test_Matrix3f ]\n\n";


   // construct
   {
      bool isOk_ = true;

      {
         Matrix3f m;
         isOk_ &= m.getRow0().isZero();
         isOk_ &= m.getRow1().isZero();
         isOk_ &= m.getRow2().isZero();
         isOk_ &= m.getCol3().isZero();
         if( pOut && isVerbose ) *pOut << isOk_ << "\n" << m;
      }
      {
         static const float p[] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f,
            6.0f, 7.0f, 8.0f };
         Matrix3f m( p );
         isOk_ &= m.getRow0() == Vector3f( 0.0f, 1.0f, 2.0f );
         isOk_ &= m.getRow1() == Vector3f( 3.0f, 4.0f, 5.0f );
         isOk_ &= m.getRow2() == Vector3f( 6.0f, 7.0f, 8.0f );
         isOk_ &= m.getCol3() == Vector3f( 0.0f, 0.0f, 0.0f );
         if( pOut && isVerbose ) *pOut << isOk_ << "\n" << m;
      }
      {
         Matrix3f m( 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f,
            6.0f, 7.0f, 8.0f );
         isOk_ &= m.getRow0() == Vector3f( 0.0f, 1.0f, 2.0f );
         isOk_ &= m.getRow1() == Vector3f( 3.0f, 4.0f, 5.0f );
         isOk_ &= m.getRow2() == Vector3f( 6.0f, 7.0f, 8.0f );
         isOk_ &= m.getCol3() == Vector3f( 0.0f, 0.0f, 0.0f );
         if( pOut && isVerbose ) *pOut << isOk_ << "\n" << m;
      }
      {
         Matrix3f m( 0.0f, 1.0f, 2.0f, 9.0f, 3.0f, 4.0f, 5.0f, 10.0f,
            6.0f, 7.0f, 8.0f, 11.0f );
         isOk_ &= m.getRow0() == Vector3f( 0.0f, 1.0f, 2.0f );
         isOk_ &= m.getRow1() == Vector3f( 3.0f, 4.0f, 5.0f );
         isOk_ &= m.getRow2() == Vector3f( 6.0f, 7.0f, 8.0f );
         isOk_ &= m.getCol3() == Vector3f( 9.0f, 10.0f, 11.0f );
         if( pOut && isVerbose ) *pOut << isOk_ << "\n" << m;
      }
      {
         Matrix3f m(
            Vector3f( 0.0f, 1.0f, 2.0f ),
            Vector3f( 3.0f, 4.0f, 5.0f ),
            Vector3f( 6.0f, 7.0f, 8.0f ),
            Vector3f( 9.0f, 10.0f, 11.0f ), false );
         isOk_ &= m.getRow0() == Vector3f( 0.0f, 3.0f, 6.0f );
         isOk_ &= m.getRow1() == Vector3f( 1.0f, 4.0f, 7.0f );
         isOk_ &= m.getRow2() == Vector3f( 2.0f, 5.0f, 8.0f );
         isOk_ &= m.getCol3() == Vector3f( 9.0f, 10.0f, 11.0f );
         if( pOut && isVerbose ) *pOut << isOk_ << "\n" << m;
      }

      if( pOut ) *pOut << "construct : " <<
         (isOk_ ? "--- succeeded" : "*** failed") << "\n\n";

      isOk &= isOk_;
   }

   // set
   {
      bool isOk_ = true;

      {
         static const float p[] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f,
            6.0f, 7.0f, 8.0f };
         Matrix3f m;
         m.setElements( p );
         isOk_ &= m.getRow0() == Vector3f( 0.0f, 1.0f, 2.0f );
         isOk_ &= m.getRow1() == Vector3f( 3.0f, 4.0f, 5.0f );
         isOk_ &= m.getRow2() == Vector3f( 6.0f, 7.0f, 8.0f );
         isOk_ &= m.getCol3() == Vector3f( 0.0f, 0.0f, 0.0f );
         if( pOut && isVerbose ) *pOut << isOk_ << "\n" << m;
      }
      {
         Matrix3f m;
         m.setElements( 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f,
            6.0f, 7.0f, 8.0f );
         isOk_ &= m.getRow0() == Vector3f( 0.0f, 1.0f, 2.0f );
         isOk_ &= m.getRow1() == Vector3f( 3.0f, 4.0f, 5.0f );
         isOk_ &= m.getRow2() == Vector3f( 6.0f, 7.0f, 8.0f );
         isOk_ &= m.getCol3() == Vector3f( 0.0f, 0.0f, 0.0f );
         if( pOut && isVerbose ) *pOut << isOk_ << "\n" << m;
      }
      {
         Matrix3f m;
         m.setElements( 0.0f, 1.0f, 2.0f, 9.0f, 3.0f, 4.0f, 5.0f, 10.0f,
            6.0f, 7.0f, 8.0f, 11.0f );
         isOk_ &= m.getRow0() == Vector3f( 0.0f, 1.0f, 2.0f );
         isOk_ &= m.getRow1() == Vector3f( 3.0f, 4.0f, 5.0f );
         isOk_ &= m.getRow2() == Vector3f( 6.0f, 7.0f, 8.0f );
         isOk_ &= m.getCol3() == Vector3f( 9.0f, 10.0f, 11.0f );
         if( pOut && isVerbose ) *pOut << isOk_ << "\n" << m;
      }
      {
         Matrix3f m;
         m.setColumns(
            Vector3f( 0.0f, 1.0f, 2.0f ),
            Vector3f( 3.0f, 4.0f, 5.0f ),
            Vector3f( 6.0f, 7.0f, 8.0f ),
            Vector3f( 9.0f, 10.0f, 11.0f ) );
         isOk_ &= m.getRow0() == Vector3f( 0.0f, 3.0f, 6.0f );
         isOk_ &= m.getRow1() == Vector3f( 1.0f, 4.0f, 7.0f );
         isOk_ &= m.getRow2() == Vector3f( 2.0f, 5.0f, 8.0f );
         isOk_ &= m.getCol3() == Vector3f( 9.0f, 10.0f, 11.0f );
         if( pOut && isVerbose ) *pOut << isOk_ << "\n" << m;
      }

      if( pOut ) *pOut << "set : " <<
         (isOk_ ? "--- succeeded" : "*** failed") << "\n\n";

      isOk &= isOk_;
   }

   // translate, scale, rotate
   {
   }

   // operators
   {
      bool isOk_ = true;

      // matrix multiply
      const Matrix3f m0( 0.5141f, 0.3239f, 0.1604f,
                         0.2651f, 0.6702f, 0.0641f,
                         0.0241f, 0.1228f, 0.8444f );
      const Matrix3f m1(  0.3897f,  0.6890f, -0.0787f,
                         -0.2298f,  1.1834f,  0.0464f,
                          0.0000f,  0.0000f,  1.0000f );
      const Matrix3f m2( 0.3811f, 0.5783f, 0.0402f,
                         0.1967f, 0.7244f, 0.0782f,
                         0.0241f, 0.1228f, 0.8444f );
      const Matrix3f m( m1 * m0 );
      isOk_ &= isAlmostEqual( m2, m );
      if( pOut && isVerbose ) *pOut << isOk_ << "\n" << m;

      // matrix vector multiply
      const Vector3f v0( 1.0f, 2.0f, 3.0f );
      const Vector3f a1( m1 * (m0 * v0) );
      const Vector3f a2( m2 * v0 );
      isOk_ &= (a1 - a2).abs().largest() < 1e-3;
      if( pOut && isVerbose ) *pOut << isOk_ << "  " << a1 << "  " << a2 << "\n";

      // -
      // * float

      if( pOut ) *pOut << "operators : " <<
         (isOk_ ? "--- succeeded" : "*** failed") << "\n\n";

      isOk &= isOk_;
   }

   // transpose, invert
   {
      bool isOk_ = true;

      // transpose
      {
         const Matrix3f mt(
            0.0f, 3.0f, 6.0f,
            1.0f, 4.0f, 7.0f,
            2.0f, 5.0f, 8.0f );
         Matrix3f m(
            0.0f, 1.0f, 2.0f,
            3.0f, 4.0f, 5.0f,
            6.0f, 7.0f, 8.0f );
         isOk_ &= mt == m.transpose();
         if( pOut && isVerbose ) *pOut << isOk_ << "\n" << m;
      }

      // invert
      {
         const Matrix3f mi(  4.4679f, -3.5873f,  0.1193f,
                            -1.2186f,  2.3809f, -0.1624f,
                             0.0497f, -0.2439f,  1.2045f );
         Matrix3f m( 0.3811f, 0.5783f, 0.0402f,
                     0.1967f, 0.7244f, 0.0782f,
                     0.0241f, 0.1228f, 0.8444f );
         isOk_ &= isAlmostEqual( Matrix3f::IDENTITY(), (mi * m) );
         if( pOut && isVerbose ) *pOut << isOk_ << "\n" << (mi * m);

         isOk_ &= m.invert();
         isOk_ &= isAlmostEqual( m, mi );
         if( pOut && isVerbose ) *pOut << isOk_ << "\n" << m;
      }

      if( pOut ) *pOut << "transpose invert : " <<
         (isOk_ ? "--- succeeded" : "*** failed") << "\n\n";

      isOk &= isOk_;
   }


   if( pOut ) *pOut << (isOk ? "--- successfully" : "*** failurefully") <<
      " completed " << "\n\n\n";

   if( pOut ) pOut->flush();


   return isOk;
}


}//namespace


#endif//TESTING
