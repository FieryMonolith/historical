#ifndef __SPIDER_BASIC_H__
#define __SPIDER_BASIC_H__

#include <map>
#include <vector>
#include <utility>

#include "defs.h"
#include "jvector.h"
#include "spider.h"

#include "spacial_description.h"

class Genome;
class Body;
class PhysicalPoint;
class PhysicalPointInfo;
class Spring;
class SpringInfo;
class Instruction;
class BasicInstruction;
class EnvironmentData;

class SpiderBasic : public Spider {
public:
  SpiderBasic(void);
  ~SpiderBasic(void);

  bool RunLoop(const Genome &, Body &, EnvironmentData &) const;

private:
  class DontcareCompare {
  public:
    bool operator()(const Vector3d &, const Vector3d &) { return true; }
  };

protected:
  State *NewState(void) const;
  bool Initialize(State &, EnvironmentData &) const;
  bool Complete(State &) const;
  bool Run(State &, const Genome &, Body &, EnvironmentData &) const;
  int Execute(State &, const Instruction &, Body &, EnvironmentData &) const;

  class ControlData {
  public:
    unsigned int d_start;
    unsigned int d_end;
    unsigned int d_type;

    unsigned int d_currentIteration;
    Vector3d d_test;

    static const unsigned int d_maxIterations;
  };

  class Frame {
  public:
    std::vector<Instruction *> d_instructions;
    unsigned int d_currentLine;
    std::map<unsigned int, ControlData> d_controlData;
    Vector3d d_id;
  };

  class BasicState : public State {
  public:
    BasicState(void);
    ~BasicState(void);

    std::vector<Frame *> d_frames;
    std::vector<Frame *> d_frameStack;
    std::map<Vector3d, Frame *, DontcareCompare> d_frameMap;
    std::vector<Instruction *> d_myInstructions;
    static const unsigned int d_maxStackDepth;

    unsigned int d_currentStack;
    std::vector<std::vector<Vector3d> > d_stacks;
    Vector3d d_springConstants;
    Vector3d d_springFluctuations;
    Vector3d d_pointParameters;
    Vector3d d_gravity;
    Vector3d d_dilation;
    Vector3d d_partial;
    std::vector<SpacialDescription> d_spacialDescriptionStack;

    SpacialDescription d_spacialDescription;

    // current point is genetic id
    int64 d_currentPoint;
  };

private:
  static const Vector3d d_null;
  static const Vector3d d_zero;
  static const unsigned int d_numberOfStacks = 4;

  // INSTRUCTIONS
  int nop(void) const;

  int pop(BasicState &) const;
  int push(BasicState &, const BasicInstruction &) const;
  int push_1(BasicState &, const BasicInstruction &) const;
  int push_2(BasicState &, const BasicInstruction &) const;
  int push_3(BasicState &, const BasicInstruction &) const;
  int switch_stack(BasicState &, const BasicInstruction &) const;
  int shift_data_left(BasicState &, const BasicInstruction &) const;
  int shift_data_right(BasicState &, const BasicInstruction &) const;

  int rotate(BasicState &, const BasicInstruction &) const;

  int add(BasicState &, const BasicInstruction &) const;
  int sub(BasicState &, const BasicInstruction &) const;
  int mult(BasicState &, const BasicInstruction &) const;
  int div(BasicState &, const BasicInstruction &) const;
  int mod(BasicState &, const BasicInstruction &) const;

  int set_spring_constants(BasicState &, const BasicInstruction &) const;
  int set_spring_fluctuations(BasicState &,const BasicInstruction &) const;
  int set_point_constants(BasicState &, const BasicInstruction &) const;
  int set_dilation(BasicState &, const BasicInstruction &) const;

  int split(BasicState &, const BasicInstruction &, Body &,
	    EnvironmentData &) const;
  int split_dup(BasicState &, const BasicInstruction &, Body &,
		EnvironmentData &) const;
  int add_line(BasicState &, const BasicInstruction &, Body &,
	       EnvironmentData &) const;
  int remove_line(BasicState &, const BasicInstruction &, Body &,
		  EnvironmentData &) const;
  int remove_point(BasicState &, const BasicInstruction &, Body &,
		   EnvironmentData &) const;
  int merge(BasicState &, const BasicInstruction &, Body &,
	    EnvironmentData &) const;

