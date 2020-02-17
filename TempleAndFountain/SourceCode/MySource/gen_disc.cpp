/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * Four functions are defined here:
 *   -GenerateDisc
 *   -GenerateRecord
 *   -GenerateRingEstimate
 *   -GenerateOrientedRingEstimate

 * The first two functions create surfaces.
 * The second two functions create a vertex list that describe either a full
 *   or partial circle of a given radius.

 */


#define _USE_MATH_DEFINES
#include <cmath>

#include "Vector3d.h"

#include "gen_disc.h"

using namespace std;

// GenerateDisc creates a optionally-closed surface of defined by a circle
//   around the origin.  A closed disc implies that the first and last
//   vertices will be connected which can cause an abrupt edge to form.
bool GenerateDisc(vector<Vector3d> &vertices,
                  vector<unsigned int> &points,
                  double radius,
                  int numberOfVertices,
                  double startAngle,
                  double percentCircumference,
                  bool closeEnd)
{
  // Check if the circumference percentage requested is greater than 100%
  bool extra = false; // Yes, closeEnd for ring estimate
  if(percentCircumference < 1.0) {
    // No, don't closeEnd for ring estimate, already "visibly" closed
    extra = true;
  }

  // Add the origin of the disc as the first vertex
  Vector3d origin(0.0, 0.0, 0.0);
  vertices.push_back(origin);

  // Generate the vertices for the outer ring of the disc
  GenerateRingEstimate(vertices, numberOfVertices, radius, 
                       percentCircumference, startAngle, extra);

  // To generate a triangle stip, simply add all the vertices in order
  for(unsigned int i = 0; i < (unsigned int)numberOfVertices+1; i++) {
    points.push_back(i);
  }

  // To close the strip, add vertex 1, not 0 or 2.
  if(closeEnd) {
    points.push_back(1);
  }

  return true;
}

// GenerateRecord determines the vertices that define a surface between two
//   circles that lie in the same plane.  This function utilizes the
//   GenerateRingEstimate function for both the inner and outer circles.
//   This function returns a list of unique vertices and (points) a list of
//   of indices into the vertice list that defines a triangle strip that 
//   characterizes the record. reversePoints determine the direction of a
//   triangle in the strip, ie. whether it is clockwise or counter-clockwise.
// Be aware that the record surface will always close, so if one only specifies
//   a partial circumference, the surface will have an abrupt edge.
bool GenerateRecord(vector<Vector3d> &vertices,
                    vector<unsigned int> &points,
                    double innerRadius,
                    double outerRadius,
                    int numberOfVertices,
                    double startAngle,
                    double percentCircumference,
                    bool reversePoints)
{
  // Don't allow records with a radius less than one millimeter
  if(innerRadius < 0.001) { return false; }

  // Check if the circumference percentage requested is greater than 100%
  bool extra = false; // Yes, closeEnd for ring estimate
  if(percentCircumference < 1.0) {
    // No, don't closeEnd for ring estimate, already "visibly" closed
    extra = true;
  }

  GenerateRingEstimate(vertices, numberOfVertices, innerRadius, 
                       percentCircumference, startAngle, extra);
  GenerateRingEstimate(vertices, numberOfVertices, outerRadius, 
                       percentCircumference, startAngle, extra);

  // Create the triangle strip by simply creating a triangle between
  //   the last vertex of the previous triangle and the next vertex on the
  //   inner and outer circle.
  for(unsigned int i=0; i<(unsigned int)numberOfVertices; i++) {
    if(reversePoints) {
      points.push_back(i+numberOfVertices);
      points.push_back(i);
    } else {
      points.push_back(i);
      points.push_back(i+numberOfVertices);
    }
  }

  // Connect the points to close the surface.
  if(reversePoints) {
    points.push_back(numberOfVertices);
    points.push_back(0);
  } else {
    points.push_back(0);
    points.push_back(numberOfVertices);
  }
  return true;
}

