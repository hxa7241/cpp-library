/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2004-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef RandomMwc2_h
#define RandomMwc2_h




#include "hxa7241_general.hpp"
namespace hxa7241_general
{


/**
 * Simple, fast, good random number generator (Multiply-with-carry).<br/><br/>
 *
 * Perhaps the fastest of any generator that passes the Diehard tests.<br/>
 * Twice as fast as RandomMwc1.
 * (Maybe about 1 every 10 CPU clock-cycles, ie. 100 million per second on a
 * 1GHz CPU (timed with a Pentium 3).)<br/><br/>
 *
 * Constant (sort-of: internally/non-semantically modifying).
 *
 * @implementation
 * Concatenation of following two 16-bit multiply-with-carry generators
 * x(n)=a*x(n-1)+carry mod 2^16 and y(n)=b*y(n-1)+carry mod 2^16, number and
 * carry packed within the same 32 bit integer. Algorithm recommended by
 * Marsaglia. Copyright (c) 2005, Glenn Rhoads.<br/><br/>
 *
 * <cite>http://web.archive.org/web/20050213041650/http://
 * paul.rutgers.edu/~rhoads/Code/code.html</cite>
 *
 * The first 16 and the 1000000th values are:
 * 1168299085
 *  520487819
 * 1761612921
 * 3632618539
 *  610669668
 * 2136514290
 * 3850311835
 * 2494138816
 * 3923280858
 * 1280618954
 *  309986706
 *  924303156
 * 2252542156
 * 1444019197
 * 2955985350
 * 1185139548
 *
 * 2289498853
*/
class RandomMwc2
{
/// standard object services ---------------------------------------------------
public:
   explicit RandomMwc2( udword seed = 0 );

// use defaults
//         ~RandomMwc2();
//          RandomMwc2( const RandomMwc2& );
private:
   RandomMwc2& operator=( const RandomMwc2& );
public:


/// commands -------------------------------------------------------------------


/// queries --------------------------------------------------------------------
           udword getUdword()                                             const;
           float  getFloat()                                              const;


/// fields ---------------------------------------------------------------------
private:
   mutable udword seeds_m[2];
};


}//namespace




#endif//RandomMwc2_h
