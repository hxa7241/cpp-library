/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2005-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include <math.h>

#include "LogFast.hpp"


using namespace hxa7241_general;




/// rough //////////////////////////////////////////////////////////////////////

/// implementation -------------------------------------------------------------

/**
 * Fast approximation to log2.
 *
 * Laurent de Soras, 2001.
 * http://flipcode.com/cgi-bin/fcarticles.cgi?show=63828
 *
 * Here is a code snippet to replace the slow log() function... It just performs
 * an approximation, but the maximum error is below 0.007. Speed gain is about
 * x5, and probably could be increased by tweaking the assembly code.
 *
 * The function is based on floating point coding. It's easy to get
 * floor(log2(N)) by isolating exponent part. We can refine the approximation by
 * using the mantissa. This function returns log2(N) .
 *
 * The line (1) computes 1+log2(m), m ranging from 1 to 2. The proposed formula
 * is a 2nd degree polynomial keeping first derivate continuity. Higher degree
 * could be used for more accuracy. For faster results, one can remove this
 * line, if accuracy is not the matter (it gives some linear interpolation
 * between powers of 2).
 *
 * Now we got log2(N), we have to multiply it by ln(2) to get the natural log.
 *
 * [Rewritten by HXA7241, 2007.]
 */
float hxa7241_general::logFast2
(
   float val
)
{
   // get access to float bits
   int* const pVal = reinterpret_cast<int*>(&val);

   // extract exponent, and reset val exponent to zero
   const int  log2 = ((*pVal >> 23) & 0xFF) - 128;
   *pVal = (*pVal & ~(0xFF << 23)) | (127 << 23);

   // make refinement by interpolation
   const float v = ((-1.0f / 3.0f) * val + 2.0f) * val - 2.0f / 3.0f;   // (1)

   return static_cast<float>(log2) + v;


//   original:
//
//   int* const exp_ptr = reinterpret_cast<int*>(&val);
//   int        x       = *exp_ptr;
//   const int  log_2   = ((x >> 23) & 255) - 128;
//   x &= ~(255 << 23);
//   x += 127 << 23;
//   *exp_ptr = x;
//
//   val = ((-1.0f / 3) * val + 2) * val - 2.0f / 3;   // (1)
//
//   return val + log_2;
}








/// adjustable /////////////////////////////////////////////////////////////////

/// implementation -------------------------------------------------------------

/**
 * 'Revisiting a basic function on current CPUs: A fast logarithm implementation
 * with adjustable accuracy'
 * Oriol Vinyals, Gerald Friedland, Nikki Mirghafori;
 * ICSI,
 * 2007-06-21.
 *
 * [Improved (doubled accuracy) and rewritten by HXA7241, 2007.]
 */
namespace
{

/**
 * @pTable     length must be 2 ^ precision
 * @precision  number of mantissa bits used, >= 0 and <= 23
 */
void log2SetTable
(
   float* const pTable,
   const udword precision
)
{
   // step along table elements and x-axis positions
   float oneToTwo = 1.0f + (1.0f / static_cast<float>( 1 << (precision + 1) ));
   for( int i = 0;  i < (1 << precision);  ++i )
//   float oneToTwo = 1.0f;
//   for( int i = 0;  i < ((1 << precision) + 1);  ++i )   /// ALTERNATIVE ///
   {
      // make y-axis value for table element
      pTable[i] = ::logf(oneToTwo) / 0.69314718055995f;

      oneToTwo += 1.0f / static_cast<float>( 1 << precision );
   }
}


/**
 * @pTable     length must be 2 ^ precision
 * @precision  number of mantissa bits used, >= 0 and <= 23
 */
inline
float log2Lookup
(
   const float  val,
   float* const pTable,
   const udword precision
)
{
   // get access to float bits
   const int* const pVal = reinterpret_cast<const int*>(&val);

   // extract exponent and mantissa (quantized)
   const int exp = ((*pVal >> 23) & 0xFF) - 127;
   const int man = (*pVal & 0x7FFFFF) >> (23 - precision);
//   const int man = ((*pVal & 0x7FFFFF) + (0x400000 >> precision)) >>
//      (23 - precision);   /// ALTERNATIVE ///

   // exponent plus lookup refinement
   return static_cast<float>(exp) + pTable[ man ];
}

}




/// wrapper class --------------------------------------------------------------

LogFast::LogFast
(
   const udword precision
)
 : precision_m( precision <= 23u ? precision : 23u )
 , pTable_m   ( new float[ 1 << precision_m ] )
// , pTable_m   ( new float[ (1 << precision_m) + 1 ] )   /// ALTERNATIVE ///
{
   log2SetTable( pTable_m, precision_m );
}


LogFast::~LogFast()
{
   delete[] pTable_m;
}


float LogFast::two
(
   const float f
) const
{
   return log2Lookup( f, pTable_m, precision_m );
}




/// default instance -----------------------------------------------------------

const LogFast& hxa7241_general::LOGFAST()
{
   static const LogFast k( 11 );
   return k;
}




