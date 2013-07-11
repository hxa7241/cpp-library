/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include <math.h>

#include "XyzE.hpp"


using namespace hxa7241_graphics;




/// core ----------------------------------------------------------------------
dword hxa7241_graphics::xyzeFromTriple
(
   float x,
   float y,
   float z
)
{
   // clamp to min of zero
   x = x > 0.0f ? x : 0.0f;
   y = y > 0.0f ? y : 0.0f;
   z = z > 0.0f ? z : 0.0f;

   const float largestOfxyz = (x >= y) ? (x >= z ? x : z) : (y >= z ? y : z);

   dword xyze = 0;

   if( largestOfxyz >= 1e-9f )
   {
      dword exponentOfLargest = 0;
      float mantissaOfLargest = ::frexpf( largestOfxyz, &exponentOfLargest );

      // shouldnt be needed, but... (it has happened in the past)
      if( mantissaOfLargest == 1.0f )
      {
         mantissaOfLargest = 0.5f;
         exponentOfLargest++;
      }

      const float amount = mantissaOfLargest * 256.0f / largestOfxyz;

      xyze = (static_cast<dword>(::floorf( x * amount )) << 24) |
             (static_cast<dword>(::floorf( y * amount )) << 16) |
             (static_cast<dword>(::floorf( z * amount )) <<  8) |
             (static_cast<dword>( exponentOfLargest + 128 ));
   }

   return xyze;
}


void hxa7241_graphics::xyzeToTriple
(
   const dword xyze,
   float&      x,
   float&      y,
   float&      z
)
{
   const dword exponentOfLargest = xyze & 0x000000FF;

   if( 0 == exponentOfLargest )
   {
      x = 0.0f;
      y = 0.0f;
      z = 0.0f;
   }
   else
   {
      const float amount = ::ldexpf( 1.0f / 256.0f, exponentOfLargest - 128 );

      x = (static_cast<float>((xyze >> 24) & 0xFF) + 0.5f) * amount;
      y = (static_cast<float>((xyze >> 16) & 0xFF) + 0.5f) * amount;
      z = (static_cast<float>((xyze >>  8) & 0xFF) + 0.5f) * amount;
   }
}




dword hxa7241_graphics::xyzeFromTriple
(
   const float xyz[3]
)
{
   return xyzeFromTriple( xyz[0], xyz[1], xyz[2] );
}


float* hxa7241_graphics::xyzeToTriple
(
   const dword xyze,
   float       xyz[3]
)
{
   xyzeToTriple( xyze, xyz[0], xyz[1], xyz[2] );
   return xyz;
}
