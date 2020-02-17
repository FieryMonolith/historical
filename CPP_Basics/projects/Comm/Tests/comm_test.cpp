#include <iostream>

#include "Comm/Interface/Connection.h"
#include "Type/Interface/Sequence.h"
#include "Functor/Interface/OneToMany.h"

using namespace std;
using namespace nFunctor;
using namespace nComm;
using namespace nType;

void Print0(int x) { cout << "P0: " << x << endl; }
void Print1(int x) { cout << "P1: " << x << endl; }

class A0 {
public:
  void Print(int x) { cout << "A0: " << x << endl; }
  void Print1(int x) { cout << "A01: " << x << endl; }
};

class A1 {
public:
  void Print(int x) { cout << "A1: " << x << endl; }
};

class A2 {
public:
  void Print(int x) { cout << "A2: " << x << endl; }
};

class B0 {
public:
  void operator()(int x) { cout << "B0: " << x << endl; }
};

class B1 {
public:
  void operator()(int x) { cout << "B1: " << x << endl; }
};

class C3 {
public:
  void MySlot1(int x) { cout << "MySlot1: " << x << endl; }
protected:
  void MySlot2(int x) { cout << "MySlot2: " << x << endl; }
private:
  void MySlot3(int x) { cout << "MySlot3: " << x << endl; }
};

class C1 {
public:
  void Conn(C3 &u) {
    Connection::Create(d_mySignal2, &u, &C3::MySlot1);
    Connection::Create(d_mySignal3, &u, &C3::MySlot1);
    return;
  }
  void operator()(int x) { d_mySignal1(x); d_mySignal2(x); d_mySignal3(x); }
  OneToMany< void, Seq<int> > d_mySignal1;
protected:
  OneToMany< void, Seq<int> > d_mySignal2;
private:
  OneToMany< void, Seq<int> > d_mySignal3;
};

class C2 {
public:
  void Conn(C1 &c1) {
    Connection::Create(c1.d_mySignal1, this, &C2::MySlot2);
    Connection::Create(c1.d_mySignal1, this, &C2::MySlot3);
    return;
  }
  void MySlot1(int x) { cout << "MySlot1: " << x << endl; }
protected:
  void MySlot2(int x) { cout << "MySlot2: " << x << endl; }
private:
  void MySlot3(int x) { cout << "MySlot3: " << x << endl; }
};

class Test {
public:
  void operator()(int x) { d_signal(x); }
  OneToMany< void, Seq<int> > d_signal;
};

