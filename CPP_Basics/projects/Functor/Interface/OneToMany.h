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

#ifndef __Functor_Inteface_OneToMany_h__
#define __Functor_Inteface_OneToMany_h__

/*!
 *
 * \class OneToMany
 *
 * \ingroup nFunctor
 *
 * OneToMany- OneToMany is similar to Chain in that it allows for multiple
 *   callable entities to be executed with one call to the OneToMany object.
 *   However, Chain is more generic and can handle any callable entity while
 *   OneToMany can only handle Functors or those who derive from iFunctor.
 *   Therefore, all incoming callable entities must be converted to functors
 *   before they can be added to this class.  This less general approach
 *   allows me to keep a list of iFunctors that allow me to manipulate the
 *   chain of callable entities where that is not possible in the Chain
 *   class without losing generality.  Therefore, this allows for a more
 *   dyanmic chaining of callable entities.
 *
 * Since this class is intended to be dynamic, it can also be empty.  To handle
 *   handle the empty case, I had to define a local variable for the result
 *   that will act like a default if nothing is executed and a return value
 *   is required.  The return value can be given in the constructor, through
 *   the assignment operator, or get/set pair of functions.  However, the use
 *   of a default does not work for void return types, so the OneToMany
 *   class had to be specialized for it.  There is a lot of redundant code
 *   because of it, but I have not figured out a way around it yet.
 *
 * Other than the functions to Add, Remove, and Clear all the entities, this
 *   class exactly like a functor and uses the same interface as them.
 *
 * Finally, I talk about threading and its relation to this class in
 *   iStorageType.  It is mentioned there because this class does not
 *   implement any type of threading but needs to manage its own data.
 *   Thus a StorageType is given as a template parameter and is used by
 *   the operator() during execution to access the entities.  The StorageType
 *   is the actual manager of the data.
 *
 * Comparison operators are at the bottom of the file.
 */

#include "Type/Interface/NullType.h"
#include "Type/Interface/ReassignableType.h"
#include "Functor/Interface/Exceptions.h"
#include "Functor/Interface/iFunctor.h"
#include "Functor/Interface/iStorageType.h"

namespace nFunctor
{

  // OneToMany- This is the version for all return types except void.
  template <
            typename R        = void,
            class TList       = nType::NullType,
            class StorageType = DefaultOneToManyStorage<R, TList>
           >
  class OneToMany : public iFunctor<R, TList> {
  public:
    typedef typename iFunctor<R, TList>::Interface      Interface;
    typedef typename iFunctor<R, TList>::ResultType     ResultType;
    typedef typename iFunctor<R, TList>::ArguementTypes ArguementTypes;
    typedef typename iFunctor<R, TList>::Parm1 Parm1;
    typedef typename iFunctor<R, TList>::Parm2 Parm2;
    typedef typename iFunctor<R, TList>::Parm3 Parm3;
    typedef typename iFunctor<R, TList>::Parm4 Parm4;
    typedef typename iFunctor<R, TList>::Parm5 Parm5;
    typedef typename iFunctor<R, TList>::Parm6 Parm6;
    typedef typename iFunctor<R, TList>::Parm7 Parm7;
    typedef typename iFunctor<R, TList>::Parm8 Parm8;
    typedef typename iFunctor<R, TList>::Parm9 Parm9;
    typedef typename iFunctor<R, TList>::Parm10 Parm10;
    typedef typename iFunctor<R, TList>::Parm11 Parm11;
    typedef typename iFunctor<R, TList>::Parm12 Parm12;
    typedef typename iFunctor<R, TList>::Parm13 Parm13;
    typedef typename iFunctor<R, TList>::Parm14 Parm14;
    typedef typename iFunctor<R, TList>::Parm15 Parm15;

    // OptResultType is the optimized version of the ResultType used for passing
    //   that type around to the various functions.
    typedef typename nType::ReassignableType<R>::ParameterType OptResultType;

    // The default constructor has an empty entity list and an uninitialized
    //   default return value.
    OneToMany(void) : d_default() { return; }

    // Constructor that initializes the default return value
    OneToMany(OptResultType r) : d_default(r) { return; }

    // Constructor that begins with an entity in the list
    OneToMany(Interface *i) : d_default() {
      if(!i) { throw BadCallableEntity(); }
      d_iCallable.Add(i);
      return;
    }

    // Constructor that begins with an entity in the list and an
    //   initialized default return value.
    OneToMany(Interface *i, OptResultType r) : d_default(r)
    {
      if(!i) { throw BadCallableEntity(); }
      d_iCallable.Add(i);
      return;
    }

    // Destructor is responsible for clearing the list in case there is
    //   clearing management that needs to occur.
    ~OneToMany(void) { d_iCallable.Clear(); return; }

    // These are the methods for modifying the list of callable entities.
    void Add(Interface *i) {
      if(!i) { throw BadCallableEntity(); }
      d_iCallable.Add(i);
      return;
    }

    void Remove(Interface *i) {
      if(!i) { throw BadCallableEntity(); }
      d_iCallable.Remove(i);
      return;
    }

