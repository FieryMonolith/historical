/*

 * Copyright 2009 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * LICENSE information in the license.txt file or at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

/*

 * This program tests the specialization of the ReverseBytes(inout) 
 *   functionality.  The specialization is for a stream whose type has a 
 *   unit size of four bytes.  

 * The test first reverses the bytes of an eleven dword stream.  It then goes 
 *   through and reverses the bytes for varying stream sizes from 1 to 9.  
 *   For each size it processes the stream twice: once starting at the first 
 *   element and once at the second element.  The 2x processing ensures that 
 *   the process is tested on data that are not aligned to sixteen byte
 *   boundaries.  If it works for unaligned data, it will work for aligned 
 *   data.  Plus it checks the special case of starting on an unaligned 
 *   boundary.  The stream sizes also test the special case where extra 
 *   data exists at the end of the stream that does not take up an entire 
 *   sixteen byte chunk.
 
 */

#include <iostream>

//#define _USE_SSSE3_ASM
#define _USE_SSSE3_INTRINSICS
#include "Endian_SSSE3.h"

using namespace std;
using namespace nType;

// Print out each dword separated by a space.
void Print(dword *values, dword quantity);

int main(void)
{
  cout << "Test SSSE3 specialization" << endl << endl;

  SSSE3_Data sd1;

  // Create the data
  const dword size = 12;
  dword sd[size];
  sd[0]  = 0x10111213;
  sd[1]  = 0x14151617;
  sd[2]  = 0x18191a1b;
  sd[3]  = 0x1c1d1e1f;
  sd[4]  = 0x20212223;
  sd[5]  = 0x24252627;
  sd[6]  = 0x28292a2b;
  sd[7]  = 0x2c2d2e2f;
  sd[8]  = 0x30313233;
  sd[9]  = 0x34353637;
  sd[10] = 0x38393a3b;
  sd[11] = 0x3c3d3e3f;

  cout << "Reverse all eleven dwords." << endl;
  // Prepare the SSSE3_Data structure.
  sd1.data = reinterpret_cast<byte*>(sd);
  sd1.totalBytes = sizeof(dword) * size;
  Print(sd, size);
  Endian::ReverseBytes(sd1);
  Print(sd, size);

  cout << endl;

  // Loop through nine different stream sizes.
  for(dword i=1; i<=9; ++i)
  {
    // Process the stream starting with the first four byte unit.
    cout << "Reverse " << i << "  --  Start " << 0 << endl;
    // Prepare the SSSE3_Data structure.
    sd1.data = reinterpret_cast<byte*>(&sd[0]);
    sd1.totalBytes = sizeof(dword) * i;
    Print(&sd[0], i);
    Endian::ReverseBytes(sd1);
    Print(&sd[0], i);

    cout << endl;

    // Process the stream starting with the second four byte unit.
    cout << "Reverse " << i << "  --  Start " << 1 << endl;
    // Prepare the SSSE3_Data structure.
    sd1.data = reinterpret_cast<byte*>(&sd[1]);
    sd1.totalBytes = sizeof(dword) * i;
    Print(&sd[1], i);
    Endian::ReverseBytes(sd1);
    Print(&sd[1], i);

    cout << endl;
  }

  return 0;
}

// Print out each dword separated by a space.
void Print(dword *values, dword quantity)
{
  for(dword i=0; i<quantity; ++i)
  {
    cout << hex << values[i] << " ";
  }
  cout << endl;
  return;
}
