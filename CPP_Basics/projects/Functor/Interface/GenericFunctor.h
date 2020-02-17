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
//   class into its own file, and did some renaming.  This class is the old
//   Functor class

#ifndef __Functor_Inteface_GenericFunctor_h__
#define __Functor_Inteface_GenericFunctor_h__

/*!
 *
 * \class GenericFunctor
 *
 * \ingroup nFunctor
 *  A generalized functor implementation with value semantics
 *
 * GenericFunctor - A Generic Functor can wrap all literal callable entities
 *   into a new callable entity.  All parameter passing is optimized, see
 *   comments in iFunctor.  This functor is consider generic because its
 *   template arguements do not include the type of the callable entity
 *   wrapped up within.  Using the constructor as a template function allows
 *   for template arguement deduction, which is not allowed for class
 *   template arguements.  Thus, the constructors can figure out what
 *   type the callable entity is and create an appropriate data structure
 *   which happens to also be a callable entity.  The data is then stored
 *   and managed within the class.  This is a trade-off with the more explicit
 *   functor because while it is more general that generality forces it to
 *   lose type information about the wrapped callable entity and adds an
 *   extra level of indirection by having to forward the function arguements
 *   twice.  With the loss of type information means that comparing two
 *   GenericFunctors is possible only by comparing the internal interface
 *   pointers.  However, it is not possible to determine if two Functors wrap
 *   the same callable entities without using the RTTI, which I am not prepared
 *   to do at this point.  While you can compare wrapped entities with
 *   Functor, it can not be done by comparing two iFunctors.
 *
 * Due to the lack of type information, operator== can only compare whether
 *   or not two GenericFunctors are the sameobject, not if they are equivalent.
 *
 * Because the GenericFunctor allocates new internal callable entites, it can
 *   take an Allocator policy as a template parameter.  It defaults to a
 *   default allocator that does nothing.  For example, you can use a
 *   SmallObjectAllocation scheme with threading if one wants.
 *
 * Another note on allocation.  The GenericFunctor now assumes that it always
 *   has access to a valid wrapped entity pointer, never null.  However, it
 *   could still succumb to dangling pointers and such.  Because this class
 *   creates its own entity or clones another, to ensure validity, I assume
 *   that new will throw std::bad_alloc.  If it does not then the constructors
 *   and assignment operators need to add a test for null and throw an
 *   exception if it is null.  By ensuring that the class always has a valid
 *   pointer, the validity tests can occur in the construction and assignment
 *   and not every time the class is executed.
 *
 * The executing of operator() from the iFunctor base class involves
 *   a virtual call to operator(), a call to d_spInterface.get(), and a pointer
 *   dereferencing before the actual callable entity is invoked.
 *
 */

#include <memory> // Included for std::auto_ptr

#include "Type/Interface/NullType.h"
#include "Type/Interface/EmptyType.h"
#include "Functor/Interface/iFunctor.h"
#include "Functor/Interface/MemberFunctionWrapper.h"
#include "Functor/Interface/Functor.h"

namespace nFunctor
{

  // This class utilizes mutliple inheritence, but because it inherits from
  //   iFunctor first, explicit casting to the iFunctor should still be
  //   comparable by pointer value between an iFunctor and a GenericFunctor.
  // R is the return type
  // TList is the list of arguement types
  // Allocator is the allocation policy
  // iFunctor is inherited first so that GenericFunctor * == iFunctor *
  template <
            typename R=void,
            class TList=nType::NullType,
            class Allocator=nType::EmptyType
           >
  class GenericFunctor : public iFunctor<R, TList>, public Allocator
  {
  public:
    // Useful type information about the GenericFunctor.
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

    /*** BEGIN Constructors **********************************************/
    // No default constructor is allowed.  No destructor is necessary because
    //   an auto_ptr is used to store the data.  The copy constructor and
    //   assignment operations clone their copies.  Assumes that new and
    //   Clone will throw std::bad_alloc upon error within new.
    GenericFunctor(const GenericFunctor &f)
    : d_spInterface(f.d_spInterface.get()->Clone())
    { return; }

    // Fun can be an object, reference, or pointer to a callable object
    // Functor may also throw BadCallableEntity for null pointers
    template <typename Fun>
    GenericFunctor(Fun fun)
    : d_spInterface(new Functor<Fun, R, TList>(fun))
    { return; }

    // Obj can be an object, reference, or pointer to a callable object
    // MemberFunctionWrapper may also throw BadCallableEntity for null objects
    template <typename Obj, typename MemFn>
    GenericFunctor(Obj p, MemFn memFn)
    : d_spInterface(new MemberFunctionWrapper<Obj, MemFn, R, TList>(p, memFn))
    { return; }
    /*** END Constructors ************************************************/

    /*** BEGIN Assignment/Set ********************************************/
    // The assignment operators all clone the right hand side's data, and
    //   I assume that Clone will throw a std::bad_alloc if it can not
    //   successful create a new Functor.  If exceptions are not thrown then
    //   each assignment operator will need to check for null status of the
    //   new copy and handle it appropriately, most likely throwing an
    //   exception itself.

