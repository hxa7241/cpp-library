/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef Quaternion_h
#define Quaternion_h


#include "Vector3f.hpp"




#include "hxa7241_graphics.hpp"
namespace hxa7241_graphics
{


/**
 * A unit quaternion.<br/><br/>
 *
 * All angles in radians.<br/><br/>
 *
 * This has a dependency on the matrix class, in that rotations produced by each
 * must match - and the matrix can have either of two handednesss.<br/>
 * For a different matrix, swap-in the code marked 'ALTERNATE FORM' and run the
 * tests.<br/><br/>
 *
 * References:<br/>
 * <cite>'The Matrix and Quaternions FAQ',
 * http://www.j3d.org/matrix_faq/matrfaq_latest.html</cite><br/>
 * <cite>'CS184: Using Quaternions to Represent Rotation',
 * http://www-inst.eecs.berkeley.edu/~cs184/</cite>
 *
 * @invariants
 * * s_m and v_m, considered as a 4 element vector, is always unitized
 */
class Quaternion
{
/// standard object services ---------------------------------------------------
public:
            Quaternion();
            Quaternion( float               angle,
                        const UnitVector3f& axis );
   explicit Quaternion( const Matrix3f& );
            Quaternion( float rotationX,
                        float rotationY,
                        float rotationZ );
protected:
            Quaternion( float           s,
                        const Vector3f& v,
                        bool            dummy );
            Quaternion( float s,
                        float vx,
                        float vy,
                        float vz );
public:
           ~Quaternion();
            Quaternion( const Quaternion& );
   Quaternion& operator=( const Quaternion& );


/// commands -------------------------------------------------------------------
           void  setAngleAndAxis( float               angle,
                                  const UnitVector3f& axis );
           void  setMatrix( const Matrix3f& );
           void  setRotations( float rotationX,
                               float rotationY,
                               float rotationZ );

           Quaternion& invert();
           Quaternion& operator*=( const Quaternion& );
           Quaternion& operator*=( float );


/// queries --------------------------------------------------------------------
           void  getAngleAndAxis( float&        angle,
                                  UnitVector3f& axis )                    const;
           void  getMatrix( Matrix3f& )                                   const;

           Quaternion getInverse()                                        const;
           Quaternion operator*( const Quaternion& )                      const;
           Quaternion operator*( float )                                  const;

           void  rotate( Vector3f& )                                      const;

           bool  operator==( const Quaternion& )                          const;
           bool  operator!=( const Quaternion& )                          const;


/// constants ------------------------------------------------------------------
   static const Quaternion& QUARTER_X();
   static const Quaternion& QUARTER_Y();
   static const Quaternion& QUARTER_Z();


/// implementation -------------------------------------------------------------
protected:
           void  setElements( float           s,
                              const Vector3f& v );
           void  setElements( float s,
                              float vx,
                              float vy,
                              float vz );
           void  unitize();


/// fields ---------------------------------------------------------------------
private:
   float    s_m;
   Vector3f v_m;
};


}//namespace




#endif//Quaternion_h
