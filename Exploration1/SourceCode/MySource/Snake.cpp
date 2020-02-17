/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#include "stdafx.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <gl/gl.h>
#include <gl/glu.h>

#include "Vector3d.h"

#include "Snake.h"

#include "Path.h"
#include "Curves.h"
#include "Rotation.h"
#include "Matrix.h"

using namespace std;

// The Snake is a tube, so define its length and radius.  Also the tube
//   is subdivided into subsegments which are divided into a mesh defined
//   by the other input properties.
Snake::Snake(double length,
             double radius,
             unsigned int lengthUnits,
             unsigned int widthUnits,
             unsigned int lengthUnitDivisions,
             unsigned int widthUnitDivisions)
: d_position(0.0),
  d_path(0),
  d_slice(0),
  d_bodyLength(length),
  d_radius(radius),
  d_lengthUnits(lengthUnits),
  d_lengthUnitDivisions(lengthUnitDivisions),
  d_widthUnits(widthUnits),
  d_widthUnitDivisions(widthUnitDivisions),
  d_pillar1(0),
  d_pillar2(0),
  d_pillarRadius(15.0),
  d_pillarHeight(20.0),
  d_pillar(0),
  d_cap(0)
{
  if(length < 1.0 || radius < 0.5 || lengthUnits < 1 ||
    lengthUnitDivisions < 1 || lengthUnits < 1 || widthUnits < 1 ||
    widthUnitDivisions < 1)
  {
    assert(0);
  }

  // Create the two pillar models by using glu quadrics
  d_cap = gluNewQuadric();
  assert(d_cap);
  d_pillar = gluNewQuadric();
  assert(d_pillar);
  d_pillar1 = new Vector3d(65.0, 0.0, 19.5);
  assert(d_pillar1);
  d_pillar2 = new Vector3d(65.0, 0.0, 28.5);
  assert(d_pillar2);

  *d_pillar1 *= 5.0;
  *d_pillar2 *= 5.0;

  CreatePath();
  CreateSlice();

  return;
}

Snake::~Snake(void) {
  if(d_path != 0) {
    delete d_path;
    d_path = 0;
  }
  if(d_slice != 0) {
    delete d_slice;
    d_slice = 0;
  }
  if(d_pillar != 0) {
    gluDeleteQuadric(d_pillar);
  }
  if(d_pillar1 != 0) {
    delete d_pillar1;
  }
  if(d_pillar2 != 0) {
    delete d_pillar2;
  }
  if(d_cap != 0) {
    gluDeleteQuadric(d_cap);
  }

  return;
}

// Draw Snake and Pillars
void Snake::Draw(void) {
  glLineWidth(1.0);
  glColor3f(1.0, 0.0, 0.0);

  //Draw cylinders (pillars)
  Vector3d up(0.0, 1.0, 0.0);
  Matrix4x4d temp = Transpose(RotationFromZ(up));

  float materialColor[4];
  materialColor[0] = 0.65f;
  materialColor[1] = 0.65f;
  materialColor[2] = 0.65f;
  materialColor[3] = 1.0f;
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
  glPushMatrix();
  glTranslated(d_pillar1->at(0), d_pillar1->at(1), d_pillar1->at(2));
  glPushMatrix();
  glMultMatrixd(temp.Array());
  gluCylinder(d_pillar, d_pillarRadius, d_pillarRadius, d_pillarHeight, 32, 1);
  glPopMatrix();
  glTranslated(0.0, d_pillarHeight, 0.0);
  glMultMatrixd(temp.Array());
  gluDisk(d_cap, 0.0, d_pillarRadius, 32, 1);
  glPopMatrix();

  glPushMatrix();
  glTranslated(d_pillar2->at(0), d_pillar2->at(1), d_pillar2->at(2));
  glPushMatrix();
  glMultMatrixd(temp.Array());
  gluCylinder(d_pillar, d_pillarRadius, d_pillarRadius, d_pillarHeight, 32, 1);
  glPopMatrix();
  glTranslated(0.0, d_pillarHeight, 0.0);
  glMultMatrixd(temp.Array());
  gluDisk(d_cap, 0.0, d_pillarRadius, 32, 1);
  glPopMatrix();

  materialColor[0] = 0.0f;
  materialColor[1] = 0.2f;
  materialColor[2] = 1.0f;
  materialColor[3] = 1.0f;
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);

  // Draw Snake
  Vector3d current;
  Vector3d next;
  Vector3d currentTangent;
  Vector3d nextTangent;
  double currentPosition = d_position;
  double nextPosition = d_position;
  for(unsigned int i=0; i<d_lengthUnits+1; i++) {
    // Compute the location of the segment along the control path
    d_path->Curve(next, nextTangent, nextPosition);

    // Draw the mesh of one of the current Snake segment
    if(i > 0) {
      DrawSegment(current, next, currentTangent, nextTangent,
                  currentPosition-d_position, nextPosition-d_position);
    }

    // Move to the next segment for drawing
    current = next;
    currentTangent = nextTangent;
    currentPosition = nextPosition;
    nextPosition = fmod(nextPosition + 1.0, d_path->PathUnitLength());
  }

