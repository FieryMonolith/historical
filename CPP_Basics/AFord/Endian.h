/*

 * Copyright 2006 Jason Stredwick.

 * Distributed under the Boost Software License, Version 1.0. (See
 * LICENSE information at the bottom of this file or the copy at
 * http://www.boost.org/LICENSE_1_0.txt)

 */

#ifndef __Endian_h__
#define __Endian_h__

#include <boost/detail/endian.hpp>
#include "QuantityUnits.h"


template <typename T>
class ByteReverser
{
   private:
      ~ByteReverser();

   public:
   // These reverse bytes for non-array data
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

   static void Copy(T &out, const T &in)
   { out = in; }
};


template <typename T, std::size_t N>
class ByteReverser< T[N] >
{
   private:
      ~ByteReverser();

   public:
   // These reverse bytes for non-array data
   static void ReverseBytes(T out[N], const T in[N])
   {
      for( std::size_t i( 0U ); i < N; ++i )
      { ByteReverser< T >::ReverseBytes( out[i], in[i] ); }

      return;
   }


   // These reverse bytes for non-array data
   void ReverseBytes(T inout[N])
   {
      for( std::size_t i( 0U ); i < N; ++i )
      { ByteReverser< T >::ReverseBytes( inout[i] ); }

      return;
   }

   static void Copy(T out[N], const T in[N])
   {
      std::memcpy( static_cast< void* >( out ),
                   static_cast< const void* >( in ),
                   sizeof( T ) * N );
   }
};


template< int > class MachineEndian;


typedef MachineEndian< 4321 > BigEndian;
typedef MachineEndian< 1234 > LittleEndian;


template<>
class MachineEndian< 4321 > // a.k.a. Big Endian
{
   private:
      ~MachineEndian(void);

   public:
      enum{ IsBigEndian = 0, IsLittleEndian = 1};

      ////// Convert to/from big endian (Do nothing) //////
      template< typename T >
      static void ToBigEndian( T& out, const T& in )
      { ByteReverser< T >::Copy( out, in ); }

      template< typename T >
      static void ToBigEndian( T& /*inout*/ ){}

      template< typename T >
      static void FromBigEndian( T& out, const T& in )
      { ByteReverser< T >::Copy( out, in ); }

      template< typename T >
      static void FromBigEndian( T& /*inout*/ ){}

      ////// Convert to little endian (Reverse bytes) //////
      template< typename T >
      static void ToLittleEndian( T& out, const T& in )
      { ByteReverser< T >::ReverseBytes( out, in ); }

      template< typename T >
      static void ToLittleEndian( T& inout )
      { ByteReverser< T >::ReverseBytes( inout ); }

      template< typename T >
      static void FromLittleEndian( T& out, const T& in )
      { ByteReverser< T >::ReverseBytes( out, in ); }

      template< typename T >
      static void FromLittleEndian( T& inout )
      { ByteReverser< T >::ReverseBytes( inout ); }
};


template<>
class MachineEndian< 1234 > // a.k.a. Little Endian
{
   private:
      ~MachineEndian(void);

   public:
      enum{ IsBigEndian = 0, IsLittleEndian = 1};

      ////// Convert to/from big endian (Do nothing) //////
      template< typename T >
      static void ToBigEndian( T& out, const T& in )
      { ByteReverser< T >::ReverseBytes( out, in ); }

      template< typename T >
      static void ToBigEndian( T& inout )
      { ByteReverser< T >::ReverseBytes( inout ); }

      template< typename T >
      static void FromBigEndian( T& out, const T& in )
      { ByteReverser< T >::ReverseBytes( out, in ); }

      template< typename T >
      static void FromBigEndian( T& inout )
      { ByteReverser< T >::ReverseBytes( inout ); }

      ////// Convert to/from little endian (Reverse bytes) //////
      template< typename T >
      static void ToLittleEndian( T& out, const T& in )
      { ByteReverser< T >::Copy( out, in ); }

      template< typename T >
      static void ToLittleEndian( T& /*inout*/ ){}

      template< typename T >
      static void FromLittleEndian( T& out, const T& in )
      { ByteReverser< T >::Copy( out, in ); }

      template< typename T >
      static void FromLittleEndian( T& /*inout*/ ){}
};


class Endian : public MachineEndian< BOOST_BYTE_ORDER >
{
   private:
      ~Endian(void);

   public:
      // These functions are simply convenience at this point.  The compiler
      // will inline these properly.  We don't want the user directly accessing
      // the ByteReverser.
      template <typename T>
      static void ReverseBytes(T &out, const T &in)
      { ByteReverser< T >::ReverseBytes(out, in); }

      template <typename T>
      static void ReverseBytes(T &inout)
      { ByteReverser< T >::ReverseBytes(inout); }

      template <typename T>
      static void Copy(T &out, const T &in)
      { ByteReverser< T >::Copy( out, in ); }
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
