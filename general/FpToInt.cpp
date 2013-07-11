/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include "FpToInt.hpp"




namespace hxa7241_general
{


dword fpToIntTrunc
(
   const float f
)
{
   dword d;

   if( static_cast<float>(DWORD_MIN) > f )
   {
      d = DWORD_MIN;
   }
   else if( static_cast<float>(static_cast<udword>(DWORD_MAX) + 1u) <= f )
   {
      d = DWORD_MAX;
   }
   else
   {
      d = static_cast<dword>( f );
   }

   return d;


// // 01111111,10000000,00000000,00000000
// static const dword EXPONENT_BITS   = static_cast<dword>(0x7F800000);
// // 00000000,01111111,11111111,11111111
// static const dword MANTISSA_BITS   = static_cast<dword>(0x007FFFFF);
// // 00000000,10000000,00000000,00000000
// static const dword IMPLICIT_1      = static_cast<dword>(0x00800000);
// // 00000000,00000000,00000000,10011101
// static const dword EXP_2POW_PLUS30 = static_cast<dword>(0x0000009D);
//
// const dword floatBits = *(reinterpret_cast<const dword*>( &f ));
//
// dword d = 0;
//
// // if not +/- 0
// if( (floatBits << 1) != 0 )
// {
//    // extract mantissa
//    d = ((floatBits & MANTISSA_BITS) | IMPLICIT_1) << 7;
//    const dword shift = EXP_2POW_PLUS30 - ((floatBits & EXPONENT_BITS) >> 23);
//    d >>= shift;
//
//    // clamp, to min (DWORD_MIN+1, not DWORD_MIN), then max
//    d &= (shift - 32) >> 31;
//    d = (d | (shift >> 31)) & 0x7FFFFFFF;
//
//    // set sign
//    d = (d - static_cast<dword>(static_cast<udword>(floatBits) >> 31)) ^
//       (floatBits >> 31);
// }
//
// return d;
}




const float FRACTIONALIZER::_16BIT =       1.0f / 16777216.0f;
const float FRACTIONALIZER::_8BIT  =   65793.0f / 16777216.0f;
const float FRACTIONALIZER::_4BIT  = 1118481.0f / 16777216.0f;


}//namespace








/// test -----------------------------------------------------------------------
#ifdef TESTING


#include <math.h>
#include <iostream>


