/*

 * Copyright 2006 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * LICENSE information at the bottom of this file or the copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __Type_Interface_QuantityBuiltin_h__
#define __Type_Interface_QuantityBuiltin_h__

/*

 * This header defines the standard C++ builtin types in terms of
 *   Quantity.  They are meant to turn builtin types into classes
 *   with the same size footprint and will transparently use global
 *   operators as if they are their internal type.

 * Jason Stredwick March 31, 2006 Finalized first version of builtin
 *   quantities.

 */

#include "Quantity.h"
#include "QuantityUnits.h"

namespace nType
{

  typedef Quantity < byte  > Byte;
  typedef Quantity < word  > Word;
  typedef Quantity < dword > DWord;
  typedef Quantity < qword > QWord;

  typedef Quantity <   int8 > Int8;
  typedef Quantity <  uint8 > UInt8;
  typedef Quantity <  int16 > Int16;
  typedef Quantity < uint16 > UInt16;
  typedef Quantity <  int32 > Int32;
  typedef Quantity < uint32 > UInt32;
  typedef Quantity <  int64 > Int64;
  typedef Quantity < uint64 > UInt64;

  typedef Quantity <   signed char  > Char;
  typedef Quantity < unsigned char  > UChar;
  typedef Quantity <   signed short > Short;
  typedef Quantity < unsigned short > UShort;
  typedef Quantity <   signed int   > Int;
  typedef Quantity < unsigned int   > UInt;
  typedef Quantity <   signed long  > Long;
  typedef Quantity < unsigned long  > ULong;

  typedef Quantity < float > Float;
  typedef Quantity < double > Double;

  typedef Quantity < size_t > Size_t;

  typedef QWord MaxQ; // MAX_Q is the largest builtin unsigned quantity
  typedef DWord SecQ; // SEC_Q is the second largest builtin unsigned quantity

  const Size_t BITS_PER_BYTE         = 8;
  const Size_t BYTES_PER_SIZEOF_UNIT = 1;

} // end namespace nType

#endif // __Type_Interface_QuantityBuiltin_h__

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
