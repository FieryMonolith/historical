/*

 * Copyright 2009 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * LICENSE information in the license.txt file or at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __Pointer_h__
#define __Pointer_h__

/*

 * This code is intended to provide functionality to ease certain programming 
 *   issues related to using pointers.

 */

#include "QuantityUnits.h"

namespace nType
{

  // typdef: uint_ptr - unsigned value capable of holding an address
  //
  // The following template and specializations for PointerType are used to
  //   specify a type called uint_ptr.  uint_ptr is a type that can hold
  //   all possible pointer addresses as an unsigned value.  By specializing 
  //   the template, I can specify the size of the void* to determine what 
  //   the actual size of the uint_ptr should be.
  template <int N> class PointerType { public: typedef nType::uint32 Type; };
  template <> class PointerType<1>   { public: typedef nType::uint8 Type;  };
  template <> class PointerType<2>   { public: typedef nType::uint16 Type; };
  template <> class PointerType<4>   { public: typedef nType::uint32 Type; };
  template <> class PointerType<8>   { public: typedef nType::uint64 Type; };

  typedef PointerType<sizeof(void*)>::Type uint_ptr;

} // namespace nType

#endif // __Pointer_h__
