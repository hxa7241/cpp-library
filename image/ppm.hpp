/*------------------------------------------------------------------------------

   HXA7241 Image library.
   Copyright (c) 2005-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef ppm_h
#define ppm_h


#include <iosfwd>




#include "hxa7241_image.hpp"
namespace hxa7241_image
{
   using std::istream;
   using std::ostream;


/**
 * IO for the nice-and-simple PPM format.<br/><br/>
 *
 * PPMs are uncompressed, unadorned 24 or 48 bit RGB images.
 * (just: ID, then width and height and depth, then triples)
 * Easy to use and code for, or good just to hack up a quick image IO, for
 * testing etc.<br/><br/>
 *
 * Only one image per file is implemented here.<br/><br/>
 *
 * <cite>http://netpbm.sourceforge.net/doc/ppm.html</cite>
 */
namespace ppm
{
   enum EOrderingFlags
   {
      IS_TOP_FIRST = 1,
      IS_BGR       = 2
   };


   /**
    * Report whether the stream is a PPM file.
    */
   bool isRecognised
   (
      istream& i_inBytes,
      bool     i_isRewind = false
   );


   /**
    * Read PPM image.<br/><br/>
    *
    * Only first image read.<br/>
    * Triples are bottom row first, R then G then B.<br/>
    * width * height * 3 will be <= DWORD_MAX
    *
    * @i_orderingFlags combination of EOrderingFlags
    * @o_quantMax      max value of quantization, 1 to 65535
    * @o_pTriples      array of ubyte triples, or uword triples if
    *                  o_quantMax >= 256. orphaned storage
    *
    * @exceptions throws char[] message exceptions
    */
   void read
   (
      istream& i_inBytes,
      dword    i_orderingFlags,
      dword&   o_width,
      dword&   o_height,
      dword&   o_quantMax,
      void*&   o_pTriples
   );


   /**
    * Write PPM image.<br/><br/>
    *
    * Triples are bottom row first, R then G then B.<br/>
    * width * height * 3 must be <= DWORD_MAX
    *
    * @i_pComment      no newlines allowed
    * @i_quantMax      max value of quantization, 1 to 65535
    * @i_orderingFlags combination of EOrderingFlags
    * @i_pTriples      array of ubyte triples, or uword triples if
    *                  o_quantMax >= 256
    *
    * @exceptions throws char[] message exceptions
    */
   void write
   (
      const char* i_pComment,
      dword       i_width,
      dword       i_height,
      dword       i_quantMax,
      dword       i_orderingFlags,
      const void* i_pTriples,
      ostream&    o_outBytes
   );
}


}//namespace




#endif//ppm_h
