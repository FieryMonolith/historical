#ifndef __MAP2_H__
#define __MAP2_H__

/*

 Notes:
 1. If either/both Allocate and Copy are NULL, then even thought the class 
    was created, it can not be used until it is set.
 2. Allocate and Copy are both either static member functions or regular 
    global functions.
 3. Allocate should return a allocated, default object.
 4. Copy(DATA **copyTo, const DATA *copyFrom)
    a. If copyFrom is NULL, this function does nothing.
    b. If copyTo is NULL, this function is required to allocate a new object 
       who has the same data as copyFrom
    c. If copyTo is not NULL, then the values need to somehow be copied into 
       copyTo from copyFrom.
 5. Behavior is undefined if the above criteria are not met.

 6. This specific map class does internal indexing and doesn't allow user 
    indexing.
 7. The comments are how a user should interpret the methods, not necessarily 
    how those methods are implemented.

 */

#include <qmutex.h>
#include "tArray.h"

template <class DATA> class Map2;

// iMap class is the interface for a map using ids. //////////////////

template <class DATA> class iMap {
public:
  iMap(void) { return; }

  virtual ~iMap(void) { return; }

  // Add makes a copy of the data and returns an id to it.
  virtual        int  Add(const DATA &data) = 0;
  // Returns a copy of the data
  virtual       DATA *Remove(int id)        = 0;
  virtual       DATA *Copy(int id)          = 0;
  virtual       DATA &Modify(int id)        = 0;
  virtual const DATA &Access(int id)        = 0;
  virtual        int  Size(void)            = 0;
};

// END iMap class is the interface for a map using ids. //////////////

// Iterators /////////////////////////////////////////////////////////

template <class DATA> class Map2Iterator {
public:
  Map2Iterator(Map2<DATA> *map);
  virtual ~Map2Iterator(void);

  void Begin(void);
  bool IsBeginning(void);
  void End(void);
  bool IsEnd(void);

  void Next(void);
  void NextLoop(void);
  void Previous(void);
  void PreviousLoop(void);

  void Goto(int id);
  int Id(void);

protected:
  Map2<DATA> *d_map;
  int d_currentIndex;
};

template <class DATA> class Map2IteratorAccess : public Map2Iterator<DATA> {
public:
  Map2IteratorAccess(Map2<DATA> *map) : Map2Iterator<DATA>(map) { return; }
  ~Map2IteratorAccess(void) { return; }

  const DATA &Data(void);
  const DATA &CheckOut(void);
        void  CheckIn(void);
};

template <class DATA> class Map2IteratorModify : public Map2Iterator<DATA> {
public:
  Map2IteratorModify(Map2<DATA> *map) : Map2Iterator<DATA>(map) { return; }
  ~Map2IteratorModify(void) { return; }

  DATA &Data(void);
  DATA &CheckOut(void);
  void  CheckIn(void);
};

// END Iterators //////////////////////////////////////////////////////

// Map2 ///////////////////////////////////////////////////////////////

template <class DATA> class Map2 : public iMap<DATA> {
public:
  Map2(DATA *(*Allocate)(), void (*Copy)(DATA **, const DATA *));
  Map2(Map2 &);
  ~Map2(void);

  void SetAllocator(DATA *(*Allocate)());
  void SetCopy(void (*Copy)(DATA **, const DATA *));

  Map2IteratorAccess<DATA> *AccessMap(void);
  Map2IteratorModify<DATA> *ModifyMap(void);

  // Add makes a copy of the data and returns an id to it.
         int  Add(const DATA &data) ;
  // Returns a copy of the data
        DATA *Remove(int id);
        DATA *Copy(int id);
        DATA &Modify(int id);
  const DATA &Access(int id);
         int  Size(void);

  QMutex d_access;

private:
  class DataStorage {
  public:
    // No initialization due to algorithm design.
    DataStorage(void) { data=NULL; index=-1; return; }
    ~DataStorage(void) { data=NULL; index=-1; return; }

    DATA *data;
    int index;
  };
  class IndexStorage {
  public:
    IndexStorage(void) { dataIndex=-1; return; }
    ~IndexStorage(void) { dataIndex=-1; return; }

    int dataIndex;
  };

  int BLOCK_SIZE;

  DATA *(*AllocateFunction)();
  void (*CopyFunction)(DATA **, const DATA *);

  tArray<DataStorage> *d_data;
  tArray<IndexStorage> *d_indices;
  int d_currentOpening;
  int d_max;
  int d_smallestOpenId;

  friend class Map2IteratorAccess<DATA>;
  friend class Map2IteratorModify<DATA>;
  friend class Map2Iterator<DATA>;

  void FindOpenIndex(void);
};

// END Map2 ////////////////////////////////////////////////////////////

// IMPLEMENTATION //////////////////////////////////////////////////////

