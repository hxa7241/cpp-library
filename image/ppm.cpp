/*------------------------------------------------------------------------------

   HXA7241 Image library.
   Copyright (c) 2005-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include <stdlib.h>
#include <istream>
#include <ostream>
#include <string>

#include "PixelsPtr.hpp"
#include "StreamExceptionSet.hpp"

#include "ppm.hpp"


using namespace hxa7241_image;




/*

( From: http://netpbm.sourceforge.net/doc/ppm.html )

Each PPM image consists of the following:

* A "magic number" for identifying the file type. A PPM image's magic number is
  the two characters "P6".
* Whitespace (blanks, TABs, CRs, LFs).
* A width, formatted as ASCII characters in decimal.
* Whitespace.
* A height, again in ASCII decimal.
* Whitespace.
* The maximum color value (Maxval), again in ASCII decimal. Must be less than
  65536 and more than zero.
* Newline or other single whitespace character.
* A raster of Height rows, in order from top to bottom. Each row consists of
  Width pixels, in order from left to right. Each pixel is a triplet of red,
  green, and blue samples, in that order. Each sample is represented in pure
  binary by either 1 or 2 bytes. If the Maxval is less than 256, it is 1 byte.
  Otherwise, it is 2 bytes. The most significant byte is first.
  A row of an image is horizontal. A column is vertical. The pixels in the image
  are square and contiguous.
* In the raster, the sample values are "nonlinear." They are proportional to the
  intensity of the ITU-R Recommendation BT.709 red, green, and blue in the
  pixel, adjusted by the BT.709 gamma transfer function. (That transfer function
  specifies a gamma number of 2.2 and has a linear section for small
  intensities). A value of Maxval for all three samples represents CIE D65 white
  and the most intense color in the color universe of which the image is part
  (the color universe is all the colors in all images to which this image might
  be compared).
  ITU-R Recommendation BT.709 is a renaming of the former CCIR Recommendation
  709. When CCIR was absorbed into its parent organization, the ITU, ca. 2000,
  the standard was renamed. This document once referred to the standard as CIE
  Rec. 709, but it isn't clear now that CIE ever sponsored such a standard.
  Note that another popular color space is the newer sRGB. A common variation on
  PPM is to subsitute this color space for the one specified.
* Note that a common variation on the PPM format is to have the sample values be
  "linear," i.e. as specified above except without the gamma adjustment.
  pnmgamma takes such a PPM variant as input and produces a true PPM as output.
* Characters from a "#" to the next end-of-line, before the maxval line, are
  comments and are ignored.

*/




namespace
{

/// constants ------------------------------------------------------------------

const char PPM_ID[] = "P6";

const char IN_FORMAT_EXCEPTION_MESSAGE[] =
   "unrecognized file format, in PPM read";
const char IN_DIMENSIONS_EXCEPTION_MESSAGE[] =
   "image dimensions invalid, in PPM read";
const char IN_STREAM_EXCEPTION_MESSAGE[] =
   "stream read failure, in PPM read";
const char OUT_DIMENSIONS_EXCEPTION_MESSAGE[] =
   "image dimensions invalid, in PPM write";
const char OUT_NULL_POINTER_EXCEPTION_MESSAGE[] =
   "pixel pointer null, in PPM write";
const char OUT_STREAM_EXCEPTION_MESSAGE[] =
   "stream write failure, in PPM write";


void checkDimensions
(
   dword       width,
   dword       height,
   dword       maxval,
   const char* pMessage
);

}




bool hxa7241_image::ppm::isRecognised
(
   istream&   i_in,
   const bool i_isRewind
)
{
   // enable stream exceptions
   StreamExceptionSet streamExceptionSet( i_in,
      istream::badbit | istream::failbit | istream::eofbit );

   try
   {
      // save stream start position
      const istream::pos_type streamStartPos = i_in.tellg();

      // read id
      char id[] = "..";
      i_in >> std::noskipws >> id[0] >> id[1] >> std::skipws;

      // rewind stream
      if( i_isRewind )
      {
         i_in.seekg( streamStartPos );
      }

      // compare id
      return std::string(id) == PPM_ID;
   }
   // translate exceptions
   catch( const std::ios_base::failure& )
   {
      throw IN_STREAM_EXCEPTION_MESSAGE;
   }
}




