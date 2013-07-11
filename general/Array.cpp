/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include "Array.hpp"








/// test -----------------------------------------------------------------------
#ifdef TESTING


#include <iostream>


namespace hxa7241_general
{


bool test_Array
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   //seed
)
{
   bool isOk = true;

   if( pOut ) *pOut << "[ test_Array ]\n\n";


   // this is a *long* method, but do not panic! -- it is very simple:
   // (almost) every 'test-expression' is isolated and atomic.


   // basics
   {
      bool is = true;

      // constructors
      {
         // default
         {
            const Array<word> aw;

            bool ok = (0 == aw.getStorage()) & (0 == aw.getLength()) &
               aw.isOwning();
            is &= ok;
            if( pOut && isVerbose ) *pOut << "default  " << aw.getStorage() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }

         // length
         {
            const dword awl = 7;
            const Array<word> aw( awl );

            bool ok = (0 != aw.getStorage()) & (awl == aw.getLength()) &
               aw.isOwning();
            is &= ok;
            if( pOut && isVerbose ) *pOut << "length1  " << aw.getStorage() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }
         {
            const Array<word> aw( -7 );

            bool ok = (0 == aw.getStorage()) & (0 == aw.getLength()) &
               aw.isOwning();
            is &= ok;
            if( pOut && isVerbose ) *pOut << "length2  " << aw.getStorage() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }

         // storage
         word awa[] = { 11, 12, 13 };
         const dword awal  = sizeof(awa) / sizeof(awa[0]);
         {
            const Array<word> aw( awa, awal, false );

            bool ok = (awa == aw.getStorage()) & (awal == aw.getLength()) &
               !aw.isOwning();
            is &= ok;
            if( pOut && isVerbose ) *pOut << "storage1  " << aw.getStorage() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }
         {
            word* pawa = new word[ awal ];
            const Array<word> aw( pawa, awal, true );

            bool ok = (pawa == aw.getStorage()) & (awal == aw.getLength()) &
               aw.isOwning();
            is &= ok;
            if( pOut && isVerbose ) *pOut << "storage2  " << aw.getStorage() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }
         {
            const Array<word> aw( 0, 3, false );

            bool ok = (0 == aw.getStorage()) & (3 == aw.getLength()) &
               !aw.isOwning();
            is &= ok;
            if( pOut && isVerbose ) *pOut << "storage3  " << aw.getStorage() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }
         {
            const Array<word> aw( 0, 3, true );

            bool ok = (0 == aw.getStorage()) & (3 == aw.getLength()) &
               aw.isOwning();
            is &= ok;
            if( pOut && isVerbose ) *pOut << "storage4  " << aw.getStorage() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }
         {
            const Array<word> aw( awa, -3, false );

            bool ok = (awa == aw.getStorage()) & (0 == aw.getLength()) &
               !aw.isOwning();
            is &= ok;
            if( pOut && isVerbose ) *pOut << "storage5  " << aw.getStorage() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }
         {
            word* pawa = new word[ awal ];
            const Array<word> aw( pawa, -3, true );

            bool ok = (pawa == aw.getStorage()) & (0 == aw.getLength()) &
               aw.isOwning();
            is &= ok;
            if( pOut && isVerbose ) *pOut << "storage6  " << aw.getStorage() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }

         // copy
         {
            const Array<word> aww( awa, awal, false );
            const Array<word> aw( aww );

            bool ok = (aww.getStorage() != aw.getStorage()) &
               (aww.getLength() == aw.getLength()) & aw.isOwning();

            for( dword i = aw.getLength();  i-- > 0; )
            {
               ok &= (aw[i] == aww[i]);
            }
            is &= ok;
            if( pOut && isVerbose ) *pOut << "copy1  " << aw.getStorage() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }
         {
            word* pawa = new word[ awal ];
            const Array<word> awo( pawa, awal, true );
            const Array<word> aw( awo );

            bool ok = (awo.getStorage() != aw.getStorage()) &
               (awo.getLength() == aw.getLength()) & aw.isOwning();
            is &= ok;
            if( pOut && isVerbose ) *pOut << "copy2  " << aw.getStorage() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }
      }

      // copy assignment
      {
         {
            word  awa[] = { 11, 12, 13 };
            const dword awal  = sizeof(awa) / sizeof(awa[0]);
            const Array<word> aww( awa, awal, false );

            Array<word> aw( 5 );
            aw = aww;

            bool ok = (aww.getStorage() != aw.getStorage()) &
               (aww.getLength() == aw.getLength()) & aw.isOwning();
            if( pOut && isVerbose ) *pOut << "copyassign1  " <<
               aw.getStorage() << " " << aw.getLength() << " " <<
               aw.isOwning() << "  " << ok << "\n";

            for( dword i = aw.getLength();  i-- > 0; )
            {
               ok &= (aw[i] == aww[i]);
            }
            is &= ok;
            if( pOut && isVerbose ) *pOut << "copyassign2  " <<
               aw.getStorage() << " " << aw.getLength() << " " <<
               aw.isOwning() << "  " << ok << "\n";
         }
         {
            const Array<word> aww( 0, 0, false );
            Array<word> aw( 5 );
            aw = aww;

            bool ok = (aww.getStorage() != aw.getStorage()) &
               (aww.getLength() == aw.getLength()) & aw.isOwning();
            is &= ok;
            if( pOut && isVerbose ) *pOut << "copyassign3  " <<
               aw.getStorage() << " " << aw.getLength() << " " <<
               aw.isOwning() << "  " << ok << "\n";
         }
      }

      if( pOut && isVerbose ) *pOut << "\n";

      if( pOut ) *pOut << "basics : " <<
         (is ? "--- succeeded" : "*** failed") << "\n\n";

      isOk &= is;
   }


   // commands
   {
      bool is = true;

      for( dword j = 0;  j < 11;  ++j )
      {
         word wa[] = { 11, 12, 13 };
         const dword wal  = sizeof(wa) / sizeof(wa[0]);
         Array<word> aww( wa, wal, false );

         word* pwa = new word[ wal ];
         Array<word> awo( pwa, wal, true );

         Array<word> awm( wal );

         for( dword i = wal;  i-- > 0; )
         {
            awo[i] = wa[i];
            awm[i] = wa[i];
         }

         Array<word>* aws[] = { &aww, &awo, &awm };

         // using made, adopted, reffed
         for( dword a = 0;  a < 3;  ++a )
         {
            Array<word>& ar = *(aws[a]);

            bool ok = true;

            switch( j )
            {
            case 0 :
               // setLength
               {
                  const word* m = ar.getStorage();
                  ar.setLength( 5 );
                  ok &= (0 != ar.getStorage()) & (m != ar.getStorage()) &
                     (5 == ar.getLength()) & ar.isOwning();
               }
               break;

            case 1 :
               // setStorage ref
               {
                  word a[] = { 21, 22, 23 };
                  const dword al  = sizeof(a) / sizeof(a[0]);
                  ar.setStorage( a, al, false );
                  ok &= (a == ar.getStorage()) & (al == ar.getLength()) &
                     !ar.isOwning();
               }
               break;

            case 2 :
               // setStorage adopt
               {
                  const dword pal = 5;
                  word* pa = new word[ pal ];
                  ar.setStorage( pa, pal, true );
                  ok &= (pa == ar.getStorage()) & (pal == ar.getLength()) &
                     ar.isOwning();
               }
               break;

            case 3 :
               // swap made
               {
                  Array<word> a2( 5 );
                  const word* m1 = ar.getStorage();
                  const dword l1 = ar.getLength();
                  const bool  o1 = ar.isOwning();
                  const word* m2 = a2.getStorage();
                  const dword l2 = a2.getLength();
                  const bool  o2 = a2.isOwning();
                  ar.swap( a2 );
                  ok &= (m2 == ar.getStorage()) & (l2 == ar.getLength()) &
                     (o2 == ar.isOwning());
                  ok &= (m1 == a2.getStorage()) & (l1 == a2.getLength()) &
                     (o1 == a2.isOwning());
               }
               break;

            case 4 :
               // swap adopted
               {
                  word* paa2 = new word[ 5 ];
                  Array<word> a2( paa2, 5, true );
                  const word* m1 = ar.getStorage();
                  const dword l1 = ar.getLength();
                  const bool  o1 = ar.isOwning();
                  const word* m2 = a2.getStorage();
                  const dword l2 = a2.getLength();
                  const bool  o2 = a2.isOwning();
                  ar.swap( a2 );
                  ok &= (m2 == ar.getStorage()) & (l2 == ar.getLength()) &
                     (o2 == ar.isOwning());
                  ok &= (m1 == a2.getStorage()) & (l1 == a2.getLength()) &
                     (o1 == a2.isOwning());
               }
               break;

            case 5 :
               // swap reffed
               {
                  word aa2[] = { 31, 32, 33, 34, 35 };
                  Array<word> a2( aa2, 5, false );
                  const word* m1 = ar.getStorage();
                  const dword l1 = ar.getLength();
                  const bool  o1 = ar.isOwning();
                  const word* m2 = a2.getStorage();
                  const dword l2 = a2.getLength();
                  const bool  o2 = a2.isOwning();
                  ar.swap( a2 );
                  ok &= (m2 == ar.getStorage()) & (l2 == ar.getLength()) &
                     (o2 == ar.isOwning());
                  ok &= (m1 == a2.getStorage()) & (l1 == a2.getLength()) &
                     (o1 == a2.isOwning());
               }
               break;

            case 6 :
               // append
               {
                  const word* m1 = ar.getStorage();
                  const dword l1 = ar.getLength();
                  //const bool  o1 = ar.isOwning();

                  ar.append( static_cast<word>(-100) );

                  ok &= (m1 != ar.getStorage()) & (l1 + 1 == ar.getLength()) &
                     ar.isOwning();
                  for( dword i = wal;  i-- > 0; )
                  {
                     ok &= (ar[i] == wa[i]);
                  }
                  ok &= (-100 == ar[wal]);
               }
               break;

            case 7 :
               // remove
               {
                  const word* m1 = ar.getStorage();
                  const dword l1 = ar.getLength();
                  //const bool  o1 = ar.isOwning();

                  const dword r = 1;
                  ar.remove( r );

                  ok &= (m1 != ar.getStorage()) & (l1 - 1 == ar.getLength()) &
                     ar.isOwning();
                  for( dword i = 0;  i < ar.getLength();  ++i )
                  {
                     ok &= (ar[i] == wa[i + static_cast<dword>(i >= r)]);
                  }
               }
               break;

            case 8 :
               {
                  // operator[]
                  // get
                  for( dword i = wal;  i-- > 0; )
                  {
                     if( 0 == (i & 1) )
                     {
                        ar[i] = -100;
                     }
                     else
                     {
                        ar.get(i) = -100;
                     }
                  }
                  for( dword i = wal;  i-- > 0; )
                  {
                     ok &= (-100 == ar[i]);
                  }
               }
               break;

            case 9 :
               {
                  // getStorage
                  const word w = wa[1];
                  ar.getStorage()[1] = wa[1] - 100;
                  ok &= ((w - 100) == ar[1]);
                  ok &= (&(ar[1]) == ar.getStorage() + 1);
               }
               break;

            case 10 :
               {
                  // zeroStorage
                  ar.zeroStorage();
                  for( dword i = wal;  i-- > 0; )
                  {
                     ok &= (0 == ar[i]);
                  }
               }
               break;
            }
            if( pOut && isVerbose ) *pOut << "commands " << j << " " << a <<
               "  " << ok << "\n";
            is &= ok;
         }
      }

      if( pOut && isVerbose ) *pOut << "\n";

      if( pOut ) *pOut << "commands : " <<
         (is ? "--- succeeded" : "*** failed") << "\n\n";

      isOk &= is;
   }


   // queries
   {
      bool is = true;

      word wa[] = { 41, 42, 43 };
      const dword wal  = sizeof(wa) / sizeof(wa[0]);
      Array<word> aww( wa, wal, false );

      word* pwa = new word[ wal ];
      Array<word> awo( pwa, wal, true );

      Array<word> awm( wal );

      for( dword i = wal;  i-- > 0; )
      {
         awo[i] = wa[i];
         awm[i] = wa[i];
      }

      Array<word>* aws[] = { &aww, &awo, &awm };

      // using made, adopted, reffed
      for( dword a = 0;  a < 3;  ++a )
      {
         Array<word>& ar = *(aws[a]);

         // getLength
         is &= (wal == ar.getLength());
         if( pOut && isVerbose ) *pOut << "getLength " << a << "  " << is <<
            "\n";

         // isEmpty
         is &= ar.isEmpty() == (0 == ar.getLength());
         if( pOut && isVerbose ) *pOut << "isEmpty   " << a << "  " << is <<
            "\n";

         // isOwning
         is &= ((0 != a) == ar.isOwning());
         if( pOut && isVerbose ) *pOut << "isOwning  " << a << "  " << is <<
            "\n";

         // getStorage
         is &= (0 == a) ? (wa == ar.getStorage()) : (wa != ar.getStorage());
         is &= (0 != ar.getStorage());
         if( pOut && isVerbose ) *pOut << "getStorage " << a << "  " << is <<
            "\n";

         // operator[]
         // get
         for( dword i = wal;  i-- > 0; )
         {
            is &= (ar[i] == wa[i]);
            is &= (ar.get(i) == wa[i]);
         }
         if( pOut && isVerbose ) *pOut << "get       " << a << "  " << is <<
            "\n";
      }

      // getMaxLength
      is &= (DWORD_MAX == Array<word>::getMaxLength());
      if( pOut && isVerbose ) *pOut << "getMaxLength " << is << "\n";

      if( pOut && isVerbose ) *pOut << "\n";

      if( pOut ) *pOut << "queries : " <<
         (is ? "--- succeeded" : "*** failed") << "\n\n";

      isOk &= is;
   }


   if( pOut ) *pOut << (isOk ? "--- successfully" : "*** failurefully") <<
      " completed " << "\n\n\n";

   if( pOut ) pOut->flush();


   return isOk;
}


}//namespace


#endif//TESTING
