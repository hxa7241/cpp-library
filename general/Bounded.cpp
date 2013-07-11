/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include "Bounded.hpp"


using namespace hxa7241_general;




/// queries --------------------------------------------------------------------
template<class _TYPE, dword _MIN, dword _MAX>
_TYPE Bounded<_TYPE, _MIN, _MAX>::clamp
(
   _TYPE v
)
{
   if( f < static_cast<_TYPE>(_MIN) )
   {
      f = static_cast<_TYPE>(_MIN);
   }
   else if( f > static_cast<_TYPE>(_MAX) )
   {
      f = static_cast<_TYPE>(_MAX);
   }

   return f;
}
