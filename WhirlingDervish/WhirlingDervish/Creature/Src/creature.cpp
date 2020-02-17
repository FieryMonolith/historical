#include "environment_data.h"
#include "spider.h"
#include "turtle.h"

#include "creature.h"

Creature::Creature(void) {
  return;
}

Creature::~Creature(void) {
  return;
}

bool Creature::Kill(EnvironmentData &ed) {
  d_body.Clear(ed);
  d_genome.Clear();

  return true;
}

Creature *Creature::CopyReproduction(EnvironmentData &ed, Spider &spider,
				     Turtle &turtle) {
  Creature *c = new Creature;
  if(c != NULL && !CopyReproduction(*c, ed, spider, turtle)) {
    delete c;
    c = NULL;
  }

  return c;
}

bool Creature::CopyReproduction(Creature &c, EnvironmentData &ed,
				Spider &spider, Turtle &turtle) {
  if(!turtle.CopyDivide(c.d_genome, d_genome)) {
    return false;
  }

  return c.Create(c, ed, spider);
}

Creature *Creature::AsexualReproduction(EnvironmentData &ed,
					Spider &spider,	Turtle &turtle) {
  Creature *c = new Creature;
  if(c != NULL && !AsexualReproduction(*c, ed, spider, turtle)) {
    delete c;
    c = NULL;
  }

  return c;
}

bool Creature::AsexualReproduction(Creature &c, EnvironmentData &ed,
				   Spider &spider, Turtle &turtle) {
  if(!turtle.Divide(c.d_genome, d_genome, ed)) {
    return false;
  }

  return c.Create(c, ed, spider);
}

bool Creature::Create(Creature &c, EnvironmentData &ed, Spider &spider) {
  return spider.Run(c.d_genome, c.d_body, ed);
}
