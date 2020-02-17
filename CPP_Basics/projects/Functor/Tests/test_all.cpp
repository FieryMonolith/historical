#include <iostream>

#include "Type/Interface/TypelistMacros.h"
#include "Type/Interface/Sequence.h"
#include "Functor/Interface/Functor.h"
#include "Functor/Interface/GenericFunctor.h"
#include "Functor/Interface/BindFirst.h"
#include "Functor/Interface/Chain.h"
#include "Functor/Interface/OneToMany.h"
#include "Functor/Interface/ReturnlessFunctor.h"

using namespace std;
using namespace nFunctor;

void Print(void);
void Print(char x1);
void Print(char x1, char x2);
void Print(char x1, char x2, char x3);
void Print(char x1, char x2, char x3, char x4);
void Print(char x1, char x2, char x3, char x4, char x5);
void Print(char x1, char x2, char x3, char x4, char x5, char x6);
void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7);
void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8);
void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9);
void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9, char x10);
void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9, char x10, char x11);
void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9, char x10, char x11, char x12);
void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9, char x10, char x11, char x12, char x13);
void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9, char x10, char x11, char x12, char x13, char x14);
void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9, char x10, char x11, char x12, char x13, char x14,
           char x15);

void Print2(char x1, char x2, char x3, char x4) {
  cout << x1 << " " << x2 << " " << x3 << " " << x4 << endl;
  return;
}

void Print22(char x1, char x2, char x3, char x4) {
  cout << x1 << " " << x2 << " " << x3 << " " << x4 << endl;
  return;
}

int Print3(char x1, char x2, char x3, char x4) {
  cout << x1 << " " << x2 << " " << x3 << " " << x4 << endl;
  return 3;
}

int Print4(char x1, char x2, char x3, char x4) {
  cout << x1 << " " << x2 << " " << x3 << " " << x4 << endl;
  return 4;
}

int Print5(char x1, char x2, char x3, char x4) {
  cout << x1 << " " << x2 << " " << x3 << " " << x4 << endl;
  return 5;
}

struct A {
  A(void) { return; }
  void Print(void);
  void Print(char x1) const;
  void Print(char x1, char x2);
  void Print(char x1, char x2, char x3);
  void Print(char x1, char x2, char x3, char x4);
  void Print(char x1, char x2, char x3, char x4, char x5);
  void Print(char x1, char x2, char x3, char x4, char x5, char x6);
  void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7);
  void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
             char x8);
  void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
             char x8, char x9);
  void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
             char x8, char x9, char x10);
  void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
             char x8, char x9, char x10, char x11);
  void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
             char x8, char x9, char x10, char x11, char x12);
  void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
             char x8, char x9, char x10, char x11, char x12, char x13);
  void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
             char x8, char x9, char x10, char x11, char x12, char x13, char x14);
  void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
             char x8, char x9, char x10, char x11, char x12, char x13, char x14,
             char x15);

  void Print2(char x1, char x2, char x3, char x4) {
    cout << x1 << " " << x2 << " " << x3 << " " << x4 << endl;
    return;
  }
};

class cCallable {
public:
  cCallable(void) { return; }

  void operator()(void);
  void operator()(char x1) const;
  void operator()(char x1, char x2);
  void operator()(char x1, char x2, char x3);
  void operator()(char x1, char x2, char x3, char x4);
  void operator()(char x1, char x2, char x3, char x4, char x5);
  void operator()(char x1, char x2, char x3, char x4, char x5, char x6);
  void operator()(char x1, char x2, char x3, char x4, char x5, char x6,
                  char x7);
  void operator()(char x1, char x2, char x3, char x4, char x5, char x6,
                  char x7, char x8);
  void operator()(char x1, char x2, char x3, char x4, char x5, char x6,
                  char x7, char x8, char x9);
  void operator()(char x1, char x2, char x3, char x4, char x5, char x6,
                  char x7, char x8, char x9, char x10);
  void operator()(char x1, char x2, char x3, char x4, char x5, char x6,
                  char x7, char x8, char x9, char x10, char x11);
  void operator()(char x1, char x2, char x3, char x4, char x5, char x6,
                  char x7, char x8, char x9, char x10, char x11, char x12);
  void operator()(char x1, char x2, char x3, char x4, char x5, char x6,
                  char x7, char x8, char x9, char x10, char x11, char x12,
                  char x13);
  void operator()(char x1, char x2, char x3, char x4, char x5, char x6,
                  char x7, char x8, char x9, char x10, char x11, char x12,
                  char x13, char x14);
  void operator()(char x1, char x2, char x3, char x4, char x5, char x6,
                  char x7, char x8, char x9, char x10, char x11, char x12,
                  char x13, char x14, char x15);
};

const bool operator==(const cCallable &, const cCallable &) {
  return true;
}

const bool operator!=(const cCallable &, const cCallable &) {
  return false;
}

typedef TYPELIST_1(char) TP1;
typedef TYPELIST_2(char, char) TP2;
typedef TYPELIST_3(char, char, char) TP3;
typedef TYPELIST_4(char, char, char, char) TP4;
typedef TYPELIST_5(char, char, char, char, char) TP5;
typedef TYPELIST_6(char, char, char, char, char, char) TP6;
typedef TYPELIST_7(char, char, char, char, char, char, char) TP7;
typedef TYPELIST_8(char, char, char, char, char, char, char, char) TP8;
typedef TYPELIST_9(char, char, char, char, char, char, char, char, char) TP9;
typedef TYPELIST_10(char, char, char, char, char, char, char, char, char,
                    char) TP10;
typedef TYPELIST_11(char, char, char, char, char, char, char, char, char,
                    char, char) TP11;
typedef TYPELIST_12(char, char, char, char, char, char, char, char, char,
                    char, char, char) TP12;
typedef TYPELIST_13(char, char, char, char, char, char, char, char, char,
                    char, char, char, char) TP13;
typedef TYPELIST_14(char, char, char, char, char, char, char, char, char,
                    char, char, char, char, char) TP14;
typedef TYPELIST_15(char, char, char, char, char, char, char, char, char,
                    char, char, char, char, char, char) TP15;

typedef nType::Seq<char> SP1;
typedef nType::Seq<char, char> SP2;
typedef nType::Seq<char, char, char> SP3;
typedef nType::Seq<char, char, char, char> SP4;
typedef nType::Seq<char, char, char, char, char> SP5;
typedef nType::Seq<char, char, char, char, char, char> SP6;
typedef nType::Seq<char, char, char, char, char, char, char> SP7;
typedef nType::Seq<char, char, char, char, char, char, char, char> SP8;
typedef nType::Seq<char, char, char, char, char, char, char, char,
                       char> SP9;
typedef nType::Seq<char, char, char, char, char, char, char, char, char,
                       char> SP10;
typedef nType::Seq<char, char, char, char, char, char, char, char, char,
                       char, char> SP11;
typedef nType::Seq<char, char, char, char, char, char, char, char, char,
                       char, char, char> SP12;
typedef nType::Seq<char, char, char, char, char, char, char, char, char,
                       char, char, char, char> SP13;
typedef nType::Seq<char, char, char, char, char, char, char, char, char,
                       char, char, char, char, char> SP14;
typedef nType::Seq<char, char, char, char, char, char, char, char, char,
                       char, char, char, char, char, char> SP15;

typedef void (&SRF1)(char);

typedef void (*SF0)(void);
typedef void (*SF1)(char);
typedef void (*SF2)(char, char);
typedef void (*SF3)(char, char, char);
typedef void (*SF4)(char, char, char, char);
typedef void (*SF5)(char, char, char, char, char);
typedef void (*SF6)(char, char, char, char, char, char);
typedef void (*SF7)(char, char, char, char, char, char, char);
typedef void (*SF8)(char, char, char, char, char, char, char, char);
typedef void (*SF9)(char, char, char, char, char, char, char, char, char);
typedef void (*SF10)(char, char, char, char, char, char, char, char, char,
                     char);
typedef void (*SF11)(char, char, char, char, char, char, char, char, char,
                     char, char);
typedef void (*SF12)(char, char, char, char, char, char, char, char, char,
                     char, char, char);
