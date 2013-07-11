/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2004-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef RandomMwc1_h
#define RandomMwc1_h




#include "hxa7241_general.hpp"
namespace hxa7241_general
{


/**
 * Simple, fast, good random number generator (Multiply-with-carry).<br/><br/>
 *
 * Perhaps the fastest of any generator that passes the Diehard tests.<br/>
 * Half as fast as RandomMwc2.<br/><br/>
 *
 * Constant (sort-of: internally/non-semantically modifying).
 *
 * @implementation
 * multiply-with-carry generator x{n} = a*x{n-1} + carry mod 2^32.
 * period = (a*2^31)-1 (approx. between 2e+18 and 5e+18). Algorithm recommended
 * by Marsaglia. Copyright (c) 2005, Glenn Rhoads.<br/><br/>
 *
 * <cite>http://web.archive.org/web/20050213041650/http://
 * paul.rutgers.edu/~rhoads/Code/code.html</cite>
 *
 * The first 16 and the 1000000th values are:
 *  266434319
 * 1184179474
 * 3729546975
 * 1269245813
 * 3172582254
 *  395758859
 *  156028346
 *   20773141
 * 1703483041
 * 1308543988
 * 2416731867
 * 3626613104
 * 1283819493
 *  912189592
 * 3799492147
 *   75739638
 *
 *  547770101
 */
class RandomMwc1
{
/// standard object services ---------------------------------------------------
public:
   explicit RandomMwc1( udword seed = 0 );

// use defaults
//         ~RandomMwc1();
//          RandomMwc1( const RandomMwc1& );
private:
   RandomMwc1& operator=( const RandomMwc1& );
public:


/// commands -------------------------------------------------------------------


/// queries --------------------------------------------------------------------
           udword getUdword()                                             const;
           float  getFloat()                                              const;


/// fields ---------------------------------------------------------------------
private:
   mutable udword x_m;
   mutable udword c_m;
};


}//namespace




#endif//RandomMwc1_h
