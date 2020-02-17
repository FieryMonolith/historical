#include <cstdlib>
#include <cmath>
#include <iostream>

#include "genome.h"
#include "body.h"
#include "physical_point.h"
#include "physical_point_info.h"
#include "spring.h"
#include "spring_info.h"
#include "instruction_basic.h"
#include "environment_data.h"
#include "physics_settings.h"

#include "spider_basic.h"

#include "instruction_set.h"

using namespace std;

const Vector3d SpiderBasic::d_null = Vector3d(0.0, 0.0, 0.0);
const Vector3d SpiderBasic::d_zero = Vector3d(0.0, 0.0, 0.0);
const unsigned int SpiderBasic::ControlData::d_maxIterations = 10;
const unsigned int SpiderBasic::BasicState::d_maxStackDepth  = 20;

SpiderBasic::SpiderBasic(void) {
  return;
}

SpiderBasic::~SpiderBasic(void) {
  return;
}

Spider::State *SpiderBasic::NewState(void) const {
  return new BasicState;
}

// Called from Spider::Run to initialize a new state.
bool SpiderBasic::Initialize(State &state, EnvironmentData &ed) const {
  PhysicsSettings &ps = ed.GetPhysicsSettings();

  BasicState &s = (BasicState &)state;
  s.d_currentPoint = 0;

  s.d_currentStack = 0;
  s.d_stacks.resize(SpiderBasic::d_numberOfStacks);

  for(unsigned int i=0; i<SpiderBasic::d_numberOfStacks; ++i) {
    s.d_stacks[i].push_back(d_zero);
  }

  s.d_springConstants[0] = ps.KS();
  s.d_springFluctuations[0] = ps.FluctuationDelta();
  s.d_springFluctuations[1] = ps.FluctuationStart();
  s.d_springFluctuations[2] = ps.FluctuationRate();

  s.d_pointParameters[0] = ps.Mass();

  s.d_gravity[0] = ps.GravityX();
  s.d_gravity[1] = ps.GravityY();
  s.d_gravity[2] = ps.GravityZ();

  s.d_dilation[0] = 1.0;
  s.d_dilation[1] = 1.0;
  s.d_dilation[2] = 1.0;

  s.d_partial[0] = 1.0;
  s.d_partial[1] = 0.0;
  s.d_partial[2] = 0.0;

  // Push the start location/orientation onto the stack because the
  // last element on the stack will never be removed.
  s.d_spacialDescription = SpacialDescription(Vector3d(0,0,0), // location
					      Vector3d(1,0,0), // right
					      Vector3d(0,1,0), // up
					      Vector3d(0,0,-1)); //forward
  s.d_spacialDescriptionStack.push_back(s.d_spacialDescription);

  return true;
}

bool SpiderBasic::Complete(State &state) const {
  BasicState &s = (BasicState &)state;

  bool success = true;
  // Clean up frames
  vector<Frame *>::iterator i;
  for(i = s.d_frames.begin(); i != s.d_frames.end(); ++i) {
    if(*i == NULL) { success = false; continue; }
    delete *i;
    *i = NULL;
  }
  s.d_frames.clear();

  // Clean up frame stack
  for(i = s.d_frameStack.begin(); i != s.d_frameStack.end(); ++i) {
    if(*i == NULL) { success = false; continue; }
    delete *i;
    *i = NULL;
  }
  s.d_frameStack.clear();

  s.d_frameMap.clear();

  vector<Instruction *>::iterator ii;
  for(ii = s.d_myInstructions.begin(); ii != s.d_myInstructions.end(); ++ii) {
    if(*ii == NULL) { success = false; continue; }
    delete *ii;
    *ii = NULL;
  }
  s.d_myInstructions.clear();

  return success;
}

bool SpiderBasic::Run(State &state, const Genome &g, Body &b,
		      EnvironmentData &ed) const {
  BasicState &s = (BasicState &)state;
  const vector<Instruction *> &instructions = g.Instructions();

  if(!CreateFrames(s, instructions)) { return false; }

  // Create the process control blocks for each frame
  vector<Frame *>::iterator frames;
  for(frames = s.d_frames.begin(); frames != s.d_frames.end(); ++frames) {
    if(*frames == NULL) { return false; }

    if(!ProcessControlBlocks(**frames))  { return false; }
  }

  // Put "main" frame onto the stack, should be the only one at first
  Frame *mainFrame = new Frame(*(s.d_frames.front()));
  if(mainFrame == NULL) { return false; }
  s.d_frameStack.push_back(mainFrame);
  Frame *currentFrame = s.d_frameStack.back();
  if(currentFrame == NULL) { return false; }

  while(!s.d_frameStack.empty() && s.d_frameStack.size() <= s.d_maxStackDepth) {
    unsigned int &line = currentFrame->d_currentLine;

    // Check to see if we are done with a frame
    if(line >= currentFrame->d_instructions.size()) {
      delete currentFrame;
      s.d_frameStack.pop_back();
      // currentFrame will equal s.d_frameStack.end() if stack is empty
      if(!s.d_frameStack.empty()) {
        currentFrame = s.d_frameStack.back();
      }
      continue;
    }

    if(currentFrame->d_instructions.at(line) == NULL) {
      return false;
    }

    Instruction &instruction = *(currentFrame->d_instructions.at(line));

    int value = Execute(s, instruction, b, ed);
    if(value) { return false; }

    // Increment the line number in the current frame
    line++;

    // If a jump command was just executed, set the current frame to the
    // frame that was just pushed on the frame stack.
    if(instruction.InstructionId() == InstructionSet::JUMP) {
      currentFrame = s.d_frameStack.back();
    }
  }

  return true;
}

bool SpiderBasic::CreateFrames(BasicState &s,
			       const vector<Instruction *> &instructions) const
{
  Frame *f = new Frame;
  if(f == NULL) { return false; }
  f->d_currentLine = 0;
  // The first frame is the "main" frame and is not associated with a
  // label and thus does not go into the map
  s.d_frames.push_back(f);

  // Create an empty frame if there are no instructions
  if(instructions.empty()) {
    return true;
  }

  // Process labels
  vector<Instruction *>::const_iterator ii;
  for(ii = instructions.begin(); ii != instructions.end(); ++ii) {
    if(*ii == NULL) { return false; }

    // A LABEL instruction is now, no where in the frame code
    if((*ii)->InstructionId() != InstructionSet::LABEL) {
      f->d_instructions.push_back(*ii);
    } else {
      Vector3d id = GetParameter(s, (const BasicInstruction &)(**ii));

      // No comparison operator for Vector3d, so do it the hard way
      map<Vector3d, Frame *, DontcareCompare>::iterator ifmap;
      for(ifmap = s.d_frameMap.begin(); ifmap != s.d_frameMap.end(); ++ifmap) {
	if(ifmap->first == id) { break; }
      }
      // Can't have duplicate labels, so take the last one
      if(ifmap != s.d_frameMap.end()) {
	// Has a frame with current id
	f = ifmap->second;
	f->d_instructions.clear();
      } else {
	// Didn't find a frame with that id, so add one
	f = new Frame;
	if(f == NULL) { return false; }
	f->d_currentLine = 0;
	f->d_id = id;

	s.d_frames.push_back(f);
	s.d_frameMap.insert(make_pair(id, f));
      }
    }
  }

  // Check to see if "main" is empty, if it is, add a jump to the first label.
  // There will always be one label if "main" is empty, or it would have 
  // been taken care of above.
  if(s.d_frames.front()->d_instructions.empty()) {
    Vector3d id = s.d_frames.at(1)->d_id;
    Instruction *bi = new BasicInstruction(InstructionSet::JUMP,
					   InstructionSet::DATA,
					   id);
    if(bi == NULL) { return false; }
    s.d_frames.front()->d_instructions.push_back(bi);
    s.d_myInstructions.push_back(bi);
  }

  return true;
}

