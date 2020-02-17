/*

 * Copyright 2002 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __PHYSICS_H__
#define __PHYSICS_H__

class Vector3d;

/*

 * These are three simple functions that model simple physics principles
 *   of gravity, air resistance, and ground friction.  Not all of these
 *   functions are used, but the class was stripped from one of my other
 *   projects.

 */

class Physics {
public:
  // Output is returned in frictionalForce
  static void CalculateFrictionalForce(double staticMu,
                                       double kineticMu,
                                       double mass,
                                       const Vector3d &force,
                                       const Vector3d &velocity,
                                       double timeStep,
                                       Vector3d &frictionalForce);

  // Output is returned in resistanceForce
  static void CalculateAirResistance(double drag,
                                     const Vector3d &velocity,
                                     Vector3d &resistanceForce);

  // Output is returned in graitationalForce
  static void CalculateGravitationalForce(double mass,
                                          Vector3d &gravitationalForce);
};

#endif
