// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Lab2.h"
#include "ChildView.h"
#include "TetraDlg.h"
#include "OpenGLRenderer.h"
#include "MyRaytraceRenderer.h"
#include "FloatFactory.h"
#include "ShapeFactory.h"
#include "AntiADlg.h"
#include "MonteDlg.h"
#include <cmath>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
: m_spinangle(0)
, m_spintimer(0)
, m_size(2)
, m_iterations(1)
#ifdef MONTE_CARLO
, m_mcRays(1)
#endif
{
	m_raytrace = false;
	m_rayimage = NULL;
	SetDoubleBuffer(true);
	m_camera.Set(20, 10, 50, 0, 0, 0, 0, 1, 0);
	m_camera.MouseMode(CGrCamera::PITCHYAW);

	CFloatFactory Float;
	Float.Create();
	m_scene = Float.SceneGraph();

	
}

CChildView::~CChildView()
{
	 if (m_rayimage != NULL)
   {
	   delete [] m_rayimage[0];
	   delete [] m_rayimage;
   }
}


 BEGIN_MESSAGE_MAP(CChildView,COpenGLWnd )
	ON_WM_PAINT()
	ON_COMMAND(ID_LABSTUFF_SPIN, OnLabstuffSpin)
	ON_WM_TIMER()
	ON_COMMAND(ID_LABSTUFF_TETRASIZE, OnLabstuffTetrasize)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_CAMERA_PITCH, OnCameraPitch)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_PITCH, OnUpdateCameraPitch)
	ON_COMMAND(ID_CAMERA_ROLL, OnCameraRoll)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_ROLL, OnUpdateCameraRoll)
	ON_COMMAND(ID_CAMERA_PAN, OnCameraPan)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_PAN, OnUpdateCameraPan)
	ON_COMMAND(ID_RENDER_RAYTRACE, OnRenderRaytrace)
	ON_UPDATE_COMMAND_UI(ID_RENDER_RAYTRACE, OnUpdateRenderRaytrace)
	ON_COMMAND(ID_OPTIONS_ANTI, OnOptionsAnti)
	ON_COMMAND(ID_OPTIONS_MONTE, OnOptionsMonte)
 END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!COpenGLWnd::PreCreateWindow(cs))
        return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}


void CChildView::OnGLDraw(CDC *pDC)
{
	if(m_raytrace)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f) ;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int width, height;
		GetSize(width, height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity(); 
		glOrtho(0, width, 0, height, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity(); 

		if(m_rayimage)
		{
			glRasterPos3i(0, 0, 0);
			glDrawPixels(m_rayimagewidth, m_rayimageheight, GL_RGB, GL_UNSIGNED_BYTE, m_rayimage[0]);
		}

		glFlush();
	}
	else
	{	
		//
		// Instantiate a renderer
		//

		COpenGLRenderer renderer;

		// Configure the renderer
		ConfigureRenderer(&renderer);

		//
		// Render the scene
		//

		renderer.Render(m_scene);
	}
}


void CChildView::ConfigureRenderer(CGrRenderer *p_renderer)
{
	// Determine the screen size so we can determine the aspect ratio
	int width, height;
	GetSize(width, height);
	double aspectratio = double(width) / double(height);

	//
	// Set up the camera in the renderer
	//

	p_renderer->Perspective(m_camera.FieldOfView(), 
		aspectratio, // The aspect ratio.
		20., // Near clipping
		1000.); // Far clipping

	// m_camera.FieldOfView is the vertical field of view in degrees.

	//
	// Set the camera location
	//

	p_renderer->LookAt(m_camera.Eye()[0], m_camera.Eye()[1], m_camera.Eye()[2], 
		m_camera.Center()[0], m_camera.Center()[1], m_camera.Center()[2], 
		m_camera.Up()[0], m_camera.Up()[1], m_camera.Up()[2]);

	//
	// Set the light locations and colors
	//

	float dimd = 0.5f;
	GLfloat dim[] = {dimd, dimd, dimd, 1.0f};
	GLfloat brightwhite[] = {1.f, 1.f, 1.f, 1.0f};
	GLfloat dimwhite[] = {.5f, .5f, .5f, 1.f};

//#ifndef MONTE_CARLO
	p_renderer->AddLight(CGrPoint(20, 100, -50), dim, brightwhite, brightwhite);
	//p_renderer->AddLight(CGrPoint(0, 100, 50), dim, dimwhite, dimwhite);
//#endif
}



