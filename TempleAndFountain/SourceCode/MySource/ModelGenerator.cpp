/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * ModelGenerator - ModelGenerator is the class that centralizes the creation
 *   of the models used in this project.  Each method is responsible for
 *   generating the vertices, texture coordinates, and vertex normals for the
 *   entire model.

 */

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <sstream>

#include "Vector3d.h"
#include "Model.h"

#include "ModelGenerator.h"

#include "loader_def.h"
#include "gen_cylinder.h"
#include "gen_fluted_cylinder.h"
#include "gen_disc.h"

using namespace std;

Model *ModelGenerator::GenCylinder(int numberOfVertices, double radius,
                                   double segmentLength,
                                   double percentCircumference, double startAngle,
                                   bool reversePointOrder, bool closeEnd,
                                   double texScaleX, double texScaleY,
                                   double texStartOffsetX, double texStartOffsetY,
                                   const char *name) {
  string sName;
  if(name == 0) {
    stringstream nameStream;
    nameStream << "cylinder-" << numberOfVertices << "-" << radius << "-";
    nameStream << segmentLength;
    sName = nameStream.str();
  } else {
    sName = name;
  }

  vector<Vector3d> texCoords;
  vector<Vector3d> vertices;
  vector<unsigned int> points;
  if(!GenerateCylinder(vertices, points, texCoords, numberOfVertices, radius, 
		                   segmentLength, texScaleX, texScaleY, 
                       texStartOffsetX, texStartOffsetY, percentCircumference,
                       reversePointOrder, closeEnd, startAngle)) {
    return 0;
  }
                   
  vector<Vector3d> normals;
  ComputeNormals(vertices, points, normals, GL_TRIANGLE_STRIP);

  // For wrapping texture
  if(percentCircumference == 1.0) {
    double width = CalculateCylinderWidth(numberOfVertices, radius, percentCircumference, closeEnd);
    double tx = texStartOffsetX + (width / texScaleX);

    vertices.push_back(vertices[0]);
    normals.push_back(normals[0]);
    texCoords.push_back(Vector3d(tx, texStartOffsetY, 0.0));

    vertices.push_back(vertices[numberOfVertices]);
    normals.push_back(normals[numberOfVertices]);
    texCoords.push_back(Vector3d(tx, texStartOffsetY + (segmentLength / texScaleY), 0.0));

    if(reversePointOrder) {
      points[points.size() - 1] = (unsigned int)(vertices.size() - 2);
      points[points.size() - 2] = (unsigned int)(vertices.size() - 1);
    } else {
      points[points.size() - 1] = (unsigned int)(vertices.size() - 1);
      points[points.size() - 2] = (unsigned int)(vertices.size() - 2);
    }
  }

  return ComposeModel(vertices, points, normals, texCoords, sName.c_str(), GL_TRIANGLE_STRIP);
}

Model *ModelGenerator::GenFlutedColumn(double columnRadius, double columnHeight,
                                       unsigned int numberOfFlutes,
                                       double interfluteDistance,
                                       unsigned int fluteVerticeCount,
                                       double texScaleX, double texScaleY,
                                       double texStartOffsetX, double texStartOffsetY,
                                       const char *name) {
  string sName;
  if(name == 0) {
    stringstream nameStream;
    nameStream << "fluted_cylinder-" << columnRadius << "-" << columnHeight << "-";
    nameStream << numberOfFlutes << "-" << interfluteDistance << "-";
    nameStream << fluteVerticeCount;
    sName = nameStream.str();
  } else {
    sName = name;
  }

  vector<Vector3d> vertices;
  vector<unsigned int> points;
  if(!GenerateFlutedCylinder(vertices, points, columnRadius, 
                  			     columnHeight, numberOfFlutes, 
			                       interfluteDistance, fluteVerticeCount)) {
    return 0;
  }

  vector<Vector3d> normals;
  ComputeNormals(vertices, points, normals, GL_TRIANGLE_STRIP);

  vector<Vector3d> texCoords;
  double width = GenerateFlutedCylinderTexCoords(vertices, texCoords, columnHeight, 
                                                 fluteVerticeCount, texScaleX, texScaleY,
                                                 texStartOffsetX, texStartOffsetY);

  // For wrapping texture
  double tx = texStartOffsetX + (width / texScaleX);

  vertices.push_back(vertices[fluteVerticeCount]);
  normals.push_back(normals[fluteVerticeCount]);
  texCoords.push_back(Vector3d(tx, texStartOffsetY + (columnHeight / texScaleY), 0.0));

  vertices.push_back(vertices[0]);
  normals.push_back(normals[0]);
  texCoords.push_back(Vector3d(tx, texStartOffsetY, 0.0));

  points[points.size() - 1] = (unsigned int)(vertices.size() - 1);
  points[points.size() - 2] = (unsigned int)(vertices.size() - 2);

  return ComposeModel(vertices, points, normals, texCoords, sName.c_str(), GL_TRIANGLE_STRIP);
}

