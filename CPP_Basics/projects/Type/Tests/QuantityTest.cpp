#include <iostream>
#include <sstream>
#include <string>
#include <ctime>

#include "Endian.h"
#include "QuantityBuiltin.h"

using namespace std;

// Compiler Error/Warnings
void DefaultConstructor(void);
void TUConstructor(void);
void ImplicitConstructor(void);
void Assignment(void);

void Sizeof(void);
void TestInc(void);
template <typename T, typename U> void TestIncType(void);
void TestDec(void);
template <typename T, typename U> void TestDecType(void);
void TestBitwise(void);
void TestMath(void);
void TestLogic(void);
void TestStream(void);

template <typename T> void Performance(void);

void QuantityTest(void) {
  cout << "These first tests check for compiler error/warning.  There" << endl;
  cout << "   should be none." << endl << endl;

  cout << "Testing Default Constructor/Destructor............";
  DefaultConstructor();
  cout << "Complete." << endl;

  cout << "Testing Constructors..............................";
  TUConstructor();
  cout << "Complete." << endl;

  cout << "Testing Implicit Constructor......................";
  ImplicitConstructor();
  cout << "Complete." << endl;

  cout << "Testing Assignment................................";
  Assignment();
  cout << "Complete.";

  cout << endl << endl;
  cout << "Now testing how Quantity<T> compares with T iteslf.";
  cout << endl << endl;

  cout << "Testing endianness................................";
  if(Endian::IsBigEndian()) {
    cout << ".....Big.";
  } else {
    cout << "..Little.";
  }
  cout << endl;

  cout << "Testing sizeof Quantity<T> and T..................";
  Sizeof();
  cout << "Complete." << endl;

  cout << "Testing Logic Operations:.........................";
  TestLogic();
  cout << "Complete." << endl;

  cout << "Testing Stream Operations:........................";
  TestStream();
  cout << "Complete." << endl;

  cout << endl;

  cout << "Increment and decrement testing are used to compare the   " << endl;
  cout << "the full range of values for a Quantity<T> versus its     " << endl;
  cout << "builtin type equivalent.  Each test will work on signed   " << endl;
  cout << "and unsigned values by incrementing through the range     " << endl;
  cout << "starting at zero, roll over, and increment a few more     " << endl;
  cout << "times.  By comparing to its builtin equivalent will ensure" << endl;
  cout << "that the full range is accessed and in the same way.      " << endl;
  cout << endl;
  cout << "Testing Increment:" << endl;
  cout << endl;
  TestInc();
  cout << endl;
  cout << "Testing Increment Complete." << endl << endl;

  cout << endl;

  cout << "Testing Decrement:" << endl;
  cout << endl;
  TestDec();
  cout << endl;
  cout << "Testing Decrement Complete." << endl << endl;

  cout << "Testing Bitwise Operations:.......................";
  TestBitwise();
  cout << "Complete." << endl;

  cout << "Testing Math Operations:..........................";
  TestMath();
  cout << "Complete." << endl;

  cout << endl;
  
  cout << "Performance testing." << endl;

  cout << endl;

  double b = 0;
  double q = 0;

  for(unsigned int i=0; i<10; i++) {
    clock_t start = clock();
    Performance<int32>();
    clock_t end = clock();

    clock_t diff = end - start;
    b += static_cast<double>(diff) / static_cast<double>(CLK_TCK);
  }
  b /= 10.0;

  for(unsigned int i=0; i<10; i++) {
    clock_t start = clock();
    Performance<Int32>();
    clock_t end = clock();

    clock_t diff = end - start;
    q += static_cast<double>(diff) / static_cast<double>(CLK_TCK);
  }
  q /= 10.0;

  cout << "int32: " << b << "  Int32: " << q << endl;

  return;
}

/*** BEGIN PERFORMANCE TESTING *******************************************/
template <typename T> void Performance(void) {
  T a = 1230456;
  T b = 1230456;
  T c = 1230456;
  T d = 1230456;

#if DEBUG
  for(uint64 i=0; i<10000000; i++) {
    a = a + 1;
    b = b * 37;
    c = c / 21;
    if(c < 100) { c = 1230456; }
    d = d - 127;
  }
#else
  uint64 max = 0xffffffff;
  max <<= 32;
  max += 0xffffffff;
  for(uint64 j=0; j<10000000000000000000; j++) {
    for(uint64 i=0; i<10000000000000000000; i++) {
      a = a + 1;
      b = b * 37;
      c = c / 21;
      if(c < 100) { c = 1230456; }
      d = d - 127;
    }
  }
#endif

  return;
}

/*** BEGIN TESTING ABILITITES ********************************************/
void Sizeof(void) {
#pragma warning(disable : 4127)
  if(sizeof(Byte) != sizeof(BYTE)) {
    cout << endl;
    cout << "ERROR: sizeof(Byte) <" << sizeof(Byte) << "> != <" << sizeof(BYTE) << "> sizeof(BYTE)" << endl;
  }

  if(sizeof(Word) != sizeof(WORD)) {
    cout << endl;
    cout << "ERROR: sizeof(Word) <" << sizeof(Word) << "> != <" << sizeof(WORD) << "> sizeof(WORD)" << endl;
  }

  if(sizeof(DWord) != sizeof(DWORD)) {
    cout << endl;
    cout << "ERROR: sizeof(DWord) <" << sizeof(DWord) << "> != <" << sizeof(DWORD) << "> sizeof(DWORD)" << endl;
  }

  if(sizeof(QWord) != sizeof(QWORD)) {
    cout << endl;
    cout << "ERROR: sizeof(QWord) <" << sizeof(QWord) << "> != <" << sizeof(QWORD) << "> sizeof(QWORD)" << endl;
  }

  if(sizeof(Int8) != sizeof(int8)) {
    cout << endl;
    cout << "ERROR: sizeof(Int8) <" << sizeof(Int8) << "> != <" << sizeof(int8) << "> sizeof(int8)" << endl;
  }

  if(sizeof(UInt8) != sizeof(uint8)) {
    cout << endl;
    cout << "ERROR: sizeof(UInt8) <" << sizeof(UInt8) << "> != <" << sizeof(uint8) << "> sizeof(uint8)" << endl;
  }

  if(sizeof(Int16) != sizeof(int16)) {
    cout << endl;
    cout << "ERROR: sizeof(Int16) <" << sizeof(Int16) << "> != <" << sizeof(int16) << "> sizeof(int16)" << endl;
  }

  if(sizeof(UInt16) != sizeof(uint16)) {
    cout << endl;
    cout << "ERROR: sizeof(UInt16) <" << sizeof(UInt16) << "> != <" << sizeof(uint16) << "> sizeof(uint16)" << endl;
  }

  if(sizeof(Int32) != sizeof(int32)) {
    cout << endl;
    cout << "ERROR: sizeof(Int32) <" << sizeof(Int32) << "> != <" << sizeof(int32) << "> sizeof(int32)" << endl;
  }

  if(sizeof(UInt32) != sizeof(uint32)) {
    cout << endl;
    cout << "ERROR: sizeof(UInt32) <" << sizeof(UInt32) << "> != <" << sizeof(uint32) << "> sizeof(uint32)" << endl;
  }

  if(sizeof(Int64) != sizeof(int64)) {
    cout << endl;
    cout << "ERROR: sizeof(Int64) <" << sizeof(Int64) << "> != <" << sizeof(int64) << "> sizeof(int64)" << endl;
  }

  if(sizeof(UInt64) != sizeof(uint64)) {
    cout << endl;
    cout << "ERROR: sizeof(UInt64) <" << sizeof(UInt64) << "> != <" << sizeof(uint64) << "> sizeof(uint64)" << endl;
  }

  if(sizeof(Char) != sizeof(signed char)) {
    cout << endl;
    cout << "ERROR: sizeof(Char) <" << sizeof(Char) << "> != <" << sizeof(signed char) << "> sizeof(signed char)" << endl;
  }

  if(sizeof(UChar) != sizeof(unsigned char)) {
    cout << endl;
    cout << "ERROR: sizeof(UChar) <" << sizeof(UChar) << "> != <" << sizeof(unsigned char) << "> sizeof(unsigned char)" << endl;
  }

  if(sizeof(Short) != sizeof(signed short)) {
    cout << endl;
    cout << "ERROR: sizeof(Short) <" << sizeof(Short) << "> != <" << sizeof(signed short) << "> sizeof(signed short)" << endl;
  }

  if(sizeof(UShort) != sizeof(unsigned short)) {
    cout << endl;
    cout << "ERROR: sizeof(UShort) <" << sizeof(UShort) << "> != <" << sizeof(unsigned short) << "> sizeof(unsigned short)" << endl;
  }

  if(sizeof(Int) != sizeof(signed int)) {
    cout << endl;
    cout << "ERROR: sizeof(Int) <" << sizeof(Int) << "> != <" << sizeof(signed int) << "> sizeof(signed int)" << endl;
  }

  if(sizeof(UInt) != sizeof(unsigned int)) {
    cout << endl;
    cout << "ERROR: sizeof(UInt) <" << sizeof(UInt) << "> != <" << sizeof(unsigned int) << "> sizeof(unsigned int)" << endl;
  }

  if(sizeof(Long) != sizeof(signed long)) {
    cout << endl;
    cout << "ERROR: sizeof(Long) <" << sizeof(Long) << "> != <" << sizeof(signed long) << "> sizeof(signed long)" << endl;
  }

  if(sizeof(ULong) != sizeof(unsigned long)) {
    cout << endl;
    cout << "ERROR: sizeof(ULong) <" << sizeof(ULong) << "> != <" << sizeof(unsigned long) << "> sizeof(unsigned long)" << endl;
  }
#pragma warning(default : 4127)

  return;
}

// Testing int8 - int16 and uint8 - uint16, which takes care of all the types
//   except char and uchar which may have special types.  Otherwise, the other
//   types are redundant.  This is a brute force test that increments from
//   zero, rolls over, and increments a few more times.  When compared to its
//   builtin type equivalent will ensure that the full range of numbers is
//   accessable in the same way.  int32, uint32, int64, and uint64 are not
//   tested due to the time required to increment through all of their values.
void TestInc(void) {
  cout << "Testing <signed char> and <Char>..................";
  TestIncType<signed char, Char>();
  cout << "Complete." << endl;

  cout << "Testing <unsigned char> and <UChar>...............";
  TestIncType<unsigned char, UChar>();
  cout << "Complete." << endl;

  cout << "Testing <int8> and <Int8>.........................";
  TestIncType<int8, Int8>();
  cout << "Complete." << endl;

  cout << "Testing <uint8> and <UInt8>.......................";
  TestIncType<uint8, UInt8>();
  cout << "Complete." << endl;

  cout << "Testing <int16> and <Int16>.......................";
  TestIncType<int16, Int16>();
  cout << "Complete." << endl;

  cout << "Testing <uint16> and <UInt16>.....................";
  TestIncType<uint16, UInt16>();
  cout << "Complete." << endl;

  return;
}

// Same as TestInc except uses the decrement operator.
void TestDec(void) {
  cout << "Testing <signed char> and <Char>..................";
  TestDecType<signed char, Char>();
  cout << "Complete." << endl;

  cout << "Testing <unsigned char> and <UChar>...............";
  TestDecType<unsigned char, UChar>();
  cout << "Complete." << endl;

  cout << "Testing <int8> and <Int8>.........................";
  TestDecType<int8, Int8>();
  cout << "Complete." << endl;

  cout << "Testing <uint8> and <UInt8>.......................";
  TestDecType<uint8, UInt8>();
  cout << "Complete." << endl;

  cout << "Testing <int16> and <Int16>.......................";
  TestDecType<int16, Int16>();
  cout << "Complete." << endl;

  cout << "Testing <uint16> and <UInt16>.....................";
  TestDecType<uint16, UInt16>();
  cout << "Complete." << endl;

  return;
}

template <typename T, typename U>
void TestIncType(void) {
  T control = 0;
  U solo = 0;

  QWORD index = 0;
  const size_t bits = sizeof(T) * BYTES_PER_SIZEOF_UNIT * BITS_PER_BYTE;

  QWORD max = 0;
  #pragma warning(disable : 4127)
  if(bits == 8) {
    max = 0xff;
  } else if(bits == 16) {
    max = 0xffff;
  } else if(bits == 32) {
    max = 0xffffffff;
  } else if(bits == 64) {
    max = 0xffffffff;
    max <<= 32;
    max += 0xffffffff;
  } else {
    cout << "ERROR: Invalid type size > 64 bits." << endl;
    return;
  }
  #pragma warning(default : 4127)

  for(; index < max; index++) {
    control++;
    solo++;
    if(control != solo) {
      cout << endl;
      cout << "ERROR (++): " << control << " != " << solo << endl;
    }
  }

  // Now overflow
  control++;
  solo++;
  if(control != solo) {
    cout << endl;
    cout << "ERROR (++): " << control << " != " << solo << endl;
  }

  control++;
  solo++;
  if(control != solo) {
    cout << endl;
    cout << "ERROR (++): " << control << " != " << solo << endl;
  }

  return;
}

template <typename T, typename U>
void TestDecType(void) {
  T control = 0;
  U solo = 0;

  QWORD index = 0;
  const size_t bits = sizeof(T) * BYTES_PER_SIZEOF_UNIT * BITS_PER_BYTE;

  QWORD max = 0;
  #pragma warning(disable : 4127)
  if(bits == 8) {
    max = 0xff;
  } else if(bits == 16) {
    max = 0xffff;
  } else if(bits == 32) {
    max = 0xffffffff;
  } else if(bits == 64) {
    max = 0xffffffff;
    max <<= 32;
    max += 0xffffffff;
  } else {
    cout << "ERROR: Invalid type size > 64 bits." << endl;
    return;
  }
  #pragma warning(default : 4127)

  for(; index < max; index++) {
    control--;
    solo--;
    if(control != solo) {
      cout << endl;
      cout << "ERROR (--): " << control << " != " << solo << endl;
    }
  }

  // Now overflow
  control--;
  solo--;
  if(control != solo) {
    cout << endl;
    cout << "ERROR (--): " << control << " != " << solo << endl;
  }

  control--;
  solo--;
  if(control != solo) {
    cout << endl;
    cout << "ERROR (--): " << control << " != " << solo << endl;
  }

  return;
}

void TestBitwise(void) {
  unsigned char c=1, c2=0x80;
  UChar qc=1, qc2=0x80;

  // Do not test 8 because shifting by 8 or more is undefined with most
  //   compilers.
  for(unsigned int i=0; i<8; i++) {
    c  <<= 1;
    qc <<= 1;

    c2  >>= 1;
    qc2 >>= 1;

    if(c != qc || c2 != qc2) {
      cout << endl;
      cout << "ERROR: Shifting not equal on shift " << i+1 << "." << endl;
    }
  }

  int16 i16=1;
  Int16 qi16=1;
  for(unsigned int i=0; i<16; i++) {
    i16 = i16 << 1;
    qi16 = qi16 << 1;

    if(i16 != qi16) {
      cout << endl;
      cout << "ERROR: Shifting not equal on shift " << i+1 << "." << endl;
    }
  }

  c = 0x5a;
  qc = 0x5a;
  if((c & qc) != 0x5a || (qc & c) != 0x5a) {
    cout << endl;
    cout << "ERROR: unsigned char(0x5a) & UChar(0x5a) != 0x5a" << endl;
  }

  c = 0x5a;
  qc = 0x5f;
  if((c | qc) != 0x5f || (qc | c) != 0x5f) {
    cout << endl;
    cout << "ERROR: unsigned char(0x5a) | UChar(0x5f) != 0x5f" << endl;
  }

  c = 0x5a;
  qc = 0xdf;
  if((c ^ qc) != 0x85 || (qc ^ c) != 0x85) {
    cout << endl;
    cout << "ERROR: unsigned char(0x5a) ^ UChar(0xdf) != 0x85" << endl;
  }

  return;
}

void TestMath(void) {
  int32 i32 = 645;
  signed char c = 13;

  Int32 qi32 = 645;
  Char qc = 13;

  // Test type promotion and operator type mismatch
  if(i32 + c != qi32 + qc || c + i32 != qc + i32 ||
     i32 + qc != qi32 + c || qc + i32 != c + qi32) {
    cout << endl;
    cout << "ERROR: Add not equal int32(645) + signed char(13)." << endl;
  }

  if(i32 - 125 != qi32 - 125 || -125 + i32 != -125 + i32) {
    cout << endl;
    cout << "ERROR: Add not equal int32(645) - 125." << endl;
  }

  c = -128;
  i32 = -128;
  qi32 = -128;
  qc = -128;

  if(c * -1 != qc * -1 || qi32 * -1 != i32 * -1) {
    cout << endl;
    cout << "ERROR: Mult not equal signed char and int32(-128) * -1" << endl;
  }

  i32 = 67;
  qi32 = 67;
  if(i32 / 4 != qi32 / 4 || i32 % 4 != qi32 % 4) {
    cout << endl;
    cout << "ERROR: Div and Mod not equal for int32(67) / 4" << endl;
  }

  char c1, c2;
  Char C1, C2;
  short s1, s2;
  Short S1, S2;
  int i1, i2;
  Int I1, I2;

  c1 = 3;
  c2 = 101;
  s1 = 57;
  s2 = 31;
  i1 = 7;
  i2 = 87;

  C1 = 3;
  C2 = 101;
  S1 = 57;
  S2 = 31;
  I1 = 7;
  I2 = 87;

  i2 = c1 + i1 + s1 + s2 + c2;
  I2 = C1 + I1 + S1 + S2 + C2;
  if(i2 != I2) {
    cout << endl;
    cout << "ERROR: Sum1 - " << i2 << " != " << I2 << endl;
  }

  c2 = c1 + i1 + s1 + s2 + i2;
  C2 = C1 + I1 + S1 + S2 + I2;
  if(c2 != C2) {
    cout << endl;
    cout << "ERROR: Sum2 - " << (int)c2 << " != " << (int)C2 << endl;
  }

  c1 = C1 = 0x12;
  s1 = S1 = 0x00ef;

  c2 = c1 + s1;
  C2 = C1 + S1;
  if(c2 != C2) {
    cout << endl;
    cout << "ERROR: Sum3 - " << (int)c2 << " != " << C2 << endl;
  }

  c2 = s1 + c1;
  C2 = S1 + C1;
  if(c2 != C2) {
    cout << endl;
    cout << "ERROR: Sum4 - " << (int)c2 << " != " << (int)C2 << endl;
  }

  s2 = c1 + s1;
  S2 = C1 + S1;
  if(s2 != S2) {
    cout << endl;
    cout << "ERROR: Sum5 - " << s2 << " != " << S2 << endl;
  }

  s2 = s1 + c1;
  S2 = S1 + C1;
  if(s2 != S2) {
    cout << endl;
    cout << "ERROR: Sum6 - " << s2 << " != " << S2 << endl;
  }

  return;
}

