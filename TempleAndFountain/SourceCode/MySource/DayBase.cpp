/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * DayBase - The DayBase class is slightly misnamed.  This class encapsulates
 *   a model and associated light source.  It also contains all the data
 *   necessary to move the model and light source through a circular orbit of
 *   <0,0,0> within the scene.  It also assumes the model is circular and the
 *   model radius is specified in the constructor.

 */

#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include <gl/gl.h>
#include <gl/glu.h>

#include "model.h"

#include "DayBase.h"

#include "Matrix.h"

using namespace std;

// Create a spherical light source with an associated model that orbits
//   <0, 0, 0> in a circular fashion.  The light value specifies the
//   opengl identifier for a light source.  The normal is used to determine
//   the angle of the orbital plane.  The radius is the radius of the orbit.
//   sphereRadius is the radius of model, which is assumed to be a sphere.
DayBase::DayBase(unsigned int light, const Vector3d &normal, double radius, 
                 double sphereRadius, Model *model)
{
  d_normal = normal;
  Normalize(d_normal);

  d_light = light;
  d_enabled = true;

  d_radius = radius;
  d_sphereRadius = sphereRadius;

  d_ambient[0] = 1.0f; d_ambient[1] = 1.0f;
  d_ambient[2] = 1.0f; d_ambient[3] = 1.0f;

  d_diffuse[0] = 1.0f; d_diffuse[1] = 1.0f;
  d_diffuse[2] = 1.0f; d_diffuse[2] = 1.0f;

  d_specular[0] = 1.0f; d_specular[1] = 1.0f;
  d_specular[2] = 1.0f; d_specular[2] = 1.0f;

  d_model = model;

  return;
}

DayBase::~DayBase(void) {
  if(d_model != 0) {
    delete d_model;
  }

  return;
}

// Draw the light source model
void DayBase::Draw(void) const {
  if(d_model != 0) {
    d_model->Draw();
  }

  return;
}

// Update the light source position along its orbit.
void DayBase::Rotate(double timeIncrement) {
  double rotationAngle = (timeIncrement / 240.0) * 2.0 * M_PI;
  d_angle += rotationAngle;
  d_angle = fmod(d_angle, 2.0 * M_PI);

  Vector3d sun(d_position);
  Rotate(sun, rotationAngle, d_radius - d_sphereRadius - 50.0);
  d_sunLocation[0] = (float)sun[0];
  d_sunLocation[1] = (float)sun[1];
  d_sunLocation[2] = (float)sun[2];

  d_model->Transform(Matrix4x4d(1.0, 0.0, 0.0, -d_position[0],
                                0.0, 1.0, 0.0, -d_position[1],
                                0.0, 0.0, 1.0, -d_position[2],
                                0.0, 0.0, 0.0, 1.0));
  Rotate(d_position, rotationAngle, d_radius);
  d_model->Transform(Matrix4x4d(1.0, 0.0, 0.0, d_position[0],
                                0.0, 1.0, 0.0, d_position[1],
                                0.0, 0.0, 1.0, d_position[2],
                                0.0, 0.0, 0.0, 1.0));

  return;
}

// Calculate the actual position of the light source along its orbit.  An orbit
//   is a circle about the origin through the plane defined by normal.
void DayBase::Rotate(Vector3d &p, double angle, double radius) {
  Vector3d n(d_normal);
  Vector3d v(p);
  Normalize(v);

  Vector3d location(0.0, 0.0, 0.0);
  double costheta = radius*cos(angle);
  double sintheta = radius*sin(angle);

  location[0] += (costheta + (1 - costheta) * n[0] * n[0]) * v[0];
  location[0] += ((1 - costheta) * n[0] * n[1] - n[2] * sintheta) * v[1];
  location[0] += ((1 - costheta) * n[0] * n[2] + n[1] * sintheta) * v[2];
  if(location[0] < 0.0000000001 && location[0] > -0.0000000001) {
    location[0] = 0.0;
  }

  location[1] += ((1 - costheta) * n[0] * n[1] + n[2] * sintheta) * v[0];
  location[1] += (costheta + (1 - costheta) * n[1] * n[1]) * v[1];
  location[1] += ((1 - costheta) * n[1] * n[2] - n[0] * sintheta) * v[2];
  if(location[1] < 0.0000000001 && location[1] > -0.0000000001) {
    location[1] = 0.0;
  }

  location[2] += ((1 - costheta) * n[0] * n[2] - n[1] * sintheta) * v[0];
  location[2] += ((1 - costheta) * n[1] * n[2] + n[0] * sintheta) * v[1];
  location[2] += (costheta + (1 - costheta) * n[2] * n[2]) * v[2];
  if(location[2] < 0.0000000001 && location[2] > -0.0000000001) {
    location[2] = 0.0;
  }

  p = location;

  return;
}

// Enable/Disable the opengl light source with the current properties.
void DayBase::GenerateLight(void) const {
  if(d_enabled) {
    glEnable(d_light);
    glLightfv(d_light, GL_POSITION, Location());
    glLightfv(d_light, GL_DIFFUSE,  Diffuse());
    glLightfv(d_light, GL_SPECULAR, Specular());
    glLightfv(d_light, GL_AMBIENT,  Ambient());

    Draw();
  } else {
    glDisable(d_light);
  }

  return;
}
