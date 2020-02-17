#include <iostream>

#include "tcp_error.h"

using namespace std;

void DieWithError(char *errorMessage) {
  cout << "ERROR: " << errorMessage << endl;
  cout << "Aborting" << endl;

  return;
}
