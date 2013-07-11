/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2005-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include <math.h>

#include "PowFast.hpp"


using namespace hxa7241_general;




/// rough //////////////////////////////////////////////////////////////////////

/// implementation -------------------------------------------------------------

namespace
{

/**
 * Following the bit-twiddling idea in:
 *
 * 'A Fast, Compact Approximation of the Exponential Function'
 * Technical Report IDSIA-07-98
 * Nicol N. Schraudolph;
 * IDSIA,
 * 1998-06-24.
 *
 * [Rewritten for floats and quadratic interpolation by HXA7241, 2007.]
 */
inline
float powFast
(
   const float val,
   const float ilog2
)
{
   static const float _2p23 = 8388608.0f;

   // build float bits, with linear interpolation of fraction
   const int i = static_cast<int>( (val * (_2p23 * ilog2)) + (127.0f * _2p23) );

   // extract exponent, and mantissa as 1 to 2 float
   const int   exp = i & 0xFF800000;
   const int   man = 0x3F800010 | (i & 0x7FFFFF);
   const float m12 = *reinterpret_cast<const float*>( &man );

   // make quadratic interpolation with mantissa (x[1,2) -> y[1,2))
   // (minimise relative error)
   const float q12 = (0.34f * (m12 * m12)) - (0.02f * m12) + 0.68f;
   // (minimise absolute error)
   //const float q12 = (0.344285f * (m12 * m12)) - (0.032855f * m12) + 0.68857f;

   // assemble exponent and interpolation, and convert bits to float
   const int eq = exp | (*reinterpret_cast<const int*>( &q12 ) & 0x7FFFFF);
   return *reinterpret_cast<const float*>( &eq );


   /*// scaling to move point to between exp and man in float bits
   static const float _2p23 = 8388608.0f;

   // build float bits, with linear interpolation of fraction
   const int i = static_cast<int>( (val * (_2p23 * ilog2)) + (127.0f * _2p23) );

   // convert bits to float
   return *reinterpret_cast<const float*>( &i );*/
}

}


/// wrapper functions ----------------------------------------------------------
float hxa7241_general::powFast2
(
   const float f
)
{
   return powFast( f, 1.0f );
}


float hxa7241_general::powFastE
(
   const float f
)
{
   return powFast( f, 1.44269504088896f );
}


float hxa7241_general::powFast10
(
   const float f
)
{
   return powFast( f, 3.32192809488736f );
}


float hxa7241_general::powFastR
(
   const float logr,
   const float f
)
{
   return powFast( f, (logr * 1.44269504088896f) );
}








/// fixed //////////////////////////////////////////////////////////////////////

/// implementation -------------------------------------------------------------

namespace
{

class PowFast2
{
public:
/// standard object services
            PowFast2();
           ~PowFast2();
private:
   static void  setTable( float* pTable,
                          udword precision,
                          udword extent,
                          bool   isRound );

            PowFast2( const PowFast2& );
   PowFast2& operator=( const PowFast2& );

/// queries
public:
   float lookup( float f,
                 float ilog2 )                                            const;

/// fields
private:
   float tableH_m[ 1 << 9 ];
   float tableL_m[ 1 << 9 ];
};


PowFast2::PowFast2()
{
   setTable( tableH_m,  9, 9, false );
   setTable( tableL_m, 18, 9, true  );
}


PowFast2::~PowFast2()
{
}


void PowFast2::setTable
(
   float* const pTable,
   const udword precision,
   const udword extent,
   const bool   isRound
)
{
   // step along table elements and x-axis positions
   float zeroToOne = !isRound ?
      0.0f : (1.0f / (static_cast<float>(1 << precision) * 2.0f));
   for( int i = 0;  i < (1 << extent);  ++i )
   {
      // make y-axis value for table element
      pTable[i] = ::powf( 2.0f, zeroToOne );

      zeroToOne += 1.0f / static_cast<float>(1 << precision);
   }
}


inline
float PowFast2::lookup
(
   const float val,
   const float ilog2
) const
{
   const float _2p23 = 8388608.0f;

   // build float bits, with linear interpolation of fraction
   const int i = static_cast<int>( (val * (_2p23 * ilog2)) + (127.0f * _2p23) );

   // replace mantissa with combined lookups
   const float t  = tableH_m[(i >> 14) & 0x1FF] * tableL_m[(i >> 5)  & 0x1FF];
   const int   it = (i & 0xFF800000) |
      (*reinterpret_cast<const int*>( &t ) & 0x7FFFFF);

   // convert bits to float
   return *reinterpret_cast<const float*>( &it );
}




/// instance -------------------------------------------------------------------
const PowFast2 POWFAST2;

}




/// wrapper functions ----------------------------------------------------------
float hxa7241_general::powFast2Two
(
   const float f
)
{
   return POWFAST2.lookup( f, 1.0f );
}


float hxa7241_general::powFast2E
(
   const float f
)
{
   return POWFAST2.lookup( f, 1.44269504088896f );
}


