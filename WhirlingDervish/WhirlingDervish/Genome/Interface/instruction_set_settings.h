#ifndef __INSTRUCTION_SET_SETTINGS_H__
#define __INSTRUCTION_SET_SETTINGS_H__

#include <string>
#include <vector>

#include "setting.h"

class InstructionSetSettings {
public:
  InstructionSetSettings(void);
  ~InstructionSetSettings(void);

  bool Initialize(void);
  bool Setup(const std::string &filename);

  const std::vector<std::string> &Set(void) const { return d_set; }

private:
  std::vector<std::string> d_set;
};

#endif