#if 0
  // Draw path
  Vector3d x;
  glBegin(GL_LINE_LOOP);
  for(double tt=0.0; tt<d_path->PathUnitLength(); tt+=0.01) {
    d_path->CurvePosition(x, tt);
    glVertex3dv(x.Array());
  }
  glEnd();
#endif

  return;
}

// Move the Snake along its path over the given timestep
void Snake::Update(double timestep) {
  timestep *= 10.0;
  d_position += timestep;
  d_position = fmod(d_position, d_path->PathUnitLength());

  return;
}

// The Snake is subdivided into segments of polygon meshes.  This methods draws
//   a single segment.
void Snake::DrawSegment(const Vector3d &p0, const Vector3d &p1,
                        const Vector3d &tan0, const Vector3d &tan1,
                        double position, double nextPosition) {
  if(p0 == p1) {
    return;
  }

  // path1 is the first circle in the set of circles that define this segment.
  // Create a path equivalent to slice...
  // This will be the control points for P(0)
  Path *path1 = new Path;
  assert(path1);
  path1->Loop(d_slice->IsLooped());

  // Orient path1 based on its location along the path of motion and the
  //   tangent at that point.  The tangent determines the orientation of the
  //   new slice, path1.
  const vector<Vector3d> &cp0 = d_slice->ControlPoints();
  vector<Vector3d>::const_iterator it;
  for(it = cp0.begin(); it != cp0.end(); ++it) {
    Vector3d t(*it);
    AlterPoint(t, p0, tan0);
    path1->AddPoint(t);
  }

  // Now create an oriented slice, called path2, at position p1
  // Store the control points for P(1) which are known
  Path *path2 = new Path;
  assert(path2);
  path2->Loop(d_slice->IsLooped());

  const vector<Vector3d> &cp1 = d_slice->ControlPoints();
  for(it = cp1.begin(); it != cp1.end(); ++it) {
    Vector3d t(*it);
    AlterPoint(t, p1, tan1);
    path2->AddPoint(t);
  }

  // This subdivides the segment into n slices, where n is the number of
  //   lengthUnitDivisions+1.  Each slice's control points are generated using
  //   the Ferguson curve method, essentially extrapolating the control point
  //   along the motion curve between the control points in path1 and path2.
  //   Then each slice, with along with the next slice, generate a polygon mesh
  //   of triangles between the two slices.  By iterating through each slice
  //   a polygon mesh of triangles is generated for the entire segment.
  Path *pathc = path1;
  Path *pathn = 0;

  double pdelta = 0.0;

  double deltaUnit = 1.0 / (double)(d_lengthUnitDivisions + 1);
  double delta = deltaUnit;
  for(unsigned int i=1; i<=d_lengthUnitDivisions+1; i++, delta += deltaUnit) {
    if(i == d_lengthUnitDivisions + 1) {
      pathn = path2;
    } else {
      pathn = new Path;
      pathn->Loop(d_slice->IsLooped());
      assert(pathn);

      // Create control points for temporary slice
      const vector<Vector3d> &pcp1 = path1->ControlPoints();
      const vector<Vector3d> &pcp2 = path2->ControlPoints();
      vector<Vector3d>::const_iterator it = pcp1.begin();
      vector<Vector3d>::const_iterator iit = pcp2.begin();
      for(; it != pcp1.end() && iit != pcp2.end(); it++, iit++) {
        Vector3d cp1(*it);
        Vector3d cp2(*iit);

        Vector3d t;
        Curves::Ferguson(t, delta, cp1, cp2, tan0, tan1);
        pathn->AddPoint(t);
      }
    }

    // Compute the origin of the slice path for each path in the same
    //   manner as the control points for the slice itself
    Vector3d norigin;
    Curves::Ferguson(norigin, delta, p0, p1, tan0, tan1);
    Vector3d porigin;
    Curves::Ferguson(porigin, pdelta, p0, p1, tan0, tan1);

    // Draw the mesh between the two slices
    DrawStrip(pathc, pathn, porigin, norigin);

    if(i > 1) {
      delete pathc;
    }
    pdelta = delta;
    pathc = pathn;
    pathn = 0;
  }

  delete path1;
  delete path2;

  return;
}

