#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include <iostream>
#include <cstdlib>
#include "stack.h"
#include "queue.h"
#include "list.h"

template <class T> class LinkedList 
: public Stack<T>, public Queue<T>, public List<T>
{
public:
  LinkedList(void);
  LinkedList(T *data) { Push(data); return; }
  virtual ~LinkedList(void);

  virtual void Insert(T *);
  virtual void Push(T *);
  virtual void Append(T *);
  virtual void InsertBefore(T *);
  virtual void InsertAfter(T *);

  virtual T *Remove(void);
  virtual T *Remove(T *);
  virtual T *Pop(void);
  virtual T *Chop(void);

  virtual T *GetCurrentData(void);
  virtual T *Next(void);
  virtual T *NextLoop(void);
  virtual T *Previous(void);
  virtual T *PreviousLoop(void);
  virtual T *Find(T *toFind);

  virtual void Reset(void);
  virtual bool IsEmpty(void);
  virtual int Size(void);
  virtual bool AtEnd(void);
  virtual void Clear(void);

  virtual T *operator[](int index);

private:
  class Node {
  public:
    Node(void) { next=previous=NULL; }
    ~Node(void) {
      previous->next = next;
      next->previous = previous;
    }
    T *data;
    Node *next;
    Node *previous;
  };

  Node *listStart;
  Node *listEnd;
  Node *currentNode;
  int currentNodeIndex;
  int size;
};

template <class T> LinkedList<T>::LinkedList(void) {
  listStart = listEnd = currentNode = NULL;
  size = 0;
  currentNodeIndex = 0;

  return;
}

template <class T> LinkedList<T>::~LinkedList(void) {
  Clear();

  return;
}

template <class T> void LinkedList<T>::Insert(T *data) {
  if(data == NULL) { return; }
  Node *newNode = new Node();
  newNode->data = data;

  if(listStart == NULL) {
    listStart = newNode;
    listStart->next = listStart->previous = listStart;
    currentNode = newNode;
    currentNodeIndex = 0;
    listEnd = newNode;
  } else {
    newNode->next = currentNode;
    newNode->previous = currentNode->previous;
    currentNode->previous->next = newNode;
    currentNode->previous = newNode;
    currentNode = newNode;
    // No need to change currentNodeIndex because the new node was 
    // inserted before currentNode taking its index, then currentNode
    // is moved back one to equal newNode which has the original index
  }

  size++;

  return;
}

template <class T> void LinkedList<T>::Push(T *data) {
  if(data == NULL) { return; }
  currentNode = listStart;
  Insert(data);
  listStart = currentNode;
  currentNode = listStart;
  currentNodeIndex = 0;

  return;
}

template <class T> void LinkedList<T>::Append(T *data) {
  if(data == NULL) { return; }
  currentNode = listStart;
  Insert(data);
  listEnd = currentNode;
  currentNodeIndex = size - 1;

  return;
}

template <class T> void LinkedList<T>::InsertBefore(T *data) {
  if(data == NULL) { return; }
  Insert(data);
  if(currentNode == listStart) {
    listStart = currentNode;
  }

  return;
}

template <class T> void LinkedList<T>::InsertAfter(T *data) {
  if(data == NULL) { return; }
  if(currentNode != NULL) {
    currentNode = currentNode->next;
    currentNodeIndex = (currentNodeIndex+1)%size;
  }
  Insert(data);
  if(currentNode->next == listStart) {
    listEnd = currentNode;
  }

  return;
}

template <class T> T *LinkedList<T>::Remove(T *data) {
  if(data == NULL) { return NULL; }
  Find(data);
  return Remove();
}

template <class T> T *LinkedList<T>::Remove(void) {
  T *returnValue = NULL;

  int which = 0;
  if(currentNode == NULL) {
    return returnValue;
  } else if(currentNode == listStart) {
    which = 1;
  } else if(currentNode == listEnd) {
    which = 2;
  }

  returnValue = currentNode->data;
  currentNode->data = NULL;

  currentNode->next->previous = currentNode->previous;
  currentNode->previous->next = currentNode->next;

  Node *temp = currentNode;
  if(currentNode->next == currentNode) {
    currentNode = NULL;
    currentNodeIndex = 0;
    listStart = listEnd = NULL;
  } else {
    currentNode = currentNode->next;
    // Don't need to change the currentNodeIndex because although it moves
    // ahead one, the previous one in the list is removed putting the
    // index back where it started.
  }
  delete temp;

  if(which == 1) {
    listStart = currentNode;
  } else if(which == 2) {
    currentNode = currentNode->previous;
    currentNodeIndex--;
    if(currentNodeIndex < 0) {
      currentNodeIndex = size - 1;
    }
    listEnd = currentNode;
  }

  size--;

  return returnValue;
}

