#ifndef __PARSER_BASIC_H__
#define __PARSER_BASIC_H__

#include <string>

#include "jvector.h"
#include "parser.h"
#include "instruction_set.h"

class ParserBasic : public Parser {
public:
  ParserBasic(InstructionSet &is);
  ~ParserBasic(void);

protected:
  class BasicState : public State {
  public:
    BasicState(void) : d_vector(0.0, 0.0, 0.0) { return; }
    ~BasicState(void) { return; }

    Vector3d d_vector;
    int d_arguement;
    int d_instruction;
  };

  State *NewState(void) const;
  bool CreateBasicInstruction(State &state) const;
  bool Parse(State &state, const std::string &str) const;

private:
  InstructionSet &d_is;
};

#endif
