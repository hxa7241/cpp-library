/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef HaltonSequence_h
#define HaltonSequence_h




#include "hxa7241_graphics.hpp"
namespace hxa7241_graphics
{


/// core -----------------------------------------------------------------------
/**
 * Standalone Halton sequence functions.<br/><br/>
 *
 * (The following class wraps some state around these.)<br/><br/>
 */

/** @param index >= 0 */
float getHaltonPoint
(
   dword primeNumber,
   dword index
);

/** @param index >= 0 */
float getHaltonPointJittered
(
   dword primeNumber,
   dword index,
   float random01
);

/** @param index >= 0 */
float jitterHaltonPoint
(
   float point,
   dword index,
   float random01
);




/**
 * A Halton sequence, with optional jittering.<br/><br/>
 *
 * Only the first 15 primes are implemented.<br/><br/>
 *
 * Allows random access to the sequence.<br/><br/>
 *
 * Jittering is inversely proportional to index.
 *
 * @implementation
 * adapted from:
 * <cite>'Sampling with Hammersley and Halton Points' - Wong, Luk, Heng
 * (Journal of Graphics Tools, vol2, no2, 1997)</cite>
 */
class HaltonSequence
{
/// standard object services ---------------------------------------------------
public:
            HaltonSequence();
            /**
             * @param base the index of the prime: first index is 0,
             * corresponding to first prime, 2.
             */
   explicit HaltonSequence( dword base );

           ~HaltonSequence();
            HaltonSequence( const HaltonSequence& );
   HaltonSequence& operator=( const HaltonSequence& );


/// commands -------------------------------------------------------------------
           /**
            * @param base the index of the prime: first index is 0,
            * corresponding to first prime, 2.
            */
           void  setBase( dword base );
           /** @param index >= 0 */
           void  setIndex( dword index );

           void  next();


/// queries --------------------------------------------------------------------
           dword getPrime()                                               const;
           dword getIndex()                                               const;

           float getPoint()                                               const;


/// constants ------------------------------------------------------------------
   static const dword PRIMES[];


/// fields ---------------------------------------------------------------------
private:
   dword prime_m;

   dword index_m;
   float point_m;
};








/// INLINES ///

/// queries --------------------------------------------------------------------
inline
dword HaltonSequence::getPrime() const
{
   return prime_m;
}


inline
dword HaltonSequence::getIndex() const
{
   return index_m;
}


inline
float HaltonSequence::getPoint() const
{
   return point_m;
}


}//namespace




#endif//HaltonSequence_h
