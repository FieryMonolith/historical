#include "setting.h"

using namespace std;

void Setting::Destroy(void) {
  map<string, SettingParser::InputBase *>::iterator i;
  for(i=d_rules.begin(); i!=d_rules.end(); ++i) {
    if(i->second != NULL) {
      i->second->Release();
      i->second = NULL;
    }
  }

  d_rules.clear();

  return;
}
