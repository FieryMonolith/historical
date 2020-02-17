#include "genome.h"
#include "random.h"
#include "instruction_set_simple.h"

#include "turtle_simple.h"

#include "constraints.h"

TurtleSimple::TurtleSimple(void) {
  return;
}

TurtleSimple::~TurtleSimple(void) {
  return;
}

State *TurtleSimple::NewState(const Genome &g) const {
  return new StateSimple(g.Instructions());
}

bool TurtleSimple::Initialize(State &s) const {
  s.d_pointIds.reserve(genome_instructions.size()+10);
  s.d_pointIdAvailable = 1;

  // Fills pointIds with the used values.  An id is considered in use 
  // if its value is taken from a valid split, split_dup, or merge instruction
  vector<Instruction *>::const_iterator it;
  for(it = s.d_instructions.begin(); it != s.d_instructions.end(); ++it) {
    if(!(*it)->IsValid()) { continue; }

    int id = -1;
    switch((*it)->InstructionId()) {
    case SimpleInstruction::SPLIT:
      id = ((Split *)(*it))->d_id;
      s.d_pointIds.push_back(id);
    case SimpleInstruction::SPLIT_DUP:
      id = ((SplitDup *)(*it))->d_id;
      s.d_pointIds.push_back(id);
    case SimpleInstruction::MERGE:
      id = ((Merge *)(*it))->d_newId;
      s.d_pointIds.push_back(id);
    }

    if(id >= s.d_pointIdAvailable) {
      s.d_pointIdAvailable = id + 1;
    }
  }

  return true;
}

bool TurtleSimple::Complete(State &s) const {
  s.d_pointIds.clear();
  s.d_pointIdAvailable = 0;

  return true;
}

Instruction *TurtleSimple::RandomInstruction(RandomNumberGenerator &rng,
					     State &s) const{
  unsigned int which =
    rng.GetUInt(SimpleInstruction::NUMBER_OF_SIMPLE_INSTRUCTIONS) +
    SimpleInstruction::SPLIT;
  int newId = 0;

  // Potential Problem with overflow here
  if(which == SimpleInstruction::SPLIT ||
     which == SimpleInstruction::SPLIT_DUP ||
     which == SimpleInstruction::MERGE) {
    newId = NewId(s);
    s.d_pointIds.push_back(newId);
  }

  switch(which) {
  case SimpleInstruction::SPLIT:
    return new Split(newId,
		     0, // delta from default mass
		     rng.GetDouble(-1.0, 1.0), // x
		     rng.GetDouble(-1.0, 1.0), // y
		     rng.GetDouble(-1.0, 1.0), // z
		     MSDConstraints::DEFAULT_KS,
		     MSDConstraints::DEFAULT_KD,
		     MSDConstraints::DEFAULT_FLUCTUATION_DELTA,
		     MSDConstraints::DEFAULT_FLUCTUATION_START);
  case SimpleInstruction::SPLIT_DUP:
    return new SplitDup(newId,
		     0, // delta from mass
		     rng.GetDouble(-1.0, 1.0), // x
		     rng.GetDouble(-1.0, 1.0), // y
		     rng.GetDouble(-1.0, 1.0), // z
		     MSDConstraints::DEFAULT_KS,
		     MSDConstraints::DEFAULT_KD,
		     MSDConstraints::DEFAULT_FLUCTUATION_DELTA,
		     MSDConstraints::DEFAULT_FLUCTUATION_START);
  case SimpleInstruction::MERGE:
    return new Merge(RandomPointId(s.d_pointIds), newId);
  case SimpleInstruction::REMOVE_POINT:
    return new RemovePoint(RandomPointId(s.d_pointIds));
  case SimpleInstruction::ADD_LINE:
    return new AddLine(RandomPointId(s.d_pointIds),
		       MSDConstraints::DEFAULT_KS,
		       MSDConstraints::DEFAULT_KD,
		       MSDConstraints::DEFAULT_FLUCTUATION_DELTA,
		       MSDConstraints::DEFAULT_FLUCTUATION_START);
  case SimpleInstruction::REMOVE_LINE:
    return new RemoveLine(RandomPointId(s.d_pointIds));
  case SimpleInstruction::CHANGE_MASS:
    return new ChangeMass(rng.GetDouble(-1.0, 1.0));
  case SimpleInstruction::CHANGE_FLUCTUATION_DELTA:
    return new ChangeFluctuationDelta(RandomPointId(s.d_pointIds),
				      rng.GetDouble(-1.0, 1.0));
  case SimpleInstruction::CHANGE_FLUCTUATION_START:
    return new ChangeFluctuationStart(RandomPointId(s.d_pointIds),
				      rng.GetDouble(-1.0, 1.0));
  case SimpleInstruction::CHANGE_KS:
    return new ChangeKS(RandomPointId(s.d_pointIds), rng.GetDouble(-1.0, 1.0));
  case SimpleInstruction::CHANGE_KD:
    return new ChangeKD(RandomPointId(s.d_pointIds), rng.GetDouble(-1.0, 1.0));
  case SimpleInstruction::MOVE_TO:
    return new MoveTo(RandomPointId(s.d_pointIds));
  };

  // Nop command or default
  return new Nop;
}


