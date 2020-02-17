/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __PATH_H__
#define __PATH_H__

#include <vector>

class Vector3d;

/*

 * Path - Path defines a curve using control points.  Depending on the
 *   algorithm that computes locations along the curve, the curve may or
 *   may not pass through all of the control points.  In its current state,
 *   Path uses Catmull-Rom or Ferguson curves which do pass through all of
 *   their control points.  This class holds all of its control points and
 *   gives the user the ability to calculate their position along the curve.

 */
class Path {
public:
  Path(void);
  Path(const Path &);
  ~Path(void);

  // Add control point
  void AddPoint(const Vector3d &);
  void AddPoint(double x, double y, double z);

  // Scale the path, can specify the dimension of scaling
  void Scale(double s, bool x=true, bool y=true, bool z=true);

  // Determine if the path forms a closed loop or not
  void Loop(bool);
  bool IsLooped(void) const;

  // The number of control points
  double PathUnitLength(void) const;

  // Calculates the new position along the curve and the tangent at that point
  void Curve(Vector3d &p, Vector3d &tangent, double pathPosition) const;
  // Calculates the new position along the curve at the given pathPosition
  void CurvePosition(Vector3d &p, double pathPosition) const;
  // Calculates the tangent at the given pathPosition
  void CurveTangent(Vector3d &p, double pathPosition) const;

  const std::vector<Vector3d> &ControlPoints(void) const { return d_controlPoints; }

  // Curve types
  enum { FERGUSON, CATMULL_ROM };

private:
  std::vector<Vector3d> d_controlPoints;
  unsigned int d_curveType; // Ferguson or Catmull-Rom
  bool d_loop;
};

#endif
