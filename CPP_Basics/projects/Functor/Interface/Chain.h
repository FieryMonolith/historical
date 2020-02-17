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

// June 22, 2006 Jason Stredwick.  Made some modifications, moved this
//   class into its own file, and did some renaming.

#ifndef __Functor_Inteface_Chainer_h__
#define __Functor_Inteface_Chainer_h__

/*!
 *
 * \class Chain
 * \ingroup nFunctor
 *
 * Chain allows two callable entities to be called one after the other
 *   using a single forwarding call.  Chain can take any callable entity
 *   even another chain which allows this class to form a chain of
 *   executions.
 * Because Chain accepts any type, but that type is specified as a template
 *   arguement, this class can be compared for equivalence, ie. holding the
 *   same callable entities in the same order.
 *
 * All parameter passing is optimized, see comments in iFunctor.
 *
 * The executing of operator() from the iFunctor base class involves
 *   a virtual call to operator(), direct access to a structure variable,
 *   and a pointer dereferencing before the actual callable entity is invoked.
 *
 */

#include <memory> // std::auto_ptr

#include "Type/Interface/NullType.h"
#include "Type/Interface/PtrAccessibleType.h"
#include "Functor/Interface/Exceptions.h"
#include "Functor/Interface/iFunctor.h"

namespace nFunctor
{
  /*** FORWARD DECLARATIONS **********************************************/
  // Forward declaration
  template <typename Fun1, typename Fun2, typename R, class TList>
  class Chain;

  // Forward declaration of the Chainer function which allows for the
  //   creation of a Chain from two callable entities.
  template <class Fun1, class Fun2, typename R, class TList>
  Chain<Fun1, Fun2, R, TList> *Chainer(
    typename nType::PtrAccessibleType<Fun1>::ParameterType fun1,
    typename nType::PtrAccessibleType<Fun2>::ParameterType fun2);

  // Allows for any functor that has an interface of
  //   iFunctor<ResultType, ArguementTypes> so that Chainer can be called
  //   without specifying template arguements.
  template <typename R, class TList>
  Chain< iFunctor<R, TList>*, iFunctor<R, TList>*, R, TList > *
  Chainer(iFunctor<R, TList> *fun1, iFunctor<R, TList> *fun2);
  /*** FORWARD DECLARATIONS **********************************************/

  // Chain- Chain allows for two callable entities to be hooked together into
  //   a single callable entity.
  template <typename Fun1, typename Fun2, typename R=void,
            class TList=nType::NullType>
  class Chain : public iFunctor<R, TList> {
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

    typedef typename nType::PtrAccessibleType<Fun1> Data1;
    typedef typename Data1::ParameterType OptFun1;
    typedef typename Data1::StorageType DataValue1;

    typedef typename nType::PtrAccessibleType<Fun2> Data2;
    typedef typename Data2::ParameterType OptFun2;
    typedef typename Data2::StorageType DataValue2;

    // Fun can be an object, reference, or pointer to a callable object.  This
    //   uses default copy constructor, assignment, and destructor.

    // A pointer is used for the incoming object, a BadCallableEntity exception
    //   will be thrown if it is null.
    Chain(OptFun1 fun1, OptFun2 fun2) : d_f1(fun1), d_f2(fun2) {
      if(!d_f1() || !d_f2()) { throw BadCallableEntity(); }
      return;
    }
    void SetFun1(OptFun1 f1) {
      d_f1 = f1;
      if(!d_f1()) { throw BadCallableEntity(); }
      return;
    }
    void SetFun2(OptFun2 f2) {
      d_f2 = f2;
      if(!d_f2()) { throw BadCallableEntity(); }
      return;
    }

    // Allows read-only access to the internal callable entity.  This is
    //   currently used by operator==.
    const DataValue1 &GetData1(void) const { return d_f1.Value(); }
    const DataValue2 &GetData2(void) const { return d_f2.Value(); }

    // Different ways to get the interface for a functor rather
    //   than casting.
    Interface *InterfacePtr(void) { return  this; }
    Interface &InterfaceRef(void) { return *this; }

    // Cloning allows copies of the functor to be created.  Perhaps at some
    //   point, an allocator should be given for people to have more control
    //   over how new Functors are created.  Cloning allows for copying to
    //   occur through the interface who doesn't have all the necessary
    //   information to create an actual copy.
    Chain *Clone(void) const { return new Chain(*this); }

