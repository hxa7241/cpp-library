/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef Bounded_h
#define Bounded_h




#include "hxa7241_general.hpp"
namespace hxa7241_general
{


/**
 * A scalar value within a restricted range.<br/><br/>
 *
 * Provides implicit conversion to and from its basic type.<br/><br/>
 *
 * Constant.
 */
template<class _TYPE, dword _MIN, dword _MAX>
class Bounded
{
/// standard object services ---------------------------------------------------
public:
            Bounded();
private:
            Bounded( bool,
                     _TYPE );
public:
           ~Bounded();
            Bounded( const Bounded& );
   Bounded& operator=( const Bounded& );

/// implicit conversions
            Bounded( _TYPE );
            operator _TYPE() const;


/// queries --------------------------------------------------------------------
   static  _TYPE clamp( _TYPE );


/// constants ------------------------------------------------------------------
   static  Bounded MIN();
   static  Bounded MAX();


/// fields ---------------------------------------------------------------------
private:
   _TYPE bounded_m;
};








/// standard object services ---------------------------------------------------
template<class _TYPE, dword _MIN, dword _MAX>
inline
Bounded<_TYPE, _MIN, _MAX>::Bounded()
 : bounded_m( static_cast<_TYPE>(_MIN) )
{
}


template<class _TYPE, dword _MIN, dword _MAX>
inline
Bounded<_TYPE, _MIN, _MAX>::Bounded
(
   bool, //dummy
   const _TYPE v
)
 : bounded_m( v )
{
}


template<class _TYPE, dword _MIN, dword _MAX>
inline
Bounded<_TYPE, _MIN, _MAX>::~Bounded()
{
}


template<class _TYPE, dword _MIN, dword _MAX>
inline
Bounded<_TYPE, _MIN, _MAX>::Bounded
(
   const Bounded& other
)
 : bounded_m( other.bounded_m )
{
}


template<class _TYPE, dword _MIN, dword _MAX>
inline
Bounded<_TYPE, _MIN, _MAX>& Bounded<_TYPE, _MIN, _MAX>::operator=
(
   const Bounded& other
)
{
   bounded_m = other.bounded_m;

   return *this;
}


template<class _TYPE, dword _MIN, dword _MAX>
inline
Bounded<_TYPE, _MIN, _MAX>::Bounded
(
   const _TYPE v
)
 : bounded_m( Bounded::clamp( v ) )
{
}


template<class _TYPE, dword _MIN, dword _MAX>
inline
Bounded<_TYPE, _MIN, _MAX>::operator _TYPE() const
{
   return bounded_m;
}




/// constants ------------------------------------------------------------------
template<class _TYPE, dword _MIN, dword _MAX>
inline
Bounded<_TYPE, _MIN, _MAX> Bounded<_TYPE, _MIN, _MAX>::MIN()
{
   return Bounded( false, static_cast<_TYPE>(_MIN) );
}


template<class _TYPE, dword _MIN, dword _MAX>
inline
Bounded<_TYPE, _MIN, _MAX> Bounded<_TYPE, _MIN, _MAX>::MAX()
{
   return Bounded( false, static_cast<_TYPE>(_MAX) );
}


}//namespace




#endif//Bounded_h
