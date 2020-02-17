#include <sstream>

#include "instruction_basic.h"
#include "random.h"
#include "jvector.h"

#include "instruction_set.h"

using namespace std;

map<unsigned int, std::string> InstructionSet::d_instructionsById;
map<string, unsigned int>      InstructionSet::d_instructionsByName;
map<unsigned int, std::string> InstructionSet::d_arguementsById;
map<string, unsigned int>      InstructionSet::d_arguementsByName;

const unsigned int InstructionSet::BadInstructionId        = 0;
const unsigned int InstructionSet::NOP                     = 1;
const unsigned int InstructionSet::POP                     = 2;
const unsigned int InstructionSet::PUSH                    = 3;
const unsigned int InstructionSet::PUSH1                   = 4;
const unsigned int InstructionSet::PUSH2                   = 5;
const unsigned int InstructionSet::PUSH3                   = 6;
const unsigned int InstructionSet::ROTATE                  = 7;
const unsigned int InstructionSet::ADD                     = 8;
const unsigned int InstructionSet::SUB                     = 9;
const unsigned int InstructionSet::MULT                    = 10;
const unsigned int InstructionSet::DIV                     = 11;
const unsigned int InstructionSet::MOD                     = 12;
const unsigned int InstructionSet::SET_SPRING_CONSTANTS    = 13;
const unsigned int InstructionSet::SET_SPRING_FLUCTUATIONS = 14;
const unsigned int InstructionSet::SET_POINT_PARAMETERS    = 15;
const unsigned int InstructionSet::ADD_LINE                = 16;
const unsigned int InstructionSet::REMOVE_LINE             = 17;
const unsigned int InstructionSet::REMOVE_POINT            = 18;
const unsigned int InstructionSet::MERGE                   = 19;
const unsigned int InstructionSet::MOVE_TO                 = 20;
const unsigned int InstructionSet::SPLIT                   = 21;
const unsigned int InstructionSet::SPLIT_DUP               = 22;
const unsigned int InstructionSet::SHIFT_DATA_LEFT         = 23;
const unsigned int InstructionSet::SHIFT_DATA_RIGHT        = 24;
const unsigned int InstructionSet::SWITCH_STACK            = 25;
const unsigned int InstructionSet::PARTIAL                 = 26;
const unsigned int InstructionSet::JUMP                    = 27;
const unsigned int InstructionSet::LABEL                   = 28;
const unsigned int InstructionSet::PUSH_D                  = 29;
const unsigned int InstructionSet::POP_D                   = 30;
const unsigned int InstructionSet::TOP_D                   = 31;
const unsigned int InstructionSet::SPLIT_PARTIAL           = 32;
const unsigned int InstructionSet::SPLIT_ORIENTED          = 33;
const unsigned int InstructionSet::SPLIT_DUP_ORIENTED      = 34;
const unsigned int InstructionSet::SPLIT_PARTIAL_ORIENTED  = 35;
const unsigned int InstructionSet::IF                      = 36;
const unsigned int InstructionSet::ELSE                    = 37;
const unsigned int InstructionSet::END                     = 38;
const unsigned int InstructionSet::LOOP                    = 39;
const unsigned int InstructionSet::PRINT                   = 40;
const unsigned int InstructionSet::SET_DILATION            = 41;
const unsigned int InstructionSet::POP_LINE_D              = 42;
const unsigned int InstructionSet::TOP_LINE_D              = 43;

const unsigned int InstructionSet::BadArguementId      = 0;
const unsigned int InstructionSet::NO_PARAMETER        = 1;
const unsigned int InstructionSet::DATA                = 2;
const unsigned int InstructionSet::STACK_A             = 3;
const unsigned int InstructionSet::STACK_B             = 4;
const unsigned int InstructionSet::STACK_C             = 5;
const unsigned int InstructionSet::STACK_D             = 6;
const unsigned int InstructionSet::GRAVITY             = 7;
const unsigned int InstructionSet::POINT_PARAMETERS    = 8;
const unsigned int InstructionSet::SPRING_CONSTANTS    = 9;
const unsigned int InstructionSet::SPRING_FLUCTUATIONS = 10;
const unsigned int InstructionSet::LOCATION            = 11;
const unsigned int InstructionSet::ORIENTATION_UP      = 12;
const unsigned int InstructionSet::ORIENTATION_RIGHT   = 13;
const unsigned int InstructionSet::ORIENTATION_FORWARD = 14;
const unsigned int InstructionSet::DILATION            = 15;

