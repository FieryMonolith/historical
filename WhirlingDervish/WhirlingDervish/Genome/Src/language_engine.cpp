#include <cassert>

#include "spider_basic.h"
#include "turtle_basic.h"
#include "parser_basic.h"
#include "instruction_set.h"

#include "language_engine.h"

using namespace std;

LanguageEngine::LanguageEngine(void)
  : d_spider(NULL), d_turtle(NULL), d_parser(NULL), d_instructionSet(NULL)
{
  d_instructionSet = new InstructionSet;
  if(d_instructionSet != NULL) {
    d_spider = new SpiderBasic();
    d_turtle = new TurtleBasic(d_settings.ChanceOfData(), *d_instructionSet);
    d_parser = new ParserBasic(*d_instructionSet);
  }

  return;
}

LanguageEngine::~LanguageEngine(void) {
  if(d_spider != NULL) { delete d_spider; d_spider = NULL; }
  if(d_turtle != NULL) { delete d_turtle; d_turtle = NULL; }
  if(d_parser != NULL) { delete d_parser; d_parser = NULL; }
  if(d_instructionSet != NULL) {
    delete d_instructionSet;
    d_instructionSet=NULL;
  }

  return;
}

bool LanguageEngine::Initialize(void) {
  if(d_spider == NULL || d_turtle == NULL || d_parser == NULL ||
     d_instructionSet == NULL) {
    return false;
  }

  if(!d_settings.Initialize() || !d_iss.Initialize()) {
    return false;
  }

  return true;
}

bool LanguageEngine::PrepareInstructionSet(void) {
  assert(d_instructionSet);
  return d_instructionSet->Initialize(d_iss.Set());
}

Spider &LanguageEngine::GetSpider(void) {
  assert(d_spider);
  return *d_spider;
}

Turtle &LanguageEngine::GetTurtle(void) {
  assert(d_turtle);
  return *d_turtle;
}

Parser &LanguageEngine::GetParser(void) {
  assert(d_parser);
  return *d_parser;
}

const InstructionSet &LanguageEngine::GetInstructionSet(void) {
  assert(d_instructionSet);
  return *d_instructionSet;
}
