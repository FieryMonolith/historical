/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * CChildView - This class is a MFC class that handles the keyboard, mouse,
 *   timer, and window messaging.  It is also responsible for controlling the
 *   scene and drawing the graphics.

 */

#include <fstream>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>

#include "stdafx.h"
#include "AntSkeleton.h"
#include "ChildView.h"
#include ".\childview.h"

#include "DeathSphere.h"
#include "Snake.h"
#include "Vector3d.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// CChildView

CChildView::CChildView()
: d_first(true),
  d_timer(0),
  d_toggle(0),
  d_pause(false)
{
  // Initialize camera and mouse controls
  d_deltax = 0;
  d_deltay = 0;
  d_camera.Set(300.0, 300.0, 300.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  d_camera.MouseMode(CGrCamera::PITCHYAW);

  SetDoubleBuffer(true);

  // Create the head of the target dummy
  d_head = gluNewQuadric();
  assert(d_head);
  gluQuadricOrientation(d_head, GLU_OUTSIDE);

  // Create DeathSphere 1
  Vector3d startPosition1(48.0, 103.0, -5.0);
  Vector3d killPosition1(-50.0, 57.5, -200.0);
  d_sphere1 = new DeathSphere(startPosition1, killPosition1, 2, 8);
  assert(d_sphere1);

  // Create DeathSphere 2
  Vector3d startPosition2(70.0, 200.0, -10.0);
  Vector3d killPosition2(-60.0, 70.0, -210.0);
  d_sphere2 = new DeathSphere(startPosition2, killPosition2, 2, 8);
  assert(d_sphere2);

  // Create DeathSphere 3
  Vector3d startPosition3(35.0, 87.0, -23.0);
  Vector3d killPosition3(-50.0, 57.5, -200.0);
  d_sphere3 = new DeathSphere(startPosition3, killPosition3, 2, 8);
  assert(d_sphere3);

  // Create DeathSphere 4
  Vector3d startPosition4(-50.0, 0.0, -250.0);
  Vector3d killPosition4(-50.0, 57.5, -200.0);
  d_sphere4 = new DeathSphere(startPosition4, killPosition4, 2, 8);
  assert(d_sphere4);

  // Create DeathSphere 5
  Vector3d startPosition5(75.0, 57.5, -160.0);
  Vector3d killPosition5(-50.0, 57.5, -200.0);
  d_sphere5 = new DeathSphere(startPosition5, killPosition5, 2, 8);
  assert(d_sphere5);

  // Create DeathSphere 6
  Vector3d startPosition6(-70.0, 10.0, 10.0);
  Vector3d killPosition6(-50.0, 57.5, -200.0);
  d_sphere6 = new DeathSphere(startPosition6, killPosition6, 2, 8);
  assert(d_sphere6);

  // Create DeathSphere 7
  Vector3d startPosition7(40.0, 100.0, -10.0);
  Vector3d killPosition7(-50.0, 57.5, -200.0);
  d_sphere7 = new DeathSphere(startPosition7, killPosition7, 2, 8);
  assert(d_sphere7);

  // Create the Snake
  d_snake = 0;
  d_snake = new Snake(50.0, 5.0, 5, 8, 10, 1);
  assert(d_snake);

  return;
}

CChildView::~CChildView()
{
  if(!d_timer) {
    KillTimer(d_timer);
  }

  if(d_sphere1 != 0) {
    delete d_sphere1;
  }
  if(d_sphere2 != 0) {
    delete d_sphere2;
  }
  if(d_sphere3 != 0) {
    delete d_sphere3;
  }
  if(d_sphere4 != 0) {
    delete d_sphere4;
  }
  if(d_sphere5 != 0) {
    delete d_sphere5;
  }
  if(d_sphere6 != 0) {
    delete d_sphere6;
  }
  if(d_sphere7 != 0) {
    delete d_sphere7;
  }

  if(d_snake != 0) {
    delete d_snake;
  }

  if(d_head != 0) {
    gluDeleteQuadric(d_head);
  }

  return;
}


BEGIN_MESSAGE_MAP(CChildView, COpenGLWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
  ON_WM_CHAR()
  ON_WM_TIMER()
END_MESSAGE_MAP()

// CChildView message handlers
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!COpenGLWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(0, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), 0);

	return TRUE;
}