  private:
    Interface *DoClone(void) const { return new Chain(*this); }

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
    { return (*d_f1.p)(), (*d_f2.p)(); }

    ResultType operator()(Parm1 p1) const
    { return (*d_f1.p)(p1), (*d_f2.p)(p1); }

    ResultType operator()(Parm1 p1, Parm2 p2) const
    { return (*d_f1.p)(p1, p2), (*d_f2.p)(p1, p2); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) const
    { return (*d_f1.p)(p1, p2, p3), (*d_f2.p)(p1, p2, p3); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4) const
    { return (*d_f1.p)(p1, p2, p3, p4), (*d_f2.p)(p1, p2, p3, p4); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4,
                          Parm5 p5) const
    { return (*d_f1.p)(p1, p2, p3, p4, p5), (*d_f2.p)(p1, p2, p3, p4, p5); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6) const
    {
      return (*d_f1.p)(p1, p2, p3, p4, p5, p6),
             (*d_f2.p)(p1, p2, p3, p4, p5, p6);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7) const
    {
      return (*d_f1.p)(p1, p2, p3, p4, p5, p6, p7),
             (*d_f2.p)(p1, p2, p3, p4, p5, p6, p7);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8) const
    {
      return (*d_f1.p)(p1, p2, p3, p4, p5, p6, p7, p8),
             (*d_f2.p)(p1, p2, p3, p4, p5, p6, p7, p8);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9) const
    {
      return (*d_f1.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9),
             (*d_f2.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9,
                          Parm10 p10) const
    {
      return (*d_f1.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10),
             (*d_f2.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11) const
    {
      return (*d_f1.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11),
             (*d_f2.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12) const
    {
      return (*d_f1.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,
                       p12),
             (*d_f2.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,
                       p12);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13) const
    {
      return (*d_f1.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,
                       p12, p13),
             (*d_f2.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,
                       p12, p13);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13, Parm14 p14) const
    {
      return (*d_f1.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,
                       p12, p13, p14),
             (*d_f2.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,
                       p12, p13, p14);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13, Parm14 p14,
                          Parm15 p15) const
    {
      return (*d_f1.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,
                       p12, p13, p14, p15),
             (*d_f2.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,
                       p12, p13, p14, p15);
    }

  private:
    // Holds the minimum amount of data to call the given entity.
    Data1 d_f1;
    Data2 d_f2;
  };
  
  // Comparison operators, determines if two functors are equivalent, ie.
  //   they point to the same data or have copies of the same data.
  template <class A, class B, class T, class U>
  const bool operator==(const Chain<A,B,T,U> &lhs,
                        const Chain<A,B,T,U> &rhs)
  {
    if(lhs.GetData1() == rhs.GetData1() && lhs.GetData2() == rhs.GetData2()) {
      return true;
    }
    return false;
  }

  template <class A, class B, class T, class U>
  const bool operator!=(const Chain<A,B,T,U> &lhs,
                        const Chain<A,B,T,U> &rhs)
  {
    if(lhs.GetData1() == rhs.GetData1() && lhs.GetData2() == rhs.GetData2()) {
      return false;
    }
    return true;
  }

  // Helper functions - Chainer
  // The Chainer function which allows for the creation of a Chain from
  //   two callable entities.
  template <class Fun1, class Fun2, typename R, class TList>
  Chain<Fun1, Fun2, R, TList> *Chainer(
    typename nType::PtrAccessibleType<Fun1>::ParameterType fun1,
    typename nType::PtrAccessibleType<Fun2>::ParameterType fun2)
  {
    std::auto_ptr< Chain<Fun1, Fun2, R, TList> >
      p(new Chain<Fun1, Fun2, R, TList>(fun1, fun2));
    return p.release();
  }

  // Allows for any functor that has an interface of
  //   iFunctor<ResultType, ArguementTypes> so that Chainer can be called
  //   without specifying template arguements.
  template <typename R, class TList>
  Chain< iFunctor<R, TList>*, iFunctor<R, TList>*, R, TList> *
  Chainer(iFunctor<R, TList> *fun1, iFunctor<R, TList> *fun2) {
    if(!fun1 || !fun2) { return 0; }
    std::auto_ptr<
                   Chain<iFunctor<R, TList>*,
                         iFunctor<R, TList>*,
                         R, TList>
                 >  p(new Chain<iFunctor<R, TList>*,
                                iFunctor<R, TList>*,
                                R, TList>(fun1, fun2));
    return p.release();
  }

} // end namespace nFunctor

#endif
