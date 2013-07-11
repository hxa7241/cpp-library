/*------------------------------------------------------------------------------

   HXA7241 Graphics library.
   Copyright (c) 2004-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/


#include <math.h>
#include <map>
#include <set>

#include "Spherahedron.hpp"


using namespace hxa7241_graphics;




// notes:
//
// subdiv  0  1   2    3    4     5     6       8   (max) 13     (14)
//
// icosa  20 80 320 1280 5120 20480 81920 1310720 1342177280 (> 2^32)

// faceCount   = polyfaces * (4 ^ subdivision)
// vertexCount = (0.5 * faceCount) + 2
// edgeCount   = faceCount + vertexCount - 2




// module-private --------------------------------------------------------------
namespace
{

void makeIcosahedron
(
   const float                    i_radius,
   std::vector<Vector3f>&         o_vertexs,
   std::vector<TriangleImplicit>& o_faces
);

void subdivideTrianglePolyhedron
(
   const float                    i_radius,
   const float                    i_maxEdgeSize,
   std::vector<Vector3f>&         o_vertexs,
   std::vector<TriangleImplicit>& o_faces
);


struct Edge
{
   Edge( udword vertexIndex0,
         udword vertexIndex1 );

   udword operator[]( const int ) const;
   bool   operator<( const Edge& other ) const;

   udword vertexIndexs_m[2];
};

Edge::Edge
(
   const udword one,
   const udword two
)
{
   // consistent ordering
   vertexIndexs_m[0] = one <= two ? one : two;
   vertexIndexs_m[1] = one <= two ? two : one;
}

udword Edge::operator[]
(
   const int index
) const
{
   return vertexIndexs_m[ index & 1 ];
}

bool Edge::operator<
(
   const Edge& other
) const
{
   return vertexIndexs_m[1] != other.vertexIndexs_m[1] ?
      vertexIndexs_m[1] < other.vertexIndexs_m[1] :
      vertexIndexs_m[0] < other.vertexIndexs_m[0];
}

}




namespace hxa7241_graphics
{


// supporting types ------------------------------------------------------------
TriangleImplicit::TriangleImplicit()
{
   vertexs_m[0] = vertexs_m[1] = vertexs_m[2] = 0;
}


TriangleImplicit::TriangleImplicit
(
   const udword vertex0,
   const udword vertex1,
   const udword vertex2
)
{
   vertexs_m[0] = vertex0;
   vertexs_m[1] = vertex1;
   vertexs_m[2] = vertex2;
}




// implementation --------------------------------------------------------------
void makeSpherahedron
(
   const float                    i_diameter,
   const float                    i_maxEdgeSize,
   std::vector<Vector3f>&         o_vertexs,
   std::vector<TriangleImplicit>& o_faces
)
{
   // check diameter more than zero
   if( i_diameter > 0.0f )
   {
      const float radius = i_diameter * 0.5f;

      // condition maxEdgeSize
      const float maxEdgeSize = i_maxEdgeSize < 0.0f ? 0.0f :
         (i_maxEdgeSize > (radius * 2.0f) ? (radius * 2.0f) : i_maxEdgeSize);

      // execute
      makeIcosahedron( radius, o_vertexs, o_faces );
      subdivideTrianglePolyhedron( radius, maxEdgeSize, o_vertexs, o_faces );
   }
   // empty result for invalid input
   else
   {
      o_vertexs.clear();
      o_faces.clear();
   }
}


}//namespace




namespace
{


void makeIcosahedron
(
   const float                    i_radius,
   std::vector<Vector3f>&         o_vertexs,
   std::vector<TriangleImplicit>& o_faces
)
{
   // icosahedron definition (produces most regular subdivision)
   static const float  ICOSAHEDRON_VERTEXS[12][3] = {
      { 1.61803398875f, 1, 0}, {-1.61803398875f, 1, 0}, {1.61803398875f, -1, 0},
      {-1.61803398875f, -1, 0}, {1, 0, 1.61803398875f}, {1, 0, -1.61803398875f},
      {-1, 0, 1.61803398875f}, {-1, 0, -1.61803398875f}, {0, 1.61803398875f, 1},
      {0, -1.61803398875f, 1}, {0, 1.61803398875f, -1}, {0, -1.61803398875f, -1}
      };
   static const udword ICOSAHEDRON_FACES[20][3] = {
      { 0,  4, 2 }, { 0,  2, 5 }, { 0, 8,  4 }, { 0,  5, 10 }, { 0, 10,  8 },
      { 1,  3, 6 }, { 1,  7, 3 }, { 1, 6,  8 }, { 1, 10,  7 }, { 1,  8, 10 },
      { 2,  4, 9 }, { 2, 11, 5 }, { 2, 9, 11 }, { 3,  9,  6 }, { 3,  7, 11 },
      { 3, 11, 9 }, { 4,  8, 6 }, { 4, 6,  9 }, { 5,  7, 10 }, { 5, 11,  7 } };
   static const dword ICOSAHEDRON_VERTEX_COUNT = 12;
   static const dword ICOSAHEDRON_FACE_COUNT   = 20;

   // cannot delete these: One day I will want a tetrahedron or octahedron, and
   // remember they were in the spherahedron code, so if I arrive and find them
   // gone it will be exceedingly annoying.
   //
   // (faces might not all have same orientation)
   //
   //static const float  TETRAHEDRON_VERTEXS[4][3] = {
   //   {+1, +1, +1}, {-1, -1, +1}, {-1, +1, -1}, {+1, -1, -1} };
   //static const udword TETRAHEDRON_FACES[4][3]   = {
   //   { 0,  1,  2}, { 0,  1,  3}, { 0,  2,  3}, { 1,  2,  3} };
   //static const dword TETRAHEDRON_VERTEX_COUNT   = 4;
   //static const dword TETRAHEDRON_FACE_COUNT     = 4;
   //
   //static const float  OCTAHEDRON_VERTEXS[6][3]  = {
   //   {+1, 0, 0}, {-1, 0, 0}, {0, +1, 0}, {0, -1, 0}, {0, 0, +1}, {0, 0, -1}};
   //static const udword OCTAHEDRON_FACES[8][3]    = {
   //   { 0, 2, 4 }, { 0, 2, 5 }, { 0, 3, 4 }, { 0, 3, 5 },
   //   { 1, 2, 4 }, { 1, 2, 5 }, { 1, 3, 4 }, { 1, 3, 5 } };
   //static const dword OCTAHEDRON_VERTEX_COUNT    = 6;
   //static const dword OCTAHEDRON_FACE_COUNT      = 8;

   // copy to vectors
   {
      // make scaling factor
      const float scaling = i_radius / Vector3f( ICOSAHEDRON_VERTEXS[0]
         ).length();

      // reset vectors
      o_vertexs.resize( 0 );
      o_faces.resize( 0 );

      // vertexs
      for( int i = 0;  i < ICOSAHEDRON_VERTEX_COUNT;  ++i )
      {
         o_vertexs.push_back( Vector3f( ICOSAHEDRON_VERTEXS[i] ) *= scaling );
      }

      // faces
      for( int i = 0;  i < ICOSAHEDRON_FACE_COUNT;  ++i )
      {
         o_faces.push_back( TriangleImplicit( ICOSAHEDRON_FACES[i][0],
            ICOSAHEDRON_FACES[i][1], ICOSAHEDRON_FACES[i][2] ) );
      }
   }
}


void subdivideTrianglePolyhedron
(
   const float                    i_radius,
   const float                    i_maxEdgeSize,
   std::vector<Vector3f>&         o_vertexs,
   std::vector<TriangleImplicit>& o_faces
)
{
   // iteratively: subdivide all triangle faces into four, and each edge into
   // two, putting new vertexs on circumsphere.
   // (Note: doesn't produce equal size faces.)

   // for edge and associated new middle vertex
   std::map<Edge, udword> edges;

   // iterate until i_maxEdgeSize threshold reached, or maximum collection sizes
   // reached (edge count = 1.5 * face count)
   for( ; ; )
   {
      // get current max edge size
      // (requires that each face is overwritten with its new middle sub-face
      // at each iteration, therefore always having the longest edges)
      const float maxEdgeSize = ( o_vertexs[o_faces[0].vertexs_m[1]] -
         o_vertexs[o_faces[0].vertexs_m[0]] ).length();

      // check if need to stop subdivision iteration
      // (being aware of uint overflow)
      if( (edges.size() > (UDWORD_MAX / 4)) |
         ((edges.size() * 4) > edges.max_size()) |
         (o_faces.size() > (UDWORD_MAX / 4)) |
         ((o_faces.size() * 4) > o_faces.max_size()) |
         (maxEdgeSize <= i_maxEdgeSize) )
      {
         break;
      }

      // reset edges
      edges.clear();

      // make new subdivided edges and new vertexs
      // -- step through all edges, make subdivided edge vertex, append it to
      // output vertexs, append its index associated with edge to edge map

      // step through faces
      for( int f = o_faces.size();  f-- > 0; )
      {
         const udword* vertIndexs = o_faces[f].vertexs_m;

         // step through edges
         for( int v = 3;  v-- > 0; )
         {
            // make edge
            const Edge edge( vertIndexs[v], vertIndexs[(v + 1) % 3] );

            // ensure edge not already done
            if( edges.end() == edges.find( edge ) )
            {
               // make and append middle vertex
               const Vector3f middleVertex( (o_vertexs[edge[0]] +
                  o_vertexs[edge[1]]).unitize() * i_radius );
               o_vertexs.push_back( middleVertex );

               // append edge and associated new vertex index
               edges[ edge ] = static_cast<udword>( o_vertexs.size() - 1 );
            }
         }
      }

      // make new subdivided faces
      // -- step through each face's vertexs, retrieve adjacent new subdivided
      // edge vertexs from edge map, make new face from them, overwrite old face
      // with new face made of all new vertexs

      // step through faces
      for( int f = 0, end = o_faces.size();  f < end;  ++f )
      {
         udword middles[3];

         // step through vertexs, making corner pieces
         for( int v = 3;  v-- > 0; )
         {
            const udword* vertIndexs = o_faces[f].vertexs_m;

            // make new face, with corner and two subdivided edge vertexs
            const TriangleImplicit corner(
               edges[ Edge( vertIndexs[(v + 2) % 3], vertIndexs[v] ) ],
               vertIndexs[v],
               edges[ Edge( vertIndexs[v], vertIndexs[(v + 1) % 3] ) ] );

            // append new face
            o_faces.push_back( corner );

            middles[v] = corner.vertexs_m[0];
         }

         // make center piece with subdivided edge vertexs, and overwrite old
         // face
         o_faces[f] = TriangleImplicit( middles[0], middles[1], middles[2] );
      }
   }

   // trim output reserves
   std::vector<Vector3f>( o_vertexs ).swap( o_vertexs );
   std::vector<TriangleImplicit>( o_faces ).swap( o_faces );
}


}//namespace








/// test -----------------------------------------------------------------------
#ifdef TESTING


#include <iostream>


namespace hxa7241_graphics
{


bool test_Spherahedron
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   //seed
)
{
   bool isOk = true;

   if( pOut ) *pOut << "[ test_Spherahedron ]\n\n";


   using std::vector;


   // make spherahedron
   const float              diameter    = 103.7f;
   const float              maxEdgeSize = 1.2f;
   vector<Vector3f>         vertexs;
   vector<TriangleImplicit> faces;

   makeSpherahedron( diameter, maxEdgeSize, vertexs, faces );


   // general mesh validity (single, closed, non-intersecting surface)
   {
      bool isOk1 = true;

      // triangle euler relation (V - 0.5F = 2*(1 - Genus))
      {
         // V - 0.5F should be an even integer and <= 2
         // F should be an even integer
         const udword vmhf = vertexs.size() - (faces.size() / 2);
         const bool isTe = (0 == ((vmhf & 1) | static_cast<udword>(vmhf > 2) |
            (faces.size() & 1)));

         if( pOut && isVerbose ) *pOut << "triangle-Euler  " << "F: " <<
            faces.size() << "  V: " << vertexs.size() << "  V - 0.5F: " <<
            vmhf << "  " << isTe << "\n";

         isOk1 &= isTe;
      }

      // face edge relation (and get edge count)
      udword edgeCount = 0;
      {
         // every edge should be shared by two faces

         // build edge collection
         std::multiset<Edge> edges;
         // step through faces
         for( int f = faces.size();  f-- > 0; )
         {
            const udword* vertIndexs = faces[f].vertexs_m;

            // step through edges
            for( int v = 3;  v-- > 0; )
            {
               // add edge to collection
               edges.insert( Edge( vertIndexs[v], vertIndexs[(v + 1) % 3] ) );
            }
         }
         edgeCount = edges.size() / 2;

         // check edge counts
         bool isFe = true;
         for( std::multiset<Edge>::const_iterator it = edges.begin();
            (it != edges.end()) & isFe;  ++it )
         {
            isFe &= (2 == edges.count( *it ));
         }

         if( pOut && isVerbose ) *pOut << "face-edge  " << "E: " <<
            edgeCount << "  " << isFe << "\n";

         isOk1 &= isFe;
      }

      // general euler relation (F + V - E = 2*(1 - Genus))
      {
         // F + V - E should be an even integer and <= 2
         // (detect: multiple objects, unclosed, dangling pieces)
         const udword fpvme = faces.size() + vertexs.size() - edgeCount;
         const bool isGe = (0 == ((fpvme & 1) | static_cast<udword>(fpvme >
            2)));

         if( pOut && isVerbose ) *pOut << "general Euler  " << "F: " <<
            faces.size() << "  V: " << vertexs.size() << "  F + V - E: " <<
            fpvme << "  " << isGe << "\n";

         isOk1 &= isGe;
      }

      // self intersection
      {
         // (oh well, maybe later sometime)
         // ...
      }

      if( pOut && isVerbose ) *pOut << "\n";

      if( pOut ) *pOut << "general mesh validity : " <<
         (isOk1 ? "--- succeeded" : "*** failed") << "\n\n";

      isOk &= isOk1;
   }


   // sphere requirements fulfillment
   {
      bool isOk2 = true;

      const float radius = diameter * 0.5f;

      // vertexs are near enough sphere surface
      {
         bool isR = true;

         float maxInaccuracy = 0.0f;
         // step through vertexs
         for( udword v = vertexs.size();  (v-- > 0) & isR; )
         {
            // get distance of vertex to center
            const float distanceToCenter = vertexs[v].length();

            // compare to radius
            const float inaccuracy = ::fabsf(radius - distanceToCenter);
            maxInaccuracy = maxInaccuracy >= inaccuracy ? maxInaccuracy :
               inaccuracy;
            isR &= (inaccuracy < (radius * 0.0001f));
         }

         if( pOut && isVerbose ) *pOut << "radius  " << "maxInaccuracy: " <<
            maxInaccuracy << "  " << isR << "\n";

         isOk2 &= isR;
      }

      // edge lengths within range
      {
         bool isE = true;

         // get min and max edge lengths
         float min = FLOAT_MAX;
         float max = FLOAT_MIN_NEG;

         // step through faces
         for( udword f = faces.size();  f-- > 0; )
         {
            const udword* vertIndexs = faces[f].vertexs_m;

            // step through edges (doing each edge twice overall -- ok)
            for( udword v = 3;  v-- > 0; )
            {
               // get length
               const float length = (vertexs[vertIndexs[v]] -
                  vertexs[vertIndexs[(v + 1) % 3]]).length();

               // update min and max
               min = min <= length ? min : length;
               max = max >= length ? max : length;
            }
         }

         const float maxRatio = max / maxEdgeSize;
         const float minRatio = min / maxEdgeSize;
         const float ratio    = max / min;

         // edge max length <= required value
         isE &= (max <= maxEdgeSize);
         // edge max length < twice edge min length
         isE &= (ratio < 2.0f);

         if( pOut && isVerbose ) *pOut << "edge  " << "maxRatio: " <<
            maxRatio << "  minRatio: " << minRatio << "  ratio: " << ratio <<
            "  " << isE << "\n";

         isOk2 &= isE;
      }

      // faces all same orientation
      {
         udword signSum = 0;

         // step through faces
         for( udword f = faces.size();  f-- > 0; )
         {
            const udword* vertIndexs = faces[f].vertexs_m;

            // make normal
            const Vector3f edge1( vertexs[vertIndexs[1]] -
               vertexs[vertIndexs[0]] );
            const Vector3f edge2( vertexs[vertIndexs[2]] -
               vertexs[vertIndexs[1]] );
            const Vector3f normal( edge1.cross( edge2 ).unitizeEq() );

            // dot normal with a vertex - center
            const bool orientation = 0.0f <= normal.dot(
               vertexs[vertIndexs[0]] );

            // sum
            signSum += static_cast<udword>( orientation );
         }

         // are all dots same sign
         const bool isO = ((0 == signSum) | (faces.size() == signSum));

         if( pOut && isVerbose ) *pOut << "orientation  " << "signSum: " <<
            signSum << "  " << isO << "\n";

         isOk2 &= isO;
      }

      if( pOut && isVerbose ) *pOut << "\n";

      if( pOut ) *pOut << "sphere requirements fulfillment : " <<
         (isOk2 ? "--- succeeded" : "*** failed") << "\n\n";

      isOk &= isOk2;
   }


   if( pOut ) *pOut << (isOk ? "--- successfully" : "*** failurefully") <<
      " completed " << "\n\n\n";

   if( pOut ) pOut->flush();


   return isOk;
}


}//namespace


#endif//TESTING
