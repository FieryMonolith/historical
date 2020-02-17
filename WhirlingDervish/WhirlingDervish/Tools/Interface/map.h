#ifndef __MAP_H__
#define __MAP_H__

#include <cstdlib>
#include <iostream>
using namespace std;

template <class KEY, class DATA> class Map;

template <class KEY, class DATA> class MapData {
public:
  MapData(void) { next = NULL; return; }
  ~MapData(void) { return; }

  DATA *data;
  KEY key;
  MapData *next;
};

template <class KEY, class DATA> class MapIterator {
public:
  MapIterator(void) { index = -1; return; }
  ~MapIterator(void) { return; }

  void operator delete(void *data);
  DATA *Next(void);
  DATA *Begin(void);
  bool IsEnd();
  KEY  *CurrentKey(void);
  bool IsEmpty(void);

private:
  int id;

  int index;
  MapData<KEY,DATA> *current;

  MapIterator<KEY, DATA> *next;
  Map<KEY, DATA> *d_myMap;

  friend class Map<KEY,DATA>;
};

template <class KEY, class DATA> class Map {
public:
  Map(int size=11) {
    d_iterators = NULL;
    d_auto = false;
    d_size = size;
    d_count = 0;
    d_iteratorCount = 0;
    d_data = new MapData<KEY,DATA>*[d_size];
    for(int i=0; i<d_size; i++) { d_data[i] = NULL; }
    return;
  }
  ~Map(void) { Clear(); RemoveIterators(); delete d_data; return; }

  bool IsEmpty(void) { if(d_count <= 0) { return true; } return false; }
  int Count(void) { return d_count; }
  int Size(void) { return d_size; }

  bool  Insert(KEY key, DATA *data);
  DATA *Retrieve(KEY key) { return Find(key); }
  DATA *Remove(KEY key) { return RemoveFromMap(key); }
  bool  Delete(KEY key) { DATA *a = RemoveFromMap(key); 
                          if(a!=NULL) { delete a; return true; }
                          return false; }
  bool  HasKey(KEY key);

  void Resize(int size);
  void Clear(void);
  void SetAutoDelete(bool autodelete) { d_auto = autodelete; return; }
  MapIterator<KEY, DATA> *Iterator(void);

private:
  DATA *Find(KEY key);
  DATA *RemoveFromMap(KEY key);
  void RemoveIterators(void);

  bool d_auto;
  int d_size;
  int d_count;
  MapData<KEY,DATA> **d_data;

  int d_iteratorCount;
  MapIterator<KEY,DATA> *d_iterators;

  friend class MapIterator<KEY,DATA>;
};

template <class KEY, class DATA>
bool Map<KEY, DATA>::Insert(KEY key, DATA *data) {
  if(data == NULL) { return false; }

  MapData<KEY,DATA> *moreData = new MapData<KEY,DATA>;
  if(moreData == NULL) { return false; }
  moreData->data = data;
  moreData->key = key;

  DATA *tempData = RemoveFromMap(key);
  if(tempData != NULL && d_auto) {
    delete tempData;
  }

  int index = (int)(key % (KEY)d_size);
  moreData->next = d_data[index];
  d_data[index] = moreData;

  d_count++;

  return true;
}

template <class KEY, class DATA>
bool Map<KEY, DATA>::HasKey(KEY key) {
  if(!d_size) { return false; }

  MapData<KEY,DATA> *data;
  int index = (int)(key % (KEY)d_size);
  data = d_data[index];

  while(data != NULL) {
    if(data->key == key) {
      return true;
    }
    data = data->next;
  }

  return false;
}

template <class KEY, class DATA>
DATA *Map<KEY, DATA>::Find(KEY key) {
  if(!d_size) { return NULL; }

  MapData<KEY,DATA> *data;
  int index = (int)(key % (KEY)d_size);
  data = d_data[index];

  while(data != NULL) {
    if(data->key == key) {
      return data->data;
    }
    data = data->next;
  }

  return NULL;
}

template <class KEY, class DATA>
DATA *Map<KEY, DATA>::RemoveFromMap(KEY key) {
  if(!d_size) { return NULL; }

  MapData<KEY,DATA> *data;
  int index = (int)(key % (KEY)d_size);
  data = d_data[index];

  if(data == NULL) { return NULL; }

  DATA *actualData = NULL;
  if(d_data[index]->key == key) {
    MapData<KEY,DATA> *temp = d_data[index];
    actualData = d_data[index]->data;
    d_data[index] = d_data[index]->next;
    delete temp;
    d_count--;
    return actualData;
  }

  while(data->next != NULL) {
    if(data->next->key == key) {
      actualData = data->next->data;
      MapData<KEY,DATA> *temp = data->next;
      data->next = data->next->next;
      delete temp;
      d_count--;
      return actualData;
    }
    data = data->next;
  }

  return NULL;
}

