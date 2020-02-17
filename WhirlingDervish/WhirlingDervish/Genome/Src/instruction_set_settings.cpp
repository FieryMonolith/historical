#include <fstream>
#include <sstream>

#include "instruction_set_settings.h"

using namespace std;

InstructionSetSettings::InstructionSetSettings(void) {
  return;
}

InstructionSetSettings::~InstructionSetSettings(void) {
  return;
}

bool InstructionSetSettings::Initialize(void) {
  d_set.clear();

  return true;
}

bool InstructionSetSettings::Setup(const string &filename) {
  d_set.clear();

  ifstream in(filename.c_str());
  if(!in) { return false; }

  bool success = true;
  char line[1024];
  while(!in.eof() && in.good()) {
    in.getline(line, 1024);
    stringstream ss;
    ss << line;

    ss >> ws;

    if(ss.eof()) { continue; }

    // Check for comment
    if((char)ss.peek() == '#') { continue; }

    string name;
    ss >> name;
    if(!ss.good() && !ss.eof()) { success = false; break; }

    // Add name to list of instructions to turn on
    d_set.push_back(name);

    // Check for valid end of line
    ss >> ws;
    if(!ss.eof()) {
      if(!ss.good()) { success = false; break; }
      // Check for comment
      if((char)ss.peek() != '#') { success = false; break; }
    }
  }

  in.close();

  if(!in.good() && !in.eof()) { success = false; }

  return success;
}
