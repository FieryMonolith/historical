//
// Name :         RayIntersection.cpp
// Description :  Implementation of CRayIntersection class.
//                This is an implementation of a Cleary and Wyvill ray walking
//                algorithm for polygon intersection with enhancements by Owen
// Author :       Charles B. Owen
// Version :      1.01  4-03-01 Thread safe
//

#include "stdafx.h"
#include <cassert>
#include "RayIntersection.h"

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// 
// To use:
//
// 1.  Call Initialize() to clear the system.
// 2.  Call Material() to set a pointer to the current material property
// 3.  Add polygons to the system:
//     A.  Call PolygonBegin()
//     B.  Call Vertex() to add vertices for the polygon
//         Call Texture() to specify a texture for the polygon
//         Call Normal() to specify a normal for the polygon
//         Call TexVertex() to specify a vertex for the polygon
//     C.  Call PolygonEnd()
// 4.  Call LoadingComplete()
// 5.  Call Intersect() to test for intersections
// 6.  Call IntersectInfo() to get intersection information for rendering
//


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRayIntersection::CRayIntersection()
{
   m_subdivide = 50;    // 125,000 blocks
   // m_subdivide = 5; //15 blocks
   m_blocks = NULL;
   m_fast = true;
}

CRayIntersection::~CRayIntersection()
{
   DeleteBlocks();
}

//
// Name :         CRayIntersection::Initialize()
// Description :  Initialize the system to begin receiving polygons.
//

void CRayIntersection::Initialize()
{
   DeleteBlocks();

}


//
// Name :         CRayIntersection::PolygonBegin()
// Description :  Begin the creation of a new polygon in the intersection system.
//                This is the first step in the polygon creation process.
//

void CRayIntersection::PolygonBegin()
{
   m_polys.push_back(Polygon()); 
}


//
// Name :         CRayIntersection::Texture()
// Description :  Assign a texture to the current polygon we are creating.
//

void CRayIntersection::Texture(CGrTexture *p_texture)
{
   assert(m_polys.size() > 0);
   m_polys.back().m_texture = p_texture;
}

//
// Name :         CRayIntersection::Normal()
// Description :  Add a normal to the current polygon we are creating.
//

void CRayIntersection::Normal(const CGrPoint &p_normal)
{
   assert(m_polys.size() > 0);
   m_polys.back().m_normals.push_back(p_normal);
}

//
// Name :         CRayIntersection::TexVertex()
// Description :  Add a texture vertex to the current polygon we are creating.
//

void CRayIntersection::TexVertex(const CGrPoint &p_tvertex)
{
   assert(m_polys.size() > 0);
   m_polys.back().m_tvertices.push_back(p_tvertex);
}


//
// Name :         CRayIntersection::Vertex()
// Description :  Add a vertex to the current polygon we are creating.
//

void CRayIntersection::Vertex(const CGrPoint &p_vertex)
{
   assert(m_polys.size() > 0);
   m_polys.back().m_vertices.push_back(p_vertex);
}

//
// Name :         CRayIntersection::Material()
// Description :  Set the material for the current polgon we are creating.
//

void CRayIntersection::Material(CGrMaterial *p_material)
{
   assert(m_polys.size() > 0);
   m_polys.back().m_material = p_material; 
}

//
// Name :         CRayIntersection::PolygonEnd()
// Description :  Indicate the end of a polygon creation.
//

