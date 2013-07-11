/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include <math.h>

#include "Filters.hpp"


using namespace hxa7241_graphics;




static const float PI = 3.1415926535897932384626433832795f;


namespace hxa7241_graphics
{


float boxFilter
(
   float kernelRadius,
   float pointDistance
)
{
   if( kernelRadius < 0.0f )
   {
      kernelRadius = -kernelRadius;
   }

   if( pointDistance < 0.0f )
   {
      pointDistance = -pointDistance;
   }


   float result;

   if( pointDistance < kernelRadius )
   {
      result = 1.0f / (PI * kernelRadius * kernelRadius);
   }
   else
   {
      result = 0.0f;
   }

   return result;
}


float coneFilter
(
   float kernelRadius,
   float pointDistance
)
{
   if( kernelRadius < 0.0f )
   {
      kernelRadius = -kernelRadius;
   }

   if( pointDistance < 0.0f )
   {
      pointDistance = -pointDistance;
   }


   float result;

   if( pointDistance < kernelRadius )
   {
      result = (1.0f - (pointDistance / kernelRadius)) /
         ((1.0f / 3.0f) * PI * kernelRadius * kernelRadius);
   }
   else
   {
      result = 0.0f;
   }

   return result;
}


float pavicicFilter
(
   const float kernelRadius,
   const float pointDistance
)
{
   return pavicicFilter2( kernelRadius * kernelRadius,
      pointDistance * pointDistance );
}


float pavicicFilter2
(
   float kernelRadius2,
   float pointDistance2
)
{
   if( kernelRadius2 < 0.0f )
   {
      kernelRadius2 = -kernelRadius2;
   }

   if( pointDistance2 < 0.0f )
   {
      pointDistance2 = -pointDistance2;
   }


   float result;

   if( pointDistance2 < kernelRadius2 )
   {
      // 1.0f - ::expf( -1.953f )
      static const float ONE_MINUS_EXP_NEG_1P953 =
         0.85815211103467726466203689184461f;

      result = 1.883f * (0.918f / (PI * kernelRadius2)) *
         (1.0f - ( (1.0f - ::expf( -1.953f * (pointDistance2 /
         (2.0f * kernelRadius2)) )) / ONE_MINUS_EXP_NEG_1P953 ));
   }
   else
   {
      result = 0.0f;
   }

   return result;
}


float cookFilter
(
   const float kernelRadius,
   const float pointDistance
)
{
   return cookFilter2( kernelRadius * kernelRadius,
      pointDistance * pointDistance );
}


float cookFilter2
(
   float kernelRadius2,
   float pointDistance2
)
{
   if( kernelRadius2 < 0.0f )
   {
      kernelRadius2 = -kernelRadius2;
   }

   if( pointDistance2 < 0.0f )
   {
      pointDistance2 = -pointDistance2;
   }


   float result;

   if( pointDistance2 < kernelRadius2 )
   {
      const float expNegKR = ::expf( -kernelRadius2 );
      result = ( ::expf( -pointDistance2 ) - expNegKR ) /
         ( PI * (1.0f - (expNegKR * (1.0f + kernelRadius2))) );

//    const float expNegKR = ::expf( -kernelRadius2 );
//    result = ( ::expf( -pointDistance2 ) - expNegKR ) /
//       ((1.0f - (expNegKR * (1.0f + kernelRadius2))) / kernelRadius2);
   }
   else
   {
      result = 0.0f;
   }

   return result;
}


float maxFilter
(
   float kernelRadius,
   float pointDistance
)
{
   if( kernelRadius < 0.0f )
   {
      kernelRadius = -kernelRadius;
   }

   if( pointDistance < 0.0f )
   {
      pointDistance = -pointDistance;
   }


   float result;

   if( pointDistance < kernelRadius )
   {
      const float t = kernelRadius;
      const float s = t * 0.350788f;

      if( pointDistance < s )
      {
         result = 1.0f - ((pointDistance * pointDistance) / (s * t));
      }
      else
      {
         const float tMr = t - pointDistance;
         result = (tMr * tMr) / (t * (t - s));
      }

      const float tMs = t - s;
      const float s2  = s * s;
      const float t2  = t * t;
      result /= (2.0f * PI * ( (s2 * 0.5f) -
         ((s2 * s2) / (4.0f * s * t)) +
         ((t2 * t2) / (12.0f * t * tMs)) -
         ((s2 * t2) / (2.0f * t * tMs)) +
         ((2.0f * s2 * s * t) / (3.0f * t * tMs)) -
         ((s2 * s2) / (4.0f * t * tMs)) ) );
   }
   else
   {
      result = 0.0f;
   }

   return result;
}




float mitchellNetravaliFilter
(
   float kernelRadius,
   float pointDistance
)
{
   if( kernelRadius < 0.0f )
   {
      kernelRadius = -kernelRadius;
   }

   if( pointDistance < 0.0f )
   {
      pointDistance = -pointDistance;
   }


   float result;

   if( pointDistance < kernelRadius )
   {
      const float p  = 2.0f * pointDistance / kernelRadius;
      const float p2 = p * p;
      const float p3 = p * p2;

      if( p < 1.0f )
      {
         result = (7.0f * p3) + (-12.0f * p2) + (16.0f / 3.0f);
      }
      else
      {
         result = ((-7.0f / 3.0f) * p3) + (12.0f * p2) + (-20.0f * p) +
            (32.0f / 3.0f);
      }

      result *= 1.357150f / (PI * kernelRadius * kernelRadius *
         (19.0f / 180.0f) * 6.0f);
   }
   else
   {
      result = 0.0f;
   }

   return result;
}


}//namespace








