#ifndef __TURTLE_BASIC_H__
#define __TURTLE_BASIC_H__

#include "turtle.h"
#include "instruction_set.h"

class Vector3d;

class TurtleBasic : public Turtle {
public:
  TurtleBasic(double &chanceOfData, InstructionSet &is);
  ~TurtleBasic(void);

protected:
  double &d_chanceOfData;
  InstructionSet &d_is;

  Instruction *RandomInstruction(RandomNumberGenerator &, State &) const;
  unsigned int RandomParameter(unsigned int id, RandomNumberGenerator &) const;
  Vector3d RandomData(RandomNumberGenerator &) const;
  Instruction *MutateParameter(const Instruction &,
			       RandomNumberGenerator &,
			       State &) const;
};

#endif
