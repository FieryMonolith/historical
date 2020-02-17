#ifndef __COMMAND_LINE_SETTINGS_H__
#define __COMMAND_LINE_SETTINGS_H__

#include <string>

#include "setting.h"

class CommandLineSettings : public Setting {
public:
  CommandLineSettings(void);
  ~CommandLineSettings(void);

  bool Initialize(void);

  bool AddRules(const std::map<std::string, SettingParser::InputBase *> &);

  bool ProcessFileInfo(int argc, char **argv);
  bool ProcessSets(int argc, char **argv);

  const std::string &RunSettingsFile(void) const { return d_runSettingsFile; }
        std::string &RunSettingsFile(void)       { return d_runSettingsFile; }

  const std::string &ECSettingsFile(void) const { return d_ecSettingsFile; }
        std::string &ECSettingsFile(void)       { return d_ecSettingsFile; }

  const std::string &InstructionSetSettingsFile(void) const {
    return d_instructionSetSettingsFile;
  }
  std::string &InstructionSetSettingsFile(void) {
    return d_instructionSetSettingsFile;
  }

  const std::string &PhysicsSettingsFile(void) const {
    return d_physicsSettingsFile;
  }
  std::string &PhysicsSettingsFile(void) { return d_physicsSettingsFile; }

  const CommandLineSettings &Set(void) const { return *this; }
        CommandLineSettings &Set(void)       { return *this; }

private:
  std::string d_runSettingsFile;
  std::string d_ecSettingsFile;
  std::string d_instructionSetSettingsFile;
  std::string d_physicsSettingsFile;

  static const std::string n_runSettingsFile;
  static const std::string n_ecSettingsFile;
  static const std::string n_instructionSetSettingsFile;
  static const std::string n_physicsSettingsFile;
  static const std::string n_set;
};

std::istream &operator>>(std::istream &, CommandLineSettings &);
std::ostream &operator<<(std::ostream &, CommandLineSettings &);

#endif
