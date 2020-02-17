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

// July 6, 2006 Jason Stredwick.  Added this class to nFunctor.

#ifndef __Functor_Inteface_ReturnlessFunctor_h__
#define __Functor_Inteface_ReturnlessFunctor_h__

/*!
 *
 * \class ReturnlessFunctor
 *
 * \ingroup nFunctor
 *
 * ReturnlessFunctor- This class is intended to wrap any callable entity
 *   and strip off its return value.  This class would be useful for creating
 *   lists of callable entities that share the same function input but don't
 *   care about output.  This functor is essentially, Functor, except it
 *   does not return anything.
 *
 * All parameter passing is optimized, see comments in iFunctor.
 *
 * The executing of operator() from the iFunctor base class involves
 *   a virtual call to operator(), direct access to a structure variable,
 *   and a pointer dereferencing before the actual callable entity is invoked.
 *
 */

#include "Type/Interface/PtrAccessibleType.h"
#include "Functor/Interface/Exceptions.h"
#include "Functor/Interface/iFunctor.h"

namespace nFunctor
{

  // Fun is the callable entity type, which can be a value, ref, or ptr.
  // R is the return type
  // T is the list of arguement types
  template <typename Fun, typename R=void, typename T=nType::NullType>
  class ReturnlessFunctor : public iFunctor<void, T> {
  public:
    // Useful type information about the Functor.
    typedef typename iFunctor<void, T>::Interface      Interface;
    typedef typename iFunctor<void, T>::ResultType     ResultType;
    typedef typename iFunctor<void, T>::ArguementTypes ArguementTypes;
    typedef typename iFunctor<void, T>::Parm1 Parm1;
    typedef typename iFunctor<void, T>::Parm2 Parm2;
    typedef typename iFunctor<void, T>::Parm3 Parm3;
    typedef typename iFunctor<void, T>::Parm4 Parm4;
    typedef typename iFunctor<void, T>::Parm5 Parm5;
    typedef typename iFunctor<void, T>::Parm6 Parm6;
    typedef typename iFunctor<void, T>::Parm7 Parm7;
    typedef typename iFunctor<void, T>::Parm8 Parm8;
    typedef typename iFunctor<void, T>::Parm9 Parm9;
    typedef typename iFunctor<void, T>::Parm10 Parm10;
    typedef typename iFunctor<void, T>::Parm11 Parm11;
    typedef typename iFunctor<void, T>::Parm12 Parm12;
    typedef typename iFunctor<void, T>::Parm13 Parm13;
    typedef typename iFunctor<void, T>::Parm14 Parm14;
    typedef typename iFunctor<void, T>::Parm15 Parm15;

    // These are the typedefs that allow for value/ref/ptr Fun types
    typedef typename nType::PtrAccessibleType<Fun> Data;
    typedef typename Data::ParameterType OptFun;
    typedef typename Data::StorageType DataValue;

    // Fun can be an object, reference, or pointer to a callable object.  This
    //   uses default copy constructor, assignment, and destructor.

    // A pointer is used for the incoming object, a BadCallableEntity exception
    //   will be thrown if it is null.
    ReturnlessFunctor(OptFun fun) : d_f(fun) {
      if(!d_f()) { throw BadCallableEntity(); }
      return;
    }
    void SetFun(OptFun fun) {
      d_f = fun;
      if(!d_f()) { throw BadCallableEntity(); }
      return;
    }

    // Allows read-only access to the internal callable entity.  This is
    //   currently used by operator==.
    const DataValue &GetData(void) const { return d_f.Value(); }

    // Different ways to get the interface for a functor rather
    //   than casting.
    Interface *InterfacePtr(void) { return  this; }
    Interface &InterfaceRef(void) { return *this; }

    // Cloning allows copies of the functor to be created.  Perhaps at some
    //   point, an allocator should be given for people to have more control
    //   over how new Functors are created.  Cloning allows for copying to
    //   occur through the interface who doesn't have all the necessary
    //   information to create an actual copy.
    ReturnlessFunctor *Clone(void) const {
      return new ReturnlessFunctor(*this);
    }

  private:
    Interface *DoClone(void) const { return new ReturnlessFunctor(*this); }

  public:
    // All the following operator() essentially cover all the possible
    //   arguement list length combinations.  By inheriting iFunctor
    //   which only defines a single version of one of these functions allows
    //   the compiler to report at compile time that the wrong one is being
    //   used.  It doesn't hold with "make the interface hard to use
    //   inappropriately", but is really one of the better solutions given
    //   the working of templates.
    // Also, see comments in iFunctor to understand why these functions are
    //   const.
    ResultType operator()(void) const
    {
      (*d_f.p)();
      return;
    }

    ResultType operator()(Parm1 p1) const
    {
      (*d_f.p)(p1);
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2) const
    {
      (*d_f.p)(p1, p2);
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) const
    {
      (*d_f.p)(p1, p2, p3);
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4) const
    {
      (*d_f.p)(p1, p2, p3, p4);
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4,
                          Parm5 p5) const
    {
      (*d_f.p)(p1, p2, p3, p4, p5);
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6) const
    {
      (*d_f.p)(p1, p2, p3, p4, p5, p6);
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7) const
    {
      (*d_f.p)(p1, p2, p3, p4, p5, p6, p7);
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8) const
    {
      (*d_f.p)(p1, p2, p3, p4, p5, p6, p7, p8);
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9) const
    {
      (*d_f.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9);
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9,
                          Parm10 p10) const
    {
      (*d_f.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11) const
    {
      (*d_f.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12) const
    {
      (*d_f.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13) const
    {
      (*d_f.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13,
                          Parm14 p14) const
    {
      (*d_f.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
      return;
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13, Parm14 p14,
                          Parm15 p15) const
    {
      (*d_f.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
               p15);
      return;
    }

  private:
    // Holds the minimum amount of data to call the given entity.
    Data d_f;
  };

  // Comparison operators, determines if two functors are equivalent, ie.
  //   they point to the same data or have copies of the same data.
  template <typename T, typename U>
  const bool operator==(const ReturnlessFunctor<T,U> &lhs,
                        const ReturnlessFunctor<T,U> &rhs)
  {
    if(lhs.GetData() == rhs.GetData()) { return true; }
    return false;
  }

  template <typename T, typename U>
  const bool operator!=(const ReturnlessFunctor<T,U> &lhs,
                        const ReturnlessFunctor<T,U> &rhs)
  {
    if(lhs.GetData() != rhs.GetData()) { return true; }
    return false;
  }

}

#endif
