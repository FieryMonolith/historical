/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __NIGHT_H__
#define __NIGHT_H__

#include "DayBase.h"

/*

 * Night - The Night class is meant to be representative of a moon.  It is 
 *   whitish/greyish colored spherical light source that rotates about
 *   <0, 0, 0> in a circular fashion.  This class inherits from the DayBase
 *   which does the work of moving and setting up the opengl light source and
 *   model.  This class is responsible for modifying the basic dynamics to
 *   "look" like the moon.

 */
class Night : public DayBase {
private:
  Night(void);
  Night(const Night &);
  Night &operator=(Night &);

public:
  // Create a spherical light source with an associated model that orbits
  //   <0, 0, 0> in a circular fashion.  The light value specifies the
  //   opengl identifier for a light source.  The normal is used to determine
  //   the angle of the orbital plane.  The radius is the radius of the orbit.
  //   sphereRadius is the radius of model, which is assumed to be a sphere.
  Night(unsigned int light, const Vector3d &normal, double radius, 
        double sphereRadius, Model *model);
  ~Night(void);

  // Update the light source properties over time
  void Update(double timestep);

private:
  // These two angles are used to determine the properties of the light source
  //   depending on where it is along its orbit.
  double d_offAngle1, d_offAngle2;
};

#endif