const string InstructionSet::BadInstructionName("BadInstructionId");
const string InstructionSet::BadArguementName("BadArguementId");

const string InstructionSet::DataName("data");

InstructionSet::InstructionSet(void) {
  MasterGenerate();
  d_instructionsToUse.reserve(d_instructionsById.size()+1);
  d_used.reserve(d_instructionsById.size()+1);

  return;
}

InstructionSet::~InstructionSet(void) {
  return;
}

bool InstructionSet::Initialize(const vector<string> &instructionList) {
  d_used.clear();
  for(unsigned int i=0; i<d_instructionsById.size()+1; i++) {
    d_used.push_back(0);
  }

  vector<string>::const_iterator i;
  for(i=instructionList.begin(); i!=instructionList.end(); ++i) {
    unsigned int id = GetInstructionIdByName(*i);
    if(id == BadInstructionId) {
      return false;
    }
    d_used[id] = 1;
    d_instructionsToUse.push_back(id);
  }

  return true;
}

string InstructionSet::Stringify(const BasicInstruction &bi) {
  stringstream ss;

  ss << GetInstructionName(bi);

  ss << "(";
  if(IsData(bi.ArguementId())) {
    ss << bi.Data()[0] << ", " << bi.Data()[1] << ", " << bi.Data()[2];
  } else {
    ss << GetArguementName(bi);
  }
  ss << ")";

  ss << ";";

  return ss.str();
}

const string &InstructionSet::GetInstructionName(const BasicInstruction &bi) {
  return GetInstructionNameById(bi.InstructionId());
}

const string &InstructionSet::GetInstructionNameById(unsigned int instructionId) {
  map<unsigned int, string>::const_iterator i;
  i = d_instructionsById.find(instructionId);
  if(i == d_instructionsById.end()) {
    return BadInstructionName;
  }

  return i->second;
}

unsigned int InstructionSet::GetInstructionIdByName(const string &name) {
  map<string, unsigned int>::const_iterator i;
  i = d_instructionsByName.find(name);
  if(i == d_instructionsByName.end()) {
    return BadInstructionId;
  }

  return i->second;
}

const string &InstructionSet::GetArguementName(const BasicInstruction &bi) {
  return GetArguementNameById(bi.ArguementId());
}

const string &InstructionSet::GetArguementNameById(unsigned int arguementId) {
  map<unsigned int, string>::const_iterator i;
  i = d_arguementsById.find(arguementId);
  if(i == d_arguementsById.end()) {
    return BadArguementName;
  }

  return i->second;
}

unsigned int InstructionSet::GetArguementIdByName(const string &name) {
  map<string, unsigned int>::const_iterator i;
  i = d_arguementsByName.find(name);
  if(i == d_arguementsByName.end()) {
    return BadArguementId;
  }

  return i->second;
}

bool InstructionSet::Validate(const BasicInstruction &bi) const {
  return Validate(bi.InstructionId(), bi.ArguementId());
}