namespace hxa7241_general
{


/// random number generator 1

//static dword random_m;
//
//static void setRand( const dword seed )
//{
// random_m = seed;
//}
//
//static dword getRand()
//{
// random_m = static_cast<dword>(1664525) * random_m +
//    static_cast<dword>(1013904223);
// return random_m;
//}


/// random number generator 2

// concatenation of following two 16-bit multiply with carry generators
// x(n)=a*x(n-1)+carry mod 2^16 and y(n)=b*y(n-1)+carry mod 2^16,
// number and carry packed within the same 32 bit integer.
//
// Perhaps the fastest of any generator that passes the Diehard tests.
//
// http://paul.rutgers.edu/~rhoads/Code/code.html

static void  setRand2( udword seedX, udword seedY = 0 );
static dword getRand2();

static const udword SEED_X = 521288629u;
static const udword SEED_Y = 362436069u;
static udword seedX_m = SEED_X;
static udword seedY_m = SEED_Y;

static void setRand2
(
   const udword seedX,
   const udword seedY
)
{
   seedX_m = 0 != seedX ? seedX : SEED_X;
   seedY_m = 0 != seedY ? seedY : SEED_Y;
}

static dword getRand2()
{
   // Use any pair of non-equal numbers from this list for the two constants
   // 18000 18030 18273 18513 18879 19074 19098 19164 19215 19584
   // 19599 19950 20088 20508 20544 20664 20814 20970 21153 21243
   // 21423 21723 21954 22125 22188 22293 22860 22938 22965 22974
   // 23109 23124 23163 23208 23508 23520 23553 23658 23865 24114
   // 24219 24660 24699 24864 24948 25023 25308 25443 26004 26088
   // 26154 26550 26679 26838 27183 27258 27753 27795 27810 27834
   // 27960 28320 28380 28689 28710 28794 28854 28959 28980 29013
   // 29379 29889 30135 30345 30459 30714 30903 30963 31059 31083

   seedX_m = 18000u * (seedX_m & 0xFFFFu) + (seedX_m >> 16);
   seedY_m = 30903u * (seedY_m & 0xFFFFu) + (seedY_m >> 16);

   return static_cast<dword>((seedX_m << 16) + (seedY_m & 0xFFFFu));
}



bool test_FpToInt
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   seed
)
{
   bool isOk = true;

// setRand( seed );
   setRand2( seed );

   if( pOut ) *pOut << "[ test_FpToInt ]\n\n";


   // dword fpToIntTrunc( float );
   // dword fpToIntRound( float );
   // dword round( float );
   {
      bool isOk_ = true;

      static const float BASICS[] = {
         0.0f, 1.0f, -1.0f, 10000.0f, -10000.0f,
         static_cast<float>(DWORD_MAX - 127),
         -static_cast<float>(DWORD_MAX) };
      static const float FRACT = 100000.0f;

      // float to int, back to float, almost equals original float
      for( udword i = 0;  i < 100000 && isOk_;  ++i )
      {
         float value = 0.0f;
         // first lot: presets
         if( i < sizeof(BASICS)/sizeof(BASICS[1]) )
         {
            value = BASICS[i];
         }
         // second lot: fractions
         else if( i < (sizeof(BASICS)/sizeof(BASICS[1]) + 1000) )
         {
            value = FRACT + (static_cast<float>(i -
               (sizeof(BASICS)/sizeof(BASICS[1]))) / 1000.0f);
         }
         // last lot: random
         else
         {
            value = static_cast<float>(getRand2()) +
               (static_cast<float>(getRand2()) /
               static_cast<float>(static_cast<udword>(DWORD_MAX) + 1u));
            value = (i & 1) ? -value : value;
         }

         const dword ds[] = {
            fpToIntTrunc( value ), fpToIntRound( value ), round( value ) };
         const float tols[] = { 1.0f, 0.5f, 0.5f };
         for( udword j = 0;  j < sizeof(ds)/sizeof(ds[0]);  ++j )
         {
            const float f = static_cast<float>(ds[j]);
            const float diff = ::fabsf(value - f);
            bool is = (diff <= tols[j]);

            // if round fails with a large value, increase tolerance
            if( !is & (0 < j) & (::fabsf(f) > 8.39e+6f) )
            {
               is = (diff <= 1.0f);
            }

            if( pOut && isVerbose && !is ) *pOut << i << " " << j << "  " <<
               value << "  " << ds[j] << "  " << f << "  " << diff << "  " <<
               is << "\n";
            isOk_ &= is;
         }

         if( pOut && isVerbose && !isOk_ ) *pOut << "\n";
      }

      // clamped to dword range
      for( dword i = 0;  i < 10000 && isOk_;  ++i )
      {
         dword clamp = DWORD_MAX;
         float value = static_cast<float>(static_cast<udword>(DWORD_MAX) + 1u) +
            static_cast<float>(getRand2() & DWORD_MAX) + 0.5f;
         if( 1 == (getRand2() & 1) )
         {
            clamp = DWORD_MIN;
            value = -value;
         }

         const dword d1 = fpToIntTrunc( value );
         isOk_ &= d1 == clamp;

         const dword d2 = fpToIntRound( value );
         isOk_ &= d2 == clamp;

         if( pOut && isVerbose && !isOk_ ) *pOut << value << "  " << clamp <<
            "  " << d1 << "  " << d2 << "  " << (d1 - clamp) << "  " <<
            (d2 - clamp) << "\n";
      }

      if( pOut ) *pOut << "fpToInt : " <<
         (isOk_ ? "--- succeeded" : "*** failed") << "\n\n";

      isOk &= isOk_;
   }


   // dword fpToInt1616( float );
   {
      bool isOk_ = true;

      static const float BASICS[] = { 0.0f, 1.0f, -1.0f, 10000.0f, -10000.0f,
                                      static_cast<float>(WORD_MAX),
                                      -static_cast<float>(WORD_MAX) };
      static const float FRACT = 100.0f;

      // float to int, back to float, almost equals original float
      for( udword i = 0;  i < 100000 && isOk_;  ++i )
      {
         float value = 0.0f;
         // first lot: presets
         if( i < sizeof(BASICS)/sizeof(BASICS[1]) )
         {
            value = BASICS[i];
         }
         // second lot: fractions
         else if( i < (sizeof(BASICS)/sizeof(BASICS[1]) + 1000) )
         {
            value = FRACT +
               (static_cast<float>(i - (sizeof(BASICS)/sizeof(BASICS[1]))) /
               (1000.0f));
         }
         // last lot: random
         else
         {
            value = static_cast<float>(getRand2() &
               static_cast<dword>(static_cast<udword>(WORD_MAX))) +
               (static_cast<float>(getRand2()) /
               static_cast<float>(static_cast<udword>(DWORD_MAX) + 1u));
            value = (i & 1) ? -value : value;
         }

         const dword d = fpToInt1616( value );
         const float f = static_cast<float>(d) /
            (static_cast<float>(UWORD_MAX) + 1);
         const float diff = ::fabsf(value - f);
         const bool is = diff < (1.0f / 65536.0f);
         if( pOut && isVerbose && !is ) *pOut << i << "  " << value << "  " <<
            d << "  " << f << "  " << diff << "  " << is << "\n";
         isOk_ &= is;
      }

      if( pOut ) *pOut << "fpToInt1616 : " <<
         (isOk_ ? "--- succeeded" : "*** failed") << "\n\n";

      isOk &= isOk_;
   }


   // dword fp01ToDword( float );
   // dword fp01ToWord( float );
   // dword fp01ToByte( float );
   // dword fp01ToNybble( float );
   {
      bool isOk_ = true;

      static const float BASICS[] = { 0.0f, FLOAT_ALMOST_ONE, 0.5f,
         0.9f, 0.00001f };

      // float to int, back to float, almost equals original float
      for( udword i = 0;  i < 100000 && isOk_;  ++i )
      {
         float value = 0.0f;
         // first lot: presets
         if( i < sizeof(BASICS)/sizeof(BASICS[1]) )
         {
            value = BASICS[i];
         }
         // last lot: random [0,1)
         else
         {
            value = static_cast<float>(getRand2() & DWORD_MAX) /
               static_cast<float>(static_cast<udword>(DWORD_MAX) + 1u);
         }

         const udword ds[] = {
            fp01ToDword( value ), fp01ToWord( value ),
            fp01ToByte( value ), fp01ToNybble( value ) };
         const udword cs[] = {
            static_cast<udword>( (value * 4294967296.0f) ),
            static_cast<udword>( (value * 65536.0f) ),
            static_cast<udword>( ::floorf(value * 256.0f) ),
            static_cast<udword>( ::floorf(value * 16.0f) ) };
         const float divs[] = {
            4294967296.0f,
            static_cast<float>(static_cast<udword>(UWORD_MAX) + 1u),
            static_cast<float>(static_cast<udword>(UBYTE_MAX) + 1u),
            16.0f };
         const float tolerances[] = {
            (1.0f / static_cast<float>(1 << 23)),
            (1.0f / static_cast<float>(1 << 16)),
            (1.0f / static_cast<float>(1 << 8)),
            (1.0f / static_cast<float>(1 << 4)) };

         for( udword j = 0;  j < sizeof(ds)/sizeof(ds[0]);  ++j )
         {
            const float f = static_cast<float>(ds[j]) / divs[j];
            const float diff = ::fabsf(value - f);
            bool  is = diff <= tolerances[j];
            is &= ds[j] == cs[j];
            if( pOut && isVerbose && !is ) *pOut << i << " " << j << "  " <<
               value << "  " << ds[j] << "  " << cs[j] << "  " << f << "  " <<
               diff << "  " << is << "\n";
            isOk_ &= is;
         }

         if( pOut && isVerbose && !isOk_ ) *pOut << "\n";
      }

      if( pOut ) *pOut << "fp01ToDword : " <<
         (isOk_ ? "--- succeeded" : "*** failed") << "\n\n";

      isOk &= isOk_;
   }


   // float uwordToFp( dword );
   // float ubyteToFp( dword );
   // float unybbleToFp( dword );
   {
      bool isOk_ = true;

      for( int i = 0;  i < 100000 && isOk_;  ++i )
      {
         const udword r = static_cast<udword>(getRand2());

         const udword rs[] = { static_cast<dword>(r >> 16),
            static_cast<dword>(r >> 24), static_cast<dword>(r >> 28) };
         const float fs[] = { uwordToFp( rs[0] ), ubyteToFp( rs[1] ),
            unybbleToFp( rs[2] ) };
         udword ds[3];
         for( udword j = 0;  j < sizeof(fs)/sizeof(fs[0]);  ++j )
         {
            ds[j] = static_cast<udword>(::floorf(fs[j] * 4294967296.0f));
         }

         udword ss[3];
         ss[0] = (rs[0] | (rs[0] << 16)) & 0xFFFFFF00;
         ss[1] = (rs[1] << 8) | (rs[1] << 16) | (rs[1] << 24);
         ss[2] = (rs[2] << 8) | (rs[2] << 12) | (rs[2] << 16) |
            (rs[2] << 20) | (rs[2] << 24) | (rs[2] << 28);
         for( udword j = 0;  j < sizeof(ds)/sizeof(ds[0]);  ++j )
         {
            const dword diff = ds[j] - ss[j];
            const bool is = ds[j] == ss[j];
            if( pOut && isVerbose && !isOk_ ) *pOut << i << " " << j << "  " <<
               rs[j] << "  " << fs[j] << "  " << ds[j] << "  " << ss[j] <<
               "  " << diff << "  " << is << "\n";
            isOk_ &= is;
         }

         if( pOut && isVerbose && !isOk_ ) *pOut << "\n";
      }

      if( pOut ) *pOut << "uToFp : " <<
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
