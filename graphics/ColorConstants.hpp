/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef ColorConstants_h
#define ColorConstants_h




#include "hxa7241_graphics.hpp"
namespace hxa7241_graphics
{


/**
 * Standard and common color constants, etc.
 */
namespace color
{

/// sRGB and ITU-R BT.709 ------------------------------------------------------

/**
 * ITU-R BT.709 reference primaries (sRGB).
 *
 * @return  pointer to a float[6] of: { rx, ry,  gx, gy,  bx, by }
 */
const float* getSrgbChromaticities();

/**
 * CIE standard illuminant D65 (ITU-R BT.709) (sRGB).
 *
 * @return  pointer to a float[2] of: { wx, wy }
 */
const float* getSrgbWhitePoint();

/**
 * ITU-R BT.709 / sRGB standard color space.
 *
 * @return  pointer to a float[8] of: { rx, ry,  gx, gy,  bx, by,  wx, wy }
 */
const float* getSrgbColorSpace();

/**
 * sRGB standard gamma.
 */
float getSrgbGamma();

/**
 * ITU-R BT.709 standard gamma.
 */
float get709Gamma();

/**
 * sRGB standard gamma transfer function.
 */
float gammaEncodeSrgb( float fp01 );

/**
 * ITU-R BT.709 standard gamma transfer function.
 */
float gammaEncode709( float fp01 );

/**
 * sRGB standard gamma inverse transfer function.
 */
float gammaDecodeSrgb( float fp01prime );

/**
 * ITU-R BT.709 standard gamma inverse transfer function.
 */
float gammaDecode709( float fp01prime );



/// monitor --------------------------------------------------------------------

/**
 * Ordinary CRT range, in ordinary conditions -- overall, not very good.
 *
 * @return  pointer to array of 2 float: min and max
 */
const float* getCrtLuminanceRange();

/**
 * Ordinary TFT range, in ordinary conditions -- overall, not very good.
 *
 * @return  pointer to array of 2 float: min and max
 */
const float* getTftLuminanceRange();

/**
 * Good CRT range, in optimal conditions (2004).
 *
 * @return  pointer to array of 2 float: min and max
 */
const float* getCrtBestLuminanceRange();

/**
 * Good TFT range, in optimal conditions (2004).
 *
 * @return  pointer to array of 2 float: min and max
 */
const float* getTftBestLuminanceRange();



/// perceptual -----------------------------------------------------------------

/**
 * Minimum luminance (human-visible).
 */
float getLuminanceMin();

/**
 * Maximum luminance (human-visible).
 */
float getLuminanceMax();

}//namespace


}//namespace




#endif//ColorConstants_h
