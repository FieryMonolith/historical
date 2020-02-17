#include <iostream>
#include "../Interface/jint64dict.h"

using namespace std;

int main(void) {
  cout << "Start" << endl;

  int b = 4;
  JInt64Dict<int> a;
  cout << "Is a empty? " << a.isEmpty() << endl;
  a.insert(3, &b);
  cout << "a has " << a.count() << " data members in an array of size ";
  cout << a.size() << endl;
  int *c = a.find(3);
  cout << *c << " found at key 3" << endl;
  c = a.find(1);
  if(c == NULL) {
    cout << "Nothing found at key 1" << endl;
  } else {
    cout << *c << " found at key 1" << endl;
  }

  cout << "End" << endl;

  return 0;
}
