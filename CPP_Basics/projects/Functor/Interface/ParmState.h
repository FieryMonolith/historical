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

// June 22, 2006 Jason Stredwick.  I made these classes using iFunctor as a
//   template.

#ifndef __Functor_Inteface_ParmState_h__
#define __Functor_Inteface_ParmState_h__

#include "Type/Interface/NullType.h"
#include "Type/Interface/EmptyType.h"
#include "Type/Interface/TypeTraits.h"
#include "Type/Interface/Typelist.h"
#include "Type/Interface/Sequence.h"

namespace nFunctor
{

  /*** NOTE **************************************************************/
  // This set of class specializations is not complete.  It is a work in
  //   progress.  The main issue I have yet to resolve is what to do with
  //   pointers.  How to copy, can they be copied?
  // Also, I may add a storage policy that allows either explicit storage or
  //   copy storage.  One problem is explicit storage of a reference means
  //   that it can not be reassigned, which prevents resetting the values.
  //   However, one possible solution is to use my ReRef class.  It would
  //   have to be modified to default construct with a null value for
  //   possible methods of creating a state.

  /*!
   * \class ParmState
   * \ingroup nFunctor
   *
   * ParmState - This class is meant to store the paramters of a functor
   *   call.  The class can then be passed around and used to execute a
   *   a number of functors with the same data or store the data for later
   *   use.  I see two obvious ways of thinking about this class: 1) It can
   *   only store data and take a pointer to a functor to apply it to, or
   *   2) It can just store the data and the owning class must find a way
   *   to access that data.  The second way seems less feasible because
   *   a) One approach would be to also store the functor to apply, but
   *   this makes the class rigid and prevents passing to different 
   *   functors, and b) functors tend to be templated with Typelists which
   *   makes it difficult to know which parameters to extract (for example
   *   if you delay exectuion, the programmer will not specify which method
   *   to use, but a Exec() could be called which then needs to know which of
   *   the 15 types of methods to call).  Therefore, I am going with the
   *   first approach and add a function that takes a callable object pointer,
   *   but not a class method explicitly, and calls the functor with the
   *   internal data.
   *
   * By using ReferencedType in the ParmState specializations forces the
   *   storage of a copy of that data type rather than storing the
   *   reference itself.  However, ParameterType is used by the functions
   *   themselves to bring the value in by reference.
   *
   */

  // ParmState
  // R is the return type
  // TList is the list of arguement types
  template <typename R=void, typename TList=nType::NullType>
  class ParmState;

  namespace Private
  {

    template <typename R, TList>
    class ParmStateBase {
    public:
      typedef R     ResultType;
      typedef TList ArguementTypes;

      // Modified parameters to ensure copying occurs instead of
      //   storing a reference.
      typedef nType::EmptyType Parm1;
      typedef nType::EmptyType Parm2;
      typedef nType::EmptyType Parm3;
      typedef nType::EmptyType Parm4;
      typedef nType::EmptyType Parm5;
      typedef nType::EmptyType Parm6;
      typedef nType::EmptyType Parm7;
      typedef nType::EmptyType Parm8;
      typedef nType::EmptyType Parm9;
      typedef nType::EmptyType Parm10;
      typedef nType::EmptyType Parm11;
      typedef nType::EmptyType Parm12;
      typedef nType::EmptyType Parm13;
      typedef nType::EmptyType Parm14;
      typedef nType::EmptyType Parm15;

      // Optimized parameters for passing
      typedef nType::EmptyType oParm1;
      typedef nType::EmptyType oParm2;
      typedef nType::EmptyType oParm3;
      typedef nType::EmptyType oParm4;
      typedef nType::EmptyType oParm5;
      typedef nType::EmptyType oParm6;
      typedef nType::EmptyType oParm7;
      typedef nType::EmptyType oParm8;
      typedef nType::EmptyType oParm9;
      typedef nType::EmptyType oParm10;
      typedef nType::EmptyType oParm11;
      typedef nType::EmptyType oParm12;
      typedef nType::EmptyType oParm13;
      typedef nType::EmptyType oParm14;
      typedef nType::EmptyType oParm15;

    protected:
      ~ParmStateBase(void) { return; }
    };

  }