// GenerateRingEstimate returns the vertices that descibe either a full or
//   partial circle in the xy-plane.  The startAngle and percentCircumference,
//   and closeEnd are used to determine the arc of the circle the vertics will
//   fall on.  The startAngle is the angle from 0 radians to start the first
//   vertex.  The percentCircumference determines how much of the circle will
//   be described, and closeEnd will determine if the last vertex will close
//   the circle or leave it unconnected.
void GenerateRingEstimate(vector<Vector3d> &vertices,
                          int numberOfVertices,
                          double radius,
                          double percentCircumference,
                          double startAngle,
                          bool closeEnd)
{
  // How much of the circle will be described
  double circumference = 2.0 * M_PI * percentCircumference;

  // If closeEnd is true the last vertex is the first vertex
  double arc = 0.0;
  if(closeEnd) {
	  arc = circumference / (double)(numberOfVertices-1);
  } else {
	  arc = circumference / (double)(numberOfVertices);
  }

  // The first vertex starts at startAngle and proceeds counterclockwise.
  double angle = startAngle;
  for(unsigned int count=0; count < (unsigned int)numberOfVertices; count++) {
    Vector3d location;

    // Vertex[count].X = r cos(theta);
    location[0] = radius * cos(angle);
    if(location[0] < 0.0000000001 && location[0] > -0.0000000001) {
      location[0] = 0.0;
    }

    // Vertex[count].Y = r sin(theta);
    location[1] = radius * sin(angle);
    if(location[1] < 0.0000000001 && location[1] > -0.0000000001) {
      location[1] = 0.0;
    }

    // The ring is in the xy-plane, so Z is set to zero.
    location[2] = 0.0;
    vertices.push_back(location);

    // The angle is incremented by the computed delta, arc.
    angle += arc;
  }

  return;
}

// GenerateOrientedRingEstimate is the same as GenerateRingEstimate, except
//   that the ring is oriented - lies within a plane perpendicular to the
//   given normal.  The startVector provides an arbitrary x-axis within the
//   plane.
void GenerateOrientedRingEstimate(std::vector<Vector3d> &vertices,
                                  int numberOfVertices,
                                  double radius,
                                  const Vector3d &offset,
                                  double percentCircumference,
                                  double startAngle,
                                  const Vector3d &normal,
                                  const Vector3d &startVector,
                                  bool closeEnd)
{
  Vector3d n(normal);
  Normalize(n);
  Vector3d v(startVector);
  Normalize(v);

  // How much of the circle will be described
  double circumference = 2.0 * M_PI * percentCircumference;

  // If closeEnd is true the last vertex is the first vertex
  double arc;
  if(closeEnd) {
	  arc = circumference / (double)(numberOfVertices-1);
  } else {
	  arc = circumference / (double)(numberOfVertices);
  }

  // The first vertex starts at startAngle and proceeds counterclockwise.
  double angle = startAngle;
  for(unsigned int count=0; count < (unsigned int)numberOfVertices; count++) {
    // Calculate the vertex as if it is the xy-plane
    Vector3d location(0.0, 0.0, 0.0);
    double costheta = radius*cos(angle);
    double sintheta = radius*sin(angle);

    // Transform the vertex position into the plane defined by the plane
    //   normal vector, n, and the start vector, v.
    location[0] += (costheta + (1 - costheta) * n[0] * n[0]) * v[0];
    location[0] += ((1 - costheta) * n[0] * n[1] - n[2] * sintheta) * v[1];
    location[0] += ((1 - costheta) * n[0] * n[2] + n[1] * sintheta) * v[2];
    if(location[0] < 0.0000000001 && location[0] > -0.0000000001) {
      location[0] = 0.0;
    }

    location[1] += ((1 - costheta) * n[0] * n[1] + n[2] * sintheta) * v[0];
    location[1] += (costheta + (1 - costheta) * n[1] * n[1]) * v[1];
    location[1] += ((1 - costheta) * n[1] * n[2] - n[0] * sintheta) * v[2];
    if(location[1] < 0.0000000001 && location[1] > -0.0000000001) {
      location[1] = 0.0;
    }

    location[2] += ((1 - costheta) * n[0] * n[2] - n[1] * sintheta) * v[0];
    location[2] += ((1 - costheta) * n[1] * n[2] + n[0] * sintheta) * v[1];
    location[2] += (costheta + (1 - costheta) * n[2] * n[2]) * v[2];
    if(location[2] < 0.0000000001 && location[2] > -0.0000000001) {
      location[2] = 0.0;
    }

    location += offset;
    vertices.push_back(location);
    
    angle += arc;
  }

  return;
}
