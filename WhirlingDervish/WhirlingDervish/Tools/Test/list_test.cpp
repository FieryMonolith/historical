#include "../Interface/list.h"
#include <stdlib.h>
#include <stdio.h>

class a {
public:
  a() { x = y = 0; }
  int value() { return (x+y); }
  int x, y;
};

int main(void) {
  printf("Begin test of List class\n");

  printf("Pushing...\n");
  List<int> list;
  list.Push(10);
  list.Push(5);
  list.Push(21);
  list.Push(25);

  printf("Using next...\n");
  printf("Reset to beginning of list.\n");
  printf("List values:");
  list.Reset();
  do {
    int value = list.GetCurrentData();
    printf(" %d", value);
  } while(list.Next() != 0);
  printf("\n");

  printf("\nUsing next with looping...\n");
  printf("Reset to beginning of list.\n");
  printf("List values:");
  list.Reset();
  int b = 0;
  do {
    int value = list.GetCurrentData();
    printf(" %d", value);
    b++;
  } while(list.NextLoop() != 0 && b < 8);
  printf("\n");

  List<a*> *classList;
  for(int z=0; z<3; z++) {
    if(z == 0) {
      printf("\nInsert Before\n");
    } else if(z == 1) {
      printf("\nInsert After\n");
    } else {
      printf("\nAppend\n");
    }

    classList = new List<a*>();
    for(int j=0; j<6; j++) {
      a *temp = new a();
      temp->x = j*j;
      temp->y = (j+1)*(j+1);
      if(z == 0) {
	classList->InsertBefore(temp);
      } else if(z == 1) {
	classList->InsertAfter(temp);
      } else {
	classList->Append(temp);
      }
    }

    printf("Values for class:");
    classList->Reset();
    for(int j=0; j<6; j++) {
      a* temp = classList->GetCurrentData();
      printf(" %d", temp->value());
      classList->Next();
    }
    printf("\n");

    printf("Size is %d\n", classList->Size());

    if(z == 0) {
      printf("Expected values for popping: 1 1 1 1 1 1\n");
      printf("Values while popping:       ");
    } else if(z == 1) {
      printf("Expected values for chopping: 5 13 25 41 61 1\n");
      printf("Values while chopping:       ");
    } else {
      printf("Expected values for Removing: 61 41 25 13 5 1\n");
      printf("Values while Removing:       ");
    }

    while(classList->Size()) {
      a* temp = classList->GetCurrentData();
      printf(" %d", temp->value());

      a* toRemove;
      if(z == 0) {
	toRemove = classList->Pop();
      } else if(z == 1) {
	toRemove = classList->Chop();
      } else {
	toRemove = classList->Remove();
      }
      if(toRemove != 0) {
	delete toRemove;
      }
    }
    printf("\n");
    delete classList;
  }

  printf("End\n");
  return 1;
}
