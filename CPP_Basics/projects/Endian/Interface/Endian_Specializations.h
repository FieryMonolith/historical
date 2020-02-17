/*

 * Copyright 2009 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * LICENSE information in the license.txt file or at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __Endian_Specializations_h__
#define __Endian_Specializations_h__

/*

 * After searching around online, I found there were a number sites that 
 *   describe how to reverse bytes.  There were many different methods and 
 *   for many languages.  However, I went with a specific method that 
 *   involved bit shifting and boolean algebra.  One such site that had 
 *   this information was
 *
 * http://www.codeguru.com/cpp/data/data-misc/values/article.php/c10243/
 *
 * These functions are specializations of the ReverseBytes methods 
 *   in the Endian class.  The specializations are for signed and
 *   unsigned 32 and 64 bit integers.  It is trivial to add your own 
 *   specializations and some types may automatically use these functions 
 *   due to type promotion.
 * One note is that there are signed and unsigned versions which is due
 *   to the workings of the bit shift.  When a signed number is shifted
 *   the new bits are filled with ones.  If an unsigned number is shifted
 *   the new bits are filled with zeros.  Thus the two variants.
 * I did not add the variants for the ReverseBytes(inout) due to the 
 *   redundancy as you can call ReverseBytes(in, in).

 */

#include "Endian.h"

template <>
void Endian::ReverseBytes<nType::int32>(      nType::int32 &out,
                                        const nType::int32 &in)
{
  // Each bit shift must be anded because the shift fills with ones 
  //   instead of zeros.
  out = in >> 24 & 0x000000ff |
        in >> 8  & 0x0000ff00 |
        in << 8  & 0x00ff0000 |
        in << 24 & 0xff000000;
  return;
}

template <>
void Endian::ReverseBytes<nType::uint32>(      nType::uint32 &out,
                                         const nType::uint32 &in)
{
  // The ends don't need to be anded because the shift fills with zeros.
  out = in >> 24              |
        in >> 8  & 0x0000ff00 |
        in << 8  & 0x00ff0000 |
        in << 24;
  return;
}

template <>
void Endian::ReverseBytes<nType::int64>(      nType::int64 &out,
                                        const nType::int64 &in)
{
  // Each bit shift must be anded because the shift fills with ones 
  //   instead of zeros.
  out = in >> 56 & 0x00000000000000ff |
        in >> 40 & 0x000000000000ff00 |
        in >> 24 & 0x0000000000ff0000 |
        in >> 8  & 0x00000000ff000000 |
        in << 8  & 0x000000ff00000000 |
        in << 24 & 0x0000ff0000000000 |
        in << 40 & 0x00ff000000000000 |
        in << 56 & 0xff00000000000000;
  return;
}

template <>
void Endian::ReverseBytes<nType::uint64>(      nType::uint64 &out,
                                         const nType::uint64 &in)
{
  // The ends don't need to be anded because the shift fills with zeros.
  out =  in >> 56                      |
         in >> 40 & 0x000000000000ff00 |
         in >> 24 & 0x0000000000ff0000 |
         in >>  8 & 0x00000000ff000000 |
         in <<  8 & 0x000000ff00000000 |
         in << 24 & 0x0000ff0000000000 |
         in << 40 & 0x00ff000000000000 |
         in << 56;
  return;
}

#endif // __Endian_Specializations_h__