    void Clear(void) { d_iCallable.Clear(); return; }

    // Get and change the default return value.
    void ChangeDefault(OptResultType r) { d_default = r; return; }
    const ResultType Default(void) const { return d_default.Value(); }

    // Allows read-only access to the internal callable entity.  This is
    //   currently used by operator==.
    const StorageType &GetData(void) const { return d_iCallable; }
          StorageType &GetData(void)       { return d_iCallable; }

    iStorageType<R, TList> *GetStorage(void) { return &d_iCallable; }

    // Different ways to get the interface for a functor rather
    //   than casting.
    Interface *InterfacePtr(void) { return  this; }
    Interface &InterfaceRef(void) { return *this; }

    // Cloning allows copies of the functor to be created.  Perhaps at some
    //   point, an allocator should be given for people to have more control
    //   over how new Functors are created.  Cloning allows for copying to
    //   occur through the interface who doesn't have all the necessary
    //   information to create an actual copy.
    OneToMany *Clone(void) const { return new OneToMany(*this); }

  private:
    Interface *DoClone(void) const { return new OneToMany(*this); }

  public:
    // All the following operator() essentially cover all the possible
    //   arguement list length combinations.  By inheriting iFunctor
    //   which only defines a single version of one of these functions allows
    //   the compiler to report at compile time that the wrong one is being
    //   used.  It doesn't hold with make the interface hard to use
    //   inappropriately, but is really one of the better solutions given
    //   the working of templates.
    // All of these execution operations work in the same fashion:
    //   1. Get an iterator that starts at the beginning of the list
    //   2. Set the default return type
    //   3. Loop over all the callable entities
    //      a. Get the callable entity - may do some threading
    //      b. execute forwarding
    //      c. Release the callable entity - may do some threading
    //   4. return the return value
    ResultType operator()(void) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      ResultType r = d_default.Value();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { r = (*f)(); }
        i.Release();
      }
      return r;
    }

    ResultType operator()(Parm1 p1) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      ResultType r = d_default.Value();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { r = (*f)(p1); }
        i.Release();
      }
      return r;
    }

    ResultType operator()(Parm1 p1, Parm2 p2) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      ResultType r = d_default.Value();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { r = (*f)(p1, p2); }
        i.Release();
      }
      return r;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      ResultType r = d_default.Value();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { r = (*f)(p1, p2, p3); }
        i.Release();
      }
      return r;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      ResultType r = d_default.Value();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { r = (*f)(p1, p2, p3, p4); }
        i.Release();
      }
      return r;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4,
                          Parm5 p5) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      ResultType r = d_default.Value();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { r = (*f)(p1, p2, p3, p4, p5); }
        i.Release();
      }
      return r;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      ResultType r = d_default.Value();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { r = (*f)(p1, p2, p3, p4, p5, p6); }
        i.Release();
      }
      return r;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      ResultType r = d_default.Value();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { r = (*f)(p1, p2, p3, p4, p5, p6, p7); }
        i.Release();
      }
      return r;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      ResultType r = d_default.Value();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { r = (*f)(p1, p2, p3, p4, p5, p6, p7, p8); }
        i.Release();
      }
      return r;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      ResultType r = d_default.Value();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { r = (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9); }
        i.Release();
      }
      return r;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9,
                          Parm10 p10) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      ResultType r = d_default.Value();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { r = (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); }
        i.Release();
      }
      return r;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      ResultType r = d_default.Value();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { r = (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11); }
        i.Release();
      }
      return r;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      ResultType r = d_default.Value();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { r = (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12); }
        i.Release();
      }
      return r;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      ResultType r = d_default.Value();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) {
          r = (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
        }
        i.Release();
      }
      return r;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13, Parm14 p14) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      ResultType r = d_default.Value();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) {
          r = (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13,
                   p14);
        }
        i.Release();
      }
      return r;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13, Parm14 p14,
                          Parm15 p15) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      ResultType r = d_default.Value();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) {
          r = (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13,
                   p14, p15);
        }
        i.Release();
      }
      return r;
    }

  private:
    nType::ReassignableType<ResultType> d_default;
    StorageType d_iCallable;
  };

  // OneToMany- This is the version for the void return type
  template <class TList, class StorageType>
  class OneToMany<void, TList, StorageType> : public iFunctor<void, TList> {
  public:
    typedef typename iFunctor<void, TList>::Interface      Interface;
    typedef typename iFunctor<void, TList>::ResultType     ResultType;
    typedef typename iFunctor<void, TList>::ArguementTypes ArguementTypes;
    typedef typename iFunctor<void, TList>::Parm1 Parm1;
    typedef typename iFunctor<void, TList>::Parm2 Parm2;
    typedef typename iFunctor<void, TList>::Parm3 Parm3;
    typedef typename iFunctor<void, TList>::Parm4 Parm4;
    typedef typename iFunctor<void, TList>::Parm5 Parm5;
    typedef typename iFunctor<void, TList>::Parm6 Parm6;
    typedef typename iFunctor<void, TList>::Parm7 Parm7;
    typedef typename iFunctor<void, TList>::Parm8 Parm8;
    typedef typename iFunctor<void, TList>::Parm9 Parm9;
    typedef typename iFunctor<void, TList>::Parm10 Parm10;
    typedef typename iFunctor<void, TList>::Parm11 Parm11;
    typedef typename iFunctor<void, TList>::Parm12 Parm12;
    typedef typename iFunctor<void, TList>::Parm13 Parm13;
    typedef typename iFunctor<void, TList>::Parm14 Parm14;
    typedef typename iFunctor<void, TList>::Parm15 Parm15;

    // Constructors and assignment operations.

    // Default constructor begins with an empty list of entities.
    OneToMany(void) { return; }

    // This constructor begins with a list of size one, possibly
    OneToMany(Interface *i) {
      if(!i) { throw BadCallableEntity(); }
      d_iCallable.Add(i);
      return;
    }

    // It is the destructor's responsibility to clear the list in case
    //   any clean up needs to occur prior to destruction.
    ~OneToMany(void) { d_iCallable.Clear(); return; }

    // These are the methods for modifying the list of callable entities.
    void Add(Interface *i) {
      if(!i) { throw BadCallableEntity(); }
      d_iCallable.Add(i);
      return;
    }
    void Remove(Interface *i) {
      if(!i) { throw BadCallableEntity(); }
      d_iCallable.Remove(i);
      return;
    }

    void Clear(void) { d_iCallable.Clear(); return; }

    // Allows read-only access to the internal callable entity.  This is
    //   currently used by operator==.
    const StorageType &GetData(void) const { return d_iCallable; }
          StorageType &GetData(void)       { return d_iCallable; }

    iStorageType<void,TList> *GetStorage(void) { return &d_iCallable; }

    // Different ways to get the interface for a functor rather
    //   than casting.
    Interface *InterfacePtr(void) { return  this; }
    Interface &InterfaceRef(void) { return *this; }

    // Cloning allows copies of the functor to be created.  Perhaps at some
    //   point, an allocator should be given for people to have more control
    //   over how new Functors are created.  Cloning allows for copying to
    //   occur through the interface who doesn't have all the necessary
    //   information to create an actual copy.
    OneToMany *Clone(void) const { return new OneToMany(*this); }

  private:
    Interface *DoClone(void) const { return new OneToMany(*this); }

  public:
    // All the following operator() essentially cover all the possible
    //   arguement list length combinations.  By inheriting iFunctor
    //   which only defines a single version of one of these functions allows
    //   the compiler to report at compile time that the wrong one is being
    //   used.  It doesn't hold with make the interface hard to use
    //   inappropriately, but is really one of the better solutions given
    //   the working of templates.
    // All of these execution operations work in the same fashion:
    //   1. Get an iterator that starts at the beginning of the list
    //   2. Loop over all the callable entities
    //      a. Get the callable entity - may do some threading
    //      b. execute forwarding
    //      c. Release the callable entity - may do some threading
    //   4. return
    ResultType operator()(void) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { (*f)(); }
        i.Release();
      }
      return;
    }

    ResultType operator()(Parm1 p1) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { (*f)(p1); }
        i.Release();
      }
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { (*f)(p1, p2); }
        i.Release();
      }
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { (*f)(p1, p2, p3); }
        i.Release();
      }
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { (*f)(p1, p2, p3, p4); }
        i.Release();
      }
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4,
                          Parm5 p5) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { (*f)(p1, p2, p3, p4, p5); }
        i.Release();
      }
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { (*f)(p1, p2, p3, p4, p5, p6); }
        i.Release();
      }
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { (*f)(p1, p2, p3, p4, p5, p6, p7); }
        i.Release();
      }
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { (*f)(p1, p2, p3, p4, p5, p6, p7, p8); }
        i.Release();
      }
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9); }
        i.Release();
      }
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9,
                          Parm10 p10) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); }
        i.Release();
      }
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11); }
        i.Release();
      }
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) { (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12); }
        i.Release();
      }
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) {
          (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
        }
        i.Release();
      }
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13, Parm14 p14) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) {
          (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13,
               p14);
        }
        i.Release();
      }
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13, Parm14 p14,
                          Parm15 p15) const
    {
      typename StorageType::Iterator i = d_iCallable.begin();
      for(; i != d_iCallable.end(); ++i) {
        iFunctor<ResultType, ArguementTypes> *f = i.Get();
        if(f) {
          (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13,
               p14, p15);
        }
        i.Release();
      }
      return;
    }

  private:
    StorageType d_iCallable;
  };

  // Comparison operators
  template <typename T, typename U, typename V>
  const bool operator==(const OneToMany<T,U,V> &lhs,
                        const OneToMany<T,U,V> &rhs)
  {
    if(lhs.GetData() == rhs.GetData()) { return true; }
    return false;
  }

  template <typename T, typename U, typename V>
  const bool operator!=(const OneToMany<T,U,V> &lhs,
                        const OneToMany<T,U,V> &rhs)
  {
    if(lhs.GetData() != rhs.GetData()) { return true; }
    return false;
  }

} // end namespace nFunctor

#endif