// Don't need to test all logic operations since if == and != works, so 
//   should <, <=, >, and >=.
void TestLogic(void) {
  int16 a = 32;
  Int16 b = 32;

  if(a == b && b == a) {
    ;
  } else {
    cout << endl;
    cout << "ERROR: int16(32) != Int16(32)" << endl;
  }

  b = -33;
  if(a == b || b == a) {
    cout << endl;
    cout << "ERROR: int16(32) == Int16(-33)" << endl;
  }

  if(a != b && b != a) {
    ;
  } else {
    cout << endl;
    cout << "ERROR: int16(32) == Int16(-33)" << endl;
  }

  b = 32;
  if(a != b || b != a) {
    cout << endl;
    cout << "ERROR: int16(32) != Int16(32)" << endl;
  }

  Char c = -64;
  if(c == -64 || -64 == c) {
    ;
  } else {
    cout << endl;
    cout << "ERROR: Char(-64) != -64" << endl;
  }

  a = 12;
  b = -12;
  if(-b == a && a == -b) {
    ;
  } else {
    cout << endl;
    cout << "ERROR: !Int16(-12) != int16(12)" << endl;
  }

  int8 e = 12;
  Int64 d = -12;
  if(-d == e && e == -d) {
    ;
  } else {
    cout << endl;
    cout << "ERROR: !Int64(-12) != int8(12)" << endl;
  }

  Int32 i = 3;
  Int32 j = 0;

  if(i && !j) {
    ;
  } else {
    cout << "ERROR: (Int32(3) && !Int32(0)) == false)" << endl;
  }

  return;
}

void TestStream(void) {
  UChar qc;
  unsigned char c;
  Int32 qi32;
  int32 i32;
  ULong qul;
  unsigned long ul;

  string s("a 137234 89103");
  stringstream ss, qss;
  ss << s;
  qss << s;

  ss >> c;
  ss >> ws;
  ss >> i32;
  ss >> ws;
  ss >> ul;

  qss >> qc;
  qss >> ws;
  qss >> qi32;
  qss >> ws;
  qss >> qul;

  if(c != qc) {
    cout << endl;
    cout << "ERROR: Improper read of UChar from stream." << endl;
    cout << "       unsigned char(" << c << ") != UChar(" << qc << ").";
    cout << endl;
  }
  if(i32 != qi32) {
    cout << endl;
    cout << "ERROR: Improper read of Int32 from stream." << endl;
    cout << "       int32(" << i32 << ") != Int32(" << qi32 << ")." << endl;
  }
  if(c != qc) {
    cout << endl;
    cout << "ERROR: Improper read of ULong from stream." << endl;
    cout << "       unsigned long(" << ul << ") != ULong(" << qul << ").";
    cout << endl;
  }

  stringstream ss2, qss2;
  ss2 << c << " " << i32 << " " << ul;
  qss2 << qc << " " << qi32 << " " << qul;

  if(ss2.str() != qss2.str()) {
    cout << endl;
    cout << "ERROR: Output to stream not correct for" << endl;
    cout << "       unsigned char << int32 << unsigned long." << endl;
    cout << "       builtin(\"" << ss2.str() << "\") != Quantity(\"";
    cout << qss2.str() << "\").";
    cout << endl;
  }

  return;
}

/*** END TESTING ABILITITES **********************************************/
/*** BEGIN CONSTRUCTOR/ASSIGNMENT ****************************************/
// Testing compiler error/warnings here : Should be none.
void DefaultConstructor(void) {
  // Byte
  Byte byte;
  Byte byte_1;
  Byte *byte_2 = new Byte;
  if(byte_2 != NULL) { delete byte_2; }

  // Word
  Word word;
  Word word_1;
  Word *word_2 = new Word;
  if(word_2 != NULL) { delete word_2; }

  // DWord
  DWord dword;
  DWord dword_1;
  DWord *dword_2 = new DWord;
  if(dword_2 != NULL) { delete dword_2; }

  // QWord
  QWord qword;
  QWord qword_1;
  QWord *qword_2 = new QWord;
  if(qword_2 != NULL) { delete qword_2; }

  // Int8
  Int8 q_Int8;
  Int8 q_Int8_1;
  Int8 *q_Int8_2 = new Int8;
  if(q_Int8_2 != NULL) { delete q_Int8_2; }

  // UInt8
  UInt8 q_UInt8;
  UInt8 q_UInt8_1;
  UInt8 *q_UInt8_2 = new UInt8;
  if(q_UInt8_2 != NULL) { delete q_UInt8_2; }

  // Int16
  Int16 q_Int16;
  Int16 q_Int16_1;
  Int16 *q_Int16_2 = new Int16;
  if(q_Int16_2 != NULL) { delete q_Int16_2; }

  // UInt16
  UInt16 q_UInt16;
  UInt16 q_UInt16_1;
  UInt16 *q_UInt16_2 = new UInt16;
  if(q_UInt16_2 != NULL) { delete q_UInt16_2; }

  // Int32
  Int32 q_Int32;
  Int32 q_Int32_1;
  Int32 *q_Int32_2 = new Int32;
  if(q_Int32_2 != NULL) { delete q_Int32_2; }

  // UInt32
  UInt32 q_UInt32;
  UInt32 q_UInt32_1;
  UInt32 *q_UInt32_2 = new UInt32;
  if(q_UInt32_2 != NULL) { delete q_UInt32_2; }

  // Int64
  Int64 q_Int64;
  Int64 q_Int64_1;
  Int64 *q_Int64_2 = new Int64;
  if(q_Int64_2 != NULL) { delete q_Int64_2; }

  // UInt64
  UInt64 q_UInt64;
  UInt64 q_UInt64_1;
  UInt64 *q_UInt64_2 = new UInt64;
  if(q_UInt64_2 != NULL) { delete q_UInt64_2; }

  // Char
  Char q_Char;
  Char q_Char_1;
  Char *q_Char_2 = new Char;
  if(q_Char_2 != NULL) { delete q_Char_2; }

  // UChar
  UChar q_UChar;
  UChar q_UChar_1;
  UChar *q_UChar_2 = new UChar;
  if(q_UChar_2 != NULL) { delete q_UChar_2; }

  // Short
  Short q_Short;
  Short q_Short_1;
  Short *q_Short_2 = new Short;
  if(q_Short_2 != NULL) { delete q_Short_2; }

  // UShort
  UShort q_UShort;
  UShort q_UShort_1;
  UShort *q_UShort_2 = new UShort;
  if(q_UShort_2 != NULL) { delete q_UShort_2; }

  // Int
  Int q_Int;
  Int q_Int_1;
  Int *q_Int_2 = new Int;
  if(q_Int_2 != NULL) { delete q_Int_2; }

  // UInt
  UInt q_UInt;
  UInt q_UInt_1;
  UInt *q_UInt_2 = new UInt;
  if(q_UInt_2 != NULL) { delete q_UInt_2; }

  // Long
  Long q_Long;
  Long q_Long_1;
  Long *q_Long_2 = new Long;
  if(q_Long_2 != NULL) { delete q_Long_2; }

  // ULong
  ULong q_ULong;
  ULong q_ULong_1;
  ULong *q_ULong_2 = new ULong;
  if(q_ULong_2 != NULL) { delete q_ULong_2; }

  return;
}