void CRayIntersection::PolygonEnd()
{
   assert(m_polys.size() > 0);
   Polygon &p = m_polys.back();

   // The following error checks make sure we 
   // have valid polygon configurations.  This avoids
   // having to error check in later code.

   // We must have at least 3 vertices.
   assert(p.m_vertices.size() >= 3);

   // Other error checks
   // We allow one normal or a normal for every vertex.  
   // No other options are valid..
   assert(p.m_normals.size() == 1 || (p.m_normals.size() == p.m_vertices.size()));

   // If we have a texture, we have a texture vertex for every vertex
   assert(!p.m_texture || p.m_tvertices.size() == p.m_vertices.size());
   

   // We need a surface normal for intersection testing 
   // independent of the polygon normals.
   vector<CGrPoint>::iterator a = p.m_vertices.begin();
   vector<CGrPoint>::iterator b = a;     b++;
   vector<CGrPoint>::iterator c = b;     c++;

   p.m_normal = Normalize3(Cross3(*c - *b, *a - *b));

   // Compute d
   p.m_d = -Dot3(*a, p.m_normal);

   // My polygon intersection test requires a set of edge normals.
   // These are vectors that are normal to the edge and to the
   // surface normal and point into the interior of the polygon.
   a = p.m_vertices.begin();
   b = a;  b++;

   for(; a != p.m_vertices.end();  a++)
   {
      p.m_enormals.push_back(Normalize3(Cross3(p.m_normal, *b - *a)));

      b++;
      if(b == p.m_vertices.end())
         b = p.m_vertices.begin();
   }

   // It helps in later code if the first vertex is also
   // the last vertex.  Then we can treat the list of 
   // vertices as a list of edges.
   p.m_vertices.push_back(*p.m_vertices.begin());

   // Same for the normals if there are more than one
   if(p.m_normals.size() > 1)
      p.m_normals.push_back(*p.m_normals.begin());

   // Same for texture vertices
   if(p.m_texture)
      p.m_tvertices.push_back(*p.m_tvertices.begin());
}

//
// Name :         CRayIntersection::Intersect()  
// Description :  This is the actual ray intersection test.  
// Parameters :   p_ray - The ray we are testing against the scene.
//                p_ignore - Optional point to some object we'll ignore
//                 when testing for intersections.
//                p_nearest - Location to put a pointer to the object
//                 we intersected with.
//                p_t - Location to put the distance to the object.
// Returns :      true for insertion or false if none.
//

