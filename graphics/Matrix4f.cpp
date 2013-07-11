/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include <math.h>
#include <iostream>
#include <sstream>

#include "Polar.hpp"
#include "Vector3f.hpp"
#include "UnitVector3f.hpp"
#include "Matrix3f.hpp"

#include "Matrix4f.hpp"


using namespace hxa7241_graphics;




/// standard object services ---------------------------------------------------
Matrix4f::Matrix4f()
{
   operator=( IDENTITY() );
}


Matrix4f::Matrix4f
(
   const bool init
)
{
   if( init )
   {
      operator=( IDENTITY() );
   }
}


Matrix4f::Matrix4f
(
   const bool     isColumns,
   const Vector4f parts[4]
)
{
   setParts( isColumns, parts );
}


Matrix4f::Matrix4f
(
   const float m00,  const float m10,  const float m20,  const float m30,
   const float m01,  const float m11,  const float m21,  const float m31,
   const float m02,  const float m12,  const float m22,  const float m32,
   const float m03,  const float m13,  const float m23,  const float m33
)
{
   const Vector4f rows[4] = {
      Vector4f( m00, m10, m20, m30 ),
      Vector4f( m01, m11, m21, m31 ),
      Vector4f( m02, m12, m22, m32 ),
      Vector4f( m03, m13, m23, m33 ) };
   setRows( rows );
}


Matrix4f::Matrix4f
(
   const Matrix3f& m3
)
{
   setMatrix( m3 );
}


Matrix4f::Matrix4f
(
   const ETranslateOrScale translateOrScale,
   const Vector3f&         amount
)
{
   switch( translateOrScale )
   {
   case TRANSLATE :
      {
         setToTranslate( amount );
         break;
      }
   case SCALE :
      {
         setToScale( amount );
         break;
      }
   default :
      operator=( IDENTITY() );
   }
}


Matrix4f::Matrix4f
(
   const ERotationAxis axis,
   const float         degrees
)
{
   switch( axis )
   {
   case ROTATE_X :
      {
         setToRotateX( degrees );
         break;
      }
   case ROTATE_Y :
      {
         setToRotateY( degrees );
         break;
      }
   case ROTATE_Z :
      {
         setToRotateZ( degrees );
         break;
      }
   default :
      operator=( IDENTITY() );
   }
}


Matrix4f::Matrix4f
(
   const float viewAngleDegrees,
   const float zNear,
   const float zFar
)
{
   setToPerspective( viewAngleDegrees, zNear, zFar );
}


Matrix4f::~Matrix4f()
{
}


Matrix4f::Matrix4f
(
   const Matrix4f& other
)
{
   operator=( other );
}


Matrix4f& Matrix4f::operator=
(
   const Matrix4f& other
)
{
   if( &other != this )
   {
      int y = 3;
      do
      {
         int x = 3;
         do
         {
            elements_m[x][y] = other.elements_m[x][y];
         }
         while( x-- > 0 );
      }
      while( y-- > 0 );
   }

   return *this;
}




/// streaming ------------------------------------------------------------------
std::ostream& hxa7241_graphics::operator<<
(
   std::ostream&   out,
   const Matrix4f& obj
)
{
   out << "( ";

   for( int y = 0;  y < 4;  ++y )
   {
      if( y > 0 )
      {
         out << "  ";
      }
      for( int x = 0;  x < 4;  ++x )
      {
         out << obj.elements_m[x][y];
         if( x < 3 )
         {
            out << ", ";
         }
      }
      if( y < 3 )
      {
         out << "\n";
      }
   }

   out << " )";

   return out;
}


std::istream& hxa7241_graphics::operator>>
(
   std::istream& in,
   Matrix4f&     obj
)
{
   char c;

   for( int y = 0;  y < 4;  ++y )
   {
      for( int x = 0;  x < 4;  ++x )
      {
         in >> c >> obj.elements_m[x][y];
      }
   }

   in >> c;

   return in;
}




/// commands -------------------------------------------------------------------
void Matrix4f::setColumns
(
   const Vector4f columns[4]
)
{
   setParts( true, columns );
}