bool InstructionSet::Validate(unsigned int instructionId,
			      unsigned int arguementId) const {
  if(!d_used[instructionId]) { return false; }

  if(instructionId == BadInstructionId || arguementId == BadArguementId) {
    return false;
  }

  switch(instructionId) {
  case NOP:
  case POP:
  case ELSE:
  case END:
  case PUSH_D:
  case POP_D:
  case TOP_D:
  case POP_LINE_D:
  case TOP_LINE_D:
    if(arguementId != NO_PARAMETER) { return false; }
    break;
  case LABEL:
    if(arguementId != DATA) { return false; }
    break;
  case PUSH:
  case PUSH1:
  case PUSH2:
  case PUSH3:
  case ROTATE:
  case ADD:
  case SUB:
  case MULT:
  case DIV:
  case MOD:
  case SET_SPRING_CONSTANTS:
  case SET_SPRING_FLUCTUATIONS:
  case SET_POINT_PARAMETERS:
  case ADD_LINE:
  case REMOVE_LINE:
  case REMOVE_POINT:
  case MERGE:
  case MOVE_TO:
  case SPLIT:
  case SPLIT_DUP:
  case PARTIAL:
  case JUMP:
  case SPLIT_PARTIAL:
  case SPLIT_ORIENTED:
  case SPLIT_DUP_ORIENTED:
  case SPLIT_PARTIAL_ORIENTED:
  case IF:
  case LOOP:
  case PRINT:
  case SET_DILATION:
    if(arguementId == NO_PARAMETER) { return false; }
    break;
  case SHIFT_DATA_LEFT:
  case SHIFT_DATA_RIGHT:
    if(arguementId != STACK_A &&
       arguementId != STACK_B &&
       arguementId != STACK_C &&
       arguementId != STACK_D &&
       arguementId != NO_PARAMETER) {
      return false;
    }
    break;
  case SWITCH_STACK:
    if(arguementId != STACK_A &&
       arguementId != STACK_B &&
       arguementId != STACK_C &&
       arguementId != STACK_D) {
      return false;
    }
    break;
  default:
    return false;
  };

  return true;
}

Instruction *InstructionSet::RandomInstruction(RandomNumberGenerator &rng,
					       double chanceOfData) const {
  unsigned int which = rng.GetUInt(NumberOfInstructions());
  which = d_instructionsToUse.at(which);

  unsigned int whichArguement = RandomArguement(which, rng, chanceOfData);

  Vector3d data(0.0, 0.0, 0.0);
  if(IsData(whichArguement)) {
    data = RandomData(rng);
  }

  BasicInstruction *ci = new BasicInstruction(which, whichArguement, data);
  if(ci != NULL && !Validate(*ci)) {
    delete ci;
    ci = NULL;
  }

  return (Instruction *)ci;
}

unsigned int InstructionSet::RandomArguement(unsigned int id,
					     RandomNumberGenerator &rng,
					     double chanceOfData) const {
  unsigned int which;

  double chance = rng.GetDouble();
  switch(id) {
  case NOP:
  case POP:
  case PUSH_D:
  case POP_D:
  case TOP_D:
  case POP_LINE_D:
  case TOP_LINE_D:
  case ELSE:
  case END:
    return NO_PARAMETER;
  case LABEL:
    return DATA;
  case PUSH:
  case PUSH1:
  case PUSH2:
  case PUSH3:
  case ROTATE:
  case ADD:
  case SUB:
  case MULT:
  case DIV:
  case MOD:
  case SET_SPRING_CONSTANTS:
  case SET_SPRING_FLUCTUATIONS:
  case SET_POINT_PARAMETERS:
  case ADD_LINE:
  case REMOVE_LINE:
  case REMOVE_POINT:
  case MERGE:
  case MOVE_TO:
  case SPLIT:
  case SPLIT_DUP:
  case JUMP:
  case PARTIAL:
  case SPLIT_PARTIAL:
  case SPLIT_ORIENTED:
  case SPLIT_DUP_ORIENTED:
  case SPLIT_PARTIAL_ORIENTED:
  case IF:
  case LOOP:
  case PRINT:
  case SET_DILATION:
    // Can't have NO_PARAMETER
    if(chance >= chanceOfData) {
      return STACK_A +
	     rng.GetUInt(NumberOfArguements() - 2);
    }
    return DATA;
  case SHIFT_DATA_LEFT:
  case SHIFT_DATA_RIGHT:
    which = rng.GetUInt(5);

    if(which == 0) {
      return STACK_A;
    } else if(which == 1) {
      return STACK_B;
    } else if(which == 2) {
      return STACK_C;
    } else if(which == 3) {
      return STACK_D;
    } else {
      return NO_PARAMETER;
    }
  case SWITCH_STACK:
    which = rng.GetUInt(4);

    if(which == 0) {
      return STACK_A;
    } else if(which == 1) {
      return STACK_B;
    } else if(which == 2) {
      return STACK_C;
    } else {
      return STACK_D;
    }
  }

  return BadArguementId;
}

