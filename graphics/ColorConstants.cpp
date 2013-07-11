/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include <math.h>

#include "ColorConstants.hpp"


using namespace hxa7241_graphics;




namespace
{
   // ITU-R BT.709 / sRGB standard color space
   const float SRGB_COLORSPACE[] =
   {
      // x      y

      // chromaticities
      0.64f, 0.33f,      // r
      0.30f, 0.60f,      // g
      0.15f, 0.06f,      // b

      // whitepoint (D65)
      0.3127f, 0.3290f   // w
   };

   const float* SRGB_CHROMATICITIES = SRGB_COLORSPACE;
   const float* SRGB_WHITEPOINT     = SRGB_COLORSPACE + 6;
}




namespace hxa7241_graphics
{
namespace color
{


/// sRGB and ITU-R BT.709 ------------------------------------------------------

const float* getSrgbChromaticities()
{
   // ITU-R BT.709 reference primaries (sRGB)
   return SRGB_CHROMATICITIES;
}


const float* getSrgbWhitePoint()
{
   // CIE standard illuminant D65 (ITU-R BT.709) (sRGB)
   return SRGB_WHITEPOINT;
}


const float* getSrgbColorSpace()
{
   return SRGB_COLORSPACE;
}


float getSrgbGamma()
{
   // sRGB standard gamma
   static const float SRGB_GAMMA = 1.0f / 2.2f;

   return SRGB_GAMMA;
}


float get709Gamma()
{
   // ITU-R BT.709 standard gamma
   static const float _709_GAMMA = 0.45f;

   return _709_GAMMA;
}


float gammaEncodeSrgb
(
   const float fp01
)
{
   float fp01prime = 0.0f;

   // sRGB standard transfer function
   if( fp01 <= 0.00304f )
   {
      fp01prime = 12.92f * fp01;
   }
   else
   {
      fp01prime = 1.055f * ::powf( fp01, 1.0f / 2.4f ) - 0.055f;
   }

   return fp01prime;
}


float gammaEncode709
(
   const float fp01
)
{
   float fp01prime = 0.0f;

   // ITU-R BT.709 standard transfer function
   if( fp01 <= 0.018f )
   {
      fp01prime = 4.5f * fp01;
   }
   else
   {
      fp01prime = 1.099f * ::powf( fp01, 0.45f ) - 0.099f;
   }

   return fp01prime;
}


float gammaDecodeSrgb
(
   const float fp01prime
)
{
   float fp01 = 0.0f;

   // sRGB standard inverse transfer function
   if( fp01prime <= 0.03928f )
   {
      fp01 = fp01prime * (1.0f / 12.92f);
   }
   else
   {
      fp01 = ::powf( (fp01prime + 0.055f) * (1.0f / 1.055f), 2.4f );
   }

   return fp01;
}


float gammaDecode709
(
   const float fp01prime
)
{
   float fp01 = 0.0f;

   // ITU-R BT.709 standard inverse transfer function
   if( fp01prime <= 0.081f )
   {
      fp01 = fp01prime * (1.0f / 4.5f);
   }
   else
   {
      fp01 = ::powf( (fp01prime + 0.099f) * (1.0f / 1.099f), (1.0f / 0.45f) );
   }

   return fp01;
}




/// monitor --------------------------------------------------------------------

const float* getCrtLuminanceRange()
{
   // approximate CRT luminance limits
   static const float CRT_BLACK =   2.0f;
   static const float CRT_WHITE = 100.0f;

   static const float CRT_RANGE[2] = { CRT_BLACK, CRT_WHITE };

   return CRT_RANGE;
}


const float* getTftLuminanceRange()
{
   // approximate TFT luminance limits
   static const float TFT_BLACK =   2.0f;
   static const float TFT_WHITE = 150.0f;

   static const float TFT_RANGE[2] = { TFT_BLACK, TFT_WHITE };

   return TFT_RANGE;
}


const float* getCrtBestLuminanceRange()
{
   // approximate CRT luminance limits
   static const float CRT_BLACK =   0.01f;
   static const float CRT_WHITE = 175.0f;

   static const float CRT_RANGE[2] = { CRT_BLACK, CRT_WHITE };

   return CRT_RANGE;
}


const float* getTftBestLuminanceRange()
{
   // approximate TFT luminance limits
   // (backlight at medium level)
   static const float TFT_BLACK =   0.5f;
   static const float TFT_WHITE = 300.0f;

   static const float TFT_RANGE[2] = { TFT_BLACK, TFT_WHITE };

   return TFT_RANGE;
}




/// perceptual -----------------------------------------------------------------

float getLuminanceMin()
{
   static const float LUMINANCE_MIN = 1e-4f;

   return LUMINANCE_MIN;
}


float getLuminanceMax()
{
   static const float LUMINANCE_MAX = 1e+6f;

   return LUMINANCE_MAX;
}


}//namespace
}//namespace