void hxa7241_image::ppm::read
(
   istream&    i_in,
   const dword i_orderingFlags,
   dword&      o_width,
   dword&      o_height,
   dword&      o_quantMax,
   void*&      o_pTriples
)
{
   // enable stream exceptions
   StreamExceptionSet streamExceptionSet( i_in,
      istream::badbit | istream::failbit | istream::eofbit );

   try
   {
      // read and check id
      if( !isRecognised( i_in ) )
      {
         throw IN_FORMAT_EXCEPTION_MESSAGE;
      }

      // read width, height, maxval
      dword width;
      dword height;
      dword maxval;
      {
         dword ints[3];
         for( int i = 0;  i < 3;  ++i )
         {
            // skip blanks and comments
            for( char c = '#';  '#' == c; )
            {
               // read chars until non blank
               (i_in >> c).unget();

               // if comment-start then read chars until after newline
               if( '#' == c )
               {
                  i_in.ignore( INT_MAX, '\n' );
               }
            }

            // read next token (non-blank chunk)
            std::string s;
            i_in >> s;

            // convert token to integer
            ints[i] = ::atoi( s.c_str() );
         }
         // read single blank
         i_in.ignore();

         width  = ints[0];
         height = ints[1];
         maxval = ints[2];
      }

      // check validity
      checkDimensions( width, height, maxval, IN_DIMENSIONS_EXCEPTION_MESSAGE );

      // allocate storage
      PixelsPtr pTriples( (maxval >= 256), (width * height * 3) );

      // read rows (top first)
      for( dword y = 0;  y < height;  ++y )
      {
         // read pixels (left first)
         for( dword x = 0;  x < width;  ++x )
         {
            const dword row    = (i_orderingFlags & IS_TOP_FIRST) ?
               y : height - 1 - y;
            const dword offset = ((row * width) + x) * 3;

            // read channels (R then G then B)
            for( dword c = 0;  c < 3;  ++c )
            {
               const dword i = (i_orderingFlags & IS_BGR) ? 2 - c : c;

               // read value
               // 24 bit
               if( !pTriples.is48Bit() )
               {
                  char* pTriple = static_cast<char*>(pTriples.get()) + offset;

                  i_in.read( pTriple + i, 1 );
               }
               // 48 bit
               else
               {
                  uword* pTriple = static_cast<uword*>(pTriples.get()) + offset;

                  // most significant byte first
                  char b[2];
                  i_in.read( b, 2 );
                  pTriple[i] = static_cast<uword>( (static_cast<ubyte>(b[0]) <<
                     8) | static_cast<ubyte>(b[1]) );
               }
            }
         }
      }

      // set outputs (now that no exceptions can happen)
      o_width    = width;
      o_height   = height;
      o_quantMax = maxval;
      o_pTriples = pTriples.release();
   }
   // translate exceptions
   catch( const std::ios_base::failure& )
   {
      throw IN_STREAM_EXCEPTION_MESSAGE;
   }
}