// Testing compiler error/warnings here : Should be none.
void TUConstructor(void) {
  BYTE b = 1;
  WORD w = 1;
  DWORD dw = 1;
  QWORD qw = 1;
  int8 i8 = 1;
  uint8 ui8 = 1;
  int16 i16 = 1;
  uint16 ui16 = 1;
  int32 i32 = 1;
  uint32 ui32 = 1;
  int64 i64 = 1;
  uint64 ui64 = 1;
  signed char c = 1;
  unsigned char uc = 1;
  signed short s = 1;
  unsigned short us = 1;
  signed int i = 1;
  unsigned int ui = 1;
  signed long l = 1;
  unsigned long ul = 1;

  Byte q_Byte_1(b);
  Byte q_Byte_2(w);
  Byte q_Byte_3(dw);
  Byte q_Byte_4(qw);
  Byte q_Byte_5(i8);
  Byte q_Byte_6(ui8);
  Byte q_Byte_7(i16);
  Byte q_Byte_8(ui16);
  Byte q_Byte_9(i32);
  Byte q_Byte_10(ui32);
  Byte q_Byte_11(i64);
  Byte q_Byte_12(ui64);
  Byte q_Byte_13(c);
  Byte q_Byte_14(uc);
  Byte q_Byte_15(s);
  Byte q_Byte_16(us);
  Byte q_Byte_17(i);
  Byte q_Byte_18(ui);
  Byte q_Byte_19(l);
  Byte q_Byte_20(ul);

  Word q_Word_1(b);
  Word q_Word_2(w);
  Word q_Word_3(dw);
  Word q_Word_4(qw);
  Word q_Word_5(i8);
  Word q_Word_6(ui8);
  Word q_Word_7(i16);
  Word q_Word_8(ui16);
  Word q_Word_9(i32);
  Word q_Word_10(ui32);
  Word q_Word_11(i64);
  Word q_Word_12(ui64);
  Word q_Word_13(c);
  Word q_Word_14(uc);
  Word q_Word_15(s);
  Word q_Word_16(us);
  Word q_Word_17(i);
  Word q_Word_18(ui);
  Word q_Word_19(l);
  Word q_Word_20(ul);

  DWord q_DWord_1(b);
  DWord q_DWord_2(w);
  DWord q_DWord_3(dw);
  DWord q_DWord_4(qw);
  DWord q_DWord_5(i8);
  DWord q_DWord_6(ui8);
  DWord q_DWord_7(i16);
  DWord q_DWord_8(ui16);
  DWord q_DWord_9(i32);
  DWord q_DWord_10(ui32);
  DWord q_DWord_11(i64);
  DWord q_DWord_12(ui64);
  DWord q_DWord_13(c);
  DWord q_DWord_14(uc);
  DWord q_DWord_15(s);
  DWord q_DWord_16(us);
  DWord q_DWord_17(i);
  DWord q_DWord_18(ui);
  DWord q_DWord_19(l);
  DWord q_DWord_20(ul);

  QWord q_QWord_1(b);
  QWord q_QWord_2(w);
  QWord q_QWord_3(dw);
  QWord q_QWord_4(qw);
  QWord q_QWord_5(i8);
  QWord q_QWord_6(ui8);
  QWord q_QWord_7(i16);
  QWord q_QWord_8(ui16);
  QWord q_QWord_9(i32);
  QWord q_QWord_10(ui32);
  QWord q_QWord_11(i64);
  QWord q_QWord_12(ui64);
  QWord q_QWord_13(c);
  QWord q_QWord_14(uc);
  QWord q_QWord_15(s);
  QWord q_QWord_16(us);
  QWord q_QWord_17(i);
  QWord q_QWord_18(ui);
  QWord q_QWord_19(l);
  QWord q_QWord_20(ul);

  Int8 q_Int8_1(b);
  Int8 q_Int8_2(w);
  Int8 q_Int8_3(dw);
  Int8 q_Int8_4(qw);
  Int8 q_Int8_5(i8);
  Int8 q_Int8_6(ui8);
  Int8 q_Int8_7(i16);
  Int8 q_Int8_8(ui16);
  Int8 q_Int8_9(i32);
  Int8 q_Int8_10(ui32);
  Int8 q_Int8_11(i64);
  Int8 q_Int8_12(ui64);
  Int8 q_Int8_13(c);
  Int8 q_Int8_14(uc);
  Int8 q_Int8_15(s);
  Int8 q_Int8_16(us);
  Int8 q_Int8_17(i);
  Int8 q_Int8_18(ui);
  Int8 q_Int8_19(l);
  Int8 q_Int8_20(ul);

  UInt8 q_UInt8_1(b);
  UInt8 q_UInt8_2(w);
  UInt8 q_UInt8_3(dw);
  UInt8 q_UInt8_4(qw);
  UInt8 q_UInt8_5(i8);
  UInt8 q_UInt8_6(ui8);
  UInt8 q_UInt8_7(i16);
  UInt8 q_UInt8_8(ui16);
  UInt8 q_UInt8_9(i32);
  UInt8 q_UInt8_10(ui32);
  UInt8 q_UInt8_11(i64);
  UInt8 q_UInt8_12(ui64);
  UInt8 q_UInt8_13(c);
  UInt8 q_UInt8_14(uc);
  UInt8 q_UInt8_15(s);
  UInt8 q_UInt8_16(us);
  UInt8 q_UInt8_17(i);
  UInt8 q_UInt8_18(ui);
  UInt8 q_UInt8_19(l);
  UInt8 q_UInt8_20(ul);

  Int16 q_Int16_1(b);
  Int16 q_Int16_2(w);
  Int16 q_Int16_3(dw);
  Int16 q_Int16_4(qw);
  Int16 q_Int16_5(i8);
  Int16 q_Int16_6(ui8);
  Int16 q_Int16_7(i16);
  Int16 q_Int16_8(ui16);
  Int16 q_Int16_9(i32);
  Int16 q_Int16_10(ui32);
  Int16 q_Int16_11(i64);
  Int16 q_Int16_12(ui64);
  Int16 q_Int16_13(c);
  Int16 q_Int16_14(uc);
  Int16 q_Int16_15(s);
  Int16 q_Int16_16(us);
  Int16 q_Int16_17(i);
  Int16 q_Int16_18(ui);
  Int16 q_Int16_19(l);
  Int16 q_Int16_20(ul);

  UInt16 q_UInt16_1(b);
  UInt16 q_UInt16_2(w);
  UInt16 q_UInt16_3(dw);
  UInt16 q_UInt16_4(qw);
  UInt16 q_UInt16_5(i8);
  UInt16 q_UInt16_6(ui8);
  UInt16 q_UInt16_7(i16);
  UInt16 q_UInt16_8(ui16);
  UInt16 q_UInt16_9(i32);
  UInt16 q_UInt16_10(ui32);
  UInt16 q_UInt16_11(i64);
  UInt16 q_UInt16_12(ui64);
  UInt16 q_UInt16_13(c);
  UInt16 q_UInt16_14(uc);
  UInt16 q_UInt16_15(s);
  UInt16 q_UInt16_16(us);
  UInt16 q_UInt16_17(i);
  UInt16 q_UInt16_18(ui);
  UInt16 q_UInt16_19(l);
  UInt16 q_UInt16_20(ul);

  Int32 q_Int32_1(b);
  Int32 q_Int32_2(w);
  Int32 q_Int32_3(dw);
  Int32 q_Int32_4(qw);
  Int32 q_Int32_5(i8);
  Int32 q_Int32_6(ui8);
  Int32 q_Int32_7(i16);
  Int32 q_Int32_8(ui16);
  Int32 q_Int32_9(i32);
  Int32 q_Int32_10(ui32);
  Int32 q_Int32_11(i64);
  Int32 q_Int32_12(ui64);
  Int32 q_Int32_13(c);
  Int32 q_Int32_14(uc);
  Int32 q_Int32_15(s);
  Int32 q_Int32_16(us);
  Int32 q_Int32_17(i);
  Int32 q_Int32_18(ui);
  Int32 q_Int32_19(l);
  Int32 q_Int32_20(ul);

  UInt32 q_UInt32_1(b);
  UInt32 q_UInt32_2(w);
  UInt32 q_UInt32_3(dw);
  UInt32 q_UInt32_4(qw);
  UInt32 q_UInt32_5(i8);
  UInt32 q_UInt32_6(ui8);
  UInt32 q_UInt32_7(i16);
  UInt32 q_UInt32_8(ui16);
  UInt32 q_UInt32_9(i32);
  UInt32 q_UInt32_10(ui32);
  UInt32 q_UInt32_11(i64);
  UInt32 q_UInt32_12(ui64);
  UInt32 q_UInt32_13(c);
  UInt32 q_UInt32_14(uc);
  UInt32 q_UInt32_15(s);
  UInt32 q_UInt32_16(us);
  UInt32 q_UInt32_17(i);
  UInt32 q_UInt32_18(ui);
  UInt32 q_UInt32_19(l);
  UInt32 q_UInt32_20(ul);

  Int64 q_Int64_1(b);
  Int64 q_Int64_2(w);
  Int64 q_Int64_3(dw);
  Int64 q_Int64_4(qw);
  Int64 q_Int64_5(i8);
  Int64 q_Int64_6(ui8);
  Int64 q_Int64_7(i16);
  Int64 q_Int64_8(ui16);
  Int64 q_Int64_9(i32);
  Int64 q_Int64_10(ui32);
  Int64 q_Int64_11(i64);
  Int64 q_Int64_12(ui64);
  Int64 q_Int64_13(c);
  Int64 q_Int64_14(uc);
  Int64 q_Int64_15(s);
  Int64 q_Int64_16(us);
  Int64 q_Int64_17(i);
  Int64 q_Int64_18(ui);
  Int64 q_Int64_19(l);
  Int64 q_Int64_20(ul);

  UInt64 q_UInt64_1(b);
  UInt64 q_UInt64_2(w);
  UInt64 q_UInt64_3(dw);
  UInt64 q_UInt64_4(qw);
  UInt64 q_UInt64_5(i8);
  UInt64 q_UInt64_6(ui8);
  UInt64 q_UInt64_7(i16);
  UInt64 q_UInt64_8(ui16);
  UInt64 q_UInt64_9(i32);
  UInt64 q_UInt64_10(ui32);
  UInt64 q_UInt64_11(i64);
  UInt64 q_UInt64_12(ui64);
  UInt64 q_UInt64_13(c);
  UInt64 q_UInt64_14(uc);
  UInt64 q_UInt64_15(s);
  UInt64 q_UInt64_16(us);
  UInt64 q_UInt64_17(i);
  UInt64 q_UInt64_18(ui);
  UInt64 q_UInt64_19(l);
  UInt64 q_UInt64_20(ul);

  Char q_Char_1(b);
  Char q_Char_2(w);
  Char q_Char_3(dw);
  Char q_Char_4(qw);
  Char q_Char_5(i8);
  Char q_Char_6(ui8);
  Char q_Char_7(i16);
  Char q_Char_8(ui16);
  Char q_Char_9(i32);
  Char q_Char_10(ui32);
  Char q_Char_11(i64);
  Char q_Char_12(ui64);
  Char q_Char_13(c);
  Char q_Char_14(uc);
  Char q_Char_15(s);
  Char q_Char_16(us);
  Char q_Char_17(i);
  Char q_Char_18(ui);
  Char q_Char_19(l);
  Char q_Char_20(ul);

  UChar q_UChar_1(b);
  UChar q_UChar_2(w);
  UChar q_UChar_3(dw);
  UChar q_UChar_4(qw);
  UChar q_UChar_5(i8);
  UChar q_UChar_6(ui8);
  UChar q_UChar_7(i16);
  UChar q_UChar_8(ui16);
  UChar q_UChar_9(i32);
  UChar q_UChar_10(ui32);
  UChar q_UChar_11(i64);
  UChar q_UChar_12(ui64);
  UChar q_UChar_13(c);
  UChar q_UChar_14(uc);
  UChar q_UChar_15(s);
  UChar q_UChar_16(us);
  UChar q_UChar_17(i);
  UChar q_UChar_18(ui);
  UChar q_UChar_19(l);
  UChar q_UChar_20(ul);

  Short q_Short_1(b);
  Short q_Short_2(w);
  Short q_Short_3(dw);
  Short q_Short_4(qw);
  Short q_Short_5(i8);
  Short q_Short_6(ui8);
  Short q_Short_7(i16);
  Short q_Short_8(ui16);
  Short q_Short_9(i32);
  Short q_Short_10(ui32);
  Short q_Short_11(i64);
  Short q_Short_12(ui64);
  Short q_Short_13(c);
  Short q_Short_14(uc);
  Short q_Short_15(s);
  Short q_Short_16(us);
  Short q_Short_17(i);
  Short q_Short_18(ui);
  Short q_Short_19(l);
  Short q_Short_20(ul);

  UShort q_UShort_1(b);
  UShort q_UShort_2(w);
  UShort q_UShort_3(dw);
  UShort q_UShort_4(qw);
  UShort q_UShort_5(i8);
  UShort q_UShort_6(ui8);
  UShort q_UShort_7(i16);
  UShort q_UShort_8(ui16);
  UShort q_UShort_9(i32);
  UShort q_UShort_10(ui32);
  UShort q_UShort_11(i64);
  UShort q_UShort_12(ui64);
  UShort q_UShort_13(c);
  UShort q_UShort_14(uc);
  UShort q_UShort_15(s);
  UShort q_UShort_16(us);
  UShort q_UShort_17(i);
  UShort q_UShort_18(ui);
  UShort q_UShort_19(l);
  UShort q_UShort_20(ul);

  Int q_Int_1(b);
  Int q_Int_2(w);
  Int q_Int_3(dw);
  Int q_Int_4(qw);
  Int q_Int_5(i8);
  Int q_Int_6(ui8);
  Int q_Int_7(i16);
  Int q_Int_8(ui16);
  Int q_Int_9(i32);
  Int q_Int_10(ui32);
  Int q_Int_11(i64);
  Int q_Int_12(ui64);
  Int q_Int_13(c);
  Int q_Int_14(uc);
  Int q_Int_15(s);
  Int q_Int_16(us);
  Int q_Int_17(i);
  Int q_Int_18(ui);
  Int q_Int_19(l);
  Int q_Int_20(ul);

  UInt q_UInt_1(b);
  UInt q_UInt_2(w);
  UInt q_UInt_3(dw);
  UInt q_UInt_4(qw);
  UInt q_UInt_5(i8);
  UInt q_UInt_6(ui8);
  UInt q_UInt_7(i16);
  UInt q_UInt_8(ui16);
  UInt q_UInt_9(i32);
  UInt q_UInt_10(ui32);
  UInt q_UInt_11(i64);
  UInt q_UInt_12(ui64);
  UInt q_UInt_13(c);
  UInt q_UInt_14(uc);
  UInt q_UInt_15(s);
  UInt q_UInt_16(us);
  UInt q_UInt_17(i);
  UInt q_UInt_18(ui);
  UInt q_UInt_19(l);
  UInt q_UInt_20(ul);

  Long q_Long_1(b);
  Long q_Long_2(w);
  Long q_Long_3(dw);
  Long q_Long_4(qw);
  Long q_Long_5(i8);
  Long q_Long_6(ui8);
  Long q_Long_7(i16);
  Long q_Long_8(ui16);
  Long q_Long_9(i32);
  Long q_Long_10(ui32);
  Long q_Long_11(i64);
  Long q_Long_12(ui64);
  Long q_Long_13(c);
  Long q_Long_14(uc);
  Long q_Long_15(s);
  Long q_Long_16(us);
  Long q_Long_17(i);
  Long q_Long_18(ui);
  Long q_Long_19(l);
  Long q_Long_20(ul);

  ULong q_ULong_1(b);
  ULong q_ULong_2(w);
  ULong q_ULong_3(dw);
  ULong q_ULong_4(qw);
  ULong q_ULong_5(i8);
  ULong q_ULong_6(ui8);
  ULong q_ULong_7(i16);
  ULong q_ULong_8(ui16);
  ULong q_ULong_9(i32);
  ULong q_ULong_10(ui32);
  ULong q_ULong_11(i64);
  ULong q_ULong_12(ui64);
  ULong q_ULong_13(c);
  ULong q_ULong_14(uc);
  ULong q_ULong_15(s);
  ULong q_ULong_16(us);
  ULong q_ULong_17(i);
  ULong q_ULong_18(ui);
  ULong q_ULong_19(l);
  ULong q_ULong_20(ul);

  Byte q_Byte_21(q_Byte_1);
  Byte q_Byte_22(q_Word_1);
  Byte q_Byte_23(q_DWord_1);
  Byte q_Byte_24(q_QWord_1);
  Byte q_Byte_25(q_Int8_1);
  Byte q_Byte_26(q_UInt8_1);
  Byte q_Byte_27(q_Int16_1);
  Byte q_Byte_28(q_UInt16_1);
  Byte q_Byte_29(q_Int32_1);
  Byte q_Byte_30(q_UInt32_1);
  Byte q_Byte_31(q_Int64_1);
  Byte q_Byte_32(q_UInt64_1);
  Byte q_Byte_33(q_Char_1);
  Byte q_Byte_34(q_UChar_1);
  Byte q_Byte_35(q_Short_1);
  Byte q_Byte_36(q_UShort_1);
  Byte q_Byte_37(q_Int_1);
  Byte q_Byte_38(q_UInt_1);
  Byte q_Byte_39(q_Long_1);
  Byte q_Byte_40(q_ULong_1);

  Word q_Word_21(q_Byte_1);
  Word q_Word_22(q_Word_1);
  Word q_Word_23(q_DWord_1);
  Word q_Word_24(q_QWord_1);
  Word q_Word_25(q_Int8_1);
  Word q_Word_26(q_UInt8_1);
  Word q_Word_27(q_Int16_1);
  Word q_Word_28(q_UInt16_1);
  Word q_Word_29(q_Int32_1);
  Word q_Word_30(q_UInt32_1);
  Word q_Word_31(q_Int64_1);
  Word q_Word_32(q_UInt64_1);
  Word q_Word_33(q_Char_1);
  Word q_Word_34(q_UChar_1);
  Word q_Word_35(q_Short_1);
  Word q_Word_36(q_UShort_1);
  Word q_Word_37(q_Int_1);
  Word q_Word_38(q_UInt_1);
  Word q_Word_39(q_Long_1);
  Word q_Word_40(q_ULong_1);

  DWord q_DWord_21(q_Byte_1);
  DWord q_DWord_22(q_Word_1);
  DWord q_DWord_23(q_DWord_1);
  DWord q_DWord_24(q_QWord_1);
  DWord q_DWord_25(q_Int8_1);
  DWord q_DWord_26(q_UInt8_1);
  DWord q_DWord_27(q_Int16_1);
  DWord q_DWord_28(q_UInt16_1);
  DWord q_DWord_29(q_Int32_1);
  DWord q_DWord_30(q_UInt32_1);
  DWord q_DWord_31(q_Int64_1);
  DWord q_DWord_32(q_UInt64_1);
  DWord q_DWord_33(q_Char_1);
  DWord q_DWord_34(q_UChar_1);
  DWord q_DWord_35(q_Short_1);
  DWord q_DWord_36(q_UShort_1);
  DWord q_DWord_37(q_Int_1);
  DWord q_DWord_38(q_UInt_1);
  DWord q_DWord_39(q_Long_1);
  DWord q_DWord_40(q_ULong_1);

  QWord q_QWord_21(q_Byte_1);
  QWord q_QWord_22(q_Word_1);
  QWord q_QWord_23(q_DWord_1);
  QWord q_QWord_24(q_QWord_1);
  QWord q_QWord_25(q_Int8_1);
  QWord q_QWord_26(q_UInt8_1);
  QWord q_QWord_27(q_Int16_1);
  QWord q_QWord_28(q_UInt16_1);
  QWord q_QWord_29(q_Int32_1);
  QWord q_QWord_30(q_UInt32_1);
  QWord q_QWord_31(q_Int64_1);
  QWord q_QWord_32(q_UInt64_1);
  QWord q_QWord_33(q_Char_1);
  QWord q_QWord_34(q_UChar_1);
  QWord q_QWord_35(q_Short_1);
  QWord q_QWord_36(q_UShort_1);
  QWord q_QWord_37(q_Int_1);
  QWord q_QWord_38(q_UInt_1);
  QWord q_QWord_39(q_Long_1);
  QWord q_QWord_40(q_ULong_1);

  Int8 q_Int8_21(q_Byte_1);
  Int8 q_Int8_22(q_Word_1);
  Int8 q_Int8_23(q_DWord_1);
  Int8 q_Int8_24(q_QWord_1);
  Int8 q_Int8_25(q_Int8_1);
  Int8 q_Int8_26(q_UInt8_1);
  Int8 q_Int8_27(q_Int16_1);
  Int8 q_Int8_28(q_UInt16_1);
  Int8 q_Int8_29(q_Int32_1);
  Int8 q_Int8_30(q_UInt32_1);
  Int8 q_Int8_31(q_Int64_1);
  Int8 q_Int8_32(q_UInt64_1);
  Int8 q_Int8_33(q_Char_1);
  Int8 q_Int8_34(q_UChar_1);
  Int8 q_Int8_35(q_Short_1);
  Int8 q_Int8_36(q_UShort_1);
  Int8 q_Int8_37(q_Int_1);
  Int8 q_Int8_38(q_UInt_1);
  Int8 q_Int8_39(q_Long_1);
  Int8 q_Int8_40(q_ULong_1);

  UInt8 q_UInt8_21(q_Byte_1);
  UInt8 q_UInt8_22(q_Word_1);
  UInt8 q_UInt8_23(q_DWord_1);
  UInt8 q_UInt8_24(q_QWord_1);
  UInt8 q_UInt8_25(q_Int8_1);
  UInt8 q_UInt8_26(q_UInt8_1);
  UInt8 q_UInt8_27(q_Int16_1);
  UInt8 q_UInt8_28(q_UInt16_1);
  UInt8 q_UInt8_29(q_Int32_1);
  UInt8 q_UInt8_30(q_UInt32_1);
  UInt8 q_UInt8_31(q_Int64_1);
  UInt8 q_UInt8_32(q_UInt64_1);
  UInt8 q_UInt8_33(q_Char_1);
  UInt8 q_UInt8_34(q_UChar_1);
  UInt8 q_UInt8_35(q_Short_1);
  UInt8 q_UInt8_36(q_UShort_1);
  UInt8 q_UInt8_37(q_Int_1);
  UInt8 q_UInt8_38(q_UInt_1);
  UInt8 q_UInt8_39(q_Long_1);
  UInt8 q_UInt8_40(q_ULong_1);

  Int16 q_Int16_21(q_Byte_1);
  Int16 q_Int16_22(q_Word_1);
  Int16 q_Int16_23(q_DWord_1);
  Int16 q_Int16_24(q_QWord_1);
  Int16 q_Int16_25(q_Int8_1);
  Int16 q_Int16_26(q_UInt8_1);
  Int16 q_Int16_27(q_Int16_1);
  Int16 q_Int16_28(q_UInt16_1);
  Int16 q_Int16_29(q_Int32_1);
  Int16 q_Int16_30(q_UInt32_1);
  Int16 q_Int16_31(q_Int64_1);
  Int16 q_Int16_32(q_UInt64_1);
  Int16 q_Int16_33(q_Char_1);
  Int16 q_Int16_34(q_UChar_1);
  Int16 q_Int16_35(q_Short_1);
  Int16 q_Int16_36(q_UShort_1);
  Int16 q_Int16_37(q_Int_1);
  Int16 q_Int16_38(q_UInt_1);
  Int16 q_Int16_39(q_Long_1);
  Int16 q_Int16_40(q_ULong_1);

  UInt16 q_UInt16_21(q_Byte_1);
  UInt16 q_UInt16_22(q_Word_1);
  UInt16 q_UInt16_23(q_DWord_1);
  UInt16 q_UInt16_24(q_QWord_1);
  UInt16 q_UInt16_25(q_Int8_1);
  UInt16 q_UInt16_26(q_UInt8_1);
  UInt16 q_UInt16_27(q_Int16_1);
  UInt16 q_UInt16_28(q_UInt16_1);
  UInt16 q_UInt16_29(q_Int32_1);
  UInt16 q_UInt16_30(q_UInt32_1);
  UInt16 q_UInt16_31(q_Int64_1);
  UInt16 q_UInt16_32(q_UInt64_1);
  UInt16 q_UInt16_33(q_Char_1);
  UInt16 q_UInt16_34(q_UChar_1);
  UInt16 q_UInt16_35(q_Short_1);
  UInt16 q_UInt16_36(q_UShort_1);
  UInt16 q_UInt16_37(q_Int_1);
  UInt16 q_UInt16_38(q_UInt_1);
  UInt16 q_UInt16_39(q_Long_1);
  UInt16 q_UInt16_40(q_ULong_1);

  Int32 q_Int32_21(q_Byte_1);
  Int32 q_Int32_22(q_Word_1);
  Int32 q_Int32_23(q_DWord_1);
  Int32 q_Int32_24(q_QWord_1);
  Int32 q_Int32_25(q_Int8_1);
  Int32 q_Int32_26(q_UInt8_1);
  Int32 q_Int32_27(q_Int16_1);
  Int32 q_Int32_28(q_UInt16_1);
  Int32 q_Int32_29(q_Int32_1);
  Int32 q_Int32_30(q_UInt32_1);
  Int32 q_Int32_31(q_Int64_1);
  Int32 q_Int32_32(q_UInt64_1);
  Int32 q_Int32_33(q_Char_1);
  Int32 q_Int32_34(q_UChar_1);
  Int32 q_Int32_35(q_Short_1);
  Int32 q_Int32_36(q_UShort_1);
  Int32 q_Int32_37(q_Int_1);
  Int32 q_Int32_38(q_UInt_1);
  Int32 q_Int32_39(q_Long_1);
  Int32 q_Int32_40(q_ULong_1);

  UInt32 q_UInt32_21(q_Byte_1);
  UInt32 q_UInt32_22(q_Word_1);
  UInt32 q_UInt32_23(q_DWord_1);
  UInt32 q_UInt32_24(q_QWord_1);
  UInt32 q_UInt32_25(q_Int8_1);
  UInt32 q_UInt32_26(q_UInt8_1);
  UInt32 q_UInt32_27(q_Int16_1);
  UInt32 q_UInt32_28(q_UInt16_1);
  UInt32 q_UInt32_29(q_Int32_1);
  UInt32 q_UInt32_30(q_UInt32_1);
  UInt32 q_UInt32_31(q_Int64_1);
  UInt32 q_UInt32_32(q_UInt64_1);
  UInt32 q_UInt32_33(q_Char_1);
  UInt32 q_UInt32_34(q_UChar_1);
  UInt32 q_UInt32_35(q_Short_1);
  UInt32 q_UInt32_36(q_UShort_1);
  UInt32 q_UInt32_37(q_Int_1);
  UInt32 q_UInt32_38(q_UInt_1);
  UInt32 q_UInt32_39(q_Long_1);
  UInt32 q_UInt32_40(q_ULong_1);

  Int64 q_Int64_21(q_Byte_1);
  Int64 q_Int64_22(q_Word_1);
  Int64 q_Int64_23(q_DWord_1);
  Int64 q_Int64_24(q_QWord_1);
  Int64 q_Int64_25(q_Int8_1);
  Int64 q_Int64_26(q_UInt8_1);
  Int64 q_Int64_27(q_Int16_1);
  Int64 q_Int64_28(q_UInt16_1);
  Int64 q_Int64_29(q_Int32_1);
  Int64 q_Int64_30(q_UInt32_1);
  Int64 q_Int64_31(q_Int64_1);
  Int64 q_Int64_32(q_UInt64_1);
  Int64 q_Int64_33(q_Char_1);
  Int64 q_Int64_34(q_UChar_1);
  Int64 q_Int64_35(q_Short_1);
  Int64 q_Int64_36(q_UShort_1);
  Int64 q_Int64_37(q_Int_1);
  Int64 q_Int64_38(q_UInt_1);
  Int64 q_Int64_39(q_Long_1);
  Int64 q_Int64_40(q_ULong_1);

  UInt64 q_UInt64_21(q_Byte_1);
  UInt64 q_UInt64_22(q_Word_1);
  UInt64 q_UInt64_23(q_DWord_1);
  UInt64 q_UInt64_24(q_QWord_1);
  UInt64 q_UInt64_25(q_Int8_1);
  UInt64 q_UInt64_26(q_UInt8_1);
  UInt64 q_UInt64_27(q_Int16_1);
  UInt64 q_UInt64_28(q_UInt16_1);
  UInt64 q_UInt64_29(q_Int32_1);
  UInt64 q_UInt64_30(q_UInt32_1);
  UInt64 q_UInt64_31(q_Int64_1);
  UInt64 q_UInt64_32(q_UInt64_1);
  UInt64 q_UInt64_33(q_Char_1);
  UInt64 q_UInt64_34(q_UChar_1);
  UInt64 q_UInt64_35(q_Short_1);
  UInt64 q_UInt64_36(q_UShort_1);
  UInt64 q_UInt64_37(q_Int_1);
  UInt64 q_UInt64_38(q_UInt_1);
  UInt64 q_UInt64_39(q_Long_1);
  UInt64 q_UInt64_40(q_ULong_1);

  Char q_Char_21(q_Byte_1);
  Char q_Char_22(q_Word_1);
  Char q_Char_23(q_DWord_1);
  Char q_Char_24(q_QWord_1);
  Char q_Char_25(q_Int8_1);
  Char q_Char_26(q_UInt8_1);
  Char q_Char_27(q_Int16_1);
  Char q_Char_28(q_UInt16_1);
  Char q_Char_29(q_Int32_1);
  Char q_Char_30(q_UInt32_1);
  Char q_Char_31(q_Int64_1);
  Char q_Char_32(q_UInt64_1);
  Char q_Char_33(q_Char_1);
  Char q_Char_34(q_UChar_1);
  Char q_Char_35(q_Short_1);
  Char q_Char_36(q_UShort_1);
  Char q_Char_37(q_Int_1);
  Char q_Char_38(q_UInt_1);
  Char q_Char_39(q_Long_1);
  Char q_Char_40(q_ULong_1);

  UChar q_UChar_21(q_Byte_1);
  UChar q_UChar_22(q_Word_1);
  UChar q_UChar_23(q_DWord_1);
  UChar q_UChar_24(q_QWord_1);
  UChar q_UChar_25(q_Int8_1);
  UChar q_UChar_26(q_UInt8_1);
  UChar q_UChar_27(q_Int16_1);
  UChar q_UChar_28(q_UInt16_1);
  UChar q_UChar_29(q_Int32_1);
  UChar q_UChar_30(q_UInt32_1);
  UChar q_UChar_31(q_Int64_1);
  UChar q_UChar_32(q_UInt64_1);
  UChar q_UChar_33(q_Char_1);
  UChar q_UChar_34(q_UChar_1);
  UChar q_UChar_35(q_Short_1);
  UChar q_UChar_36(q_UShort_1);
  UChar q_UChar_37(q_Int_1);
  UChar q_UChar_38(q_UInt_1);
  UChar q_UChar_39(q_Long_1);
  UChar q_UChar_40(q_ULong_1);

  Short q_Short_21(q_Byte_1);
  Short q_Short_22(q_Word_1);
  Short q_Short_23(q_DWord_1);
  Short q_Short_24(q_QWord_1);
  Short q_Short_25(q_Int8_1);
  Short q_Short_26(q_UInt8_1);
  Short q_Short_27(q_Int16_1);
  Short q_Short_28(q_UInt16_1);
  Short q_Short_29(q_Int32_1);
  Short q_Short_30(q_UInt32_1);
  Short q_Short_31(q_Int64_1);
  Short q_Short_32(q_UInt64_1);
  Short q_Short_33(q_Char_1);
  Short q_Short_34(q_UChar_1);
  Short q_Short_35(q_Short_1);
  Short q_Short_36(q_UShort_1);
  Short q_Short_37(q_Int_1);
  Short q_Short_38(q_UInt_1);
  Short q_Short_39(q_Long_1);
  Short q_Short_40(q_ULong_1);

  UShort q_UShort_21(q_Byte_1);
  UShort q_UShort_22(q_Word_1);
  UShort q_UShort_23(q_DWord_1);
  UShort q_UShort_24(q_QWord_1);
  UShort q_UShort_25(q_Int8_1);
  UShort q_UShort_26(q_UInt8_1);
  UShort q_UShort_27(q_Int16_1);
  UShort q_UShort_28(q_UInt16_1);
  UShort q_UShort_29(q_Int32_1);
  UShort q_UShort_30(q_UInt32_1);
  UShort q_UShort_31(q_Int64_1);
  UShort q_UShort_32(q_UInt64_1);
  UShort q_UShort_33(q_Char_1);
  UShort q_UShort_34(q_UChar_1);
  UShort q_UShort_35(q_Short_1);
  UShort q_UShort_36(q_UShort_1);
  UShort q_UShort_37(q_Int_1);
  UShort q_UShort_38(q_UInt_1);
  UShort q_UShort_39(q_Long_1);
  UShort q_UShort_40(q_ULong_1);

  Int q_Int_21(q_Byte_1);
  Int q_Int_22(q_Word_1);
  Int q_Int_23(q_DWord_1);
  Int q_Int_24(q_QWord_1);
  Int q_Int_25(q_Int8_1);
  Int q_Int_26(q_UInt8_1);
  Int q_Int_27(q_Int16_1);
  Int q_Int_28(q_UInt16_1);
  Int q_Int_29(q_Int32_1);
  Int q_Int_30(q_UInt32_1);
  Int q_Int_31(q_Int64_1);
  Int q_Int_32(q_UInt64_1);
  Int q_Int_33(q_Char_1);
  Int q_Int_34(q_UChar_1);
  Int q_Int_35(q_Short_1);
  Int q_Int_36(q_UShort_1);
  Int q_Int_37(q_Int_1);
  Int q_Int_38(q_UInt_1);
  Int q_Int_39(q_Long_1);
  Int q_Int_40(q_ULong_1);

  UInt q_UInt_21(q_Byte_1);
  UInt q_UInt_22(q_Word_1);
  UInt q_UInt_23(q_DWord_1);
  UInt q_UInt_24(q_QWord_1);
  UInt q_UInt_25(q_Int8_1);
  UInt q_UInt_26(q_UInt8_1);
  UInt q_UInt_27(q_Int16_1);
  UInt q_UInt_28(q_UInt16_1);
  UInt q_UInt_29(q_Int32_1);
  UInt q_UInt_30(q_UInt32_1);
  UInt q_UInt_31(q_Int64_1);
  UInt q_UInt_32(q_UInt64_1);
  UInt q_UInt_33(q_Char_1);
  UInt q_UInt_34(q_UChar_1);
  UInt q_UInt_35(q_Short_1);
  UInt q_UInt_36(q_UShort_1);
  UInt q_UInt_37(q_Int_1);
  UInt q_UInt_38(q_UInt_1);
  UInt q_UInt_39(q_Long_1);
  UInt q_UInt_40(q_ULong_1);

  Long q_Long_21(q_Byte_1);
  Long q_Long_22(q_Word_1);
  Long q_Long_23(q_DWord_1);
  Long q_Long_24(q_QWord_1);
  Long q_Long_25(q_Int8_1);
  Long q_Long_26(q_UInt8_1);
  Long q_Long_27(q_Int16_1);
  Long q_Long_28(q_UInt16_1);
  Long q_Long_29(q_Int32_1);
  Long q_Long_30(q_UInt32_1);
  Long q_Long_31(q_Int64_1);
  Long q_Long_32(q_UInt64_1);
  Long q_Long_33(q_Char_1);
  Long q_Long_34(q_UChar_1);
  Long q_Long_35(q_Short_1);
  Long q_Long_36(q_UShort_1);
  Long q_Long_37(q_Int_1);
  Long q_Long_38(q_UInt_1);
  Long q_Long_39(q_Long_1);
  Long q_Long_40(q_ULong_1);

  ULong q_ULong_21(q_Byte_1);
  ULong q_ULong_22(q_Word_1);
  ULong q_ULong_23(q_DWord_1);
  ULong q_ULong_24(q_QWord_1);
  ULong q_ULong_25(q_Int8_1);
  ULong q_ULong_26(q_UInt8_1);
  ULong q_ULong_27(q_Int16_1);
  ULong q_ULong_28(q_UInt16_1);
  ULong q_ULong_29(q_Int32_1);
  ULong q_ULong_30(q_UInt32_1);
  ULong q_ULong_31(q_Int64_1);
  ULong q_ULong_32(q_UInt64_1);
  ULong q_ULong_33(q_Char_1);
  ULong q_ULong_34(q_UChar_1);
  ULong q_ULong_35(q_Short_1);
  ULong q_ULong_36(q_UShort_1);
  ULong q_ULong_37(q_Int_1);
  ULong q_ULong_38(q_UInt_1);
  ULong q_ULong_39(q_Long_1);
  ULong q_ULong_40(q_ULong_1);

  return;
}

