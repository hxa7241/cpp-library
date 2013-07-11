/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef ColorSpace_h
#define ColorSpace_h


#include "Vector3f.hpp"
#include "Matrix3f.hpp"




#include "hxa7241_graphics.hpp"
namespace hxa7241_graphics
{


/// core -----------------------------------------------------------------------
namespace color
{

/**
 * Make conversions between CIE XYZ and sRGB.
 */
void makeSrgbConversions
(
   Matrix3f* pXyzToRgb,
   Matrix3f* pRgbToXyz
);


/**
 * Make conversions between CIE XYZ and a color space.<br/><br/>
 *
 * @pColorspace6
 * which way around is the chromaticities array? -- the sRGB ones would be:
 * <pre>{
 *    // x    y
 *    0.64, 0.33,   // r
 *    0.30, 0.60,   // g
 *    0.15, 0.06    // b
 * }</pre><br/><br/>
 * each value is [0,1]
 *
 * @pWhitePoint2 each value is (0,1)
 *
 * @exceptions throws if colorspace or whitepoint invalid
 */
void makeColorSpaceConversions
(
   const float* pColorspace6,
   const float* pWhitePoint2,
   Matrix3f*    pXyzToRgb,
   Matrix3f*    pRgbToXyz
);


/**
 * ITU-R BT.709 standard RGB luminance weighting
 */
float getLuminanceOfSrgb( float r, float g, float b );

/**
 * Get z from chromaticities.
 */
float getZfromXy( float x, float y );

/**
 * Get XYZ from xyY.
 */
void getXyzfromXyy( float x, float y, float Y, float* pXYZ );


/**
 * CIE XYZ to Cone space.<br/><br/>
 *
 * Hunter-Point-Estevez cone responses.
 *
 * @return  pointer to a float[9] of matrix values:
 *    { a, b, c,
 *      d, e, f,
 *      g, h, i }
 */
const float* getXyzToCone();

}




/**
 * A trichromatic color space, with conversions to/from XYZ.
 */
class ColorSpace
{
/// standard object services ---------------------------------------------------
public:
            ColorSpace();
            /**
             * @pChromaticities6
             * which way around is the chromaticities array? -- the sRGB ones
             * would be:<pre>
             * {
             *    // x    y
             *    0.64, 0.33,   // r
             *    0.30, 0.60,   // g
             *    0.15, 0.06    // b
             * }</pre><br/><br/>
             *
             * each value is [0,1]
             *
             * @pWhitePoint2 each value is (0,1)
             *
             * @exceptions throws if chromaticities or whitepoint invalid
             */
            ColorSpace( const float* pChromaticities6,
                        const float* pWhitePoint2 );

           ~ColorSpace();
            ColorSpace( const ColorSpace& );
   ColorSpace& operator=( const ColorSpace& );


/// commands -------------------------------------------------------------------
           void  setSrgb();
           void  setXyz();
           /**
            * @pChromaticities6
            * which way around is the chromaticities array? -- the sRGB ones
            * would be:<pre>
            * {
            *    // x    y
            *    0.64, 0.33,   // r
            *    0.30, 0.60,   // g
            *    0.15, 0.06    // b
            * }</pre><br/><br/>
            *
            * each value is [0,1]
            *
            * @pWhitePoint2 each value is (0,1)
            *
            * @exceptions throws if chromaticities or whitepoint invalid
            */
           void  set( const float* pChromaticities6,
                      const float* pWhitePoint2 );


/// queries --------------------------------------------------------------------
           void  transXyzToRgb( Vector3f& xyz )                           const;
           void  transRgbToXyz( Vector3f& rgb )                           const;

           Vector3f transXyzToRgb_( const Vector3f& xyz )                 const;
           Vector3f transRgbToXyz_( const Vector3f& rgb )                 const;

           float getRgbLuminance( const Vector3f& rgb )                   const;

           const float* getChromaticities()                               const;
           const float* getWhitePoint()                                   const;


/// implementation -------------------------------------------------------------
protected:
           void  setPrimaries( const float* pChromaticities6,
                               const float* pWhitePoint2 );


/// fields ---------------------------------------------------------------------
private:
   float    chromaticities32_m[6];
   float    whitePoint2_m[2];

   Matrix3f XyzToRgb_m;
   Matrix3f RgbToXyz_m;
};


}//namespace




#endif//ColorSpace_h
