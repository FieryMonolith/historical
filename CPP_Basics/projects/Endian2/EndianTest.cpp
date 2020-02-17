#include <iostream>

#include "Endian_Unwrapped.h"

using namespace std;
using namespace nType;

class A
{
public:
  dword d1;
  dword d2;
  dword d3;
  dword d4;
};

int main(void) {
  cout << "Is this machine big endian?    ... ";
  cout << boolalpha << Endian::IsBigEndian();
  cout << endl;

  cout << "Is this machine little endian? ... ";
  cout << boolalpha << Endian::IsLittleEndian();
  cout << endl;

  word w1, w2=0xaabb;
  Endian::ReverseBytes(w1, w2);
  cout << hex << w1 << " : " << w2 << endl;

  dword d1, d2=0xaabbccdd;
  Endian::ReverseBytes(d1, d2);
  cout << hex << d1 << " : " << d2 << endl;

  int i1, i2=0xaabbccdd;
  Endian::ReverseBytes(i1, i2);
  cout << hex << (unsigned int)i1 << " : " << (unsigned int)i2 << endl;

  unsigned int ui1, ui2=0xaabbccdd;
  Endian::ReverseBytes(ui1, ui2);
  cout << hex << ui1 << " : " << ui2 << endl;

  qword q1, q2=0xaabbccdd11223344;
  Endian::ReverseBytes(q1, q2);
  cout << hex << q1 << " : " << q2 << endl;

  int64 b1, b2=0xaabbccdd11223344;
  Endian::ReverseBytes(b1, b2);
  cout << hex << b1 << " : " << b2 << endl;

  uint64 c1, c2=0xaabbccdd11223344;
  Endian::ReverseBytes(c1, c2);
  cout << hex << c1 << " : " << c2 << endl;

  A a1, a2;
  a2.d1 = 0xaabbccdd;
  a2.d2 = 0xeeff1122;
  a2.d3 = 0x33445566;
  a2.d4 = 0x77889910;
  Endian::ReverseBytes(a1, a2);
  cout << hex << a1.d1 << a1.d2 << a1.d3 << a1.d4 << " : ";
  cout << hex << a2.d1 << a2.d2 << a2.d3 << a2.d4 << endl;

  return 0;
}
