//
// Name :         RayIntersection.h
// Description :  Header for CRayIntersection
//                Polygon ray intersection support class.
// Author :       Charles B. Owen
//                For further information, see RayIntersection.cpp
//

#if !defined(AFX_RAYINTERSECTION_H__95B0534E_1C6F_42AC_BBF3_8CBC530AF23A__INCLUDED_)
#define AFX_RAYINTERSECTION_H__95B0534E_1C6F_42AC_BBF3_8CBC530AF23A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <vector>
#include "GrTexture.h"

class CRay
{
public:
   CRay(const CGrPoint &o, const CGrPoint &d) {m_o=o;  m_d=d;}

   const CGrPoint &Origin() const {return m_o;}
   const CGrPoint &Direction() const {return m_d;}

private:
   CGrPoint    m_o;
   CGrPoint    m_d;
};

class CRayIntersection  
{
public:
	void LoadingComplete();
	CRayIntersection();
	virtual ~CRayIntersection();

	void Initialize();

   // Polygon insertion
	void PolygonBegin();
	void PolygonEnd();

   // Generic insertion routines
	void Material(CGrMaterial *p_material);
	void Vertex(const CGrPoint &p_vertex);
	void TexVertex(const CGrPoint &p_tvertex);
	void Normal(const CGrPoint &p_normal);
	void Texture(CGrTexture *p_texture);

   enum ObjectType {POLYGON};

   // This is a generic superclass for any type of 
   // object we may compute a ray intersection on.
   struct Object
   {
      virtual ObjectType Type() const = 0;
   };

   // This is the structure we'll use to define
   // polygons in the ray intersection data structures.
   struct Polygon : public Object
   {
      virtual ObjectType Type() const {return POLYGON;}

      Polygon() {}
      Polygon(const Polygon &p) {}

      CGrPtr<CGrTexture>   m_texture;
      std::vector<CGrPoint>  m_vertices;
      std::vector<CGrPoint>  m_normals;
      std::vector<CGrPoint>  m_tvertices;
      CGrMaterial         *m_material;

      // Intersection assistance
      CGrPoint             m_normal;      // The polygon normal
      double               m_d;           // The D value for ax+by+cz+d=0;
      std::list<CGrPoint>  m_enormals;    // Edge normals
   };
   
   // Intersection testing
   bool Intersect(const CRay &p_ray, const Object *p_ignore, Object *&p_object, double &p_t, CGrPoint &p_intersect) const;
   void IntersectInfo(const CRay &p_ray, const Object *p_object, double p_t, 
                      CGrPoint &p_normal, CGrMaterial *&p_material, 
                      CGrTexture *&p_texture, CGrPoint &p_texcoord) const; 

   void Subdivisions(int p_subdivisions) {m_subdivide = p_subdivisions;}
   void Fast(bool s) {m_fast = s;}

private:
   void ClipOffX(int p_x, Polygon *p_poly, 
                 const std::vector<CGrPoint> &p_vertices, std::vector<CGrPoint> &p_dest);
	void ClipOffY(int p_x, int p_y, Polygon *p_poly, 
                 const std::vector<CGrPoint> &p_vertices, std::vector<CGrPoint> &p_dest);

	void RenderPolygons();
	void DetermineExtents();
	void AllocateBlocks();
	void DeleteBlocks();
	bool m_fast;
   std::list<Polygon>   m_polys;
   int                  m_mark;
   int                  m_subdivide;      // Parameter to use
   int                  m_blocksdim;      // What we actually used
   std::vector<Polygon *> ***m_blocks;
   CGrPoint             m_min, m_max;     // Minimum and maximum x,y,z
};

#endif // !defined(AFX_RAYINTERSECTION_H__95B0534E_1C6F_42AC_BBF3_8CBC530AF23A__INCLUDED_)
