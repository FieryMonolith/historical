#ifndef WDFLTKWINDOW
#define WDFLTKWINDOW

#include <Fl/Fl_Window.h>

#include "WD_menubar.h"
#include "WD_mediator.h"

using namespace std;

class WD_Window : public Fl_Window{

 public:
  WD_Window(int w, int h, WD_mediator* med):Fl_Window(w, h, "WhirilingDervish")
  {
    m = new WD_menubar();
    add(m);
    
    m->registerListner(med);
  }

 private:
  WD_menubar *m;

};


#endif