Model *ModelGenerator::GenCurvedCylinder(double cylinderRadius, double curveRadius,
                                         unsigned int numberOfCylinderVertices,
                                         unsigned int numberOfCurveVertices,
                                         double percentCylinderCircumference,
                                         double percentCurveCircumference,
                                         double startCylinderAngle,
                                         double startCurveAngle,
                                         bool reversePointOrder,
                                         double texScaleX, double texScaleY,
                                         double texStartOffsetX, double texStartOffsetY,
                                         const char *name) {
  string sName;
  if(name == 0) {
    stringstream nameStream;
    nameStream << "curved_cylinder-" << numberOfCylinderVertices << "-";
    nameStream << cylinderRadius << "-" << numberOfCurveVertices << "-";
    nameStream << curveRadius;
    sName = nameStream.str();
  } else {
    sName = name;
  }


  vector<Vector3d> texCoords;
  vector<Vector3d> vertices;
  vector<unsigned int> points;
  if(!GenerateCurvedCylinder(vertices, points, texCoords, 
                             curveRadius, numberOfCurveVertices,
                             percentCurveCircumference, startCurveAngle,
                             cylinderRadius, numberOfCylinderVertices,
                             percentCylinderCircumference, startCylinderAngle,
                             reversePointOrder,
                             texScaleX, texScaleY,
                             texStartOffsetX, texStartOffsetY)) {
    return 0;
  }
#if 0
  // Ignoring the wrap for now, because the models I am using at the moment look fine
  double width = GenerateCylinderWidth(numberOfCylinderVertices, cylinderRadius,
                                       percentCylinderCircumference, closeCylinder);
  double height = GenerateCylinderWidth(numberOfCurveVertices, curveRadius,
                                        curvePercentCircumference, closeCurve);
#endif

  return ComposeModel(vertices, points, texCoords, sName.c_str(), GL_TRIANGLES);
}

Model *ModelGenerator::GenBox(double length, double width, double height,
                              const char *name) {
  string sName;
  if(name == 0) {
    stringstream nameStream;
    nameStream << "box-" << length << "-" << width << "-";
    nameStream << height;
    sName = nameStream.str();
  } else {
    sName = name;
  }

  vector<Vector3d> texCoords;
  vector<Vector3d> vertices;
  vector<unsigned int> points;

  vertices.push_back(Vector3d(0.0 - (length/2.0), 0.0 - (height/2.0),       (width/2.0)));
  vertices.push_back(Vector3d(      (length/2.0), 0.0 - (height/2.0),       (width/2.0)));
  vertices.push_back(Vector3d(0.0 - (length/2.0),       (height/2.0),       (width/2.0)));
  vertices.push_back(Vector3d(      (length/2.0),       (height/2.0),       (width/2.0)));
  vertices.push_back(Vector3d(0.0 - (length/2.0), 0.0 - (height/2.0), 0.0 - (width/2.0)));
  vertices.push_back(Vector3d(      (length/2.0), 0.0 - (height/2.0), 0.0 - (width/2.0)));
  vertices.push_back(Vector3d(0.0 - (length/2.0),       (height/2.0), 0.0 - (width/2.0)));
  vertices.push_back(Vector3d(      (length/2.0),       (height/2.0), 0.0 - (width/2.0)));
  
  points.push_back(0);
  points.push_back(1);
  points.push_back(2);
  points.push_back(3);
  points.push_back(7);
  points.push_back(1);
  points.push_back(5);
  points.push_back(0);
  points.push_back(4);
  points.push_back(2);
  points.push_back(6);
  points.push_back(7);
  points.push_back(4);
  points.push_back(5);

  return ComposeModel(vertices, points, texCoords, sName.c_str(), GL_TRIANGLE_STRIP);
}

