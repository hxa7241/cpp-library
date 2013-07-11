/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef Matrix4f_h
#define Matrix4f_h


#include <iosfwd>

#include "Vector4f.hpp"




#include "hxa7241_graphics.hpp"
namespace hxa7241_graphics
{


/**
 * A 4d matrix of floats.<br/><br/>
 *
 * Usable for general 3D transforms, and perspective projection.<br/><br/>
 *
 * Inversion code adapted from MGF, from Graphics Gems.
 */
class Matrix4f
{
/// standard object services ---------------------------------------------------
public:
            Matrix4f();
protected:
   explicit Matrix4f( bool );
public:
            Matrix4f( bool           isColumns,
                      const Vector4f parts[4] );
            Matrix4f( float, float, float, float,
                      float, float, float, float,
                      float, float, float, float,
                      float, float, float, float );
   explicit Matrix4f( const Matrix3f& );

   enum ETranslateOrScale { TRANSLATE, SCALE };
   enum ERotationAxis     { ROTATE_X, ROTATE_Y, ROTATE_Z };
            Matrix4f( ETranslateOrScale,
                      const Vector3f& );
            Matrix4f( ERotationAxis,
                      float degrees );
            Matrix4f( float viewAngleDegrees,
                      float zNear,
                      float zFar );

           ~Matrix4f();
            Matrix4f( const Matrix4f& );
   Matrix4f& operator=( const Matrix4f& );

   // streaming
   friend  std::ostream& operator<<( std::ostream&, const Matrix4f& );
   friend  std::istream& operator>>( std::istream&,       Matrix4f& );


/// commands -------------------------------------------------------------------
           void   setColumns( const Vector4f columns[4] );
           void   setRows   ( const Vector4f rows[4] );
           void   setParts  ( bool           isColumns,
                              const Vector4f parts[4] );
           void   setMatrix( const Matrix3f& );

           void   setToTranslate( const Vector3f& );
           void   setToScale( const Vector3f& );
           void   setToRotateX( float degrees );
           void   setToRotateY( float degrees );
           void   setToRotateZ( float degrees );
           void   setToRotateXYZ( float degreesX,
                                  float degreesY,
                                  float degreesZ );
           void   setToPerspective( float viewAngleDegrees,
                                    float zNear,
                                    float zFar );

           /**
            * this = param * this.
            */
           void   multiplyEqPre ( const Matrix4f& );
           /**
            * this = this * param.
            */
           void   multiplyEqPost( const Matrix4f& );

           void   transpose();
           bool   invert();


/// queries --------------------------------------------------------------------
           void   getColumns( Vector4f columns[4] )                       const;
           void   getRows   ( Vector4f rows[4] )                          const;
           void   getParts  ( bool     isColumns,
                              Vector4f parts[4] )                         const;

           void   multiply( const Vector4f&,
                            Vector4f& )                                   const;

           void   multiply( const Vector3f&,
                            Vector4f& )                                   const;
           void   multiply( const Vector3f&,
                            Vector3f& )                                   const;
           void   multiply( const UnitVector3f&,
                            UnitVector3f& )                               const;

           bool   operator==( const Matrix4f& )                           const;
           bool   operator!=( const Matrix4f& )                           const;


/// constants ------------------------------------------------------------------
   static const Matrix4f& ZERO();
   static const Matrix4f& IDENTITY();


/// implementation -------------------------------------------------------------
protected:
           void   setColumn( int             index,
                             const Vector4f& column );
           void   setRow   ( int             index,
                             const Vector4f& row );
           void   setPart  ( bool            isColumn,
                             int             index,
                             const Vector4f& part );

           void   setToRotate( float degrees,
                               int   axis );

           /**
            * this = first * second.
            */
           void   multiplyEq( const Matrix4f&,
                              const Matrix4f& );

           void   getColumn( int       index,
                             Vector4f& column )                           const;
           void   getRow   ( int       index,
                             Vector4f& row )                              const;
           void   getPart  ( bool      isColumn,
                             int       index,
                             Vector4f& part )                             const;

   static  bool   invert( float matrix[4][4] );
   static  void   adjoint( float matrix[4][4] );
   static  double determinant4x4( const float matrix[4][4] );
   static  double determinant3x3( double a1, double a2, double a3,
                                  double b1, double b2, double b3,
                                  double c1, double c2, double c3 );
   static  double determinant2x2( double a, double b, double c, double d );


/// fields ---------------------------------------------------------------------
private:
   float elements_m[4][4];
};




/// streaming --------------------------------------------------------------------------------------
std::ostream& operator<<( std::ostream&, const Matrix4f& );
std::istream& operator>>( std::istream&,       Matrix4f& );


}//namespace




#endif//Matrix4f_h
