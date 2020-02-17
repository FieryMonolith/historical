/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * The following four functions: RotationFromZ, RotationToZ,
 *   CalculateRotationAnglesToZ, and Orthonormalize are used to orient models.

 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>

#include "Rotation.h"

// Generates a rotation matrix from <0, 0, 1> to the given vector(to)
Matrix4x4d RotationFromZ(const Vector3d &to) {
  double theta = 0.0;
  double phi = 0.0;
  assert(CalculateRotationAnglesToZ(theta, phi, to));

  theta = (2.0 * M_PI) - theta;
  phi = (2.0 * M_PI) - phi;

  double ct = cos(theta);
  double cp = cos(phi);
  double st = sin(theta);
  double sp = sin(phi);
  return Matrix4x4d( ct,  sp*st,  st*cp, 0.0,
                    0.0,     cp,    -sp, 0.0,
                    -st,   sp*ct,  cp*ct, 0.0,
                    0.0,    0.0,    0.0, 1.0);
}

// Generates a rotation matrix from the given vector (from) to <0, 0, 1>
Matrix4x4d RotationToZ(const Vector3d &from) {
  double theta = 0.0;
  double phi = 0.0;
  assert(CalculateRotationAnglesToZ(theta, phi, from));

  double ct = cos(theta);
  double cp = cos(phi);
  double st = sin(theta);
  double sp = sin(phi);
  return Matrix4x4d(    ct, 0.0,     st, 0.0,
                     sp*st,  cp, -sp*ct, 0.0,
                    -st*cp,  sp,  cp*ct, 0.0,
                       0.0, 0.0,    0.0, 1.0);
}

// Given a vector, what is the spherical coordinate rotations to change it
//   the vector <0, 0, 1>
bool CalculateRotationAnglesToZ(double &theta, double &phi,
                                const Vector3d &direction)
{
  if(direction[0] == 0 && direction[1] == 0 && direction[2] == 0) {
    return false;
  }

  // Convert the direction values to be positive in order to check
  //   boundary conditions
  double xc = direction[0];
  if(xc < 0) { xc *= -1.0; }
  double yc = direction[1];
  if(yc < 0) { yc *= -1.0; }
  double zc = direction[2];
  if(zc < 0) { zc *= -1.0; }

  // Determine if the direction vector is essentially pointing straight up or
  //   down.
  if(xc < 0.00001 && direction[1] > 0.99999 && zc < 0.00001) {
    phi = M_PI / 2.0;
    theta = 0.0;
    return true;
  } else if(xc < 0.00001 && direction[1] < -0.99999 && zc < 0.00001) {
    phi = 3.0 * M_PI / 2.0;
    theta = 0.0;
    return true;
  }

  Vector3d z(0.0, 0.0, 1.0);
  Vector3d v(direction);
  Normalize(v);

  // Theta is the angle in the xz-plane
  Vector3d thetaV(v[0], 0.0, v[2]);
  Normalize(thetaV);

  // Phi is the angle in the yz-plane
  Vector3d phiV(0.0, v[1], v[2]);
  Normalize(phiV);

  // Compute theta
  theta = acos(Inner(z, thetaV));
  if(thetaV[0] > 0) {
    theta = (2.0 * M_PI) - theta;
  }

  if(phiV[2] < 0) {
    phiV[2] *= -1.0;
  }

  // Check if the phiV vector is in the xy-plane instead of yz-plane
  if(xc > 0.00001 && yc > 0.00001 && zc < 0.00001) {
    phiV[0] = xc;
    phiV[2] = 0.0;
    z = Vector3d(1.0, 0.0, 0.0);
  }

  // Compute phi
  // 3*pi/2 -> pi/2
  phi = acos(Inner(z, phiV));
  if(phiV[1] == 0) {
    phi = 0.0;
  } else if(phiV[1] < 0) {
    phi = (2.0 * M_PI) - phi;
  }

  return true;
}

// Normalize the given matrix such that pitch, yaw, roll, and position are
//   all unit vectors.
void Orthonormalize(Matrix4x4d &m) {
  Vector3d left(m.At(0,0), m.At(0,1), m.At(0,2));
  Vector3d up(m.At(1,0), m.At(1,1), m.At(1,2));

  Normalize(left);
  Normalize(up);

  // NEED to verify that the vectors are still perpendicular to each other

  // Can get funny stuff with opengl, because z is into the screen instead
  // of out of the screen

  Vector3d out = Cross(left, up);
  Normalize(out);

  left = Cross(up, out);
  Normalize(left);

  m.At(0,0) = left[0];
  m.At(0,1) = left[1];
  m.At(0,2) = left[2];

  m.At(1,0) = up[0];
  m.At(1,1) = up[1];
  m.At(1,2) = up[2];

  m.At(2,0) = out[0];
  m.At(2,1) = out[1];
  m.At(2,2) = out[2];

  return;
}