void ImplicitConstructor(void) {
  BYTE b = 1;
  WORD w = 1;
  DWORD dw = 1;
  QWORD qw = 1;
  int8 i8 = 1;
  uint8 ui8 = 1;
  int16 i16 = 1;
  uint16 ui16 = 1;
  int32 i32 = 1;
  uint32 ui32 = 1;
  int64 i64 = 1;
  uint64 ui64 = 1;
  signed char c = 1;
  unsigned char uc = 1;
  signed short s = 1;
  unsigned short us = 1;
  signed int i = 1;
  unsigned int ui = 1;
  signed long l = 1;
  unsigned long ul = 1;

  Byte q_Byte_1 = b;
  Byte q_Byte_2 = w;
  Byte q_Byte_3 = dw;
  Byte q_Byte_4 = qw;
  Byte q_Byte_5 = i8;
  Byte q_Byte_6 = ui8;
  Byte q_Byte_7 = i16;
  Byte q_Byte_8 = ui16;
  Byte q_Byte_9 = i32;
  Byte q_Byte_10 = ui32;
  Byte q_Byte_11 = i64;
  Byte q_Byte_12 = ui64;
  Byte q_Byte_13 = c;
  Byte q_Byte_14 = uc;
  Byte q_Byte_15 = s;
  Byte q_Byte_16 = us;
  Byte q_Byte_17 = i;
  Byte q_Byte_18 = ui;
  Byte q_Byte_19 = l;
  Byte q_Byte_20 = ul;

  Word q_Word_1 = b;
  Word q_Word_2 = w;
  Word q_Word_3 = dw;
  Word q_Word_4 = qw;
  Word q_Word_5 = i8;
  Word q_Word_6 = ui8;
  Word q_Word_7 = i16;
  Word q_Word_8 = ui16;
  Word q_Word_9 = i32;
  Word q_Word_10 = ui32;
  Word q_Word_11 = i64;
  Word q_Word_12 = ui64;
  Word q_Word_13 = c;
  Word q_Word_14 = uc;
  Word q_Word_15 = s;
  Word q_Word_16 = us;
  Word q_Word_17 = i;
  Word q_Word_18 = ui;
  Word q_Word_19 = l;
  Word q_Word_20 = ul;

  DWord q_DWord_1 = b;
  DWord q_DWord_2 = w;
  DWord q_DWord_3 = dw;
  DWord q_DWord_4 = qw;
  DWord q_DWord_5 = i8;
  DWord q_DWord_6 = ui8;
  DWord q_DWord_7 = i16;
  DWord q_DWord_8 = ui16;
  DWord q_DWord_9 = i32;
  DWord q_DWord_10 = ui32;
  DWord q_DWord_11 = i64;
  DWord q_DWord_12 = ui64;
  DWord q_DWord_13 = c;
  DWord q_DWord_14 = uc;
  DWord q_DWord_15 = s;
  DWord q_DWord_16 = us;
  DWord q_DWord_17 = i;
  DWord q_DWord_18 = ui;
  DWord q_DWord_19 = l;
  DWord q_DWord_20 = ul;

  QWord q_QWord_1 = b;
  QWord q_QWord_2 = w;
  QWord q_QWord_3 = dw;
  QWord q_QWord_4 = qw;
  QWord q_QWord_5 = i8;
  QWord q_QWord_6 = ui8;
  QWord q_QWord_7 = i16;
  QWord q_QWord_8 = ui16;
  QWord q_QWord_9 = i32;
  QWord q_QWord_10 = ui32;
  QWord q_QWord_11 = i64;
  QWord q_QWord_12 = ui64;
  QWord q_QWord_13 = c;
  QWord q_QWord_14 = uc;
  QWord q_QWord_15 = s;
  QWord q_QWord_16 = us;
  QWord q_QWord_17 = i;
  QWord q_QWord_18 = ui;
  QWord q_QWord_19 = l;
  QWord q_QWord_20 = ul;

  Int8 q_Int8_1 = b;
  Int8 q_Int8_2 = w;
  Int8 q_Int8_3 = dw;
  Int8 q_Int8_4 = qw;
  Int8 q_Int8_5 = i8;
  Int8 q_Int8_6 = ui8;
  Int8 q_Int8_7 = i16;
  Int8 q_Int8_8 = ui16;
  Int8 q_Int8_9 = i32;
  Int8 q_Int8_10 = ui32;
  Int8 q_Int8_11 = i64;
  Int8 q_Int8_12 = ui64;
  Int8 q_Int8_13 = c;
  Int8 q_Int8_14 = uc;
  Int8 q_Int8_15 = s;
  Int8 q_Int8_16 = us;
  Int8 q_Int8_17 = i;
  Int8 q_Int8_18 = ui;
  Int8 q_Int8_19 = l;
  Int8 q_Int8_20 = ul;

  UInt8 q_UInt8_1 = b;
  UInt8 q_UInt8_2 = w;
  UInt8 q_UInt8_3 = dw;
  UInt8 q_UInt8_4 = qw;
  UInt8 q_UInt8_5 = i8;
  UInt8 q_UInt8_6 = ui8;
  UInt8 q_UInt8_7 = i16;
  UInt8 q_UInt8_8 = ui16;
  UInt8 q_UInt8_9 = i32;
  UInt8 q_UInt8_10 = ui32;
  UInt8 q_UInt8_11 = i64;
  UInt8 q_UInt8_12 = ui64;
  UInt8 q_UInt8_13 = c;
  UInt8 q_UInt8_14 = uc;
  UInt8 q_UInt8_15 = s;
  UInt8 q_UInt8_16 = us;
  UInt8 q_UInt8_17 = i;
  UInt8 q_UInt8_18 = ui;
  UInt8 q_UInt8_19 = l;
  UInt8 q_UInt8_20 = ul;

  Int16 q_Int16_1 = b;
  Int16 q_Int16_2 = w;
  Int16 q_Int16_3 = dw;
  Int16 q_Int16_4 = qw;
  Int16 q_Int16_5 = i8;
  Int16 q_Int16_6 = ui8;
  Int16 q_Int16_7 = i16;
  Int16 q_Int16_8 = ui16;
  Int16 q_Int16_9 = i32;
  Int16 q_Int16_10 = ui32;
  Int16 q_Int16_11 = i64;
  Int16 q_Int16_12 = ui64;
  Int16 q_Int16_13 = c;
  Int16 q_Int16_14 = uc;
  Int16 q_Int16_15 = s;
  Int16 q_Int16_16 = us;
  Int16 q_Int16_17 = i;
  Int16 q_Int16_18 = ui;
  Int16 q_Int16_19 = l;
  Int16 q_Int16_20 = ul;

  UInt16 q_UInt16_1 = b;
  UInt16 q_UInt16_2 = w;
  UInt16 q_UInt16_3 = dw;
  UInt16 q_UInt16_4 = qw;
  UInt16 q_UInt16_5 = i8;
  UInt16 q_UInt16_6 = ui8;
  UInt16 q_UInt16_7 = i16;
  UInt16 q_UInt16_8 = ui16;
  UInt16 q_UInt16_9 = i32;
  UInt16 q_UInt16_10 = ui32;
  UInt16 q_UInt16_11 = i64;
  UInt16 q_UInt16_12 = ui64;
  UInt16 q_UInt16_13 = c;
  UInt16 q_UInt16_14 = uc;
  UInt16 q_UInt16_15 = s;
  UInt16 q_UInt16_16 = us;
  UInt16 q_UInt16_17 = i;
  UInt16 q_UInt16_18 = ui;
  UInt16 q_UInt16_19 = l;
  UInt16 q_UInt16_20 = ul;

  Int32 q_Int32_1 = b;
  Int32 q_Int32_2 = w;
  Int32 q_Int32_3 = dw;
  Int32 q_Int32_4 = qw;
  Int32 q_Int32_5 = i8;
  Int32 q_Int32_6 = ui8;
  Int32 q_Int32_7 = i16;
  Int32 q_Int32_8 = ui16;
  Int32 q_Int32_9 = i32;
  Int32 q_Int32_10 = ui32;
  Int32 q_Int32_11 = i64;
  Int32 q_Int32_12 = ui64;
  Int32 q_Int32_13 = c;
  Int32 q_Int32_14 = uc;
  Int32 q_Int32_15 = s;
  Int32 q_Int32_16 = us;
  Int32 q_Int32_17 = i;
  Int32 q_Int32_18 = ui;
  Int32 q_Int32_19 = l;
  Int32 q_Int32_20 = ul;

  UInt32 q_UInt32_1 = b;
  UInt32 q_UInt32_2 = w;
  UInt32 q_UInt32_3 = dw;
  UInt32 q_UInt32_4 = qw;
  UInt32 q_UInt32_5 = i8;
  UInt32 q_UInt32_6 = ui8;
  UInt32 q_UInt32_7 = i16;
  UInt32 q_UInt32_8 = ui16;
  UInt32 q_UInt32_9 = i32;
  UInt32 q_UInt32_10 = ui32;
  UInt32 q_UInt32_11 = i64;
  UInt32 q_UInt32_12 = ui64;
  UInt32 q_UInt32_13 = c;
  UInt32 q_UInt32_14 = uc;
  UInt32 q_UInt32_15 = s;
  UInt32 q_UInt32_16 = us;
  UInt32 q_UInt32_17 = i;
  UInt32 q_UInt32_18 = ui;
  UInt32 q_UInt32_19 = l;
  UInt32 q_UInt32_20 = ul;

  Int64 q_Int64_1 = b;
  Int64 q_Int64_2 = w;
  Int64 q_Int64_3 = dw;
  Int64 q_Int64_4 = qw;
  Int64 q_Int64_5 = i8;
  Int64 q_Int64_6 = ui8;
  Int64 q_Int64_7 = i16;
  Int64 q_Int64_8 = ui16;
  Int64 q_Int64_9 = i32;
  Int64 q_Int64_10 = ui32;
  Int64 q_Int64_11 = i64;
  Int64 q_Int64_12 = ui64;
  Int64 q_Int64_13 = c;
  Int64 q_Int64_14 = uc;
  Int64 q_Int64_15 = s;
  Int64 q_Int64_16 = us;
  Int64 q_Int64_17 = i;
  Int64 q_Int64_18 = ui;
  Int64 q_Int64_19 = l;
  Int64 q_Int64_20 = ul;

  UInt64 q_UInt64_1 = b;
  UInt64 q_UInt64_2 = w;
  UInt64 q_UInt64_3 = dw;
  UInt64 q_UInt64_4 = qw;
  UInt64 q_UInt64_5 = i8;
  UInt64 q_UInt64_6 = ui8;
  UInt64 q_UInt64_7 = i16;
  UInt64 q_UInt64_8 = ui16;
  UInt64 q_UInt64_9 = i32;
  UInt64 q_UInt64_10 = ui32;
  UInt64 q_UInt64_11 = i64;
  UInt64 q_UInt64_12 = ui64;
  UInt64 q_UInt64_13 = c;
  UInt64 q_UInt64_14 = uc;
  UInt64 q_UInt64_15 = s;
  UInt64 q_UInt64_16 = us;
  UInt64 q_UInt64_17 = i;
  UInt64 q_UInt64_18 = ui;
  UInt64 q_UInt64_19 = l;
  UInt64 q_UInt64_20 = ul;

  Char q_Char_1 = b;
  Char q_Char_2 = w;
  Char q_Char_3 = dw;
  Char q_Char_4 = qw;
  Char q_Char_5 = i8;
  Char q_Char_6 = ui8;
  Char q_Char_7 = i16;
  Char q_Char_8 = ui16;
  Char q_Char_9 = i32;
  Char q_Char_10 = ui32;
  Char q_Char_11 = i64;
  Char q_Char_12 = ui64;
  Char q_Char_13 = c;
  Char q_Char_14 = uc;
  Char q_Char_15 = s;
  Char q_Char_16 = us;
  Char q_Char_17 = i;
  Char q_Char_18 = ui;
  Char q_Char_19 = l;
  Char q_Char_20 = ul;

  UChar q_UChar_1 = b;
  UChar q_UChar_2 = w;
  UChar q_UChar_3 = dw;
  UChar q_UChar_4 = qw;
  UChar q_UChar_5 = i8;
  UChar q_UChar_6 = ui8;
  UChar q_UChar_7 = i16;
  UChar q_UChar_8 = ui16;
  UChar q_UChar_9 = i32;
  UChar q_UChar_10 = ui32;
  UChar q_UChar_11 = i64;
  UChar q_UChar_12 = ui64;
  UChar q_UChar_13 = c;
  UChar q_UChar_14 = uc;
  UChar q_UChar_15 = s;
  UChar q_UChar_16 = us;
  UChar q_UChar_17 = i;
  UChar q_UChar_18 = ui;
  UChar q_UChar_19 = l;
  UChar q_UChar_20 = ul;

  Short q_Short_1 = b;
  Short q_Short_2 = w;
  Short q_Short_3 = dw;
  Short q_Short_4 = qw;
  Short q_Short_5 = i8;
  Short q_Short_6 = ui8;
  Short q_Short_7 = i16;
  Short q_Short_8 = ui16;
  Short q_Short_9 = i32;
  Short q_Short_10 = ui32;
  Short q_Short_11 = i64;
  Short q_Short_12 = ui64;
  Short q_Short_13 = c;
  Short q_Short_14 = uc;
  Short q_Short_15 = s;
  Short q_Short_16 = us;
  Short q_Short_17 = i;
  Short q_Short_18 = ui;
  Short q_Short_19 = l;
  Short q_Short_20 = ul;

  UShort q_UShort_1 = b;
  UShort q_UShort_2 = w;
  UShort q_UShort_3 = dw;
  UShort q_UShort_4 = qw;
  UShort q_UShort_5 = i8;
  UShort q_UShort_6 = ui8;
  UShort q_UShort_7 = i16;
  UShort q_UShort_8 = ui16;
  UShort q_UShort_9 = i32;
  UShort q_UShort_10 = ui32;
  UShort q_UShort_11 = i64;
  UShort q_UShort_12 = ui64;
  UShort q_UShort_13 = c;
  UShort q_UShort_14 = uc;
  UShort q_UShort_15 = s;
  UShort q_UShort_16 = us;
  UShort q_UShort_17 = i;
  UShort q_UShort_18 = ui;
  UShort q_UShort_19 = l;
  UShort q_UShort_20 = ul;

  Int q_Int_1 = b;
  Int q_Int_2 = w;
  Int q_Int_3 = dw;
  Int q_Int_4 = qw;
  Int q_Int_5 = i8;
  Int q_Int_6 = ui8;
  Int q_Int_7 = i16;
  Int q_Int_8 = ui16;
  Int q_Int_9 = i32;
  Int q_Int_10 = ui32;
  Int q_Int_11 = i64;
  Int q_Int_12 = ui64;
  Int q_Int_13 = c;
  Int q_Int_14 = uc;
  Int q_Int_15 = s;
  Int q_Int_16 = us;
  Int q_Int_17 = i;
  Int q_Int_18 = ui;
  Int q_Int_19 = l;
  Int q_Int_20 = ul;

  UInt q_UInt_1 = b;
  UInt q_UInt_2 = w;
  UInt q_UInt_3 = dw;
  UInt q_UInt_4 = qw;
  UInt q_UInt_5 = i8;
  UInt q_UInt_6 = ui8;
  UInt q_UInt_7 = i16;
  UInt q_UInt_8 = ui16;
  UInt q_UInt_9 = i32;
  UInt q_UInt_10 = ui32;
  UInt q_UInt_11 = i64;
  UInt q_UInt_12 = ui64;
  UInt q_UInt_13 = c;
  UInt q_UInt_14 = uc;
  UInt q_UInt_15 = s;
  UInt q_UInt_16 = us;
  UInt q_UInt_17 = i;
  UInt q_UInt_18 = ui;
  UInt q_UInt_19 = l;
  UInt q_UInt_20 = ul;

  Long q_Long_1 = b;
  Long q_Long_2 = w;
  Long q_Long_3 = dw;
  Long q_Long_4 = qw;
  Long q_Long_5 = i8;
  Long q_Long_6 = ui8;
  Long q_Long_7 = i16;
  Long q_Long_8 = ui16;
  Long q_Long_9 = i32;
  Long q_Long_10 = ui32;
  Long q_Long_11 = i64;
  Long q_Long_12 = ui64;
  Long q_Long_13 = c;
  Long q_Long_14 = uc;
  Long q_Long_15 = s;
  Long q_Long_16 = us;
  Long q_Long_17 = i;
  Long q_Long_18 = ui;
  Long q_Long_19 = l;
  Long q_Long_20 = ul;

  ULong q_ULong_1 = b;
  ULong q_ULong_2 = w;
  ULong q_ULong_3 = dw;
  ULong q_ULong_4 = qw;
  ULong q_ULong_5 = i8;
  ULong q_ULong_6 = ui8;
  ULong q_ULong_7 = i16;
  ULong q_ULong_8 = ui16;
  ULong q_ULong_9 = i32;
  ULong q_ULong_10 = ui32;
  ULong q_ULong_11 = i64;
  ULong q_ULong_12 = ui64;
  ULong q_ULong_13 = c;
  ULong q_ULong_14 = uc;
  ULong q_ULong_15 = s;
  ULong q_ULong_16 = us;
  ULong q_ULong_17 = i;
  ULong q_ULong_18 = ui;
  ULong q_ULong_19 = l;
  ULong q_ULong_20 = ul;

  Byte q_Byte_21 = q_Byte_1;
  Byte q_Byte_22 = q_Word_1;
  Byte q_Byte_23 = q_DWord_1;
  Byte q_Byte_24 = q_QWord_1;
  Byte q_Byte_25 = q_Int8_1;
  Byte q_Byte_26 = q_UInt8_1;
  Byte q_Byte_27 = q_Int16_1;
  Byte q_Byte_28 = q_UInt16_1;
  Byte q_Byte_29 = q_Int32_1;
  Byte q_Byte_30 = q_UInt32_1;
  Byte q_Byte_31 = q_Int64_1;
  Byte q_Byte_32 = q_UInt64_1;
  Byte q_Byte_33 = q_Char_1;
  Byte q_Byte_34 = q_UChar_1;
  Byte q_Byte_35 = q_Short_1;
  Byte q_Byte_36 = q_UShort_1;
  Byte q_Byte_37 = q_Int_1;
  Byte q_Byte_38 = q_UInt_1;
  Byte q_Byte_39 = q_Long_1;
  Byte q_Byte_40 = q_ULong_1;

  Word q_Word_21 = q_Byte_1;
  Word q_Word_22 = q_Word_1;
  Word q_Word_23 = q_DWord_1;
  Word q_Word_24 = q_QWord_1;
  Word q_Word_25 = q_Int8_1;
  Word q_Word_26 = q_UInt8_1;
  Word q_Word_27 = q_Int16_1;
  Word q_Word_28 = q_UInt16_1;
  Word q_Word_29 = q_Int32_1;
  Word q_Word_30 = q_UInt32_1;
  Word q_Word_31 = q_Int64_1;
  Word q_Word_32 = q_UInt64_1;
  Word q_Word_33 = q_Char_1;
  Word q_Word_34 = q_UChar_1;
  Word q_Word_35 = q_Short_1;
  Word q_Word_36 = q_UShort_1;
  Word q_Word_37 = q_Int_1;
  Word q_Word_38 = q_UInt_1;
  Word q_Word_39 = q_Long_1;
  Word q_Word_40 = q_ULong_1;

  DWord q_DWord_21 = q_Byte_1;
  DWord q_DWord_22 = q_Word_1;
  DWord q_DWord_23 = q_DWord_1;
  DWord q_DWord_24 = q_QWord_1;
  DWord q_DWord_25 = q_Int8_1;
  DWord q_DWord_26 = q_UInt8_1;
  DWord q_DWord_27 = q_Int16_1;
  DWord q_DWord_28 = q_UInt16_1;
  DWord q_DWord_29 = q_Int32_1;
  DWord q_DWord_30 = q_UInt32_1;
  DWord q_DWord_31 = q_Int64_1;
  DWord q_DWord_32 = q_UInt64_1;
  DWord q_DWord_33 = q_Char_1;
  DWord q_DWord_34 = q_UChar_1;
  DWord q_DWord_35 = q_Short_1;
  DWord q_DWord_36 = q_UShort_1;
  DWord q_DWord_37 = q_Int_1;
  DWord q_DWord_38 = q_UInt_1;
  DWord q_DWord_39 = q_Long_1;
  DWord q_DWord_40 = q_ULong_1;

  QWord q_QWord_21 = q_Byte_1;
  QWord q_QWord_22 = q_Word_1;
  QWord q_QWord_23 = q_DWord_1;
  QWord q_QWord_24 = q_QWord_1;
  QWord q_QWord_25 = q_Int8_1;
  QWord q_QWord_26 = q_UInt8_1;
  QWord q_QWord_27 = q_Int16_1;
  QWord q_QWord_28 = q_UInt16_1;
  QWord q_QWord_29 = q_Int32_1;
  QWord q_QWord_30 = q_UInt32_1;
  QWord q_QWord_31 = q_Int64_1;
  QWord q_QWord_32 = q_UInt64_1;
  QWord q_QWord_33 = q_Char_1;
  QWord q_QWord_34 = q_UChar_1;
  QWord q_QWord_35 = q_Short_1;
  QWord q_QWord_36 = q_UShort_1;
  QWord q_QWord_37 = q_Int_1;
  QWord q_QWord_38 = q_UInt_1;
  QWord q_QWord_39 = q_Long_1;
  QWord q_QWord_40 = q_ULong_1;

  Int8 q_Int8_21 = q_Byte_1;
  Int8 q_Int8_22 = q_Word_1;
  Int8 q_Int8_23 = q_DWord_1;
  Int8 q_Int8_24 = q_QWord_1;
  Int8 q_Int8_25 = q_Int8_1;
  Int8 q_Int8_26 = q_UInt8_1;
  Int8 q_Int8_27 = q_Int16_1;
  Int8 q_Int8_28 = q_UInt16_1;
  Int8 q_Int8_29 = q_Int32_1;
  Int8 q_Int8_30 = q_UInt32_1;
  Int8 q_Int8_31 = q_Int64_1;
  Int8 q_Int8_32 = q_UInt64_1;
  Int8 q_Int8_33 = q_Char_1;
  Int8 q_Int8_34 = q_UChar_1;
  Int8 q_Int8_35 = q_Short_1;
  Int8 q_Int8_36 = q_UShort_1;
  Int8 q_Int8_37 = q_Int_1;
  Int8 q_Int8_38 = q_UInt_1;
  Int8 q_Int8_39 = q_Long_1;
  Int8 q_Int8_40 = q_ULong_1;

  UInt8 q_UInt8_21 = q_Byte_1;
  UInt8 q_UInt8_22 = q_Word_1;
  UInt8 q_UInt8_23 = q_DWord_1;
  UInt8 q_UInt8_24 = q_QWord_1;
  UInt8 q_UInt8_25 = q_Int8_1;
  UInt8 q_UInt8_26 = q_UInt8_1;
  UInt8 q_UInt8_27 = q_Int16_1;
  UInt8 q_UInt8_28 = q_UInt16_1;
  UInt8 q_UInt8_29 = q_Int32_1;
  UInt8 q_UInt8_30 = q_UInt32_1;
  UInt8 q_UInt8_31 = q_Int64_1;
  UInt8 q_UInt8_32 = q_UInt64_1;
  UInt8 q_UInt8_33 = q_Char_1;
  UInt8 q_UInt8_34 = q_UChar_1;
  UInt8 q_UInt8_35 = q_Short_1;
  UInt8 q_UInt8_36 = q_UShort_1;
  UInt8 q_UInt8_37 = q_Int_1;
  UInt8 q_UInt8_38 = q_UInt_1;
  UInt8 q_UInt8_39 = q_Long_1;
  UInt8 q_UInt8_40 = q_ULong_1;

  Int16 q_Int16_21 = q_Byte_1;
  Int16 q_Int16_22 = q_Word_1;
  Int16 q_Int16_23 = q_DWord_1;
  Int16 q_Int16_24 = q_QWord_1;
  Int16 q_Int16_25 = q_Int8_1;
  Int16 q_Int16_26 = q_UInt8_1;
  Int16 q_Int16_27 = q_Int16_1;
  Int16 q_Int16_28 = q_UInt16_1;
  Int16 q_Int16_29 = q_Int32_1;
  Int16 q_Int16_30 = q_UInt32_1;
  Int16 q_Int16_31 = q_Int64_1;
  Int16 q_Int16_32 = q_UInt64_1;
  Int16 q_Int16_33 = q_Char_1;
  Int16 q_Int16_34 = q_UChar_1;
  Int16 q_Int16_35 = q_Short_1;
  Int16 q_Int16_36 = q_UShort_1;
  Int16 q_Int16_37 = q_Int_1;
  Int16 q_Int16_38 = q_UInt_1;
  Int16 q_Int16_39 = q_Long_1;
  Int16 q_Int16_40 = q_ULong_1;

  UInt16 q_UInt16_21 = q_Byte_1;
  UInt16 q_UInt16_22 = q_Word_1;
  UInt16 q_UInt16_23 = q_DWord_1;
  UInt16 q_UInt16_24 = q_QWord_1;
  UInt16 q_UInt16_25 = q_Int8_1;
  UInt16 q_UInt16_26 = q_UInt8_1;
  UInt16 q_UInt16_27 = q_Int16_1;
  UInt16 q_UInt16_28 = q_UInt16_1;
  UInt16 q_UInt16_29 = q_Int32_1;
  UInt16 q_UInt16_30 = q_UInt32_1;
  UInt16 q_UInt16_31 = q_Int64_1;
  UInt16 q_UInt16_32 = q_UInt64_1;
  UInt16 q_UInt16_33 = q_Char_1;
  UInt16 q_UInt16_34 = q_UChar_1;
  UInt16 q_UInt16_35 = q_Short_1;
  UInt16 q_UInt16_36 = q_UShort_1;
  UInt16 q_UInt16_37 = q_Int_1;
  UInt16 q_UInt16_38 = q_UInt_1;
  UInt16 q_UInt16_39 = q_Long_1;
  UInt16 q_UInt16_40 = q_ULong_1;

  Int32 q_Int32_21 = q_Byte_1;
  Int32 q_Int32_22 = q_Word_1;
  Int32 q_Int32_23 = q_DWord_1;
  Int32 q_Int32_24 = q_QWord_1;
  Int32 q_Int32_25 = q_Int8_1;
  Int32 q_Int32_26 = q_UInt8_1;
  Int32 q_Int32_27 = q_Int16_1;
  Int32 q_Int32_28 = q_UInt16_1;
  Int32 q_Int32_29 = q_Int32_1;
  Int32 q_Int32_30 = q_UInt32_1;
  Int32 q_Int32_31 = q_Int64_1;
  Int32 q_Int32_32 = q_UInt64_1;
  Int32 q_Int32_33 = q_Char_1;
  Int32 q_Int32_34 = q_UChar_1;
  Int32 q_Int32_35 = q_Short_1;
  Int32 q_Int32_36 = q_UShort_1;
  Int32 q_Int32_37 = q_Int_1;
  Int32 q_Int32_38 = q_UInt_1;
  Int32 q_Int32_39 = q_Long_1;
  Int32 q_Int32_40 = q_ULong_1;

  UInt32 q_UInt32_21 = q_Byte_1;
  UInt32 q_UInt32_22 = q_Word_1;
  UInt32 q_UInt32_23 = q_DWord_1;
  UInt32 q_UInt32_24 = q_QWord_1;
  UInt32 q_UInt32_25 = q_Int8_1;
  UInt32 q_UInt32_26 = q_UInt8_1;
  UInt32 q_UInt32_27 = q_Int16_1;
  UInt32 q_UInt32_28 = q_UInt16_1;
  UInt32 q_UInt32_29 = q_Int32_1;
  UInt32 q_UInt32_30 = q_UInt32_1;
  UInt32 q_UInt32_31 = q_Int64_1;
  UInt32 q_UInt32_32 = q_UInt64_1;
  UInt32 q_UInt32_33 = q_Char_1;
  UInt32 q_UInt32_34 = q_UChar_1;
  UInt32 q_UInt32_35 = q_Short_1;
  UInt32 q_UInt32_36 = q_UShort_1;
  UInt32 q_UInt32_37 = q_Int_1;
  UInt32 q_UInt32_38 = q_UInt_1;
  UInt32 q_UInt32_39 = q_Long_1;
  UInt32 q_UInt32_40 = q_ULong_1;

  Int64 q_Int64_21 = q_Byte_1;
  Int64 q_Int64_22 = q_Word_1;
  Int64 q_Int64_23 = q_DWord_1;
  Int64 q_Int64_24 = q_QWord_1;
  Int64 q_Int64_25 = q_Int8_1;
  Int64 q_Int64_26 = q_UInt8_1;
  Int64 q_Int64_27 = q_Int16_1;
  Int64 q_Int64_28 = q_UInt16_1;
  Int64 q_Int64_29 = q_Int32_1;
  Int64 q_Int64_30 = q_UInt32_1;
  Int64 q_Int64_31 = q_Int64_1;
  Int64 q_Int64_32 = q_UInt64_1;
  Int64 q_Int64_33 = q_Char_1;
  Int64 q_Int64_34 = q_UChar_1;
  Int64 q_Int64_35 = q_Short_1;
  Int64 q_Int64_36 = q_UShort_1;
  Int64 q_Int64_37 = q_Int_1;
  Int64 q_Int64_38 = q_UInt_1;
  Int64 q_Int64_39 = q_Long_1;
  Int64 q_Int64_40 = q_ULong_1;

  UInt64 q_UInt64_21 = q_Byte_1;
  UInt64 q_UInt64_22 = q_Word_1;
  UInt64 q_UInt64_23 = q_DWord_1;
  UInt64 q_UInt64_24 = q_QWord_1;
  UInt64 q_UInt64_25 = q_Int8_1;
  UInt64 q_UInt64_26 = q_UInt8_1;
  UInt64 q_UInt64_27 = q_Int16_1;
  UInt64 q_UInt64_28 = q_UInt16_1;
  UInt64 q_UInt64_29 = q_Int32_1;
  UInt64 q_UInt64_30 = q_UInt32_1;
  UInt64 q_UInt64_31 = q_Int64_1;
  UInt64 q_UInt64_32 = q_UInt64_1;
  UInt64 q_UInt64_33 = q_Char_1;
  UInt64 q_UInt64_34 = q_UChar_1;
  UInt64 q_UInt64_35 = q_Short_1;
  UInt64 q_UInt64_36 = q_UShort_1;
  UInt64 q_UInt64_37 = q_Int_1;
  UInt64 q_UInt64_38 = q_UInt_1;
  UInt64 q_UInt64_39 = q_Long_1;
  UInt64 q_UInt64_40 = q_ULong_1;

  Char q_Char_21 = q_Byte_1;
  Char q_Char_22 = q_Word_1;
  Char q_Char_23 = q_DWord_1;
  Char q_Char_24 = q_QWord_1;
  Char q_Char_25 = q_Int8_1;
  Char q_Char_26 = q_UInt8_1;
  Char q_Char_27 = q_Int16_1;
  Char q_Char_28 = q_UInt16_1;
  Char q_Char_29 = q_Int32_1;
  Char q_Char_30 = q_UInt32_1;
  Char q_Char_31 = q_Int64_1;
  Char q_Char_32 = q_UInt64_1;
  Char q_Char_33 = q_Char_1;
  Char q_Char_34 = q_UChar_1;
  Char q_Char_35 = q_Short_1;
  Char q_Char_36 = q_UShort_1;
  Char q_Char_37 = q_Int_1;
  Char q_Char_38 = q_UInt_1;
  Char q_Char_39 = q_Long_1;
  Char q_Char_40 = q_ULong_1;

  UChar q_UChar_21 = q_Byte_1;
  UChar q_UChar_22 = q_Word_1;
  UChar q_UChar_23 = q_DWord_1;
  UChar q_UChar_24 = q_QWord_1;
  UChar q_UChar_25 = q_Int8_1;
  UChar q_UChar_26 = q_UInt8_1;
  UChar q_UChar_27 = q_Int16_1;
  UChar q_UChar_28 = q_UInt16_1;
  UChar q_UChar_29 = q_Int32_1;
  UChar q_UChar_30 = q_UInt32_1;
  UChar q_UChar_31 = q_Int64_1;
  UChar q_UChar_32 = q_UInt64_1;
  UChar q_UChar_33 = q_Char_1;
  UChar q_UChar_34 = q_UChar_1;
  UChar q_UChar_35 = q_Short_1;
  UChar q_UChar_36 = q_UShort_1;
  UChar q_UChar_37 = q_Int_1;
  UChar q_UChar_38 = q_UInt_1;
  UChar q_UChar_39 = q_Long_1;
  UChar q_UChar_40 = q_ULong_1;

  Short q_Short_21 = q_Byte_1;
  Short q_Short_22 = q_Word_1;
  Short q_Short_23 = q_DWord_1;
  Short q_Short_24 = q_QWord_1;
  Short q_Short_25 = q_Int8_1;
  Short q_Short_26 = q_UInt8_1;
  Short q_Short_27 = q_Int16_1;
  Short q_Short_28 = q_UInt16_1;
  Short q_Short_29 = q_Int32_1;
  Short q_Short_30 = q_UInt32_1;
  Short q_Short_31 = q_Int64_1;
  Short q_Short_32 = q_UInt64_1;
  Short q_Short_33 = q_Char_1;
  Short q_Short_34 = q_UChar_1;
  Short q_Short_35 = q_Short_1;
  Short q_Short_36 = q_UShort_1;
  Short q_Short_37 = q_Int_1;
  Short q_Short_38 = q_UInt_1;
  Short q_Short_39 = q_Long_1;
  Short q_Short_40 = q_ULong_1;

  UShort q_UShort_21 = q_Byte_1;
  UShort q_UShort_22 = q_Word_1;
  UShort q_UShort_23 = q_DWord_1;
  UShort q_UShort_24 = q_QWord_1;
  UShort q_UShort_25 = q_Int8_1;
  UShort q_UShort_26 = q_UInt8_1;
  UShort q_UShort_27 = q_Int16_1;
  UShort q_UShort_28 = q_UInt16_1;
  UShort q_UShort_29 = q_Int32_1;
  UShort q_UShort_30 = q_UInt32_1;
  UShort q_UShort_31 = q_Int64_1;
  UShort q_UShort_32 = q_UInt64_1;
  UShort q_UShort_33 = q_Char_1;
  UShort q_UShort_34 = q_UChar_1;
  UShort q_UShort_35 = q_Short_1;
  UShort q_UShort_36 = q_UShort_1;
  UShort q_UShort_37 = q_Int_1;
  UShort q_UShort_38 = q_UInt_1;
  UShort q_UShort_39 = q_Long_1;
  UShort q_UShort_40 = q_ULong_1;

  Int q_Int_21 = q_Byte_1;
  Int q_Int_22 = q_Word_1;
  Int q_Int_23 = q_DWord_1;
  Int q_Int_24 = q_QWord_1;
  Int q_Int_25 = q_Int8_1;
  Int q_Int_26 = q_UInt8_1;
  Int q_Int_27 = q_Int16_1;
  Int q_Int_28 = q_UInt16_1;
  Int q_Int_29 = q_Int32_1;
  Int q_Int_30 = q_UInt32_1;
  Int q_Int_31 = q_Int64_1;
  Int q_Int_32 = q_UInt64_1;
  Int q_Int_33 = q_Char_1;
  Int q_Int_34 = q_UChar_1;
  Int q_Int_35 = q_Short_1;
  Int q_Int_36 = q_UShort_1;
  Int q_Int_37 = q_Int_1;
  Int q_Int_38 = q_UInt_1;
  Int q_Int_39 = q_Long_1;
  Int q_Int_40 = q_ULong_1;

  UInt q_UInt_21 = q_Byte_1;
  UInt q_UInt_22 = q_Word_1;
  UInt q_UInt_23 = q_DWord_1;
  UInt q_UInt_24 = q_QWord_1;
  UInt q_UInt_25 = q_Int8_1;
  UInt q_UInt_26 = q_UInt8_1;
  UInt q_UInt_27 = q_Int16_1;
  UInt q_UInt_28 = q_UInt16_1;
  UInt q_UInt_29 = q_Int32_1;
  UInt q_UInt_30 = q_UInt32_1;
  UInt q_UInt_31 = q_Int64_1;
  UInt q_UInt_32 = q_UInt64_1;
  UInt q_UInt_33 = q_Char_1;
  UInt q_UInt_34 = q_UChar_1;
  UInt q_UInt_35 = q_Short_1;
  UInt q_UInt_36 = q_UShort_1;
  UInt q_UInt_37 = q_Int_1;
  UInt q_UInt_38 = q_UInt_1;
  UInt q_UInt_39 = q_Long_1;
  UInt q_UInt_40 = q_ULong_1;

  Long q_Long_21 = q_Byte_1;
  Long q_Long_22 = q_Word_1;
  Long q_Long_23 = q_DWord_1;
  Long q_Long_24 = q_QWord_1;
  Long q_Long_25 = q_Int8_1;
  Long q_Long_26 = q_UInt8_1;
  Long q_Long_27 = q_Int16_1;
  Long q_Long_28 = q_UInt16_1;
  Long q_Long_29 = q_Int32_1;
  Long q_Long_30 = q_UInt32_1;
  Long q_Long_31 = q_Int64_1;
  Long q_Long_32 = q_UInt64_1;
  Long q_Long_33 = q_Char_1;
  Long q_Long_34 = q_UChar_1;
  Long q_Long_35 = q_Short_1;
  Long q_Long_36 = q_UShort_1;
  Long q_Long_37 = q_Int_1;
  Long q_Long_38 = q_UInt_1;
  Long q_Long_39 = q_Long_1;
  Long q_Long_40 = q_ULong_1;

  ULong q_ULong_21 = q_Byte_1;
  ULong q_ULong_22 = q_Word_1;
  ULong q_ULong_23 = q_DWord_1;
  ULong q_ULong_24 = q_QWord_1;
  ULong q_ULong_25 = q_Int8_1;
  ULong q_ULong_26 = q_UInt8_1;
  ULong q_ULong_27 = q_Int16_1;
  ULong q_ULong_28 = q_UInt16_1;
  ULong q_ULong_29 = q_Int32_1;
  ULong q_ULong_30 = q_UInt32_1;
  ULong q_ULong_31 = q_Int64_1;
  ULong q_ULong_32 = q_UInt64_1;
  ULong q_ULong_33 = q_Char_1;
  ULong q_ULong_34 = q_UChar_1;
  ULong q_ULong_35 = q_Short_1;
  ULong q_ULong_36 = q_UShort_1;
  ULong q_ULong_37 = q_Int_1;
  ULong q_ULong_38 = q_UInt_1;
  ULong q_ULong_39 = q_Long_1;
  ULong q_ULong_40 = q_ULong_1;

  Byte q_Byte_41 = 1;
  Byte q_Byte_42 = -1;
  Byte q_Byte_43 = 0x1f;
  Byte q_Byte_44 = 'a';

  Word q_Word_41 = 1;
  Word q_Word_42 = -1;
  Word q_Word_43 = 0x1f;
  Word q_Word_44 = 'a';

  DWord q_DWord_41 = 1;
  DWord q_DWord_42 = -1;
  DWord q_DWord_43 = 0x1f;
  DWord q_DWord_44 = 'a';

  QWord q_QWord_41 = 1;
  QWord q_QWord_42 = -1;
  QWord q_QWord_43 = 0x1f;
  QWord q_QWord_44 = 'a';

  Int8 q_Int8_41 = 1;
  Int8 q_Int8_42 = -1;
  Int8 q_Int8_43 = 0x1f;
  Int8 q_Int8_44 = 'a';

  UInt8 q_UInt8_41 = 1;
  UInt8 q_UInt8_42 = -1;
  UInt8 q_UInt8_43 = 0x1f;
  UInt8 q_UInt8_44 = 'a';

  Int16 q_Int16_41 = 1;
  Int16 q_Int16_42 = -1;
  Int16 q_Int16_43 = 0x1f;
  Int16 q_Int16_44 = 'a';

  UInt16 q_UInt16_41 = 1;
  UInt16 q_UInt16_42 = -1;
  UInt16 q_UInt16_43 = 0x1f;
  UInt16 q_UInt16_44 = 'a';

  Int32 q_Int32_41 = 1;
  Int32 q_Int32_42 = -1;
  Int32 q_Int32_43 = 0x1f;
  Int32 q_Int32_44 = 'a';

  UInt32 q_UInt32_41 = 1;
  UInt32 q_UInt32_42 = -1;
  UInt32 q_UInt32_43 = 0x1f;
  UInt32 q_UInt32_44 = 'a';

  Int64 q_Int64_41 = 1;
  Int64 q_Int64_42 = -1;
  Int64 q_Int64_43 = 0x1f;
  Int64 q_Int64_44 = 'a';

  UInt64 q_UInt64_41 = 1;
  UInt64 q_UInt64_42 = -1;
  UInt64 q_UInt64_43 = 0x1f;
  UInt64 q_UInt64_44 = 'a';

  Char q_Char_41 = 1;
  Char q_Char_42 = -1;
  Char q_Char_43 = 0x1f;
  Char q_Char_44 = 'a';

  UChar q_UChar_41 = 1;
  UChar q_UChar_42 = -1;
  UChar q_UChar_43 = 0x1f;
  UChar q_UChar_44 = 'a';

  Short q_Short_41 = 1;
  Short q_Short_42 = -1;
  Short q_Short_43 = 0x1f;
  Short q_Short_44 = 'a';

  UShort q_UShort_41 = 1;
  UShort q_UShort_42 = -1;
  UShort q_UShort_43 = 0x1f;
  UShort q_UShort_44 = 'a';

  Int q_Int_41 = 1;
  Int q_Int_42 = -1;
  Int q_Int_43 = 0x1f;
  Int q_Int_44 = 'a';

  UInt q_UInt_41 = 1;
  UInt q_UInt_42 = -1;
  UInt q_UInt_43 = 0x1f;
  UInt q_UInt_44 = 'a';

  Long q_Long_41 = 1;
  Long q_Long_42 = -1;
  Long q_Long_43 = 0x1f;
  Long q_Long_44 = 'a';

  ULong q_ULong_41 = 1;
  ULong q_ULong_42 = -1;
  ULong q_ULong_43 = 0x1f;
  ULong q_ULong_44 = 'a';

  return;
}

