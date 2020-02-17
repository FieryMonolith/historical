#include <sstream>

#include "instruction_set_simple.h"

using namespace std;

const string SimpleInstruction::splitName("split");
const string SimpleInstruction::splitDupName("split_dup");
const string SimpleInstruction::mergeName("merge");
const string SimpleInstruction::removePointName("remove_point");
const string SimpleInstruction::addLineName("add_line");
const string SimpleInstruction::removeLineName("remove_line");
const string SimpleInstruction::changeMassName("change_mass");
const string SimpleInstruction::changeFluctuationDeltaName("change_fluctuation_delta");
const string SimpleInstruction::changeFluctuationStartName("change_fluctuation_start");
const string SimpleInstruction::changeKSName("change_ks");
const string SimpleInstruction::changeKDName("change_kd");
const string SimpleInstruction::moveToName("move_to");
const string SimpleInstruction::nopName("nop");

Split::Split(int id, double mass, double x, double y, double z,
	     double ks, double kd, double fluctuation_delta, 
	     double fluctuation_start)
  : d_id(id), d_mass(mass), d_x(x), d_y(y), d_z(z), 
    d_ks(ks), d_kd(kd), d_fluctuationDelta(fluctuation_delta), 
    d_fluctuationStart(fluctuation_start) {
  return;
}

string Split::Stringify(void) const {
  stringstream ss;
  ss << SimpleInstruction::splitName << "(" << d_id << ", " << d_mass << ", ";
  ss << d_x << ", " << d_y << ", " << d_z << ", ";
  ss << d_ks << ", " << d_kd << ", " << d_fluctuationDelta << ", ";
  ss << d_fluctuationStart << ");";

  return ss.str();
}

Instruction *Split::Copy(void) const {
  return new Split(d_id, d_mass, d_x, d_y, d_z, d_ks, d_kd,
		   d_fluctuationDelta, d_fluctuationStart);
}

int Split::operator==(const Instruction &i) const {
  return IsEqual(i);
}

int Split::operator!=(const Instruction &i) const {
  return !IsEqual(i);
}

int Split::IsEqual(const Instruction &i) const {
  if(i.InstructionId != SPLIT) { return 0; }

  const Split &x = (const Split &)i;
  if(d_id != x.d_id ||
     d_mass != x.d_mass ||
     d_x != x.d_x ||
     d_y != x.d_y ||
     d_z != x.d_z ||
     d_ks != x.d_ks ||
     d_kd != x.d_kd ||
     d_fluctuationDelta != x.d_fluctuationDelta ||
     d_fluctuationStart != x.d_fluctuationStart) {
    return 0;
  }

  return 1;
}

SplitDup::SplitDup(int id, double mass, double x, double y, double z,
		   double ks, double kd, double fluctuation_delta, 
		   double fluctuation_start)
  : d_id(id), d_mass(mass), d_x(x), d_y(y), d_z(z), 
    d_ks(ks), d_kd(kd), d_fluctuationDelta(fluctuation_delta), 
    d_fluctuationStart(fluctuation_start) {
  return;
}

string SplitDup::Stringify(void) const {
  stringstream ss;
  ss << SimpleInstruction::splitDupName << "(" << d_id << ", ";
  ss << d_mass << ", ";
  ss << d_x << ", " << d_y << ", " << d_z << ", ";
  ss << d_ks << ", " << d_kd << ", " << d_fluctuationDelta << ", ";
  ss << d_fluctuationStart << ");";

  return ss.str();
}

Instruction *SplitDup::Copy(void) const {
  return new SplitDup(d_id, d_mass, d_x, d_y, d_z, d_ks, d_kd,
		      d_fluctuationDelta, d_fluctuationStart);
}

int SplitDup::operator==(const Instruction &i) const {
  return IsEqual(i);
}

int SplitDup::operator!=(const Instruction &i) const {
  return !IsEqual(i);
}

int SplitDup::IsEqual(const Instruction &i) const {
  if(i.InstructionId != SPLIT_DUP) { return 0; }

  const SplitDup &x = (const SplitDup &)i;
  if(d_id != x.d_id ||
     d_mass != x.d_mass ||
     d_x != x.d_x ||
     d_y != x.d_y ||
     d_z != x.d_z ||
     d_ks != x.d_ks ||
     d_kd != x.d_kd ||
     d_fluctuationDelta != x.d_fluctuationDelta ||
     d_fluctuationStart != x.d_fluctuationStart) {
    return 0;
  }

  return 1;
}

Merge::Merge(int pointId, int newId)
  : d_pointId(pointId), d_newId(newId) {
  return;
}

string Merge::Stringify(void) const {
  stringstream ss;
  ss << SimpleInstruction::mergeName << "(" << d_pointId << ", ";
  ss << d_newId << ");";
  return ss.str();
}

Instruction *Merge::Copy(void) const {
  return new Merge(d_pointId, d_newId);
}

int Merge::operator==(const Instruction &i) const {
  return IsEqual(i);
}

