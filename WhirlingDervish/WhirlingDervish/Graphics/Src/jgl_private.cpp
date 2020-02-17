#ifdef VISUAL_STUDIO
#include <windows.h>
#endif
#include <map>
#include <vector>
#include <GL/gl.h>

#include "ipopulation_storage.h"

#include "jgl_private.h"

#include "defs.h"
#include "geometry.h"
#include "creature_info.h"
#include "creature.h"
#include "body.h"
#include "physical_point_info.h"
#include "physical_point.h"
#include "spring_info.h"
#include "spring.h"

using namespace std;

JGLPrivate::JGLPrivate(iPopulationStorage &ps)
  : d_ps(ps),
    d_eyepointDown(0, -1100, 0,
		   0,     1, 1,
		   0,     0, 1),
    d_grid(  0.0,  0.0,  0.0,
	   -50.0, 50.0,
	   -50.0, 50.0,
	   -50.0, 50.0,
	    10.0,  10.0, 10.0),
    d_whichView(0),
    d_width(-1), d_height(-1),
    d_nearPlaneDistance(20.0), d_farPlaneDistance(10000.0),
    d_scaleX(8.0*50.0), d_scaleY(8.0*50.0),
    d_originX(0), d_originY(0) {
  return;
}

JGLPrivate::~JGLPrivate(void) {
  return;
}

void JGLPrivate::InitializeGL(void) {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glDisable(GL_LIGHTING);

  return;
}

void JGLPrivate::ResizeGL(int width, int height) {
  glViewport(0, 0, (GLint)width, (GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glFrustum(-width/(d_scaleX*2.0), width/(d_scaleX*2.0), 
	    -height/(d_scaleY*2.0), height/(d_scaleY*2.0), 
	    d_nearPlaneDistance, d_farPlaneDistance);

  d_originX = width/2;
  d_originY = height/2;

  return;
}

void JGLPrivate::PaintGL(void) const {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  DrawEyepoint();
  DrawGrid();

  //glPointSize(4.0);
  //glLineWidth(2.0);
  glPointSize(4.0);
  glLineWidth(1.0);
  DrawExternalOpengl();
  glPointSize(1.0);
  glLineWidth(1.0);

  return;
}

void JGLPrivate::DrawEyepoint(void) const {
  if(d_whichView == 1) {
    d_eyepointDown.Draw();
  } else {
    d_eyepoint.Draw();
  }

  return;
}

void JGLPrivate::DrawGrid(void) const {
  d_grid.Draw();

  return;
}

void JGLPrivate::DrawExternalOpengl(void) const {
  glColor3f(1.0, 1.0, 1.0);

  const vector<CreatureInfo *> &cis = d_ps.Population();
  vector<CreatureInfo *>::const_iterator ici;

  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_POINTS);
  for(ici=cis.begin(); ici!=cis.end(); ++ici) {
    if(*ici == NULL) { continue; }

    const Body &body = (*ici)->GetCreature().GetBody();

    const map<int64, PhysicalPointInfo *> &ps = body.GetPoints();
    map<int64, PhysicalPointInfo *>::const_iterator ip;
    for(ip=ps.begin(); ip!= ps.end(); ++ip) {
      if(ip->second == NULL) { continue; }

      const PhysicalPoint &p = ip->second->GetPhysicalPoint();

      glVertex3dv(p.Position().Array());
    }
  }
  glEnd();

  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);
  for(ici=cis.begin(); ici!=cis.end(); ++ici) {
    if(*ici == NULL) { continue; }

    const Body &body = (*ici)->GetCreature().GetBody();

    const map<int64, Body::ConnectionData> &connections = body.Connections();
    map<int64, Body::ConnectionData>::const_iterator iconnection;
    for(iconnection=connections.begin(); iconnection!=connections.end();
	++iconnection) {
      if(iconnection->second.d_point1 == NULL ||
	 iconnection->second.d_point2 == NULL) {
	continue;
      }

      glVertex3dv(iconnection->second.d_point1->GetPhysicalPoint().Position().Array());
      glVertex3dv(iconnection->second.d_point2->GetPhysicalPoint().Position().Array());
    }
  }
  glEnd();

  return;
}

void JGLPrivate::ConvertWindowCoordinates(Vector3d &to, int fromX, int fromY, 
					  const Vector3d &eyepointLocation,
					  const Vector3d &out) {
  fromY = d_height - fromY;
  fromX -= d_originX;
  fromY -= d_originY;

  double x = (double)fromX / d_scaleX;
  double y = (double)fromY / d_scaleY;

  Vector3d right = d_eyepoint.RightVector();
  Normalize(right);
  Vector3d up = d_eyepoint.UpVector();
  Normalize(up);

  to = eyepointLocation;
  to += (d_nearPlaneDistance * out);
  to += (x*right + y*up);

  return;
}

Eyepoint &JGLPrivate::GetEyepoint(void) {
  if(d_whichView == 1) {
    return (Eyepoint &)d_eyepointDown;
  }

  return (Eyepoint &)d_eyepoint;
}

void JGLPrivate::NormalView(void) {
  d_whichView = 0;

  return;
}

void JGLPrivate::DownView(void) {
  d_whichView = 1;

  return;
}