void hxa7241_image::ppm::write
(
   const char* i_pComment,
   const dword i_width,
   const dword i_height,
   const dword i_quantMax,
   const dword i_orderingFlags,
   const void* i_pTriples,
   ostream&    o_out
)
{
   // check preconditions
   checkDimensions( i_width, i_height, i_quantMax,
      OUT_DIMENSIONS_EXCEPTION_MESSAGE );
   if( !i_pTriples )
   {
      throw OUT_NULL_POINTER_EXCEPTION_MESSAGE;
   }

   // enable stream exceptions
   StreamExceptionSet streamExceptionSet( o_out,
      ostream::badbit | ostream::failbit | ostream::eofbit );

   try
   {
      // write header
      {
         // ID
         o_out << PPM_ID << '\n';

         // comment
         if( i_pComment )
         {
            // max length and truncate at first newline
            const std::string comment( std::string(i_pComment).substr(0, 128) );
            const udword pn = comment.find_first_of( "\n\r" );

            o_out << "# " << comment.substr( 0, pn ) << '\n';
         }

         // width, height, maxval
         o_out << i_width <<  ' ' << i_height << '\n';
         o_out << i_quantMax << '\n';
      }

      // write rows (top first)
      for( dword y = i_height;  y-- > 0; )
      {
         // write pixels (left first)
         for( dword x = 0;  x < i_width;  ++x )
         {
            const dword row    = (i_orderingFlags & IS_TOP_FIRST) ?
               i_height - 1 - y : y;
            const dword offset = ((row * i_width) + x) * 3;

            // write channels (R then G then B)
            for( dword c = 0;  c < 3;  ++c )
            {
               const dword i = (i_orderingFlags & IS_BGR) ? 2 - c : c;

               // write value
               // 24 bit
               if( i_quantMax <= 255 )
               {
                  const ubyte* pTriple =
                     static_cast<const ubyte*>(i_pTriples) + offset;

                  o_out << pTriple[i];
               }
               // 48 bit
               else
               {
                  const uword* pTriple =
                     static_cast<const uword*>(i_pTriples) + offset;

                  // most significant byte first
                  o_out << static_cast<ubyte>((pTriple[i] & 0xFF00) >> 8);
                  o_out << static_cast<ubyte>( pTriple[i] & 0xFF);
               }
            }
         }
      }
   }
   // translate exceptions
   catch( const std::ios_base::failure& )
   {
      throw OUT_STREAM_EXCEPTION_MESSAGE;
   }
}




/// implementation -------------------------------------------------------------
namespace
{

void checkDimensions
(
   const dword width,
   const dword height,
   const dword maxval,
   const char* pMessage
)
{
   if( (maxval < 1) || (maxval > 65535) || (width <= 0) || (height <= 0) ||
      (height > (DWORD_MAX / 3)) || (width > (DWORD_MAX / (height * 3))) )
   {
      throw pMessage;
   }
}

}








/// test -----------------------------------------------------------------------
#ifdef TESTING


#include <fstream>
#include <sstream>


namespace hxa7241_image
{
namespace ppm
{
   using namespace hxa7241;

   bool test_read24Bit
   (
      std::ostream* pOut,
      const bool    isVerbose,
      const dword   seed
   );

   bool test_read48Bit
   (
      std::ostream* pOut,
      const bool    isVerbose,
      const dword   seed
   );

   bool test_write24Bit
   (
      std::ostream* pOut,
      const bool    isVerbose,
      const dword   seed
   );

   bool test_write48Bit
   (
      std::ostream* pOut,
      const bool    isVerbose,
      const dword   seed
   );

