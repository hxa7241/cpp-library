/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef Clamps_h
#define Clamps_h




#include "hxa7241_general.hpp"
namespace hxa7241_general
{


/**
 * Functions to clamp numerics between bounds.
 */


template<class TYPE>
TYPE clampMin( TYPE v, TYPE min );

template<class TYPE>
TYPE clampMax( TYPE v, TYPE max );

template<class TYPE>
TYPE clamp( TYPE v, TYPE min, TYPE max );

float clamp01o( float );

float clamp01c( float );


/// inline versions ///

template<class TYPE>
TYPE clampMin_( TYPE v, TYPE min );

template<class TYPE>
TYPE clampMax_( TYPE v, TYPE max );

template<class TYPE>
TYPE clamp_( TYPE v, TYPE min, TYPE max );

float clamp01o_( float );

float clamp01c_( float );




/// INLINES ///

template<class TYPE>
inline
TYPE clampMin_
(
         TYPE v,
   const TYPE min
)
{
   if( v < min )
   {
      v = min;
   }

   return v;
}


template<class TYPE>
inline
TYPE clampMax_
(
         TYPE v,
   const TYPE max
)
{
   if( v > max )
   {
      v = max;
   }

   return v;
}


template<class TYPE>
inline
TYPE clamp_
(
         TYPE v,
   const TYPE min,
   const TYPE max
)
{
   if( v < min )
   {
      v = min;
   }
   else if( v > max )
   {
      v = max;
   }

   return v;
}


inline
float clamp01o_
(
   float f
)
{
   if( f < 0.0f )
   {
      f = 0.0f;
   }
   else if( f >= 1.0f )
   {
      f = FLOAT_ALMOST_ONE;
   }

   return f;
}


inline
float clamp01c_
(
   float f
)
{
   if( f < 0.0f )
   {
      f = 0.0f;
   }
   else if( f > 1.0f )
   {
      f = 1.0f;
   }

   return f;
}


}//namespace




#endif//Clamps_h
