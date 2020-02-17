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

// June 22, 2006 Jason Stredwick.  Made some modifications to the iFunctorBase
//   moved these class into their own file, made slight changes to each
//   class, and did some renaming.

#ifndef __Functor_Inteface_iFunctor_h__
#define __Functor_Inteface_iFunctor_h__

/*!
 * \class iFunctor
 *
 * \ingroup nFunctor
 *
 * iFunctor - iFunctor is the interface for all functors.  While there is a
 *   private iFunctorBase, iFunctor is the actual interface and iFunctorBase
 *   is not intended for use.  The iFunctorBase defines all possible types
 *   and common functions so that iFunctor can define only what is necessary.
 *   iFunctor is defined here as a series of template specializations, one
 *   recursive (Typelist) and one linear (Seq).  Each specialization defines
 *   two functions: a virtual destructor and a pure virtual operator() that
 *   has the correct number of arguements.  Each specialization defines the
 *   arguement types it uses in operator() as typedefs.  To reduce overhead
 *   the parameters may be redefined to optimize parameter passing using
 *   nType::TypeTraits<T>::ParameterType.  By converting, you relieve the
 *   cost of copying data until it is copied by the intended callable entity is
 *   executed.  For example a functor of a functor of a global function
 *   will pass by reference to the first function who will pass by reference
 *   to the global function, then because the global function types are
 *   not changed, the function will make a copy of any data passed by value.
 *
 * You may want to derive your own functors from iFunctor.
 *
 * Added a templateless base class that simply allows for the deletion of
 *   a functor through a virtual interface.  This class is called bFunctor.
 *
 * There are specializations of iFunctor up to 15 parameters follow.  To
 *   increase the number of arguements available to functors, one will
 *   have to increase the number of specializations and modify the
 *   iFunctorBase class.  Then the functor classes that are to be used will
 *   also need to be changed.
 *
 * A few notes on why the operator()s are const and what that means for you.
 *  What do we see here?  If you do not have const on the operator() for the
 *    functors, you can not wrap const callable entities.  However, it seems
 *    that while const operator() allows for correct enforcement of calling
 *    const methods for const entities, it also allows for calling non-const
 *    methods on non-const entities.  I did not think that the later was valid,
 *    but apparently it is.  I am going to leave it now for two reasons.  One
 *    I would have to add duplicates of all operator() for both a const and
 *    non-const version (how to handle volatile?), and 2) I did a quick test by
 *    adding both versions to iFunctor and Functor and it will call the
 *    appropriate version for a non-const Functor, but if the Functor itself is
 *    const we are right back at square one, calling non-const object's
 *    non-const member functions.  Hopefully, someone more knowledgable than I
 *    will figure this out, otherwise I will try to come back at some point and
 *    figure it out.  What would be nice is to make the functor's operator()s
 *    const/volatile reflect the wrapped entity's const/volatile settings.
 *    However this could be tricky for callable classes because you have to
 *    look at two places: the operator() and the constness of the class itself.
 *
 */

#include "Type/Interface/NullType.h"
#include "Type/Interface/EmptyType.h"
#include "Type/Interface/TypeTraits.h"
#include "Type/Interface/Typelist.h"
#include "Type/Interface/Sequence.h"

namespace nFunctor
{

  /*

   * class template iFunctor
   * The base class for a hierarchy of functors. The iFunctor class is
   *   supposed to be used directly; where a Functor class can cast to a
   *   iFunctor pointer or return an iFunctor pointer version of itself.
   * You may want to derive your own functors from iFunctor.
   * Specializations of iFunctor for up to 15 parameters follow.  To
   *   increase the number of arguements available to functors, one will
   *   have to increase the number of specializations and modify the
   *   iFunctorBase class.

   */

  // Forward declaration of iFunctor where R is the ReturnType and T is
  //   the list of arguement types.
  template <typename R, typename T> class iFunctor;

  // This is a templateless base class for all functors.  This can be useful
  //   in particular instances where heterogeneous storage of functors is
  //   is required, and deletion of those functors needs to occur without
  //   access to their template types.
  class bFunctor {
  public:
    virtual ~bFunctor(void) { return; }
  };

