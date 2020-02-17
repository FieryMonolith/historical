/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * Three functions are declared here:
 *   -GenerateFlutedCylinder
 *   -GenerateFlutedCylinderTexCoords
 *   -CalculateFlutedCylinderCircumference

 * These three functions are used to create fluted columns, which are the
 *   stone columns that have half cylinders carved out of the sides at
 *   regular intervals.  These functions compute the vertices and tex vertices
 *   for a triangle strip that defines the shape.

 */

#define _USE_MATH_DEFINES
#include <cmath>

#include "Vector3d.h"

#include "gen_disc.h"
#include "gen_cylinder.h"
#include "gen_fluted_cylinder.h"

using namespace std;

// Creates the list of vertices that define the column shape
//   Output-
//     vertices - a list of unique vertices
//     points   - an ordered list of indices into vertices that defines the
//                triangle strip
//   Input-
//     columnRadius/columnHeight - defines its overarching properties
//     numberOfFlutes - A flute is the half-cylinder cut into the side of the
//                      column surface
//     interfluteDistance - distance along the surface between flutes
//     fluteVerticeCount - How many vertices should be used to define half
//                         circle of the flute
bool GenerateFlutedCylinder(vector<Vector3d> &vertices,
                            vector<unsigned int> &points,
                            double columnRadius,
                            double columnHeight,
                            unsigned int numberOfFlutes,
                            double interfluteDistance,
                            unsigned int fluteVerticeCount)
{
  double circumference = 2.0 * M_PI * columnRadius; // of column
  double fluteDistance = circumference / (double)numberOfFlutes;
  double fluteDiameter = (fluteDistance - interfluteDistance);
  double fluteRadius =  fluteDiameter/ 2.0;

  vector<Vector3d> endpoints;
  GenerateRingEstimate(endpoints, numberOfFlutes, columnRadius, 1.0, 0.0);

  unsigned int currentVertexCount = 0;

  for(unsigned int i=numberOfFlutes; i>0; i--) {
    Vector3d offset(endpoints[i%numberOfFlutes] - endpoints[i-1]);
    /*double mag =*/ Normalize(offset);

    double x = atan2(offset[1], offset[0]);
    if(x < 0.0) {
      x += (2.0 * M_PI);
    }

    vector<Vector3d> tempVertices;
    vector<unsigned int> tempPoints;
    vector<Vector3d> tempTexCoords;
    if(!GenerateCylinder(tempVertices, tempPoints, tempTexCoords, fluteVerticeCount, 
			 fluteRadius, columnHeight, 1.0, 1.0, 0.0, 0.0, 0.5, true, false, x)) {
      return false;
    }

    offset *= ((interfluteDistance / 2.0) + fluteRadius);
    offset += endpoints[i-1];

    vector<Vector3d>::iterator tvi;
    for(tvi=tempVertices.begin(); tvi!=tempVertices.end(); ++tvi) {
      Vector3d t(*tvi);
      t += offset;
      vertices.push_back(t);
    }

    vector<unsigned int>::iterator tpi;
    for(tpi=tempPoints.begin(); tpi!=tempPoints.end(); ++tpi) {
      points.push_back(*tpi + currentVertexCount);
    }

    currentVertexCount += (unsigned int)tempVertices.size();
  }

  points.push_back(fluteVerticeCount);
  points.push_back(0);

  return true;
}

//
double CalculateFlutedCylinderCircumference(const vector<Vector3d> &vertices,
                                            unsigned int fluteVerticeCount)
{
  double circumference = 0.0;

  for(unsigned int i=0; i<(unsigned int)vertices.size(); i++) {
    Vector3d p1(vertices[i]);
    if(p1[2] == 0) {
      unsigned int index = (i+1) % fluteVerticeCount;
      if(index == 0) {
        index = (i+1+fluteVerticeCount) % vertices.size();
      } else {
        index = i+1;
      }
      Vector3d p2(vertices[index]);
      p1[2] = 0.0; p2[2] = 0.0;

      double distance = Distance(p1, p2);
      circumference += distance;
    }
  }

  return circumference;
}

double GenerateFlutedCylinderTexCoords(const vector<Vector3d> &vertices,
                                       vector<Vector3d> &texCoords,
                                       double columnHeight, unsigned int fluteVerticeCount,
                                       double texScaleX, double texScaleY,
                                       double texStartOffsetX, double texStartOffsetY) {
  //double height = columnHeight;
  double width = CalculateFlutedCylinderCircumference(vertices, fluteVerticeCount);

  unsigned int which = 0;
  double currentWidth1 = 0.0;
  double currentWidth2 = 0.0;

  for(unsigned int i=0; i<vertices.size(); i++) {
    Vector3d p1(vertices[i]);
    if(p1[2] > 0.0) {
      which = 1;
    } else {
      which = 0;
    }
    Vector3d p2(vertices[(i+1)%vertices.size()]);
    if((p2[2] > 0 && which == 0) || (p2[2] == 0 && which == 1)) {
      p2 = vertices[(i+1+fluteVerticeCount)%vertices.size()];
    }
    p1[2] = 0.0; p2[2] = 0.0;
    double distance = Distance(p1, p2);

    if(which) {
      texCoords.push_back(Vector3d(texStartOffsetX + (currentWidth2 / texScaleX), 
                                   texStartOffsetY + (columnHeight / texScaleY),
                                   0.0));
      currentWidth2 += distance;
    } else {
      texCoords.push_back(Vector3d(texStartOffsetX + (currentWidth1 / texScaleY), 
                                   texStartOffsetY, 0.0));
      currentWidth1 += distance;
    }
  }

  return width;
}