typedef void (*SF13)(char, char, char, char, char, char, char, char, char,
                     char, char, char, char);
typedef void (*SF14)(char, char, char, char, char, char, char, char, char,
                     char, char, char, char, char);
typedef void (*SF15)(char, char, char, char, char, char, char, char, char,
                     char, char, char, char, char, char);

typedef void (A::*MF0)(void);
typedef void (A::*MF1)(char) const;
typedef void (A::*MF2)(char, char);
typedef void (A::*MF3)(char, char, char);
typedef void (A::*MF4)(char, char, char, char);
typedef void (A::*MF5)(char, char, char, char, char);
typedef void (A::*MF6)(char, char, char, char, char, char);
typedef void (A::*MF7)(char, char, char, char, char, char, char);
typedef void (A::*MF8)(char, char, char, char, char, char, char, char);
typedef void (A::*MF9)(char, char, char, char, char, char, char, char, char);
typedef void (A::*MF10)(char, char, char, char, char, char, char, char, char,
                        char);
typedef void (A::*MF11)(char, char, char, char, char, char, char, char, char,
                        char, char);
typedef void (A::*MF12)(char, char, char, char, char, char, char, char, char,
                        char, char, char);
typedef void (A::*MF13)(char, char, char, char, char, char, char, char, char,
                        char, char, char, char);
typedef void (A::*MF14)(char, char, char, char, char, char, char, char, char,
                        char, char, char, char, char);
typedef void (A::*MF15)(char, char, char, char, char, char, char, char, char,
                        char, char, char, char, char, char);

