/*------------------------------------------------------------------------------

   HXA7241 Image library.
   Copyright (c) 2005-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef PixelsPtr_h
#define PixelsPtr_h




#include "hxa7241_image.hpp"
namespace hxa7241_image
{


/**
 * Simplified auto_ptr for void-polymorphic pixels.<br/><br/>
 *
 * No copying.<br/><br/>
 *
 * @implementation
 * ...so, can you delete[] builtins through void* ?
 *
 * 'The C++ Programming Language' doesn't say
 *
 * 'The C++ Standard ISO-IEC 14882-1998' is ambiguous and confusing...
 * "In the second alternative (delete array), the value of the operand of
 * delete shall be the pointer value which resulted from a previous
 * new-expression. If not, the behavior is undefined."
 * -- ok, that is fine and clear, but then: --
 * "[note: this means that the syntax of the delete-expression must match
 * the type of the object allocated by new, not the syntax of the
 * new-expression.]"
 * -- what does that mean?? does it mean the *type* of the delete-expr,
 * rather than 'syntax'? and does that apply to void*?
 *
 * ...
 */
class PixelsPtr
{
/// standard object services ---------------------------------------------------
public:
   PixelsPtr();
   PixelsPtr( const bool  is48Bit,
              const dword length );
   ~PixelsPtr();

private:
   PixelsPtr( const PixelsPtr& );
   PixelsPtr& operator=( const PixelsPtr& );
public:


/// commands -------------------------------------------------------------------
   void  set( const bool  is48Bit,
              const dword length );

   void* release();


/// queries --------------------------------------------------------------------
   bool  is48Bit() const
   {
      return is48Bit_m;
   }

   void* get() const
   {
      return ptr_m;
   }


/// implementation -------------------------------------------------------------
protected:
   void  destruct();


/// fields ---------------------------------------------------------------------
private:
   bool  is48Bit_m;
   void* ptr_m;
};


}//namespace




#endif//PixelsPtr_h
