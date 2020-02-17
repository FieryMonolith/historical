/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __FOUNTAIN_H__
#define __FOUNTAIN_H__

#include <vector>
#include "Random.h"

class Drop;
class Model;

/*

 * Fountain - The Fountain class manages flow of water in the fountain.  The
 *   The models encapsulated in this class are the static bubble of water
 *   at the top of the fountain head, the static pool of water at the bottom
 *   of the fountain, and the emerging Drops.  The static bubble of water is
 *   used to hide the appearance of new Drops of water emerging.  This class
 *   is also responsible for determining the initial direction, velocity, and
 *   start time of new drop.  There are three major directions in which a Drop
 *   will be thrown.

 * The Drops used by the fountain are created in a fixed quantity and the ones
 *   in reserve and the ones in motion are managed by this class.

 */
class Fountain {
private:
  Fountain(void);
  Fountain(Fountain &);
  Fountain &operator=(Fountain &);

public:
  // Create a the fountain dynamics and it associated drops of water
  Fountain(Model *drop, Model *waterTop);
  ~Fountain(void);

  // Draw all of the Drop models
  void Draw(void) const;
  // Update all the Drops currently in motion
  void Update(double timestep);

private:
  Model *d_baseDrop; // Model for the static bubble of water on the head
  Model *d_waterTop; // Model for the pool of water at the fountain bottom
  std::vector<Vector3d> d_directions; // The general directions of flow
  // There are a fixed number of Drops in total
  std::vector<Drop *> d_inUse; // Drops in motion
  std::vector<Drop *> d_free;  // Drop waiting to be released
  double d_time; // Used to determine when to release the next Drop

  // Used to generate randomness in the emergin "streams" of water
  RandomNumberGenerator d_rng;
};

#endif
