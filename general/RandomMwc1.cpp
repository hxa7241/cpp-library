/*------------------------------------------------------------------------------

   HXA7241 General library.
   Copyright (c) 2004-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include "RandomMwc1.hpp"


using namespace hxa7241_general;




/// standard object services ---------------------------------------------------
RandomMwc1::RandomMwc1
(
   const udword seed
)
 : x_m( seed ? seed : 30903u )
 , c_m( seed )
{
}




/// queries --------------------------------------------------------------------
udword RandomMwc1::getUdword() const
{
   // choose a value for A from this list:
   // 1075433238 1447497129 1517746329 1554115554 1557985959
   // 1606218150 1631296680 1655692410 1675393560 1683268614
   // 1781943330 1791398085 1873196400 1893513180 1929682203
   // 1965537969 1967773755 2051013963 2083801278 2131995753
   static const udword A  = 2131995753u;
   static const udword AH = A >> 16;
   static const udword AL = A & 65535;

   const udword xh = x_m >> 16;
   const udword xl = x_m & 65535;

   x_m = x_m * A + c_m;
   c_m = (xh * AH) + ((xh * AL) >> 16) + ((xl * AH) >> 16);
   // thanks to Don Mitchell for this correction:
   if( (xl * AL) >= (~c_m + 1) )   ++c_m;

   return x_m;
}


float RandomMwc1::getFloat() const
{
   return static_cast<float>(getUdword()) / 4294967296.0f;
}


/*float RandomMwc1::getFloat
(
   udword noOfBits
) const
{
   noOfBits = noOfBits <= 32 ? noOfBits : 32;

   return static_cast<float>( getUdword() >> (32 - noOfBits) ) /
      ( static_cast<float>( 1 << (noOfBits - 1) ) * 2.0f );
}*/








/*
From: George Marsaglia <g...@stat.fsu.edu>
Subject: A very fast and very good random number generator
Date: 1998/01/30
Message-ID: <34D263C0.354E67F0@stat.fsu.edu>#1/1
X-Deja-AN: 320663379
X-Priority: 3 (Normal)
Mime-Version: 1.0
Content-Type: multipart/mixed; boundary="------------E0581F7DB237E297832252E5"
Organization: Florida State University
Newsgroups: sci.math,sci.stat.math


This is a multi-part message in MIME format.
--------------E0581F7DB237E297832252E5
Content-Type: text/plain; charset=us-ascii
Content-Type: text/plain; charset=us-ascii
Content-Transfer-Encoding: 7bit
Content-Transfer-Encoding: 7bit



--------------E0581F7DB237E297832252E5
Content-Type: text/plain; charset=us-ascii; name="Mwcpost"
Content-Transfer-Encoding: 7bit
Content-Disposition: inline; filename="Mwcpost"

This newsgroup has frequent queries about good random mumber generators.
A particularly promising new method is multiply-with-carry, since it
produces 32-bit random integers that seem to pass all tests of randomness
(such as those in the Diehard battery of my random number CDROM), and is
exceptionally fast because it exploits the way modern CPU's get a 64-bit
product from two 32-bit integers.   But it must do that via machine
language.  I offer a version below, for those who may be able to use
it with Fortran on their Intel machines, or as an outline for those who
may be able to provide versions for C or other compilers or other platforms.

The following is assembler code, for Intel 386/486+ processors, that
will provide a Fortran callable routine, mwc32(), for 32-bit random integers,
based on my multiply-with-carry method.  It is extremely fast and seems to
pass all tests of randomness put to it.    It works for Microsoft Fortran,
but may work for other kinds, or for C.  I would welcome input from those
more knowledgable in assembler on making versions for C or other Fortrans.
I must confess that I wrote this by looking up rules in an assembler manual;
otherwise I know little about programming in assembly language.
To use the mwc32() subroutine, the statement j=mwc32() will make j a
signed 32-bit random integer (in 130 nanoseconds on a Pentium 120).
Two default seeds are included, but to set your own seeds, use a statement
such as k=mwcset(i1,i2), for any two seeds i1,i2.
You will have to assemble this code to provide a mwc32.obj
file, then link it when compiling your main program.   If you ask, I will
send a mwc32.obj file, but it may only work with Microsoft Fortran.
Any experts who can provide versions for C or other compilers, (or other
platforms) are welcome to contribute to this thread.
Here is the assembler code. Comment lines start with a semicolon (;).

           .486
;  The multiply-with-carry generator for 32-bit integers:
;        x(n)=a*x(n-1) + carry mod 2^32
; Choose multiplier a from this list:
;   1791398085 1929682203 1683268614 1965537969 1675393560
;   1967773755 1517746329 1447497129 1655692410 1606218150
;   2051013963 1075433238 1557985959 1781943330 1893513180
;   1631296680 2131995753 2083801278 1873196400 1554115554
; ( or any 'a' for which both a*2^32-1 and a*2^31-1 are prime)
;  This version, as it stands, uses a=2083801278 as the multiplier.
;  Replace 2083801278 with your choice of 'a' in the line: mov edx,2083801278
;  Forms a*x+c as 64 bits in the two 32-bit registers edx:eax
;  The new x is in eax, the new carry c is in edx.
;  The period is a*2^31-1 > 2^60.
;  Takes 130 nanoseconds on a Pentium 120
;  Theory is in the file mwc1.ps of the Marsaglia Random Number CDROM.
           .MODEL    FLAT, STDCALL
MWC32      PROTO     STDCALL
MWCSET     PROTO     STDCALL, xseed:ptr dword,cseed:ptr dword
           .DATA
x          DWORD    362436069
crry       DWORD    1234567
           .CODE
MWC32      PROC     STDCALL
           mov      eax,x
           mov      edx,2083801278
           mul      edx           ; a*x in edx:eax
           mov      ecx,crry      ; get crry
           add      eax,ecx       ; ax+c in eax
           adc      edx,0         ; if carry, increment edx
           mov      crry,edx      ; store new crry
           mov      x,eax         ; store new x
           ret
MWC32      ENDP
MWCSET    PROC     STDCALL,xseed:PTR DWORD,cseed:PTR DWORD
           mov      ecx, xseed
           mov      eax,[ecx]
           mov      x,eax
           mov      ecx, cseed
           mov      eax,[ecx]
           mov      crry,eax
           ret
MWCSET     ENDP
;
           END


--------------E0581F7DB237E297832252E5--
*/