bool SpiderBasic::ProcessControlBlocks(Frame &frame) const {
  vector<ControlLines> controlStack;

  // Process For Control Structures
  for(unsigned int i=0; i<frame.d_instructions.size(); i++) {
    if(frame.d_instructions.at(i) == NULL) { return false; }
    const BasicInstruction &bi =
      (const BasicInstruction &)(*(frame.d_instructions.at(i)));

    unsigned int id = bi.InstructionId();

    if(id == InstructionSet::IF) {
      controlStack.push_back(ControlLines(id, i));
    } else if(id == InstructionSet::ELSE) {
      if(controlStack.empty()) { continue; }

      if(controlStack.back().type == InstructionSet::IF) {
	controlStack.back().type = id;
	controlStack.back().b = i;
      }
    } else if(id == InstructionSet::LOOP) {
      controlStack.push_back(ControlLines(id, i));
    } else if(id == InstructionSet::END) {
      if(controlStack.empty()) { continue; }

      ControlLines cl = controlStack.back();
      controlStack.pop_back();

      ControlData cd;
      cd.d_currentIteration = 0;

      // All line numbers are for the control instruction because after
      // execution, the instruction execution loop will increment the line
      // number by one.
      unsigned int type = cl.type;
      if(type == InstructionSet::IF || type == InstructionSet::LOOP) {
	// Add control structure for end
	cd.d_start = cl.a - 1;
	cd.d_end = i;
	cd.d_type = type;
	frame.d_controlData.insert(make_pair(i, cd));

	// type remains the same
	cd.d_end = i;
      } else if(type == InstructionSet::ELSE) {
	// Add control structure for end
	cd.d_start = cl.b;
	cd.d_end = i;
	cd.d_type = type;
	frame.d_controlData.insert(make_pair(i, cd));

	// Insert the else control structure
	cd.d_start = cl.b;
	cd.d_end = i;
	cd.d_type = type;
	frame.d_controlData.insert(make_pair(cd.d_start, cd));

	// Prepare for insertion of the if control structure
	cd.d_end = cl.b;
	cd.d_type = InstructionSet::IF;
      } else {
	return false;
      }

      cd.d_start = cl.a;
      frame.d_controlData.insert(make_pair(cd.d_start, cd));
    }
  }

  return true;
}

int SpiderBasic::Execute(State &state, const Instruction &instruction,
			 Body &body, EnvironmentData &ed) const {
  const BasicInstruction &i = (const BasicInstruction &)instruction;
  BasicState &s = (BasicState &)state;

  int fail = 0;
  unsigned int id = i.InstructionId();
  if(id == InstructionSet::NOP) {
    fail = nop();
  } else if (id == InstructionSet::POP) {
    fail = pop(s);
  } else if (id == InstructionSet::PUSH) {
    fail = push(s, i);
  } else if (id == InstructionSet::PUSH1) {
    fail = push_1(s, i);
  } else if (id == InstructionSet::PUSH2) {
    fail = push_2(s, i);
  } else if (id == InstructionSet::PUSH3) {
    fail = push_3(s, i);
  } else if (id == InstructionSet::SWITCH_STACK) {
    fail = switch_stack(s, i);
  } else if (id == InstructionSet::SHIFT_DATA_LEFT) {
    fail = shift_data_left(s, i);
  } else if (id == InstructionSet::SHIFT_DATA_RIGHT) {
    fail = shift_data_right(s, i);
  } else if (id == InstructionSet::ROTATE) {
    fail = rotate(s, i);
  } else if (id == InstructionSet::ADD) {
    fail = add(s, i);
  } else if (id == InstructionSet::SUB) {
    fail = sub(s, i);
  } else if (id == InstructionSet::MULT) {
    fail = mult(s, i);
  } else if (id == InstructionSet::DIV) {
    fail = div(s, i);
  } else if (id == InstructionSet::MOD) {
    fail = mod(s, i);
  } else if (id == InstructionSet::SET_SPRING_CONSTANTS) {
    fail = set_spring_constants(s, i);
  } else if (id == InstructionSet::SET_SPRING_FLUCTUATIONS) {
    fail = set_spring_fluctuations(s, i);
  } else if (id == InstructionSet::SET_POINT_PARAMETERS) {
    fail = set_point_constants(s, i);
  } else if (id == InstructionSet::SPLIT) {
    fail = split(s, i, body, ed);
  } else if (id == InstructionSet::SPLIT_DUP) {
    fail = split_dup(s, i, body, ed);
  } else if (id == InstructionSet::ADD_LINE) {
    fail = add_line(s, i, body, ed);
  } else if (id == InstructionSet::REMOVE_LINE) {
    fail = remove_line(s, i, body, ed);
  } else if (id == InstructionSet::REMOVE_POINT) {
    fail = remove_point(s, i, body, ed);
  } else if (id == InstructionSet::MERGE) {
    fail = merge(s, i, body, ed);
  } else if (id == InstructionSet::MOVE_TO) {
    fail = move_to(s, i, body);
  } else if (id == InstructionSet::LABEL) {
    fail = label(s, i, body);
  } else if (id == InstructionSet::JUMP) {
    fail = jump(s, i, body);
  } else if (id == InstructionSet::PARTIAL) {
    fail = partial(s, i, body);
  } else if (id == InstructionSet::PUSH_D) {
    fail = push_d(s, i, body);
  } else if (id == InstructionSet::POP_D) {
    fail = pop_d(s, i, body);
  } else if (id == InstructionSet::TOP_D) {
    fail = top_d(s, i, body);
  } else if (id == InstructionSet::POP_LINE_D) {
    fail = pop_line_d(s, i, body, ed);
  } else if (id == InstructionSet::TOP_LINE_D) {
    fail = top_line_d(s, i, body, ed);
  } else if (id == InstructionSet::SPLIT_PARTIAL) {
    fail = split_partial(s, i, body, ed);
  } else if (id == InstructionSet::SPLIT_ORIENTED) {
    fail = split_oriented(s, i, body, ed);
  } else if (id == InstructionSet::SPLIT_DUP_ORIENTED) {
    fail = split_dup_oriented(s, i, body, ed);
  } else if (id == InstructionSet::SPLIT_PARTIAL_ORIENTED) {
    fail = split_partial_oriented(s, i, body, ed);
  } else if (id == InstructionSet::IF) {
    fail = control_if(s, i, body);
  } else if (id == InstructionSet::ELSE) {
    fail = control_else(s, i, body);
  } else if (id == InstructionSet::LOOP) {
    fail = control_loop(s, i, body);
  } else if (id == InstructionSet::END) {
    fail = control_end(s, i, body);
  } else if (id == InstructionSet::PRINT) {
    fail = print(s, i, body);
  } else if (id == InstructionSet::SET_DILATION) {
    fail = set_dilation(s, i);
  } else {
    fail = -2;
  }

  return fail;
}

