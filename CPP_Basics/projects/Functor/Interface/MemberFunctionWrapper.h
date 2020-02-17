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
//   class into its own file, and did some renaming.  This class is related
//   to the MemFunHandler class.

#ifndef __Functor_Inteface_MemberFunctionWrapper_h__
#define __Functor_Inteface_MemberFunctionWrapper_h__

/*!
 *
 * \class MemberFunctionWrapper
 *
 * \ingroup nFunctor
 *
 * MemberFunctionWrapper- Due to the incompatibility of member function calling
 *   and all other callable entity execution, a special wrapper class is
 *   needed to create a unified method of calling an entity.  This class acts
 *   exactly like a Functor as it inherits from iFunctor, therefore it is
 *   compatible with all structures and algorithms that use a functor interface.
 *   It allows for all types of callable entites and combinations of consts in
 *   conjunction with value, ref, and ptr versions.  For notes on const
 *   operator() please see iFunctor documentation.
 *
 * All parameter passing is optimized, see comments in iFunctor.
 *
 * The executing of operator() from the iFunctor base class involves
 *   a virtual call to operator(), direct access to a structure variable,
 *   a pointer dereferencing, and member access before the actual callable
 *   entity is invoked.
 *
 */

#include "Type/Interface/NullType.h"
#include "Type/Interface/PtrAccessibleType.h"
#include "Functor/Interface/Exceptions.h"
#include "Functor/Interface/iFunctor.h"

namespace nFunctor
{

  /////////////////////////////////////////////////////////////////////////
  // class template MemberFunctionWrapper
  // Wraps pointers to member functions
  /////////////////////////////////////////////////////////////////////////
  // Obj is the callable entity type, which can be a value, ref, or ptr.
  // PointerToMemFn is the method to call from Obj.
  // R is the return type
  // TList is the list of arguement types.
  template <typename Obj, typename PointerToMemFn,
            typename R=void, class TList=nType::NullType>
  class MemberFunctionWrapper : public iFunctor<R, TList>
  {
  public:
    typedef typename iFunctor<R, TList>::Interface Interface;
    typedef typename iFunctor<R, TList>::ResultType ResultType;
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

    // These are the typedefs that allow for value/ref/ptr Obj types
    typedef typename nType::PtrAccessibleType<Obj> Data;
    typedef typename Data::ParameterType OptObj;
    typedef typename Data::StorageType DataValue;

    // Can take a pointer, reference, or non-pointer version of the object.
    //   Using default copy constructor, assignment, and destructor.

    // A pointer is used for the incoming object, a BadCallableEntity exception
    //   will be thrown if it is null.
    MemberFunctionWrapper(OptObj o, PointerToMemFn pMemFn)
    : d_obj(o), d_pMemFun(pMemFn)
    {
      if(!d_obj()) { throw BadCallableEntity(); }
      return;
    }

    // Allow the changing of the internal callable entity, see above comment.
    void SetMemFun(OptObj p, PointerToMemFn memFn) {
      d_obj = p;
      d_pMemFun = memFn;
      if(!d_obj()) { throw BadCallableEntity(); }
      return;
    }

    // Allows read-only access to the internal callable entity.  This is
    //   currently used by operator==.
    const PointerToMemFn &Function(void) const { return d_pMemFun;     }
    const DataValue      &Owner(void)    const { return d_obj.Value(); }

    // Different ways to get the interface for a functor rather
    //   than casting.
    Interface *InterfacePtr(void) { return  this; }
    Interface &InterfaceRef(void) { return *this; }

    // Cloning allows copies of the functor to be created.  Perhaps at some
    //   point, an allocator should be given for people to have more control
    //   over how new Functors are created.  Cloning allows for copying to
    //   occur through the interface who doesn't have all the necessary
    //   information to create an actual copy.
    MemberFunctionWrapper *Clone(void) const {
      return new MemberFunctionWrapper(*this);
    }

  private:
    Interface *DoClone(void) const { return new MemberFunctionWrapper(*this); }

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
    ResultType operator()() const
    { return (d_obj.p->*d_pMemFun)(); }

    ResultType operator()(Parm1 p1) const
    { return (d_obj.p->*d_pMemFun)(p1); }
    
    ResultType operator()(Parm1 p1, Parm2 p2) const
    { return (d_obj.p->*d_pMemFun)(p1, p2); }
    
    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) const
    { return (d_obj.p->*d_pMemFun)(p1, p2, p3); }
    
    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4) const
    { return (d_obj.p->*d_pMemFun)(p1, p2, p3, p4); }
    
    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4,
                          Parm5 p5) const
    { return (d_obj.p->*d_pMemFun)(p1, p2, p3, p4, p5); }
    
    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6) const
    { return (d_obj.p->*d_pMemFun)(p1, p2, p3, p4, p5, p6); }
    
    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7) const
    { return (d_obj.p->*d_pMemFun)(p1, p2, p3, p4, p5, p6, p7); }
    
    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8) const
    { return (d_obj.p->*d_pMemFun)(p1, p2, p3, p4, p5, p6, p7, p8); }
    
    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9) const
    { return (d_obj.p->*d_pMemFun)(p1, p2, p3, p4, p5, p6, p7, p8, p9); }
    
    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9,
                          Parm10 p10) const
    { return (d_obj.p->*d_pMemFun)(p1, p2, p3, p4, p5, p6, p7, p8, p9,
                                   p10); }
    
    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11) const
    {
        return (d_obj.p->*d_pMemFun)(p1, p2, p3, p4, p5, p6, p7, p8, p9,
                                     p10, p11);
    }
    
    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12) const
    {
        return (d_obj.p->*d_pMemFun)(p1, p2, p3, p4, p5, p6, p7, p8, p9,
                                     p10, p11, p12);
    }
    
    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13) const
    {
        return (d_obj.p->*d_pMemFun)(p1, p2, p3, p4, p5, p6, p7, p8, p9,
                                     p10, p11, p12, p13);
    }
    
    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13, Parm14 p14) const
    {
        return (d_obj.p->*d_pMemFun)(p1, p2, p3, p4, p5, p6, p7, p8, p9,
                                     p10, p11, p12, p13, p14);
    }
    
    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13, Parm14 p14,
                          Parm15 p15) const
    {
        return (d_obj.p->*d_pMemFun)(p1, p2, p3, p4, p5, p6, p7, p8, p9,
                                     p10, p11, p12, p13, p14, p15);
    }
    
  private:
    // Holds the minimum amount of data to call the given entity.
    Data d_obj;
    PointerToMemFn d_pMemFun;
  };

  // Comparison operators, determines if two wrappers are equivalent, ie.
  //   they point to the same data or have copies of the same data.
  template <typename T, typename U, typename V, typename W>
  const bool operator==(const MemberFunctionWrapper<T,U,V,W> &lhs,
                        const MemberFunctionWrapper<T,U,V,W> &rhs)
  {
    if(lhs.Function() == rhs.Function() &&
       lhs.Owner() == rhs.Owner())
    {
      return true;
    }
    return false;
  }

  template <typename T, typename U, typename V, typename W>
  const bool operator!=(const MemberFunctionWrapper<T,U,V,W> &lhs,
                        const MemberFunctionWrapper<T,U,V,W> &rhs)
  {
    if(lhs.Function() == rhs.Function() &&
       lhs.Owner() == rhs.Owner())
    {
      return false;
    }
    return true;
  }

} // end namespace nFunctor

#endif