Vector3d InstructionSet::RandomData(RandomNumberGenerator &rng) const {
  return Vector3d(rng.GetDouble(-1.0, 1.0),
		  rng.GetDouble(-1.0, 1.0),
		  rng.GetDouble(-1.0, 1.0));
}

Instruction *InstructionSet::MutateArguement(const Instruction &i,
					     RandomNumberGenerator &rng,
					     double chanceOfData) const {
  const BasicInstruction &instruction = (const BasicInstruction &)i;
  unsigned int id = instruction.InstructionId();
  unsigned int argId = instruction.ArguementId();
  Vector3d data = instruction.Data();

  // Mutation of a non-data arguement is a random arguement.
  // Mutation of a data arguement has a chance to mutate to a random arguement,
  //   or delta the current value.
  // chanceOfData is the probability of a data arguement for mutation of
  //   non-data arguement, or the chance staying data if it is a data arguement

  double chance = rng.GetDouble();
  if(!IsData(argId) || chance > chanceOfData) {
    // Generate a random arguement
    argId = RandomArguement(id, rng, chanceOfData);
    data = Vector3d(0.0, 0.0, 0.0);
    if(IsData(argId)) {
      data = RandomData(rng);
    }
  } else {
    // Delta a data component
    unsigned int whichComponent = rng.GetUInt(3);
    data[whichComponent] *= pow(1.1, rng.GetDouble(-1.0, 1.0));
    if(data[whichComponent] > 1.0) {
      data[whichComponent] = 1.0;
    } else if(data[whichComponent] < -1.0) {
      data[whichComponent] = -1.0;
    }
  }

  BasicInstruction *ci = new BasicInstruction(id, argId, data);
  if(ci != NULL && !Validate(*ci)) {
    delete ci;
    ci = NULL;
  }

  return (Instruction *)ci;
}

