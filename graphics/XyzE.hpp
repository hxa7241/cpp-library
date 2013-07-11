/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef XyzE_h
#define XyzE_h




#include "hxa7241_graphics.hpp"
namespace hxa7241_graphics
{


/// core ----------------------------------------------------------------------
/**
 * Standalone realpixel-floattriple conversion functions.<br/><br/>
 *
 * (The following class wraps some state around these.)<br/><br/>
 */

dword  xyzeFromTriple
(
   float x,
   float y,
   float z
);

void   xyzeToTriple
(
   dword xyze,
   float& x,
   float& y,
   float& z
);

dword  xyzeFromTriple
(
   const float xyz[3]
);

float* xyzeToTriple
(
   dword xyze,
   float xyz[3]
);




/**
 * A Greg Ward-Larson realpixel, for any float triplet.<br/><br/>
 *
 * Stores 3 related floats into one dword, by sharing an 8 bit exponent, and
 * using 8 bits per mantissa.<br/><br/>
 *
 * * Doesn't handle negative values -- clamps to min of zero.<br/>
 * * Zero triple maps to zero dword (and vice-versa).<br/>
 *
 * reference:
 * <cite>Real Pixels, 'Graphics Gems II' (AP 91)</cite>
 */
class XyzE
{
/// standard object services ---------------------------------------------------
public:
            XyzE();
            XyzE( float x,
                  float y,
                  float z );
   explicit XyzE( const float xyz[3] );
   explicit XyzE( dword xyze );

           ~XyzE();
            XyzE( const XyzE& );
   XyzE&    operator=( const XyzE& );


/// commands -------------------------------------------------------------------
           void   set( float x,
                       float y,
                       float z );
           void   set( const float xyz[3] );

           void   set( dword xyze );


/// queries --------------------------------------------------------------------
           void   get( float& x,
                       float& y,
                       float& z )                                         const;
           float* get( float xyz[3] )                                     const;

           dword  get()                                                   const;

           bool   operator==( const XyzE& )                               const;
           bool   operator!=( const XyzE& )                               const;


/// fields ---------------------------------------------------------------------
private:
   // 0xRRGGBBEE
   dword xyze_m;
};








/// INLINES ///


/// standard object services ---------------------------------------------------
inline
XyzE::XyzE()
 : xyze_m()
{
}


inline
XyzE::XyzE
(
   const float x,
   const float y,
   const float z
)
 : xyze_m( xyzeFromTriple( x, y, z ) )
{
}


inline
XyzE::XyzE
(
   const float xyz[3]
)
 : xyze_m( xyzeFromTriple( xyz ) )
{
}


inline
XyzE::XyzE
(
   const dword xyze
)
 : xyze_m( xyze )
{
}


inline
XyzE::~XyzE()
{
}


inline
XyzE::XyzE
(
   const XyzE& other
)
 : xyze_m( other.xyze_m )
{
}


inline
XyzE& XyzE::operator=
(
   const XyzE& other
)
{
   xyze_m = other.xyze_m;

   return *this;
}




/// commands -------------------------------------------------------------------
inline
void XyzE::set
(
   const float x,
   const float y,
   const float z
)
{
   xyze_m = xyzeFromTriple( x, y, z );
}


inline
void XyzE::set
(
   const float xyz[3]
)
{
   xyze_m = xyzeFromTriple( xyz );
}


inline
void XyzE::set
(
   const dword xyze
)
{
   xyze_m = xyze;
}




/// queries --------------------------------------------------------------------
inline
void XyzE::get
(
   float& x,
   float& y,
   float& z
) const
{
   xyzeToTriple( xyze_m, x, y, z );
}


inline
float* XyzE::get
(
   float xyz[3]
) const
{
   return xyzeToTriple( xyze_m, xyz );
}


inline
dword XyzE::get() const
{
   return xyze_m;
}


inline
bool XyzE::operator==
(
   const XyzE& other
) const
{
   return xyze_m == other.xyze_m;
}


inline
bool XyzE::operator!=
(
   const XyzE& other
) const
{
   return xyze_m != other.xyze_m;
}


}//namespace




#endif//XyzE_h
