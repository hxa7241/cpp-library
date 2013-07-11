/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include <math.h>

#include "ColorConstants.hpp"

#include "ColorSpace.hpp"


using namespace hxa7241_graphics;




/// core -----------------------------------------------------------------------
namespace
{

const char INVALID_CHROMATICITIES_MESSAGE[] =
   "invalid chromaticities given to makeColorSpaceConversions";
const char INVALID_WHITEPOINT_MESSAGE[] =
   "invalid whitepoint given to makeColorSpaceConversions";
const char INVALID_COLORSPACE_MESSAGE[] =
   "invalid colorspace given to makeColorSpaceConversions";

}


namespace hxa7241_graphics
{
namespace color
{


void makeSrgbConversions
(
   Matrix3f* pXyzToRgb,
   Matrix3f* pRgbToXyz
)
{
   makeColorSpaceConversions( getSrgbChromaticities(), getSrgbWhitePoint(),
      pXyzToRgb, pRgbToXyz );
}


void makeColorSpaceConversions
(
   const float* pColorspace6,
   const float* pWhitePoint2,
   Matrix3f*    pXyzToRgb,
   Matrix3f*    pRgbToXyz
)
{
   if( !pColorspace6 )
   {
      throw INVALID_CHROMATICITIES_MESSAGE;
   }
   if( !pWhitePoint2 )
   {
      throw INVALID_WHITEPOINT_MESSAGE;
   }

   // make chromaticities matrix
   Matrix3f chrm;
   {
      Vector3f cvs[3];
      for( dword i = 3;  i-- > 0; )
      {
         const float x = pColorspace6[i * 2 + 0];
         const float y = pColorspace6[i * 2 + 1];

         if( (x < 0.0f) | (x > 1.0f) | (y < 0.0f) | (y > 1.0f) )
         {
            throw INVALID_CHROMATICITIES_MESSAGE;
         }

         cvs[i].set( x, y, 1.0f - (x + y) );
      }

      chrm.setColumns( cvs[0], cvs[1], cvs[2], Vector3f::ZERO() );
   }

   // make white color vector
   Vector3f whiteColor;
   {
      const float x = pWhitePoint2[0];
      const float y = pWhitePoint2[1];

      if( (x < FLOAT_EPSILON) | (x >= 1.0f) |
         (y < FLOAT_EPSILON) | (y >= 1.0f) )
      {
         throw INVALID_WHITEPOINT_MESSAGE;
      }

      // check special middle case -- to make identity transform exact
      if( (::fabsf(x - (1.0f / 3.0f)) < 1e-3) &&
         (::fabsf(y - (1.0f / 3.0f)) < 1e-3) )
      {
         whiteColor = Vector3f::ONE();
      }
      else
      {
         whiteColor.set( x, y, 1.0f - (x + y) );

         whiteColor /= whiteColor.getY();
      }
   }

   Matrix3f rgbToXyz( chrm );

   // inverted chromaticities * white color to calculate the unknown
   if( !chrm.invert() )
   {
      throw INVALID_CHROMATICITIES_MESSAGE;
   }
   const Vector3f c( chrm * whiteColor );

   // scaled chrms makes the conversion matrix
   rgbToXyz *= c;

   // set output
   if( pRgbToXyz )
   {
      *pRgbToXyz = rgbToXyz;
   }

   if( pXyzToRgb )
   {
      // inverse conversion is the same, but inverted
      *pXyzToRgb = rgbToXyz;
      if( !pXyzToRgb->invert() )
      {
         throw INVALID_COLORSPACE_MESSAGE;
      }
   }
}


float getLuminanceOfSrgb( float r, float g, float b )
{
   return (r * 0.2126f) + (g * 0.7152f) + (b * 0.0722f);
}


float getZfromXy
(
   const float x,
   const float y
)
{
   return 1.0f - x - y;
}


void getXyzfromXyy
(
   const float x,
   const float y,
   const float Y,
   float*      pXYZ
)
{
   pXYZ[0] = (x / y) * Y;
   pXYZ[1] = Y;
   pXYZ[2] = ((1.0f - x - y) / y) * Y;
}


const float* getXyzToCone()
{
   static const float XYZ_TO_CONE[] =
   {
      // from Wyszecki and Stiles
      // Hunter-Point-Estevez cone responses
       0.38971f, 0.68898f, -0.07868f,
      -0.22981f, 1.18340f,  0.04641f,
       0.00000f, 0.00000f,  1.00000f

      // Wandell ?
//       0.243f,  0.856f, -0.044f,
//      -0.391f,  1.165f,  0.087f,
//       0.010f, -0.008f,  0.563f
   };

   return XYZ_TO_CONE;
}


/*const float* getConeToRuderman()
{
   static const float CONE_TO_RUDERMAN[] =
   {
      // 'Statistics of Cone Responses to Natural Images'
      // Ruderman, Cronin, Chiao;
      // J. Optical Soc. of America, vol. 15, no. 8.
      // 1998
      //0.57735026918963f,  0.57735026918963f,  0.57735026918963f,
      //0.40824829046386f,  0.40824829046386f, -0.81649658092773f,
      //0.70710678118655f, -0.70710678118655f,  0.00000000000000f
      1.0f/::sqrtf(3.0f),  1.0f/::sqrtf(3.0f),  1.0f/::sqrtf(3.0f),
      1.0f/::sqrtf(6.0f),  1.0f/::sqrtf(6.0f), -2.0f/::sqrtf(6.0f),
      1.0f/::sqrtf(2.0f), -1.0f/::sqrtf(2.0f),  0.0f
   };

   return CONE_TO_RUDERMAN;
}*/


/*const float* getConeToOpponent()
{
   static const float CONE_TO_OPPONENT1[] =
   {
      // Wandell ?
       1.00f,  0.00f,  0.00f,
      -0.59f,  0.80f, -0.12f,
      -0.34f, -0.11f,  0.93f
   };
   static const float CONE_TO_OPPONENT2[] =
   {
      // ?
       0.4523f,  0.8724f,  0.1853f,
       0.7976f, -0.5499f, -0.2477f,
      -0.2946f, -0.5132f,  0.8062f
   };

   return CONE_TO_OPPONENT;
}*/


}//namespace
}//namespace








