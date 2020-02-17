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

#ifndef __Functor_Inteface_BindFirst_h__
#define __Functor_Inteface_BindFirst_h__

/*!
 * \class BinderFirst
 * \ingroup nFunctor
 *
 * BinderFirst -The BindersFirst assumes that the incoming original callable
 *   entity will be executed as is from within the BinderFirst.  Thus it
 *   removes the first parameter from its interface so it will be called
 *   with one less parameter.  Then the bound item will be put at the front
 *   of the list of parameters proceeding to the original callable entity.
 *
 */

#include <memory> // std::auto_ptr

#include "Type/Interface/NullType.h"
#include "Type/Interface/Typelist.h"
#include "Type/Interface/TypeManip.h"
#include "Type/Interface/ReassignableType.h"
#include "Type/Interface/PtrAccessibleType.h"
#include "Functor/Interface/Exceptions.h"
#include "Functor/Interface/iFunctor.h"

namespace nFunctor
{

  // Forward declaration of the BinderFirst class.
  template <typename Fun, typename R, class TList> class BinderFirst;

  // This private class is a helper that converts the incoming arguement list
  //   into a new list with the first arguement removed, called ParmList.  The
  //   original first arguement's type is stored as OriginalParm1.
  namespace Private
  {

    template <class TList> struct BinderFirstTraits
    {
      typedef typename nType::TL::TypeAt<TList, 0>::Result OriginalParm1;

      // Remove the first parameter from TList and call the new list ParmList
      typedef typename nType::TL::Erase<TList,
        typename nType::TL::TypeAt<TList, 0>::Result>::Result ParmList;

      typedef typename nType::ReassignableType<OriginalParm1>::ParameterType
        OptBoundType;
    };

  } // namespace Private

  // Forward declarations of the helper functions that create BinderFirst.

  // Was this intended to make a copy of the functor in BinderFirst or
  //   was it just to hold the pointer to the functor?  I am assuming the
  //   later, so it makes more sense to store the interface pointer now.
  template <typename Fun, typename R, typename TList>
  BinderFirst<Fun, R, TList> *BindFirst(
    typename nType::PtrAccessibleType<Fun>::ParameterType,
    typename Private::BinderFirstTraits<TList>::OptBoundType);

  // Allows for any type of functor who has a iFunctor<R,TList> interface so
  //   that the template arguements on BindFirst are not required because they
  //   can be deduced where they can not be deduced with the above function.
  template <typename R, typename TList>
  BinderFirst<iFunctor<R, TList> *, R, TList> *BindFirst(iFunctor<R, TList> *,
    typename Private::BinderFirstTraits<TList>::OptBoundType);

