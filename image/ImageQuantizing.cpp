/*------------------------------------------------------------------------------

   HXA7241 Image library.
   Copyright (c) 2004-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include <math.h>

#include "ImageQuantizing.hpp"


using namespace hxa7241_image;




namespace
{

/// constants ------------------------------------------------------------------

const float SRGB_GAMMA = 1.0f / 2.2f;

const float GAMMA_MIN  = 1.0f / 8.0f;
const float GAMMA_MAX  = 8.0f;

const char DIMENSIONS_EXCEPTION_MESSAGE[] =
   "image dimensions invalid, in ImageQuantizing";
const char NULL_POINTER_EXCEPTION_MESSAGE[] =
   "pixel pointer null, in ImageQuantizing";
const char GAMMA_EXCEPTION_MESSAGE[] =
   "gamma value invalid, in ImageQuantizing";

void checkDimensionsAndPointer
(
   dword       width,
   dword       height,
   dword       quantMax,
   const void* pTriples
);

void checkGamma
(
   float gamma
);


inline
float clamp01
(
   const float f
)
{
   return (f >= 0.0f) ? (f <= 1.0f ? f : 1.0f) : 0.0f;
}


inline
dword clamp0Max
(
   const dword i,
   const dword max
)
{
   return (i >= 0) ? (i <= max ? i : max) : 0;
}

}




void hxa7241_image::quantizing::makeFloatImage
(
   const float i_deGamma,
   const dword i_width,
   const dword i_height,
   const dword i_quantMax,
   const void* i_pTriplesI,
   float*      o_pDeGamma,
   float*&     o_pTriplesF
)
{
   // check preconditions
   checkDimensionsAndPointer( i_width, i_height, i_quantMax, i_pTriplesI );
   checkGamma( i_deGamma );

   // set gamma decode
   // default for negative or zero
   // reciprocal if less than one
   const float deGamma = (0.0f >= i_deGamma) ? (1.0f / SRGB_GAMMA) :
      ((1.0f > i_deGamma) ? (1.0f / i_deGamma) : i_deGamma);

   // allocate float storage
   const dword length = i_width * i_height * 3;
   float* pTriplesF = new float[ length ];

   // convert pixels
   for( dword t = 0;  t < length;  t += 3 )
   {
      // convert channels
      for( dword c = 0;  c < 3;  ++c )
      {
         // get channel value
         const udword channel = static_cast<udword>( (i_quantMax <= 255) ?
            static_cast<const ubyte*>(i_pTriplesI)[t + c] :
            static_cast<const uword*>(i_pTriplesI)[t + c] );

         // scale to [0,1]
         const float fraction = static_cast<float>(channel) /
            static_cast<float>(i_quantMax);

         // gamma decode
         const float decoded = ::powf( fraction, deGamma );

         // write, clamped to [0,1]
         pTriplesF[t + c] = clamp01( decoded );
      }
   }

   // set outputs
   if( o_pDeGamma )
   {
      *o_pDeGamma = deGamma;
   }
   o_pTriplesF = pTriplesF;
}




void hxa7241_image::quantizing::makeIntegerImage
(
   const float  i_enGamma,
   const dword  i_width,
   const dword  i_height,
   const dword  i_quantMax,
   const float* i_pTriplesF,
   float*       o_pEnGamma,
   void*&       o_pTriplesI
)
{
   // check preconditions
   checkDimensionsAndPointer( i_width, i_height, i_quantMax, i_pTriplesF );
   checkGamma( i_enGamma );

   // set gamma encode
   // default for negative or zero
   // reciprocal if greater than one
   const float enGamma = (0.0f >= i_enGamma) ? SRGB_GAMMA :
      ((1.0f < i_enGamma) ? (1.0f / i_enGamma) : i_enGamma);

   // allocate integer storage
   const dword length = i_width * i_height * 3;
   void* pTriplesI = i_quantMax <= 255 ? static_cast<void*>(new ubyte[length]) :
      static_cast<void*>(new uword[length]);

   // convert pixels
   for( dword t = 0;  t < length;  t += 3 )
   {
      // convert channels
      for( dword c = 0;  c < 3;  ++c )
      {
         // get channel value, clamped to [0,1]
         const float channel = clamp01( i_pTriplesF[t + c] );

         // gamma encode
         const float encoded = ::powf( channel, enGamma );

         // scale, round, and clamp to [0,quantMax]
         dword integer = static_cast<dword>( ::floorf(
            (encoded * static_cast<float>(i_quantMax)) + 0.5f ) );
         integer = clamp0Max( integer, i_quantMax );

         // write
         if( i_quantMax <= 255 )
         {
            static_cast<ubyte*>(pTriplesI)[t + c] = static_cast<ubyte>(integer);
         }
         else
         {
            static_cast<uword*>(pTriplesI)[t + c] = static_cast<uword>(integer);
         }
      }
   }

   // set outputs
   if( o_pEnGamma )
   {
      *o_pEnGamma = enGamma;
   }
   o_pTriplesI = pTriplesI;
}




/// implementation -------------------------------------------------------------
namespace
{

void checkDimensionsAndPointer
(
   const dword width,
   const dword height,
   const dword quantMax,
   const void* pTriples
)
{
   // check size
   if( (quantMax < 1) || (quantMax > 65535) ||
      (width <= 0) || (height <= 0) || (height > (DWORD_MAX / 3)) ||
      (width > (DWORD_MAX / (height * 3))) )
   {
      throw DIMENSIONS_EXCEPTION_MESSAGE;
   }

   // check pointer
   if( !pTriples )
   {
      throw NULL_POINTER_EXCEPTION_MESSAGE;
   }
}


void checkGamma
(
   const float gamma
)
{
   if( (gamma < 0.0f) ||
      ((gamma > 0.0f) & (gamma < GAMMA_MIN)) || (gamma > GAMMA_MAX) )
   {
      throw GAMMA_EXCEPTION_MESSAGE;
   }
}

}








/// test -----------------------------------------------------------------------
#ifdef TESTING


#include <fstream>
#include <vector>


namespace hxa7241_image
{
namespace quantizing
{
   using namespace hxa7241;


bool test_quantizing
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   //seed
)
{
   bool isOk = true;

   if( pOut ) *pOut << "[ test_quantizing ]\n\n";


   {
      static const dword SIZES[]  = { 4, 8, 6 };

      // for 8, 16 and 12 bit pixels
      for( dword m = 0;  m < dword(sizeof(SIZES)/sizeof(SIZES[0]));  ++m )
      {
         const dword width       = 1 << SIZES[m];
         const dword imageLength = width * width;
         const dword maxVal      = imageLength - 1;
         const dword pixelSize   = (maxVal <= 255) ? 1 : 2;

         // make image
         // including all possible values
         std::vector<ubyte> image( imageLength * pixelSize * 3 );
         for( dword i = 0;  i < imageLength;  ++i )
         {
            for( dword c = 0;  c < 3;  ++c )
            {
               if( 2 == pixelSize )
               {
                  reinterpret_cast<uword*>(&(image[0]))[(i * 3) + c] =
                     static_cast<uword>( i );
               }
               else
               {
                  image[(i * 3) + c] = static_cast<ubyte>( i );
               }
            }
         }

         // make float image
         float* pTriplesF = 0;
         makeFloatImage( 0.0f, width, width, maxVal, &(image[0]),
            0, pTriplesF );
         {
            // check ends are 0 and 1
            bool isOk1 = true;
            for( dword i = 0;  i < 3;  ++i )
            {
               isOk1 &= (0.0f == pTriplesF[i]);
               isOk1 &= (1.0f == pTriplesF[(imageLength * 3) - 3 + i]);

               //if( pOut && isVerbose ) *pOut << "  " << pTriplesF[i] << "  " <<
               //   pTriplesF[(imageLength * 3) - 1 - i] << "    " << isOk1 << "\n";
            }
            if( pOut && isVerbose ) *pOut << "ends: " << isOk1 << "\n";
            isOk &= isOk1;

            // check monotonicity
            bool isOk2 = true;
            float previous[] = { -FLOAT_EPSILON, -FLOAT_EPSILON, -FLOAT_EPSILON };
            for( dword i = 0;  i < imageLength;  ++i )
            {
               for( dword c = 0;  c < 3;  ++c )
               {
                  const float current = pTriplesF[(i * 3) + c];
                  const bool  isMore  = (current > previous[c]);

                  if( pOut && isVerbose && !isMore ) *pOut << "  " << previous[c] << "  " <<
                     current << "    " << isMore << "\n";

                  previous[c] = current;
                  isOk2 &= isMore;
               }
            }
            if( pOut && isVerbose ) *pOut << "monotonicity: " << isOk2 << "\n";
            isOk &= isOk2;
         }

         // make integer image
         void* pTriplesI = 0;
         makeIntegerImage( 0.0f, width, width, maxVal, pTriplesF,
            0, pTriplesI );

         // check is same as original
         bool isOk3 = true;
         for( dword i = 0;  i < imageLength;  ++i )
         {
            for( dword c = 0;  c < 3;  ++c )
            {
               if( 2 == pixelSize )
               {
                  const udword a = reinterpret_cast<uword*>(&(image[0]))[(i * 3) + c];
                  const udword b = static_cast<uword*>(pTriplesI)[(i * 3) + c];
                  const bool   t = (a == b);

                  if( pOut && isVerbose && !t ) *pOut << "  " << a << "  " << b << "    " << t << "\n";

                  isOk3 &= t;
               }
               else
               {
                  const udword a = image[(i * 3) + c];
                  const udword b = static_cast<ubyte*>(pTriplesI)[(i * 3) + c];
                  const bool   t = (a == b);

                  if( pOut && isVerbose && !t ) *pOut << "  " << a << "  " << b << "    " << t << "\n";

                  isOk3 &= t;
               }
            }
         }
         if( pOut && isVerbose ) *pOut << "identity: " << isOk3 << "\n\n";
         isOk &= isOk3;

         delete[] pTriplesF;
         if( 2 == pixelSize )
         {
            delete[] reinterpret_cast<uword*>(pTriplesI);
         }
         else
         {
            delete[] reinterpret_cast<ubyte*>(pTriplesI);
         }
      }
      if( pOut && isVerbose ) *pOut << "\n";
   }


   if( pOut ) *pOut << (isOk ? "--- successfully" : "*** failurefully") <<
      " completed " << "\n\n\n";

   if( pOut ) pOut->flush();


   return isOk;
}


}//namespace
}//namespace


#endif//TESTING
