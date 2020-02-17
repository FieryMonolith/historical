/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __SNAKE_H__
#define __SNAKE_H__

class Vector3d;
class Matrix4x4d;
class Path;

/*

 * Snake - The Snake class represents an entire situation.  It encompasses
 *   a polygon mesh tube that moves along a predefined, arbitrarily shaped
 *   path, and two obstacles along that path.  The tube moves along the path
 *   and its mesh is dependent on its location along the path.  When it
 *   arrives at the two cylinders that stand opposite each other across the
 *   path form a narrow passage that forces the deformation of the normal
 *   tube shape by narrowing it.  However, everything is based on the tube,
 *   snake, itself, so that is why the constructor only handles tube
 *   properties.

 * The Snake is a tube whose cross-section is a circle.  That circle is a
 *   curve defined by control points.  In this situation, slice refers to
 *   the set of control points that define this cross-section circle.

 */
class Snake {
public:
  // The Snake is a tube, so define its length and radius.  Also the tube
  //   is subdivided into subsegments which are divided into a mesh defined
  //   by the other input properties.
  Snake(double length, double radius, unsigned int lengthUnits,
        unsigned int widthUnits, unsigned int lengthUnitDivisions,
        unsigned int widthUnitDivisions);
  ~Snake(void);

  // Move the Snake along its path over the given timestep
  void Update(double timestep);

  // Draw the Snake and obstacles
  void Draw(void);

private:
  double d_position; // Location along the path relative to the path origin
  Path *d_path;      // The control points for the path

  // The Snake is a tube whose cross-section is a circle.  That circle is a
  //   curve defined by control points.  In this situation, slice refers to
  //   the set of control points that define this cross-section circle.
  Path *d_slice;

  double d_bodyLength;
  double d_radius; // The Snake is a tube, so it has a radius

  unsigned int d_lengthUnits;
  unsigned int d_lengthUnitDivisions;
  unsigned int d_widthUnits;
  unsigned int d_widthUnitDivisions;

  // Define the two pillar properties and models
  Vector3d *d_pillar1, *d_pillar2; // Define the pillar collision volume
  double d_pillarRadius;
  double d_pillarHeight;
  GLUquadric *d_pillar; // Hold the quadric used to generate the pillar model
  GLUquadric *d_cap;    // Can be used to generate a model for a pillar cap

  // Create the control points for the path
  void CreatePath(void);
  // Create the control points for the slice
  void CreateSlice(void);
  // The Snake is subdivided into segments of polygon meshes.  This methods
  //   draws a single segment.
  void DrawSegment(const Vector3d &p0, const Vector3d &p1,
                   const Vector3d &tan0, const Vector3d &tan1,
                   double position, double nextPosition);
  // Takes two oriented slices and generates triangles by using the control
  //   points for the slices as vertices.
  void DrawStrip(const Path *p0, const Path *p1, const Vector3d &pOrigin,
                 const Vector3d &nOrigin);
  // AlterPoint takes a point and translates/rotates it into its new position
  //   defined by the offset and tangent.  The tangent is the normal to the
  //   plane the point, p, lies in.
  void AlterPoint(Vector3d &p, const Vector3d &offset,
                  const Vector3d &tangent);
  // Checks for collision of a point in a slice with a pillar's collision
  //   volume and adjusted accordingly.  Not sure how it is working for
  //   pillar2 since I don't seem to check for it.
  void Adjust(Vector3d &p, Vector3d &tangent, const Vector3d &origin);
  // Supposed to generate the cap on the Snake's two ends, but currently does
  //   nothing.
  void GenerateCap(const Vector3d *base, const Matrix4x4d &orientation,
                   const Matrix4x4d &offset) const;
};

#endif