template <class KEY, class DATA> void Map<KEY,DATA>::Resize(int size) {
  MapData<KEY,DATA> **temp = d_data;
  d_data = new MapData<KEY,DATA>*[size];

  for(int i=0; i<size; i++) { d_data[i] = NULL; }

  for(int i=0; i<d_size; i++) {
    MapData<KEY,DATA> *temp2 = temp[i];
    while(temp2 != NULL) {
      MapData<KEY,DATA> *temp3 = temp2;
      Insert(temp3->key, temp3->data);
      temp2 = temp2->next;
      delete temp3;
    }
  }

  delete temp;

  d_size = size;

  return;
}

template <class KEY, class DATA> void Map<KEY,DATA>::Clear(void) {
  for(int i=0; i<d_size; i++) {
    MapData<KEY,DATA> *temp = d_data[i];
    while(temp != NULL) {
      d_data[i] = temp->next;
      if(d_auto) {
	if(temp->data != NULL) {
	  delete temp->data;
	  temp->data = NULL;
	}
      }
      delete temp;
      temp = d_data[i];
    }
  }

  return;
}

template <class KEY, class DATA>
MapIterator<KEY, DATA> *Map<KEY,DATA>::Iterator(void) {
  MapIterator<KEY, DATA> *newIterator = new MapIterator<KEY,DATA>;

  newIterator->id = d_iteratorCount;
  d_iteratorCount++;

  newIterator->index = 0;
  newIterator->current = NULL;
  newIterator->d_myMap = this;
  newIterator->next = d_iterators;
  d_iterators = newIterator;

  newIterator->Begin();

  return newIterator;
}

template <class KEY, class DATA> void Map<KEY,DATA>::RemoveIterators(void) {
  while(d_iterators != NULL) {
    d_iterators->d_myMap = NULL;
    d_iterators = d_iterators->next;
  }
  
  return;
}

template <class KEY, class DATA> 
void MapIterator<KEY,DATA>::operator delete(void *data) {
  MapIterator<KEY,DATA> *mapIterator = (MapIterator<KEY,DATA> *)data;
  if(mapIterator->d_myMap != NULL) {
    MapIterator *temp = mapIterator->d_myMap->d_iterators;
    MapIterator *temp2 = NULL;
    while(temp != NULL) {
      if(temp->id == mapIterator->id) {
	if(temp2 == NULL) {
	  mapIterator->d_myMap->d_iterators = temp->next;
	} else {
	  temp2->next = temp->next;
	}
      }
      temp2 = temp;
      temp = temp->next;
    }
  }

  free(data);

  return;
}

template <class KEY, class DATA>
DATA *MapIterator<KEY,DATA>::Next(void) {
  if(d_myMap == NULL || !d_myMap->Size()) {
    return NULL;
  }

  if(current->next != NULL) {
    current = current->next;
    return current->data;
  }

  int i = index+1;
  for(; i<d_myMap->Size(); i++) {
    if(d_myMap->d_data[i] != NULL) {
      index = i;
      break;
    }
  }

  if(i >= d_myMap->Size()) {
    return NULL;
  }

  current = d_myMap->d_data[index];

  return current->data;
}

template <class KEY, class DATA>
DATA *MapIterator<KEY,DATA>::Begin(void) {
  if(d_myMap == NULL || !d_myMap->Size()) {
    return NULL;
  }

  index = -1;

  for(int i=0; i<d_myMap->Size(); i++) {
    if(d_myMap->d_data[i] != NULL) {
      index = i;
      break;
    }
  }
  if(index < 0) { return NULL; }

  current = d_myMap->d_data[index];

  return current->data;
}

template <class KEY, class DATA> bool MapIterator<KEY,DATA>::IsEmpty(void) {
  if(d_myMap == NULL) { return true; }

  return d_myMap->IsEmpty();
}

template <class KEY, class DATA> bool MapIterator<KEY,DATA>::IsEnd(void) {
  if(current->next != NULL) { return false; }

  for(int tempIndex=index+1; tempIndex<d_myMap->Size(); tempIndex++) {
    if(d_myMap->d_data[tempIndex] != NULL) {
      return false;
    }
  }

  return true;
}

template <class KEY, class DATA> KEY *MapIterator<KEY,DATA>::CurrentKey(void) {
  if(current == NULL) {
    return NULL;
  }

  return &(current->key);
}

#endif
