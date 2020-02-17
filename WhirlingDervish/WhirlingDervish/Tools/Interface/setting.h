#ifndef __SETTING_H__
#define __SETTING_H__

#include <string>
#include <map>

#include "setting_parser.h"

class Setting {
public:
  Setting(void) { return; }
  virtual ~Setting(void) { Destroy(); return; }

  virtual bool Initialize(void)=0;

  const std::map<std::string, SettingParser::InputBase *> &Rules(void) const {
    return d_rules;
  }

protected:
  std::map<std::string, SettingParser::InputBase *> d_rules;

  void Destroy(void);
};

#endif
