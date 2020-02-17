#include <iostream>

#include "Endian.h"

using namespace std;

int main(void) {
  cout << "Is this machine big endian?    ... ";
  cout << boolalpha << Endian::IsBigEndian();
  cout << endl;

  cout << "Is this machine little endian? ... ";
  cout << boolalpha << Endian::IsLittleEndian();
  cout << endl;

  return 0;
}
