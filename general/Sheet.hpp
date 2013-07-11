/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef Sheet_h
#define Sheet_h


#include "Array.hpp"




#include "hxa7241_general.hpp"
namespace hxa7241_general
{


/**
 * A simple 2D dynamic array, suitable for images.<br/><br/>
 *
 * Size is explicit - there is no hidden reserve.<br/>
 * Non-owning reference capability.<br/><br/>
 *
 * isAdopt false means make a non-owning reference to supplied storage: Sheet
 * will not delete the pointer. But subsequent use of copy-assignor, or setSize
 * will revert Sheet back to owning new storage.<br/><br/>
 *
 * isAdopt true means take ownership of a pointer to storage: Sheet will delete
 * the pointer when needed.<br/><br/>
 *
 * Copy constructor and copy assignor always make new owned storage (rather
 * than copy ref-status from a non-owning ref).<br/><br/>
 *
 * Non-default constructor, copy constructor, copy assignor and setSize all can
 * throw.<br/><br/>
 *
 * getRow and getElement wrap around out of bound indexs (length-wise).
 *
 * @implementation
 * A variant of a 'Decorator' for Array -- which does all the storage
 * management, Sheet just mapping two dimensions to one.
 */
template<class TYPE>
class Sheet
{
/// standard object services ---------------------------------------------------
public:
                  Sheet();
                  Sheet( dword width,
                         dword height );
                  Sheet( TYPE* pStorage,
                         dword width,
                         dword height,
                         bool  isAdopt );

   virtual       ~Sheet();
                  Sheet( const Sheet& );
           Sheet& operator=( const Sheet& );


/// commands -------------------------------------------------------------------
   virtual void   setSize( dword width,
                           dword height );
   virtual void   setStorage( TYPE* pStorage,
                              dword width,
                              dword height,
                              bool  isAdopt );

   virtual void   zeroStorage();

   virtual TYPE*  getStorage();
   virtual TYPE*  getRow( dword y );

   virtual TYPE&  getElement( dword x,
                              dword y );
   virtual TYPE&  get( dword x,
                       dword y );

   virtual TYPE&  getElement( dword i );
   virtual TYPE&  operator[]( int i );


/// queries --------------------------------------------------------------------
   virtual dword  getLength()                                             const;
   virtual dword  getWidth()                                              const;
   virtual dword  getHeight()                                             const;
   virtual bool   isOwning()                                              const;

   static  bool   isSizeWithinRange( dword width,
                                     dword height );
   static  dword  getMaxSize();

   virtual bool   isInRange( dword x,
                             dword y )                                    const;

   virtual const TYPE* getStorage()                                       const;
   virtual const TYPE* getRow( dword y )                                  const;

   virtual const TYPE& getElement( dword x,
                                   dword y )                              const;
   virtual const TYPE& get( dword x,
                            dword y )                                     const;

   virtual const TYPE& getElement( dword i )                              const;
   virtual const TYPE& operator[]( int i )                                const;

   virtual Sheet* clone()                                                 const;


/// implementation -------------------------------------------------------------
protected:
   virtual void   assign( const Sheet<TYPE>& );


/// fields ---------------------------------------------------------------------
private:
   Array<TYPE> array_m;

   dword       width_m;
   dword       height_m;

