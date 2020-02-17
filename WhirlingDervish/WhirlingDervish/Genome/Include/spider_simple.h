#ifndef __SPIDER_SIMPLE_H__
#define __SPIDER_SIMPLE_H__

#include <vector>
#include <map>

#include "defs.h"
#include "spider.h"

class Genome;
class Body;
class PhysicalPoint;
class MasslessSpringDampner;
class Instruction;
class EnvironmentData;

class SpiderSimple : public Spider {
public:
  SpiderSimple(void);
  ~SpiderSimple(void);

protected:
  State *NewState(void) const;
  bool Initialize(State &) const;
  int Execute(State &, const Instruction &, Body &, EnvironmentData &) const;

  class SimpleState : public State {
  public:
    SimpleState(const std::vector<Instruction *> &);
    ~SimpleState(void);

    std::map<int, int64> d_conversion;
    int d_currentId;
    int64 d_currentPoint;
  };

private:
  int nop(void) const;

  int split(SimpleState &, const Instruction &, Body &,
	    EnvironmentData &) const;
  int split_dup(SimpleState &, const Instruction &, Body &,
		EnvironmentData &) const;
  int add_line(SimpleState &, const Instruction &, Body &,
	       EnvironmentData &) const;
  int remove_line(SimpleState &, const Instruction &, Body &,
		  EnvironmentData &) const;
  int remove_point(SimpleState &, const Instruction &, Body &,
		   EnvironmentData &) const;
  int merge(SimpleState &, const Instruction &, Body &,
	    EnvironmentData &) const;

  int move_to(SimpleState &, const Instruction &, Body &) const;

  int change_mass(SimpleState &, const Instruction &) const;
  int change_innate_length(SimpleState &, const Instruction &) const;
  int change_ks(SimpleState &, const Instruction &) const;
  int change_kd(SimpleState &, const Instruction &) const;
  int change_fluctuation_delta(SimpleState &, const Instruction &) const;
  int change_fluctuation_start(SimpleState &, const Instruction &) const;

  PhysicalPoint *new_point(SimpleState &, const Vector3d &, double, Body &,
			   EnvironmentData &) const;
  PhysicalPoint *marge_point(SimpleState &, Body &, EnvironmentData &,
			     const PhysicalPoint &,
			     const PhysicalPoint &) const;
  MasslessSpringDampner *new_spring(SimpleState &, Body &, int64 &id,
				    const PhysicalPoint &,
				    const PhysicalPoint &,
				    EnvironmentData &) const;
  MasslessSpringDampner *copy_spring(SimpleState &, Body &, int64 &id,
				     const PhysicalPoint &,
				     const PhysicalPoint &,
				     int64 msdId,
				     EnvironmentData &) const;
  int64 FindSpring(Body &, int64, int64) const;
};

#endif