/// standard object services ---------------------------------------------------
ColorSpace::ColorSpace()
 : XyzToRgb_m()
 , RgbToXyz_m()
{
   ColorSpace::setSrgb();
}


ColorSpace::ColorSpace
(
   const float* pChromaticities6,
   const float* pWhitePoint2
)
 : XyzToRgb_m()
 , RgbToXyz_m()
{
   ColorSpace::set( pChromaticities6, pWhitePoint2 );
}


ColorSpace::~ColorSpace()
{
}


ColorSpace::ColorSpace
(
   const ColorSpace& other
)
 : XyzToRgb_m()
 , RgbToXyz_m()
{
   ColorSpace::operator=( other );
}


ColorSpace& ColorSpace::operator=
(
   const ColorSpace& other
)
{
   if( &other != this )
   {
      setPrimaries( other.chromaticities32_m, other.whitePoint2_m );

      XyzToRgb_m = other.XyzToRgb_m;
      RgbToXyz_m = other.RgbToXyz_m;
   }

   return *this;
}




/// commands -------------------------------------------------------------------
void ColorSpace::setSrgb()
{
   set( color::getSrgbChromaticities(), color::getSrgbWhitePoint() );
}


void ColorSpace::setXyz()
{
   for( dword i = 6;  i-- > 0; )
   {
      chromaticities32_m[i] = 0.0f;
   }
   chromaticities32_m[0] = 1.0f;
   chromaticities32_m[3] = 1.0f;
   whitePoint2_m[0] = 0.333333f;
   whitePoint2_m[1] = 0.333333f;

   XyzToRgb_m = Matrix3f::IDENTITY();
   RgbToXyz_m = Matrix3f::IDENTITY();
}


void ColorSpace::set
(
   const float*const pChromaticities6,
   const float*const pWhitePoint2
)
{
   // copy primaries
   setPrimaries( pChromaticities6, pWhitePoint2 );

   color::makeColorSpaceConversions( pChromaticities6, pWhitePoint2,
      &XyzToRgb_m, &RgbToXyz_m );
}




/// queries --------------------------------------------------------------------
void ColorSpace::transXyzToRgb
(
   Vector3f& xyz
) const
{
   xyz = XyzToRgb_m ^ xyz;
}


void ColorSpace::transRgbToXyz
(
   Vector3f& rgb
) const
{
   rgb = RgbToXyz_m ^ rgb;
}


Vector3f ColorSpace::transXyzToRgb_
(
   const Vector3f& xyz
) const
{
   return XyzToRgb_m ^ xyz;
}


Vector3f ColorSpace::transRgbToXyz_
(
   const Vector3f& rgb
) const
{
   return RgbToXyz_m ^ rgb;
}


float ColorSpace::getRgbLuminance
(
   const Vector3f& rgb
) const
{
   return rgb.dot( RgbToXyz_m.getRow1() );
}


const float* ColorSpace::getChromaticities() const
{
   return chromaticities32_m;
}


const float* ColorSpace::getWhitePoint() const
{
   return whitePoint2_m;
}




