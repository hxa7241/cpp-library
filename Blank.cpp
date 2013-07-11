/*------------------------------------------------------------------------------

   A Project, version 1.0
   Copyright (c) 2006, Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include "otherthing.hpp"

#include "Blank.hpp"   // own header is included last


using namespace hxa7241_package;




/// conditional compilations




/// local declarations
namespace
{
}




/// constants




/// standard object services ---------------------------------------------------
Blank::Blank()
{
}


Blank::~Blank()
{
}


Blank::Blank
(
   const Blank& other
)
{
   Blank::operator=( other );
}


Blank& Blank::operator=
(
   const Blank& other
)
{
   if( &other != this )
   {
   }

   return *this;
}




/// commands -------------------------------------------------------------------




/// queries --------------------------------------------------------------------




/// implementation -------------------------------------------------------------
