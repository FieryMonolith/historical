/*

 * This file was provided by Dr. Charles Owen at Michigan State University.

 */

/*

 * This class was given to me, but I modified it to use my source.

 * The ChildView class is the entry point to the graphics portion of this
 *   program, including the OnGLDraw that is used to render the scene.

 */

// ChildView.cpp : implementation of the CChildView class
//

#include <vector>
#include <string>
#include <cassert>

#include "stdafx.h"
#include "Project1-NET2003.h"
#include "ChildView.h"

#include "ModelLoader.h"
#include "Model.h"
#include "Texture.h"
#include "Eyepoint.h"
#include "EyepointImplementation.h"
#include "Day.h"
#include "Night.h"
#include "Fountain.h"
#include "Matrix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

/*** BEGIN CChildView Methods ***********************************************/
CChildView::CChildView()
: d_modelLoader(new ModelLoader),
  d_fountain(0),
  d_background(0),
  d_day(0),
  d_night(0),
  d_pause(false),
  d_first(true),
  d_timer(0),
  d_toggle(0),
  d_eyepoint(0)
{
  assert(d_modelLoader);

#ifdef __JASON_EYEPOINT__
  // Prepare mouse movement information.
  d_prevX = 0;
  d_prevY = 0;

  // Set the default eyepoint position and orientation.
  d_eyepoint = new EyepointImplementation(0.0, -72.0, 500.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
  assert(d_eyepoint);
#else
  // Prepare mouse movement information.
  d_tsm = 2;
  d_deltax = 0;
  d_deltay = 0;
  // Set the default eyepoint position and orientation.
  d_camera.Set(0.0, 72.0, -500.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  d_camera.MouseMode(CGrCamera::PITCHYAW);
#endif

  // Inherited method from COpenGLWnd
  SetDoubleBuffer(true);

  return;
}

CChildView::~CChildView()
{
  // Delete all the independent models used in this program
	vector<Model *>::iterator i;
	for(i=d_models.begin(); i!=d_models.end(); ++i) {
		if(*i != 0) {
			delete *i;
			*i = 0;
		}
	}

#ifdef __JASON_EYEPOINT__
  if(d_eyepoint != 0) {
    delete d_eyepoint;
  }
#endif
 
  if(d_modelLoader != 0) {
    delete d_modelLoader;
  }

  if(!d_timer) {
    KillTimer(d_timer);
  }

  // Delete the dependent models
  if(d_day != 0) {
    delete d_day;
  }

  if(d_night != 0) {
    delete d_night;
  }

  if(d_fountain != 0) {
    delete d_fountain;
  }

  if(d_background != 0) {
    delete d_background;
  }
  
  return;
}


// Generated code
BEGIN_MESSAGE_MAP(CChildView, COpenGLWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
  ON_WM_CHAR()
  ON_WM_TIMER()
END_MESSAGE_MAP()

// Generated code
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

// This method renders the scene
void CChildView::OnGLDraw(CDC *pDC) {
  if(d_first) { // First time through the method, do initialization
    // Initialization
    d_first = false;

    // Load the models from file for the program.
    LoadModels();

    // Generate the background
    d_background = d_modelLoader->Load("Models/background.jml");
    unsigned int bTexture = d_modelLoader->LoadTexture("Textures/mountain_512_512.bmp");
    assert(d_background);
    assert(bTexture);
    d_background->AddTexture(bTexture);
    Matrix4x4d bT(1.0, 0.0, 0.0, 0.0,
                  0.0, 0.0, 1.0, 0.0,
                  0.0, -1.0, 0.0, 0.0,
                  0.0, 0.0, 0.0, 1.0);
    d_background->Transform(bT);

    // Generate the sun and day, the models and daytime light controls
    Model *sunModel = d_modelLoader->Load("Models/sun.jml");
    assert(sunModel);
    Vector3d dayNormal(1.0, 0.0, 0.0);
    d_day = new Day(GL_LIGHT0, dayNormal, 2800.0, 250.0, sunModel);
    assert(d_day);

    // Generate the moon and night, the models and nighttime light controls
    Model *moonModel = d_modelLoader->Load("Models/moon.jml");
    assert(moonModel);
    Vector3d nightNormal(1.0, 0.0, 0.0);
    d_night = new Night(GL_LIGHT1, nightNormal, 2500.0, 225.0, moonModel);
    assert(d_night);

    // Generate the water drops used to create flowing water in the fountain
    Model *drop = d_modelLoader->Load("Models/water_sphere.jml");
    assert(drop);
    unsigned int dropTexture = d_modelLoader->LoadTexture("Textures/water_128_128.bmp");
    assert(dropTexture);
    drop->AddTexture(dropTexture);
    Model *waterTop = d_modelLoader->Load("Models/water_top.jml");
    assert(waterTop);

    // Generate the fountain
    d_fountain = new Fountain(drop, waterTop);
    assert(d_fountain);

    // Create the timer that drives the system dynamics
    d_timer = SetTimer(1, 11, 0);
    if(!d_timer) { exit(0); }

    // Enable texture and lighting
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
  }

  /*** Scene rendering ******************************************************/
  // Clear the scene
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set up the camera
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Determine the screen size so we can determine the aspect ratio
  int width, height;
  GetSize(width, height);
  GLdouble aspectratio = GLdouble(width) / GLdouble(height);

  // Set the camera parameters
  gluPerspective(25., // Vertical field of view in degrees.
  aspectratio, // The aspect ratio.
  20.0, // Near clipping
  10000.0); // Far clipping

  // Enable depth test
  glEnable(GL_DEPTH_TEST);

  // Cull backfacing polygons
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  // Set the camera location
#ifdef __JASON_EYEPOINT__
  d_eyepoint->Draw();
#else
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  d_camera.gluLookAt();
#endif

  // Enable lighting
  glEnable(GL_LIGHTING);
  GLfloat lmodel_ambient[] = { 0.05f, 0.05f, 0.05f, 0.05f };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  if(d_day != 0) {
    d_day->GenerateLight();
  }
  if(d_night != 0) {
    d_night->GenerateLight();
  }

  GLfloat materialColor[4];
  materialColor[0] = 1.0f;
  materialColor[1] = 1.0f;
  materialColor[2] = 1.0f;
  materialColor[3] = 1.0f;
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);

  // Draw the models
  vector<Model *>::const_iterator iModel;
  for(iModel = d_models.begin(); iModel != d_models.end(); ++iModel) {
    if(*iModel == 0) { continue; }
    (*iModel)->Draw();
  }

  // Draw background
  if(d_background != 0) {
    d_background->Draw();
  }

  glEnable(GL_BLEND);
  glDepthMask(GL_FALSE);

  // This was an attempt to do blue screening on the background by
  //   blending out part of the background, but it doesn't work at the moment.
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  //if(d_background != 0) {
  //  d_background->Draw();
  //}

  // Draw the fountain and make the water translucent
  glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
  materialColor[3] = 0.7f;
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
  if(d_fountain != 0) {
    d_fountain->Draw();
  }
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);

  glFlush();

  return;
}

// The keyboard callback.
// This is where a keypress is translated into the appropriate interface
//    interaction.
void CChildView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
#ifdef __JASON_EYEPOINT__
  if(d_eyepoint == 0) { return; }
  switch(nChar) {
  case 97:
    // a
    d_eyepoint->MoveRight();
    break;
  case 100:
    // d
    d_eyepoint->MoveLeft();
    break;
  case 115:
    // s
    d_eyepoint->MoveForward();
    break;
  case 120:
    //x
    d_eyepoint->MoveBackward();
    break;
  case 61:
    // =, but is for + without shift
    d_eyepoint->IncreaseForwardSpeed();
    break;
  case 45:
    // -
    d_eyepoint->DecreaseForwardSpeed();
    break;
  case 93:
    d_eyepoint->IncreaseRotationSpeed();
    break;
  case 91:
    d_eyepoint->DecreaseRotationSpeed();
    break;
  case 116:
    // t
    d_eyepoint->ToggleView();
    break;
  case 113:
    // q
    //DestroyWindow();
    break;
  case 122:
    d_eyepoint->MoveDown();
    break;
  case 99:
    d_eyepoint->MoveUp();
    break;
  case 108:
    d_eyepoint->YawRight();
    break;
  case 106:
    d_eyepoint->YawLeft();
    break;
  case 107:
    d_eyepoint->PitchUp();
    break;
  case 44:
    d_eyepoint->PitchDown();
    break;
  case 109:
    d_eyepoint->RollLeft();
    break;
  case 46:
    d_eyepoint->RollRight();
    break;
  };
#else // __JASON_EYEPOINT__
  switch(nChar) {
  case 97:
    // a
    d_camera.MouseMode(CGrCamera::ROLLMOVE);
    d_deltax -= d_tsm;
    d_camera.MouseMove(d_deltax, 0);
    break;
  case 100:
    // d
    d_deltax += d_tsm;
    d_camera.MouseMode(CGrCamera::ROLLMOVE);
    d_camera.MouseMove(d_deltax, 0);
    break;
  case 115:
    // s
    d_deltay -= d_tsm;
    d_camera.MouseMode(CGrCamera::ROLLMOVE);
    d_camera.MouseMove(0, d_deltay);
    break;
  case 120:
    //x
    d_deltay += d_tsm;
    d_camera.MouseMode(CGrCamera::ROLLMOVE);
    d_camera.MouseMove(0, d_deltay);
    break;
  case 61:
    // =, but is for + without shift
    d_tsm *= 2.0;
    if(d_tsm > 128) {
      d_tsm = 128;
    }
    break;
  case 45:
    // -
    d_tsm /= 2;
    if(d_tsm < 1) {
      d_tsm = 1;
    }
    break;
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
#endif // __JASON_EYEPOINT__

  return;
}


// The callback for the left mouse button - pauses dynamics but not eyepoint
void CChildView::OnLButtonDown(UINT nFlags, CPoint point) {
  if(d_pause) {
    d_pause = false;
  } else {
    d_pause = true;
  }

  return;
}

// The callback for mouse motion - causes the eyepoint to change
void CChildView::OnMouseMove(UINT nFlags, CPoint point) {
#ifdef __JASON_EYEPOINT__
  // Check if the right mouse button is down
	if(nFlags & MK_RBUTTON)	{
    // Check if mouse has moved in the x direction
    if(d_prevX != point.x) {
      if(d_prevX < point.x) {
        d_eyepoint->YawRight();
      } else {
        d_eyepoint->YawLeft();
      }
    }
    // Check if the mouse has moved in the y direction
    if(d_prevY != point.y) {
      if(d_prevY < point.y) {
        d_eyepoint->PitchUp();
      } else {
        d_eyepoint->PitchDown();
      }
    }

    // Store current mouse position
    if(d_prevY != point.y || d_prevX != point.x) {
      d_prevX = point.x;
      d_prevY = point.y;
    }
	}
#else
	if(nFlags & MK_RBUTTON)	{
    d_camera.MouseMove(point.x, point.y);
  }
#endif

  return;
}

// Right mouse button callback - Allows the rotation of the eyepoint within
//   the world. Default mouse motion is selection mode.
void CChildView::OnRButtonDown(UINT nFlags, CPoint point) {
#ifdef __JASON_EYEPOINT__
  d_prevX = point.x;
  d_prevY = point.y;
#else
  d_camera.MouseDown(point.x, point.y);
  COpenGLWnd ::OnLButtonDown(nFlags, point);
#endif

  return;
}

// The timer callback drives the dynamics of the system - day/night and water
void CChildView::OnTimer(UINT nIDEvent) {
  if(!d_pause) {
    d_fountain->Update(0.11/10.0);
  }
  if(!d_pause) {
    d_day->Update(0.11);
    d_night->Update(0.11);
  }
  // Cause the system to send a redraw command to the program
  Invalidate();

  return;
}

// Loads the models.  The files are recursive so they can include other
//   models to load.  This starts at the highest level.
void CChildView::LoadModels(void) {
  Model *m = 0;
  m = d_modelLoader->Load("Models/world.jml");
  if(m != 0) {
    d_models.push_back(m);
  }

  return;
}
/*** END CChildView Methods *************************************************/