Vector3d SpiderBasic::GetParameter(BasicState &s,
				   const BasicInstruction &i) const {
  unsigned int id = i.ArguementId();
  Vector3d x(d_null);

  if(id == InstructionSet::NO_PARAMETER) {
    x = d_null;
  } else if(id == InstructionSet::DATA) {
    x = i.Data();
  } else if(id == InstructionSet::STACK_A) {
    x = s.d_stacks[0].back();
  } else if(id == InstructionSet::STACK_B) {
    x = s.d_stacks[1].back();
  } else if(id == InstructionSet::STACK_C) {
    x = s.d_stacks[2].back();
  } else if(id == InstructionSet::STACK_D) {
    x = s.d_stacks[3].back();
  } else if(id == InstructionSet::GRAVITY) {
    x = s.d_gravity;
    Normalize(x);
  } else if(id == InstructionSet::POINT_PARAMETERS) {
    x = s.d_pointParameters;
  } else if(id == InstructionSet::SPRING_CONSTANTS) {
    x = s.d_springConstants;
  } else if(id == InstructionSet::SPRING_FLUCTUATIONS) {
    x = s.d_springFluctuations;
  } else if(id == InstructionSet::LOCATION) {
    x = s.d_spacialDescription.Position();
  } else if(id == InstructionSet::ORIENTATION_UP) {
    x = s.d_spacialDescription.Up();
  } else if(id == InstructionSet::ORIENTATION_RIGHT) {
    x = s.d_spacialDescription.Right();
  } else if(id == InstructionSet::ORIENTATION_FORWARD) {
    x = s.d_spacialDescription.Forward();
  } else if(id == InstructionSet::DILATION) {
    x = s.d_dilation;
  }

  return x;
}

int SpiderBasic::nop(void) const {
  return 0;
}

int SpiderBasic::pop(BasicState &s) const {
  s.d_stacks[s.d_currentStack].pop_back();
  if(s.d_stacks[s.d_currentStack].empty()) {
    s.d_stacks[s.d_currentStack].push_back(d_zero);
  }

  return 0;
}

int SpiderBasic::push(BasicState &s,
		      const BasicInstruction &instruction) const {
  s.d_stacks[s.d_currentStack].push_back(GetParameter(s, instruction));

  return 0;
}

int SpiderBasic::push_1(BasicState &s,
			const BasicInstruction &instruction) const {
  Vector3d v(GetParameter(s, instruction));
  v[1] = v[2] = 0;
  s.d_stacks[s.d_currentStack].push_back(v);

  return 0;
}

int SpiderBasic::push_2(BasicState &s,
			const BasicInstruction &instruction) const {
  Vector3d v(GetParameter(s, instruction));
  v[0] = v[2] = 0;
  s.d_stacks[s.d_currentStack].push_back(v);

  return 0;
}

int SpiderBasic::push_3(BasicState &s,
			const BasicInstruction &instruction) const {
  Vector3d v(GetParameter(s, instruction));
  v[0] = v[1] = 0;
  s.d_stacks[s.d_currentStack].push_back(v);

  return 0;
}

int SpiderBasic::switch_stack(BasicState &s,
			      const BasicInstruction &instruction) const {
  unsigned int id = instruction.ArguementId();
  if(id == InstructionSet::STACK_A) {
    s.d_currentStack = 0;
  } else if(id == InstructionSet::STACK_B) {
    s.d_currentStack = 1;
  } else if(id == InstructionSet::STACK_C) {
    s.d_currentStack = 2;
  } else if(id == InstructionSet::STACK_D) {
    s.d_currentStack = 3;
  }

  return 0;
}

int SpiderBasic::shift_data_left(BasicState &s,
				 const BasicInstruction &instruction) const {
  unsigned int id = instruction.ArguementId();
  if(id != InstructionSet::NO_PARAMETER) {
    id = s.d_currentStack;
  } else if(id != InstructionSet::STACK_A) {
    id = 0;
  } else if(id != InstructionSet::STACK_B) {
    id = 1;
  } else if(id != InstructionSet::STACK_C) {
    id = 2;
  } else if(id != InstructionSet::STACK_D) {
    id = 3;
  } else {
    return 0;
  }

  Vector3d &v = s.d_stacks[id].back();

  double temp = v[0];
  v[0] = v[1];
  v[1] = v[2];
  v[2] = temp;

  return 0;
}

int SpiderBasic::shift_data_right(BasicState &s,
				  const BasicInstruction &instruction) const {
  unsigned int id = instruction.ArguementId();
  if(id != InstructionSet::NO_PARAMETER) {
    id = s.d_currentStack;
  } else if(id != InstructionSet::STACK_A) {
    id = 0;
  } else if(id != InstructionSet::STACK_B) {
    id = 1;
  } else if(id != InstructionSet::STACK_C) {
    id = 2;
  } else if(id != InstructionSet::STACK_D) {
    id = 3;
  } else {
    return 0;
  }

  Vector3d &v = s.d_stacks[id].back();

  double temp = v[2];
  v[2] = v[1];
  v[1] = v[0];
  v[0] = temp;

  return 0;
}

int SpiderBasic::rotate(BasicState &s,
			const BasicInstruction &instruction) const {
  // Angle rotation is between -1 and 1
  // 0 means no rotation, 1 means 2*PI, so you only need the fractional
  // portion because each whole number represents a full revolution.
  RotateSpider(GetParameter(s, instruction), s);

  return 0;
}

void SpiderBasic::RotateSpider(const Vector3d &rotations,
			       BasicState &s) const {
  Vector3d scaledRotations(rotations);
  scaledRotations *= (M_PI * 2.0);

  s.d_spacialDescription.YawRight(scaledRotations[0]);
  s.d_spacialDescription.PitchUp(scaledRotations[1]);
  s.d_spacialDescription.RollLeft(scaledRotations[2]);

  return;
}

int SpiderBasic::add(BasicState &s,
		     const BasicInstruction &instruction) const {
  Vector3d v(GetParameter(s, instruction));
  unsigned int i = s.d_currentStack;

  s.d_stacks[i].back() += v;

  return 0;
}

