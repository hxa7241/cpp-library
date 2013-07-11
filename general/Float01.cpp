/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include "Float01.hpp"


using namespace hxa7241_general;




/// half open interval /////////////////////////////////////////////////////////

/// queries --------------------------------------------------------------------
float Float01o::clamp
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








/// closed interval ////////////////////////////////////////////////////////////

/// queries --------------------------------------------------------------------
float Float01c::clamp
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