void InstructionSet::MasterGenerate(void) {
  d_instructionsById.insert(make_pair(NOP,              "nop"));
  d_instructionsById.insert(make_pair(POP,              "pop"));
  d_instructionsById.insert(make_pair(PUSH,             "push"));
  d_instructionsById.insert(make_pair(PUSH1,            "push-1"));
  d_instructionsById.insert(make_pair(PUSH2,            "push-2"));
  d_instructionsById.insert(make_pair(PUSH3,            "push-3"));
  d_instructionsById.insert(make_pair(ROTATE,           "rotate"));
  d_instructionsById.insert(make_pair(ADD,              "add"));
  d_instructionsById.insert(make_pair(SUB,              "sub"));
  d_instructionsById.insert(make_pair(MULT,             "mult"));
  d_instructionsById.insert(make_pair(DIV,              "div"));
  d_instructionsById.insert(make_pair(MOD,              "mod"));
  d_instructionsById.insert(make_pair(SET_SPRING_CONSTANTS,
				      "set-spring-constants"));
  d_instructionsById.insert(make_pair(SET_SPRING_FLUCTUATIONS,
				      "set-spring-fluctuations"));
  d_instructionsById.insert(make_pair(SET_POINT_PARAMETERS,
				      "set-point-parameters"));
  d_instructionsById.insert(make_pair(ADD_LINE,         "add-line"));
  d_instructionsById.insert(make_pair(REMOVE_LINE,      "remove-line"));
  d_instructionsById.insert(make_pair(REMOVE_POINT,     "remove-point"));
  d_instructionsById.insert(make_pair(MERGE,            "merge"));
  d_instructionsById.insert(make_pair(MOVE_TO,          "move-to"));
  d_instructionsById.insert(make_pair(SPLIT,            "split"));
  d_instructionsById.insert(make_pair(SPLIT_DUP,        "split-dup"));
  d_instructionsById.insert(make_pair(SHIFT_DATA_LEFT,  "shift-data-left"));
  d_instructionsById.insert(make_pair(SHIFT_DATA_RIGHT, "shift-data-right"));
  d_instructionsById.insert(make_pair(SWITCH_STACK,     "switch-stack"));
  d_instructionsById.insert(make_pair(PARTIAL,          "partial"));
  d_instructionsById.insert(make_pair(JUMP,             "jump"));
  d_instructionsById.insert(make_pair(LABEL,            "label"));
  d_instructionsById.insert(make_pair(PUSH_D,           "push-d"));
  d_instructionsById.insert(make_pair(POP_D,            "pop-d"));
  d_instructionsById.insert(make_pair(TOP_D,            "top-d"));
  d_instructionsById.insert(make_pair(SPLIT_PARTIAL,    "split-partial"));
  d_instructionsById.insert(make_pair(SPLIT_ORIENTED,   "split-oriented"));
  d_instructionsById.insert(make_pair(SPLIT_DUP_ORIENTED,
				      "split-dup-oriented"));
  d_instructionsById.insert(make_pair(SPLIT_PARTIAL_ORIENTED,
				      "split-partial-oriented"));
  d_instructionsById.insert(make_pair(IF,               "if"));
  d_instructionsById.insert(make_pair(ELSE,             "else"));
  d_instructionsById.insert(make_pair(END,              "end"));
  d_instructionsById.insert(make_pair(LOOP,             "loop"));
  d_instructionsById.insert(make_pair(PRINT,            "print"));
  d_instructionsById.insert(make_pair(SET_DILATION,     "set-dilation"));
  d_instructionsById.insert(make_pair(POP_LINE_D,       "pop-line-d"));
  d_instructionsById.insert(make_pair(TOP_LINE_D,       "top-line-d"));

  d_instructionsByName.insert(make_pair("nop",              NOP));
  d_instructionsByName.insert(make_pair("pop",              POP));
  d_instructionsByName.insert(make_pair("push",             PUSH));
  d_instructionsByName.insert(make_pair("push-1",           PUSH1));
  d_instructionsByName.insert(make_pair("push-2",           PUSH2));
  d_instructionsByName.insert(make_pair("push-3",           PUSH3));
  d_instructionsByName.insert(make_pair("rotate",           ROTATE));
  d_instructionsByName.insert(make_pair("add",              ADD));
  d_instructionsByName.insert(make_pair("sub",              SUB));
  d_instructionsByName.insert(make_pair("mult",             MULT));
  d_instructionsByName.insert(make_pair("div",              DIV));
  d_instructionsByName.insert(make_pair("mod",              MOD));
  d_instructionsByName.insert(make_pair("set-spring-constants",
					SET_SPRING_CONSTANTS));
  d_instructionsByName.insert(make_pair("set-spring-fluctuations",
					SET_SPRING_FLUCTUATIONS));
  d_instructionsByName.insert(make_pair("set-point-parameters",
					SET_POINT_PARAMETERS));
  d_instructionsByName.insert(make_pair("add-line",         ADD_LINE));
  d_instructionsByName.insert(make_pair("remove-line",      REMOVE_LINE));
  d_instructionsByName.insert(make_pair("remove-point",     REMOVE_POINT));
  d_instructionsByName.insert(make_pair("merge",            MERGE));
  d_instructionsByName.insert(make_pair("move-to",          MOVE_TO));
  d_instructionsByName.insert(make_pair("split",            SPLIT));
  d_instructionsByName.insert(make_pair("split-dup",        SPLIT_DUP));
  d_instructionsByName.insert(make_pair("shift-data-left",  SHIFT_DATA_LEFT));
  d_instructionsByName.insert(make_pair("shift-data-right", SHIFT_DATA_RIGHT));
  d_instructionsByName.insert(make_pair("switch-stack",     SWITCH_STACK));
  d_instructionsByName.insert(make_pair("partial",          PARTIAL));
  d_instructionsByName.insert(make_pair("jump",             JUMP));
  d_instructionsByName.insert(make_pair("label",            LABEL));
  d_instructionsByName.insert(make_pair("push-d",           PUSH_D));
  d_instructionsByName.insert(make_pair("pop-d",            POP_D));
  d_instructionsByName.insert(make_pair("top-d",            TOP_D));
  d_instructionsByName.insert(make_pair("split-partial",    SPLIT_PARTIAL));
  d_instructionsByName.insert(make_pair("split-oriented",   SPLIT_ORIENTED));
  d_instructionsByName.insert(make_pair("split-dup-oriented",
					SPLIT_DUP_ORIENTED));
  d_instructionsByName.insert(make_pair("split-partial-oriented",
					SPLIT_PARTIAL_ORIENTED));
  d_instructionsByName.insert(make_pair("if",               IF));
  d_instructionsByName.insert(make_pair("else",             ELSE));
  d_instructionsByName.insert(make_pair("end",              END));
  d_instructionsByName.insert(make_pair("loop",             LOOP));
  d_instructionsByName.insert(make_pair("print",            PRINT));
  d_instructionsByName.insert(make_pair("set-dilation",     SET_DILATION));
  d_instructionsByName.insert(make_pair("pop-line-d",            POP_LINE_D));
  d_instructionsByName.insert(make_pair("top-line-d",            TOP_LINE_D));

  d_arguementsById.insert(make_pair(NO_PARAMETER,      ""));
  d_arguementsById.insert(make_pair(DATA,              DataName));
  d_arguementsById.insert(make_pair(STACK_A,           "stack-a"));
  d_arguementsById.insert(make_pair(STACK_B,           "stack-b"));
  d_arguementsById.insert(make_pair(STACK_C,           "stack-c"));
  d_arguementsById.insert(make_pair(STACK_D,           "stack-d"));
  d_arguementsById.insert(make_pair(GRAVITY,           "gravity"));
  d_arguementsById.insert(make_pair(POINT_PARAMETERS,  "point-parameters"));
  d_arguementsById.insert(make_pair(SPRING_CONSTANTS,  "spring-constants"));
  d_arguementsById.insert(make_pair(SPRING_FLUCTUATIONS,
				    "spring-fluctuations"));
  d_arguementsById.insert(make_pair(LOCATION,          "location"));
  d_arguementsById.insert(make_pair(ORIENTATION_UP,    "orientation-up"));
  d_arguementsById.insert(make_pair(ORIENTATION_RIGHT, "orientation-right"));
  d_arguementsById.insert(make_pair(ORIENTATION_FORWARD,
				    "orientation-forward"));
  d_arguementsById.insert(make_pair(DILATION,          "dilation"));

  d_arguementsByName.insert(make_pair("",                  NO_PARAMETER));
  d_arguementsByName.insert(make_pair(DataName,            DATA));
  d_arguementsByName.insert(make_pair("stack-a",           STACK_A));
  d_arguementsByName.insert(make_pair("stack-b",           STACK_B));
  d_arguementsByName.insert(make_pair("stack-c",           STACK_C));
  d_arguementsByName.insert(make_pair("stack-d",           STACK_D));
  d_arguementsByName.insert(make_pair("gravity",           GRAVITY));
  d_arguementsByName.insert(make_pair("point-parameters",  POINT_PARAMETERS));
  d_arguementsByName.insert(make_pair("spring-constants",  SPRING_CONSTANTS));
  d_arguementsByName.insert(make_pair("spring-fluctuations",
				      SPRING_FLUCTUATIONS));
  d_arguementsByName.insert(make_pair("location",          LOCATION));
  d_arguementsByName.insert(make_pair("orientation-up",    ORIENTATION_UP));
  d_arguementsByName.insert(make_pair("orientation-right", ORIENTATION_RIGHT));
  d_arguementsByName.insert(make_pair("orientation-forward",
				      ORIENTATION_FORWARD));
  d_arguementsByName.insert(make_pair("dilation",          DILATION));

  return;
}