Model *ModelGenerator::GenDisc(double radius, unsigned int numberOfVertices,
                               double percentCircumference, double startAngle,
                               double texScaleX, double texScaleY,
                               double texStartOffsetX, double texStartOffsetY,
                               bool closeEnd, const char *name) {
  string sName;
  if(name == 0) {
    stringstream nameStream;
    nameStream << "disc-" << numberOfVertices << "-" << radius;
    sName = nameStream.str();
  } else {
    sName = name;
  }

  vector<Vector3d> vertices;
  vector<unsigned int> points;
  if(!GenerateDisc(vertices, points, radius, numberOfVertices,
                   startAngle, percentCircumference, closeEnd)) {
    return 0;
  }

  // Generate texture coordinates
  vector<Vector3d> texCoords;
  //double size = radius * 2.0;
  double offset = radius;
  vector<Vector3d>::iterator it;
  for(it = vertices.begin(); it != vertices.end(); ++it) {
    const Vector3d &t = *it;

    //double x = (offset + t[0]) / size;
    //double y = (offset + t[1]) / size;
    double x = texStartOffsetX + ((offset + t[0]) / texScaleX);
    double y = texStartOffsetY + ((offset + t[1]) / texScaleY);
    texCoords.push_back(Vector3d(x, y, 0.0));
  }

  return ComposeModel(vertices, points, texCoords, sName.c_str(), GL_TRIANGLE_FAN);
}

Model *ModelGenerator::GenRecord(double innerRadius, double outerRadius,
                                 unsigned int numberOfVertices,
                                 double percentCircumference,
                                 double startAngle, bool reversePoints,
                                 double texScaleX, double texScaleY,
                                 double texStartOffsetX, double texStartOffsetY,
                                 const char *name) {
  string sName;
  if(name == 0) {
    stringstream nameStream;
    nameStream << "record-" << numberOfVertices << "-" << innerRadius << "-";
    nameStream << outerRadius;
    sName = nameStream.str();
  } else {
    sName = name;
  }

  vector<Vector3d> vertices;
  vector<unsigned int> points;
  if(!GenerateRecord(vertices, points, innerRadius, outerRadius, 
		   numberOfVertices, startAngle, percentCircumference, reversePoints)) {
    return 0;
  }

  // Generate texture coordinates
  vector<Vector3d> texCoords;
  //double size = outerRadius * 2.0;
  double offset = outerRadius;
  vector<Vector3d>::const_iterator it;
  for(it = vertices.begin(); it != vertices.end(); ++it) {
    const Vector3d &t = *it;

    //double x = (offset + t[0]) / size;
    //double y = (offset + t[1]) / size;
    double x = texStartOffsetX + ((offset + t[0]) / texScaleX);
    double y = texStartOffsetY + ((offset + t[1]) / texScaleY);
    texCoords.push_back(Vector3d(x, y, 0.0));
  }

  return ComposeModel(vertices, points, texCoords, sName.c_str(), GL_TRIANGLE_STRIP);
}