int main(void) {
  A a;

  cout << "This test function will test the following:" << endl;
  cout << " -All 15 operator() are working properly." << endl;
  cout << " -Test each method from each class." << endl;
  cout << " -Test access and usage of iFunctor base class." << endl;
  cout << " -Test ability to take a pointer or value version of a callable";
  cout << " entity." << endl;
  cout << "Classes tested are as follows:" << endl;
  cout << " -Functor" << endl;
  cout << " -MemberFunctionWrapper" << endl;
  cout << " -GenericFunctor" << endl;
  cout << " -BinderFirst" << endl;
  cout << " -Chain" << endl;
  cout << " -OneToMany" << endl;
  cout << " -ReturnlessFunctor" << endl;

  cout << endl;

  cout << "Verify access to parameters directly from Functor class." << endl;
  Functor<SF1, void, TP1>::Parm1 jason1;
  jason1 = 0; // Get rid of warning for unused variable.
  cout << endl;

  cout << "Next - establish that all 15 forwarding functions are working" << endl;
  cout << "   for both static callables and member functions.  This will" << endl;
  cout << "   be tested with typelists for Functor and sequences all others"  << endl;
  cout << "   because iFunctor has separate definitions for typelist and" << endl;
  cout << "   sequences, thus they both need to be checked." << endl;
  cout << endl;

  cout << "Functor- Testing all 16 versions of static callables using typelists";
  cout << endl;

  Functor<SF0> sf0(&Print);
  Functor<SF1, void, TP1> sf1(&Print);
  Functor<SF2, void, TP2> sf2(&Print);
  Functor<SF3, void, TP3> sf3(&Print);
  Functor<SF4, void, TP4> sf4(&Print);
  Functor<SF5, void, TP5> sf5(&Print);
  Functor<SF6, void, TP6> sf6(&Print);
  Functor<SF7, void, TP7> sf7(&Print);
  Functor<SF8, void, TP8> sf8(&Print);
  Functor<SF9, void, TP9> sf9(&Print);
  Functor<SF10, void, TP10> sf10(&Print);
  Functor<SF11, void, TP11> sf11(&Print);
  Functor<SF12, void, TP12> sf12(&Print);
  Functor<SF13, void, TP13> sf13(&Print);
  Functor<SF14, void, TP14> sf14(&Print);
  Functor<SF15, void, TP15> sf15(&Print);
  sf0();
  sf1('a');
  sf2('a', 'b');
  sf3('a', 'b', 'c');
  sf4('a', 'b', 'c', 'd');
  sf5('a', 'b', 'c', 'd', 'e');
  sf6('a', 'b', 'c', 'd', 'e', 'f');
  sf7('a', 'b', 'c', 'd', 'e', 'f', 'g');
  sf8('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h');
  sf9('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i');
  sf10('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j');
  sf11('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k');
  sf12('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l');
  sf13('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm');
  sf14('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n');
  sf15('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o');

  cout << endl;

  // Test Static print for all 15 function types using typelists
  cout << "MemberFunctionWrapper- Testing all 16 versions of member functions" << endl;
  cout << "using sequences." << endl;

  MemberFunctionWrapper<A*, MF0> smf0(&a, &A::Print);
  MemberFunctionWrapper<A*, MF1, void, SP1> smf1(&a, &A::Print);
  MemberFunctionWrapper<A*, MF2, void, SP2> smf2(&a, &A::Print);
  MemberFunctionWrapper<A*, MF3, void, SP3> smf3(&a, &A::Print);
  MemberFunctionWrapper<A*, MF4, void, SP4> smf4(&a, &A::Print);
  MemberFunctionWrapper<A*, MF5, void, SP5> smf5(&a, &A::Print);
  MemberFunctionWrapper<A*, MF6, void, SP6> smf6(&a, &A::Print);
  MemberFunctionWrapper<A*, MF7, void, SP7> smf7(&a, &A::Print);
  MemberFunctionWrapper<A*, MF8, void, SP8> smf8(&a, &A::Print);
  MemberFunctionWrapper<A*, MF9, void, SP9> smf9(&a, &A::Print);
  MemberFunctionWrapper<A*, MF10, void, SP10> smf10(&a, &A::Print);
  MemberFunctionWrapper<A*, MF11, void, SP11> smf11(&a, &A::Print);
  MemberFunctionWrapper<A*, MF12, void, SP12> smf12(&a, &A::Print);
  MemberFunctionWrapper<A*, MF13, void, SP13> smf13(&a, &A::Print);
  MemberFunctionWrapper<A*, MF14, void, SP14> smf14(&a, &A::Print);
  MemberFunctionWrapper<A*, MF15, void, SP15> smf15(&a, &A::Print);
  smf0();
  smf1('a');
  smf2('a', 'b');
  smf3('a', 'b', 'c');
  smf4('a', 'b', 'c', 'd');
  smf5('a', 'b', 'c', 'd', 'e');
  smf6('a', 'b', 'c', 'd', 'e', 'f');
  smf7('a', 'b', 'c', 'd', 'e', 'f', 'g');
  smf8('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h');
  smf9('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i');
  smf10('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j');
  smf11('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k');
  smf12('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l');
  smf13('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm');
  smf14('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n');
  smf15('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o');

  cout << endl;

  cout << "GenericFunctor(Functor)- Testing all 16 versions of static";
  cout << "callables using sequences";
  cout << endl;

  GenericFunctor<> sgf0(static_cast<SF0>(&Print));
  GenericFunctor<void, TP1> ssgf1(static_cast<SF1>(&Print));
  GenericFunctor<void, SP2> ssgf2(static_cast<SF2>(&Print));
  GenericFunctor<void, SP3> ssgf3(static_cast<SF3>(&Print));
  GenericFunctor<void, SP4> ssgf4(static_cast<SF4>(&Print));
  GenericFunctor<void, SP5> ssgf5(static_cast<SF5>(&Print));
  GenericFunctor<void, SP6> ssgf6(static_cast<SF6>(&Print));
  GenericFunctor<void, SP7> ssgf7(static_cast<SF7>(&Print));
  GenericFunctor<void, SP8> ssgf8(static_cast<SF8>(&Print));
  GenericFunctor<void, SP9> ssgf9(static_cast<SF9>(&Print));
  GenericFunctor<void, SP10> ssgf10(static_cast<SF10>(&Print));
  GenericFunctor<void, SP11> ssgf11(static_cast<SF11>(&Print));
  GenericFunctor<void, SP12> ssgf12(static_cast<SF12>(&Print));
  GenericFunctor<void, SP13> ssgf13(static_cast<SF13>(&Print));
  GenericFunctor<void, SP14> ssgf14(static_cast<SF14>(&Print));
  GenericFunctor<void, SP15> ssgf15(static_cast<SF15>(&Print));
  sgf0();
  ssgf1('a');
  ssgf2('a', 'b');
  ssgf3('a', 'b', 'c');
  ssgf4('a', 'b', 'c', 'd');
  ssgf5('a', 'b', 'c', 'd', 'e');
  ssgf6('a', 'b', 'c', 'd', 'e', 'f');
  ssgf7('a', 'b', 'c', 'd', 'e', 'f', 'g');
  ssgf8('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h');
  ssgf9('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i');
  ssgf10('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j');
  ssgf11('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k');
  ssgf12('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l');
  ssgf13('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm');
  ssgf14('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n');
  ssgf15('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o');

  cout << endl;

  // Test Static print for all 15 function types using typelists
  cout << "GenericFunctor(MemberFunctionWrapper)- Testing all 16 versions" << endl;
  cout << "of member functions using sequences";
  cout << endl;

  GenericFunctor<> mgf0(&a, static_cast<MF0>(&A::Print));
  GenericFunctor<void, SP1> smgf1(&a, static_cast<MF1>(&A::Print));
  GenericFunctor<void, SP2> smgf2(&a, static_cast<MF2>(&A::Print));
  GenericFunctor<void, SP3> smgf3(&a, static_cast<MF3>(&A::Print));
  GenericFunctor<void, SP4> smgf4(&a, static_cast<MF4>(&A::Print));
  GenericFunctor<void, SP5> smgf5(&a, static_cast<MF5>(&A::Print));
  GenericFunctor<void, SP6> smgf6(&a, static_cast<MF6>(&A::Print));
  GenericFunctor<void, SP7> smgf7(&a, static_cast<MF7>(&A::Print));
  GenericFunctor<void, SP8> smgf8(&a, static_cast<MF8>(&A::Print));
  GenericFunctor<void, SP9> smgf9(&a, static_cast<MF9>(&A::Print));
  GenericFunctor<void, SP10> smgf10(&a, static_cast<MF10>(&A::Print));
  GenericFunctor<void, SP11> smgf11(&a, static_cast<MF11>(&A::Print));
  GenericFunctor<void, SP12> smgf12(&a, static_cast<MF12>(&A::Print));
  GenericFunctor<void, SP13> smgf13(&a, static_cast<MF13>(&A::Print));
  GenericFunctor<void, SP14> smgf14(&a, static_cast<MF14>(&A::Print));
  GenericFunctor<void, SP15> smgf15(&a, static_cast<MF15>(&A::Print));
  mgf0();
  smgf1('a');
  smgf2('a', 'b');
  smgf3('a', 'b', 'c');
  smgf4('a', 'b', 'c', 'd');
  smgf5('a', 'b', 'c', 'd', 'e');
  smgf6('a', 'b', 'c', 'd', 'e', 'f');
  smgf7('a', 'b', 'c', 'd', 'e', 'f', 'g');
  smgf8('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h');
  smgf9('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i');
  smgf10('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j');
  smgf11('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k');
  smgf12('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l');
  smgf13('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm');
  smgf14('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n');
  smgf15('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o');

  cout << endl;

  cout << "Chain- Testing all 16 versions of member functions using sequences" << endl;

  Chain<SF0, SF0> sch0(&Print, &Print);
  Chain<SF1, SF1, void, SP1> sch1(&Print, Print);
  Chain<SF2, SF2, void, SP2> sch2(&Print, Print);
  Chain<SF3, SF3, void, SP3> sch3(&Print, Print);
  Chain<SF4, SF4, void, SP4> sch4(&Print, Print);
  Chain<SF5, SF5, void, SP5> sch5(&Print, Print);
  Chain<SF6, SF6, void, SP6> sch6(&Print, Print);
  Chain<SF7, SF7, void, SP7> sch7(&Print, Print);
  Chain<SF8, SF8, void, SP8> sch8(&Print, Print);
  Chain<SF9, SF9, void, SP9> sch9(&Print, Print);
  Chain<SF10, SF10, void, SP10> sch10(&Print, Print);
  Chain<SF11, SF11, void, SP11> sch11(&Print, Print);
  Chain<SF12, SF12, void, SP12> sch12(&Print, Print);
  Chain<SF13, SF13, void, SP13> sch13(&Print, Print);
  Chain<SF14, SF14, void, SP14> sch14(&Print, Print);
  Chain<SF15, SF15, void, SP15> sch15(&Print, Print);
  sch0();
  sch1('a');
  sch2('a', 'b');
  sch3('a', 'b', 'c');
  sch4('a', 'b', 'c', 'd');
  sch5('a', 'b', 'c', 'd', 'e');
  sch6('a', 'b', 'c', 'd', 'e', 'f');
  sch7('a', 'b', 'c', 'd', 'e', 'f', 'g');
  sch8('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h');
  sch9('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i');
  sch10('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j');
  sch11('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k');
  sch12('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l');
  sch13('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm');
  sch14('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n');
  sch15('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o');

  cout << endl;

  cout << "OneToMany- Testing all 16 versions of member functions using sequences" << endl;
  OneToMany<> to0(&sf0);
  OneToMany<void, TP1> so1(&sf1);
  OneToMany<void, TP2> so2(&sf2);
  OneToMany<void, TP3> so3(&sf3);
  OneToMany<void, TP4> so4(&sf4);
  OneToMany<void, TP5> so5(&sf5);
  OneToMany<void, TP6> so6(&sf6);
  OneToMany<void, TP7> so7(&sf7);
  OneToMany<void, TP8> so8(&sf8);
  OneToMany<void, TP9> so9(&sf9);
  OneToMany<void, TP10> so10(&sf10);
  OneToMany<void, TP11> so11(&sf11);
  OneToMany<void, TP12> so12(&sf12);
  OneToMany<void, TP13> so13(&sf13);
  OneToMany<void, TP14> so14(&sf14);
  OneToMany<void, TP15> so15(&sf15);
  to0();
  so1('a');
  so2('a', 'b');
  so3('a', 'b', 'c');
  so4('a', 'b', 'c', 'd');
  so5('a', 'b', 'c', 'd', 'e');
  so6('a', 'b', 'c', 'd', 'e', 'f');
  so7('a', 'b', 'c', 'd', 'e', 'f', 'g');
  so8('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h');
  so9('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i');
  so10('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j');
  so11('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k');
  so12('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l');
  so13('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm');
  so14('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n');
  so15('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o');

  cout << endl;

  cout << "BinderFirst- Testing all 15 versions of member functions using sequences";
  cout << endl;

  typedef Functor<SF1, void, TP1> FUN1;
  typedef Functor<SF2, void, TP2> FUN2;
  typedef Functor<SF3, void, TP3> FUN3;
  typedef Functor<SF4, void, TP4> FUN4;
  typedef Functor<SF5, void, TP5> FUN5;
  typedef Functor<SF6, void, TP6> FUN6;
  typedef Functor<SF7, void, TP7> FUN7;
  typedef Functor<SF8, void, TP8> FUN8;
  typedef Functor<SF9, void, TP9> FUN9;
  typedef Functor<SF10, void, TP10> FUN10;
  typedef Functor<SF11, void, TP11> FUN11;
  typedef Functor<SF12, void, TP12> FUN12;
  typedef Functor<SF13, void, TP13> FUN13;
  typedef Functor<SF14, void, TP14> FUN14;
  typedef Functor<SF15, void, TP15> FUN15;

  BinderFirst<FUN1, void, TP1> *bf1 = BindFirst<FUN1, void, TP1>(sf1, 'a');
  if(bf1) {
    (*bf1)();
    delete bf1;
  }
  BinderFirst<FUN2, void, TP2> *bf2 = BindFirst<FUN2, void, TP2>(sf2, 'a');
  if(bf2) {
    (*bf2)('b');
    delete bf2;
  }
  BinderFirst<FUN3, void, TP3> *bf3 = BindFirst<FUN3, void, TP3>(sf3, 'a');
  if(bf3) {
    (*bf3)('b', 'c');
    delete bf3;
  }
  BinderFirst<FUN4, void, TP4> *bf4 = BindFirst<FUN4, void, TP4>(sf4, 'a');
  if(bf4) {
    (*bf4)('b', 'c', 'd');
    delete bf4;
  }
  BinderFirst<FUN5, void, TP5> *bf5 = BindFirst<FUN5, void, TP5>(sf5, 'a');
  if(bf5) {
    (*bf5)('b', 'c', 'd', 'e');
    delete bf5;
  }
  BinderFirst<FUN6, void, TP6> *bf6 = BindFirst<FUN6, void, TP6>(sf6, 'a');
  if(bf6) {
    (*bf6)('b', 'c', 'd', 'e', 'f');
    delete bf6;
  }
  BinderFirst<FUN7, void, TP7> *bf7 = BindFirst<FUN7, void, TP7>(sf7, 'a');
  if(bf7) {
    (*bf7)('b', 'c', 'd', 'e', 'f', 'g');
    delete bf7;
  }
  BinderFirst<FUN8, void, TP8> *bf8 = BindFirst<FUN8, void, TP8>(sf8, 'a');
  if(bf8) {
    (*bf8)('b', 'c', 'd', 'e', 'f', 'g', 'h');
    delete bf8;
  }
  BinderFirst<FUN9, void, TP9> *bf9 = BindFirst<FUN9, void, TP9>(sf9, 'a');
  if(bf9) {
    (*bf9)('b', 'c', 'd', 'e', 'f', 'g', 'h', 'i');
    delete bf9;
  }
  BinderFirst<FUN10, void, TP10> *bf10 = BindFirst<FUN10, void, TP10>(sf10, 'a');
  if(bf10) {
    (*bf10)('b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j');
    delete bf10;
  }
  BinderFirst<FUN11, void, TP11> *bf11 = BindFirst<FUN11, void, TP11>(sf11, 'a');
  if(bf11) {
    (*bf11)('b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k');
    delete bf11;
  }
  BinderFirst<FUN12, void, TP12> *bf12 = BindFirst<FUN12, void, TP12>(sf12, 'a');
  if(bf12) {
    (*bf12)('b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l');
    delete bf12;
  }
  BinderFirst<FUN13, void, TP13> *bf13 = BindFirst<FUN13, void, TP13>(sf13, 'a');
  if(bf13) {
    (*bf13)('b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm');
    delete bf13;
  }
  BinderFirst<FUN14, void, TP14> *bf14 = BindFirst<FUN14, void, TP14>(sf14, 'a');
  if(bf14) {
    (*bf14)('b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n');
    delete bf14;
  }
  BinderFirst<FUN15, void, TP15> *bf15 = BindFirst<FUN15, void, TP15>(sf15, 'a');
  if(bf15) {
    (*bf15)('b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o');
    delete bf15;
  }

  cout << endl;
  cout << "ReturnlessFunctor- Testing all 16 versions of member functions using sequences";
  cout << endl;

  ReturnlessFunctor<SF0> rf0(&Print);
  ReturnlessFunctor<SF1, void, TP1> rf1(&Print);
  ReturnlessFunctor<SF2, void, TP2> rf2(&Print);
  ReturnlessFunctor<SF3, void, TP3> rf3(&Print);
  ReturnlessFunctor<SF4, void, TP4> rf4(&Print);
  ReturnlessFunctor<SF5, void, TP5> rf5(&Print);
  ReturnlessFunctor<SF6, void, TP6> rf6(&Print);
  ReturnlessFunctor<SF7, void, TP7> rf7(&Print);
  ReturnlessFunctor<SF8, void, TP8> rf8(&Print);
  ReturnlessFunctor<SF9, void, TP9> rf9(&Print);
  ReturnlessFunctor<SF10, void, TP10> rf10(&Print);
  ReturnlessFunctor<SF11, void, TP11> rf11(&Print);
  ReturnlessFunctor<SF12, void, TP12> rf12(&Print);
  ReturnlessFunctor<SF13, void, TP13> rf13(&Print);
  ReturnlessFunctor<SF14, void, TP14> rf14(&Print);
  ReturnlessFunctor<SF15, void, TP15> rf15(&Print);
  sf0();
  sf1('a');
  sf2('a', 'b');
  sf3('a', 'b', 'c');
  sf4('a', 'b', 'c', 'd');
  sf5('a', 'b', 'c', 'd', 'e');
  sf6('a', 'b', 'c', 'd', 'e', 'f');
  sf7('a', 'b', 'c', 'd', 'e', 'f', 'g');
  sf8('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h');
  sf9('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i');
  sf10('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j');
  sf11('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k');
  sf12('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l');
  sf13('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm');
  sf14('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n');
  sf15('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o');

  cout << endl << endl;

  /*** general callable, const, and storage tests *****************************/
  cout << "The next test is general callable structures, const, and storage.";
  cout << endl;

  cout << endl << endl;

  /*** Functor ***************************************************************/
  cout << "Functor-" << endl;
  Functor<SF1, void, SP1> f1(&Print);
  cout << "Static function pointer - \t";
  f1('c');

  cout << "Static function reference - \t";
  Functor<SRF1, void, SP1> f2(Print);
  f2('c');

  cCallable c;
  Functor<cCallable, void, SP1> fc(c);
  cout << "cCallable - \t\t\t";
  fc('c');

  const cCallable cc;
  Functor<const cCallable, void, SP1> fcc(cc);
  cout << "const cCallable - \t\t";
  fcc('c');

  cCallable &rc = c;
  Functor<cCallable &, void, SP1> frc(rc);
  cout << "cCallable & - \t\t\t";
  frc('c');

  const cCallable &rcc = c;
  Functor<const cCallable &, void, SP1> frcc(rcc);
  cout << "const cCallable & - \t\t";
  frcc('c');

  cCallable *pc = &c;
  Functor<cCallable *, void, SP1> fpc(pc);
  cout << "cCallable * - \t\t\t";
  fpc('c');

  const cCallable *ccp = &c;
  Functor<const cCallable *, void, SP1> fccp(ccp);
  cout << "const cCallable * - \t\t";
  fccp('c');

  cCallable * const cpc = &c;
  Functor<cCallable * const, void, SP1> fcpc(cpc);
  cout << "cCallable * const - \t\t";
  fcpc('c');

  const cCallable * const ccpc = &c;
  Functor<const cCallable * const, void, SP1> fccpc(ccpc);
  cout << "const cCallable * const - \t";
  fccpc('c');

  cout << endl << endl;

  /*** MemberFunctionWrapper ************************************************/
  cout << "MemberFunctionWrapper-" << endl;
  MemberFunctionWrapper<A, MF1, void, SP1> ma(a, &A::Print);
  cout << "A - \t\t\t";
  ma('c');

  const A ca;
  MemberFunctionWrapper<const A, MF1, void, SP1> mca(ca, &A::Print);
  cout << "const A - \t\t";
  mca('c');

  A &ra = a;
  MemberFunctionWrapper<A &, MF1, void, SP1> mra(ra, &A::Print);
  cout << "A & - \t\t\t";
  mra('c');

  const A &rca = a;
  MemberFunctionWrapper<const A &, MF1, void, SP1> mrca(rca, &A::Print);
  cout << "const A & - \t\t";
  mrca('c');

  A *pa = &a;
  MemberFunctionWrapper<A *, MF1, void, SP1> mpa(pa, &A::Print);
  cout << "      A *       - \t";
  mpa('c');

  const A *cap = &a;
  MemberFunctionWrapper<const A *, MF1, void, SP1> mcap(cap, &A::Print);
  cout << "const A *       - \t";
  mcap('c');

  A * const cpa = &a;
  MemberFunctionWrapper<A * const, MF1, void, SP1> mcpa(cpa, &A::Print);
  cout << "      A * const - \t";
  mcpa('c');

  const A * const capc = &a;
  MemberFunctionWrapper<const A * const, MF1, void, SP1> mcapc(capc, &A::Print);
  cout << "const A * const - \t";
  mcapc('c');

  cout << endl << endl;

  /*** GenericFunctor ********************************************************/
  /*** Functor ***************************************************************/
  cout << "GenericFunctor (Functor)-" << endl;
  GenericFunctor<void, SP1> gf1(static_cast<SF1>(&Print));
  cout << "Static function pointer - \t";
  gf1('c');

  cout << "Static function reference - \t";
  GenericFunctor<void, SP1> gf2a(static_cast<void (&)(char)>(Print));
  gf2a('c');

  GenericFunctor<void, SP1> gfc(c);
  cout << "cCallable - \t\t\t";
  gfc('c');

  GenericFunctor<void, SP1> gfcc(cc);
  cout << "const cCallable - \t\t";
  gfcc('c');

  GenericFunctor<void, SP1> gfrc(rc);
  cout << "cCallable & - \t\t\t";
  gfrc('c');

  GenericFunctor<void, SP1> gfrcc(rcc);
  cout << "const cCallable & - \t\t";
  gfrcc('c');

  GenericFunctor<void, SP1> gfpc(pc);
  cout << "cCallable * - \t\t\t";
  gfpc('c');

  GenericFunctor<void, SP1> gfccp(ccp);
  cout << "const cCallable * - \t\t";
  gfccp('c');

  GenericFunctor<void, SP1> gfcpc(cpc);
  cout << "cCallable * const - \t\t";
  gfcpc('c');

  GenericFunctor<void, SP1> gfccpc(ccpc);
  cout << "const cCallable * const - \t";
  gfccpc('c');

  cout << endl << endl;

  /*** GenericFunctor ********************************************************/
  /*** MemberFunctionWrapper ************************************************/
  cout << "GenericFunctor (MemberFunctionWrapper)-" << endl;
  GenericFunctor<void, SP1> gma(a, static_cast<MF1>(&A::Print));
  cout << "A - \t\t\t";
  gma('c');

  GenericFunctor<void, SP1> gmca(ca, static_cast<MF1>(&A::Print));
  cout << "const A - \t\t";
  gmca('c');

  GenericFunctor<void, SP1> gmra(ra, static_cast<MF1>(&A::Print));
  cout << "A & - \t\t\t";
  gmra('c');

  GenericFunctor<void, SP1> gmrca(rca, static_cast<MF1>(&A::Print));
  cout << "const A & - \t\t";
  gmrca('c');

  GenericFunctor<void, SP1> gmpa(pa, static_cast<MF1>(&A::Print));
  cout << "      A *       - \t";
  gmpa('c');

  GenericFunctor<void, SP1> gmcap(cap, static_cast<MF1>(&A::Print));
  cout << "const A *       - \t";
  gmcap('c');

  GenericFunctor<void, SP1> gmcpa(cpa, static_cast<MF1>(&A::Print));
  cout << "      A * const - \t";
  gmcpa('c');

  GenericFunctor<void, SP1> gmcapc(capc, static_cast<MF1>(&A::Print));
  cout << "const A * const - \t";
  gmcapc('c');

  cout << endl << endl;

  cout << "Time to test Chainer" << endl;

  cout << endl;
  cout << endl;

  cout << "Now lets check the ability to take a callable entity other" << endl;
  cout << "than a function, so it will use a callable class." << endl;

  //cCallable c;
  Functor<cCallable, void, TP4> cf(c);
  cf('a', 'b', 'c', 'd');
  GenericFunctor<void, TP4> cgf(c);
  cgf('a', 'b', 'c', 'd');
  BinderFirst<cCallable, void, TP4> *cbf =
    BindFirst<cCallable, void, TP4>(c, 'a');
  if(cbf) {
    (*cbf)('b', 'c', 'd');
    delete cbf;
  }

  cout << endl << endl;

  cout << "Now lets test the ability of these functors to handle" << endl;
  cout << "a pointer to a callable entity.  It is not possible to" << endl;
  cout << "handle references due to the constructor type requirements." << endl;
  cout << "Testing by value was tested above." << endl;

  Functor<cCallable *, void, TP4> pcf(&c);
  pcf('a', 'b', 'c', 'd');
  GenericFunctor<void, TP4> pcgf(c);
  pcgf('a', 'b', 'c', 'd');
  BinderFirst<cCallable *, void, TP4> *pcbf =
    BindFirst<cCallable *, void, TP4>(&c, 'a');
  if(pcbf) {
    (*pcbf)('b', 'c', 'd');
    delete pcbf;
  }

  cout << endl << endl;

  cout << "Now that the operator() have been tested to ensure proper" << endl;
  cout << "working.  Now it is time to test the rest of the member" << endl;
  cout << "functions." << endl;

  cout << endl;

  cout << "Testing Functor" << endl;

  // Copy constructor
  cout << "Test copy constructor-" << endl;
  Functor<SF4, void, TP4> cf2(&Print);
  Functor<SF4, void, TP4> cf3(cf2);
  cf3('a', 'b', 'c', 'd');

  if(cf2 == cf2) { cout << "Identical is same" << endl; }
  else { cout << "Identical is not same" << endl; }
  if(cf2 == cf3) { cout << "Equivalent is same" << endl; }
  else { cout << "Equivalent is not same" << endl; }

  cout << endl;

  // Test assignment
  cout << "Test assignment-" << endl;
  Functor<SF4, void, TP4> cf4(&Print2);
  if(cf2 == cf4) { cout << "Not equal are the same" << endl; }
  else { cout << "Not equal are not the same" << endl; }
  cf4 = cf3;
  if(cf2 == cf4) { cout << "Equal are the same" << endl; }
  else { cout << "Equal are not the same" << endl; }
  cf4 = &Print2;
  if(cf2 == cf4) { cout << "Not equal are the same" << endl; }
  else { cout << "Not equal are not the same" << endl; }
  cf4 = static_cast<SF4>(&Print);
  if(cf2 == cf4) { cout << "Equal are the same" << endl; }
  else { cout << "Equal are not the same" << endl; }

  cout << endl;

  cout << "Interface access-" << endl;
  Functor<SF4, void, TP4>::Interface *i1 = cf2.InterfacePtr();
  Functor<SF4, void, TP4>::Interface &i2 = cf2.InterfaceRef();
  Functor<SF4, void, TP4>::Interface *i3 = &cf2;

  (*i1)('a', 'b', 'c', 'd');
    i2 ('a', 'b', 'c', 'd');
  (*i3)('a', 'b', 'c', 'd');

  iFunctor<void, TP4> *i4 = i1->Clone();
  if(i4) {
    (*i4)('a', 'b', 'c', 'd');
    delete i4;
  }

  Functor<SF4, void, TP4> * i5 = cf2.Clone();
  if(i5) {
    (*i5)('a', 'b', 'c', 'd');
    delete i5;
  }

  cout << endl;

  cout << "Testing GenericFunctor" << endl;

  // Copy constructor
  cout << "Test copy constructor-" << endl;
  GenericFunctor<void, TP4> gf2(static_cast<SF4>(&Print));
  GenericFunctor<void, TP4> gf3(gf2);
  gf3('a', 'b', 'c', 'd');

  if(gf2 == gf2) { cout << "Identical is same" << endl; }
  else { cout << "Identical is not same" << endl; }
  if(gf2 == gf3) { cout << "Equivalent is same" << endl; }
  else { cout << "Equivalent is not same" << endl; }

  cout << endl;

  // Test assignment
  cout << "Test assignment-" << endl;
  GenericFunctor<void, TP4> gf4(&Print2);
  if(gf2 == gf4) { cout << "Not equal are the same" << endl; }
  else { cout << "Not equal are not the same" << endl; }
  gf4 = gf3;
  if(gf2 == gf4) { cout << "Equal are the same" << endl; }
  else { cout << "Equal are not the same" << endl; }
  gf4 = &Print2;
  if(gf2 == gf4) { cout << "Not equal are the same" << endl; }
  else { cout << "Not equal are not the same" << endl; }
  gf4 = static_cast<SF4>(&Print);
  if(gf2 == gf4) { cout << "Equal are the same" << endl; }
  else { cout << "Equal are not the same" << endl; }

  cout << endl;

  cout << "Interface access-" << endl;
  GenericFunctor<void, TP4>::Interface *gi1 = gf2.InterfacePtr();
  GenericFunctor<void, TP4>::Interface &gi2 = gf2.InterfaceRef();
  GenericFunctor<void, TP4>::Interface *gi3 = &gf2;

  (*gi1)('a', 'b', 'c', 'd');
    gi2 ('a', 'b', 'c', 'd');
  (*gi3)('a', 'b', 'c', 'd');

  iFunctor<void, TP4> *gi4 = gi1->Clone();
  if(gi4) {
    (*gi4)('a', 'b', 'c', 'd');
    delete gi4;
  }

  GenericFunctor<void, TP4> *gi5 = gf2.Clone();
  if(gi5) {
    (*gi5)('a', 'b', 'c', 'd');
    delete gi5;
  }

  cout << endl;

  cout << "Testing MemberFunctionWrapper" << endl;

  // Copy constructor
  cout << "Test copy constructor-" << endl;
  MemberFunctionWrapper<A*, MF4, void, TP4> tmf2(pa, &A::Print);
  MemberFunctionWrapper<A*, MF4, void, TP4> tmf3(tmf2);
  tmf3('a', 'b', 'c', 'd');

  if(tmf2 == tmf2) { cout << "Identical is same" << endl; }
  else { cout << "Identical is not same" << endl; }
  if(tmf2 == tmf3) { cout << "Equivalent is same" << endl; }
  else { cout << "Equivalent is not same" << endl; }

  cout << endl;

  // Test assignment
  cout << "Test assignment-" << endl;
  MemberFunctionWrapper<A*, MF4, void, TP4> tmf4(pa, &A::Print2);
  if(tmf2 == tmf4) { cout << "Not equal are the same" << endl; }
  else { cout << "Not equal are not the same" << endl; }
  tmf4 = tmf3;
  if(tmf2 == tmf4) { cout << "Equal are the same" << endl; }
  else { cout << "Equal are not the same" << endl; }

  cout << endl;

  cout << "Interface access-" << endl;
  MemberFunctionWrapper<A*, MF4, void, TP4>::Interface *mi1 = tmf2.InterfacePtr();
  MemberFunctionWrapper<A*, MF4, void, TP4>::Interface &mi2 = tmf2.InterfaceRef();
  MemberFunctionWrapper<A*, MF4, void, TP4>::Interface *mi3 = &tmf2;

  (*mi1)('a', 'b', 'c', 'd');
    mi2 ('a', 'b', 'c', 'd');
  (*mi3)('a', 'b', 'c', 'd');

  iFunctor<void, TP4> *mi4 = mi1->Clone();
  if(mi4) {
    (*mi4)('a', 'b', 'c', 'd');
    delete mi4;
  }

  MemberFunctionWrapper<A*, MF4, void, TP4> *mi5 = tmf2.Clone();
  if(mi5) {
    (*mi5)('a', 'b', 'c', 'd');
    delete mi5;
  }

  cout << endl;

  cout << "Testing BinderFirst" << endl;

  // Copy constructor
  cout << "Test copy constructor-" << endl;
  BinderFirst<SF4, void, TP4> bcf2(&Print, 'a');
  BinderFirst<SF4, void, TP4> bcf3(bcf2);
  bcf3('b', 'c', 'd');

  if(bcf2 == bcf2) { cout << "Identical is same" << endl; }
  else { cout << "Identical is not same" << endl; }
  if(bcf2 == bcf3) { cout << "Equivalent is same" << endl; }
  else { cout << "Equivalent is not same" << endl; }

  cout << endl;

  // Test assignment
  cout << "Test assignment-" << endl;
  BinderFirst<SF4, void, TP4> bcf4(&Print2, 'a');
  if(bcf2 == bcf4) { cout << "Not equal are the same" << endl; }
  else { cout << "Not equal are not the same" << endl; }
  bcf4 = bcf3;
  if(bcf2 == bcf4) { cout << "Equal are the same" << endl; }
  else { cout << "Equal are not the same" << endl; }

  bcf4.SetFun(&Print2);
  if(bcf2 == bcf4) { cout << "Not equal are the same" << endl; }
  else { cout << "Not equal are not the same" << endl; }
  bcf4.SetFun(&Print);
  if(bcf2 == bcf4) { cout << "Equal are the same" << endl; }
  else { cout << "Equal are not the same" << endl; }

  bcf4.SetBound('b');
  if(bcf2 == bcf4) { cout << "Not equal are the same" << endl; }
  else { cout << "Not equal are not the same" << endl; }
  bcf4.SetBound('a');
  if(bcf2 == bcf4) { cout << "Equal are the same" << endl; }
  else { cout << "Equal are not the same" << endl; }

  cout << endl;

  cout << "Interface access-" << endl;
  BinderFirst<SF4, void, TP4>::Interface *bi1 = bcf2.InterfacePtr();
  BinderFirst<SF4, void, TP4>::Interface &bi2 = bcf2.InterfaceRef();
  BinderFirst<SF4, void, TP4>::Interface *bi3 = &bcf2;

  (*bi1)('b', 'c', 'd');
    bi2 ('b', 'c', 'd');
  (*bi3)('b', 'c', 'd');

  // iFunctor of a binder has one less arguement
  iFunctor<void, TP3> *bi4 = bi1->Clone();
  if(bi4) {
    (*bi4)('b', 'c', 'd');
    delete bi4;
  }

  BinderFirst<SF4, void, TP4> *bi5 = bcf2.Clone();
  if(bi5) {
    (*bi5)('b', 'c', 'd');
    delete bi5;
  }

  cout << endl;

  cout << "Testing Chain" << endl;

  // Copy constructor
  cout << "Test copy constructor-" << endl;
  Chain<SF4, SF4, void, TP4> chcf2(&Print, &Print);
  Chain<SF4, SF4, void, TP4> chcf3(chcf2);
  chcf3('a', 'b', 'c', 'd');

  if(chcf2 == chcf2) { cout << "Identical is same" << endl; }
  else { cout << "Identical is not same" << endl; }
  if(chcf2 == chcf3) { cout << "Equivalent is same" << endl; }
  else { cout << "Equivalent is not same" << endl; }

  cout << endl;

  // Test assignment
  cout << "Test assignment-" << endl;
  Chain<SF4, SF4, void, TP4> chcf4(&Print2, &Print2);
  if(chcf2 == chcf4) { cout << "Not equal are the same" << endl; }
  else { cout << "Not equal are not the same" << endl; }
  chcf4 = chcf3;
  if(chcf2 == chcf4) { cout << "Equal are the same" << endl; }
  else { cout << "Equal are not the same" << endl; }

  cout << endl;

  cout << "Interface access-" << endl;
  Chain<SF4, SF4, void, TP4>::Interface *chi1 = chcf2.InterfacePtr();
  Chain<SF4, SF4, void, TP4>::Interface &chi2 = chcf2.InterfaceRef();
  Chain<SF4, SF4, void, TP4>::Interface *chi3 = &chcf2;

  (*chi1)('a', 'b', 'c', 'd');
    chi2 ('a', 'b', 'c', 'd');
  (*chi3)('a', 'b', 'c', 'd');

  iFunctor<void, TP4> *chi4 = chi1->Clone();
  if(chi4) {
    (*chi4)('a', 'b', 'c', 'd');
    // I use it below so don't delete yet.
  }

  Chain<SF4, SF4, void, TP4> *chi5 = chcf2.Clone();
  if(chi5) {
    (*chi5)('a', 'b', 'c', 'd');
    delete chi5;
  }

  Chain<SF4, SF4, void, TP4> *chc1 = Chainer<SF4, SF4, void, TP4>(&Print, &Print);
  if(chc1) {
    (*chc1)('a', 'b', 'c', 'd');
    delete chc1;
  }

  cout << endl;

  if(chi4) {
    cout << "Print out a chain of two chains:" << endl;
    Chain<iFunctor<void,TP4>*, iFunctor<void,TP4>*, void, TP4> *chc2 = Chainer(chi4, chi4);
    if(chc2) {
      (*chc2)('a', 'b', 'c', 'd');
      delete chc2;
    }
    delete chi4;
  }

  cout << endl;

  cout << "Test OneToMany" << endl;

  Functor<int (*)(char, char, char, char), int, TP4> fi1(&Print3);
  Functor<int (*)(char, char, char, char), int, TP4> fi2(&Print4);
  Functor<int (*)(char, char, char, char), int, TP4> fi3(&Print5);
  OneToMany<int, TP4> otm1(-1);
  otm1.Add(&fi1);
  otm1.Add(&fi2);
  otm1.Add(&fi1);
  otm1.Add(&fi3);
  cout << "Return: " << otm1('z', 'y', 'x', 'w') << endl;
  otm1.Remove(&fi2);
  cout << "Return: " << otm1('z', 'y', 'x', 'w') << endl;
  otm1.Add(&fi2);
  cout << "Return: " << otm1('z', 'y', 'x', 'w') << endl;
  otm1.Clear();
  cout << "Return: " << otm1('z', 'y', 'x', 'w') << endl;
  otm1.Add(&fi1);
  cout << "Return: " << otm1('z', 'y', 'x', 'w') << endl;
  otm1.Add(&fi2);
  otm1.Add(&fi3);
  cout << "Return: " << otm1('z', 'y', 'x', 'w') << endl;

  // Copy constructor
  cout << "Test copy constructor-" << endl;
  OneToMany<int, TP4> otmi2(otm1);
  OneToMany<int, TP4> otmi3(otmi2);
  otmi3('a', 'b', 'c', 'd');

  if(otmi2 == otmi2) { cout << "Identical is same" << endl; }
  else { cout << "Identical is not same" << endl; }
  if(otmi2 == otmi3) { cout << "Equivalent is same" << endl; }
  else { cout << "Equivalent is not same" << endl; }

  cout << endl;

  // Test assignment
  cout << "Test assignment-" << endl;
  OneToMany<int, TP4> otmi4(-1);
  otmi4.Add(&fi1);
  otmi4.Add(&fi2);
  if(otmi2 == otmi4) { cout << "Not equal are the same" << endl; }
  else { cout << "Not equal are not the same" << endl; }
  otmi4 = otmi3;
  if(otmi2 == otmi4) { cout << "Equal are the same" << endl; }
  else { cout << "Equal are not the same" << endl; }

  cout << endl;

  cout << "Interface access-" << endl;
  OneToMany<int, TP4>::Interface *iotmi1 = otmi2.InterfacePtr();
  OneToMany<int, TP4>::Interface &iotmi2 = otmi2.InterfaceRef();
  OneToMany<int, TP4>::Interface *iotmi3 = &otmi2;

  (*iotmi1)('a', 'b', 'c', 'd');
    iotmi2 ('a', 'b', 'c', 'd');
  (*iotmi3)('a', 'b', 'c', 'd');

  iFunctor<int, TP4> *iotmi4 = iotmi1->Clone();
  if(iotmi4) {
    (*iotmi4)('a', 'b', 'c', 'd');
    // I use it below so don't delete yet.
  }

  OneToMany<int, TP4> *iotmi5 = otmi2.Clone();
  if(iotmi5) {
    (*iotmi5)('a', 'b', 'c', 'd');
    delete iotmi5;
  }

  cout << endl;

  if(iotmi4) {
    OneToMany<int, TP4> otm2;
    otm2.Add(iotmi4);
    otm2.Add(&otm1);
    cout << "Print out a OneToMany of two OneToManys:" << endl;
    cout << "Return: " << otm2('a', 'b', 'c', 'd') << endl;
    delete iotmi4;
  }

  cout << endl;

  cout << "Test OneToMany void specialization" << endl;

  Functor<void (*)(char, char, char, char), void, TP4> vfi1(&Print);
  Functor<void (*)(char, char, char, char), void, TP4> vfi2(&Print2);
  Functor<void (*)(char, char, char, char), void, TP4> vfi3(&Print22);
  OneToMany<void, TP4> votm1;
  votm1.Add(&vfi1);
  votm1.Add(&vfi2);
  votm1.Add(&vfi1);
  votm1.Add(&vfi3);
  cout << "Should be 4 lines" << endl;
  votm1('z', 'y', 'x', 'w');
  votm1.Remove(&vfi2);
  cout << "Should be 3 lines" << endl;
  votm1('z', 'y', 'x', 'w');
  votm1.Add(&vfi2);
  cout << "Should be 4 lines" << endl;
  votm1('z', 'y', 'x', 'w');
  votm1.Clear();
  cout << "Should be 0 lines" << endl;
  votm1('z', 'y', 'x', 'w');
  votm1.Add(&vfi1);
  cout << "Should be 1 lines" << endl;
  votm1('z', 'y', 'x', 'w');
  votm1.Add(&vfi2);
  votm1.Add(&vfi3);
  cout << "Should be 3 lines" << endl;
  votm1('z', 'y', 'x', 'w');

  // Copy constructor
  cout << "Test copy constructor-" << endl;
  OneToMany<void, TP4> votmi2(votm1);
  OneToMany<void, TP4> votmi3(votmi2);
  cout << "Should be 3 lines" << endl;
  votmi3('a', 'b', 'c', 'd');

  if(votmi2 == votmi2) { cout << "Identical is same" << endl; }
  else { cout << "Identical is not same" << endl; }
  if(votmi2 == votmi3) { cout << "Equivalent is same" << endl; }
  else { cout << "Equivalent is not same" << endl; }

  cout << endl;

  // Test assignment
  cout << "Test assignment-" << endl;
  OneToMany<void, TP4> votmi4;
  votmi4.Add(&vfi1);
  votmi4.Add(&vfi2);
  if(votmi2 == votmi4) { cout << "Not equal are the same" << endl; }
  else { cout << "Not equal are not the same" << endl; }
  votmi4 = votmi3;
  if(votmi2 == votmi4) { cout << "Equal are the same" << endl; }
  else { cout << "Equal are not the same" << endl; }

  cout << endl;

  cout << "Interface access-" << endl;
  OneToMany<void, TP4>::Interface *ivotmi1 = votmi2.InterfacePtr();
  OneToMany<void, TP4>::Interface &ivotmi2 = votmi2.InterfaceRef();
  OneToMany<void, TP4>::Interface *ivotmi3 = &votmi2;

  cout << "Should be 3 lines" << endl;
  (*ivotmi1)('a', 'b', 'c', 'd');
  cout << "Should be 3 lines" << endl;
    ivotmi2 ('a', 'b', 'c', 'd');
  cout << "Should be 3 lines" << endl;
  (*ivotmi3)('a', 'b', 'c', 'd');

  iFunctor<void, TP4> *ivotmi4 = ivotmi1->Clone();
  if(ivotmi4) {
    cout << "Should be 3 lines" << endl;
    (*ivotmi4)('a', 'b', 'c', 'd');
    // I use it below so don't delete yet.
  }

  OneToMany<void, TP4> *ivotmi5 = votmi2.Clone();
  if(ivotmi5) {
    cout << "Should be 3 lines" << endl;
    (*ivotmi5)('a', 'b', 'c', 'd');
    delete ivotmi5;
  }

  cout << endl;

  if(ivotmi4) {
    OneToMany<void, TP4> votm2;
    votm2.Add(ivotmi4);
    votm2.Add(&votm1);
    cout << "Print out a OneToMany of two OneToManys:" << endl;
    cout << "Should be 6 lines" << endl;
    votm2('a', 'b', 'c', 'd');
    delete ivotmi4;
  }

  cout << endl << endl;

  cout << "Test exception for bad callable entity." << endl;
  try {
    Functor<cCallable*, void, TP1> ef1(0);
  } catch(BadCallableEntity b) {
    cout << b.what() << endl;
  }

  return 0;
}

