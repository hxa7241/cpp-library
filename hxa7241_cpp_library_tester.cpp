/*------------------------------------------------------------------------------

   HXA7241 C++ library.
   Copyright (c) 2004-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include <exception>
#include <string>
#include <iostream>

#include "Primitives.hpp"




using namespace hxa7241;




// constants -------------------------------------------------------------------
namespace
{
   const char BANNER_MESSAGE[] =
      "\n  hxa7241_cpp_library tester 20071226 http://www.hxa7241.org/\n";
   const char HELP_MESSAGE[] =
   "\n"
   "usage:\n"
   "  tester [-t...] [-o...] [-s...]\n"
   "\n"
   "switches:\n"
   "  -t<int>        which test: 1 to 14, or 0 for all\n"
   "  -o<0 | 1 | 2>  set output level: 0 = none, 1 = summaries, 2 = verbose\n"
   "  -s<32bit int>  set random seed\n"
   "\n";

   const char EXCEPTION_PREFIX[]   = "*** execution failed:  ";
   const char EXCEPTION_ABSTRACT[] = "no annotation for cause";
}




// implementation declarations -------------------------------------------------
static bool test
(
   int   argc,
   char* argv[]
);

static void readParameters
(
   const int  argc,
   char*const argv[],
   udword&    whichTest,
   dword&     outputLevel,
   dword&     seed
);




/// entry point ////////////////////////////////////////////////////////////////
int main
(
   int   argc,
   char* argv[]
)
{
   int returnValue = EXIT_FAILURE;

   // catch everything
   try
   {
      std::cout << BANNER_MESSAGE;

      // check for help request
      if( (argc <= 1) || (std::string(argv[1]) == "-?") ||
         (std::string(argv[1]) == "--help") )
      {
         std::cout << HELP_MESSAGE;
      }
      // execute
      else
      {
         returnValue = test( argc, argv ) ? EXIT_SUCCESS : EXIT_FAILURE;
      }
   }
   // print exception message
   catch( const std::exception& e )
   {
      std::cout << '\n' << EXCEPTION_PREFIX << e.what() << '\n';
   }
   catch( const char*const pExceptionString )
   {
      std::cout << '\n' << EXCEPTION_PREFIX << pExceptionString << '\n';
   }
   catch( const std::string exceptionString )
   {
      std::cout << '\n' << EXCEPTION_PREFIX << exceptionString << '\n';
   }
   catch( ... )
   {
      std::cout << '\n' << EXCEPTION_PREFIX << EXCEPTION_ABSTRACT << '\n';
   }

   try
   {
      std::cout.flush();
   }
   catch( ... )
   {
      // suppress exceptions
   }

   return returnValue;
}








/// implementation -------------------------------------------------------------

/// unit test declarations
namespace hxa7241_general
{
   bool test_Array( std::ostream* pOut, bool isVerbose, dword seed );
   bool test_FpToInt( std::ostream* pOut, bool isVerbose, dword seed );
   bool test_LogFast( std::ostream* pOut, bool isVerbose, dword seed );
   bool test_PowFast( std::ostream* pOut, bool isVerbose, dword seed );
   bool test_Sheet( std::ostream* pOut, bool isVerbose, dword seed );
}

namespace hxa7241_graphics
{
   bool test_ColorSpace( std::ostream* pOut, bool isVerbose, dword seed );
   bool test_Filters( std::ostream* pOut, bool isVerbose, dword seed );
   bool test_Matrix3f( std::ostream* pOut, bool isVerbose, dword seed );
   bool test_Polar( std::ostream* pOut, bool isVerbose, dword seed );
   bool test_Quaternion( std::ostream* pOut, bool isVerbose, dword seed );
   bool test_Spherahedron( std::ostream* pOut, bool isVerbose, dword seed );
}

namespace hxa7241_image
{
   namespace quantizing
   {
      bool test_quantizing( std::ostream* pOut, bool isVerbose, dword seed );
   }
   namespace ppm
   {
      bool test_ppm( std::ostream* pOut, bool isVerbose, dword seed );
   }
   namespace rgbe
   {
      bool test_rgbe( std::ostream* pOut, bool isVerbose, dword seed );
   }
}


/// unit test caller
static bool (*TESTERS[])(std::ostream*, bool, dword) =
{
   &hxa7241_general::test_Array                  //  1
,  &hxa7241_general::test_FpToInt                //  2
,  &hxa7241_general::test_LogFast                //  3
,  &hxa7241_general::test_PowFast                //  4
,  &hxa7241_general::test_Sheet                  //  5
,  &hxa7241_graphics::test_ColorSpace            //  6
,  &hxa7241_graphics::test_Filters               //  7
,  &hxa7241_graphics::test_Matrix3f              //  8
,  &hxa7241_graphics::test_Polar                 //  9
,  &hxa7241_graphics::test_Quaternion            // 10
,  &hxa7241_graphics::test_Spherahedron          // 11
,  &hxa7241_image::quantizing::test_quantizing   // 12
,  &hxa7241_image::ppm::test_ppm                 // 13
,  &hxa7241_image::rgbe::test_rgbe               // 14
};




bool test
(
   int   argc,
   char* argv[]
)
{
   // default options
   udword whichTest = 0;
   bool   isVerbose = false;
   bool   isQuiet   = true;
   dword  seed      = 0;

   // read options
   {
      dword outputLevel = 0;
      readParameters( argc, argv, whichTest, outputLevel, seed );
      isVerbose = (2 == outputLevel);
      isQuiet   = (0 == outputLevel);
   }

   // run tests
   bool isOk = true;
   {
      if( !isQuiet ) std::cout << "\n\n";

      const udword noOfTests = sizeof(TESTERS)/sizeof(TESTERS[0]);

      // all tests
      if( 0 == whichTest )
      {
         for( udword i = 0;  i < noOfTests;  ++i )
         {
            isOk &= (TESTERS[i])(
               !isQuiet ? &std::cout : 0, isVerbose, seed );
         }
      }
      // one chosen test
      else
      {
         const udword testIndex = ((whichTest <= noOfTests) ?
            whichTest : noOfTests) - 1;
         isOk &= (TESTERS[testIndex])( !isQuiet ? &std::cout : 0, isVerbose,
            seed );
      }
   }

   // print result
   if( !isQuiet )
   {
      std::cout <<
         (isOk ? "--- successfully" : "*** failurefully") << " completed " <<
         ((0 == whichTest) ? "all unit tests" : "one unit test") << "\n\n";
   }
   else
   {
      std::cout << "\n" << (isOk ? "--- succeeded" : "*** failed") << "\n";
   }

   std::cout.flush();

   return isOk;
}


void readParameters
(
   const int  argc,
   char*const argv[],
   udword&    whichTest,
   dword&     outputLevel,
   dword&     seed
)
{
   // read switches from args
   for( int i = argc;  i-- > 1; )
   {
      // only read if first char identifies a switch
      const char first = argv[i][0];
      if( '-' == first )
      {
         const char       key    = argv[i][1];
         const char*const pValue = argv[i] + 2;
         switch( key )
         {
            // which test
            case 't' :
            {
               // read int, defaults to 0
               whichTest = std::strtoul( pValue, 0, 10 );

               break;
            }
            // output level
            case 'o' :
            {
               // read int, defaults to 0
               outputLevel = std::atoi( pValue );

               // clamp to range
               if( outputLevel < 0 )
               {
                  outputLevel = 0;
               }
               else if( outputLevel > 2 )
               {
                  outputLevel = 2;
               }

               break;
            }
            // seed
            case 's' :
            {
               // read int, defaults to 0
               seed = std::atoi( pValue );

               break;
            }
            default  :
               ;//break;
         }
      }
   }

//   // print all args
//   std::cout << '\n';
//   std::cout << "  argc    = " << argc << '\n';
//   for( int i = 0;  i < argc;  ++i )
//   {
//      std::cout << "  argv[" << i << "] = " << argv[i] << '\n';
//   }

//   // print parameters
//   std::cout << '\n';
//   std::cout << "  which test   = " << whichTest << '\n';
//   std::cout << "  output level = " << outputLevel << '\n';
//   std::cout << "  seed         = " << seed << '\n';
}