Instruction *TurtleSimple::MutateParameter(const Instruction &i,
					   RandomNumberGenerator &rng,
					   State &s) const {
  Instruction *i = i.Copy();
  if(i == NULL) { return NULL; }
  SimpleInstruction *si = (SimpleInstruction *)i;

  unsigned int which;
  switch(si->InstructionId()) {
  case SimpleInstruction::SPLIT:
    which = rng.GetUInt(9);
    if(which == 0) {
      ((Split *)i)->d_id = NewId(s);
    } else if(which == 1) {
      ((Split *)i)->d_mass += rng.GetDouble(-1.0, 1.0);
    } else if(which == 2) {
      ((Split *)i)->d_x += rng.GetDouble(-1.0, 1.0);
    } else if(which == 3) {
      ((Split *)i)->d_y += rng.GetDouble(-1.0, 1.0);
    } else if(which == 4) {
      ((Split *)i)->d_z += rng.GetDouble(-1.0, 1.0);
    } else if(which == 5) {
      ((Split *)i)->d_ks += rng.GetDouble(-1.0, 1.0);
    } else if(which == 6) {
      ((Split *)i)->d_kd += rng.GetDouble(-1.0, 1.0);
    } else if(which == 7) {
      ((Split *)i)->d_fluctuationDelta += rng.GetDouble(-1.0, 1.0);
    } else {
      ((Split *)i)->d_fluctuationStart += rng.GetDouble(-1.0, 1.0);
    }
    break;
  case SimpleInstruction::SPLIT_DUP:
    which = rng.GetUInt(9);
    if(which == 0) {
      ((SplitDup *)i)->d_id = NewId(s);
    } else if(which == 1) {
      ((SplitDup *)i)->d_mass += rng.GetDouble(-1.0, 1.0);
    } else if(which == 2) {
      ((SplitDup *)i)->d_x += rng.GetDouble(-1.0, 1.0);
    } else if(which == 3) {
      ((SplitDup *)i)->d_y += rng.GetDouble(-1.0, 1.0);
    } else if(which == 4) {
      ((SplitDup *)i)->d_z += rng.GetDouble(-1.0, 1.0);
    } else if(which == 5) {
      ((SplitDup *)i)->d_ks += rng.GetDouble(-1.0, 1.0);
    } else if(which == 6) {
      ((SplitDup *)i)->d_kd += rng.GetDouble(-1.0, 1.0);
    } else if(which == 7) {
      ((SplitDup *)i)->d_fluctuationDelta += rng.GetDouble(-1.0, 1.0);
    } else {
      ((SplitDup *)i)->d_fluctuationStart += rng.GetDouble(-1.0, 1.0);
    }
    break;
  case SimpleInstruction::MERGE:
    if(rng.GetDouble() < 0.5) {
      ((Merge *)i)->d_newId = NewId(s);
    } else {
      ((Merge *)i)->d_pointId = RandomPointId(s.d_pointIds);
    }
    break;
  case SimpleInstruction::ADD_LINE:
    which = rng.GetUInt(5);
    if(which == 0) {
      ((AddLine *)i)->d_pointId = RandomPointId(s.d_pointIds);
    } else if(which == 1) {
      ((AddLine *)i)->d_ks += rng.GetDouble(-1.0, 1.0);
    } else if(which == 2) {
      ((AddLine *)i)->d_kd += rng.GetDouble(-1.0, 1.0);
    } else if(which == 3) {
      ((AddLine *)i)->d_fluctuationDelta += rng.GetDouble(-1.0, 1.0);
    } else {
      ((AddLine *)i)->d_fluctuationStart += rng.GetDouble(-1.0, 1.0);
    }
    break;
  case SimpleInstruction::REMOVE_POINT:
    ((RemovePoint *)i)->d_pointId = RandomPointId(s.d_pointIds);
    break;
  case SimpleInstruction::REMOVE_LINE:
    ((RemoveLine *)i)->d_pointId = RandomPointId(s.d_pointIds);
    break;
  case SimpleInstruction::MOVE_TO:
    ((MoveTo *)i)->d_pointId = RandomPointId(s.d_pointIds);
    break;
  case SimpleInstruction::CHANGE_MASS:
    ((ChangeMass *)i)->d_delta += rng.GetDouble(-1.0, 1.0);
    break;
  case SimpleInstruction::CHANGE_FLUCTUATION_DELTA:
    if(rng.GetDouble() < 0.5) {
      ((ChangeFluctuationDelta *)i)->d_pointId = RandomPointId(s.d_pointIds);
    } else {
      ((ChangeFluctuationDelta *)i)->d_delta += rng.GetDouble(-1.0, 1.0);
    }
    break;
  case SimpleInstruction::CHANGE_FLUCTUATION_START:
    if(rng.GetDouble() < 0.5) {
      ((ChangeFluctuationStart *)i)->d_pointId = RandomPointId(s.d_pointIds);
    } else {
      ((ChangeFluctuationStart *)i)->d_delta += rng.GetDouble(-1.0, 1.0);
    }
    break;
  case SimpleInstruction::CHANGE_KS:
    if(rng.GetDouble() < 0.5) {
      ((ChangeKS *)i)->d_pointId = RandomPointId(s.d_pointIds);
    } else {
      ((ChangeKS *)i)->d_delta += rng.GetDouble(-1.0, 1.0);
    }
    break;
  case SimpleInstruction::CHANGE_KD:
    if(rng.GetDouble() < 0.5) {
      ((ChangeKD *)i)->d_pointId = RandomPointId(s.d_pointIds);
    } else {
      ((ChangeKD *)i)->d_delta += rng.GetDouble(-1.0, 1.0);
    }
    break;
  };

  return i;
}

int TurtleSimple::RandomPointId(const vector<int> &ids) const {
  if(ids.empty()) {
    return 0;
  }

  unsigned int which = rng.GetUInt(ids.size());

  return ids[which];
}

int TurtleSimple::NewId(State &s) const {
  s.d_pointIdAvailable++;
  return (s.d_pointIdAvailable - 1);
}

TurtleSimple::StateSimple::StateSimple(const vector<Instruction *> &i) 
  : Simple(i) {
  return;
}