int SpiderBasic::sub(BasicState &s,
		     const BasicInstruction &instruction) const {
  Vector3d v(GetParameter(s, instruction));
  unsigned int i = s.d_currentStack;

  s.d_stacks[i].back() -= v;

  return 0;
}

int SpiderBasic::mult(BasicState &s,
		      const BasicInstruction &instruction) const {
  Vector3d v(GetParameter(s, instruction));
  unsigned int i = s.d_currentStack;

  s.d_stacks[i].back()[0] = s.d_stacks[i].back()[0] * v[0];
  s.d_stacks[i].back()[1] = s.d_stacks[i].back()[1] * v[1];
  s.d_stacks[i].back()[2] = s.d_stacks[i].back()[2] * v[2];

  return 0;
}

int SpiderBasic::div(BasicState &s,
		     const BasicInstruction &instruction) const {
  Vector3d v(GetParameter(s, instruction));
  unsigned int i = s.d_currentStack;

  if(v[0] == 0) {
    s.d_stacks[i].back()[0] = 0;
  } else {
    s.d_stacks[i].back()[0] = s.d_stacks[i].back()[0] / v[0];
  }

  if(v[1] == 0) {
    s.d_stacks[i].back()[1] = 0;
  } else {
    s.d_stacks[i].back()[1] = s.d_stacks[i].back()[1] / v[1];
  }

  if(v[2] == 0) {
    s.d_stacks[i].back()[2] = 0;
  } else {
    s.d_stacks[i].back()[2] = s.d_stacks[i].back()[2] / v[2];
  }

  return 0;
}

int SpiderBasic::mod(BasicState &s,
		     const BasicInstruction &instruction) const {
  Vector3d v(GetParameter(s, instruction));
  unsigned int i = s.d_currentStack;

  if(v[0] == 0) {
    s.d_stacks[i].back()[0] = 0;
  } else {
    s.d_stacks[i].back()[0] = fmod(s.d_stacks[i].back()[0],v[0]);
  }

  if(v[1] == 0) {
    s.d_stacks[i].back()[1] = 0;
  } else {
    s.d_stacks[i].back()[1] = fmod(s.d_stacks[i].back()[1],v[1]);
  }

  if(v[2] == 0) {
    s.d_stacks[i].back()[2] = 0;
  } else {
    s.d_stacks[i].back()[2] = fmod(s.d_stacks[i].back()[2],v[2]);
  }

  return 0;
}

int SpiderBasic::set_spring_constants(BasicState &s,
				   const BasicInstruction &instruction) const {
  return 0;
  s.d_springConstants = GetParameter(s, instruction);

  return 0;
}

int SpiderBasic::set_spring_fluctuations(BasicState &s,
				   const BasicInstruction &instruction) const {
  s.d_springFluctuations = GetParameter(s, instruction);
  Constrain(s.d_springFluctuations);

  return 0;
}

int SpiderBasic::set_point_constants(BasicState &s,
				   const BasicInstruction &instruction) const {
  return 0;
  s.d_pointParameters = GetParameter(s, instruction);

  return 0;
}

int SpiderBasic::set_dilation(BasicState &s,
			      const BasicInstruction &instruction) const {
  s.d_dilation = GetParameter(s, instruction);

  return 0;
}

int SpiderBasic::split(BasicState &s,
		       const BasicInstruction &instruction,
		       Body &body, EnvironmentData &ed) const {
  int64 id1 = s.d_currentPoint;
  PhysicalPointInfo *current = body.GetPoint(id1);
  PhysicalPointInfo *p = new_point_independent(s, instruction, body, ed);

  if(id1 == 0 && p != NULL) { return 0; }
  if(p == NULL || current == NULL) { return -3; }

  int64 springId;
  if(new_spring(s, body, springId, *current, *p, ed) == NULL) {
    return -4;
  }

  MoveSpider(s, body);

  return 0;
}

int SpiderBasic::split_dup(BasicState &s,
			   const BasicInstruction &instruction,
			   Body &body, EnvironmentData &ed) const {
  int64 id1 = s.d_currentPoint;
  PhysicalPointInfo *current = body.GetPoint(id1);
  PhysicalPointInfo *p = new_point_independent(s, instruction, body, ed);

  if(id1 == 0 && p != NULL) { return 0; }
  if(p == NULL || current == NULL) { return -3; }

  vector<pair<int64, int64> >temp;
  const map<int64, Body::ConnectionData> &lines = body.Connections();
  map<int64, Body::ConnectionData>::const_iterator line;
  for(line = lines.begin(); line != lines.end(); ++line) {
    if(line->second.d_point1 == NULL || line->second.d_point2 == NULL) {
      continue;
    }

    int64 t1 = line->second.d_point1->Id();
    int64 t2 = line->second.d_point2->Id();
    int64 t;
    if(t1 == id1) {
      t = t2;
    } else if(t2 == id1) {
      t = t1;
    } else {
      continue;
    }

    temp.push_back(make_pair(line->first, t));
  }

  vector<pair<int64, int64> >::iterator ti;
  for(ti=temp.begin(); ti!=temp.end(); ++ti) {
    int64 springId = ti->first;
    int64 t = ti->second;

    PhysicalPointInfo *p2 = body.GetPoint(t);
    if(p2 == NULL) { return -3; }

    int64 tId;
    if(copy_spring(body, tId, springId, *p, *p2, ed) == NULL) {
      return -3;
    }
  }

  int64 springId;
  if(new_spring(s, body, springId, *current, *p, ed) == NULL) {
    return -3;
  }

  MoveSpider(s, body);

  return 0;
}