   static const char ALLOCATION_EXCEPTION_MESSAGE[];
   static const char SIZE_EXCEPTION_MESSAGE[];
};








template<class TYPE1, class TYPE2>
bool isSameSize
(
   const Sheet<TYPE1>&,
   const Sheet<TYPE2>&
);








/// statics --------------------------------------------------------------------
template<class TYPE>
const char Sheet<TYPE>::ALLOCATION_EXCEPTION_MESSAGE[] =
   "Sheet<>::clone() - storage allocation failed";
template<class TYPE>
const char Sheet<TYPE>::SIZE_EXCEPTION_MESSAGE[] =
   "Sheet<>::setSize() - size outside range";




/// standard object services ---------------------------------------------------
template<class TYPE>
Sheet<TYPE>::Sheet()
 : array_m ()
 , width_m ( 0 )
 , height_m( 0 )
{
}


template<class TYPE>
Sheet<TYPE>::Sheet
(
   const dword width,
   const dword height
)
 : array_m ()
 , width_m ( 0 )
 , height_m( 0 )
{
   Sheet<TYPE>::setSize( width, height );
}


template<class TYPE>
Sheet<TYPE>::Sheet
(
   TYPE*const  pStorage,
   const dword width,
   const dword height,
   const bool  isAdopt
)
 : array_m ()
 , width_m ( 0 )
 , height_m( 0 )
{
   Sheet<TYPE>::setStorage( pStorage, width, height, isAdopt );
}


template<class TYPE>
Sheet<TYPE>::~Sheet()
{
}


template<class TYPE>
Sheet<TYPE>::Sheet
(
   const Sheet<TYPE>& other
)
 : array_m ()
 , width_m ( 0 )
 , height_m( 0 )
{
   Sheet<TYPE>::assign( other );
}


template<class TYPE>
Sheet<TYPE>& Sheet<TYPE>::operator=
(
   const Sheet<TYPE>& other
)
{
   assign( other );

   return *this;
}




/// commands -------------------------------------------------------------------
template<class TYPE>
void Sheet<TYPE>::setSize
(
   const dword width,
   const dword height
)
{
   if( !isSizeWithinRange( width, height ) )
   {
      throw SIZE_EXCEPTION_MESSAGE;
   }

   array_m.setLength( width * height );

   width_m  = width;
   height_m = height;
}


template<class TYPE>
void Sheet<TYPE>::setStorage
(
   TYPE*const  pStorage,
   const dword width,
   const dword height,
   const bool  isAdopt
)
{
   if( !isSizeWithinRange( width, height ) )
   {
      throw SIZE_EXCEPTION_MESSAGE;
   }

   array_m.setStorage( pStorage, width * height, isAdopt );

   width_m  = width;
   height_m = height;
}


template<class TYPE>
void Sheet<TYPE>::zeroStorage()
{
   array_m.zeroStorage();
}


template<class TYPE>
inline
TYPE* Sheet<TYPE>::getStorage()
{
   return array_m.getStorage();
}


template<class TYPE>
TYPE* Sheet<TYPE>::getRow
(
   dword y
)
{
   y = (y >= 0) ? y : -y;
   return array_m.getStorage() + ((y % height_m) * width_m);
}


template<class TYPE>
TYPE& Sheet<TYPE>::getElement
(
   const dword x,
   const dword y
)
{
   return array_m.get( x + (y * width_m) );
}


template<class TYPE>
TYPE& Sheet<TYPE>::get
(
   const dword x,
   const dword y
)
{
   return array_m[ x + (y * width_m) ];
}


template<class TYPE>
inline
TYPE& Sheet<TYPE>::getElement
(
   const dword i
)
{
   return array_m.get( i );
}


template<class TYPE>
inline
TYPE& Sheet<TYPE>::operator[]
(
   const int i
)
{
   return array_m[ i ];
}




/// queries --------------------------------------------------------------------
template<class TYPE>
inline
dword Sheet<TYPE>::getLength() const
{
   return array_m.getLength();
}


template<class TYPE>
inline
dword Sheet<TYPE>::getWidth() const
{
   return width_m;
}


template<class TYPE>
inline
dword Sheet<TYPE>::getHeight() const
{
   return height_m;
}


template<class TYPE>
inline
bool Sheet<TYPE>::isOwning() const
{
   return array_m.isOwning();
}


template<class TYPE>
bool Sheet<TYPE>::isSizeWithinRange
(
   const dword width,
   const dword height
)
{
   bool isWithin = true;

   // is negative ?
   if( (width < 0) | (height < 0) )
   {
      isWithin = false;
   }
   else if( height != 0 )   // prevent divide by zero
   {
      // is the total number of elements greater than max length ?
      if( width > (getMaxSize() / height) )
      {
         isWithin = false;
      }
   }

   return isWithin;
}


template<class TYPE>
inline
dword Sheet<TYPE>::getMaxSize()
{
   return Array<TYPE>::getMaxLength();
}


template<class TYPE>
bool Sheet<TYPE>::isInRange
(
   const dword x,
   const dword y
) const
{
   return (x >= 0) && (x < width_m) && (y >= 0) && (y < height_m);
}


template<class TYPE>
inline
const TYPE* Sheet<TYPE>::getStorage() const
{
   return array_m.getStorage();
}


template<class TYPE>
const TYPE* Sheet<TYPE>::getRow
(
   dword y
) const
{
   y = (y >= 0) ? y : -y;
   return array_m.getStorage() + ((y % height_m) * width_m);
}


template<class TYPE>
const TYPE& Sheet<TYPE>::getElement
(
   const dword x,
   const dword y
) const
{
   return array_m.get( x + (y * width_m) );
}


template<class TYPE>
const TYPE& Sheet<TYPE>::get
(
   const dword x,
   const dword y
) const
{
   return array_m[ x + (y * width_m) ];
}


template<class TYPE>
inline
const TYPE& Sheet<TYPE>::getElement
(
   const dword i
) const
{
   return array_m.get( i );
}


template<class TYPE>
inline
const TYPE& Sheet<TYPE>::operator[]
(
   const int i
) const
{
   return array_m[ i ];
}


template<class TYPE1, class TYPE2>
bool isSameSize
(
   const Sheet<TYPE1>& first,
   const Sheet<TYPE2>& second
)
{
   return (first.getWidth()  == second.getWidth()) &
          (first.getHeight() == second.getHeight());
}


template<class TYPE>
Sheet<TYPE>* Sheet<TYPE>::clone() const
{
   Sheet<TYPE>* pClone = new Sheet<TYPE>( *this );
   if( 0 == pClone )
   {
      throw ALLOCATION_EXCEPTION_MESSAGE;
   }

   return  pClone;
}




/// implementation -------------------------------------------------------------
template<class TYPE>
void Sheet<TYPE>::assign
(
   const Sheet<TYPE>& other
)
{
   array_m  = other.array_m;

   width_m  = other.width_m;
   height_m = other.height_m;
}


}//namespace




#endif//Sheet_h
