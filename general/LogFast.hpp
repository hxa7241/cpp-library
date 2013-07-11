/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2005-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef LogFast_h
#define LogFast_h




#include "hxa7241_general.hpp"
namespace hxa7241_general
{

/// rough //////////////////////////////////////////////////////////////////////

/**
 * Fast approximation to log, with rough precision.<br/><br/>
 */


/** log2( number ). Number must be > 0. Error: mean < 0.006, max < 0.01.*/
float logFast2( float f );


/** ln( number ). Number must be > 0. Error: mean < 0.004, max < 0.007. */
float logFastE( float f );


/** log10( number ). Number must be > 0. Error: mean < 0.002, max < 0.003. */
float logFast10( float f );




/// adjustable /////////////////////////////////////////////////////////////////

/**
 * Fast approximation to log, with adjustable precision.<br/><br/>
 *
 * Storage is (2 ^ precision) * 4 bytes.<br/><br/>
 */
class LogFast
{
/// standard object services ---------------------------------------------------
public:
   explicit LogFast( udword precision = 11 );

           ~LogFast();
private:
            LogFast( const LogFast& );
   LogFast& operator=( const LogFast& );
public:

/// queries --------------------------------------------------------------------
           /** log2( number ). Number must be > 0.
             * Error (11): mean < 0.0002, max < 0.0004. */
           float two( float )                                             const;

           /** ln( number ). Number must be > 0.
             * Error (11): mean < 0.00009, max < 0.0003. */
           float e  ( float )                                             const;

           /** log10( number ). Number must be > 0.
             * Error (11): mean < 0.00004, max < 0.0002. */
           float ten( float )                                             const;

           udword precision()                                             const;

/// fields ---------------------------------------------------------------------
private:
   udword precision_m;
   float* pTable_m;
};


/// default instance
const LogFast& LOGFAST();








/// INLINES ///

/// rough ----------------------------------------------------------------------
inline
float logFastE
(
   const float f
)
{
   return logFast2( f ) * 0.69314718055995f;
}


inline
float logFast10
(
   const float f
)
{
   return logFast2( f ) * 0.30102999566398f;
}




/// adjustable -----------------------------------------------------------------
inline
float LogFast::e
(
   const float f
) const
{
   return two( f ) * 0.69314718055995f;
}


inline
float LogFast::ten
(
   const float f
) const
{
   return two( f ) * 0.30102999566398f;
}


inline
udword LogFast::precision() const
{
   return precision_m;
}

}//namespace


#endif//LogFast_h
