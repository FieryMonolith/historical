#ifndef __TURTLE_SIMPLE_H__
#define __TURTLE_SIMPLE_H__

#include <vector>

#include "turtle.h"

class TurtleSimple : public Turtle {
public:
  TurtleSimple(void);
  ~TurtleSimple(void);

protected:
  State *NewState(const Genome &) const;
  bool Initialize(State &) const;
  bool Complete(State &) const;
  Instruction *RandomInstruction(RandomNumberGenerator &,
				 State &) const;
  Instruction *MutateParameter(const Instruction &,
			       RandomNumberGenerator &,
			       State &) const;

  class StateSimple : public State {
  public:
    StateSimple(const std::vector<Instruction *> &);
    ~StateSimple(void) { return; }

    std::vector<int> d_pointIds;
    int d_pointIdAvailable;

  private:
    StateSimple(void);
    StateSimple(const StateSimple &);
    StateSimple &operator=(const StateSimple &);
  };

private:
  // returns a random integer from ids, or zero if ids is empty
  int RandomPointId(const std::vector<int> &ids) const;
  int NewId(State &) const;
};

#endif