  // BinderFirst
  //   Binds the first parameter of a callabe entity to a specific value.
  template <typename Fun, typename R, class TList>
  class BinderFirst
  : public iFunctor<R, typename Private::BinderFirstTraits<TList>::ParmList>
  {
  public:
    // Useful type information
    typedef typename Private::BinderFirstTraits<TList> TTraits;
    typedef iFunctor<R, TList>                         OriginalInterface;

    typedef typename iFunctor<R, typename TTraits::ParmList>::Interface
      Interface;
    typedef typename iFunctor<R, typename TTraits::ParmList>::ResultType
      ResultType;
    typedef typename iFunctor<R, typename TTraits::ParmList>::ArguementTypes
      ArguementTypes;
    typedef typename iFunctor<R, typename TTraits::ParmList>::Parm1 Parm1;
    typedef typename iFunctor<R, typename TTraits::ParmList>::Parm2 Parm2;
    typedef typename iFunctor<R, typename TTraits::ParmList>::Parm3 Parm3;
    typedef typename iFunctor<R, typename TTraits::ParmList>::Parm4 Parm4;
    typedef typename iFunctor<R, typename TTraits::ParmList>::Parm5 Parm5;
    typedef typename iFunctor<R, typename TTraits::ParmList>::Parm6 Parm6;
    typedef typename iFunctor<R, typename TTraits::ParmList>::Parm7 Parm7;
    typedef typename iFunctor<R, typename TTraits::ParmList>::Parm8 Parm8;
    typedef typename iFunctor<R, typename TTraits::ParmList>::Parm9 Parm9;
    typedef typename iFunctor<R, typename TTraits::ParmList>::Parm10 Parm10;
    typedef typename iFunctor<R, typename TTraits::ParmList>::Parm11 Parm11;
    typedef typename iFunctor<R, typename TTraits::ParmList>::Parm12 Parm12;
    typedef typename iFunctor<R, typename TTraits::ParmList>::Parm13 Parm13;
    typedef typename iFunctor<R, typename TTraits::ParmList>::Parm14 Parm14;
    typedef typename iFunctor<R, typename TTraits::ParmList>::Parm15 Parm15;

    typedef typename nType::PtrAccessibleType<Fun> Data;
    typedef typename Data::ParameterType             OptFun;
    typedef typename Data::StorageType               DataValue;
    typedef typename TTraits::OriginalParm1          BoundType;
    typedef typename TTraits::OptBoundType           OptBoundType;

    // Fun can be an object, reference, or pointer to a callable object.  This
    //   uses default copy constructor, assignment, and destructor.
    // A pointer is used for the incoming object, a BadCallableEntity exception
    //   will be thrown if it is null.
    BinderFirst(OptFun fun, OptBoundType b) : d_f(fun), d_b(b) {
      if(!d_f()) { throw BadCallableEntity(); }
      return;
    }

    void SetFun(OptFun fun) {
      d_f = fun;
      if(!d_f()) { throw BadCallableEntity(); }
      return;
    }
    void SetBound(OptBoundType b) { d_b = b; return; }

    // Allows read-only access to the internal callable entity.  This is
    //   currently used by operator==
    const DataValue &FunctionData(void) const { return d_f.Value(); }
    const BoundType BoundData(void)     const { return d_b.Value(); }

    // Different ways to get the interface for a functor rather tan casting.
    Interface *InterfacePtr(void) { return  this; }
    Interface &InterfaceRef(void) { return *this; }

    // Cloning allows copies of the functor to be created.  Perhaps at some
    //   point, an allocator should be given for people to have more control
    //   over how new Functors are created.  Cloning allows for copying to
    //   occur through the interface who doesn't have all the necessary
    //   information to create an actual copy.
    BinderFirst *Clone(void) const { return new BinderFirst(*this); }

  private:
    BinderFirst *DoClone(void) const { return new BinderFirst(*this); }

  public:
    // All the following operator() essentially cover all the possible
    //   arguement list length combinations.  By inheriting iFunctor
    //   which only defines a single version of one of these functions allows
    //   the compiler to report at compile time that the wrong one is being
    //   used.  It doesn't hold with make the interface hard to use
    //   inappropriately, but is really one of the better solutions given
    //   the working of templates.
    ResultType operator()(void) const
    { return (*d_f.p)(d_b()); }

    ResultType operator()(Parm1 p1) const
    { return (*d_f.p)(d_b(), p1); }

    ResultType operator()(Parm1 p1, Parm2 p2) const
    { return (*d_f.p)(d_b(), p1, p2); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) const
    { return (*d_f.p)(d_b(), p1, p2, p3); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4) const
    { return (*d_f.p)(d_b(), p1, p2, p3, p4); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4,
                          Parm5 p5) const
    { return (*d_f.p)(d_b(), p1, p2, p3, p4, p5); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6) const
    { return (*d_f.p)(d_b(), p1, p2, p3, p4, p5, p6); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7) const
    { return (*d_f.p)(d_b(), p1, p2, p3, p4, p5, p6, p7); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8) const
    { return (*d_f.p)(d_b(), p1, p2, p3, p4, p5, p6, p7, p8); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9) const
    { return (*d_f.p)(d_b(), p1, p2, p3, p4, p5, p6, p7, p8, p9); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9,
                          Parm10 p10) const
    { return (*d_f.p)(d_b(), p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11) const
    { return (*d_f.p)(d_b(), p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12) const
    {
      return (*d_f.p)(d_b(), p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,
                      p12);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13) const
    {
      return (*d_f.p)(d_b(), p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                      p13);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13, Parm14 p14) const
    {
      return (*d_f.p)(d_b(), p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                      p13, p14);
    }

  private:
    Data d_f;
    nType::ReassignableType<BoundType> d_b;
  };

  // Comparison operators
  template <typename S, typename T, typename U>
  const bool operator==(const BinderFirst<S,T,U> &lhs,
                        const BinderFirst<S,T,U> &rhs)
  {
    if(lhs.FunctionData() == rhs.FunctionData() &&
       lhs.BoundData() == rhs.BoundData())
    {
      return true;
    }
    return false;
  }

  template <typename S, typename T, typename U>
  const bool operator!=(const BinderFirst<S,T,U> &lhs,
                        const BinderFirst<S,T,U> &rhs)
  {
    if(lhs.FunctionData() == rhs.FunctionData() &&
       lhs.BoundData() == rhs.BoundData())
    {
      return false;
    }
    return true;
  }

  /*
   * BindFirst helper functions
   * Creates a new BinderFirst class with the appropriate data.
   */

  // Was this intended to make a copy of the functor in BinderFirst or
  //   was it just to hold the pointer to the functor?  I am assuming the
  //   later, so it makes more sense to store the interface pointer now.
  template <typename Fun, typename R, typename TList>
  BinderFirst<Fun, R, TList> *BindFirst(
    typename nType::PtrAccessibleType<Fun>::ParameterType fun,
    typename Private::BinderFirstTraits<TList>::OptBoundType bound)
  {
    std::auto_ptr< BinderFirst<Fun, R, TList> >
      p(new BinderFirst<Fun, R, TList>(fun, bound));
    return p.release();
  }

  // Allows for any type of functor who has a iFunctor<R,TList> interface so
  //   that the template arguements on BindFirst are not required because they
  //   can be deduced where they can not be deduced with the above function.
  template <typename R, typename TList>
  BinderFirst<iFunctor<R, TList> *, R, TList> *BindFirst(
    iFunctor<R, TList> *fun,
    typename Private::BinderFirstTraits<TList>::OptBoundType bound)
  {
    if(!fun) { return 0; }
    std::auto_ptr< BinderFirst<iFunctor<R, TList>*, R, TList> >
      p(new BinderFirst<iFunctor<R, TList>*, R, TList>(*fun, bound));
    return p.release();
  }

} // end namespace nFunctor

#endif
