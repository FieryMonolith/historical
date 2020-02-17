#include "genome.h"
#include "random.h"
#include "instruction.h"
#include "environment_data.h"

#include "turtle.h"

using namespace std;

Turtle::Turtle(void) {
  return;
}

Turtle::~Turtle(void) {
  return;
}

bool Turtle::CopyDivide(Genome &out, const Genome &in) {
  State *tempState = NewState(in);
  if(tempState == NULL) { return false; }
  State &s = *tempState;
  
  if(!Initialize(s)) {
    return false;
  }

  const vector<Instruction *> &genome_instructions = s.genome_instructions;
  vector<Instruction *> &instructions = s.d_instructions;

  // Estimate to help reduce array resizing
  instructions.reserve(genome_instructions.size());

  Instruction *instruction = NULL;

  bool success = true;
  vector<Instruction *>::const_iterator it;
  for(it = genome_instructions.begin(); it!=genome_instructions.end(); ++it) {
    if(*it == NULL) {
      success = false;
      break;
    }

    // No mutation, so simply make a copy
    instruction = (*it)->Copy();
    if(instruction == NULL) { success = false; break; }
    instructions.push_back(instruction);
  }

  if(success) {
    success = out.CopyInstructions(instructions);
  }

  Complete(s);

  delete tempState;
  tempState = NULL;

  if(success) {
    return true;
  }

  return false;
}

bool Turtle::Divide(Genome &out, const Genome &in, EnvironmentData &ed) {
  State *tempState = NewState(in);
  if(tempState == NULL) { return false; }
  State &s = *tempState;
  
  if(!Initialize(s)) {
    return false;
  }

  const vector<Instruction *> &genome_instructions = s.genome_instructions;
  vector<Instruction *> &instructions = s.d_instructions;

  RandomNumberGenerator &rng = ed.GetRng();
  ECSettings &ecSettings = ed.GetECSettings();

  // Estimate to help reduce array resizing
  instructions.reserve(genome_instructions.size() +
		       (int)(0.01*genome_instructions.size()) +
		       1);

  double alterationRate = ecSettings.NonGenomicRate();
  alterationRate += ecSettings.GenomicRate() / 
                    ((double)genome_instructions.size() + 1.0);

  double chance;
  double d_chanceOfInsertion = ecSettings.InsertionRandom();
  double d_chanceOfInsertionDuplication = ecSettings.InsertionDuplication();
  double d_chanceOfDeletion = ecSettings.Deletion();
  double d_chanceOfPoint = ecSettings.Point();

  double d_i = d_chanceOfInsertion;
  double d_iid = d_i + d_chanceOfInsertionDuplication;
  double d_iidd = d_iid + d_chanceOfDeletion;

  NormalizeChances(d_chanceOfInsertion, d_chanceOfInsertionDuplication,
		   d_chanceOfDeletion, d_chanceOfPoint);

  Instruction *instruction = NULL;

  int mutations = 0;
  // Check for insert at front of the genome
  chance = rng.GetDouble();
  if(chance <= alterationRate) {
    chance = rng.GetDouble();
    if(chance < d_chanceOfInsertion) {
      mutations++;
      instruction = RandomInstruction(rng, s);
      if(instruction == NULL) {
	Complete(s);
	delete tempState;
	tempState = NULL;
	return false;
      }
      instructions.push_back(instruction);
    }
  }

  bool success = true;
  vector<Instruction *>::const_iterator it;
  for(it = genome_instructions.begin(); it!=genome_instructions.end(); ++it) {
    if(*it == NULL) {
      success = false;
      break;
    }

    // Determine if a mutation occurs
    chance = rng.GetDouble();
    if(chance < alterationRate) {
      mutations++;
      double type = rng.GetDouble();
      if(type < d_i) {
	if(!Insert(instructions, **it, rng, s)) { success = false; break; }
      } else if(type < d_iid) {
	if(!InsertCopy(instructions, **it)) { success = false; break; }
      } else if(type < d_iidd) {
	// Deletion, so the instruction is not copied
	continue;
      } else {
	// Point mutation
	if(!Mutation(instructions, **it, rng, s)) { success = false; break; }
      }
      continue;
    }

    // No mutation, so simply make a copy
    instruction = (*it)->Copy();
    if(instruction == NULL) { success = false; break; }
    instructions.push_back(instruction);
  }

  if(success) {
    success = out.CopyInstructions(instructions);
  }

  out.Mutations() = mutations;

  Complete(s);

  delete tempState;
  tempState = NULL;

  if(success) {
    return true;
  }

  return false;
}

bool Turtle::Insert(vector<Instruction *> &instructions,
		    const Instruction &instruction,
		    RandomNumberGenerator &rng,
		    State &s) {
  // Insert always occurs after the copy, because we do an initial check
  // for insertion at the beginning of the genome
  Instruction *i = instruction.Copy();
  if(i == NULL) { return false; }
  instructions.push_back(i);

  // Insert random instruction
  i = RandomInstruction(rng, s);
  if(i == NULL) { return false; }
  instructions.push_back(i);

  return true;
}

bool Turtle::InsertCopy(vector<Instruction *> &instructions,
			const Instruction &instruction) {
  // Insertion follows the idea of gene duplication, thus it is 
  // an exact copy of the current instruction
  Instruction *i = instruction.Copy();
  if(i == NULL) { return false; }
  instructions.push_back(i);

  i = instruction.Copy();
  if(i == NULL) { return false; }
  instructions.push_back(i);

  return true;
}

bool Turtle::Mutation(vector<Instruction *> &instructions,
		      const Instruction &instruction,
		      RandomNumberGenerator &rng,
		      State &s) {
  Instruction *i = NULL;
  double chance = rng.GetDouble();
  if(chance < 0.5) {
    i = RandomInstruction(rng, s);
    if(i == NULL) { return false; }
    instructions.push_back(i);
  } else {
    i = MutateParameter(instruction, rng, s);
    if(i == NULL) { return false; }
    instructions.push_back(i);
  }

  return true;
}

Turtle::State *Turtle::NewState(const Genome &g) const {
  return new State(g.Instructions());
}

bool Turtle::Initialize(State &) const {
  return true;
}

bool Turtle::Complete(State &) const {
  return true;
}

Instruction *Turtle::RandomInstruction(RandomNumberGenerator &,
				       State &) const {
  return NULL;
}

Instruction *Turtle::MutateParameter(const Instruction &,
				     RandomNumberGenerator &,
				     State &) const {
  return NULL;
}

void Turtle::NormalizeChances(double &insertion, double &duplication,
			      double &deletion, double &point) const {
  double sum = insertion + duplication + deletion + point;
  if(sum == 0) { return; }

  insertion   /= sum;
  duplication /= sum;
  deletion    /= sum;
  point       /= sum;

  return;
}

Turtle::State::State(const vector<Instruction *> &i) : genome_instructions(i) {
  return;
}

Turtle::State::~State(void) {
  // Temporarily we duplicate the duplicates, so will always remove the 
  // temporary instructions
  vector<Instruction *>::iterator i;
  for(i = d_instructions.begin(); i != d_instructions.end(); ++i) {
    if(*i != NULL) {
      delete *i;
      *i = NULL;
    }
  }
  d_instructions.clear();

  return;
}