//
//        Name : Quad()
// Description : Inline function for drawing 
// a quadralateral.
//

inline void Quad(GLdouble *v1, GLdouble *v2, GLdouble *v3, GLdouble *v4)
{
	glBegin(GL_QUADS);
	glVertex3dv(v1);
	glVertex3dv(v2);
	glVertex3dv(v3);
	glVertex3dv(v4);
	glEnd();
}

//
//        Name : CChildView::Box()
// Description : Draw an arbitrary size box. p_x, 
//              p_y, and p_z are the height of
//              the box. We'll use this 
//               as a common primitive.
//      Origin : The back corner is at 0, 0, 0, and
//               the box is entirely in the
//               positive octant.
//

void CChildView::Box(GLdouble p_x, GLdouble p_y, 
					 GLdouble p_z, const GLdouble *p_color)
{
	GLdouble a[] = {0., 0., p_z};
	GLdouble b[] = {p_x, 0., p_z};
	GLdouble c[] = {p_x, p_y, p_z};
	GLdouble d[] = {0., p_y, p_z};
	GLdouble e[] = {0., 0., 0.};
   GLdouble f[] = {p_x, 0., 0.};
   GLdouble g[] = {p_x, p_y, 0.};
   GLdouble h[] = {0., p_y, 0.};

   // I'm going to mess with the colors a bit so 
   // the faces will be visible in solid shading
   glColor3d(p_color[0], p_color[1], p_color[2]);
   Quad(a, b, c, d); // Front

   glColor3d(p_color[0] * 0.95, p_color[1] * 0.95, p_color[2] * 0.95);
   Quad(c, b, f, g); // Right

   glColor3d(p_color[0] * 0.85, p_color[1] * 0.85, p_color[2] * 0.85);
   Quad(h, g, f, e); // Back

   glColor3d(p_color[0] * 0.90, p_color[1] * 0.90, p_color[2] * 0.90);
   Quad(d, h, e, a); // Left

   glColor3d(p_color[0] * 0.92, p_color[1] * 0.92, p_color[2] * 0.92);
   Quad(d, c, g, h); // Top

   glColor3d(p_color[0] * 0.80, p_color[1] * 0.80, p_color[2] * 0.80);
   Quad(e, f, b, a); // Bottom
}

void CChildView::Barbell()
{
	glPushMatrix();
	glTranslated(-1., 0., -1.);
	const double RED[] = {0.8, 0.0, 0.0};
	const double BLUE[] = {0, 0, .8};
	const double PURPLE[] = {.4, .1, .8};
	Box(2., 1., 2., PURPLE);

	glTranslated(0., 5., 0.);
	Box(2., 1., 2., PURPLE);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.25, 1., -0.25);
	Box(0.5, 4.0, 0.5, BLUE);
	glPopMatrix();
}


void CChildView::OnLabstuffSpin()
{
	if(m_spintimer)
	{
		KillTimer(m_spintimer);
		m_spintimer = 0;
	}
	else
	{
		m_spintimer = SetTimer(1, 30, NULL);
	}
}

void CChildView::OnTimer(UINT nIDEvent)
{
	m_spinangle+=3;
	Invalidate();
}

