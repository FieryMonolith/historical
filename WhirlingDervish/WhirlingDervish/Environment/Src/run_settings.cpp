#include "run_settings.h"

using namespace std;

const std::string RunSettings::n_populationSize("POPULATION_SIZE");
const std::string RunSettings::n_maxGenerations("MAX_GENERATIONS");
const std::string RunSettings::n_randomSeed("RANDOM_SEED");
const std::string RunSettings::n_startState("START_STATE");
const std::string RunSettings::n_algorithm("ALGORITHM");
const std::string RunSettings::n_storeFile("STORE_FILE");
const std::string RunSettings::n_loadFile("LOAD_FILE");
const std::string RunSettings::n_outputFitnesstxt("OUTPUT_FITNESS_TXT");

RunSettings::RunSettings(void)
  : d_populationSize(0),
    d_maxGenerations(0),
    d_randomSeed(-1),
    d_startState(0),
    d_algorithm(0)
{
  return;
}

RunSettings::~RunSettings(void) {
  return;
}

bool RunSettings::Initialize(void) {
  Destroy();

  SettingParser::InputBase *ib;

  // RunSettings::PopulationSize
  ib = new SettingParser::Input<unsigned int &, RunSettings>(
					     n_populationSize, this,
					     &RunSettings::PopulationSize);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_populationSize, ib));

  // RunSettings::MaxGeneration
  ib = new SettingParser::Input<unsigned int &, RunSettings>(
					     n_maxGenerations, this,
					     &RunSettings::MaxGenerations);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_maxGenerations, ib));

  // RunSettings::RandomSeed
  ib = new SettingParser::Input<int &, RunSettings>(
					     n_randomSeed, this,
					     &RunSettings::RandomSeed);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_randomSeed, ib));

  // RunSettings::StartState
  ib = new SettingParser::Input<unsigned int &, RunSettings>(
					     n_startState, this,
					     &RunSettings::StartState);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_startState, ib));

  // RunSettings::Algorithm
  ib = new SettingParser::Input<unsigned int &, RunSettings>(
					     n_algorithm, this,
					     &RunSettings::Algorithm);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_algorithm, ib));

  // RunSettings::StoreFile
  ib = new SettingParser::Input<string &, RunSettings>(
					     n_storeFile, this,
					     &RunSettings::StoreFile);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_storeFile, ib));

  // RunSettings::LoadFile
  ib = new SettingParser::Input<string &, RunSettings>(
					     n_loadFile, this,
					     &RunSettings::LoadFile);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_loadFile, ib));

  // RunSettings::outputFitnesstxt
  ib = new SettingParser::Input<unsigned int &, RunSettings>(
					     n_outputFitnesstxt, this,
					     &RunSettings::outputFitnesstxt);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_outputFitnesstxt, ib));


  return true;
}