bool CRayIntersection::Intersect(const CRay &p_ray, const Object *p_ignore, 
                                 Object *&p_nearest, double &p_t, CGrPoint &p_intersect) const
{
   double nearestt = 1e20;    // Big, big
   Polygon *nearest = NULL;   // None, yet.

   if(m_fast)
   {
      // Cleary and Wyvill ray walking algorithm implementation

      // We need the ray parameters in the block space for this algorithm
      // Ray starting location
      double scalex = m_blocksdim / (m_max.X() - m_min.X());
      double scaley = m_blocksdim / (m_max.Y() - m_min.Y());
      double scalez = m_blocksdim / (m_max.Z() - m_min.Z());

      CGrPoint rs((p_ray.Origin().X() - m_min.X()) * scalex,
                  (p_ray.Origin().Y() - m_min.Y()) * scaley,
                  (p_ray.Origin().Z() - m_min.Z()) * scalez);

      // Ray direction
      // Note:  no longer normalized!
      CGrPoint rd(p_ray.Direction().X() * scalex,
                  p_ray.Direction().Y() * scaley,
                  p_ray.Direction().Z() * scalez);

      // We need the ray to be in the volume.  If it's not, we'll compute the
      // intersection with the volume and start there.
      if(rs.X() < 0)
      {
         // We're to the left of the volume
         if(rd.X() <= 0)
            return false;

         rs += rd * (-rs.X() / rd.X());
      }
      else if(rs.X() > m_blocksdim)
      {
         // We're to the left of the volume
         if(rd.X() >= 0)
            return false;

         rs += rd * ((m_blocksdim - rs.X()) / rd.X());
      }

      if(rs.Y() < 0)
      {
         // We're to the left of the volume
         if(rd.Y() <= 0)
            return false;

         rs += rd * (- rs.Y() / rd.Y());
         if(rs.X() < 0 || rs.X() > m_blocksdim)
            return false;
      }
      else if(rs.Y() > m_blocksdim)
      {
         // We're to the left of the volume
         if(rd.Y() >= 0)
            return false;

         rs += rd * ((m_blocksdim - rs.Y()) / rd.Y());
         if(rs.X() < 0 || rs.X() > m_blocksdim)
            return false;
      }

      if(rs.Z() < 0)
      {
         // We're to the left of the volume
         if(rd.Z() <= 0)
            return false;

         rs += rd * (- rs.Z() / rd.Z());
         if(rs.X() < 0 || rs.X() > m_blocksdim ||
            rs.Y() < 0 || rs.Y() > m_blocksdim)
            return false;
      }
      else if(rs.Z() > m_blocksdim)
      {
         // We're to the left of the volume
         if(rd.Z() >= 0)
            return false;

         rs += rd * ((m_blocksdim - rs.Z()) / rd.Z());
         if(rs.X() < 0 || rs.X() > m_blocksdim ||
            rs.Y() < 0 || rs.Y() > m_blocksdim)
            return false;
      }

      //
      // If we got to here, we know we are on one of the faces
      // or inside the block.
      //

      double dx, dy, dz;
      double deltax, deltay, deltaz;
      int idx, idy, idz;

      // We set the value of dx, dy, dz to distance to the next pending intersection with
      // a cell boundary in each dimension.
      if(rd.X() == 0)
         dx = 1e20;        // Big, big...  (infinite step)
      else
      {
         deltax = (rd / rd.X()).Length3();
         if(rd.X() < 0)
         {
            dx = fmod(rs.X(), 1) * deltax;
            idx = -1;
         }
         else
         {
            dx = (1. - fmod(rs.X(), 1)) * deltax;
            idx = 1;
         }

      }

      if(rd.Y() == 0)
         dy = 1e20;        // Big, big...  (infinite step)
      else
      {
         deltay = (rd / rd.Y()).Length3();
         if(rd.Y() < 0)
         {
            dy = fmod(rs.Y(), 1) * deltay;
            idy = -1;
         }
         else
         {
            dy = (1. - fmod(rs.Y(), 1)) * deltay;
            idy = 1;
         }

      }

      if(rd.Z() == 0)
         dz = 1e20;        // Big, big...  (infinite step)
      else
      {
         deltaz = (rd / rd.Z()).Length3();
         if(rd.Z() < 0)
         {
            dz = fmod(rs.Z(), 1) * deltaz;
            idz = -1;
         }
         else
         {
            dz = (1. - fmod(rs.Z(), 1)) * deltaz;
            idz = 1;
         }

      }


      // Determine the cell we are in
      int ix = int(rs.X());
      int iy = int(rs.Y());
      int iz = int(rs.Z());

      double nearestd = 1e20;

      //
      // This is the actual ray walking loop
      //

      for( ; ; )
      {
         //
         // Test cell ix, iy, iz
         //

         vector<Polygon *> &cell = m_blocks[iz][iy][ix];
         vector<Polygon *>::iterator poly = cell.begin();
         for( ; poly != cell.end();  poly++)
         {
            // Is this the polygon we will ignore?
            Polygon *p = *poly;

            if(p == p_ignore)      // Polygon we can ignore?
               continue;

            // What's the t value here?  Note that we're using
            // an actual t, not block space t values.
            double bottom = Dot3(p->m_normal, p_ray.Direction());
            if(bottom >= -0.000000001 && bottom <= 0.000000001)
               continue;

            double t = -(Dot3(p->m_normal, p_ray.Origin()) + p->m_d) / bottom;

            // If the intersection is farther away than what we're doing now,
            // we can ignore it.
            if(t <= 0. || t >= nearestt)
               continue;

            // What's the intersection point?
            CGrPoint intersect = p_ray.Origin() + p_ray.Direction() * t;

            // Interior test for this point
            list<CGrPoint>::iterator n=p->m_enormals.begin();
            vector<CGrPoint>::iterator v=p->m_vertices.begin();

            for( ; n!=p->m_enormals.end();  n++, v++)
               {
                  if(Dot3(*n, intersect - *v) < 0)
                     break;
               }

            if(n!=p->m_enormals.end())
               continue;

            nearestt = t;
            nearest = p;

            // What is t in the block space?
            // Convert the intersection to block space and how far is it?
            nearestd = (CGrPoint((intersect.X() - m_min.X()) * scalex,
                                 (intersect.Y() - m_min.Y()) * scaley,
                                 (intersect.Z() - m_min.Z()) * scalez) - rs).Length3();
         }

         // 
         // Increment values
         //

         if(dx <= dy)
         {
            if(dx <= dz)
            {
               if(dx > nearestd)
                  break;

               // Nearest intersection is with an x location
               dx += deltax;
               ix += idx;
               if(ix < 0 || ix > m_blocksdim)
                  break;
            }
            else
            {
               if(dz > nearestd)
                  break;

               dz += deltaz;
               iz += idz;
               if(iz < 0 || iz > m_blocksdim)
                  break;
            }
         }
         else
         {
            if(dy <= dz)
            {
               if(dy > nearestd)
                  break;

               dy += deltay;
               iy += idy;
               if(iy < 0 || iy > m_blocksdim)
                  break;
            }
            else
            {
               if(dz > nearestd)
                  break;

               dz += deltaz;
               iz += idz;
               if(iz < 0 || iz > m_blocksdim)
                  break;
            }

         }

      }
   }
   else
   {
      // Initial brute-force implementation.  
      // Simply loops over every polygon
      list<Polygon>::const_iterator poly = m_polys.begin();
      for( ; poly!=m_polys.end();  poly++)
      {
         // Is this the polygon we will ignore?
         if(&(*poly) == p_ignore)
            continue;

         const Polygon *p = &(*poly);

         // What's the t value here?
         double bottom = Dot3(p->m_normal, p_ray.Direction());
         if(bottom >= -0.000000001 && bottom <= 0.000000001)
            continue;

         double t = -(Dot3(p->m_normal, p_ray.Origin()) + p->m_d) / bottom;

         // If the intersection is farther away than what we're doing now,
         // we can ignore it.
         if(t <= 0. || t >= nearestt)
            continue;

         // What's the intersection point?
         CGrPoint intersect = p_ray.Origin() + p_ray.Direction() * t;

         // Interior test for this point
         list<CGrPoint>::const_iterator n=poly->m_enormals.begin();
         vector<CGrPoint>::const_iterator v=poly->m_vertices.begin();

         for( ; n!=poly->m_enormals.end();  n++, v++)
            {
               if(Dot3(*n, intersect - *v) < 0)
                  break;
            }

         if(n!=poly->m_enormals.end())
            continue;

         nearestt = t;
         nearest = (CRayIntersection::Polygon *)p;
      }
   }

   if(nearest)
   {
      p_nearest = nearest;
      p_t = nearestt;
      p_intersect = p_ray.Origin() + p_ray.Direction() * p_t;

      return true;
   }
   else
      return false;
}


