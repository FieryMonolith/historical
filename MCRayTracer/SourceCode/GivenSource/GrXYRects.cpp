// GrXYRects.cpp: implementation of the CGrXYRects class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "GrXYRects.h"
#include "GrRenderer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;

inline double _MAX(double a, double b) {return a > b ? a : b;}
inline double _MIN(double a, double b) {return a < b ? a : b;}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrXYRects::CGrXYRects()
{
}

CGrXYRects::~CGrXYRects()
{
}


//
// Name :         CGrXYRects::Texture()
// Description :  Assigns a texture for use with this class.
//

void CGrXYRects::Texture(CGrTexture *p_texture, double xd, double yd, double so, double to)
{
   m_texture = p_texture;
   m_xd = xd;  m_yd = yd;
   m_so = so;  m_to = to;
}

//
// Name :         CGrXYRects::AddRect()
// Description :  Add a rectangle to the list of Rects.
//

void CGrXYRects::AddRect(CGrPoint &ll, CGrPoint &ur)
{
   m_rects.push_back(Rect(ll.X(), ll.Y(), ur.X(), ur.Y(), ll.Z()));
}

void CGrXYRects::AddRect(double x1, double y1, double x2, double y2, double z)
{
   m_rects.push_back(Rect(x1, y1, x2, y2, z));
}



void CGrXYRects::glRender()
{
   if(m_texture)
   {
      // Textured version
      glEnable(GL_TEXTURE_2D);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D, m_texture->TexName());

      glBegin(GL_QUADS);
      glNormal3d(0, 0, 1.);

      for(list<Rect>::iterator q=m_rects.begin();  q!=m_rects.end();  q++)
      {
         double x1 = q->m_x1;;
         double y1 = q->m_y1;
         double x2 = q->m_x2;;
         double y2 = q->m_y2;
         double z = q->m_z;;

         glTexCoord2d(x1 / m_xd + m_so, y1 / m_yd + m_to);
         glVertex3d(x1, y1, z);
         glTexCoord2d(x2 / m_xd + m_so, y1 / m_yd + m_to);
         glVertex3d(x2, y1, z);
         glTexCoord2d(x2 / m_xd + m_so, y2 / m_yd + m_to);
         glVertex3d(x2, y2, z);
         glTexCoord2d(x1 / m_xd + m_so, y2 / m_yd + m_to);
         glVertex3d(x1, y2, z);
      }

      glEnd();
      glDisable(GL_TEXTURE_2D);
   }
   else
   {
      // Untextured version
      glBegin(GL_QUADS);
      glNormal3d(0, 0, 1.);

      for(list<Rect>::iterator q=m_rects.begin();  q!=m_rects.end();  q++)
      {
         double x1 = q->m_x1;;
         double y1 = q->m_y1;
         double x2 = q->m_x2;;
         double y2 = q->m_y2;
         double z = q->m_z;;

         glVertex3d(x1, y1, z);
         glVertex3d(x2, y1, z);
         glVertex3d(x2, y2, z);
         glVertex3d(x1, y2, z);
      }

      glEnd();
   }
}  


void CGrXYRects::Render(CGrRenderer *p_renderer)
{
   if(m_texture)
   {
      // Textured version
      for(list<Rect>::iterator q=m_rects.begin();  q!=m_rects.end();  q++)
      {
         p_renderer->RendererBeginPolygon();
         p_renderer->RendererTexture(m_texture);

         p_renderer->RendererNormal(CGrPoint(0, 0, 1, 0));

         double x1 = q->m_x1;;
         double y1 = q->m_y1;
         double x2 = q->m_x2;;
         double y2 = q->m_y2;
         double z = q->m_z;;

         p_renderer->RendererTexVertex(CGrPoint(x1 / m_xd + m_so, y1 / m_yd + m_to, 0));
         p_renderer->RendererVertex(CGrPoint(x1, y1, z));
         p_renderer->RendererTexVertex(CGrPoint(x2 / m_xd + m_so, y1 / m_yd + m_to, 0));
         p_renderer->RendererVertex(CGrPoint(x2, y1, z));
         p_renderer->RendererTexVertex(CGrPoint(x2 / m_xd + m_so, y2 / m_yd + m_to, 0));
         p_renderer->RendererVertex(CGrPoint(x2, y2, z));
         p_renderer->RendererTexVertex(CGrPoint(x1 / m_xd + m_so, y2 / m_yd + m_to, 0));
         p_renderer->RendererVertex(CGrPoint(x1, y2, z));
 
         p_renderer->RendererEndPolygon();
      }

   }
   else
   {
      // Untextured version

      for(list<Rect>::iterator q=m_rects.begin();  q!=m_rects.end();  q++)
      {
         p_renderer->RendererBeginPolygon();
         p_renderer->RendererNormal(CGrPoint(0, 0, 1, 0));

         double x1 = q->m_x1;;
         double y1 = q->m_y1;
         double x2 = q->m_x2;;
         double y2 = q->m_y2;
         double z = q->m_z;;

         p_renderer->RendererVertex(CGrPoint(x1, y1, z));
         p_renderer->RendererVertex(CGrPoint(x2, y1, z));
         p_renderer->RendererVertex(CGrPoint(x2, y2, z));
         p_renderer->RendererVertex(CGrPoint(x1, y2, z));

         p_renderer->RendererEndPolygon();
      }
   }
}


