#ifndef __INSTRUCTION_BASIC_H__
#define __INSTRUCTION_BASIC_H__

#include "instruction.h"
#include "jvector.h"

class BasicInstruction : public Instruction {
public:
  BasicInstruction(unsigned int instructionId, unsigned int arguementId,
		   const Vector3d &data=Vector3d(0.0, 0.0, 0.0));
  ~BasicInstruction(void);

  Instruction *Copy(void) const;

  unsigned int InstructionId(void) const { return d_instructionId; }
  unsigned int ArguementId(void) const { return d_arguementId; }
  const Vector3d &Data(void) const { return d_value; }

  int operator==(const Instruction &) const;
  int operator!=(const Instruction &) const;

protected:
  unsigned int d_instructionId;
  unsigned int d_arguementId;
  Vector3d d_value;

private:
  int IsEqual(const Instruction &) const;
};

#endif
