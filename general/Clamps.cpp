/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include "Clamps.hpp"




namespace hxa7241_general
{


template<class TYPE>
TYPE clampMin
(
         TYPE v,
   const TYPE min
)
{
   return clampMin_( v, min );
}


template<class TYPE>
TYPE clampMax
(
         TYPE v,
   const TYPE max
)
{
   return clampMax_( v, max );
}


template<class TYPE>
TYPE clamp
(
         TYPE v,
   const TYPE min,
   const TYPE max
)
{
   return clamp_( v, min, max );
}


float clamp01o
(
   float f
)
{
   return clamp01o_( f );
}


float clamp01c
(
   float f
)
{
   return clamp01c_( f );
}


}//namespace
