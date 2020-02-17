#include "WD_mediator.h"


void WD_mediator::callFunc(int c){
  switch(c)
    {
    case 0x1:
      cerr << "open" << endl;
      break;
    case 0x2:
      exit(1);
      break;
    case 0x3:
      cerr << "Start" << endl;
      break;
    case 0x4:
      cerr << "Freeze" << endl;
      break;
    case 0x5:
      cerr << "Reset" << endl;
      break;
    case 0x6:
      cerr << "Edit" << endl;
      break;
    case 0x7:
      cerr << "About" << endl;
      break;
    }
}
