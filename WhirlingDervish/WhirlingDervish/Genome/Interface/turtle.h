#ifndef __TURTLE_H__
#define __TURTLE_H__

#include <vector>

class EnvironmentData;
class Genome;
class RandomNumberGenerator;
class Instruction;

class Turtle {
protected:
  class State {
  public:
    State(const std::vector<Instruction *> &);
    virtual ~State(void);

    std::vector<Instruction *> d_instructions;
    const std::vector<Instruction *> &genome_instructions;

  private:
    State(const State &);
    State &operator=(const State &);
  };


public:
  Turtle(void);
  virtual ~Turtle(void);

  bool CopyDivide(Genome &out, const Genome &in);
  bool Divide(Genome &out, const Genome &in, EnvironmentData &);

protected:
  virtual State *NewState(const Genome &) const;
  virtual bool Initialize(State &) const;
  virtual bool Complete(State &) const;
  virtual Instruction *RandomInstruction(RandomNumberGenerator &,
					 State &) const;
  virtual Instruction *MutateParameter(const Instruction &,
				       RandomNumberGenerator &,
				       State &) const;

private:
  bool Insert(std::vector<Instruction *> &instructions,
	      const Instruction &instruction,
	      RandomNumberGenerator &rng, State &);
  bool InsertCopy(std::vector<Instruction *> &instructions,
		  const Instruction &instruction);
  bool Mutation(std::vector<Instruction *> &instructions,
		const Instruction &instruction,
		RandomNumberGenerator &rng, State &);
  void NormalizeChances(double &insertion, double &duplication,
			double &deletion, double &point) const;
};

#endif
