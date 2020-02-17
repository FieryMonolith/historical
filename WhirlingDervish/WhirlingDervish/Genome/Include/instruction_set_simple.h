#ifndef __INSTRUCTION_SET_SIMPLE_H__
#define __INSTRUCTION_SET_SIMPLE_H__

#include <string>

#include "instruction_set.h"

class SimpleInstruction : public Instruction {
public:
  SimpleInstruction(void) { return; }
  ~SimpleInstruction(void) { return; }

  enum { SPLIT=0, SPLIT_DUP, MERGE, REMOVE_POINT, ADD_LINE, REMOVE_LINE,
	 CHANGE_MASS, CHANGE_FLUCTUATION_DELTA, CHANGE_FLUCTUATION_START,
	 CHANGE_KS, CHANGE_KD, MOVE_TO, NOP, NUMBER_OF_INSTRUCTIONS
  };
  enum { X=137, Y, Z, MASS, ID1, ID2, KS, KD, FDELTA, FSTART };

  static const std::string splitName;
  static const std::string splitDupName;
  static const std::string mergeName;
  static const std::string removePointName;
  static const std::string addLineName;
  static const std::string removeLineName;
  static const std::string changeMassName;
  static const std::string changeFluctuationDeltaName;
  static const std::string changeFluctuationStartName;
  static const std::string changeKSName;
  static const std::string changeKDName;
  static const std::string moveToName;
  static const std::string nopName;
};

class Split : public SimpleInstruction {
public:
  Split(int id, double mass, double x, double y, double z, 
	double ks, double kd,
	double fluctuationDelta, double fluctuationStart);
  ~Split(void) { return; }

  std::string Stringify(void) const;
  Instruction *Copy(void) const;
  const std::string &Name(void) const { return SimpleInstruction::splitName; }
  int InstructionId(void) const { return SPLIT; }
  int operator==(const Instruction &) const;
  int operator!=(const Instruction &) const;
  int IsEqual(const Instruction &) const;

  int d_id;
  double d_mass;
  double d_x, d_y, d_z;
  double d_ks, d_kd, d_fluctuationDelta, d_fluctuationStart;
};

class SplitDup : public SimpleInstruction {
public:
  SplitDup(int id, double mass, double x, double y, double z, 
	   double ks, double kd,
	   double fluctuationDelta, double fluctuationStart);
  ~SplitDup(void) { return; }

  std::string Stringify(void) const;
  Instruction *Copy(void) const;
  const std::string &Name(void) const {
    return SimpleInstruction::splitDupName;
  }
  int InstructionId(void) const { return SPLIT_DUP; }
  int operator==(const Instruction &) const;
  int operator!=(const Instruction &) const;
  int IsEqual(const Instruction &) const;

  int d_id;
  double d_mass;
  double d_x, d_y, d_z;
  double d_ks, d_kd, d_fluctuationDelta, d_fluctuationStart;
};

class Merge : public SimpleInstruction {
public:
  Merge(int pointId, int newId);
  ~Merge(void) { return; }

  std::string Stringify(void) const;
  Instruction *Copy(void) const;
  const std::string &Name(void) const { return SimpleInstruction::mergeName; }
  int InstructionId(void) const { return MERGE; }
  int operator==(const Instruction &) const;
  int operator!=(const Instruction &) const;
  int IsEqual(const Instruction &) const;

  int d_pointId, d_newId;
};

class RemovePoint : public SimpleInstruction {
public:
  RemovePoint(int pointId);
  ~RemovePoint(void) { return; }

  std::string Stringify(void) const;
  Instruction *Copy(void) const;
  const std::string &Name(void) const {
    return SimpleInstruction::removePointName;
  }
  int InstructionId(void) const { return REMOVE_POINT; }
  int operator==(const Instruction &) const;
  int operator!=(const Instruction &) const;
  int IsEqual(const Instruction &) const;

  int d_pointId;
};

class AddLine : public SimpleInstruction {
public:
  AddLine(int pointId, double ks, double kd,
	  double fluctuationDelta, double fluctuationStart);
  ~AddLine(void) { return; }

  std::string Stringify(void) const;
  Instruction *Copy(void) const;
  const std::string &Name(void) const {
    return SimpleInstruction::addLineName;
  }
  int InstructionId(void) const { return ADD_LINE; }
  int operator==(const Instruction &) const;
  int operator!=(const Instruction &) const;
  int IsEqual(const Instruction &) const;