// Takes two oriented slices and generates triangles by using the control
//   points for the slices as vertices.
void Snake::DrawStrip(const Path *p0, const Path *p1,
                      const Vector3d &pOrigin, const Vector3d &nOrigin) {
  const vector<Vector3d> &cp0 = p0->ControlPoints();
  const vector<Vector3d> &cp1 = p1->ControlPoints();

  if(cp0.size() != cp1.size()) {
    return;
  }

  Vector3d p00, p01, p10, p11;
  double size = p0->PathUnitLength();
  double delta = 1.0 / (double)(d_widthUnitDivisions + 1);

  glBegin(GL_TRIANGLES);
  for(double i=0; i<size; i+=delta) {
    p0->CurvePosition(p00, i);
    p0->CurvePosition(p01, i+delta);
    p1->CurvePosition(p10, i);
    p1->CurvePosition(p11, i+delta);

    Vector3d tangent;
    Adjust(p00, tangent, pOrigin);
    Adjust(p01, tangent, pOrigin);
    Adjust(p10, tangent, nOrigin);
    Adjust(p11, tangent, nOrigin);

    glVertex3dv(p00.Array());
    glVertex3dv(p11.Array());
    glVertex3dv(p10.Array());

    glVertex3dv(p00.Array());
    glVertex3dv(p01.Array());
    glVertex3dv(p11.Array());
  }
  glEnd();

  return;
}

// Checks for collision of a point in a slice with a pillar's collision
//   volume and adjusted accordingly.  Not sure how it is working for pillar2
//   since I don't seem to check for it.
void Snake::Adjust(Vector3d &p, Vector3d &tangent, const Vector3d &origin) {
  // Add buffer to the pillar to ensure visibility of collision
  double radius = d_pillarRadius + 5.0;

  // Project each value onto the xz-plane
  // Pillar's origin
  Vector3d pillar(*d_pillar1);
  pillar[1] = 0.0;
  // The point to be adjusted
  Vector3d pos(p);
  pos[1] = 0.0;
  // The origin of the slice the contains the point to be adjusted
  Vector3d o(origin);
  o[1] = 0.0;

  // Determine the vectors from the origin to pillar and origin to point
  Vector3d vco(pillar - o);
  Vector3d vpo(pos - o);
  double doc = Normalize(vco);
  double dop = Normalize(vpo);
  if(doc - dop >= radius) {
    // No collision, do nothing
    return;
  }

  // Collision has occurred.
  // Adjust the position of the given point

  double theta = asin(radius / doc);
  if(theta < 0) { theta *= -1.0; }
  double phi = Inner(vco, vpo);
  if(phi < 0) { phi *= -1.0; }
  if(phi >= theta) {
    return;
  }

  double a = doc * sin(phi);
  double u = sqrt((a*a) + (doc*doc));
  double v = sqrt((a*a) + (radius * radius));
  double alpha = u - v;

  Vector3d np(p - o);
  np[1] = 0.0;
  Normalize(np);
  pos = o + alpha * np;
  double run = sqrt((np[0] * np[0]) + (np[2] * np[2]));
  np = Vector3d(run, p[1]-origin[1], 0.0);
  Normalize(np);
  double rise = (alpha / run * np[1]);
  pos[1] = origin[1] + rise;
  double length = sqrt((rise * rise) + (alpha * alpha));
  dop = Distance(p, origin);
  if(pos[1] < 0 || pos[1] > d_pillarHeight || length >= dop) {
    return;
  }

  p = pos;

  return;
}

