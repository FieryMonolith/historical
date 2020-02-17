/*

 * Copyright 2009 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * LICENSE information in the license.txt file or at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * This program tests the functionality of the Endian class.  It begins 
 *   by testing the ability to determine the endianness of the machine.
 *   It then tests the default reversing methods.
 
 */

#include <iostream>

#include "Endian.h"

using namespace std;
using namespace nType;

// class A is used a user-defined type larger than the built-in types.
class A
{
public:
  dword d1;
  dword d2;
  dword d3;
  dword d4;
  dword d5;
};

int main(void)
{
  // Test the ability to determine the endianness of the machine.
  cout << "Testing endianness." << endl;
  cout << "Is this machine big endian?    ... ";
  cout << boolalpha << Endian::IsBigEndian();
  cout << endl;

  cout << "Is this machine little endian? ... ";
  cout << boolalpha << Endian::IsLittleEndian();
  cout << endl;

  cout << endl;

  // Test the default ReverseBytes functionality.
  cout << "Testing ReverseBytes(out, in)" << endl;

  byte B2, B1=0xab;
  Endian::ReverseBytes(B2, B1);
  cout << "byte:\t";
  cout.width(16);
  cout << right << hex << static_cast<unsigned int>(B1);
  cout << left << " - " << static_cast<unsigned int>(B2) << endl;

  word w2, w1=0xaabb;
  Endian::ReverseBytes(w2, w1);
  cout << "word:\t";
  cout.width(16);
  cout << right << hex << w1 << left << " - " << w2 << endl;

  dword d2, d1=0xaabbccdd;
  Endian::ReverseBytes(d2, d1);
  cout << "dword:\t";
  cout.width(16);
  cout << right << hex << d1 << left << " - " << d2 << endl;

  qword q2, q1=0xaabbccdd11223344;
  Endian::ReverseBytes(q2, q1);
  cout << "qword:\t";
  cout.width(16);
  cout << right << hex << q1 << left << " - " << q2 << endl;

  int i2, i1=0xaabbccdd;
  Endian::ReverseBytes(i2, i1);
  cout << "int:\t";
  cout.width(16);
  cout << right << hex << static_cast<unsigned int>(i1) << left << " - ";
  cout << static_cast<unsigned int>(i2) << endl;

  unsigned int ui2, ui1=0xaabbccdd;
  Endian::ReverseBytes(ui2, ui1);
  cout << "unsigned int:\t";
  cout << right << hex << ui1 << left << " - " << ui2 << endl;

  int64 b2, b1=0xaabbccdd11223344;
  Endian::ReverseBytes(b2, b1);
  cout << "int64:\t";
  cout.width(16);
  cout << right << hex << b1 << left << " - " << b2 << endl;

  uint64 c2, c1=0xaabbccdd11223344;
  Endian::ReverseBytes(c2, c1);
  cout << "uint64:\t";
  cout.width(16);
  cout << right << hex << c1 << left << " - " << c2 << endl;

  A a2, a1;
  a1.d1 = 0xaabbccdd;
  a1.d2 = 0xeeff1122;
  a1.d3 = 0x33445566;
  a1.d4 = 0x77889910;
  a1.d5 = 0xaabbccdd;
  Endian::ReverseBytes(a2, a1);
  cout << "class A:" << endl;
  cout << hex << a1.d1 << a1.d2 << a1.d3 << a1.d4 << a1.d5 << endl;
  cout << hex << a2.d1 << a2.d2 << a2.d3 << a2.d4 << a2.d5 << endl;

  cout << endl;

  cout << "Testing ReverseBytes(inout)" << endl;

  byte B3=0xab;
  cout << "byte:\t";
  cout.width(16);
  cout << right << hex << static_cast<unsigned int>(B3) << left << " - ";
  Endian::ReverseBytes(B3);
  cout << static_cast<unsigned int>(B3) << endl;

  word w3=0xaabb;
  cout << "word:\t";
  cout.width(16);
  cout << right << hex << w3 << left << " - ";
  Endian::ReverseBytes(w3);
  cout << w3 << endl;

  dword d3=0xaabbccdd;
  cout << "dword:\t";
  cout.width(16);
  cout<< right << hex << d3 << left << " - ";
  Endian::ReverseBytes(d3);
  cout << d3 << endl;

  qword q3=0xaabbccdd11223344;
  cout << "qword:\t";
  cout.width(16);
  cout<< right << hex << q3 << left << " - ";
  Endian::ReverseBytes(q3);
  cout << q3 << endl;

  int i3=0xaabbccdd;
  cout << "int:\t";
  cout.width(16);
  cout << right << hex << static_cast<unsigned int>(i3) << left << " - ";
  Endian::ReverseBytes(i3);
  cout << static_cast<unsigned int>(i3) << endl;

  unsigned int ui3=0xaabbccdd;
  cout << "unsigned int:\t";
  cout << right << hex << ui3 << left << " - ";
  Endian::ReverseBytes(ui3);
  cout << ui3 << endl;

  int64 b3=0xaabbccdd11223344;
  cout << "int64:\t";
  cout.width(16);
  cout << right << hex << b3 << left << " - ";
  Endian::ReverseBytes(b3);
  cout << b3 << endl;

  uint64 c3=0xaabbccdd11223344;
  cout << "uint64:\t";
  cout.width(16);
  cout << right << hex << c3;
  Endian::ReverseBytes(c3);
  cout << left << " - " << c3 << endl;

  A a3;
  a3.d1 = 0xaabbccdd;
  a3.d2 = 0xeeff1122;
  a3.d3 = 0x33445566;
  a3.d4 = 0x77889910;
  a3.d5 = 0xaabbccdd;
  cout << "class A: " << endl;
  cout << hex << a3.d1 << a3.d2 << a3.d3 << a3.d4 << a3.d5 << endl;
  Endian::ReverseBytes(a3);
  cout << hex << a3.d1 << a3.d2 << a3.d3 << a3.d4 << a3.d5 << endl;

  return 0;
}
