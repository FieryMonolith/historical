#include <string>
#include <cassert>

#include "instruction.h"
#include "instruction_set.h"

#include "genome.h"

using namespace std;

Genome::Genome(void) { // : d_hash(-1) {
  return;
}

Genome::~Genome(void) {
  Clear();

  return;
}

bool Genome::Copy(const Genome &g) {
  //d_hash = g.d_hash;
  return CopyInstructions(g.d_instructions);
}

bool Genome::CopyInstructions(const vector<Instruction *> &instructions) {
  //d_hash = -1;

  d_instructions.reserve(instructions.size());

  vector<Instruction *>::const_iterator i;
  for(i=instructions.begin(); i!=instructions.end(); ++i) {
    // Error in my algorithm if a NULL can creep in
    if(*i == NULL) { return false; }

    Instruction *instruction = (*i)->Copy();
    if(instruction == NULL) { return false; }

    d_instructions.push_back(instruction);
  }

  //CalculateHash();

  return true;
}

bool Genome::Stringify(ostream &s) const {
  vector<Instruction *>::const_iterator i;
  for(i = d_instructions.begin(); i != d_instructions.end(); ++i) {
    // Error in my algorithm if a NULL can creep in do not want to handle
    if(*i == NULL) { return false; }

    s << InstructionSet::Stringify((const BasicInstruction &)(**i)) << endl;
  }

  return true;
}

void Genome::Clear(void) {
  vector<Instruction *>::iterator i;
  for(i = d_instructions.begin(); i != d_instructions.end(); ++i) {
    if((*i) != NULL) {
      delete (*i);
      *i = NULL;
    }
  }

  d_instructions.clear();
  //d_hash = -1;

  return;
}

const std::vector<Instruction *> &Genome::Instructions(void) const { 
  return d_instructions;
}

int Genome::operator==(const Genome &g) const {
  return IsEqual(g);
}

int Genome::operator!=(const Genome &g) const {
  return !IsEqual(g);
}

int Genome::IsEqual(const Genome &g) const {
  if(d_instructions.size() != g.d_instructions.size()) { return 0; }

#if 0
  if(d_hash < 0) { CalculateHash(); }
  if(g.d_hash < 0) { g.CalculateHash(); }

  if(d_hash != g.d_hash) { return 0; }
#endif

  for(unsigned int i=0; i<d_instructions.size(); i++) {
    if(d_instructions[i] != (g.d_instructions)[i]) {
      return 0;
    }
  }

  return 1;
}

#if 0
void Genome::CalculateHash(void) const {
  d_hash = 0;
  for(unsigned int i=0; i<d_instructions.size(); i++) {
    if(d_instructions[i] == NULL) { continue; }

    d_hash += d_instructions[i]->InstructionId();
  }

  return;
}
#endif