// AlterPoint takes a point and translates/rotates it into its new position
//   defined by the offset and tangent.  The tangent is the normal to the
//   plane the point, p, lies in.
void Snake::AlterPoint(Vector3d &p, const Vector3d &offset,
                       const Vector3d &tangent)
{
  Vector3d t(tangent);
  Normalize(t);

  Matrix4x4d orientation = RotationFromZ(t);
  Orthonormalize(orientation);

  Matrix4x4d o(1.0, 0.0, 0.0, offset[0],
               0.0, 1.0, 0.0, offset[1],
               0.0, 0.0, 1.0, offset[2],
               0.0, 0.0, 0.0, 1.0);
  Matrix4x4d loc(1.0, 0.0, 0.0, p[0],
                 0.0, 1.0, 0.0, p[1],
                 0.0, 0.0, 1.0, p[2],
                 0.0, 0.0, 0.0, 1.0);

  Matrix4x4d newLoc = o * orientation * loc;
  p[0] = newLoc.At(0,3);
  p[1] = newLoc.At(1,3);
  p[2] = newLoc.At(2,3);

  return;
}

// Supposed to generate the cap on the Snake's two ends, but currently does
//   nothing.
void Snake::GenerateCap(const Vector3d *base, const Matrix4x4d &orientation,
                        const Matrix4x4d &offset) const {
#if 0
  assert(base);

  Vector3d *p = new Vector3d[d_bodyDivisions];
  assert(p);

  Matrix4x4d inv(offset);
  inv.At(0,3) *= -1.0;
  inv.At(1,3) *= -1.0;
  inv.At(2,3) *= -1.0;
  for(unsigned int i=0; i<d_bodyDivisions; i++) {
    Matrix4x4d t(1.0, 0.0, 0.0, base[i][0],
                 0.0, 1.0, 0.0, base[i][1],
                 0.0, 0.0, 1.0, base[i][2],
                 0.0, 0.0, 0.0, 1.0);
    Matrix4x4d tt = t * Transpose(orientation) * inv;
    p[i][0] = tt.At(0,3);
    p[i][1] = tt.At(1,3);
    p[i][2] = tt.At(2,3);
  }

  unsigned int which = 0;
  Vector3d *rings = new Vector3d[d_bodyDivisions*2];
  for(unsigned int i=0; i<d_bodyDivisions; i++) {
    rings[(which * d_bodyDivisions) + i] = base[i];
  }
  which = (which + 1) % 2;

  for(unsigned int i=0; i<3; i++) {
    for(unsigned int j=0; j<d_bodyDivisions; j++) {
      // Gradually smaller rings
      Vector3d x(p[i] * ((5-i-1)/5));
      Matrix4x4d loc(1.0, 0.0, 0.0, x[0],
                     0.0, 1.0, 0.0, x[1],
                     0.0, 0.0, 1.0, x[2],
                     0.0, 0.0, 0.0, 1.0);
      Matrix4x4d newLoc = offset * orientation * loc;
      rings[(which * d_bodyDivisions) + i][0] = newLoc.At(0,3);
      rings[(which * d_bodyDivisions) + i][1] = newLoc.At(1,3);
      rings[(which * d_bodyDivisions) + i][2] = newLoc.At(2,3) + ((double)(i+1) * (d_radius)/4.0);
    }

    unsigned int curWhich = which;
    unsigned int prevWhich = (which + 1) % 2;
    glBegin(GL_TRIANGLES);
    for(unsigned int j=0; j<d_bodyDivisions; j++) {
      glVertex3dv(rings[(prevWhich * d_bodyDivisions) + j].Array());
      glVertex3dv(rings[(curWhich * d_bodyDivisions) + j].Array());
      glVertex3dv(rings[(curWhich * d_bodyDivisions) + ((j+1)%d_bodyDivisions)].Array());

      glVertex3dv(rings[(prevWhich * d_bodyDivisions) + j].Array());
      glVertex3dv(rings[(curWhich * d_bodyDivisions) + ((j+1)%d_bodyDivisions)].Array());
      glVertex3dv(rings[(prevWhich * d_bodyDivisions) + ((j+1)%d_bodyDivisions)].Array());
    }
    glEnd();


    if(i == 2) {
      Matrix4x4d loc(1.0, 0.0, 0.0, 0.0,
                     0.0, 1.0, 0.0, 0.0,
                     0.0, 0.0, 1.0, ((double)(i+2) * (d_radius)/4.0),
                     0.0, 0.0, 0.0, 1.0);
      Matrix4x4d newLoc = offset * orientation * loc;
      Vector3d x;
      x[0] = newLoc.At(0,3);
      x[1] = newLoc.At(1,3);
      x[2] = newLoc.At(2,3);

      glBegin(GL_TRIANGLES);
      for(unsigned int j=0; j<d_bodyDivisions; j++) {
        glVertex3dv(rings[(curWhich * d_bodyDivisions) + j].Array());
        glVertex3dv(rings[(curWhich * d_bodyDivisions) + ((j+1)%d_bodyDivisions)].Array());
        glVertex3dv(x.Array());
      }
      glEnd();
    }

    which = (which + 1) % 2;
  }

  delete [] p;
  delete [] rings;
#endif

  return;
}

