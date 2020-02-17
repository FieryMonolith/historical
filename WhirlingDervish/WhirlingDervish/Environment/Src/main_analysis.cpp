#include <iostream>
#include <string>
#include <sstream>

#include "environment_analysis.h"
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
    cerr << "ERROR: Could not process command line." << endl;
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
    cerr << "ERROR: Could not process command line." << endl;
    return -6;
  }

  environment.GetRunSettings().PopulationSize() = 1;
  environment.GetRunSettings().StartState() = 3;

  if(!environment.InitializeExperiment(fh)) {
    cerr << "ERROR: Unable to initialize the experiment." << endl;
    return -7;
  }

  char line[1024];
  int which = 0;
  for(int i=0; i<argc; i++) {
    string arguement = argv[i];

    if(which == 1) {
      //cout << "Running file " << arguement << endl;
      if(!environment.LoadCreature(fh, arguement) || !environment.Run(fh)) {
	cerr << "ERROR: Could not load creature or failed to run." << endl;
	return -8;
      }
    } else if(which == 2) {
      int fileId = fh.Create(arguement, ios::in);
      if(fileId < 0) {
	cerr << "ERROR: ( " << arguement; 
	cerr << " ) File doesn't exist, or failed to open. " << endl;
	return -9;
      }
      bool success = false;
      istream &in = fh.InputStream(fileId, success);
      if(!success) {
	cerr << "ERROR: Could not get file stream." << endl;
	return -10;
      }
      // Remove intro lines that are not necessary for this.
      in.getline(line, 1023, '\n');
      in.getline(line, 1023, '\n');
      in.getline(line, 1023, '\n');

      int counter = 1;
      while(!in.eof()) {
	unsigned int position = in.tellg();

	in.getline(line, 1023, '\n');
	stringstream linestream;
	linestream << line;

	string testLine;
	linestream >> testLine;

	if(testLine == "CREATURE_START") {
	  in.seekg(position);
	  cout << counter << " ";
	  if(!environment.LoadCreatureFromGeneration(fh, fileId) ||
	     !environment.Run(fh)) {
	    cerr << "ERROR: LoadCreatureFromGeneration" << endl;
	    return false;
	  }
	} else if(testLine == "GENERATION_END") {
	  break;
	}
	counter++;
      }

      fh.Close(fileId);
    }

    if(arguement == "-l") {
      which = 1;
    } else if(arguement == "-g") {
      which = 2;
    }
  }

  environment.Shutdown(fh);

  return 0;
}
