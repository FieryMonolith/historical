/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * DeathSphere - DeathSphere is a supposed to be one of those killing spheres
 *   from the movie Phantasm (as specified in the project description).
 *   Essentially, the DeathSphere is an object that moves along a curved
 *   path (a corkscrew style path) and always keeping itself oriented so that
 *   its "front" is points at the destination.

 */

#include "stdafx.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include <cmath>
#include <gl/gl.h>
#include <gl/glu.h>

#include "Vector3d.h"

#include "DeathSphere.h"

#include "Path.h"
#include "Curves.h"
#include "Rotation.h"
#include "Matrix.h"

// The path is a corkscrew, so the number of control points is equal to
//   the number of revolutions * divisions + 1.  Essentially, for each
//   revolution in the corkscrew pattern, how many control points should
//   define it.
DeathSphere::DeathSphere(const Vector3d &startLocation,
                         const Vector3d &killPoint,
                         unsigned int revolutions,
                         unsigned int divisions)
: d_sphereRadius(2.5),
  d_drillLength(5.0),
  d_drillRadius(2.0),
  d_location(0),
  d_killPoint(0),
  d_position(0.0),
  d_path(0),
  d_sphereQuadric(0),
  d_cylinderQuadric(0)
{
  // Create the quadric that will be used as the model for the sphere
  //   portion of this object
  d_sphereQuadric = gluNewQuadric();
  assert(d_sphereQuadric);
  gluQuadricOrientation(d_sphereQuadric, GLU_OUTSIDE);

  // Create the quadric that will be used as the model for the drill portion
  //   of this object
  d_cylinderQuadric = gluNewQuadric();
  assert(d_cylinderQuadric);
  gluQuadricOrientation(d_sphereQuadric, GLU_OUTSIDE);

  // Initialize the location
  d_location = new Vector3d(startLocation);
  assert(d_location);

  // Initialize the kill point
  d_killPoint = new Vector3d(killPoint);
  assert(d_killPoint);

  // Create the corkscrew path the DeathSphere will take before hitting the
  //   target.
  assert(divisions);
  assert(revolutions);
  unsigned int pathSize = (divisions * revolutions) + 1;
  if(pathSize < 3) {
    assert(0);
  }

  d_path = new Path;
  assert(d_path);

  double angleIncrement = 2.0 * M_PI / (double)divisions;
  double angle = 0.0;
  double segmentLength = Distance(killPoint, startLocation) / (pathSize - 1);
  double radius = 25.1;
  double radiusReductor = 25.0 / (pathSize - 1);
  for(unsigned int i=0; i<pathSize; i++) {
    if(i % divisions == 0) {
      angle = 0.0;
    }

    Vector3d t(cos(angle)*radius, sin(angle)*radius, (double)i * segmentLength);
    d_path->AddPoint(t);
    angle += angleIncrement;
    radius -= radiusReductor;
  }

  return;
}

DeathSphere::~DeathSphere(void) {
  if(d_sphereQuadric != 0) {
    gluDeleteQuadric(d_sphereQuadric);
  }
  if(d_cylinderQuadric != 0) {
    gluDeleteQuadric(d_cylinderQuadric);
  }

  if(d_location != 0) {
    delete d_location;
    d_location = 0;
  }
  if(d_killPoint != 0) {
    delete d_killPoint;
    d_killPoint = 0;
  }
  if(d_path != 0) {
    delete d_path;
    d_path = 0;
  }

  return;
}

// Draw the DeathSphere
void DeathSphere::Draw(void) const {
  // Compute where the DeathSphere is along its path
  Vector3d p;
  d_path->CurvePosition(p, d_position);

  // Calculate the move and orientation of the DeathSphere so that it is
  //   always pointing at the target location
  Vector3d t(*d_killPoint - *d_location);
  Normalize(t);
  Matrix4x4d orientation = RotationFromZ(t);
  Orthonormalize(orientation);
  Matrix4x4d off(1.0, 0.0, 0.0, d_location->at(0),
                 0.0, 1.0, 0.0, d_location->at(1),
                 0.0, 0.0, 1.0, d_location->at(2),
                 0.0, 0.0, 0.0, 1.0);
  Matrix4x4d loc(1.0, 0.0, 0.0, p[0],
                 0.0, 1.0, 0.0, p[1],
                 0.0, 0.0, 1.0, p[2],
                 0.0, 0.0, 0.0, 1.0);
  Matrix4x4d newLoc = off * orientation * loc;
  p[0] = newLoc.At(0,3);
  p[1] = newLoc.At(1,3);
  p[2] = newLoc.At(2,3);

  // Draw Sphere
  t = *d_killPoint - p;
  Normalize(t);
  Matrix4x4d temp = RotationFromZ(t);
  Orthonormalize(temp);

  glPushMatrix();
  glTranslated(p[0], p[1], p[2]);
  glMultMatrixd(Transpose(temp).Array());

  float materialColor[4];
  materialColor[0] = 0.5f;
  materialColor[1] = 0.5f;
  materialColor[2] = 0.5f;
  materialColor[3] = 1.0f;
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
  gluSphere(d_sphereQuadric, d_sphereRadius, 32, 32);

  materialColor[0] = 1.0f;
  materialColor[1] = 0.0f;
  materialColor[2] = 0.0f;
  materialColor[3] = 1.0f;
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
  gluCylinder(d_cylinderQuadric, d_drillRadius, d_drillRadius/10.0, d_drillLength, 16, 1);
  glPopMatrix();

  return;
}

// Update the DeathSphere's position along its path
void DeathSphere::Update(double timestep) {
  timestep *= 10.0;

  d_position += timestep;
  d_position = fmod(d_position, d_path->PathUnitLength()-1.0);

  return;
}
