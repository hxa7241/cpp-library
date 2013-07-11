/*------------------------------------------------------------------------------

   HXA7241 Image library.
   Copyright (c) 2004-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef ImageQuantizing_h
#define ImageQuantizing_h




#include "hxa7241_image.hpp"
namespace hxa7241_image
{


/**
 * Conversions between integer and float [0,1] triplet images.<br/><br/>
 */
namespace quantizing
{


/**
 * Convert integer image to float [0,1] image (three channels).<br/><br/>
 *
 * @i_deGamma    gamma to decode with, or 0 for default (sRGB 2.2)
 * @i_quantMax   max value of quantization, 1 to 65535
 * @i_pTriplesI  if i_quantMax <= 255 then ubyte* else uword*
 * @o_pDeGamma   gamma that was used to decode with
 * @o_pTriplesF  orphaned storage
 *
 * @exceptions throws storage allocation and char* exceptions
 */
void makeFloatImage
(
   float       i_deGamma,
   dword       i_width,
   dword       i_height,
   dword       i_quantMax,
   const void* i_pTriplesI,
   float*      o_pDeGamma,
   float*&     o_pTriplesF
);


/**
 * Convert float [0,1] image to integer image (three channels).<br/><br/>
 *
 * @i_enGamma    gamma to encode with, or 0 for default (sRGB 1/2.2)
 * @i_quantMax   max value of quantization, 1 to 65535
 * @o_pEnGamma   gamma that was used to encode with
 * @o_pTriplesI  if i_quantMax <= 255 then ubyte* else uword*
 *               orphaned storage
 *
 * @exceptions throws storage allocation and char* exceptions
 */
void makeIntegerImage
(
   float        i_enGamma,
   dword        i_width,
   dword        i_height,
   dword        i_quantMax,
   const float* i_pTriplesF,
   float*       o_pEnGamma,
   void*&       o_pTriplesI
);


}//namespace


}//namespace




#endif//ImageQuantizing_h
