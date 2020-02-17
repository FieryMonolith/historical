#include <iostream>

#include "Ref.h"
#include "ReRef.h"

using namespace std;

void TestRefAddress(void);
template <typename T> void TRA1(Ref<T> r);
template <typename T> void TRA2(Ref<T> &r);
template <typename T> void TRA3(Ref<T> *r);
void TRA4(int &r);
void TRA5(int r);

// BEGIN CUT from boost::addressof
// Copyright (C) 2002 Brad King (brad.king@kitware.com) 
//                    Doug Gregor (gregod@cs.rpi.edu)
//                    Peter Dimov
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.

// For more information, see http://www.boost.org
template <typename T> T*
addressof(T& v)
{
  return reinterpret_cast<T*>(&const_cast<char&>(
         reinterpret_cast<const volatile char &>(v)));
}
// END CUT

int main(void) {
  int i = 31;
  int i2 = 32;
  const int j = 37;

  int &si = i;
  const int &csi = i;

  /*** BEGIN Test Ref<T> and Ref<const T> *******************/
  // Test construction
  Ref<int> ri = i;
  Ref<const int> ri2 = j;
  //Ref<int> ri3 = j; // -> Compiler Error
  Ref<const int> ri4 = i;
  
  Ref<const int> ri5 = si;
  Ref<const int> ri6 = csi;
  Ref<int> ri7 = si;
  //Ref<int> ri8 = csi; // -> Compiler Error

  cout << ri << " " << ri2 << " " << ri4 << endl;

  Ref<int> ari(i);
  Ref<const int> ari2(j);
  //Ref<int> ari3(j); // -> Compiler Error
  Ref<const int> ari4(i);

  cout << ari << " " << ari2 << " " << ari4 << endl;

  Ref<Ref<int>::type> zeta = i;
  cout << zeta << endl;

  // Test assignment operators
  Ref<int> tri(i2);
  ri = tri;
  cout << ri << " " << tri << endl;
  ri = 31;
  cout << ri << " " << tri << endl;
  //ri2 = i;  // -> Compiler Error
  //ri2 = ri; // -> Compiler Error

  // Test operator&()
  int *pi = &ri;
  const int *pi2 = &ri;
  //int *pj = &ri2; // -> Compiler Error
  const int *pj2 = &ri2;

  cout << *pi2 << " " << *pj2 << endl;

  // Test operator T &()
  int &ii = ri;
  const int &ii2 = ri;
  //int &ij = ri2; // -> Compiler error
  const int &ij2 = ri2;

  cout << ii2 << " " << ij2 << endl;
  /*** END Test Ref<T> and Ref<const T> *********************/

  /*** BEGIN Test const Ref<T> and const Ref<const T> *******/
  // Test construction
  const Ref<int> cri = i;
  const Ref<const int> cri2 = j;
  //const Ref<int> cri3 = j; // -> Compiler Error
  const Ref<const int> cri4 = i;

  const Ref<const int> cri5 = si;
  const Ref<const int> cri6 = csi;
  const Ref<int> cri7 = si;
  //const Ref<int> cri8 = csi; // -> Compiler Error

  cout << cri << " " << cri2 << " " << cri4 << endl;

  Ref<int> cari(i);
  Ref<const int> cari2(j);
  //Ref<int> cari3(j); // -> Compiler Error
  Ref<const int> cari4(i);

  cout << cari << " " << cari2 << " " << cari4 << endl;

  // Test assignment operators
  Ref<int> ctri(i2);
  //cri = ctri; // -> Compiler Error
  cout << cri << " " << ctri << endl;
  //cri = 31;   // -> Compiler Error
  cout << cri << " " << ctri << endl;
  //cri2 = i;   // -> Compiler Error
  //cri2 = cri; // -> Compiler Error

  // Test operator&()
  //int *cpi = &cri;  // -> Compiler Error
  const int *cpi2 = &cri;
  //int *cpj = &cri2; // -> Compiler Error
  const int *cpj2 = &ri2;

  cout << *cpi2 << " " << *cpj2 << endl;

  // Test operator T &()
  //int &cii = cri;  // -> Compiler error
  const int &cii2 = cri;
  //int &cij = cri2; // -> Compiler error
  const int &cij2 = ri2;

  cout << cii2 << " " << cij2 << endl;
  /*** END Test const Ref<T> and const Ref<const T> *********/

  /*** BEGIN Test Ref<T> & and const Ref<T> & ***************/
  Ref<int> &rri = ri;
  const Ref<int> &crri = cri;

  //Ref<int> &rri2 = i;        // -> Compiler Error
  //const Ref<int> &crri2 = j; // -> Compiler Error

  cout << rri << " " << crri << endl;

  cout << rri << " " << ri << endl;
  rri++;
  cout << rri << " " << ri << endl;
  /*** END Test Ref<T> & and const Ref<T> & *****************/

  /*** BEGIN Test Reassign **********************************/
  ReRef<const int> reri(i);
  cout << reri << endl;
  reri.Reassign(j);
  cout << reri << endl;
  /*** END Test Reassign ************************************/

  /*** BEGIN Manipulation Tests *****************************/
  int number = 25;
  int &iRef = number;

  Ref<int> rRef = number;
  Ref<int> rRef1 = iRef;
  int &iRef3 = iRef;

  ReRef<int> rrRef = number;
  ReRef<int> rrRef1 = iRef;
  ReRef<int> rrRef2(rRef);
  ReRef<int> rrRef3 = rRef;

  Ref<int> rRef2 = (int &)rrRef;
  Ref<int> rRef3 = rrRef;

  int &iRef1 = rRef;
  int &iRef2 = rrRef;

  cout << "Number: " << number << endl;

  cout << "*(&iRef)  = " << *(&iRef) << endl;
  cout << "*(&rRef)  = " << *(&rRef) << endl;
  cout << "*(&rrRef) = " << *(&rrRef) << endl;

  cout << "&:     " << iRef  << " " << iRef1  << " ";
  cout << iRef2  << " " << iRef3  <<  endl;
  cout << "Ref:   " << rRef  << " " << rRef1  << " ";
  cout << rRef2  << " " << rRef3  << endl;
  cout << "ReRef: " << rrRef << " " << rrRef1 << " ";
  cout << rrRef2 << " " << rrRef3 << endl;

  cout << endl;

  int number2 = 37;
  rrRef2.Reassign(number2);

  cout << "Changing reference: " << number2 << " " << rrRef2 << endl;
  rrRef2++;
  cout << "Changing reference: " << number2 << " " << rrRef2 << endl;
  number2++;
  cout << "Changing reference: " << number2 << " " << rrRef2 << endl;

  cout << endl;

  cout << "Manipulate references:" << endl;
  cout << number << " " << iRef << " " << rRef << " " << rrRef << endl;
  number++;
  cout << number << " " << iRef << " " << rRef << " " << rrRef << endl;
  iRef++;
  cout << number << " " << iRef << " " << rRef << " " << rrRef << endl;
  rRef++;
  cout << number << " " << iRef << " " << rRef << " " << rrRef << endl;
  rrRef--;
  cout << number << " " << iRef << " " << rRef << " " << rrRef << endl;

  rrRef = 3;
  cout << number << " " << iRef << " " << rRef << " " << rrRef << endl;
  rRef = rrRef + 45;
  cout << number << " " << iRef << " " << rRef << " " << rrRef << endl;
  rRef = 45 + iRef;
  cout << number << " " << iRef << " " << rRef << " " << rrRef << endl;
  iRef = rRef;
  cout << number << " " << iRef << " " << rRef << " " << rrRef << endl;
  number = rrRef + rRef + iRef + 100;
  cout << number << " " << iRef << " " << rRef << " " << rrRef << endl;

  cout << endl;

  cout << "int n1 = 31;" << endl;
  cout << "int n2 = 51;" << endl;
  cout << "Ref<int> r = n1;" << endl;
  cout << "ReRef<int> rr = n2;" << endl;

  int n1 = 31;
  int n2 = 51;
  Ref<int> r = n1;
  ReRef<int> rr = n2;

  cout << r << " " << rr << endl;

  cout << "r = rr;" << endl;
  r = rr;

  cout << r << " " << rr << endl;

  cout << "rr++;" << endl;
  rr++;

  cout << r << " " << rr << endl;

  cout << endl;
  /*** END Manipulation Tests *******************************/

  /*** BEGIN TestRefAddress *********************************/
  TestRefAddress();
  /*** END TestRefAddress ***********************************/

  return 0;
}