int SpiderBasic::split_partial(BasicState &s,
			       const BasicInstruction &instruction,
			       Body &body, EnvironmentData &ed) const {
  int64 id1 = s.d_currentPoint;
  PhysicalPointInfo *current = body.GetPoint(id1);
  PhysicalPointInfo *p = new_point_independent(s, instruction, body, ed);

  if(id1 == 0 && p != NULL) { return 0; }
  if(p == NULL || current == NULL) { return -3; }

  if(s.d_partial[0] > 1 || s.d_partial[1] > 1 || s.d_partial[2] > 1) {
    return -4;
  }

  vector<pair<int64, int64> >temp;
  const map<int64, Body::ConnectionData> &lines = body.Connections();
  map<int64, Body::ConnectionData>::const_iterator line;

  unsigned int quantityBeginning = 0;
  if(s.d_partial[0] > 0) {
    quantityBeginning = (unsigned int)((double)lines.size() * s.d_partial[0]);
  }
  unsigned int quantityMiddle = 0;
  if(s.d_partial[1] > 0) {
    quantityMiddle = (unsigned int)((double)lines.size() * s.d_partial[1]);
  }
  unsigned int quantityEnd = 0;
  if(s.d_partial[2] > 0) {
    quantityEnd = (unsigned int)((double)lines.size() * s.d_partial[2]);
  }

  unsigned int endBeginning = quantityBeginning;

  unsigned int middle = (lines.size() / 2);
  if(lines.size() > 0 && lines.size() % 2 == 0) { middle--; }
  unsigned int beginMiddle = middle - (quantityMiddle / 2);
  if(quantityMiddle > 0 && quantityMiddle % 2 == 0) { beginMiddle++; }
  unsigned int endMiddle = beginMiddle + quantityMiddle;

  if(quantityMiddle == 0 || endBeginning >= endMiddle) {
    beginMiddle = endBeginning;
    endMiddle = endBeginning;
  } else if(endBeginning > beginMiddle) {
    beginMiddle = endBeginning;
  }

  unsigned int beginEnd = lines.size() - quantityEnd;
  if(beginEnd <= endBeginning) {
    endBeginning = beginEnd;
    beginMiddle = beginEnd;
    endMiddle = beginEnd;
  } else if(beginEnd <= beginMiddle) {
    beginMiddle = beginEnd;
    endMiddle = beginEnd;
  } else if(beginEnd < endMiddle) {
    endMiddle = beginEnd;
  }

  unsigned int index = 0;
  line = lines.begin();
  for(; line != lines.end() && index < endBeginning; ++line, index++) {
    if(line->second.d_point1 == NULL || line->second.d_point2 == NULL) {
      continue;
    }

    int64 t1 = line->second.d_point1->Id();
    int64 t2 = line->second.d_point2->Id();
    int64 t;
    if(t1 == id1) {
      t = t2;
    } else if(t2 == id1) {
      t = t1;
    } else {
      continue;
    }

    temp.push_back(make_pair(line->first, t));
  }

  for(; line != lines.end() && index < beginMiddle; ++line, index++);

  for(; line != lines.end() && index < endMiddle; ++line, index++) {
    if(line->second.d_point1 == NULL || line->second.d_point2 == NULL) {
      continue;
    }

    int64 t1 = line->second.d_point1->Id();
    int64 t2 = line->second.d_point2->Id();
    int64 t;
    if(t1 == id1) {
      t = t2;
    } else if(t2 == id1) {
      t = t1;
    } else {
      continue;
    }

    temp.push_back(make_pair(line->first, t));
  }

  for(; line != lines.end() && index < beginEnd; ++line, index++);

  for(; line != lines.end(); ++line) {
    if(line->second.d_point1 == NULL || line->second.d_point2 == NULL) {
      continue;
    }

    int64 t1 = line->second.d_point1->Id();
    int64 t2 = line->second.d_point2->Id();
    int64 t;
    if(t1 == id1) {
      t = t2;
    } else if(t2 == id1) {
      t = t1;
    } else {
      continue;
    }

    temp.push_back(make_pair(line->first, t));
  }

  vector<pair<int64, int64> >::iterator ti;
  for(ti=temp.begin(); ti!=temp.end(); ++ti) {
    int64 springId = ti->first;
    int64 t = ti->second;

    PhysicalPointInfo *p2 = body.GetPoint(t);
    if(p2 == NULL) { return -3; }

    int64 tId;
    if(copy_spring(body, tId, springId, *p, *p2, ed) == NULL) {
      return -3;
    }
  }

  int64 springId;
  if(new_spring(s, body, springId, *current, *p, ed) == NULL) {
    return -3;
  }

  MoveSpider(s, body);

  return 0;
}

int SpiderBasic::split_oriented(BasicState &s,
				const BasicInstruction &instruction,
				Body &body, EnvironmentData &ed) const {
  int64 id1 = s.d_currentPoint;
  PhysicalPointInfo *current = body.GetPoint(id1);
  PhysicalPointInfo *p = new_point_oriented(s, instruction, body, ed);

  if(id1 == 0 && p != NULL) { return 0; }
  if(p == NULL || current == NULL) { return -3; }

  int64 springId;
  if(new_spring(s, body, springId, *current, *p, ed) == NULL) {
    return -4;
  }

  MoveSpider(s, body);

  return 0;
}

int SpiderBasic::split_dup_oriented(BasicState &s,
				    const BasicInstruction &instruction,
				    Body &body, EnvironmentData &ed) const {
  int64 id1 = s.d_currentPoint;
  PhysicalPointInfo *current = body.GetPoint(id1);
  PhysicalPointInfo *p = new_point_oriented(s, instruction, body, ed);

  if(id1 == 0 && p != NULL) { return 0; }
  if(p == NULL || current == NULL) { return -3; }

  vector<pair<int64, int64> >temp;
  const map<int64, Body::ConnectionData> &lines = body.Connections();
  map<int64, Body::ConnectionData>::const_iterator line;
  for(line = lines.begin(); line != lines.end(); ++line) {
    if(line->second.d_point1 == NULL || line->second.d_point2 == NULL) {
      continue;
    }

    int64 t1 = line->second.d_point1->Id();
    int64 t2 = line->second.d_point2->Id();
    int64 t;
    if(t1 == id1) {
      t = t2;
    } else if(t2 == id1) {
      t = t1;
    } else {
      continue;
    }

    temp.push_back(make_pair(line->first, t));
  }

  vector<pair<int64, int64> >::iterator ti;
  for(ti=temp.begin(); ti!=temp.end(); ++ti) {
    int64 springId = ti->first;
    int64 t = ti->second;

    PhysicalPointInfo *p2 = body.GetPoint(t);
    if(p2 == NULL) { return -3; }

    int64 tId;
    if(copy_spring(body, tId, springId, *p, *p2, ed) == NULL) {
      return -3;
    }
  }

  int64 springId;
  if(new_spring(s, body, springId, *current, *p, ed) == NULL) {
    return -3;
  }

  MoveSpider(s, body);

  return 0;
}

