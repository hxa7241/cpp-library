/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef SobolSequence_h
#define SobolSequence_h




#include "hxa7241_graphics.hpp"
namespace hxa7241_graphics
{


#define SOBOL_MAX_DIMENSION 40
#define SOBOL_BIT_COUNT 30




/**
 * One dimension of a sobol sequence.<br/><br/>
 *
 * Only 40 dimensions implemented, sequence length ~ 1e+9.<br/><br/>
 *
 * @implementation
 * adapted from:
 * <cite>'GSL - GNU Scientific Library' v1.3, www.gnu.org/software/gsl/</cite>
 * The GSL code was used to generate a lookup table, and the code that executes
 * at runtime is a rewrite following a small part of the GSL code.
 */
class SobolSequence
{
/// standard object services ---------------------------------------------------
public:
            SobolSequence();
            /** @param dimension [0, SOBOL_MAX_DIMENSION-1] */
   explicit SobolSequence( dword dimension );

           ~SobolSequence();
            SobolSequence( const SobolSequence& );
   SobolSequence& operator=( const SobolSequence& );


/// commands -------------------------------------------------------------------
           /** @param dimension [0, SOBOL_MAX_DIMENSION-1] */
           void  setDimension( dword dimension );
           /** @param index value >= 0 */
           void  setIndex( dword index );

           void  next();


/// queries --------------------------------------------------------------------
           dword getDimension()                                           const;
           dword getIndex()                                               const;

           float getPoint()                                               const;


/// implementation -------------------------------------------------------------
protected:
           /** @return new point */
           float nextImplementation();


/// fields ---------------------------------------------------------------------
private:
   dword  dimension_m;

   dword  index_m;
   float  lastDenominatorInv_m;
   dword  lastNumerator_m;
   dword  vDirection_m[SOBOL_BIT_COUNT];

   float  point_m;

   /// constant precalculations
   static const dword V_DIRECTION[SOBOL_BIT_COUNT][SOBOL_MAX_DIMENSION];
};








/// INLINES ///

/// queries --------------------------------------------------------------------
inline
dword SobolSequence::getDimension() const
{
   return dimension_m;
}


inline
dword SobolSequence::getIndex() const
{
   return index_m;
}


inline
float SobolSequence::getPoint() const
{
   return point_m;
}


}//namespace




#endif//SobolSequence_h
