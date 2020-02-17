#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

class Instruction {
public:
  Instruction(void);
  virtual ~Instruction(void);

  virtual Instruction *Copy(void) const = 0;
  virtual unsigned int InstructionId(void) const = 0;
  virtual int operator==(const Instruction &) const = 0;
  virtual int operator!=(const Instruction &) const = 0;
};

#endif