void CChildView::Tetrahedron(double height)
{
	double rad = height*(sqrt(2.)/2.);
	GLdouble a[] = {0., 0., height};
    GLdouble b[] = {rad*cos(0.0), 0.0, rad*sin(0.0)};
    GLdouble c[] = {rad*cos(2.094), 0.0, rad*sin(2.094)};
    GLdouble d[] = {rad*cos(4.188), 0.0, rad*sin(4.188)};

	glBegin(GL_POLYGON);
		glVertex3d(0,height,0);
		glVertex3d(rad*cos(2.094), 0.0, rad*sin(2.094));
		glVertex3d(rad*cos(0.0),0,rad*sin(0.0));
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3d(0,height,0);
		glVertex3d(rad*cos(2.094), 0, rad*sin(2.094));
		glVertex3d(rad*cos(0.0),0,rad*sin(0.0));
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3d(0,height,0);
		glVertex3d(rad*cos(2.094), 0, rad*sin(2.094));
		glVertex3d(rad*cos(0.0),0,rad*sin(0.0));
	glEnd();

	glBegin(GL_POLYGON);
		glVertex3d(0,height,0);
		glVertex3d(rad*cos(2.094), 0, rad*sin(2.094));
		glVertex3d(rad*cos(0.0),0,rad*sin(0.0));
	glEnd();
}

void CChildView::Tetra(GLdouble p_size, const GLdouble *p_color)
{
   //Tetrahedron
   GLdouble leg = sqrt((3./2.)*p_size*p_size);
   GLdouble a[] = {0.0, p_size, 0.0};
   GLdouble b[] = {-(leg/2.), 0.0, -((1./3.)*p_size)};
   GLdouble c[] = {0.0, 0.0, ((2./3.)*p_size)};
   GLdouble d[] = {(leg/2.), 0.0, -((1./3.)*p_size)};
   glColor3d(p_color[0], p_color[1], p_color[2]);
   Tri(a, d, b); // Back
   glColor3d(p_color[0] * 0.80, p_color[1] * 0.80, p_color[2] * 0.80);
   Tri(a, b, c); // Left
   glColor3d(p_color[0] * 0.60, p_color[1] * 0.60, p_color[2] * 0.60);
   Tri(a, c, d); // Right
   glColor3d(p_color[0] * 0.40, p_color[1] * 0.40, p_color[2] * 0.40);
   Tri(b, d, c); // Bottom
}


void CChildView::OnLabstuffTetrasize()
{
	CTetraDlg dlg;
	dlg.m_size = m_size;	
	if(dlg.DoModal() == IDOK)
	{
		m_size = dlg.m_size;
		Invalidate();
	}
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_camera.MouseDown(point.x, point.y);

	COpenGLWnd ::OnLButtonDown(nFlags, point);

}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(nFlags & MK_LBUTTON)
	{
		m_camera.MouseMove(point.x, point.y);
		Invalidate();
	}

	COpenGLWnd ::OnMouseMove(nFlags, point);
}

void CChildView::OnCameraPitch()
{
	m_camera.MouseMode(CGrCamera::PITCHYAW);

}

void CChildView::OnUpdateCameraPitch(CCmdUI *pCmdUI)
{
	 pCmdUI->SetCheck(m_camera.MouseMode() == CGrCamera::PITCHYAW);
}

void CChildView::OnCameraRoll()
{
	m_camera.MouseMode(CGrCamera::ROLLMOVE);

}

void CChildView::OnUpdateCameraRoll(CCmdUI *pCmdUI)
{
	 pCmdUI->SetCheck(m_camera.MouseMode() == CGrCamera::ROLLMOVE);
}

void CChildView::OnCameraPan()
{
	 m_camera.MouseMode(CGrCamera::PANTILT);

}

void CChildView::OnUpdateCameraPan(CCmdUI *pCmdUI)
{
	  pCmdUI->SetCheck(m_camera.MouseMode() == CGrCamera::PANTILT);

}

