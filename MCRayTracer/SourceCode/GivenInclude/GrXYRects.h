// GrXYRects.h: interface for the CGrXYRects class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRXYRECTS_H__3DC882D6_C671_4CB1_9A0D_54316872B252__INCLUDED_)
#define AFX_GRXYRECTS_H__3DC882D6_C671_4CB1_9A0D_54316872B252__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GrObject.h"
#include "GrTexture.h"

class CGrXYRects : public CGrObject  
{
public:
	virtual void Render(CGrRenderer *p_renderer);
	virtual void glRender();
	void AddRect(CGrPoint &ll, CGrPoint &ur);
	void Texture(CGrTexture *p_texture, double xd, double yd, double so, double to);
	CGrXYRects();
	virtual ~CGrXYRects();

	void AddRect(double x1, double y1, double x2, double y2, double z);
	void Cutout(double p_x1, double p_y1, double p_x2, double p_y2);
	void CutoutCenterWidths(double p_xc, double p_yc, double p_xw, double p_yw)
   {
      Cutout(p_xc-p_xw/2., p_yc-p_yw/2., p_xc+p_xw/2., p_yc+p_yw/2.);
   }


private:
   struct Rect
   {
      Rect(double x1, double y1, double x2, double y2, double z) 
      {m_x1=x1;  m_y1=y1;  m_x2=x2;  m_y2=y2;  m_z=z;}
      double m_x1, m_y1, m_x2, m_y2, m_z;
   };

   std::list<Rect>      m_rects;

   // Do we have an associated texture?
   // Texture coordinates will be s = x / m_xd + m_so
   CGrPtr<CGrTexture>   m_texture;
   double      m_xd;    // X divisor for texture mapping
   double      m_yd;     
   double      m_so;    // S offset for texture
   double      m_to;    
};

#endif // !defined(AFX_GRXYRECTS_H__3DC882D6_C671_4CB1_9A0D_54316872B252__INCLUDED_)