  int d_pointId;
  double d_ks, d_kd, d_fluctuationDelta, d_fluctuationStart;
};

class RemoveLine : public SimpleInstruction {
public:
  RemoveLine(int pointId);
  ~RemoveLine(void) { return; }

  std::string Stringify(void) const;
  Instruction *Copy(void) const;
  const std::string &Name(void) const {
    return SimpleInstruction::removeLineName;
  }
  int InstructionId(void) const { return REMOVE_LINE; }
  int operator==(const Instruction &) const;
  int operator!=(const Instruction &) const;
  int IsEqual(const Instruction &) const;

  int d_pointId;
};

class ChangeMass : public SimpleInstruction {
public:
  ChangeMass(double delta);
  ~ChangeMass(void) { return; }

  std::string Stringify(void) const;
  Instruction *Copy(void) const;
  const std::string &Name(void) const {
    return SimpleInstruction::changeMassName;
  }
  int InstructionId(void) const { return CHANGE_MASS; }
  int operator==(const Instruction &) const;
  int operator!=(const Instruction &) const;
  int IsEqual(const Instruction &) const;

  double d_delta;
};

class ChangeFluctuationDelta : public SimpleInstruction {
public:
  ChangeFluctuationDelta(int pointId, double delta);
  ~ChangeFluctuationDelta(void) { return; }

  std::string Stringify(void) const;
  Instruction *Copy(void) const;
  const std::string &Name(void) const {
    return SimpleInstruction::changeFluctuationDeltaName;
  }
  int InstructionId(void) const { return CHANGE_FLUCTUATION_DELTA; }
  int operator==(const Instruction &) const;
  int operator!=(const Instruction &) const;
  int IsEqual(const Instruction &) const;

  int d_pointId;
  double d_delta;
};

class ChangeFluctuationStart : public SimpleInstruction {
public:
  ChangeFluctuationStart(int pointId, double delta);
  ~ChangeFluctuationStart(void) { return; }

  std::string Stringify(void) const;
  Instruction *Copy(void) const;
  const std::string &Name(void) const {
    return SimpleInstruction::changeFluctuationStartName;
  }
  int InstructionId(void) const { return CHANGE_FLUCTUATION_START; }
  int operator==(const Instruction &) const;
  int operator!=(const Instruction &) const;
  int IsEqual(const Instruction &) const;

  int d_pointId;
  double d_delta;
};

class ChangeKS : public SimpleInstruction {
public:
  ChangeKS(int pointId, double delta);
  ~ChangeKS(void) { return; }

  std::string Stringify(void) const;
  Instruction *Copy(void) const;
  const std::string &Name(void) const {
    return SimpleInstruction::changeKSName;
  }
  int InstructionId(void) const { return CHANGE_KS; }
  int operator==(const Instruction &) const;
  int operator!=(const Instruction &) const;
  int IsEqual(const Instruction &) const;

  int d_pointId;
  double d_delta;
};

class ChangeKD : public SimpleInstruction {
public:
  ChangeKD(int pointId, double delta);
  ~ChangeKD(void) { return; }

  std::string Stringify(void) const;
  Instruction *Copy(void) const;
  const std::string &Name(void) const {
    return SimpleInstruction::changeKDName;
  }
  int InstructionId(void) const { return CHANGE_KD; }
  int operator==(const Instruction &) const;
  int operator!=(const Instruction &) const;
  int IsEqual(const Instruction &) const;

  int d_pointId;
  double d_delta;
};

class MoveTo : public SimpleInstruction {
public:
  MoveTo(int pointId);
  ~MoveTo(void) { return; }

  std::string Stringify(void) const;
  Instruction *Copy(void) const;
  const std::string &Name(void) const { return SimpleInstruction::moveToName; }
  int InstructionId(void) const { return MOVE_TO; }
  int operator==(const Instruction &) const;
  int operator!=(const Instruction &) const;
  int IsEqual(const Instruction &) const;

  int d_pointId;
};

class Nop : public SimpleInstruction {
public:
  Nop(void);
  ~Nop(void) { return; }

  std::string Stringify(void) const;
  Instruction *Copy(void) const;
  const std::string &Name(void) const { return SimpleInstruction::nopName; }
  int InstructionId(void) const { return NOP; }
  int operator==(const Instruction &) const;
  int operator!=(const Instruction &) const;
  int IsEqual(const Instruction &) const;
};

#endif