// Map2 //////////////////////////
template <class DATA> Map2<DATA>::Map2(DATA *(*Allocate)(), 
				       void (*Copy)(DATA **, const DATA *)) {
  BLOCK_SIZE = 100;
  d_max = BLOCK_SIZE;
  d_currentOpening = 0;
  d_smallestOpenId = 0;

  d_data = new tArray<DataStorage>(d_max);
  d_indices = new tArray<IndexStorage>(d_max);

  CopyFunction = Copy;
  AllocateFunction = Allocate;

  return;
}

template <class DATA> Map2<DATA>::Map2(Map2 &map) {
  BLOCK_SIZE = map.BLOCK_SIZE;

  AllocateFunction = map.AllocateFunction;
  CopyFunction = map.CopyFunction;
  d_max = map.d_max;
  d_currentOpening = map.d_currentOpening;
  d_data = new tArray<DataStorage>(*(map.d_data));
  d_indices = new tArray<IndexStorage>(*(map.d_indices));

  return;
}

template <class DATA> Map2<DATA>::~Map2(void) {
  d_access.unlock();
  CopyFunction = NULL;
  AllocateFunction = NULL;

  for(int i=0; i<d_data->GetSize(); i++) {
    if((*d_data)[i].data != NULL) {
      delete (*d_data)[i].data;
      (*d_data)[i].data = NULL;
    }
  }

  delete d_data;
  delete d_indices;

  return;
}

template <class DATA> int Map2<DATA>::Add(const DATA &data) {
  if(CopyFunction == NULL) {
    return -1;
  }

  if(d_currentOpening + 1 >= d_max) {
    d_max += BLOCK_SIZE;
    d_data->Resize(d_max);
    d_indices->Resize(d_max);
  }

  int index = d_smallestOpenId;
  CopyFunction(&((*d_data)[d_currentOpening].data), &data);
  (*d_data)[d_currentOpening].index = index;
  (*d_indices)[index].dataIndex = d_currentOpening;

  // Mark next open id;
  FindOpenIndex();
  d_currentOpening++;

  return index;
}

template <class DATA> DATA *Map2<DATA>::Remove(int id) {
  if(CopyFunction == NULL || id >= d_max || d_currentOpening <= 0) {
    return NULL;
  }

  int removeDataIndex = (*d_indices)[id].dataIndex;
  if(removeDataIndex == -1) {
    return NULL;
  }

  d_currentOpening--;

  if(id < d_smallestOpenId) {
    d_smallestOpenId = id;
  }

  DATA *data = (*d_data)[removeDataIndex].data;
  DATA *swapData = (*d_data)[d_currentOpening].data;
  int swapId = (*d_data)[d_currentOpening].index;

  (*d_data)[removeDataIndex].index = swapId;
  (*d_data)[removeDataIndex].data = swapData;
  (*d_data)[d_currentOpening].data = NULL;

  (*d_indices)[swapId].dataIndex = removeDataIndex;
  (*d_indices)[id].dataIndex = -1;

  return data;
}

template <class DATA> DATA *Map2<DATA>::Copy(int id) {
  if(CopyFunction == NULL || id >= d_max) {
    return NULL;
  }

  int dataIndex = (*d_indices)[id].dataIndex;
  if(dataIndex == -1) {
    return NULL;
  }

  DATA *data = NULL;
  CopyFunction(&data, (*d_data)[dataIndex].data);

  return data;
}

template <class DATA> DATA & Map2<DATA>::Modify(int id) {
  if(id >= d_max) {
    //return NULL;
  }
  return *((*d_data)[(*d_indices)[id].dataIndex].data);
}

template <class DATA> const DATA & Map2<DATA>::Access(int id) {
  if(id >= d_max) {
    //return NULL;
  }
  return *((*d_data)[(*d_indices)[id].dataIndex].data);
}

template <class DATA> int Map2<DATA>::Size(void) {
  return d_currentOpening;
}

template <class DATA> void Map2<DATA>::SetAllocator(DATA *(*Allocate)()) {
  if(Allocate == NULL) {
    return;
  }

  AllocateFunction = Allocate;
  
  return;
}

template <class DATA> void Map2<DATA>::SetCopy(void (*Copy)(DATA **, 
							    const DATA *)) {
  if(Copy == NULL) {
    return;
  }

  CopyFunction = Copy;

  return;
}

template <class DATA> Map2IteratorAccess<DATA> * Map2<DATA>::AccessMap(void) {
  Map2IteratorAccess<DATA> *access = new Map2IteratorAccess<DATA>(this);

  return access;
}

template <class DATA> Map2IteratorModify<DATA> * Map2<DATA>::ModifyMap(void) {
  Map2IteratorModify<DATA> *modify = new Map2IteratorModify<DATA>(this);

  return modify;
}

