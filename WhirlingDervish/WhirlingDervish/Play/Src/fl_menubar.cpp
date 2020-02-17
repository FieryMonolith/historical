#include <assert.h>

#include "WD_menubar.h"

using namespace std;

void menucallback(Fl_Widget* w, void* v){

  WD_menubar *target = dynamic_cast<WD_menubar*>(w);
  assert(target != 0);
  for(int i = 0; i < target->listners.size(); i++){
    target->listners.at(i)->callFunc((int)v);
  }
}



void WD_menubar::registerListner(WD_MenuBarListner* MBL){
  listners.push_back( MBL );
}