void Matrix4f::setRows
(
   const Vector4f rows[4]
)
{
   setParts( false, rows );
}


void Matrix4f::setParts
(
   const bool     isColumns,
   const Vector4f parts[4]
)
{
   int p = 3;
   do
   {
      setPart( isColumns, p, parts[p] );
   }
   while( p-- > 0 );
}


void Matrix4f::setMatrix
(
   const Matrix3f& m3
)
{
   const Vector4f cols[] = {
      Vector4f( m3.getRow0() ),
      Vector4f( m3.getRow1() ),
      Vector4f( m3.getRow2() ),
      Vector4f() };
   setColumns( cols );
   setRow( 3, Vector4f( m3.getCol3() ) );
}


void Matrix4f::setToTranslate
(
   const Vector3f& xyz
)
{
   operator=( IDENTITY() );

   setRow( 3, Vector4f( xyz ) );
}


void Matrix4f::setToScale
(
   const Vector3f& xyz
)
{
   operator=( IDENTITY() );

   const Vector4f xyz1( xyz );
   int i = 3;
   do
   {
      elements_m[i][i] = xyz1[ i ];
   }
   while( i-- > 0 );
}


void Matrix4f::setToRotateX
(
   const float degrees
)
{
   setToRotate( degrees, 1 );
}


void Matrix4f::setToRotateY
(
   const float degrees
)
{
   setToRotate( degrees, 2 );
}


void Matrix4f::setToRotateZ
(
   const float degrees
)
{
   setToRotate( degrees, 0 );
}


void Matrix4f::setToRotateXYZ
(
   const float degreesX,
   const float degreesY,
   const float degreesZ
)
{
   float sinX;
   float cosX;
   sinAndcos( degreesToRadians(degreesX), sinX, cosX );
   float sinY;
   float cosY;
   sinAndcos( degreesToRadians(degreesY), sinY, cosY );
   float sinZ;
   float cosZ;
   sinAndcos( degreesToRadians(degreesZ), sinZ, cosZ );

   const float cosXsinY = cosX * sinY;
   const float sinXsinY = sinX * sinY;

   elements_m[0][0] =  cosY * cosZ;
   elements_m[1][0] = -cosY * sinZ;
   elements_m[2][0] =  sinY;
   elements_m[0][1] = ( sinXsinY * cosZ) + (cosX * sinZ);
   elements_m[1][1] = (-sinXsinY * sinZ) + (cosX * cosZ);
   elements_m[2][1] = -sinX * cosY;
   elements_m[0][2] = (-cosXsinY * cosZ) + (sinX * sinZ);
   elements_m[1][2] = ( cosXsinY * sinZ) + (sinX * cosZ);
   elements_m[2][2] = cosX * cosY;

   setRow( 3, Vector4f::W() );
   setColumn( 3, Vector4f::W() );
}


void Matrix4f::setToPerspective
(
   const float viewAngleDegrees,
   const float zNear,
   const float zFar
)
{
   // See: ch18, 'Jim Blinns Corner, A Trip Down The Graphics Pipeline'; Blinn;
   // (Morgan Kaufmann 1996)

   operator=( IDENTITY() );

   float sin;
   float cos;
   sinAndcos( degreesToRadians( viewAngleDegrees * 0.5f ), sin, cos );

   const float q   = (zFar < FLOAT_MAX) ? (sin / (1.0f - (zNear / zFar))) : sin;
   const float mqzn = -q * zNear;

   ///  cos   0    0    0
   ///   0   cos   0    0
   ///   0    0    q   sin
   ///   0    0  -qnz   0
   elements_m[0][0] = cos;
   elements_m[1][1] = cos;
   elements_m[2][2] = q;
   elements_m[2][3] = mqzn;
   elements_m[3][2] = sin;
   elements_m[3][3] = 0.0f;
}


void Matrix4f::multiplyEqPre
(
   const Matrix4f& other
)
{
   /// this  =  other * this
   multiplyEq( other, *this );
}