//
// Name :         CRayIntersection::IntersectInfo()
// Description :  Given a polygon that we have intersected with, return
//                intersection information.  
// Input Parms :  p_ray - The ray we are intersecting with.
//                p_object - Pointer to the intersting object (from CRayIntersection::Intersect)
//                p_t - Distance to the intersection point.
// Output Parms : p_normal - The polygon normal at the intersection.
//                p_material - Pointer to the material for this polygon
//                p_texture - Pointer to the texture for this polygon (or NULL)
//                p_texcoord - The texture coordinate at the intersection
//

void CRayIntersection::IntersectInfo(const CRay &p_ray, const Object *p_object, double p_t, 
                      CGrPoint &p_normal, CGrMaterial *&p_material, 
                      CGrTexture *&p_texture, CGrPoint &p_texcoord) const
{
   // I created this stucture so I can have different primitives.
   // Since the only primitive at this time is a polygon, we just
   // do an assertion.
   assert(p_object->Type() == POLYGON);

   // Yes, this is a yucky typecast, but essential so we'll have a real polygon.
   Polygon *poly = (Polygon *)p_object;
   p_texture = poly->m_texture;
   p_material = poly->m_material;

   CGrPoint intersect = p_ray.Origin() + p_ray.Direction() * p_t;

   //
   // Now we need to determine the interpolated values.  We use the
   // rule here that polygons are convex, so that simplifies things 
   // a bit.  But, we need to get the best numeric resolution, so we 
   // determine the largest component of the polygon normal and
   // project onto the appropriate plane that will maximize the polgon area.
   //

   enum Max {X, Y, Z};

   Max m;
   if(fabs(poly->m_normal.X()) > fabs(poly->m_normal.Y()))
   {
      // Either X or Z
      if(fabs(poly->m_normal.X()) > fabs(poly->m_normal.Z()))
         m = X;
      else
         m = Z;
   }
   else
   {
      // Either Y or Z
      if(fabs(poly->m_normal.Y()) > fabs(poly->m_normal.Z()))
         m = Y;
      else
         m = Z;
   }

   // We're looking of the two edges that overlap
   // the intersection in one dimension.

   // We'll need 4 vertices.  They are:
   // a, b - Left edge
   // c, d - Right edge
   int a = 0;
   int b = 1;
   int c;
   int d;
   int cnt = poly->m_vertices.size();

   // f is the percent from a to b
   // g is the percent from c to d
   // h is the percent from ab side to cd side
   double f, g, h;

   if(m == X)
   {
      // Projecting onto the YZ plane
      // Look for the overlap in the Y dimension
      for( ; b < cnt;  a++, b++)
      {
         if((poly->m_vertices[a].Y() >= intersect.Y() && poly->m_vertices[b].Y() < intersect.Y()) ||
            (poly->m_vertices[b].Y() >= intersect.Y() && poly->m_vertices[a].Y() < intersect.Y()))
         {
            break;
         }
      }


      c = b;
      d = c;  
      if(d < cnt)
         d++;

      for( ; d < cnt;  c++, d++)
      {
         if((poly->m_vertices[c].Y() >= intersect.Y() && poly->m_vertices[d].Y() < intersect.Y()) ||
            (poly->m_vertices[d].Y() >= intersect.Y() && poly->m_vertices[c].Y() < intersect.Y()))
         {
            break;
         }
      }
 
      f = (intersect.Y() - poly->m_vertices[a].Y()) / 
          (poly->m_vertices[b].Y() - poly->m_vertices[a].Y());
      g = (intersect.Y() - poly->m_vertices[c].Y()) / 
          (poly->m_vertices[d].Y() - poly->m_vertices[c].Y());

      double lz = poly->m_vertices[a].Z() + f * (poly->m_vertices[b].Z() - poly->m_vertices[a].Z());
      double rz = poly->m_vertices[c].Z() + g * (poly->m_vertices[d].Z() - poly->m_vertices[c].Z());

      h = (intersect.Z() - lz) / (rz - lz);
   }
   else if(m == Y)
   {
      // Project onto XZ plane
      // Look for the overlap in the X dimension
      for( ; b < cnt;  a++, b++)
      {
         if((poly->m_vertices[a].X() >= intersect.X() && poly->m_vertices[b].X() < intersect.X()) ||
            (poly->m_vertices[b].X() >= intersect.X() && poly->m_vertices[a].X() < intersect.X()))
         {
            break;
         }
      }


      c = b;
      d = c;  
      if(d < cnt)
         d++;

      for( ; d < cnt;  c++, d++)
      {
         if((poly->m_vertices[c].X() >= intersect.X() && poly->m_vertices[d].X() < intersect.X()) ||
            (poly->m_vertices[d].X() >= intersect.X() && poly->m_vertices[c].X() < intersect.X()))
         {
            break;
         }
      }
 
      f = (intersect.X() - poly->m_vertices[a].X()) / 
          (poly->m_vertices[b].X() - poly->m_vertices[a].X());
      g = (intersect.X() - poly->m_vertices[c].X()) / 
          (poly->m_vertices[d].X() - poly->m_vertices[c].X());

      double lz = poly->m_vertices[a].Z() + f * (poly->m_vertices[b].Z() - poly->m_vertices[a].Z());
      double rz = poly->m_vertices[c].Z() + g * (poly->m_vertices[d].Z() - poly->m_vertices[c].Z());

      h = (intersect.Z() - lz) / (rz - lz);
   }
   else if(m == Z)
   {
      // Project onto XY plane
      // Look for the overlap in the X dimension
      for( ; b < cnt;  a++, b++)
      {
         if((poly->m_vertices[a].X() >= intersect.X() && poly->m_vertices[b].X() < intersect.X()) ||
            (poly->m_vertices[b].X() >= intersect.X() && poly->m_vertices[a].X() < intersect.X()))
         {
            break;
         }
      }


      c = b;
      d = c;  
      if(d < cnt)
         d++;

      for( ; d < cnt;  c++, d++)
      {
         if((poly->m_vertices[c].X() >= intersect.X() && poly->m_vertices[d].X() < intersect.X()) ||
            (poly->m_vertices[d].X() >= intersect.X() && poly->m_vertices[c].X() < intersect.X()))
         {
            break;
         }
      }
 
      f = (intersect.X() - poly->m_vertices[a].X()) / 
          (poly->m_vertices[b].X() - poly->m_vertices[a].X());
      g = (intersect.X() - poly->m_vertices[c].X()) / 
          (poly->m_vertices[d].X() - poly->m_vertices[c].X());

      double lz = poly->m_vertices[a].Y() + f * (poly->m_vertices[b].Y() - poly->m_vertices[a].Y());
      double rz = poly->m_vertices[c].Y() + g * (poly->m_vertices[d].Y() - poly->m_vertices[c].Y());

      h = (intersect.Y() - lz) / (rz - lz);
   }

   // Convert values to what is multiplied by each part
   double am = 1 - f - h + f * h;
   double bm = f - f * h;
   double cm = h - g * h;
   double dm = g * h;

   // Set the normal
   if(poly->m_normals.size() == 1)
      p_normal = poly->m_normals[0];
   else
   {
      p_normal = poly->m_normals[a] * am + poly->m_normals[b] * bm + 
         poly->m_normals[c] * cm + poly->m_normals[d] * dm;
      p_normal.Normalize3();
   }

   if(p_texture)
   {
      CGrPoint ta = poly->m_tvertices[a];
      CGrPoint tb = poly->m_tvertices[b];
      CGrPoint tc = poly->m_tvertices[c];
      CGrPoint td = poly->m_tvertices[d];

      p_texcoord = poly->m_tvertices[a] * am + poly->m_tvertices[b] * bm + 
         poly->m_tvertices[c] * cm + poly->m_tvertices[d] * dm;
   }

}