// Create the control points for the path
void Snake::CreatePath(void) {
  d_path = new Path;
  assert(d_path);

  d_path->AddPoint(14.0, 5.0, 10.0);
  d_path->AddPoint(16.0, 5.0, 7.5);
  d_path->AddPoint(20.0, 5.0, 5.5);
  d_path->AddPoint(25.0, 5.0, 6.5);
  d_path->AddPoint(31.0, 5.0, 9.5);
  d_path->AddPoint(35.0, 5.0, 10.0);
  d_path->AddPoint(40.0, 5.0, 10.0);
  d_path->AddPoint(45.0, 5.0, 10.0);
  d_path->AddPoint(50.0, 5.0, 10.0);
  d_path->AddPoint(60.0, 5.0, 10.0);
  d_path->AddPoint(75.0, 5.0, 10.0);
  d_path->AddPoint(80.0, 5.0, 11.0);
  d_path->AddPoint(85.0, 5.0, 12.0);

  d_path->AddPoint(88.0, 5.0, 13.0);
  d_path->AddPoint(90.0, 5.0, 17.0);
  d_path->AddPoint(88.0, 5.0, 21.0);

  d_path->AddPoint(85.0, 5.0, 22.0);
  d_path->AddPoint(80.0, 5.0, 23.0);

  d_path->AddPoint(75.0, 5.0, 24.0);
  d_path->AddPoint(65.0, 5.0, 24.0);
  d_path->AddPoint(55.0, 5.0, 24.0);
  d_path->AddPoint(50.0, 5.0, 25.0);
  d_path->AddPoint(45.5, 5.0, 26.5);
  d_path->AddPoint(44.0, 5.0, 30.0);
  d_path->AddPoint(45.0, 5.0, 35.0);
  d_path->AddPoint(47.5, 5.0, 37.5);
  d_path->AddPoint(50.0, 5.0, 40.0);
  d_path->AddPoint(52.0, 5.0, 45.0);
  d_path->AddPoint(51.5, 5.0, 50.0);
  d_path->AddPoint(49.0, 5.0, 53.0);
  d_path->AddPoint(45.0, 5.0, 55.5);
  d_path->AddPoint(35.0, 5.0, 58.0);
  d_path->AddPoint(30.0, 5.0, 57.0);
  d_path->AddPoint(26.0, 5.0, 56.0);
  d_path->AddPoint(20.0, 5.0, 55.0);
  d_path->AddPoint(15.0, 5.0, 50.0);
  d_path->AddPoint(9.0, 5.0, 45.0);
  d_path->AddPoint(6.0, 5.0, 40.0);
  d_path->AddPoint(5.0, 5.0, 35.0);
  d_path->AddPoint(10.0, 5.0, 28.5);
  d_path->AddPoint(12.5, 5.0, 25.0);
  d_path->AddPoint(14.0, 5.0, 23.0);
  d_path->AddPoint(13.0, 5.0, 20.0);
  d_path->AddPoint(12.5, 5.0, 15.0);

  d_path->Scale(5.0, true, false, true);
  d_path->Loop(true);

  return;
}

void Snake::CreateSlice(void) {
  d_slice = new Path;
  assert(d_slice);
  d_slice->Loop(true);

  double angleDelta = 2.0 * M_PI / (double)d_widthUnits;
  double angle = 0.0;
  for(unsigned int j=0; j<d_widthUnits; j++, angle+=angleDelta) {
    d_slice->AddPoint(cos(angle), sin(angle), 0.0);
  }
  d_slice->Scale(d_radius, true, true, false);

  return;
}