int SpiderBasic::split_partial_oriented(BasicState &s,
					const BasicInstruction &instruction,
					Body &body, EnvironmentData &ed) const{
  int64 id1 = s.d_currentPoint;
  PhysicalPointInfo *current = body.GetPoint(id1);
  PhysicalPointInfo *p = new_point_oriented(s, instruction, body, ed);

  if(id1 == 0 && p != NULL) { return 0; }
  if(p == NULL || current == NULL) { return -3; }

  if(s.d_partial[0] > 1 || s.d_partial[1] > 1 || s.d_partial[2] > 1) {
    return -4;
  }

  vector<pair<int64, int64> >temp;
  const map<int64, Body::ConnectionData> &lines = body.Connections();
  map<int64, Body::ConnectionData>::const_iterator line;

  unsigned int quantityBeginning = 0;
  if(s.d_partial[0] > 0) {
    quantityBeginning = (unsigned int)((double)lines.size() * s.d_partial[0]);
  }
  unsigned int quantityMiddle = 0;
  if(s.d_partial[1] > 0) {
    quantityMiddle = (unsigned int)((double)lines.size() * s.d_partial[1]);
  }
  unsigned int quantityEnd = 0;
  if(s.d_partial[2] > 0) {
    quantityEnd = (unsigned int)((double)lines.size() * s.d_partial[2]);
  }

  unsigned int endBeginning = quantityBeginning;

  unsigned int middle = (lines.size() / 2);
  if(lines.size() > 0 && lines.size() % 2 == 0) { middle--; }
  unsigned int beginMiddle = middle - (quantityMiddle / 2);
  if(quantityMiddle > 0 && quantityMiddle % 2 == 0) { beginMiddle++; }
  unsigned int endMiddle = beginMiddle + quantityMiddle;

  if(quantityMiddle == 0 || endBeginning >= endMiddle) {
    beginMiddle = endBeginning;
    endMiddle = endBeginning;
  } else if(endBeginning > beginMiddle) {
    beginMiddle = endBeginning;
  }

  unsigned int beginEnd = lines.size() - quantityEnd;
  if(beginEnd <= endBeginning) {
    endBeginning = beginEnd;
    beginMiddle = beginEnd;
    endMiddle = beginEnd;
  } else if(beginEnd <= beginMiddle) {
    beginMiddle = beginEnd;
    endMiddle = beginEnd;
  } else if(beginEnd < endMiddle) {
    endMiddle = beginEnd;
  }

  unsigned int index = 0;
  line = lines.begin();
  for(; line != lines.end() && index < endBeginning; ++line, index++) {
    if(line->second.d_point1 == NULL || line->second.d_point2 == NULL) {
      continue;
    }

    int64 t1 = line->second.d_point1->Id();
    int64 t2 = line->second.d_point2->Id();
    int64 t;
    if(t1 == id1) {
      t = t2;
    } else if(t2 == id1) {
      t = t1;
    } else {
      continue;
    }

    temp.push_back(make_pair(line->first, t));
  }

  for(; line != lines.end() && index < beginMiddle; ++line, index++);

  for(; line != lines.end() && index < endMiddle; ++line, index++) {
    if(line->second.d_point1 == NULL || line->second.d_point2 == NULL) {
      continue;
    }

    int64 t1 = line->second.d_point1->Id();
    int64 t2 = line->second.d_point2->Id();
    int64 t;
    if(t1 == id1) {
      t = t2;
    } else if(t2 == id1) {
      t = t1;
    } else {
      continue;
    }

    temp.push_back(make_pair(line->first, t));
  }

  for(; line != lines.end() && index < beginEnd; ++line, index++);

  for(; line != lines.end(); ++line) {
    if(line->second.d_point1 == NULL || line->second.d_point2 == NULL) {
      continue;
    }

    int64 t1 = line->second.d_point1->Id();
    int64 t2 = line->second.d_point2->Id();
    int64 t;
    if(t1 == id1) {
      t = t2;
    } else if(t2 == id1) {
      t = t1;
    } else {
      continue;
    }

    temp.push_back(make_pair(line->first, t));
  }

  vector<pair<int64, int64> >::iterator ti;
  for(ti=temp.begin(); ti!=temp.end(); ++ti) {
    int64 springId = ti->first;
    int64 t = ti->second;

    PhysicalPointInfo *p2 = body.GetPoint(t);
    if(p2 == NULL) { return -3; }

    int64 tId;
    if(copy_spring(body, tId, springId, *p, *p2, ed) == NULL) {
      return -3;
    }
  }

  int64 springId;
  if(new_spring(s, body, springId, *current, *p, ed) == NULL) {
    return -3;
  }

  MoveSpider(s, body);

  return 0;
}

int SpiderBasic::add_line(BasicState &s,
			  const BasicInstruction &instruction,
			  Body &body, EnvironmentData &ed) const {
  return AddLine(GetParameter(s, instruction), s, body, ed);
}

int SpiderBasic::AddLine(const Vector3d &location, BasicState &s,
			 Body &body, EnvironmentData &ed) const {
  if(s.d_currentPoint == 0) { return 0; }
  int64 otherId = find_point(location, body);
  if(otherId == 0) { return 0; }

  PhysicalPointInfo *p1 = body.GetPoint(s.d_currentPoint);
  PhysicalPointInfo *p2 = body.GetPoint(otherId);
  if(p1 == NULL || p2 == NULL) { return -3; }

  int64 springId;
  if(new_spring(s, body, springId, *p1, *p2, ed) == NULL) {
    return -3;
  }

  s.d_currentPoint = otherId;

  MoveSpider(s, body);

  return 0;
}

int SpiderBasic::remove_line(BasicState &s,
			     const BasicInstruction &instruction,
			     Body &body, EnvironmentData &ed) const {
  if(s.d_currentPoint == 0) { return 0; }
  int64 otherId = find_point(GetParameter(s, instruction), body);
  if(otherId == 0) { return 0; }

  int64 springId = find_spring(body, s.d_currentPoint, otherId);
  if(springId == 0) { return 0; }

  body.RemoveSpring(ed, springId);

  s.d_currentPoint = otherId;

  MoveSpider(s, body);

  return 0;
}

int SpiderBasic::remove_point(BasicState &s,
			      const BasicInstruction &instruction,
			      Body &body, EnvironmentData &ed) const {
  if(s.d_currentPoint == 0) { return 0; }
  body.RemovePoint(ed, s.d_currentPoint);
  return move_to(s, instruction, body);
}

int SpiderBasic::merge(BasicState &s,
		       const BasicInstruction &instruction,
		       Body &body, EnvironmentData &ed) const {
  int64 id1 = s.d_currentPoint;
  int64 id2 = find_point(GetParameter(s, instruction), body);
  if(id1 == 0 || id2 == 0) { return 0; }

  PhysicalPointInfo *current1 = body.GetPoint(id1);
  PhysicalPointInfo *current2 = body.GetPoint(id2);
  if(current1 == NULL || current2 == NULL) { return -3; }

  PhysicalPointInfo *p = merge_point(s, body, ed, current1->GetPhysicalPoint(),
				     current2->GetPhysicalPoint());
  if(p == NULL) { return -3; }

  vector<pair<int64, int64> > temp;
  const map<int64, Body::ConnectionData> &lines = body.Connections();
  map<int64, Body::ConnectionData>::const_iterator line;
  for(line = lines.begin(); line != lines.end(); ++line) {
    if(line->second.d_point1 == NULL || line->second.d_point2 == NULL) {
      continue;
    }
    int64 t1 = line->second.d_point1->Id();
    int64 t2 = line->second.d_point2->Id();
    int64 t;
    if(t1 == id1 && t2 != id2) {
      t = t2;
    } else if(t2 == id1 && t1 != id2) {
      t = t1;
    } else if(t1 == id2) {
      t = t2;
    } else if(t2 == id2) {
      t = t1;
    } else {
      continue;
    }

    temp.push_back(make_pair(line->first, t));
  }

  vector<pair<int64, int64> >::iterator ti;
  for(ti=temp.begin(); ti!=temp.end(); ++ti) {
    int64 springId = ti->first;
    int64 t = ti->second;

    PhysicalPointInfo *p2 = body.GetPoint(t);
    if(p2 == NULL) { return -3; }

    int64 tId;
    if(copy_spring(body, tId, springId, *p, *p2, ed) == NULL) {
      return -3;
    }
  }

  body.RemovePoint(ed, id1);
  body.RemovePoint(ed, id2);

  MoveSpider(s, body);

  return 0;
}

