/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __DEATH_SPHERE_H__
#define __DEATH_SPHERE_H__

class Path;
class Vector3d;
class Matrix4x4d;

/*

 * DeathSphere - DeathSphere is a supposed to be one of those killing spheres
 *   from the movie Phantasm (as specified in the project description).
 *   Essentially, the DeathSphere is an object that moves along a curved
 *   path (a corkscrew style path) and always keeping itself oriented so that
 *   its "front" is points at the destination.

 */
class DeathSphere {
public:
  // The path is a corkscrew, so the number of control points is equal to
  //   the number of revolutions * divisions + 1.  Essentially, for each
  //   revolution in the corkscrew pattern, how many control points should
  //   define it.
  DeathSphere(const Vector3d &startLocation, const Vector3d &killPoint,
              unsigned int revolutions, unsigned int divisions);
  ~DeathSphere(void);

  // Move the DeathSphere
  void Update(double timestep);
  // Draw the DeathSphere
  void Draw(void) const;

private:
  double d_sphereRadius;
  double d_drillLength;
  double d_drillRadius;

  Vector3d *d_location;
  Vector3d *d_killPoint;
  double d_position; // Position along the path
  Path *d_path;      // Control points that define the path

  // Quadrics used as the DeathSphere models
  GLUquadric *d_sphereQuadric;
  GLUquadric *d_cylinderQuadric;
};

#endif
