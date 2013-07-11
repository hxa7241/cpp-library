/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef Filters_h
#define Filters_h




#include "hxa7241_graphics.hpp"
namespace hxa7241_graphics
{

/**
 * Some 2D radial filter functions.
 */


/**
 * A box radial 2D filter kernel function.<br/><br/>
 *
 * Supply the kernel radius, and the query point distance.<br/><br/>
 */
float boxFilter
(
   float kernelRadius,
   float pointDistance
);

/**
 * A cone radial 2D filter kernel function.<br/><br/>
 *
 * Supply the kernel radius, and the query point distance.<br/><br/>
 */
float coneFilter
(
   float kernelRadius,
   float pointDistance
);


/**
 * A gaussian radial 2D filter kernel function.<br/><br/>
 *
 * Supply the kernel radius, and the query point distance.<br/><br/>
 *
 * Reference: <cite>'Convienient Anti Aliasing Filters That Minimize Bumpy
 * Sampling' (p144-p146) - Pavicic (Graphics Gems 1, AP 1990)</cite>
 */
float pavicicFilter
(
   float kernelRadius,
   float pointDistance
);

/**
 * A gaussian radial 2D filter kernel function.<br/><br/>
 *
 * Supply the kernel radius squared, and the query point distance squared.
 * <br/><br/>
 *
 * Reference: <cite>'Convienient Anti Aliasing Filters That Minimize Bumpy
 * Sampling' (p144-p146) - Pavicic (Graphics Gems 1, AP 1990)</cite>
 */
float pavicicFilter2
(
   float kernelRadiusSquared,
   float pointDistanceSquared
);


/**
 * A gaussian radial 2D filter kernel function.<br/><br/>
 *
 * Supply the kernel radius, and the query point distance.<br/><br/>
 *
 * Reference: <cite>'Principles Of Digital Image Synthesis' ch10
 * - Glassner (Morgan Kaufmann 1995)</cite>
 *
 * Reference: <cite>'Stochastic Sampling In Computer Graphics'
 * - Cook [ACM Transactions On Graphics, Jan 1986]</cite>
 */
float cookFilter
(
   float kernelRadius,
   float pointDistance
);


/**
 * A gaussian radial 2D filter kernel function.<br/><br/>
 *
 * Supply the kernel radius squared, and the query point distance squared.
 * <br/><br/>
 *
 * Reference: <cite>'Principles Of Digital Image Synthesis' ch10
 * - Glassner (Morgan Kaufmann 1995)</cite>
 *
 * Reference: <cite>'Stochastic Sampling In Computer Graphics'
 * - Cook [ACM Transactions On Graphics, Jan 1986]</cite>
 */
float cookFilter2
(
   float kernelRadiusSquared,
   float pointDistanceSquared
);


/**
 * A quadratic spline radial 2D filter kernel function.<br/><br/>
 *
 * Supply the kernel radius, and the query point distance.<br/><br/>
 *
 * Reference: <cite>'Principles Of Digital Image Synthesis' ch10
 * - Glassner (Morgan Kaufmann 1995)</cite>
 *
 * Reference: <cite>'An Optimal Filter For Image Reconstruction' (p101-p104)
 * - Max (Graphics Gems 2, AP 1991)</cite>
 */
float maxFilter
(
   float kernelRadius,
   float pointDistance
);


/**
 * A cubic spline radial 2D filter kernel function.<br/><br/>
 *
 * Supply the kernel radius, and the query point distance.<br/><br/>
 *
 * Has negative tail.<br/><br/>
 *
 * Reference: <cite>'Principles Of Digital Image Synthesis' ch10
 * - Glassner (Morgan Kaufmann 1995)</cite>
 *
 * Reference: <cite>'Reconstruction Filters In Computer Graphics'
 * - Mitchell, Netravali [Siggraph Conference Proceedings 1988]</cite>
 */
float mitchellNetravaliFilter
(
   float kernelRadius,
   float pointDistance
);


}//namespace




#endif//Filters_h
