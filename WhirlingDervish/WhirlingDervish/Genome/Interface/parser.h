#ifndef __PARSER_H__
#define __PARSER_H__

#include <vector>
#include <string>

class Genome;
class Instruction;

class Parser {
public:
  class State {
  public:
    State(void);
    virtual ~State(void);

    std::vector<Instruction *> d_instructions;
    std::vector<int> d_lineErrors;

    unsigned int d_instructionLine;
    bool d_validate;

  private:
    State(const State &);
    State &operator=(const State &);
  };

public:
  Parser(void) { return; }
  virtual ~Parser(void) { return; }

  bool Validate(std::vector<int> &, const std::string &genomeString) const;
  bool Parse(Genome &, std::vector<int> &errors,
	     const std::string &genomeString) const;

protected:
  virtual State *NewState(void) const;
  virtual bool Initialize(State &) const;
  virtual bool Complete(State &) const;
  virtual void CreateInstruction(State &) const;
  virtual bool Parse(State &, const std::string &) const;

private:
  bool DoParse(Genome &, std::vector<int> &, const std::string &,
	       bool validate=true) const;
};

#endif