Model *ModelGenerator::GenPlaneWithHole(double width, double height,
                                        double radius, unsigned int numberOfVertices,
                                        double texScaleX, double texScaleY,
                                        double texStartOffsetX, double texStartOffsetY,
                                        const char *name) {
  if(numberOfVertices % 4 != 0 || width/2.0 < radius || height/2.0 < radius) {
    return 0;
  }

  string sName;
  if(name == 0) {
    stringstream nameStream;
    nameStream << "plane_with_hole-" << numberOfVertices << "-" << radius;
    sName = nameStream.str();
  } else {
    sName = name;
  }

  vector<Vector3d> vertices;
  vector<unsigned int> points;

  GenerateRingEstimate(vertices, numberOfVertices, radius, 1.0, 0.0, false);

  // Create inner box corners
  Vector3d upperRight(radius, radius, 0.0);
  vertices.push_back(upperRight);
  Vector3d upperLeft(-1.0*radius, radius, 0.0);
  vertices.push_back(upperLeft);
  Vector3d lowerLeft(-1.0*radius, -1.0*radius, 0.0);
  vertices.push_back(lowerLeft);
  Vector3d lowerRight(radius, -1.0*radius, 0.0);
  vertices.push_back(lowerRight);

  // Create triangles around hole by connecting each vertex in a quadrant to 
  // the corner for that quadrant.
  unsigned int sectionCount = (unsigned int)(numberOfVertices / 4.0);
  for(unsigned int i=0; i<4; i++) {
    unsigned int c = i + numberOfVertices;
    unsigned int d = sectionCount * i;
    unsigned int e = d + sectionCount;
    for(unsigned int j=d; j<e; j++) {
      points.push_back(c);
      points.push_back((j+1)%numberOfVertices);
      points.push_back(j);
    }
  }

  unsigned int prevStart = numberOfVertices;
  unsigned int prevVerts = 4;
  unsigned int prevQuarter = prevVerts / 4;
  unsigned int offset = prevStart + prevVerts;
  unsigned int inner = 4;
  unsigned int outer = 4 + (inner * 2);
  unsigned int quarter = outer / 4;
  double unit = width / (double)quarter;

  // Add outer
  for(unsigned int i=0; i<outer; i++) {
    double cw = 0.0;
    double ch = 0.0;

    if(i < quarter) {
      cw = (width / 2.0) - ((double)(i%quarter) * unit);
      ch = height / 2.0;
    } else if(i < quarter * 2) {
      cw = -1.0 * width / 2.0;
      ch = (height / 2.0) - ((double)(i%quarter) * unit);
    } else if(i < quarter * 3) {
      cw = (-1.0 * width / 2.0) + ((double)(i%quarter) * unit);
      ch = -1.0 * height / 2.0;
    } else {
      cw = width / 2.0;
      ch = (-1.0 * height / 2.0) + ((double)(i%quarter) * unit);
    }

    Vector3d t(cw, ch, 0.0);
    vertices.push_back(t);
  }

  // generator outer triangles
  for(unsigned int i=0; i<4; i++) {
    unsigned int toffset = offset + (i * quarter);
    unsigned int poffset = prevStart + (i * prevQuarter);

    points.push_back(toffset);
    points.push_back(toffset + 1);
    points.push_back(poffset);

    for(unsigned int j=1; j<quarter-1; j++) {
      points.push_back(toffset + j);
      if(i == 3) {
        points.push_back(prevStart + (((i+1) * prevQuarter)%inner));
      } else {
        points.push_back(poffset + j);
      }
      points.push_back(poffset + j - 1);

      points.push_back(toffset + j);
      points.push_back(toffset + j + 1);
      if(i == 3) {
        points.push_back(prevStart + (((i+1) * prevQuarter)%inner));
      } else {
        points.push_back(poffset + j);
      }
    }

    points.push_back(toffset + quarter - 1);
    if(i == 3) {
      points.push_back(offset + (((i+1) * quarter)%outer));
      points.push_back(prevStart);
    } else {
      points.push_back(toffset + quarter);
      points.push_back(poffset + prevQuarter);
    }
  }

  vector<Vector3d> texCoords;
  vector<Vector3d>::iterator iv;
  for(iv = vertices.begin(); iv != vertices.end(); ++iv) {
    Vector3d &t = *iv;
    //double x = ((width / 2.0) + t[0]) / width;
    //double y = ((height / 2.0) + t[1]) / height;
    double x = texStartOffsetX + (((width / 2.0) + t[0]) / texScaleX);
    double y = texStartOffsetY + (((height / 2.0) + t[1]) / texScaleY);
    Vector3d tc(x, y, 0.0);
    texCoords.push_back(tc);
  }

  return ComposeModel(vertices, points, texCoords, sName.c_str(), GL_TRIANGLES);
}