//*** Static Prints ******************************************************//
void Print(void)
{
  cout << "Print: Empty" << endl;
  return;
}
void Print(char x1)
{
  cout << "Print: ";
  cout << x1;
  cout << endl;
  return;
}
void Print(char x1, char x2)
{
  cout << "Print: ";
  cout << x1 << x2;
  cout << endl;
  return;
}
void Print(char x1, char x2, char x3)
{
  cout << "Print: ";
  cout << x1 << x2 << x3;
  cout << endl;
  return;
}
void Print(char x1, char x2, char x3, char x4)
{
  cout << "Print: ";
  cout << x1 << x2 << x3 << x4;
  cout << endl;
  return;
}
void Print(char x1, char x2, char x3, char x4, char x5)
{
  cout << "Print: ";
  cout << x1 << x2 << x3 << x4 << x5;
  cout << endl;
  return;
}
void Print(char x1, char x2, char x3, char x4, char x5, char x6)
{
  cout << "Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6;
  cout << endl;
  return;
}
void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7)
{
  cout << "Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7;
  cout << endl;
  return;
}
void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8)
{
  cout << "Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8;
  cout << endl;
  return;
}
void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9)
{
  cout << "Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9;
  cout << endl;
  return;
}
void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9, char x10)
{
  cout << "Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9 << x10;
  cout << endl;
  return;
}
void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9, char x10, char x11)
{
  cout << "Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9 << x10 << x11;
  cout << endl;
  return;
}
void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9, char x10, char x11, char x12)
{
  cout << "Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9 << x10 << x11;
  cout << x12;
  cout << endl;
  return;
}
void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9, char x10, char x11, char x12, char x13)
{
  cout << "Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9 << x10 << x11;
  cout << x12 << x13;
  cout << endl;
  return;
}
void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9, char x10, char x11, char x12, char x13, char x14)
{
  cout << "Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9 << x10 << x11;
  cout << x12 << x13 << x14;
  cout << endl;
  return;
}
void Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9, char x10, char x11, char x12, char x13, char x14,
           char x15)
{
  cout << "Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9 << x10 << x11;
  cout << x12 << x13 << x14 << x15;
  cout << endl;
  return;
}
//*** Static Prints ******************************************************//

