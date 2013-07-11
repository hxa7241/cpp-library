/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#ifndef Spherahedron_h
#define Spherahedron_h


#include <vector>

#include "Vector3f.hpp"




#include "hxa7241_graphics.hpp"
namespace hxa7241_graphics
{


/**
 * Triangle made of indexes into a vertex collection.
 */
struct TriangleImplicit
{
            TriangleImplicit();
   explicit TriangleImplicit( const udword vertex0,
                              const udword vertex1,
                              const udword vertex2 );

   udword vertexs_m[3];
};




/**
 * Make a sphere of triangles, in shared-vertex form.<br/><br/>
 *
 * Center at (0,0,0).<br/><br/>
 * Limited to maximum of 1,342,177,280 faces (surely that is enough for you!).
 *
 * @i_diameter    spherahedron size (larger than zero)
 * @i_maxEdgeSize maximum triangle edge size
 * @o_vertexs     vertex collection
 * @o_faces       face collection (each with three vertex indexes)
 */
void makeSpherahedron
(
   const float                    i_diameter,
   const float                    i_maxEdgeSize,
   std::vector<Vector3f>&         o_vertexs,
   std::vector<TriangleImplicit>& o_faces
);


}//namespace




#endif//Spherahedron_h