   void writeImageFiles
   (
      std::ostream* pOut,
      const bool    isVerbose,
      const dword   seed
   );


void deleteTriples
(
   const bool is48Bit,
   void*      pTriples
)
{
   if( !is48Bit )
   {
      delete[] static_cast<ubyte*>(pTriples);
   }
   else
   {
      delete[] static_cast<uword*>(pTriples);
   }
}


bool test_ppm
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   seed
)
{
   bool isOk = true;

   if( pOut ) *pOut << "[ test_ppm ]\n\n";


   isOk &= test_read24Bit( pOut, isVerbose, seed );
   isOk &= test_read48Bit( pOut, isVerbose, seed );

   isOk &= test_write24Bit( pOut, isVerbose, seed );
   isOk &= test_write48Bit( pOut, isVerbose, seed );

   //writeImageFiles( pOut, isVerbose, seed );


   if( pOut ) *pOut << (isOk ? "--- successfully" : "*** failurefully") <<
      " completed " << "\n\n\n";

   if( pOut ) pOut->flush();


   return isOk;
}


bool test_read24Bit
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   //seed
)
{
   bool isOk = true;

   // 3 width, 2 height, 255 maxval
   //
   // 50 36 0A
   // 23 20 68 74 74 70 3A 2F 2F 77 77 77 2E 68 78 61 37 32 34 31 2E 6F 72 67 2F 0A
   // 0A
   // 33 20 32 0A
   // 32 35 35 0A
   // 0A 0A 0A 64 64 64 C8 C8 C8 FF 00 00 00 FF 00 00 00 FF
   const udword fileBytes_[] = {
      0x50, 0x36, 0x0A,
      0x23, 0x20, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x77, 0x77, 0x77, 0x2E, 0x68, 0x78, 0x61, 0x37, 0x32, 0x34, 0x31, 0x2E, 0x6F, 0x72, 0x67, 0x2F, 0x0A,
      0x0A,
      0x33, 0x20, 0x32, 0x0A,
      0x32, 0x35, 0x35, 0x0A,
      0x0A, 0x0A, 0x0A, 0x64, 0x64, 0x64, 0xC8, 0xC8, 0xC8, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF };
   // (avoids lots of warnings/casts)
   char fileBytes[ sizeof(fileBytes_)/sizeof(fileBytes_[0]) ];
   for( int i = sizeof(fileBytes_)/sizeof(fileBytes_[0]);  i-- > 0; )
   {
      fileBytes[i] = static_cast<ubyte>(fileBytes_[i]);
   }

   std::istringstream in24( std::string(fileBytes, sizeof(fileBytes)/sizeof(fileBytes[0])),
      std::istringstream::binary );

   dword width    = 0;
   dword height   = 0;
   dword quantMax = 0;
   void* pTriples = 0;
   hxa7241_image::ppm::read( in24, IS_TOP_FIRST, width, height, quantMax, pTriples );

   isOk &= (3 == width) & (2 == height) & (255 == quantMax);

   try
   {
      const ubyte pixels[] = {
         ubyte(10), ubyte(10), ubyte(10),   ubyte(100), ubyte(100), ubyte(100),   ubyte(-56), ubyte(-56), ubyte(-56),
         ubyte(-1), ubyte(0),  ubyte(0),    ubyte(0),   ubyte(-1),  ubyte(0),     ubyte(0),   ubyte(0),   ubyte(-1) };

      for( dword y = height;  y-- > 0; )
      {
         for( dword x = 0;  x < width;  ++x )
         {
            const dword offset = ((y * width) + x) * 3;

            if( pOut && isVerbose ) *pOut << "( ";
            for( dword c = 0;  c < 3;  ++c )
            {
               const udword channel = static_cast<udword>( (quantMax <= 255) ?
                  static_cast<const ubyte*>(pTriples)[offset + c] :
                  static_cast<const uword*>(pTriples)[offset + c] );

               if( pOut && isVerbose ) *pOut << channel << " ";

               isOk &= (channel == static_cast<udword>(pixels[offset + c]));
            }
            if( pOut && isVerbose ) *pOut << ")  ";
         }
         if( pOut && isVerbose ) *pOut << "\n";
      }

      deleteTriples( quantMax >= 256, pTriples );
   }
   catch( ... )
   {
      deleteTriples( quantMax >= 256, pTriples );
      throw;
   }

   if( pOut && isVerbose ) *pOut << "\n";

   if( pOut ) *pOut << "read 24bit : " <<
      (isOk ? "--- succeeded" : "*** failed") << "\n\n";

   return isOk;
}