float hxa7241_general::powFast2Ten
(
   const float f
)
{
   return POWFAST2.lookup( f, 3.32192809488736f );
}


float hxa7241_general::powFast2R
(
   const float logr,
   const float f
)
{
   return POWFAST2.lookup( f, (logr * 1.44269504088896f) );
}








/// adjustable /////////////////////////////////////////////////////////////////

/// implementation -------------------------------------------------------------

namespace
{

/**
 * Following the bit-twiddling idea in:
 *
 * 'A Fast, Compact Approximation of the Exponential Function'
 * Technical Report IDSIA-07-98
 * Nicol N. Schraudolph;
 * IDSIA,
 * 1998-06-24.
 *
 * [Rewritten for floats by HXA7241, 2007.]
 *
 * and the adjustable-lookup idea in:
 *
 * 'Revisiting a basic function on current CPUs: A fast logarithm implementation
 * with adjustable accuracy'
 * Technical Report ICSI TR-07-002;
 * Oriol Vinyals, Gerald Friedland, Nikki Mirghafori;
 * ICSI,
 * 2007-06-21.
 *
 * [Improved (doubled accuracy) and rewritten by HXA7241, 2007.]
 */


const float _2p23 = 8388608.0f;


/**
 * Initialize powFast lookup table.
 *
 * @pTable     length must be 2 ^ precision
 * @precision  number of mantissa bits used, >= 0 and <= 18
 */
void powFastSetTable
(
   udword* const pTable,
   const udword  precision
)
{
   // step along table elements and x-axis positions
   float zeroToOne = 1.0f / (static_cast<float>(1 << precision) * 2.0f);
   for( int i = 0;  i < (1 << precision);  ++i )
   {
      // make y-axis value for table element
      const float f = (::powf( 2.0f, zeroToOne ) - 1.0f) * _2p23;
      pTable[i] = static_cast<udword>( f < _2p23 ? f : (_2p23 - 1.0f) );

      zeroToOne += 1.0f / static_cast<float>(1 << precision);
   }
}


/**
 * Get pow (fast!).
 *
 * @val        power to raise radix to
 * @ilog2      one over log, to required radix, of two
 * @pTable     length must be 2 ^ precision
 * @precision  number of mantissa bits used, >= 0 and <= 18
 */
inline
float powFastLookup
(
   const float   val,
   const float   ilog2,
   udword* const pTable,
   const udword  precision
)
{
   // build float bits, with linear interpolation of fraction
   const int i = static_cast<int>( (val * (_2p23 * ilog2)) + (127.0f * _2p23) );

   // replace mantissa with lookup
   const int it = (i & 0xFF800000) | pTable[(i & 0x7FFFFF) >> (23 - precision)];

   // convert bits to float
   return *reinterpret_cast<const float*>( &it );
}

}




/// wrapper class --------------------------------------------------------------

PowFast::PowFast
(
   const udword precision
)
 : precision_m( precision <= 18u ? precision : 18u )
 , pTable_m   ( new udword[ 1 << precision_m ] )
{
   powFastSetTable( pTable_m, precision_m );
}


PowFast::~PowFast()
{
   delete[] pTable_m;
}


float PowFast::two
(
   const float f
) const
{
   return powFastLookup( f, 1.0f, pTable_m, precision_m );
}


float PowFast::e
(
   const float f
) const
{
   return powFastLookup( f, 1.44269504088896f, pTable_m, precision_m );
}


float PowFast::ten
(
   const float f
) const
{
   return powFastLookup( f, 3.32192809488736f, pTable_m, precision_m );
}


float PowFast::r
(
   const float logr,
   const float f
) const
{
   return powFastLookup( f, (logr * 1.44269504088896f), pTable_m, precision_m );
}


udword PowFast::precision() const
{
   return precision_m;
}




/// default instance -----------------------------------------------------------
const PowFast& hxa7241_general::POWFAST()
{
   static const PowFast k( 11 );
   return k;
}








/// test -----------------------------------------------------------------------
#ifdef TESTING


#include <float.h>
#include <stdlib.h>
#include <ostream>
#include <iomanip>


namespace
{

class RandomMwc
{
/// standard object services ---------------------------------------------------
public:
   explicit RandomMwc( udword seed = 0 );

private:
   RandomMwc& operator=( const RandomMwc& );
public:

/// queries --------------------------------------------------------------------
           udword getUdword()                                             const;
           float  getFloat()                                              const;

/// fields ---------------------------------------------------------------------
private:
   mutable udword seeds_m[2];
};

/// standard object services ---------------------------------------------------
RandomMwc::RandomMwc
(
   const udword seed
)
{
   seeds_m[0] = seed ? seed : 521288629u;
   seeds_m[1] = seed ? seed : 362436069u;
}

/// queries --------------------------------------------------------------------
udword RandomMwc::getUdword() const
{
   seeds_m[0] = 18000u * (seeds_m[0] & 0xFFFFu) + (seeds_m[0] >> 16);
   seeds_m[1] = 30903u * (seeds_m[1] & 0xFFFFu) + (seeds_m[1] >> 16);

   return (seeds_m[0] << 16) + (seeds_m[1] & 0xFFFFu);
}

float RandomMwc::getFloat() const
{
   return static_cast<float>(getUdword()) / 4294967296.0f;
}


bool testOneFunction
(
   std::ostream*    pOut,
   const bool       isVerbose,
   const RandomMwc& rand,
   const float      radix,
   const dword      start,
   const dword      end,
   const dword      extraCount,
   const float      meanMax,
   const float      maxMax,
   const char*      name,
   float          (*fn)( float )
);


const PowFast powFastAdj( 11 );

float powFastAdj2( const float f )
{
   return powFastAdj.two( f );
}

float powFastAdjE( const float f )
{
   return powFastAdj.e( f );
}

float powFastAdj10( const float f )
{
   return powFastAdj.ten( f );
}

}


