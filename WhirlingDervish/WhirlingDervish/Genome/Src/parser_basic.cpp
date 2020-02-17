#include <sstream>

#include "parser_basic.h"

#include "instruction_basic.h"

using namespace std;

ParserBasic::ParserBasic(InstructionSet &is) : d_is(is) {
  return;
}

ParserBasic::~ParserBasic(void) {
  return;
}

Parser::State *ParserBasic::NewState(void) const {
  return new BasicState;
}

bool ParserBasic::CreateBasicInstruction(State &state) const {
  BasicState &s = (BasicState &)state;

  if(!d_is.Validate(s.d_instruction, s.d_arguement)) {
    return false;
  } else if(!s.d_validate) {
    // Validation mode does not create new instructions
    Instruction *i = new BasicInstruction(s.d_instruction, s.d_arguement,
					  s.d_vector);
    if(i == NULL) {
      return false;
    } else {
      s.d_instructions.push_back(i);
    }
  }

  return true;
}

bool ParserBasic::Parse(State &state, const string &str) const {
  BasicState &s = (BasicState &)state;

  stringstream ss;
  ss << str;

  s.d_instructionLine = 1;
  char line[1024];
  bool success = true;
  while(!ss.eof() && ss.good()) {
    ss >> ws;

    if(ss.eof()) { continue; }

    // Check for comment
    if((char)ss.peek() == '#') { ss.getline(line, 1024); continue; }

    // Process instruction name
    string name;
    char a;
    while(ss.peek() != '(' && ss.good()) {
      ss >> a;
      if(!((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || (a == '-') ||
	   (a >= '0' && a <= '9'))) {
	success = false;
	break;
      }
      name += a;
    }
    if(!ss.good() || success == false) { success = false; break; }
    s.d_instruction = d_is.GetInstructionIdByName(name);
    if(d_is.IsBadInstructionId(s.d_instruction)) { success = false; break; }

    ss >> ws;

    // Get open paranthesis
    ss >> a;
    if(!ss.good() || a != '(') { success = false; break; }

    ss >> ws;

    // Get arguement, check for stream error afterwards
    // Initialize data portion to zero
    s.d_vector = Vector3d(0.0, 0.0, 0.0);
    string arguement;
    a = (char)ss.peek();
    if(a == ')') {
      // No arguement
      arguement = "";
    } else if((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z')) {
      // Non-vector arguement
      while(ss.peek() != ')' && ss.good()) {
	ss >> a;
	if(!((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || (a == '-'))) {
	  success = false;
	  break;
	}
	arguement += a;
      }
      if(!ss.good() || success == false) { success = false; break; }
    } else {
      // Vector arguement
      arguement = InstructionSet::DataName;

      // Get first parameter
      ss >> s.d_vector[0];
      ss >> ws;

      ss >> a;
      if(a != ',') { success = false; break; }

      // Get Second parameter
      ss >> ws;
      ss >> s.d_vector[1];

      ss >> a;
      if(a != ',') { success = false; break; }

      // Get Third parameter
      ss >> ws;
      ss >> s.d_vector[2];
    }
    if(!ss.good()) { success = false; break; }
    s.d_arguement = d_is.GetArguementIdByName(arguement);
    if(d_is.IsBadArguementId(s.d_arguement)) { success = false; break; }

    // Get closing paranthesis
    ss >> ws;
    ss >> a;
    if(!ss.good() || a != ')') { success = false; break; }

    // Get semicolon for end of instruction
    ss >> ws;
    ss >> a;
    if(!ss.good() || a != ';') { success = false; break; }

    if(!CreateBasicInstruction(s)) { success = false; break; }
    s.d_instructionLine++;
  }

  if(!ss.good() && !ss.eof()) { success = false; }
  if(success == false) {
    s.d_lineErrors.push_back(s.d_instructionLine);
  }

  return success;
}