void Matrix4f::multiplyEqPost
(
   const Matrix4f& other
)
{
   /// this  =  this * other
   multiplyEq( *this, other );
}


void Matrix4f::transpose()
{
   int y = 3;
   do
   {
      int x = 3;
      do
      {
         const float temp = elements_m[x][y];
         elements_m[x][y] = elements_m[y][x];
         elements_m[y][x] = temp;
      }
      while( x-- > 0 );
   }
   while( y-- > 0 );
}


bool Matrix4f::invert()
{
   return invert( elements_m );
}




/// queries --------------------------------------------------------------------
void Matrix4f::getColumns
(
   Vector4f columns[4]
) const
{
   getParts( true, columns );
}


void Matrix4f::getRows
(
   Vector4f rows[4]
) const
{
   getParts( false, rows );
}


void Matrix4f::getParts
(
   const bool isColumns,
   Vector4f   parts[4]
) const
{
   int p = 3;
   do
   {
      getPart( isColumns, p, parts[p] );
   }
   while( p-- > 0 );
}


void Matrix4f::multiply
(
   const Vector4f& v,
   Vector4f&       result
) const
{
   if( v.getW() == 1.0f )
   {
      /// accelerate case where Vector is promotion of Vector3
      result.set(
         (v.getX() * elements_m[0][0]) + (v.getY() * elements_m[0][1]) +
            (v.getZ() * elements_m[0][2]) + elements_m[0][3],
         (v.getX() * elements_m[1][0]) + (v.getY() * elements_m[1][1]) +
            (v.getZ() * elements_m[1][2]) + elements_m[1][3],
         (v.getX() * elements_m[2][0]) + (v.getY() * elements_m[2][1]) +
            (v.getZ() * elements_m[2][2]) + elements_m[2][3],
         (v.getX() * elements_m[3][0]) + (v.getY() * elements_m[3][1]) +
            (v.getZ() * elements_m[3][2]) + elements_m[3][3] );
   }
   else
   {
      result.set(
         (v.getX() * elements_m[0][0]) + (v.getY() * elements_m[0][1]) +
            (v.getZ() * elements_m[0][2]) + (v.getW() * elements_m[0][3]),
         (v.getX() * elements_m[1][0]) + (v.getY() * elements_m[1][1]) +
            (v.getZ() * elements_m[1][2]) + (v.getW() * elements_m[1][3]),
         (v.getX() * elements_m[2][0]) + (v.getY() * elements_m[2][1]) +
            (v.getZ() * elements_m[2][2]) + (v.getW() * elements_m[2][3]),
         (v.getX() * elements_m[3][0]) + (v.getY() * elements_m[3][1]) +
            (v.getZ() * elements_m[3][2]) + (v.getW() * elements_m[3][3]) );
   }
}


void Matrix4f::multiply
(
   const Vector3f& v,
   Vector4f&       result
) const
{
   result.set(
      (v.getX() * elements_m[0][0]) + (v.getY() * elements_m[0][1]) +
         (v.getZ() * elements_m[0][2]) + elements_m[0][3],
      (v.getX() * elements_m[1][0]) + (v.getY() * elements_m[1][1]) +
         (v.getZ() * elements_m[1][2]) + elements_m[1][3],
      (v.getX() * elements_m[2][0]) + (v.getY() * elements_m[2][1]) +
         (v.getZ() * elements_m[2][2]) + elements_m[2][3],
      (v.getX() * elements_m[3][0]) + (v.getY() * elements_m[3][1]) +
         (v.getZ() * elements_m[3][2]) + elements_m[3][3] );
}


void Matrix4f::multiply
(
   const Vector3f& v,
   Vector3f&       result
) const
{
   result.set(
      (v.getX() * elements_m[0][0]) + (v.getY() * elements_m[0][1]) +
         (v.getZ() * elements_m[0][2]) + elements_m[0][3],
      (v.getX() * elements_m[1][0]) + (v.getY() * elements_m[1][1]) +
         (v.getZ() * elements_m[1][2]) + elements_m[1][3],
      (v.getX() * elements_m[2][0]) + (v.getY() * elements_m[2][1]) +
         (v.getZ() * elements_m[2][2]) + elements_m[2][3] );
}