//*** Member Prints ******************************************************//
void A::Print(void)
{
  cout << "A::Print: Empty" << endl;
  return;
}
void A::Print(char x1) const
{
  cout << "A::Print: ";
  cout << x1;
  cout << endl;
  return;
}
void A::Print(char x1, char x2)
{
  cout << "A::Print: ";
  cout << x1 << x2;
  cout << endl;
  return;
}
void A::Print(char x1, char x2, char x3)
{
  cout << "A::Print: ";
  cout << x1 << x2 << x3;
  cout << endl;
  return;
}
void A::Print(char x1, char x2, char x3, char x4)
{
  cout << "A::Print: ";
  cout << x1 << x2 << x3 << x4;
  cout << endl;
  return;
}
void A::Print(char x1, char x2, char x3, char x4, char x5)
{
  cout << "A::Print: ";
  cout << x1 << x2 << x3 << x4 << x5;
  cout << endl;
  return;
}
void A::Print(char x1, char x2, char x3, char x4, char x5, char x6)
{
  cout << "A::Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6;
  cout << endl;
  return;
}
void A::Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7)
{
  cout << "A::Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7;
  cout << endl;
  return;
}
void A::Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8)
{
  cout << "A::Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8;
  cout << endl;
  return;
}
void A::Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9)
{
  cout << "A::Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9;
  cout << endl;
  return;
}
void A::Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9, char x10)
{
  cout << "A::Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9 << x10;
  cout << endl;
  return;
}
void A::Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9, char x10, char x11)
{
  cout << "A::Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9 << x10 << x11;
  cout << endl;
  return;
}
void A::Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9, char x10, char x11, char x12)
{
  cout << "A::Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9 << x10 << x11;
  cout << x12;
  cout << endl;
  return;
}
void A::Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9, char x10, char x11, char x12, char x13)
{
  cout << "A::Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9 << x10 << x11;
  cout << x12 << x13;
  cout << endl;
  return;
}
void A::Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9, char x10, char x11, char x12, char x13, char x14)
{
  cout << "A::Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9 << x10 << x11;
  cout << x12 << x13 << x14;
  cout << endl;
  return;
}
void A::Print(char x1, char x2, char x3, char x4, char x5, char x6, char x7,
           char x8, char x9, char x10, char x11, char x12, char x13, char x14,
           char x15)
{
  cout << "A::Print: ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9 << x10 << x11;
  cout << x12 << x13 << x14 << x15;
  cout << endl;
  return;
}
//*** Member A::Prints ******************************************************//