  namespace Private {
    // To increase the number of parameters available to the functors, one
    //   must modify iFunctorBase (see comments above) and add more
    //   specializations of iFunctor for the desired parameter combinations.

    // This class defines typedefs for all the possible parameters for a
    //   functor.  This is a convenience class that defines extra parameter
    //   types and interface functions that are common to all the iFunctors so
    //   they don't have to be added specifically, reducing clutter.

    // This class is not meant to be used as a base class pointer to the
    //   derived types.
    template <typename R, typename T> class iFunctorBase : public bFunctor {
    public:
      typedef iFunctor<R, T> Interface;
      typedef R              ResultType;
      typedef T              ArguementTypes;

      // These define all the possible parameters available for a functor.
      //   They are initially set to EmptyType so that the derived Functor
      //   class can inherit typedefs for all possible parameters, even those
      //   that are not in use for the particular Functor.  When the specific
      //   interface is determined it will overwrite the typedefs here that it
      //   defines itself, leaving the others empty.  Therefore, to increase
      //   the number of possible parameters for a functor, you must increase
      //   these typedefs.
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

    protected:
      // These are required for derived functors so they may also use default
      //   functionality.  They are protected so this class can not be
      //   instantiated or destroyed.
      iFunctorBase(void) { return; }
      iFunctorBase(const iFunctorBase &) { return; }
      iFunctorBase &operator=(const iFunctorBase &) { return *this; }
      virtual ~iFunctorBase(void) { return; }

    public:
      // This function is intended to be inherited and used by iFunctor to
      //   allow a program to make a copy of this functor if it only has
      //   access to its interface.
      Interface *Clone(void) const { return DoClone(); }

    private:
      // The actual cloning method.
      virtual Interface *DoClone(void) const = 0;
    };

  } // End namespace Private

  /*** SPECIALIZATIONS ***************************************************/
  // There are two types of specializations given here: recursive and
  //   linear, which use Typelist and Seq respectively.  Recursive types
  //   only define what they need, but can be difficult to write by hand
  //   and are more difficult to interpret than the linear version.  The
  //   linear version predefines a single Seq class with all 15 possible
  //   template parameters but gives them all default values.  This is an
  //   attractive method, which appears fine for this situation.

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 0 (zero) parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R>
  class iFunctor<R, nType::NullType>
  : public Private::iFunctorBase<R, nType::NullType>
  {
  public:
    virtual ~iFunctor(void) { return; }
    virtual R operator()(void) const = 0;
  };