bool test_read48Bit
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   //seed
)
{
   bool isOk = true;

   // 3 width, 2 height, 65535 maxval
   //
   // 50 36 0A
   // 23 20 68 74 74 70 3A 2F 2F 77 77 77 2E 68 78 61 37 32 34 31 2E 6F 72 67 2F 0A
   // 0A
   // 33 20 32 0A
   // 36 35 35 33 35 0A
   // FF 00 00 00 00 00 00 00 FF 00 00 00 00 00 00 00 FF 00 0A 00 0A 00 0A 00 64 00 64 00 64 00 C8 00 C8 00 C8 00
   const udword fileBytes_[] = {
      0x50, 0x36, 0x0A,
      0x23, 0x20, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x77, 0x77, 0x77, 0x2E, 0x68, 0x78, 0x61, 0x37, 0x32, 0x34, 0x31, 0x2E, 0x6F, 0x72, 0x67, 0x2F, 0x0A,
      0x0A,
      0x33, 0x20, 0x32, 0x0A,
      0x36, 0x35, 0x35, 0x33, 0x35, 0x0A,
      0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x0A, 0x00, 0x0A, 0x00, 0x0A, 0x00, 0x64, 0x00, 0x64, 0x00, 0x64, 0x00, 0xC8, 0x00, 0xC8, 0x00, 0xC8, 0x00 };
   // (avoids lots of warnings/casts)
   char fileBytes[ sizeof(fileBytes_)/sizeof(fileBytes_[0]) ];
   for( int i = sizeof(fileBytes_)/sizeof(fileBytes_[0]);  i-- > 0; )
   {
      fileBytes[i] = static_cast<ubyte>(fileBytes_[i]);
   }

   std::istringstream in48( std::string(fileBytes, sizeof(fileBytes)/sizeof(fileBytes[0])),
      std::istringstream::binary );

   dword width    = 0;
   dword height   = 0;
   dword quantMax = 0;
   void* pTriples = 0;
   hxa7241_image::ppm::read( in48, IS_TOP_FIRST, width, height, quantMax, pTriples );

   isOk &= (3 == width) & (2 == height) & (65535 == quantMax);

   try
   {
      const uword pixelsW[] = {
         uword(-256),   uword(0),      uword(0),        uword(0),      uword(-256),   uword(0),        uword(0),      uword(0),      uword(-256),
         uword(0x0A00), uword(0x0A00), uword(0x0A00),   uword(0x6400), uword(0x6400), uword(0x6400),   uword(-14336), uword(-14336), uword(-14336) };

      for( dword y = height;  y-- > 0; )
      {
         for( dword x = 0;  x < width;  ++x )
         {
            const dword offset = ((y * width) + x) * 3;

            if( pOut && isVerbose ) *pOut << "( ";
            for( dword c = 0;  c < 3;  ++c )
            {
               const udword channel = static_cast<udword>( (quantMax <= 255) ?
                  static_cast<const ubyte*>(pTriples)[offset + c] :
                  static_cast<const uword*>(pTriples)[offset + c] );

               if( pOut && isVerbose ) *pOut << channel << " ";

               isOk &= (channel == static_cast<udword>(pixelsW[offset + c]));
            }
            if( pOut && isVerbose ) *pOut << ")  ";
         }
         if( pOut && isVerbose ) *pOut << "\n";
      }

      deleteTriples( quantMax >= 256, pTriples );
   }
   catch( ... )
   {
      deleteTriples( quantMax >= 256, pTriples );
      throw;
   }

   if( pOut && isVerbose ) *pOut << "\n";

   if( pOut ) *pOut << "read 48bit : " <<
      (isOk ? "--- succeeded" : "*** failed") << "\n\n";

   return isOk;
}


