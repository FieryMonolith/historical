#include <cmath>

#include "jvector.h"
#include "random.h"
#include "genome.h"
#include "instruction_set.h"

#include "turtle_basic.h"

TurtleBasic::TurtleBasic(double &chanceOfData, InstructionSet &is)
  : d_chanceOfData(chanceOfData), d_is(is) {
  return;
}

TurtleBasic::~TurtleBasic(void) {
  return;
}

Instruction *TurtleBasic::RandomInstruction(RandomNumberGenerator &rng,
					    State &) const {
  return d_is.RandomInstruction(rng, d_chanceOfData);
}

unsigned int TurtleBasic::RandomParameter(unsigned int id,
					  RandomNumberGenerator &rng) const {
  return d_is.RandomArguement(id, rng, d_chanceOfData);
}

Vector3d TurtleBasic::RandomData(RandomNumberGenerator &rng) const {
  return d_is.RandomData(rng);
}

Instruction *TurtleBasic::MutateParameter(const Instruction &i,
					  RandomNumberGenerator &rng,
					  State &) const {
  return d_is.MutateArguement(i, rng, d_chanceOfData);
}