int Merge::operator!=(const Instruction &i) const {
  return !IsEqual(i);
}

int Merge::IsEqual(const Instruction &i) const {
  if(i.InstructionId != MERGE) { return 0; }

  const Merge &x = (const Merge &)i;
  if(d_pointId != x.d_pointId ||
     d_newId != x.d_newId) {
    return 0;
  }

  return 1;
}

RemovePoint::RemovePoint(int pointId) : d_pointId(pointId) {
  return;
}

string RemovePoint::Stringify(void) const {
  stringstream ss;
  ss << SimpleInstruction::removePointName << "(" << d_pointId << ");";
  return ss.str();
}

Instruction *RemovePoint::Copy(void) const {
  return new RemovePoint(d_pointId);
}

int RemovePoint::operator==(const Instruction &i) const {
  return IsEqual(i);
}

int RemovePoint::operator!=(const Instruction &i) const {
  return !IsEqual(i);
}

int RemovePoint::IsEqual(const Instruction &i) const {
  if(i.InstructionId != REMOVE_POINT) { return 0; }

  const RemovePoint &x = (const RemovePoint &)i;
  if(d_pointId != x.d_pointId) {
    return 0;
  }

  return 1;
}

AddLine::AddLine(int pointId, double ks, double kd, double fluctuation_delta,
		 double fluctuation_start)
  : d_pointId(pointId), d_ks(ks), d_kd(kd),
    d_fluctuationDelta(fluctuation_delta),
    d_fluctuationStart(fluctuation_start) {
  return;
}

string AddLine::Stringify(void) const {
  stringstream ss;
  ss << SimpleInstruction::addLineName << "(" << d_pointId;
  ss << ", " << d_ks << ", " << d_kd << ", ";
  ss << d_fluctuationDelta << ", " << d_fluctuationStart << ");";
  return ss.str();
}

Instruction *AddLine::Copy(void) const {
  return new AddLine(d_pointId, d_ks, d_kd, d_fluctuationDelta,
		     d_fluctuationStart);
}

int AddLine::operator==(const Instruction &i) const {
  return IsEqual(i);
}

int AddLine::operator!=(const Instruction &i) const {
  return !IsEqual(i);
}

int AddLine::IsEqual(const Instruction &i) const {
  if(i.InstructionId != ADD_LINE) { return 0; }

  const AddLine &x = (const AddLine &)i;
  if(d_pointId != x.d_pointId ||
     d_ks != x.d_ks ||
     d_kd != x.d_kd ||
     d_fluctuationDelta != x.d_fluctuationDelta ||
     d_fluctuationStart != x.d_fluctuationStart) {
    return 0;
  }

  return 1;
}

RemoveLine::RemoveLine(int pointId) : d_pointId(pointId) {
  return;
}

string RemoveLine::Stringify(void) const {
  stringstream ss;
  ss << SimpleInstruction::removeLineName << "(" << d_pointId << ");";
  return ss.str();
}

Instruction *RemoveLine::Copy(void) const {
  return new RemoveLine(d_pointId);
}

int RemoveLine::operator==(const Instruction &i) const {
  return IsEqual(i);
}

int RemoveLine::operator!=(const Instruction &i) const {
  return !IsEqual(i);
}

int RemoveLine::IsEqual(const Instruction &i) const {
  if(i.InstructionId != REMOVE_LINE) { return 0; }

  const RemoveLine &x = (const RemoveLine &)i;
  if(d_pointId != x.d_pointId) {
    return 0;
  }

  return 1;
}

ChangeMass::ChangeMass(double delta) : d_delta(delta) {
  return;
}

string ChangeMass::Stringify(void) const {
  stringstream ss;
  ss << SimpleInstruction::changeMassName << "(" << d_delta << ");";
  return ss.str();
}

Instruction *ChangeMass::Copy(void) const {
  return new ChangeMass(d_delta);
}

int ChangeMass::operator==(const Instruction &i) const {
  return IsEqual(i);
}

int ChangeMass::operator!=(const Instruction &i) const {
  return !IsEqual(i);
}

int ChangeMass::IsEqual(const Instruction &i) const {
  if(i.InstructionId != CHANGE_MASS) { return 0; }

  const ChangeMass &x = (const ChangeMass &)i;
  if(d_delta != x.d_delta) {
    return 0;
  }

  return 1;
}

ChangeFluctuationDelta::ChangeFluctuationDelta(int pointId, double delta) 
  : d_pointId(pointId), d_delta(delta) {
  return;
}

string ChangeFluctuationDelta::Stringify(void) const {
  stringstream ss;
  ss << SimpleInstruction::changeFluctuationDeltaName << "(" << d_pointId;
  ss << ", " << d_delta << ");";
  return ss.str();
}

Instruction *ChangeFluctuationDelta::Copy(void) const {
  return new ChangeFluctuationDelta(d_pointId, d_delta);
}

int ChangeFluctuationDelta::operator==(const Instruction &i) const {
  return IsEqual(i);
}

