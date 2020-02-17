#include <iostream>

#include "environment.h"
#include "file_handler.h"
#include "command_line_settings.h"

using namespace std;

int main(int argc, char **argv) {
  CommandLineSettings cls;
  if(!cls.Initialize()) {
    cerr << "Could not Initialize CommandLineSettings, could not process";
    cout << endl << "command line arguements." << endl;
    return -1;
  }

  // This loads the location and file names of the configuration files.
  // If they are on the command line, they override the default name
  // and location.
  if(!cls.ProcessFileInfo(argc, argv)) {
    cerr << "Could not process command line." << endl;
    return -2;
  }

  // Prepare Experiment
  FileHandler fh;
  Environment environment;
  if(!environment.Initialize()) {
    cerr << "Could not initialize environment." << endl;
    return -3;
  }

  // Load Configuration files
  if(!environment.LoadConfiguration(fh,
				    cls.RunSettingsFile(),
				    cls.PhysicsSettingsFile(),
				    cls.ECSettingsFile(),
				    cls.InstructionSetSettingsFile())) {
    cerr << "ERROR: Failed to load configuration files." << endl;
    return -4;
  }

  // Process command line
  // Command line values override configuration values, so we process those
  // last.
  if(!cls.AddRules(environment.GetPhysicsSettings().Rules()) ||
     !cls.AddRules(environment.GetRunSettings().Rules()) ||
     !cls.AddRules(environment.GetECSettings().Rules())) {
    cerr << "Could not get rules to parse command line." << endl;
    return -5;
  }

  if(!cls.ProcessSets(argc, argv)) {
    cerr << "Could not process command line." << endl;
    return -6;
  }

  if(!environment.InitializeExperiment(fh)) {
    cerr << "ERROR: Unable to initialize the experiment." << endl;
    return -7;
  }

  if(!environment.Run(fh)) {
    return -8;
  }

  environment.Shutdown(fh);

  return 0;
}
