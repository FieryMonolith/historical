/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __CURVES_H__
#define __CURVES_H__

class Vector3d;

/*

 * Curves - The Curves class holds the algorithms that determine the location
 *   along a curve, defined by four control points, given a proportial
 *   distance from the start of the curve.

 */
class Curves {
public:
  // Determine the location (out) from the start of the curve using a
  //   proportional distance (t).  The curve is defined by four control
  //   points.
  static bool CatmullRom(Vector3d &out,
                         double t,
                         const Vector3d &p0,
                         const Vector3d &p1,
                         const Vector3d &p2,
                         const Vector3d &p3);

  // Determine the tangent (out) at a position relative to the start of the
  //   curve using a proportional distance (t).  The curve is defined by four
  //   control points.
  static bool CatmullRomTangent(Vector3d &out,
                                double t,
                                const Vector3d &p0,
                                const Vector3d &p1,
                                const Vector3d &p2,
                                const Vector3d &p3);

  // Determine the location (out) from the start of the curve using a
  //   proportional distance (t).  The curve is defined by two control
  //   points and the tangent of the curve at those positions.
  static bool Ferguson(Vector3d &out,
                       double t,
                       const Vector3d &p0,
                       const Vector3d &p1,
                       const Vector3d &tangent0,
                       const Vector3d &tangent1);

  // Determine the tangent (out) at a position relative to the start of the
  //   curve using a proportional distance (t).  The curve is defined by two
  //   control points and the tangent of the curve at those positions.
  static bool FergusonTangent(Vector3d &out,
                              double t,
                              const Vector3d &p0,
                              const Vector3d &p1,
                              const Vector3d &tangent0,
                              const Vector3d &tangent1);

private:
  Curves(void);
  ~Curves(void);
  Curves(const Curves &);
};

#endif