//
// Name :         CRayIntersection::LoadingComplete()
// Description :  Indicate that everything we might want to load
//                into the ray intersection structure is loaded.
//

void CRayIntersection::LoadingComplete()
{
   assert(m_polys.size() > 0);

   // Allocate the walking blocks data structure
   AllocateBlocks();

   // Determine the extents in each dimension
   DetermineExtents();

   // Render the polygons into the data structure
   RenderPolygons();
}


//
// Name :         CRayIntersection::AllocateBlocks()
// Description :  Allocate the walking blocks data structure array.
//                This is a 3D array.
//

void CRayIntersection::AllocateBlocks()
{
   DeleteBlocks();

   // Note that I make the data structure one too big in each
   // dimension to simplify round-up

   m_blocksdim = m_subdivide;
   m_blocks = new vector<Polygon *> **[m_blocksdim+1];

   for(int i=0;  i<m_blocksdim+1;  i++)
   {
      m_blocks[i] = new vector<Polygon *> *[m_blocksdim+1];

      for(int j=0;  j<m_blocksdim+1;  j++)
      {
         m_blocks[i][j] = new vector<Polygon *>[m_blocksdim+1];
      }

   }

}


//
// Name :         CRayIntersection::DeleteBlocks()
// Description :  Delete any memory allocated to the walking blocks structure.
//