Model *ModelGenerator::GenPlane(double width, double height,
                                double texScaleX, double texScaleY,
                                double texStartOffsetX, double texStartOffsetY,
                                const char *name) {
  string sName;
  if(name == 0) {
    stringstream nameStream;
    nameStream << "plane-" << width << "-" << height;
    sName = nameStream.str();
  } else {
    sName = name;
  }

  vector<Vector3d> texCoords;
  vector<Vector3d> vertices;
  vector<unsigned int> points;

  Vector3d upperRight(width/2.0, height/2.0, 0.0);
  vertices.push_back(upperRight);
  Vector3d upperLeft(-1.0*width/2.0, height/2.0, 0.0);
  vertices.push_back(upperLeft);
  Vector3d lowerLeft(-1.0*width/2.0, -1.0*height/2.0, 0.0);
  vertices.push_back(lowerLeft);
  Vector3d lowerRight(width/2.0, -1.0*height/2.0, 0.0);
  vertices.push_back(lowerRight);

  double w = width / texScaleX;
  double h = height / texScaleY;
  Vector3d tur(texStartOffsetX + w, texStartOffsetY + h, 0.0);
  texCoords.push_back(tur);
  Vector3d tul(texStartOffsetX, texStartOffsetY + h, 0.0);
  texCoords.push_back(tul);
  Vector3d tll(texStartOffsetX, texStartOffsetY, 0.0);
  texCoords.push_back(tll);
  Vector3d tlr(texStartOffsetX + w, texStartOffsetY, 0.0);
  texCoords.push_back(tlr);

  points.push_back(0);
  points.push_back(1);
  points.push_back(3);
  points.push_back(2);

  return ComposeModel(vertices, points, texCoords, sName.c_str(), GL_TRIANGLE_STRIP);
}

Model *ModelGenerator::GenSpecificQuad(double x1, double y1, double z1,
                                       double x2, double y2, double z2,
                                       double x3, double y3, double z3,
                                       double x4, double y4, double z4,
                                       double texScaleX, double texScaleY,
                                       double texStartOffsetX, double texStartOffsetY,
                                       const char *name) {
  string sName;
  if(name == 0) {
    stringstream nameStream;
    nameStream << "specific_quad";
    sName = nameStream.str();
  } else {
    sName = name;
  }

  Vector3d line1(x2 - x1, y2 - y1, z2 - z1);
  Vector3d line2(x3 - x2, y3 - y2, z3 - z2);
  Vector3d line3(x4 - x3, y4 - y3, z4 - z3);
  Vector3d line4(x1 - x4, y1 - y4, z1 - z4);
  bool which1 = true;
  double d1 = Normalize(line1);
  double d2 = Normalize(line3);
  if(d1 < d2) { which1 = false; }
  bool which2 = true;
  double d3 = Normalize(line2);
  double d4 = Normalize(line4);
  if(d3 < d4) { which2 = false; }

  double urx=0.0, ury=0.0;
  double ulx=0.0, uly=0.0;
  double llx=0.0, lly=0.0;
  double lrx=0.0, lry=0.0;

  if(which1) {
    urx = texStartOffsetX + (d1 / texScaleX);
    ulx = texStartOffsetX;
    double tempDiff = ((d1 - d2) / 2.0) / texScaleX;
    llx = texStartOffsetX + tempDiff;
    lrx = texStartOffsetX + tempDiff + (d2 / texScaleX);
  } else {
    lrx = texStartOffsetX + (d2 / texScaleX);
    llx = texStartOffsetX;
    double tempDiff = ((d2 - d1) / 2.0) / texScaleX;
    ulx = texStartOffsetX + tempDiff;
    urx = texStartOffsetX + tempDiff + (d1 / texScaleX);
  }
  if(which2) {
    uly = texStartOffsetY + (d3 / texScaleY);
    lly = texStartOffsetY;
    double tempDiff = ((d3 - d4) / 2.0) / texScaleY;
    lry = texStartOffsetY + tempDiff;
    ury = texStartOffsetY + tempDiff + (d4 / texScaleY);
  } else {
    ury = texStartOffsetY + (d4 / texScaleY);
    lry = texStartOffsetY;
    double tempDiff = ((d4 - d3) / 2.0) / texScaleY;
    lly = texStartOffsetY + tempDiff;
    uly = texStartOffsetY + tempDiff + (d3 / texScaleY);
  }

  vector<Vector3d> texCoords;
  vector<Vector3d> vertices;
  vector<unsigned int> points;

  Vector3d q1(x1, y1, z1);
  vertices.push_back(q1);
  Vector3d q2(x2, y2, z2);
  vertices.push_back(q2);
  Vector3d q3(x3, y3, z3);
  vertices.push_back(q3);
  Vector3d q4(x4, y4, z4);
  vertices.push_back(q4);

  Vector3d tur(urx, ury, 0.0);
  texCoords.push_back(tur);
  Vector3d tul(ulx, uly, 0.0);
  texCoords.push_back(tul);
  Vector3d tll(llx, lly, 0.0);
  texCoords.push_back(tll);
  Vector3d tlr(lrx, lry, 0.0);
  texCoords.push_back(tlr);

  points.push_back(0);
  points.push_back(1);
  points.push_back(3);
  points.push_back(2);

  return ComposeModel(vertices, points, texCoords, sName.c_str(), GL_TRIANGLE_STRIP);
}

