#ifndef __INSTRUCTION_SET_H__
#define __INSTRUCTION_SET_H__

#include <vector>
#include <map>
#include <string>

#include "jvector.h"

class BasicInstruction;
class RandomNumberGenerator;

class InstructionSet {
public:
  InstructionSet(void);
  ~InstructionSet(void);

  // If not initialized, default to no instructions
  bool Initialize(const std::vector<std::string> &desiredInstructionList);

  // Determine attribute of instructions and arguements
  static bool IsData(unsigned int id) { return id == DATA; }
  static bool IsBadInstructionId(unsigned int id) {
    return id==BadInstructionId;
  }
  static bool IsBadArguementId(unsigned int id) {
    return id==BadArguementId;
  }

  // Conversions between the integer and string values of an instruction
  static const std::string &GetInstructionName(const BasicInstruction &);
  static const std::string &GetInstructionNameById(unsigned int);
  static unsigned int GetInstructionIdByName(const std::string &);

  // Conversions between the integer and string values of an instruction
  static const std::string &GetArguementName(const BasicInstruction &);
  static const std::string &GetArguementNameById(unsigned int);
  static unsigned int GetArguementIdByName(const std::string &);

  // Information about the current instruction set
  unsigned int NumberOfInstructions(void) const {
    return d_instructionsToUse.size();
  }
  const std::vector<unsigned int> &InstructionIdList(void) const {
    return d_instructionsToUse;
  }
  unsigned int NumberOfArguements(void) const {
    // Don't include BadArguementId
    return d_arguementsById.size();
  }

  // Convert an instruction into a string
  static std::string Stringify(const BasicInstruction &);

  // Validate instruction
  bool Validate(const BasicInstruction &) const;
  // Validate the arguement type for an instruction
  bool Validate(unsigned int instructionId, unsigned int arguementId) const;

  // Random functions
  Instruction *RandomInstruction(RandomNumberGenerator &,
				 double chanceOfData) const;
  unsigned int RandomArguement(unsigned int id, RandomNumberGenerator &,
			       double chanceOfData) const;
  Vector3d RandomData(RandomNumberGenerator &) const;
  Instruction *MutateArguement(const Instruction &,
			       RandomNumberGenerator &,
			       double chanceOfData) const;

private:
  std::vector<unsigned int> d_instructionsToUse;
  std::vector<unsigned int> d_used;

  static std::map<unsigned int, std::string> d_instructionsById;
  static std::map<std::string, unsigned int> d_instructionsByName;
  static std::map<unsigned int, std::string> d_arguementsById;
  static std::map<std::string, unsigned int> d_arguementsByName;

  void MasterGenerate(void);

public:
  static const std::string BadInstructionName;
  static const std::string BadArguementName;
  static const std::string DataName;

  static const unsigned int BadInstructionId;
  static const unsigned int NOP;
  static const unsigned int POP;
  static const unsigned int PUSH;
  static const unsigned int PUSH1;
  static const unsigned int PUSH2;
  static const unsigned int PUSH3;
  static const unsigned int ROTATE;
  static const unsigned int ADD;
  static const unsigned int SUB;
  static const unsigned int MULT;
  static const unsigned int DIV;
  static const unsigned int MOD;
  static const unsigned int SET_SPRING_CONSTANTS;
  static const unsigned int SET_SPRING_FLUCTUATIONS;
  static const unsigned int SET_POINT_PARAMETERS;
  static const unsigned int ADD_LINE;
  static const unsigned int REMOVE_LINE;
  static const unsigned int REMOVE_POINT;
  static const unsigned int MERGE;
  static const unsigned int MOVE_TO;
  static const unsigned int SPLIT;
  static const unsigned int SPLIT_DUP;
  static const unsigned int SHIFT_DATA_LEFT;
  static const unsigned int SHIFT_DATA_RIGHT;
  static const unsigned int SWITCH_STACK;
  static const unsigned int PARTIAL;
  static const unsigned int JUMP;
  static const unsigned int LABEL;
  static const unsigned int PUSH_D;
  static const unsigned int POP_D;
  static const unsigned int TOP_D;
  static const unsigned int SPLIT_PARTIAL;
  static const unsigned int SPLIT_ORIENTED;
  static const unsigned int SPLIT_DUP_ORIENTED;
  static const unsigned int SPLIT_PARTIAL_ORIENTED;
  static const unsigned int IF;
  static const unsigned int ELSE;
  static const unsigned int END;
  static const unsigned int LOOP;
  static const unsigned int PRINT;
  static const unsigned int SET_DILATION;
  static const unsigned int POP_LINE_D;
  static const unsigned int TOP_LINE_D;

  static const unsigned int BadArguementId;
  static const unsigned int NO_PARAMETER;
  static const unsigned int DATA;
  static const unsigned int STACK_A;
  static const unsigned int STACK_B;
  static const unsigned int STACK_C;
  static const unsigned int STACK_D;
  static const unsigned int GRAVITY;
  static const unsigned int POINT_PARAMETERS;
  static const unsigned int SPRING_CONSTANTS;
  static const unsigned int SPRING_FLUCTUATIONS;
  static const unsigned int LOCATION;
  static const unsigned int ORIENTATION_UP;
  static const unsigned int ORIENTATION_RIGHT;
  static const unsigned int ORIENTATION_FORWARD;
  static const unsigned int DILATION;
};

#endif
