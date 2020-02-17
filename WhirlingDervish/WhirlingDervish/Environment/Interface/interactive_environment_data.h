#ifndef __INTERACTIVE_ENVIRONMENT_DATA_H__
#define __INTERACTIVE_ENVIRONMENT_DATA_H__

#include "defs.h"

class InteractiveEnvironmentData {
public:
  InteractiveEnvironmentData(void) : d_selectedId(0), d_selected(false),
                                     d_pause(false) { return; }
  ~InteractiveEnvironmentData(void) { return; }

  int64 d_selectedId;
  bool d_selected;
  bool d_pause;
};

#endif
