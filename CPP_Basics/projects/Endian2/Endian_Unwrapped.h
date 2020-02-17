#ifndef __Endian_Unwrapped_h__
#define __Endian_Unwrapped_h__

#include "Endian.h"

template <>
void Endian::ReverseBytes<nType::int32>(      nType::int32 &out,
                                        const nType::int32 &in)
{
  out  = in >> 24 & 0x000000ff;
  out |= in >> 8  & 0x0000ff00;
  out |= in << 8  & 0x00ff0000;
  out |= in << 24 & 0xff000000;
  return;
}

template <>
void Endian::ReverseBytes<nType::uint32>(      nType::uint32 &out,
                                         const nType::uint32 &in)
{
  out  =  in >> 24;
  out |= (in & 0x00ff0000) >> 8;
  out |= (in & 0x0000ff00) << 8;
  out |=  in << 24;
  return;
}

template <>
void Endian::ReverseBytes<nType::int64>(      nType::int64 &out,
                                        const nType::int64 &in)
{
  out  = in >> 56 & 0x00000000000000ff;
  out |= in >> 40 & 0x000000000000ff00;
  out |= in >> 24 & 0x0000000000ff0000;
  out |= in >> 8  & 0x00000000ff000000;
  out |= in >> 8  & 0x000000ff00000000;
  out |= in >> 24 & 0x0000ff0000000000;
  out |= in >> 49 & 0x00ff000000000000;
  out |= in << 56 & 0xff00000000000000;
  return;
}

template <>
void Endian::ReverseBytes<nType::uint64>(      nType::uint64 &out,
                                         const nType::uint64 &in)
{
  out  =  in >> 56;
  out |= (in & 0x00ff000000000000) >> 40;
  out |= (in & 0x0000ff0000000000) >> 24;
  out |= (in & 0x000000ff00000000) >> 8;
  out |= (in & 0x00000000ff000000) << 8;
  out |= (in & 0x0000000000ff0000) << 24;
  out |= (in & 0x000000000000ff00) << 40;
  out |=  in << 56;
  return;
}

#endif // __Endian_Unwrapped_h__