void CChildView::OnRenderRaytrace()
{
   // If turning off...
   if(m_raytrace)
   {
      m_raytrace = false;
      Invalidate();
      return;
   }
   m_raytrace = true;
	
   if (m_rayimage != NULL)
   {
	   delete [] m_rayimage[0];
	   delete [] m_rayimage;
   }

   GetSize(m_rayimagewidth, m_rayimageheight);
   m_rayimage = new GLbyte *[m_rayimageheight];
   int rowwid = m_rayimagewidth * 3;
   while(rowwid % 4)
	   rowwid++;
   
   m_rayimage[0] = new GLbyte[m_rayimageheight * rowwid];

   int i;
   for(i=1; i<m_rayimageheight; i++)
   {
	   m_rayimage[i] = m_rayimage[0] + i * rowwid;
   } 

   for(i=0; i<m_rayimageheight; i++)
   {
	   // Fill the image with blue
	   for(int j=0; j<m_rayimagewidth; j++)
	   {
			   m_rayimage[i][j * 3] = 0; // red
			   m_rayimage[i][j * 3 + 1] = 0; // green
			   m_rayimage[i][j * 3 + 2] = GLbyte(255); // blue
	   }
   } 

   // Instantiate a raytrace object
   CMyRaytraceRenderer raytrace;

   raytrace.SetIterations(m_iterations);
#ifdef MONTE_CARLO
   raytrace.SetMonteCarloRays(m_mcRays);
#endif

   raytrace.SetDimensions(m_rayimageheight, m_rayimagewidth, m_rayimage);
   raytrace.SetWindow(this);

   // Generic configurations for all renderers
   ConfigureRenderer(&raytrace);

#ifdef MONTE_CARLO
	float dimd = 0.5f;
	GLfloat dim[] = {dimd, dimd, dimd, 1.0f};
	GLfloat brightwhite[] = {1.f, 1.f, 1.f, 1.0f};
	GLfloat dimwhite[] = {.5f, .5f, .5f, 1.f};

  CGrPoint lightOrientation(0.0, -1.0, 0.0);
	//raytrace.AddLight(CGrPoint(20, 100, -50), dim, brightwhite, brightwhite, lightOrientation, 30.0);
	//raytrace.AddLight(CGrPoint(20, 200, 50), dim, brightwhite, brightwhite, lightOrientation, 1.0);
	//raytrace.AddLight(CGrPoint(20, 100, 50), dim, brightwhite, brightwhite, lightOrientation, 30.0);
	//raytrace.AddLight(CGrPoint(20, 100, -50), dim, brightwhite, brightwhite, lightOrientation, 1.0);
	//raytrace.AddLight(CGrPoint(20, 300, -50), dim, brightwhite, brightwhite,
  //                     lightOrientation, 10.0);

	//raytrace.AddLight(CGrPoint(0, 100, 0), dim, brightwhite, brightwhite, lightOrientation, 1.0);
	//raytrace.AddLight(CGrPoint(0, 1000, 0), dim, brightwhite, brightwhite, lightOrientation, 1.0);
	//raytrace.AddLight(CGrPoint(100, 0, 0), dim, brightwhite, brightwhite, lightOrientation, 1.0);
	//raytrace.AddLight(CGrPoint(0, 0, 100), dim, brightwhite, brightwhite, lightOrientation, 1.0);
	//raytrace.AddLight(CGrPoint(100, 100, 100), dim, brightwhite, brightwhite, lightOrientation, 1.0);

  
  //CGrPoint lo(0.0, -1.0, 0.0);
  //raytrace.AddLight(CGrPoint(0, 70, 0), dim, brightwhite, brightwhite, lo, 1.0, M_PI/60.0, true);
  //CGrPoint lo(0.0, 0.0, 1.0);
  //raytrace.AddLight(CGrPoint(0, 35, 70), dim, brightwhite, brightwhite, lo, 1.0, M_PI/60.0, true);
#endif
  
  //
   // Render the Scene
   //

   raytrace.Render(m_scene);
   Invalidate();
}

void CChildView::OnUpdateRenderRaytrace(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_raytrace);
}

void CChildView::OnOptionsAnti()
{
	CAntiADlg dlg;

   dlg.m_iterations = m_iterations;
  
   if(dlg.DoModal() == IDOK)
   {
      m_iterations = dlg.m_iterations;
      Invalidate();
   }

}

#ifdef MONTE_CARLO
void CChildView::OnOptionsMonte()
{
	CMonteDlg dlg;

   dlg.m_iterations = m_mcRays;
  
   if(dlg.DoModal() == IDOK)
   {
      m_mcRays = dlg.m_iterations;
      Invalidate();
   }

}
#endif
