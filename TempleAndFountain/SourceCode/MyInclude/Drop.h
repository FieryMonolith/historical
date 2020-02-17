/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __DROP_H__
#define __DROP_H__

#include <vector>
#include "Vector3d.h"

class Model;

/*

 * Drop - The Drop class represents a physical unit of water, assumed to be a
 *   sphere.  It has mass and is affected by the environment as it moves.
 *   Each Drop is launched from the fountain head and travels down into the
 *   water at the bottom of the fountain following standard time-stepped
 *   projectile motion.

 */
class Drop {
private:
  Drop(void);
  Drop(Drop &);
  Drop &operator=(Drop &);

public:
  // Create a Drop with the given model
  Drop(Model *model);
  ~Drop(void);

  void Draw(void) const;        // Draw the model
  void Update(double timestep); // Move the Drop over the given timestep
  void SetPosition(double x, double y, double z);
  void SetInitialVelocity(double x, double y, double z, double timestep);
  double YPosition(void) const { return d_position[1]; }
  void ResetVertices(void);

private:
  Model *d_model;
  std::vector<Vector3d> d_originalVertices;
  Vector3d d_previousPosition;
  Vector3d d_position;
  double d_mass;
  double d_drag;
  double d_previousTimestep;

  void Grow(double delta); // Changes the size of the model by a delta
  void FindCenter(Vector3d &center); // Finds the bounding square's center
  void StoreOriginalVertices(void);
};

#endif
