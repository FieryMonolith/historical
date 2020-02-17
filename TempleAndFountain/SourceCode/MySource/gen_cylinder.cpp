/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <sstream>

#include "Vector3d.h"

#include "gen_cylinder.h"

#include "gen_disc.h"

using namespace std;

bool GenerateCylinder(vector<Vector3d> &vertices,
                      vector<unsigned int> &points,
                      vector<Vector3d> &texCoords,
                      int numberOfVertices, double radius,
                      double segmentLength,
                      double texScaleX, double texScaleY,
                      double texStartOffsetX, double texStartOffsetY,
                      double percentCircumference,
                      bool reversePointOrder, bool closeEnd,
                      double startAngle) {
  if(numberOfVertices < 3 || radius < 0.0000001 || segmentLength < 0.000001) {
    return false;
  }

  GenerateRingEstimate(vertices, numberOfVertices, radius, 
		                   percentCircumference, startAngle, !closeEnd);

  // Generate Texture Coordinates for bottom ring
  //double width = CalculateCylinderWidth(numberOfVertices, radius, percentCircumference, closeEnd);
  Vector3d p0(vertices[0]);
  Vector3d p1(vertices[1]);
  double distance = Distance(p0, p1);

  double currentWidth = 0.0;
  for(unsigned int i=0; i<(unsigned int)vertices.size(); i++) {
    texCoords.push_back(Vector3d(texStartOffsetX + (currentWidth / texScaleX),
                                 texStartOffsetY, 0.0));
    currentWidth += distance;
  }

  double ty = texStartOffsetY + (segmentLength / texScaleY);
  for(int i=0; i<numberOfVertices; i++) {
    Vector3d t(vertices[i]);
    t[2] += segmentLength;
    vertices.push_back(t);

    Vector3d u(texCoords[i]);
    u[1] = ty;
    texCoords.push_back(u);
  }

  GenerateCylinderSegment(vertices, points, numberOfVertices,
                          0, numberOfVertices,
                          reversePointOrder, closeEnd);

  return true;
}

double CalculateCylinderWidth(unsigned int numberOfVertices, double radius,
                              double percentCircumference, bool closeEnd) {
  vector<Vector3d> vertices;
  GenerateRingEstimate(vertices, numberOfVertices, radius, 
		                   percentCircumference, 0.0, !closeEnd);
  unsigned int count = (unsigned int)vertices.size();
  if(!closeEnd) { count--; }
  double width = 0.0;

  for(unsigned int i=0; i<count; i++) {
    Vector3d p1 = vertices[i];
    Vector3d p2 = vertices[(i+1)%count];
    p1[2] = 0.0; p2[2] = 0.0;

    double distance = Distance(p1, p2);
    width += distance;
  }

  return width;
}

bool GenerateCurvedCylinder(vector<Vector3d> &vertices,
                            vector<unsigned int> &points,
                            vector<Vector3d> &texCoords,
                            double curveRadius,
                            unsigned int numberOfCurveVertices,
                            double percentCurveCircumference,
                            double startCurveAngle,
                            double cylinderRadius,
                            unsigned int numberOfCylinderVertices,
                            double percentCylinderCircumference,
                            double startCylinderAngle,
                            bool reversePointOrder,
                            double texScaleX,
                            double texScaleY,
                            double texStartOffsetX,
                            double texStartOffsetY)
{
  // Close the curve if the circumference is >= 100%
  bool closeCurve = false;
  if(percentCurveCircumference >= 1.0) { closeCurve = true; }

  // Close the cylinder if the circumference is >= 100%
  bool closeCylinder = false;
  if(percentCylinderCircumference >= 1.0) { closeCylinder = true; }

  vector<Vector3d> tempCylinder;
  GenerateRingEstimate(tempCylinder, numberOfCylinderVertices, cylinderRadius,
                       percentCylinderCircumference, 0.0, closeCylinder);
  Vector3d cyp0(tempCylinder[0]);
  Vector3d cyp1(tempCylinder[1]);
  double deltaWidth = Distance(cyp0, cyp1);

  vector<Vector3d> center;
  GenerateRingEstimate(center, numberOfCurveVertices, curveRadius,
                       percentCurveCircumference, startCurveAngle, closeCurve);
  Vector3d cup0(center[0]);
  Vector3d cup1(center[1]);
  double deltaHeight = Distance(cup0, cup1);

  // Generate data for curved cylinder
  Vector3d up(0.0, 0.0, 1.0);
  Vector3d normal;
  double currentHeight = 0.0;
  for(int i=-1; i < (int)numberOfCurveVertices - 1; i++) {
    unsigned int currentIndex = i*numberOfCylinderVertices;
    unsigned int endIndex = currentIndex + numberOfCylinderVertices;
    Vector3d out(center[i+1]);
    normal = Cross(up, out);

    GenerateOrientedRingEstimate(vertices, numberOfCylinderVertices, cylinderRadius,
                                 out,
                                 percentCylinderCircumference, startCylinderAngle,
                                 normal, out,
                                 !closeCylinder);

    double currentWidth = 0.0;
    double ty = texStartOffsetY + (currentHeight / texScaleY);
    for(unsigned int j=0; j<numberOfCylinderVertices; j++) {
      texCoords.push_back(Vector3d(texStartOffsetX + (currentWidth / texScaleX), ty, 0.0));
      currentWidth += deltaWidth;
    }
    currentHeight += deltaHeight;
    
    if(i < 0) {
      continue;
    }

    GenerateCurvedCylinderSegment(vertices, points, numberOfCylinderVertices,
                                  currentIndex, endIndex,
                                  reversePointOrder, closeCylinder);
  }

  return true;
}

