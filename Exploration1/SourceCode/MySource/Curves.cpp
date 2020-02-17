/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * Curves - The Curves class holds the algorithms that determine the location
 *   along a curve, defined by four control points, given a proportial
 *   distance from the start of the curve.

 */

#include "Vector3d.h"

#include "Curves.h"

#include "Matrix.h"

// Determine the location (out) from the start of the curve using a
//   proportional distance (t).  The curve is defined by four control
//   points.
bool Curves::CatmullRom(Vector3d &out,
                         double t,
                         const Vector3d &p0,
                         const Vector3d &p1,
                         const Vector3d &p2,
                         const Vector3d &p3)
{
  Vector3d tangent0((p2 - p0) / 2.0);
  Vector3d tangent1((p3 - p1) / 2.0);

  return Ferguson(out, t, p1, p2, tangent0, tangent1);
}

// Determine the tangent (out) at a position relative to the start of the
//   curve using a proportional distance (t).  The curve is defined by four
//   control points.
bool Curves::CatmullRomTangent(Vector3d &out,
                                double t,
                                const Vector3d &p0,
                                const Vector3d &p1,
                                const Vector3d &p2,
                                const Vector3d &p3)
{
  Vector3d tangent0((p2 - p0) / 2.0);
  Vector3d tangent1((p3 - p1) / 2.0);

  return FergusonTangent(out, t, p1, p2, tangent0, tangent1);
}

// Determine the location (out) from the start of the curve using a
//   proportional distance (t).  The curve is defined by two control
//   points and the tangent of the curve at those positions.
bool Curves::Ferguson(Vector3d &out,
                       double t,
                       const Vector3d &p0,
                       const Vector3d &p1,
                       const Vector3d &tangent0,
                       const Vector3d &tangent1)
{
  if(t < 0) {
    // The location is not within this curve segment, return start point
    out = p0;
    return false;
  } else if(t > 1) {
    // The location is not within this curve segment, return end point
    out = p1;
    return false;
  }

  const double t2 = t * t;
  const double t3 = t2 * t;

  // This shows the Ferguson algorithm in a fuller form
  /*
    P(t)  = a0 + a1*t + a2*t^2 + a3*t3
    P(0)  = a0
    P'(0) = a1
    P(1)  = a0 + a1 + a2 + a3
    P'(1) = a1 + 2*a2 + 3*a3

    P(U) = [1 t t^2 t^3][ 1  0  0  0 ][ P(0)  ]
                          0  0  1  0    P(1)
                         -3  3 -2 -1    P'(0)
                          2 -2  1  1    P'(1)

    out is everything multiplied together

    a0 = P(0)
    a1 = P'(0)
    a2 = 3*P(1) - 3*P(0) - 2*P'(0) - P'(1)
    a3 = 2*P(0) - 2*P(1) + P'(0) + P'(1)
  */

  const double i0 = 1.0 +     (-3.0 * t2) + (2.0 * t3);
  const double i1 =           ( 3.0 * t2) + (-2.0 * t3);
  const double i2 =       t + (-2.0 * t2) + t3;
  const double i3 =           -t2         + t3;

  out = (i0 * p0) + (i1 * p1) + (i2 * tangent0) + (i3 * tangent1);

  return true;
}

// Determine the tangent (out) at a position relative to the start of the
//   curve using a proportional distance (t).  The curve is defined by two
//   control points and the tangent of the curve at those positions.
bool Curves::FergusonTangent(Vector3d &out, double t, const Vector3d &p0, const Vector3d &p1,
                              const Vector3d &tangent0, const Vector3d &tangent1) {
  if(t < 0) {
    // The location is not within this curve segment, return start point
    out = p0;
    return false;
  } else if(t > 1) {
    // The location is not within this curve segment, return end point
    out = p1;
    return false;
  }

  // This shows the Ferguson algorithm in a fuller form
  /*
    P(t)  = a0 + a1*t + a2*t^2 + a3*t3
    P(0)  = a0
    P'(0) = a1
    P(1)  = a0 + a1 + a2 + a3
    P'(1) = a1 + 2*a2 + 3*a3

    P(U) = [1 t t^2 t^3][ 1  0  0  0 ][ P(0)  ]
                          0  0  1  0    P(1)
                         -3  3 -2 -1    P'(0)
                          2 -2  1  1    P'(1)

    out = P'(t) = a1 + 2*a2*t + 3*a3*t^2

    a0 = P(0)
    a1 = P'(0)
    a2 = 3*P(1) - 3*P(0) - 2*P'(0) - P'(1)
    a3 = 2*P(0) - 2*P(1) + P'(0) + P'(1)
  */

  out = tangent0 +
        (2.0 * ((3.0 * p1) - (3.0 * p0) - (2.0 * tangent0) - tangent1) * t) +
        (3.0 * ((2.0 * p0) - (2.0 * p1) + tangent0 + tangent1) * t * t);

  return true;
}