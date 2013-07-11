/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include "UnitVector3f.hpp"


using namespace hxa7241_graphics;




/// constants ------------------------------------------------------------------
const UnitVector3f& UnitVector3f::ZERO()
{
   static const UnitVector3f k( 0.0f, 0.0f, 0.0f );
   return k;
}


const UnitVector3f& UnitVector3f::X()
{
   static const UnitVector3f k( 1.0f, 0.0f, 0.0f );
   return k;
}


const UnitVector3f& UnitVector3f::Y()
{
   static const UnitVector3f k( 0.0f, 1.0f, 0.0f );
   return k;
}


const UnitVector3f& UnitVector3f::Z()
{
   static const UnitVector3f k( 0.0f, 0.0f, 1.0f );
   return k;
}
