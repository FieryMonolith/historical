/*

 * Copyright 2009 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * LICENSE information in the license.txt file or at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __SSSE3_h__
#define __SSSE3_h__

/*

 * This file is meant to encapsulate code related to the SSSE3 instruction 
 *   set for SIMD.  There are two methods I provide here: assembly and 
 *   intrinsics.  The choice can be made through macro enabling.  If you
 *   want to use assembly use _USE_SSSE3_ASM or use _USE_SSSE3_INTRINSICS 
 *   to use the intrinsics method.  This code assumes that SSSE3 instructions
 *   are available.  I have not yet added the code to detect it's presence,
 *   but when I do it will be up to the user to determine it's availability 
 *   prior to use of these functions.

 * The reason why two alternative approaches exist is due to availability.
 *   Visual Studio will not compile inline assembly in 64bit mode, but it 
 *   does not provide up to date material to use the most recent intrinsics 
 *   at least in VS 2005.  I read that most of the recent(2009) extensions 
 *   are available for version VS2008+.  While I could get the assembly 
 *   working under 32bit windows, I could not get the assembly to work for 
 *   gcc.  However, the intrinsics work for gcc.

 */

#include "QuantityUnits.h"

#if defined(_USE_SSSE3_ASM)

// *** Note: This can only be used with Visual Studio in 32bit mode.
// This function takes two pointers, one to the data in need of byte 
//   swapping and one that specifies the swap order.  This is the 
//   assembly version and uses emit to force the program to call the 
//   pshufb function using the xmm0 and xmm1 registers.  Also, the 
//   function does not work correctly if the pointers are not first 
//   loaded into the EAX register before loading into an XMM register.
// This function also assumes that the data is already sixteen byte aligned.
// I got the idea to use the emit from a forum at
//   http://www.gamedev.net/community/forums/topic.asp?topic_id=457389&whichpage=1&#3018211
// I also found help with the SSE assembly from 
//   http://softpixel.com/~cwright/programming/simd/

inline void SSSE3_Swap(nType::byte *data, const nType::byte *swapOrder)
{
#if defined(_MSC_VER) || defined(VISUAL_STUDIO)
  __asm
  {
    mov eax, swapOrder
    movaps xmm1, [eax]
    mov eax, data
    movaps xmm0, [eax]
    _emit 0x66
    _emit 0x0f
    _emit 0x38
    _emit 0x00
    _emit 0xc1
    movaps [eax], xmm0
  }
#elif defined(__GNUC__)
  asm("movl %0, %%eax" : : "g" (swapOrder));
  asm("movaps (%eax), %xmm1");
  asm ("movl $0, %%eax" : : "g" (data));
  asm ("movaps (%eax), %xmm0");
  asm __volatile__ (".byte 0x66");
  asm __volatile__ (".byte 0x0f");
  asm __volatile__ (".byte 0x38");
  asm __volatile__ (".byte 0x00");
  asm __volatile__ (".byte 0xc1");
  asm("movaps %xmm0, (%eax)");

#else

#error "Unknown compiler type for inline assembly. SSSE3.h"

#endif // _MSC_VER

  return;
}

#elif defined(_USE_SSSE3_INTRINSICS)

// I pieced this code together from the information on the MSDN website.

// tmmintrin contains the _mm_shuffle_epi8 function.
#include <tmmintrin.h>

inline void SSSE3_Swap(nType::byte *data, const nType::byte *swapOrder)
{
  __m128i xmm0 = _mm_load_si128(reinterpret_cast<const __m128i*>(data));
  __m128i xmm1 = _mm_load_si128(reinterpret_cast<const __m128i*>(swapOrder));

  xmm0 = _mm_shuffle_epi8(xmm0, xmm1);

  _mm_store_si128(reinterpret_cast<__m128i*>(data), xmm0);

  return;
}

#else

#error "Unknown SSSE3 define. SSSE3.h"

#endif

#endif // __SSSE3_h_
