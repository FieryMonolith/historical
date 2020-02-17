#include "defs.h"
#include "file_handler.h"

#include "environment_data.h"

using namespace std;

EnvironmentData::EnvironmentData(void)
  : d_currentTime(0.0),
    d_generation(1),
    d_cycle(0),
    d_outputFileId(0),
    d_avgPoints(0.0),
    d_maxPoints(0),
    d_avgSprings(0.0),
    d_maxSprings(0),
    d_avgFitness(0.0),
    d_maxFitness(0.0),
    d_avgHeight(0.0),
    d_maxHeight(0.0),
    d_avgMove(0.0),
    d_maxMove(0.0),
    d_dataInitialized(false) {
  return;
}

EnvironmentData::~EnvironmentData(void) {
  d_populationStorage.Clear((EnvironmentData &)(*this));
  d_dataInitialized = false;

  return;
}

bool EnvironmentData::InitializeData(void) {
  if(d_dataInitialized) { return true; }

  if(!d_languageEngine.Initialize()) { return false; }

  if(!d_runSettings.Initialize()) { return false; }

  if(!d_physicsSettings.Initialize()) { return false; }

  d_dataInitialized = true;

  return true;
}

// Process configuration files
bool EnvironmentData::LoadConfigurationFiles(FileHandler &fh,
				       const string &runSettingsFilename,
				       const string &physicsSettingsFilename,
				       const string &ecSettingsFilename,
				       const string &instructionSetFilename) {
  // Process Run Settings
  LoadConfigurationFile(fh, GetRunSettings(), runSettingsFilename);

  // Process Physics Settings
  LoadConfigurationFile(fh, GetPhysicsSettings(), physicsSettingsFilename);

  // Process EC Settings
  LoadConfigurationFile(fh, GetECSettings(), ecSettingsFilename);

  // Process InstructionSet Settings
  if(!GetLanguageEngine().GetInstructionSetSettings().Setup(instructionSetFilename)) {
    return false;
  }

  return true;
}

bool EnvironmentData::LoadConfigurationFile(FileHandler &fh, Setting &s,
					    const string &filename) {
  int fileId = fh.Create(filename.c_str(), ios::in);
  if(fileId <= 0) {
    return false;
  }

  bool success = false;
  istream &in = fh.InputStream(fileId, success);
  if(!success) {
    fh.Close(fileId);
    return false;
  }

  if(!SettingParser::ParseStream(in, s.Rules())) {
    fh.Close(fileId);
    return false;
  }

  fh.Close(fileId);

  return true;
}