// This function creates a triangle strip that describes a cylinder segment.
//   A cylinder segment only uses part of the vertice set for the top and
//   and bottom rings that define the cylinder.
// Assumes you are doing things correctly, otherwise should check that the 
//   vertices that you are going to look at are there.
void GenerateCylinderSegment(const vector<Vector3d> &/*vertices*/,
                             vector<unsigned int> &points,
                             int numberOfVertices,
                             unsigned int currentIndex,
                             unsigned int endIndex,
                             bool reversePointOrder,
                             bool closeEnd)
{
  // Create the triangle strip by creating an ordered list of vertex indices.
  for(unsigned int i=currentIndex; i<endIndex; i++) {
    if(reversePointOrder) {
      points.push_back(i+numberOfVertices);
      points.push_back(i);
    } else {
      points.push_back(i);
      points.push_back(i+numberOfVertices);
    }
  }

  // Connect the last and first vertices of both rings if the segment is
  //   supposed to be closed
  if(closeEnd) {
    if(reversePointOrder) {
      points.push_back(currentIndex+numberOfVertices);
      points.push_back(currentIndex);
    } else {
      points.push_back(currentIndex);
      points.push_back(currentIndex+numberOfVertices);
    }
  }

  return;
}

// This function takes a list of vertices and assumes vertices
//   0-numberOfVertices is the vertices for the ring on one side and the rest
//   form the ring on the other side of the cylinder segment.  A set of
//   triangle is then created put generating an ordered list of vertex
//   indices.
// Assumes you are doing things correctly, otherwise should check that the 
//   vertices that you are going to look at are there.
void GenerateCurvedCylinderSegment(const vector<Vector3d> &/*vertices*/,
                                   vector<unsigned int> &points,
                                   int numberOfVertices,
                                   unsigned int currentIndex,
                                   unsigned int endIndex,
                                   bool reversePointOrder,
                                   bool closeEnd)
{
  // All cylinders are broken down into pairs of triangles in order to
  //   ensure the same order of drawing vertices for each triangle.
  for(unsigned int i=currentIndex; i<endIndex-1; i++) {
    if(reversePointOrder) {
      points.push_back(i+numberOfVertices+1);
      points.push_back(i+numberOfVertices);
      points.push_back(i);

      points.push_back(i+1);
      points.push_back(i+numberOfVertices+1);
      points.push_back(i);
    } else {
      points.push_back(i);
      points.push_back(i+numberOfVertices);
      points.push_back(i+numberOfVertices+1);

      points.push_back(i);
      points.push_back(i+numberOfVertices+1);
      points.push_back(i+1);
    }
  }

  // If closeEnd, connect the first and last vertices of the top and bottom
  //   rings.
  if(closeEnd) {
    if(reversePointOrder) {
      points.push_back(currentIndex + numberOfVertices);
      points.push_back(endIndex - 1 + numberOfVertices);
      points.push_back(endIndex - 1);

      points.push_back(currentIndex);
      points.push_back(currentIndex + numberOfVertices);
      points.push_back(endIndex - 1);
    } else {
      points.push_back(endIndex - 1);
      points.push_back(endIndex - 1 + numberOfVertices);
      points.push_back(currentIndex + numberOfVertices);

      points.push_back(endIndex - 1);
      points.push_back(currentIndex + numberOfVertices);
      points.push_back(currentIndex);
    }
  }

  return;
}