    // The assignment operator used to swap the internal callable entites
    //   between the right hand side and the left hand side.  However, this
    //   seems like strange functionality and would not have been what I would
    //   have expected.  Thus, I changed it to be more like I would expect
    //   ie. to actually copy the callable entity for myself.  A swap
    //   function could be written otherwise.
    GenericFunctor& operator=(const GenericFunctor& rhs) {
      if(&rhs != this) {
        Interface *i = d_spInterface.release();
        if(i) { delete i; }
        // Cloning is possible, because all GenericFunctors
        //   create only Functor or MemberFunctionWrapper, both
        //   of which have Clone.  If the rhs is null, ie a bad
        //   functor, Clone will throw std::bad_alloc.
        d_spInterface.reset(rhs.d_spInterface.get()->Clone());
      }
      return *this;
    }

    const Interface &operator=(const Interface &rhs) {
      Interface *i = d_spInterface.release();
      if(i) { delete i; }
      d_spInterface.reset(rhs.Clone());
      return rhs;
    }

    // Allow the changing of the internal callable entity
    // Functor may throw BadCallableEntity if a null pointer is used
    template <typename Fun>
    void SetFun(Fun rhs) {
      Interface *i = d_spInterface.release();
      if(i) { delete i; }
      d_spInterface.reset(new Functor<Fun, R, TList>(rhs));
      return;
    }

    // Allow the changing of the internal callable entity
    // The MemberFunctionWrapper may throw a BadCallableEntity if a null object
    //   is used.
    template <class Obj, typename MemFn>
    void SetMemFun(Obj p, MemFn memFn)
    {
      Interface *i = d_spInterface.release();
      if(i) { delete i; }
      d_spInterface.reset(new MemberFunctionWrapper<Obj, MemFn,
                                                    R, TList>(p, memFn));
      return;
    }
    /*** END Assignment/Set **********************************************/

    // Retrieves the pointer to internal data.  This is used by operator==.
    const Interface *GetData(void) const { return d_spInterface.get(); }

    // Different ways to get the interface for a functor rather
    //   than casting.
    Interface *InterfacePtr(void) { return  this; }
    Interface &InterfaceRef(void) { return *this; }

    // Cloning allows copies of the functor to be created.  Perhaps at some
    //   point, an allocator should be given for people to have more control
    //   over how new GenericFunctors are created.  Cloning allows for copying
    //   to occur through the interface who doesn't have all the necessary
    //   information to create an actual copy.
    GenericFunctor *Clone(void) const { return new GenericFunctor(*this); }

  private:
    Interface *DoClone(void) const { return new GenericFunctor(*this); }

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
    { return (*d_spInterface.get())(); }

    ResultType operator()(Parm1 p1) const
    { return (*d_spInterface.get())(p1); }

    ResultType operator()(Parm1 p1, Parm2 p2) const
    { return (*d_spInterface.get())(p1, p2); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) const
    { return (*d_spInterface.get())(p1, p2, p3); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4) const
    { return (*d_spInterface.get())(p1, p2, p3, p4); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4,
                          Parm5 p5) const
    { return (*d_spInterface.get())(p1, p2, p3, p4, p5); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6) const
    { return (*d_spInterface.get())(p1, p2, p3, p4, p5, p6); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7) const
    { return (*d_spInterface.get())(p1, p2, p3, p4, p5, p6, p7); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8) const
    { return (*d_spInterface.get())(p1, p2, p3, p4, p5, p6, p7, p8); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9) const
    { return (*d_spInterface.get())(p1, p2, p3, p4, p5, p6, p7, p8, p9); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9,
                          Parm10 p10) const
    { return (*d_spInterface.get())(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11) const
    {
      return (*d_spInterface.get())(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10,
                                    p11);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12) const
    {
      return (*d_spInterface.get())(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10,
                                    p11, p12);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13) const
    {
      return (*d_spInterface.get())(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10,
                                    p11, p12, p13);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13, Parm14 p14) const
    {
      return (*d_spInterface.get())(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10,
                                    p11, p12, p13, p14);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5,
                          Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9, Parm10 p10,
                          Parm11 p11, Parm12 p12, Parm13 p13, Parm14 p14,
                          Parm15 p15) const
    {
      return (*d_spInterface.get())(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10,
                                    p11, p12, p13, p14, p15);
    }

  private:
    std::auto_ptr<Interface> d_spInterface;
  };

  // Comparison operators, currently it will only determine if two class are
  //   the same object.
  template <typename T, typename U>
  const bool operator==(const GenericFunctor<T,U> &lhs,
                        const GenericFunctor<T,U> &rhs)
  {
    if(lhs.GetData() == rhs.GetData()) { return true; }
    return false;
  }

  template <typename T, typename U>
  const bool operator!=(const GenericFunctor<T,U> &lhs,
                        const GenericFunctor<T,U> &rhs)
  {
    if(lhs.GetData() != rhs.GetData()) { return true; }
    return false;
  }

} // end namespace nFunctor

#endif
