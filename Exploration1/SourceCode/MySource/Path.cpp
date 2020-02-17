/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * Path - Path defines a curve using control points.  Depending on the
 *   algorithm that computes locations along the curve, the curve may or
 *   may not pass through all of the control points.  In its current state,
 *   Path uses Catmull-Rom or Ferguson curves which do pass through all of
 *   their control points.  This class holds all of its control points and
 *   gives the user the ability to calculate their position along the curve.

 */

#include <cmath>

#include "path.h"

#include "Vector3d.h"
#include "Matrix.h"
#include "Curves.h"

using namespace std;

Path::Path(void) : d_curveType(CATMULL_ROM), d_loop(false) {
  return;
}

Path::Path(const Path &p) {
  vector<Vector3d>::const_iterator i;
  for(i=p.d_controlPoints.begin(); i!=p.d_controlPoints.end(); ++i) {
    d_controlPoints.push_back(*i);
  }
  d_curveType = p.d_curveType;
  d_loop = p.d_loop;

  return;
}

Path::~Path(void) {
  return;
}

// Add control point
void Path::AddPoint(double x, double y, double z) {
  AddPoint(Vector3d(x, y, z));

  return;
}

// Add control point
void Path::AddPoint(const Vector3d &v) {
  d_controlPoints.push_back(v);

  return;
}

// Scale the path, can specify the dimension of scaling
void Path::Scale(double s, bool x, bool y, bool z) {
  vector<Vector3d>::iterator i;
  for(i = d_controlPoints.begin(); i != d_controlPoints.end(); i++) {
    if(x) { (*i)[0] *= s; }
    if(y) { (*i)[1] *= s; }
    if(z) { (*i)[2] *= s; }
  }

  return;
}

// Determine if the path forms a closed loop or not
void Path::Loop(bool n) {
  d_loop = n;

  return;
}

bool Path::IsLooped(void) const {
  return d_loop;
}

// The number of control points
double Path::PathUnitLength(void) const {
  return (double)d_controlPoints.size();
}

// Calculates the new position along the curve and the tangent at that point
void Path::Curve(Vector3d &p, Vector3d &tangent, double pathPosition) const {
  // Loop the position based on the number of control points
  double position = fmod(pathPosition, (double)d_controlPoints.size());
  if(position < 0) {
    position = (double)d_controlPoints.size() + position;
  }

  // Determine which control point position falls within
  unsigned int segment = (unsigned int)position;

  // Find the proportion of the segment already covered.
  position -= (double)segment;

  if(d_curveType == CATMULL_ROM && d_loop) {
    // Loop curve
    unsigned int a = segment - 1;
    unsigned int b = segment;
    unsigned int c = (segment + 1) % d_controlPoints.size();
    unsigned int d = (segment + 2) % d_controlPoints.size();

    // If b == 0, then need to use the last control point
    if(b == 0) { a = (unsigned int)d_controlPoints.size() - 1; }

    // Calculate the new position and tangent
    Curves::CatmullRom(p, position, d_controlPoints[a], d_controlPoints[b],
                        d_controlPoints[c], d_controlPoints[d]);
    Curves::CatmullRomTangent(tangent, position, 
                               d_controlPoints[a], d_controlPoints[b],
                               d_controlPoints[c], d_controlPoints[d]);
  } else if(d_curveType == CATMULL_ROM) {
    // The curve does not loop, so check its bounds
    if(pathPosition < 0 || pathPosition >= (double)d_controlPoints.size()) {
      p = Vector3d(0.0, 0.0, 0.0);
      return;
    }

    unsigned int a = segment - 1;
    unsigned int b = segment;
    unsigned int c = segment + 1;
    unsigned int d = segment + 2;

    Vector3d v;
    if(b == 0) {
      v = Normalize(d_controlPoints[b] - d_controlPoints[c]) + d_controlPoints[b];
      Curves::CatmullRom(p, position,
                         v, d_controlPoints[b],
                         d_controlPoints[c], d_controlPoints[d]);
      Curves::CatmullRomTangent(tangent, position,
                                v, d_controlPoints[b],
                                d_controlPoints[c], d_controlPoints[d]);
    } else if(b == d_controlPoints.size() - 2) {
      v = Normalize(d_controlPoints[b] - d_controlPoints[c]) + d_controlPoints[c];
      Curves::CatmullRom(p, position,
                         d_controlPoints[a], d_controlPoints[b],
                         d_controlPoints[c], v);
      Curves::CatmullRomTangent(tangent, position, 
                                d_controlPoints[a], d_controlPoints[b],
                                d_controlPoints[c], v);
    } else if(b < d_controlPoints.size() - 2 && b > 0) {
      Curves::CatmullRom(p, position,
                         d_controlPoints[a], d_controlPoints[b],
                         d_controlPoints[c], d_controlPoints[d]);
      Curves::CatmullRomTangent(tangent, position, 
                                d_controlPoints[a], d_controlPoints[b],
                                d_controlPoints[c], d_controlPoints[d]);
    } else {
      p = Vector3d();
    }
  } else {
    // Guess I forgot to add Ferguson here
    p = Vector3d();
  }

  return;
}