bool test_write24Bit
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   //seed
)
{
   bool isOk = true;

   const byte pixels[] = {
      byte(-1), byte(0), byte(0),   byte(0), byte(-1), byte(0),   byte(0), byte(0), byte(-1),
      byte(10), byte(10), byte(10),   byte(100), byte(100), byte(100),   byte(-56), byte(-56), byte(-56) };

   //std::ofstream out24f( "zzztest24.ppm", std::ofstream::binary );
   //hxa7241_image::ppm::write( 3, 2, false, 0, pixels, out24f );

   std::ostringstream out24( std::ostringstream::binary );

   hxa7241_image::ppm::write( "http://www.hxa7241.org/\n", 3, 2, 255, 0, pixels, out24 );

   // should result in a file consisting of these hex values:
   //
   // 50 36 0A
   // 23 20 68 74 74 70 3A 2F 2F 77 77 77 2E 68 78 61 37 32 34 31 2E 6F 72 67 2F 0A
   // 33 20 32 0A
   // 32 35 35 0A
   // 0A 0A 0A 64 64 64 C8 C8 C8 FF 00 00 00 FF 00 00 00 FF
   const udword correct[] = {
      0x50, 0x36, 0x0A,
      0x23, 0x20, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x77, 0x77, 0x77, 0x2E, 0x68, 0x78, 0x61, 0x37, 0x32, 0x34, 0x31, 0x2E, 0x6F, 0x72, 0x67, 0x2F, 0x0A,
      0x33, 0x20, 0x32, 0x0A,
      0x32, 0x35, 0x35, 0x0A,
      0x0A, 0x0A, 0x0A, 0x64, 0x64, 0x64, 0xC8, 0xC8, 0xC8, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF };

   bool isFail = false;
   std::string buf( out24.str() );
   isFail |= (buf.size() != sizeof(correct)/sizeof(correct[0]));
   {
      if( pOut && isVerbose ) std::hex( *pOut );
      if( pOut && isVerbose ) std::uppercase( *pOut );
      for( dword i = 0;  i < dword(sizeof(correct)/sizeof(correct[0]));  ++i )
      {
         if( udword(i) < buf.size() )
         {
            const udword value = udword(ubyte(buf[i]));
            const bool isMatch = (correct[i] == value);
            if( pOut && isVerbose ) *pOut << (isMatch ? "" : "*") <<
               (value < 16 ? "0" : "") << value << ((i & 0x1F) == 0x1F ? "\n" : " ");
            isFail |= !isMatch;
         }
         else
         {
            if( pOut && isVerbose ) *pOut << '.';
         }
      }
      if( pOut && isVerbose ) std::nouppercase( *pOut );
      if( pOut && isVerbose ) std::dec( *pOut );
      if( pOut && isVerbose ) *pOut << "\n";
   }

   if( pOut && isVerbose ) *pOut << "\n";

   if( pOut ) *pOut << "write 24bit : " <<
      (!isFail ? "--- succeeded" : "*** failed") << "\n\n";
   isOk &= !isFail;

   return isOk;
}