Model *ModelGenerator::GenSphere(double radius, unsigned int numberOfVertices,
                                 double texScaleX, double texScaleY,
                                 double texStartOffsetX, double texStartOffsetY,
                                 bool reversePointOrder,
                                 const char *name) {
  if(numberOfVertices < 4 || numberOfVertices % 4 != 0) {
    return 0;
  }

  string sName;
  if(name == 0) {
    stringstream nameStream;
    nameStream << "sphere-" << numberOfVertices << "-" << radius;
    sName = nameStream.str();
  } else {
    sName = name;
  }

  vector<Vector3d> vertices;
  vector<Vector3d> texCoords;
  vector<unsigned int> points;

  // center ring and top points, which are formed by the first 4 points
  unsigned int numberOfCircles = 1 + (numberOfVertices / 2);
  double diameter = 2.0 * radius;
  double ringDelta = diameter / (double)(numberOfCircles - 1);

  // Generate vertices of the sphere
  Vector3d top(0.0, 0.0, radius);
  vertices.push_back(top);
  double up = radius;
  double radiusSquared = radius * radius;
  for(unsigned int i=1; i<numberOfCircles; i++) {
    double currentRadius = sqrt(radiusSquared - (up * up));

    vector<Vector3d> tempVertices;
    GenerateRingEstimate(tempVertices, numberOfVertices, currentRadius, 1.0, 0.0, false);

    double distance = (radius - currentRadius) / 2.0;
    for(unsigned int j=0; j<numberOfVertices; j++) {
      Vector3d t(tempVertices[j]);
      t[2] = up;
      vertices.push_back(t);

      Vector3d p1(tempVertices[j]);
      Vector3d p2(tempVertices[(j+1)%numberOfVertices]);
      p1[2] = 0.0; p2[2] = 0.0;
      double d = Distance(p1, p2);
      distance += d;
      Vector3d t2(texStartOffsetX + (distance / texScaleX),
                  texStartOffsetY + ((radius + up) / texScaleY),
                  0.0);
      texCoords.push_back(t2);
    }

    up -= ringDelta;
  }
  Vector3d bottom(0.0, 0.0, -radius);
  vertices.push_back(bottom);

  // Generate triangles
  for(unsigned int i=0; i<numberOfVertices; i++) {
    if(reversePointOrder) {
      points.push_back(0);
      points.push_back(i + 2);
      points.push_back(i + 1);
    } else {
      points.push_back(0);
      points.push_back(i + 1);
      points.push_back(i + 2);
    }
  }

  unsigned int currentIndex = 1;;
  unsigned int endIndex = 1 + numberOfVertices;
  for(unsigned int j=1; j<numberOfCircles-1; j++) {
    for(unsigned int i=currentIndex; i<endIndex; i++) {
      unsigned int n1 = i + 1;
      if(n1 == endIndex) { n1 = endIndex - numberOfVertices; }
      unsigned int n2 = i + numberOfVertices + 1;
      if(n2 == endIndex + numberOfVertices) { n2 = endIndex; }

      if(reversePointOrder) {
        points.push_back(i+numberOfVertices);
        points.push_back(n2);
        points.push_back(i);

        points.push_back(n2);
        points.push_back(n1);
        points.push_back(i);
      } else {
        points.push_back(i);
        points.push_back(n2);
        points.push_back(i+numberOfVertices);

        points.push_back(i);
        points.push_back(n1);
        points.push_back(n2);
      }
    }

    currentIndex += numberOfVertices;
    endIndex += numberOfVertices;
  }

  unsigned int last = (unsigned int)vertices.size() - 1;
  for(unsigned int i=currentIndex; i<endIndex; i++) {
    unsigned int n1 = i + 1;
    if(n1 == endIndex) { n1 = endIndex - numberOfVertices; }

    if(reversePointOrder) {
      points.push_back(i);
      points.push_back(last);
      points.push_back(n1);
    } else {
      points.push_back(i);
      points.push_back(n1);
      points.push_back(last);
    }
  }

  return ComposeModel(vertices, points, texCoords, sName.c_str(), GL_TRIANGLES);
}