  int move_to(BasicState &, const BasicInstruction &, Body &) const;
  int partial(BasicState &, const BasicInstruction &, Body &) const;
  int jump(BasicState &, const BasicInstruction &, Body &) const;
  int label(BasicState &, const BasicInstruction &, Body &) const;
  int push_d(BasicState &, const BasicInstruction &, Body &) const;
  int pop_d(BasicState &, const BasicInstruction &, Body &) const;
  int top_d(BasicState &, const BasicInstruction &, Body &) const;
  int pop_line_d(BasicState &, const BasicInstruction &, Body &,
		 EnvironmentData &) const;
  int top_line_d(BasicState &, const BasicInstruction &, Body &,
		 EnvironmentData &) const;
  int split_partial(BasicState &, const BasicInstruction &, Body &,
		    EnvironmentData &) const;
  int split_oriented(BasicState &, const BasicInstruction &, Body &,
		     EnvironmentData &) const;
  int split_dup_oriented(BasicState &, const BasicInstruction &, Body &,
			 EnvironmentData &) const;
  int split_partial_oriented(BasicState &, const BasicInstruction &, Body &,
			     EnvironmentData &) const;
  int control_if(BasicState &, const BasicInstruction &, Body &) const;
  int control_else(BasicState &, const BasicInstruction &, Body &) const;
  int control_loop(BasicState &, const BasicInstruction &, Body &) const;
  int control_end(BasicState &, const BasicInstruction &, Body &) const;
  int print(BasicState &, const BasicInstruction &, Body &) const;
  // END INSTRUCTIONS

  PhysicalPointInfo *new_point_independent(BasicState &,
					   const BasicInstruction &,
					   Body &, EnvironmentData &) const;
  PhysicalPointInfo *new_point_oriented(BasicState &, const BasicInstruction &,
					Body &, EnvironmentData &) const;
  PhysicalPointInfo *new_point(const Vector3d &, BasicState &,
			       const BasicInstruction &,
			       Body &, EnvironmentData &) const;
  PhysicalPointInfo *merge_point(BasicState &, Body &, EnvironmentData &,
				 const PhysicalPoint &,
				 const PhysicalPoint &) const;
  SpringInfo *new_spring(BasicState &, Body &, int64 &id,
			 PhysicalPointInfo &p1,
			 PhysicalPointInfo &p2,
			 EnvironmentData &) const;
  SpringInfo *copy_spring(Body &, int64 &id,
			  int64 springId,
			  PhysicalPointInfo &p1,
			  PhysicalPointInfo &p2,
			  EnvironmentData &) const;
  int AddLine(const Vector3d &, BasicState &, Body &,
	      EnvironmentData &) const;
  int MoveTo(const Vector3d &, BasicState &, Body &) const;
  int64 find_point(const Vector3d &location, Body &) const;
  int64 find_spring(Body &, int64, int64) const;
  Vector3d GetParameter(BasicState &, const BasicInstruction &) const;
  void MoveSpider(BasicState &, Body &) const;
  void Constrain(Vector3d &) const;
  void RotateSpider(const Vector3d &rotations, BasicState &) const;
  bool CreateFrames(BasicState &, const std::vector<Instruction *> &) const;
  bool ProcessControlBlocks(Frame &) const;

  class ControlLines {
  public:
    ControlLines(unsigned int t, unsigned int u) : type(t),a(u),b(0) {return;}
    ControlLines(const ControlLines &cl) { *this = cl; return; }
    const ControlLines &operator=(const ControlLines &cl) {
      type = cl.type;
      a = cl.a;
      b = cl.b;
      return *this;
    }
    unsigned int type;
    unsigned int a;
    unsigned int b;
  };
};

#endif