void CChildView::OnGLDraw(CDC *pDC) {
  if(d_first) {
    // Initialization
    d_first = false;

    // Create timer
    d_timer = SetTimer(1, 11, 0);
    if(!d_timer) { exit(0); }

#if 0
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
#endif
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set up the camera
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Determine the screen size so we can determine the aspect ratio
  int width, height;
  GetSize(width, height);
  GLdouble aspectratio = GLdouble(width) / GLdouble(height);

  // Set the camera parameters
  gluPerspective(25.0,        // Vertical field of view in degrees.
                 aspectratio, // The aspect ratio.
                 20.0,        // Near clipping
                 1000.0);     // Far clipping

  // Enable depth test
  glEnable(GL_DEPTH_TEST);

  // Cull backfacing polygons
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  // Set the camera location
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  d_camera.gluLookAt();

  // Enable lighting
  glEnable(GL_LIGHTING);
  GLfloat lmodel_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

  GLfloat materialColor[4];
  materialColor[0] = 1.0f;
  materialColor[1] = 1.0f;
  materialColor[2] = 1.0f;
  materialColor[3] = 1.0f;
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);

  glBegin(GL_LINES);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(500.0, 0.0, 0.0);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(0.0, 500.0, 0.0);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(0.0, 0.0, 500.0);
  glEnd();

  glLineWidth(3.0);
  glColor3f(1.0, 0.0, 0.0);

  // Draw stick figure
  glPushMatrix();
  glTranslated(-50.0, 0.0, -200.0);
  glBegin(GL_LINES);
  glVertex3d(-10.0, 0.0, 0.0);
  glVertex3d(0.0, 20.0, 0.0);
  glVertex3d(10.0, 0.0, 0.0);
  glVertex3d(0.0, 20.0, 0.0);
  glVertex3d(0.0, 50.0, 0.0);
  glVertex3d(0.0, 20.0, 0.0);
  glVertex3d(-20.0, 40.0, 0.0);
  glVertex3d(20.0, 40.0, 0.0);
  glEnd();
  glTranslated(0.0, 55.0, 0.0);
  gluSphere(d_head, 5.0, 32, 32);
  glPopMatrix();

  if(d_sphere1 != 0) {
    d_sphere1->Draw();
  }
  if(d_sphere2 != 0) {
    d_sphere2->Draw();
  }
  if(d_sphere3 != 0) {
    d_sphere3->Draw();
  }
  if(d_sphere4 != 0) {
    d_sphere4->Draw();
  }
  if(d_sphere5 != 0) {
    d_sphere5->Draw();
  }
  if(d_sphere6 != 0) {
    d_sphere6->Draw();
  }
  if(d_sphere7 != 0) {
    d_sphere7->Draw();
  }

  if(d_snake != 0) {
    d_snake->Draw();
  }

  glFlush();

  return;
}

// Handle keyboard
void CChildView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
  switch(nChar) {
  case 116:
    // t
    d_toggle = (d_toggle + 1) % 2;
    break;
  };

  if(!d_toggle) {
    d_camera.MouseMode(CGrCamera::PITCHYAW);
  } else {
    d_camera.MouseMode(CGrCamera::PANTILT);
  }

  return;
}

// Left mouse button pauses
void CChildView::OnLButtonDown(UINT nFlags, CPoint point) {
  if(d_pause) {
    d_pause = false;
  } else {
    d_pause = true;
  }

  return;
}

// If the right mouse button is down, then rotate the eyepoint
void CChildView::OnMouseMove(UINT nFlags, CPoint point) {
	if(nFlags & MK_RBUTTON)	{
    d_camera.MouseMove(point.x, point.y);
  }

  return;
}

// Right mouse button allows one to rotate the eyepoint
void CChildView::OnRButtonDown(UINT nFlags, CPoint point) {
  d_camera.MouseDown(point.x, point.y);
  COpenGLWnd ::OnLButtonDown(nFlags, point);

  return;
}

// The timer updates the positions all of the dynamic elements
void CChildView::OnTimer(UINT nIDEvent) {
  if(!d_pause) {
    if(d_sphere1 != 0) {
      d_sphere1->Update(0.011);
    }
    if(d_sphere2 != 0) {
      d_sphere2->Update(0.011);
    }
    if(d_sphere3 != 0) {
      d_sphere3->Update(0.011);
    }
    if(d_sphere4 != 0) {
      d_sphere4->Update(0.011);
    }
    if(d_sphere5 != 0) {
      d_sphere5->Update(0.011);
    }
    if(d_sphere6 != 0) {
      d_sphere6->Update(0.011);
    }
    if(d_sphere7 != 0) {
      d_sphere7->Update(0.011);
    }
    if(d_snake != 0) {
      d_snake->Update(0.01);
    }
  }

  // Cause the program to redraw the scene
  Invalidate();

  return;
}
