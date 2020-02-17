#ifndef __MANAGED_FACTORY_H__
#define __MANAGED_FACTORY_H__

#include <cstdlib>
#include "HierarchyGenerators.h"

template <class T> class BaseListUnit {
public:
  BaseListUnit(void) { inUse = false; return; }
  ~BaseListUnit(void) { return; }

  BaseListUnit<T> *Requisition(void) {
    if(previous != NULL) { previous->next = next; }
    if(next != NULL) { next->previous = previous; }
    return this;
  }

  bool inUse;
  T *data;
  BaseListUnit *previous, *next;
};

template <class T> class BaseBlock {
public:
  BaseBlock(int size) {
    next = NULL;
    if(size <= 0) { d_size = 0; return; }

    d_size = size;
    d_block = (T *)malloc(size*sizeof(T));
    d_baseHolders = new BaseListUnit<T>[size];
    int i = 0;
    d_baseHolders[i].data = &d_block[i];
    d_baseHolders[i].previous = NULL;
    for(; i<size-1; i++) {
      d_baseHolders[i].next = &d_baseHolders[i+1];
      d_baseHolders[i+1].previous = &d_baseHolders[i];
      d_baseHolders[i+1].data = &d_block[i+1];
    }
    d_baseHolders[i].next = NULL;

    return;
  }
  ~BaseBlock(void) {
    if(d_baseHolders != NULL) { delete [] d_baseHolders; }
    if(d_block != NULL) { free(d_block); }
    if(next != NULL) { delete next; }
    return;
  }

  BaseListUnit<T> *GetFirstAvailable(void) {
    for(int i=0; i<d_size; i++) {
      if(!d_baseHolders[i].inUse) {
	return &d_baseHolders[i];
      }
    }
    return NULL;
  }

  BaseBlock<T> *next;

private:
  int d_size;
  BaseListUnit<T> *d_baseHolders;
  T *d_block;
};

template <class T> class Base {
public:
  Base(void) {
    d_allocated = d_lastBlock = NULL;
    d_available = d_used = NULL;
    d_blockSize = 100;
    return;
  }
  virtual ~Base(void) {
    if(d_allocated != NULL) { delete d_allocated; }
    return;
  }

  T *Allocate(Loki::Type2Type<T> type2type) {
    if(d_available == NULL) {
      BaseBlock<T> *newBlock = new BaseBlock<T>(d_blockSize);
      if(d_allocated == NULL) {
	d_allocated = newBlock;
	d_lastBlock = newBlock;
      } else {
	d_lastBlock->next = newBlock;
      }
      d_available = newBlock->GetFirstAvailable();
    }
    BaseListUnit<T> *unit = d_available->Requisition();
    d_available = unit->next;
    T *data = unit->data;
    unit->data = NULL;
    unit->previous = NULL;
    unit->next = d_used;
    d_used = unit;

    return data;
  }
  void Release(T *data, Loki::Type2Type<T> type2type) {
    if(d_used == NULL) {
      return;
    }
    BaseListUnit<T> *unit = d_used->Requisition();
    d_used = unit->next;
    unit->data = data;
    unit->previous = NULL;
    unit->next = NULL;
    if(d_available == NULL) {
      d_available = unit;
    } else {
      unit->next = d_available;
      if(d_available->previous != NULL) {
	unit->previous = d_available->previous;
	d_available->previous->next = unit;
      }
      d_available->previous = unit;
    }
    d_available = unit;

    return;
  }

private:
  int d_blockSize;

  BaseBlock<T> *d_allocated, *d_lastBlock;
  BaseListUnit<T> *d_available, *d_used;
};

template <class TList, template <class> class Unit = Base>
class ManagedFactory : public Loki::GenScatterHierarchy<TList, Unit> {
public:
  typedef TList ProductList;
  template <class T> T *Allocate(void) {
    Unit<T> &unit = *this;
    return unit.Allocate(Loki::Type2Type<T>());
  }
  template <class T> void Release(T *data) {
    Unit<T> &unit = *this;
    unit.Release(data, Loki::Type2Type<T>());
    return;
  }
};

#endif