bool test_write48Bit
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   //seed
)
{
   bool isOk = true;

   const word pixelsW[] = {
      word(-256), word(0), word(0),   word(0), word(-256), word(0),   word(0), word(0), word(-256),
      word(0x0A00), word(0x0A00), word(0x0A00),   word(0x6400), word(0x6400), word(0x6400),   word(-14336), word(-14336), word(-14336) };

   //std::ofstream out48f( "zzztest48.ppm", std::ofstream::binary );
   //hxa7241_image::ppm::write( 3, 2, true, IS_TOP_FIRST, pixelsW, out48f );

   std::ostringstream out48( std::ostringstream::binary );

   hxa7241_image::ppm::write( "http://www.hxa7241.org/\n", 3, 2, 65535, IS_TOP_FIRST, pixelsW, out48 );

   // should result in a file consisting of these hex values:
   //
   // 50 36 0A
   // 23 20 68 74 74 70 3A 2F 2F 77 77 77 2E 68 78 61 37 32 34 31 2E 6F 72 67 2F 0A
   // 33 20 32 0A
   // 36 35 35 33 35 0A
   // 0A 00 0A 00 0A 00 64 00 64 00 64 00 C8 00 C8 00 C8 00 FF 00 00 00 00 00 00 00 FF 00 00 00 00 00 00 00 FF 00
   const udword correct[] = {
      0x50, 0x36, 0x0A,
      0x23, 0x20, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x77, 0x77, 0x77, 0x2E, 0x68, 0x78, 0x61, 0x37, 0x32, 0x34, 0x31, 0x2E, 0x6F, 0x72, 0x67, 0x2F, 0x0A,
      0x33, 0x20, 0x32, 0x0A,
      0x36, 0x35, 0x35, 0x33, 0x35, 0x0A,
      0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x0A, 0x00, 0x0A, 0x00, 0x0A, 0x00, 0x64, 0x00, 0x64, 0x00, 0x64, 0x00, 0xC8, 0x00, 0xC8, 0x00, 0xC8, 0x00 };

   bool isFail = false;
   std::string buf( out48.str() );
   isFail |= (buf.size() != sizeof(correct)/sizeof(correct[0]));
   {
      if( pOut && isVerbose ) std::hex( *pOut );
      if( pOut && isVerbose ) std::uppercase( *pOut );
      for( dword i = 0;  i < dword(sizeof(correct)/sizeof(correct[0]));  ++i )
      {
         if( udword(i) < buf.size() )
         {
            const udword value = udword(ubyte(buf[i]));
            const bool isMatch = (correct[i] == value);
            if( pOut && isVerbose ) *pOut << (isMatch ? "" : "*") <<
               (value < 16 ? "0" : "") << value << ((i & 0x1F) == 0x1F ? "\n" : " ");
            isFail |= !isMatch;
         }
         else
         {
            if( pOut && isVerbose ) *pOut << '.';
         }
      }
      if( pOut && isVerbose ) std::nouppercase( *pOut );
      if( pOut && isVerbose ) std::dec( *pOut );
      if( pOut && isVerbose ) *pOut << "\n";
   }

   if( pOut && isVerbose ) *pOut << "\n";

   if( pOut ) *pOut << "write 48bit : " <<
      (!isFail ? "--- succeeded" : "*** failed") << "\n\n";
   isOk &= !isFail;

   return isOk;
}


void writeImageFiles
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   //seed
)
{
   // write image files -- for manual inspection
   {
      for( dword i = 0;  i < 2;  ++i )
      {
         // make pixels
         const dword wpow    = 8;   // >= 2
         const dword width   = 1 << wpow;
         const dword height  = width / 2;
         const bool  is48Bit = i & 1;

         ubyte* pPixels = new ubyte[ (width * height * 3) << dword(is48Bit) ];

         for( dword h = height;  h-- > 0; )
         {
            for( dword w = width;  w-- > 0; )
            {
               const dword column = (w >> (wpow - 2)) & 0x03;
               const float row    = float(h) / float(height - 1);
               const dword offset = (w + (h * width)) * 3;

               for( dword p = 3;  p-- > 0; )
               {
                  if( !is48Bit )
                  {
                     const ubyte value = ubyte( row * float(ubyte(-1)) );

                     pPixels[ offset + p ] =
                        ((p == column) | (3 == column)) ? value : 0;
                  }
                  else
                  {
                     const uword value = uword( row * float(uword(-1)) );

                     reinterpret_cast<uword*>(pPixels)[ offset + p ] =
                        ((p == column) | (3 == column)) ? value : 0;
                  }
               }
            }
         }

         // write image
         std::ofstream outf( !is48Bit ? "zzztest24.ppm" : "zzztest48.ppm",
            std::ofstream::binary );

         try
         {
            hxa7241_image::ppm::write( "http://www.hxa7241.org/\n",
               width, height, is48Bit, 0, pPixels, outf );

            if( pOut && isVerbose ) *pOut << "wrote image to " <<
               (!is48Bit ? "zzztest24.ppm" : "zzztest48.ppm") << "\n";
         }
         catch( ... )
         {
            delete[] pPixels;

            throw;
         }

         delete[] pPixels;
      }

      if( pOut && isVerbose ) *pOut << "\n";
   }
}


}//namespace
}//namespace


#endif//TESTING
