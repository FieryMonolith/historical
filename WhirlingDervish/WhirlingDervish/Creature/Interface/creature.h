#ifndef __CREATURE_H__
#define __CREATURE_H__

#include "body.h"
#include "genome.h"

class EnvironmentData;
class Spider;
class Turtle;

class Creature {
public:
  Creature(void);
  ~Creature(void);

  // Creates a copy, not mutated, and returns it.  It returns NULL if it could
  // not create the creature or if the copy process fails
  Creature *CopyReproduction(EnvironmentData &, Spider &, Turtle &);
  // Takes this creature and makes a copy, not mutated, and that copy goes into
  // the Creature &, the first arguement.
  bool CopyReproduction(Creature &, EnvironmentData &, Spider &, Turtle &);
  // Creates a copy, mutated, and returns it.  It returns NULL if it could
  // not create the creature or if the copy process fails
  Creature *AsexualReproduction(EnvironmentData &, Spider &, Turtle &);
  // Takes this creature and makes a copy, mutated, and that copy goes into
  // the Creature &, the first arguement.
  bool AsexualReproduction(Creature &, EnvironmentData &, Spider &, Turtle &);
  // Makes the creature, one that has an empty genome.
  bool Kill(EnvironmentData &);

  const Body &GetBody(void) const { return d_body; }
        Body &GetBody(void)       { return d_body; }
  const Genome &GetGenome(void) const { return d_genome; }
        Genome &GetGenome(void)       { return d_genome; }

private:
  Body d_body;
  Genome d_genome;

  // Responsibility of the outside world to make sure that the
  // current genome and body state is correct for where this function
  // begins.  Currently, the assume no body has not been constructed yet.
  bool Create(Creature &, EnvironmentData &, Spider &);

  Creature(const Creature &);
  Creature &operator=(const Creature &);
};

#endif