int SpiderBasic::move_to(BasicState &s,
			 const BasicInstruction &instruction,
			 Body &body) const {
  return MoveTo(GetParameter(s, instruction), s, body);
}

int SpiderBasic::MoveTo(const Vector3d &location, BasicState &s,
			Body &body) const {
  s.d_currentPoint = find_point(location, body);

  MoveSpider(s, body);

  return 0;
}

int SpiderBasic::label(BasicState &,
		       const BasicInstruction &,
		       Body &) const {
  return 0;
}

int SpiderBasic::jump(BasicState &s,
		      const BasicInstruction &instruction,
		      Body &) const {
  if(s.d_frameMap.empty()) { return 0; }

  Vector3d id = GetParameter(s, instruction);

  map<Vector3d, Frame *, DontcareCompare>::iterator i, found;
  // Start with the first one, then try to find a closer one
  i = found = s.d_frameMap.begin();
  double distance = DistanceSquared(id, i->first);
  ++i;

  for(; i != s.d_frameMap.end(); ++i) {
    double tempDistance = DistanceSquared(id, i->first);
    if(tempDistance < distance) {
      distance = tempDistance;
      found = i;
    }
  }

  if(found == s.d_frameMap.end() || found->second == NULL) { return 1; }
  Frame *f = new Frame(*(found->second));
  if(f == NULL) { return 2; }

  s.d_frameStack.push_back(f);

  return 0;
}

int SpiderBasic::partial(BasicState &s,
			 const BasicInstruction &instruction,
			 Body &) const {
  s.d_partial = GetParameter(s, instruction);
  double max = s.d_partial[0];
  if(s.d_partial[1] > max) {
    max = s.d_partial[1];
  }
  if(s.d_partial[2] > max) {
    max = s.d_partial[2];
  }

  if(max > 1) {
    s.d_partial /= max;
  }

  return 0;
}

int SpiderBasic::push_d(BasicState &s,
			const BasicInstruction &instruction,
			Body &body) const {
  s.d_spacialDescriptionStack.push_back(s.d_spacialDescription);

  return 0;
}

int SpiderBasic::pop_d(BasicState &s,
		       const BasicInstruction &instruction,
		       Body &body) const {
  if(!s.d_spacialDescriptionStack.empty()) {
    s.d_spacialDescription = s.d_spacialDescriptionStack.back();
    s.d_spacialDescriptionStack.pop_back();
  }

  return MoveTo(s.d_spacialDescription.Position(), s, body);

  return 0;
}

int SpiderBasic::top_d(BasicState &s,
		       const BasicInstruction &instruction,
		       Body &body) const {
  if(!s.d_spacialDescriptionStack.empty()) {
    s.d_spacialDescription = s.d_spacialDescriptionStack.back();
  }

  return MoveTo(s.d_spacialDescription.Position(), s, body);

  return 0;
}

int SpiderBasic::pop_line_d(BasicState &s,
			    const BasicInstruction &instruction,
			    Body &body,
			    EnvironmentData &ed) const {
  if(!s.d_spacialDescriptionStack.empty()) {
    s.d_spacialDescription = s.d_spacialDescriptionStack.back();
  }

  return AddLine(s.d_spacialDescription.Position(), s, body, ed);
}

int SpiderBasic::top_line_d(BasicState &s,
			    const BasicInstruction &instruction,
			    Body &body,
			    EnvironmentData &ed) const {
  if(!s.d_spacialDescriptionStack.empty()) {
    s.d_spacialDescription = s.d_spacialDescriptionStack.back();
  }

  return AddLine(s.d_spacialDescription.Position(), s, body, ed);
}

int SpiderBasic::control_if(BasicState &s,
			    const BasicInstruction &instruction,
			    Body &body) const {
  Frame *frame = s.d_frameStack.back();
  if(frame == NULL) { return 1; }

  map<unsigned int, ControlData>::iterator i;
  i = frame->d_controlData.find(frame->d_currentLine);
  if(i == frame->d_controlData.end()) {
    // Means there was no end for the if, and so it is ignored
    return 0;
  }

  // Each component ranges from -1 to 1.  Instead of capping the value of
  // each component, the component roles over. 1 goes to 0 and -1 goes to 0
  Vector3d parameters(GetParameter(s, instruction));

  // <x, y, z>
  // Conditions based on z
  // -z -> if x >= y then true
  // +z -> if x <= y then true
  //  0 -> if x == y then true
  if((parameters[2] == 0 && parameters[0] != parameters[1]) ||
     (parameters[2] <  0 && parameters[0] <  parameters[1]) ||
     (parameters[2] >  0 && parameters[0] >  parameters[1])) {
    // condition test failed, go to else or end.  If this is an ifelse, end
    // is the else instruction, and if not end is the end instruction.
    frame->d_currentLine = i->second.d_end;
  }

  return 0;
}

int SpiderBasic::control_else(BasicState &s,
			      const BasicInstruction &instruction,
			      Body &body) const {
  Frame *frame = s.d_frameStack.back();
  if(frame == NULL) { return 1; }

  map<unsigned int, ControlData>::iterator i;
  i = frame->d_controlData.find(frame->d_currentLine);
  if(i == frame->d_controlData.end()) {
    // Means there was no corresponding if, so ignore instruction
    return 0;
  }

  // Should only execute and have control data if we just finished
  // executing the if(true) instructions
  frame->d_currentLine = i->second.d_end;

  return 0;
}

int SpiderBasic::control_loop(BasicState &s,
			       const BasicInstruction &instruction,
			       Body &body) const {
  Frame *frame = s.d_frameStack.back();
  if(frame == NULL) { return 1; }

  map<unsigned int, ControlData>::iterator i;
  i = frame->d_controlData.find(frame->d_currentLine);
  if(i == frame->d_controlData.end()) {
    // Means there was no end for the if, and so it is ignored
    return 0;
  }

  // Each component ranges from -1 to 1.  Instead of capping the value of
  // each component, the component roles over. 1 goes to 0 and -1 goes to 0
  Vector3d parameters(GetParameter(s, instruction));

  if(i->second.d_currentIteration == 0) {
    i->second.d_test = i->first;
  } else if(i->second.d_currentIteration >= i->second.d_maxIterations) {
    frame->d_currentLine = i->second.d_end;
    i->second.d_currentIteration = 0;
    return 0;
  } else {
    parameters = i->second.d_test;
  }

  // <x, y, z>
  // Conditions based on z
  // -z -> if x >= y then true
  // +z -> if x <= y then true
  //  0 -> if x == y then true
  if((parameters[2] == 0 && parameters[0] == parameters[1]) ||
     (parameters[2] <  0 && parameters[0] >= parameters[1]) ||
     (parameters[2] >  0 && parameters[0] <= parameters[1])) {
    parameters[0] += parameters[2];
    i->second.d_test = parameters;
    i->second.d_currentIteration++;
  } else {
    // condition test failed
    frame->d_currentLine = i->second.d_end;
    i->second.d_currentIteration = 0;
  }

  return 0;
}

