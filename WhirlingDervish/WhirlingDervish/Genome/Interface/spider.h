#ifndef __SPIDER_H__
#define __SPIDER_H__

class EnvironmentData;
class Genome;
class Body;
class Instruction;

class Spider {
protected:
  class State {
  public:
    State(void);
    virtual ~State(void);

  private:
    State(const State &);
    State &operator=(const State &);
  };

public:
  Spider(void);
  virtual ~Spider(void);

  virtual bool Run(const Genome &, Body &, EnvironmentData &) const;

protected:
  virtual State *NewState(void) const;
  virtual bool Initialize(State &, EnvironmentData &) const;
  virtual bool Complete(State &) const;
  virtual bool Run(State &, const Genome &, Body &, EnvironmentData &) const;
};

#endif
