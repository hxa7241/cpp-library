/*--------------------------------------------------------------------

   Perceptuum3 renderer, version 1.0
   Copyright (c) 2004,  Harrison Ainsworth.  All rights reserved.

   http://www.hxa7241.org/

--------------------------------------------------------------------*/


#include <time.h>
#include <math.h>

#include "Polar.hpp"




namespace hxa7241_graphics
{


/*
 * 'A Low Distortion Map Between Disk And Square'. - Shirley, Chiu
 * (Journal Of Graphics Tools, v2n3p45 1997)
 *
 * This transforms points on [0,1]^2 to points on unit disk centered at
 * origin.  Each "pie-slice" quadrant of square is handled as a seperate
 * case.  The bad floating point cases are all handled appropriately.
 * The regions for (a,b) are:
 *
 *        phi = pi/2
 *       -----*-----
 *       |\       /|
 *       |  \ 2 /  |
 *       |   \ /   |
 * phi=pi* 3  *  1 *phi = 0
 *       |   / \   |
 *       |  / 4 \  |
 *       |/       \|
 *       -----*-----
 *        phi = 3pi/2
 *
 * squrx, squry is point on [0,1]^2.  x, y is point on radius 1 disk
 *
 * [hxa7241: changed double to float, clamped input, factored out phi * pi/4,
 * added borland/intel sincos usage]
 */
void squareToDisk
(
   float  squrx,
   float  squry,
   float& x,
   float& y
)
{
   static const float PI_OVER_4 = static_cast<float>(3.14159265358979 / 4.0);

   /// clamp input to [0,1] square
   squrx = (squrx < 0.0f) ? 0.0f : ((squrx > 1.0f) ? 1.0f : squrx);
   squry = (squry < 0.0f) ? 0.0f : ((squry > 1.0f) ? 1.0f : squry);


   float phi;
   float r;

   float a = 2.0f * squrx - 1.0f;   /// (a,b) is now on [-1,1]^2
   float b = 2.0f * squry - 1.0f;

   /// region 1 or 2
   if( a > -b )
   {
      /// region 1, also |a| > |b|
      if( a > b )
      {
         r   = a;
         phi = (b/a);
      }
      /// region 2, also |b| > |a|
      else
      {
         r   = b;
         phi = 2.0f - (a/b);
      }
   }
   /// region 3 or 4
   else
   {
      /// region 3, also |a| >= |b|, a != 0
      if( a < b )
      {
         r   = -a;
         phi = 4.0f + (b/a);
      }
      /// region 4, |b| >= |a|, but a==0 and b==0 could occur
      else
      {
         r = -b;
         if( b != 0.0f )
         {
            phi = 6.0f - (a/b);
         }
         else
         {
            phi = 0.0f;
         }
      }
   }

   phi *= PI_OVER_4;

#ifdef BORLANDCPP551
   /// use the intel single instruction
   double sine;
   double cosine;
   ::_fm_sincos( phi, &sine, &cosine );
   x = r * static_cast<float>(cosine);
   y = r * static_cast<float>(sine);
#else
   /// use the std lib
   x = r * ::cosf(phi);
   y = r * ::sinf(phi);
#endif
}


/*
 * 'A Low Distortion Map Between Disk And Square'. - Shirley, Chiu
 * (Journal Of Graphics Tools, v2n3p45 1997)
 *
 * This transforms points on unit disk centered at origin to points on
 * [0,1]^2.  Each "pie-slice" quadrant of square is handled as a
 * seperate case.  The bad floating point cases are all handled
 * appropriately.
 * The regions for (a,b) are:
 *
 *        phi = pi/2
 *       -----*-----
 *       |\       /|
 *       |  \ 2 /  |
 *       |   \ /   |
 * phi=pi* 3  *  1 *phi = 0
 *       |   / \   |
 *       |  / 4 \  |
 *       |/       \|
 *       -----*-----
 *        phi = 3pi/2
 *
 * diskx, disky is point on radius 1 disk.  x, y is point on [0,1]^2
 *
 * [hxa7241: changed double to float, clamped input radius]
 */
void diskToSquare
(
   const float diskx,
   const float disky,
   float&      x,
   float&      y
)
{
   static const float PI = 3.14159265358979f;

   float r = ::sqrt( diskx*diskx + disky*disky );
   /// clamp input to radius 1 disk
   if( r > 1.0f )
   {
      r = 1.0f;
   }

   float phi = ::atan2( disky, diskx );
   float a;
   float b;

   /// in range [-pi/4,7pi/4]
   if( phi < -PI/4.0f )
   {
      phi += 2.0f*PI;
   }

   /// region 1
   if ( phi < PI/4.0f )
   {
      a = r;
      b = phi * a / (PI/4.0f);
   }
   /// region 2
   else if( phi < 3.0f*PI/4.0f )
   {
      b = r;
      a = -(phi - PI/2.0f) * b / (PI/4.0f);
   }
   /// region 3
   else if( phi < 5.0f*PI/4.0f )
   {
      a = -r;
      b = (phi - PI) * a / (PI/4.0f);
   }
   /// region 4
   else
   {
      b = -r;
      a = -(phi - 3.0f*PI/2.0f) * b / (PI/4.0f);
   }

   x = (a + 1.0f) * 0.5f;
   y = (b + 1.0f) * 0.5f;
}




void squareToHemisphere
(
   const float squrX01,
   const float squrY01,
   float&      x,
   float&      y,
   float&      z
)
{
   float diskX;
   float diskY;
   squareToDisk( squrX01, squrY01, diskX, diskY );

   float r2 = (diskX * diskX) + (diskY * diskY);
   if( r2 > 1.0f )
   {
      r2 = 1.0f;
   }

   z = 1.0f - r2;

   float a = 0.0f;
   if( r2 > 0.0f )
   {
      a = ::sqrt( (1.0f - (z * z)) / r2 );
   }

   x = diskX * a;
   y = diskY * a;
}


void hemisphereToSquare
(
   const float x,
   const float y,
   float       z,
   float&      squrX01,
   float&      squrY01
)
{
   if( z < 0.0f )
   {
      z = 0.0f;
   }

   const float r2 = 1.0f - z;

   float a = 0.0f;
   if( r2 > 0.0f )
   {
      a = 1.0f / ::sqrt( (1.0f - (z * z)) / r2 );
   }

   const float u = x * a;
   const float v = y * a;

   diskToSquare( u, v, squrX01, squrY01 );
}




//void squareToDisk( const float squrX01, const float squrY01, float* x, float* y )
//{
// float phi;
// float r;
// rectangularUnitToPolarUnit( squrX01, squrY01, &phi, &r );
//
// polarToRectangular( phi, r, x, y );
//}


//void squareToHemisphere( const float squrX01, const float squrY01, float* x, float* y, float* z )
//{
// float phi;
// float r;
// rectangularUnitToPolarUnit( squrX01, squrY01, &phi, &r );
//
// static const float HALF_PI = static_cast<float>(3.14159265358979 * 0.5);
// float h;
// polarToRectangular( r * HALF_PI, 1.0f, z, &h );
// polarToRectangular( phi, h, x, y );
//}


//void squareToSphere( const float squrX01, const float squrY01, float& x, float& y, float& z )
//{
// float phi;
// float r;
// rectangularUnitToPolarUnit( squrX01, squrY01, &phi, &r );
//
// static const float PI = 3.14159265358979f;
// float h;
// polarToRectangular( r * PI, 1.0f, z, h );
// polarToRectangular( phi, h, x, y );
//}




/// squareToDisk() factored into two pieces ///

//void rectangularUnitToPolarUnit( float x01, float y01, float* pPhi, float* pR )
//{
// static const float PI_OVER_4 = static_cast<float>(3.14159265358979 * 0.25);
//
// x01 = hxa7241_general::clamp01c( x01 );
// y01 = hxa7241_general::clamp01c( y01 );
//
//
// float a = 2.0f * x01 - 1.0f;   /* (a,b) is now on [-1,1]^2 */
// float b = 2.0f * y01 - 1.0f;
//
// float phi;
// float r;
//
// if (a > -b) {     /* region 1 or 2 */
//    if (a > b) {  /* region 1, also |a| > |b| */
//       r = a;
//       phi = (b/a);
//    }
//    else       {  /* region 2, also |b| > |a| */
//       r = b;
//       phi = 2.0f - (a/b);
//    }
// }
// else {            /* region 3 or 4 */
//    if (a < b) {  /* region 3, also |a| >= |b|, a != 0 */
//       r = -a;
//       phi = 4.0f + (b/a);
//    }
//    else       {  /* region 4, |b| >= |a|, but a==0 and b==0 could occur */
//       r = -b;
//       if (b != 0.0f)
//          phi = 6.0f - (a/b);
//       else
//          phi = 0.0f;
//    }
// }
//
// phi *= PI_OVER_4;
//
// *pPhi = phi;
// *pR   = r;
//}




void rectangularToPolar
(
   const float x,
   const float y,
   float&      phi,
   float&      r
)
{
   r   = ::sqrt( x*x + y*y );
   phi = ::atan2( y, x );
}


void polarToRectangular
(
   const float phi,
   const float r,
   float&      x,
   float&      y
)
{
#ifdef BORLANDCPP551
   /// use the intel single instruction
   double sine;
   double cosine;
   ::_fm_sincos( phi, &sine, &cosine );
   x = r * static_cast<float>(cosine);
   y = r * static_cast<float>(sine);
#else
   /// use the std lib
   x = r * ::cosf(phi);
   y = r * ::sinf(phi);
#endif
}


void sinAndcos
(
   const float angle,
   float&      sin,
   float&      cos
)
{
#ifdef BORLANDCPP551
   /// use the intel single instruction
   double sine;
   double cosine;
   ::_fm_sincos( angle, &sine, &cosine );
   sin = static_cast<float>(sine);
   cos = static_cast<float>(cosine);
#else
   /// use the std lib
   sin = ::sinf( angle );
   cos = ::cosf( angle );
#endif
}


}//namespace








