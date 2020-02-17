/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __MODELGENERATOR_H__
#define __MODELGENERATOR_H__

#include <vector>

class Model;
class Vector3d;

/*

 * ModelGenerator - ModelGenerator is the class that centralizes the creation
 *   of the models used in this project.  Each method is responsible for
 *   generating the vertices, texture coordinates, and vertex normals for the
 *   entire model.

 */
class ModelGenerator {
public:
  static Model *GenCylinder(int numberOfVertices, double radius,
                            double segmentLength,
                            double percentCircumference, double startAngle,
                            bool reversePointOrder, bool closeEnd,
                            double texScaleX, double texScaleY,
                            double texStartOffsetX=0.0, double texStartOffsetY=0.0,
                            const char *name=0);
  static Model *GenFlutedColumn(double columnRadius, double columnHeight,
                                unsigned int numberOfFlutes,
                                double interfluteDistance,
                                unsigned int fluteVerticeCount,
                                double texScaleX, double texScaleY,
                                double texStartOffsetX=0.0, double texStartOffsetY=0.0,
                                const char *name=0);
  static Model *GenCurvedCylinder(double cylinderRadius, double curveRadius,
                                  unsigned int numberOfCylinderVertices,
                                  unsigned int numberOfCurveVertices,
                                  double percentCylinderCircumference,
                                  double percentCurveCircumference,
                                  double startCylinderAngle,
                                  double startCurveAngle,
                                  bool reversePointOrder,
                                  double texScaleX, double texScaleY, 
                                  double texStartOffsetX=0.0, double texStartOffsetY=0.0,
                                  const char *name=0);
  static Model *GenBox(double length, double width, double height,
                       const char *name=0);
  static Model *GenDisc(double radius, unsigned int numberOfVertices,
                        double percentCircumference, double startAngle,
                        double texScaleX, double texScaleY,
                        double texStartOffsetX=0.0, double texStartOffsetY=0.0,
                        bool closeEnd=false, const char *name=0);
  static Model *GenRecord(double innerRadius, double outerRadius,
                          unsigned int numberOfVertices,
                          double percentCircumference,
                          double startAngle, bool reversePoints,
                          double texScaleX, double texScaleY, 
                          double texStartOffsetX=0.0, double texStartOffsetY=0.0,
                          const char *name=0);
  static Model *GenPlaneWithHole(double width, double height,
                                 double radius, unsigned int numberOfVertices, 
                                 double texScaleX, double texScaleY,
                                 double texStartOffsetX=0.0, double texStartOffsetY=0.0,
                                 const char *name=0);
  static Model *GenPlane(double width, double height,
                         double texScaleX, double texScaleY, 
                         double texStartOffsetX=0.0, double texStartOffsetY=0.0,
                         const char *name=0);
  static Model *GenSpecificQuad(double x1, double y1, double z1,
                                double x2, double y2, double z2,
                                double x3, double y3, double z3,
                                double x4, double y4, double z4,
                                double texScaleX, double texScaleY, 
                                double texStartOffsetX=0.0, double texStartOffsetY=0.0,
                                const char *name=0);
  static Model *GenSphere(double radius, unsigned int numberOfVertices,
                          double texScaleX, double texScaleY,
                          double texStartOffsetX=0.0, double texStartOffsetY=0.0,
                          bool reversePointOrder=false,
                          const char *name=0);

  static void ComputeNormals(const std::vector<Vector3d> &vertices,
	                  		     const std::vector<unsigned int> &points,
			                       std::vector<Vector3d> &normals,
                             unsigned int polygonMeshType);

private:
  ModelGenerator(void);
  ModelGenerator(const ModelGenerator &);
  ~ModelGenerator(void);
  ModelGenerator &operator=(const ModelGenerator &);

  static Model *ComposeModel(const std::vector<Vector3d> &, const std::vector<unsigned int> &,
                             const std::vector<Vector3d> &,
							               const char *, unsigned int);
  static Model *ComposeModel(const std::vector<Vector3d> &, const std::vector<unsigned int> &,
                             const std::vector<Vector3d> &, const std::vector<Vector3d> &,
							               const char *, unsigned int);
};

#endif