void Matrix4f::multiply
(
   const UnitVector3f& u,
   UnitVector3f&       result
) const
{
   result.set( Vector3f(
      (u.get().getX() * elements_m[0][0]) +
         (u.get().getY() * elements_m[0][1]) +
         (u.get().getZ() * elements_m[0][2]),
      (u.get().getX() * elements_m[1][0]) +
         (u.get().getY() * elements_m[1][1]) +
         (u.get().getZ() * elements_m[1][2]),
      (u.get().getX() * elements_m[2][0]) +
         (u.get().getY() * elements_m[2][1]) +
         (u.get().getZ() * elements_m[2][2]) ) );
}


bool Matrix4f::operator==
(
   const Matrix4f& other
) const
{
   bool isEqual;

   int y = 3;
   do
   {
      int x = 3;
      do
      {
         isEqual = (elements_m[x][y] == other.elements_m[x][y]);
      }
      while( (x-- > 0) & isEqual );
   }
   while( (y-- > 0) & isEqual );

   return isEqual;
}


bool Matrix4f::operator!=
(
   const Matrix4f& other
) const
{
   return !operator==( other );
}




/// constants ------------------------------------------------------------------
const Matrix4f& Matrix4f::ZERO()
{
   static const Matrix4f k(
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f );
   return k;
}


const Matrix4f& Matrix4f::IDENTITY()
{
   static const Matrix4f k(
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f );
   return k;
}




/// implementation -------------------------------------------------------------
void Matrix4f::setColumn
(
   const int       index,
   const Vector4f& column
)
{
   setPart( true, index, column );
}


void Matrix4f::setRow
(
   const int       index,
   const Vector4f& row
)
{
   setPart( false, index, row );
}


void Matrix4f::setPart
(
   const bool      isColumn,
   const int       index,
   const Vector4f& part
)
{
   const int p = index & 0x03;

   int i = 3;
   do
   {
      const int a[2] = { i, p };
      const int x = a[ static_cast<int>( isColumn) ];
      const int y = a[ static_cast<int>(!isColumn) ];

      elements_m[x][y] = part[ i ];
   }
   while( i-- > 0 );
}


void Matrix4f::setToRotate
(
   const float degrees,
   const int   axis
)
{
   operator=( IDENTITY() );

   float sin;
   float cos;
   sinAndcos( degreesToRadians( degrees ), sin, cos );

   const int a0 = axis % 3;
   const int a1 = (a0 + 1) % 3;

   elements_m[a0][a0] =  cos;
   elements_m[a1][a0] = -sin;
   elements_m[a0][a1] =  sin;
   elements_m[a1][a1] =  cos;
}


void Matrix4f::multiplyEq
(
   const Matrix4f& other1,
   const Matrix4f& other2
)
{
   /// this  =  other1 * other2

   Matrix4f result( false );

   int i = 3;
   do
   {
      int y = 3;
      do
      {
         float dot = 0.0f;
         int x = 3;
         do
         {
            dot += other1.elements_m[x][i] * other2.elements_m[y][x];
         }
         while( x-- > 0 );
         result.elements_m[y][i] = dot;

         //result.elements_m[y][i] =
         //   (other1.elements_m[0][i] * other2.elements_m[y][0]) +
         //   (other1.elements_m[1][i] * other2.elements_m[y][1]) +
         //   (other1.elements_m[2][i] * other2.elements_m[y][2]) +
         //   (other1.elements_m[3][i] * other2.elements_m[y][3]);
      }
      while( y-- > 0 );
   }
   while( i-- > 0 );

   *this = result;
}


void Matrix4f::getColumn
(
   const int index,
   Vector4f& column
) const
{
   getPart( true, index, column );
}


void Matrix4f::getRow
(
   const int index,
   Vector4f& row
) const
{
   getPart( false, index, row );
}