template <class DATA> void Map2<DATA>::FindOpenIndex(void) {
  for(int i=d_smallestOpenId+1; i<d_max; i++) {
    if((*d_indices)[i].dataIndex == -1) {
      d_smallestOpenId = i;
      return;
    }
  }

  // This is an error, should never be full!!!!
  d_smallestOpenId = d_max + 1;

  return;
}

// END Map2 ///////////////////////

// Map2Interators /////////////////

template <class DATA> Map2Iterator<DATA>::Map2Iterator(Map2<DATA> *map) {
  d_map = map;
  if(map == NULL) {
    return;
  }

  for(d_currentIndex=0; d_currentIndex<d_map->d_max; d_currentIndex++) {
    if((*d_map->d_indices)[d_currentIndex].dataIndex != -1) {
      break;
    }
  }

  if(d_currentIndex == map->d_max) {
    d_currentIndex = 0;
  }

  return;
}

template <class DATA> Map2Iterator<DATA>::~Map2Iterator(void) {
  d_map = NULL;

  return;
}

template <class DATA> void Map2Iterator<DATA>::Begin(void) {
  if(d_map == NULL || d_map->d_currentOpening <= 0) {
    return;
  }

  d_map->d_access.lock();

  for(d_currentIndex=0; d_currentIndex<d_map->d_max; d_currentIndex++) {
    if((*d_map->d_indices)[d_currentIndex].dataIndex != -1) {
      break;
    }
  }

  if(d_currentIndex == d_map->d_max) {
    d_currentIndex = 0;
  }

  d_map->d_access.unlock();

  return;
}

template <class DATA> bool Map2Iterator<DATA>::IsBeginning(void) {
  if(d_map == NULL || d_map->d_currentOpening <= 0) {
    return true;
  }

  d_map->d_access.lock();

  int index = 0;
  for(index=0; index<d_map->d_max; index++) {
    if((*d_map->d_indices)[index].dataIndex != -1) {
      break;
    }
  }

  if(index == d_currentIndex || index == d_map->d_max) {
    d_map->d_access.unlock();
    return true;
  }

  d_map->d_access.unlock();

  return false;
}

template <class DATA> void Map2Iterator<DATA>::End(void) {
  if(d_map == NULL || d_map->d_currentOpening <= 0) {
    return;
  }

  d_map->d_access.lock();

  for(d_currentIndex=d_map->d_max-1; d_currentIndex>=0; d_currentIndex--) {
    if((*d_map->d_indices)[d_currentIndex].dataIndex != -1) {
      break;
    }
  }

  if(d_currentIndex < 0) {
    d_currentIndex = 0;
  }

  d_map->d_access.unlock();

  return;
}

template <class DATA> bool Map2Iterator<DATA>::IsEnd(void) {
  if(d_map == NULL || d_map->d_currentOpening <= 0) {
    return true;
  }

  d_map->d_access.lock();

  int index = 0;
  for(index=d_map->d_max-1; index>=0; index--) {
    if((*d_map->d_indices)[index].dataIndex != -1) {
      break;
    }
  }

  if(index == d_currentIndex || index < 0) {
    d_map->d_access.unlock();
    return true;
  }

  d_map->d_access.unlock();

  return false;
}

template <class DATA> void Map2Iterator<DATA>::Next(void) {
  if(d_map == NULL || d_map->d_currentOpening <= 0) {
    return;
  }

  d_map->d_access.lock();

  int index = d_currentIndex;

  for(d_currentIndex++; d_currentIndex<d_map->d_max; d_currentIndex++) {
    if((*d_map->d_indices)[d_currentIndex].dataIndex != -1) {
      break;
    }
  }

  if(d_currentIndex == d_map->d_max) {
    d_currentIndex = index;
  }

  d_map->d_access.unlock();

  return;
}

template <class DATA> void Map2Iterator<DATA>::NextLoop(void) {
  if(d_map == NULL || d_map->d_currentOpening <= 0) {
    return;
  }

  d_map->d_access.lock();

  int index = d_currentIndex;

  for(d_currentIndex++; d_currentIndex<d_map->d_max; d_currentIndex++) {
    if((*d_map->d_indices)[d_currentIndex].dataIndex != -1) {
      break;
    }
  }

  if(d_currentIndex != d_map->d_max) {
    d_map->d_access.unlock();
    return;
  }

  for(d_currentIndex=0; d_currentIndex<index; d_currentIndex++) {
    if((*d_map->d_indices)[d_currentIndex].dataIndex != -1) {
      break;
    }
  }

  d_map->d_access.unlock();

  return;
}

template <class DATA> void Map2Iterator<DATA>::Previous(void) {
  if(d_map == NULL || d_map->d_currentOpening <= 0) {
    return;
  }

  d_map->d_access.lock();

  int index = d_currentIndex;

  for(d_currentIndex--; d_currentIndex>=0; d_currentIndex--) {
    if((*d_map->d_indices)[d_currentIndex].dataIndex != -1) {
      break;
    }
  }

  if(d_currentIndex < 0) {
    d_currentIndex = index;
  }

  d_map->d_access.unlock();

  return;
}

