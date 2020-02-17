#include "instruction_basic.h"

BasicInstruction::BasicInstruction(unsigned int instructionId, 
				   unsigned int arguementId,
				   const Vector3d &data)
  : d_instructionId(instructionId), d_arguementId(arguementId), d_value(data)
{
  return;
}

BasicInstruction::~BasicInstruction(void) {
  return;
}

Instruction *BasicInstruction::Copy(void) const {
  return new BasicInstruction(d_instructionId, d_arguementId, d_value);
}

int BasicInstruction::operator==(const Instruction &i) const {
  return IsEqual(i);
}

int BasicInstruction::operator!=(const Instruction &i) const {
  return !IsEqual(i);
}

int BasicInstruction::IsEqual(const Instruction &i) const {
  const BasicInstruction &ci = (const BasicInstruction &)i;

  if(d_instructionId != ci.d_instructionId) { return 0; }

  if(d_arguementId != ci.d_arguementId) { return 0; }

  if(d_value != ci.d_value) { return 0; }

  return 1;
}
