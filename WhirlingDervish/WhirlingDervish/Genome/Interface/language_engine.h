#ifndef __LANGUAGE_ENGINE_H__
#define __LANGUAGE_ENGINE_H__

#include <vector>

#include "ec_settings.h"
#include "instruction_set_settings.h"

class Spider;
class Turtle;
class Parser;
class InstructionSet;

class LanguageEngine {
public:
  LanguageEngine(void);
  ~LanguageEngine(void);

  bool Initialize(void);
  bool PrepareInstructionSet(void);

  Spider &GetSpider(void);
  Turtle &GetTurtle(void);
  Parser &GetParser(void);
  const InstructionSet &GetInstructionSet(void);

  ECSettings &GetECSettings(void) { return d_settings; }
  InstructionSetSettings &GetInstructionSetSettings(void) { return d_iss; }

private:
  ECSettings d_settings;
  InstructionSetSettings d_iss;

  Spider *d_spider;
  Turtle *d_turtle;
  Parser *d_parser;
  InstructionSet *d_instructionSet;
};

#endif
