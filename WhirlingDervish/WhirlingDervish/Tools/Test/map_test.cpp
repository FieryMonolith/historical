#include <iostream.h>
#include "map.h"

int main(int argc, char **argv) {
  cout << "Start" << endl;

  Map<int, int> *test1 = new Map<int, int>();
  if(test1->IsEmpty()) {
    cout << "Test1<int, int> is empty, size is " << test1->Size() << endl;
  } else {
    cout << "Test1<int, int> is not empty, size is " << test1->Size() << endl;
  }
  test1->Insert(1, 1);
  test1->Insert(5, 3);
  test1->Insert(2, 4);
  cout << "Inserted <1,1> <5,3> <2,4>" << endl;
  if(test1->IsEmpty()) {
    cout << "Test1<int, int> is empty, size is " << test1->Size() << endl;
  } else {
    cout << "Test1<int, int> is not empty, size is " << test1->Size() << endl;
  }
  cout << "RetrieveData(1) is " << test1->RetrieveData(1) << endl;
  cout << "RetrieveData(2) is " << test1->RetrieveData(2) << endl;
  cout << "RetrieveData(5) is " << test1->RetrieveData(5) << endl;
  cout << "RetrieveKey(1) is " << test1->RetrieveKey(1) << endl;
  cout << "RetrieveKey(4) is " << test1->RetrieveKey(4) << endl;
  cout << "RetrieveKey(3) is " << test1->RetrieveKey(3) << endl;
  cout << "RetrieveData(33)(not there) is " << test1->RetrieveData(33) << endl;
  cout << "RetrieveKey(33)(not there) is " << test1->RetrieveKey(33) << endl;
  int *keys = test1->Keys();
  cout << "Keys(" << test1->Size() << ") are " << keys[0] << " ";
  cout << keys[1] << " " << keys[2] << endl;
  delete keys;
  cout << "DeleteByKey(1) is " << test1->DeleteByKey(1) << endl;
  cout << "DeleteByKey(5) is " << test1->DeleteByKey(5) << endl;
  keys = test1->Keys();
  cout << "Keys(" << test1->Size() << ") are " << keys[0] << endl;
  delete keys;
  cout << "DeleteByData(4) is " << test1->DeleteByData(4) << endl;
  if(test1->IsEmpty()) {
    cout << "Test1<int, int> is empty, size is " << test1->Size() << endl;
  } else {
    cout << "Test1<int, int> is not empty, size is " << test1->Size() << endl;
  }

  cout << "End" << endl;

  return 0;
}