void Matrix4f::getPart
(
   const bool isColumn,
   const int  index,
   Vector4f&  part
) const
{
   const int p = index & 0x03;

   int i = 3;
   do
   {
      const int a[2] = { i, p };
      const int x = a[ static_cast<int>( isColumn) ];
      const int y = a[ static_cast<int>(!isColumn) ];

      part.set( i, elements_m[x][y] );
   }
   while( i-- > 0 );
}




/* Matrix inversion code adapted from MGF, from Graphics Gems. */

bool Matrix4f::invert
(
   float matrix[4][4]
)
{
   const double determinant = determinant4x4( matrix );

   const bool isInvertable = (determinant >= FLOAT_EPSILON) |
      (-determinant >= FLOAT_EPSILON);
   if( isInvertable )
   {
      /*const double oneOverDeterminant = 1.0 / determinant;

      const double a1 = matrix[0][0],  b1 = matrix[0][1];
      const double c1 = matrix[0][2],  d1 = matrix[0][3];

      const double a2 = matrix[1][0],  b2 = matrix[1][1];
      const double c2 = matrix[1][2],  d2 = matrix[1][3];

      const double a3 = matrix[2][0],  b3 = matrix[2][1];
      const double c3 = matrix[2][2],  d3 = matrix[2][3];

      const double a4 = matrix[3][0],  b4 = matrix[3][1];
      const double c4 = matrix[3][2],  d4 = matrix[3][3];

      matrix[0][0]  =  determinant3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4 ) *
         oneOverDeterminant;
      matrix[1][0]  = -determinant3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4 ) *
         oneOverDeterminant;
      matrix[2][0]  =  determinant3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4 ) *
         oneOverDeterminant;
      matrix[3][0]  = -determinant3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4 ) *
         oneOverDeterminant;

      matrix[0][1]  = -determinant3x3( b1, b3, b4, c1, c3, c4, d1, d3, d4 ) *
         oneOverDeterminant;
      matrix[1][1]  =  determinant3x3( a1, a3, a4, c1, c3, c4, d1, d3, d4 ) *
         oneOverDeterminant;
      matrix[2][1]  = -determinant3x3( a1, a3, a4, b1, b3, b4, d1, d3, d4 ) *
         oneOverDeterminant;
      matrix[3][1]  =  determinant3x3( a1, a3, a4, b1, b3, b4, c1, c3, c4 ) *
         oneOverDeterminant;

      matrix[0][2]  =  determinant3x3( b1, b2, b4, c1, c2, c4, d1, d2, d4 ) *
         oneOverDeterminant;
      matrix[1][2]  = -determinant3x3( a1, a2, a4, c1, c2, c4, d1, d2, d4 ) *
         oneOverDeterminant;
      matrix[2][2]  =  determinant3x3( a1, a2, a4, b1, b2, b4, d1, d2, d4 ) *
         oneOverDeterminant;
      matrix[3][2]  = -determinant3x3( a1, a2, a4, b1, b2, b4, c1, c2, c4 ) *
         oneOverDeterminant;

      matrix[0][3]  = -determinant3x3( b1, b2, b3, c1, c2, c3, d1, d2, d3 ) *
         oneOverDeterminant;
      matrix[1][3]  =  determinant3x3( a1, a2, a3, c1, c2, c3, d1, d2, d3 ) *
         oneOverDeterminant;
      matrix[2][3]  = -determinant3x3( a1, a2, a3, b1, b2, b3, d1, d2, d3 ) *
         oneOverDeterminant;
      matrix[3][3]  =  determinant3x3( a1, a2, a3, b1, b2, b3, c1, c2, c3 ) *
         oneOverDeterminant;*/

      adjoint( matrix );

      const float oneOverDeterminant = static_cast<float>(1.0 / determinant);
      int i = 3;
      do
      {
         int j = 3;
         do
         {
            matrix[i][j] *= oneOverDeterminant;
         }
         while( j-- > 0 );
      }
      while( i-- > 0 );
   }

    return isInvertable;
}


