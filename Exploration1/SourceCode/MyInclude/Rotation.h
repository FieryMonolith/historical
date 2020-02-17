/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __JROTATION_H__
#define __JROTATION_H__

#include "matrix.h"
#include "Vector3d.h"

/*

 * The following four functions: RotationFromZ, RotationToZ,
 *   CalculateRotationAnglesToZ, and Orthonormalize are used to orient models.

 */

// Generates a rotation matrix from <0, 0, 1> to the given vector(to)
Matrix4x4d RotationFromZ(const Vector3d &to);

// Generates a rotation matrix from the given vector (from) to <0, 0, 1>
Matrix4x4d RotationToZ(const Vector3d &from);

// Given a vector, what is the spherical coordinate rotations to change it
//   the vector <0, 0, 1>
bool CalculateRotationAnglesToZ(double &theta, double &phi,
                                const Vector3d &direction);

// Normalize the given matrix such that pitch, yaw, roll, and position are
//   all unit vectors.
void Orthonormalize(Matrix4x4d &m);

#endif