int main(void) {
  try {
    Test t;
    t(1);

    cout << "Create Print0" << endl;
    Connection::Create(t.d_signal, Print0);
    t(2);
    cout << endl;

    cout << "Create &Print1" << endl;
    Connection::Create(t.d_signal, &Print1);
    t(3);
    cout << endl;

    cout << "Create B0" << endl;
    B0 b;
    Connection::Create(t.d_signal, b);
    t(4);
    cout << endl;

    cout << "Create B1*" << endl;
    B1 b1;
    Connection::Create(t.d_signal, &b1);
    t(5);
    cout << endl;

    cout << "Create A0 - &A0::Print" << endl;
    A0 a;
    Connection::Create(t.d_signal, a, &A0::Print);
    t(6);
    cout << endl;

    cout << "Create A1* - &A1::Print" << endl;
    A1 a1;
    Connection::Create(t.d_signal, &a1, &A1::Print);
    t(7);
    cout << endl;

    cout << "Create A2& - &A2::Print" << endl;
    A2 a2;
    Connection::Create<Seq<int>, A2&>(t.d_signal, a2, &A2::Print);
    t(8);
    cout << endl;

    cout << "Break A2& - &A2::Print" << endl;
    Connection::Break<Seq<int>, A2&>(t.d_signal, a2, &A2::Print);
    t(9);
    cout << endl;

    cout << "Break A1* - &A1::Print" << endl;
    Connection::Break(t.d_signal, &a1, &A1::Print);
    t(10);
    cout << endl;

    cout << "Break A0 - &A0::Print" << endl;
    Connection::Break(t.d_signal, a, &A0::Print);
    t(11);
    cout << endl;

    cout << "Break B1*" << endl;
    Connection::Break(t.d_signal, &b1);
    t(12);
    cout << endl;

    cout << "Break B0" << endl;
    Connection::Break(t.d_signal, b);
    t(13);
    cout << endl;

    cout << "Break &Print1" << endl;
    Connection::Break(t.d_signal, &Print1);
    t(14);
    cout << endl;

    cout << "Break Print0" << endl;
    Connection::Break(t.d_signal, Print0);
    t(15);
    cout << endl;

    cout << "BreakAll Signal" << endl;
    Connection::BreakAll(t.d_signal);
    t(16);
    cout << endl;

#if 0
    Connection::Create(t.d_signal, Print0);
    Connection::Create(t.d_signal, &Print1);
    Connection::Create(t.d_signal, b);
    Connection::Create(t.d_signal, &b1);
    Connection::Create(t.d_signal, a, &A0::Print);
    Connection::Create(t.d_signal, &a1, &A1::Print);
    Connection::Create< Seq<int>, A2&>(t.d_signal, a2, &A2::Print);
#endif

    cout << "Create Print0 - Print1 - Print1 - Print0 - Print1 - Print0";
    cout << endl;
    Connection::Create(t.d_signal, Print0);
    Connection::Create(t.d_signal, &Print1);
    Connection::Create(t.d_signal, &Print1);
    Connection::Create(t.d_signal, Print0);
    Connection::Create(t.d_signal, &Print1);
    Connection::Create(t.d_signal, Print0);
    t(17);
    cout << endl;

    cout << "Break Print1" << endl;
    Connection::Break(t.d_signal, &Print1);
    t(18);
    cout << endl;
    cout << "Break Print1" << endl;
    Connection::Break(t.d_signal, &Print1);
    t(19);
    cout << endl;
    cout << "Break Print1" << endl;
    Connection::Break(t.d_signal, &Print1);
    t(20);
    cout << endl;
    cout << "Break Print1" << endl;
    Connection::Break(t.d_signal, &Print1);
    t(21);
    cout << endl;

    cout << "Create Print1 - Print1 - A0*::Print - A0*::Print1" << endl;
    cout << "     - A0*::Print1 - A0*::Print - B0*" << endl;
    Connection::Create(t.d_signal, &Print1);
    Connection::Create(t.d_signal, &Print1);
    Connection::Create(t.d_signal, &a, &A0::Print);
    Connection::Create(t.d_signal, &a, &A0::Print1);
    Connection::Create(t.d_signal, &a, &A0::Print1);
    Connection::Create(t.d_signal, &a, &A0::Print);
    Connection::Create(t.d_signal, &b);
    t(22);
    cout << endl;

    cout << "BreakAll Print1" << endl;
    Connection::BreakAll< Seq<int> >(&Print1);
    t(23);
    cout << endl;

    cout << "BreakAll A0* ::Print" << endl;
    Connection::BreakAll< Seq<int> >(&a, &A0::Print);
    t(24);
    cout << endl;

    cout << "Create 3 A0* ::Print" << endl;
    Connection::Create(t.d_signal, &a, &A0::Print);
    Connection::Create(t.d_signal, &a, &A0::Print);
    Connection::Create(t.d_signal, &a, &A0::Print);
    t(25);
    cout << endl;

    cout << "Break A0* ::Print1" << endl;
    Connection::Break(t.d_signal, &a, &A0::Print1);
    t(26);
    cout << endl;

    cout << "Break A0* ::Print1" << endl;
    Connection::Break(t.d_signal, &a, &A0::Print1);
    t(27);
    cout << endl;

    cout << "Break A0* ::Print1" << endl;
    Connection::Break(t.d_signal, &a, &A0::Print1);
    t(28);
    cout << endl;

    cout << "BreakAll signal" << endl;
    Connection::BreakAll(t.d_signal);
    t(29);
    cout << endl;

    C1 c1;
    C2 c2;
    C3 c3;
    Connection::Create(c1.d_mySignal1, &c2, &C2::MySlot1);
    //Connection::Create(c1.d_mySignal1, &c2, &C2::MySlot2); // Error protected
    //Connection::Create(c1.d_mySignal1, &c2, &C2::MySlot3); // Error private
    //Connection::Create(c1.d_mySignal2, &c2, &C2::MySlot1); // Error protected
    //Connection::Create(c1.d_mySignal3, &c2, &C2::MySlot1); // Error private
    c1(30);
    cout << endl;

    c2.Conn(c1);
    c1(31);
    cout << endl;

    c1.Conn(c3);
    c1(32);
    cout << endl;

    C2 *c4 = 0;
    Connection::Create(c1.d_mySignal1, c4, &C2::MySlot1); // Throw BadCallableEntity

  } catch(nFunctor::BadCallableEntity bce) {
    cout << bce.what() << endl;
  } catch(std::bad_alloc ba) {
    cout << ba.what() << endl;
  } catch(...) {
    cout << "Unknown exception caught." << endl;
  }

  return 0;
}