void Assignment(void) {
  BYTE b = 1;
  WORD w = 1;
  DWORD dw = 1;
  QWORD qw = 1;
  int8 i8 = 1;
  uint8 ui8 = 1;
  int16 i16 = 1;
  uint16 ui16 = 1;
  int32 i32 = 1;
  uint32 ui32 = 1;
  int64 i64 = 1;
  uint64 ui64 = 1;
  signed char c = 1;
  unsigned char uc = 1;
  signed short s = 1;
  unsigned short us = 1;
  signed int i = 1;
  unsigned int ui = 1;
  signed long l = 1;
  unsigned long ul = 1;

  Byte q_Byte_1;
  Word q_Word_1;
  DWord q_DWord_1;
  QWord q_QWord_1;
  Int8 q_Int8_1;
  UInt8 q_UInt8_1;
  Int16 q_Int16_1;
  UInt16 q_UInt16_1;
  Int32 q_Int32_1;
  UInt32 q_UInt32_1;
  Int64 q_Int64_1;
  UInt64 q_UInt64_1;
  Char q_Char_1;
  UChar q_UChar_1;
  Short q_Short_1;
  UShort q_UShort_1;
  Int q_Int_1;
  UInt q_UInt_1;
  Long q_Long_1;
  ULong q_ULong_1;

  q_Byte_1 = b;
  q_Byte_1 = w;
  q_Byte_1 = dw;
  q_Byte_1 = qw;
  q_Byte_1 = i8;
  q_Byte_1 = ui8;
  q_Byte_1 = i16;
  q_Byte_1 = ui16;
  q_Byte_1 = i32;
  q_Byte_1 = ui32;
  q_Byte_1 = i64;
  q_Byte_1 = ui64;
  q_Byte_1 = c;
  q_Byte_1 = uc;
  q_Byte_1 = s;
  q_Byte_1 = us;
  q_Byte_1 = i;
  q_Byte_1 = ui;
  q_Byte_1 = l;
  q_Byte_1 = ul;

  q_Word_1 = b;
  q_Word_1 = w;
  q_Word_1 = dw;
  q_Word_1 = qw;
  q_Word_1 = i8;
  q_Word_1 = ui8;
  q_Word_1 = i16;
  q_Word_1 = ui16;
  q_Word_1 = i32;
  q_Word_1 = ui32;
  q_Word_1 = i64;
  q_Word_1 = ui64;
  q_Word_1 = c;
  q_Word_1 = uc;
  q_Word_1 = s;
  q_Word_1 = us;
  q_Word_1 = i;
  q_Word_1 = ui;
  q_Word_1 = l;
  q_Word_1 = ul;

  q_DWord_1 = b;
  q_DWord_1 = w;
  q_DWord_1 = dw;
  q_DWord_1 = qw;
  q_DWord_1 = i8;
  q_DWord_1 = ui8;
  q_DWord_1 = i16;
  q_DWord_1 = ui16;
  q_DWord_1 = i32;
  q_DWord_1 = ui32;
  q_DWord_1 = i64;
  q_DWord_1 = ui64;
  q_DWord_1 = c;
  q_DWord_1 = uc;
  q_DWord_1 = s;
  q_DWord_1 = us;
  q_DWord_1 = i;
  q_DWord_1 = ui;
  q_DWord_1 = l;
  q_DWord_1 = ul;

  q_QWord_1 = b;
  q_QWord_1 = w;
  q_QWord_1 = dw;
  q_QWord_1 = qw;
  q_QWord_1 = i8;
  q_QWord_1 = ui8;
  q_QWord_1 = i16;
  q_QWord_1 = ui16;
  q_QWord_1 = i32;
  q_QWord_1 = ui32;
  q_QWord_1 = i64;
  q_QWord_1 = ui64;
  q_QWord_1 = c;
  q_QWord_1 = uc;
  q_QWord_1 = s;
  q_QWord_1 = us;
  q_QWord_1 = i;
  q_QWord_1 = ui;
  q_QWord_1 = l;
  q_QWord_1 = ul;

  q_Int8_1 = b;
  q_Int8_1 = w;
  q_Int8_1 = dw;
  q_Int8_1 = qw;
  q_Int8_1 = i8;
  q_Int8_1 = ui8;
  q_Int8_1 = i16;
  q_Int8_1 = ui16;
  q_Int8_1 = i32;
  q_Int8_1 = ui32;
  q_Int8_1 = i64;
  q_Int8_1 = ui64;
  q_Int8_1 = c;
  q_Int8_1 = uc;
  q_Int8_1 = s;
  q_Int8_1 = us;
  q_Int8_1 = i;
  q_Int8_1 = ui;
  q_Int8_1 = l;
  q_Int8_1 = ul;

  q_UInt8_1 = b;
  q_UInt8_1 = w;
  q_UInt8_1 = dw;
  q_UInt8_1 = qw;
  q_UInt8_1 = i8;
  q_UInt8_1 = ui8;
  q_UInt8_1 = i16;
  q_UInt8_1 = ui16;
  q_UInt8_1 = i32;
  q_UInt8_1 = ui32;
  q_UInt8_1 = i64;
  q_UInt8_1 = ui64;
  q_UInt8_1 = c;
  q_UInt8_1 = uc;
  q_UInt8_1 = s;
  q_UInt8_1 = us;
  q_UInt8_1 = i;
  q_UInt8_1 = ui;
  q_UInt8_1 = l;
  q_UInt8_1 = ul;

  q_Int16_1 = b;
  q_Int16_1 = w;
  q_Int16_1 = dw;
  q_Int16_1 = qw;
  q_Int16_1 = i8;
  q_Int16_1 = ui8;
  q_Int16_1 = i16;
  q_Int16_1 = ui16;
  q_Int16_1 = i32;
  q_Int16_1 = ui32;
  q_Int16_1 = i64;
  q_Int16_1 = ui64;
  q_Int16_1 = c;
  q_Int16_1 = uc;
  q_Int16_1 = s;
  q_Int16_1 = us;
  q_Int16_1 = i;
  q_Int16_1 = ui;
  q_Int16_1 = l;
  q_Int16_1 = ul;

  q_UInt16_1 = b;
  q_UInt16_1 = w;
  q_UInt16_1 = dw;
  q_UInt16_1 = qw;
  q_UInt16_1 = i8;
  q_UInt16_1 = ui8;
  q_UInt16_1 = i16;
  q_UInt16_1 = ui16;
  q_UInt16_1 = i32;
  q_UInt16_1 = ui32;
  q_UInt16_1 = i64;
  q_UInt16_1 = ui64;
  q_UInt16_1 = c;
  q_UInt16_1 = uc;
  q_UInt16_1 = s;
  q_UInt16_1 = us;
  q_UInt16_1 = i;
  q_UInt16_1 = ui;
  q_UInt16_1 = l;
  q_UInt16_1 = ul;

  q_Int32_1 = b;
  q_Int32_1 = w;
  q_Int32_1 = dw;
  q_Int32_1 = qw;
  q_Int32_1 = i8;
  q_Int32_1 = ui8;
  q_Int32_1 = i16;
  q_Int32_1 = ui16;
  q_Int32_1 = i32;
  q_Int32_1 = ui32;
  q_Int32_1 = i64;
  q_Int32_1 = ui64;
  q_Int32_1 = c;
  q_Int32_1 = uc;
  q_Int32_1 = s;
  q_Int32_1 = us;
  q_Int32_1 = i;
  q_Int32_1 = ui;
  q_Int32_1 = l;
  q_Int32_1 = ul;

  q_UInt32_1 = b;
  q_UInt32_1 = w;
  q_UInt32_1 = dw;
  q_UInt32_1 = qw;
  q_UInt32_1 = i8;
  q_UInt32_1 = ui8;
  q_UInt32_1 = i16;
  q_UInt32_1 = ui16;
  q_UInt32_1 = i32;
  q_UInt32_1 = ui32;
  q_UInt32_1 = i64;
  q_UInt32_1 = ui64;
  q_UInt32_1 = c;
  q_UInt32_1 = uc;
  q_UInt32_1 = s;
  q_UInt32_1 = us;
  q_UInt32_1 = i;
  q_UInt32_1 = ui;
  q_UInt32_1 = l;
  q_UInt32_1 = ul;

  q_Int64_1 = b;
  q_Int64_1 = w;
  q_Int64_1 = dw;
  q_Int64_1 = qw;
  q_Int64_1 = i8;
  q_Int64_1 = ui8;
  q_Int64_1 = i16;
  q_Int64_1 = ui16;
  q_Int64_1 = i32;
  q_Int64_1 = ui32;
  q_Int64_1 = i64;
  q_Int64_1 = ui64;
  q_Int64_1 = c;
  q_Int64_1 = uc;
  q_Int64_1 = s;
  q_Int64_1 = us;
  q_Int64_1 = i;
  q_Int64_1 = ui;
  q_Int64_1 = l;
  q_Int64_1 = ul;

  q_UInt64_1 = b;
  q_UInt64_1 = w;
  q_UInt64_1 = dw;
  q_UInt64_1 = qw;
  q_UInt64_1 = i8;
  q_UInt64_1 = ui8;
  q_UInt64_1 = i16;
  q_UInt64_1 = ui16;
  q_UInt64_1 = i32;
  q_UInt64_1 = ui32;
  q_UInt64_1 = i64;
  q_UInt64_1 = ui64;
  q_UInt64_1 = c;
  q_UInt64_1 = uc;
  q_UInt64_1 = s;
  q_UInt64_1 = us;
  q_UInt64_1 = i;
  q_UInt64_1 = ui;
  q_UInt64_1 = l;
  q_UInt64_1 = ul;

  q_Char_1 = b;
  q_Char_1 = w;
  q_Char_1 = dw;
  q_Char_1 = qw;
  q_Char_1 = i8;
  q_Char_1 = ui8;
  q_Char_1 = i16;
  q_Char_1 = ui16;
  q_Char_1 = i32;
  q_Char_1 = ui32;
  q_Char_1 = i64;
  q_Char_1 = ui64;
  q_Char_1 = c;
  q_Char_1 = uc;
  q_Char_1 = s;
  q_Char_1 = us;
  q_Char_1 = i;
  q_Char_1 = ui;
  q_Char_1 = l;
  q_Char_1 = ul;

  q_UChar_1 = b;
  q_UChar_1 = w;
  q_UChar_1 = dw;
  q_UChar_1 = qw;
  q_UChar_1 = i8;
  q_UChar_1 = ui8;
  q_UChar_1 = i16;
  q_UChar_1 = ui16;
  q_UChar_1 = i32;
  q_UChar_1 = ui32;
  q_UChar_1 = i64;
  q_UChar_1 = ui64;
  q_UChar_1 = c;
  q_UChar_1 = uc;
  q_UChar_1 = s;
  q_UChar_1 = us;
  q_UChar_1 = i;
  q_UChar_1 = ui;
  q_UChar_1 = l;
  q_UChar_1 = ul;

  q_Short_1 = b;
  q_Short_1 = w;
  q_Short_1 = dw;
  q_Short_1 = qw;
  q_Short_1 = i8;
  q_Short_1 = ui8;
  q_Short_1 = i16;
  q_Short_1 = ui16;
  q_Short_1 = i32;
  q_Short_1 = ui32;
  q_Short_1 = i64;
  q_Short_1 = ui64;
  q_Short_1 = c;
  q_Short_1 = uc;
  q_Short_1 = s;
  q_Short_1 = us;
  q_Short_1 = i;
  q_Short_1 = ui;
  q_Short_1 = l;
  q_Short_1 = ul;

  q_UShort_1 = b;
  q_UShort_1 = w;
  q_UShort_1 = dw;
  q_UShort_1 = qw;
  q_UShort_1 = i8;
  q_UShort_1 = ui8;
  q_UShort_1 = i16;
  q_UShort_1 = ui16;
  q_UShort_1 = i32;
  q_UShort_1 = ui32;
  q_UShort_1 = i64;
  q_UShort_1 = ui64;
  q_UShort_1 = c;
  q_UShort_1 = uc;
  q_UShort_1 = s;
  q_UShort_1 = us;
  q_UShort_1 = i;
  q_UShort_1 = ui;
  q_UShort_1 = l;
  q_UShort_1 = ul;

  q_Int_1 = b;
  q_Int_1 = w;
  q_Int_1 = dw;
  q_Int_1 = qw;
  q_Int_1 = i8;
  q_Int_1 = ui8;
  q_Int_1 = i16;
  q_Int_1 = ui16;
  q_Int_1 = i32;
  q_Int_1 = ui32;
  q_Int_1 = i64;
  q_Int_1 = ui64;
  q_Int_1 = c;
  q_Int_1 = uc;
  q_Int_1 = s;
  q_Int_1 = us;
  q_Int_1 = i;
  q_Int_1 = ui;
  q_Int_1 = l;
  q_Int_1 = ul;

  q_UInt_1 = b;
  q_UInt_1 = w;
  q_UInt_1 = dw;
  q_UInt_1 = qw;
  q_UInt_1 = i8;
  q_UInt_1 = ui8;
  q_UInt_1 = i16;
  q_UInt_1 = ui16;
  q_UInt_1 = i32;
  q_UInt_1 = ui32;
  q_UInt_1 = i64;
  q_UInt_1 = ui64;
  q_UInt_1 = c;
  q_UInt_1 = uc;
  q_UInt_1 = s;
  q_UInt_1 = us;
  q_UInt_1 = i;
  q_UInt_1 = ui;
  q_UInt_1 = l;
  q_UInt_1 = ul;

  q_Long_1 = b;
  q_Long_1 = w;
  q_Long_1 = dw;
  q_Long_1 = qw;
  q_Long_1 = i8;
  q_Long_1 = ui8;
  q_Long_1 = i16;
  q_Long_1 = ui16;
  q_Long_1 = i32;
  q_Long_1 = ui32;
  q_Long_1 = i64;
  q_Long_1 = ui64;
  q_Long_1 = c;
  q_Long_1 = uc;
  q_Long_1 = s;
  q_Long_1 = us;
  q_Long_1 = i;
  q_Long_1 = ui;
  q_Long_1 = l;
  q_Long_1 = ul;

  q_ULong_1 = b;
  q_ULong_1 = w;
  q_ULong_1 = dw;
  q_ULong_1 = qw;
  q_ULong_1 = i8;
  q_ULong_1 = ui8;
  q_ULong_1 = i16;
  q_ULong_1 = ui16;
  q_ULong_1 = i32;
  q_ULong_1 = ui32;
  q_ULong_1 = i64;
  q_ULong_1 = ui64;
  q_ULong_1 = c;
  q_ULong_1 = uc;
  q_ULong_1 = s;
  q_ULong_1 = us;
  q_ULong_1 = i;
  q_ULong_1 = ui;
  q_ULong_1 = l;
  q_ULong_1 = ul;

  q_Byte_1 = q_Byte_1;
  q_Byte_1 = q_Word_1;
  q_Byte_1 = q_DWord_1;
  q_Byte_1 = q_QWord_1;
  q_Byte_1 = q_Int8_1;
  q_Byte_1 = q_UInt8_1;
  q_Byte_1 = q_Int16_1;
  q_Byte_1 = q_UInt16_1;
  q_Byte_1 = q_Int32_1;
  q_Byte_1 = q_UInt32_1;
  q_Byte_1 = q_Int64_1;
  q_Byte_1 = q_UInt64_1;
  q_Byte_1 = q_Char_1;
  q_Byte_1 = q_UChar_1;
  q_Byte_1 = q_Short_1;
  q_Byte_1 = q_UShort_1;
  q_Byte_1 = q_Int_1;
  q_Byte_1 = q_UInt_1;
  q_Byte_1 = q_Long_1;
  q_Byte_1 = q_ULong_1;

  q_Word_1 = q_Byte_1;
  q_Word_1 = q_Word_1;
  q_Word_1 = q_DWord_1;
  q_Word_1 = q_QWord_1;
  q_Word_1 = q_Int8_1;
  q_Word_1 = q_UInt8_1;
  q_Word_1 = q_Int16_1;
  q_Word_1 = q_UInt16_1;
  q_Word_1 = q_Int32_1;
  q_Word_1 = q_UInt32_1;
  q_Word_1 = q_Int64_1;
  q_Word_1 = q_UInt64_1;
  q_Word_1 = q_Char_1;
  q_Word_1 = q_UChar_1;
  q_Word_1 = q_Short_1;
  q_Word_1 = q_UShort_1;
  q_Word_1 = q_Int_1;
  q_Word_1 = q_UInt_1;
  q_Word_1 = q_Long_1;
  q_Word_1 = q_ULong_1;

  q_DWord_1 = q_Byte_1;
  q_DWord_1 = q_Word_1;
  q_DWord_1 = q_DWord_1;
  q_DWord_1 = q_QWord_1;
  q_DWord_1 = q_Int8_1;
  q_DWord_1 = q_UInt8_1;
  q_DWord_1 = q_Int16_1;
  q_DWord_1 = q_UInt16_1;
  q_DWord_1 = q_Int32_1;
  q_DWord_1 = q_UInt32_1;
  q_DWord_1 = q_Int64_1;
  q_DWord_1 = q_UInt64_1;
  q_DWord_1 = q_Char_1;
  q_DWord_1 = q_UChar_1;
  q_DWord_1 = q_Short_1;
  q_DWord_1 = q_UShort_1;
  q_DWord_1 = q_Int_1;
  q_DWord_1 = q_UInt_1;
  q_DWord_1 = q_Long_1;
  q_DWord_1 = q_ULong_1;

  q_QWord_1 = q_Byte_1;
  q_QWord_1 = q_Word_1;
  q_QWord_1 = q_DWord_1;
  q_QWord_1 = q_QWord_1;
  q_QWord_1 = q_Int8_1;
  q_QWord_1 = q_UInt8_1;
  q_QWord_1 = q_Int16_1;
  q_QWord_1 = q_UInt16_1;
  q_QWord_1 = q_Int32_1;
  q_QWord_1 = q_UInt32_1;
  q_QWord_1 = q_Int64_1;
  q_QWord_1 = q_UInt64_1;
  q_QWord_1 = q_Char_1;
  q_QWord_1 = q_UChar_1;
  q_QWord_1 = q_Short_1;
  q_QWord_1 = q_UShort_1;
  q_QWord_1 = q_Int_1;
  q_QWord_1 = q_UInt_1;
  q_QWord_1 = q_Long_1;
  q_QWord_1 = q_ULong_1;

  q_Int8_1 = q_Byte_1;
  q_Int8_1 = q_Word_1;
  q_Int8_1 = q_DWord_1;
  q_Int8_1 = q_QWord_1;
  q_Int8_1 = q_Int8_1;
  q_Int8_1 = q_UInt8_1;
  q_Int8_1 = q_Int16_1;
  q_Int8_1 = q_UInt16_1;
  q_Int8_1 = q_Int32_1;
  q_Int8_1 = q_UInt32_1;
  q_Int8_1 = q_Int64_1;
  q_Int8_1 = q_UInt64_1;
  q_Int8_1 = q_Char_1;
  q_Int8_1 = q_UChar_1;
  q_Int8_1 = q_Short_1;
  q_Int8_1 = q_UShort_1;
  q_Int8_1 = q_Int_1;
  q_Int8_1 = q_UInt_1;
  q_Int8_1 = q_Long_1;
  q_Int8_1 = q_ULong_1;

  q_UInt8_1 = q_Byte_1;
  q_UInt8_1 = q_Word_1;
  q_UInt8_1 = q_DWord_1;
  q_UInt8_1 = q_QWord_1;
  q_UInt8_1 = q_Int8_1;
  q_UInt8_1 = q_UInt8_1;
  q_UInt8_1 = q_Int16_1;
  q_UInt8_1 = q_UInt16_1;
  q_UInt8_1 = q_Int32_1;
  q_UInt8_1 = q_UInt32_1;
  q_UInt8_1 = q_Int64_1;
  q_UInt8_1 = q_UInt64_1;
  q_UInt8_1 = q_Char_1;
  q_UInt8_1 = q_UChar_1;
  q_UInt8_1 = q_Short_1;
  q_UInt8_1 = q_UShort_1;
  q_UInt8_1 = q_Int_1;
  q_UInt8_1 = q_UInt_1;
  q_UInt8_1 = q_Long_1;
  q_UInt8_1 = q_ULong_1;

  q_Int16_1 = q_Byte_1;
  q_Int16_1 = q_Word_1;
  q_Int16_1 = q_DWord_1;
  q_Int16_1 = q_QWord_1;
  q_Int16_1 = q_Int8_1;
  q_Int16_1 = q_UInt8_1;
  q_Int16_1 = q_Int16_1;
  q_Int16_1 = q_UInt16_1;
  q_Int16_1 = q_Int32_1;
  q_Int16_1 = q_UInt32_1;
  q_Int16_1 = q_Int64_1;
  q_Int16_1 = q_UInt64_1;
  q_Int16_1 = q_Char_1;
  q_Int16_1 = q_UChar_1;
  q_Int16_1 = q_Short_1;
  q_Int16_1 = q_UShort_1;
  q_Int16_1 = q_Int_1;
  q_Int16_1 = q_UInt_1;
  q_Int16_1 = q_Long_1;
  q_Int16_1 = q_ULong_1;

  q_UInt16_1 = q_Byte_1;
  q_UInt16_1 = q_Word_1;
  q_UInt16_1 = q_DWord_1;
  q_UInt16_1 = q_QWord_1;
  q_UInt16_1 = q_Int8_1;
  q_UInt16_1 = q_UInt8_1;
  q_UInt16_1 = q_Int16_1;
  q_UInt16_1 = q_UInt16_1;
  q_UInt16_1 = q_Int32_1;
  q_UInt16_1 = q_UInt32_1;
  q_UInt16_1 = q_Int64_1;
  q_UInt16_1 = q_UInt64_1;
  q_UInt16_1 = q_Char_1;
  q_UInt16_1 = q_UChar_1;
  q_UInt16_1 = q_Short_1;
  q_UInt16_1 = q_UShort_1;
  q_UInt16_1 = q_Int_1;
  q_UInt16_1 = q_UInt_1;
  q_UInt16_1 = q_Long_1;
  q_UInt16_1 = q_ULong_1;

  q_Int32_1 = q_Byte_1;
  q_Int32_1 = q_Word_1;
  q_Int32_1 = q_DWord_1;
  q_Int32_1 = q_QWord_1;
  q_Int32_1 = q_Int8_1;
  q_Int32_1 = q_UInt8_1;
  q_Int32_1 = q_Int16_1;
  q_Int32_1 = q_UInt16_1;
  q_Int32_1 = q_Int32_1;
  q_Int32_1 = q_UInt32_1;
  q_Int32_1 = q_Int64_1;
  q_Int32_1 = q_UInt64_1;
  q_Int32_1 = q_Char_1;
  q_Int32_1 = q_UChar_1;
  q_Int32_1 = q_Short_1;
  q_Int32_1 = q_UShort_1;
  q_Int32_1 = q_Int_1;
  q_Int32_1 = q_UInt_1;
  q_Int32_1 = q_Long_1;
  q_Int32_1 = q_ULong_1;

  q_UInt32_1 = q_Byte_1;
  q_UInt32_1 = q_Word_1;
  q_UInt32_1 = q_DWord_1;
  q_UInt32_1 = q_QWord_1;
  q_UInt32_1 = q_Int8_1;
  q_UInt32_1 = q_UInt8_1;
  q_UInt32_1 = q_Int16_1;
  q_UInt32_1 = q_UInt16_1;
  q_UInt32_1 = q_Int32_1;
  q_UInt32_1 = q_UInt32_1;
  q_UInt32_1 = q_Int64_1;
  q_UInt32_1 = q_UInt64_1;
  q_UInt32_1 = q_Char_1;
  q_UInt32_1 = q_UChar_1;
  q_UInt32_1 = q_Short_1;
  q_UInt32_1 = q_UShort_1;
  q_UInt32_1 = q_Int_1;
  q_UInt32_1 = q_UInt_1;
  q_UInt32_1 = q_Long_1;
  q_UInt32_1 = q_ULong_1;

  q_Int64_1 = q_Byte_1;
  q_Int64_1 = q_Word_1;
  q_Int64_1 = q_DWord_1;
  q_Int64_1 = q_QWord_1;
  q_Int64_1 = q_Int8_1;
  q_Int64_1 = q_UInt8_1;
  q_Int64_1 = q_Int16_1;
  q_Int64_1 = q_UInt16_1;
  q_Int64_1 = q_Int32_1;
  q_Int64_1 = q_UInt32_1;
  q_Int64_1 = q_Int64_1;
  q_Int64_1 = q_UInt64_1;
  q_Int64_1 = q_Char_1;
  q_Int64_1 = q_UChar_1;
  q_Int64_1 = q_Short_1;
  q_Int64_1 = q_UShort_1;
  q_Int64_1 = q_Int_1;
  q_Int64_1 = q_UInt_1;
  q_Int64_1 = q_Long_1;
  q_Int64_1 = q_ULong_1;

  q_UInt64_1 = q_Byte_1;
  q_UInt64_1 = q_Word_1;
  q_UInt64_1 = q_DWord_1;
  q_UInt64_1 = q_QWord_1;
  q_UInt64_1 = q_Int8_1;
  q_UInt64_1 = q_UInt8_1;
  q_UInt64_1 = q_Int16_1;
  q_UInt64_1 = q_UInt16_1;
  q_UInt64_1 = q_Int32_1;
  q_UInt64_1 = q_UInt32_1;
  q_UInt64_1 = q_Int64_1;
  q_UInt64_1 = q_UInt64_1;
  q_UInt64_1 = q_Char_1;
  q_UInt64_1 = q_UChar_1;
  q_UInt64_1 = q_Short_1;
  q_UInt64_1 = q_UShort_1;
  q_UInt64_1 = q_Int_1;
  q_UInt64_1 = q_UInt_1;
  q_UInt64_1 = q_Long_1;
  q_UInt64_1 = q_ULong_1;

  q_Char_1 = q_Byte_1;
  q_Char_1 = q_Word_1;
  q_Char_1 = q_DWord_1;
  q_Char_1 = q_QWord_1;
  q_Char_1 = q_Int8_1;
  q_Char_1 = q_UInt8_1;
  q_Char_1 = q_Int16_1;
  q_Char_1 = q_UInt16_1;
  q_Char_1 = q_Int32_1;
  q_Char_1 = q_UInt32_1;
  q_Char_1 = q_Int64_1;
  q_Char_1 = q_UInt64_1;
  q_Char_1 = q_Char_1;
  q_Char_1 = q_UChar_1;
  q_Char_1 = q_Short_1;
  q_Char_1 = q_UShort_1;
  q_Char_1 = q_Int_1;
  q_Char_1 = q_UInt_1;
  q_Char_1 = q_Long_1;
  q_Char_1 = q_ULong_1;

  q_UChar_1 = q_Byte_1;
  q_UChar_1 = q_Word_1;
  q_UChar_1 = q_DWord_1;
  q_UChar_1 = q_QWord_1;
  q_UChar_1 = q_Int8_1;
  q_UChar_1 = q_UInt8_1;
  q_UChar_1 = q_Int16_1;
  q_UChar_1 = q_UInt16_1;
  q_UChar_1 = q_Int32_1;
  q_UChar_1 = q_UInt32_1;
  q_UChar_1 = q_Int64_1;
  q_UChar_1 = q_UInt64_1;
  q_UChar_1 = q_Char_1;
  q_UChar_1 = q_UChar_1;
  q_UChar_1 = q_Short_1;
  q_UChar_1 = q_UShort_1;
  q_UChar_1 = q_Int_1;
  q_UChar_1 = q_UInt_1;
  q_UChar_1 = q_Long_1;
  q_UChar_1 = q_ULong_1;

  q_Short_1 = q_Byte_1;
  q_Short_1 = q_Word_1;
  q_Short_1 = q_DWord_1;
  q_Short_1 = q_QWord_1;
  q_Short_1 = q_Int8_1;
  q_Short_1 = q_UInt8_1;
  q_Short_1 = q_Int16_1;
  q_Short_1 = q_UInt16_1;
  q_Short_1 = q_Int32_1;
  q_Short_1 = q_UInt32_1;
  q_Short_1 = q_Int64_1;
  q_Short_1 = q_UInt64_1;
  q_Short_1 = q_Char_1;
  q_Short_1 = q_UChar_1;
  q_Short_1 = q_Short_1;
  q_Short_1 = q_UShort_1;
  q_Short_1 = q_Int_1;
  q_Short_1 = q_UInt_1;
  q_Short_1 = q_Long_1;
  q_Short_1 = q_ULong_1;

  q_UShort_1 = q_Byte_1;
  q_UShort_1 = q_Word_1;
  q_UShort_1 = q_DWord_1;
  q_UShort_1 = q_QWord_1;
  q_UShort_1 = q_Int8_1;
  q_UShort_1 = q_UInt8_1;
  q_UShort_1 = q_Int16_1;
  q_UShort_1 = q_UInt16_1;
  q_UShort_1 = q_Int32_1;
  q_UShort_1 = q_UInt32_1;
  q_UShort_1 = q_Int64_1;
  q_UShort_1 = q_UInt64_1;
  q_UShort_1 = q_Char_1;
  q_UShort_1 = q_UChar_1;
  q_UShort_1 = q_Short_1;
  q_UShort_1 = q_UShort_1;
  q_UShort_1 = q_Int_1;
  q_UShort_1 = q_UInt_1;
  q_UShort_1 = q_Long_1;
  q_UShort_1 = q_ULong_1;

  q_Int_1 = q_Byte_1;
  q_Int_1 = q_Word_1;
  q_Int_1 = q_DWord_1;
  q_Int_1 = q_QWord_1;
  q_Int_1 = q_Int8_1;
  q_Int_1 = q_UInt8_1;
  q_Int_1 = q_Int16_1;
  q_Int_1 = q_UInt16_1;
  q_Int_1 = q_Int32_1;
  q_Int_1 = q_UInt32_1;
  q_Int_1 = q_Int64_1;
  q_Int_1 = q_UInt64_1;
  q_Int_1 = q_Char_1;
  q_Int_1 = q_UChar_1;
  q_Int_1 = q_Short_1;
  q_Int_1 = q_UShort_1;
  q_Int_1 = q_Int_1;
  q_Int_1 = q_UInt_1;
  q_Int_1 = q_Long_1;
  q_Int_1 = q_ULong_1;

  q_UInt_1 = q_Byte_1;
  q_UInt_1 = q_Word_1;
  q_UInt_1 = q_DWord_1;
  q_UInt_1 = q_QWord_1;
  q_UInt_1 = q_Int8_1;
  q_UInt_1 = q_UInt8_1;
  q_UInt_1 = q_Int16_1;
  q_UInt_1 = q_UInt16_1;
  q_UInt_1 = q_Int32_1;
  q_UInt_1 = q_UInt32_1;
  q_UInt_1 = q_Int64_1;
  q_UInt_1 = q_UInt64_1;
  q_UInt_1 = q_Char_1;
  q_UInt_1 = q_UChar_1;
  q_UInt_1 = q_Short_1;
  q_UInt_1 = q_UShort_1;
  q_UInt_1 = q_Int_1;
  q_UInt_1 = q_UInt_1;
  q_UInt_1 = q_Long_1;
  q_UInt_1 = q_ULong_1;

  q_Long_1 = q_Byte_1;
  q_Long_1 = q_Word_1;
  q_Long_1 = q_DWord_1;
  q_Long_1 = q_QWord_1;
  q_Long_1 = q_Int8_1;
  q_Long_1 = q_UInt8_1;
  q_Long_1 = q_Int16_1;
  q_Long_1 = q_UInt16_1;
  q_Long_1 = q_Int32_1;
  q_Long_1 = q_UInt32_1;
  q_Long_1 = q_Int64_1;
  q_Long_1 = q_UInt64_1;
  q_Long_1 = q_Char_1;
  q_Long_1 = q_UChar_1;
  q_Long_1 = q_Short_1;
  q_Long_1 = q_UShort_1;
  q_Long_1 = q_Int_1;
  q_Long_1 = q_UInt_1;
  q_Long_1 = q_Long_1;
  q_Long_1 = q_ULong_1;

  q_ULong_1 = q_Byte_1;
  q_ULong_1 = q_Word_1;
  q_ULong_1 = q_DWord_1;
  q_ULong_1 = q_QWord_1;
  q_ULong_1 = q_Int8_1;
  q_ULong_1 = q_UInt8_1;
  q_ULong_1 = q_Int16_1;
  q_ULong_1 = q_UInt16_1;
  q_ULong_1 = q_Int32_1;
  q_ULong_1 = q_UInt32_1;
  q_ULong_1 = q_Int64_1;
  q_ULong_1 = q_UInt64_1;
  q_ULong_1 = q_Char_1;
  q_ULong_1 = q_UChar_1;
  q_ULong_1 = q_Short_1;
  q_ULong_1 = q_UShort_1;
  q_ULong_1 = q_Int_1;
  q_ULong_1 = q_UInt_1;
  q_ULong_1 = q_Long_1;
  q_ULong_1 = q_ULong_1;

  q_Byte_1 = 1;
  q_Byte_1 = -1;
  q_Byte_1 = 0x1f;
  q_Byte_1 = 'a';

  q_Word_1 = 1;
  q_Word_1 = -1;
  q_Word_1 = 0x1f;
  q_Word_1 = 'a';

  q_DWord_1 = 1;
  q_DWord_1 = -1;
  q_DWord_1 = 0x1f;
  q_DWord_1 = 'a';

  q_QWord_1 = 1;
  q_QWord_1 = -1;
  q_QWord_1 = 0x1f;
  q_QWord_1 = 'a';

  q_Int8_1 = 1;
  q_Int8_1 = -1;
  q_Int8_1 = 0x1f;
  q_Int8_1 = 'a';

  q_UInt8_1 = 1;
  q_UInt8_1 = -1;
  q_UInt8_1 = 0x1f;
  q_UInt8_1 = 'a';

  q_Int16_1 = 1;
  q_Int16_1 = -1;
  q_Int16_1 = 0x1f;
  q_Int16_1 = 'a';

  q_UInt16_1 = 1;
  q_UInt16_1 = -1;
  q_UInt16_1 = 0x1f;
  q_UInt16_1 = 'a';

  q_Int32_1 = 1;
  q_Int32_1 = -1;
  q_Int32_1 = 0x1f;
  q_Int32_1 = 'a';

  q_UInt32_1 = 1;
  q_UInt32_1 = -1;
  q_UInt32_1 = 0x1f;
  q_UInt32_1 = 'a';

  q_Int64_1 = 1;
  q_Int64_1 = -1;
  q_Int64_1 = 0x1f;
  q_Int64_1 = 'a';

  q_UInt64_1 = 1;
  q_UInt64_1 = -1;
  q_UInt64_1 = 0x1f;
  q_UInt64_1 = 'a';

  q_Char_1 = 1;
  q_Char_1 = -1;
  q_Char_1 = 0x1f;
  q_Char_1 = 'a';

  q_UChar_1 = 1;
  q_UChar_1 = -1;
  q_UChar_1 = 0x1f;
  q_UChar_1 = 'a';

  q_Short_1 = 1;
  q_Short_1 = -1;
  q_Short_1 = 0x1f;
  q_Short_1 = 'a';

  q_UShort_1 = 1;
  q_UShort_1 = -1;
  q_UShort_1 = 0x1f;
  q_UShort_1 = 'a';

  q_Int_1 = 1;
  q_Int_1 = -1;
  q_Int_1 = 0x1f;
  q_Int_1 = 'a';

  q_UInt_1 = 1;
  q_UInt_1 = -1;
  q_UInt_1 = 0x1f;
  q_UInt_1 = 'a';

  q_Long_1 = 1;
  q_Long_1 = -1;
  q_Long_1 = 0x1f;
  q_Long_1 = 'a';

  q_ULong_1 = 1;
  q_ULong_1 = -1;
  q_ULong_1 = 0x1f;
  q_ULong_1 = 'a';

  return;
}
/*** END CONSTRUCTOR/ASSIGNMENT ******************************************/