//
// Name :         CGrXYRects::Cutout()
// Description :  This is the really neat "cutout" function.  It
//                divides any existing rectangles into smaller
//                rectangles to handle making a cutout in the 
//                main rectangle.
//
//                This is just a bit of constructive solid geometry.
//

void CGrXYRects::Cutout(double p_x1, double p_y1, double p_x2, double p_y2)
{
    // This is an iterator.  It will step over the 
    // elements of the current list of rectangles.
    list<Rect>::iterator rect;

    for(rect = m_rects.begin();  rect != m_rects.end();  )
    {
        // We check this rect for overlap.  If we have it,
        // we split and add the new rectangles to the end
        // of the list and delete this from the list.  
          
        // First, are we completely above, to the left of,
        // to the right of, or below the cutout?
        if(rect->m_x2 <= p_x1 ||      // To the left of
            rect->m_x1 >= p_x2 ||      // To the right of
            rect->m_y1 >= p_y2 ||      // Above
            rect->m_y2 <= p_y1)        // Below
        {
            rect++;
        }
        // We have an overlap...
        else 
        {
            // Do we overlap p_x1?
            // This is asking if we have any rectangle
            // to the left of p_x1
            if(rect->m_x1 < p_x1 && rect->m_x2 > p_x1)
            {
                // We're going to cut off the part left
                // of p_x1 and add to the front of the list
                // because it can't possibly overlap
                // Is there anything above p_y2?
                if(rect->m_y2 > p_y2)
                    m_rects.push_front(Rect(rect->m_x1, p_y2, p_x1, rect->m_y2, rect->m_z));

                // Is there anything below p_y1?
                if(rect->m_y1 < p_y1)
                    m_rects.push_front(Rect(rect->m_x1, rect->m_y1, p_x1, p_y1, rect->m_z));

                // And the overlap area
                m_rects.push_front(Rect(rect->m_x1, _MAX(rect->m_y1, p_y1), p_x1, _MIN(rect->m_y2, p_y2), rect->m_z));

                // And substract that from the current rectangle
                rect->m_x1 = p_x1;
            }

            // Do we overlap p_x2?
            // This is asking if we have any rectangle to the
            // right of p_x2
            if(rect->m_x1 < p_x2 && rect->m_x2 > p_x2)
            {
                // We're going to cut off the part right
                // of p_x2 and add to the front of the list
                // because it can't possibly overlap
                // Is there anything above p_y2?
                if(rect->m_y2 > p_y2)
                    m_rects.push_front(Rect(p_x2, p_y2, rect->m_x2, rect->m_y2, rect->m_z));

                // Is there anything below p_y1?
                if(rect->m_y1 < p_y1)
                    m_rects.push_front(Rect(p_x2, rect->m_y1, rect->m_x2, p_y1, rect->m_z));

                // And the overlap area
                m_rects.push_front(Rect(p_x2, _MAX(rect->m_y1, p_y1), rect->m_x2, _MIN(rect->m_y2, p_y2), rect->m_z));

                // And substract that from the current rectangle
                rect->m_x2 = p_x2;
            }

            // Do we overlap p_y1?
            // This is asking if we have any rectangle that
            // is below p_y1.
            if(rect->m_y1 < p_y1 && rect->m_y2 > p_y1)
            {
                // We're going to cut off the part below
                // p_y1 and add to the front of the list
                m_rects.push_front(Rect(rect->m_x1, rect->m_y1, rect->m_x2, p_y1, rect->m_z));

                // And substract that from the current rectangle
                rect->m_y1 = p_y1;
            }

            // Do we overlap p_y2?
            // This is asking if we have any rectangle that
            // is above p_y2.
            if(rect->m_y1 < p_y2 && rect->m_y2 > p_y2)
            {
                // We're going to cut off the part above
                // p_y2 and add to the front of the list
                m_rects.push_front(Rect(rect->m_x1, p_y2, rect->m_x2, rect->m_y2, rect->m_z));

                // And substract that from the current rectangle
                rect->m_y2 = p_y2;
            }

            // Whatever is left is going to be overlapping
            // Just remove it from the list

            list<Rect>::iterator next = rect;
            next++;                 // Move next to the next element
            m_rects.erase(rect);    // Remove the current element
            rect = next;         
        }
    }

}