int SpiderBasic::control_end(BasicState &s,
			     const BasicInstruction &instruction,
			     Body &body) const {
  Frame *frame = s.d_frameStack.back();
  if(frame == NULL) { return 1; }

  map<unsigned int, ControlData>::iterator i;
  i = frame->d_controlData.find(frame->d_currentLine);
  if(i == frame->d_controlData.end()) {
    return 0;
  }

  if(i->second.d_type == InstructionSet::LOOP) {
    frame->d_currentLine = i->second.d_start;
  }

  return 0;
}

int SpiderBasic::print(BasicState &s,
		       const BasicInstruction &instruction,
		       Body &body) const {
  Vector3d x(GetParameter(s, instruction));
  cout << "<" << x[0] << " , " << x[1] << " , " << x[2] << ">" << endl;

  return 0;
}

PhysicalPointInfo *SpiderBasic::new_point_independent(
					  BasicState &s,
					  const BasicInstruction &instruction,
					  Body &body,
					  EnvironmentData &ed) const {
  Vector3d heading(GetParameter(s, instruction));
  return new_point(heading, s, instruction, body, ed);
}

PhysicalPointInfo *SpiderBasic::new_point_oriented(
					  BasicState &s,
					  const BasicInstruction &instruction,
					  Body &body,
					  EnvironmentData &ed) const {
  RotateSpider(GetParameter(s, instruction), s);
  Vector3d heading(s.d_spacialDescription.Forward());
  return new_point(heading, s, instruction, body, ed);
}

PhysicalPointInfo *SpiderBasic::new_point(const Vector3d &heading,
					  BasicState &s,
					  const BasicInstruction &instruction,
					  Body &body,
					  EnvironmentData &ed) const {
  Vector3d offset(heading[0] * s.d_dilation[0], heading[1] * s.d_dilation[1], 
		  heading[2] * s.d_dilation[2]);
  Vector3d newLocation(s.d_spacialDescription.Position() + offset);
  if(newLocation[1] < 0) { newLocation[1] = 0; }

  if(body.PointCount() <= 0) { newLocation = Vector3d(0.0, 0.0, 0.0); }

  return body.AddPoint(ed, s.d_currentPoint, newLocation,
		       s.d_pointParameters[0]);
}

PhysicalPointInfo *SpiderBasic::merge_point(BasicState &s, Body &body,
					    EnvironmentData &ed,
					    const PhysicalPoint &p1,
					    const PhysicalPoint &p2) const {
  Vector3d newLocation((p1.Position() + p2.Position()) / 2.0);
  double mass = p1.Mass() + p2.Mass();

  return body.AddPoint(ed, s.d_currentPoint, newLocation, mass);
}

SpringInfo *SpiderBasic::new_spring(BasicState &s, Body &body,
				    int64 &id,
				    PhysicalPointInfo &p1,
				    PhysicalPointInfo &p2,
				    EnvironmentData &ed) const {
#if 0
  return body.AddSpring(ed, id, &p1, &p2,
			Distance(p1.GetPhysicalPoint().Position(),
				 p2.GetPhysicalPoint().Position()),
			s.d_springConstants[0], s.d_springConstants[1],
			s.d_springFluctuations[0], s.d_springFluctuations[1],
			s.d_springFluctuations[2]);
#else
  double dist = Distance(p1.GetPhysicalPoint().Position(),
			 p2.GetPhysicalPoint().Position());
  Vector3d sc = s.d_springConstants;
  if(dist <= 0) {
    sc = Vector3d(0.0, 0.0, 0.0);
  } else {
    sc = Vector3d(9.8/(1.0 + 0.001*dist), 0.0, 0.0);
  }
  return body.AddSpring(ed, id, &p1, &p2, dist, sc[0], sc[1],
			s.d_springFluctuations[0], s.d_springFluctuations[1],
			s.d_springFluctuations[2]);
#endif
}

SpringInfo *SpiderBasic::copy_spring(Body &body,
				     int64 &id, int64 springId,
				     PhysicalPointInfo &p1,
				     PhysicalPointInfo &p2,
				     EnvironmentData &ed) const {
  SpringInfo *si = body.GetSpring(springId);
  if(si == NULL) { id = 0; return NULL; }

  const Spring &spring = si->GetSpring();

  return body.AddSpring(ed, id, &p1, &p2,
			Distance(p1.GetPhysicalPoint().Position(),
				 p2.GetPhysicalPoint().Position()),
			spring.ks(), spring.kd(), spring.FluctuationDelta(),
			spring.FluctuationStart(), spring.FluctuationRate());
}

int64 SpiderBasic::find_point(const Vector3d &location, Body &body) const {
  int64 id = 0;

  bool first = true;
  double minDistance = 0.0;

  const map<int64, PhysicalPointInfo *> &points = body.GetPoints();
  map<int64, PhysicalPointInfo *>::const_iterator i;
  for(i = points.begin(); i != points.end(); ++i) {
    if(i->second == NULL) { continue; }

    double dist = DistanceSquared(location,
				  i->second->GetPhysicalPoint().Position());

    if(first || dist < minDistance) {
      id = i->first;
      minDistance = dist;
      first = false;
    }
  }

  return id;
}

int64 SpiderBasic::find_spring(Body &body, int64 id1, int64 id2) const {
  const map<int64, Body::ConnectionData> &springs = body.Connections();
  map<int64, Body::ConnectionData>::const_iterator i;
  for(i=springs.begin(); i!=springs.end(); ++i) {
    if(i->second.d_point1 == NULL || i->second.d_point2 == NULL) {
      continue;
    }

    int64 p1 = i->second.d_point1->Id();
    int64 p2 = i->second.d_point2->Id();

    if((p1 == id1 && p2 == id2) ||
       (p1 == id2 && p2 == id1)) {
      return i->first;
    }
  }

  return 0;
}

SpiderBasic::BasicState::BasicState(void) {
  return;
}

SpiderBasic::BasicState::~BasicState(void) {
  d_stacks.clear();
  return;
}

void SpiderBasic::MoveSpider(BasicState &s, Body &body) const {
  Vector3d loc(0.0, 0.0, 0.0);

  PhysicalPointInfo *p = body.GetPoint(s.d_currentPoint);
  if(p != NULL) {
    loc = p->GetPhysicalPoint().Position();
  }

  s.d_spacialDescription.SetPosition(loc);

  return;
}

void SpiderBasic::Constrain(Vector3d &x) const {
  // Data values are not normalized, but currently need to be constrained to
  // the range -1 to 1 for each component
  if(x[0] < -1) { x[0] = -1.0; }
  if(x[0] >  1) { x[0] =  1.0; }
  if(x[1] < -1) { x[1] = -1.0; }
  if(x[1] >  1) { x[1] =  1.0; }
  if(x[2] < -1) { x[2] = -1.0; }
  if(x[2] >  1) { x[2] =  1.0; }

  return;
}
