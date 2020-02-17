/*

 * Copyright 2009 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * LICENSE information in the license.txt file or at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __Endian_SSSE3_h__
#define __Endian_SSSE3_h__

/*

 * This is a specialization of the ReverseBytes(inout) method in the Endian 
 *   class.  It accepts a SSSE3_Data as input.  The data holds a pointer 
 *   to consecutive four byte units and the now many units are to be
 *   processed.  There is no verification of the data.  Once the function 
 *   has the input, it walks it's way through the data working on four 
 *   units at once.  Each four unit block is processed by the SIMD 
 *   extensions simultaneously.  When processed it reverses the bytes of 
 *   each unit, but leaves the unit order the same.

 * During design the option existed to extend the Endian class instead of 
 *   specializing the ReverseBytes method.  This would have allowed the 
 *   programmer to give access to the date directly versus the current 
 *   approach that utilizes a class to hold the data as it is passed to 
 *   the method.  However, I felt the small overhead was worth the 
 *   reduction in complexity.  Thus to trick the template into accepting 
 *   a stream of four byte units, they had to be wrapped in a class.

 */

#include <cstring>

#include "Endian.h"

#include "SSSE3.h"
#include "Alignment.h"
#include "Pointer.h"

// This class holds the data to be processed and is used to specify when 
//   to use the given ReverseBytes for a stream of four byte units.
class SSSE3_Data
{
public:
  nType::byte *data;
  nType::dword totalBytes;
};

// ReverseBytes is a specialized version for the Endian class that swaps 
//   bytes using the SSSE3 instructions.  The input holds a stream of bytes 
//   that correspond to an array whose units are four bytes each.  The 
//   stream of bytes does not have to start on a sixteen byte boundary as 
//   the function will handle all cases where the beginning does not 
//   align and the end does not fill a full sixteen byte chunk.  The 
//   function only swaps the bytes within each four byte unit, but does 
//   not swap the units themselves.
template <>
void Endian::ReverseBytes<SSSE3_Data>(SSSE3_Data &inout)
{
  // This is a static variable that specifies the order in which the 
  //   bytes will be swapped.  It is set up to reverse the bytes for 
  //   each of the four four byte units, but does not affect their 
  //   order relative to each other.
  static DATA_ALIGN(const nType::byte sortOrder[16], 16) = 
    { 0x03, 0x02, 0x01, 0x00, 0x07, 0x06, 0x05, 0x04,
      0x0b, 0x0a, 0x09, 0x08, 0x0f, 0x0e, 0x0d, 0x0c };

  // Create an aligned storage that can be used for special cases
  DATA_ALIGN(nType::byte partial[16], 16);

  // Data used in this function.
  // data points to the start of the input stream.  Not only is this 
  //   variable modified to traverse the stream, but is required to 
  //   pass the data to the XMM register.  Inline assembly does not 
  //   explicitly accept data that is a reference.
  nType::byte *data = inout.data;
  // Holds the memory address for the start of the stream.
  nType::uint_ptr address = reinterpret_cast<nType::uint_ptr>(data);
  // Holds the memory address for the first byte after the stream.
  nType::uint_ptr end = address + inout.totalBytes;
  // This variable is used to determine how to align data at the start 
  //   and end of the stream.
  nType::uint_ptr delta = address % 16;

  // If delta is not zero, then the start of the stream does not fall on 
  //   a sixteen byte boundary.
  if(delta != 0)
  {
    // Since delta holds the spacer needed at the beginning, 16-delta is 
    //   the quantity of bytes that need swapping.
    delta = 16 - delta;
    // Make sure that we only process the necessary amount of data.
    delta = (inout.totalBytes < delta) ? inout.totalBytes : delta;
    // Copy the unaligned data into an aligned buffer.
    memcpy(partial, data, delta);
    // Swap the bytes for each four byte unit.
    SSSE3_Swap(partial, sortOrder);
    // Retrieve swapped data from aligned buffer
    memcpy(data, partial, delta);

    // Move the data pointer and address ahead by the ammount processed.
    data    += delta;
    address += delta;
  }

  // Process all sixteen byte aligned units
  while(address + 16 <= end)
  {
    SSSE3_Swap(data, sortOrder);
    data    += 16;
    address += 16;
  }

  // Process any remaining data that is smaller than sixteen bytes in 
  //   size.
  if(address < end)
  {
    // Compute how much data to process
    delta = end - address;
    // Move data into aligned, sixteen byte buffer
    memcpy(partial, data, delta);
    // Swap bytes for each four byte unit.
    SSSE3_Swap(partial, sortOrder);
    // Retrieve data from aligned buffer
    memcpy(data, partial, delta);
  }

  return;
}

#endif // __Endian_SSE_h__