/// test -----------------------------------------------------------------------
#ifdef TESTING


#include <iostream>
#include "RandomMwc2.hpp"


namespace hxa7241_graphics
{


bool test_Polar
(
   std::ostream* pOut,
   const bool    isVerbose,
         dword   seed
)
{
   seed = (0 != seed) ? seed : 1198674274;
   hxa7241_general::RandomMwc2 rand( seed );


   bool isOk = true;

   if( pOut ) *pOut << "[ test_Polar ] (" << seed << ")\n\n";


   static const float TOLERANCE = 0.0001f;


   /// test squareToDisk and diskToSquare
   {
      for( dword i = 100000;  (i-- > 0) & isOk; )
      {
         const float sx = rand.getFloat();
         const float sy = rand.getFloat();

         float dx, dy;
         squareToDisk( sx, sy, dx, dy );

         /// radius <= 1
         {
            bool test = true;

            float dx2, dy2;
            squareToDisk( (sx-0.5f)*10.0f, (sy-0.5f)*10.0f, dx2, dy2 );

            test &= (::sqrtf((dx*dx) + (dy*dy))     <= (1.0f + FLOAT_EPSILON));
            test &= (::sqrtf((dx2*dx2) + (dy2*dy2)) <= (1.0f + FLOAT_EPSILON));
            if( !test )
            {
               if( pOut && isVerbose ) *pOut << "sqr-dsk radius  " <<
                  "failed (" << i << ")\n";
            }

            isOk &= test;
         }

         /// same octant
         {
            bool test = true;

            test &= ((sx >= 0.5f) == (dx >= 0.0f));
            test &= ((sy >= 0.5f) == (dy >= 0.0f));
            test &= (((::fabsf(sx-0.5f) >= ::fabsf(sy-0.5f)) ==
               (::fabsf(dx) >= ::fabsf(dy))) |
               ((::fabsf(::fabsf(sx-0.5f) - ::fabsf(sy-0.5f)) < TOLERANCE) &
               (::fabsf(::fabsf(dx) - ::fabsf(dy)) < TOLERANCE)));
            if( !test )
            {
               if( pOut && isVerbose ) *pOut << "sqr-dsk octant  " <<
                  "failed (" << i << ")\n";
            }

            isOk &= test;
         }

         /// squareToDisk is inverse of diskToSquare
         {
            bool test = true;

            float sx2, sy2;
            diskToSquare( dx, dy, sx2, sy2 );

            test &= (::fabsf(sx2 - sx) < TOLERANCE);
            test &= (::fabsf(sy2 - sy) < TOLERANCE);
            if( !test )
            {
               if( pOut && isVerbose ) *pOut << "sqr-dsk inverse  " <<
                  "failed  (" << i << ")\n";
            }

            isOk &= test;
         }

         /// square range
         {
            bool test = true;

            float sx2, sy2;
            diskToSquare( (sx-0.5f)*10.0f, (sy-0.5f)*10.0f, sx2, sy2 );

            test &= ((sx2 >= 0.0f) & (sx2 <= 1.0f));
            test &= ((sy2 >= 0.0f) & (sy2 <= 1.0f));
            if( !test )
            {
               if( pOut && isVerbose ) *pOut << "sqr-dsk range  " <<
                  "failed  (" << i << ")\n";
            }

            isOk &= test;
         }
      }

      /// center to center
      {
         bool test = true;

         float a, b;
         squareToDisk( 0.5f, 0.5f, a, b );
         test &= ((a == 0.0f) & (b == 0.0f));
         diskToSquare( 0.0f, 0.0f, a, b );
         test &= ((a == 0.5f) & (b == 0.5f));
         if( !test )
         {
            if( pOut && isVerbose ) *pOut << "sqr-dsk center  " << "failed\n";
         }

         isOk &= test;
      }
   }


   /// squareToHemisphere and hemisphereToSquare
   {
      for( dword i = 100000;  (i-- > 0) & isOk; )
      {
         const float sx = rand.getFloat();
         const float sy = rand.getFloat();

         float hx, hy, hz;
         squareToHemisphere( sx, sy, hx, hy, hz );

         {
            float hx2, hy2, hz2;
            squareToHemisphere( (sx-0.5f)*10.0f, (sy-0.5f)*10.0f,
               hx2, hy2, hz2 );

            /// positive z
            {
               bool test = true;

               test &= (hz >= 0.0f);
               test &= (hz2 >= 0.0f);
               if( !test )
               {
                  if( pOut && isVerbose ) *pOut << "sqr-hem positive  " <<
                     "failed  (" << i << ")\n";
               }

               isOk &= test;
            }

            /// unitized direction
            {
               bool test = true;

               const float length  = ::sqrtf( (hx*hx) + (hy*hy) + (hz*hz) );
               const float length2 = ::sqrtf( (hx2*hx2) + (hy2*hy2) +
                  (hz2*hz2) );

               test &= (::fabsf(length - 1.0f) < TOLERANCE);
               test &= (::fabsf(length2 - 1.0f) < TOLERANCE);
               if( !test )
               {
                  if( pOut && isVerbose ) *pOut << "sqr-hem unitized  " <<
                     "failed  (" << i << ")\n";
               }

               isOk &= test;
            }
         }

         /// same octant
         {
            bool test = true;

            test &= ((sx >= 0.5f) == (hx >= 0.0f));
            test &= ((sy >= 0.5f) == (hy >= 0.0f));
            test &= (((::fabsf(sx-0.5f) >= ::fabsf(sy-0.5f)) ==
               (::fabsf(hx) >= ::fabsf(hy))) | ((::fabsf(::fabsf(sx-0.5f) -
               ::fabsf(sy-0.5f)) < TOLERANCE) & (::fabsf(::fabsf(hx) -
               ::fabsf(hy)) < TOLERANCE)));
            if( !test )
            {
               if( pOut && isVerbose ) *pOut << "sqr-hem octant  " <<
                  "failed  (" << i << ")\n";
            }

            isOk &= test;
         }

         /// squareToHemisphere is inverse of hemisphereToSquare
         {
            bool test = true;

            float sx2, sy2;
            hemisphereToSquare( hx, hy, hz, sx2, sy2 );

            test &= (::fabsf(sx2 - sx) < TOLERANCE);
            test &= (::fabsf(sy2 - sy) < TOLERANCE);
            if( !test )
            {
               if( pOut && isVerbose ) *pOut << "sqr-hem inverse  " <<
                  "failed  (" << i << ")\n";
            }

            isOk &= test;
         }

         /// square range
         {
            bool test = true;

            const float c = rand.getFloat();

            float sx2, sy2;
            hemisphereToSquare( (sx-0.5f)*10.0f, (sy-0.5f)*10.0f,
               (c-0.5f)*10.0f, sx2, sy2 );

            test &= ((sx2 >= 0.0f) & (sx2 <= 1.0f));
            test &= ((sy2 >= 0.0f) & (sy2 <= 1.0f));
            if( !test )
            {
               if( pOut && isVerbose ) *pOut << "sqr-hem range  " <<
                  "failed  (" << i << ")\n";
            }

            isOk &= test;
         }
      }

      /// center to center
      {
         bool test = true;

         float a, b, c;
         squareToHemisphere( 0.5f, 0.5f, a, b, c );
         test &= ((a == 0.0f) & (b == 0.0f) & (c == 1.0f));
         hemisphereToSquare( 0.0f, 0.0f, 1.0f, a, b );
         test &= ((a == 0.5f) & (b == 0.5f));
         if( !test )
         {
            if( pOut && isVerbose ) *pOut << "sqr-hem center  " << "failed\n";
         }

         isOk &= test;
      }
   }


   /// rectangularToPolar and polarToRectangular
   {
      for( dword i = 100000;  (i-- > 0) & isOk; )
      {
         static const double PI = 3.14159265358979f;
         static const float  SCALE     = 1000.0f;
         static const float  TOLERANCE = SCALE / static_cast<float>(0x00100000);

         const float rx = (rand.getFloat() - 0.5f) * SCALE;
         const float ry = (rand.getFloat() - 0.5f) * SCALE;

         float pa, pr;
         rectangularToPolar( rx, ry, pa, pr );

         /// correct quadrant
         {
            bool test = true;

            static const float LO[] = { static_cast<float>(-PI),
               static_cast<float>(PI / 2.0), static_cast<float>(-PI / 2.0),
               0.0 };
            static const float HI[] = { static_cast<float>(-PI / 2.0),
               static_cast<float>(PI), 0.0, static_cast<float>(PI / 2.0) };
            const dword quadrant = ((rx >= 0.0f) << 1) | (ry >= 0.0f);

            test &= ((pa >= LO[quadrant]) & (pa <= HI[quadrant]));
            if( !test )
            {
               if( pOut && isVerbose ) *pOut << "rec-pol quadrant  " <<
                  "failed  (" << i << ")\n";
            }

            isOk &= test;
         }

         /// rectangularToPolar is inverse of polarToRectangular
         {
            bool test = true;

            float rx2, ry2;
            polarToRectangular( pa, pr, rx2, ry2 );

            test &= (::fabsf(rx2 - rx) < TOLERANCE);
            test &= (::fabsf(ry2 - ry) < TOLERANCE);
            if( !test )
            {
               if( pOut && isVerbose ) *pOut << "rec-pol inverse  " <<
                  "failed  (" << i << ")\n";
            }

            isOk &= test;
         }

         /// radius magnitude
         {
            bool test = true;

            const float pa2 = (rand.getFloat() - 0.5f) *
               static_cast<float>(PI * 2.0);
            const float pr2 = rand.getFloat() * SCALE;

            float rx2, ry2;
            polarToRectangular( pa2, pr2, rx2, ry2 );

            test &= (::fabsf(::sqrt( (rx2*rx2) + (ry2*ry2) ) - pr2) <
               TOLERANCE);
            if( !test )
            {
               if( pOut && isVerbose ) *pOut << "rec-pol radius  " <<
                  "failed  (" << i << ")\n";
            }

            isOk &= test;
         }

         /// xy
         {
            bool test = true;

            test &= (::fabsf( rx - (pr * ::cosf(pa)) ) < TOLERANCE);
            test &= (::fabsf( ry - (pr * ::sinf(pa)) ) < TOLERANCE);
            if( !test )
            {
               if( pOut && isVerbose ) *pOut << "rec-pol xy  " <<
                  "failed  (" << i << ")\n";
            }

            isOk &= test;
         }
      }
   }


   if( pOut ) *pOut << (isOk ? "--- successfully" : "*** failurefully") <<
      " completed " << "\n\n\n";

   if( pOut ) pOut->flush();


   return isOk;
}


}//namespace


#endif//TESTING