/// implementation -------------------------------------------------------------
void ColorSpace::setPrimaries
(
   const float*const pChromaticities6,
   const float*const pWhitePoint2
)
{
   for( dword i = 6;  i-- > 0; )
   {
      chromaticities32_m[i] = pChromaticities6[i];
   }

   whitePoint2_m[0] = pWhitePoint2[0];
   whitePoint2_m[1] = pWhitePoint2[1];
}








/// test -----------------------------------------------------------------------
#ifdef TESTING


#include <iostream>


namespace hxa7241_graphics
{


std::ostream& operator<<( std::ostream&, const Vector3f& );
std::istream& operator>>( std::istream&,       Vector3f& );


bool test_ColorSpace
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   //seed
)
{
   bool isOk = true;

   if( pOut ) *pOut << "[ test_ColorSpace ]\n\n";


   static const float TOLERANCE1 = 1e-3f;
   static const float TOLERANCE2 = 1e-6f;


   // constants comparison
   {
      bool isOk_ = true;

      // from http://www.poynton.com/ color faq
      static const Vector3f RGB_OF_XYZ111(  1.204794f,  0.948292f,  0.908916f );
      static const Vector3f RGB_OF_XYZ100(  3.240479f, -0.969256f,  0.055648f );
      static const Vector3f RGB_OF_XYZ010( -1.537150f,  1.875992f, -0.204043f );
      static const Vector3f RGB_OF_XYZ001( -0.498535f,  0.041556f,  1.057331f );

      static const Vector3f XYZS[] = {
         Vector3f::ONE(), Vector3f::X(), Vector3f::Y(), Vector3f::Z() };
      static const Vector3f RGBS[] = {
         RGB_OF_XYZ111, RGB_OF_XYZ100, RGB_OF_XYZ010, RGB_OF_XYZ001 };

      ColorSpace ct1;
      ct1.setSrgb();

      for( udword i = 0;  i < sizeof(XYZS)/sizeof(XYZS[0]);  ++i )
      {
         // forward to RGB
         const Vector3f rgb( ct1.transXyzToRgb_( XYZS[i] ) );
         const Vector3f dif1( (rgb - RGBS[i]).absEq() );
         isOk_ &= (dif1.largest() < TOLERANCE1);

         if( pOut && isVerbose ) *pOut << rgb << "  " << RGBS[i] << "  " <<
            dif1 << "  " << isOk_ << "\n";

         // back to XYZ
         const Vector3f xyz( ct1.transRgbToXyz_( rgb ) );
         const Vector3f dif2( (xyz - XYZS[i]).absEq() );
         isOk_ &= (dif2.largest() < TOLERANCE2);

         if( pOut && isVerbose ) *pOut << xyz << "  " << XYZS[i] << "  " <<
            dif2 << "  " << isOk_ << "\n\n";
      }

      //if( pOut && isVerbose ) *pOut << "\n";

      if( pOut ) *pOut << "constants : " <<
         (isOk_ ? "--- succeeded" : "*** failed") << "\n\n";

      isOk &= isOk_;
   }


   // queries consistency
   {
      bool isOk_ = true;

      ColorSpace ct1;

      const Vector3f rgb1( ct1.transXyzToRgb_( Vector3f::ONE() * 3.25f ) );
      Vector3f rgb2( Vector3f::ONE() * 3.25f );
      ct1.transXyzToRgb( rgb2 );

      isOk_ &= (rgb1 == rgb2);
      if( pOut && isVerbose ) *pOut << rgb1 << "  " << rgb2 << "  " <<
         (rgb1 - rgb2) << "  " << isOk_ << "\n";

      const Vector3f xyz1( ct1.transRgbToXyz_( Vector3f::ONE() * 3.25f ) );
      Vector3f xyz2( Vector3f::ONE() * 3.25f );
      ct1.transRgbToXyz( xyz2 );
      const float y3 = ct1.getRgbLuminance( Vector3f::ONE() * 3.25f );

      isOk_ &= (::fabsf((xyz1 - xyz2).largest()) < TOLERANCE2) &
         (::fabsf(xyz1.getY() - y3) < TOLERANCE2);
      if( pOut && isVerbose ) *pOut << xyz1 << "  " << xyz2 << "  " <<
         (xyz1 - xyz2) << "  " << y3 << "  " << (xyz1.getY() - y3) << "  " <<
         isOk_ << "\n";

      if( pOut && isVerbose ) *pOut << "\n";

      if( pOut ) *pOut << "consistency : " <<
         (isOk_ ? "--- succeeded" : "*** failed") << "\n\n";

      isOk &= isOk_;
   }


   if( pOut ) *pOut << (isOk ? "--- successfully" : "*** failurefully") <<
      " completed " << "\n\n\n";

   if( pOut ) pOut->flush();


   return isOk;
}


}//namespace


#endif//TESTING