namespace hxa7241_general
{

bool test_PowFast
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   seed
)
{
   bool isOk = true;

   if( pOut ) *pOut << "[ test_PowFast ]\n\n";

   if( pOut )
   {
      pOut->setf( std::ios_base::scientific, std::ios_base::floatfield );
      pOut->precision( 6 );
   }


   const RandomMwc rand( seed );
   const dword     extraCount = 10000;


   // rough
   {
      isOk &= testOneFunction( pOut, isVerbose, rand,
         2.0f, -125, +127, extraCount, 0.2f, 0.3f,
         "rough 2", &powFast2 );

      isOk &= testOneFunction( pOut, isVerbose, rand,
         2.71828182845905f, -86, +88, extraCount, 0.2f, 0.3f,
         "rough E", &powFastE );

      isOk &= testOneFunction( pOut, isVerbose, rand,
         10.0f, -36, +38, extraCount, 0.2f, 0.3f,
         "rough 10", &powFast10 );
   }

   // fixed
   {
      isOk &= testOneFunction( pOut, isVerbose, rand,
         2.0f, -125, +127, extraCount, 0.0003f, 0.002f,
         "fixed 2", &powFast2Two );

      isOk &= testOneFunction( pOut, isVerbose, rand,
         2.71828182845905f, -86, +88, extraCount, 0.0003f, 0.002f,
         "fixed E", &powFast2E );

      isOk &= testOneFunction( pOut, isVerbose, rand,
         10.0f, -36, +38, extraCount, 0.0003f, 0.002f,
         "fixed 10", &powFast2Ten );
   }

   // adjustable
   {
      isOk &= testOneFunction( pOut, isVerbose, rand,
         2.0f, -125, +127, extraCount, 0.01f, 0.02f,
         "adjustable(11) 2", &powFastAdj2 );

      isOk &= testOneFunction( pOut, isVerbose, rand,
         2.71828182845905f, -86, +88, extraCount, 0.01f, 0.02f,
         "adjustable(11) E", &powFastAdjE );

      isOk &= testOneFunction( pOut, isVerbose, rand,
         10.0f, -36, +38, extraCount, 0.01f, 0.02f,
         "adjustable(11) 10", &powFastAdj10 );
   }

   if( pOut ) *pOut << (isOk ? "--- successfully" : "*** failurefully") <<
      " completed " << "\n\n\n";

   if( pOut ) pOut->flush();


   return isOk;
}

}


namespace
{

bool testOneFunction
(
   std::ostream*    pOut,
   const bool       isVerbose,
   const RandomMwc& rand,
   const float      radix,
   const dword      start,
   const dword      end,
   const dword      extraCount,
   const float      meanMax,
   const float      maxMax,
   const char*      name,
   float          (*fn)( float )
)
{
   float sumDif = 0.0f;
   float maxDif = static_cast<float>(FLT_MIN);

   for( dword i = start;  i < end;  ++i )
   {
      for( dword j = 0;  j < extraCount;  ++j )
      {
         const float number = static_cast<float>(i) + rand.getFloat();

         const float p    = ::powf( radix, number );
         const float pf   = fn( number );
         const float pDif = ::fabsf( pf - p ) / p;
         sumDif += pDif;
         maxDif = (maxDif >= pDif) ? maxDif : pDif;

         if( (0 == j) && pOut && isVerbose )
         {
            *pOut << std::showpos << std::fixed << std::setw(11);
            *pOut << number << std::scientific << std::noshowpos;
            *pOut << "  " << pf << "  " << pDif << "\n";
         }
      }
   }
   if( pOut && isVerbose ) *pOut << "\n";

   const float meanDif = 100.0f * ( sumDif / (static_cast<float>(end - start) *
      static_cast<float>(extraCount)) );
   maxDif *= 100.0f;

   if( pOut && isVerbose ) *pOut << std::fixed <<
      "mean diff: " << meanDif << "%  max diff: " << maxDif << "%" << "\n\n";

   bool isOk = (meanDif < meanMax) & (maxDif < maxMax);

   if( pOut ) *pOut << name << " : " <<
      (isOk ? "--- succeeded" : "*** failed") << "\n\n";

   return isOk;
}

}


#endif//TESTING
