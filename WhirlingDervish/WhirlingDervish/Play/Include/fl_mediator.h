#ifndef WDMEDIATOR
#define WDMEDIATOR

#include <iostream>

#include "WD_menubar.h"

#include "listners.h"

using namespace std;

class WD_mediator: public WD_MenuBarListner{
 public:
  void callFunc(int c);

 private:
  

};


#endif
