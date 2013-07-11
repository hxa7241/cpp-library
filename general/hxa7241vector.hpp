/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef hxa7241vector_h
#define hxa7241vector_h


#include <vector>




#include "hxa7241_general.hpp"
namespace hxa7241_general
{


/**
 * Some functions to manipulate std::vector storage.
 */


/**
 * Deallocates a std::vector reserve.
 */
template<class TYPE>
void rezero
(
   std::vector<TYPE>& v
)
{
   if( v.capacity() != 0 )
   {
      std::vector<TYPE> tmp;
      v.swap( tmp );
   }
}


/**
 * Makes a std::vector reserve equal its size.
 */
template<class TYPE>
void unreserve
(
   std::vector<TYPE>& v
)
{
   if( v.capacity() != v.size() )
   {
      std::vector<TYPE> tmp( v );
      v.swap( tmp );
   }
}


/**
 * Makes a std::vector size and reserve to the same specified amount.
 */
template<class TYPE>
void resizeReserve
(
   std::vector<TYPE>& v,
   int                size
)
{
   if( size < 0 )
   {
      size = 0;
   }

   if( v.capacity() != static_cast<unsigned>(size) )
   {
      std::vector<TYPE> tmp;
      tmp.reserve( size );

      if( v.size() <= static_cast<unsigned>(size) )
      {
         tmp = v;
         tmp.resize( size );
      }
      else
      {
         v.resize( size );
         tmp = v;
      }

      v.swap( tmp );
   }
   else
   {
      v.resize( size );
   }
}


/**
 * Makes a std::vector empty with the specified reserve.
 */
template<class TYPE>
void reserveExactClear
(
   std::vector<TYPE>& v,
   int                size
)
{
   if( size < 0 )
   {
      size = 0;
   }

   if( v.capacity() != static_cast<unsigned>(size) )
   {
      std::vector<TYPE> tmp;
      tmp.reserve( size );
      v.swap( tmp );
   }
   else
   {
      v.clear();
   }
}


/**
 * Assigns to a std::vector and makes the reserve equal its size.
 */
template<class TYPE>
void assignUnreserve
(
   const std::vector<TYPE>& other,
   std::vector<TYPE>&       v
)
{
   if( v.capacity() != other.size() )
   {
      std::vector<TYPE> tmp( other );
      v.swap( tmp );
   }
   else
   {
      v = other;
   }
}


}//namespace




#endif//hxa7241vector_h
