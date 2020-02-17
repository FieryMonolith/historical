/*

 * Copyright 2009 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * LICENSE information in the license.txt file or at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * This program tests the specializations of the ReverseBytes functionality.
 *   The specializations are for a few standard built in types.
 
 */

#include <iostream>

#include "Endian_Specializations.h"

using namespace std;
using namespace nType;

int main(void)
{
  // Test the default ReverseBytes functionality.
  cout << "Testing ReverseBytes(out, in)" << endl;

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

  cout << endl;

  cout << "Testing ReverseBytes(inout) equivalent" << endl;

  int i3=0xaabbccdd;
  cout << "int:\t";
  cout.width(16);
  cout << right << hex << static_cast<unsigned int>(i3) << left << " - ";
  Endian::ReverseBytes(i3, i3);
  cout << static_cast<unsigned int>(i3) << endl;

  unsigned int ui3=0xaabbccdd;
  cout << "unsigned int:\t";
  cout << right << hex << ui3 << left << " - ";
  Endian::ReverseBytes(ui3, ui3);
  cout << ui3 << endl;

  int64 b3=0xaabbccdd11223344;
  cout << "int64:\t";
  cout.width(16);
  cout << right << hex << b3 << left << " - ";
  Endian::ReverseBytes(b3, b3);
  cout << b3 << endl;

  uint64 c3=0xaabbccdd11223344;
  cout << "uint64:\t";
  cout.width(16);
  cout << right << hex << c3;
  Endian::ReverseBytes(c3, c3);
  cout << left << " - " << c3 << endl;

  return 0;
}
