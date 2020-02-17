/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __DAY_BASE_H__
#define __DAY_BASE_H__

#include <vector>
#include "Vector3d.h"

class Model;

/*

 * DayBase - The DayBase class is slightly misnamed.  This class encapsulates
 *   a model and associated light source.  It also contains all the data
 *   necessary to move the model and light source through a circular orbit of
 *   <0,0,0> within the scene.  It also assumes the model is circular and the
 *   model radius is specified in the constructor.

 */
class DayBase {
private:
  DayBase(void);
  DayBase(const DayBase &);
  DayBase &operator=(DayBase &);

public:
  // Create a spherical light source with an associated model that orbits
  //   <0, 0, 0> in a circular fashion.  The light value specifies the
  //   opengl identifier for a light source.  The normal is used to determine
  //   the angle of the orbital plane.  The radius is the radius of the orbit.
  //   sphereRadius is the radius of model, which is assumed to be a sphere.
  DayBase(unsigned int light, const Vector3d &normal, double radius, 
          double sphereRadius, Model *model);
  ~DayBase(void);

  // Draw the light model
  void Draw(void) const;

  unsigned int Light(void)    const { return d_light; }
  const float *Location(void) const { return d_sunLocation; }
  const float *Ambient(void)  const { return d_ambient; }
  const float *Diffuse(void)  const { return d_diffuse; }
  const float *Specular(void) const { return d_specular; }

  // Is the light source enabled (turned on)?
  bool Enabled(void) const { return d_enabled; }

  // Enable/Disable the light in opengl with the current properties.
  void GenerateLight(void) const;

  // Move the model and light source through its orbit by an amount dependent
  //   on the amount of time that elapsed.
  void Rotate(double timeIncrement);

protected:
  Model *d_model;
  unsigned int d_light;
  float d_sunLocation[4];
  float d_ambient[4];
  float d_diffuse[4];
  float d_specular[4];

  double d_radius;
  double d_sphereRadius;
  double d_angle;
  bool d_enabled;

  Vector3d d_position;
  Vector3d d_normal;

  // Compute the actual new location of the model and light source.
  void Rotate(Vector3d &p, double angle, double radius);
};

#endif
