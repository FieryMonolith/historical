/*

 * Copyright 2002 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * These are three simple functions that model simple physics principles
 *   of gravity, air resistance, and ground friction.  Not all of these
 *   functions are used, but the class was stripped from one of my other
 *   projects.

 */

#include "Vector3d.h"
#include "Physics.h"

#include <iostream>
using namespace std;

void Physics::CalculateFrictionalForce(double staticMu,
                                       double kineticMu,
                                       double mass,
                                       const Vector3d &force,
                                       const Vector3d &velocity,
                                       double timeStep,
                                       Vector3d &frictionalForce)
{
  double weight = 9.8 * mass;

  // Zero out the y component since it is not used as part of the
  //   frictional force in relation to the ground
  Vector3d v(velocity[0], 0, velocity[2]);
  double vmag = Normalize(v);

  // Calculate the current force in the xz-direction
  Vector3d f(force[0], force[1], force[2]);
  double forceMagnitude = Normalize(f);

  if(vmag == 0 && force[0] == 0 && force[2] == 0) {
    // No velocity and no force, so no friction
    frictionalForce[0] = 0;
    frictionalForce[1] = 0;
    frictionalForce[2] = 0;
  } else if(vmag == 0 && forceMagnitude < staticMu * weight) {
    // If it is not moving, the object must overcome static friction before
    //   it can start moving.  Since it did not overcome it, the force in the
    //   xz-direction is zeroed out.
    frictionalForce[0] = -f[0];
    frictionalForce[1] = 0;
    frictionalForce[2] = -f[2];

    frictionalForce *= forceMagnitude;
  } else {
    // The object is moving, so friction is in the opposite direction of
    //   motion
    frictionalForce[0] = -v[0];
    frictionalForce[1] = 0;
    frictionalForce[2] = -v[2];

    frictionalForce *= kineticMu * weight;
  }

  return;
}

void Physics::CalculateAirResistance(double drag,
                                     const Vector3d &velocity,
                                     Vector3d &resistanceForce)
{
  // Air resistance occurs in the opposite direction of motion and is
  //   calculated by f_a = -||v||^2 * drag * norm(v)
  resistanceForce = velocity;
  double magnitude = Normalize(resistanceForce);
  resistanceForce *= -1;
  resistanceForce *= (magnitude*magnitude);
  resistanceForce *= drag;
  
  return;
}

void Physics::CalculateGravitationalForce(double mass,
                                          Vector3d &gravitationalForce)
{
  gravitationalForce[0] = 0.0;
  gravitationalForce[1] = -9.8 * mass;
  gravitationalForce[2] = 0.0;

  return;
}