template <class T> T *LinkedList<T>::Pop(void) {
  currentNode = listStart;
  currentNodeIndex = 0;
  return Remove();
}

template <class T> T *LinkedList<T>::Chop(void) {
  currentNode = listEnd;
  currentNodeIndex = size - 1;
  return Remove();
}

template <class T> T *LinkedList<T>::GetCurrentData(void) {
  if(currentNode == NULL || listStart == NULL) {
    return NULL;
  }

  return currentNode->data;
}

template <class T> T *LinkedList<T>::Next(void) {
  if(currentNode == NULL || listStart == NULL) {
    return NULL;
  }

  currentNode = currentNode->next;
  currentNodeIndex++;

  if(currentNode == listStart) {
    currentNode = listEnd;
    currentNodeIndex = size - 1;
    return NULL;
  }

  return currentNode->data;
}

template <class T> T *LinkedList<T>::NextLoop(void) {
  if(currentNode == NULL || listStart == NULL) {
    return NULL;
  }

  currentNode = currentNode->next;
  currentNodeIndex = (currentNodeIndex+1)%size;

  return currentNode->data;
}

template <class T> T *LinkedList<T>::Previous(void) {
  if(currentNode == NULL || listStart == NULL) {
    return NULL;
  }

  currentNode = currentNode->previous;
  currentNodeIndex--;

  if(currentNode == listEnd) {
    currentNode = listStart;
    currentNodeIndex = 0;
    return NULL;
  }

  return currentNode->data;
}

template <class T> T *LinkedList<T>::PreviousLoop(void) {
  if(currentNode == NULL || listStart == NULL) {
    return NULL;
  }

  currentNode = currentNode->previous;
  currentNodeIndex--;
  if(currentNodeIndex < 0) {
    currentNodeIndex = size - 1;
  }

  return currentNode->data;
}

template <class T> void LinkedList<T>::Reset(void) {
  currentNode = listStart;
  currentNodeIndex = 0;

  return;
}

template <class T> bool LinkedList<T>::IsEmpty(void) {
  if(size == 0) {
    return true;
  } else {
    return false;
  }
}

template <class T> int LinkedList<T>::Size(void) {
  return size;
}

template <class T> bool LinkedList<T>::AtEnd(void) {
  if(currentNode == listEnd) {
    return true;
  } else {
    return false;
  }
}

template <class T> T *LinkedList<T>::operator[](int index) {
  if(size == 0 || listStart == NULL) {
    return NULL;
  }

  int offset = 0;
  int findDirection = 1;
  int findIndex = index;
  if(index < 0) {
    findIndex *= -1;
    findIndex = findIndex % size;
    findIndex = size - findIndex;
  } else {
    findIndex = findIndex % size;
  }

  int fromStart = findIndex;
  int fromEnd = size - findIndex - 1;
  int fromCurrent = currentNodeIndex - findIndex;
  int current = fromCurrent;
  if(fromCurrent < 0) {
    fromCurrent *= -1;
  }
  if(fromStart < fromEnd && fromStart < fromCurrent) {
    findDirection = 1;
    currentNode = listStart;
    currentNodeIndex = 0;
    offset = fromStart;
  } else if(fromEnd < fromStart && fromEnd < fromCurrent) {
    findDirection = -1;
    currentNode = listEnd;
    currentNodeIndex = size - 1;
    offset = fromEnd;
  } else {
    if(current < 0) {
      findDirection = 1;
    } else {
      findDirection = -1;
    }
    offset = fromCurrent;
  }

  for(int i=0; i<offset; i++) {
    if(findDirection > 0) {
      currentNode = currentNode->next;
      currentNodeIndex++;
    } else {
      currentNode = currentNode->previous;
      currentNodeIndex--;
    }
  }

  return currentNode->data;
}

template <class T> T *LinkedList<T>::Find(T *toFind) {
  if(listStart == NULL) {
    return NULL;
  }
  if(currentNode != NULL && currentNode->data == toFind) {
    return currentNode->data;
  }

  Node *find = currentNode;
  int index = currentNodeIndex;
  currentNode = listStart;
  currentNodeIndex = 0;
  while(currentNode->data != toFind && currentNodeIndex < size) {
    currentNodeIndex++;
    currentNode = currentNode->next;
  }

  if(currentNodeIndex == size) {
    currentNode = find;
    currentNodeIndex = index;
    return NULL;
  }

  return currentNode->data;
}

template <class T> void LinkedList<T>::Clear(void) {
  while(!IsEmpty()) { Pop(); }

  return;
}

#endif