// original:
//
///* Creates the ICSILog lookup table. Must be called
//   once before any call to icsi_log().
//   n is the number of bits to be taken from the mantissa
//   (0<=n<=23)
//   lookup_table is a pointer to a floating point array
//   (memory has to be allocated by the user) of 2^n positions.
//*/
//void fill_icsi_log_table(const int n, float *lookup_table)
//{
//   float numlog;
//   int *const exp_ptr = ((int*)&numlog);
//   int x = *exp_ptr; //x is the float treated as an integer
//   x = 0x3F800000; //set the exponent to 0 so numlog=1.0
//   *exp_ptr = x;
//   int incr = 1 << (23-n); //amount to increase the mantissa
//   int p=pow(2,n);
//   for(int i=0;i<p;++i)
//   {
//      lookup_table[i] = log2(numlog); //save the log value
//      x += incr;
//      *exp_ptr = x; //update the float value
//   }
//}
//
///* Computes an approximation of log(val) quickly.
//   val is a IEEE 754 float value, must be >0.
//   lookup_table and n must be the same values as
//   provided to fill_icsi_table.
//   returns: log(val). No input checking performed.
//*/
//inline float icsi_log(register float val,
//register const float *lookup_table, register const int n)
//{
//   register int *const exp_ptr = ((int*)&val);
//   register int x = *exp_ptr; //x is treated as integer
//   register const int log_2 = ((x >> 23) & 255) - 127;//exponent
//   x &= 0x7FFFFF; //mantissa
//   x = x >> (23-n); //quantize mantissa
//   val = lookup_table[x]; //lookup precomputed value
//   return ((val + log_2)* 0.69314718); //natural logarithm
//}








/// test -----------------------------------------------------------------------
#ifdef TESTING


#include <float.h>
#include <stdlib.h>
#include <ostream>
#include <iomanip>


namespace
{

float log2f( float f )
{
   return ::logf( f ) * 1.44269504088896f;
}

/**
 * rand [0,1) (may be coarsely quantized).
 */
float randFloat()
{
   return static_cast<float>(static_cast<udword>(::rand())) /
      static_cast<float>(static_cast<udword>(RAND_MAX) + 1);
}

bool testOneFunction
(
   std::ostream* pOut,
   const bool    isVerbose,
   float       (*reference)( float ),
   const float   radix,
   const dword   count,
   const float   meanMax,
   const float   maxMax,
   const char*   name,
   float       (*fn)( float )
);


const LogFast logFastAdj( 11 );

float logFastAdj2( const float f )
{
   return logFastAdj.two( f );
}

float logFastAdjE( const float f )
{
   return logFastAdj.e( f );
}

float logFastAdj10( const float f )
{
   return logFastAdj.ten( f );
}

}


namespace hxa7241_general
{

bool test_LogFast
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   seed
)
{
   bool isOk = true;

   if( pOut ) *pOut << "[ test_LogFast ]\n\n";

   if( pOut )
   {
      pOut->setf( std::ios_base::scientific, std::ios_base::floatfield );
      pOut->precision( 6 );
   }


   ::srand( static_cast<unsigned>(seed) );
   static const dword COUNT = 1000000;

   // rough
   {
      isOk &= testOneFunction( pOut, isVerbose, &::log2f, 2.0f,
         COUNT, 0.006f, 0.01f, "rough 2", &logFast2 );

      isOk &= testOneFunction( pOut, isVerbose, &::logf, 2.71828182845905f,
         COUNT, 0.004f, 0.007f, "rough E", &logFastE );

      isOk &= testOneFunction( pOut, isVerbose, &::log10f, 10.0f,
         COUNT, 0.002f, 0.003f, "rough 10", &logFast10 );
   }

   // adjustable
   {
      isOk &= testOneFunction( pOut, isVerbose, &::log2f, 2.0f,
         COUNT, 0.0002f, 0.0004f, "adjustable(11) 2", &logFastAdj2 );

      isOk &= testOneFunction( pOut, isVerbose, &::logf, 2.71828182845905f,
         COUNT, 0.00009f, 0.0003f, "adjustable(11) E", &logFastAdjE );

      isOk &= testOneFunction( pOut, isVerbose, &::log10f, 10.0f,
         COUNT, 0.00004f, 0.0002f, "adjustable(11) 10", &logFastAdj10 );
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
   std::ostream* pOut,
   const bool    isVerbose,
   float       (*reference)( float ),
   const float   ,//radix,
   const dword   count,
   const float   meanMax,
   const float   maxMax,
   const char*   name,
   float       (*fn)( float )
)
{
   static const int PRINT = 50;

   float sumDif = 0.0f;
   float maxDif = static_cast<float>(FLT_MIN);

   for( dword j = 0;  j < count;  ++j )
   {
      const float a = ((static_cast<float>( j ) + randFloat()) /
         (static_cast<float>(count) / 13.0f)) - 6.0f;
      const float number = ::powf( 10.0f, a );

      const float l    = reference( number );
      const float lf   = fn( number );
      const float lDif = ::fabsf( ( lf - l ) );// / l );
      sumDif += lDif;
      maxDif = (maxDif >= lDif) ? maxDif : lDif;

      if( ((count < PRINT) || (0 == (j % (count / PRINT)))) && pOut && isVerbose )
      {
         *pOut <<  std::scientific << number << "  " <<
            //std::setw(14) << l << "  " <<
            std::setw(14) << lf << "  " <<
            std::setw(14) << lDif << "\n";
      }
   }
   if( pOut && isVerbose ) *pOut << "\n";

   const float meanDif = sumDif / static_cast<float>(count);

   if( pOut && isVerbose ) *pOut << std::fixed <<
      "mean diff: " << meanDif << "  max diff: " << maxDif << "" << "\n\n";

   bool isOk = (meanDif < meanMax) & (maxDif < maxMax);

   if( pOut ) *pOut << name << " : " <<
      (isOk ? "--- succeeded" : "*** failed") << "\n\n";

   return isOk;
}

}


#endif//TESTING
