#include <iostream>
#include <qapplication.h>
#include <qgl.h>

#include "qt_main_window.h"
#include "interactive_environment.h"
#include "language_engine.h"
#include "file_handler.h"
#include "command_line_settings.h"

using namespace std;

int main(int argc, char **argv) {
  QApplication a(argc, argv);

  if(!QGLFormat::hasOpenGL()) {
    qWarning( "This system has no OpenGL support. Exiting." );
    return -1;
  }

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
  InteractiveEnvironment ie(0);
  if(!ie.Initialize()) {
    cerr << "ERROR: Failed to initialize the environment." << endl;
    return -3;
  }

  // Load Configuration files
  if(!ie.LoadConfiguration(fh, cls.RunSettingsFile(),
			   cls.PhysicsSettingsFile(),
			   cls.ECSettingsFile(),
			   cls.InstructionSetSettingsFile())) {
    cerr << "ERROR: Failed to load configuration files." << endl;
    return -4;
  }

  // Process command line
  // Command line values override configuration values, so we process those
  // last.
  if(!cls.AddRules(ie.GetPhysicsSettings().Rules()) ||
     !cls.AddRules(ie.GetRunSettings().Rules()) ||
     !cls.AddRules(ie.GetECSettings().Rules())) {
    cerr << "Could not get rules to parse command line." << endl;
    return -5;
  }

  if(!cls.ProcessSets(argc, argv)) {
    cerr << "Could not process command line." << endl;
    return -6;
  }

  ie.GetRunSettings().PopulationSize() = 1;
  ie.GetRunSettings().StartState() = 3;

  if(!ie.InitializeExperiment(fh)) {
    cerr << "ERROR: Unable to initialize the experiment." << endl;
    return -7;
  }

  MainWindow mw(ie, fh);
  mw.setGeometry(100, 100, 550, 600);
  a.setMainWidget(&mw);

  mw.show();

  return a.exec();
}
