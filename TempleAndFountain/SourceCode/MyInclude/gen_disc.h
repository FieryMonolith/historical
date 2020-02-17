/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __GEN_DISC_H__
#define __GEN_DISC_H__

#include <vector>

class Vector3d;

/*

 * Four functions are declared here:
 *   -GenerateDisc
 *   -GenerateRecord
 *   -GenerateRingEstimate
 *   -GenerateOrientedRingEstimate

 * The first two functions create surfaces.
 * The second two functions create a vertex list that describe either a full
 *   or partial circle of a given radius.

 */

// GenerateDisc creates a optionally-closed surface of defined by a circle
//   around the origin.  A closed disc implies that the first and last
//   vertices will be connected which can cause an abrupt edge to form.
extern bool GenerateDisc(std::vector<Vector3d> &vertices,
                         std::vector<unsigned int> &points,
                         double radius,
                         int numberOfVertices,
                         double startAngle,
                         double percentCircumference,
                         bool closeEnd);

// GenerateRecord determines the vertices that define a surface between two
//   circles that lie in the same plane.  This function utilizes the
//   GenerateRingEstimate function for both the inner and outer circles.
//   This function returns a list of unique vertices and (points) a list of
//   of indices into the vertice list that defines a triangle strip that 
//   characterizes the record. reversePoints determine the direction of a
//   triangle in the strip, ie. whether it is clockwise or counter-clockwise.
// Be aware that the record surface will always close, so if one only specifies
//   a partial circumference, the surface will have an abrupt edge.
extern bool GenerateRecord(std::vector<Vector3d> &vertices,
                           std::vector<unsigned int> &points,
                           double innerRadius,
                           double outerRadius,
                           int numberOfVertices,
                           double startAngle,
                           double percentCircumference,
                           bool reversePoints);

// GenerateRingEstimate returns the vertices that descibe either a full or
//   partial circle in the xy-plane.  The startAngle and percentCircumference,
//   and closeEnd are used to determine the arc of the circle the vertics will
//   fall on.  The startAngle is the angle from 0 radians to start the first
//   vertex.  The percentCircumference determines how much of the circle will
//   be described, and closeEnd will determine if the last vertex will close
//   the circle or leave it unconnected.
extern void GenerateRingEstimate(std::vector<Vector3d> &vertices,
                                 int numberOfVertices,
                                 double radius,
                                 double percentCircumference,
                                 double startAngle,
                                 bool closeEnd=false);

// GenerateOrientedRingEstimate is the same as GenerateRingEstimate, except
//   that the ring is oriented - lies within a plane perpendicular to the
//   given normal.  The startVector provides an arbitrary x-axis within the
//   plane.
extern void GenerateOrientedRingEstimate(std::vector<Vector3d> &vertices,
                                         int numberOfVertices,
                                         double radius,
                                         const Vector3d &offset,
                                         double percentCircumference,
                                         double startAngle,
                                         const Vector3d &normal,
                                         const Vector3d &startVector,
                                         bool closeEnd=false);

#endif