  /*** Recursive(Typelist) Specialization ********************************/

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 1 parameter
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1>
  class iFunctor<R, TYPELIST_1(P1)>
  : public Private::iFunctorBase<R, TYPELIST_1(P1)>
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 2 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2>
  class iFunctor<R, TYPELIST_2(P1, P2)>
  : public Private::iFunctorBase<R, TYPELIST_2(P1, P2)>
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 3 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3>
  class iFunctor<R, TYPELIST_3(P1, P2, P3)>
  : public Private::iFunctorBase<R, TYPELIST_3(P1, P2, P3)>
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 4 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4>
  class iFunctor<R, TYPELIST_4(P1, P2, P3, P4)>
  : public Private::iFunctorBase<R, TYPELIST_4(P1, P2, P3, P4)>
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 5 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5>
  class iFunctor<R, TYPELIST_5(P1, P2, P3, P4, P5)>
  : public Private::iFunctorBase<R, TYPELIST_5(P1, P2, P3, P4, P5)>
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 6 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6>
  class iFunctor<R, TYPELIST_6(P1, P2, P3, P4, P5, P6)>
  : public Private::iFunctorBase<R, TYPELIST_6(P1, P2, P3, P4, P5, P6)>
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 7 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7>
  class iFunctor<R, TYPELIST_7(P1, P2, P3, P4, P5, P6, P7)>
  : public Private::iFunctorBase<R, TYPELIST_7(P1, P2, P3, P4, P5, P6,
                                                    P7)>
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;
    typedef typename nType::TypeTraits<P7>::ParameterType Parm7;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6, 
                         Parm7) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 8 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8>
  class iFunctor<R, TYPELIST_8(P1, P2, P3, P4, P5, P6, P7, P8)>
  : public Private::iFunctorBase<R, TYPELIST_8(P1, P2, P3, P4, P5, P6,
                                                    P7, P8)>
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;
    typedef typename nType::TypeTraits<P7>::ParameterType Parm7;
    typedef typename nType::TypeTraits<P8>::ParameterType Parm8;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6, 
                         Parm7, Parm8) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 9 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9>
  class iFunctor<R, TYPELIST_9(P1, P2, P3, P4, P5, P6, P7, P8, P9)>
  : public Private::iFunctorBase<R, TYPELIST_9(P1, P2, P3, P4, P5, P6,
                                                    P7, P8, P9)>
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;
    typedef typename nType::TypeTraits<P7>::ParameterType Parm7;
    typedef typename nType::TypeTraits<P8>::ParameterType Parm8;
    typedef typename nType::TypeTraits<P9>::ParameterType Parm9;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6, 
                         Parm7, Parm8, Parm9) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 10 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10>
  class iFunctor<R, TYPELIST_10(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)>
  : public Private::iFunctorBase<R, TYPELIST_10(P1, P2, P3, P4, P5, P6,
                                                     P7, P8, P9, P10)>
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;
    typedef typename nType::TypeTraits<P7>::ParameterType Parm7;
    typedef typename nType::TypeTraits<P8>::ParameterType Parm8;
    typedef typename nType::TypeTraits<P9>::ParameterType Parm9;
    typedef typename nType::TypeTraits<P10>::ParameterType Parm10;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6, 
                         Parm7, Parm8, Parm9, Parm10) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 11 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11>
  class iFunctor<R, TYPELIST_11(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10,
                                     P11)>
  : public Private::iFunctorBase<R, TYPELIST_11(P1, P2, P3, P4, P5, P6,
                                                     P7, P8, P9, P10, P11)>
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;
    typedef typename nType::TypeTraits<P7>::ParameterType Parm7;
    typedef typename nType::TypeTraits<P8>::ParameterType Parm8;
    typedef typename nType::TypeTraits<P9>::ParameterType Parm9;
    typedef typename nType::TypeTraits<P10>::ParameterType Parm10;
    typedef typename nType::TypeTraits<P11>::ParameterType Parm11;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6,
                         Parm7, Parm8, Parm9, Parm10, Parm11) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 12 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11, typename P12>
  class iFunctor<R, TYPELIST_12(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10,
                                     P11, P12)>
  : public Private::iFunctorBase<R, TYPELIST_12(P1, P2, P3, P4, P5, P6,
                                                     P7, P8, P9, P10, P11,
                                                     P12)>
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;
    typedef typename nType::TypeTraits<P7>::ParameterType Parm7;
    typedef typename nType::TypeTraits<P8>::ParameterType Parm8;
    typedef typename nType::TypeTraits<P9>::ParameterType Parm9;
    typedef typename nType::TypeTraits<P10>::ParameterType Parm10;
    typedef typename nType::TypeTraits<P11>::ParameterType Parm11;
    typedef typename nType::TypeTraits<P12>::ParameterType Parm12;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6, 
                         Parm7, Parm8, Parm9, Parm10, Parm11, Parm12) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 13 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11, typename P12, typename P13>
  class iFunctor<R, TYPELIST_13(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10,
                                     P11, P12, P13)>
  : public Private::iFunctorBase<R, TYPELIST_13(P1, P2, P3, P4, P5, P6,
                                                     P7, P8, P9, P10, P11,
                                                     P12, P13)>
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;
    typedef typename nType::TypeTraits<P7>::ParameterType Parm7;
    typedef typename nType::TypeTraits<P8>::ParameterType Parm8;
    typedef typename nType::TypeTraits<P9>::ParameterType Parm9;
    typedef typename nType::TypeTraits<P10>::ParameterType Parm10;
    typedef typename nType::TypeTraits<P11>::ParameterType Parm11;
    typedef typename nType::TypeTraits<P12>::ParameterType Parm12;
    typedef typename nType::TypeTraits<P13>::ParameterType Parm13;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6, 
                         Parm7, Parm8, Parm9, Parm10, Parm11, Parm12,
                         Parm13) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 14 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11, typename P12, typename P13,
            typename P14>
  class iFunctor<R, TYPELIST_14(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10,
                                     P11, P12, P13, P14)>
  : public Private::iFunctorBase<R, TYPELIST_14(P1, P2, P3, P4, P5, P6,
                                                     P7, P8, P9, P10, P11,
                                                     P12, P13, P14)>
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;
    typedef typename nType::TypeTraits<P7>::ParameterType Parm7;
    typedef typename nType::TypeTraits<P8>::ParameterType Parm8;
    typedef typename nType::TypeTraits<P9>::ParameterType Parm9;
    typedef typename nType::TypeTraits<P10>::ParameterType Parm10;
    typedef typename nType::TypeTraits<P11>::ParameterType Parm11;
    typedef typename nType::TypeTraits<P12>::ParameterType Parm12;
    typedef typename nType::TypeTraits<P13>::ParameterType Parm13;
    typedef typename nType::TypeTraits<P14>::ParameterType Parm14;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6, 
                         Parm7, Parm8, Parm9, Parm10, Parm11, Parm12,
                         Parm13, Parm14) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 15 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11, typename P12, typename P13,
            typename P14, typename P15>
  class iFunctor<R, TYPELIST_15(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10,
                                     P11, P12, P13, P14, P15)>
  : public Private::iFunctorBase<R, TYPELIST_15(P1, P2, P3, P4, P5, P6,
                                                     P7, P8, P9, P10, P11,
                                                     P12, P13, P14, P15)>
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;
    typedef typename nType::TypeTraits<P7>::ParameterType Parm7;
    typedef typename nType::TypeTraits<P8>::ParameterType Parm8;
    typedef typename nType::TypeTraits<P9>::ParameterType Parm9;
    typedef typename nType::TypeTraits<P10>::ParameterType Parm10;
    typedef typename nType::TypeTraits<P11>::ParameterType Parm11;
    typedef typename nType::TypeTraits<P12>::ParameterType Parm12;
    typedef typename nType::TypeTraits<P13>::ParameterType Parm13;
    typedef typename nType::TypeTraits<P14>::ParameterType Parm14;
    typedef typename nType::TypeTraits<P15>::ParameterType Parm15;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6, 
                         Parm7, Parm8, Parm9, Parm10, Parm11, Parm12,
                         Parm13, Parm14, Parm15) const = 0;
  };

  /*** END Recursive(Typelist) Specialization ****************************/

  /*** Linear(Seq) Specialization ****************************************/

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 1 parameter
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1>
  class iFunctor<R, nType::Seq<P1> >
  : public Private::iFunctorBase<R, nType::Seq<P1> >
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 2 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2>
  class iFunctor<R, nType::Seq<P1, P2> >
  : public Private::iFunctorBase<R, nType::Seq<P1, P2> >
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 3 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3>
  class iFunctor<R, nType::Seq<P1, P2, P3> >
  : public Private::iFunctorBase<R, nType::Seq<P1, P2, P3> >
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 4 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4>
  class iFunctor<R, nType::Seq<P1, P2, P3, P4> >
  : public Private::iFunctorBase<R, nType::Seq<P1, P2, P3, P4> >
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 5 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5>
  class iFunctor<R, nType::Seq<P1, P2, P3, P4, P5> >
  : public Private::iFunctorBase<R, nType::Seq<P1, P2, P3, P4, P5> >
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 6 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6>
  class iFunctor<R, nType::Seq<P1, P2, P3, P4, P5, P6> >
  : public Private::iFunctorBase<R, nType::Seq<P1, P2, P3, P4, P5, P6> >
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 7 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7>
  class iFunctor<R, nType::Seq<P1, P2, P3, P4, P5, P6, P7> >
  : public Private::iFunctorBase<R, nType::Seq<P1, P2, P3, P4, P5, P6,
                                                   P7> >
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;
    typedef typename nType::TypeTraits<P7>::ParameterType Parm7;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6, 
                         Parm7) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 8 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8>
  class iFunctor<R, nType::Seq<P1, P2, P3, P4, P5, P6, P7, P8> >
  : public Private::iFunctorBase<R, nType::Seq<P1, P2, P3, P4, P5, P6, P7,
                                                   P8> >
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;
    typedef typename nType::TypeTraits<P7>::ParameterType Parm7;
    typedef typename nType::TypeTraits<P8>::ParameterType Parm8;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6, 
                         Parm7, Parm8) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 9 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9>
  class iFunctor<R, nType::Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9> >
  : public Private::iFunctorBase<R, nType::Seq<P1, P2, P3, P4, P5, P6, P7,
                                                   P8, P9> >
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;
    typedef typename nType::TypeTraits<P7>::ParameterType Parm7;
    typedef typename nType::TypeTraits<P8>::ParameterType Parm8;
    typedef typename nType::TypeTraits<P9>::ParameterType Parm9;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6, 
                         Parm7, Parm8, Parm9) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 10 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10>
  class iFunctor<R, nType::Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10> >
  : public Private::iFunctorBase<R, nType::Seq<P1, P2, P3, P4, P5, P6, P7,
                                                   P8, P9, P10> >
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;
    typedef typename nType::TypeTraits<P7>::ParameterType Parm7;
    typedef typename nType::TypeTraits<P8>::ParameterType Parm8;
    typedef typename nType::TypeTraits<P9>::ParameterType Parm9;
    typedef typename nType::TypeTraits<P10>::ParameterType Parm10;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6, 
                         Parm7, Parm8, Parm9, Parm10) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 11 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11>
  class iFunctor<R, nType::Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10,
                                   P11> >
  : public Private::iFunctorBase<R, nType::Seq<P1, P2, P3, P4, P5, P6, P7,
                                                   P8, P9, P10, P11> >
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;
    typedef typename nType::TypeTraits<P7>::ParameterType Parm7;
    typedef typename nType::TypeTraits<P8>::ParameterType Parm8;
    typedef typename nType::TypeTraits<P9>::ParameterType Parm9;
    typedef typename nType::TypeTraits<P10>::ParameterType Parm10;
    typedef typename nType::TypeTraits<P11>::ParameterType Parm11;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6,
                         Parm7, Parm8, Parm9, Parm10, Parm11) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 12 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11, typename P12>
  class iFunctor<R, nType::Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10,
                                   P11, P12> >
  : public Private::iFunctorBase<R, nType::Seq<P1, P2, P3, P4, P5, P6, P7,
                                                   P8, P9, P10, P11, P12> >
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;
    typedef typename nType::TypeTraits<P7>::ParameterType Parm7;
    typedef typename nType::TypeTraits<P8>::ParameterType Parm8;
    typedef typename nType::TypeTraits<P9>::ParameterType Parm9;
    typedef typename nType::TypeTraits<P10>::ParameterType Parm10;
    typedef typename nType::TypeTraits<P11>::ParameterType Parm11;
    typedef typename nType::TypeTraits<P12>::ParameterType Parm12;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6, 
                         Parm7, Parm8, Parm9, Parm10, Parm11, Parm12) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 13 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11, typename P12, typename P13>
  class iFunctor<R, nType::Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10,
                                   P11, P12, P13> >
  : public Private::iFunctorBase<R, nType::Seq<P1, P2, P3, P4, P5, P6, P7,
                                                   P8, P9, P10, P11, P12,
                                                   P13> >
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;
    typedef typename nType::TypeTraits<P7>::ParameterType Parm7;
    typedef typename nType::TypeTraits<P8>::ParameterType Parm8;
    typedef typename nType::TypeTraits<P9>::ParameterType Parm9;
    typedef typename nType::TypeTraits<P10>::ParameterType Parm10;
    typedef typename nType::TypeTraits<P11>::ParameterType Parm11;
    typedef typename nType::TypeTraits<P12>::ParameterType Parm12;
    typedef typename nType::TypeTraits<P13>::ParameterType Parm13;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6, 
                         Parm7, Parm8, Parm9, Parm10, Parm11, Parm12,
                         Parm13) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 14 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11, typename P12, typename P13,
            typename P14>
  class iFunctor<R, nType::Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10,
                                   P11, P12, P13, P14> >
  : public Private::iFunctorBase<R, nType::Seq<P1, P2, P3, P4, P5, P6, P7,
                                                   P8, P9, P10, P11, P12, P13,
                                                   P14> >
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;
    typedef typename nType::TypeTraits<P7>::ParameterType Parm7;
    typedef typename nType::TypeTraits<P8>::ParameterType Parm8;
    typedef typename nType::TypeTraits<P9>::ParameterType Parm9;
    typedef typename nType::TypeTraits<P10>::ParameterType Parm10;
    typedef typename nType::TypeTraits<P11>::ParameterType Parm11;
    typedef typename nType::TypeTraits<P12>::ParameterType Parm12;
    typedef typename nType::TypeTraits<P13>::ParameterType Parm13;
    typedef typename nType::TypeTraits<P14>::ParameterType Parm14;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6, 
                         Parm7, Parm8, Parm9, Parm10, Parm11, Parm12,
                         Parm13, Parm14) const = 0;
  };

  /////////////////////////////////////////////////////////////////////////
  // class template iFunctor
  // Specialization for 15 parameters
  /////////////////////////////////////////////////////////////////////////

  template <typename R, typename P1, typename P2, typename P3, typename P4,
            typename P5, typename P6, typename P7, typename P8, typename P9,
            typename P10, typename P11, typename P12, typename P13,
            typename P14, typename P15>
  class iFunctor<R, nType::Seq<P1, P2, P3, P4, P5, P6, P7, P8, P9, P10,
                                   P11, P12, P13, P14, P15> >
  : public Private::iFunctorBase<R, nType::Seq<P1, P2, P3, P4, P5, P6, P7,
                                                   P8, P9, P10, P11, P12, P13,
                                                   P14, P15> >
  {
  public:
    typedef typename nType::TypeTraits<P1>::ParameterType Parm1;
    typedef typename nType::TypeTraits<P2>::ParameterType Parm2;
    typedef typename nType::TypeTraits<P3>::ParameterType Parm3;
    typedef typename nType::TypeTraits<P4>::ParameterType Parm4;
    typedef typename nType::TypeTraits<P5>::ParameterType Parm5;
    typedef typename nType::TypeTraits<P6>::ParameterType Parm6;
    typedef typename nType::TypeTraits<P7>::ParameterType Parm7;
    typedef typename nType::TypeTraits<P8>::ParameterType Parm8;
    typedef typename nType::TypeTraits<P9>::ParameterType Parm9;
    typedef typename nType::TypeTraits<P10>::ParameterType Parm10;
    typedef typename nType::TypeTraits<P11>::ParameterType Parm11;
    typedef typename nType::TypeTraits<P12>::ParameterType Parm12;
    typedef typename nType::TypeTraits<P13>::ParameterType Parm13;
    typedef typename nType::TypeTraits<P14>::ParameterType Parm14;
    typedef typename nType::TypeTraits<P15>::ParameterType Parm15;

    virtual ~iFunctor(void) { return; }
    virtual R operator()(Parm1, Parm2, Parm3, Parm4, Parm5, Parm6, 
                         Parm7, Parm8, Parm9, Parm10, Parm11, Parm12,
                         Parm13, Parm14, Parm15) const = 0;
  };

  /*** END Linear(Seq) Specialization ************************************/

  /*** END SPECIALIZATIONS ***********************************************/

} // END namespace nFunctor

#endif