template <class DATA> void Map2Iterator<DATA>::PreviousLoop(void) {
  if(d_map == NULL || d_map->d_currentOpening <= 0) {
    return;
  }

  d_map->d_access.lock();

  int index = d_currentIndex;

  for(d_currentIndex--; d_currentIndex>=0; d_currentIndex--) {
    if((*d_map->d_indices)[d_currentIndex].dataIndex != -1) {
      break;
    }
  }

  if(d_currentIndex >= 0) {
    d_map->d_access.unlock();
    return;
  }

  for(d_currentIndex=d_map->d_max-1; d_currentIndex>index; d_currentIndex--) {
    if((*d_map->d_indices)[d_currentIndex].dataIndex != -1) {
      break;
    }
  }

  d_map->d_access.unlock();

  return;
}

template <class DATA> void Map2Iterator<DATA>::Goto(int id) {
  if(d_map == NULL || d_map->d_currentOpening <= 0 || id >= d_map->d_max) {
    return;
  }

  d_map->d_access.lock();

  if((*d_map->d_indices)[id].dataIndex != -1) {
    d_currentIndex = id;
  }

  d_map->d_access.unlock();

  return;
}

template <class DATA> int Map2Iterator<DATA>::Id(void) {
  if(d_map == NULL || d_map->d_currentOpening <= 0) {
    return -1;
  }

  d_map->d_access.lock();

  if((*d_map->d_indices)[d_currentIndex].dataIndex == -1) {
    int index = d_currentIndex;
    NextLoop();
    if(d_currentIndex == index) {
      d_map->d_access.unlock();
      return -1;
    }
  }

  int returnIndex = d_currentIndex;

  d_map->d_access.unlock();

  return returnIndex;
}

template <class DATA> const DATA & Map2IteratorAccess<DATA>::Data(void) {
  if(d_map == NULL || d_map->d_currentOpening <= 0) {
    return NULL;
  }

  d_map->d_access.lock();

  if((*d_map->d_indices)[d_currentIndex].dataIndex == -1) {
    int index = d_currentIndex;
    NextLoop();
    if(d_currentIndex == index) {
      d_map->d_access.unlock();
      return NULL;
    }
  }

  int index = (*d_map->d_indices)[d_currentIndex].dataIndex;
  DATA &data = *((*d_map->d_data)[index].data); 

  d_map->d_access.unlock();

  return data;
}

template <class DATA> const DATA & Map2IteratorAccess<DATA>::CheckOut(void) {
  if(d_map == NULL || d_map->d_currentOpening <= 0) {
    // Return null
  }

  d_map->d_access.lock();

  if((*d_map->d_indices)[d_currentIndex].dataIndex == -1) {
    int index = d_currentIndex;
    NextLoop();
    if(d_currentIndex == index) {
      d_map->d_access.unlock();
      // Return null
    }
  }

  int index = (*d_map->d_indices)[d_currentIndex].dataIndex;
  DATA &data = *((*d_map->d_data)[index].data); 

  return data;
}

template <class DATA> void Map2IteratorAccess<DATA>::CheckIn(void) {
  d_map->d_access.unlock();

  return;
}

template <class DATA> DATA & Map2IteratorModify<DATA>::Data(void) {
  if(d_map == NULL || dmap->d_currentOpening <= 0) {
    // Return null
  }

  d_map->d_access.lock();

  if((*d_map->d_indices)[d_currentIndex].dataIndex == -1) {
    int index = d_currentIndex;
    NextLoop();
    if(d_currentIndex == index) {
      d_map->d_access.unlock();
      // Return null
    }
  }

  int index = (*d_map->d_indices)[d_currentIndex].dataIndex;
  DATA &data = *((*d_map->d_data)[index].data); 

  d_map->d_access.unlock();

  return data;
}

template <class DATA> DATA & Map2IteratorModify<DATA>::CheckOut(void) {
  if(d_map == NULL) {
    // Return null
  }

  d_map->d_access.lock();

  if((*d_map->d_indices)[d_currentIndex].dataIndex == -1) {
    int index = d_currentIndex;
    NextLoop();
    if(d_currentIndex == index) {
      d_map->d_access.unlock();
      // Return null
    }
  }

  int index = (*d_map->d_indices)[d_currentIndex].dataIndex;
  DATA &data = *((*d_map->d_data)[index].data); 

  return data;
}

template <class DATA> void Map2IteratorModify<DATA>::CheckIn(void) {
  d_map->d_access.unlock();

  return;
}

// END Map2Interators /////////////

// END IMPLEMENTATION ///////////////////////////////////////////////////

#endif
