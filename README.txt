

HXA7241 C++ Library v200712
======================================================================


Harrison Ainsworth / HXA7241 : 2004-2007.  
http://wwww.hxa7241.org/rendering

2007-12-26




Contents
--------

* Description
* Module List
* Acknowledgements
* License




Description
-----------

A collection of quite reusable graphics-oriented C++ code. Fairly refined as
individual parts, but the package as a whole is a bit casual.

Comprising 29 modules, and about 8500 lines. Mostly classes, but several
functions too. Not all tests are included.




Module List
-----------

### General ###

Standalone:
* FpToInt       -- Int-float conversion functions.
* Clamps        -- Functions to clamp numerics between bounds.
* Bounded       -- Scalar value within a restricted range.
* Float01       -- Float value between 0 and 1, open or closed interval.
* hxa7241vector -- std::vector storage manipulation functions.
* Array         -- Simpler, compacter alternative to std::vector.
* RandomMwc1    -- Simple, fast, good random number generator, v1.
* RandomMwc2    -- Simple, fast, good random number generator, v2.
* LogFast       -- Fast approximation to log.
* PowFast       -- Fast approximation to pow.

Dependent:
* Sheet         -- A simple 2D dynamic array, suitable for images.


### Graphics ###

Standalone:
* ColorConstants -- Standard and common color constants.
* Filters        -- Some 2D radial filter functions.
* Polar          -- Disk-square mapping, and coordinate conversion functions.
* HaltonSequence -- Halton sequence, with optional jittering.
* SobolSequence  -- One dimension of a sobol sequence (choice of 40).
* Coord2         -- 2D coordinate class template.
* Vector3f       -- Yes, its the 3D vector class!.
* XyzE           -- Greg Ward-Larson realpixel, for any float triplet.

Dependent:
* ColorSpace     -- Trichromatic color space, with conversions to/from XYZ.
* UnitVector3f   -- Unitized 3D vector.
* Vector4f       -- 4D vector of floats.
* Matrix3f       -- 3D matrix of floats, with extra translation capability.
* Matrix4f       -- 4D matrix of floats.
* Quaternion     -- Unit quaternion.
* Spherahedron   -- Make a sphere of triangles, in shared-vertex form.


### Image ###

Standalone:
* ppm        -- IO for the nice-and-simple PPM format.
* rgbe       -- IO for the Radiance RGBE format.
* quantizing -- Conversions between integer and float [0,1] triplet images.




Acknowledgements
----------------

### Techniques and code ###

* Gaussian Radial 2D Filter.  
  'Convienient Anti Aliasing Filters That Minimize Bumpy Sampling' (p144-p146)  
  Pavicic;  
  Graphics Gems 1, AP 1990.

* Gaussian Radial 2D Filter.  
  'Stochastic Sampling In Computer Graphics'  
  Cook;  
  ACM Transactions On Graphics, Jan 1986.

* Quadratic Spline Radial 2D Filter.  
  'An Optimal Filter For Image Reconstruction' (p101-p104)  
  Max;  
  Graphics Gems 2, AP 1991.

* Cubic Spline Radial 2D Filter.  
  'Reconstruction Filters In Computer Graphics'  
  Mitchell, Netravali;  
  Siggraph Conference Proceedings 1988.

* Halton sequence.  
  'Sampling with Hammersley and Halton Points'  
  Wong, Luk, Heng;  
  Journal of Graphics Tools, vol2, no2, 1997.

* Matrix inversion code.  
  'MGF'  
  Graphics Gems.

* Filtering.
  'Principles Of Digital Image Synthesis' ch10  
  Glassner;  
  Morgan Kaufmann 1995.

* 'A Low Distortion Map Between Disk And Square'  
  Shirley, Chiu;  
  Journal Of Graphics Tools, v2n3p45 1997.

* 'The Matrix and Quaternions FAQ'  
  http://www.j3d.org/matrix_faq/matrfaq_latest.html

* 'CS184: Using Quaternions to Represent Rotation'  
  http://www-inst.eecs.berkeley.edu/~cs184/

* 'GSL - GNU Scientific Library' v1.3  
  http://www.gnu.org/software/gsl/

* RGBE pixel format.  
  'Real Pixels'  
  Greg Ward/Larson;  
  Graphics Gems II (AP 91).

* RGBE image format.  
  http://radsite.lbl.gov/radiance/refer/filefmts.pdf

* PPM image format.  
  http://netpbm.sourceforge.net/doc/ppm.html

* Random number generator (simple and fast but good).  
  Glenn Rhoads;  
  http://web.archive.org/web/20050213041650/http://
  paul.rutgers.edu/~rhoads/Code/code.html

* Fast log2 approximation adapted from:  
  Laurent de Soras, 2001.  
  <http://flipcode.com/cgi-bin/fcarticles.cgi?show=63828>

* Fast log with adjustable accuracy, adapted from:  
  'Revisiting a basic function on current CPUs: A fast logarithm implementation
  with adjustable accuracy'  
  Oriol Vinyals, Gerald Friedland, Nikki Mirghafori;  
  ICSI,  
  2007-06-21.

* Fast pow approximation adapted from:  
  'A Fast, Compact Approximation of the Exponential Function'  
  Nicol N. Schraudolph;  
  Technical Report IDSIA-07-98,  
  IDSIA,  
  1998-06-24.


### Tools ###

* TextPad 4.7.3 editor  
  http://www.textpad.com/
* MS Visual C++ 2005 compiler for Microsoft Windows  
  http://msdn.microsoft.com/vstudio/express/visualc/
* GCC 3.3.5 compiler for Suse GNU/Linux  
  http://gcc.gnu.org/




License
-------

### (New) BSD ###

Harrison Ainsworth / HXA7241 : 2004-2007.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.
* The name of the author may not be used to endorse or promote products derived
  from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
