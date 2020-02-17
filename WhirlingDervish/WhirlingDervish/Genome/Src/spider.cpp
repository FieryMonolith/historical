#include <vector>

#include "environment_data.h"
#include "genome.h"
#include "body.h"

#include "spider.h"

using namespace std;

Spider::Spider(void) {
  return;
}

Spider::~Spider(void) {
  return;
}

bool Spider::Run(const Genome &g, Body &b, EnvironmentData &ed) const {
  if(g.Instructions().empty()) { return true; }

  State *tempState = NewState();
  if(tempState == NULL) { return false; }
  State &s = *tempState;

  if(!Initialize(s, ed)) {
    return false;
  }

  bool success = Run(s, g, b, ed);

  Complete(s);

  delete tempState;
  tempState = NULL;

  return success;
}

Spider::State *Spider::NewState(void) const {
  return new State;
}

bool Spider::Initialize(State &, EnvironmentData &) const {
  return true;
}

bool Spider::Complete(State &) const {
  return true;
}

bool Spider::Run(State &, const Genome &, Body &, EnvironmentData &) const {
  return true;
}

Spider::State::State(void) {
  return;
}

Spider::State::~State(void) {
  return;
}