// Calculates the new position along the curve at the given pathPosition
void Path::CurvePosition(Vector3d &p, double pathPosition) const {
  // Loop the position based on the number of control points
  double position = fmod(pathPosition, (double)d_controlPoints.size());
  if(position < 0) {
    position = (double)d_controlPoints.size() + position;
  }

  // Determine which control point position falls within
  unsigned int segment = (unsigned int)position;

  // Find the proportion of the segment already covered.
  position -= (double)segment;

  if(d_curveType == CATMULL_ROM && d_loop) {
    // Loop curve
    unsigned int a = segment - 1;
    unsigned int b = segment;
    unsigned int c = (segment + 1) % d_controlPoints.size();
    unsigned int d = (segment + 2) % d_controlPoints.size();

    // If b == 0, then need to use the last control point
    if(b == 0) { a = (unsigned int)d_controlPoints.size() - 1; }

    // Calculate the new position and tangent
    Curves::CatmullRom(p, position, d_controlPoints[a], d_controlPoints[b],
                       d_controlPoints[c], d_controlPoints[d]);
  } else if(d_curveType == CATMULL_ROM) {
    // The curve does not loop, so check its bounds
    if(pathPosition < 0 || pathPosition >= (double)d_controlPoints.size()) {
      p = Vector3d(0.0, 0.0, 0.0);
      return;
    }

    unsigned int a = segment - 1;
    unsigned int b = segment;
    unsigned int c = segment + 1;
    unsigned int d = segment + 2;

    Vector3d v;
    if(b == 0) {
      v = Normalize(d_controlPoints[b] - d_controlPoints[c]) + d_controlPoints[b];
      Curves::CatmullRom(p, position,
                         v, d_controlPoints[b],
                         d_controlPoints[c], d_controlPoints[d]);
    } else if(b == d_controlPoints.size() - 2) {
      v = Normalize(d_controlPoints[b] - d_controlPoints[c]) + d_controlPoints[c];
      Curves::CatmullRom(p, position,
                         d_controlPoints[a], d_controlPoints[b],
                         d_controlPoints[c], v);
    } else if(b < d_controlPoints.size() - 2 && b > 0) {
      Curves::CatmullRom(p, position,
                         d_controlPoints[a], d_controlPoints[b],
                         d_controlPoints[c], d_controlPoints[d]);
    } else {
      p = Vector3d();
    }
  } else {
    // Guess I forgot to add Ferguson here
    p = Vector3d();
  }

  return;
}

// Calculates the tangent at the given pathPosition
void Path::CurveTangent(Vector3d &p, double pathPosition) const {
  // Loop the position based on the number of control points
  double position = fmod(pathPosition, (double)d_controlPoints.size());
  if(position < 0) {
    position = (double)d_controlPoints.size() + position;
  }

  // Determine which control point position falls within
  unsigned int segment = (unsigned int)position;

  // Find the proportion of the segment already covered.
  position -= (double)segment;

  if(d_curveType == CATMULL_ROM && d_loop) {
    // Loop curve
    unsigned int a = segment - 1;
    unsigned int b = segment;
    unsigned int c = (segment + 1) % d_controlPoints.size();
    unsigned int d = (segment + 2) % d_controlPoints.size();

    // If b == 0, then need to use the last control point
    if(b == 0) { a = (unsigned int)d_controlPoints.size() - 1; }

    // Calculate the new position and tangent
    Curves::CatmullRomTangent(p, position, 
                              d_controlPoints[a], d_controlPoints[b],
                              d_controlPoints[c], d_controlPoints[d]);
  } else if(d_curveType == CATMULL_ROM) {
    // The curve does not loop, so check its bounds
    if(pathPosition < 0 || pathPosition >= (double)d_controlPoints.size()) {
      p = Vector3d(0.0, 0.0, 0.0);
      return;
    }

    unsigned int a = segment - 1;
    unsigned int b = segment;
    unsigned int c = segment + 1;
    unsigned int d = segment + 2;

    Vector3d v;
    if(b == 0) {
      v = Normalize(d_controlPoints[b] - d_controlPoints[c]) + d_controlPoints[b];
      Curves::CatmullRomTangent(p, position,
                                v, d_controlPoints[b],
                                d_controlPoints[c], d_controlPoints[d]);
    } else if(b == d_controlPoints.size() - 2) {
      v = Normalize(d_controlPoints[b] - d_controlPoints[c]) + d_controlPoints[c];
      Curves::CatmullRomTangent(p, position, 
                                d_controlPoints[a], d_controlPoints[b],
                                d_controlPoints[c], v);
    } else if(b < d_controlPoints.size() - 2 && b > 0) {
      Curves::CatmullRomTangent(p, position, 
                                d_controlPoints[a], d_controlPoints[b],
                                d_controlPoints[c], d_controlPoints[d]);
    } else {
      p = Vector3d();
    }
  } else {
    // Guess I forgot to add Ferguson here
    p = Vector3d();
  }

  return;
}