void CRayIntersection::DeleteBlocks()
{
   if(m_blocks)
   {
      for(int i=0;  i<m_blocksdim+1;  i++)
      {
         for(int j=0;  j<m_blocksdim+1;  j++)
         {
            delete [] m_blocks[i][j];
         }

         delete [] m_blocks[i];
      }

      delete [] m_blocks;
   }

   m_blocks = NULL;
}

//
// Name :         CRayIntersection::DetermineExtents()
// Description :  We need to know the range of the blocks, so determine the
//                maximum and minimum extents in each dimension.
//

void CRayIntersection::DetermineExtents()
{
   // Initially, just fill min and max with an arbitrary vertex.
   list<Polygon>::iterator poly = m_polys.begin();
   m_min = m_max = poly->m_vertices[0];

   // Iterate over all polygons and vertices.
   for( ; poly!=m_polys.end();  poly++)
   {
      for(unsigned int i=0;  i<poly->m_vertices.size();  i++)
      {
         CGrPoint &p = poly->m_vertices[i];

         if(p.X() < m_min.X())
            m_min.X(p.X());
         if(p.Y() < m_min.Y())
            m_min.Y(p.Y());
         if(p.Z() < m_min.Z())
            m_min.Z(p.Z());

         if(p.X() > m_max.X())
            m_max.X(p.X());
         if(p.Y() > m_max.Y())
            m_max.Y(p.Y());
         if(p.Z() > m_max.Z())
            m_max.Z(p.Z());
      }

   }

}


