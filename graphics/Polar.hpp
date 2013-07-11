/*--------------------------------------------------------------------

   Perceptuum3 renderer, version 1.0
   Copyright (c) 2004,  Harrison Ainsworth.  All rights reserved.

   http://www.hxa7241.org/

--------------------------------------------------------------------*/


#ifndef Polar_h
#define Polar_h




#include "hxa7241_graphics.hpp"
namespace hxa7241_graphics
{


/**
 * Disk-square mapping, and coordinate conversion functions.
 */


/**
 * Transforms a point on the [0,1] square, to a point on the unit disk centered
 * at the origin.<br/><br/>
 *
 * Inverse of diskToSquare.<br/><br/>
 *
 * reference:
 * <cite>'A Low Distortion Map Between Disk And Square' - Shirley, Chiu
 * (Journal Of Graphics Tools, v2n3p45 1997)</cite>
 *
 * @see diskToSquare
 */
void squareToDisk
(
   float  squrX01,
   float  squrY01,
   float& Xminus1plus1,
   float& Yminus1plus1
);

/**
 * Transforms a point on the unit disk centered at the origin, to a point on
 * the [0,1] square.<br/><br/>
 *
 * Inverse of squareToDisk.<br/><br/>
 *
 * Reference:
 * <cite>'A Low Distortion Map Between Disk And Square' - Shirley, Chiu
 * (Journal Of Graphics Tools, v2n3p45 1997)</cite>
 *
 * @see squareToDisk
 */
void diskToSquare
(
   const float Xminus1plus1,
   const float Yminus1plus1,
   float&      squrX01,
   float&      squrY01
);




/**
 * Transforms a point on the [0,1] square, to a point on the unit hemisphere
 * surface centered at the origin on the positive z side.
 */
void squareToHemisphere
(
   const float squrX01,
   const float squrY01,
   float&      Xminus1plus1,
   float&      Yminus1plus1,
   float&      Z01
);

/**
 * Transforms a point on the unit hemisphere surface centered at the origin on
 * the positive z side, to a point on the [0,1] square.<br/><br/>
 *
 * Inverse of squareToHemisphere.
 */
void hemisphereToSquare
(
   float  Xminus1plus1,
   float  Yminus1plus1,
   float  Z01,
   float& squrX01,
   float& squrY01
);

/**
 * Transforms a point on the [0,1] square, to a point on the unit sphere surface
 * centered at the origin.
 */
//void squareToSphere
//(
//   float  squrX01,
//   float  squrY01,
//   float& Xminus1plus1,
//   float& Yminus1plus1,
//   float& Zminus1plus1
//);




/**
 * Converts rectangular coords to polar coords.
 */
void rectangularToPolar
(
   const float x,
   const float y,
   float&      angle,
   float&      radius
);

/**
 * Converts polar coords to rectangular coords.
 */
void polarToRectangular
(
   const float angle,
   const float radius,
   float&      x,
   float&      y
);

/**
 * Calculates the sine and cosine of an angle, maybe faster than the std
 * library.
 */
void sinAndcos
(
   const float angle,
   float&      sin,
   float&      cos
);

/**
 * Converts degrees to radians.
 */
inline
float degreesToRadians
(
   const float deg
)
{
   const float PI_OVER_180 = static_cast<float>(3.14159265358979 / 180.0);
   return deg * PI_OVER_180;
}

/**
 * Converts radians to degrees.
 */
inline
float radiansToDegrees
(
   const float rad
)
{
   const float _180_OVER_PI = static_cast<float>(180.0 / 3.14159265358979);
   return rad * _180_OVER_PI;
}


}//namespace




#endif//Polar_h
