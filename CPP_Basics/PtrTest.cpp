#include <iostream>

#include "Ptr.h"

using namespace std;

int main(void)
{
  int i = 31;
  const int j = 37;

  Ptr<int> p1;
  Ptr<int> p2 = &i;
  Ptr<int> p3 = p2;

  Ptr<const int> p4;
  Ptr<const int> p5 = &i;
  Ptr<const int> p6 = p5;

  //Ptr<int> p7 = p5;       // -> Compiler Error
  //Ptr<const int> p8 = p2; // -> Compiler Error

  //Ptr<int> p9 = &j; // -> Compiler Error
  Ptr<const int> p10 = &j;

  Ptr<int> p11(&i);
  //Ptr<int> p12(&j); // -> Compiler Error
  Ptr<int> p13(p11);
  //Ptr<int> p14(p5); // -> Compiler Error
  Ptr<const int> p15(&i);
  Ptr<const int> p16(&j);
  Ptr<const int> p17(p11);
  Ptr<const int> p18(p5);

  cout << *p2 << " " << *p5 << endl;

  //long l = 41;
  //Ptr<int> p19(&l); // -> Compiler Error

  

  return 0;
}