//
// Name :         CRayIntersection::RenderPolygons()
// Description :  Render the polygons into the walking blocks data structure.
//

void CRayIntersection::RenderPolygons()
{
   // Iterate over all polygons
   list<Polygon>::iterator poly = m_polys.begin();
   for( ; poly!=m_polys.end();  poly++)
   {
      Polygon *p = &(*poly);
      if(p->m_vertices.size() < 4)
         continue;

      // Copy polygon into a ClipPoly structure
      // We'll change the dimensions when we do so 
      // so that x values are relative to the m_blocks
      // data structure!  While we're at it, determine
      // the maximum value in the X direction

      vector<CGrPoint> vertices;
      vertices.resize(p->m_vertices.size() - 1);
      double maxx = -1;
      double xmult = double(m_blocksdim) / (m_max.X() - m_min.X());
      double ymult = double(m_blocksdim) / (m_max.Y() - m_min.Y());
      double zmult = double(m_blocksdim) / (m_max.Z() - m_min.Z());

      // Recall that the data structure duplicates the last vertex.
      // We don't want that here...
      for(unsigned int i=0;  i<p->m_vertices.size() - 1;  i++)
      {
         vertices[i].X( (p->m_vertices[i].X() - m_min.X()) * xmult );
         vertices[i].Y( (p->m_vertices[i].Y() - m_min.Y()) * ymult );
         vertices[i].Z( (p->m_vertices[i].Z() - m_min.Z()) * zmult );
         vertices[i].W(1);

         if(vertices[i].X() > maxx)
            maxx = vertices[i].X();
      }

      // For maximum efficiency, I use two vertex lists and
      // bounce the polygon back and forth.  This avoids an
      // extra copy each pass.
      vector<CGrPoint> vertices1;

      // Now loop from the maximum X value down
      // Bound back and forth between vertices and vertices1
      for(int x=int(maxx);  ;  )
      {
         ClipOffX(x, p, vertices, vertices1);
         x--;
         if(vertices1.empty() || x < 0)
            break;

         ClipOffX(x, p, vertices1, vertices);
         x--;
         if(vertices.empty() || x < 0)
            break;

      }

   }
}

//
// Name :         CRayIntersection::ClipOffX()
// Description :  Split the passed polygon into two parts:  The parts
//                to the right of p_x (block dimension) and those to the
//                left.  
// Parameters :   p_x - The current x value in m_blocks.  It is asserted that
//                the maximum x value in p_poly will not be more than (x+1) 
//

void CRayIntersection::ClipOffX(int p_x, Polygon *p_poly, 
                                const vector<CGrPoint> &p_vertices, vector<CGrPoint> &p_lte)
{
   double xclip = p_x;

   p_lte.clear();          // <= destination
   vector<CGrPoint> gte;   // >= destination

   // Split into p_lte (<=) and gte (>=)
   // Source is in p_vertices

   const CGrPoint *s = &p_vertices[p_vertices.size() - 1];  // Last vertex
   for(unsigned int j=0;  j<p_vertices.size();  j++)
   {
      const CGrPoint *p = &p_vertices[j];
      if(p->X() == xclip)
      {
         // Add p to both sides
         p_lte.push_back(*p);
         gte.push_back(*p);
      }
      else if(p->X() >= xclip)
      {
         if(s->X() >= xclip)
         {
            // Add p to the >= side 
            gte.push_back(*p);
         }
         else
         {
            // Add i to <= side, 
            // Add i, p to >= side
            double px = (xclip - p->X()) / (s->X() - p->X());
            CGrPoint i(xclip, px * s->Y() + (1 - px) * p->Y(), px * s->Z() + (1 - px) * p->Z());
            p_lte.push_back(i);
            gte.push_back(i);
            gte.push_back(*p);
         }
      }
      else
      {
         if(s->X() <= xclip)
         {
            // Add p to the <= side
            p_lte.push_back(*p);
         }
         else
         {
            // Add i to the >= side
            // Add i, p to the <= side
            double px = (xclip - p->X()) / (s->X() - p->X());
            CGrPoint i(xclip, px * s->Y() + (1 - px) * p->Y(), px * s->Z() + (1 - px) * p->Z());
            gte.push_back(i);
            p_lte.push_back(i);
            p_lte.push_back(*p);
         }
      }

      s = p;
   }

   // 
   // Now, gte contains only a single x value.  
   // Determine the range in the Y direction and repeat
   // the process.
   //

   double maxy = gte[0].Y();
   for(unsigned int i=0;  i<gte.size();  i++)
   {
      if(gte[i].Y() > maxy)
         maxy = gte[i].Y();
   }

   vector<CGrPoint> gte1;

   // Now loop from the maximum Y value down
   for(int y=int(maxy);   ;  )
   {
      ClipOffY(p_x, y, p_poly, gte, gte1);
      y--;
      if(gte1.empty() || y < 0)
         break;

      ClipOffY(p_x, y, p_poly, gte1, gte);
      y--;
      if(gte.empty() || y < 0)
         break;
   }


}



