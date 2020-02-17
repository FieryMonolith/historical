/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * Drop - The Drop class represents a physical unit sphere of water.  It has
 *   mass and is affected by the environment as it moves.  It is also a model
 *   of a sphere wrapped with a bluish texture that has transparent qualities.
 *   Each Drop is launched from the fountain head and travels down into the
 *   the water at the bottom of the fountain following standard time-stepped
 *   projectile motion.

 */

#include <cassert>
#include <map>

#include "Model.h"

#include "Drop.h"

#include "Matrix.h"
#include "Physics.h"

using namespace std;

// Create a Drop with the given model, which can not be NULL.  It also sets
//   some default values that describe the Drop's physical properties.
Drop::Drop(Model *model)
: d_model(model),
  d_previousPosition(Vector3d()),
  d_position(Vector3d()),
  d_mass(1.0),
  d_drag(0.0),
  d_previousTimestep(0.132)
{
  assert(model);

  StoreOriginalVertices();

  return;
}

Drop::~Drop(void) {
  if(d_model != 0) {
    delete d_model;
  }

  return;
}

// Draw the model
void Drop::Draw(void) const {
  d_model->Draw();

  return;
}

void Drop::SetPosition(double x, double y, double z) {
  d_position = Vector3d(x, y, z);

  Vector3d center;
  FindCenter(center);
  d_model->Transform(Matrix4x4d(1.0, 0.0, 0.0, -center[0],
                                0.0, 1.0, 0.0, -center[1],
                                0.0, 0.0, 1.0, -center[2],
                                0.0, 0.0, 0.0, 1.0));
  d_model->Transform(Matrix4x4d(1.0, 0.0, 0.0, d_position[0],
                                0.0, 1.0, 0.0, d_position[1],
                                0.0, 0.0, 1.0, d_position[2],
                                0.0, 0.0, 0.0, 1.0));
  return;
}

void Drop::SetInitialVelocity(double x, double y, double z, double timestep) {
  Vector3d v(x, y, z);
  d_previousPosition = d_position - (v * timestep);
  d_previousTimestep = timestep;

  return;
}

// Move the drop through space over the given amount of time.
void Drop::Update(double timestep) {
  if(d_model == 0) { return; }

  if(d_position[1] < 0.0) {
    return;
  }

  // Compute velocity
  Vector3d v((d_position - d_previousPosition) / d_previousTimestep);
  v *= 2.54/100.0; // inches per meter

  Vector3d force;
  Physics::CalculateAirResistance(d_drag, v, force);
  Vector3d gravity;
  Physics::CalculateGravitationalForce(d_mass, gravity);
  force += gravity;

  d_previousPosition = d_position;
  d_previousTimestep = timestep;

  // Compute the new position using projectile motion
  // p1 - p0 = v0 * t + (f/m) * t^2
  Vector3d delta = (v * timestep) + ((force / d_mass) * timestep * timestep);
  d_position += (delta * 100.0 / 2.54); // convert back to inches
  if(d_position[1] < 0.0) {
    return;
  }

  d_model->Transform(Matrix4x4d(1.0, 0.0, 0.0, -d_previousPosition[0],
                                0.0, 1.0, 0.0, -d_previousPosition[1],
                                0.0, 0.0, 1.0, -d_previousPosition[2],
                                0.0, 0.0, 0.0, 1.0));
  d_model->Transform(Matrix4x4d(1.0, 0.0, 0.0, d_position[0],
                                0.0, 1.0, 0.0, d_position[1],
                                0.0, 0.0, 1.0, d_position[2],
                                0.0, 0.0, 0.0, 1.0));

  // The water is supposed to increase in volume over time
  Grow(timestep * 4.0);

  return;
}

// Changes the volume of the sphere by changing its "radius", assumed to be
//   a sphere.
void Drop::Grow(double delta) {
  if(d_model == 0) { return; }

  Vector3d center;
  FindCenter(center);

  map<unsigned int, Vector3d *> &vertices = d_model->VertexList();
  map<unsigned int, Vector3d *>::iterator vi;
  for(vi=vertices.begin(); vi!=vertices.end(); ++vi) {
    if(vi->second == 0) { continue; }

    // Create a unit vector in the direction of the sphere center to each
    //   vertex
    Vector3d &t = *(vi->second);
    Vector3d u(t - center);
    double d = Normalize(u);

    // Increase the distance from the center by delta and compute the new
    //   position of the vertex relative to the center
    d += delta;
    u *= d;
    u += center;
    t[0] = u[0]; t[1] = u[1]; t[2] = u[2];
  }

  return;
}

// Find the center of the bounding box for the model
void Drop::FindCenter(Vector3d &center) {
  map<unsigned int, Vector3d *> &vertices = d_model->VertexList();
  map<unsigned int, Vector3d *>::iterator vi;
  double minX, maxX, minY, maxY, minZ, maxZ;
  bool first = true;
  for(vi=vertices.begin(); vi!=vertices.end(); ++vi) {
    if(vi->second == 0) { continue; }

    Vector3d &t = *(vi->second);
    if(first) {
      minX = maxX = t[0];
      minY = maxY = t[1];
      minZ = maxZ = t[2];
      first = false;
    } else {
      if(t[0] < minX) { minX = t[0]; }
      if(t[0] > maxX) { maxX = t[0]; }
      if(t[1] < minY) { minY = t[1]; }
      if(t[1] > maxY) { maxY = t[1]; }
      if(t[2] < minZ) { minZ = t[2]; }
      if(t[2] > maxZ) { maxZ = t[2]; }
    }
  }

  if(!first) {
    center = Vector3d((minX + maxX)/2.0, (minY + maxY)/2.0, (minZ + maxZ)/2.0);
  }

  return;
}

void Drop::StoreOriginalVertices(void) {
  const map<unsigned int, Vector3d *> &vertices = d_model->VertexList();
  map<unsigned int, Vector3d *>::const_iterator vi;

  d_originalVertices.resize(vertices.size());
  unsigned int i=0;
  for(vi=vertices.begin(); vi!=vertices.end(); ++vi) {
    if(vi->second == 0) { continue; }

    Vector3d &t = *(vi->second);
    d_originalVertices[i] = t;
    i++;
  }

  return;
}

// Set all the vertices' positions back to their original values
void Drop::ResetVertices(void) {
  map<unsigned int, Vector3d *> &vertices = d_model->VertexList();
  map<unsigned int, Vector3d *>::iterator vi;

  unsigned int i=0;
  for(vi=vertices.begin(); vi!=vertices.end(); ++vi) {
    if(vi->second == 0) { continue; }

    Vector3d &t = *(vi->second);
    t = d_originalVertices[i];
    i++;
  }

  return;
}
