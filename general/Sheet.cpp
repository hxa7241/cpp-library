/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2004-2006,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include "Sheet.hpp"








/// test -----------------------------------------------------------------------
#ifdef TESTING


#include <iostream>


namespace hxa7241_general
{


bool test_Sheet
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   //seed
)
{
   bool isOk = true;

   if( pOut ) *pOut << "[ test_Sheet ]\n\n";


   // this is a *long* method, but do not panic! -- it is very simple:
   // (almost) every 'test-expression' is isolated and atomic.


   // basics
   {
      bool is = true;

      // constructors
      {
         // default
         {
            const Sheet<word> aw;

            bool ok = (0 == aw.getStorage()) & (0 == aw.getLength()) &
               (0 == aw.getWidth()) & (0 == aw.getHeight()) &
               aw.isOwning();
            is &= ok;
            if( pOut && isVerbose ) *pOut << "default  " << aw.getStorage() <<
               " " << aw.getWidth() << " " << aw.getHeight() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }

         // size
         {
            const dword awsw = 3;
            const dword awsh = 2;
            const dword awl  = 6;
            const Sheet<word> aw( awsw, awsh );

            bool ok = (0 != aw.getStorage()) & (awl == aw.getLength()) &
               (awsw == aw.getWidth()) & (awsh == aw.getHeight()) &
               aw.isOwning();
            is &= ok;
            if( pOut && isVerbose ) *pOut << "size1  " << aw.getStorage() <<
               " " << aw.getWidth() << " " << aw.getHeight() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }
         {
            bool ok;
            try
            {
               const Sheet<word> aw( -3, 2 );
               ok = false;
            }
            catch( ... )
            {
               ok = true;
            }

            is &= ok;
            if( pOut && isVerbose ) *pOut << "size2  " << ok << "\n";
         }
         {
            bool ok;
            try
            {
               const Sheet<word> aw( 0x00100000, 0x00100000 );
               ok = false;
            }
            catch( ... )
            {
               ok = true;
            }

            is &= ok;
            if( pOut && isVerbose ) *pOut << "size3  " << ok << "\n";
         }

         // storage
         word awa[] = { 11, 21, 31, 12, 22, 32 };
         const dword awasw = 3;
         const dword awash = 2;
         const dword awal  = 6;
         {
            const Sheet<word> aw( awa, awasw, awash, false );

            bool ok = (awa == aw.getStorage()) & (awal == aw.getLength()) &
               (awasw == aw.getWidth()) & (awash == aw.getHeight()) &
               !aw.isOwning();
            is &= ok;
            if( pOut && isVerbose ) *pOut << "storage1  " << aw.getStorage() <<
               " " << aw.getWidth() << " " << aw.getHeight() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }
         {
            word* pawa = new word[ awal ];
            const Sheet<word> aw( pawa, awasw, awash, true );

            bool ok = (pawa == aw.getStorage()) & (awal == aw.getLength()) &
               (awasw == aw.getWidth()) & (awash == aw.getHeight()) &
               aw.isOwning();
            is &= ok;
            if( pOut && isVerbose ) *pOut << "storage2  " << aw.getStorage() <<
               " " << aw.getWidth() << " " << aw.getHeight() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }
         {
            const Sheet<word> aw( 0, 3, 2, false );

            bool ok = (0 == aw.getStorage()) & ((3 * 2) == aw.getLength()) &
               (3 == aw.getWidth()) & (2 == aw.getHeight()) &
               !aw.isOwning();
            is &= ok;
            if( pOut && isVerbose ) *pOut << "storage3  " << aw.getStorage() <<
               " " << aw.getWidth() << " " << aw.getHeight() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }
         {
            const Sheet<word> aw( 0, 3, 2, true );

            bool ok = (0 == aw.getStorage()) & ((3 * 2) == aw.getLength()) &
               (3 == aw.getWidth()) & (2 == aw.getHeight()) &
               aw.isOwning();
            is &= ok;
            if( pOut && isVerbose ) *pOut << "storage4  " << aw.getStorage() <<
               " " << aw.getWidth() << " " << aw.getHeight() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }
         {
            bool ok;
            try
            {
               const Sheet<word> aw( awa, -3, 2, false );
               ok = false;
            }
            catch( ... )
            {
               ok = true;
            }

            is &= ok;
            if( pOut && isVerbose ) *pOut << "storage5  " << ok << "\n";
         }
         {
            bool ok;
            try
            {
               const Sheet<word> aw( awa, 0x00010000, 0x01000000, false );
               ok = false;
            }
            catch( ... )
            {
               ok = true;
            }

            is &= ok;
            if( pOut && isVerbose ) *pOut << "storage6  " << ok << "\n";
         }
         {
            bool ok;
            try
            {
               word* pawa = new word[ awal ];
               const Sheet<word> aw( pawa, -3, 2, true );
               ok = false;
            }
            catch( ... )
            {
               ok = true;
            }

            is &= ok;
            if( pOut && isVerbose ) *pOut << "storage7  " << ok << "\n";
         }

         // copy
         {
            const Sheet<word> aww( awa, awasw, awash, false );
            const Sheet<word> aw( aww );

            bool ok = (aww.getStorage() != aw.getStorage()) &
               (aww.getWidth() == aw.getWidth()) &
               (aww.getHeight() == aw.getHeight()) &
               (aww.getLength() == aw.getLength()) & aw.isOwning();

            for( dword i = aw.getLength();  i-- > 0; )
            {
               ok &= (aw[i] == aww[i]);
            }
            is &= ok;
            if( pOut && isVerbose ) *pOut << "copy1  " << aw.getStorage() <<
               " " << aw.getWidth() << " " << aw.getHeight() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }
         {
            word* pawa = new word[ awal ];
            const Sheet<word> awo( pawa, awasw, awash, true );
            const Sheet<word> aw( awo );

            bool ok = (awo.getStorage() != aw.getStorage()) &
               (awo.getWidth() == aw.getWidth()) &
               (awo.getHeight() == aw.getHeight()) &
               (awo.getLength() == aw.getLength()) & aw.isOwning();
            is &= ok;
            if( pOut && isVerbose ) *pOut << "copy2  " << aw.getStorage() <<
               " " << aw.getWidth() << " " << aw.getHeight() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }
      }

      // copy assignment
      {
         {
            word awa[] = { 110, 210, 310, 120, 220, 320 };
            const dword awasw = 3;
            const dword awash = 2;
            //const dword awal  = 6;
            const Sheet<word> aww( awa, awasw, awash, false );

            Sheet<word> aw( 4, 5 );
            aw = aww;

            bool ok = (aww.getStorage() != aw.getStorage()) &
               (aww.getWidth() == aw.getWidth()) &
               (aww.getHeight() == aw.getHeight()) &
               (aww.getLength() == aw.getLength()) & aw.isOwning();
            if( pOut && isVerbose ) *pOut << "copyassign1  "<< aw.getStorage() <<
               " " << aw.getWidth() << " " << aw.getHeight() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";

            for( dword i = aw.getLength();  i-- > 0; )
            {
               ok &= (aw[i] == aww[i]);
            }
            is &= ok;
            if( pOut && isVerbose ) *pOut << "copyassign2  "<< aw.getStorage() <<
               " " << aw.getWidth() << " " << aw.getHeight() <<
               " " << aw.getLength() << " " << aw.isOwning() << "  " << ok <<
               "\n";
         }
// gcc compiled executables dont make exceptions out of storage access violations
//       {
//          bool ok;
//          try
//          {
//             const Sheet<word> aww( 0, 3, 2, false );
//             Sheet<word> aw( 4, 5 );
//             aw = aww;
//             ok = false;
//          }
//          catch( ... )
//          {
//             ok = true;
//          }
//
//          is &= ok;
//          if( pOut && isVerbose ) *pOut << "copyassign3  " << ok << "\n";
//       }
      }

      if( pOut && isVerbose ) *pOut << "\n";

      if( pOut ) *pOut << "basics : " <<
         (is ? "--- succeeded" : "*** failed") << "\n\n";

      isOk &= is;
   }


   // commands
   {
      bool is = true;

      for( dword j = 0;  j < 8;  ++j )
      {
         word wa[] = { 1100, 2100, 3100, 1200, 2200, 3200 };
         const dword waw = 3;
         const dword wah = 2;
         const dword wal = 6;
         Sheet<word> aww( wa, waw, wah, false );

         word* pwa = new word[ wal ];
         Sheet<word> awo( pwa, waw, wah, true );

         Sheet<word> awm( waw, wah );

         for( dword i = wal;  i-- > 0; )
         {
            awo[i] = wa[i];
            awm[i] = wa[i];
         }

         Sheet<word>* aws[] = { &aww, &awo, &awm };

         // using made, adopted, reffed
         for( dword a = 0;  a < 3;  ++a )
         {
            Sheet<word>& ar = *(aws[a]);

            bool ok = true;

            switch( j )
            {
            case 0 :
               // setSize
               {
                  const word* m = ar.getStorage();
                  ar.setSize( 2, 5 );
                  ok &= (0 != ar.getStorage()) & (m != ar.getStorage()) &
                     (2 == ar.getWidth()) & (5 == ar.getHeight()) &
                     (10 == ar.getLength()) & ar.isOwning();
               }
               break;

            case 1 :
               // setStorage ref
               {
                  word a[] = { 11, 21, 12, 22, 13, 23 };
                  const dword aw = 2;
                  const dword ah = 3;
                  const dword al = 6;
                  ar.setStorage( a, aw, ah, false );
                  ok &= (a == ar.getStorage()) & (al == ar.getLength()) &
                     (2 == ar.getWidth()) & (3 == ar.getHeight()) &
                     !ar.isOwning();
               }
               break;

            case 2 :
               // setStorage adopt
               {
                  const dword paw = 2;
                  const dword pah = 3;
                  const dword pal = 6;
                  word* pa = new word[ pal ];
                  ar.setStorage( pa, paw, pah, true );
                  ok &= (pa == ar.getStorage()) & (pal == ar.getLength()) &
                     (2 == ar.getWidth()) & (3 == ar.getHeight()) &
                     ar.isOwning();
               }
               break;

            case 3 :
               {
                  // operator[]
                  // getElement
                  for( dword i = wal;  i-- > 0; )
                  {
                     if( 0 == (i & 1) )
                     {
                        ar[i] = -100;
                     }
                     else
                     {
                        ar.getElement(i) = -100;
                     }
                  }
                  for( dword i = wal;  i-- > 0; )
                  {
                     ok &= (-100 == ar[i]);
                  }
               }
               break;

            case 4 :
               {
                  // get
                  // getElement
                  for( dword y = wah;  y-- > 0; )
                  {
                     for( dword x = waw;  x-- > 0; )
                     {
                        if( 0 == ((x ^ y) & 1) )
                        {
                           ar.get( x, y ) = -100;
                        }
                        else
                        {
                           ar.getElement( x, y ) = -100;
                        }
                     }
                  }
                  for( dword i = wal;  i-- > 0; )
                  {
                     ok &= (-100 == ar[i]);
                  }
               }
               break;

            case 5 :
               {
                  // getStorage
                  const word w = wa[1];
                  ar.getStorage()[1] = wa[1] - 100;
                  ok &= ((w - 100) == ar[1]);
                  ok &= (&(ar[1]) == ar.getStorage() + 1);
               }
               break;

            case 6 :
               {
                  // getRow
                  const word w = wa[ 1 + (1 * waw) ];
                  ar.getRow(1)[1] = w - 100;
                  ok &= ((w - 100) == ar[ 1 + (1 * waw) ]);
                  ok &= (&(ar[ 1 + (1 * waw) ]) == ar.getRow(1) + 1);
               }
               break;

            case 7 :
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

      word wa[] = { -1100, -2100, -3100, -1200, -2200, -3200 };
      const dword waw = 3;
      const dword wah = 2;
      const dword wal = 6;
      Sheet<word> aww( wa, waw, wah, false );

      word* pwa = new word[ wal ];
      Sheet<word> awo( pwa, waw, wah, true );

      Sheet<word> awm( waw, wah );

      for( dword i = wal;  i-- > 0; )
      {
         awo[i] = wa[i];
         awm[i] = wa[i];
      }

      Sheet<word>* aws[] = { &aww, &awo, &awm };

      // using made, adopted, reffed
      for( dword a = 0;  a < 3;  ++a )
      {
         Sheet<word>& ar = *(aws[a]);

         // getLength
         is &= (wal == ar.getLength());
         if( pOut && isVerbose ) *pOut << "getLength " << a << "  " << is <<
            "\n";

         // getWidth
         is &= (waw == ar.getWidth());
         if( pOut && isVerbose ) *pOut << "getWidth " << a << "  " << is <<
            "\n";

         // getHeight
         is &= (wah == ar.getHeight());
         if( pOut && isVerbose ) *pOut << "getHeight " << a << "  " << is <<
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

         // getRow
         is &= (0 == a) ? (wa + waw == ar.getRow(1)) : (wa + waw != ar.getRow(1));
         is &= (0 != ar.getRow(1));
         if( pOut && isVerbose ) *pOut << "getRow " << a << "  " << is <<
            "\n";

         // operator[]
         // getElement
         for( dword i = wal;  i-- > 0; )
         {
            is &= (ar[i] == wa[i]);
            is &= (ar.getElement(i) == wa[i]);
         }
         if( pOut && isVerbose ) *pOut << "get1      " << a << "  " << is <<
            "\n";

         // get
         // getElement
         for( dword y = wah;  y-- > 0; )
         {
            for( dword x = waw;  x-- > 0; )
            {
               is &= (ar.get(x,y) == wa[x + (y * waw)]);
               is &= (ar.getElement(x,y) == wa[x + (y * waw)]);
            }
         }
         if( pOut && isVerbose ) *pOut << "get2      " << a << "  " << is <<
            "\n";
      }

      is &= !Sheet<word>::isSizeWithinRange( -10, 20 ) &
         !Sheet<word>::isSizeWithinRange( 10, -20 ) &
         !Sheet<word>::isSizeWithinRange( 0x00100000, 0x00000800 );

      // getMaxSize
      is &= (DWORD_MAX == Sheet<word>::getMaxSize());
      if( pOut && isVerbose ) *pOut << "getMaxSize " << is << "\n";

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
