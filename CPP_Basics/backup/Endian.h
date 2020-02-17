/*

 * Copyright 2006 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * LICENSE information at the bottom of this file or the copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __Endian_h__
#define __Endian_h__

#include "QuantityUnits.h"

class Endian {
private:
  ~Endian(void);

public:
  static const bool IsBigEndian(void)
  {
    static const word eWord = 0xaabb;
    static const bool lIsBigEndian = *(byte *)&eWord == 0xaa;
    return lIsBigEndian;
  }

  static const bool IsLittleEndian(void) {
    static const word eWord = 0xaabb;
    static const bool lIsLittleEndian = *(byte *)&eWord == 0xbb;
    return lIsLittleEndian;
  }

  // These reverse bytes for non-array data
  template <typename T>
  static void ReverseBytes(T &out, const T &in)
  {
    byte *input  = (byte *)&in;
    byte *output = (byte *)&out;
    for(unsigned int i=0, j=sizeof(T)-1; i<sizeof(T); i++, j--) {
      output[j] = input[i];
    }

    return;
  }

  // These reverse bytes for non-array data
  template <typename T>
  static void ReverseBytes(T &inout)
  {
    byte *in = (byte *)&inout;
    for(unsigned int i=0, j=sizeof(T)-1; i<j; i++, j--) {
      byte b = in[i];
      in[i] = in[j];
      in[j] = b;
    }

    return;
  }
};

#endif

/*

 * Boost Software License - Version 1.0 - August 17th, 2003

 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:

 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.

 */