//
// Name :         CRayIntersection::ClipOffY()
// Description :  Split the passed polygon into two parts:  The parts
//                above p_y (block dimension) and those below.
// Parameters :   p_x - The current x value in m_blocks.  
//                p_y - The current y  value.  It is asserted that
//                the maximum y value in p_poly will not be more than (y+1)
//

void CRayIntersection::ClipOffY(int p_x, int p_y, Polygon *p_poly, 
                                const vector<CGrPoint> &p_vertices, vector<CGrPoint> &p_lte)
{
   double yclip = p_y;

   p_lte.clear();          // <= destination
   vector<CGrPoint> gte;   // >= destination

   // Split into p_lte (<=) and gte (>=)
   // Source is in p_vertices

   const CGrPoint *s = &p_vertices[p_vertices.size() - 1];
   for(unsigned int j=0;  j<p_vertices.size();  j++)
   {
      const CGrPoint *p = &p_vertices[j];
      if(p->Y() == yclip)
      {
         // Add p to both sides
         p_lte.push_back(*p);
         gte.push_back(*p);
      }
      else if(p->Y() >= yclip)
      {
         if(s->Y() >= yclip)
         {
            // Add p to the >= side 
            gte.push_back(*p);
         }
         else
         {
            // Add i to <= side, 
            // Add i, p to >= side
            double py = (yclip - p->Y()) / (s->Y() - p->Y());
            CGrPoint i(py * s->X() + (1 - py) * p->X(), yclip, py * s->Z() + (1 - py) * p->Z());
            p_lte.push_back(i);
            gte.push_back(i);
            gte.push_back(*p);
         }
      }
      else
      {
         if(s->Y() <= yclip)
         {
            // Add p to the <= side
            p_lte.push_back(*p);
         }
         else
         {
            // Add i to the >= side
            // Add i, p to the <= side
            double py = (yclip - p->Y()) / (s->Y() - p->Y());
            CGrPoint i(py * s->X() + (1 - py) * p->X(), yclip, py * s->Z() + (1 - py) * p->Z());
            gte.push_back(i);
            p_lte.push_back(i);
            p_lte.push_back(*p);
         }
      }

      s = p;
   }

   // 
   // Now, dest contains only a single x and y value.  
   // This is basically a strip out of the original polygon.
   // Determine the range in the Z direction and we know
   // the blocks this polygon utilizes.
   //

   double maxz = 0;
   double minz = m_blocksdim - 1;
   for(unsigned int i=0;  i<gte.size();  i++)
   {
      if(gte[i].Z() > maxz)
         maxz = gte[i].Z();
      if(gte[i].Z() < minz)
         minz = gte[i].Z();
   }

   // Now loop over the Z values, seeing the polygon in the 
   // blocks data structure.
   for(int z=int(minz);  z <= int(maxz);  z++)
   {
      m_blocks[z][p_y][p_x].push_back(p_poly);
   }
}


