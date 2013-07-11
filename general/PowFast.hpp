/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2005-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef PowFast_h
#define PowFast_h




#include "hxa7241_general.hpp"
namespace hxa7241_general
{

/// rough //////////////////////////////////////////////////////////////////////

/**
 * Fast approximation to pow, with moderate precision.<br/><br/>
 *
 * Mean error < 0.2%, max error < 0.3%.
 */


/** 2 ^ number. Number must be > -125 and < +128.*/
float powFast2( float f );


/** e ^ number. Number must be > -87.3ish and < +88.7ish. */
float powFastE( float f );


/** 10 ^ number. Number must be > -37.9ish and < +38.5ish. */
float powFast10( float f );


/**
 * Get r ^ number.<br/><br/>
 *
 * @logr  logE of radix for power
 * @f     power to apply (beware under/over-flow)
 */
float powFastR
(
   const float logr,
   const float f
);




/// fixed //////////////////////////////////////////////////////////////////////

/**
 * Fast approximation to pow, with maximum precision.<br/><br/>
 *
 * Mean error < 0.0003%, max error < 0.002%.<br/>
 * Storage is 4KB.
 */


/** 2 ^ number. Number must be > -125 and < +128.*/
float powFast2Two( float f );


/** e ^ number. Number must be > -87.3ish and < +88.7ish. */
float powFast2E( float f );


/** 10 ^ number. Number must be > -37.9ish and < +38.5ish. */
float powFast2Ten( float f );


/**
* Get r ^ number.<br/><br/>
*
* @logr  logE of radix for power
* @f     power to apply (beware under/over-flow)
*/
float powFast2R
(
   float logr,
   float f
);




/// adjustable /////////////////////////////////////////////////////////////////

/**
 * Fast approximation to pow, with adjustable precision.<br/><br/>
 *
 * Precision can be 0 to 18.<br/>
 * For precision 11: mean error < 0.01%, max error < 0.02%.<br/>
 * Storage is (2 ^ precision) * 4 bytes -- 4B to 1MB<br/><br/>
 */
class PowFast
{
/// standard object services ---------------------------------------------------
public:
   explicit PowFast( udword precision = 11 );

           ~PowFast();
private:
            PowFast( const PowFast& );
   PowFast& operator=( const PowFast& );
public:

/// queries --------------------------------------------------------------------
           /** 2 ^ number. Number must be > -125 and < +128.*/
           float two( float )                                             const;

           /** e ^ number. Number must be > -87.3ish and < +88.7ish. */
           float e  ( float )                                             const;

           /** 10 ^ number. Number must be > -37.9ish and < +38.5ish. */
           float ten( float )                                             const;

          /**
           * Get r ^ number.<br/><br/>
           *
           * @logr  logE of radix for power
           * @f     power to apply (beware under/over-flow)
           */
           float r  ( float logr,
                      float f )                                           const;

           udword precision()                                             const;

/// fields ---------------------------------------------------------------------
private:
   udword  precision_m;
   udword* pTable_m;
};


/// default instance
const PowFast& POWFAST();

}//namespace




#endif//PowFast_h