//*** cCallable **************************************************************//
void cCallable::operator()(void)
{
  cout << "cCallable::operator(): Empty" << endl;
  return;
}
void cCallable::operator()(char x1) const
{
  cout << "cCallable::operator(): ";
  cout << x1;
  cout << endl;
  return;
}
void cCallable::operator()(char x1, char x2)
{
  cout << "cCallable::operator(): ";
  cout << x1 << x2;
  cout << endl;
  return;
}
void cCallable::operator()(char x1, char x2, char x3)
{
  cout << "cCallable::operator(): ";
  cout << x1 << x2 << x3;
  cout << endl;
  return;
}
void cCallable::operator()(char x1, char x2, char x3, char x4)
{
  cout << "cCallable::operator(): ";
  cout << x1 << x2 << x3 << x4;
  cout << endl;
  return;
}
void cCallable::operator()(char x1, char x2, char x3, char x4, char x5)
{
  cout << "cCallable::operator(): ";
  cout << x1 << x2 << x3 << x4 << x5;
  cout << endl;
  return;
}
void cCallable::operator()(char x1, char x2, char x3, char x4, char x5, char x6)
{
  cout << "cCallable::operator(): ";
  cout << x1 << x2 << x3 << x4 << x5 << x6;
  cout << endl;
  return;
}
void cCallable::operator()(char x1, char x2, char x3, char x4, char x5, char x6,
                          char x7)
{
  cout << "cCallable::operator(): ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7;
  cout << endl;
  return;
}
void cCallable::operator()(char x1, char x2, char x3, char x4, char x5, char x6,
                          char x7, char x8)
{
  cout << "cCallable::operator(): ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8;
  cout << endl;
  return;
}
void cCallable::operator()(char x1, char x2, char x3, char x4, char x5, char x6,
                          char x7, char x8, char x9)
{
  cout << "cCallable::operator(): ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9;
  cout << endl;
  return;
}
void cCallable::operator()(char x1, char x2, char x3, char x4, char x5, char x6,
                          char x7, char x8, char x9, char x10)
{
  cout << "cCallable::operator(): ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9 << x10;
  cout << endl;
  return;
}
void cCallable::operator()(char x1, char x2, char x3, char x4, char x5, char x6,
                          char x7, char x8, char x9, char x10, char x11)
{
  cout << "cCallable::operator(): ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9 << x10 << x11;
  cout << endl;
  return;
}
void cCallable::operator()(char x1, char x2, char x3, char x4, char x5, char x6,
                          char x7, char x8, char x9, char x10, char x11,
                          char x12)
{
  cout << "cCallable::operator(): ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9 << x10 << x11;
  cout << x12;
  cout << endl;
  return;
}
void cCallable::operator()(char x1, char x2, char x3, char x4, char x5, char x6,
                          char x7, char x8, char x9, char x10, char x11,
                          char x12, char x13)
{
  cout << "cCallable::operator(): ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9 << x10 << x11;
  cout << x12 << x13;
  cout << endl;
  return;
}
void cCallable::operator()(char x1, char x2, char x3, char x4, char x5, char x6,
                          char x7, char x8, char x9, char x10, char x11,
                          char x12, char x13, char x14)
{
  cout << "cCallable::operator(): ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9 << x10 << x11;
  cout << x12 << x13 << x14;
  cout << endl;
  return;
}
void cCallable::operator()(char x1, char x2, char x3, char x4, char x5, char x6,
                          char x7, char x8, char x9, char x10, char x11,
                          char x12, char x13, char x14, char x15)
{
  cout << "cCallable::operator(): ";
  cout << x1 << x2 << x3 << x4 << x5 << x6 << x7 << x8 << x9 << x10 << x11;
  cout << x12 << x13 << x14 << x15;
  cout << endl;
  return;
}
//*** cCallable ***********************************************************//
