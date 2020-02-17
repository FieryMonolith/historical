#ifndef __BuiltinTypes_h__
#define __BuiltinTypes_h__

/*** Compiler Types **********************************************************/

#ifdef _MSC_VER

typedef unsigned __int8  byte;
typedef unsigned __int16 word;
typedef unsigned __int32 dword;
typedef unsigned __int64 qword;

typedef signed   __int8  sbyte;
typedef signed   __int16 sword;
typedef signed   __int32 sdword;
typedef signed   __int64 sqword;

typedef unsigned __int8  uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

typedef   signed __int8  int8;
typedef   signed __int16 int16;
typedef   signed __int32 int32;
typedef   signed __int64 int64;

#else

typedef unsigned char          byte;
typedef unsigned short         word;
typedef unsigned int           dword;
typedef long long unsigned int qword;

typedef signed char            sbyte;
typedef signed short           sword;
typedef signed int             sdword;
typedef long long signed int   sqword;

typedef unsigned char          uint8;
typedef unsigned short         uint16;
typedef unsigned int           uint32;
typedef long long unsigned int uint64;

typedef signed char            int8;
typedef signed short           int16;
typedef signed int             int32;
typedef long long signed int   int64;

#endif

/*** System Types ************************************************************/

// The following template and specializations for UType and SType are used to
//   specify a type based on size.
template <int N> struct UType { };
template <> struct UType<1>   { typedef uint8  Type; };
template <> struct UType<2>   { typedef uint16 Type; };
template <> struct UType<4>   { typedef uint32 Type; };
template <> struct UType<8>   { typedef uint64 Type; };

template <int N> struct SType { };
template <> struct SType<1>   { typedef  int8  Type; };
template <> struct SType<2>   { typedef  int16 Type; };
template <> struct SType<4>   { typedef  int32 Type; };
template <> struct SType<8>   { typedef  int64 Type; };

// Assumption: The sizeof(void*) will be the size of a processor's register
//   and can be used to determine the default system size for an integer.

// typdef: ptr - unsigned value capable of holding an address
//   By specializing the UType template, I can specify the size of the void*
//   to determine what the actual size of the uint_ptr should be.
typedef UType<sizeof(void*)>::Type uint_ptr;

// typdef: uint - unsigned integer corresponding to the processor register size
//   By specializing the UType template, I can specify the size of the void*
//   to determine what the size of the default processor unsigned type should
//   be.
typedef UType<sizeof(void*)>::Type uint;

// typdef: sint - signed integer corresponding to the processor register size
//   By specializing the SType template, I can specify the size of the void*
//   to determine what the size of the default processor signed type should
//   be.
typedef SType<sizeof(void*)>::Type sint;

/*** System Type *************************************************************/


#endif // __BuiltinTypes_h__