  /*** SPECIALIZATIONS ***************************************************/
  // There are two types of specializations given here: recursive and
  //   linear, which use Typelist and Seq respectively.

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 0 (zero) parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R>
  class ParmState<R, nType::NullType>
  : public Private:ParmStateBase<R, nType::NullType>
  {
  public:
    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) { return (*f)(); }
      return f();
    }
  };

  /*** Recursive(Typelist) Specialization ********************************/

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 1 parameter
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1>
  class ParmState<R, TYPELIST_1(P1)>
  : public Private:ParmStateBase<R, TYPELIST_1(P1)>
  {
  public:
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;

    ParmState(oParm1 _p1) : p1(_p1)
    {
      return;
    }

    void Set(oParm1 _p1)
    {
      p1 = _p1;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) { return (*f)(p1); }
      return f(p1);
    }

  private:
    Parm1 p1;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 2 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2>
  class ParmState<R, TYPELIST_2(P1, P2)>
  : public Private:ParmStateBase<R, TYPELIST_2(P1, P2)>
  {
  public:
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;

    ParmState(oParm1 _p1, oParm2 _p2) : p1(_p1), p2(_p2)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2)
    {
      p1 = _p1;
      p2 = _p2;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) { return (*f)(p1, p2); }
      return f(p1, p2);
    }

  private:
    Parm1 p1;
    Parm2 p2;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 3 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3>
  class ParmState<R, TYPELIST_3(P1, P2, P3)>
  : public Private:ParmStateBase<R, TYPELIST_3(P1, P2, P3)>
  {
  public:
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3) : p1(_p1), p2(_p2), p3(_p3)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) { return (*f)(p1, p2, p3); }
      return f(p1, p2, p3);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 4 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4>
  class ParmState<R, TYPELIST_4(P1, P2, P3, P4)>
  : public Private:ParmStateBase<R, TYPELIST_4(P1, P2, P3, P4)>
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4);
      }
      return f(p1, p2, p3, p4);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 5 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5>
  class ParmState<R, TYPELIST_5(P1, P2, P3, P4, P5)>
  : public Private:ParmStateBase<R, TYPELIST_5(P1, P2, P3, P4, P5)>
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5);
      }
      return f(p1, p2, p3, p4, p5);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 6 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6>
  class ParmState<R, TYPELIST_6(P1, P2, P3, P4, P5, P6)>
  : public Private:ParmStateBase<R, TYPELIST_6(P1, P2, P3, P4, P5, P6)>
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6)
    {
      return;
    }

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6);
      }
      return f(p1, p2, p3, p4, p5, p6);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 7 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7>
  class ParmState<R, TYPELIST_7(P1, P2, P3, P4, P5, P6, P7)>
  : public Private:ParmStateBase<R, TYPELIST_7(P1, P2, P3, P4, P5, P6, P7)>
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P7>::ReferencedType Parm7;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;
    typedef typename nType::TypeTraits<P7>::ParameterType oParm7;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6, oParm7 _p7)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
             oParm6 _p6, oParm7 _p7)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      p7 = _p7;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6, p7);
      }
      return f(p1, p2, p3, p4, p5, p6, p7);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
    Parm7 p7;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 8 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8>
  class ParmState<R, TYPELIST_8(P1, P2, P3, P4, P5, P6, P7, P8)>
  : public Private:ParmStateBase<R, TYPELIST_8(P1, P2, P3, P4, P5, P6, P7,
                                               P8)>
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P7>::ReferencedType Parm7;
    typedef typename nType::TypeTraits<P8>::ReferencedType Parm8;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;
    typedef typename nType::TypeTraits<P7>::ParameterType oParm7;
    typedef typename nType::TypeTraits<P8>::ParameterType oParm8;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6, oParm7 _p7, oParm8 _p8)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7), p8(_p8)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
             oParm6 _p6, oParm7 _p7, oParm8 _p8)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      p7 = _p7;
      p8 = _p8;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6, p7, p8);
      }
      return f(p1, p2, p3, p4, p5, p6, p7, p8);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
    Parm7 p7;
    Parm8 p8;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 9 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9>
  class ParmState<R, TYPELIST_9(P1, P2, P3, P4, P5, P6, P7, P8, P9)>
  : public Private:ParmStateBase<R, TYPELIST_9(P1, P2, P3, P4, P5, P6, P7,
                                               P8, P9)>
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P7>::ReferencedType Parm7;
    typedef typename nType::TypeTraits<P8>::ReferencedType Parm8;
    typedef typename nType::TypeTraits<P9>::ReferencedType Parm9;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;
    typedef typename nType::TypeTraits<P7>::ParameterType oParm7;
    typedef typename nType::TypeTraits<P8>::ParameterType oParm8;
    typedef typename nType::TypeTraits<P9>::ParameterType oParm9;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7), p8(_p8),
      p9(_p9)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
             oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      p7 = _p7;
      p8 = _p8;
      p9 = _p9;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9);
      }
      return f(p1, p2, p3, p4, p5, p6, p7, p8, p9);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
    Parm7 p7;
    Parm8 p8;
    Parm9 p9;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 10 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10>
  class ParmState<R, TYPELIST_10(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)>
  : public Private:ParmStateBase<R, TYPELIST_10(P1, P2, P3, P4, P5, P6, P7,
                                                P8, P9, P10)>
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P7>::ReferencedType Parm7;
    typedef typename nType::TypeTraits<P8>::ReferencedType Parm8;
    typedef typename nType::TypeTraits<P9>::ReferencedType Parm9;
    typedef typename nType::TypeTraits<P10>::ReferencedType Parm10;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;
    typedef typename nType::TypeTraits<P7>::ParameterType oParm7;
    typedef typename nType::TypeTraits<P8>::ParameterType oParm8;
    typedef typename nType::TypeTraits<P9>::ParameterType oParm9;
    typedef typename nType::TypeTraits<P10>::ParameterType oParm10;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7), p8(_p8),
      p9(_p9), p10(_p10)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
             oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      p7 = _p7;
      p8 = _p8;
      p9 = _p9;
      p10 = _p10;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
      }
      return f(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
    Parm7 p7;
    Parm8 p8;
    Parm9 p9;
    Parm10 p10;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 11 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11>
  class ParmState<R, TYPELIST_11(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10,
                                     P11)>
  : public Private:ParmStateBase<R, TYPELIST_11(P1, P2, P3, P4, P5, P6, P7,
                                                P8, P9, P10, P11)>
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P7>::ReferencedType Parm7;
    typedef typename nType::TypeTraits<P8>::ReferencedType Parm8;
    typedef typename nType::TypeTraits<P9>::ReferencedType Parm9;
    typedef typename nType::TypeTraits<P10>::ReferencedType Parm10;
    typedef typename nType::TypeTraits<P11>::ReferencedType Parm11;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;
    typedef typename nType::TypeTraits<P7>::ParameterType oParm7;
    typedef typename nType::TypeTraits<P8>::ParameterType oParm8;
    typedef typename nType::TypeTraits<P9>::ParameterType oParm9;
    typedef typename nType::TypeTraits<P10>::ParameterType oParm10;
    typedef typename nType::TypeTraits<P11>::ParameterType oParm11;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
              oParm11 _p11)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7), p8(_p8),
      p9(_p9), p10(_p10), p11(_p11)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
             oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
             oParm11 _p11)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      p7 = _p7;
      p8 = _p8;
      p9 = _p9;
      p10 = _p10;
      p11 = _p11;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
      }
      return f(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
    Parm7 p7;
    Parm8 p8;
    Parm9 p9;
    Parm10 p10;
    Parm11 p11;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 12 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11, typename P12>
  class ParmState<R, TYPELIST_12(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10,
                                     P11, P12)>
  : public Private:ParmStateBase<R, TYPELIST_12(P1, P2, P3, P4, P5, P6, P7,
                                                P8, P9, P10, P11, P12)>
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P7>::ReferencedType Parm7;
    typedef typename nType::TypeTraits<P8>::ReferencedType Parm8;
    typedef typename nType::TypeTraits<P9>::ReferencedType Parm9;
    typedef typename nType::TypeTraits<P10>::ReferencedType Parm10;
    typedef typename nType::TypeTraits<P11>::ReferencedType Parm11;
    typedef typename nType::TypeTraits<P12>::ReferencedType Parm12;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;
    typedef typename nType::TypeTraits<P7>::ParameterType oParm7;
    typedef typename nType::TypeTraits<P8>::ParameterType oParm8;
    typedef typename nType::TypeTraits<P9>::ParameterType oParm9;
    typedef typename nType::TypeTraits<P10>::ParameterType oParm10;
    typedef typename nType::TypeTraits<P11>::ParameterType oParm11;
    typedef typename nType::TypeTraits<P12>::ParameterType oParm12;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
              oParm11 _p11, oParm12 _p12)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7), p8(_p8),
      p9(_p9), p10(_p10), p11(_p11), p12(_p12)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
             oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
             oParm11 _p11, oParm12 _p12)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      p7 = _p7;
      p8 = _p8;
      p9 = _p9;
      p10 = _p10;
      p11 = _p11;
      p12 = _p12;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
      }
      return f(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
    Parm7 p7;
    Parm8 p8;
    Parm9 p9;
    Parm10 p10;
    Parm11 p11;
    Parm12 p12;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 13 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11, typename P12, typename P13>
  class ParmState<R, TYPELIST_13(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10,
                                     P11, P12, P13)>
  : public Private:ParmStateBase<R, TYPELIST_13(P1, P2, P3, P4, P5, P6, P7,
                                                P8, P9, P10, P11, P12, P13)>
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P7>::ReferencedType Parm7;
    typedef typename nType::TypeTraits<P8>::ReferencedType Parm8;
    typedef typename nType::TypeTraits<P9>::ReferencedType Parm9;
    typedef typename nType::TypeTraits<P10>::ReferencedType Parm10;
    typedef typename nType::TypeTraits<P11>::ReferencedType Parm11;
    typedef typename nType::TypeTraits<P12>::ReferencedType Parm12;
    typedef typename nType::TypeTraits<P13>::ReferencedType Parm13;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;
    typedef typename nType::TypeTraits<P7>::ParameterType oParm7;
    typedef typename nType::TypeTraits<P8>::ParameterType oParm8;
    typedef typename nType::TypeTraits<P9>::ParameterType oParm9;
    typedef typename nType::TypeTraits<P10>::ParameterType oParm10;
    typedef typename nType::TypeTraits<P11>::ParameterType oParm11;
    typedef typename nType::TypeTraits<P12>::ParameterType oParm12;
    typedef typename nType::TypeTraits<P13>::ParameterType oParm13;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
              oParm11 _p11, oParm12 _p12, oParm13 _p13)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7), p8(_p8),
      p9(_p9), p10(_p10), p11(_p11), p12(_p12), p13(_p13)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
             oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
             oParm11 _p11, oParm12 _p12, oParm13 _p13)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      p7 = _p7;
      p8 = _p8;
      p9 = _p9;
      p10 = _p10;
      p11 = _p11;
      p12 = _p12;
      p13 = _p13;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
      }
      return f(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
    Parm7 p7;
    Parm8 p8;
    Parm9 p9;
    Parm10 p10;
    Parm11 p11;
    Parm12 p12;
    Parm13 p13;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 14 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11, typename P12, typename P13,
            typename P14>
  class ParmState<R, TYPELIST_14(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10,
                                     P11, P12, P13, P14)>
  : public Private:ParmStateBase<R, TYPELIST_14(P1, P2, P3, P4, P5, P6, P7,
                                                P8, P9, P10, P11, P12, P13,
                                                P14)>
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P7>::ReferencedType Parm7;
    typedef typename nType::TypeTraits<P8>::ReferencedType Parm8;
    typedef typename nType::TypeTraits<P9>::ReferencedType Parm9;
    typedef typename nType::TypeTraits<P10>::ReferencedType Parm10;
    typedef typename nType::TypeTraits<P11>::ReferencedType Parm11;
    typedef typename nType::TypeTraits<P12>::ReferencedType Parm12;
    typedef typename nType::TypeTraits<P13>::ReferencedType Parm13;
    typedef typename nType::TypeTraits<P14>::ReferencedType Parm14;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;
    typedef typename nType::TypeTraits<P7>::ParameterType oParm7;
    typedef typename nType::TypeTraits<P8>::ParameterType oParm8;
    typedef typename nType::TypeTraits<P9>::ParameterType oParm9;
    typedef typename nType::TypeTraits<P10>::ParameterType oParm10;
    typedef typename nType::TypeTraits<P11>::ParameterType oParm11;
    typedef typename nType::TypeTraits<P12>::ParameterType oParm12;
    typedef typename nType::TypeTraits<P13>::ParameterType oParm13;
    typedef typename nType::TypeTraits<P14>::ParameterType oParm14;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
              oParm11 _p11, oParm12 _p12, oParm13 _p13, oParm14 _p14)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7), p8(_p8),
      p9(_p9), p10(_p10), p11(_p11), p12(_p12), p13(_p13), p14(_p14)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
             oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
             oParm11 _p11, oParm12 _p12, oParm13 _p13, oParm14 _p14)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      p7 = _p7;
      p8 = _p8;
      p9 = _p9;
      p10 = _p10;
      p11 = _p11;
      p12 = _p12;
      p13 = _p13;
      p14 = _p14;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13,
                    p14);
      }
      return f(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
    Parm7 p7;
    Parm8 p8;
    Parm9 p9;
    Parm10 p10;
    Parm11 p11;
    Parm12 p12;
    Parm13 p13;
    Parm14 p14;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 15 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11, typename P12, typename P13,
            typename P14, typename P15>
  class ParmState<R, TYPELIST_15(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10,
                                     P11, P12, P13, P14, P15)>
  : public Private:ParmStateBase<R, TYPELIST_15(P1, P2, P3, P4, P5, P6, P7,
                                                P8, P9, P10, P11, P12, P13,
                                                P14, P15)>
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P7>::ReferencedType Parm7;
    typedef typename nType::TypeTraits<P8>::ReferencedType Parm8;
    typedef typename nType::TypeTraits<P9>::ReferencedType Parm9;
    typedef typename nType::TypeTraits<P10>::ReferencedType Parm10;
    typedef typename nType::TypeTraits<P11>::ReferencedType Parm11;
    typedef typename nType::TypeTraits<P12>::ReferencedType Parm12;
    typedef typename nType::TypeTraits<P13>::ReferencedType Parm13;
    typedef typename nType::TypeTraits<P14>::ReferencedType Parm14;
    typedef typename nType::TypeTraits<P15>::ReferencedType Parm15;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;
    typedef typename nType::TypeTraits<P7>::ParameterType oParm7;
    typedef typename nType::TypeTraits<P8>::ParameterType oParm8;
    typedef typename nType::TypeTraits<P9>::ParameterType oParm9;
    typedef typename nType::TypeTraits<P10>::ParameterType oParm10;
    typedef typename nType::TypeTraits<P11>::ParameterType oParm11;
    typedef typename nType::TypeTraits<P12>::ParameterType oParm12;
    typedef typename nType::TypeTraits<P13>::ParameterType oParm13;
    typedef typename nType::TypeTraits<P14>::ParameterType oParm14;
    typedef typename nType::TypeTraits<P15>::ParameterType oParm15;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
              oParm11 _p11, oParm12 _p12, oParm13 _p13, oParm14 _p14,
              oParm15 _p15)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7), p8(_p8),
      p9(_p9), p10(_p10), p11(_p11), p12(_p12), p13(_p13), p14(_p14), p15(_p15)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
             oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
             oParm11 _p11, oParm12 _p12, oParm13 _p13, oParm14 _p14,
             oParm15 _p15)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      p7 = _p7;
      p8 = _p8;
      p9 = _p9;
      p10 = _p10;
      p11 = _p11;
      p12 = _p12;
      p13 = _p13;
      p14 = _p14;
      p15 = _p15;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13,
                    p14, p15);
      }
      return f(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
               p15);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
    Parm7 p7;
    Parm8 p8;
    Parm9 p9;
    Parm10 p10;
    Parm11 p11;
    Parm12 p12;
    Parm13 p13;
    Parm14 p14;
    Parm15 p15;
  };

  /*** END Recursive(Typelist) Specialization ****************************/

  /*** Linear(Seq) Specialization ****************************************/

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 1 parameter
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1>
  class ParmState<R, Seq<P1> >
  : public Private:ParmStateBase<R, Seq<P1> >
  {
  public:
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;

    ParmState(oParm1 _p1) : p1(_p1)
    {
      return;
    }

    void Set(oParm1 _p1)
    {
      p1 = _p1;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) { return (*f)(p1); }
      return f(p1);
    }

  private:
    Parm1 p1;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 2 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2>
  class ParmState<R, Seq<P1, P2> >
  : public Private:ParmStateBase<R, Seq<P1, P2> >
  {
  public:
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;

    ParmState(oParm1 _p1, oParm2 _p2) : p1(_p1), p2(_p2)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2)
    {
      p1 = _p1;
      p2 = _p2;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) { return (*f)(p1, p2); }
      return f(p1, p2);
    }

  private:
    Parm1 p1;
    Parm2 p2;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 3 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3>
  class ParmState<R, Seq<P1, P2, P3> >
  : public Private:ParmStateBase<R, Seq<P1, P2, P3> >
  {
  public:
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3) : p1(_p1), p2(_p2), p3(_p3)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) { return (*f)(p1, p2, p3); }
      return f(p1, p2, p3);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 4 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4>
  class ParmState<R, Seq<P1, P2, P3, P4> >
  : public Private:ParmStateBase<R, Seq<P1, P2, P3, P4> >
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4);
      }
      return f(p1, p2, p3, p4);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 5 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5>
  class ParmState<R, Seq<P1, P2, P3, P4, P5> >
  : public Private:ParmStateBase<R, Seq<P1, P2, P3, P4, P5> >
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5);
      }
      return f(p1, p2, p3, p4, p5);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 6 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6>
  class ParmState<R, Seq<P1, P2, P3, P4, P5, P6> >
  : public Private:ParmStateBase<R, Seq<P1, P2, P3, P4, P5, P6> >
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6)
    {
      return;
    }

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6);
      }
      return f(p1, p2, p3, p4, p5, p6);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 7 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7>
  class ParmState<R, Seq<P1, P2, P3, P4, P5, P6, P7> >
  : public Private:ParmStateBase<R, Seq<P1, P2, P3, P4, P5, P6, P7> >
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P7>::ReferencedType Parm7;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;
    typedef typename nType::TypeTraits<P7>::ParameterType oParm7;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6, oParm7 _p7)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
             oParm6 _p6, oParm7 _p7)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      p7 = _p7;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6, p7);
      }
      return f(p1, p2, p3, p4, p5, p6, p7);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
    Parm7 p7;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 8 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8>
  class ParmState<R, Seq<P1, P2, P3, P4, P5, P6, P7, P8> >
  : public Private:ParmStateBase<R, Seq<P1, P2, P3, P4, P5, P6, P7, P8> >
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P7>::ReferencedType Parm7;
    typedef typename nType::TypeTraits<P8>::ReferencedType Parm8;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;
    typedef typename nType::TypeTraits<P7>::ParameterType oParm7;
    typedef typename nType::TypeTraits<P8>::ParameterType oParm8;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6, oParm7 _p7, oParm8 _p8)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7), p8(_p8)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
             oParm6 _p6, oParm7 _p7, oParm8 _p8)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      p7 = _p7;
      p8 = _p8;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6, p7, p8);
      }
      return f(p1, p2, p3, p4, p5, p6, p7, p8);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
    Parm7 p7;
    Parm8 p8;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 9 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9>
  class ParmState<R, Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9> >
  : public Private:ParmStateBase<R, Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9> >
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P7>::ReferencedType Parm7;
    typedef typename nType::TypeTraits<P8>::ReferencedType Parm8;
    typedef typename nType::TypeTraits<P9>::ReferencedType Parm9;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;
    typedef typename nType::TypeTraits<P7>::ParameterType oParm7;
    typedef typename nType::TypeTraits<P8>::ParameterType oParm8;
    typedef typename nType::TypeTraits<P9>::ParameterType oParm9;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7), p8(_p8),
      p9(_p9)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
             oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      p7 = _p7;
      p8 = _p8;
      p9 = _p9;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9);
      }
      return f(p1, p2, p3, p4, p5, p6, p7, p8, p9);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
    Parm7 p7;
    Parm8 p8;
    Parm9 p9;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 10 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10>
  class ParmState<R, Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10> >
  : public Private:ParmStateBase<R, Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9,
                                        P10> >
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P7>::ReferencedType Parm7;
    typedef typename nType::TypeTraits<P8>::ReferencedType Parm8;
    typedef typename nType::TypeTraits<P9>::ReferencedType Parm9;
    typedef typename nType::TypeTraits<P10>::ReferencedType Parm10;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;
    typedef typename nType::TypeTraits<P7>::ParameterType oParm7;
    typedef typename nType::TypeTraits<P8>::ParameterType oParm8;
    typedef typename nType::TypeTraits<P9>::ParameterType oParm9;
    typedef typename nType::TypeTraits<P10>::ParameterType oParm10;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7), p8(_p8),
      p9(_p9), p10(_p10)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
             oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      p7 = _p7;
      p8 = _p8;
      p9 = _p9;
      p10 = _p10;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
      }
      return f(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
    Parm7 p7;
    Parm8 p8;
    Parm9 p9;
    Parm10 p10;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 11 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11>
  class ParmState<R, Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11> >
  : public Private:ParmStateBase<R, Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9,
                                        P10, P11> >
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P7>::ReferencedType Parm7;
    typedef typename nType::TypeTraits<P8>::ReferencedType Parm8;
    typedef typename nType::TypeTraits<P9>::ReferencedType Parm9;
    typedef typename nType::TypeTraits<P10>::ReferencedType Parm10;
    typedef typename nType::TypeTraits<P11>::ReferencedType Parm11;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;
    typedef typename nType::TypeTraits<P7>::ParameterType oParm7;
    typedef typename nType::TypeTraits<P8>::ParameterType oParm8;
    typedef typename nType::TypeTraits<P9>::ParameterType oParm9;
    typedef typename nType::TypeTraits<P10>::ParameterType oParm10;
    typedef typename nType::TypeTraits<P11>::ParameterType oParm11;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
              oParm11 _p11)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7), p8(_p8),
      p9(_p9), p10(_p10), p11(_p11)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
             oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
             oParm11 _p11)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      p7 = _p7;
      p8 = _p8;
      p9 = _p9;
      p10 = _p10;
      p11 = _p11;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
      }
      return f(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
    Parm7 p7;
    Parm8 p8;
    Parm9 p9;
    Parm10 p10;
    Parm11 p11;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 12 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11, typename P12>
  class ParmState<R, Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12> >
  : public Private:ParmStateBase<R, Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9,
                                        P10, P11, P12> >
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P7>::ReferencedType Parm7;
    typedef typename nType::TypeTraits<P8>::ReferencedType Parm8;
    typedef typename nType::TypeTraits<P9>::ReferencedType Parm9;
    typedef typename nType::TypeTraits<P10>::ReferencedType Parm10;
    typedef typename nType::TypeTraits<P11>::ReferencedType Parm11;
    typedef typename nType::TypeTraits<P12>::ReferencedType Parm12;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;
    typedef typename nType::TypeTraits<P7>::ParameterType oParm7;
    typedef typename nType::TypeTraits<P8>::ParameterType oParm8;
    typedef typename nType::TypeTraits<P9>::ParameterType oParm9;
    typedef typename nType::TypeTraits<P10>::ParameterType oParm10;
    typedef typename nType::TypeTraits<P11>::ParameterType oParm11;
    typedef typename nType::TypeTraits<P12>::ParameterType oParm12;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
              oParm11 _p11, oParm12 _p12)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7), p8(_p8),
      p9(_p9), p10(_p10), p11(_p11), p12(_p12)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
             oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
             oParm11 _p11, oParm12 _p12)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      p7 = _p7;
      p8 = _p8;
      p9 = _p9;
      p10 = _p10;
      p11 = _p11;
      p12 = _p12;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
      }
      return f(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
    Parm7 p7;
    Parm8 p8;
    Parm9 p9;
    Parm10 p10;
    Parm11 p11;
    Parm12 p12;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 13 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11, typename P12, typename P13>
  class ParmState<R, Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12,
                         P13> >
  : public Private:ParmStateBase<R, Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9,
                                        P10, P11, P12, P13> >
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P7>::ReferencedType Parm7;
    typedef typename nType::TypeTraits<P8>::ReferencedType Parm8;
    typedef typename nType::TypeTraits<P9>::ReferencedType Parm9;
    typedef typename nType::TypeTraits<P10>::ReferencedType Parm10;
    typedef typename nType::TypeTraits<P11>::ReferencedType Parm11;
    typedef typename nType::TypeTraits<P12>::ReferencedType Parm12;
    typedef typename nType::TypeTraits<P13>::ReferencedType Parm13;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;
    typedef typename nType::TypeTraits<P7>::ParameterType oParm7;
    typedef typename nType::TypeTraits<P8>::ParameterType oParm8;
    typedef typename nType::TypeTraits<P9>::ParameterType oParm9;
    typedef typename nType::TypeTraits<P10>::ParameterType oParm10;
    typedef typename nType::TypeTraits<P11>::ParameterType oParm11;
    typedef typename nType::TypeTraits<P12>::ParameterType oParm12;
    typedef typename nType::TypeTraits<P13>::ParameterType oParm13;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
              oParm11 _p11, oParm12 _p12, oParm13 _p13)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7), p8(_p8),
      p9(_p9), p10(_p10), p11(_p11), p12(_p12), p13(_p13)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
             oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
             oParm11 _p11, oParm12 _p12, oParm13 _p13)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      p7 = _p7;
      p8 = _p8;
      p9 = _p9;
      p10 = _p10;
      p11 = _p11;
      p12 = _p12;
      p13 = _p13;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
      }
      return f(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
    Parm7 p7;
    Parm8 p8;
    Parm9 p9;
    Parm10 p10;
    Parm11 p11;
    Parm12 p12;
    Parm13 p13;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 14 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11, typename P12, typename P13,
            typename P14>
  class ParmState<R, Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12,
                         P13, P14> >
  : public Private:ParmStateBase<R, Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9,
                                        P10, P11, P12, P13, P14> >
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P7>::ReferencedType Parm7;
    typedef typename nType::TypeTraits<P8>::ReferencedType Parm8;
    typedef typename nType::TypeTraits<P9>::ReferencedType Parm9;
    typedef typename nType::TypeTraits<P10>::ReferencedType Parm10;
    typedef typename nType::TypeTraits<P11>::ReferencedType Parm11;
    typedef typename nType::TypeTraits<P12>::ReferencedType Parm12;
    typedef typename nType::TypeTraits<P13>::ReferencedType Parm13;
    typedef typename nType::TypeTraits<P14>::ReferencedType Parm14;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;
    typedef typename nType::TypeTraits<P7>::ParameterType oParm7;
    typedef typename nType::TypeTraits<P8>::ParameterType oParm8;
    typedef typename nType::TypeTraits<P9>::ParameterType oParm9;
    typedef typename nType::TypeTraits<P10>::ParameterType oParm10;
    typedef typename nType::TypeTraits<P11>::ParameterType oParm11;
    typedef typename nType::TypeTraits<P12>::ParameterType oParm12;
    typedef typename nType::TypeTraits<P13>::ParameterType oParm13;
    typedef typename nType::TypeTraits<P14>::ParameterType oParm14;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
              oParm11 _p11, oParm12 _p12, oParm13 _p13, oParm14 _p14)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7), p8(_p8),
      p9(_p9), p10(_p10), p11(_p11), p12(_p12), p13(_p13), p14(_p14)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
             oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
             oParm11 _p11, oParm12 _p12, oParm13 _p13, oParm14 _p14)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      p7 = _p7;
      p8 = _p8;
      p9 = _p9;
      p10 = _p10;
      p11 = _p11;
      p12 = _p12;
      p13 = _p13;
      p14 = _p14;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13,
                    p14);
      }
      return f(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
    Parm7 p7;
    Parm8 p8;
    Parm9 p9;
    Parm10 p10;
    Parm11 p11;
    Parm12 p12;
    Parm13 p13;
    Parm14 p14;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template ParmState
  // Specialization for 15 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11, typename P12, typename P13,
            typename P14, typename P15>
  class ParmState<R, Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12,
                         P13, P14, P15> >
  : public Private:ParmStateBase<R, Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9,
                                        P10, P11, P12, P13, P14, P15> >
  {
    typedef typename nType::TypeTraits<P1>::ReferencedType Parm1;
    typedef typename nType::TypeTraits<P2>::ReferencedType Parm2;
    typedef typename nType::TypeTraits<P3>::ReferencedType Parm3;
    typedef typename nType::TypeTraits<P4>::ReferencedType Parm4;
    typedef typename nType::TypeTraits<P5>::ReferencedType Parm5;
    typedef typename nType::TypeTraits<P6>::ReferencedType Parm6;
    typedef typename nType::TypeTraits<P7>::ReferencedType Parm7;
    typedef typename nType::TypeTraits<P8>::ReferencedType Parm8;
    typedef typename nType::TypeTraits<P9>::ReferencedType Parm9;
    typedef typename nType::TypeTraits<P10>::ReferencedType Parm10;
    typedef typename nType::TypeTraits<P11>::ReferencedType Parm11;
    typedef typename nType::TypeTraits<P12>::ReferencedType Parm12;
    typedef typename nType::TypeTraits<P13>::ReferencedType Parm13;
    typedef typename nType::TypeTraits<P14>::ReferencedType Parm14;
    typedef typename nType::TypeTraits<P15>::ReferencedType Parm15;
    typedef typename nType::TypeTraits<P1>::ParameterType oParm1;
    typedef typename nType::TypeTraits<P2>::ParameterType oParm2;
    typedef typename nType::TypeTraits<P3>::ParameterType oParm3;
    typedef typename nType::TypeTraits<P4>::ParameterType oParm4;
    typedef typename nType::TypeTraits<P5>::ParameterType oParm5;
    typedef typename nType::TypeTraits<P6>::ParameterType oParm6;
    typedef typename nType::TypeTraits<P7>::ParameterType oParm7;
    typedef typename nType::TypeTraits<P8>::ParameterType oParm8;
    typedef typename nType::TypeTraits<P9>::ParameterType oParm9;
    typedef typename nType::TypeTraits<P10>::ParameterType oParm10;
    typedef typename nType::TypeTraits<P11>::ParameterType oParm11;
    typedef typename nType::TypeTraits<P12>::ParameterType oParm12;
    typedef typename nType::TypeTraits<P13>::ParameterType oParm13;
    typedef typename nType::TypeTraits<P14>::ParameterType oParm14;
    typedef typename nType::TypeTraits<P15>::ParameterType oParm15;

    ParmState(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
              oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
              oParm11 _p11, oParm12 _p12, oParm13 _p13, oParm14 _p14,
              oParm15 _p15)
    : p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7), p8(_p8),
      p9(_p9), p10(_p10), p11(_p11), p12(_p12), p13(_p13), p14(_p14), p15(_p15)
    {
      return;
    }

    void Set(oParm1 _p1, oParm2 _p2, oParm3 _p3, oParm4 _p4, oParm5 _p5,
             oParm6 _p6, oParm7 _p7, oParm8 _p8, oParm9 _p9, oParm10 _p10,
             oParm11 _p11, oParm12 _p12, oParm13 _p13, oParm14 _p14,
             oParm15 _p15)
    {
      p1 = _p1;
      p2 = _p2;
      p3 = _p3;
      p4 = _p4;
      p5 = _p5;
      p6 = _p6;
      p7 = _p7;
      p8 = _p8;
      p9 = _p9;
      p10 = _p10;
      p11 = _p11;
      p12 = _p12;
      p13 = _p13;
      p14 = _p14;
      p15 = _p15;
      return;
    }

    template <typename Fun> R operator()(Fun &f) {
      if(nType::TypeTraits<Fun>::isPointer) {
        return (*f)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13,
                    p14, p15);
      }
      return f(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
               p15);
    }

  private:
    Parm1 p1;
    Parm2 p2;
    Parm3 p3;
    Parm4 p4;
    Parm5 p5;
    Parm6 p6;
    Parm7 p7;
    Parm8 p8;
    Parm9 p9;
    Parm10 p10;
    Parm11 p11;
    Parm12 p12;
    Parm13 p13;
    Parm14 p14;
    Parm15 p15;
  };

  /*** END Linear(Seq) Specialization ************************************/

  /*** END SPECIALIZATIONS ***********************************************/

} // END namespace nFunctor

#endif
