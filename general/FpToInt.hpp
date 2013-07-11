/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef FpToInt_h
#define FpToInt_h




#include "hxa7241_general.hpp"
namespace hxa7241_general
{


/**
 * int-float conversion functions.
 */


/// fp to int ------------------------------------------------------------------
/**
 * Converts from float to 32bit int, truncating fractional part.<br/><br/>
 *
 * Clamps to dword range.
 */
dword fpToIntTrunc( const float f );


/**
 * Converts from float to 32bit int, rounding fractional part.<br/><br/>
 *
 * Clamps to dword range.<br/><br/>
 *
 * Some large magnitude floats may round inaccurately.
 */
inline
dword fpToIntRound
(
   const float f
)
{
   return fpToIntTrunc( f + (f >= 0.0f ? 0.5f : -0.5f) );
}


/**
 * Converts from float to 32bit int, rounding fractional part.<br/><br/>
 *
 * floats outside dword range produce undefined results.<br/><br/>
 *
 * Some large magnitude floats may round inaccurately.
 */
inline
dword round
(
   const float f
)
{
   return static_cast<dword>( f + (f >= 0.0f ? 0.5f : -0.5f) );
}




/// fp to int 16.16 bit --------------------------------------------------------
/**
 * Converts from float to 16.16bit fixed point int, truncating fractional
 * part.<br/><br/>
 *
 * Clamps to dword range.<br/><br/>
 */
inline
dword fpToInt1616
(
   const float f
)
{
   return fpToIntTrunc( f * 65536.0f );
}




/// fp [0-1) to int ------------------------------------------------------------
/**
 * Converts from [0-1) float to 32bit int, truncating fractional part.<br/><br/>
 *
 * floats outside range produce undefined results.<br/><br/>
 */
inline
udword fp01ToDword
(
   const float f
)
{
   return static_cast<udword>( f * 4294967296.0f );
}


/**
 * Converts from [0-1) float to 16bit int, truncating fractional part.<br/><br/>
 *
 * floats outside range produce undefined results.<br/><br/>
 */
inline
dword fp01ToWord
(
   const float f
)
{
   return static_cast<dword>( f * 65536.0f );
}


/**
 * Converts from [0-1) float to 8bit int, truncating fractional part.<br/><br/>
 *
 * floats outside range produce undefined results.<br/><br/>
 */
inline
dword fp01ToByte
(
   const float f
)
{
   return static_cast<dword>( f * 256.0f );
}


/**
 * Converts from [0-1) float to 4bit int, truncating fractional part.<br/><br/>
 *
 * floats outside range produce undefined results.<br/><br/>
 */
inline
dword fp01ToNybble
(
   const float f
)
{
   return static_cast<dword>( f * 16.0f );
}




/// int to fp ------------------------------------------------------------------
struct FRACTIONALIZER
{
   static const float _16BIT;
   static const float  _8BIT;
   static const float  _4BIT;
};


/**
 * Converts from 16bit int to float fraction.<br/><br/>
 *
 * Repeats int bit pattern along fraction bits.
 */
inline
float uwordToFp
(
   const dword w
)
{
   return static_cast<float>((w << 8) | (w >> 8)) * FRACTIONALIZER::_16BIT;
}


/**
 * Converts from 8bit int to float fraction.<br/><br/>
 *
 * Repeats int bit pattern along fraction bits.
 */
inline
float ubyteToFp
(
   const dword b
)
{
   return static_cast<float>(b) * FRACTIONALIZER::_8BIT;
}


/**
 * Converts from 4bit int to float fraction.<br/><br/>
 *
 * Repeats int bit pattern along fraction bits.
 */
inline
float unybbleToFp
(
   const dword n
)
{
   return static_cast<float>(n) * FRACTIONALIZER::_4BIT;
}


}//namespace




#endif//FpToInt_h
