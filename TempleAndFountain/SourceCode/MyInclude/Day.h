/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __DAY_H__
#define __DAY_H__

#include "DayBase.h"

/*

 * Day - The Day class is meant to be representative of a sun.  It is 
 *   yellowish/redish colored spherical light source that rotates about
 *   <0, 0, 0> in a circular fashion.  This class inherits from the DayBase
 *   which does the work of moving and setting up the opengl light source and
 *   model.  This class is responsible for modifying the basic dynamics to
 *   "look" like the sun.

 */
class Day : public DayBase {
private:
  Day(void);
  Day(Day &);
  Day &operator=(Day &);

public:
  // Create a spherical light source with an associated model that orbits
  //   <0, 0, 0> in a circular fashion.  The light value specifies the
  //   opengl identifier for a light source.  The normal is used to determine
  //   the angle of the orbital plane.  The radius is the radius of the orbit.
  //   sphereRadius is the radius of model, which is assumed to be a sphere.
  Day(unsigned int light, const Vector3d &normal, double radius, 
      double sphereRadius, Model *model);
  ~Day(void);

  // Update the light source properties over time
  void Update(double timestep);

private:
  // These two angles are used to determine the properties of the light source
  //   depending on where it is along its orbit.
  double d_offAngle1, d_offAngle2;
};

#endif
