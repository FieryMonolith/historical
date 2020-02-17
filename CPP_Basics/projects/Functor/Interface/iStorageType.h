////////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// The author or Addison-Wesley Longman make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

// July 2, 2006 Jason Stredwick.  Added this class to nFunctor.

#ifndef __Functor_Interface_iStorageType_h__
#define __Functor_Interface_iStorageType_h__

#include <list>

#include "Functor/Interface/iFunctor.h"

/*!
 * \class iStorageIterator, iStorage, DefaultOneToManyStorage
 *
 * \ingroup nFunctor
 *
 * The iStorage and iStorageIterator define the interface that the class
 *   OneToMany expects for its storage type.  Also, to allow OneToMany to
 *   run immediately, a default storage type was created called
 *   DefaultOneToManyStorage.  This can also be used as a template for making
 *   other storage types for OneToMany.
 *
 * Because functors are intended to not handle threading themselves, I consider
 *   the wrapped callable entity should ensure its own reentrant ability, the
 *   OneToMany class does have its own data beyond merely the entity.  This
 *   means that it may need to be thread-enabled.  However, to keep with the
 *   other functors, I kept threading out OneToMany.  Instead the storage
 *   type used by OneToMany can encapsulate threading behavior, which is why
 *   the storage type interface uses both a get and release methods.  This
 *   also allows for a more flexible type of threading behavior.  For any given
 *   OneToMany, the locking could occur per callable entity, for the entity
 *   set of entities, or not at all.
 *
 * While descriptions of each of the three classes follows, the key point
 *   about iStorage is that it must provide a typedef Iterator, which is
 *   initially defined in the interface class.  The DefaultOneToManyStorage
 *   class utilizes a list to store elements and allows for multiple
 *   instances of the same callable entity where the first in will be the
 *   first removed.  Also, it is single threaded only.
 *
 */

namespace nFunctor
{

  // iStorageIterator- The OneToMany class accesses its callable entities by
  //   requesting an iterator from its storage.  The OneToMany can then
  //   directly access the entity by calling the get function.  In case of
  //   threading, a release function is available to let the iterator know
  //   that you are done with that entity.  Finally, the operator++ is the
  //   method for moving over the set of entities.  Access to the entity
  //   only occurs through the iterator, never the actual storage unit
  //   directly.
  template <typename R, class TList>
  class iStorageIterator {
  protected:
    virtual ~iStorageIterator(void) { return; }

  public:
    virtual iFunctor<R, TList> *Get(void) = 0;
    virtual void Release(void) = 0;
    virtual void operator++(void) = 0;
  };

  // iStorageType- This is the expected interface for the storage type used
  //   by the OneToMany class.  It can be used to add and remove callable
  //   entities to the set of entities.  While I use the term set, the person
  //   who derives from this class determines whether or not multiple
  //   instances of an entity is allowed.  This class can also clear the
  //   entire set of entities, thus the derived version needs to be able to
  //   handle an empty set.  The OneToMany class already checks for a non-zero
  //   pointer value.
  template <typename R, class TList>
  class iStorageType {
  public:
    virtual ~iStorageType(void) { return; }

    virtual void Add(iFunctor<R, TList> *) = 0;
    virtual void Remove(iFunctor<R, TList> *) = 0;
    virtual void Clear(void) = 0;
  };

  // DefaultOneToManyStorage- This class is the default storage type for the
  //   OneToMany class and is single threaded only.  The default storage
  //   type is a std::list and allows for multiple instances of the same
  //   callable entity.  The first one added will be the first one removed
  //   due to the push_back of new entities and forward search for removal.
  // This class also defines its own iterator
  template <typename R, class TList>
  class DefaultOneToManyStorage : public iStorageType<R, TList> {
  private:
    // Define the data for this class
    std::list< iFunctor<R, TList>* > d_callables;

  public:
    // Upon destruction, clear the list.
    ~DefaultOneToManyStorage(void) { Clear(); return; }

    const DefaultOneToManyStorage &operator=(const DefaultOneToManyStorage &d)
    {
      if(&d != this) { Clear(); d_callables = d.d_callables; }
      return *this;
    }

    // Return a const version of the internals, this is currently used for
    //   comparison with operator==.
    const std::list< iFunctor<R, TList>* > &GetData(void) const {
      return d_callables;
    }

    // Adds a callable entity to the list
    void Add(iFunctor<R, TList> *i) { d_callables.push_back(i); return; }

    // Remove the first entity whose pointer is equal to the input.
    void Remove(iFunctor<R, TList> *i) {
      typename std::list< iFunctor<R, TList>* >::iterator it;
      for(it=d_callables.begin(); it!=d_callables.end(); ++it) {
        if(*it == i) {
          d_callables.erase(it);
          return;
        }
      }
      return;
    }

    // Remove all entities from the list
    void Clear(void) {
      d_callables.clear();
      return;
    }

  public:
    // Define the iterator and the accessors begin and end.

    // Iter- Iter is the iteration class for this storage type.  It defines
    //   the constructor, assignment, comparison, Get, Release, and forward ++.
    // There is no default constructor, but there is default copy constructor.
    template <typename R1, typename T>
    class Iter : public iStorageIterator<R1,T> {
    public:
      // Takes a reference to the list and an iterator into that list.  It
      //   stores a pointer to the list so assignment is possible.
      Iter(const std::list< iFunctor<R1,T>* > &l,
           typename std::list< iFunctor<R1,T>* >::const_iterator it)
      : d_list(&l), d_it(it)
      {
        return;
      }

      // Set the list to nothing.
      ~Iter(void) { d_list = 0; return; }

      // Return the current item unless you are at the end then return 0.
      iFunctor<R,T> *Get(void) {
        if(d_it != d_list->end()) { return *d_it; }
        return 0;
      }

      // Does nothing
      void Release(void) { return; }

      // Moves the iterator forward through the list
      void operator++(void) { d_it++; return; }

      // Comparison operators
      const bool operator==(const Iter &rhs) const {
        if(d_list == rhs.d_list && d_it == rhs.d_it) {
          return true;
        }
        return false;
      }

      const bool operator!=(const Iter &rhs) const {
        if(d_list == rhs.d_list && d_it == rhs.d_it) {
          return false;
        }
        return true;
      }

    private:
      const std::list< iFunctor<R1,T>* > *d_list;
      typename std::list< iFunctor<R1,T>* >::const_iterator d_it;
    };

    // Define the Iterator type and the accessors, begin and end.
    typedef Iter<R, TList> Iterator;

    Iterator begin(void) const {
      return Iterator(d_callables, d_callables.begin());
    }
    Iterator end(void) const {
      return Iterator(d_callables, d_callables.end());
    }
  };

  // Comparison operations for the default storage type
  template <typename R, typename S>
  const bool operator==(const DefaultOneToManyStorage<R,S> &lhs,
                        const DefaultOneToManyStorage<R,S> &rhs)
  {
    if(lhs.GetData() == rhs.GetData()) { return true; }
    return false;
  }

  template <typename R, typename S>
  const bool operator!=(const DefaultOneToManyStorage<R,S> &lhs,
                        const DefaultOneToManyStorage<R,S> &rhs)
  {
    if(lhs.GetData() != rhs.GetData()) { return true; }
    return false;
  }

}

#endif
