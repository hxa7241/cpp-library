/*------------------------------------------------------------------------------

   HXA7241 Image library.
   Copyright (c) 2005-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef rgbe_h
#define rgbe_h


#include <iosfwd>




#include "hxa7241_image.hpp"
namespace hxa7241_image
{
   using std::istream;
   using std::ostream;


/**
 * IO for the Radiance RGBE format.<br/><br/>
 *
 * <cite>the radiance fileformats doc:
 * http://radsite.lbl.gov/radiance/refer/filefmts.pdf</cite><br/>
 * <cite>'Real Pixels', Graphics Gems 2, AP 91; Ward.</cite>
 *
 * @implementation
 * read could be made more robust in two ways:
 * * fail partially instead of aborting all (simple):
 *   have an alternative top function that still catches exceptions but then
 *   returns a failure code (instead of deallocating image and rethrowing).
 * * run-length row isolation (moderate):
 *   detect when a row-run doesn't fit, then recover by finding the next
 *   row-leader and continuing.
 */
namespace rgbe
{
   enum EOrderingFlags
   {
      IS_LOW_TOP   = 1,
      IS_BGR       = 2
   };


   /**
    * Report whether the stream is a RGBE file.
    */
   bool isRecognised
   (
      istream& i_inBytes,
      bool     i_isRewind = false
   );


   /**
    * Read RGBE image.<br/><br/>
    *
    * Does not condition output values -- you get what is in file, valid or not
    * (except: width and height must be in range).<br/><br/>
    *
    * @o_pPrimaries8  chromaticities of RGB and white:
    *                 { rx, ry, gx, gy, bx, by, wx, wy }.
    *                 all 0 if not present, and should assume sRGB
    * @o_exposure     divide pixel values by this to get cd/m^2,
    *                 is 0 if not present.
    *
    * @exceptions throws char[] message and allocation exceptions
    */
   void read
   (
      istream& i_inBytes,
      dword    i_orderingFlags,
      dword&   o_width,
      dword&   o_height,
      float*   o_pPrimaries8,
      float&   o_exposure,
      float*&  o_pTriples
   );


   /**
    * Write RGBE image.<br/><br/>
    *
    * @i_pComment     no newlines allowed
    * @i_pPrimaries8  chromaticities of RGB and white:
    *                 { rx, ry, gx, gy, bx, by, wx, wy }
    *                 give 0 to disclude.
    * @i_exposure     amount to divide pixel values by to get cd/m^2.
    *                 give 0.0 to disclude.
    *
    * @exceptions throws char[] message and allocation exceptions
    */
   void write
   (
      const char*  i_pComment,
      dword        i_width,
      dword        i_height,
      const float* i_pPrimaries8,
      float        i_exposure,
      dword        i_orderingFlags,
      const float* i_pTriples,
      ostream&     o_outBytes
   );
}


}//namespace




#endif//rgbe_h
