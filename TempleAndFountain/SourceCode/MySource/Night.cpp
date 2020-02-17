/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * Night - The Night class is meant to be representative of a moon.  It is 
 *   whitish/greyish colored spherical light source that rotates about
 *   <0, 0, 0> in a circular fashion.  This class inherits from the DayBase
 *   which does the work of moving and setting up the opengl light source and
 *   model.  This class is responsible for modifying the basic dynamics to
 *   "look" like the moon.

 */

#define _USE_MATH_DEFINES
#include <cmath>

#include "Night.h"

#include "Matrix.h"
#include "model.h"

// Create a spherical light source with an associated model that orbits
//   <0, 0, 0> in a circular fashion.  The light value specifies the
//   opengl identifier for a light source.  The normal is used to determine
//   the angle of the orbital plane.  The radius is the radius of the orbit.
//   sphereRadius is the radius of model, which is assumed to be a sphere.

// In this case the spherical light source is a moon and colored appropriately.
Night::Night(unsigned int light, const Vector3d &normal, double radius, 
             double sphereRadius, Model *model)
: DayBase(light, normal, radius, sphereRadius, model)
{
  // The following angles determine when the moon is out of sight and therefore
  //   off/on.  This also determines the color of the moon as it changes as
  //   it approaches setting/rising.
  // offAngle1 is the "moon rise"
  d_offAngle1 = 15.0 * M_PI / 180.0;
  // offAngle2 is the "moon set"
  d_offAngle2 = 165.0 * M_PI / 180.0;

  // Default position along the orbit.
  d_position[0] = 0.0; d_position[1] = -d_radius; d_position[2] = 0.0;
  d_angle = 3.0*M_PI/2.0;

  // Default values
  d_sunLocation[0] = (float)d_position[0];
  d_sunLocation[1] = (float)d_position[1] + (float)d_sphereRadius + 50.0f;
  d_sunLocation[2] = (float)d_position[2];
  d_sunLocation[3] = (float)1.0;

  d_diffuse[0] = 0.5f;
  d_diffuse[1] = 0.5f;
  d_diffuse[2] = 0.5f;
  d_diffuse[3] = 0.5f;
  d_specular[0] = 0.5f;
  d_specular[1] = 0.5f;
  d_specular[2] = 0.5f;
  d_specular[3] = 0.5f;

  if(d_model != 0) {
    Matrix4x4d t( 1.0, 0.0, 0.0, d_position[0],
                  0.0, 1.0, 0.0, d_position[1],
                  0.0, 0.0, 1.0, d_position[2],
                  0.0, 0.0, 0.0, 1.0);
    d_model->Transform(t);
  }

  d_enabled = false;

  return;
}

Night::~Night(void) {
  return;
}

// This uses the offset angles to determine when the moon is out of sight
//   and therefore off/on.  This also determines the color of the moon as
//   it changes as it approaches setting/rising.
void Night::Update(double timestep) {
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
  d_ambient[0] = ambientLevel*0.25f;
  d_ambient[1] = ambientLevel*0.25f;
  d_ambient[2] = ambientLevel*0.25f;
  d_ambient[3] = ambientLevel*0.25f;

#if 0
  d_diffuse[0] = ambientLevel*0.25f;
  d_diffuse[1] = ambientLevel*0.25f;
  d_diffuse[2] = ambientLevel*0.25f;
  d_diffuse[3] = ambientLevel*0.25f;
  d_specular[0] = ambientLevel*0.25f;
  d_specular[1] = ambientLevel*0.25f;
  d_specular[2] = ambientLevel*0.25f;
  d_specular[3] = ambientLevel*0.25f;
#else
#endif

  return;
}
