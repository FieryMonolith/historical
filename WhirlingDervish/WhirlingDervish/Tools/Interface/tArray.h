// Thanks to Charles Ofria for this piece of code, started usage 11/19/2002
// Slight modifications by Jason Stredwick 11/19/2002
//////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1993 - 2002 California Institute of Technology             //
//                                                                          //
// Read the COPYING and README files, or contact 'avida@alife.org',         //
// before continuing.  SOME RESTRICTIONS MAY APPLY TO USE OF THIS FILE.     //
//////////////////////////////////////////////////////////////////////////////

// This class provides an array template.

#ifndef __TARRAY_H__
#define __TARRAY_H__

#include <assert.h>

template <class T> class tArray {
protected:
  T * d_data;
  int d_size;

public:
  explicit tArray(const int size=0);
  explicit tArray(const tArray &arrayToBeCopied);
  virtual ~tArray(void);

  bool Good(void) const;
  //bool OK() const;

  int GetSize() const;
  tArray & operator= (const tArray &array);

  void Resize(int new_size);
  void Resize(int new_size, const T & empty_value);
  void ResizeClear(const int in_size);
  void SetAll(const T & value);

        T & ElementAt(const int index);
  const T & ElementAt(const int index) const;
        T & operator[](const int index);
  const T & operator[](const int index) const;
};

template <class T> inline tArray<T>::tArray(const int size) : d_data(NULL) {
  ResizeClear(size);

  return;
}

template <class T> inline tArray<T>::tArray(const tArray<T> &arrayToBeCopied) 
  : d_data(NULL), d_size(0) {
  this->operator=(arrayToBeCopied);

  return;
}

template <class T> inline tArray<T>::~tArray(void) {
  if (d_data != NULL) {
    delete [] d_data;
  }

  return;
}

template <class T> inline bool tArray<T>::Good() const {
  return (d_data != NULL);
}

//template <class T> inline bool tArray<T>::OK() const {
  //assert(d_size >= 0);
  //return true;
//}

template <class T> inline int tArray<T>::GetSize() const {
  return d_size;
}

template <class T> inline tArray<T> &tArray<T>::operator=(const tArray<T> &array) {
  if(d_size != array.GetSize()) {
    ResizeClear(array.GetSize());
  }
  for(int i = 0; i < d_size; i++) {
    d_data[i] = array[i];
  }

  return *this;
}

template <class T> inline void tArray<T>::Resize(int new_size) {
  // If we're already at the size we want, don't bother doing anything.
  if(d_size == new_size) {
    return;
  }

  // If new size is 0, clean up and go!
  if(new_size == 0) {
    delete [] d_data;
    d_data = NULL;
    d_size = 0;
    return;
  }

  // If new size > 0
  T * new_data = new T[new_size];
  // Memory Allocation Error: Out of Memory?
  assert(new_data != NULL);

  // Copy over old data...
  for (int i = 0; i<d_size && i<new_size; i++) {
    new_data[i] = d_data[i];
  }
  if (d_data != NULL) {
    // remove old data if exists
    delete [] d_data;
  }
  d_data = new_data;

  d_size = new_size;

  return;
}

template <class T> inline void tArray<T>::Resize(int new_size, 
						 const T & empty_value) {
  int old_size = d_size;
  Resize(new_size);
  if(new_size > old_size) {
    for(int i=old_size; i<new_size; i++) {
      d_data[i] = empty_value;
    }
  }

  return;
}

template <class T> inline void tArray<T>::ResizeClear(const int in_size) {
  d_size = in_size;

  // Invalid size specified for array intialization
  assert(d_size >= 0);

  if(d_data != NULL) {
    // remove old d_data if exists
    delete [] d_data;
  }

  if(d_size > 0) {
    d_data = new T[d_size];
    // Memory allocation error: Out of Memory?
    assert(d_data != NULL);
  } else {
    d_data = NULL;
  }

  return;
}

template <class T> inline void tArray<T>::SetAll(const T & value) {
  for(int i=0; i<d_size; ++i ) {
    d_data[i] = value;
  }

  return;
}

template <class T> inline T & tArray<T>::ElementAt(const int index) {
  // check range
  assert(index >= 0);      // Lower Bounds Error
  assert(index < d_size);  // Upper Bounds Error

  return d_data[index];    // in range, so return element
}

template <class T> inline const T &tArray<T>::ElementAt(const int index) const {
  // check range
  assert(index >= 0);      // Lower Bounds Error
  assert(index < d_size);  // Upper Bounds Error

  return d_data[index];    // in range, so return element
}

template <class T> inline T & tArray<T>::operator[](const int index) {
  return ElementAt(index);
}

template <class T> inline const T &tArray<T>::operator[](const int index) const {
  return ElementAt(index);
}

#endif // __TARRAY_HH__
