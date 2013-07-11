/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef Float01_h
#define Float01_h




#include "hxa7241_general.hpp"
namespace hxa7241_general
{


/// half open interval /////////////////////////////////////////////////////////

/**
 * A float value between 0 and almost 1, ie: [0,1).<br/><br/>
 *
 * 'almost one' equals 1.0f - FLT_EPSILON.<br/><br/>
 *
 * Provides implicit conversion to and from float.<br/><br/>
 *
 * Constant.
 */
class Float01o
{
/// standard object services ---------------------------------------------------
public:
            Float01o();
private:
            Float01o( bool,
                      float );
public:
           ~Float01o();
            Float01o( const Float01o& );
   Float01o& operator=( const Float01o& );

/// implicit conversions
            Float01o( float );
            operator float() const;


/// queries --------------------------------------------------------------------
   static  float clamp( float );


/// constants ------------------------------------------------------------------
   static  Float01o MIN();
   static  Float01o MAX();


/// fields ---------------------------------------------------------------------
private:
   float f01_m;
};








/// standard object services ---------------------------------------------------
inline
Float01o::Float01o()
 : f01_m( 0.0f )
{
}


inline
Float01o::Float01o
(
   bool, //dummy
   const float f
)
 : f01_m( f )
{
}


inline
Float01o::~Float01o()
{
}


inline
Float01o::Float01o
(
   const Float01o& other
)
 : f01_m( other.f01_m )
{
}


inline
Float01o& Float01o::operator=
(
   const Float01o& other
)
{
   f01_m = other.f01_m;

   return *this;
}


inline
Float01o::Float01o
(
   const float f
)
 : f01_m( Float01o::clamp( f ) )
{
}


inline
Float01o::operator float() const
{
   return f01_m;
}




/// constants ------------------------------------------------------------------
inline
Float01o Float01o::MIN()
{
   return Float01o(false, 0.0f);
}


inline
Float01o Float01o::MAX()
{
   return Float01o(false, FLOAT_ALMOST_ONE);
}








/// closed interval ////////////////////////////////////////////////////////////

/**
 * A float value between 0 and 1, ie: [0,1].<br/><br/>
 *
 * Provides implicit conversion to and from float.<br/><br/>
 *
 * Constant.
 */
class Float01c
{
/// standard object services ---------------------------------------------------
public:
            Float01c();
private:
            Float01c( bool,
                      float );
public:
           ~Float01c();
            Float01c( const Float01c& );
   Float01c& operator=( const Float01c& );

/// implicit conversions
            Float01c( float );
            operator float() const;


/// queries --------------------------------------------------------------------
   static  float clamp( float );


/// constants ------------------------------------------------------------------
   static  Float01c MIN();
   static  Float01c MAX();


/// fields ---------------------------------------------------------------------
private:
   float f01_m;
};








/// standard object services ---------------------------------------------------
inline
Float01c::Float01c()
 : f01_m( 0.0f )
{
}


inline
Float01c::Float01c
(
   bool, //dummy
   const float f
)
 : f01_m( f )
{
}


inline
Float01c::~Float01c()
{
}


inline
Float01c::Float01c
(
   const Float01c& other
)
 : f01_m( other.f01_m )
{
}


inline
Float01c& Float01c::operator=
(
   const Float01c& other
)
{
   f01_m = other.f01_m;

   return *this;
}


inline
Float01c::Float01c
(
   const float f
)
 : f01_m( Float01c::clamp( f ) )
{
}


inline
Float01c::operator float() const
{
   return f01_m;
}




/// constants ------------------------------------------------------------------
inline
Float01c Float01c::MIN()
{
   return Float01c(false, 0.0f);
}


inline
Float01c Float01c::MAX()
{
   return Float01c(false, 1.0f);
}


}//namespace




#endif//Float01_h
