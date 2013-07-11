/*------------------------------------------------------------------------------

   HXA7241 Image library.
   Copyright (c) 2005-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include "PixelsPtr.hpp"


using namespace hxa7241_image;




/// standard object services ---------------------------------------------------
PixelsPtr::PixelsPtr()
 : is48Bit_m( false )
 , ptr_m    ( 0 )
{
}


PixelsPtr::PixelsPtr
(
   const bool  is48Bit,
   const dword length
)
 : is48Bit_m( false )
 , ptr_m    ( 0 )
{
   PixelsPtr::set( is48Bit, length );
}


PixelsPtr::~PixelsPtr()
{
   PixelsPtr::destruct();
}




/// commands -------------------------------------------------------------------
void PixelsPtr::set
(
   const bool  is48Bit,
   const dword length
)
{
   void* ptr = !is48Bit ? static_cast<void*>(new ubyte[length]) :
      static_cast<void*>(new uword[length]);

   PixelsPtr::destruct();

   is48Bit_m = is48Bit;
   ptr_m     = ptr;
}


void* PixelsPtr::release()
{
   void* p = ptr_m;
   ptr_m = 0;
   return p;
}




/// implementation -------------------------------------------------------------
void PixelsPtr::destruct()
{
   if( !is48Bit_m )
   {
      delete[] static_cast<ubyte*>( ptr_m );
   }
   else
   {
      delete[] static_cast<uword*>( ptr_m );
   }
}
