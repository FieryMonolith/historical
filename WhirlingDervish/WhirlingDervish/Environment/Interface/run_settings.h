#ifndef __RUN_SETTINGS_H__
#define __RUN_SETTINGS_H__

#include <string>

#include "setting.h"

class RunSettings : public Setting {
public:
  RunSettings(void);
  ~RunSettings(void);

  bool Initialize(void);

  unsigned int  PopulationSize(void) const { return d_populationSize; }
  unsigned int &PopulationSize(void)       { return d_populationSize; }

  unsigned int  MaxGenerations(void) const { return d_maxGenerations; }
  unsigned int &MaxGenerations(void)       { return d_maxGenerations; }

  int  RandomSeed(void) const { return d_randomSeed; }
  int &RandomSeed(void)       { return d_randomSeed; }

  // Start state
  // 0 == New experiment
  // 1 == Load experiment use configuration random seed
  // 2 == Load experiment use stored random seed
  // anything else is an error
  unsigned int  StartState(void) const { return d_startState; }
  unsigned int &StartState(void)       { return d_startState; }

  unsigned int  Algorithm(void) const { return d_algorithm; }
  unsigned int &Algorithm(void)       { return d_algorithm; }

  const std::string &StoreFile(void) const { return d_storeFile; }
        std::string &StoreFile(void)       { return d_storeFile; }

  const std::string &LoadFile(void) const { return d_loadFile; }
        std::string &LoadFile(void)       { return d_loadFile; }

  const unsigned int outputFitnesstxt(void) const {return d_outputFitnesstxt; }
  unsigned int &outputFitnesstxt(void)       { return d_outputFitnesstxt; }

private:
  unsigned int d_populationSize;
  unsigned int d_maxGenerations;
  int d_randomSeed;
  unsigned int d_startState;
  unsigned int d_algorithm;
  std::string d_storeFile;
  std::string d_loadFile;
  unsigned int d_outputFitnesstxt;

  static const std::string n_populationSize;
  static const std::string n_maxGenerations;
  static const std::string n_randomSeed;
  static const std::string n_startState;
  static const std::string n_algorithm;
  static const std::string n_storeFile;
  static const std::string n_loadFile;
  static const std::string n_outputFitnesstxt;
};

#endif