void TestRefAddress(void)
{
  int i = 31;
  Ref<int> ri(i);

  cout << "Begin Test Ref Address" << endl;

  cout << hex << &i << " " << &ri << endl;

  Ref<int> *pri = ri.Address();
  cout << hex << pri << endl;
  cout << "Test addressof()  " << addressof(ri) << endl;

  cout << "Before TRA1(Ref<T> r) : r = " << ri << endl;
  TRA1(ri);
  cout << "After TRA1(Ref<T> r) : r = " << ri << endl;

  cout << endl;

  cout << "Before TRA2(Ref<T> &r) : r = " << ri << endl;
  TRA2(ri);
  cout << "After TRA2(Ref<T> &r) : r = " << ri << endl;

  cout << endl;

  cout << "Before TRA3(Ref<T> *r) : r = " << ri << endl;
  TRA3(ri.Address());
  cout << "After TRA3(Ref<T> *r) : r = " << ri << endl;

  cout << endl;

  cout << "Before TRA4(int &r) : r = " << ri << endl;
  TRA4(ri);
  cout << "After TRA4(int &r) : r = " << ri << endl;

  cout << endl;

  cout << "Before TRA5(int r) : r = " << ri << endl;
  TRA5(ri);
  cout << "After TRA5(int r) : r = " << ri << endl;

  cout << endl;

  cout << "End Test Ref Address" << endl;

  return;
}

template <typename T> void TRA1(Ref<T> r)
{
  cout << "TRA(Ref<T> r) : r = " << r << endl;
  cout << "Address of r("<<r.Address()<<") and data("<<&r<<")"<< endl;
  r++;
  cout << "r++ : r = " << r << endl;
  return;
}

template <typename T> void TRA2(Ref<T> &r)
{
  cout << "TRA(Ref<T> &r) : r = " << r << endl;
  cout << "Address of r("<<r.Address()<<") and data("<<&r<<")"<< endl;
  r++;
  cout << "r++ : r = " << r << endl;
  return;
}

template <typename T> void TRA3(Ref<T> *r)
{
  cout << "TRA(Ref<T> *r) : r = " << *r << endl;
  cout << "Address of r("<<r->Address()<<") and data("<<&(*r)<<")"<< endl;
  (*r)++;
  cout << "r++ : r = " << *r << endl;
  return;
}

void TRA4(int &r)
{
  cout << "TRA(int &r) : r = " << r << endl;
  cout << "Address of r("<<&r<<") and data("<<&r<<")"<< endl;
  r++;
  cout << "r++ : r = " << r << endl;
  return;
}

void TRA5(int r)
{
  cout << "TRA5(int r) : r = " << r << endl;
  cout << "Address of r("<<&r<<") and data("<<&r<<")"<< endl;
  r++;
  cout << "r++ : r = " << r << endl;
  return;
}
