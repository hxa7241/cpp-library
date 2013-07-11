/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef Matrix3f_h
#define Matrix3f_h


#include "Vector3f.hpp"




#include "hxa7241_graphics.hpp"
namespace hxa7241_graphics
{


/**
 * A 3D matrix of floats, with extra translation capability.<br/><br/>
 *
 * Appropriate for general 3D transforms, not perspective projection. More
 * efficient than using a 4D matrix.
 */
class Matrix3f
{
public:
   enum ESet    { SCALE, TRANSLATE, ROTATEXYZ };
   enum ERotate { ROTATEX, ROTATEY, ROTATEZ };


/// standard object services ---------------------------------------------------
            Matrix3f();
   explicit Matrix3f( const float* a9 );
            Matrix3f( float, float, float,
                      float, float, float,
                      float, float, float );
            Matrix3f( float, float, float, float,
                      float, float, float, float,
                      float, float, float, float );
            Matrix3f( const Vector3f& row0,
                      const Vector3f& row1,
                      const Vector3f& row2,
                      const Vector3f& col3 );
            Matrix3f( const Vector3f& part0,
                      const Vector3f& part1,
                      const Vector3f& part2,
                      const Vector3f& col3,
                      bool            isRows );
            Matrix3f( ESet,
                      const Vector3f& );
            Matrix3f( ERotate,
                      float radians );
            /**
             * 3x1 * 1x3 makes 3x3.
             */
            Matrix3f( const Vector3f& v31,
                      const Vector3f& m13 );

           ~Matrix3f();
            Matrix3f( const Matrix3f& );
   Matrix3f& operator=( const Matrix3f& );


/// commands -------------------------------------------------------------------
           Matrix3f& setElements( const float* a9 );
           Matrix3f& setElements( float, float, float,
                                  float, float, float,
                                  float, float, float );
           Matrix3f& setElements( float, float, float, float,
                                  float, float, float, float,
                                  float, float, float, float );
           Matrix3f& setRowsCol( const Vector3f& row0,
                                 const Vector3f& row1,
                                 const Vector3f& row2,
                                 const Vector3f& col3 );
           Matrix3f& setColumns( const Vector3f& col0,
                                 const Vector3f& col1,
                                 const Vector3f& col2,
                                 const Vector3f& col3 );

           Matrix3f& setToScale    ( const Vector3f& );
           Matrix3f& setToTranslate( const Vector3f& );
           Matrix3f& setToRotateX  ( float radians );
           Matrix3f& setToRotateY  ( float radians );
           Matrix3f& setToRotateZ  ( float radians );
           Matrix3f& setToRotateXYZ( float radiansX,
                                     float radiansY,
                                     float radiansZ );

           Matrix3f& operator*=( const Matrix3f& );
           Matrix3f& operator*=( const Vector3f& );
           Matrix3f& operator*=( float );

           Matrix3f& operator-=( const Matrix3f& );

           Matrix3f& transpose();
           bool      invert();


/// queries --------------------------------------------------------------------
           void            getElements( float* pFloats9 )                 const;
           const Vector3f& getRow0()                                      const;
           const Vector3f& getRow1()                                      const;
           const Vector3f& getRow2()                                      const;
           const Vector3f& getCol3()                                      const;
           void            getRows( Vector3f& row0,
                                    Vector3f& row1,
                                    Vector3f& row2 )                      const;
           void            getCols( Vector3f& col0,
                                    Vector3f& col1,
                                    Vector3f& col2 )                      const;

           //void            getRotations( Vector3f& )                    const;

           Matrix3f        operator*( const Matrix3f& )                   const;
           Vector3f        operator*( const Vector3f& )                   const;
           Vector3f        operator^( const Vector3f& )                   const;
//         void            multiply( const Vector3f&,
//                                   Vector3f& )                          const;
//         Vector3f&       multiply( Vector3f& )                          const;

           Matrix3f        operator-( const Matrix3f& )                   const;

           Matrix3f        transposed()                                   const;
           Matrix3f        t() const { return transposed(); }
           Matrix3f        inverted( const char* exceptionMessage )       const;

           bool            operator==( const Matrix3f& )                  const;
           bool            operator!=( const Matrix3f& )                  const;

   friend  Matrix3f        operator*( float, const Matrix3f& );


/// constants ------------------------------------------------------------------
   static const Matrix3f& ZERO();
   static const Matrix3f& IDENTITY();


/// fields ---------------------------------------------------------------------
private:
   Vector3f row0_m;
   Vector3f row1_m;
   Vector3f row2_m;
   Vector3f col3_m;
};




/// friends
Matrix3f operator*( float, const Matrix3f& );




/// INLINES ///

inline
const Vector3f& Matrix3f::getRow0() const
{
   return row0_m;
}


inline
const Vector3f& Matrix3f::getRow1() const
{
   return row1_m;
}


inline
const Vector3f& Matrix3f::getRow2() const
{
   return row2_m;
}


inline
const Vector3f& Matrix3f::getCol3() const
{
   return col3_m;
}


inline
Vector3f Matrix3f::operator*
(
   const Vector3f& v
) const
{
   return Vector3f( v.dot(row0_m), v.dot(row1_m), v.dot(row2_m) ) +=
      col3_m;
}


inline
Vector3f Matrix3f::operator^
(
   const Vector3f& v
) const
{
   return Vector3f( v.dot(row0_m), v.dot(row1_m), v.dot(row2_m) );
}


}//namespace




#endif//Matrix3f_h
