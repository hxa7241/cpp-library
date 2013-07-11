/*------------------------------------------------------------------------------

   HXA7241 Image library.
   Copyright (c) 2005-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef StreamExceptionSet_h
#define StreamExceptionSet_h


#include <istream>




#include "hxa7241_image.hpp"
namespace hxa7241_image
{


class StreamExceptionSet
{
public:
   StreamExceptionSet( std::basic_ios<char>&  stream,
                       std::ios_base::iostate flags )
    : pStream_m      ( &stream )
    , previousFlags_m( stream.exceptions() )
   {
      stream.exceptions( flags );
   }

   ~StreamExceptionSet()
   {
      pStream_m->exceptions( previousFlags_m );
   }

private:
   StreamExceptionSet( const StreamExceptionSet& );
   StreamExceptionSet& operator=( const StreamExceptionSet& );
public:

   std::basic_ios<char>*  pStream_m;
   std::ios_base::iostate previousFlags_m;
};


}//namespace




#endif//StreamExceptionSet_h
