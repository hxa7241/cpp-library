/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include "HaltonSequence.hpp"


using namespace hxa7241_graphics;




/// core -----------------------------------------------------------------------
float hxa7241_graphics::getHaltonPoint
(
   const dword prime,
   dword       index
)
{
   if( index < 0 )
   {
      index = 0;
   }

   const float oneOverPrime = 1.0f / static_cast<float>(prime);

   float p      = oneOverPrime;
   float result = 0.0f;
   for( dword k = index + 1;  k > 0;  p *= oneOverPrime )
   {
      // assuming * and - is faster than %
      const dword kDiv = k / prime;
      const dword kMod = k - (kDiv * prime);

      result += static_cast<float>(kMod) * p;

      k = kDiv;
   }
// for( dword k = index + 1;  k > 0;  p *= oneOverPrime, k /= prime )
// {
//    // neater, but does two divides
//    result += static_cast<float>(k % prime) * p;
// }

   return result;
}


float hxa7241_graphics::getHaltonPointJittered
(
   const dword prime,
   const dword index,
   const float random01
)
{
   return jitterHaltonPoint( getHaltonPoint( prime, index ), index, random01 );
}


float hxa7241_graphics::jitterHaltonPoint
(
   const float point,
   dword       index,
   const float random01
)
{
   if( index < 0 )
   {
      index = 0;
   }

   // jitter
   const float jitter = (random01 - 0.5f) / static_cast<float>(index + 1);
   float jittered = point + jitter;

   // wrap around
   jittered += (jittered < 0.0f) ? 1.0f : ((jittered >= 1.0f) ? -1.0f : 0.0f);

   return jittered;
}




/// constants ------------------------------------------------------------------
const dword HaltonSequence::PRIMES[] =
   { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47 };




/// standard object services ---------------------------------------------------
HaltonSequence::HaltonSequence()
{
   HaltonSequence::setBase( 0 );
}


HaltonSequence::HaltonSequence
(
   const dword base
)
{
   HaltonSequence::setBase( base );
}


HaltonSequence::~HaltonSequence()
{
}


HaltonSequence::HaltonSequence( const HaltonSequence& other )
{
   HaltonSequence::operator=( other );
}


HaltonSequence& HaltonSequence::operator=( const HaltonSequence& other )
{
   if( &other != this )
   {
      prime_m = other.prime_m;
      index_m = other.index_m;
      point_m = other.point_m;
   }

   return *this;
}




/// commands -------------------------------------------------------------------
void HaltonSequence::setBase
(
   const dword base
)
{
   prime_m = PRIMES[ (base >= 0 ? base : -base) %
      (sizeof(PRIMES) / sizeof(*PRIMES)) ];

   // reset sequence position
   setIndex( 0 );
}


void HaltonSequence::setIndex
(
   const dword index
)
{
   index_m = index >= 0 ? index : 0;

   point_m = getHaltonPoint( prime_m, index_m );
}


void HaltonSequence::next()
{
   setIndex( index_m + 1 );
}








/*class Halton
{
/// standard object services ---------------------------------------------------
public:
            Halton( dword base,
                    dword index );
           ~Halton();
            Halton( const Halton& );
   Halton& operator=( const Halton& );


/// commands -------------------------------------------------------------------
           void  next();


/// queries --------------------------------------------------------------------
           float getPoint()                                               const;


/// fields ---------------------------------------------------------------------
private:
   float invBase_m;
   float value_m;
};




Halton::Halton
(
   const dword base,
   const dword index
)
 : invBase_m( 1.0f / static_cast<float>(base) )
 , value_m  ( 0.0f )
{
   dword i = index;
   float f = invBase_m;
   while( i > 0 )
   {
      value_m += f * float(i % base);
      i /= base;
      f *= invBase_m;
   }
}


inline
Halton::~Halton()
{
}


inline
Halton::Halton
(
   const Halton& other
)
{
   Halton::operator=( other );
}


Halton& Halton::operator=( const Halton& other )
{
   invBase_m = other.invBase_m;
   value_m   = other.value_m;

   return *this;
}


void Halton::next()
{
   float r = 1.0f - value_m - FLOAT_EPSILON;
   if( invBase_m < r )
   {
      value_m += invBase_m;
   }
   else
   {
      float h = invBase_m;
      float hh;
      do
      {
         hh = h;
         h *= invBase_m;
      } while( h >= r );

      value_m += hh + h - 1.0f;
   }
}


inline
float Halton::getPoint() const
{
   return value_m;
}*/
