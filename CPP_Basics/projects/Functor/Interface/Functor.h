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
//   class into its own file, and did some renaming.  This class is a more
//   explicit version of the GenericFunctor.

// $Header: /home/jason/cvs/Functor/Interface/Functor.h,v 1.3 2006/07/20 01:23:20 jason Exp $

#ifndef __Functor_Inteface_Functor_h__
#define __Functor_Inteface_Functor_h__

// $Header: /home/jason/cvs/Functor/Interface/Functor.h,v 1.3 2006/07/20 01:23:20 jason Exp $

/*!
 * \class Functor
 *
 * \ingroup nFunctor
 *
 * Functor - A Functor can wrap all literal callable entities into a new
 *   callable entity.  This class was created as a more explicit version of
 *   the GenericFunctor.  It allows for all types of callable entites and
 *   combinations of consts in conjunction with value, ref, and ptr versions.
 *   For notes on const operator() please see iFunctor documentation.
 *
 * All parameter passing is optimized, see comments in iFunctor.
 *
 * The executing of operator() from the iFunctor base class involves
 *   a virtual call to operator(), direct access to a structure variable,
 *   and a pointer dereferencing before the actual callable entity is invoked.
 *
 */

#include "Type/Interface/NullType.h"
#include "Type/Interface/PtrAccessibleType.h"
#include "Functor/Interface/Exceptions.h"
#include "Functor/Interface/iFunctor.h"

namespace nFunctor
{

  // Fun is the callable entity type, which can be a value, ref, or ptr.
  // R is the return type
  // TList is the list of arguement types.
  template <typename Fun, typename R=void, class TList=nType::NullType>
  class Functor : public iFunctor<R, TList>
  {
  public:
    // Useful type information about the Functor.
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

    // These are the typedefs that allow for value/ref/ptr Fun types
    typedef typename nType::PtrAccessibleType<Fun> Data;
    typedef typename Data::ParameterType OptFun;
    typedef typename Data::StorageType DataValue;

    // Fun can be an object, reference, or pointer to a callable object.  This
    //   uses default copy constructor, assignment, and destructor.

    // A pointer is used for the incoming object, a BadCallableEntity exception
    //   will be thrown if it is null.
    Functor(OptFun fun) : d_f(fun) {
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

    // Different ways to get the interface for a functor rather than casting.
    Interface *InterfacePtr(void) { return  this; }
    Interface &InterfaceRef(void) { return *this; }

    // Cloning allows copies of the functor to be created.  Perhaps at some
    //   point, an allocator should be given for people to have more control
    //   over how new Functors are created.  Cloning allows for copying to
    //   occur through the interface who doesn't have all the necessary
    //   information to create an actual copy.
    Functor *Clone(void) const { return new Functor(*this); }

  private:
    Interface *DoClone(void) const { return new Functor(*this); }

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
    { return (*d_f.p)(); }

    ResultType operator()(Parm1 p1) const
    { return (*d_f.p)(p1); }

    ResultType operator()(Parm1 p1, Parm2 p2) const
    { return (*d_f.p)(p1, p2); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) const
    { return (*d_f.p)(p1, p2, p3); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4) const
    { return (*d_f.p)(p1, p2, p3, p4); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4,
                          Parm5 p5) const
    { return (*d_f.p)(p1, p2, p3, p4, p5); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6) const
    { return (*d_f.p)(p1, p2, p3, p4, p5, p6); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7) const
    { return (*d_f.p)(p1, p2, p3, p4, p5, p6, p7); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8) const
    { return (*d_f.p)(p1, p2, p3, p4, p5, p6, p7, p8); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9) const
    { return (*d_f.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9,
                          Parm10 p10) const
    { return (*d_f.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11) const
    { return (*d_f.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12) const
    { return (*d_f.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13) const
    {
      return (*d_f.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                      p13);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13, Parm14 p14) const
    {
      return (*d_f.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                      p13, p14);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13, Parm14 p14,
                          Parm15 p15) const
    {
      return (*d_f.p)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12,
                      p13, p14, p15);
    }

  private:
    // Holds the minimum amount of data to call the given entity.
    Data d_f;
  };

  // Comparison operators, determines if two functors are equivalent, ie.
  //   they point to the same data or have copies of the same data.
  template <typename T, typename U, typename V>
  const bool operator==(const Functor<T,U,V> &lhs,
                        const Functor<T,U,V> &rhs)
  {
    if(lhs.GetData() == rhs.GetData()) { return true; }
    return false;
  }

  template <typename T, typename U, typename V>
  const bool operator!=(const Functor<T,U,V> &lhs,
                        const Functor<T,U,V> &rhs)
  {
    if(lhs.GetData() != rhs.GetData()) { return true; }
    return false;
  }

} // namespace nFunctor

////////////////////////////////////////////////////////////////////////////////
// Change log:
// June 20, 2001: ported by Nick Thurn to gcc 2.95.3. Kudos, Nick!!!
// January 1, 2006: pass Functor arguments in BindFirst by value, Peter Kümmel
////////////////////////////////////////////////////////////////////////////////

#endif  // __Functor_h__

// $Log: Functor.h,v $
// Revision 1.3  2006/07/20 01:23:20  jason
// Added a new functor: ReturnlessMemFunWrap.  Also, made changes to iStorageType.h.  Also, changed the passing of pointers to functors from enforcing a reference to throwing an exception.
//
// Revision 1.2  2006/07/19 16:59:40  jason
// Changed from requiring pointers to be dereferenced to using exceptions.  The dereferencing was causing too many issues for template functions due to the discontinuity between the template and parameter types.
//
// Revision 1.1  2006/07/18 19:16:47  jason
// Added first version to cvs.  This is actually round two of edits and close to the final form.
//
// Revision 1.22  2006/06/09 12:58:44  syntheticpp
// simplify Functor::operator== implementation. Thanks to Eric Beyeler
//
// Revision 1.21  2006/06/01 12:33:05  syntheticpp
// add operator== to Functor, initiated by Eric Beyeler
//
// Revision 1.20  2006/05/20 10:23:07  syntheticpp
// add warnings in the documentation about the special lifetime when using SmallObjects
//
// Revision 1.19  2006/03/08 18:22:42  syntheticpp
// doxygen fixes
//
// Revision 1.18  2006/02/27 18:53:41  syntheticpp
// make it possible for Functor not to be a small object because it introduces sometimes problems, add documentation
//
// Revision 1.17  2006/02/20 16:08:32  syntheticpp
// gcc needs the template parameter, gcc warns about missing virtual destructor (because there is already a virtual function)
//
// Revision 1.16  2006/02/18 19:37:40  rich_sposato
// Added explicit call to base copy-constructor.  Changed base class from
// SmallObject to SmallValueObject.
//
// Revision 1.15  2006/01/22 13:31:45  syntheticpp
// add additional template parameter for the changed threading classes
//
// Revision 1.14  2006/01/16 19:05:09  rich_sposato
// Added cvs keywords.
//