int ChangeFluctuationDelta::operator!=(const Instruction &i) const {
  return !IsEqual(i);
}

int ChangeFluctuationDelta::IsEqual(const Instruction &i) const {
  if(i.InstructionId != CHANGE_FLUCTUATION_DELTA) { return 0; }

  const ChangeFluctuationDelta &x = (const ChangeFluctuationDelta &)i;
  if(d_pointId != x.d_pointId ||
     d_delta != x.d_delta) {
    return 0;
  }

  return 1;
}

ChangeFluctuationStart::ChangeFluctuationStart(int pointId, double delta) 
  : d_pointId(pointId), d_delta(delta) {
  return;
}

string ChangeFluctuationStart::Stringify(void) const {
  stringstream ss;
  ss << SimpleInstruction::changeFluctuationStartName << "(" << d_pointId;
  ss << ", " << d_delta << ");";
  return ss.str();
}

Instruction *ChangeFluctuationStart::Copy(void) const {
  return new ChangeFluctuationStart(d_pointId, d_delta);
}

int ChangeFluctuationStart::operator==(const Instruction &i) const {
  return IsEqual(i);
}

int ChangeFluctuationStart::operator!=(const Instruction &i) const {
  return !IsEqual(i);
}

int ChangeFluctuationStart::IsEqual(const Instruction &i) const {
  if(i.InstructionId != CHANGE_FLUCTUATION_START) { return 0; }

  const ChangeFluctuationStart &x = (const ChangeFluctuationStart &)i;
  if(d_pointId != x.d_pointId ||
     d_delta != x.d_delta) {
    return 0;
  }

  return 1;
}

ChangeKS::ChangeKS(int pointId, double delta) : d_pointId(pointId), 
						d_delta(delta) {
  return;
}

string ChangeKS::Stringify(void) const {
  stringstream ss;
  ss << SimpleInstruction::changeKSName << "(" << d_pointId << ", ";
  ss << d_delta << ");";
  return ss.str();
}

Instruction *ChangeKS::Copy(void) const {
  return new ChangeKS(d_pointId, d_delta);
}

int ChangeKS::operator==(const Instruction &i) const {
  return IsEqual(i);
}

int ChangeKS::operator!=(const Instruction &i) const {
  return !IsEqual(i);
}

int ChangeKS::IsEqual(const Instruction &i) const {
  if(i.InstructionId != CHANGE_KS) { return 0; }

  const ChangeKS &x = (const ChangeKS &)i;
  if(d_pointId != x.d_pointId ||
     d_delta != x.d_delta) {
    return 0;
  }

  return 1;
}

ChangeKD::ChangeKD(int pointId, double delta) : d_pointId(pointId), 
						d_delta(delta) {
  return;
}

string ChangeKD::Stringify(void) const {
  stringstream ss;
  ss << SimpleInstruction::changeKDName << "(" << d_pointId << ", ";
  ss << d_delta << ");";
  return ss.str();
}

Instruction *ChangeKD::Copy(void) const {
  return new ChangeKD(d_pointId, d_delta);
}

int ChangeKD::operator==(const Instruction &i) const {
  return IsEqual(i);
}

int ChangeKD::operator!=(const Instruction &i) const {
  return !IsEqual(i);
}

int ChangeKD::IsEqual(const Instruction &i) const {
  if(i.InstructionId != CHANGE_KD) { return 0; }

  const ChangeKD &x = (const ChangeKD &)i;
  if(d_pointId != x.d_pointId ||
     d_delta != x.d_delta) {
    return 0;
  }

  return 1;
}

MoveTo::MoveTo(int pointId) : d_pointId(pointId) {
  return;
}

string MoveTo::Stringify(void) const {
  stringstream ss;
  ss << SimpleInstruction::moveToName << "(" << d_pointId << ");";
  return ss.str();
}

Instruction *MoveTo::Copy(void) const {
  return new MoveTo(d_pointId);
}

int MoveTo::operator==(const Instruction &i) const {
  return IsEqual(i);
}

int MoveTo::operator!=(const Instruction &i) const {
  return !IsEqual(i);
}

int MoveTo::IsEqual(const Instruction &i) const {
  if(i.InstructionId != MOVE_TO) { return 0; }

  const MoveTo &x = (const MoveTo &)i;
  if(d_pointId != x.d_pointId) {
    return 0;
  }

  return 1;
}

Nop::Nop(void) {
  return;
}

string Nop::Stringify(void) const {
  stringstream ss;
  ss << SimpleInstruction::nopName << "();";
  return ss.str();
}

Instruction *Nop::Copy(void) const {
  return new Nop;
}

int Nop::operator==(const Instruction &i) const {
  return IsEqual(i);
}

int Nop::operator!=(const Instruction &i) const {
  return !IsEqual(i);
}

int Nop::IsEqual(const Instruction &i) const {
  if(i.InstructionId != NOP) { return 0; }

  return 1;
}
