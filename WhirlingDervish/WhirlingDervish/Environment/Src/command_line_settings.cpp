#include <sstream>

#include "command_line_settings.h"

using namespace std;

const string CommandLineSettings::n_runSettingsFile("-run");
const string CommandLineSettings::n_ecSettingsFile("-ec");
const string CommandLineSettings::n_instructionSetSettingsFile("-iss");
const string CommandLineSettings::n_physicsSettingsFile("-physics");
const string CommandLineSettings::n_set("-set");

CommandLineSettings::CommandLineSettings(void)
  : d_runSettingsFile("run_settings.cfg"),
    d_ecSettingsFile("ec_settings.cfg"),
    d_instructionSetSettingsFile("instruction_set.cfg"),
    d_physicsSettingsFile("physics_settings.cfg") {
  return;
}

CommandLineSettings::~CommandLineSettings(void) {
  return;
}

bool CommandLineSettings::Initialize(void) {
  Destroy();

  SettingParser::InputBase *ib;

  // CommandLineSettings::RunSettingsFile
  ib = new SettingParser::Input<string &, CommandLineSettings>(
	     n_runSettingsFile, this, &CommandLineSettings::RunSettingsFile);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_runSettingsFile, ib));

  // CommandLineSettings::ECSettingsFile
  ib = new SettingParser::Input<string &, CommandLineSettings>(
	     n_ecSettingsFile, this, &CommandLineSettings::ECSettingsFile);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_ecSettingsFile, ib));

  // CommandLineSettings::InstructionSetSettingsFile
  ib = new SettingParser::Input<string &, CommandLineSettings>(
	     n_instructionSetSettingsFile, this,
	     &CommandLineSettings::InstructionSetSettingsFile);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_instructionSetSettingsFile, ib));

  // CommandLineSettings::PhysicsSettingsFile
  ib = new SettingParser::Input<string &, CommandLineSettings>(
       n_physicsSettingsFile, this, &CommandLineSettings::PhysicsSettingsFile);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_physicsSettingsFile, ib));

  // CommandLineSettings::Set
  ib = new SettingParser::Input<CommandLineSettings &, CommandLineSettings>(
	     n_set, this, &CommandLineSettings::Set);
  if(ib == NULL) { Destroy(); return false; }
  d_rules.insert(make_pair(n_set, ib));

  return true;
}

bool CommandLineSettings::AddRules(
		   const map<string, SettingParser::InputBase *> &rules) {
  map<string, SettingParser::InputBase *>::const_iterator i, loc;

  for(i=rules.begin(); i!= rules.end(); ++i) {
    if(i->second == NULL) { return false; }

    loc = d_rules.find(i->first);
    if(loc != d_rules.end()) {
      return false;
    }
  }

  for(i=rules.begin(); i!= rules.end(); ++i) {
    i->second->Count()++;
    d_rules.insert(make_pair(i->first, i->second));
  }

  return true;
}

bool CommandLineSettings::ProcessFileInfo(int argc, char **argv) {
  stringstream ss;

  for(int i=1; i<argc; i++) {
    string s(argv[i]);
    if(s == "-set") {
      i += 2;
      continue;
    }
    ss << s << " ";
  }

  if(!SettingParser::ParseStream(ss, d_rules)) {
    return false;
  }

  return true;
}

bool CommandLineSettings::ProcessSets(int argc, char **argv) {
  stringstream ss;

  for(int i=1; i<argc; i++) {
    string s(argv[i]);
    if(s == "-set") {
      ss << s << " ";

      i++;
      if(i >= argc) { return false; }
      ss << argv[i] << " ";

      i++;
      if(i >= argc) { return false; }
      ss << argv[i] << " ";
    }
  }

  if(!SettingParser::ParseStream(ss, d_rules)) {
    return false;
  }

  return true;
}

istream &operator>>(istream &in, CommandLineSettings &c) {
  if(!SettingParser::ParseVariable(in, c.Rules())) {
    in.setstate(ios_base::failbit);
  }

  return in;
}

ostream &operator<<(ostream &out, CommandLineSettings &) {
  return out;
}