void Matrix4f::adjoint
(
   float mat[4][4]
)
{
    const double a1 = mat[0][0],  b1 = mat[0][1];
    const double c1 = mat[0][2],  d1 = mat[0][3];

    const double a2 = mat[1][0],  b2 = mat[1][1];
    const double c2 = mat[1][2],  d2 = mat[1][3];

    const double a3 = mat[2][0],  b3 = mat[2][1];
    const double c3 = mat[2][2],  d3 = mat[2][3];

    const double a4 = mat[3][0],  b4 = mat[3][1];
    const double c4 = mat[3][2],  d4 = mat[3][3];

    // row column labeling reversed since we transpose rows & columns
    mat[0][0]  = static_cast<float>(
       determinant3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4 ) );
    mat[1][0]  = static_cast<float>(
       -determinant3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4 ) );
    mat[2][0]  = static_cast<float>(
       determinant3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4 ) );
    mat[3][0]  = static_cast<float>(
       -determinant3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4 ) );

    mat[0][1]  = static_cast<float>(
       -determinant3x3( b1, b3, b4, c1, c3, c4, d1, d3, d4 ) );
    mat[1][1]  = static_cast<float>(
       determinant3x3( a1, a3, a4, c1, c3, c4, d1, d3, d4 ) );
    mat[2][1]  = static_cast<float>(
       -determinant3x3( a1, a3, a4, b1, b3, b4, d1, d3, d4 ) );
    mat[3][1]  = static_cast<float>(
       determinant3x3( a1, a3, a4, b1, b3, b4, c1, c3, c4 ) );

    mat[0][2]  = static_cast<float>(
       determinant3x3( b1, b2, b4, c1, c2, c4, d1, d2, d4 ) );
    mat[1][2]  = static_cast<float>(
       -determinant3x3( a1, a2, a4, c1, c2, c4, d1, d2, d4 ) );
    mat[2][2]  = static_cast<float>(
       determinant3x3( a1, a2, a4, b1, b2, b4, d1, d2, d4 ) );
    mat[3][2]  = static_cast<float>(
       -determinant3x3( a1, a2, a4, b1, b2, b4, c1, c2, c4 ) );

    mat[0][3]  = static_cast<float>(
       -determinant3x3( b1, b2, b3, c1, c2, c3, d1, d2, d3 ) );
    mat[1][3]  = static_cast<float>(
       determinant3x3( a1, a2, a3, c1, c2, c3, d1, d2, d3 ) );
    mat[2][3]  = static_cast<float>(
       -determinant3x3( a1, a2, a3, b1, b2, b3, d1, d2, d3 ) );
    mat[3][3]  = static_cast<float>(
       determinant3x3( a1, a2, a3, b1, b2, b3, c1, c2, c3 ) );
}


double Matrix4f::determinant4x4
(
   const float mat[4][4]
)
{
    const double a1 = mat[0][0],  b1 = mat[0][1];
    const double c1 = mat[0][2],  d1 = mat[0][3];

    const double a2 = mat[1][0],  b2 = mat[1][1];
    const double c2 = mat[1][2],  d2 = mat[1][3];

    const double a3 = mat[2][0],  b3 = mat[2][1];
    const double c3 = mat[2][2],  d3 = mat[2][3];

    const double a4 = mat[3][0],  b4 = mat[3][1];
    const double c4 = mat[3][2],  d4 = mat[3][3];

    return
      a1 * determinant3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4 ) -
      b1 * determinant3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4 ) +
      c1 * determinant3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4 ) -
      d1 * determinant3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4 );
}


double Matrix4f::determinant3x3
(
   const double a1, const double a2, const double a3,
   const double b1, const double b2, const double b3,
   const double c1, const double c2, const double c3
)
{
    return
      a1 * determinant2x2( b2, b3, c2, c3 ) -
      b1 * determinant2x2( a2, a3, c2, c3 ) +
      c1 * determinant2x2( a2, a3, b2, b3 );
}


double Matrix4f::determinant2x2
(
   const double a,
   const double b,
   const double c,
   const double d
)
{
    return a * d - b * c;
}