void ModelGenerator::ComputeNormals(const vector<Vector3d> &vertices, 
				    const vector<unsigned int> &points,
				    vector<Vector3d> &normals, unsigned int polygonMeshType) {
  if(points.size() < 3 || vertices.size() < 3) { return; }

  vector<Vector3d> polygonNormal;
  polygonNormal.resize(points.size()-2);

  vector<vector<unsigned int> > verticePolygonId;
  verticePolygonId.resize(vertices.size());

  Vector3d normal;
  unsigned int index1, index2, index3;
  index1 = points[0];
  index2 = points[1];

  unsigned int polygons = 0;
  for(unsigned int i=2; i<points.size(); i++) {
    index3 = points[i];
    if(polygonMeshType == GL_TRIANGLES && (i+1) % 3 != 0) {
      index1 = index2;
      index2 = index3;
      continue;
    }

    //calculate normal;
    if(polygonMeshType == GL_TRIANGLE_STRIP) {
      Vector3d v1(vertices[index2] - vertices[index1]);
      Vector3d v2(vertices[index3] - vertices[index1]);
      Normalize(v1);
      Normalize(v2);
      if(i%2) {
        normal = Cross(v1, v2);
      } else {
        normal = Cross(v2, v1);
      }
    } else {
      Vector3d v1(vertices[index2] - vertices[index1]);
      Vector3d v2(vertices[index3] - vertices[index1]);
      Normalize(v1);
      Normalize(v2);
      normal = Cross(v2, v1);
    }
    Normalize(normal);
    if(normal[0] < 0.0000000001 && normal[0] > -0.0000000001) {
      normal[0] = 0.0;
    }
    if(normal[1] < 0.0000000001 && normal[1] > -0.0000000001) {
      normal[1] = 0.0;
    }
    if(normal[2] < 0.0000000001 && normal[2] > -0.0000000001) {
      normal[2] = 0.0;
    }

    polygonNormal[polygons] = normal;
    verticePolygonId[index1].push_back(polygons);
    verticePolygonId[index2].push_back(polygons);
    verticePolygonId[index3].push_back(polygons);

    polygons++;
    if(polygonMeshType == GL_TRIANGLE_STRIP) {
      index1 = index2;
    }
    index2 = index3;
  }

  Vector3d temp;
  for(unsigned int i=0; i<vertices.size(); i++) {
    vector<unsigned int> &polyIds = verticePolygonId[i];
    vector<unsigned int>::iterator pii;
    for(pii = polyIds.begin(); pii != polyIds.end(); ++pii) {
      temp += polygonNormal[*pii];
    }
    if(polyIds.size() > 0) {
      temp /= polyIds.size();
    }
    Normalize(temp);

    normals.push_back(temp);
  }

  return;
}

Model *ModelGenerator::ComposeModel(const vector<Vector3d> &vertices, 
									                  const vector<unsigned int> &points,
                                    const vector<Vector3d> &texCoords,
									                  const char *filename, unsigned int type) {
  vector<Vector3d> normals;
  ComputeNormals(vertices, points, normals, type);
  return ComposeModel(vertices, points, normals, texCoords, filename, type);
}

Model *ModelGenerator::ComposeModel(const vector<Vector3d> &vertices, 
                  									const vector<unsigned int> &points,
                                    const vector<Vector3d> &normals,
                                    const vector<Vector3d> &texCoords,
                  									const char *filename, unsigned int type) {
  Model *m = new Model(filename, MODEL_MESH, type);
  if(m == 0) {
	  return 0;
  }

  Vector3d *data;
  for(unsigned int i = 0; i < vertices.size(); i++) {
    data = new Vector3d(vertices[i]);
    if(data == 0) {
      delete m;
      return 0;
    }

    m->AddVertex(i, data);
    data = 0;
  }

  for(unsigned int i = 0; i < texCoords.size(); i++) {
    data = new Vector3d(texCoords[i]);
    if(data == 0) {
      delete m;
      return 0;
    }

    m->AddTextureCoordinate(i, data);
    data = 0;
  }

  for(unsigned int i=0; i < normals.size(); i++) {
    data = new Vector3d(normals[i]);
    if(data == 0) {
      delete m;
      return 0;
    }

    m->AddNormal(i, data);
    data = 0;
  }

  for(unsigned int i=0; i != points.size(); i++) {
    m->AddPoint(points[i]);
  }
  
  return m;
}
