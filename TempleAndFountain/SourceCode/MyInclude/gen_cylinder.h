/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __GEN_CYLINDER_H__
#define __GEN_CYLINDER_H__

#include <vector>

class Vector3d;

extern bool GenerateCylinder(std::vector<Vector3d> &vertices,
                             std::vector<unsigned int> &points,
                             std::vector<Vector3d> &texCoords,
                             int numberOfVertices,
                             double radius,
                             double segmentLength,
                             double texScaleX,
                             double texScaleY,
                             double texStartOffsetX,
                             double texStartOffsetY,
                             double percentCircumference=1.0,
                             bool reversePointOrder=false,
                             bool closeEnd=true,
                             double startAngle=0.0);

extern double CalculateCylinderWidth(unsigned int numberOfVertices,
                                     double radius,
                                     double percentCircumference,
                                     bool closeEnd);

extern bool GenerateCurvedCylinder(std::vector<Vector3d> &vertices,
                                   std::vector<unsigned int> &points,
                                   std::vector<Vector3d> &texCoords,
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
                                   double texStartOffsetY);

// This function creates a triangle strip that describes a cylinder segment.
//   A cylinder segment only uses part of the vertice set for the top and
//   and bottom rings that define the cylinder.
// Assumes you are doing things correctly, otherwise should check that the 
//   vertices that you are going to look at are there.
extern void GenerateCylinderSegment(const std::vector<Vector3d> &vertices,
                                    std::vector<unsigned int> &points,
                                    int numberOfVertices,
                                    unsigned int currentIndex,
                                    unsigned int endIndex,
                                    bool reversePointOrder,
                                    bool closeEnd);

// This function takes a list of vertices and assumes vertices
//   0-numberOfVertices is the vertices for the ring on one side and the rest
//   form the ring on the other side of the cylinder segment.  A set of
//   triangle is then created put generating an ordered list of vertex
//   indices.
// Assumes you are doing things correctly, otherwise should check that the 
//   vertices that you are going to look at are there.
extern void GenerateCurvedCylinderSegment(const std::vector<Vector3d> &vertices,
                                          std::vector<unsigned int> &points,
                                          int numberOfVertices,
                                          unsigned int currentIndex,
                                          unsigned int endIndex,
                                          bool reversePointOrder,
                                          bool closeEnd);

#endif
