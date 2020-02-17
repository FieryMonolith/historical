/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * Day - The Day class is meant to be representative of a sun.  It is 
 *   yellowish/redish colored spherical light source that rotates about
 *   <0, 0, 0> in a circular fashion.  This class inherits from the DayBase
 *   which does the work of moving and setting up the opengl light source and
 *   model.  This class is responsible for modifying the basic dynamics to
 *   "look" like the sun.

 */

#define _USE_MATH_DEFINES
#include <cmath>

#include "Day.h"

#include "Matrix.h"
#include "model.h"

// Create a spherical light source with an associated model that orbits
//   <0, 0, 0> in a circular fashion.  The light value specifies the
//   opengl identifier for a light source.  The normal is used to determine
//   the angle of the orbital plane.  The radius is the radius of the orbit.
//   sphereRadius is the radius of model, which is assumed to be a sphere.
Day::Day(unsigned int light, const Vector3d &normal, double radius, 
         double sphereRadius, Model *model)
  : DayBase(light, normal, radius, sphereRadius, model) {
  // The following angles determine when the sun is out of sight and therefore
  //   off/on.  This also determines the color of the sun as it changes as
  //   it approaches setting/rising.
  // offAngle1 is the "sun rise"
  d_offAngle1 = 15.0 * M_PI / 180.0;
  // offAngle2 is the "sun set"
  d_offAngle2 = 165.0 * M_PI / 180.0;

  // Default position along the orbit.
  d_position[0] = 0.0; d_position[1] = d_radius; d_position[2] = 0.0;
  d_angle = M_PI/2.0;

  // Default values
  d_sunLocation[0] = (float)d_position[0];
  d_sunLocation[1] = (float)d_position[1] - (float)d_sphereRadius - 50.0f;
  d_sunLocation[2] = (float)d_position[2];
  d_sunLocation[3] = (float)1.0;

  if(d_model != 0) {
    Matrix4x4d t( 1.0, 0.0, 0.0, d_position[0],
                  0.0, 1.0, 0.0, d_position[1],
                  0.0, 0.0, 1.0, d_position[2],
                  0.0, 0.0, 0.0, 1.0);
    d_model->Transform(t);
  }

  return;
}

Day::~Day(void) {
  return;
}

// This uses the offset angles to determine when the sun is out of sight
//   and therefore off/on.  This also determines the color of the sun as
//   it changes as it approaches setting/rising.
void Day::Update(double timestep) {
  Rotate(timestep);

  // Is it out of sight (off) ?
  if(d_angle >= d_offAngle1 && d_angle <= d_offAngle2) {
    d_enabled = true;
  } else {
    d_enabled = false;
  }

  // move the angle to the light source's position along the orbital path
  double angle = d_angle;
  if(angle < 90. * M_PI / 180.0) {
    angle -= d_offAngle1;
  } else {
    angle += d_offAngle1;
  }
  // Modify the light source properties based on its angle
  float ambientLevel = (float)sin(angle);
  d_ambient[0] = ambientLevel;
  d_ambient[1] = ambientLevel;
  d_ambient[2] = ambientLevel;
  d_ambient[3] = ambientLevel;

  d_diffuse[1] = ambientLevel;
  d_diffuse[2] = ambientLevel;
  d_specular[1] = ambientLevel;
  d_specular[2] = ambientLevel;

  return;
}
