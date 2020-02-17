/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __GEN_FLUTED_CYLINDER_H__
#define __GEN_FLUTED_CYLINDER_H__

#include <vector>

class Vector3d;

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
bool GenerateFlutedCylinder(std::vector<Vector3d> &vertices,
                            std::vector<unsigned int> &points,
                            double columnRadius,
                            double columnHeight,
                            unsigned int numberOfFlutes,
                            double interfluteDistance,
                            unsigned int fluteVerticeCount);

// Generates the texture coordinates for each of vertex
double GenerateFlutedCylinderTexCoords(const std::vector<Vector3d> &vertices,
                                       std::vector<Vector3d> &texCoords,
                                       double columnHeight,
                                       unsigned int fluteVerticeCount,
                                       double texScaleX,
                                       double texScaleY,
                                       double texStartOffsetX,
                                       double texStartOffsetY);

double CalculateFlutedCylinderCircumference(const std::vector<Vector3d> &vertices,
                                            unsigned int fluteVerticeCount);

#endif
