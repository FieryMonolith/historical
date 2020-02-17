#include "genome.h"
#include "instruction.h"

#include "parser.h"

using namespace std;

bool Parser::Validate(vector<int> &errors, const string &genomeString) const {
  if(genomeString.empty()) {
    return true;
  }

  Genome g;
  return DoParse(g, errors, genomeString, true);
}

bool Parser::Parse(Genome &g, vector<int> &errors,
		   const string &genomeString) const {
  if(genomeString.empty()) {
    return true;
  }

  return DoParse(g, errors, genomeString, false);
}

bool Parser::DoParse(Genome &g, vector<int> &errors, const string &s,
		     bool validate) const {
  State *tempState = NewState();
  if(tempState == NULL) { return false; }
  State &state = *tempState;

  if(!Initialize(state)) {
    delete tempState;
    tempState = NULL;
    return false;
  }

  state.d_validate = validate;

  bool success = true;
  if(!Parse(state, s) && state.d_lineErrors.empty()) {
    success = false;
  }

  errors.assign(state.d_lineErrors.begin(), state.d_lineErrors.end());
  if(success && !state.d_validate) {
    success = g.CopyInstructions(state.d_instructions);
  }

  Complete(state);
  delete tempState;
  tempState = NULL;

  return success;
}

Parser::State *Parser::NewState(void) const {
  return new State;
}

bool Parser::Initialize(State &) const {
  return true;
}

bool Parser::Complete(State &s) const {
  return true;
}

void Parser::CreateInstruction(State &) const {
  return;
}

bool Parser::Parse(State &, const std::string &) const {
  return true;
}

Parser::State::State(void) : d_instructionLine(1) {
  return;
}

Parser::State::~State(void) {
  d_lineErrors.clear();

  vector<Instruction *>::iterator i;
  for(i=d_instructions.begin(); i!=d_instructions.end(); ++i) {
    if(*i != NULL) {
      delete *i;
      *i = NULL;
    }
  }
  d_instructions.clear();

  return;
}