/// test -----------------------------------------------------------------------
#ifdef TESTING


#include <iostream>


namespace hxa7241_graphics
{


bool test_Filters
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   //seed
)
{
   bool isOk = true;

   if( pOut ) *pOut << "[ test_Filters ]\n\n";


   static const char* filterNames[]   = { "boxFilter", "coneFilter",
      "pavicicFilter", "cookFilter", "maxFilter" };
      //, "mitchellNetravaliFilter"
   float (*filters[])( float, float ) = { boxFilter, coneFilter, pavicicFilter,
      cookFilter, maxFilter };//, mitchellNetravaliFilter
   const dword n = sizeof(filters) / sizeof(void*);

   for( dword fi = 0;  fi < n;  ++fi )
   {
      float (*filter)( float, float ) = filters[fi];
      const char* filterName          = filterNames[fi];

      // check for positivity, symmetry, and finite support
      {
         bool isPositive  = true;
         bool isSymmetric = true;
         bool isFinite    = true;

         const float radius = 3.7f;
         const float inc    = radius / 32.0f;
         const float end    = radius * 1.5f;;
         for( float p = 0.0f;  p < end;  p += inc )
         {
            const float fp = filter( radius,  p );
            const float fn = filter( radius, -p );

            isPositive  &= (fp >= 0.0f);
            isSymmetric &= (::fabsf(fp - fn) < 1e-7f);
            isFinite    &= ((p < radius) | (fp == 0.0f));
         }

         const bool isPsf = isPositive & isSymmetric & isFinite;

         if( pOut && isVerbose ) *pOut << "features  " << filterName <<
            "  positive: " << isPositive << "  symmetric: "  << isSymmetric <<
            "  finite: " << isFinite << "  " << isPsf << "\n";

         isOk &= isPsf;
      }

      // check for normalisation
      {
         float sum   = 0.0f;
         //dword count = 0;

         static const dword HALF_WIDTH = 512 >> 1;   // < 2^16
         for( dword y = -HALF_WIDTH;  y <= HALF_WIDTH;  ++y )
         {
            for( dword x = -HALF_WIDTH;  x <= HALF_WIDTH;  ++x )
            {
               const float distance = ::sqrtf( static_cast<float>(
                  (x * x) + (y * y)) );
               if( distance < static_cast<float>(HALF_WIDTH) )
               {
                  sum += filter( static_cast<float>(HALF_WIDTH), distance );
                  //++count;
               }
            }
         }

         const float dif    = sum - 1.0f;
         const float difAbs = dif >= 0.0f ? dif : -dif;

         bool isNormalised = difAbs < 0.005f;

         if( pOut && isVerbose ) *pOut << "normalisation  " << filterName <<
            "  sum: " << sum << "  " << isNormalised << "\n";

         isOk &= isNormalised;
      }
   }

   if( pOut && isVerbose ) *pOut << "\n";


   if( pOut ) *pOut << (isOk ? "--- successfully" : "*** failurefully") <<
      " completed " << "\n\n\n";

   if( pOut ) pOut->flush();


   return isOk;
}


}//namespace


#endif//TESTING
