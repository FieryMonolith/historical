/*

 * Copyright 2006, 2009 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * LICENSE information in the license.txt file or at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __Endian_h__
#define __Endian_h__

/*

 * class Endian
 *
 * Endian - Endian is s class of a static methods for use in determining 
 *   the endianness of a machine and providing helper functionality to swap 
 *   the bytes for any type.  The class is not intended to be instantiated, 
 *   but the destructor is protected to allow others to extend the class.

 * The methods that determine endianness are intended to be fast run-time 
 *   checks at the expense of a small data footprint.  It accomplishes this 
 *   by using static variables so the return value is a constant boolean.  
 *   To avoid a small time expense caused by the initialization of the 
 *   static variables, I recommend priming the functions at the beginning 
 *   of the program if they will be heavily used.

 * The methods for byte swapping are generic and can handle types of any 
 *   sizes.  There are not intended for use with arrays or data streams 
 *   explicitly.  There are two methods available for different needs.  One 
 *   takes two inputs one that needs to be swapped and one to hold the 
 *   result.  The other swaps the bytes of the input.

 * One potential issue with this class is handle bi-endian machines if they 
 *   allow the switching of endianness while the machine runs.  As the 
 *   methods are written for speed using static variables the programmer 
 *   will be unable to change those variables while the program is running.

 * This does not work for bi-endian machines.

 */

// QuantityUnits holds an number of platform specific typedefs that 
//   specify built-in types in terms of specific sizes.
#include "QuantityUnits.h"

class Endian
{
protected:
  ~Endian(void) { return; }

public:
  // This method returns true if the machine is big endian.  You can tell 
  //   a machine is big endian if the most significant byte is first.  
  //   Casting the word to a byte gives you the first byte in the word.
  static const bool IsBigEndian(void)
  {
    static const nType::word eWord = 0xaabb;
    static const bool isBigEndian = *(nType::byte *)&eWord == 0xaa;
    return isBigEndian;
  }

  // This method returns true if the machine is little endian.  You can tell 
  //   a machine is little endian if the least significant byte is first.  
  //   Casting the word to a byte gives you the first byte in the word.
  static const bool IsLittleEndian(void)
  {
    static const nType::word eWord = 0xaabb;
    static const bool isLittleEndian = *(nType::byte *)&eWord == 0xbb;
    return isLittleEndian;
  }

  // This method reverses bytes for any type.  It does not handle array 
  //   data or streams explicitly.  The input is two references one that 
  //   will hold the resulting reversed bytes of the second input variable.
  template <typename T>
  static void ReverseBytes(T &out, const T &in)
  {
    nType::byte *input  = (nType::byte *)&in;
    nType::byte *output = (nType::byte *)&out;

    // Loop over all the bytes of the input and add them in reverse order
    //   to the output.
    for(nType::dword i=0, j=sizeof(T)-1; i<sizeof(T); i++, j--)
    {
      output[j] = input[i];
    }

    return;
  }

  // This method reverses bytes for any type.  It does not handle array 
  //   data or streams explicitly.  The input is a single reference that 
  //   will have it's data reversed.
  template <typename T>
  static void ReverseBytes(T &inout)
  {
    nType::byte *in = (nType::byte *)&inout;
    nType::byte b = 0;

    // Loop over the first half of input's bytes and swap them with the 
    //   symmetrical equivalent on the other end.  If the type has an 
    //   odd number of bytes, the middle byte will not be swapped.
    for(nType::dword i=0, j=sizeof(T)-1; i<j; i++, j--)
    {
      b = in[i];
      in[i] = in[j];
      in[j] = b;
    }

    return;
  }
}; // class Endian

#endif // __Endian_h__
