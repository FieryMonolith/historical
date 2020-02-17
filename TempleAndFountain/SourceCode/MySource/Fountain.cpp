/*

 * Copyright 2004 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

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

#include <cassert>
#define _USE_MATH_DEFINES
#include <cmath>

#include "Drop.h"
#include "Model.h"
#include "Vector3d.h"

#include "Fountain.h"

using namespace std;

Fountain::Fountain(Model *drop, Model *waterTop)
: d_baseDrop(drop),
  d_waterTop(waterTop),
  d_time(1.0)
{
  assert(drop);
  assert(waterTop);

  // Compute the three directions of water flow
  const double angleTheta = 60.0 * M_PI / 180.0;
  const double ycomponent = sin(angleTheta);
  const double xcomponent = cos(angleTheta);

  const double angle1 = 60.0 * M_PI / 180.0;
  const double angle2 = 300.0 * M_PI / 180.0;
  d_directions.resize(3);
  d_directions[0] = Vector3d(xcomponent, ycomponent, 0.0);
  Normalize(d_directions[0]);

  d_directions[1] = Vector3d(-xcomponent*cos(angle1),
                             ycomponent,
                             xcomponent*sin(angle1));
  Normalize(d_directions[1]);

  d_directions[2] = Vector3d(-xcomponent*cos(angle2),
                             ycomponent,
                             xcomponent*sin(angle2));
  Normalize(d_directions[2]);

  // Create all the drops of water that will be used for the fountain
  //   water flow.
  d_free.reserve(130);
  d_inUse.reserve(130);
  for(unsigned int i=0; i<200; i++) {
    Drop *d = new Drop(d_baseDrop->Copy());
    assert(d);
    d_free.push_back(d);
  }

  return;
}

Fountain::~Fountain(void) {
  if(d_baseDrop != 0) {
    delete d_baseDrop;
  }

  if(d_waterTop != 0) {
    delete d_waterTop;
  }

  vector<Drop *>::iterator i;
  for(i=d_inUse.begin(); i!=d_inUse.end(); ++i) {
    if(*i != 0) {
      delete *i;
      *i = 0;
    }
  }

  for(i=d_free.begin(); i!=d_free.end(); ++i) {
    if(*i != 0) {
      delete *i;
      *i = 0;
    }
  }

  return;
}

// Draw all of the Drop models
void Fountain::Draw(void) const {
  if(d_waterTop != 0) {
    d_waterTop->Draw();
  }

  vector<Drop *>::const_iterator i;
  for(i=d_inUse.begin(); i!=d_inUse.end(); ++i) {
    if(*i != 0) {
      (*i)->Draw();
    }
  }

  return;
}

// Update all the Drops in motion, throw out new ones if necessary, and
//   remove those that have fallen past a certain point
void Fountain::Update(double timestep) {
  vector<unsigned int> toremove;

  // Update position and size of all the Drops in motion, and record those to
  //   remove
  unsigned int index=0;
  vector<Drop *>::const_iterator i;
  for(i=d_inUse.begin(); i!=d_inUse.end(); ++i) {
    if(*i != 0) {
      (*i)->Update(timestep);
      if((*i)->YPosition() < 36.0) {
        toremove.push_back(index);
      }
    }
    index++;
  }

  // remove those Drops that are marked for removal by taking them out of the
  //   inUse pile and putting them into the free pile
  vector<unsigned int>::iterator itr;
  for(itr=toremove.begin(); itr!=toremove.end(); ++itr) {
    d_free.push_back(d_inUse[*itr]);
    d_inUse[*itr] = d_inUse[d_inUse.size() - 1];
    d_inUse.pop_back();
  }

  // Determine if it is time to create new drops and create them if so.
  if(d_time > 0.025) {
    d_time = 0.0;
    for(unsigned int i=0; i<3; i++) {
      if(!d_free.empty()) {
        Drop *d = d_free[d_free.size() - 1];
        d_free.pop_back();
        d_inUse.push_back(d);

        // Initialize a new drop
        d->ResetVertices();
        d->SetPosition(0.0, 78.0, 0.0);
        Vector3d velocity(d_directions[i]);
        double delta = d_rng.GetDouble(65.0, 85.0);
        //velocity *= 77.2777;
        velocity *= delta;
        d->SetInitialVelocity(velocity[0], velocity[1], velocity[2], 0.11/35.0);
      }
    }
  }

  d_time += timestep;

  return;
}
