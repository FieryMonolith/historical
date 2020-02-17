#ifndef WDFLTKMENUBAR
#define WDFLTKMENUBAR

#include <Fl/Fl_Menu_Bar.h>
#include <Fl/Fl_Menu_Item.h>

#include <vector>
#include <iostream>

#include "listners.h"

using namespace std;

class WD_menubar: public Fl_Menu_Bar
{
 public:
  WD_menubar():Fl_Menu_Bar(0,0,500,25,"")
  {
    Fl_Menu_Item temp[] = 
    {
      {"File", 0, 0, 0, FL_SUBMENU, 0, FL_HELVETICA, 12},
        {"&Open", 0, menucallback, (void*)1, FL_MENU_DIVIDER, 0, FL_HELVETICA, 12},
        {"E&xit", 0, menucallback, (void*)2, 0, 0, FL_HELVETICA, 12},
        {0},
        {"Creature", 0, 0, 0, FL_SUBMENU, 0, FL_HELVETICA, 12},
        {"&Start", 0, menucallback, (void*)3, 0, 0, FL_HELVETICA, 12},
        {"&Freeze", 0,menucallback, (void*)4, 0, 0, FL_HELVETICA, 12},
        {"&Reset", 0, menucallback, (void*)5, 0, 0, FL_HELVETICA, 12},
        {"&Edit", 0,  menucallback, (void*)6, 0, 0, FL_HELVETICA, 12},
        {0},
        {"Help", 0, 0, 0, FL_SUBMENU, 0, FL_HELVETICA, 12},
        {"About", 0, menucallback, (void*)7, 0, 0, FL_HELVETICA, 12},
        {0},
      {0}
    };

    //memset(file, 0x00, sizeof(Fl_Menu_Item) * 14);
    
    //cerr << "size of temp " << sizeof(Fl_Menu_Item) << endl;

    file = new Fl_Menu_Item[sizeof(temp) / 14];
    //cerr << "size of file " << sizeof(*file) << endl;
    memcpy(file, temp, sizeof(temp));
    menu(file);
  }

  friend void menucallback(Fl_Widget*, void*);
  void registerListner(WD_MenuBarListner*);

 private:
  Fl_Menu_Item *file;
  vector<WD_MenuBarListner*> listners;
};


#endif
