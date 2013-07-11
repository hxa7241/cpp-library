/*------------------------------------------------------------------------------

   A Project, version 1.0
   Copyright (c) 2006, Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef Blank_h
#define Blank_h


#include "Something.hpp"   // to compose thing in other package

#include "otherthing_package.hpp"   // to reference thing in other package




#include "hxa7241_package.hpp"
namespace hxa7241_package
{
   using whatever::Something;    // to use things in other packages
   using whatever::otherthing;


/**
 * An empty class to show standard code structure.<br/><br/>
 *
 * (class comments are based on java conventions.<br/>
 * The first line of the class comment summarises the class in one short line
 * ending with a '.'. it is compulsory.<br/>
 * The following lines of the class comment up to the first '@' section are
 * general commentary for users of the class. they are compulsory.)<br/><br/>
 *
 * The 'standard object services' section contains basic methods all classes
 * must treat: constructor and destructor must be declared and defined, copy
 * constructor and copy assignment must be declared and defined or declared
 * private and not defined. Also for containing contructor variants, assignment
 * variants, and conversion operators.<br/><br/>
 *
 * The 'commands' section contains methods that change object state -- that is
 * non-const methods -- that also do not provide values to callers.<br/><br/>
 *
 * The 'queries' section contains methods that do not change object state --
 * that is const methods -- that do provide values to callers.<br/><br/>
 *
 * The 'implementation' section contains misc background methods called by
 * methods in other sections.<br/><br/>
 *
 * All lines are 80 chars or less.
 *
 * @see
 * The 'see' comment section refers to other related classes or code. this is
 * optional.
 *
 * @implementation
 * The 'implementation' comment section contains general commentary for (re-)
 * implementors and derivers of the class. This is optional.
 *
 * @invariants
 * The 'invariants' comment section lists conditions that shall/must obtain for
 * each of the fields in the class. This is optional.
 */
class Blank
{
/// standard object services ---------------------------------------------------
public:
            Blank();

   virtual ~Blank();
            Blank( const Blank& );
   Blank& operator=( const Blank& );


/// commands -------------------------------------------------------------------


/// queries --------------------------------------------------------------------


/// implementation -------------------------------------------------------------
protected:


/// fields ---------------------------------------------------------------------
private:

};








/// INLINES ///


/// TEMPLATES ///


}//namespace




#endif//Blank_h
